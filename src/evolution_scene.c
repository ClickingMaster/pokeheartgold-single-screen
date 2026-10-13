#include "evolution_scene.h"

#include "global.h"

#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0197.h"

#include "assert.h"
#include "bag.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "palette_blink.h"
#include "party.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "swap_3d_buffers.h"
#include "palette_vram_addr.h"
#include "unk_02088288.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

// The evolution scene: the old and the new Pokemon's pictures flash and swap scale while particles play, then the
// Pokemon's data is updated and any moves it learns at its new level are taught (with the move summary screen when
// it already knows four moves). Started from the field, after battles and after trades.

// particle_scene.c has no header
typedef struct ParticleScene ParticleScene;

typedef struct EvolutionParticleSceneParam {
    enum HeapID heapId;
    int resource;
} EvolutionParticleSceneParam;

void ParticleScene_HidePlanes(void);
void ParticleScene_SetBlend(void);
GF3DVramMan *ParticleScene_CreateVramMan(enum HeapID heapID);
void ParticleScene_ReserveVram(void);
void ParticleScene_DrawAndSwap(void);
ParticleScene *ParticleScene_New(EvolutionParticleSceneParam *param);
void ParticleScene_CreateEmitter(ParticleScene *scene, int resource);
BOOL ParticleScene_IsActive(ParticleScene *scene);
void ParticleScene_Free(ParticleScene *scene);

// render_window.c, declared only in the module's internal header
u32 GetUserFrameGfx2PaletteMember(u32 frame);
void LoadUserFrameGfx2WithFill(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 color, u8 frame, enum HeapID heapId);

// sound_playback.c, not in its header
BOOL PlayBGMDirect(u16 seqNo);

// The scene calls Pokepic_StartAnim() with a second argument, which the function ignores.
#define EVO_START_POKEPIC_ANIM(pokepic) ((void (*)(Pokepic *, int))Pokepic_StartAnim)(pokepic, 0)

// the scene's state machine (EvolutionScene_Run)
enum EvolutionSceneState {
    EVO_STATE_WAIT_FADE_IN,
    EVO_STATE_START,
    EVO_STATE_PRINT_WHAT,
    EVO_STATE_WAIT_WHAT,
    EVO_STATE_PLAY_CRY,
    EVO_STATE_WAIT_CRY,
    EVO_STATE_START_PARTICLES,
    EVO_STATE_NARROW_WINDOW,
    EVO_STATE_OSCILLATE,
    EVO_STATE_WIDEN_WINDOW,
    EVO_STATE_NEW_MON_CRY,
    EVO_STATE_EVOLVE,
    EVO_STATE_REGISTER,
    EVO_STATE_TRY_LEARN_MOVE,
    EVO_STATE_WANTS_TO_LEARN,
    EVO_STATE_WAIT_WANTS_TO_LEARN,
    EVO_STATE_KNOWS_FOUR_MOVES,
    EVO_STATE_WAIT_KNOWS_FOUR_MOVES,
    EVO_STATE_ASK_DELETE_MOVE,
    EVO_STATE_WAIT_ASK_DELETE_MOVE,
    EVO_STATE_SHOW_DELETE_MOVE_MENU,
    EVO_STATE_DELETE_MOVE_MENU,
    EVO_STATE_OPEN_SUMMARY,
    EVO_STATE_RUN_SUMMARY,
    EVO_STATE_SUMMARY_CLOSED,
    EVO_STATE_PRINT_POOF,
    EVO_STATE_WAIT_POOF,
    EVO_STATE_PRINT_FORGOT,
    EVO_STATE_WAIT_FORGOT,
    EVO_STATE_PRINT_AND,
    EVO_STATE_WAIT_AND,
    EVO_STATE_LEARN_MOVE,
    EVO_STATE_ASK_STOP_LEARNING,
    EVO_STATE_WAIT_ASK_STOP_LEARNING,
    EVO_STATE_SHOW_STOP_LEARNING_MENU,
    EVO_STATE_STOP_LEARNING_MENU,
    EVO_STATE_WAIT_DID_NOT_LEARN,
    EVO_STATE_WAIT_LEARNED,
    EVO_STATE_WAIT_FANFARE,
    EVO_STATE_FADE_OUT,
    EVO_STATE_FINISH,
    EVO_STATE_CANCEL,
    EVO_STATE_CANCEL_CRY,
    EVO_STATE_CANCEL_WAIT_CRY,
    EVO_STATE_CANCEL_WAIT_MESSAGE,
    EVO_STATE_CANCEL_FINISH,
};

// two-option menu (EvolutionScene_RunMenu)
enum EvolutionMenuResult {
    EVO_MENU_NONE,
    EVO_MENU_TOP,
    EVO_MENU_BOTTOM,
};

// EvolutionTaskData.flags (the caller's last but one argument)
#define EVO_FLAG_CAN_CANCEL (1 << 0) // B stops the evolution
#define EVO_FLAG_PRINT_WHAT (1 << 1) // "What?" before "<mon> is evolving!"

