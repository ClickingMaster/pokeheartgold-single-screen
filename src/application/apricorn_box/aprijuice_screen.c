#include "global.h"

#include "constants/sndseq.h"

#include "application/apricorn_box/apricorn_box_overlay_internal.h"
#include "application/apricorn_box/apricorn_box_screens.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0022.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "palette.h"
#include "particle_system.h"
#include "party.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "swap_3d_buffers.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

int ApricornBox_UseJuiceServing(SaveApricornBox *box);
void ParticleSystem_DeleteAllEmitters(SPLEmitter *emitter);

// A party Pokemon that can drink the juice.
typedef struct AprijuiceMon {
    Pokemon *mon;                                  // 0x00
    u16 species;                                   // 0x04
    u8 level;                                      // 0x06
    u8 gender;                                     // 0x07
    u8 slot;                                       // 0x08
    u8 hFlip;                                      // 0x09
    u8 filler_0A[2];                               // 0x0A
    String *name;                                  // 0x0C
    PartyExtraSub aprijuice;                       // 0x10
    u8 filler_15;                                  // 0x15
    struct PokeathlonPerformanceStars stars;       // 0x16
    struct PokeathlonTodayPerformance performance; // 0x1E
} AprijuiceMon;                                    // size: 0x34

// The 3D part of the Aprijuice screen: the Pokemon picture and the particle effect.
typedef struct AprijuiceScene {
    enum HeapID heapId;             // 0x00
    GF3DVramMan *vramMan;           // 0x04
    Camera *camera;                 // 0x08
    PokepicManager *pokepicManager; // 0x0C
    u8 filler_10[0x10];             // 0x10
    Pokepic *pokepic;               // 0x20
    SPLEmitter *emitter;            // 0x24
} AprijuiceScene;                   // size: 0x28

// The Aprijuice screen (OVY_59 second app): give the juice to a party Pokemon and show its Performance change.
typedef struct AprijuiceScreen {
    enum HeapID heapId;                         // 0x000
    ApricornBoxApp *app;                        // 0x004
    SaveData *saveData;                         // 0x008
    u8 filler_0C[4];                            // 0x00C
    Party *party;                               // 0x010
    SaveApricornBox *apricornBox;               // 0x014
    ApricornJuice juiceRecord;                  // 0x018
    ApricornBoxJuice juice;                     // 0x020
    struct PokeathlonPerformanceStars oldStars; // 0x02E
    u8 stat;                                    // 0x036
    u8 starCount;                               // 0x037
    u8 starsShown;                              // 0x038
    s8 starDelta[5];                            // 0x039
    u8 numChanged;                              // 0x03E
    u8 filler_3F;                               // 0x03F
    u16 subState;                               // 0x040
    u16 animState;                              // 0x042
    MenuInputState menuInputState;              // 0x044
    u8 juiceUsed;                               // 0x048
    u8 numMons;                                 // 0x049
    u8 selectedMon;                             // 0x04A
    u8 textFrameDelay;                          // 0x04B
    u8 frame;                                   // 0x04C
    u8 textPrinterId;                           // 0x04D
    u8 filler_4E;                               // 0x04E
    u8 activeTasks;                             // 0x04F
    u8 filler_50;                               // 0x050
    u8 timer;                                   // 0x051
    u8 filler_52[2];                            // 0x052
    BgConfig *bgConfig;                         // 0x054
    u8 filler_58[4];                            // 0x058
    MsgData *msgData;                           // 0x05C
    MessageFormat *msgFormat;                   // 0x060
    String *string;                             // 0x064
    String *string2;                            // 0x068
    String *levelTemplate;                      // 0x06C, message 4
    String *genderSymbols[2];                   // 0x070, messages 10-11
    AprijuiceScene *scene;                      // 0x078
    AprijuiceMon mons[PARTY_SIZE];              // 0x07C
    Window windows[10];                         // 0x1B4
    YesNoPrompt *yesNoPrompt;                   // 0x254
    SpriteSystem *spriteSystem;                 // 0x258
    SpriteManager *spriteManager;               // 0x25C
    Sprite *sprites[34];                        // 0x260
    NARC *narc;                                 // 0x2E8
    u8 filler_2EC[8];                           // 0x2EC
} AprijuiceScreen;                              // size: 0x2F4

// Pops a Performance star in (grows and spins) or out (shrinks and hides it) (task).
typedef struct AprijuiceStarTask {
    AprijuiceScreen *owner; // 0x00
    u8 hide;                // 0x04
    u8 filler_05;           // 0x05
    u8 timer;               // 0x06
    u8 filler_07;           // 0x07
    fx32 scaleStep;         // 0x08
    u16 angle;              // 0x0C
    s16 angleStep;          // 0x0E
    VecFx32 scale;          // 0x10
    Sprite *sprite;         // 0x1C
} AprijuiceStarTask;        // size: 0x20

// Shakes the Pokemon picture (task).
typedef struct AprijuiceShakeTask {
    AprijuiceScreen *owner; // 0x00
    u8 counter;             // 0x04
    u8 phase;               // 0x05
    u8 delta;               // 0x06
} AprijuiceShakeTask;       // size: 0x08

// Particle effect and white fade of the Pokemon picture (task).
typedef struct AprijuiceFadeTask {
    AprijuiceScreen *owner; // 0x00
    u8 timer;               // 0x04
    u8 state;               // 0x05
} AprijuiceFadeTask;        // size: 0x08
void AprijuiceScene_InitCamera(AprijuiceScene *scene, enum HeapID heapId);

