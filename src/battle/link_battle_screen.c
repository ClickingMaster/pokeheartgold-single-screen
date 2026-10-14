#include "battle/link_battle_screen.h"

#include "global.h"

#include "constants/gx.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0300.h"
#include "msgdata/msg/msg_0638.h"

#include "bag.h"
#include "battle_record.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "list_menu_2d.h"
#include "math_util.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "particle_system.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "message_printer_number.h"
#include "swap_3d_buffers.h"
#include "battle_recording.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "wireless_status_icon.h"
#include "ui_util.h"
#include "pokemon_summary.h"
#include "vram_transfer_manager.h"

#define LINK_BATTLE_SCREEN_PARTY_WINDOW_COUNT 24
#define LINK_BATTLE_SCREEN_SPRITES_PER_MON    5

// One party mon as shown on the party preview panels.
typedef struct LinkBattleScreenMonInfo {
    u32 iconNaix;          // 0x00
    u16 species;           // 0x04
    u16 hp;                // 0x06
    u16 maxHp;             // 0x08
    u16 heldItem;          // 0x0A
    u8 level;              // 0x0C
    u8 gender;             // 0x0D
    u8 unk_0E;             // 0x0E, 0 when MON_DATA_NO_PRINT_GENDER is 1
    u8 isEgg;              // 0x0F
    u8 ballCapsule;        // 0x10, MON_DATA_BALL_CAPSULE_ID
    u8 form;               // 0x11
    u8 unk_12[2];          // 0x12
    u32 statusIcon;        // 0x14
} LinkBattleScreenMonInfo; // size: 0x18

typedef struct LinkBattleScreen LinkBattleScreen;

// Returns 0 to run again next frame, 1 to advance to the next step next frame,
// 2 to advance and run the next step immediately.
typedef int (*LinkBattleScreenStepFunc)(LinkBattleScreen *screen);

struct LinkBattleScreen {
    LinkBattleScreenArgs *args;                                     // 0x000
    const LinkBattleScreenStepFunc *sequence;                       // 0x004
    PaletteData *paletteData;                                       // 0x008
    BgConfig *bgConfig;                                             // 0x00C
    Window partyWindows[LINK_BATTLE_SCREEN_PARTY_WINDOW_COUNT];     // 0x010, name/level/HP/HP bar of each mon
    SpriteSystem *spriteSystem;                                     // 0x190
    SpriteManager *spriteManager;                                   // 0x194
    ManagedSprite *sprites[6 * LINK_BATTLE_SCREEN_SPRITES_PER_MON]; // 0x198
    u32 spriteCount;                                                // 0x210
    LinkBattleScreenMonInfo monInfo[PARTY_SIZE];                    // 0x214
    u16 panelTilemaps[3][16 * 6];                                   // 0x2A4, lead / occupied / empty panel
    u16 panelPalettes[3][16];                                       // 0x4E4
    u16 stashedBg1Tiles[24][16];                                    // 0x544
    u16 stashedBg2Tiles[24][16];                                    // 0x844
    FontSystem *fontSystem;                                         // 0xB44
    TextOBJ *nameTextObjs[4];                                       // 0xB48
    GF3DVramMan *gfx3DVramMan;                                      // 0xB58
    SPLEmitter *particleSystem;                                     // 0xB5C
    void *particleHeap;                                             // 0xB60
    u8 ballRowX[4];                                                 // 0xB64
    u8 ballRowY[4];                                                 // 0xB68
    u8 nameX[4];                                                    // 0xB6C
    u8 nameY[4];                                                    // 0xB70
    int slideSpeed;                                                 // 0xB74
    s16 shakeX;                                                     // 0xB78
    s16 shakeY;                                                     // 0xB7A
    s8 shakeRangeX;                                                 // 0xB7C
    s8 shakeRangeY;                                                 // 0xB7D
    u8 nudgeDirection;                                              // 0xB7E
    u8 taskState;                                                   // 0xB7F
    u8 sequenceIndex;                                               // 0xB80
    u8 delayFrames;                                                 // 0xB81
    u8 frame;                                                       // 0xB82
    u8 unk_B83;                                                     // 0xB83
    u16 recordSaveState;                                                    // 0xB84, battle record save state
    u16 recordWriteState;                                                    // 0xB86, battle record save state
    Window msgWindow;                                               // 0xB88
    Window yesNoWindow;                                             // 0xB98
    BOOL use3D;                                                     // 0xBA8
    MsgData *msgData;                                               // 0xBAC
    MessageFormat *msgFormat;                                       // 0xBB0
    String *string;                                                 // 0xBB4
    u32 printerId;                                                  // 0xBB8
    int videoPromptState;                                           // 0xBBC
    struct ListMenu2D *yesNoMenu;                                   // 0xBC0
    WaitingIcon *waitingIcon;                                       // 0xBC4
    BOOL canSaveVideo;                                              // 0xBC8
    int savedVideoCheck;                                            // 0xBCC
    u32 unk_BD0;                                                    // 0xBD0, set when a linked player's version is 0
}; // size: 0xBD4

// The 7 words a sprite of this screen is made from (see LinkBattleScreen_CreateSprite).
typedef struct LinkBattleScreenSpriteTemplate {
    int charResId;
    int plttResId;
    int cellResId;
    int animResId;
    int bgPriority;
    int drawPriority;
    int pal;
} LinkBattleScreenSpriteTemplate;

typedef struct LinkBattleScreenBallRows {
    u8 firstSprite[4];
} LinkBattleScreenBallRows;

extern const LinkBattleScreenBallRows sBallRowFirstSprite;
extern const WindowTemplate sBattleVideoYesNoTemplate;
extern const u8 sPartyPanelPositions[6][2];
extern const u8 sPanelNudgeSteps[];
extern const u8 ov05_0221EA6D[]; // second byte of each sPanelNudgeSteps pair
extern const GraphicsModes sVsGraphicsModes;
extern const GraphicsModes sPartyGraphicsModes;
extern const LinkBattleScreenStepFunc sVsSequence[];
extern const OamCharTransferParam sLinkBattleScreenOamTransferParam;
extern const LinkBattleScreenStepFunc sPartySequence[];
extern const SpriteResourceCountsListUnion sResultSpriteResourceCounts;
extern const SpriteResourceCountsListUnion sPartySpriteResourceCounts;
extern const SpriteResourceCountsListUnion sVsSpriteResourceCounts;
extern const BgTemplate sVsBg3Template;
extern const BgTemplate sPartyBg2Template;
extern const BgTemplate sVsBg2Template;
extern const BgTemplate sVsBg1Template;
extern const BgTemplate sPartyBg1Template;
extern const BgTemplate sVsBg0Template;
extern const BgTemplate sPartyBg3Template;
extern const OamManagerParam sLinkBattleScreenOamParam;
extern const GraphicsBanks sVramBanks;
extern const LinkBattleScreenStepFunc sResultDrawSequence[];
extern const u8 sMonSpritePositions[PARTY_SIZE][LINK_BATTLE_SCREEN_SPRITES_PER_MON][2];
extern const LinkBattleScreenStepFunc sResultSequence[];
extern const WindowTemplate sPartyWindowTemplates[LINK_BATTLE_SCREEN_PARTY_WINDOW_COUNT];
extern const LinkBattleScreenSpriteTemplate sVsSpriteTemplates[];
extern const LinkBattleScreenSpriteTemplate sPartySpriteTemplates[];