struct EvolutionTaskData {
    BgConfig *bgConfig;                     // 0x00
    Window *window;                         // 0x04, message box
    MsgData *msgData;                       // 0x08
    MessageFormat *msgFormat;               // 0x0C
    String *string;                         // 0x10
    PaletteData *paletteData;               // 0x14
    PokepicManager *pokepicManager;         // 0x18
    Pokepic *oldPokepic;                    // 0x1C, current species
    Pokepic *newPokepic;                    // 0x20, species it evolves into
    Party *party;                           // 0x24
    Pokemon *mon;                           // 0x28
    Options *options;                       // 0x2C
    ParticleScene *particleScene;           // 0x30
    GF3DVramMan *vramMan;                   // 0x34
    OverlayManager *summaryApp;             // 0x38
    PokemonSummaryArgs *summaryArgs;        // 0x3C
    u8 unk_40[4];                           // 0x40
    PokepicAnimManager *pokepicAnimManager; // 0x44
    Pokedex *pokedex;                       // 0x48
    Bag *bag;                               // 0x4C
    GameStats *gameStats;                   // 0x50
    u8 unk_54[4];                           // 0x54
    PaletteBlink *paletteBlink;             // 0x58
    enum HeapID heapId;                     // 0x5C
    u16 species;                            // 0x60
    u16 newSpecies;                         // 0x62
    u8 state;                               // 0x64, enum EvolutionSceneState
    u8 printerId;                           // 0x65
    u8 timer;                               // 0x66
    u8 done;                                // 0x67
    int learnsetCursor;                     // 0x68, MonTryLearnMoveOnLevelUp
    u16 newMove;                            // 0x6C
    u8 moveSlot;                            // 0x6E, the move chosen on the summary screen
    u8 unk_6F;                              // 0x6F
    u8 scaleOscillation;                    // 0x70, 0 = off; bit 0 = which picture grows
    u8 scaleStep;                           // 0x71
    u8 wnd0Left;                            // 0x72
    u8 wnd0Top;                             // 0x73
    u8 wnd0Right;                           // 0x74
    u8 wnd0Bottom;                          // 0x75
    u8 unk_76[2];                           // 0x76
    int evolutionCondition;                 // 0x78, EVO_*
    u32 flags;                              // 0x7C, EVO_FLAG_*
    u8 form;                                // 0x80
    u8 unk_81[3];                           // 0x81
    NARC *pokegraNarc;                      // 0x84
    u8 unk_88;                              // 0x88, NARC_ReadPokepicCryDelay of species
    u8 unk_89;                              // 0x89, NARC_ReadPokepicCryDelay of newSpecies
    u8 menuState;                           // 0x8A
    u8 menuResult;                          // 0x8B, enum EvolutionMenuResult
    Window menuWindows[2];                  // 0x8C
    SpriteSystem *spriteSystem;             // 0xAC
    SpriteManager *spriteManager;           // 0xB0
    ManagedSprite *cursorSprite;            // 0xB4
    BOOL spritesReady;                      // 0xB8
}; // size: 0xBC

typedef struct EvolutionBgTemplates {
    BgTemplate bg[3];
} EvolutionBgTemplates;

// asm/evolution_scene_data.s
extern const u8 gEvolutionSummaryPages[];
extern const s16 gEvolutionMenuCursorX[];
extern const s16 gEvolutionMenuCursorY[];
extern const TouchscreenHitbox gEvolutionMenuHitboxes[];
extern const GraphicsModes gEvolutionGraphicsModes;
extern const OamCharTransferParam gEvolutionOamCharTransferParam;
extern const SpriteResourceCountsListUnion gEvolutionSpriteResourceCounts;
extern const OamManagerParam gEvolutionOamManagerParam;
extern const GraphicsBanks gEvolutionGraphicsBanks;
extern const ManagedSpriteTemplate gEvolutionCursorSpriteTemplate;
extern const EvolutionBgTemplates gEvolutionMainBgTemplates;
extern const EvolutionBgTemplates gEvolutionSubBgTemplates;

ManagedSprite *EvolutionScene_CreateSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u8 animation);
void EvolutionScene_InitSprites(EvolutionTaskData *data);
void EvolutionScene_FreeSprites(EvolutionTaskData *data);
void EvolutionScene_UpdateMenuCursor(EvolutionTaskData *data);
void EvolutionScene_PrintMenuOption(EvolutionTaskData *data, Window *window, MsgData *msgData, int msgNo);
void EvolutionScene_ShowMenu(EvolutionTaskData *data, BOOL deleteMovePrompt);
BOOL EvolutionScene_HandleMenuInput(EvolutionTaskData *data);
BOOL EvolutionScene_IsMenuCursorAnimDone(EvolutionTaskData *data);
int EvolutionScene_RunMenu(EvolutionTaskData *data);
void EvolutionScene_Task(SysTask *task, void *taskData);
void EvolutionScene_Run(EvolutionTaskData *data);
void EvolutionScene_ApplyEvolutionEffects(EvolutionTaskData *data);
void EvolutionScene_InitGraphics(EvolutionTaskData *data, BgConfig *bgConfig);
void EvolutionScene_FreeBgs(BgConfig *bgConfig);
void EvolutionScene_CreatePokepics(EvolutionTaskData *data);
void EvolutionScene_VBlankCB(void *cbData);
u8 EvolutionScene_PrintMessage(EvolutionTaskData *data, int msgNo);
BOOL EvolutionScene_TextCallback(TextPrinterTemplate *template, u16 glyphId);
void EvolutionScene_OpenSummary(EvolutionTaskData *data);

ManagedSprite *EvolutionScene_CreateSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u8 animation) {
    ManagedSpriteTemplate template = gEvolutionCursorSpriteTemplate;
    ManagedSprite *sprite;

    template.x = x;
    template.y = y;
    template.animation = animation;
    sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

void EvolutionScene_InitSprites(EvolutionTaskData *data) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    data->spriteSystem = SpriteSystem_Alloc(data->heapId);
    data->spriteManager = SpriteManager_New(data->spriteSystem);
    oamManagerParam = gEvolutionOamManagerParam;
    oamCharTransferParam = gEvolutionOamCharTransferParam;
    oamCharTransferParam.maxTasks = 16;
    SpriteSystem_Init(data->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    SpriteSystem_InitSprites(data->spriteSystem, data->spriteManager, 16);
    resourceCounts = gEvolutionSpriteResourceCounts;
    SpriteSystem_InitManagerWithCapacities(data->spriteSystem, data->spriteManager, &resourceCounts);

    spriteSystem = data->spriteSystem;
    spriteManager = data->spriteManager;
    narc = NARC_New(NARC_a_2_3_7, data->heapId);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, 11, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 12, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 13, FALSE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 14, FALSE, 0);
    NARC_Delete(narc);

    data->cursorSprite = EvolutionScene_CreateSprite(data->spriteSystem, data->spriteManager, 128, 72, 1);
    ManagedSprite_SetDrawFlag(data->cursorSprite, FALSE);
    data->spritesReady = TRUE;
}

void EvolutionScene_FreeSprites(EvolutionTaskData *data) {
    SpriteSystem *spriteSystem = data->spriteSystem;
    SpriteManager *spriteManager = data->spriteManager;

    GF_ASSERT(data->cursorSprite != NULL);
    Sprite_DeleteAndFreeResources(data->cursorSprite);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
    data->spritesReady = FALSE;
}