static void AprijuiceScreen_InitFromArgs(AprijuiceScreen *screen);
static void AprijuiceScreen_WriteResult(AprijuiceScreen *screen);
static BOOL AprijuiceScreen_Setup(AprijuiceScreen *screen);
static BOOL AprijuiceScreen_Teardown(AprijuiceScreen *screen);
static int AprijuiceScreen_FadeIn(AprijuiceScreen *screen);
static int AprijuiceScreen_FadeOut(AprijuiceScreen *screen);
static int AprijuiceScreen_InputState(AprijuiceScreen *screen);
static int AprijuiceScreen_DrinkState(AprijuiceScreen *screen);
static int AprijuiceScreen_StatChangesState(AprijuiceScreen *screen);
static void AprijuiceScreen_ResetDisplay(void);
static void AprijuiceScreen_InitGraphics(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeGraphics(AprijuiceScreen *screen);
static void AprijuiceScreen_DrawScreen(AprijuiceScreen *screen);
static void AprijuiceScreen_LoadParty(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeParty(AprijuiceScreen *screen);
static void AprijuiceScreen_DrawButtons(AprijuiceScreen *screen, u32 state);
static int AprijuiceScreen_HandleInput(AprijuiceScreen *screen);
static BOOL AprijuiceScreen_PlayDrinkAnim(AprijuiceScreen *screen);
static BOOL AprijuiceScreen_PrintNothingChanged(AprijuiceScreen *screen);
static BOOL AprijuiceScreen_ShowStatChange(AprijuiceScreen *screen);
static int AprijuiceScreen_Select(AprijuiceScreen *screen, int input);
static int AprijuiceScreen_HandleKeys(AprijuiceScreen *screen);
static int AprijuiceScreen_HandleTouch(AprijuiceScreen *screen, int *hit);
static void AprijuiceScreen_VBlankCB(AprijuiceScreen *screen);
static void AprijuiceScreen_SetGfxBanks(void);
static void AprijuiceScreen_InitBgs(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeBgs(AprijuiceScreen *screen);
static void AprijuiceScreen_LoadBgGraphics(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeBgGraphics(AprijuiceScreen *screen);
static void AprijuiceScreen_InitStrings(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeStrings(AprijuiceScreen *screen);
static void AprijuiceScreen_AddWindows(AprijuiceScreen *screen);
static void AprijuiceScreen_RemoveWindows(AprijuiceScreen *screen);
static void AprijuiceScreen_InitSpriteSystem(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeSpriteSystem(AprijuiceScreen *screen);
static void AprijuiceScreen_InitSprites(AprijuiceScreen *screen);
static void AprijuiceScreen_FreeSprites(AprijuiceScreen *screen);
static void AprijuiceScreen_CreateSprites(AprijuiceScreen *screen);
static void AprijuiceScreen_DeleteSprites(AprijuiceScreen *screen);
static void AprijuiceMon_Init(AprijuiceMon *entry, Party *party, Pokemon *mon, u8 slot, enum HeapID heapId);
static void AprijuiceMon_Free(AprijuiceMon *entry);
static void AprijuiceScreen_UseJuice(AprijuiceScreen *screen);
static void AprijuiceScreen_DrawMessageBox(AprijuiceScreen *screen);
static void AprijuiceScreen_PrintMessage(AprijuiceScreen *screen, int message);
static void AprijuiceScreen_ShowMon(AprijuiceScreen *screen, int index);
static void AprijuiceScreen_DrawStatStars(AprijuiceScreen *screen, AprijuiceMon *entry, u8 stat);
static void AprijuiceScreen_DrawAllStars(AprijuiceScreen *screen, AprijuiceMon *entry);
static void AprijuiceScreen_SetStarSprite(AprijuiceScreen *screen, int index, u8 sequence, int draw);
static void AprijuiceScreen_StartStarTask(AprijuiceScreen *screen, int stat, int shrink);
static void AprijuiceScreen_StarTask(SysTask *task, AprijuiceStarTask *star);
static void AprijuiceScreen_StartShakeTask(AprijuiceScreen *screen);
static void AprijuiceScreen_ShakeTask(SysTask *task, AprijuiceShakeTask *shake);
static void AprijuiceScreen_StartFadeTask(AprijuiceScreen *screen);
static void AprijuiceScreen_FadeTask(SysTask *task, AprijuiceFadeTask *fade);
static AprijuiceScene *AprijuiceScene_New(enum HeapID heapId);
static void AprijuiceScene_Draw(AprijuiceScene *scene);
static void AprijuiceScene_Delete(AprijuiceScene *scene);
static void AprijuiceScene_TransferPokepicGfx(AprijuiceScene *scene);
static void AprijuiceScene_CreatePokepic(AprijuiceScene *scene, AprijuiceMon *entry);
static void AprijuiceScene_DeletePokepic(AprijuiceScene *scene);
static void AprijuiceScene_StartParticles(AprijuiceScene *scene, int resource);
static BOOL AprijuiceScene_AreParticlesActive(AprijuiceScene *scene);
static void AprijuiceScene_Init3D(void);
static u32 AprijuiceScene_AllocTexVram(u32 size, BOOL is4x4);
static u32 AprijuiceScene_AllocPlttVram(u32 size, BOOL is4color);
static void AprijuiceScene_SetEmitterPosition(SPLEmitter *emitter);
static SPLEmitter *AprijuiceScene_CreateParticleSystem(enum HeapID heapId);
static void AprijuiceScene_InitParticles(AprijuiceScene *scene);
static void AprijuiceScene_FreeParticles(AprijuiceScene *scene);
static void AprijuiceScene_ReserveVram(GF3DVramMan *vramMan);

static const u32 sAprijuiceButtonColors[] = {
    MAKE_TEXT_COLOR(3, 15, 0),
    MAKE_TEXT_COLOR(7, 15, 0),
};

static const u32 sAprijuiceGenderColors[] = {
    MAKE_TEXT_COLOR(5, 6, 0),
    MAKE_TEXT_COLOR(3, 4, 0),
};

static const TouchscreenHitbox sAprijuiceHitboxes[] = {
    { .rect = { 144, 168, 192, 0 } },
    { .rect = { 168, 192, 192, 0 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const ResdatIdList sAprijuiceSpriteResources = {
    .charRes = 14,
    .plttRes = 15,
    .cellRes = 13,
    .animRes = 12,
    .mcelRes = 0xFFFF,
    .manmRes = 0xFFFF,
    .headerId = 75,
};

static const GraphicsModes sAprijuiceGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const OamCharTransferParam sAprijuiceOamCharTransferParam = {
    .maxTasks = 3,
    .sizeMain = 0x4000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

// Order of the Performance stats on the screen
static const u32 sAprijuiceStatOrder[] = { 4, 0, 2, 1, 3 };

static const BgTemplate sAprijuiceBgTemplate_Main3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sAprijuiceBgTemplate_Sub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x7800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sAprijuiceBgTemplate_Main1 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sAprijuiceBgTemplate_Main2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const OamManagerParam sAprijuiceOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const WindowTemplate sAprijuiceWindowTemplates[] = {
    { GF_BG_LYR_MAIN_1, 2,  1,  27, 2, 12, 0x1F },
    { GF_BG_LYR_MAIN_1, 25, 18, 6,  6, 12, 0x55 },
    { GF_BG_LYR_MAIN_1, 25, 21, 6,  2, 12, 0x79 },
    { GF_BG_LYR_MAIN_2, 20, 1,  10, 2, 12, 0x85 },
    { GF_BG_LYR_MAIN_2, 20, 4,  10, 4, 12, 0x99 },
};

static const GraphicsBanks sAprijuiceGraphicsBanks = {
    .bg = GX_VRAM_BG_128_C,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_16_G,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_0_F,
};

// The last two are the stat modifier icon and the Performance star
static const UnmanagedSpriteTemplate sAprijuiceSpriteTemplates[] = {
    { 0, 224, 160, 0, 0,  2, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 224, 184, 0, 2,  1, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 208, 88,  0, 10, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 80,  40,  0, 8,  5, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 64,  56,  0, 4,  4, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
};

BOOL AprijuiceScreen_Init(OverlayManager *man, int *state) {
    AprijuiceScreen *screen;
    switch (*state) {
    case 0:
        AprijuiceScreen_ResetDisplay();
        Heap_Create(HEAP_ID_3, HEAP_ID_134, 9 << 16);
        screen = OverlayManager_CreateAndGetData(man, sizeof(AprijuiceScreen), HEAP_ID_134);
        MI_CpuFill8(screen, 0, sizeof(AprijuiceScreen));
        screen->heapId = HEAP_ID_134;
        screen->app = OverlayManager_GetArgs(man);
        AprijuiceScreen_InitFromArgs(screen);
        (*state)++;
        break;
    case 1:
        screen = OverlayManager_GetData(man);
        if (AprijuiceScreen_Setup(screen)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AprijuiceScreen_Main(OverlayManager *man, int *state) {
    AprijuiceScreen *screen = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        *state = AprijuiceScreen_FadeIn(screen);
        break;
    case 1:
        *state = AprijuiceScreen_FadeOut(screen);
        break;
    case 2:
        *state = AprijuiceScreen_InputState(screen);
        break;
    case 4:
        *state = AprijuiceScreen_DrinkState(screen);
        break;
    case 5:
        *state = AprijuiceScreen_StatChangesState(screen);
        break;
    case 8:
        return TRUE;
    }
    SpriteSystem_DrawSprites(screen->spriteManager);
    AprijuiceScene_Draw(screen->scene);
    return FALSE;
}

BOOL AprijuiceScreen_Exit(OverlayManager *man, int *state) {
    AprijuiceScreen *screen = OverlayManager_GetData(man);
    if (!AprijuiceScreen_Teardown(screen)) {
        return FALSE;
    }
    AprijuiceScreen_WriteResult(screen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_134);
    return TRUE;
}

static void AprijuiceScreen_InitFromArgs(AprijuiceScreen *screen) {
    int i;
    Options *options;
    u8 *entry;
    screen->menuInputState = MenuInputStateMgr_GetState(screen->app->args->menuInputStatePtr);
    screen->saveData = screen->app->args->saveData;
    options = Save_PlayerData_GetOptionsAddr(screen->saveData);
    screen->textFrameDelay = Options_GetTextFrameDelay(options);
    screen->frame = Options_GetFrame(options);
    screen->apricornBox = Save_ApricornBox_Get(screen->saveData);
    ApricornBox_GetJuice(screen->apricornBox, screen->app->juiceIndex, &screen->juiceRecord);
    ApricornJuice_RankFlavors(&screen->juiceRecord, &screen->juice);
    AprijuiceScreen_LoadParty(screen);
    i = 0;
    if (i < screen->numMons) {
        entry = (u8 *)screen;
        do {
            if (screen->app->partySlot == ((AprijuiceScreen *)entry)->mons[0].slot) {
                screen->selectedMon = i;
                return;
            }
            i++;
            entry += sizeof(AprijuiceMon);
        } while (i < screen->numMons);
    }
}

static void AprijuiceScreen_WriteResult(AprijuiceScreen *screen) {
    AprijuiceMon *entry;
    AprijuiceScreen_FreeParty(screen);
    MenuInputStateMgr_SetState(screen->app->args->menuInputStatePtr, screen->menuInputState);
    entry = &screen->mons[screen->selectedMon];
    screen->app->partySlot = entry->slot;
    screen->app->mode = screen->juiceUsed != 0 ? APRICORN_BOX_MODE_BLENDER : APRICORN_BOX_MODE_USE_JUICE;
    if (screen->app->args->juiceUsedVar != NULL) {
        *screen->app->args->juiceUsedVar = screen->juiceUsed;
    }
}

static BOOL AprijuiceScreen_Setup(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        AprijuiceScreen_InitGraphics(screen);
        screen->subState++;
        break;
    case 1:
        AprijuiceScreen_DrawScreen(screen);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

static BOOL AprijuiceScreen_Teardown(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        GF_TouchpadStopSampling();
        AprijuiceScreen_FreeGraphics(screen);
        screen->subState++;
        break;
    case 1:
    default:
        AprijuiceScreen_ResetDisplay();
        screen->subState = 0;
        return TRUE;
    }
    return FALSE;
}

static int AprijuiceScreen_FadeIn(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return 2;
        }
        break;
    }
    return 0;
}

static int AprijuiceScreen_FadeOut(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, screen->heapId);
        screen->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            screen->subState = 0;
            return 8;
        }
        break;
    }
    return 1;
}

static int AprijuiceScreen_InputState(AprijuiceScreen *screen) {
    return AprijuiceScreen_HandleInput(screen);
}

static int AprijuiceScreen_DrinkState(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        if (AprijuiceScreen_PlayDrinkAnim(screen)) {
            if (screen->numChanged != 0) {
                screen->subState = 0;
                screen->stat = 0;
                return 5;
            }
            screen->subState++;
        }
        break;
    case 1:
        if (AprijuiceScreen_PrintNothingChanged(screen)) {
            screen->subState = 0;
            return 1;
        }
        break;
    }
    return 4;
}

static int AprijuiceScreen_StatChangesState(AprijuiceScreen *screen) {
    switch (screen->subState) {
    case 0:
        while (screen->stat < 5) {
            if (screen->starDelta[screen->stat] != 0) {
                break;
            }
            screen->stat++;
        }
        screen->subState++;
        // fallthrough
    case 1:
        if (AprijuiceScreen_ShowStatChange(screen)) {
            screen->subState = 0;
            screen->stat++;
            screen->numChanged--;
            if (screen->numChanged == 0) {
                return 1;
            }
        }
        break;
    }
    return 5;
}

static void AprijuiceScreen_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

static void AprijuiceScreen_InitGraphics(AprijuiceScreen *screen) {
    AprijuiceScreen_InitBgs(screen);
    AprijuiceScreen_LoadBgGraphics(screen);
    AprijuiceScreen_InitStrings(screen);
    AprijuiceScreen_AddWindows(screen);
    AprijuiceScreen_InitSprites(screen);
    screen->scene = AprijuiceScene_New(screen->heapId);
    Main_SetVBlankIntrCB((GFIntrCB)AprijuiceScreen_VBlankCB, screen);
}

static void AprijuiceScreen_FreeGraphics(AprijuiceScreen *screen) {
    AprijuiceScene_Delete(screen->scene);
    AprijuiceScreen_FreeSprites(screen);
    AprijuiceScreen_RemoveWindows(screen);
    AprijuiceScreen_FreeStrings(screen);
    AprijuiceScreen_FreeBgGraphics(screen);
    AprijuiceScreen_FreeBgs(screen);
}

static void AprijuiceScreen_DrawScreen(AprijuiceScreen *screen) {
    int i;
    Window *windows;
    AprijuiceScreen_DrawButtons(screen, 2);
    ReadMsgDataIntoString(screen->msgData, msg_0022_00000, screen->string2);
    AddTextPrinterParameterizedWithColor(&screen->windows[3], 0, screen->string2, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&screen->windows[3]);
    i = 0;
    windows = screen->windows;
    for (; i < 5; i++) {
        ReadMsgDataIntoString(screen->msgData, sAprijuiceStatOrder[i] + msg_0022_00005, screen->string2);
        AddTextPrinterParameterizedWithColor(&windows[i + 5], 0, screen->string2, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&windows[i + 5]);
    }
    AprijuiceScreen_ShowMon(screen, screen->selectedMon);
}

static void AprijuiceScreen_LoadParty(AprijuiceScreen *screen) {
    int count;
    int i;
    int valid;
    u8 *entries;
    screen->party = SaveArray_Party_Get(screen->saveData);
    count = Party_GetCount(screen->party);
    valid = 0;
    i = valid;
    if (count > 0) {
        entries = (u8 *)screen;
        entries += offsetof(AprijuiceScreen, mons);
        do {
            Pokemon *mon = Party_GetMonByIndex(screen->party, i);
            if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0) {
                AprijuiceMon_Init((AprijuiceMon *)(entries + valid++ * sizeof(AprijuiceMon)), screen->party, mon, (u8)i, screen->heapId);
            }
            i++;
        } while (i < count);
    }
    screen->numMons = valid;
}

static void AprijuiceScreen_FreeParty(AprijuiceScreen *screen) {
    int i;
    int count;
    AprijuiceMon *entry;
    i = 0;
    count = screen->numMons;
    if (count > 0) {
        entry = screen->mons;
        do {
            AprijuiceMon_Free(entry);
            i++;
            entry++;
        } while (i < screen->numMons);
    }
}

static void AprijuiceScreen_DrawButtons(AprijuiceScreen *screen, u32 state) {
    int i;
    switch (state) {
    case 3:
        ClearWindowTilemapAndScheduleTransfer(&screen->windows[1]);
        Sprite_SetDrawFlag(screen->sprites[0], FALSE);
        Sprite_SetDrawFlag(screen->sprites[1], FALSE);
        break;
    case 2:
        FillWindowPixelBuffer(&screen->windows[1], 0);
        for (i = 0; i < 2; i++) {
            ReadMsgDataIntoString(screen->msgData, i + msg_0022_00001, screen->string2);
            AddTextPrinterParameterizedWithColor(&screen->windows[1], 4, screen->string2, 0, 4 + i * 24, TEXT_SPEED_NOTRANSFER, sAprijuiceButtonColors[i], NULL);
            Sprite_SetDrawFlag(screen->sprites[i], TRUE);
        }
        ScheduleWindowCopyToVram(&screen->windows[1]);
        break;
    case 0:
    case 1:
        Sprite_SetAnimCtrlSeq(screen->sprites[state], state * 2 + 1);
        Sprite_ResetAnimCtrlState(screen->sprites[state]);
        break;
    }
}

static int AprijuiceScreen_HandleInput(AprijuiceScreen *screen) {
    int hit = 0;
    int result = AprijuiceScreen_HandleTouch(screen, &hit);
    if (hit == 0) {
        result = AprijuiceScreen_HandleKeys(screen);
    }
    return result;
}

static BOOL AprijuiceScreen_PlayDrinkAnim(AprijuiceScreen *screen) {
    switch (screen->animState) {
    case 0:
        if (Sprite_IsAnimated(screen->sprites[0])) {
            return FALSE;
        }
        AprijuiceScreen_DrawButtons(screen, 3);
        AprijuiceScreen_DrawMessageBox(screen);
        AprijuiceScreen_PrintMessage(screen, msg_0022_00000);
        Sprite_SetDrawFlag(screen->sprites[2], TRUE);
        Sprite_SetAnimCtrlSeq(screen->sprites[2], 10);
        Sprite_ResetAnimCtrlState(screen->sprites[2]);
        PlaySE(SEQ_SE_GS_POT_HYOUJI);
        screen->animState++;
        break;
    case 1:
        if (Sprite_IsAnimated(screen->sprites[2])) {
            return FALSE;
        }
        Sprite_SetAnimCtrlSeq(screen->sprites[2], 11);
        Sprite_ResetAnimCtrlState(screen->sprites[2]);
        screen->animState++;
        break;
    case 2:
        if (Sprite_IsAnimated(screen->sprites[2])) {
            return FALSE;
        }
        AprijuiceScreen_StartShakeTask(screen);
        screen->animState++;
        break;
    case 3:
        if (screen->activeTasks != 0) {
            return FALSE;
        }
        Sprite_SetDrawFlag(screen->sprites[2], FALSE);
        AprijuiceScreen_StartFadeTask(screen);
        screen->animState++;
        break;
    case 4:
        if (screen->activeTasks != 0) {
            return FALSE;
        }
        screen->animState = 6;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 2, screen->heapId);
            screen->animState++;
        }
        break;
    default:
        if (IsPaletteFadeFinished()) {
            screen->animState = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL AprijuiceScreen_PrintNothingChanged(AprijuiceScreen *screen) {
    switch (screen->animState) {
    case 0:
        AprijuiceScreen_DrawMessageBox(screen);
        AprijuiceScreen_PrintMessage(screen, msg_0022_00001);
        screen->animState++;
        break;
    case 1:
        if (!TextPrinterCheckActive(screen->textPrinterId)) {
            screen->animState = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL AprijuiceScreen_ShowStatChange(AprijuiceScreen *screen) {
    u32 stat = sAprijuiceStatOrder[screen->stat];
    switch (screen->animState) {
    case 0: {
        s8 delta;
        screen->timer = 0;
        screen->starsShown = 0;
        delta = screen->starDelta[screen->stat];
        if (delta < 0) {
            screen->starCount = -delta;
            PlaySE(SEQ_SE_GS_PARAMETER_DOWN);
        } else {
            screen->starCount = delta;
            PlaySE(SEQ_SE_GS_PARAMETER_UP);
        }
        screen->animState++;
    }
    // fallthrough
    case 1:
        if (screen->timer-- != 0) {
            break;
        }
        if (screen->starDelta[screen->stat] < 0) {
            AprijuiceScreen_StartStarTask(screen, (u8)stat, 1);
        } else {
            AprijuiceScreen_StartStarTask(screen, (u8)stat, 0);
        }
        if (screen->starsShown >= screen->starCount) {
            screen->animState++;
        } else {
            screen->timer = 15;
        }
        break;
    case 2:
        if (screen->activeTasks != 0) {
            break;
        }
        AprijuiceScreen_DrawStatStars(screen, &screen->mons[screen->selectedMon], (u8)stat);
        AprijuiceScreen_DrawMessageBox(screen);
        if (screen->starDelta[screen->stat] < 0) {
            AprijuiceScreen_PrintMessage(screen, stat + msg_0022_00007);
        } else {
            AprijuiceScreen_PrintMessage(screen, stat + msg_0022_00002);
        }
        screen->animState++;
        break;
    default:
        if (TextPrinterCheckActive(screen->textPrinterId)) {
            break;
        }
        screen->animState = 0;
        return TRUE;
    }
    return FALSE;
}

static int AprijuiceScreen_Select(AprijuiceScreen *screen, int input) {
    switch (input) {
    case 0:
        AprijuiceScreen_UseJuice(screen);
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_DrawButtons(screen, 0);
        return 4;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_DrawButtons(screen, 1);
        return 1;
    case 2:
        screen->selectedMon = (screen->selectedMon - 1 + screen->numMons) % screen->numMons;
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_ShowMon(screen, screen->selectedMon);
        return 2;
    case 3:
        screen->selectedMon = (screen->selectedMon + 1) % screen->numMons;
        PlaySE(SEQ_SE_DP_SELECT);
        AprijuiceScreen_ShowMon(screen, screen->selectedMon);
        return 2;
    default:
        return 2;
    }
}

static int AprijuiceScreen_HandleKeys(AprijuiceScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return AprijuiceScreen_Select(screen, 1);
    }
    if (keys & PAD_BUTTON_A) {
        return AprijuiceScreen_Select(screen, 0);
    }
    if (keys & PAD_KEY_UP) {
        return AprijuiceScreen_Select(screen, 2);
    }
    if (keys & PAD_KEY_DOWN) {
        return AprijuiceScreen_Select(screen, 3);
    }
    return 2;
}

static int AprijuiceScreen_HandleTouch(AprijuiceScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sAprijuiceHitboxes);
    if (input == -1) {
        return 2;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    return AprijuiceScreen_Select(screen, input);
}

static void AprijuiceScreen_VBlankCB(AprijuiceScreen *screen) {
    if (screen->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    AprijuiceScene_TransferPokepicGfx(screen->scene);
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(screen->bgConfig);
    ((vu32 *)0x027E0000)[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

static void AprijuiceScreen_SetGfxBanks(void) {
    GraphicsBanks banks = sAprijuiceGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void AprijuiceScreen_InitBgs(AprijuiceScreen *screen) {
    GraphicsModes modes;
    BgTemplate subBg3;
    BgTemplate mainBg1;
    BgTemplate mainBg2;
    BgTemplate mainBg3;
    AprijuiceScreen_SetGfxBanks();
    screen->bgConfig = BgConfig_Alloc(screen->heapId);
    modes = sAprijuiceGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    subBg3 = sAprijuiceBgTemplate_Sub3;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_SUB_3, &subBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_SUB_3);
    mainBg1 = sAprijuiceBgTemplate_Main1;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_1, &mainBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1);
    mainBg2 = sAprijuiceBgTemplate_Main2;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_2, &mainBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_2);
    mainBg3 = sAprijuiceBgTemplate_Main3;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_3, &mainBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, screen->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, screen->heapId);
}

static void AprijuiceScreen_FreeBgs(AprijuiceScreen *screen) {
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(screen->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

static void AprijuiceScreen_LoadBgGraphics(AprijuiceScreen *screen) {
    NARC *narc = NARC_New(NARC_a_2_4_2, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, screen->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, screen->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, screen->heapId);
    screen->narc = narc;
    LoadUserFrameGfx2(screen->bgConfig, GF_BG_LYR_MAIN_1, 1, 13, screen->frame, screen->heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, screen->heapId);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 6, 26);
}

static void AprijuiceScreen_FreeBgGraphics(AprijuiceScreen *screen) {
    NARC_Delete(screen->narc);
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
}

static void AprijuiceScreen_InitStrings(AprijuiceScreen *screen) {
    int i;
    FontID_Alloc(4, screen->heapId);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0022_bin, screen->heapId);
    screen->msgFormat = MessageFormat_New_Custom(16, 16, screen->heapId);
    screen->string = String_New(256, screen->heapId);
    screen->string2 = String_New(256, screen->heapId);
    screen->levelTemplate = NewString_ReadMsgData(screen->msgData, msg_0022_00004);
    i = 0;
    do {
        screen->genderSymbols[i] = NewString_ReadMsgData(screen->msgData, i + msg_0022_00010);
        i++;
    } while (i < 2);
}

static void AprijuiceScreen_FreeStrings(AprijuiceScreen *screen) {
    String_Delete(screen->genderSymbols[1]);
    String_Delete(screen->genderSymbols[0]);
    String_Delete(screen->levelTemplate);
    String_Delete(screen->string2);
    String_Delete(screen->string);
    MessageFormat_Delete(screen->msgFormat);
    DestroyMsgData(screen->msgData);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    FontID_Release(4);
}

static void AprijuiceScreen_AddWindows(AprijuiceScreen *screen) {
    int i;
    int tile;
    int y;
    for (i = 0; i <= 4; i++) {
        AddWindow(screen->bgConfig, &screen->windows[i], &sAprijuiceWindowTemplates[i]);
        FillWindowPixelBuffer(&screen->windows[i], 0);
    }
    tile = 0xC1;
    i = 0;
    y = 4;
    for (; i < 5; i++) {
        AddWindowParameterized(screen->bgConfig, &screen->windows[i + 5], GF_BG_LYR_MAIN_2, 1, (u8)y, 16, 2, 12, (u16)tile);
        FillWindowPixelBuffer(&screen->windows[i + 5], 0);
        tile += 0x20;
        y += 4;
    }
    screen->yesNoPrompt = YesNoPrompt_Create(screen->heapId);
}

static void AprijuiceScreen_RemoveWindows(AprijuiceScreen *screen) {
    int i;
    Window *window;
    i = 0;
    window = screen->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 10);
    YesNoPrompt_Destroy(screen->yesNoPrompt);
}

static void AprijuiceScreen_InitSpriteSystem(AprijuiceScreen *screen) {
    GF_CreateVramTransferManager(32, screen->heapId);
    screen->spriteSystem = SpriteSystem_Alloc(screen->heapId);
    SpriteSystem_Init(screen->spriteSystem, &sAprijuiceOamManagerParam, &sAprijuiceOamCharTransferParam, 3);
    thunk_ClearMainOAM(screen->heapId);
    thunk_ClearSubOAM(screen->heapId);
    screen->spriteManager = SpriteManager_New(screen->spriteSystem);
    SpriteSystem_InitSprites(screen->spriteSystem, screen->spriteManager, 34);
    SpriteSystem_LoadResourceDataFromFileIdListEx(screen->spriteSystem, screen->spriteManager, &sAprijuiceSpriteResources, 0, 0);
}

static void AprijuiceScreen_FreeSpriteSystem(AprijuiceScreen *screen) {
    SpriteSystem_DestroySpriteManager(screen->spriteSystem, screen->spriteManager);
    screen->spriteManager = NULL;
    SpriteSystem_Free(screen->spriteSystem);
    screen->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(screen->heapId);
}

static void AprijuiceScreen_InitSprites(AprijuiceScreen *screen) {
    AprijuiceScreen_InitSpriteSystem(screen);
    AprijuiceScreen_CreateSprites(screen);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void AprijuiceScreen_FreeSprites(AprijuiceScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    AprijuiceScreen_DeleteSprites(screen);
    AprijuiceScreen_FreeSpriteSystem(screen);
}

static void AprijuiceScreen_CreateSprites(AprijuiceScreen *screen) {
    int i;
    int j;
    const u32 *stat;
    int yIcon;
    int yStars;
    int x;
    u8 *slot;
    u8 *cursor = (u8 *)screen;
    const UnmanagedSpriteTemplate *template = sAprijuiceSpriteTemplates;
    i = 0;
    cursor = (u8 *)screen;
    do {
        ((AprijuiceScreen *)cursor)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, template);
        Sprite_SetDrawFlag(((AprijuiceScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetAnimActiveFlag(((AprijuiceScreen *)cursor)->sprites[0], TRUE);
        Sprite_SetPriority(((AprijuiceScreen *)cursor)->sprites[0], 1);
        template ++;
        cursor += sizeof(Sprite *);
        i++;
    } while (i <= 2);
    Sprite_SetDrawFlag(screen->sprites[2], FALSE);
    Sprite_SetAffineOverwriteMode(screen->sprites[2], 2);
    i = 0;
    stat = sAprijuiceStatOrder;
    yIcon = 40;
    yStars = 56;
    do {
        slot = (u8 *)screen + (u8)(*stat + 3) * sizeof(Sprite *);
        ((AprijuiceScreen *)slot)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sAprijuiceSpriteTemplates[3]);
        Sprite_SetPositionXY(((AprijuiceScreen *)slot)->sprites[0], 80, (s16)yIcon);
        Sprite_SetAnimActiveFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
        Sprite_SetDrawFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
        j = 0;
        x = 64;
        do {
            slot = (u8 *)screen + (u8)(*stat * 5 + 8 + j) * sizeof(Sprite *);
            ((AprijuiceScreen *)slot)->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(screen->spriteSystem, screen->spriteManager, &sAprijuiceSpriteTemplates[4]);
            Sprite_SetPositionXY(((AprijuiceScreen *)slot)->sprites[0], (s16)x, (s16)yStars);
            Sprite_SetAnimActiveFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
            Sprite_SetDrawFlag(((AprijuiceScreen *)slot)->sprites[0], TRUE);
            Sprite_SetAffineOverwriteMode(((AprijuiceScreen *)slot)->sprites[0], 2);
            j++;
            x += 16;
        } while (j < 5);
        stat++;
        yIcon += 32;
        yStars += 32;
        i++;
    } while (i < 5);
}

static void AprijuiceScreen_DeleteSprites(AprijuiceScreen *screen) {
    int i;
    for (i = 0; i < 34; i++) {
        if (screen->sprites[i] != NULL) {
            Sprite_Delete(screen->sprites[i]);
        }
    }
}

static void AprijuiceMon_Init(AprijuiceMon *entry, Party *party, Pokemon *mon, u8 slot, enum HeapID heapId) {
    entry->mon = mon;
    entry->slot = slot;
    entry->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    entry->gender = GetMonData(mon, MON_DATA_GENDER, NULL);
    entry->level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    entry->hFlip = GetMonBaseStat_HandleAlternateForm(entry->species, GetMonData(mon, MON_DATA_FORM, NULL), BASE_FLIP) ^ 1;
    entry->name = String_New(12, heapId);
    GetMonData(mon, MON_DATA_NICKNAME_STRING, entry->name);
    CalcMonPokeathlonPerformance(entry->mon, &entry->performance);
    Party_GetMonAprijuiceModifiers(party, &entry->aprijuice, entry->slot);
}

static void AprijuiceMon_Free(AprijuiceMon *entry) {
    String_Delete(entry->name);
}

static void AprijuiceScreen_UseJuice(AprijuiceScreen *screen) {
    int i;
    AprijuiceMon *entry = &screen->mons[screen->selectedMon];
    ApricornBox_FeedAprijuice(&screen->juiceRecord, screen->party, entry->slot);
    if (screen->app->juiceIndex == 8) {
        ApricornBox_UseJuiceServing(screen->apricornBox);
    }
    Party_GetMonAprijuiceModifiers(screen->party, &entry->aprijuice, entry->slot);
    screen->oldStars.stars = entry->stars.stars;
    *(u16 *)&screen->oldStars.color[0] = *(u16 *)&entry->stars.color[0];
    *(u16 *)&screen->oldStars.color[2] = *(u16 *)&entry->stars.color[2];
    *(u16 *)&screen->oldStars.color[4] = *(u16 *)&entry->stars.color[4];
    CalcMonPokeathlonStars(&entry->stars, entry->mon, (const s8 *)entry->aprijuice.stats, screen->heapId);
    screen->numChanged = 0;
    for (i = 0; i < 5; i++) {
        screen->starDelta[i] = (u8)((entry->stars.stars >> (sAprijuiceStatOrder[i] * 3)) & 7) - (u8)((screen->oldStars.stars >> (sAprijuiceStatOrder[i] * 3)) & 7);
        if (screen->starDelta[i] != 0) {
            screen->numChanged++;
        }
    }
    screen->juiceUsed = 1;
}

static void AprijuiceScreen_DrawMessageBox(AprijuiceScreen *screen) {
    DrawFrameAndWindow2(&screen->windows[0], TRUE, 1, 13);
    FillWindowPixelBuffer(&screen->windows[0], 15);
}

static void AprijuiceScreen_PrintMessage(AprijuiceScreen *screen, int message) {
    u32 speed;
    switch (message) {
    case 0: {
        message = screen->juice.entries[0].stat;
        if (message == 6) {
            message = 12;
        } else if (message == 5) {
            message = screen->juice.flavorOrder[0] + 13;
        } else {
            message += 13;
        }
        speed = 0;
        break;
    }
    case 1:
        message = 18;
        speed = screen->textFrameDelay;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        message += 17;
        speed = screen->textFrameDelay;
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        message += 17;
        speed = screen->textFrameDelay;
        break;
    default:
        speed = screen->textFrameDelay;
        break;
    }
    ReadMsgDataIntoString(screen->msgData, message, screen->string2);
    screen->textPrinterId = AddTextPrinterParameterizedWithColor(&screen->windows[0], 1, screen->string2, 0, 0, speed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    if (speed == 0) {
        ScheduleWindowCopyToVram(&screen->windows[0]);
    }
}

static void AprijuiceScreen_ShowMon(AprijuiceScreen *screen, int index) {
    AprijuiceMon *entry = &screen->mons[index];
    FillWindowPixelBuffer(&screen->windows[4], 0);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, entry->name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    if (entry->gender != MON_GENDERLESS) {
        int gender = entry->gender;
        AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, screen->genderSymbols[gender], 64, 0, TEXT_SPEED_NOTRANSFER, sAprijuiceGenderColors[gender], NULL);
    }
    BufferIntegerAsString(screen->msgFormat, 0, entry->level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(screen->msgFormat, screen->string2, screen->levelTemplate);
    AddTextPrinterParameterizedWithColor(&screen->windows[4], 0, screen->string2, 16, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&screen->windows[4]);
    AprijuiceScene_DeletePokepic(screen->scene);
    AprijuiceScene_CreatePokepic(screen->scene, entry);
    AprijuiceScreen_DrawAllStars(screen, entry);
}

static void AprijuiceScreen_DrawStatStars(AprijuiceScreen *screen, AprijuiceMon *entry, u8 stat) {
    int i;
    int index;
    u8 level;

    struct {
        const u8 *bytes;
        int offset;
    } bar;

    if ((s8)entry->aprijuice.stats[stat] == 0) {
        Sprite_SetDrawFlag(screen->sprites[stat + 3], FALSE);
    } else {
        Sprite_SetDrawFlag(screen->sprites[stat + 3], TRUE);
        if ((s8)entry->aprijuice.stats[stat] > 0) {
            Sprite_SetAnimCtrlSeq(screen->sprites[stat + 3], 8);
        } else {
            Sprite_SetAnimCtrlSeq(screen->sprites[stat + 3], 9);
        }
    }
    index = stat * 5;
    bar.bytes = (const u8 *)entry + offsetof(AprijuiceMon, performance);
    bar.offset = stat * sizeof(entry->performance.stats[0]);
    level = (u8)((entry->stars.stars >> (stat * 3)) & 7);
    for (i = 0; i < 5; i++, index++) {
        if (i > (s32)(((const struct TodayPerformanceStat *)(bar.bytes + bar.offset))->hi)) {
            AprijuiceScreen_SetStarSprite(screen, index, 0xFF, 0);
        } else if (i > level) {
            AprijuiceScreen_SetStarSprite(screen, index, 7, 1);
        } else {
            AprijuiceScreen_SetStarSprite(screen, index, (u8)(entry->stars.color[stat] + 4), 1);
        }
    }
}

static void AprijuiceScreen_DrawAllStars(AprijuiceScreen *screen, AprijuiceMon *entry) {
    s32 i;
    CalcMonPokeathlonStars(&entry->stars, entry->mon, (const s8 *)entry->aprijuice.stats, screen->heapId);
    for (i = 0; i < 5; i++) {
        AprijuiceScreen_DrawStatStars(screen, entry, (u8)i);
    }
}

static void AprijuiceScreen_SetStarSprite(AprijuiceScreen *screen, int index, u8 sequence, int draw) {
    u32 offset;
    Sprite **sprites;
    index += 8;
    offset = index * sizeof(Sprite *);
    sprites = screen->sprites;
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)sprites + offset), draw);
    if (sequence != 0xFF) {
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)sprites + offset), sequence);
        Sprite_ResetAnimCtrlState(*(Sprite **)((u8 *)sprites + offset));
    }
}

static void AprijuiceScreen_StartStarTask(AprijuiceScreen *screen, int stat, int shrink) {
    AprijuiceStarTask *star = Heap_Alloc(screen->heapId, sizeof(AprijuiceStarTask));
    u8 level;
    u8 index;
    f32 frames;
    MI_CpuFill8(star, 0, sizeof(AprijuiceStarTask));
    star->owner = screen;
    star->hide = shrink;
    star->timer = 30;
    level = (screen->oldStars.stars >> (stat * 3)) & 7;
    if (shrink == 0) {
        index = level + screen->starsShown + 1;
        star->scale.x = 0x19A;
        star->scale.y = 0x19A;
        star->scale.z = 0;
        if (star->timer != 0) {
            frames = (f32)(star->timer << FX32_SHIFT) + 0.5f;
        } else {
            frames = (f32)(star->timer << FX32_SHIFT) - 0.5f;
        }
        star->scaleStep = FX_Div(FX32_ONE, (fx32)frames);
        star->angleStep = 12;
        star->angle = 0;
    } else {
        index = level - screen->starsShown;
        star->scale.x = 0x119A;
        star->scale.y = 0x119A;
        star->scale.z = FX32_ONE;
        if (star->timer != 0) {
            frames = (f32)(star->timer << FX32_SHIFT) + 0.5f;
        } else {
            frames = (f32)(star->timer << FX32_SHIFT) - 0.5f;
        }
        star->scaleStep = -FX_Div(FX32_ONE, (fx32)frames);
        star->angleStep -= 12;
        star->angle = 360;
    }
    index += stat * 5;
    star->sprite = screen->sprites[index + 8];
    Sprite_SetAffineScale(star->sprite, &star->scale);
    AprijuiceScreen_SetStarSprite(screen, index, screen->oldStars.color[stat] + 4, 1);
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_StarTask, star, 0);
    screen->activeTasks++;
    screen->starsShown++;
}

static void AprijuiceScreen_StarTask(SysTask *task, AprijuiceStarTask *star) {
    AprijuiceScreen *owner = star->owner;
    star->angle += star->angleStep;
    star->scale.x += star->scaleStep;
    star->scale.y += star->scaleStep;
    star->timer--;
    if (star->timer != 0) {
        Sprite_SetAffineZRotation(star->sprite, GF_DegreeToSinCosIdx(star->angle));
        Sprite_SetAffineScale(star->sprite, &star->scale);
        return;
    }
    Sprite_SetAffineZRotation(star->sprite, 0);
    star->scale.x = FX32_ONE;
    star->scale.y = FX32_ONE;
    star->scale.z = FX32_ONE;
    Sprite_SetAffineScale(star->sprite, &star->scale);
    if (star->hide != 0) {
        Sprite_SetDrawFlag(star->sprite, FALSE);
    }
    owner->activeTasks--;
    Heap_Free(star);
    SysTask_Destroy(task);
}

static void AprijuiceScreen_StartShakeTask(AprijuiceScreen *screen) {
    AprijuiceShakeTask *shake = Heap_Alloc(screen->heapId, sizeof(AprijuiceShakeTask));
    MI_CpuFill8(shake, 0, sizeof(AprijuiceShakeTask));
    shake->owner = screen;
    shake->counter = 30;
    shake->delta = 12;
    PlaySE(SEQ_SE_GS_JUICE_SIYOU);
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_ShakeTask, shake, 0);
    screen->activeTasks++;
}

static void AprijuiceScreen_ShakeTask(SysTask *task, AprijuiceShakeTask *shake) {
    SysTask *savedTask = task;
    AprijuiceScreen *owner = shake->owner;
    if ((shake->phase % 2) == 0) {
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEW, shake->delta);
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEH, -shake->delta);
    } else {
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEW, -shake->delta);
        Pokepic_AddAttr(owner->scene->pokepic, POKEPIC_AFFINEH, shake->delta);
    }
    if (shake->counter++ >= 8) {
        shake->counter = 0;
        if (shake->phase++ >= 4) {
            Pokepic_SetAttr(owner->scene->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(owner->scene->pokepic, POKEPIC_AFFINEH, 0x100);
            owner->activeTasks--;
            Heap_Free(shake);
            SysTask_Destroy(savedTask);
        }
    }
}

static void AprijuiceScreen_StartFadeTask(AprijuiceScreen *screen) {
    AprijuiceFadeTask *fade = Heap_Alloc(screen->heapId, sizeof(AprijuiceFadeTask));
    MI_CpuFill8(fade, 0, sizeof(AprijuiceFadeTask));
    fade->owner = screen;
    SysTask_CreateOnMainQueue((SysTaskFunc)AprijuiceScreen_FadeTask, fade, 0);
    screen->activeTasks++;
}

static void AprijuiceScreen_FadeTask(SysTask *task, AprijuiceFadeTask *fade) {
    AprijuiceScreen *owner = fade->owner;
    switch (fade->state) {
    case 0:
        AprijuiceScene_StartParticles(owner->scene, 2);
        PlaySE(SEQ_SE_GS_KOUKAEFFECT);
        fade->timer = 20;
        fade->state++;
        return;
    case 1:
        if (fade->timer-- == 0) {
            Pokepic_StartPaletteFade(owner->scene->pokepic, 0, 16, 4, RGB_WHITE);
            AprijuiceScene_StartParticles(owner->scene, 1);
            AprijuiceScene_StartParticles(owner->scene, 0);
            fade->timer = 25;
            fade->state++;
            return;
        }
        break;
    case 2:
        if (fade->timer-- == 0) {
            PlaySE(SEQ_SE_GS_KIRAKIRA);
            fade->state++;
            return;
        }
        break;
    case 3:
        if (!Pokepic_ResumePaletteFade(owner->scene->pokepic) && !AprijuiceScene_AreParticlesActive(owner->scene)) {
            ParticleSystem_DeleteAllEmitters(owner->scene->emitter);
            fade->state++;
            return;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 6, owner->heapId);
        Pokepic_StartPaletteFade(owner->scene->pokepic, 16, 0, 4, RGB_WHITE);
        fade->state++;
        return;
    default:
        if (IsPaletteFadeFinished() && !Pokepic_ResumePaletteFade(owner->scene->pokepic)) {
            owner->activeTasks--;
            Heap_Free(fade);
            SysTask_Destroy(task);
        }
        break;
    }
}

static AprijuiceScene *AprijuiceScene_New(enum HeapID heapId) {
    AprijuiceScene *scene = Heap_Alloc(heapId, sizeof(AprijuiceScene));
    MI_CpuFill8(scene, 0, sizeof(AprijuiceScene));
    scene->heapId = heapId;
    scene->vramMan = GF_3DVramMan_Create(heapId, 0, 2, 0, 1, AprijuiceScene_Init3D);
    AprijuiceScene_ReserveVram(scene->vramMan);
    AprijuiceScene_InitCamera(scene, heapId);
    AprijuiceScene_InitParticles(scene);
    scene->pokepicManager = PokepicManager_Create(heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1;
    return scene;
}

static void AprijuiceScene_Draw(AprijuiceScene *scene) {
    Thunk_G3X_Reset();
    ParticleSystem_DrawAll();
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    Camera_PushLookAtToNNSGlb();
    reg_G3_MTX_MODE = GX_MTXMODE_PROJECTION;
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_MODE = GX_MTXMODE_POSITION_VECTOR;
    reg_G3_MTX_IDENTITY = 0;
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(scene->pokepicManager);
    ParticleSystem_CalcAll();
    reg_G3_SWAP_BUFFERS = 1;
}

static void AprijuiceScene_Delete(AprijuiceScene *scene) {
    Camera_Delete(scene->camera);
    PokepicManager_Delete(scene->pokepicManager);
    AprijuiceScene_FreeParticles(scene);
    GF_3DVramMan_Delete(scene->vramMan);
    Heap_Free(scene);
}

static void AprijuiceScene_TransferPokepicGfx(AprijuiceScene *scene) {
    PokepicManager_HandleLoadImgAndOrPltt(scene->pokepicManager);
}

static void AprijuiceScene_CreatePokepic(AprijuiceScene *scene, AprijuiceMon *entry) {
    PokepicTemplate template;
    GetPokemonSpriteCharAndPlttNarcIds(&template, entry->mon, 2);
    scene->pokepic = PokepicManager_CreatePokepic(scene->pokepicManager, &template, 204, 112, 0, 0, NULL, NULL);
}

static void AprijuiceScene_DeletePokepic(AprijuiceScene *scene) {
    if (scene->pokepic) {
        Pokepic_Delete(scene->pokepic);
        scene->pokepic = NULL;
    }
}

static void AprijuiceScene_StartParticles(AprijuiceScene *scene, int resource) {
    ParticleSystem_CreateEmitterWithParam(scene->emitter, resource, AprijuiceScene_SetEmitterPosition, scene);
    ParticleSystem_SetPerspectiveType(scene->emitter, 1);
}

static BOOL AprijuiceScene_AreParticlesActive(AprijuiceScene *scene) {
    return ParticleSystem_GetActiveEmitterCount(scene->emitter) != 0;
}

static void AprijuiceScene_Init3D(void) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xFFFFCFFD;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xCFFB;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & 0xFFFFCFFF) | 8;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & 0xFFFFCFFF) | 16;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xCFDF;
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 63, FALSE);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
}

