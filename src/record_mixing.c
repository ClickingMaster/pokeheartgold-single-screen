#include "record_mixing.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "wireless_manager.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "union_room_talk.h"
#include "union_room_avatar.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define RECORD_MIXING_MAX_MEMBERS     5
#define RECORD_MIXING_BUFFER_SIZE     0xBC0
#define RECORD_MIXING_STATE_WAIT_TEXT 30

typedef struct RecordMixingMember {
    PlayerProfile *cur;
    PlayerProfile *prev;
} RecordMixingMember;

// Trainer id of the member in a slot, compared as one 64-bit value.
typedef union RecordMixingMemberKey {
    struct {
        u32 id;
        u32 valid;
    } s;

    u64 raw;
} RecordMixingMemberKey;

typedef struct RecordMixingMemberKeys {
    RecordMixingMemberKey cur;
    RecordMixingMemberKey prev;
} RecordMixingMemberKeys;

// Palette flash of the player sprite while records are mixed.
typedef struct RecordMixingFlash {
    u32 active;
    u32 frame;
    s32 timer;
    void *plttRaw;
    NNSG2dPaletteData *pltt;
} RecordMixingFlash;

// Also viewed by the comm command handlers in wireless_room.c (Room).
typedef struct RecordMixingApp {
    BgConfig *bgConfig;                                                      // 0x0000
    u32 unk_0004;                                                            // 0x0004
    RecordMixingArgs *args;                                                  // 0x0008
    RecordMixingFlash flash;                                                 // 0x000C
    SysTask *task;                                                           // 0x0020
    MessageFormat *msgFmt;                                                   // 0x0024
    MsgData *msgData;                                                        // 0x0028
    String *memberNames[RECORD_MIXING_MAX_MEMBERS];                          // 0x002C
    u32 unk_0040;                                                            // 0x0040
    String *messageText;                                                     // 0x0044
    String *titleText;                                                       // 0x0048
    u32 printerId;                                                           // 0x004C
    SpriteList *spriteList;                                                  // 0x0050
    G2dRenderer renderer;                                                    // 0x0054
    GF_2DGfxResMan *resMan[4];                                               // 0x017C
    u8 unk_018C[0x20];                                                       // 0x018C
    SpriteResource *charRes;                                                 // 0x01AC
    SpriteResource *plttRes;                                                 // 0x01B0
    SpriteResource *cellRes;                                                 // 0x01B4
    SpriteResource *animRes;                                                 // 0x01B8
    u8 unk_01BC[0x48];                                                       // 0x01BC
    SpriteResourcesHeader spriteHeader;                                      // 0x0204
    Sprite *playerSprite;                                                    // 0x0228
    Sprite *memberSprites[RECORD_MIXING_MAX_MEMBERS];                        // 0x022C
    u8 unk_0240[0x58];                                                       // 0x0240
    Window memberListWindow;                                                 // 0x0298
    Window messageWindow;                                                    // 0x02A8
    Window buttonWindows[2];                                                 // 0x02B8
    Window titleWindow;                                                      // 0x02D8
    YesNoPrompt *yesNoPrompt;                                                // 0x02E8
    void *avatarCharRaw[2];                                                  // 0x02EC
    NNSG2dCharacterData *avatarChar[2];                                      // 0x02F4
    void *avatarPlttRaw[2];                                                  // 0x02FC
    NNSG2dPaletteData *avatarPltt[2];                                        // 0x0304
    NNSG2dScreenData *buttonScreen;                                          // 0x030C
    void *buttonScreenRaw;                                                   // 0x0310
    MenuInputState menuInputState;                                           // 0x0314
    int state;                                                               // 0x0318
    int nextState;                                                           // 0x031C
    u32 unk_0320;                                                            // 0x0320
    s32 counter;                                                             // 0x0324
    u8 unk_0328[0x10];                                                       // 0x0328
    RecordMixingMember members[RECORD_MIXING_MAX_MEMBERS];                   // 0x0338
    int memberStates[RECORD_MIXING_MAX_MEMBERS];                             // 0x0360
    s32 unk_0374;                                                            // 0x0374
    u16 pulseAngle;                                                          // 0x0378
    u8 unk_037A[2];                                                          // 0x037A
    u16 *unk_037C;                                                           // 0x037C
    u8 unk_0380[8];                                                          // 0x0380
    u8 sendBuffer[RECORD_MIXING_BUFFER_SIZE];                                // 0x0388
    u8 receiveBuffers[RECORD_MIXING_MAX_MEMBERS][RECORD_MIXING_BUFFER_SIZE]; // 0x0F48
    u32 recordsReceived;                                                            // 0x4A08
    SaveData *saveData;                                                      // 0x4A0C
    u32 unk_4A10;                                                            // 0x4A10
    u8 isMixing;                                                             // 0x4A14
    u8 yesNoActive;                                                          // 0x4A15
    u16 unk_4A16;                                                            // 0x4A16
    int lastMemberCount;                                                            // 0x4A18
    int participantCount;                                                            // 0x4A1C
    int unk_4A20;                                                            // 0x4A20
    u32 leavingMask;                                                            // 0x4A24
    u8 leaveRequested;                                                             // 0x4A28
    s8 maxEntriesMode;                                                             // 0x4A29
    u8 unk_4A2A;                                                             // 0x4A2A
    u8 startMixingSent;                                                             // 0x4A2B
    int unk_4A2C;                                                            // 0x4A2C
    u16 unk_4A30;                                                            // 0x4A30
    s16 leaveWaitFrames;                                                            // 0x4A32
    s32 unk_4A34;                                                            // 0x4A34
    s32 buttonHighlightTimer;                                                // 0x4A38
    RecordMixingMemberKeys memberKeys[RECORD_MIXING_MAX_MEMBERS];            // 0x4A3C
} RecordMixingApp;                                                           // size: 0x4A8C

// No public header declares these.
void CommState_SetRecordMixingLinkMode(void);
void CommState_SetMaxEntries(int count); // defined with an s8 parameter; the callers pass an int untruncated
void Comm_InitSyncedSave(s32 *arg0);
s32 Comm_SyncedSave(SaveData *saveData, s32 arg1, u32 *arg2);
void UnionRoomObjects_ResetSlots(struct UnionRoomObjects *manager);
void RecordMixingLink_RegisterCommands(RecordMixingApp *app); // registers the comm command table (wireless_room.c)

// Record exchange, in the record data part of this overlay.
void RecordMixing_BuildSendBuffer(SaveData *saveData, void *dest);
void RecordMixing_MergeReceivedRecords(SaveData *saveData, u8 (*buffers)[RECORD_MIXING_BUFFER_SIZE]);