void EvolutionScene_UpdateMenuCursor(EvolutionTaskData *data) {
    int option;

    GF_ASSERT(data->spritesReady);
    GF_ASSERT(data->cursorSprite != NULL);
    switch (data->menuResult) {
    case EVO_MENU_TOP:
        option = 0;
        break;
    case EVO_MENU_BOTTOM:
        option = 1;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    ManagedSprite_SetPositionXY(data->cursorSprite, gEvolutionMenuCursorX[option * 2], gEvolutionMenuCursorY[option * 2]);
}

void EvolutionScene_PrintMenuOption(EvolutionTaskData *data, Window *window, MsgData *msgData, int msgNo) {
    String *string = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msgNo, data->heapId);

    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 4, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(window);
    String_Delete(string);
}

void EvolutionScene_ShowMenu(EvolutionTaskData *data, BOOL deleteMovePrompt) {
    int topMsg = msg_0197_01181;    // Forget a move!
    int bottomMsg = msg_0197_01182; // Keep old moves!

    GF_ASSERT(data->spritesReady);
    if (deleteMovePrompt == FALSE) {
        BufferMoveName(data->msgFormat, 0, data->newMove);
        topMsg = msg_0197_01186;    // Give up on <move>!
        bottomMsg = msg_0197_01187; // Don't give up on <move>!
    }
    EvolutionScene_PrintMenuOption(data, &data->menuWindows[0], data->msgData, topMsg);
    EvolutionScene_PrintMenuOption(data, &data->menuWindows[1], data->msgData, bottomMsg);
    data->menuState = 0;
    data->menuResult = EVO_MENU_TOP;
    EvolutionScene_UpdateMenuCursor(data);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
    ManagedSprite_SetDrawFlag(data->cursorSprite, TRUE);
}