void LinkBattleScreen_RunTask(SysTask *task, void *data);
void LinkBattleScreen_SetVramBanks(void);
void LinkBattleScreen_UpdateBattleVideoPrompt(LinkBattleScreen *screen);
int LinkBattleScreen_Init(LinkBattleScreen *screen);
int LinkBattleScreen_InitPartyScreen(LinkBattleScreen *screen);
int LinkBattleScreen_InitVsScreen(LinkBattleScreen *screen);
int LinkBattleScreen_InitResultScreen(LinkBattleScreen *screen);
int LinkBattleScreen_WaitFadeAndDelay(LinkBattleScreen *screen);
int LinkBattleScreen_SlidePanelsIn(LinkBattleScreen *screen);
int LinkBattleScreen_ShakePanels(LinkBattleScreen *screen);
int LinkBattleScreen_SlidePanelsOut(LinkBattleScreen *screen);
int LinkBattleScreen_DelayBeforeNudge(LinkBattleScreen *screen);
int LinkBattleScreen_DetermineNudgeDirection(LinkBattleScreen *screen);
int LinkBattleScreen_NudgePanels(LinkBattleScreen *screen);
int LinkBattleScreen_NudgePanelsRight(LinkBattleScreen *screen);
int LinkBattleScreen_NudgePanelsLeft(LinkBattleScreen *screen);
int LinkBattleScreen_DelayBeforeReveal(LinkBattleScreen *screen);
int LinkBattleScreen_RevealCenterBand(LinkBattleScreen *screen);
int LinkBattleScreen_ShowWinLoseSprites(LinkBattleScreen *screen);
int LinkBattleScreen_ShowDrawSprite(LinkBattleScreen *screen);
int LinkBattleScreen_SaveBattleVideo(LinkBattleScreen *screen);
int LinkBattleScreen_WaitForPartner(LinkBattleScreen *screen);
int LinkBattleScreen_ApplyOutcomePalette(LinkBattleScreen *screen);
int LinkBattleScreen_SlidePartyPanelsIn(LinkBattleScreen *screen);
int LinkBattleScreen_StartExit(LinkBattleScreen *screen);
int LinkBattleScreen_Finish(SysTask *task, LinkBattleScreen *screen);
BOOL LinkBattleScreen_IsLocalPlayerOnLeft(void);
void LinkBattleScreen_InitSpriteSystem(LinkBattleScreen *screen, SpriteResourceCountsListUnion *resourceCounts, u32 count);
ManagedSprite *LinkBattleScreen_CreateSprite(LinkBattleScreen *screen, const LinkBattleScreenSpriteTemplate *template);
void LinkBattleScreen_FreeSprites(LinkBattleScreen *screen);
void LinkBattleScreen_UpdateSpriteAnims(LinkBattleScreen *screen);
void LinkBattleScreen_VBlankCallback(void *data);
void LinkBattleScreen_InitVsBgLayers(LinkBattleScreen *screen, BOOL use3D, BOOL shiftBg1);
void LinkBattleScreen_RemoveMessageWindows(LinkBattleScreen *screen);
void LinkBattleScreen_FreeMessageData(LinkBattleScreen *screen);
void LinkBattleScreen_FreeVsBgLayers(LinkBattleScreen *screen);
void LinkBattleScreen_LoadVsBgGraphics(LinkBattleScreen *screen);
void LinkBattleScreen_InitMessageWindows(LinkBattleScreen *screen);
void LinkBattleScreen_SetBg1AlphaBlend(void);
void LinkBattleScreen_InitVsSprites(LinkBattleScreen *screen);
void LinkBattleScreen_LoadPartyBallGfx(LinkBattleScreen *screen);
void LinkBattleScreen_SetBallRowPosition(LinkBattleScreen *screen, u8 row, s16 x, s16 y);
void LinkBattleScreen_SetPartyBallAnims(LinkBattleScreen *screen, Party *party, int count, int firstSprite);
void LinkBattleScreen_SetAllPartyBallAnims(LinkBattleScreen *screen);
void LinkBattleScreen_CreateNameTextObj(LinkBattleScreen *screen, int index, String *name);
void LinkBattleScreen_CreateNameTextObjs(LinkBattleScreen *screen);
void LinkBattleScreen_SetNamePosition(LinkBattleScreen *screen, int index, int x, int y);
void LinkBattleScreen_FreeNameTextObjs(LinkBattleScreen *screen);
void LinkBattleScreen_InitLayout(LinkBattleScreen *screen, int lowerOffsetY, int upperOffsetY);
void LinkBattleScreen_InitResultSprites(LinkBattleScreen *screen);
void LinkBattleScreen_LoadOutcomeGfx(LinkBattleScreen *screen);
void LinkBattleScreen_SetupOutcomeSprites(LinkBattleScreen *screen);
void LinkBattleScreen_InitParticles(LinkBattleScreen *screen);
BOOL LinkBattleScreen_UpdateParticles(LinkBattleScreen *screen);
void LinkBattleScreen_FreeParticles(LinkBattleScreen *screen);
u32 LinkBattleScreen_AllocParticleTexVram(u32 size, BOOL is4x4);
u32 LinkBattleScreen_AllocParticlePlttVram(u32 size, BOOL is4pltt);
void LinkBattleScreen_InitPartyBgLayers(LinkBattleScreen *screen);
void LinkBattleScreen_FreePartyBgLayers(LinkBattleScreen *screen);
void LinkBattleScreen_LoadPartyBgGraphics(LinkBattleScreen *screen);
void LinkBattleScreen_DrawPartyPanels(LinkBattleScreen *screen);
void LinkBattleScreen_StashPanelTilemap(LinkBattleScreen *screen, u16 *dest, u8 bgId);
void LinkBattleScreen_StashPanelTilemaps(LinkBattleScreen *screen);
void LinkBattleScreen_RevealPanelColumns(LinkBattleScreen *screen, u8 width);
void LinkBattleScreen_ReadPartyMonInfo(LinkBattleScreen *screen, Party *party, int firstIndex);
void LinkBattleScreen_ReadMonInfo(LinkBattleScreen *screen);
u32 LinkBattleScreen_GetMonIconAnim(LinkBattleScreen *screen, u32 index);
void LinkBattleScreen_CreateMonSprites(LinkBattleScreen *screen, u32 index);
void LinkBattleScreen_InitPartySprites(LinkBattleScreen *screen);
void LinkBattleScreen_LoadMonIconGfx(LinkBattleScreen *screen);
void LinkBattleScreen_LoadItemIconGfx(LinkBattleScreen *screen);
void LinkBattleScreen_LoadStatusIconGfx(LinkBattleScreen *screen);
void LinkBattleScreen_LoadPlistSpriteRes(LinkBattleScreen *screen);
void LinkBattleScreen_SetPartySpriteOffset(LinkBattleScreen *screen, int shift);
void LinkBattleScreen_AddPartyWindows(LinkBattleScreen *screen);
void LinkBattleScreen_RemovePartyWindows(LinkBattleScreen *screen);
void LinkBattleScreen_PrintMonName(LinkBattleScreen *screen, MsgData *msgData, MessagePrinter *unused, MessageFormat *msgFormat, String *string, u32 index);
void LinkBattleScreen_PrintMonLevel(LinkBattleScreen *screen, MsgData *unused, MessagePrinter *printer, MessageFormat *unused2, String *unused3, u32 index);
void LinkBattleScreen_PrintMonHp(LinkBattleScreen *screen, MsgData *unused, MessagePrinter *printer, MessageFormat *unused2, String *unused3, u32 index);
void LinkBattleScreen_DrawMonHpBar(LinkBattleScreen *screen, int index);
void LinkBattleScreen_DrawMonPanelText(LinkBattleScreen *screen, MsgData *msgData, MessagePrinter *printer, MessageFormat *msgFormat, String *string, u32 index);
void LinkBattleScreen_DrawAllMonPanelText(LinkBattleScreen *screen);
void LinkBattleScreen_DrawMessageBox(LinkBattleScreen *screen);
BOOL LinkBattleScreen_IsBattleRecordActive(LinkBattleScreen *screen);
void LinkBattleScreen_ShowWaitingIcon(LinkBattleScreen *screen);
void LinkBattleScreen_HideWaitingIcon(LinkBattleScreen *screen);

void LinkBattleScreen_Start(LinkBattleScreenArgs *args) {
    SysTask *task = CreateSysTaskAndEnvironment(LinkBattleScreen_RunTask, sizeof(LinkBattleScreen), 100, args->heapId);
    LinkBattleScreen *screen = SysTask_GetData(task);
    memset(screen, 0, sizeof(LinkBattleScreen));
    screen->args = args;
    screen->taskState = 0;
    screen->canSaveVideo = BattleRecord_AreAiFlagsValid();
    if (screen->canSaveVideo == TRUE) {
        BattleSetup *setup = screen->args->setup;
        if (setup != NULL && setup->saveData != NULL) {
            Bag *bag = Save_Bag_Get(setup->saveData);
            screen->canSaveVideo = Bag_HasItem(bag, ITEM_VS__RECORDER, 1, args->heapId);
        }
    }
}

void LinkBattleScreen_RunTask(SysTask *task, void *data) {
    LinkBattleScreen *screen = data;
    int result;
    switch (screen->taskState) {
    case 0:
        screen->taskState = LinkBattleScreen_Init(screen);
        break;
    case 1:
        do {
            result = screen->sequence[screen->sequenceIndex](screen);
            if (result != 0) {
                screen->sequenceIndex++;
            }
        } while (result == 2);
        if (screen->args->mode == LINK_BATTLE_SCREEN_MODE_RESULT) {
            LinkBattleScreen_UpdateBattleVideoPrompt(screen);
        }
        break;
    case 2:
        if (LinkBattleScreen_Finish(task, data) == 1) {
            return;
        }
        break;
    }
    if (screen->sequenceIndex != 0) {
        LinkBattleScreen_UpdateSpriteAnims(screen);
        SpriteSystem_DrawSprites(screen->spriteManager);
    }
    if (screen->args->mode != LINK_BATTLE_SCREEN_MODE_PARTY) {
        G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    }
}

void LinkBattleScreen_SetVramBanks(void) {
    GraphicsBanks banks = sVramBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (u32 *)HW_BG_VRAM, 0x80000);
}

#pragma force_active on

void LinkBattleScreen_UpdateBattleVideoPrompt(LinkBattleScreen *screen) {
    if (!LinkBattleScreen_IsBattleRecordActive(screen)) {
        return;
    }
    switch (screen->videoPromptState) {
    case 0:
        break;
    case 1:
        LinkBattleScreen_DrawMessageBox(screen);
        ReadMsgDataIntoString(screen->msgData, msg_0638_00000, screen->string);
        screen->printerId = AddTextPrinterParameterized(&screen->msgWindow, 1, screen->string, 0, 0, 0, NULL);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
        screen->videoPromptState = 2;
        break;
    case 2:
        if (screen->canSaveVideo == TRUE && (gSystem.newKeys & PAD_BUTTON_A)) {
            screen->videoPromptState = 3;
        }
        break;
    case 3:
        if (screen->savedVideoCheck == 1) {
            ReadMsgDataIntoString(screen->msgData, msg_0638_00002, screen->string);
        } else {
            ReadMsgDataIntoString(screen->msgData, msg_0638_00001, screen->string);
        }
        FillWindowPixelRect(&screen->yesNoWindow, 15, 0, 0, 216, 32);
        DrawFrameAndWindow2(&screen->yesNoWindow, FALSE, 1, 15);
        screen->printerId = AddTextPrinterParameterized(&screen->yesNoWindow, 1, screen->string, 0, 0, 0, NULL);
        screen->yesNoMenu = CreateYesNoMenu(screen->bgConfig, &sBattleVideoYesNoTemplate, 31, 14, 1, screen->args->heapId);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
        screen->videoPromptState = 4;
        break;
    case 4:
        switch ((u32)Handle2dMenuInput_DeleteOnFinish(screen->yesNoMenu, screen->args->heapId)) {
        case 0:
            screen->videoPromptState = 5;
            screen->yesNoMenu = NULL;
            break;
        case -2:
            screen->videoPromptState = 1;
            screen->yesNoMenu = NULL;
            break;
        }
        break;
    case 5:
        LinkBattleScreen_DrawMessageBox(screen);
        ReadMsgDataIntoString(screen->msgData, msg_0638_00005, screen->string);
        screen->printerId = AddTextPrinterParameterized(&screen->msgWindow, 1, screen->string, 0, 0, 0, NULL);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
        screen->videoPromptState = 6;
        break;
    case 6:
        break;
    case 7:
        if (screen->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(screen->yesNoMenu, screen->args->heapId);
            screen->yesNoMenu = NULL;
        }
        ClearFrameAndWindow2(&screen->msgWindow, FALSE);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
        screen->videoPromptState = 8;
        break;
    case 8:
        break;
    }
}

#pragma force_active reset