void RecordMixing_VBlankTask(SysTask *task, RecordMixingApp *app);
void RecordMixing_VBlankCB(RecordMixingApp *app);
void RecordMixing_SetGfxBanks(void);
void RecordMixing_InitBgs(BgConfig *bgConfig);
void RecordMixing_InitResources(RecordMixingApp *app, NARC *narc);
void RecordMixing_FreeResources(RecordMixingApp *app);
void RecordMixing_FreeBgs(BgConfig *bgConfig);
void RecordMixing_LoadBgGraphics(RecordMixingApp *app, NARC *narc);
void RecordMixing_FreeButtonScreen(RecordMixingApp *app);
void RecordMixing_InitObjTransfer(void);
void RecordMixing_InitSpriteGraphics(RecordMixingApp *app, NARC *narc);
void RecordMixing_CreateSprites(RecordMixingApp *app);
void RecordMixing_AddWindows(RecordMixingApp *app, OverlayManager *unused);
void RecordMixing_PrintCenteredString(Window *window, String *string, int textSpeed);
void RecordMixing_RemoveWindows(RecordMixingApp *app);
int RecordMixing_State_PrintGroupPrompt(RecordMixingApp *app, int state);
int RecordMixing_State_HandleGroupMenu(RecordMixingApp *app, int state);
BOOL RecordMixing_UpdateMenuInputState(RecordMixingApp *app);
void RecordMixing_HandleInput(RecordMixingApp *app);
void ov73_021E66F0(RecordMixingApp *app);
int ov73_021E66F4(RecordMixingApp *app, int state);
void RecordMixing_SetStateAfterText(RecordMixingApp *app, int nextState);
int RecordMixing_State_WaitText(RecordMixingApp *app, int state);
int ov73_021E6778(RecordMixingApp *app, int state);
void RecordMixing_ShowYesNo(RecordMixingApp *app);
void RecordMixing_HideYesNo(RecordMixingApp *app);
int RecordMixing_State_ShowCancelYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_HandleCancelYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_WaitLeaveReply(RecordMixingApp *app, int state);
int RecordMixing_State_SendLeave(RecordMixingApp *app, int state);
int ov73_021E6A70(RecordMixingApp *app, int state);
int RecordMixing_State_ShowConfirmYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_HandleConfirmYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_SendStartMixing(RecordMixingApp *app, int state);
void RecordMixing_StartFlash(RecordMixingApp *app);
int RecordMixing_State_WaitRecords(RecordMixingApp *app, int state);
int RecordMixing_State_MergeRecords(RecordMixingApp *app, int state);
int RecordMixing_State_StartSave(RecordMixingApp *app, int state);
int RecordMixing_State_Save(RecordMixingApp *app, int state);
int RecordMixing_State_WaitAfterSave(RecordMixingApp *app, int state);
int RecordMixing_State_PrintCanceled(RecordMixingApp *app, int state);
int RecordMixing_State_FadeOutAfterDelay(RecordMixingApp *app, int state);
int RecordMixing_State_ShowDisbandYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_HandleDisbandYesNo(RecordMixingApp *app, int state);
int RecordMixing_State_PrintDisband(RecordMixingApp *app, int state);
int RecordMixing_State_WaitDisband(RecordMixingApp *app, int state);
int RecordMixing_State_StartExitSync(RecordMixingApp *app, int state);
int RecordMixing_State_WaitExitSync(RecordMixingApp *app, int state);
int RecordMixing_State_PrintMemberUnavailable(RecordMixingApp *app, int state);
int RecordMixing_State_WaitMemberUnavailable(RecordMixingApp *app, int state);
int RecordMixing_State_PrintMemberLeft(RecordMixingApp *app, int state);
int RecordMixing_State_StartMemberLeftWait(RecordMixingApp *app, int state);
int RecordMixing_State_WaitMemberLeft(RecordMixingApp *app, int state);
void RecordMixing_SetStateFromLink(RecordMixingApp *app, int state, int netId);
void RecordMixing_SetStateFromLinkInMenu(RecordMixingApp *app, int state, int netId);
void RecordMixing_SetButtonState(RecordMixingApp *app, int button, int buttonState);
void RecordMixing_PrintButtons(RecordMixingApp *app);
void RecordMixing_PulsePalette(u16 *angle);
int ov73_021E7340(void);
BOOL RecordMixing_PrintMemberList(Window *window, int unused, u32 color, RecordMixingApp *app);
void ov73_021E7468(RecordMixingApp *app);
int RecordMixing_CountMembers(void);
int ov73_021E7488(RecordMixingApp *app);
BOOL RecordMixing_UpdateMembers(RecordMixingApp *app);
void RecordMixing_PrintMessage(RecordMixingApp *app, int msgId, BOOL immediate);
BOOL RecordMixing_IsPrinterDone(u32 printerId);
void RecordMixing_ClearMessageWindow(RecordMixingApp *app);
void RecordMixing_UpdateMemberSprites(RecordMixingApp *app);
void RecordMixing_LoadAvatarGraphics(RecordMixingApp *app, NARC *narc);
void RecordMixing_LoadAvatar(NNSG2dCharacterData **charData, NNSG2dPaletteData **plttData, u32 slot, int trainerClass, int gender);
void RecordMixing_FreeAvatarGraphics(RecordMixingApp *app);
int RecordMixing_GetTextSpeed(RecordMixingApp *app);
void RecordMixing_UpdateMaxEntries(RecordMixingApp *app, int index);
int RecordMixing_LeaderUpdateMembers(RecordMixingApp *app, int arg1);

extern const u8 sRecordMixingButtonX[];
extern const u16 sRecordMixingAvatarVramOffsets[];
extern const u8 sRecordMixingFlashDurations[];
extern const u8 sRecordMixingFlashPalettes[];
extern const TouchscreenHitbox sRecordMixingButtonHitboxes[];
extern const GraphicsModes sRecordMixingGraphicsModes;
extern const ObjCharTransferTemplate sRecordMixingObjCharTransferTemplate;
extern const u16 sRecordMixingSpritePositions[][2];
extern const BgTemplate sRecordMixingBgTemplate_Main0;
extern const BgTemplate sRecordMixingBgTemplate_Main1;
extern const BgTemplate sRecordMixingBgTemplate_Sub0;
extern const BgTemplate sRecordMixingBgTemplate_Sub1;
extern const GraphicsBanks sRecordMixingGraphicsBanks;
extern int (*sRecordMixingStateFuncs[])(RecordMixingApp *app, int state);