BOOL EvolutionScene_HandleMenuInput(EvolutionTaskData *data) {
    BOOL selected = FALSE;
    int hitbox = TouchscreenHitbox_FindRectAtTouchNew(gEvolutionMenuHitboxes);

    GF_ASSERT(data->spritesReady);
    if (hitbox != TOUCH_MENU_NO_INPUT) {
        PlaySE(SEQ_SE_DP_SELECT);
        switch (hitbox) {
        case 0:
            data->menuResult = EVO_MENU_TOP;
            selected = TRUE;
            break;
        case 1:
            data->menuResult = EVO_MENU_BOTTOM;
            selected = TRUE;
            break;
        default:
            GF_ASSERT(FALSE);
            break;
        }
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        if (data->menuResult != EVO_MENU_TOP) {
            data->menuResult = EVO_MENU_TOP;
            EvolutionScene_UpdateMenuCursor(data);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (data->menuResult != EVO_MENU_BOTTOM) {
            data->menuResult = EVO_MENU_BOTTOM;
            EvolutionScene_UpdateMenuCursor(data);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (data->menuResult == EVO_MENU_NONE) {
            GF_ASSERT(FALSE);
            data->menuResult = EVO_MENU_BOTTOM;
        }
        selected = TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        data->menuResult = EVO_MENU_BOTTOM;
        selected = TRUE;
    }
    if (selected) {
        EvolutionScene_UpdateMenuCursor(data);
        ManagedSprite_SetAnim(data->cursorSprite, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return selected;
}

BOOL EvolutionScene_IsMenuCursorAnimDone(EvolutionTaskData *data) {
    GF_ASSERT(data->spritesReady);
    GF_ASSERT(ManagedSprite_GetActiveAnim(data->cursorSprite) == 3);
    if (!ManagedSprite_IsAnimated(data->cursorSprite)) {
        return TRUE;
    }
    return FALSE;
}

int EvolutionScene_RunMenu(EvolutionTaskData *data) {
    switch (data->menuState) {
    case 0:
        if (EvolutionScene_HandleMenuInput(data)) {
            data->menuState++;
        }
        break;
    case 1:
        if (EvolutionScene_IsMenuCursorAnimDone(data)) {
            GF_ASSERT(data->spritesReady);
            GF_ASSERT(data->menuResult != EVO_MENU_NONE);
            ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
            ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
            ManagedSprite_SetDrawFlag(data->cursorSprite, FALSE);
            return data->menuResult;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return EVO_MENU_NONE;
}

#pragma force_active on

EvolutionTaskData *EvolutionScene_New(Party *party, Pokemon *mon, int species, Options *options, u32 a4, Pokedex *dex, Bag *bag, GameStats *stats, u32 evolutionCondition, u32 a9, enum HeapID heapID) {
    EvolutionTaskData *data;

    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    FontID_Alloc(4, heapID);
    data = Heap_Alloc(heapID, sizeof(EvolutionTaskData));
    MI_CpuClearFast(data, sizeof(EvolutionTaskData));
    MI_CpuClear16(GetMainBgPlttAddr(), GetMainBgPlttSize());
    MI_CpuClear16(GetSubBgPlttAddr(), GetSubBgPlttSize());
    data->party = party;
    data->mon = mon;
    data->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    data->form = GetMonData(mon, MON_DATA_FORM, NULL);
    data->newSpecies = species;
    data->heapId = heapID;
    data->summaryApp = NULL;
    data->pokegraNarc = NARC_New(NARC_a_1_8_0, heapID);
    NARC_ReadPokepicCryDelay(data->pokegraNarc, &data->unk_88, data->species, 1);
    NARC_ReadPokepicCryDelay(data->pokegraNarc, &data->unk_89, data->newSpecies, 1);

    data->paletteData = PaletteData_Init(heapID);
    PaletteData_SetAutoTransparent(data->paletteData, TRUE);
    PaletteData_AllocBuffers(data->paletteData, PLTTBUF_MAIN_BG, 0x200, heapID);
    PaletteData_AllocBuffers(data->paletteData, PLTTBUF_SUB_BG, 0x200, heapID);
    PaletteData_AllocBuffers(data->paletteData, PLTTBUF_MAIN_OBJ, 0x1C0, heapID);
    data->bgConfig = BgConfig_Alloc(heapID);
    data->window = AllocWindows(heapID, 1);
    data->options = options;
    data->vramMan = ParticleScene_CreateVramMan(heapID);
    ParticleScene_HidePlanes();
    ParticleScene_SetBlend();
    ParticleScene_ReserveVram();
    EvolutionScene_InitGraphics(data, data->bgConfig);
    EvolutionScene_InitSprites(data);

    AddWindowParameterized(data->bgConfig, data->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 11, 31);
    FillWindowPixelBuffer(data->window, 0xFF);
    DrawFrameAndWindow2(data->window, FALSE, 1, 10);
    AddWindowParameterized(data->bgConfig, &data->menuWindows[0], GF_BG_LYR_SUB_2, 3, 8, 26, 2, 15, 80);
    AddWindowParameterized(data->bgConfig, &data->menuWindows[1], GF_BG_LYR_SUB_2, 3, 14, 26, 2, 15, 132);

    data->pokepicManager = PokepicManager_Create(heapID);
    data->pokepicAnimManager = PokepicAnimManager_New(heapID, 1, 0);
    data->done = FALSE;
    data->timer = 2;
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0197_bin, heapID);
    data->msgFormat = MessageFormat_New(heapID);
    data->string = String_New(320, heapID);
    data->summaryArgs = Heap_Alloc(heapID, sizeof(PokemonSummaryArgs));
    MI_CpuClearFast(data->summaryArgs, sizeof(PokemonSummaryArgs));
    data->summaryArgs->unk2C = a4;
    data->pokedex = dex;
    data->bag = bag;
    data->gameStats = stats;
    data->evolutionCondition = evolutionCondition;
    data->flags = a9;
    EvolutionScene_CreatePokepics(data);

    PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xFFFF, 1, 16, 0, RGB_BLACK);
    Pokepic_StartPaletteFadeAll(data->pokepicManager, 16, 0, 0, RGB_BLACK);
    data->paletteBlink = PaletteBlink_New(data->paletteData, PM_LCD_TOP, 11, heapID);
    PaletteBlink_SetState(data->paletteBlink, 1);
    SysTask_CreateOnMainQueue(EvolutionScene_Task, data, 0);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    WirelessStatusIcon_Create();
    return data;
}

#pragma force_active reset

void EvolutionScene_Task(SysTask *task, void *taskData) {
    EvolutionTaskData *data = taskData;

    EvolutionScene_Run(data);
    if (data->summaryApp == NULL) {
        PokepicManager_DrawAll(data->pokepicManager);
        ParticleScene_DrawAndSwap();
        RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    }
    if (data->done) {
        SysTask_Destroy(task);
    }
}

BOOL EvolutionScene_IsDone(EvolutionTaskData *data) {
    if (data->done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void EvolutionScene_Delete(EvolutionTaskData *data) {
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(4);
    EvolutionScene_FreeSprites(data);
    WindowArray_Delete(data->window, 1);
    RemoveWindow(&data->menuWindows[0]);
    RemoveWindow(&data->menuWindows[1]);
    PaletteData_FreeBuffers(data->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(data->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(data->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_Free(data->paletteData);
    PokepicManager_Delete(data->pokepicManager);
    PokepicAnimManager_Delete(data->pokepicAnimManager);
    GF_3DVramMan_Delete(data->vramMan);
    EvolutionScene_FreeBgs(data->bgConfig);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
    Heap_Free(data->string);
    Heap_Free(data->summaryArgs);
    PaletteBlink_Delete(data->paletteBlink);
    Heap_Free(data->bgConfig);
    NARC_Delete(data->pokegraNarc);
    Heap_Free(data);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
}

#pragma force_active on

void EvolutionScene_Run(EvolutionTaskData *data) {
    if (data->scaleOscillation) {
        if ((data->scaleOscillation & 1) == 0) {
            Pokepic_AddAttr(data->oldPokepic, POKEPIC_AFFINEW, -data->scaleStep);
            Pokepic_AddAttr(data->oldPokepic, POKEPIC_AFFINEH, -data->scaleStep);
            Pokepic_AddAttr(data->newPokepic, POKEPIC_AFFINEW, data->scaleStep);
            Pokepic_AddAttr(data->newPokepic, POKEPIC_AFFINEH, data->scaleStep);
            if (Pokepic_GetAttr(data->oldPokepic, POKEPIC_AFFINEW) == 0) {
                data->scaleOscillation ^= 1;
            }
        } else {
            Pokepic_AddAttr(data->oldPokepic, POKEPIC_AFFINEW, data->scaleStep);
            Pokepic_AddAttr(data->oldPokepic, POKEPIC_AFFINEH, data->scaleStep);
            Pokepic_AddAttr(data->newPokepic, POKEPIC_AFFINEW, -data->scaleStep);
            Pokepic_AddAttr(data->newPokepic, POKEPIC_AFFINEH, -data->scaleStep);
            if (Pokepic_GetAttr(data->newPokepic, POKEPIC_AFFINEW) == 0) {
                data->scaleOscillation ^= 1;
                if (data->scaleStep < 64) {
                    data->scaleStep *= 2;
                }
            }
        }
    }
    if ((data->flags & EVO_FLAG_CAN_CANCEL) && data->state == EVO_STATE_OSCILLATE && (gSystem.newKeys & PAD_BUTTON_B)) {
        PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xF3FF, 0, 0, 16, RGB_WHITE);
        data->state = EVO_STATE_CANCEL;
    }

    switch (data->state) {
    case EVO_STATE_WAIT_FADE_IN:
        if (--data->timer == 0) {
            data->state++;
        }
        break;
    case EVO_STATE_START:
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        PaletteBlink_SetState(data->paletteBlink, 0);
        if (data->flags & EVO_FLAG_PRINT_WHAT) {
            data->state = EVO_STATE_PRINT_WHAT;
        } else {
            data->state = EVO_STATE_PLAY_CRY;
        }
        break;
    case EVO_STATE_PRINT_WHAT:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00916); // What?
            data->state++;
        }
        break;
    case EVO_STATE_WAIT_WHAT:
        if (TextPrinterCheckActive(data->printerId)) {
            break;
        }
        data->state = EVO_STATE_PLAY_CRY;
        break;
    default:
        break;
    case EVO_STATE_PLAY_CRY:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            PokepicAnimScript animScript[10];

            NARC_StartPokepicAnim(data->pokegraNarc, data->pokepicAnimManager, data->oldPokepic, data->species, 2, 0, 0);
            NARC_ReadPokepicAnimScript(data->pokegraNarc, animScript, data->species, 1);
            Pokepic_SetAnimScript(data->oldPokepic, animScript);
            EVO_START_POKEPIC_ANIM(data->oldPokepic);
            PlayCry(data->species, data->form);
            BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
            if (data->flags & EVO_FLAG_PRINT_WHAT) {
                data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00917); // <mon> is evolving!
            } else {
                data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00915); // What? <mon> is evolving!
            }
            data->state = EVO_STATE_WAIT_CRY;
        }
        break;
    case EVO_STATE_WAIT_CRY:
        if (!TextPrinterCheckActive(data->printerId) && !IsCryFinished() && PokepicAnimManager_GetAnimState(data->pokepicAnimManager, 0) == TRUE && !Pokepic_IsAnimFinished(data->oldPokepic)) {
            PaletteBlink_SetState(data->paletteBlink, 1);
            PlayBGMDirect(SEQ_GS_SHINKA);
            data->timer = 20;
            data->state = EVO_STATE_START_PARTICLES;
        }
        break;
    case EVO_STATE_START_PARTICLES:
        if (--data->timer == 0) {
            EvolutionParticleSceneParam param;

            param.heapId = data->heapId;
            param.resource = 0;
            data->particleScene = ParticleScene_New(&param);
            ParticleScene_CreateEmitter(data->particleScene, 0);
            Pokepic_StartPaletteFade(data->oldPokepic, 0, 16, 4, RGB_WHITE);
            Pokepic_StartPaletteFade(data->newPokepic, 0, 16, 4, RGB_WHITE);
            GF_ASSERT(HeapExp_FndGetTotalFreeSize(data->heapId) > 0x8000);
            PlaySE(SEQ_SE_DP_W025);
            data->timer = 40;
            data->state++;
        }
        break;
    case EVO_STATE_NARROW_WINDOW:
        if (data->wnd0Top < 40) {
            data->wnd0Top += 2;
            data->wnd0Bottom -= 2;
        }
        if (--data->timer == 0) {
            ParticleScene_CreateEmitter(data->particleScene, 1);
            ParticleScene_CreateEmitter(data->particleScene, 2);
            ParticleScene_CreateEmitter(data->particleScene, 7);
            ParticleScene_CreateEmitter(data->particleScene, 8);
            ParticleScene_CreateEmitter(data->particleScene, 9);
            ParticleScene_CreateEmitter(data->particleScene, 11);
            PlaySE(SEQ_SE_DP_W060C);
            data->scaleOscillation = 16;
            data->scaleStep = 8;
            data->state++;
        }
        break;
    case EVO_STATE_OSCILLATE:
        if (!ParticleScene_IsActive(data->particleScene)) {
            ParticleScene_CreateEmitter(data->particleScene, 3);
            ParticleScene_CreateEmitter(data->particleScene, 4);
            ParticleScene_CreateEmitter(data->particleScene, 5);
            ParticleScene_CreateEmitter(data->particleScene, 6);
            ParticleScene_CreateEmitter(data->particleScene, 10);
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xF3FF, 2, 0, 16, RGB_WHITE);
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_AFFINEW, 0);
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_AFFINEH, 0);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEW, 256);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEH, 256);
            PlaySE(SEQ_SE_DP_W062);
            data->scaleOscillation = 0;
            data->timer = 8;
            data->state++;
        }
        break;
    case EVO_STATE_WIDEN_WINDOW:
        if (data->wnd0Top) {
            data->wnd0Top -= 2;
            data->wnd0Bottom += 2;
        }
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0 && --data->timer == 0) {
            ParticleScene_CreateEmitter(data->particleScene, 12);
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xF3FF, 4, 16, 0, RGB_WHITE);
            Pokepic_StartPaletteFadeAll(data->pokepicManager, 16, 0, 3, RGB_WHITE);
            PlaySE(SEQ_SE_DP_W080);
            data->state++;
        }
        break;
    case EVO_STATE_NEW_MON_CRY:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0 && !ParticleScene_IsActive(data->particleScene)) {
            PokepicAnimScript animScript[10];

            NARC_StartPokepicAnim(data->pokegraNarc, data->pokepicAnimManager, data->newPokepic, data->newSpecies, 2, 0, 0);
            NARC_ReadPokepicAnimScript(data->pokegraNarc, animScript, data->newSpecies, 1);
            Pokepic_SetAnimScript(data->newPokepic, animScript);
            EVO_START_POKEPIC_ANIM(data->newPokepic);
            PlayCry(data->newSpecies, data->form);
            data->state++;
        }
        break;
    case EVO_STATE_EVOLVE:
        if (!IsCryFinished() && PokepicAnimManager_GetAnimState(data->pokepicAnimManager, 0) == TRUE && !Pokepic_IsAnimFinished(data->newPokepic)) {
            SetMonData(data->mon, MON_DATA_SPECIES, &data->newSpecies);
            UpdateMonAbility(data->mon);
            CalcMonLevelAndStats(data->mon);
            BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
            BufferBoxMonSpeciesName(data->msgFormat, 1, Mon_GetBoxMon(data->mon));
            data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00918); // Congratulations! Your <mon> evolved into <species>!
            data->timer = 40;
            data->state++;
        }
        break;
    case EVO_STATE_REGISTER:
        if (!TextPrinterCheckActive(data->printerId) && --data->timer == 0) {
            Pokedex_SetMonCaughtFlag(data->pokedex, data->mon);
            GameStats_Inc(data->gameStats, GAME_STAT_UNIQUE_MONS_CAUGHT);
            GameStats_AddScore(data->gameStats, SCORE_EVENT_REGISTER_SPECIES_CAUGHT);
            if (GetMonData(data->mon, MON_DATA_HAS_NICKNAME, NULL) == FALSE) {
                SetMonData(data->mon, MON_DATA_SPECIES_NAME, NULL);
            }
            data->state++;
        }
        break;
    case EVO_STATE_TRY_LEARN_MOVE: {
        u16 move;

        switch (MonTryLearnMoveOnLevelUp(data->mon, &data->learnsetCursor, &move)) {
        case MOVE_NONE:
            data->state = EVO_STATE_FADE_OUT;
            break;
        case MOVE_APPEND_KNOWN:
            break;
        case MOVE_APPEND_FULL:
            data->newMove = move;
            data->state = EVO_STATE_WANTS_TO_LEARN;
            break;
        default:
            BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
            BufferMoveName(data->msgFormat, 1, move);
            data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00004); // <mon> learned <move>!
            data->timer = 30;
            data->state = EVO_STATE_WAIT_LEARNED;
            break;
        }
        break;
    }
    case EVO_STATE_WANTS_TO_LEARN:
        BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
        BufferMoveName(data->msgFormat, 1, data->newMove);
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01193); // <mon> wants to learn the move <move>.
        data->timer = 30;
        data->state++;
        break;
    case EVO_STATE_KNOWS_FOUR_MOVES:
        BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01194); // However, <mon> already knows four moves.
        data->timer = 30;
        data->state++;
        break;
    case EVO_STATE_ASK_DELETE_MOVE:
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01195); // Should a move be deleted?
        data->timer = 1;
        data->state++;
        break;
    case EVO_STATE_WAIT_WANTS_TO_LEARN:
    case EVO_STATE_WAIT_KNOWS_FOUR_MOVES:
    case EVO_STATE_WAIT_ASK_DELETE_MOVE:
    case EVO_STATE_WAIT_POOF:
    case EVO_STATE_WAIT_FORGOT:
    case EVO_STATE_WAIT_AND:
    case EVO_STATE_WAIT_ASK_STOP_LEARNING:
        if (!TextPrinterCheckActive(data->printerId) && --data->timer == 0) {
            data->state++;
        }
        break;
    case EVO_STATE_SHOW_DELETE_MOVE_MENU:
        EvolutionScene_ShowMenu(data, TRUE);
        data->state++;
        break;
    case EVO_STATE_DELETE_MOVE_MENU:
        switch (EvolutionScene_RunMenu(data)) {
        case EVO_MENU_TOP:
            data->state = EVO_STATE_OPEN_SUMMARY;
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xFFFF, 1, 0, 16, RGB_BLACK);
            Pokepic_StartPaletteFadeAll(data->pokepicManager, 0, 16, 0, RGB_BLACK);
            break;
        case EVO_MENU_BOTTOM:
            data->state = EVO_STATE_ASK_STOP_LEARNING;
            break;
        default:
            break;
        }
        break;
    case EVO_STATE_OPEN_SUMMARY:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            EvolutionScene_FreeBgs(data->bgConfig);
            EvolutionScene_FreeSprites(data);
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_VANISHED, TRUE);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_VANISHED, TRUE);
            data->summaryArgs->party = (Party *)data->mon;
            data->summaryArgs->options = data->options;
            data->summaryArgs->monDataType = 0;
            data->summaryArgs->partySlot = 0;
            data->summaryArgs->partyCount = 1;
            data->summaryArgs->moveToLearn = data->newMove;
            data->summaryArgs->mode = 2;
            data->summaryArgs->chatot = 0;
            data->summaryArgs->menuInputStatePtr = NULL;
            PokemonSummary_SetPages(data->summaryArgs, gEvolutionSummaryPages);
            EvolutionScene_OpenSummary(data);
            data->state++;
        }
        break;
    case EVO_STATE_RUN_SUMMARY:
        if (OverlayManager_Run(data->summaryApp)) {
            OverlayManager_Delete(data->summaryApp);
            data->summaryApp = NULL;
            EvolutionScene_InitGraphics(data, data->bgConfig);
            EvolutionScene_InitSprites(data);
            DrawFrameAndWindow2(data->window, FALSE, 1, 10);
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_VANISHED, FALSE);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_VANISHED, FALSE);
            Pokepic_ScheduleReloadFromNarc(data->oldPokepic);
            Pokepic_ScheduleReloadFromNarc(data->newPokepic);
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xFFFF, 1, 16, 0, RGB_BLACK);
            Pokepic_StartPaletteFadeAll(data->pokepicManager, 16, 0, 0, RGB_BLACK);
            WirelessStatusIcon_Create();
            data->state++;
        }
        break;
    case EVO_STATE_SUMMARY_CLOSED:
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            if (data->summaryArgs->selectedMoveSlot == 4) {
                data->state = EVO_STATE_ASK_STOP_LEARNING;
            } else {
                data->moveSlot = data->summaryArgs->selectedMoveSlot;
                data->state = EVO_STATE_PRINT_POOF;
            }
        }
        break;
    case EVO_STATE_ASK_STOP_LEARNING:
        BufferMoveName(data->msgFormat, 0, data->newMove);
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01197); // Stop trying to teach <move>?
        data->timer = 1;
        data->state++;
        break;
    case EVO_STATE_SHOW_STOP_LEARNING_MENU:
        EvolutionScene_ShowMenu(data, FALSE);
        data->state++;
        break;
    case EVO_STATE_STOP_LEARNING_MENU:
        switch (EvolutionScene_RunMenu(data)) {
        case EVO_MENU_TOP:
            BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
            BufferMoveName(data->msgFormat, 1, data->newMove);
            data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01198); // <mon> did not learn the move <move>.
            data->timer = 30;
            data->state = EVO_STATE_WAIT_DID_NOT_LEARN;
            break;
        case EVO_MENU_BOTTOM:
            data->state = EVO_STATE_WANTS_TO_LEARN;
            break;
        default:
            break;
        }
        break;
    case EVO_STATE_WAIT_DID_NOT_LEARN:
        if (!TextPrinterCheckActive(data->printerId) && --data->timer == 0) {
            data->state = EVO_STATE_TRY_LEARN_MOVE;
        }
        break;
    case EVO_STATE_PRINT_POOF:
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01199); // 1, 2, and... ... ... Poof!
        data->timer = 30;
        data->state++;
        break;
    case EVO_STATE_PRINT_FORGOT:
        BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
        BufferMoveName(data->msgFormat, 1, GetMonData(data->mon, MON_DATA_MOVE1 + data->moveSlot, NULL));
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01200); // <mon> forgot how to use <move>.
        data->timer = 30;
        data->state++;
        break;
    case EVO_STATE_PRINT_AND:
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01201); // And...
        data->timer = 30;
        data->state++;
        break;
    case EVO_STATE_LEARN_MOVE:
        BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
        BufferMoveName(data->msgFormat, 1, data->newMove);
        data->printerId = EvolutionScene_PrintMessage(data, msg_0197_01202); // <mon> learned <move>!
        data->timer = 0;
        SetMonData(data->mon, MON_DATA_MOVE1_PP_UPS + data->moveSlot, &data->timer);
        MonSetMoveInSlot(data->mon, data->newMove, data->moveSlot);
        data->timer = 30;
        data->state = EVO_STATE_WAIT_LEARNED;
        break;
    case EVO_STATE_WAIT_LEARNED:
        if (!TextPrinterCheckActive(data->printerId)) {
            data->state++;
        }
        break;
    case EVO_STATE_WAIT_FANFARE:
        if (!IsFanfarePlaying() && --data->timer == 0) {
            data->state = EVO_STATE_TRY_LEARN_MOVE;
        }
        break;
    case EVO_STATE_FADE_OUT:
        PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xFFFF, 1, 0, 16, RGB_BLACK);
        Pokepic_StartPaletteFadeAll(data->pokepicManager, 0, 16, 0, RGB_BLACK);
        data->state++;
        break;
    case EVO_STATE_FINISH:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            ParticleScene_Free(data->particleScene);
            EvolutionScene_ApplyEvolutionEffects(data);
            data->done = TRUE;
        }
        break;
    case EVO_STATE_CANCEL:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_AFFINEW, 256);
            Pokepic_SetAttr(data->oldPokepic, POKEPIC_AFFINEH, 256);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEW, 0);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEH, 0);
            Pokepic_SetAttr(data->newPokepic, POKEPIC_VANISHED, TRUE);
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xF3FF, 0, 16, 0, RGB_WHITE);
            Pokepic_StartPaletteFadeAll(data->pokepicManager, 16, 0, 0, RGB_WHITE);
            data->wnd0Left = 0;
            data->wnd0Top = 0;
            data->wnd0Right = 255;
            data->wnd0Bottom = 160;
            data->scaleOscillation = 0;
            StopBGM(SEQ_GS_SHINKA, 0);
            ParticleScene_Free(data->particleScene);
            data->state++;
        }
        break;
    case EVO_STATE_CANCEL_CRY:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            PokepicAnimScript animScript[10];

            NARC_StartPokepicAnim(data->pokegraNarc, data->pokepicAnimManager, data->oldPokepic, data->species, 2, 0, 0);
            NARC_ReadPokepicAnimScript(data->pokegraNarc, animScript, data->species, 1);
            Pokepic_SetAnimScript(data->oldPokepic, animScript);
            EVO_START_POKEPIC_ANIM(data->oldPokepic);
            PlayCry(data->species, data->form);
            data->state++;
        }
        break;
    case EVO_STATE_CANCEL_WAIT_CRY:
        if (!IsCryFinished() && PokepicAnimManager_GetAnimState(data->pokepicAnimManager, 0) == TRUE && !Pokepic_IsAnimFinished(data->oldPokepic)) {
            BufferBoxMonNickname(data->msgFormat, 0, Mon_GetBoxMon(data->mon));
            data->printerId = EvolutionScene_PrintMessage(data, msg_0197_00919); // Huh? <mon> stopped evolving!
            data->timer = 20;
            data->state++;
        }
        break;
    case EVO_STATE_CANCEL_WAIT_MESSAGE:
        if (!TextPrinterCheckActive(data->printerId) && --data->timer == 0) {
            PaletteData_BeginPaletteFade(data->paletteData, 0xF, 0xFFFF, 1, 0, 16, RGB_BLACK);
            Pokepic_StartPaletteFadeAll(data->pokepicManager, 0, 16, 0, RGB_BLACK);
            data->state++;
        }
        break;
    case EVO_STATE_CANCEL_FINISH:
        if (PaletteData_GetSelectedBuffersBitmask(data->paletteData) == 0) {
            data->done = TRUE;
        }
        break;
    }
}