int LinkBattleScreen_Init(LinkBattleScreen *screen) {
    reg_G2_BLDCNT = 0;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_POWCNT |= 0x8000;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    LinkBattleScreen_SetVramBanks();
    screen->bgConfig = BgConfig_Alloc(screen->args->heapId);
    screen->paletteData = PaletteData_Init(screen->args->heapId);
    PaletteData_AllocBuffers(screen->paletteData, PLTTBUF_MAIN_BG, 0x200, screen->args->heapId);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    screen->frame = 0;
    screen->delayFrames = 8;
    screen->sequenceIndex = 0;
    switch (screen->args->mode) {
    case LINK_BATTLE_SCREEN_MODE_PARTY:
        screen->sequence = sPartySequence;
        break;
    case LINK_BATTLE_SCREEN_MODE_VS:
        screen->sequence = sVsSequence;
        break;
    case LINK_BATTLE_SCREEN_MODE_RESULT:
        if (screen->args->outcome == BATTLE_OUTCOME_DRAW) {
            screen->sequence = sResultDrawSequence;
        } else {
            screen->sequence = sResultSequence;
        }
        break;
    }
    return 1;
}

int LinkBattleScreen_InitPartyScreen(LinkBattleScreen *screen) {
    switch (screen->frame) {
    case 0:
        LinkBattleScreen_InitPartyBgLayers(screen);
        LinkBattleScreen_LoadPartyBgGraphics(screen);
        break;
    case 1:
        LinkBattleScreen_ReadMonInfo(screen);
        LinkBattleScreen_InitPartySprites(screen);
        break;
    case 2:
        screen->frame = 0;
        screen->delayFrames = 32;
        LinkBattleScreen_DrawPartyPanels(screen);
        LinkBattleScreen_AddPartyWindows(screen);
        LinkBattleScreen_DrawAllMonPanelText(screen);
        LinkBattleScreen_StashPanelTilemaps(screen);
        WirelessStatusIcon_Create();
        BeginAppScreenFade(0, screen->args->heapId);
        Main_SetVBlankIntrCB(LinkBattleScreen_VBlankCallback, screen);
        return 1;
    }
    screen->frame++;
    return 0;
}

int LinkBattleScreen_InitVsScreen(LinkBattleScreen *screen) {
    switch (screen->frame) {
    case 0:
        LinkBattleScreen_InitVsBgLayers(screen, TRUE, FALSE);
        LinkBattleScreen_LoadVsBgGraphics(screen);
        LinkBattleScreen_SetBg1AlphaBlend();
        break;
    case 1:
        LinkBattleScreen_InitLayout(screen, 0, 0);
        LinkBattleScreen_InitVsSprites(screen);
        WirelessStatusIcon_Create();
        LinkBattleScreen_InitParticles(screen);
        break;
    case 2:
        BeginAppScreenFade(0, screen->args->heapId);
        Main_SetVBlankIntrCB(LinkBattleScreen_VBlankCallback, screen);
        screen->frame = 0;
        screen->shakeRangeX = 8;
        screen->shakeRangeY = 4;
        screen->slideSpeed = 16;
        return 1;
    }
    screen->frame++;
    return 0;
}

int LinkBattleScreen_InitResultScreen(LinkBattleScreen *screen) {
    int value;
    int i;

    switch (screen->frame) {
    case 0:
        LinkBattleScreen_InitVsBgLayers(screen, FALSE, TRUE);
        LinkBattleScreen_LoadVsBgGraphics(screen);
        LinkBattleScreen_InitMessageWindows(screen);
        LinkBattleScreen_SetBg1AlphaBlend();
        BattleRecord_IsSlotValid(screen->args->setup->saveData, screen->args->heapId, &value, 0);
        screen->savedVideoCheck = value;
        screen->unk_BD0 = 0;
        for (i = 0; i < 8; i++) {
            PlayerProfile *profile = CommPlayerInfo_GetProfile(i);
            if (profile != NULL && PlayerProfile_GetVersion(profile) == 0) {
                screen->unk_BD0 = 1;
                break;
            }
        }
        break;
    case 1:
        LinkBattleScreen_InitLayout(screen, -32, -16);
        LinkBattleScreen_InitResultSprites(screen);
        WirelessStatusIcon_Create();
        break;
    case 2:
        BeginAppScreenFade(0, screen->args->heapId);
        Main_SetVBlankIntrCB(LinkBattleScreen_VBlankCallback, screen);
        screen->frame = 0;
        screen->shakeRangeX = 4;
        screen->shakeRangeY = 2;
        screen->slideSpeed = 12;
        screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0638_bin, screen->args->heapId);
        screen->msgFormat = MessageFormat_New(screen->args->heapId);
        screen->string = String_New(320, screen->args->heapId);
        screen->videoPromptState = 1;
        return 1;
    }
    screen->frame++;
    return 0;
}

int LinkBattleScreen_WaitFadeAndDelay(LinkBattleScreen *screen) {
    if (IsPaletteFadeFinished() == FALSE) {
        return 0;
    }
    if (screen->delayFrames == screen->frame) {
        screen->frame = 0;
        return 1;
    }
    screen->frame++;
    return 0;
}

int LinkBattleScreen_SlidePanelsIn(LinkBattleScreen *screen) {
    if (Bg_GetXpos(screen->bgConfig, GF_BG_LYR_MAIN_3) <= 24) {
        screen->frame = 0;
        screen->shakeX = 0;
        screen->shakeY = 0;
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 24);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -24);
        LinkBattleScreen_SetBallRowPosition(screen, 0, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 1, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 2, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 3, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 0, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 1, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 2, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 3, 0, 0);
        return 1;
    }

    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SUB_X, screen->slideSpeed);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_ADD_X, screen->slideSpeed);
    screen->frame++;
    LinkBattleScreen_SetBallRowPosition(screen, 0, (s16)(screen->slideSpeed * screen->frame - 256), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, (s16)(screen->slideSpeed * screen->frame - 256), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, (s16)(256 - screen->slideSpeed * screen->frame), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, (s16)(256 - screen->slideSpeed * screen->frame), 0);
    LinkBattleScreen_SetNamePosition(screen, 0, (s16)(screen->slideSpeed * screen->frame - 256), 0);
    LinkBattleScreen_SetNamePosition(screen, 1, (s16)(screen->slideSpeed * screen->frame - 256), 0);
    LinkBattleScreen_SetNamePosition(screen, 2, (s16)(256 - screen->slideSpeed * screen->frame), 0);
    LinkBattleScreen_SetNamePosition(screen, 3, (s16)(256 - screen->slideSpeed * screen->frame), 0);
    return 0;
}

int LinkBattleScreen_ShakePanels(LinkBattleScreen *screen) {
    s16 dx;
    s16 dy;

    if (LinkBattleScreen_UpdateParticles(screen) == FALSE && screen->frame > 24) {
        screen->frame = 0;
        return 1;
    }
    if (screen->frame == 0) {
        if (screen->args->mode == LINK_BATTLE_SCREEN_MODE_VS) {
            PlaySE(SEQ_SE_DP_VSDEMO01);
        } else {
            PlaySE(SEQ_SE_DP_GASHIN);
        }
    }
    if (screen->frame == 24) {
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 24);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -24);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 0, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 1, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 2, 0, 0);
        LinkBattleScreen_SetBallRowPosition(screen, 3, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 0, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 1, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 2, 0, 0);
        LinkBattleScreen_SetNamePosition(screen, 3, 0, 0);
    } else if (screen->frame < 24) {
        dx = (s16)((s32)LCRandom() % screen->shakeRangeX - screen->shakeRangeX / 2);
        dy = (s16)((s32)LCRandom() % screen->shakeRangeY - screen->shakeRangeY / 2);
        if ((dx < 0 && screen->shakeX < 0) || (dx > 0 && screen->shakeX > 0)) {
            dx *= -1;
        }
        if ((dy < 0 && screen->shakeY < 0) || (dy > 0 && screen->shakeY > 0)) {
            dy *= -1;
        }
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, dx + 24);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, dx - 24);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, dy);
        ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, dy);
        LinkBattleScreen_SetBallRowPosition(screen, 0, dx, dy);
        LinkBattleScreen_SetBallRowPosition(screen, 1, dx, dy);
        LinkBattleScreen_SetBallRowPosition(screen, 2, dx, dy);
        LinkBattleScreen_SetBallRowPosition(screen, 3, dx, dy);
        LinkBattleScreen_SetNamePosition(screen, 0, dx, dy);
        LinkBattleScreen_SetNamePosition(screen, 1, dx, dy);
        LinkBattleScreen_SetNamePosition(screen, 2, dx, dy);
        LinkBattleScreen_SetNamePosition(screen, 3, dx, dy);
        screen->shakeX = dx;
        screen->shakeY = dy;
    }
    screen->frame++;
    return 0;
}

int LinkBattleScreen_SlidePanelsOut(LinkBattleScreen *screen) {
    if (screen->frame == 8) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, screen->args->heapId);
    }
    if (screen->frame >= 8 && IsPaletteFadeFinished() == TRUE) {
        screen->taskState = 2;
        return 1;
    }
    if (screen->frame == 0) {
        PlaySE(SEQ_SE_DP_VSDEMO05);
    }
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_X, 16);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_X, 16);
    screen->frame++;
    LinkBattleScreen_SetBallRowPosition(screen, 0, (s16) - (screen->frame << 4), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, (s16) - (screen->frame << 4), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, (s16)(screen->frame << 4), 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, (s16)(screen->frame << 4), 0);
    LinkBattleScreen_SetNamePosition(screen, 0, (s16) - (screen->frame << 4), 0);
    LinkBattleScreen_SetNamePosition(screen, 1, (s16) - (screen->frame << 4), 0);
    LinkBattleScreen_SetNamePosition(screen, 2, (s16)(screen->frame << 4), 0);
    LinkBattleScreen_SetNamePosition(screen, 3, (s16)(screen->frame << 4), 0);
    return 0;
}

int LinkBattleScreen_DelayBeforeNudge(LinkBattleScreen *screen) {
    screen->delayFrames = 16;
    return 2;
}

int LinkBattleScreen_DetermineNudgeDirection(LinkBattleScreen *screen) {
    if (LinkBattleScreen_IsLocalPlayerOnLeft() == TRUE) {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            screen->nudgeDirection = 0;
        } else {
            screen->nudgeDirection = 1;
        }
    } else {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            screen->nudgeDirection = 1;
        } else {
            screen->nudgeDirection = 0;
        }
    }
    return 2;
}