static void AprijuiceScene_ReserveVram(GF3DVramMan *vramMan) { // vramMan is unused
    u32 tex = NNS_GfdDefaultFuncAllocTexVram(0x8000, FALSE, 0);
    u32 pltt = NNS_GfdDefaultFuncAllocPlttVram(0xA0, FALSE, 0);
    if (tex == 0) {
        GF_AssertFail();
    }
    if (pltt == 0) {
        GF_AssertFail();
    }
    ParticleSystem_ZeroAll();
}

static u32 AprijuiceScene_AllocTexVram(u32 size, BOOL is4x4) {
    u32 key = NNS_GfdDefaultFuncAllocTexVram(size, is4x4, 0);
    ParticleSystem_RegisterTexKey(key);
    return (key << 16) >> 13;
}

static u32 AprijuiceScene_AllocPlttVram(u32 size, BOOL is4color) {
    u32 key = NNS_GfdDefaultFuncAllocPlttVram(size, is4color, 0);
    ParticleSystem_RegisterPlttKey(key);
    return (key << 16) >> 13;
}

static void AprijuiceScene_SetEmitterPosition(SPLEmitter *emitter) {
    VecFx32 offset = { 0, 0, 0 };
    offset.x = 0x3310;
    offset.y = -0x1020;
    offset.z = -0x1AE0;
    emitter->emtr_pos.x = offset.x + emitter->p_res->p_base->pos.x;
    emitter->emtr_pos.y = offset.y + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = offset.z + emitter->p_res->p_base->pos.z;
}

static SPLEmitter *AprijuiceScene_CreateParticleSystem(enum HeapID heapId) {
    void *heap = Heap_Alloc(heapId, 0x4800);
    SPLEmitter *emitter = ParticleSystem_New(AprijuiceScene_AllocTexVram, AprijuiceScene_AllocPlttVram, heap, 0x4800, TRUE, heapId);
    Camera *camera = ParticleSystem_GetCamera(emitter);
    if (camera != NULL) {
        Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, camera);
    }
    return emitter;
}

static void AprijuiceScene_InitParticles(AprijuiceScene *scene) {
    void *resource;
    scene->emitter = AprijuiceScene_CreateParticleSystem(scene->heapId);
    ParticleSystem_SetPerspectiveType(scene->emitter, 1);
    resource = ParticleSystem_LoadResourceFile(NARC_a_2_4_2, 10, scene->heapId);
    ParticleSystem_SetResource(scene->emitter, resource, 10, TRUE);
}

static void AprijuiceScene_FreeParticles(AprijuiceScene *scene) {
    ParticleSystem_Free(scene->emitter);
    Heap_Free(scene->emitter);
}