#pragma force_active reset

// Ninjask's evolution leaves a Shedinja in a free party slot when the bag holds a Poke Ball (which it uses up);
// evolutions that consume the held item clear it.
void EvolutionScene_ApplyEvolutionEffects(EvolutionTaskData *data) {
    int i;
    int value;
    Pokemon *shedinja;
    Mail *mail;
    u8 mailData[0x18];

    switch (data->evolutionCondition) {
    case EVO_LEVEL_NINJASK:
    case EVO_LEVEL_SHEDINJA:
        if (Bag_GetQuantity(data->bag, ITEM_POKE_BALL, data->heapId) == 0) {
            break;
        }
        if (Party_GetCount(data->party) >= Party_GetMaxCount(data->party)) {
            break;
        }
        shedinja = AllocMonZeroed(data->heapId);
        CopyPokemonToPokemon(data->mon, shedinja);
        value = SPECIES_SHEDINJA;
        SetMonData(shedinja, MON_DATA_SPECIES, &value);
        value = ITEM_POKE_BALL;
        SetMonData(shedinja, MON_DATA_POKEBALL, &value);
        value = 0;
        SetMonData(shedinja, MON_DATA_HELD_ITEM, &value);
        SetMonData(shedinja, MON_DATA_MARKINGS, &value);
        for (i = MON_DATA_SINNOH_CHAMP_RIBBON; i < MON_DATA_MOVE1; i++) {
            SetMonData(shedinja, i, &value);
        }
        for (i = MON_DATA_COOL_RIBBON; i < MON_DATA_FATEFUL_ENCOUNTER; i++) {
            SetMonData(shedinja, i, &value);
        }
        for (i = MON_DATA_SUPER_COOL_RIBBON; i < MON_DATA_OT_NAME; i++) {
            SetMonData(shedinja, i, &value);
        }
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A, &value);
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A + 1, &value);
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A + 2, &value);
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A + 3, &value);
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A + 4, &value);
        SetMonData(shedinja, MON_DATA_SHINY_LEAF_A + 5, &value);
        SetMonData(shedinja, MON_DATA_MOOD, &value);
        SetMonData(shedinja, MON_DATA_SPECIES_NAME, NULL);
        SetMonData(shedinja, MON_DATA_HAS_NICKNAME, &value);
        SetMonData(shedinja, MON_DATA_STATUS, &value);
        mail = Mail_New(data->heapId);
        SetMonData(shedinja, MON_DATA_MAIL, mail);
        Heap_Free(mail);
        SetMonData(shedinja, MON_DATA_BALL_CAPSULE_ID, &value);
        MI_CpuClearFast(mailData, sizeof(mailData));
        SetMonData(shedinja, MON_DATA_BALL_CAPSULE, mailData);
        UpdateMonAbility(shedinja);
        CalcMonLevelAndStats(shedinja);
        Party_AddMon(data->party, shedinja);
        Pokedex_SetMonCaughtFlag(data->pokedex, shedinja);
        GameStats_Inc(data->gameStats, GAME_STAT_UNIQUE_MONS_CAUGHT);
        GameStats_AddScore(data->gameStats, SCORE_EVENT_REGISTER_SPECIES_CAUGHT);
        Heap_Free(shedinja);
        Bag_TakeItem(data->bag, ITEM_POKE_BALL, 1, data->heapId);
        break;
    case EVO_TRADE_ITEM:
    case EVO_ITEM_DAY:
    case EVO_ITEM_NIGHT:
        i = ITEM_NONE;
        SetMonData(data->mon, MON_DATA_HELD_ITEM, &i);
        break;
    }
}