int LinkBattleScreen_NudgePanels(LinkBattleScreen *screen) {
    if (screen->nudgeDirection == 0) {
        return LinkBattleScreen_NudgePanelsRight(screen);
    }
    return LinkBattleScreen_NudgePanelsLeft(screen);
}

int LinkBattleScreen_NudgePanelsRight(LinkBattleScreen *screen) {
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_X, sPanelNudgeSteps[screen->frame * 2]);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SUB_X, sPanelNudgeSteps[screen->frame * 2]);
    LinkBattleScreen_SetBallRowPosition(screen, 0, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 0, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 1, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 2, ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 3, ov05_0221EA6D[screen->frame * 2], 0);
    if (screen->frame == 0) {
        PlaySE(SEQ_SE_DP_DODON);
    }
    if (screen->frame == 5) {
        screen->frame = 0;
        return TRUE;
    }
    screen->frame++;
    return FALSE;
}

int LinkBattleScreen_NudgePanelsLeft(LinkBattleScreen *screen) {
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_ADD_X, sPanelNudgeSteps[screen->frame * 2]);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_X, sPanelNudgeSteps[screen->frame * 2]);
    LinkBattleScreen_SetBallRowPosition(screen, 0, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 0, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 1, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 2, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    LinkBattleScreen_SetNamePosition(screen, 3, (s16)-ov05_0221EA6D[screen->frame * 2], 0);
    if (screen->frame == 0) {
        PlaySE(SEQ_SE_DP_DODON);
    }
    if (screen->frame == 5) {
        screen->frame = 0;
        return TRUE;
    }
    screen->frame++;
    return FALSE;
}

int LinkBattleScreen_DelayBeforeReveal(LinkBattleScreen *screen) {
    screen->delayFrames = 16;
    return 2;
}

#pragma force_active on

int LinkBattleScreen_RevealCenterBand(LinkBattleScreen *screen) {
    if (screen->frame == 0) {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
        reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | 0x2000;
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    }
    if (screen->frame == 16) {
        screen->frame = 0;
        screen->delayFrames = 32;
        reg_GX_DISPCNT &= 0xFFFF1FFF;
        return 1;
    }
    {
        u8 frame = screen->frame;
        u16 top = (((0x47 - frame) << 8) & 0xFF00) | (u8)(frame + 0x48);
        reg_G2_WIN0H = 255;
        reg_G2_WIN0V = top;
    }
    screen->frame++;
    return 0;
}

#pragma force_active reset

int LinkBattleScreen_ShowWinLoseSprites(LinkBattleScreen *screen) {
    PlaySE(SEQ_SE_DP_VSDEMO06);
    ManagedSprite_SetDrawFlag(screen->sprites[12], TRUE);
    ManagedSprite_SetDrawFlag(screen->sprites[13], TRUE);
    ManagedSprite_SetAnim(screen->sprites[12], 0);
    screen->delayFrames = 32;
    return 1;
}

int LinkBattleScreen_ShowDrawSprite(LinkBattleScreen *screen) {
    PlaySE(SEQ_SE_DP_VSDEMO06);
    ManagedSprite_SetDrawFlag(screen->sprites[12], TRUE);
    screen->delayFrames = 64;
    return 1;
}

int LinkBattleScreen_SaveBattleVideo(LinkBattleScreen *screen) {
    u32 result;
    if (LinkBattleScreen_IsBattleRecordActive(screen) == FALSE) {
        screen->delayFrames = 1;
        return 1;
    }
    if (screen->unk_BD0 == 0 && screen->yesNoMenu != NULL) {
        return 0;
    }
    if (screen->videoPromptState == 5) {
        return 0;
    }
    if (screen->videoPromptState != 6 && screen->videoPromptState != 5 && screen->videoPromptState != 7 && screen->videoPromptState != 8) {
        screen->videoPromptState = 7;
        screen->frame = 0xFF;
    }
    switch (screen->frame) {
    case 0:
        LinkBattleScreen_DrawMessageBox(screen);
        ReadMsgDataIntoString(screen->msgData, msg_0638_00008, screen->string);
        screen->printerId = AddTextPrinterParameterized(&screen->msgWindow, 1, screen->string, 0, 0, 0, NULL);
        LinkBattleScreen_ShowWaitingIcon(screen);
        screen->frame++;
        break;
    case 1:
        result = BattleRecord_Save(screen->args->setup->saveData, screen->args->linkBattleMode, 0, 0, &screen->recordSaveState, &screen->recordWriteState);
        if (result == 2) {
            ReadMsgDataIntoString(screen->msgData, msg_0638_00006, screen->string);
            PlaySE(SEQ_SE_DP_SAVE);
        } else if (result != 3) {
            break;
        } else {
            ReadMsgDataIntoString(screen->msgData, msg_0638_00007, screen->string);
        }
        LinkBattleScreen_HideWaitingIcon(screen);
        FillWindowPixelRect(&screen->msgWindow, 15, 0, 0, 216, 32);
        screen->printerId = AddTextPrinterParameterized(&screen->msgWindow, 1, screen->string, 0, 0, 0, NULL);
        screen->delayFrames = 0;
        screen->frame++;
        break;
    case 2:
        screen->delayFrames++;
        if (screen->delayFrames > 30) {
            screen->delayFrames = 0;
            screen->frame++;
        }
        break;
    default:
        ClearFrameAndWindow2(&screen->msgWindow, FALSE);
        ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
        screen->frame = 0;
        screen->delayFrames = 21;
        return 1;
    }
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    return 0;
}

int LinkBattleScreen_WaitForPartner(LinkBattleScreen *screen) {
    switch (screen->frame) {
    case 0:
        if (LinkBattleScreen_IsBattleRecordActive(screen) == FALSE) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
        }
        LinkBattleScreen_DrawMessageBox(screen);
        ReadMsgDataIntoString(screen->msgData, msg_0638_00009, screen->string);
        screen->printerId = AddTextPrinterParameterized(&screen->msgWindow, 1, screen->string, 0, 0, 0, NULL);
        LinkSync_StartSync(0x3E);
        if (screen->canSaveVideo == FALSE && LinkBattleScreen_IsBattleRecordActive(screen) == TRUE) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
        }
        screen->frame++;
        break;
    case 1:
        if (LinkSync_IsSynced(0x3E)) {
            screen->frame = 0;
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    return 0;
}

int LinkBattleScreen_ApplyOutcomePalette(LinkBattleScreen *screen) {
    if (LinkBattleScreen_IsLocalPlayerOnLeft() == TRUE) {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            PaletteData_CopyPalette(screen->paletteData, PLTTBUF_MAIN_BG, 0x40, PLTTBUF_MAIN_BG, 0x30, 0x20);
        } else {
            PaletteData_CopyPalette(screen->paletteData, PLTTBUF_MAIN_BG, 0x40, PLTTBUF_MAIN_BG, 0, 0x20);
        }
    } else {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            PaletteData_CopyPalette(screen->paletteData, PLTTBUF_MAIN_BG, 0x40, PLTTBUF_MAIN_BG, 0, 0x20);
        } else {
            PaletteData_CopyPalette(screen->paletteData, PLTTBUF_MAIN_BG, 0x40, PLTTBUF_MAIN_BG, 0x30, 0x20);
        }
    }
    PaletteData_SetSelectedBufferAll(screen->paletteData, TRUE);
    PlaySE(SEQ_SE_DP_VSDEMO07);
    screen->delayFrames = 64;
    return 1;
}

int LinkBattleScreen_SlidePartyPanelsIn(LinkBattleScreen *screen) {
    if (screen->frame == 0) {
        PlaySE(SEQ_SE_DP_VSDEMO05);
    }
    if (screen->frame == 16) {
        screen->frame = 0;
        screen->delayFrames = 64;
        return 1;
    }
    screen->frame++;
    LinkBattleScreen_SetPartySpriteOffset(screen, (s16)(16 - screen->frame));
    LinkBattleScreen_RevealPanelColumns(screen, screen->frame);
    return 0;
}

int LinkBattleScreen_StartExit(LinkBattleScreen *screen) {
    BeginAppScreenFade(1, screen->args->heapId);
    screen->taskState = 2;
    return 1;
}

#pragma force_active on