BOOL RecordMixing_Init(OverlayManager *man, int *state) {
    RecordMixingApp *app;
    NARC *narc;

    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        Heap_Create(HEAP_ID_3, HEAP_ID_50, 0x41000);
        narc = NARC_New(NARC_application_record_record, HEAP_ID_50);
        app = OverlayManager_CreateAndGetData(man, sizeof(RecordMixingApp), HEAP_ID_50);
        memset(app, 0, sizeof(RecordMixingApp));
        app->bgConfig = BgConfig_Alloc(HEAP_ID_50);
        app->args = OverlayManager_GetArgs(man);
        app->saveData = app->args->saveData;
        app->menuInputState = MenuInputStateMgr_GetState(app->args->menuInputStatePtr);
        app->msgFmt = MessageFormat_New(HEAP_ID_50);
        app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0422_bin, HEAP_ID_50);
        FontID_Alloc(4, HEAP_ID_50);
        SetKeyRepeatTimers(4, 8);
        RecordMixing_SetGfxBanks();
        RecordMixing_InitBgs(app->bgConfig);
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)17, (enum FadeType)17, RGB_BLACK, 16, 1, HEAP_ID_50);
        RecordMixing_LoadBgGraphics(app, narc);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(2);
        Main_SetVBlankIntrCB((GFIntrCB)RecordMixing_VBlankCB, app);
        RecordMixing_InitResources(app, narc);
        RecordMixing_InitObjTransfer();
        RecordMixing_InitSpriteGraphics(app, narc);
        RecordMixing_CreateSprites(app);
        RecordMixing_AddWindows(app, man);
        RecordMixing_PrintButtons(app);
        Sound_SetSceneAndPlayBGM(0x34, 0, 0);
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        RecordMixingLink_RegisterCommands(app);
        CommState_SetRecordMixingLinkMode();
        CommState_SetMaxEntries(3);
        if (!Comm_GetLocalNetId()) {
            UnionRoom_SetBeaconActivity(2);
        }
        WirelessStatusIcon_Create();
        RecordMixing_BuildSendBuffer(app->saveData, app->sendBuffer);
        app->task = SysTask_CreateOnVBlankQueue((SysTaskFunc)RecordMixing_VBlankTask, app, 5);
        NARC_Delete(narc);
        (*state)++;
        break;
    case 1:
        OverlayManager_GetData(man);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL RecordMixing_Main(OverlayManager *man, int *state) {
    RecordMixingApp *app = OverlayManager_GetData(man);

    if (Comm_GetLocalNetId() == 0 && app->leavingMask != 0) {
        app->leavingMask &= Wireless_GetConnectedBitmap();
    }
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
            if (Comm_GetLocalNetId() && RecordMixing_CountMembers() > 2) {
                Comm_SendCommand(114, NULL, 0);
            }
        }
        break;
    case 1:
        if (sRecordMixingStateFuncs[app->state]) {
            *state = sRecordMixingStateFuncs[app->state](app, *state);
        }
        if (app->isMixing == 0) {
            RecordMixing_PrintMemberList(&app->memberListWindow, 0, 0x10300, app);
        }
        RecordMixing_UpdateMemberSprites(app);
        if (Comm_GetLocalNetId() == 0) {
            int value = RecordMixing_LeaderUpdateMembers(app, 1);
            if (*state == 1) {
                *state = value;
            }
        }
        break;
    case 2:
        if (sRecordMixingStateFuncs[app->state]) {
            *state = sRecordMixingStateFuncs[app->state](app, *state);
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

BOOL RecordMixing_Exit(OverlayManager *man, int *state) {
    RecordMixingApp *app = OverlayManager_GetData(man);
    s32 i;

    SysTask_Destroy(app->task);
    SpriteTransfer_DeleteCharTransferTask(app->charRes);
    SpriteTransfer_DeletePlttTransferTask(app->plttRes);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(app->resMan[i]);
    }
    SpriteList_Delete(app->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    RecordMixing_RemoveWindows(app);
    RecordMixing_FreeButtonScreen(app);
    RecordMixing_FreeBgs(app->bgConfig);
    CommState_SetMaxEntries(2);
    CommState_ResumeAlternateSearch();
    UnionRoomObjects_ResetSlots(app->args->unk4);
    GF_TouchpadStopSampling();
    FontID_Release(4);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFmt);
    MenuInputStateMgr_SetState(app->args->menuInputStatePtr, app->menuInputState);
    RecordMixing_FreeResources(app);
    OverlayManager_FreeData(man);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    UnionRoom_SetBeaconActivity(0);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_50);
    return TRUE;
}

void RecordMixing_VBlankTask(SysTask *task, RecordMixingApp *app) {
    RecordMixingFlash *flash = &app->flash;
    if (app->flash.active) {
        if (flash->timer > sRecordMixingFlashDurations[flash->frame * 2]) {
            flash->timer = 0;
            flash->frame++;
            if (sRecordMixingFlashPalettes[flash->frame * 2] == 0xFF) {
                flash->frame = 0;
            }
            GX_LoadOBJPltt((u8 *)flash->pltt->pRawData + sRecordMixingFlashPalettes[flash->frame * 2] * 0x20, 0, 0x20);
        } else {
            app->flash.timer++;
        }
        RecordMixing_PulsePalette(&app->pulseAngle);
    }
}

void RecordMixing_VBlankCB(RecordMixingApp *app) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void RecordMixing_SetGfxBanks(void) {
    GraphicsBanks banks = sRecordMixingGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void RecordMixing_InitBgs(BgConfig *bgConfig) {
    {
        GraphicsModes modes = sRecordMixingGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = sRecordMixingBgTemplate_Sub0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = sRecordMixingBgTemplate_Sub1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }
    {
        BgTemplate template = sRecordMixingBgTemplate_Main0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate template = sRecordMixingBgTemplate_Main1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_50);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_50);
}

void RecordMixing_InitResources(RecordMixingApp *app, NARC *narc) {
    s32 i;

    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        app->memberNames[i] = String_New(8, HEAP_ID_50);
        app->members[i].cur = NULL;
        app->members[i].prev = NULL;
        app->memberKeys[i].cur.s.id = 0;
        app->memberKeys[i].cur.s.valid = 0;
        app->memberKeys[i].prev.s.id = 0;
        app->memberKeys[i].prev.s.valid = 0;
        app->memberStates[i] = 0;
    }
    app->messageText = String_New(180, HEAP_ID_50);
    app->titleText = String_New(40, HEAP_ID_50);
    app->state = 0;
    ReadMsgDataIntoString(app->msgData, 17, app->titleText);
    RecordMixing_LoadAvatarGraphics(app, narc);
    app->unk_037C = sub_0205B4A4(HEAP_ID_50);
    app->flash.active = 0;
    app->flash.timer = 0;
    app->flash.frame = 0;
    app->flash.plttRaw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 7, &app->flash.pltt, HEAP_ID_50);
    app->lastMemberCount = 0;
    app->participantCount = 2;
    app->yesNoActive = 0;
}

void RecordMixing_FreeResources(RecordMixingApp *app) {
    s32 i;

    RecordMixing_FreeAvatarGraphics(app);
    Heap_Free(app->flash.plttRaw);
    Heap_Free(app->unk_037C);
    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        String_Delete(app->memberNames[i]);
    }
    String_Delete(app->titleText);
    String_Delete(app->messageText);
}

void RecordMixing_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(bgConfig);
}

#pragma force_active on

void RecordMixing_LoadBgGraphics(RecordMixingApp *app, NARC *narc) {
    BgConfig *bgConfig = app->bgConfig;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_50);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_50);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_50);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_50);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_50);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_50);
    app->buttonScreenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 3, TRUE, &app->buttonScreen, HEAP_ID_50);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x180, HEAP_ID_50);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_50);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x180, HEAP_ID_50);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_50);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 11, Options_GetFrame(app->args->options), HEAP_ID_50);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 10, 0, HEAP_ID_50);
}

#pragma force_active reset

void RecordMixing_FreeButtonScreen(RecordMixingApp *app) {
    Heap_Free(app->buttonScreenRaw);
    app->buttonScreenRaw = NULL;
}