#pragma force_active on

void EvolutionScene_InitGraphics(EvolutionTaskData *data, BgConfig *bgConfig) {
    EvolutionBgTemplates mainTemplates;
    EvolutionBgTemplates subTemplates;
    GraphicsBanks banks;
    GraphicsModes modes;
    u32 frame;
    NARC *narc;

    GfGfx_DisableEngineAPlanes();
    banks = gEvolutionGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MI_CpuClear32((void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    modes = gEvolutionGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    mainTemplates = gEvolutionMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainTemplates.bg[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainTemplates.bg[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainTemplates.bg[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    subTemplates = gEvolutionSubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subTemplates.bg[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subTemplates.bg[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subTemplates.bg[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    frame = Options_GetFrame(data->options);
    LoadUserFrameGfx2WithFill(bgConfig, GF_BG_LYR_MAIN_1, 1, 10, frame, data->heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_1_5, 0, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, data->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_5, 1, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, data->heapId);
    PaletteData_LoadNarc(data->paletteData, NARC_a_1_1_5, 8, data->heapId, PLTTBUF_MAIN_BG, 0x40, 0);
    PaletteData_LoadNarc(data->paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), data->heapId, PLTTBUF_MAIN_BG, 0x20, 0xA0);
    PaletteData_LoadNarc(data->paletteData, NARC_graphic_font, 8, data->heapId, PLTTBUF_MAIN_BG, 0x20, 0xB0);
    LoadUserFrameGfx1Tiles(data->bgConfig, GF_BG_LYR_MAIN_2, 1, 0, data->heapId);
    PaletteData_LoadNarc(data->paletteData, NARC_a_0_3_8, GetUserFrameGfx1PaletteMember(), data->heapId, PLTTBUF_MAIN_BG, 0x20, 0x80);
    PaletteData_LoadNarc(data->paletteData, NARC_a_2_3_7, 0, data->heapId, PLTTBUF_SUB_BG, 0xA0, 0);
    PaletteData_LoadNarc(data->paletteData, NARC_a_2_3_7, 15, data->heapId, PLTTBUF_SUB_BG, 0x20, 0x90);
    PaletteData_LoadNarc(data->paletteData, NARC_graphic_font, 9, data->heapId, PLTTBUF_SUB_BG, 0x20, 0xF0);

    narc = NARC_New(NARC_a_2_3_7, data->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 16, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, data->heapId);
    NARC_Delete(narc);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);

    GX_SetVisibleWnd(GX_WNDMASK_W0);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ, FALSE);
    data->wnd0Left = 0;
    data->wnd0Top = 0;
    data->wnd0Right = 255;
    data->wnd0Bottom = 160;
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    Main_SetVBlankIntrCB(EvolutionScene_VBlankCB, data);
}

#pragma force_active reset

void EvolutionScene_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
}

void EvolutionScene_CreatePokepics(EvolutionTaskData *data) {
    PokepicTemplate template;
    Pokemon *newMon;

    GetPokemonSpriteCharAndPlttNarcIds(&template, data->mon, 2);
    data->oldPokepic = PokepicManager_CreatePokepic(data->pokepicManager, &template, 128, 80, 0, 0, NULL, NULL);
    newMon = AllocMonZeroed(data->heapId);
    CopyPokemonToPokemon(data->mon, newMon);
    SetMonData(newMon, MON_DATA_SPECIES, &data->newSpecies);
    CalcMonLevelAndStats(newMon);
    GetPokemonSpriteCharAndPlttNarcIds(&template, newMon, 2);
    Heap_Free(newMon);
    data->newPokepic = PokepicManager_CreatePokepic(data->pokepicManager, &template, 128, 80, 0, 0, NULL, NULL);
    Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEW, 0);
    Pokepic_SetAttr(data->newPokepic, POKEPIC_AFFINEH, 0);
}

void EvolutionScene_VBlankCB(void *cbData) {
    EvolutionTaskData *data = cbData;

    G2_SetWnd0Position(data->wnd0Left, data->wnd0Top, data->wnd0Right, data->wnd0Bottom);
    PokepicManager_HandleLoadImgAndOrPltt(data->pokepicManager);
    if (data->spritesReady) {
        GF_ASSERT(data->spriteManager != NULL);
        SpriteSystem_DrawSprites(data->spriteManager);
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    PaletteData_PushTransparentBuffers(data->paletteData);
    DoScheduledBgGpuUpdates(data->bgConfig);
    *(u32 *)((u8 *)OS_IRQTable + (HW_INTR_CHECK_BUF - HW_DTCM)) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

u8 EvolutionScene_PrintMessage(EvolutionTaskData *data, int msgNo) {
    String *string = NewString_ReadMsgData(data->msgData, msgNo);

    StringExpandPlaceholders(data->msgFormat, data->string, string);
    Heap_Free(string);
    FillWindowPixelBuffer(data->window, 0xFF);
    return AddTextPrinterParameterized(data->window, 1, data->string, 0, 0, Options_GetTextFrameDelay(data->options), EvolutionScene_TextCallback);
}

// {WAIT n} commands of the scene's messages
BOOL EvolutionScene_TextCallback(TextPrinterTemplate *template, u16 glyphId) {
    BOOL result = FALSE;

    switch (glyphId) {
    case 1:
        result = GF_IsAnySEPlaying();
        break;
    case 2:
        result = IsFanfarePlaying();
        break;
    case 3:
        PlayFanfare(SEQ_ME_SHINKAOME);
        break;
    case 4:
        PlaySE(SEQ_SE_DP_KON);
        break;
    case 5:
        PlayFanfare(SEQ_ME_LVUP);
        break;
    }
    return result;
}

void EvolutionScene_OpenSummary(EvolutionTaskData *data) {
    data->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, data->summaryArgs, data->heapId);
}