int LinkBattleScreen_Finish(SysTask *task, LinkBattleScreen *screen) {
    if (!IsPaletteFadeFinished()) {
        return 0;
    }
    Main_SetVBlankIntrCB(NULL, NULL);
    LinkBattleScreen_HideWaitingIcon(screen);
    if (screen->args->mode != LINK_BATTLE_SCREEN_MODE_PARTY) {
        LinkBattleScreen_RemoveMessageWindows(screen);
        LinkBattleScreen_FreeMessageData(screen);
        LinkBattleScreen_FreeVsBgLayers(screen);
        LinkBattleScreen_FreeParticles(screen);
        LinkBattleScreen_FreeNameTextObjs(screen);
    } else {
        LinkBattleScreen_FreePartyBgLayers(screen);
        LinkBattleScreen_RemovePartyWindows(screen);
    }
    LinkBattleScreen_FreeSprites(screen);
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(screen->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(screen->paletteData);
    screen->args->done = TRUE;
    DestroySysTaskAndEnvironment(task);
    return 1;
}

#pragma force_active reset

BOOL LinkBattleScreen_IsLocalPlayerOnLeft(void) {
    switch (Comm_GetBattleSlot(Comm_GetLocalNetId())) {
    case 0:
    case 2:
        return TRUE;
    case 1:
    case 3:
        return FALSE;
    }
    return FALSE;
}

void LinkBattleScreen_InitSpriteSystem(LinkBattleScreen *screen, SpriteResourceCountsListUnion *resourceCounts, u32 count) {
    OamManagerParam oamParam;
    OamCharTransferParam transferParam;

    GF_CreateVramTransferManager(64, screen->args->heapId);
    screen->spriteSystem = SpriteSystem_Alloc(screen->args->heapId);
    screen->spriteManager = SpriteManager_New(screen->spriteSystem);
    oamParam = sLinkBattleScreenOamParam;
    transferParam = sLinkBattleScreenOamTransferParam;
    transferParam.maxTasks = count;
    SpriteSystem_Init(screen->spriteSystem, &oamParam, &transferParam, 16);
    SpriteSystem_InitSprites(screen->spriteSystem, screen->spriteManager, count);
    SpriteSystem_InitManagerWithCapacities(screen->spriteSystem, screen->spriteManager, resourceCounts);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}

ManagedSprite *LinkBattleScreen_CreateSprite(LinkBattleScreen *screen, const LinkBattleScreenSpriteTemplate *template) {
    ManagedSpriteTemplate spriteTemplate;

    spriteTemplate.x = 0;
    spriteTemplate.y = 0;
    spriteTemplate.z = 0;
    spriteTemplate.animation = 0;
    spriteTemplate.drawPriority = template->drawPriority;
    spriteTemplate.pal = template->pal;
    spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = template->charResId;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = template->plttResId;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = template->cellResId;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = template->animResId;
    spriteTemplate.bgPriority = template->bgPriority;
    spriteTemplate.vramTransfer = 0;
    return SpriteSystem_NewSprite(screen->spriteSystem, screen->spriteManager, &spriteTemplate);
}

void LinkBattleScreen_FreeSprites(LinkBattleScreen *screen) {
    u32 i;

    for (i = 0; i < screen->spriteCount; i++) {
        Sprite_DeleteAndFreeResources(screen->sprites[i]);
    }
    SpriteSystem_FreeResourcesAndManager(screen->spriteSystem, screen->spriteManager);
    SpriteSystem_Free(screen->spriteSystem);
}

void LinkBattleScreen_UpdateSpriteAnims(LinkBattleScreen *screen) {
    u16 i;

    for (i = 0; i < screen->spriteCount; i++) {
        Sprite_UpdateAnim(screen->sprites[i]->sprite, FX32_ONE);
    }
}

void LinkBattleScreen_VBlankCallback(void *data) {
    LinkBattleScreen *screen = data;
    vu32 *dtcm;

    DoScheduledBgGpuUpdates(screen->bgConfig);
    PaletteData_PushTransparentBuffers(screen->paletteData);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    // OS_SetIrqCheckFlag(OS_IE_V_BLANK) with the DTCM address as a literal
    dtcm = (vu32 *)0x027E0000;
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK;
}

void LinkBattleScreen_InitVsBgLayers(LinkBattleScreen *screen, BOOL use3D, BOOL shiftBg1) {
    GraphicsModes graphicsModes;
    BgTemplate bg3Template;
    BgTemplate bg2Template;
    BgTemplate bg1Template;

    screen->use3D = use3D;
    graphicsModes = sVsGraphicsModes;
    graphicsModes._2d3dMode = (GXBG0As)use3D;
    SetBothScreensModesAndDisable(&graphicsModes);

    bg3Template = sVsBg3Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_3, &bg3Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 256);

    bg2Template = sVsBg2Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_2, &bg2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleSetBgPosText(screen->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -256);

    bg1Template = sVsBg1Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_1, &bg1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
    if (shiftBg1 == TRUE) {
        BgSetPosTextAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 24);
    }
    if (use3D == FALSE) {
        BgTemplate bg0Template = sVsBg0Template;

        InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_0, &bg0Template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_0);
        if (screen->canSaveVideo == TRUE) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
        } else {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
        }
    }
}

void LinkBattleScreen_RemoveMessageWindows(LinkBattleScreen *screen) {
    if (screen->msgWindow.pixelBuffer != NULL) {
        RemoveWindow(&screen->msgWindow);
    }
    if (screen->yesNoWindow.pixelBuffer != NULL) {
        RemoveWindow(&screen->yesNoWindow);
    }
}

void LinkBattleScreen_FreeMessageData(LinkBattleScreen *screen) {
    if (screen->string != NULL) {
        String_Delete(screen->string);
    }
    if (screen->msgFormat != NULL) {
        MessageFormat_Delete(screen->msgFormat);
    }
    if (screen->msgData != NULL) {
        DestroyMsgData(screen->msgData);
    }
}

void LinkBattleScreen_FreeVsBgLayers(LinkBattleScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, FALSE);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    if (screen->use3D == FALSE) {
        FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_0);
    }
    Heap_Free(screen->bgConfig);
}

#pragma force_active on

void LinkBattleScreen_LoadVsBgGraphics(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_a_1_0_4, screen->args->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0, screen->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, screen->args->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, screen->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, screen->args->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, screen->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, screen->args->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, screen->args->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, screen->args->heapId);
    PaletteData_LoadPaletteSlotFromHardware(screen->paletteData, PLTTBUF_MAIN_BG, 0, 160);
    BG_SetMaskColor(GF_BG_LYR_MAIN_1, GX_RGB(6, 6, 6));
    NARC_Delete(narc);
}

void LinkBattleScreen_InitMessageWindows(LinkBattleScreen *screen) {
    u8 frame;
    GF_ASSERT(!screen->use3D && screen->args != NULL && screen->args->setup != NULL && screen->args->setup->options != NULL);
    frame = Options_GetFrame(screen->args->setup->options);
    LoadUserFrameGfx2(screen->bgConfig, GF_BG_LYR_MAIN_0, 1, 15, frame, screen->args->heapId);
    PaletteData_LoadPaletteSlotFromHardware(screen->paletteData, PLTTBUF_MAIN_BG, 240, 32);
    LoadUserFrameGfx1(screen->bgConfig, GF_BG_LYR_MAIN_0, 31, 14, 0, screen->args->heapId);
    PaletteData_LoadPaletteSlotFromHardware(screen->paletteData, PLTTBUF_MAIN_BG, 224, 32);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, screen->args->heapId);
    PaletteData_LoadPaletteSlotFromHardware(screen->paletteData, PLTTBUF_MAIN_BG, 208, 32);
    AddWindowParameterized(screen->bgConfig, &screen->msgWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 40);
    AddWindowParameterized(screen->bgConfig, &screen->yesNoWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 40);
}

#pragma force_active reset

void LinkBattleScreen_SetBg1AlphaBlend(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 8, 8);
}

#pragma force_active on

void LinkBattleScreen_InitVsSprites(LinkBattleScreen *screen) {
    SpriteResourceCountsListUnion resourceCounts = sVsSpriteResourceCounts;
    u32 i;
    LinkBattleScreen_InitSpriteSystem(screen, &resourceCounts, 21);
    LinkBattleScreen_LoadPartyBallGfx(screen);
    for (i = 0; i < 12; i++) {
        screen->sprites[i] = LinkBattleScreen_CreateSprite(screen, &sVsSpriteTemplates[i]);
    }
    LinkBattleScreen_SetBallRowPosition(screen, 0, -256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, -256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, 256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, 256, 0);
    LinkBattleScreen_SetAllPartyBallAnims(screen);
    screen->spriteCount = 12;
    LinkBattleScreen_CreateNameTextObjs(screen);
    LinkBattleScreen_SetNamePosition(screen, 0, -256, 0);
    LinkBattleScreen_SetNamePosition(screen, 1, -256, 0);
    LinkBattleScreen_SetNamePosition(screen, 2, 256, 0);
    LinkBattleScreen_SetNamePosition(screen, 3, 256, 0);
}

#pragma force_active reset

void LinkBattleScreen_LoadPartyBallGfx(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_a_0_0_8, screen->args->heapId);
    SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0xD0, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB807);
    SpriteSystem_LoadPlttResObj(screen->spriteSystem, screen->spriteManager, NARC_a_1_0_4, 8, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB807);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0xCF, TRUE, 0xB807);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0xD1, TRUE, 0xB807);
    NARC_Delete(narc);
}

#pragma force_active on

void LinkBattleScreen_SetBallRowPosition(LinkBattleScreen *screen, u8 row, s16 x, s16 y) {
    LinkBattleScreenBallRows rows = sBallRowFirstSprite;
    u32 i;

    for (i = 0; i < 3; i++) {
        ManagedSprite_SetPositionXY(screen->sprites[rows.firstSprite[row] + i], screen->ballRowX[row] + i * 19 + x, screen->ballRowY[row] + y);
    }
}

void LinkBattleScreen_SetPartyBallAnims(LinkBattleScreen *screen, Party *party, int count, int firstSprite) {
    u8 i;
    u8 shown;
    u8 partyCount;
    u8 statusIcon;
    Pokemon *mon;

    partyCount = Party_GetCount(party);
    shown = 0;
    for (i = 0; i < count; i++) {
        ManagedSprite_SetAnim(screen->sprites[firstSprite + i], 0);
        if (partyCount > i) {
            mon = Party_GetMonByIndex(party, i);
            if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE) {
                statusIcon = Pokemon_GetStatusIconId(mon);
                if (statusIcon == 6) {
                    ManagedSprite_SetAnim(screen->sprites[firstSprite + shown], 3);
                } else if (statusIcon != 7) {
                    ManagedSprite_SetAnim(screen->sprites[firstSprite + shown], 2);
                } else {
                    ManagedSprite_SetAnim(screen->sprites[firstSprite + shown], 1);
                }
                shown++;
            }
        }
    }
}

#pragma force_active reset

void LinkBattleScreen_SetAllPartyBallAnims(LinkBattleScreen *screen) {
    LinkBattleScreenArgs *args = screen->args;
    if (args->multiOrSide == FALSE) {
        LinkBattleScreen_SetPartyBallAnims(screen, args->parties[0], PARTY_SIZE, 0);
        LinkBattleScreen_SetPartyBallAnims(screen, screen->args->parties[1], PARTY_SIZE, 6);
    } else {
        LinkBattleScreen_SetPartyBallAnims(screen, args->parties[0], 3, 0);
        LinkBattleScreen_SetPartyBallAnims(screen, screen->args->parties[2], 3, 3);
        LinkBattleScreen_SetPartyBallAnims(screen, screen->args->parties[1], 3, 6);
        LinkBattleScreen_SetPartyBallAnims(screen, screen->args->parties[3], 3, 9);
    }
}

#pragma force_active on

