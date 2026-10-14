#include "global.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0207.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "spin_trade_link.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "sound_playback.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "screen_fade_legacy.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "union_room_talk.h"
#include "union_room_avatar.h"
#include "vram_transfer_manager.h"

// View of the link menu owned by frontier_menu_communication.c (SpinTradeArgs.menu)
typedef struct SpinTradeGroupMenu {
    u8 unk_00[0x20];
    u32 count;            // 0x20
    u32 state;            // 0x24
    u32 leaderJoinPending;      // 0x28
    u32 participantCount; // 0x2C
    u32 leavingMask;  // 0x30
    u32 unk_34;           // 0x34
    u16 assignedCount;           // 0x38
} SpinTradeGroupMenu;

// Leader marker palette animation
typedef struct SpinTradeGroupBlink {
    BOOL active;             // 0x00
    int frame;               // 0x04
    int timer;               // 0x08
    void *plttFile;          // 0x0C
    NNSG2dPaletteData *pltt; // 0x10
} SpinTradeGroupBlink;

typedef struct SpinTradeGroupMember {
    PlayerProfile *profile;
    PlayerProfile *prevProfile;
} SpinTradeGroupMember;

typedef union SpinTradeGroupTrainerId {
    u64 raw;
    struct {
        u32 id;
        u32 valid;
    } f;
} SpinTradeGroupTrainerId;

typedef struct SpinTradeGroupTrainerIds {
    SpinTradeGroupTrainerId cur;
    SpinTradeGroupTrainerId prev;
} SpinTradeGroupTrainerIds;

typedef struct SpinTradeGroupPos {
    u16 x;
    u16 y;
} SpinTradeGroupPos;

typedef struct SpinTradeGroupApp {
    int state;                              // 0x0000
    u8 unk_0004[4];                         // 0x0004
    u32 result;                             // 0x0008
    SpinTradeArgs *args;                        // 0x000C
    SpinTradeGroupMenu *menu;               // 0x0010
    BgConfig *bgConfig;                     // 0x0014
    u8 unk_0018[4];                         // 0x0018
    SpinTradeGroupBlink blink;              // 0x001C
    SysTask *blinkTask;                     // 0x0030
    MessageFormat *msgFormat;               // 0x0034
    MsgData *msgData;                       // 0x0038
    String *names[5];                       // 0x003C
    u8 unk_0050[4];                         // 0x0050
    String *msgString;                      // 0x0054
    String *titleString;                    // 0x0058
    u32 printerId;                          // 0x005C
    SpriteList *spriteList;                 // 0x0060
    G2dRenderer renderer;                   // 0x0064
    GF_2DGfxResMan *resMan[4];              // 0x018C
    u8 unk_019C[0x20];                      // 0x019C
    SpriteResource *charRes;                // 0x01BC
    SpriteResource *plttRes;                // 0x01C0
    SpriteResource *cellRes;                // 0x01C4
    SpriteResource *animRes;                // 0x01C8
    u8 unk_01CC[0x48];                      // 0x01CC
    SpriteResourcesHeader resHeader;        // 0x0214
    u8 unk_0238[4];                         // 0x0238
    Sprite *avatars[5];                     // 0x023C
    u8 unk_0250[0x58];                      // 0x0250
    Window memberWindow;                    // 0x02A8
    u8 unk_02B8[0x40];                      // 0x02B8
    Window msgWindow;                       // 0x02F8
    Window unk_308;                         // 0x0308
    Window titleWindow;                     // 0x0318
    u8 unk_0328[8];                         // 0x0328
    struct ListMenu2D *yesNoMenu;           // 0x0330
    void *charFiles[2];                     // 0x0334
    NNSG2dCharacterData *chars[2];          // 0x033C
    void *plttFiles[2];                     // 0x0344
    NNSG2dPaletteData *pltts[2];            // 0x034C
    u32 subState;                           // 0x0354, index into gSpinTradeGroupStateFuncs
    u32 nextSubState;                       // 0x0358
    u8 unk_035C[4];                         // 0x035C
    s32 timer;                              // 0x0360
    u8 unk_0364[0x10];                      // 0x0364
    SpinTradeGroupMember members[5];        // 0x0374
    u32 memberStates[5];                    // 0x039C
    s32 saveState;                          // 0x03B0
    u16 blinkAngle;                         // 0x03B4
    u16 unk_3B6;                            // 0x03B6
    void *avatarPalette;                          // 0x03B8
    u8 unk_03BC[0xF80 - 0x3BC];             // 0x03BC
    u8 unk_F80[0x3AC4];                     // 0x0F80
    int lastMemberCount;                    // 0x4A44
    u32 joinPending;                           // 0x4A48
    u32 unk_4A4C;                           // 0x4A4C
    u8 leaveRequested;                            // 0x4A50
    s8 maxEntriesMode;                            // 0x4A51
    u8 startMemberCount;                            // 0x4A52
    u8 unk_4A53;                            // 0x4A53
    s32 leaveTimeout;                           // 0x4A54
    u32 leaveConnectedCount;                           // 0x4A58
    s16 leaveWaitFrames;                           // 0x4A5C
    u8 unk_4A5E[2];                         // 0x4A5E
    SpinTradeGroupTrainerIds trainerIds[5]; // 0x4A60
} SpinTradeGroupApp;                        // size 0x4AB0

extern const WindowTemplate gSpinTradeGroupYesNoWindowTemplate;
extern const u16 gSpinTradeGroupAvatarCharOffsets[];
extern const u8 gSpinTradeGroupBlinkFrames[];
extern const u8 ov85_021EA8EB[];
extern const GraphicsModes gSpinTradeGroupGraphicsModes;
extern const ObjCharTransferTemplate gSpinTradeGroupObjCharTransferTemplate;
extern const SpinTradeGroupPos gSpinTradeGroupAvatarPositions[];
extern const BgTemplate gSpinTradeGroupBgTemplate_Sub2;
extern const BgTemplate gSpinTradeGroupBgTemplate_Main0;
extern const BgTemplate gSpinTradeGroupBgTemplate_Main1;
extern const BgTemplate gSpinTradeGroupBgTemplate_Sub0;
extern const BgTemplate gSpinTradeGroupBgTemplate_Sub1;
extern const GraphicsBanks gSpinTradeGroupGraphicsBanks;
extern int (*const gSpinTradeGroupStateFuncs[])(SpinTradeGroupApp *app, int state);

// Not declared by the owners' headers
u32 Wireless_GetConnectedBitmap(void);
void CommState_SetSpinTradeLinkMode(void);
void CommState_SetMaxEntries(int count);
void Comm_InitSyncedSave(s32 *state);
BOOL Comm_SyncedSave(SaveData *saveData, int a1, s32 *state);
void SpinTradeLink_RegisterCommands(SpinTradeGroupMenu *menu);
BOOL SpinTradeLink_SendCommand(SpinTradeGroupMenu *menu, int command, void *data, int size);