void RecordMixing_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sRecordMixingObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_50);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void RecordMixing_InitSpriteGraphics(RecordMixingApp *app, NARC *narc) {
    int i;
    RecordMixingApp *walk;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_50);
    app->spriteList = G2dRenderer_Init(30, &app->renderer, HEAP_ID_50);
    G2dRenderer_SetSubSurfaceCoords(&app->renderer, 0, FX32_CONST(256));
    // walks resMan[] with a pointer to the work struct shifted by 4 bytes per entry
    i = 0;
    walk = app;
    while (i < 4) {
        walk->resMan[0] = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_50);
        i++;
        walk = (RecordMixingApp *)((u8 *)walk + 4);
    }
    app->charRes = AddCharResObjFromOpenNarc(app->resMan[GF_GFX_RES_TYPE_CHAR], narc, 8, TRUE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_50);
    app->plttRes = AddPlttResObjFromOpenNarc(app->resMan[GF_GFX_RES_TYPE_PLTT], narc, 7, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 15, HEAP_ID_50);
    app->cellRes = AddCellOrAnimResObjFromOpenNarc(app->resMan[GF_GFX_RES_TYPE_CELL], narc, 9, TRUE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_50);
    app->animRes = AddCellOrAnimResObjFromOpenNarc(app->resMan[GF_GFX_RES_TYPE_ANIM], narc, 10, TRUE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_50);
    SpriteTransfer_CreateCharTransferTask(app->charRes);
    SpriteTransfer_CreateExtPlttTransferTask(app->plttRes);
}

#pragma force_active on

void RecordMixing_CreateSprites(RecordMixingApp *app) {
    SpriteTemplate template;
    int i;
    const u16 *pos;
    RecordMixingApp *walk;

    CreateSpriteResourcesHeader(&app->spriteHeader, 2, 2, 2, 2, -1, -1, 0, 1, app->resMan[GF_GFX_RES_TYPE_CHAR], app->resMan[GF_GFX_RES_TYPE_PLTT], app->resMan[GF_GFX_RES_TYPE_CELL], app->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = app->spriteList;
    template.header = &app->spriteHeader;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_50;
    template.position.x = FX32_CONST(84);
    template.position.y = FX32_CONST(104);
    app->playerSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->playerSprite, TRUE);
    Sprite_SetDrawFlag(app->playerSprite, TRUE);
    Sprite_SetDrawPriority(app->playerSprite, 100);
    // walks memberSprites[] with a pointer to the work struct shifted by 4 bytes per entry
    for (i = 0, pos = &sRecordMixingSpritePositions[0][0], walk = app; i < RECORD_MIXING_MAX_MEMBERS; i++, pos += 2, walk = (RecordMixingApp *)((u8 *)walk + 4)) {
        template.position.x = pos[2] << FX32_SHIFT;
        template.position.y = pos[3] << FX32_SHIFT;
        walk->memberSprites[0] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(walk->memberSprites[0], TRUE);
        Sprite_SetAnimCtrlSeq(walk->memberSprites[0], (i - 1) * 2 + 27);
        Sprite_SetDrawFlag(walk->memberSprites[0], FALSE);
    }
    template.position.x = 24 << FX32_SHIFT;
    template.position.y = 416 << FX32_SHIFT;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void RecordMixing_AddWindows(RecordMixingApp *app, OverlayManager *unused) {
    AddWindowParameterized(app->bgConfig, &app->messageWindow, GF_BG_LYR_MAIN_0, 2, 1, 27, 4, 12, 0x5B);
    FillWindowPixelBuffer(&app->messageWindow, 15);
    AddWindowParameterized(app->bgConfig, &app->buttonWindows[0], GF_BG_LYR_MAIN_0, 16, 21, 8, 2, 9, 0xC7);
    FillWindowPixelBuffer(&app->buttonWindows[0], 0);
    AddWindowParameterized(app->bgConfig, &app->buttonWindows[1], GF_BG_LYR_MAIN_0, 24, 21, 8, 2, 9, 0x133);
    FillWindowPixelBuffer(&app->buttonWindows[1], 0);
    AddWindowParameterized(app->bgConfig, &app->titleWindow, GF_BG_LYR_SUB_0, 3, 1, 26, 2, 9, 1);
    RecordMixing_PrintCenteredString(&app->titleWindow, app->titleText, 0);
    AddWindowParameterized(app->bgConfig, &app->memberListWindow, GF_BG_LYR_SUB_0, 8, 7, 16, 11, 9, 0x35);
    FillWindowPixelBuffer(&app->memberListWindow, 0);
    RecordMixing_PrintMemberList(&app->memberListWindow, 0, 0x10300, app);
    app->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_50);
}

void RecordMixing_PrintCenteredString(Window *window, String *string, int textSpeed) {
    int width = FontID_String_GetWidth(1, string, 0);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 1, string, (208 - width) / 2, 1, textSpeed, 0x10400, NULL);
}

void RecordMixing_RemoveWindows(RecordMixingApp *app) {
    YesNoPrompt_Destroy(app->yesNoPrompt);
    RemoveWindow(&app->memberListWindow);
    RemoveWindow(&app->titleWindow);
    RemoveWindow(&app->messageWindow);
    RemoveWindow(&app->buttonWindows[0]);
    RemoveWindow(&app->buttonWindows[1]);
}

int RecordMixing_State_PrintGroupPrompt(RecordMixingApp *app, int state) {
    CommState_SetErrorWatch(0, 1);
    if (Comm_GetLocalNetId() == 0) {
        if (Comm_CountConnected() >= 2) {
            RecordMixing_PrintMessage(app, 2, FALSE);
        }
    } else {
        RecordMixing_PrintMessage(app, 8, FALSE);
    }
    RecordMixing_SetStateAfterText(app, 3);
    return state;
}

int RecordMixing_State_HandleGroupMenu(RecordMixingApp *app, int state) {
    RecordMixing_HandleInput(app);
    if (Comm_GetLocalNetId() == 0) {
        if (ov73_021E7340() != 1) {
            ov73_021E7468(app);
        }
    } else {
        ov73_021E7468(app);
    }
    return state;
}