void LinkBattleScreen_CreateNameTextObj(LinkBattleScreen *screen, int index, String *name) {
    TextOBJTemplate template;
    Window *window = &screen->partyWindows[0];
    AddTextWindowTopLeftCorner(screen->bgConfig, window, 8, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(window, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
    template.fontSystem = screen->fontSystem;
    template.window = window;
    template.spriteList = SpriteManager_GetSpriteList(screen->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(screen->spriteManager, 0xB807);
    template.sprite = NULL;
    template.offset = (0x3E0 - index * 16) * 32;
    template.x = 0;
    template.y = 0;
    template.unk_20 = 2;
    template.unk_24 = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = screen->args->heapId;
    screen->nameTextObjs[index] = FontOAM_Create(&template);
    TextOBJ_SetPaletteNum(screen->nameTextObjs[index], 0);
    RemoveWindow(window);
}

#pragma force_active reset

void LinkBattleScreen_CreateNameTextObjs(LinkBattleScreen *screen) {
    screen->fontSystem = FontSystem_NewInit(8, screen->args->heapId);
    LinkBattleScreen_CreateNameTextObj(screen, 0, screen->args->names[0]);
    LinkBattleScreen_CreateNameTextObj(screen, 2, screen->args->names[1]);
    if (screen->args->multiOrSide == TRUE) {
        LinkBattleScreen_CreateNameTextObj(screen, 1, screen->args->names[2]);
        LinkBattleScreen_CreateNameTextObj(screen, 3, screen->args->names[3]);
    } else {
        LinkBattleScreen_CreateNameTextObj(screen, 1, screen->args->names[0]);
        LinkBattleScreen_CreateNameTextObj(screen, 3, screen->args->names[1]);
        TextOBJ_SetSpritesDrawFlag(screen->nameTextObjs[1], FALSE);
        TextOBJ_SetSpritesDrawFlag(screen->nameTextObjs[3], FALSE);
    }
}

void LinkBattleScreen_SetNamePosition(LinkBattleScreen *screen, int index, int x, int y) {
    TextOBJ_SetPosition(screen->nameTextObjs[index], x + screen->nameX[index], y + screen->nameY[index]);
}

void LinkBattleScreen_FreeNameTextObjs(LinkBattleScreen *screen) {
    u32 i;
    if (screen->args->mode != LINK_BATTLE_SCREEN_MODE_PARTY) {
        for (i = 0; i < 4; i++) {
            FontOAM_Delete(screen->nameTextObjs[i]);
        }
        FontSystem_Delete(screen->fontSystem);
    }
}

#pragma force_active on

void LinkBattleScreen_InitLayout(LinkBattleScreen *screen, int lowerOffsetY, int upperOffsetY) {
    if (screen->args->multiOrSide == TRUE) {
        screen->ballRowX[0] = 20;
        screen->ballRowY[0] = lowerOffsetY + 0x92;
        screen->ballRowX[1] = 0x51;
        screen->ballRowY[1] = lowerOffsetY + 0xA2;
        screen->ballRowX[2] = 0x80;
        screen->ballRowY[2] = upperOffsetY + 0x2C;
        screen->ballRowX[3] = 0xBD;
        screen->ballRowY[3] = upperOffsetY + 0x3C;
        screen->nameX[0] = 11;
        screen->nameY[0] = lowerOffsetY + 0x78;
        screen->nameX[1] = 0x4C;
        screen->nameY[1] = lowerOffsetY + 0x88;
        screen->nameX[2] = 0x77;
        screen->nameY[2] = upperOffsetY + 0x12;
        screen->nameX[3] = 0xB8;
        screen->nameY[3] = upperOffsetY + 0x22;
    } else {
        u8 lowerY;
        u8 upperY;
        screen->ballRowX[0] = 24;
        lowerY = lowerOffsetY + 0xA0;
        screen->ballRowY[0] = lowerY;
        screen->ballRowX[1] = 0x51;
        screen->ballRowY[1] = lowerY;
        screen->ballRowX[2] = 0x88;
        upperY = upperOffsetY + 0x30;
        screen->ballRowY[2] = upperY;
        screen->ballRowX[3] = 0xC1;
        screen->ballRowY[3] = upperY;
        screen->nameX[0] = 15;
        screen->nameY[0] = lowerOffsetY + 0x86;
        screen->nameX[2] = 0x80;
        screen->nameY[2] = upperOffsetY + 0x16;
    }
}

void LinkBattleScreen_InitResultSprites(LinkBattleScreen *screen) {
    SpriteResourceCountsListUnion resourceCounts = sResultSpriteResourceCounts;
    u32 i;
    LinkBattleScreen_InitSpriteSystem(screen, &resourceCounts, 23);
    LinkBattleScreen_LoadPartyBallGfx(screen);
    LinkBattleScreen_LoadOutcomeGfx(screen);
    for (i = 0; i < 14; i++) {
        screen->sprites[i] = LinkBattleScreen_CreateSprite(screen, &sVsSpriteTemplates[i]);
    }
    LinkBattleScreen_SetBallRowPosition(screen, 0, -256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 1, -256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 2, 256, 0);
    LinkBattleScreen_SetBallRowPosition(screen, 3, 256, 0);
    LinkBattleScreen_SetAllPartyBallAnims(screen);
    LinkBattleScreen_SetupOutcomeSprites(screen);
    LinkBattleScreen_CreateNameTextObjs(screen);
    LinkBattleScreen_SetNamePosition(screen, 0, -256, 0);
    LinkBattleScreen_SetNamePosition(screen, 1, -256, 0);
    LinkBattleScreen_SetNamePosition(screen, 2, 256, 0);
    LinkBattleScreen_SetNamePosition(screen, 3, 256, 0);
    screen->spriteCount = 14;
}

#pragma force_active reset

void LinkBattleScreen_LoadOutcomeGfx(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_a_1_0_4, screen->args->heapId);
    SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 5, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB808);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 6, FALSE, 0xB808);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 7, FALSE, 0xB808);
    NARC_Delete(narc);
}

void LinkBattleScreen_SetupOutcomeSprites(LinkBattleScreen *screen) {
    ManagedSprite_SetDrawFlag(screen->sprites[12], FALSE);
    ManagedSprite_SetDrawFlag(screen->sprites[13], FALSE);
    if (screen->args->outcome == BATTLE_OUTCOME_DRAW) {
        ManagedSprite_SetPositionXY(screen->sprites[12], 128, 72);
        ManagedSprite_SetAnim(screen->sprites[12], 2);
        return;
    }
    if (LinkBattleScreen_IsLocalPlayerOnLeft() == TRUE) {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            ManagedSprite_SetPositionXY(screen->sprites[12], 48, 72);
            ManagedSprite_SetPositionXY(screen->sprites[13], 208, 72);
        } else {
            ManagedSprite_SetPositionXY(screen->sprites[12], 208, 72);
            ManagedSprite_SetPositionXY(screen->sprites[13], 48, 72);
        }
    } else {
        if (screen->args->outcome == BATTLE_OUTCOME_WIN) {
            ManagedSprite_SetPositionXY(screen->sprites[12], 208, 72);
            ManagedSprite_SetPositionXY(screen->sprites[13], 48, 72);
        } else {
            ManagedSprite_SetPositionXY(screen->sprites[12], 48, 72);
            ManagedSprite_SetPositionXY(screen->sprites[13], 208, 72);
        }
    }
    ManagedSprite_SetAnim(screen->sprites[12], 0);
    ManagedSprite_SetAnim(screen->sprites[13], 1);
}

#pragma force_active on

void LinkBattleScreen_InitParticles(LinkBattleScreen *screen) {
    screen->gfx3DVramMan = GF_3DVramMan_Create(screen->args->heapId, 0, 4, 0, 2, NULL);
    G3X_AlphaBlend(TRUE);
    ParticleSystem_ZeroAll();
    screen->particleHeap = Heap_Alloc(screen->args->heapId, 0x4800);
    screen->particleSystem = ParticleSystem_New(LinkBattleScreen_AllocParticleTexVram, LinkBattleScreen_AllocParticlePlttVram, screen->particleHeap, 0x4800, TRUE, screen->args->heapId);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, 900 * FX32_ONE, ParticleSystem_GetCamera(screen->particleSystem));
    ParticleSystem_SetResource(screen->particleSystem, ParticleSystem_LoadResourceFile(NARC_a_0_5_9, 2, screen->args->heapId), 10, TRUE);
    ParticleSystem_CreateEmitterWithParam(screen->particleSystem, 0, NULL, NULL);
    ParticleSystem_CreateEmitterWithParam(screen->particleSystem, 1, NULL, NULL);
    ParticleSystem_CreateEmitterWithParam(screen->particleSystem, 2, NULL, NULL);
    ParticleSystem_CreateEmitterWithParam(screen->particleSystem, 3, NULL, NULL);
    ParticleSystem_CreateEmitterWithParam(screen->particleSystem, 4, NULL, NULL);
}

#pragma force_active reset

BOOL LinkBattleScreen_UpdateParticles(LinkBattleScreen *screen) {
    if (screen->args->mode != LINK_BATTLE_SCREEN_MODE_VS) {
        return FALSE;
    }
    Thunk_G3X_Reset();
    if (ParticleSystem_GetActiveEmitterCount(screen->particleSystem) == FALSE) {
        return FALSE;
    }
    ParticleSystem_DrawAll();
    ParticleSystem_CalcAll();
    return TRUE;
}

void LinkBattleScreen_FreeParticles(LinkBattleScreen *screen) {
    if (screen->args->mode == LINK_BATTLE_SCREEN_MODE_VS) {
        ParticleSystem_Free(screen->particleSystem);
        Heap_Free(screen->particleHeap);
        GF_3DVramMan_Delete(screen->gfx3DVramMan);
    }
}

u32 LinkBattleScreen_AllocParticleTexVram(u32 size, BOOL is4x4) {
    NNSGfdTexKey key = NNS_GfdAllocTexVram(size, is4x4, 0);
    ParticleSystem_RegisterTexKey(key);
    GF_ASSERT(key != 0);
    return NNS_GfdGetTexKeyAddr(key);
}

u32 LinkBattleScreen_AllocParticlePlttVram(u32 size, BOOL is4pltt) {
    NNSGfdPlttKey key = NNS_GfdAllocPlttVram(size, is4pltt, 1);
    ParticleSystem_RegisterPlttKey(key);
    GF_ASSERT(key != 0);
    return NNS_GfdGetPlttKeyAddr(key);
}

void LinkBattleScreen_InitPartyBgLayers(LinkBattleScreen *screen) {
    GraphicsModes graphicsModes;
    BgTemplate bg1Template;
    BgTemplate bg2Template;
    BgTemplate bg3Template;
    graphicsModes = sPartyGraphicsModes;
    SetBothScreensModesAndDisable(&graphicsModes);
    bg1Template = sPartyBg1Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_1, &bg1Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_1);
    bg2Template = sPartyBg2Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_2, &bg2Template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(screen->bgConfig, GF_BG_LYR_MAIN_2);
    bg3Template = sPartyBg3Template;
    InitBgFromTemplate(screen->bgConfig, GF_BG_LYR_MAIN_3, &bg3Template, GF_BG_TYPE_TEXT);
}