BOOL SpinTradeGroupApp_Init(OverlayManager *manager, int *state);
BOOL SpinTradeGroupApp_Main(OverlayManager *man, int *state);
BOOL SpinTradeGroupApp_Exit(OverlayManager *man, int *state);
void SpinTradeGroup_VBlankTask(SysTask *task, void *data);
void SpinTradeGroup_VBlankCallback(void *data);
void SpinTradeGroup_SetGraphicsBanks(void);
void SpinTradeGroup_InitBgLayers(BgConfig *bgConfig);
void SpinTradeGroup_InitData(SpinTradeGroupApp *app, NARC *narc);
void SpinTradeGroup_FreeData(SpinTradeGroupApp *app);
void SpinTradeGroup_FreeBgLayers(BgConfig *bgConfig);
void SpinTradeGroup_LoadBgGraphics(SpinTradeGroupApp *app, NARC *narc);
void SpinTradeGroup_InitObjTransfer(void);
void SpinTradeGroup_InitSpriteResources(SpinTradeGroupApp *app, NARC *narc);
void SpinTradeGroup_CreateAvatarSprites(SpinTradeGroupApp *app);
void SpinTradeGroup_InitWindows(SpinTradeGroupApp *app);
void SpinTradeGroup_PrintTitle(Window *window, String *str, u32 speed);
void SpinTradeGroup_RemoveWindows(SpinTradeGroupApp *app);
int SpinTradeGroup_State_ShowPrompt(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_HandleInput(SpinTradeGroupApp *app, int state);
void SpinTradeGroup_HandleButtons(SpinTradeGroupApp *app);
void SpinTradeGroup_UpdateDummy(SpinTradeGroupApp *app);
int SpinTradeGroup_State_WaitTextThenResetMenu(SpinTradeGroupApp *app, int state);
void SpinTradeGroup_WaitPrinterThen(SpinTradeGroupApp *app, u32 nextSubState);
int SpinTradeGroup_State_WaitPrinter(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ResetLeaderMenuState(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowCancelYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_HandleCancelYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ResetLeaveWait(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_SendLeave(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_AbortLeave(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowConfirmYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_HandleConfirmYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_SendStartSync(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitSyncCount(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_BeforeSave(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_StartSave(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_Save(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitAfterSave(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowCanceled(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_FadeOutCanceled(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowDisbandYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_HandleDisbandYesNo(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowDisbanded(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitBeforeExit(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_StartExitSync(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitExitSync(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowMemberDropped(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitMemberDropped(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_ShowMemberLeft(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_StartMemberLeftWait(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_WaitMemberLeft(SpinTradeGroupApp *app, int state);
int SpinTradeGroup_State_Proceed(SpinTradeGroupApp *app, int state);
void SpinTradeGroup_HandleLinkEvent(SpinTradeGroupApp *app, int msg, int value);
void SpinTradeGroup_HandleMemberEvent(SpinTradeGroupApp *app, int event, int netId);
void SpinTradeGroup_CycleHighlightColor(u16 *p);
int SpinTradeGroup_CountProfiles(void);
BOOL SpinTradeGroup_PrintMemberList(Window *window, u32 a1, u32 color, SpinTradeGroupApp *app);
void SpinTradeGroup_HandleInputDummy(SpinTradeGroupApp *app);
int SpinTradeGroup_CountMembers(void);
u32 SpinTradeGroup_GetMemberMask(void);
BOOL SpinTradeGroup_UpdateMembers(SpinTradeGroupApp *app);
void SpinTradeGroup_PrintMessage(SpinTradeGroupApp *app, int msgId, int instant);
BOOL SpinTradeGroup_IsPrinterDone(u32 printerId);
void SpinTradeGroup_ClearMessage(SpinTradeGroupApp *app);
void SpinTradeGroup_UpdateAvatars(SpinTradeGroupApp *app);
void SpinTradeGroup_LoadAvatarGraphics(SpinTradeGroupApp *app, NARC *narc);
void SpinTradeGroup_LoadAvatar(NNSG2dCharacterData **chars, NNSG2dPaletteData **pltts, int slot, int avatar, int gender);
void SpinTradeGroup_FreeAvatarGraphics(SpinTradeGroupApp *app);
int SpinTradeGroup_GetTextSpeed(SpinTradeGroupApp *app);
void SpinTradeGroup_SetMaxConnections(SpinTradeGroupApp *app, int extraEntries);
int SpinTradeGroup_CheckMembers(SpinTradeGroupApp *app, int extraEntries);
void SpinTradeGroup_SaveDummy(void *a0, void *a1);

BOOL SpinTradeGroupApp_Init(OverlayManager *manager, int *state) {
    NARC *narc;
    SpinTradeArgs *args = OverlayManager_GetArgs(manager);
    SpinTradeGroupApp *app;
    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(vu32 *)0x04000000 &= 0xffffe0ff;
        *(vu32 *)0x04001000 &= 0xffffe0ff;
        Heap_Create(HEAP_ID_3, HEAP_ID_102, 0x80000);
        narc = NARC_New(NARC_a_2_1_5, HEAP_ID_102);
        GF_ASSERT(narc != NULL);
        app = OverlayManager_CreateAndGetData(manager, sizeof(SpinTradeGroupApp), HEAP_ID_102);
        memset(app, 0, sizeof(SpinTradeGroupApp));
        args->groupApp = app;
        app->args = args;
        app->menu = args->menu;
        app->bgConfig = BgConfig_Alloc(HEAP_ID_102);
        app->msgFormat = MessageFormat_New(HEAP_ID_102);
        app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0207_bin, HEAP_ID_102);
        SetKeyRepeatTimers(4, 8);
        SpinTradeGroup_SetGraphicsBanks();
        SpinTradeGroup_InitBgLayers(app->bgConfig);
        BeginNormalPaletteFade(0, 0x11, 0x11, RGB_BLACK, 16, 1, HEAP_ID_102);
        SpinTradeGroup_LoadBgGraphics(app, narc);
        Main_SetVBlankIntrCB(SpinTradeGroup_VBlankCallback, app);
        SpinTradeGroup_InitData(app, narc);
        SpinTradeGroup_InitObjTransfer();
        SpinTradeGroup_InitSpriteResources(app, narc);
        SpinTradeGroup_CreateAvatarSprites(app);
        SpinTradeGroup_InitWindows(app);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        SpinTradeLink_RegisterCommands(args->menu);
        CommState_SetSpinTradeLinkMode();
        CommState_SetMaxEntries(3);
        if (!Comm_GetLocalNetId()) {
            UnionRoom_SetBeaconActivity(13);
        }
        WirelessStatusIcon_Create();
        app->blinkTask = SysTask_CreateOnVBlankQueue(SpinTradeGroup_VBlankTask, app, 5);
        NARC_Delete(narc);
        (*state)++;
        break;
    case 1:
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
BOOL SpinTradeGroupApp_Main(OverlayManager *man, int *state) {
    SpinTradeGroupApp *app = OverlayManager_GetData(man);
    int mode;

    if (System_GetTouchNew()) {
        gSystem.simulatedInputs = 1;
    }
    if (Comm_GetLocalNetId() == 0 && app->menu->leavingMask) {
        u32 mask = Wireless_GetConnectedBitmap();
        app->menu->leavingMask = mask & app->menu->leavingMask;
    }
    mode = app->state;
    switch (mode) {
    case 0:
        if (IsPaletteFadeFinished()) {
            app->state = 1;
            if (Comm_GetLocalNetId() != 0 && SpinTradeGroup_CountMembers() > 2) {
                SpinTradeLink_SendCommand(app->menu, 4, NULL, 0);
            }
        }
        break;
    case 1:
        if (gSpinTradeGroupStateFuncs[app->subState]) {
            app->state = gSpinTradeGroupStateFuncs[app->subState](app, mode);
        }
        if (app->menu->state == 0) {
            SpinTradeGroup_PrintMemberList(&app->memberWindow, 0, MAKE_TEXT_COLOR(1, 3, 0), app);
        }
        SpinTradeGroup_UpdateAvatars(app);
        if (Comm_GetLocalNetId() == 0) {
            int next = SpinTradeGroup_CheckMembers(app, 1);
            if (app->state == 1) {
                app->state = next;
            }
        }
        break;
    case 2:
        if (gSpinTradeGroupStateFuncs[app->subState]) {
            app->state = gSpinTradeGroupStateFuncs[app->subState](app, mode);
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(app->spriteList);
    return FALSE;
}
#pragma force_active reset

BOOL SpinTradeGroupApp_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    int i;
    SpinTradeGroupApp *app = OverlayManager_GetData(man);
    SysTask_Destroy(app->blinkTask);
    SpriteTransfer_DeleteCharTransferTask(app->charRes);
    SpriteTransfer_DeletePlttTransferTask(app->plttRes);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(app->resMan[i]);
    }
    SpriteList_Delete(app->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    SpinTradeGroup_RemoveWindows(app);
    SpinTradeGroup_FreeBgLayers(app->bgConfig);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    reg_GX_POWCNT |= 0x8000;
    Main_SetVBlankIntrCB(NULL, NULL);
    app->args->result = app->result;
    app->args->unk0[2] = SpinTradeGroup_CountMembers();
    app->args->unk0[3] = SpinTradeGroup_GetMemberMask();
    SpinTradeGroup_FreeData(app);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_102);
    return TRUE;
}

void SpinTradeGroup_VBlankTask(SysTask *task, void *data) {
    SpinTradeGroupApp *app = data;
    SpinTradeGroupBlink *blink = &app->blink;
    if (app->blink.active != 0) {
        if (blink->timer > gSpinTradeGroupBlinkFrames[blink->frame * 2]) {
            blink->timer = 0;
            blink->frame++;
            if (ov85_021EA8EB[blink->frame * 2] == 0xFF) {
                blink->frame = 0;
            }
            GX_LoadOBJPltt((u8 *)blink->pltt->pRawData + ov85_021EA8EB[blink->frame * 2] * 32, 0, 0x20);
        } else {
            app->blink.timer++;
        }
        SpinTradeGroup_CycleHighlightColor(&app->blinkAngle);
    }
}

// VBlank callback; registered with the app pointer as its argument
void SpinTradeGroup_VBlankCallback(void *data) {
    volatile u32 *a = (volatile u32 *)0x027e0000;
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(data);
    a[0x3ff8 / 4] |= 1;
}

void SpinTradeGroup_SetGraphicsBanks(void) {
    GraphicsBanks banks = gSpinTradeGroupGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void SpinTradeGroup_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = gSpinTradeGroupGraphicsModes;
    BgTemplate subBg0, subBg1, subBg2, mainBg0, mainBg1;

    SetBothScreensModesAndDisable(&modes);
    subBg0 = gSpinTradeGroupBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    subBg1 = gSpinTradeGroupBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    subBg2 = gSpinTradeGroupBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subBg2, GF_BG_TYPE_TEXT);
    mainBg0 = gSpinTradeGroupBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &mainBg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    mainBg1 = gSpinTradeGroupBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainBg1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_102);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_102);
}
#pragma force_active reset

void SpinTradeGroup_InitData(SpinTradeGroupApp *app, NARC *narc) {
    int i;
    for (i = 0; i < 5; i++) {
        app->names[i] = String_New(8, HEAP_ID_102);
        app->members[i].profile = 0;
        app->members[i].prevProfile = 0;
        app->memberStates[i] = 0;
    }
    app->msgString = String_New(0xB4, HEAP_ID_102);
    app->titleString = String_New(0x28, HEAP_ID_102);
    app->subState = 0;
    ReadMsgDataIntoString(app->msgData, 0x11, app->titleString);
    SpinTradeGroup_LoadAvatarGraphics(app, narc);
    app->avatarPalette = UnionRoom_LoadAvatarPalette(HEAP_ID_102);
    app->blink.active = 0;
    app->blink.timer = 0;
    app->blink.frame = 0;
    app->blink.plttFile = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 1, &app->blink.pltt, HEAP_ID_102);
    app->lastMemberCount = 0;
    app->yesNoMenu = NULL;
    app->menu->participantCount = 2;
}

void SpinTradeGroup_FreeData(SpinTradeGroupApp *app) {
    int i;
    SpinTradeGroup_FreeAvatarGraphics(app);
    Heap_Free(app->blink.plttFile);
    Heap_Free(app->avatarPalette);
    for (i = 0; i < 5; i++) {
        String_Delete(app->names[i]);
    }
    String_Delete(app->titleString);
    String_Delete(app->msgString);
}

void SpinTradeGroup_FreeBgLayers(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(bgConfig);
}

#pragma force_active on
void SpinTradeGroup_LoadBgGraphics(SpinTradeGroupApp *app, NARC *narc) {
    BgConfig *bgConfig = app->bgConfig;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_102);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_4, 71, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_102);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_102);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_102);
    GfGfxLoader_LoadCharData(NARC_a_0_1_4, 10, bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_102);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_4, 11, bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_102);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x2000, TRUE, HEAP_ID_102);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_102);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, Options_GetFrame(app->args->input.options), HEAP_ID_102);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_102);
}
#pragma force_active reset

void SpinTradeGroup_InitObjTransfer(void) {
    ObjCharTransferTemplate tmpl = gSpinTradeGroupObjCharTransferTemplate;
    ObjCharTransfer_Init(&tmpl);
    ObjPlttTransfer_Init(0x14, HEAP_ID_102);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void SpinTradeGroup_InitSpriteResources(SpinTradeGroupApp *app, NARC *narc) {
    int i;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_102);
    app->spriteList = G2dRenderer_Init(0x1E, &app->renderer, HEAP_ID_102);
    if (app->spriteList == NULL) {
        GF_AssertFail();
    }
    G2dRenderer_SetSubSurfaceCoords(&app->renderer, 0, 0x100000);
    for (i = 0; i < 4; i++) {
        app->resMan[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_102);
    }
    app->charRes = AddCharResObjFromOpenNarc(app->resMan[0], narc, 0xC, TRUE, 2, 1, HEAP_ID_102);
    app->plttRes = AddPlttResObjFromOpenNarc(app->resMan[1], narc, 1, FALSE, 2, 1, 0xF, HEAP_ID_102);
    app->cellRes = AddCellOrAnimResObjFromOpenNarc(app->resMan[2], narc, 0xD, TRUE, 2, (GfGfxResType)2, HEAP_ID_102);
    app->animRes = AddCellOrAnimResObjFromOpenNarc(app->resMan[3], narc, 0xE, TRUE, 2, (GfGfxResType)3, HEAP_ID_102);
    SpriteTransfer_CreateCharTransferTask(app->charRes);
    SpriteTransfer_CreateExtPlttTransferTask(app->plttRes);
}

void SpinTradeGroup_CreateAvatarSprites(SpinTradeGroupApp *app) {
    SpriteTemplate tmpl;
    int i;

    CreateSpriteResourcesHeader(&app->resHeader, 2, 2, 2, 2, -1, -1, 0, 1, app->resMan[0], app->resMan[1], app->resMan[2], app->resMan[3], NULL, NULL);
    tmpl.spriteList = (SpriteList *)app->spriteList;
    tmpl.header = &app->resHeader;
    tmpl.position.z = 0;
    tmpl.scale.x = 0x1000;
    tmpl.scale.y = 0x1000;
    tmpl.scale.z = 0x1000;
    tmpl.rotation = 0;
    tmpl.drawPriority = 1;
    tmpl.whichScreen = (NNS_G2D_VRAM_TYPE)1;
    tmpl.heapID = HEAP_ID_102;
    for (i = 0; i < 5; i++) {
        tmpl.position.x = gSpinTradeGroupAvatarPositions[i + 1].x << 12;
        tmpl.position.y = gSpinTradeGroupAvatarPositions[i + 1].y << 12;
        app->avatars[i] = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(app->avatars[i], 1);
        Sprite_SetAnimCtrlSeq(app->avatars[i], (i - 1) * 2 + 0x1B);
        Sprite_SetDrawFlag(app->avatars[i], 0);
    }
    tmpl.position.x = 0x18000;
    tmpl.position.y = 0x1A0000;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void SpinTradeGroup_InitWindows(SpinTradeGroupApp *app) {
    AddWindowParameterized(app->bgConfig, &app->unk_308, 5, 0x1A, 0x14, 6, 2, 0xD, 0x1C3);
    FillWindowPixelBuffer(&app->unk_308, 0);
    AddWindowParameterized(app->bgConfig, &app->msgWindow, 0, 2, 0x13, 0x1B, 4, 0xD, 0x28);
    FillWindowPixelBuffer(&app->msgWindow, 0xF);
    AddWindowParameterized(app->bgConfig, &app->titleWindow, 0, 3, 1, 0x1A, 2, 0xF, 0x94);
    SpinTradeGroup_PrintTitle(&app->titleWindow, app->titleString, 0);
    AddWindowParameterized(app->bgConfig, &app->memberWindow, 0, 2, 6, 0x10, 0xB, 0xF, 0xC8);
    FillWindowPixelBuffer(&app->memberWindow, 0);
    SpinTradeGroup_PrintMemberList(&app->memberWindow, 0, 0x10300, app);
}

void SpinTradeGroup_PrintTitle(Window *window, String *str, u32 speed) {
    u32 width = FontID_String_GetWidth(1, str, 0);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 1, str, (u32)((0xD0 - (int)width) / 2), 1, speed, 0x10400, NULL);
}

void SpinTradeGroup_RemoveWindows(SpinTradeGroupApp *app) {
    RemoveWindow(&app->memberWindow);
    RemoveWindow(&app->titleWindow);
    RemoveWindow(&app->unk_308);
    RemoveWindow(&app->msgWindow);
}

int SpinTradeGroup_State_ShowPrompt(SpinTradeGroupApp *app, int state) {
    CommState_SetErrorWatch(0, 1);
    if (!Comm_GetLocalNetId()) {
        if ((int)Comm_CountConnected() >= 2) {
            SpinTradeGroup_PrintMessage(app, 2, 0);
        }
    } else {
        SpinTradeGroup_PrintMessage(app, 8, 0);
    }
    SpinTradeGroup_WaitPrinterThen(app, 3);
    return state;
}

int SpinTradeGroup_State_HandleInput(SpinTradeGroupApp *app, int state) {
    SpinTradeGroup_HandleButtons(app);
    if (!Comm_GetLocalNetId()) {
        if (SpinTradeGroup_CountProfiles() != 1) {
            SpinTradeGroup_HandleInputDummy(app);
        }
    } else {
        SpinTradeGroup_HandleInputDummy(app);
    }
    return state;
}

void SpinTradeGroup_HandleButtons(SpinTradeGroupApp *app) {
    u8 a;
    u8 b;
    u8 c;

    if (gSystem.newKeys & 1) {
        if (!Comm_GetLocalNetId()) {
            if (app->menu->participantCount == (u32)SpinTradeGroup_CountMembers() && app->menu->leavingMask == 0) {
                a = 1;
                SpinTradeGroup_PrintMessage(app, 3, 0);
                SpinTradeGroup_WaitPrinterThen(app, 0x16);
                SpinTradeLink_SendCommand(app->menu, 7, &a, 1);
                SpinTradeGroup_SetMaxConnections(app, 0);
            } else {
                PlaySE(0x5F2);
            }
        }
    } else if (gSystem.newKeys & 2) {
        if (Comm_GetLocalNetId()) {
            if (app->menu->leaderJoinPending == 0) {
                SpinTradeGroup_PrintMessage(app, 4, 0);
                SpinTradeGroup_WaitPrinterThen(app, 4);
            } else {
                PlaySE(0x5F2);
            }
        } else {
            if (app->menu->participantCount == Comm_CountConnected() && app->menu->leavingMask == 0) {
                b = 1;
                SpinTradeGroup_PrintMessage(app, 4, 0);
                SpinTradeGroup_WaitPrinterThen(app, 4);
                SpinTradeLink_SendCommand(app->menu, 7, &b, 1);
                SpinTradeGroup_SetMaxConnections(app, 0);
            } else {
                PlaySE(0x5F2);
            }
        }
    } else {
        if (app->joinPending == 0 && !Comm_GetLocalNetId() && app->menu->participantCount == Comm_CountConnected()) {
            c = 0;
            SpinTradeLink_SendCommand(app->menu, 7, &c, 1);
        }
    }
}

void SpinTradeGroup_UpdateDummy(SpinTradeGroupApp *app) {
#pragma unused(app)
}

int SpinTradeGroup_State_WaitTextThenResetMenu(SpinTradeGroupApp *app, int state) {
    SpinTradeGroup_WaitPrinterThen(app, 2);
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

void SpinTradeGroup_WaitPrinterThen(SpinTradeGroupApp *app, u32 nextSubState) {
    app->subState = 30;
    app->nextSubState = nextSubState;
}

int SpinTradeGroup_State_WaitPrinter(SpinTradeGroupApp *app, int state) {
    if (SpinTradeGroup_IsPrinterDone(app->printerId)) {
        app->subState = app->nextSubState;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ResetLeaderMenuState(SpinTradeGroupApp *app, int state) {
    u32 v;
    if (!Comm_GetLocalNetId()) {
        v = 0;
        SpinTradeLink_SendCommand(app->menu, 7, &v, 1);
    }
    app->subState = 3;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowCancelYesNo(SpinTradeGroupApp *app, int state) {
    app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &gSpinTradeGroupYesNoWindowTemplate, 0x1F, 0xB, HEAP_ID_102);
    app->subState = 5;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_HandleCancelYesNo(SpinTradeGroupApp *app, int state) {
    u32 v;
    u8 buf[4];
    int input;

    if (Comm_GetLocalNetId()) {
        if (app->menu->leaderJoinPending) {
            if (gSystem.newKeys & 0xC3) {
                PlaySE(0x5F2);
            }
            SpinTradeGroup_UpdateDummy(app);
            return state;
        }
    } else {
        if (app->menu->leavingMask) {
            if (gSystem.newKeys & 0xC3) {
                PlaySE(0x5F2);
            }
            SpinTradeGroup_UpdateDummy(app);
            return state;
        }
    }
    if ((u32)SpinTradeGroup_CountMembers() != Comm_CountConnected()) {
        SpinTradeGroup_UpdateDummy(app);
        return state;
    }
    input = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_102);
    if (input != -1) {
        if (input == -2) {
            if (!Comm_GetLocalNetId()) {
                v = 0;
                SpinTradeLink_SendCommand(app->menu, 7, &v, 1);
                SpinTradeGroup_SetMaxConnections(app, 1);
            }
            SpinTradeGroup_WaitPrinterThen(app, 0);
        } else if (!Comm_GetLocalNetId()) {
            SpinTradeGroup_WaitPrinterThen(app, 0xB);
            SpinTradeGroup_PrintMessage(app, 0xE, 0);
        } else {
            MI_CpuFill8(buf, 0, 4);
            buf[2] = 0;
            buf[0] = (u8)Comm_GetLocalNetId();
            app->leaveRequested = 1;
            app->leaveWaitFrames = 0;
            app->subState = 6;
            SpinTradeLink_SendCommand(app->menu, 2, buf, 4);
        }
        app->yesNoMenu = NULL;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ResetLeaveWait(SpinTradeGroupApp *app, int state) {
    app->leaveWaitFrames = 0;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_SendLeave(SpinTradeGroupApp *app, int state) {
    if (app->menu->assignedCount != Comm_CountConnected() || app->menu->assignedCount != SpinTradeGroup_CountMembers()) {
        app->leaveWaitFrames = 0;
        app->subState = 8;
        SpinTradeGroup_UpdateDummy(app);
        return state;
    }
    app->leaveWaitFrames++;
    if (app->leaveWaitFrames > 30) {
        u8 buf[4];
        MI_CpuFill8(buf, 0, 4);
        buf[2] = 1;
        buf[0] = (u8)Comm_GetLocalNetId();
        SpinTradeLink_SendCommand(app->menu, 2, buf, 4);
        app->leaveWaitFrames = 0;
        app->subState = 9;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_AbortLeave(SpinTradeGroupApp *app, int state) {
    app->leaveRequested = 0;
    SpinTradeGroup_WaitPrinterThen(app, 0);
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowConfirmYesNo(SpinTradeGroupApp *app, int state) {
    app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &gSpinTradeGroupYesNoWindowTemplate, 0x1F, 0xB, HEAP_ID_102);
    app->subState = 0x17;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_HandleConfirmYesNo(SpinTradeGroupApp *app, int state) {
    SpinTradeGroupMenu *p = app->menu;
    if (p->participantCount != SpinTradeGroup_CountMembers() || p->leavingMask != 0) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2);
        }
        SpinTradeGroup_UpdateDummy(app);
        return state;
    }
    {
        int r = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_102);
        if (r != -1) {
            if (r == -2) {
                u32 v = 0;
                SpinTradeLink_SendCommand(app->menu, 7, &v, 1);
                SpinTradeGroup_SetMaxConnections(app, 1);
                SpinTradeGroup_WaitPrinterThen(app, 0);
            } else if (Comm_GetLocalNetId() == 0) {
                app->subState = 0x18;
                app->startMemberCount = (u8)SpinTradeGroup_CountMembers();
                UnionRoom_SetBeaconActivity(0xC);
            } else {
                GF_AssertFail();
            }
            app->yesNoMenu = NULL;
        }
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_SendStartSync(SpinTradeGroupApp *app, int state) {
    if (app->unk_4A53 == 0) {
        if (SpinTradeLink_SendCommand(app->menu, 5, NULL, 0) == 1) {
            app->subState = 0x1F;
        }
    }
    return state;
}

int SpinTradeGroup_State_WaitSyncCount(SpinTradeGroupApp *app, int state) {
    if (!Comm_GetLocalNetId()) {
        Comm_CountConnected();
    }
    if (app->menu->count == Comm_CountConnected()) {
        app->subState = 0x1F;
    }
    return state;
}

int SpinTradeGroup_State_BeforeSave(SpinTradeGroupApp *app, int state) {
    SpinTradeGroup_SaveDummy(app->args->input.saveData, app->unk_F80);
    app->subState = 0x1B;
    return state;
}

int SpinTradeGroup_State_StartSave(SpinTradeGroupApp *app, int state) {
    gSystem.softResetDisabled = 1;
    GameStats_AddScore(app->args->input.gameStats, SCORE_EVENT_RECORDS_MIXED);
    Comm_InitSyncedSave(&app->saveState);
    app->subState = 0x1C;
    return state;
}

int SpinTradeGroup_State_Save(SpinTradeGroupApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        Comm_CountConnected();
    }
    if (Comm_SyncedSave(app->args->input.saveData, 2, &app->saveState) != 0) {
        StopSE(0x657, 8);
        SpinTradeGroup_PrintMessage(app, 0xD, 0);
        SpinTradeGroup_WaitPrinterThen(app, 0x1D);
        app->blink.active = 0;
        app->timer = 0;
        gSystem.softResetDisabled = 0;
        app->menu->state = 0;
    }
    return state;
}

int SpinTradeGroup_State_WaitAfterSave(SpinTradeGroupApp *app, int state) {
    if (app->timer++ > 0x3C) {
        app->subState = 0xF;
    }
    return state;
}

int SpinTradeGroup_State_ShowCanceled(SpinTradeGroupApp *app, int state) {
    SpinTradeGroup_PrintMessage(app, 5, 0);
    SpinTradeGroup_WaitPrinterThen(app, 0xA);
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_FadeOutCanceled(SpinTradeGroupApp *app, int state) {
    app->timer++;
    if (app->timer > 0x3C) {
        BeginNormalPaletteFade(0, 0x10, 0x10, 0, 0x10, 1, HEAP_ID_102);
        state = 3;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowDisbandYesNo(SpinTradeGroupApp *app, int state) {
    app->yesNoMenu = Std_CreateYesNoMenu(app->bgConfig, &gSpinTradeGroupYesNoWindowTemplate, 0x1F, 0xB, HEAP_ID_102);
    app->subState = 0xC;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_HandleDisbandYesNo(SpinTradeGroupApp *app, int state) {
    SpinTradeGroupMenu *p = app->menu;
    if (p->participantCount != SpinTradeGroup_CountMembers() || p->leavingMask != 0) {
        if (0xC3 & gSystem.newKeys) {
            PlaySE(0x5F2);
        }
        SpinTradeGroup_UpdateDummy(app);
        return state;
    }
    {
        int r = Handle2dMenuInput_DeleteOnFinish(app->yesNoMenu, HEAP_ID_102);
        if (r != -1) {
            if (r == -2) {
                u32 v = 0;
                app->subState = 0;
                SpinTradeLink_SendCommand(app->menu, 7, &v, 1);
                SpinTradeGroup_SetMaxConnections(app, 1);
            } else {
                app->subState = 0xD;
                SpinTradeLink_SendCommand(app->menu, 3, NULL, 0);
                BufferPlayersName(app->msgFormat, 0, CommPlayerInfo_GetProfile(0));
            }
            app->yesNoMenu = NULL;
        }
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowDisbanded(SpinTradeGroupApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        SpinTradeGroup_PrintMessage(app, 5, 0);
    } else {
        BufferPlayersName(app->msgFormat, 0, CommPlayerInfo_GetProfile(0));
        SpinTradeGroup_PrintMessage(app, 0x10, 0);
    }
    SpinTradeGroup_WaitPrinterThen(app, 14);
    app->timer = 0;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_WaitBeforeExit(SpinTradeGroupApp *app, int state) {
    app->timer++;
    if (app->timer > 0x2D) {
        app->subState = 0xF;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_StartExitSync(SpinTradeGroupApp *app, int state) {
    LinkSync_StartSync(0xCA);
    app->subState = 0x10;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_WaitExitSync(SpinTradeGroupApp *app, int state) {
    if (LinkSync_IsSynced(0xCA)) {
        CommState_SetErrorWatch(0, 0);
        BeginNormalPaletteFade(0, 0x10, 0x10, 0, 0x10, 1, HEAP_ID_102);
        state = 3;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowMemberDropped(SpinTradeGroupApp *app, int state) {
    if (SpinTradeGroup_IsPrinterDone(app->printerId)) {
        SpinTradeGroup_PrintMessage(app, 9, 0);
    }
    app->subState = 0x12;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_WaitMemberDropped(SpinTradeGroupApp *app, int state) {
    if (SpinTradeGroup_IsPrinterDone(app->printerId)) {
        app->subState = 0xA;
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_ShowMemberLeft(SpinTradeGroupApp *app, int state) {
    if (app->printerId != 0xFF && SpinTradeGroup_IsPrinterDone(app->printerId) == 0) {
        RemoveTextPrinter((u8)app->printerId);
    }
    SpinTradeGroup_PrintMessage(app, 0xF, 1);
    app->subState = 0x14;
    if (Comm_GetLocalNetId() == 0) {
        SpinTradeGroup_SetMaxConnections(app, 0);
    }
    app->leaveConnectedCount = Comm_CountConnected();
    app->leaveTimeout = 0x12C;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_StartMemberLeftWait(SpinTradeGroupApp *app, int state) {
    if (app->leaveConnectedCount != 0) {
        if (app->leaveConnectedCount != Comm_CountConnected()) {
            app->leaveConnectedCount = 0;
        }
    }
    app->subState = 0x15;
    app->timer = 0;
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_WaitMemberLeft(SpinTradeGroupApp *app, int state) {
    if (app->leaveConnectedCount != 0) {
        if (app->leaveConnectedCount != Comm_CountConnected()) {
            app->leaveConnectedCount = 0;
        }
    }
    app->leaveTimeout--;
    if (app->leaveTimeout < 0) {
        app->leaveConnectedCount = 0;
    }
    app->timer++;
    if (app->timer > 0x3C && app->leaveConnectedCount == 0) {
        app->subState = 0;
        if (Comm_GetLocalNetId() == 0) {
            SpinTradeGroup_SetMaxConnections(app, 1);
        }
    }
    SpinTradeGroup_UpdateDummy(app);
    return state;
}

int SpinTradeGroup_State_Proceed(SpinTradeGroupApp *app, int state) {
#pragma unused(state)
    UnionRoom_SetBeaconActivity(0xC);
    BeginNormalPaletteFade(0, 0x10, 0x10, 0, 0x10, 1, HEAP_ID_102);
    app->result = 1;
    SpinTradeGroup_UpdateDummy(app);
    return 3;
}

void SpinTradeGroup_HandleLinkEvent(SpinTradeGroupApp *app, int msg, int value) {
    switch (msg) {
    case 2:
        SpinTradeGroup_ClearMessage(app);
        break;
    case 7:
    case 8:
        break;
    case 0xD:
        if (app->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_102);
            app->yesNoMenu = NULL;
        }
        break;
    case 0x19:
        if (!SpinTradeGroup_IsPrinterDone(app->printerId)) {
            RemoveTextPrinter((u8)app->printerId);
        }
        SpinTradeGroup_PrintMessage(app, 0xC, 0);
        if (app->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_102);
            app->yesNoMenu = NULL;
        }
        break;
    case 0x13:
        if (app->leaveRequested == 1) {
            return;
        }
        BufferPlayersName(app->msgFormat, 0, CommPlayerInfo_GetProfile(value));
        if (value == Comm_GetLocalNetId()) {
            return;
        }
        if (app->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_102);
            app->yesNoMenu = NULL;
        }
        if (Comm_GetLocalNetId() == 0) {
            app->menu->leavingMask &= 0xFFFF ^ value;
        }
        break;
    case 0x1F:
        if (!SpinTradeGroup_IsPrinterDone(app->printerId)) {
            RemoveTextPrinter((u8)app->printerId);
        }
        if (app->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_102);
            app->yesNoMenu = NULL;
        }
        break;
    default:
        return;
    }
    app->subState = msg;
}

void SpinTradeGroup_HandleMemberEvent(SpinTradeGroupApp *app, int event, int netId) {
    if (app->subState == 3) {
        switch (event) {
        case 1: {
            SpinTradeGroupMenu *p;
            BufferPlayersName(app->msgFormat, 0, CommPlayerInfo_GetProfile(netId));
            SpinTradeGroup_SetMaxConnections(app, 1);
            app->subState = event;
            app->menu->participantCount = Comm_CountConnected();
            p = app->menu;
            p->leavingMask = 0;
            if (Comm_GetLocalNetId() == 0) {
                u32 v = 1;
                SpinTradeLink_SendCommand(app->menu, 7, &v, 1);
            }
            break;
        }
        case 0x13:
            BufferPlayersName(app->msgFormat, 0, CommPlayerInfo_GetProfile(netId));
            if (netId != Comm_GetLocalNetId()) {
                if (Comm_GetLocalNetId() == 0) {
                    SpinTradeGroupMenu *p = app->menu;
                    p->leavingMask = (0xFFFF ^ netId) & p->leavingMask;
                }
                app->subState = event;
            }
            break;
        default:
            GF_AssertFail();
            break;
        }
    } else if (event == 1) {
        app->menu->leavingMask = 0;
        app->menu->participantCount = Comm_CountConnected();
    }
}

void SpinTradeGroup_CycleHighlightColor(u16 *p) {
    u16 color;
    *p += 10;
    if (*p > 360) {
        *p = 0;
    }
    color = (u16)((((GF_SinDeg(*p) * 10) / 0x1000) + 15) << 5 | 29);
    GX_LoadOBJPltt(&color, 10, 2);
    GX_LoadOBJPltt(&color, 42, 2);
}

int SpinTradeGroup_CountProfiles(void) {
    int i;
    int count;
    count = 0;
    i = 0;
    for (; i < 5; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            count++;
        }
    }
    return count;
}

BOOL SpinTradeGroup_PrintMemberList(Window *window, u32 a1, u32 color, SpinTradeGroupApp *app) {
    int i;
    int sel;
    String *str;
    u32 tid;

    sel = Comm_GetLocalNetId();
    if (!SpinTradeGroup_UpdateMembers(app)) {
        return FALSE;
    }
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < 5; i++) {
        if (app->members[i].profile != 0) {
            tid = PlayerProfile_GetTrainerID_VisibleHalf(app->members[i].profile);
            PlayerName_FlatToString(app->members[i].profile, app->names[i]);
            BufferIntegerAsString(app->msgFormat, 0, (s32)tid, 5, (PrintingMode)2, TRUE);
            str = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->msgData, 1, HEAP_ID_102);
            if (sel == i) {
                AddTextPrinterParameterizedWithColor(window, 0, app->names[i], 5, i * 0x12 + 1, 0xFF, 0x20300, NULL);
                AddTextPrinterParameterizedWithColor(window, 0, str, 0x46, i * 0x12 + 1, 0xFF, 0x20300, NULL);
            } else {
                AddTextPrinterParameterizedWithColor(window, 0, app->names[i], 5, i * 0x12 + 1, 0xFF, color, NULL);
                AddTextPrinterParameterizedWithColor(window, 0, str, 0x46, i * 0x12 + 1, 0xFF, color, NULL);
            }
            String_Delete(str);
        }
    }
    CopyWindowToVram(window);
    return TRUE;
}

void SpinTradeGroup_HandleInputDummy(SpinTradeGroupApp *app) {
#pragma unused(app)
}

int SpinTradeGroup_CountMembers(void) {
    int i;
    int count;
    count = 0;
    i = 0;
    for (; i < 5; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            count++;
        }
    }
    return count;
}

u32 SpinTradeGroup_GetMemberMask(void) {
    int i;
    u32 mask;
    mask = 0;
    i = 0;
    for (; i < 5; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            mask |= 1 << i;
        }
    }
    return mask;
}

BOOL SpinTradeGroup_UpdateMembers(SpinTradeGroupApp *app) {
    int i;
    BOOL changed = FALSE;
    for (i = 0; i < 5; i++) {
        app->members[i].prevProfile = app->members[i].profile;
        app->members[i].profile = CommPlayerInfo_GetProfile(i);
        app->trainerIds[i].prev = app->trainerIds[i].cur;
        if (app->members[i].profile != 0) {
            app->trainerIds[i].cur.f.id = PlayerProfile_GetTrainerID(app->members[i].profile);
            app->trainerIds[i].cur.f.valid = 1;
        } else {
            app->trainerIds[i].cur.f.id = 0;
            app->trainerIds[i].cur.f.valid = 0;
        }
    }
    for (i = 0; i < 5; i++) {
        if (app->members[i].prevProfile != app->members[i].profile) {
            changed = TRUE;
            if (app->members[i].profile == 0) {
                app->memberStates[i] = 3;
            } else {
                app->memberStates[i] = 1;
            }
        } else if (app->trainerIds[i].cur.raw != app->trainerIds[i].prev.raw) {
            changed = TRUE;
            if (app->trainerIds[i].cur.f.valid == 0) {
                app->memberStates[i] = 3;
            } else {
                app->memberStates[i] = 1;
            }
        }
    }
    return changed;
}

void SpinTradeGroup_PrintMessage(SpinTradeGroupApp *app, int msgId, int instant) {
    String *str = String_New(0xB4, HEAP_ID_102);
    ReadMsgDataIntoString(app->msgData, msgId, str);
    StringExpandPlaceholders(app->msgFormat, app->msgString, str);
    String_Delete(str);
    FillWindowPixelBuffer(&app->msgWindow, 0xF);
    DrawFrameAndWindow2(&app->msgWindow, 0, 1, 0xA);
    if (instant == 0) {
        app->printerId = AddTextPrinterParameterized(&app->msgWindow, 1, app->msgString, 0, 0, SpinTradeGroup_GetTextSpeed(app), NULL);
    } else {
        AddTextPrinterParameterized(&app->msgWindow, 1, app->msgString, 0, 0, 0, NULL);
        app->printerId = 0xFF;
    }
}

BOOL SpinTradeGroup_IsPrinterDone(u32 printerId) {
    if (printerId == 0xFF) {
        return TRUE;
    }
    if (!TextPrinterCheckActive((u8)printerId)) {
        return TRUE;
    }
    return FALSE;
}

void SpinTradeGroup_ClearMessage(SpinTradeGroupApp *app) {
    ClearFrameAndWindow2(&app->msgWindow, 0);
}

void SpinTradeGroup_UpdateAvatars(SpinTradeGroupApp *app) {
    int i;
    int played = 0;
    for (i = 0; i < 5; i++) {
        switch (app->memberStates[i]) {
        case 0:
            break;
        case 1: {
            int gender;
            int avatar;
            if (app->members[i].profile == 0) {
                GF_AssertFail();
            }
            gender = PlayerProfile_GetTrainerGender(app->members[i].profile);
            avatar = PlayerProfile_GetAvatar(app->members[i].profile);
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(app->avatars[i], gender * 2 + 0x26);
            } else {
                SpinTradeGroup_LoadAvatar(app->chars, app->pltts, i, avatar, gender);
                Sprite_SetAnimCtrlSeq(app->avatars[i], i * 2 + 0x1B);
            }
            Sprite_SetDrawFlag(app->avatars[i], 1);
            app->memberStates[i] = 2;
            played = 1;
            break;
        }
        case 2:
            break;
        case 3:
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(app->avatars[i], PlayerProfile_GetTrainerGender(app->members[i].profile) * 2 + 0x27);
            } else {
                Sprite_SetAnimCtrlSeq(app->avatars[i], i * 2 + 0x1C);
            }
            app->memberStates[i] = 0;
            break;
        }
    }
    if (played) {
        PlaySE(0x64E);
    }
}

void SpinTradeGroup_LoadAvatarGraphics(SpinTradeGroupApp *app, NARC *narc) {
    app->plttFiles[0] = GfGfxLoader_GetPlttData(NARC_a_1_0_0, 9, (NNSG2dPaletteData **)&app->pltts[0], HEAP_ID_102);
    app->plttFiles[1] = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 7, (NNSG2dPaletteData **)&app->pltts[1], HEAP_ID_102);
    app->charFiles[0] = GfGfxLoader_GetCharData(NARC_a_1_0_0, 0x2B, TRUE, (NNSG2dCharacterData **)&app->chars[0], HEAP_ID_102);
    app->charFiles[1] = GfGfxLoader_GetCharDataFromOpenNarc(narc, 9, TRUE, (NNSG2dCharacterData **)&app->chars[1], HEAP_ID_102);
}

void SpinTradeGroup_LoadAvatar(NNSG2dCharacterData **chars, NNSG2dPaletteData **pltts, int slot, int avatar, int gender) {
    int idx = SpriteToUnionRoomAvatarIdx(gender, avatar);
    void *pltt = pltts[1]->pRawData;
    GX_LoadOBJ((u8 *)chars[1]->pRawData + idx * 0x600, gSpinTradeGroupAvatarCharOffsets[slot], 0x600);
    GX_LoadOBJPltt((u8 *)pltt + idx * 0x20, (slot + 7) * 0x20, 0x20);
}

void SpinTradeGroup_FreeAvatarGraphics(SpinTradeGroupApp *app) {
    Heap_Free(app->plttFiles[0]);
    Heap_Free(app->plttFiles[1]);
    Heap_Free(app->charFiles[0]);
    Heap_Free(app->charFiles[1]);
}

int SpinTradeGroup_GetTextSpeed(SpinTradeGroupApp *app) {
#pragma unused(app)
    return 1;
}

void SpinTradeGroup_SetMaxConnections(SpinTradeGroupApp *app, int extraEntries) {
    if (Comm_GetLocalNetId() == 0) {
        if (extraEntries == -1) {
            CommState_SetMaxEntries(1);
        } else {
            int v = extraEntries + Comm_CountConnected();
            if (v > 5) {
                v = 5;
            }
            CommState_SetMaxEntries(v);
        }
        if (extraEntries == -1) {
            app->maxEntriesMode = 2;
        } else if (extraEntries == 0) {
            app->maxEntriesMode = 1;
        } else {
            app->maxEntriesMode = 0;
        }
    }
}

int SpinTradeGroup_CheckMembers(SpinTradeGroupApp *app, int extraEntries) {
    int v = SpinTradeGroup_CountMembers();
    if (v > (int)app->menu->participantCount) {
        u8 x = 1;
        SpinTradeLink_SendCommand(app->menu, 7, &x, 1);
        app->joinPending = 1;
    } else {
        app->joinPending = 0;
    }
    if (v == (int)app->lastMemberCount) {
        return 1;
    }
    switch (v) {
    case 1:
        if ((int)Comm_CountConnected() > 1 || Wireless_GetConnectedBitmap() > 1) {
            return 1;
        }
        app->subState = 0x11;
        SpinTradeGroup_SetMaxConnections(app, -1);
        if (app->yesNoMenu != NULL) {
            Clear2dMenuWindowAndDelete(app->yesNoMenu, HEAP_ID_102);
            app->yesNoMenu = NULL;
        }
        return 2;
    case 2:
    case 3:
    case 4:
        UnionRoom_SetBeaconActivity(0xD);
        if (v < (int)app->lastMemberCount) {
            switch (app->maxEntriesMode) {
            case 0:
                SpinTradeGroup_SetMaxConnections(app, extraEntries);
                break;
            case 1:
                SpinTradeGroup_SetMaxConnections(app, 0);
                break;
            }
            app->menu->participantCount = Comm_CountConnected();
        }
        break;
    case 5:
        UnionRoom_SetBeaconActivity(0xC);
        SpinTradeGroup_SetMaxConnections(app, extraEntries);
        break;
    }
    app->lastMemberCount = SpinTradeGroup_CountMembers();
    return 1;
}

void SpinTradeGroup_SaveDummy(void *a0, void *a1) {
#pragma unused(a0)
#pragma unused(a1)
}