BOOL RecordMixing_UpdateMenuInputState(RecordMixingApp *app) {
    if (app->menuInputState == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys != 0) {
            app->menuInputState = MENU_INPUT_STATE_BUTTONS;
            return TRUE;
        }
    } else {
        if (gSystem.heldKeys != 0) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            app->menuInputState = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

#pragma opt_common_subs off
#pragma force_active on

void RecordMixing_HandleInput(RecordMixingApp *app) {
    u8 button = 0;

    struct {
        u8 send[3];
        u16 pixel;
    } packet;

    int index;

    RecordMixing_UpdateMenuInputState(app);
    index = TouchscreenHitbox_FindRectAtTouchNew(sRecordMixingButtonHitboxes);
    packet.pixel = 0xFFD5;
    if (index != -1 && DoesPixelAtScreenXYMatchPtrVal(app->bgConfig, GF_BG_LYR_MAIN_1, gSystem.touchX, gSystem.touchY, &packet.pixel)) {
        button = (u8)(index + 1);
    }
    if ((gSystem.newKeys & PAD_BUTTON_A) || button == 1) {
        if (!Comm_GetLocalNetId()) {
            if (app->participantCount == RecordMixing_CountMembers() && app->leavingMask == 0) {
                packet.send[2] = 1;
                RecordMixing_PrintMessage(app, 3, FALSE);
                RecordMixing_SetStateAfterText(app, 22);
                Comm_SendCommand(117, &packet.send[2], 1);
                RecordMixing_UpdateMaxEntries(app, 0);
                PlaySE(SEQ_SE_DP_SELECT);
                RecordMixing_SetButtonState(app, 0, 1);
                ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
            } else {
                PlaySE(SEQ_SE_DP_CUSTOM06);
            }
        }
    } else if ((gSystem.newKeys & PAD_BUTTON_B) || button == 2) {
        if (Comm_GetLocalNetId()) {
            if (app->unk_4A10 == 0) {
                RecordMixing_PrintMessage(app, 4, FALSE);
                RecordMixing_SetStateAfterText(app, 4);
                PlaySE(SEQ_SE_GS_GEARCANCEL);
            } else if (app->buttonHighlightTimer < 0) {
                app->buttonHighlightTimer = 2;
                PlaySE(SEQ_SE_DP_CUSTOM06);
            }
        } else {
            if (app->participantCount == Comm_CountConnected() && app->leavingMask == 0) {
                packet.send[1] = 1;
                RecordMixing_PrintMessage(app, 4, FALSE);
                RecordMixing_SetStateAfterText(app, 4);
                Comm_SendCommand(117, &packet.send[1], 1);
                RecordMixing_UpdateMaxEntries(app, 0);
                PlaySE(SEQ_SE_GS_GEARCANCEL);
                RecordMixing_SetButtonState(app, 1, 1);
                ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
            } else {
                PlaySE(SEQ_SE_DP_CUSTOM06);
            }
        }
    } else if (app->unk_4A20 == 0 && !Comm_GetLocalNetId() && app->participantCount == Comm_CountConnected()) {
        packet.send[0] = 0;
        Comm_SendCommand(117, packet.send, 1);
    }
    if (app->buttonHighlightTimer >= 0) {
        app->buttonHighlightTimer--;
    }
    if (app->buttonHighlightTimer == 0) {
        RecordMixing_SetButtonState(app, 1, 0);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
    }
}

#pragma force_active reset
#pragma opt_common_subs reset

void ov73_021E66F0(RecordMixingApp *app) {
}

int ov73_021E66F4(RecordMixingApp *app, int state) {
    RecordMixing_SetStateAfterText(app, 2);
    ov73_021E66F0(app);
    return state;
}

void RecordMixing_SetStateAfterText(RecordMixingApp *app, int nextState) {
    app->state = RECORD_MIXING_STATE_WAIT_TEXT;
    app->nextState = nextState;
}

int RecordMixing_State_WaitText(RecordMixingApp *app, int state) {
    if (RecordMixing_IsPrinterDone(app->printerId)) {
        app->state = app->nextState;
        if (app->nextState == 3) {
            if (Comm_GetLocalNetId() == 0) {
                RecordMixing_SetButtonState(app, 0, 0);
            }
            app->buttonHighlightTimer = -1;
            RecordMixing_SetButtonState(app, 1, 0);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
        }
    }
    ov73_021E66F0(app);
    return state;
}

int ov73_021E6778(RecordMixingApp *app, int state) {
    u32 data;

    if (Comm_GetLocalNetId() == 0) {
        data = 0;
        Comm_SendCommand(117, &data, 1);
    }
    app->state = 3;
    ov73_021E66F0(app);
    return state;
}

void RecordMixing_ShowYesNo(RecordMixingApp *app) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = app->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 31;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 6;
    template.ignoreTouchFlag = app->menuInputState;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
    app->yesNoActive = 1;
    if (Comm_GetLocalNetId() == 0) {
        RecordMixing_SetButtonState(app, 0, 2);
    }
    RecordMixing_SetButtonState(app, 1, 2);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
}

void RecordMixing_HideYesNo(RecordMixingApp *app) {
    if (app->yesNoActive != 0) {
        app->menuInputState = YesNoPrompt_IsInTouchMode(app->yesNoPrompt);
        YesNoPrompt_Reset(app->yesNoPrompt);
        app->yesNoActive = 0;
        if (Comm_GetLocalNetId() == 0) {
            RecordMixing_SetButtonState(app, 0, 0);
        }
        app->buttonHighlightTimer = -1;
        RecordMixing_SetButtonState(app, 1, 0);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
    }
}