void LinkBattleScreen_FreePartyBgLayers(LinkBattleScreen *screen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, FALSE);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    Heap_Free(screen->bgConfig);
}

void LinkBattleScreen_LoadPartyBgGraphics(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, screen->args->heapId);
    void *nclrFile;
    NNSG2dPaletteData *paletteData;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 15, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, screen->args->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, screen->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, screen->args->heapId);
    nclrFile = NARC_AllocAndReadWholeMember(narc, 16, screen->args->heapId);
    NNS_G2dGetUnpackedPaletteData(nclrFile, &paletteData);
    BG_LoadPlttData(GF_BG_LYR_MAIN_3, paletteData->pRawData, (u16)paletteData->szByte, 0);
    memcpy(screen->panelPalettes, (u8 *)paletteData->pRawData + 0x60, sizeof(screen->panelPalettes));
    Heap_Free(nclrFile);
    PartyMenu_LoadPanelTilemaps(screen->args->heapId, screen->panelTilemaps[0], screen->panelTilemaps[1], screen->panelTilemaps[2]);
    NARC_Delete(narc);
}

#pragma force_active on

void LinkBattleScreen_DrawPartyPanels(LinkBattleScreen *screen) {
    u32 i;
    for (i = 0; i < PARTY_SIZE; i++) {
        u16 *tilemap;
        if (i == 0 || i == 3) {
            tilemap = screen->panelTilemaps[0];
        } else if (screen->monInfo[i].species == SPECIES_NONE) {
            tilemap = screen->panelTilemaps[2];
        } else {
            tilemap = screen->panelTilemaps[1];
        }
        CopyToBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_2, sPartyPanelPositions[i][0], sPartyPanelPositions[i][1], 16, 6, tilemap, 0, 0, 16, 6);
        if (screen->monInfo[i].species != SPECIES_NONE) {
            BgTilemapRectChangePalette(screen->bgConfig, GF_BG_LYR_MAIN_2, sPartyPanelPositions[i][0], sPartyPanelPositions[i][1], 16, 6, (u8)(i + 3));
            if ((i < 3 && screen->args->multiOrSide == 1) || (i >= 3 && screen->args->multiOrSide == 0)) {
                BG_LoadPlttData(GF_BG_LYR_MAIN_2, screen->panelPalettes[1], 32, (u16)((i + 3) * 32));
            } else {
                BG_LoadPlttData(GF_BG_LYR_MAIN_2, screen->panelPalettes[0], 32, (u16)((i + 3) * 32));
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_2);
}

void LinkBattleScreen_StashPanelTilemap(LinkBattleScreen *screen, u16 *dest, u8 bgId) {
    u16 *tilemap = GetBgTilemapBuffer(screen->bgConfig, bgId);
    int x;
    if (screen->args->multiOrSide == 0) {
        x = 16;
    } else {
        x = 0;
    }
    u16 i;
    for (i = 0; i < 24; i++) {
        int offset = x + (i << 5);
        memcpy(dest + 16 * i, tilemap + offset, 32);
        memset(tilemap + offset, 0, 32);
    }
}

#pragma force_active reset

void LinkBattleScreen_StashPanelTilemaps(LinkBattleScreen *screen) {
    LinkBattleScreen_StashPanelTilemap(screen, screen->stashedBg1Tiles[0], GF_BG_LYR_MAIN_1);
    LinkBattleScreen_StashPanelTilemap(screen, screen->stashedBg2Tiles[0], GF_BG_LYR_MAIN_2);
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_2);
}

static inline void LinkBattleScreen_CopyStashedColumns(LinkBattleScreen *screen, u8 width, s32 destX, u8 srcX) {
    CopyToBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_1, (u8)destX, 0, width, 24, screen->stashedBg1Tiles, srcX, 0, 16, 24);
    CopyToBgTilemapRect(screen->bgConfig, GF_BG_LYR_MAIN_2, (u8)destX, 0, width, 24, screen->stashedBg2Tiles, srcX, 0, 16, 24);
}

#pragma force_active on

void LinkBattleScreen_RevealPanelColumns(LinkBattleScreen *screen, u8 width) {
    s32 x;
    if (screen->args->multiOrSide == 0) {
        x = 32 - width;
        LinkBattleScreen_CopyStashedColumns(screen, width, x, 0);
    } else {
        x = 16 - width;
        LinkBattleScreen_CopyStashedColumns(screen, width, 0, (u8)x);
    }
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_1);
    ScheduleBgTilemapBufferTransfer(screen->bgConfig, GF_BG_LYR_MAIN_2);
}

void LinkBattleScreen_ReadPartyMonInfo(LinkBattleScreen *screen, Party *party, int firstIndex) {
    u16 count = Party_GetCount(party);
    u16 i;
    LinkBattleScreen *base;
    i = 0;
    // record i of this party is reached as monInfo[0] of the screen pointer moved by firstIndex + i records
    base = (LinkBattleScreen *)((u8 *)screen + firstIndex * sizeof(LinkBattleScreenMonInfo));
    for (; i < 3; i++) {
        LinkBattleScreen *record;
        if (count <= i) {
            u32 iconNaix = GetMonIconNaixEx(SPECIES_NONE, FALSE, 0);
            record = (LinkBattleScreen *)((u8 *)base + i * sizeof(LinkBattleScreenMonInfo));
            record->monInfo[0].iconNaix = iconNaix;
        } else {
            Pokemon *mon = Party_GetMonByIndex(party, i);
            LinkBattleScreen *details;
            record = (LinkBattleScreen *)((u8 *)base + i * sizeof(LinkBattleScreenMonInfo));
            record->monInfo[0].iconNaix = Pokemon_GetIconNaix(mon);
            record->monInfo[0].species = GetMonData(mon, MON_DATA_SPECIES, NULL);
            if (record->monInfo[0].species != SPECIES_NONE) {
                details = (LinkBattleScreen *)((u8 *)base + i * (int)sizeof(LinkBattleScreenMonInfo));
                details->monInfo[0].isEgg = GetMonData(mon, MON_DATA_IS_EGG, NULL);
                details->monInfo[0].hp = GetMonData(mon, MON_DATA_HP, NULL);
                details->monInfo[0].maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
                details->monInfo[0].level = GetMonData(mon, MON_DATA_LEVEL, NULL);
                details->monInfo[0].heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
                details->monInfo[0].ballCapsule = GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL);
                details->monInfo[0].form = GetMonData(mon, MON_DATA_FORM, NULL);
                if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == TRUE) {
                    record->monInfo[0].unk_0E = 0;
                } else {
                    record->monInfo[0].unk_0E = 1;
                }
                details->monInfo[0].gender = GetMonGender(mon);
                details->monInfo[0].statusIcon = (u8)Pokemon_GetStatusIconId(mon);
            }
        }
    }
}

#pragma force_active reset

void LinkBattleScreen_ReadMonInfo(LinkBattleScreen *screen) {
    memset(screen->monInfo, 0, sizeof(screen->monInfo));
    LinkBattleScreen_ReadPartyMonInfo(screen, screen->args->parties[0], 0);
    LinkBattleScreen_ReadPartyMonInfo(screen, screen->args->parties[2], 3);
}

#pragma force_active on

u32 LinkBattleScreen_GetMonIconAnim(LinkBattleScreen *screen, u32 index) {
    u16 hp = screen->monInfo[index].hp;
    if (hp == 0) {
        return 0;
    }
    if (screen->monInfo[index].statusIcon != 7 && screen->monInfo[index].statusIcon != 0) {
        return 5;
    }
    switch (CalculateHpBarColor(hp, screen->monInfo[index].maxHp, 48)) {
    case 4:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    case 1:
        return 4;
    default:
        return 0;
    }
}

void LinkBattleScreen_CreateMonSprites(LinkBattleScreen *screen, u32 index) {
    u32 i;
    for (i = 0; i < LINK_BATTLE_SCREEN_SPRITES_PER_MON; i++) {
        screen->sprites[index * 5 + i] = LinkBattleScreen_CreateSprite(screen, &sPartySpriteTemplates[index * 5 + i]);
        ManagedSprite_SetPositionXY(screen->sprites[index * 5 + i], sMonSpritePositions[index][i][0], sMonSpritePositions[index][i][1]);
    }
    if (screen->monInfo[index].species == SPECIES_NONE) {
        for (i = 0; i < LINK_BATTLE_SCREEN_SPRITES_PER_MON; i++) {
            ManagedSprite_SetDrawFlag(screen->sprites[index * 5 + i], FALSE);
        }
        return;
    }
    ManagedSprite_SetPaletteOverride(screen->sprites[index * 5], GetMonIconPaletteEx(screen->monInfo[index].species, screen->monInfo[index].form, screen->monInfo[index].isEgg));
    ManagedSprite_SetAnim(screen->sprites[index * 5], LinkBattleScreen_GetMonIconAnim(screen, index));
    if (screen->monInfo[index].heldItem == ITEM_NONE) {
        ManagedSprite_SetDrawFlag(screen->sprites[index * 5 + 1], FALSE);
    } else if (ItemIdIsMail(screen->monInfo[index].heldItem) == TRUE) {
        ManagedSprite_SetAnim(screen->sprites[index * 5 + 1], 1);
    } else {
        ManagedSprite_SetAnim(screen->sprites[index * 5 + 1], 0);
    }
    if (screen->monInfo[index].ballCapsule == 0) {
        ManagedSprite_SetDrawFlag(screen->sprites[index * 5 + 2], FALSE);
    } else {
        ManagedSprite_SetAnim(screen->sprites[index * 5 + 2], 2);
    }
    if (screen->monInfo[index].statusIcon == 7) {
        ManagedSprite_SetDrawFlag(screen->sprites[index * 5 + 3], FALSE);
    } else {
        // reads the record at the loop counter (always 5), not at index
        ManagedSprite_SetAnim(screen->sprites[index * 5 + 3], screen->monInfo[i].statusIcon);
    }
}

#pragma force_active reset