int RecordMixing_State_ShowCancelYesNo(RecordMixingApp *app, int state) {
    RecordMixing_ShowYesNo(app);
    app->state = 5;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_HandleCancelYesNo(RecordMixingApp *app, int state) {
    u32 noData;
    u8 data[4];
    int input;

    if (Comm_GetLocalNetId() != 0) {
        if (app->unk_4A10 != 0) {
            if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
                PlaySE(SEQ_SE_DP_CUSTOM06);
            }
            ov73_021E66F0(app);
            return state;
        }
    } else {
        if (app->leavingMask != 0) {
            if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
                PlaySE(SEQ_SE_DP_CUSTOM06);
            }
            ov73_021E66F0(app);
            return state;
        }
    }
    if (RecordMixing_CountMembers() != Comm_CountConnected()) {
        ov73_021E66F0(app);
        return state;
    }
    input = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (input != YESNORESPONSE_WAIT) {
        if (input == YESNORESPONSE_NO) {
            if (Comm_GetLocalNetId() == 0) {
                noData = 0;
                Comm_SendCommand(117, &noData, 1);
                RecordMixing_UpdateMaxEntries(app, 1);
            }
            RecordMixing_SetStateAfterText(app, 0);
        } else if (Comm_GetLocalNetId() == 0) {
            RecordMixing_SetStateAfterText(app, 11);
            RecordMixing_PrintMessage(app, 14, FALSE);
        } else {
            MI_CpuFill8(data, 0, 4);
            data[2] = 0;
            data[0] = Comm_GetLocalNetId();
            app->leaveRequested = 1;
            app->leaveWaitFrames = 0;
            app->state = 6;
            Comm_SendCommand(112, data, 4);
        }
        RecordMixing_HideYesNo(app);
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_WaitLeaveReply(RecordMixingApp *app, int state) {
    app->leaveWaitFrames = 0;
    ov73_021E66F0(app);
    return state;
}

#pragma force_active on

int RecordMixing_State_SendLeave(RecordMixingApp *app, int state) {
    u8 data[4];

    if (app->unk_4A30 != Comm_CountConnected() || app->unk_4A30 != RecordMixing_CountMembers()) {
        app->leaveWaitFrames = 0;
        app->state = 8;
        ov73_021E66F0(app);
        return state;
    }
    app->leaveWaitFrames++;
    if (app->leaveWaitFrames > 30) {
        MI_CpuFill8(data, 0, 4);
        data[2] = 1;
        data[0] = Comm_GetLocalNetId();
        Comm_SendCommand(112, data, 4);
        app->leaveWaitFrames = 0;
        app->state = 9;
    }
    ov73_021E66F0(app);
    return state;
}

#pragma force_active reset

int ov73_021E6A70(RecordMixingApp *app, int state) {
    app->leaveRequested = 0;
    RecordMixing_SetStateAfterText(app, 0);
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_ShowConfirmYesNo(RecordMixingApp *app, int state) {
    RecordMixing_ShowYesNo(app);
    app->state = 23;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_HandleConfirmYesNo(RecordMixingApp *app, int state) {
    u32 data;
    int input;

    if (app->participantCount != RecordMixing_CountMembers() || app->leavingMask != 0) {
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
        }
        ov73_021E66F0(app);
        return state;
    }
    input = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (input != YESNORESPONSE_WAIT) {
        RecordMixing_HideYesNo(app);
        if (input == YESNORESPONSE_NO) {
            data = 0;
            Comm_SendCommand(117, &data, 1);
            RecordMixing_UpdateMaxEntries(app, 1);
            RecordMixing_SetStateAfterText(app, 0);
        } else {
            RecordMixing_SetButtonState(app, 1, 2);
            RecordMixing_SetButtonState(app, 0, 2);
            if (Comm_GetLocalNetId() == 0) {
                app->state = 24;
                app->unk_4A2A = RecordMixing_CountMembers();
            } else {
                GF_AssertFail();
            }
        }
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_SendStartMixing(RecordMixingApp *app, int state) {
    if (app->startMixingSent == 0 && Comm_SendCommand(115, NULL, 0) == TRUE) {
        app->startMixingSent = 1;
    }
    return state;
}

void RecordMixing_StartFlash(RecordMixingApp *app) {
    ReadMsgDataIntoString(app->msgData, 18, app->titleText);
    RecordMixing_PrintCenteredString(&app->titleWindow, app->titleText, 0);
    Sprite_SetAnimCtrlSeq(app->playerSprite, 37);
    app->flash.active = 1;
}

int RecordMixing_State_WaitRecords(RecordMixingApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        Comm_CountConnected();
    }
    if (app->recordsReceived == Comm_CountConnected()) {
        app->state = 26;
    }
    return state;
}

int RecordMixing_State_MergeRecords(RecordMixingApp *app, int state) {
    RecordMixing_MergeReceivedRecords(app->saveData, app->receiveBuffers);
    app->state = 27;
    return state;
}

int RecordMixing_State_StartSave(RecordMixingApp *app, int state) {
    gSystem.softResetDisabled = TRUE;
    GameStats_AddScore(app->args->gameStats, 19);
    Comm_InitSyncedSave(&app->unk_0374);
    app->state = 28;
    return state;
}

int RecordMixing_State_Save(RecordMixingApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        Comm_CountConnected();
    }
    if (Comm_SyncedSave(app->args->saveData, 2, (u32 *)&app->unk_0374)) {
        StopSE(SEQ_SE_DP_F209, 8);
        RecordMixing_PrintMessage(app, 13, FALSE);
        RecordMixing_SetStateAfterText(app, 29);
        Sprite_SetAnimCtrlSeq(app->playerSprite, 0);
        app->flash.active = 0;
        app->counter = 0;
        gSystem.softResetDisabled = FALSE;
        app->isMixing = 0;
    }
    return state;
}

int RecordMixing_State_WaitAfterSave(RecordMixingApp *app, int state) {
    if (app->counter++ > 60) {
        app->state = 15;
    }
    return state;
}

int RecordMixing_State_PrintCanceled(RecordMixingApp *app, int state) {
    RecordMixing_PrintMessage(app, 5, FALSE);
    RecordMixing_SetStateAfterText(app, 10);
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_FadeOutAfterDelay(RecordMixingApp *app, int state) {
    app->counter++;
    if (app->counter > 60) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)16, (enum FadeType)16, RGB_BLACK, 16, 1, HEAP_ID_50);
        state = 3;
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_ShowDisbandYesNo(RecordMixingApp *app, int state) {
    RecordMixing_ShowYesNo(app);
    app->state = 12;
    ov73_021E66F0(app);
    return state;
}

#pragma force_active on

int RecordMixing_State_HandleDisbandYesNo(RecordMixingApp *app, int state) {
    int input;
    int data;

    if (app->participantCount != RecordMixing_CountMembers() || app->leavingMask) {
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
        }
        ov73_021E66F0(app);
        return state;
    }
    input = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (input) {
        if (input == YESNORESPONSE_NO) {
            data = 0;
            app->state = 0;
            Comm_SendCommand(117, &data, 1);
            RecordMixing_UpdateMaxEntries(app, 1);
        } else {
            app->state = 13;
            Comm_SendCommand(113, NULL, 0);
            BufferPlayersName(app->msgFmt, 0, CommPlayerInfo_GetProfile(0));
        }
        RecordMixing_HideYesNo(app);
    }
    ov73_021E66F0(app);
    return state;
}

#pragma force_active reset

int RecordMixing_State_PrintDisband(RecordMixingApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        RecordMixing_PrintMessage(app, 5, FALSE);
    } else {
        PlayerProfile *leader = CommPlayerInfo_GetProfile(0);
        BufferPlayersName(app->msgFmt, 0, leader);
        RecordMixing_PrintMessage(app, 16, FALSE);
    }
    RecordMixing_SetStateAfterText(app, 14);
    app->counter = 0;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_WaitDisband(RecordMixingApp *app, int state) {
    app->counter++;
    if (app->counter > 45) {
        app->state = 15;
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_StartExitSync(RecordMixingApp *app, int state) {
    LinkSync_StartSync(201);
    app->state = 16;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_WaitExitSync(RecordMixingApp *app, int state) {
    if (LinkSync_IsSynced(201) != FALSE) {
        CommState_SetErrorWatch(0, 0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)16, (enum FadeType)16, RGB_BLACK, 16, 1, HEAP_ID_50);
        state = 3;
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_PrintMemberUnavailable(RecordMixingApp *app, int state) {
    if (RecordMixing_IsPrinterDone(app->printerId) != FALSE) {
        RecordMixing_PrintMessage(app, 9, FALSE);
    }
    app->state = 18;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_WaitMemberUnavailable(RecordMixingApp *app, int state) {
    if (RecordMixing_IsPrinterDone(app->printerId) != FALSE) {
        app->state = 10;
    }
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_PrintMemberLeft(RecordMixingApp *app, int state) {
    if (Comm_GetLocalNetId() == 0) {
        RecordMixing_SetButtonState(app, 0, 2);
    }
    app->buttonHighlightTimer = -1;
    RecordMixing_SetButtonState(app, 1, 2);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
    if (app->printerId != 0xFF && RecordMixing_IsPrinterDone(app->printerId) == FALSE) {
        RemoveTextPrinter((u8)app->printerId);
    }
    RecordMixing_PrintMessage(app, 15, TRUE);
    app->state = 20;
    if (Comm_GetLocalNetId() == 0) {
        RecordMixing_UpdateMaxEntries(app, 0);
    }
    app->unk_4A2C = Comm_CountConnected();
    app->unk_4A34 = 300;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_StartMemberLeftWait(RecordMixingApp *app, int state) {
    if (app->unk_4A2C != 0) {
        int connected = Comm_CountConnected();
        if (app->unk_4A2C != connected) {
            app->unk_4A2C = 0;
        }
    }
    app->state = 21;
    app->counter = 0;
    ov73_021E66F0(app);
    return state;
}

int RecordMixing_State_WaitMemberLeft(RecordMixingApp *app, int state) {
    if (app->unk_4A2C != 0) {
        int connected = Comm_CountConnected();
        if (app->unk_4A2C != connected) {
            app->unk_4A2C = 0;
        }
    }
    app->unk_4A34--;
    if (app->unk_4A34 < 0) {
        app->unk_4A2C = 0;
    }
    app->counter++;
    if (app->counter > 60 && app->unk_4A2C == 0) {
        app->state = 0;
        if (Comm_GetLocalNetId() == 0) {
            RecordMixing_UpdateMaxEntries(app, 1);
        }
    }
    ov73_021E66F0(app);
    return state;
}

void RecordMixing_SetStateFromLink(RecordMixingApp *app, int state, int netId) {
    switch (state) {
    case 2:
        RecordMixing_ClearMessageWindow(app);
        break;
    case 13:
        RecordMixing_HideYesNo(app);
        break;
    case 25:
        if (!RecordMixing_IsPrinterDone(app->printerId)) {
            RemoveTextPrinter((u8)app->printerId);
        }
        RecordMixing_SetButtonState(app, 1, 2);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
        RecordMixing_PrintMessage(app, 12, FALSE);
        RecordMixing_HideYesNo(app);
        break;
    case 19:
        if (app->leaveRequested == 1) {
            return;
        }
        BufferPlayersName(app->msgFmt, 0, CommPlayerInfo_GetProfile(netId));
        if (netId == Comm_GetLocalNetId()) {
            return;
        }
        RecordMixing_HideYesNo(app);
        if (Comm_GetLocalNetId() == 0) {
            app->leavingMask &= netId ^ 0xFFFF;
        }
        break;
    case 7:
    case 8:
        break;
    default:
        return;
    }
    app->state = state;
}

void RecordMixing_SetStateFromLinkInMenu(RecordMixingApp *app, int state, int netId) {
    if (app->state == 3) {
        switch (state) {
        case 1: {
            u32 one;
            BufferPlayersName(app->msgFmt, 0, CommPlayerInfo_GetProfile(netId));
            RecordMixing_UpdateMaxEntries(app, 1);
            app->state = state;
            app->participantCount = Comm_CountConnected();
            app->leavingMask = 0;
            if (Comm_GetLocalNetId() == 0) {
                one = 1;
                Comm_SendCommand(117, &one, one);
            }
            return;
        }
        case 19:
            BufferPlayersName(app->msgFmt, 0, CommPlayerInfo_GetProfile(netId));
            if (netId == Comm_GetLocalNetId()) {
                return;
            }
            if (Comm_GetLocalNetId() == 0) {
                app->leavingMask &= netId ^ 0xFFFF;
            }
            app->state = state;
            return;
        default:
            GF_AssertFail();
            return;
        }
    }
    if (state == 1) {
        app->leavingMask = 0;
        app->participantCount = Comm_CountConnected();
    }
}

// buttonState: 0 normal, 1 pressed, 2 disabled (rows of the button screen)
void RecordMixing_SetButtonState(RecordMixingApp *app, int button, int buttonState) {
    NNSG2dScreenData *screen = app->buttonScreen;
    CopyToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, sRecordMixingButtonX[button], 20, 8, 4, screen->rawData, 0, buttonState * 4, screen->screenWidth / 8, screen->screenHeight / 8);
}

#pragma force_active on

void RecordMixing_PrintButtons(RecordMixingApp *app) {
    String *string;
    int x;

    if (!Comm_GetLocalNetId()) {
        RecordMixing_SetButtonState(app, 0, 0);
        string = NewString_ReadMsgData(app->msgData, 21);
        x = 64 - FontID_String_GetWidth(4, string, 0);
        AddTextPrinterParameterizedWithColor(&app->buttonWindows[0], 4, string, x / 2, 0, TEXT_SPEED_INSTANT, 0x40100, NULL);
        String_Delete(string);
    }
    app->buttonHighlightTimer = -1;
    RecordMixing_SetButtonState(app, 1, 0);
    string = NewString_ReadMsgData(app->msgData, 22);
    x = 64 - FontID_String_GetWidth(4, string, 0);
    AddTextPrinterParameterizedWithColor(&app->buttonWindows[1], 4, string, x / 2, 0, TEXT_SPEED_INSTANT, 0x40100, NULL);
    String_Delete(string);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active reset

#pragma force_active on

void RecordMixing_PulsePalette(u16 *angle) {
    u16 color;

    *angle += 10;
    if (*angle > 360) {
        *angle = 0;
    }
    color = 29 | ((GF_SinDeg(*angle) * 10 / FX32_ONE + 15) << 5);
    GX_LoadOBJPltt(&color, 10, 2);
    GX_LoadOBJPltt(&color, 42, 2);
}

#pragma force_active reset

int ov73_021E7340(void) {
    int i, n = 0;
    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            n++;
        }
    }
    return n;
}

#pragma force_active on

BOOL RecordMixing_PrintMemberList(Window *window, int unused, u32 color, RecordMixingApp *app) {
    int i;
    int localId = Comm_GetLocalNetId();
    RecordMixingApp *memberWalk;
    RecordMixingApp *nameWalk;
    int y;

    if (!RecordMixing_UpdateMembers(app)) {
        return FALSE;
    }
    FillWindowPixelBuffer(window, 0);
    // walks members[] and memberNames[] with pointers to the work struct shifted per entry
    memberWalk = app;
    i = 0;
    nameWalk = app;
    y = 1;
    do {
        if (memberWalk->members[0].cur) {
            int id = PlayerProfile_GetTrainerID_VisibleHalf(memberWalk->members[0].cur);
            String *idText;
            PlayerName_FlatToString(memberWalk->members[0].cur, nameWalk->memberNames[0]);
            BufferIntegerAsString(app->msgFmt, 0, id, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
            idText = ReadMsgData_ExpandPlaceholders(app->msgFmt, app->msgData, 1, HEAP_ID_50);
            if (localId == i) {
                AddTextPrinterParameterizedWithColor(window, 0, nameWalk->memberNames[0], 5, y, TEXT_SPEED_NOTRANSFER, 0x20300, NULL);
                AddTextPrinterParameterizedWithColor(window, 0, idText, 70, y, TEXT_SPEED_NOTRANSFER, 0x20300, NULL);
            } else {
                AddTextPrinterParameterizedWithColor(window, 0, nameWalk->memberNames[0], 5, y, TEXT_SPEED_NOTRANSFER, color, NULL);
                AddTextPrinterParameterizedWithColor(window, 0, idText, 70, y, TEXT_SPEED_NOTRANSFER, color, NULL);
            }
            String_Delete(idText);
        }
        memberWalk = (RecordMixingApp *)((u8 *)memberWalk + sizeof(RecordMixingMember));
        nameWalk = (RecordMixingApp *)((u8 *)nameWalk + sizeof(String *));
        y += 18;
        i++;
    } while (i < RECORD_MIXING_MAX_MEMBERS);
    CopyWindowToVram(window);
    return TRUE;
}

#pragma force_active reset

void ov73_021E7468(RecordMixingApp *app) {
}

int RecordMixing_CountMembers(void) {
    int i, n = 0;
    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            n++;
        }
    }
    return n;
}

int ov73_021E7488(RecordMixingApp *app) {
    return RecordMixing_CountMembers();
}

// memberStates: 0 none, 1 joined, 2 shown, 3 left
#pragma force_active on

BOOL RecordMixing_UpdateMembers(RecordMixingApp *app) {
    int i;
    BOOL changed = FALSE;

    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        app->members[i].prev = app->members[i].cur;
        app->members[i].cur = CommPlayerInfo_GetProfile(i);
        app->memberKeys[i].prev = app->memberKeys[i].cur;
        if (app->members[i].cur != NULL) {
            app->memberKeys[i].cur.s.id = PlayerProfile_GetTrainerID(app->members[i].cur);
            app->memberKeys[i].cur.s.valid = TRUE;
        } else {
            app->memberKeys[i].cur.s.id = 0;
            app->memberKeys[i].cur.s.valid = FALSE;
        }
    }
    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        if (app->members[i].prev != app->members[i].cur) {
            changed = TRUE;
            if (app->members[i].cur == NULL) {
                app->memberStates[i] = 3;
            } else {
                app->memberStates[i] = 1;
            }
        } else if (app->memberKeys[i].cur.raw != app->memberKeys[i].prev.raw) {
            changed = TRUE;
            if (app->memberKeys[i].cur.s.valid == FALSE) {
                app->memberStates[i] = 3;
            } else {
                app->memberStates[i] = 1;
            }
        }
    }
    return changed;
}

#pragma force_active reset

#pragma force_active on

void RecordMixing_PrintMessage(RecordMixingApp *app, int msgId, BOOL immediate) {
    String *string = String_New(180, HEAP_ID_50);
    ReadMsgDataIntoString(app->msgData, msgId, string);
    StringExpandPlaceholders(app->msgFmt, app->messageText, string);
    String_Delete(string);
    FillWindowPixelBuffer(&app->messageWindow, 15);
    DrawFrameAndWindow2(&app->messageWindow, FALSE, 1, 11);
    if (immediate == FALSE) {
        int textSpeed = RecordMixing_GetTextSpeed(app);
        app->printerId = AddTextPrinterParameterized(&app->messageWindow, 1, app->messageText, 0, 0, textSpeed, NULL);
    } else {
        AddTextPrinterParameterized(&app->messageWindow, 1, app->messageText, 0, 0, TEXT_SPEED_INSTANT, NULL);
        app->printerId = 0xFF;
    }
}

#pragma force_active reset

BOOL RecordMixing_IsPrinterDone(u32 printerId) {
    if (printerId == 0xFF) {
        return TRUE;
    }
    if (!TextPrinterCheckActive((u8)printerId)) {
        return TRUE;
    }
    return FALSE;
}

void RecordMixing_ClearMessageWindow(RecordMixingApp *app) {
    ClearFrameAndWindow2(&app->messageWindow, FALSE);
}

#pragma force_active on

void RecordMixing_UpdateMemberSprites(RecordMixingApp *app) {
    int i;
    BOOL appeared = FALSE;
    int gender;
    int avatar;

    for (i = 0; i < RECORD_MIXING_MAX_MEMBERS; i++) {
        switch (app->memberStates[i]) {
        case 0:
            break;
        case 1:
            GF_ASSERT(app->members[i].cur != NULL);
            gender = PlayerProfile_GetTrainerGender(app->members[i].cur);
            avatar = PlayerProfile_GetAvatar(app->members[i].cur);
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(app->memberSprites[i], gender * 2 + 38);
            } else {
                RecordMixing_LoadAvatar(app->avatarChar, app->avatarPltt, i, avatar, gender);
                Sprite_SetAnimCtrlSeq(app->memberSprites[i], i * 2 + 27);
            }
            Sprite_SetDrawFlag(app->memberSprites[i], TRUE);
            app->memberStates[i] = 2;
            appeared = TRUE;
            break;
        case 2:
            break;
        case 3:
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(app->memberSprites[i], PlayerProfile_GetTrainerGender(app->members[i].cur) * 2 + 39);
            } else {
                Sprite_SetAnimCtrlSeq(app->memberSprites[i], i * 2 + 28);
            }
            app->memberStates[i] = 0;
            break;
        }
    }
    if (appeared) {
        PlaySE(SEQ_SE_DP_TELE2);
    }
}

#pragma force_active reset

void RecordMixing_LoadAvatarGraphics(RecordMixingApp *app, NARC *narc) {
    app->avatarPlttRaw[0] = GfGfxLoader_GetPlttData(NARC_a_1_0_0, 9, &app->avatarPltt[0], HEAP_ID_50);
    app->avatarPlttRaw[1] = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 11, &app->avatarPltt[1], HEAP_ID_50);
    app->avatarCharRaw[0] = GfGfxLoader_GetCharData(NARC_a_1_0_0, 43, TRUE, &app->avatarChar[0], HEAP_ID_50);
    app->avatarCharRaw[1] = GfGfxLoader_GetCharDataFromOpenNarc(narc, 12, TRUE, &app->avatarChar[1], HEAP_ID_50);
}