void LinkBattleScreen_InitPartySprites(LinkBattleScreen *screen) {
    SpriteResourceCountsListUnion resourceCounts = sPartySpriteResourceCounts;
    u32 i;
    LinkBattleScreen_InitSpriteSystem(screen, &resourceCounts, 31);
    LinkBattleScreen_LoadMonIconGfx(screen);
    LinkBattleScreen_LoadItemIconGfx(screen);
    LinkBattleScreen_LoadStatusIconGfx(screen);
    LinkBattleScreen_LoadPlistSpriteRes(screen);
    for (i = 0; i < PARTY_SIZE; i++) {
        LinkBattleScreen_CreateMonSprites(screen, i);
    }
    LinkBattleScreen_SetPartySpriteOffset(screen, 16);
    screen->spriteCount = 30;
}

void LinkBattleScreen_LoadMonIconGfx(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, screen->args->heapId);
    u32 i;
    SpriteSystem_LoadPlttResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB808);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, GetMonIconCell32KNaix(), FALSE, 0xB809);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, GetMonIconAnim32KNaix(), FALSE, 0xB809);
    for (i = 0; i < PARTY_SIZE; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, screen->monInfo[i].iconNaix, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB809 + i);
    }
    NARC_Delete(narc);
}

void LinkBattleScreen_LoadItemIconGfx(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, screen->args->heapId);
    SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x14, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB80F);
    SpriteSystem_LoadPlttResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x15, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB809);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x13, FALSE, 0xB80A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x12, FALSE, 0xB80A);
    NARC_Delete(narc);
}

void LinkBattleScreen_LoadStatusIconGfx(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_a_0_3_9, screen->args->heapId);
    SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x40, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB810);
    SpriteSystem_LoadPlttResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x41, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB80A);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x3F, FALSE, 0xB80B);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0x3E, FALSE, 0xB80B);
    NARC_Delete(narc);
}

void LinkBattleScreen_LoadPlistSpriteRes(LinkBattleScreen *screen) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, screen->args->heapId);
    SpriteSystem_LoadCharResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 2, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB811);
    SpriteSystem_LoadPlttResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 8, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0xB80B);
    SpriteSystem_LoadCellResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 1, FALSE, 0xB80C);
    SpriteSystem_LoadAnimResObjFromOpenNarc(screen->spriteSystem, screen->spriteManager, narc, 0, FALSE, 0xB80C);
    NARC_Delete(narc);
}

#pragma force_active on

void LinkBattleScreen_SetPartySpriteOffset(LinkBattleScreen *screen, int shift) {
    int i;
    int firstMon;
    int j;
    const u8(*positions)[2];
    if (!screen->args->multiOrSide) {
        firstMon = 3;
        shift = (s16)(shift * 8);
    } else {
        firstMon = 0;
        shift = (s16)(-(shift * 8));
    }
    i = 0;
    positions = sMonSpritePositions[firstMon];
    for (; i < 3; i++) {
        for (j = 0; j < LINK_BATTLE_SCREEN_SPRITES_PER_MON; j++) {
            ManagedSprite_SetPositionXY(screen->sprites[(firstMon + i) * 5 + j], shift + positions[j][0], positions[j][1]);
        }
        positions += LINK_BATTLE_SCREEN_SPRITES_PER_MON;
    }
}

#pragma force_active reset

void LinkBattleScreen_AddPartyWindows(LinkBattleScreen *screen) {
    u32 i;
    for (i = 0; i < LINK_BATTLE_SCREEN_PARTY_WINDOW_COUNT; i++) {
        AddWindow(screen->bgConfig, &screen->partyWindows[i], &sPartyWindowTemplates[i]);
    }
}

void LinkBattleScreen_RemovePartyWindows(LinkBattleScreen *screen) {
    u32 i;
    for (i = 0; i < LINK_BATTLE_SCREEN_PARTY_WINDOW_COUNT; i++) {
        RemoveWindow(&screen->partyWindows[i]);
    }
}

#pragma force_active on

void LinkBattleScreen_PrintMonName(LinkBattleScreen *screen, MsgData *msgData, MessagePrinter *unused, MessageFormat *msgFormat, String *string, u32 index) {
    Window *window = &screen->partyWindows[index * 4];
    Pokemon *mon;
    String *template;
    if (index < 3) {
        mon = Party_GetMonByIndex(screen->args->parties[0], index);
    } else {
        mon = Party_GetMonByIndex(screen->args->parties[2], index - 3);
    }
    template = NewString_ReadMsgData(msgData, msg_0300_00008 + index);
    BufferBoxMonNickname(msgFormat, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(msgFormat, string, template);
    String_Delete(template);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    if (screen->monInfo[index].unk_0E == 0) {
        if (screen->monInfo[index].gender == MON_MALE) {
            ReadMsgDataIntoString(msgData, msg_0300_00027, string);
            AddTextPrinterParameterizedWithColor(window, 0, string, 64, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
        } else if (screen->monInfo[index].gender == MON_FEMALE) {
            ReadMsgDataIntoString(msgData, msg_0300_00028, string);
            AddTextPrinterParameterizedWithColor(window, 0, string, 64, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
        }
    }
}

#pragma force_active reset

void LinkBattleScreen_PrintMonLevel(LinkBattleScreen *screen, MsgData *unused, MessagePrinter *printer, MessageFormat *unused2, String *unused3, u32 index) {
    Window *window = &screen->partyWindows[index * 4 + 1];
    MessagePrinter_PrintGlyphAndUInt(printer, 1, screen->monInfo[index].level, 3, PRINTING_MODE_LEFT_ALIGN, window, 0, 2);
}

void LinkBattleScreen_PrintMonHp(LinkBattleScreen *screen, MsgData *unused, MessagePrinter *printer, MessageFormat *unused2, String *unused3, u32 index) {
    Window *window = &screen->partyWindows[index * 4 + 2];
    PrintUIntOnWindow(printer, screen->monInfo[index].hp, 3, PRINTING_MODE_RIGHT_ALIGN, window, 0, 2);
    MessagePrinter_PrintGlyph(printer, 0, window, 24, 2);
    PrintUIntOnWindow(printer, screen->monInfo[index].maxHp, 3, PRINTING_MODE_LEFT_ALIGN, window, 32, 2);
}

#pragma force_active on

void LinkBattleScreen_DrawMonHpBar(LinkBattleScreen *screen, int index) {
    Window *window = &screen->partyWindows[index * 4 + 3];
    int color = CalculateHpBarColor(screen->monInfo[index].hp, screen->monInfo[index].maxHp, 48);
    switch (color) {
    case 3:
    case 4:
        BG_LoadPlttData(GF_BG_LYR_MAIN_1, &screen->panelPalettes[0][9], 4, (u16)((window->paletteNum * 16 + 9) * 2));
        break;
    case 2:
        BG_LoadPlttData(GF_BG_LYR_MAIN_1, &screen->panelPalettes[1][9], 4, (u16)((window->paletteNum * 16 + 9) * 2));
        break;
    case 1:
        BG_LoadPlttData(GF_BG_LYR_MAIN_1, &screen->panelPalettes[2][9], 4, (u16)((window->paletteNum * 16 + 9) * 2));
        break;
    }
    int pixels = CalculateHpBarPixelsLength(screen->monInfo[index].hp, screen->monInfo[index].maxHp, 48);
    FillWindowPixelRect(window, 10, 0, 2, (u16)pixels, 1);
    FillWindowPixelRect(window, 9, 0, 3, (u16)pixels, 2);
    FillWindowPixelRect(window, 10, 0, 5, (u16)pixels, 1);
}

#pragma force_active reset

void LinkBattleScreen_DrawMonPanelText(LinkBattleScreen *screen, MsgData *msgData, MessagePrinter *printer, MessageFormat *msgFormat, String *string, u32 index) {
    Window *windows = &screen->partyWindows[index * 4];
    FillWindowPixelBuffer(&windows[0], 0);
    FillWindowPixelBuffer(&windows[1], 0);
    FillWindowPixelBuffer(&windows[2], 0);
    FillWindowPixelBuffer(&windows[3], 0);
    LinkBattleScreen_PrintMonName(screen, msgData, printer, msgFormat, string, index);
    LinkBattleScreen_PrintMonLevel(screen, msgData, printer, msgFormat, string, index);
    LinkBattleScreen_PrintMonHp(screen, msgData, printer, msgFormat, string, index);
    LinkBattleScreen_DrawMonHpBar(screen, index);
    ScheduleWindowCopyToVram(&windows[0]);
    ScheduleWindowCopyToVram(&windows[1]);
    ScheduleWindowCopyToVram(&windows[2]);
    ScheduleWindowCopyToVram(&windows[3]);
}

void LinkBattleScreen_DrawAllMonPanelText(LinkBattleScreen *screen) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0300_bin, screen->args->heapId);
    MessagePrinter *printer = MessagePrinter_New(15, 14, 0, screen->args->heapId);
    MessageFormat *msgFormat = MessageFormat_New(screen->args->heapId);
    String *string = String_New(32, screen->args->heapId);
    u32 i;
    for (i = 0; i < PARTY_SIZE; i++) {
        if (screen->monInfo[i].species != SPECIES_NONE) {
            LinkBattleScreen_DrawMonPanelText(screen, msgData, printer, msgFormat, string, i);
        }
    }
    String_Delete(string);
    DestroyMsgData(msgData);
    MessagePrinter_Delete(printer);
    MessageFormat_Delete(msgFormat);
}

void LinkBattleScreen_DrawMessageBox(LinkBattleScreen *screen) {
    FillWindowPixelRect(&screen->msgWindow, 15, 0, 0, 216, 32);
    DrawFrameAndWindow2(&screen->msgWindow, FALSE, 1, 15);
}

BOOL LinkBattleScreen_IsBattleRecordActive(LinkBattleScreen *screen) {
    if (screen->args->setup->saveData == NULL || BattleRecord_Exists() == FALSE) {
        return FALSE;
    }
    return TRUE;
}

void LinkBattleScreen_ShowWaitingIcon(LinkBattleScreen *screen) {
    if (screen->waitingIcon == NULL) {
        screen->waitingIcon = WaitingIcon_New(&screen->msgWindow, 1);
    }
}

void LinkBattleScreen_HideWaitingIcon(LinkBattleScreen *screen) {
    if (screen->waitingIcon != NULL) {
        WaitingIcon_Delete(screen->waitingIcon);
        screen->waitingIcon = NULL;
    }
}