void RecordMixing_LoadAvatar(NNSG2dCharacterData **charData, NNSG2dPaletteData **plttData, u32 slot, int trainerClass, int gender) {
    int avatar = SpriteToUnionRoomAvatarIdx(gender, trainerClass);
    u8 *pltt = plttData[1]->pRawData;
    u8 *chars = charData[1]->pRawData;
    GX_LoadOBJ(chars + avatar * 0x600, sRecordMixingAvatarVramOffsets[slot], 0x600);
    GX_LoadOBJPltt(pltt + avatar * 0x20, (slot + 7) * 0x20, 0x20);
}

void RecordMixing_FreeAvatarGraphics(RecordMixingApp *app) {
    Heap_Free(app->avatarPlttRaw[0]);
    Heap_Free(app->avatarPlttRaw[1]);
    Heap_Free(app->avatarCharRaw[0]);
    Heap_Free(app->avatarCharRaw[1]);
}

int RecordMixing_GetTextSpeed(RecordMixingApp *app) {
    return 1;
}

void RecordMixing_UpdateMaxEntries(RecordMixingApp *app, int index) {
    if (Comm_GetLocalNetId() == 0) {
        if (index == -1) {
            CommState_SetMaxEntries(1);
        } else {
            int count = Comm_CountConnected() + index;
            if (count > RECORD_MIXING_MAX_MEMBERS) {
                count = RECORD_MIXING_MAX_MEMBERS;
            }
            CommState_SetMaxEntries(count);
        }
        if (index == -1) {
            app->maxEntriesMode = 2;
        } else if (index == 0) {
            app->maxEntriesMode = 1;
        } else {
            app->maxEntriesMode = 0;
        }
    }
}

#pragma force_active on

int RecordMixing_LeaderUpdateMembers(RecordMixingApp *app, int arg1) {
    u8 flag[1];
    int n = RecordMixing_CountMembers();

    if (n > app->participantCount) {
        flag[0] = 1;
        Comm_SendCommand(117, flag, 1);
        app->unk_4A20 = 1;
    } else {
        app->unk_4A20 = 0;
    }
    if (n == app->lastMemberCount) {
        return 1;
    }
    switch (n) {
    case 1:
        if (Comm_CountConnected() > 1 || Wireless_GetConnectedBitmap() > 1) {
            return 1;
        }
        app->state = 17;
        RecordMixing_UpdateMaxEntries(app, -1);
        RecordMixing_HideYesNo(app);
        return 2;
    case 2:
    case 3:
    case 4:
        UnionRoom_SetBeaconActivity(2);
        if (n < app->lastMemberCount) {
            switch (app->maxEntriesMode) {
            case 0:
                RecordMixing_UpdateMaxEntries(app, arg1);
                break;
            case 1:
                RecordMixing_UpdateMaxEntries(app, 0);
                break;
            }
            app->participantCount = Comm_CountConnected();
        }
        break;
    case 5:
        UnionRoom_SetBeaconActivity(9);
        RecordMixing_UpdateMaxEntries(app, arg1);
        break;
    }
    app->lastMemberCount = RecordMixing_CountMembers();
    return 1;
}

#pragma force_active reset
