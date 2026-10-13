#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "nnsys/fnd/expheap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "frontier_data_access.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "nintendo_wfc_connect.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "record_mixing.h"
#include "render_text.h"
#include "render_window.h"
#include "sav_system_info.h"
#include "save.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "save_wii_message.h"
#include "comm_wireless.h"
#include "wireless_status_icon.h"
#include "unk_02069528.h"
#include "network_overlay_loader.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// An older copy of the Nintendo WFC connection app of OVY_72 (nintendo_wfc_connect.c): the Wi-Fi Battle Room
// downloads and uploads with the same states, but with a scrolling number instead of the touch number pad and a
// YesNoPrompt instead of the touch YES/NO menu. Nothing launches it except LaunchApplication_DeadstrippedFunction,
// which loads OVY_72 for it (the HTTP requests are OVY_72's).

typedef struct BattleRoomDownload BattleRoomDownload;

typedef struct NintendoWfcUnusedPaletteCycle {
    SysTask *task;               // 0x000
    int active;                  // 0x004
    u16 basePltt[64];            // 0x008
    u16 cyclePltt[21 * 16];      // 0x088
    s16 index;                   // 0x328
    s8 direction;                // 0x32A
    u8 tick;                     // 0x32B
    u32 screens;                 // 0x32C, 0: both, 1: main only, 2: sub only
} NintendoWfcUnusedPaletteCycle; // size: 0x330

typedef struct NintendoWfcUnused {
    NintendoWifiConnectArgs *args;              // 0x0000
    BgConfig *bgConfig;                         // 0x0004
    u8 filler_0008[8];                          // 0x0008
    int mode;                                   // 0x0010, index into the sNintendoWfcUnusedMode* tables
    int nextMode;                               // 0x0014
    int unk_0018;                               // 0x0018
    int state;                                  // 0x001C, index into sNintendoWfcUnusedStates
    int nextState;                              // 0x0020
    void *heapMem;                              // 0x0024
    NNSFndHeapHandle expHeap;                   // 0x0028
    u8 connection[0x64];                          // 0x002C, passed to DWC_InitInetEx
    u32 ownRank;                                // 0x0090
    u32 rank;                                   // 0x0094
    int roomCount;                              // 0x0098
    u32 room;                                   // 0x009C
    u8 downloadBuffer[0xA38];                   // 0x00A0
    UnkStruct_02069528 uploadRecord;            // 0x0AD8
    u8 filler_0BBC[0x14];                       // 0x0BBC
    MessageFormat *msgFormat;                   // 0x0BD0
    MsgData *msgDataTower;                      // 0x0BD4, Wi-Fi Battle Tower (bank 799)
    MsgData *msgDataWfc;                        // 0x0BD8, Nintendo WFC (bank 778)
    MsgData *msgDataError;                      // 0x0BDC, Nintendo WFC errors (bank 800)
    String *msgString;                          // 0x0BE0
    String *titleString;                        // 0x0BE4
    String *roomString;                         // 0x0BE8
    String *rankString;                         // 0x0BEC
    String *errorString;                        // 0x0BF0
    u32 textPrinterId;                          // 0x0BF4
    SpriteList *spriteList;                     // 0x0BF8
    G2dRenderer renderer;                       // 0x0BFC
    GF_2DGfxResMan *resMan[4];                  // 0x0D24
    SpriteResource *resObjs[4];                 // 0x0D34
    u8 filler_0D44[0x20];                       // 0x0D44
    SpriteResourcesHeader spriteHeader;         // 0x0D64
    u8 filler_0D88[0x48];                       // 0x0D88
    Sprite *arrows[2];                          // 0x0DD0, up/down arrows next to the number windows
    Window msgWindow;                           // 0x0DD8
    Window rankWindow;                          // 0x0DE8
    Window roomWindow;                          // 0x0DF8
    Window titleWindow;                         // 0x0E08
    Window errorWindow;                         // 0x0E18
    u8 filler_0E28[0xE4];                       // 0x0E28
    WaitingIcon *waitingIcon;                   // 0x0F0C
    int dwcError;                               // 0x0F10
    int dwcErrorCode;                           // 0x0F14
    int dwcErrorType;                           // 0x0F18
    int httpResult;                             // 0x0F1C
    int messageTimer;                           // 0x0F20
    WifiServerProfile profile;                        // 0x0F24
    u32 profileResponse[2];                     // 0x0F88
    s16 subState;                               // 0x0F90
    s16 subTimer;                               // 0x0F92
    int timeoutCounter;                         // 0x0F94
    NintendoWfcUnusedPaletteCycle paletteCycle; // 0x0F98
    YesNoPrompt *yesNoPrompt;                   // 0x12C8
} NintendoWfcUnused;                            // size: 0x12CC

typedef struct NintendoWfcUnusedGlobal {
    NintendoWfcUnused *wfc; // 0x0
    NNSFndHeapHandle heap;  // 0x4, heap of the DWC allocator callbacks
} NintendoWfcUnusedGlobal;

typedef int (*NintendoWfcUnusedModeFunc)(NintendoWfcUnused *wfc, int state);

// One row per mode: {init, main, exit}. Each column is addressed through its own label.
typedef struct NintendoWfcUnusedModeRow {
    NintendoWfcUnusedModeFunc func[3];
} NintendoWfcUnusedModeRow;

// Defined in OVY_0 / OVY_38 (DWC wrappers); not declared in any header.
int DWC_GetLastError(void *a0);
int DWC_GetLastErrorEx(int *a0, int *a1);
void DWC_ClearError(void);
void DWC_SetMemFunc(void *(*alloc)(void *, u32, int), void (*freeFunc)(void *, void *));
void DWC_InitInetEx(void *a0, int a1, int a2, int a3);
void DWC_SetAuthServer(int a0);
void DWC_ConnectInetAsync(void);
int DWC_CheckInet(void);
void DWC_ProcessInet(void);
int DWC_GetInetStatus(void);
void DWC_CleanupInet(void);
int DWC_GetLinkLevel(void);
void DWC_GetApInfo(void *a0);
void DWC_UpdateConnection(void);
void DWC_GetDateTime(void *a0, void *a1);
void DWC_NASLoginAsync(void);
int DWC_NASLoginProcess(void);
void DWC_ShutdownFriendsMatch(void);
void DWC_ShutdownGHTTP(void);

// Defined in OVY_72 (nintendo_wfc_connect.c); not declared in its header.
void NintendoWfcHttp_Update(void);
BOOL NintendoWfcHttp_IsDone(void);
s32 NintendoWfcHttp_GetResult(void);
void NintendoWfcHttp_RequestRoomCount(int rank);
void NintendoWfcHttp_RequestDownload(int rank, int room, void *buffer);
void NintendoWfcHttp_RequestUpload(int rank, int room, int a2, UnkStruct_02069528 *record);
void NintendoWfcHttp_RequestInfo(void);
void NintendoWfcHttp_RequestSetProfile(WifiServerProfile *profile, u32 *response);
// The definition takes (u32 profileId, u64 friendKey); this app passes the key as two words.
void NintendoWfcHttp_Init(u32 profileId, u32 friendKeyLo, u32 friendKeyHi);

// Defined in other ARM9 sources; not declared in their headers.
BOOL LoadOVY38(void);
void UnloadOVY38(void);
void CommWireless_Terminate(void);
void CommState_InitWfcSession(SaveData *saveData);
void CommState_FreeWfcSession(void);
void CommState_ShowRestartErrorAndHalt(void);
void WirelessStatusIcon_SetLevel(s32 a0);
u64 DWC_CreateFriendKey(const void *data);
u8 FrontierData_GetWifiWinCount(FrontierData *data);
void BattleRoomDownload_MarkRoom(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
BOOL BattleRoomDownload_IsRoomMarked(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
void BattleRoomDownload_SetTrainers(BattleRoomDownload *data, void *records, u32 a, u32 b);
void BattleRoomDownload_GetTrainersRoom(BattleRoomDownload *data, u8 *out);
void BattleRoomDownload_SetLeaders(BattleRoomDownload *data, void *records, u32 a, u32 b);
// A record mixing state function in record_mixing.c; the copy of the OVY_72 code calls it where OVY_72 calls
// WifiConnection_ClassifyErrorCode.
int ov73_021E6A70(int errorCode, int errorType);

extern NintendoWfcUnusedGlobal _021EA940;
extern const u16 sNintendoWfcUnusedArrowPositions[2][2];
extern const GraphicsModes sNintendoWfcUnusedGraphicsModes;
extern const ObjCharTransferTemplate sNintendoWfcUnusedObjCharTransferTemplate;
extern const GraphicsBanks sNintendoWfcUnusedGraphicsBanks;
extern const BgTemplate sNintendoWfcUnusedBgTemplate_Sub1;
extern const BgTemplate sNintendoWfcUnusedBgTemplate_Main1;
extern const BgTemplate sNintendoWfcUnusedBgTemplate_Main0;
extern const BgTemplate sNintendoWfcUnusedBgTemplate_Sub0;
extern const NintendoWfcUnusedModeRow sNintendoWfcUnusedModeInit[];
extern const NintendoWfcUnusedModeRow sNintendoWfcUnusedModeMain[];
extern const NintendoWfcUnusedModeRow sNintendoWfcUnusedModeExit[];
extern int (*const sNintendoWfcUnusedStates[])(NintendoWfcUnused *wfc);

void NintendoWfcUnused_VBlankCB(void *arg);
void NintendoWfcUnused_SetGfxBanks(void);
void NintendoWfcUnused_InitArgs(NintendoWfcUnused *wfc, OverlayManager *man);
void ov73_021E8164(NintendoWfcUnused *wfc);
void NintendoWfcUnused_InitObjTransfer(void);
void NintendoWfcUnused_InitSpriteResources(NintendoWfcUnused *wfc);
void NintendoWfcUnused_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcUnused *wfc, SpriteResourcesHeader *header, u32 whichScreen);
void NintendoWfcUnused_CreateSprites(NintendoWfcUnused *wfc);
YesNoPrompt *NintendoWfcUnused_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart);
int NintendoWfcUnused_HandleYesNoPrompt(YesNoPrompt *prompt);
void NintendoWfcUnused_SetState(NintendoWfcUnused *wfc, int state, int nextState);
void *NintendoWfcUnused_Alloc(void *unused, u32 size, int alignment);
void NintendoWfcUnused_Free(void *unused, void *ptr);
int NintendoWfcUnused_GetWifiIconLevel(NintendoWfcUnused *wfc);
void NintendoWfcUnused_SetNextMode(NintendoWfcUnused *wfc, int mode, int a2);
void NintendoWfcUnused_InitGraphics(NintendoWfcUnused *wfc);
void NintendoWfcUnused_FreeGraphics(NintendoWfcUnused *wfc);
int NintendoWfcUnused_SessionInit(NintendoWfcUnused *wfc);
int NintendoWfcUnused_SessionMain(NintendoWfcUnused *wfc);
int NintendoWfcUnused_SessionExit(NintendoWfcUnused *wfc);
void NintendoWfcUnused_InitBgs(BgConfig *bgConfig);
void NintendoWfcUnused_FreeBgs(BgConfig *bgConfig);
void NintendoWfcUnused_LoadBgGraphics(NintendoWfcUnused *wfc);
void NintendoWfcUnused_PaletteCycleTask(SysTask *task, void *data);
void NintendoWfcUnused_AddWindows(NintendoWfcUnused *wfc);
void NintendoWfcUnused_RemoveWindows(NintendoWfcUnused *wfc);
void NintendoWfcUnused_InitStrings(NintendoWfcUnused *wfc);
void NintendoWfcUnused_FreeStrings(NintendoWfcUnused *wfc);
int ov73_021E6A70(int errorCode, int errorType);
int NintendoWfcUnused_AskConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleConnectChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLogOut(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLogOutChoice(NintendoWfcUnused *wfc);
void NintendoWfcUnused_StartConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartAuth(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitAuth(NintendoWfcUnused *wfc);
int NintendoWfcUnused_InitHttp(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestInfo(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitInfo(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestSetProfile(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitSetProfile(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestRoomCount(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitRoomCount(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskChallengeRoom(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartChallengeRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleChallengeRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintReceivingTrainers(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintRoomUnavailable(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestTrainersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitTrainersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintTrainersReceived(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintSendingRecord(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestUpload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitUpload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintRecordSent(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLeadersRank(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartLeadersRankInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLeadersRankInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLeadersRoom(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartLeadersRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLeadersRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintReceivingLeaders(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestLeadersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitLeadersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintLeadersReceived(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartDisconnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskCancelChallenge(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleCancelChallengeChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskCancelDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleCancelDownloadChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintDisconnecting(NintendoWfcUnused *wfc);
int NintendoWfcUnused_CloseConnection(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintDisconnected(NintendoWfcUnused *wfc);
int ov73_021E9D10(void);
int ov73_021E9D44(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartSave(NintendoWfcUnused *wfc);
int NintendoWfcUnused_Save(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartExit(NintendoWfcUnused *wfc);
int NintendoWfcUnused_ShowHttpError(NintendoWfcUnused *wfc);
int NintendoWfcUnused_Disconnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitMessage(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitMessageAndDelay(NintendoWfcUnused *wfc);
int NintendoWfcUnused_OpenYesNoPrompt(NintendoWfcUnused *wfc);
void NintendoWfcUnused_PrintFormattedMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
int NintendoWfcUnused_ShowDwcError(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitErrorDismiss(NintendoWfcUnused *wfc);
void NintendoWfcUnused_PrintMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
int NintendoWfcUnused_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId);
void NintendoWfcUnused_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color);
void NintendoWfcUnused_PrintAlignedTextSmall(Window *window, String *string, int x, int y, int alignment, u32 color);
void NintendoWfcUnused_PrintNumber(Window *window, String *string, int number, int digits);
void NintendoWfcUnused_CreateWirelessIcon(NintendoWfcUnused *wfc);
void NintendoWfcUnused_SetArrowsVisible(NintendoWfcUnused *wfc, u32 which, BOOL visible);
void NintendoWfcUnused_ShowWaitingIcon(NintendoWfcUnused *wfc);
void NintendoWfcUnused_HideWaitingIcon(NintendoWfcUnused *wfc);
void NintendoWfcUnused_PrintErrorMessage(NintendoWfcUnused *wfc, int msgNo);
void NintendoWfcUnused_ShowErrorMessage(NintendoWfcUnused *wfc, int msgNo, int errorCode);

BOOL NintendoWfcUnused_Init(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc;
    GraphicsModes modes;
    void *heapMem;

    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        Heap_Create(HEAP_ID_3, HEAP_ID_150, 0x50000);
        wfc = OverlayManager_CreateAndGetData(man, sizeof(NintendoWfcUnused), HEAP_ID_150);
        memset(wfc, 0, sizeof(NintendoWfcUnused));
        wfc->bgConfig = BgConfig_Alloc(HEAP_ID_150);
        _021EA940.wfc = wfc;
        modes = sNintendoWfcUnusedGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
        wfc->msgFormat = MessageFormat_New_Custom(11, 32, HEAP_ID_150);
        wfc->msgDataTower = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0799_bin, HEAP_ID_150);
        wfc->msgDataWfc = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_150);
        wfc->msgDataError = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_150);
        SetKeyRepeatTimers(4, 8);
        NintendoWfcUnused_InitArgs(wfc, man);
        NintendoWfcUnused_InitGraphics(wfc);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        heapMem = Heap_Alloc(HEAP_ID_150, 0x20020);
        wfc->heapMem = heapMem;
        wfc->expHeap = NNS_FndCreateExpHeapEx((void *)(((u32)heapMem + 31) & ~31), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        *state = 1;
        break;
    case 1:
        LoadDwcOverlay();
        LoadOVY38();
        CommWireless_StartUp();
        *state = 0;
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL NintendoWfcUnused_Main(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc = OverlayManager_GetData(man);
    DWC_UpdateConnection();
    NintendoWfcHttp_Update();
    DWC_UpdateConnection();
    switch (*state) {
    case 0:
        if (CommWireless_IsStartedUp()) {
            _021EA940.heap = wfc->expHeap;
            DWC_SetMemFunc(NintendoWfcUnused_Alloc, NintendoWfcUnused_Free);
            *state = 1;
        }
        break;
    case 1:
        *state = sNintendoWfcUnusedModeInit[wfc->mode].func[0](wfc, *state);
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *state = 3;
        }
        break;
    case 3:
        *state = sNintendoWfcUnusedModeMain[wfc->mode].func[0](wfc, *state);
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            *state = sNintendoWfcUnusedModeExit[wfc->mode].func[0](wfc, *state);
        }
        break;
    case 5:
        return TRUE;
    }
    if (wfc->spriteList) {
        SpriteList_RenderAndAnimateSprites(wfc->spriteList);
    }
    return FALSE;
}

#pragma force_active reset

BOOL NintendoWfcUnused_Exit(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc = OverlayManager_GetData(man);

    Heap_Free(wfc->heapMem);
    UnloadOVY38();
    UnloadDwcOverlay();
    NintendoWfcUnused_FreeGraphics(wfc);
    DestroyMsgData(wfc->msgDataError);
    DestroyMsgData(wfc->msgDataWfc);
    DestroyMsgData(wfc->msgDataTower);
    MessageFormat_Delete(wfc->msgFormat);
    ov73_021E8164(wfc);
    CommWireless_Terminate();
    Heap_Free(wfc->bgConfig);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_150);
    return TRUE;
}

void NintendoWfcUnused_VBlankCB(void *arg) {
    volatile u32 *irqCheck = (volatile u32 *)0x027E0000; // the OS IRQ check flag lives at +0x3FF8
    NintendoWfcUnused *wfc = arg;

    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(wfc->bgConfig);
    OamManager_ApplyAndResetBuffers();
    irqCheck[0x3FF8 / 4] |= 1;
}

void NintendoWfcUnused_SetGfxBanks(void) {
    GraphicsBanks banks = sNintendoWfcUnusedGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void NintendoWfcUnused_InitArgs(NintendoWfcUnused *wfc, OverlayManager *man) {
    wfc->args = OverlayManager_GetArgs(man);
    wfc->mode = 0;
    NintendoWfcUnused_SetNextMode(wfc, 0, 0);
}

void ov73_021E8164(NintendoWfcUnused *wfc) {
}

void NintendoWfcUnused_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sNintendoWfcUnusedObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_150);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active on

void NintendoWfcUnused_InitSpriteResources(NintendoWfcUnused *wfc) {
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_150);
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_150);
    wfc->spriteList = G2dRenderer_Init(10, &wfc->renderer, HEAP_ID_150);
    G2dRenderer_SetSubSurfaceCoords(&wfc->renderer, 0, 256 << FX32_SHIFT);
    for (i = 0; i < 4; i++) {
        wfc->resMan[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_150);
    }
    wfc->resObjs[0] = AddCharResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CHAR], narc, 46, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_150);
    wfc->resObjs[1] = AddPlttResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_PLTT], narc, 10, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_150);
    wfc->resObjs[2] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CELL], narc, 47, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_150);
    wfc->resObjs[3] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_ANIM], narc, 48, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_150);
    SpriteTransfer_CreateCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_CreateExtPlttTransferTask(wfc->resObjs[1]);
    NARC_Delete(narc);
}

#pragma force_active reset

void NintendoWfcUnused_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcUnused *wfc, SpriteResourcesHeader *header, u32 whichScreen) {
    template->spriteList = wfc->spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 1;
    template->whichScreen = (NNS_G2D_VRAM_TYPE)whichScreen;
    template->heapID = HEAP_ID_150;
}

#pragma force_active on

void NintendoWfcUnused_CreateSprites(NintendoWfcUnused *wfc) {
    SpriteTemplate template;
    int i;
    CreateSpriteResourcesHeader(&wfc->spriteHeader, 0, 0, 0, 0, -1, -1, 0, 0, wfc->resMan[0], wfc->resMan[1], wfc->resMan[2], wfc->resMan[3], NULL, NULL);
    NintendoWfcUnused_InitSpriteTemplate(&template, wfc, &wfc->spriteHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    for (i = 0; i < 2; i++) {
        template.position.x = sNintendoWfcUnusedArrowPositions[i][0] << FX32_SHIFT;
        template.position.y = sNintendoWfcUnusedArrowPositions[i][1] << FX32_SHIFT;
        wfc->arrows[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(wfc->arrows[i], TRUE);
        Sprite_SetAnimCtrlSeq(wfc->arrows[i], i);
        Sprite_SetDrawFlag(wfc->arrows[i], FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    WirelessStatusIcon_Create();
}

#pragma force_active reset

YesNoPrompt *NintendoWfcUnused_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart) {
    YesNoPromptTemplate template;
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_150);
    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = tileStart;
    template.plttSlot = 8;
    template.x = 23;
    template.y = y;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}

int NintendoWfcUnused_HandleYesNoPrompt(YesNoPrompt *prompt) {
    int response = YesNoPrompt_HandleInput(prompt);
    if (response) {
        YesNoPrompt_Destroy(prompt);
    }
    return response;
}

void NintendoWfcUnused_SetState(NintendoWfcUnused *wfc, int state, int nextState) {
    wfc->state = state;
    wfc->nextState = nextState;
}

void *NintendoWfcUnused_Alloc(void *unused, u32 size, int alignment) {
    OSIntrMode enabled = OS_DisableInterrupts();
    void *result = NNS_FndAllocFromExpHeapEx(_021EA940.heap, size, alignment);
    OS_RestoreInterrupts(enabled);
    return result;
}

void NintendoWfcUnused_Free(void *unused, void *ptr) {
    if (ptr) {
        OSIntrMode enabled = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(_021EA940.heap, ptr);
        OS_RestoreInterrupts(enabled);
    }
}

int NintendoWfcUnused_GetWifiIconLevel(NintendoWfcUnused *wfc) {
    return 3 - DWC_GetLinkLevel();
}

void NintendoWfcUnused_SetNextMode(NintendoWfcUnused *wfc, int mode, int a2) {
    wfc->nextMode = mode;
    wfc->unk_0018 = a2;
}

void NintendoWfcUnused_InitGraphics(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_SetGfxBanks();
    NintendoWfcUnused_InitObjTransfer();
    NintendoWfcUnused_InitSpriteResources(wfc);
    NintendoWfcUnused_CreateSprites(wfc);
    Main_SetVBlankIntrCB(NintendoWfcUnused_VBlankCB, wfc);
}

void NintendoWfcUnused_FreeGraphics(NintendoWfcUnused *wfc) {
    int i;
    NintendoWfcUnused *p;

    SpriteTransfer_DeleteCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(wfc->resObjs[1]);
    i = 0;
    p = wfc;
    while (i < 4) {
        Destroy2DGfxResObjMan(p->resMan[0]);
        i++;
        p = (NintendoWfcUnused *)((u8 *)p + sizeof(GF_2DGfxResMan *)); // walks resMan[] through a shifted base pointer
    }
    SpriteList_Delete(wfc->spriteList);
    wfc->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

int NintendoWfcUnused_SessionInit(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_InitStrings(wfc);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 8, 1, HEAP_ID_150);
    NintendoWfcUnused_InitBgs(wfc->bgConfig);
    NintendoWfcUnused_LoadBgGraphics(wfc);
    NintendoWfcUnused_AddWindows(wfc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, TRUE);
    CommState_InitWfcSession(wfc->args->saveData);
    NintendoWfcUnused_CreateWirelessIcon(wfc);
    wfc->ownRank = FrontierData_WifiRankAction(wfc->args->frontierData, 0);
    wfc->rank = wfc->ownRank;
    wfc->room = 1;
    wfc->waitingIcon = NULL;
    if (DWC_CheckInet() == 0) {
        if (wfc->args->connectNow != 0) {
            NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, 1, 1, 0xF0F);
            NintendoWfcUnused_SetState(wfc, 47, 2);
            NintendoWfcUnused_ShowWaitingIcon(wfc);
        } else {
            wfc->state = 0;
        }
    } else {
        wfc->state = 51;
    }
    return 2;
}

int NintendoWfcUnused_SessionMain(NintendoWfcUnused *wfc) {
    int prevState;
    int newState;
    int result;

    WirelessStatusIcon_SetLevel(NintendoWfcUnused_GetWifiIconLevel(wfc));
    prevState = wfc->state;
    result = sNintendoWfcUnusedStates[prevState](wfc);
    newState = wfc->state;
    if (prevState != newState) {
        wfc->subState = 0;
        wfc->subTimer = 0;
    }
    return result;
}

int NintendoWfcUnused_SessionExit(NintendoWfcUnused *wfc) {
    if (wfc->paletteCycle.task != NULL) {
        SysTask_Destroy(wfc->paletteCycle.task);
        wfc->paletteCycle.task = NULL;
        wfc->paletteCycle.active = 0;
    }
    NintendoWfcUnused_FreeStrings(wfc);
    NintendoWfcUnused_RemoveWindows(wfc);
    NintendoWfcUnused_FreeBgs(wfc->bgConfig);
    wfc->paletteCycle.active = 0;
    wfc->mode = wfc->nextMode;
    if (wfc->mode == 0) {
        return 5;
    }
    return 1;
}

void NintendoWfcUnused_InitBgs(BgConfig *bgConfig) {
    BgTemplate main0 = sNintendoWfcUnusedBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    {
        BgTemplate main1 = sNintendoWfcUnusedBgTemplate_Main1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
        {
            BgTemplate sub0 = sNintendoWfcUnusedBgTemplate_Sub0;
            InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, 0);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, FALSE);
            BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
            {
                BgTemplate sub1 = sNintendoWfcUnusedBgTemplate_Sub1;
                InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, 0);
            }
        }
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, FALSE);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_150);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_150);
}

void NintendoWfcUnused_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on

void NintendoWfcUnused_LoadBgGraphics(NintendoWfcUnused *wfc) {
    NNSG2dPaletteData *plttData;
    NARC *narc;
    int outer;
    int count;
    int blend;
    int last;
    u16 *dest;
    u16 *src;
    BgConfig *bgConfig = wfc->bgConfig;
    void *raw;
    int i;
    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_150);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_150);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_150);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_150);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_150);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(wfc->args->options), HEAP_ID_150);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_150);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_150);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_150);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    MI_CpuFill8(&wfc->paletteCycle, 0, sizeof(NintendoWfcUnusedPaletteCycle));
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_150);
    MIi_CpuCopy16(plttData->pRawData, wfc->paletteCycle.basePltt, 0x80);
    MIi_CpuCopy16(plttData->pRawData, wfc->paletteCycle.cyclePltt, 0x80);
    Heap_Free(raw);
    // 21 palettes: three blends of 7 steps each between consecutive base palettes
    count = 0;
    outer = 0;
    dest = wfc->paletteCycle.cyclePltt;
    src = wfc->paletteCycle.basePltt;
    while (outer < 3) {
        blend = 0;
        last = 0;
        while (TRUE) {
            GF_ASSERT(count < 21);
            for (i = 1; i < 16; i++) {
                BlendPalette(&src[i], &dest[i], 1, blend >> 8, wfc->paletteCycle.basePltt[(outer + 1) * 16 + i]);
            }
            dest += 16;
            count++;
            if (last == 1) {
                break;
            }
            blend += 0x300;
            if (blend >= 0x1000) {
                blend = 0x1000;
                last = 1;
            }
        }
        src += 16;
        outer++;
    }
    DC_FlushRange(wfc->paletteCycle.cyclePltt, sizeof(wfc->paletteCycle.cyclePltt));
    wfc->paletteCycle.active = 1;
    wfc->paletteCycle.screens = 0;
    wfc->paletteCycle.task = SysTask_CreateOnVBlankQueue(NintendoWfcUnused_PaletteCycleTask, &wfc->paletteCycle, 20);
    NARC_Delete(narc);
}

#pragma force_active reset

void NintendoWfcUnused_PaletteCycleTask(SysTask *task, void *data) {
    NintendoWfcUnusedPaletteCycle *cycle = data;
    if (cycle->active != 0) {
        cycle->tick ^= 1;
        if ((cycle->tick & 1) == 0) {
            if (cycle->screens <= 1) {
                GX_LoadBGPltt(cycle->cyclePltt + cycle->index * 16, 0, 0x20);
            }
            if (cycle->screens == 0 || cycle->screens == 2) {
                GXS_LoadBGPltt(cycle->cyclePltt + cycle->index * 16, 0, 0x20);
            }
            if (cycle->direction == 0) {
                cycle->index++;
                if (cycle->index >= 21) {
                    cycle->index = 19;
                    cycle->direction ^= 1;
                }
            } else {
                cycle->index--;
                if (cycle->index < 0) {
                    cycle->index = 1;
                    cycle->direction ^= 1;
                }
            }
        }
    }
}

void NintendoWfcUnused_AddWindows(NintendoWfcUnused *wfc) {
    AddWindowParameterized(wfc->bgConfig, &wfc->errorWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 0xA4);
    FillWindowPixelBuffer(&wfc->errorWindow, 0);
    AddWindowParameterized(wfc->bgConfig, &wfc->roomWindow, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x94);
    FillWindowPixelBuffer(&wfc->roomWindow, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->rankWindow, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x9C);
    FillWindowPixelBuffer(&wfc->rankWindow, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 0x214);
    FillWindowPixelBuffer(&wfc->titleWindow, 0);
    NintendoWfcUnused_PrintAlignedText(&wfc->titleWindow, wfc->titleString, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(wfc->bgConfig, &wfc->msgWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
}

void NintendoWfcUnused_RemoveWindows(NintendoWfcUnused *wfc) {
    RemoveWindow(&wfc->msgWindow);
    RemoveWindow(&wfc->titleWindow);
    RemoveWindow(&wfc->rankWindow);
    RemoveWindow(&wfc->roomWindow);
    RemoveWindow(&wfc->errorWindow);
}

void NintendoWfcUnused_InitStrings(NintendoWfcUnused *wfc) {
    wfc->msgString = String_New(180, HEAP_ID_150);
    wfc->roomString = String_New(4, HEAP_ID_150);
    wfc->rankString = String_New(3, HEAP_ID_150);
    wfc->titleString = NewString_ReadMsgData(wfc->msgDataTower, 10);
    wfc->errorString = String_New(256, HEAP_ID_150);
}

void NintendoWfcUnused_FreeStrings(NintendoWfcUnused *wfc) {
    String_Delete(wfc->errorString);
    String_Delete(wfc->titleString);
    String_Delete(wfc->rankString);
    String_Delete(wfc->roomString);
    String_Delete(wfc->msgString);
}

int NintendoWfcUnused_AskConnect(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, 17, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 48, 1);
    return 3;
}

int NintendoWfcUnused_HandleConnectChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            CommState_FreeWfcSession();
            NintendoWfcUnused_SetNextMode(wfc, 0, 0);
            wfc->state = 46;
        } else {
            NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, 1, 1, 0xF0F);
            NintendoWfcUnused_SetState(wfc, 47, 2);
            NintendoWfcUnused_ShowWaitingIcon(wfc);
        }
    }
    return 3;
}

int NintendoWfcUnused_AskLogOut(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataWfc, 12, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 48, 52);
    return 3;
}

int NintendoWfcUnused_HandleLogOutChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response != 0) {
        if (response == 2) {
            if (DWC_CheckInet() == 0) {
                wfc->state = 0;
            } else {
                wfc->state = 41;
            }
        } else {
            if (DWC_CheckInet() == 0) {
                NintendoWfcUnused_SetNextMode(wfc, 0, 0);
                wfc->state = 46;
            } else {
                wfc->state = 41;
            }
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

void NintendoWfcUnused_StartConnect(NintendoWfcUnused *wfc) {
    DWC_InitInetEx(wfc->connection, 2, 1, 20);
    DWC_SetAuthServer(2);
    DWC_ConnectInetAsync();
    wfc->state = 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitConnect(NintendoWfcUnused *wfc) {
    int errorCode, ignored;
    u8 info[60];

    DWC_ProcessInet();
    if (DWC_CheckInet()) {
        switch (DWC_GetInetStatus()) {
        case 7:
            wfc->dwcError = DWC_GetLastError(&errorCode);
            wfc->dwcErrorCode = errorCode;
            DWC_ClearError();
            DWC_CleanupInet();
            NintendoWfcUnused_HideWaitingIcon(wfc);
            wfc->state = 55;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            DWC_GetLastError(&ignored);
            NintendoWfcUnused_HideWaitingIcon(wfc);
            wfc->state = 53;
            wfc->httpResult = -2;
            break;
        case 4:
            DWC_GetApInfo(info);
            wfc->state = 4;
            break;
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_StartAuth(NintendoWfcUnused *wfc) {
    DWC_NASLoginAsync();
    wfc->state = 5;
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitAuth(NintendoWfcUnused *wfc) {
    int errorCode, errorKind;
    switch (DWC_NASLoginProcess()) {
    case 3:
        wfc->state = 6;
        break;
    case 0:
    case 4:
    case 5:
        NintendoWfcUnused_HideWaitingIcon(wfc);
        wfc->dwcError = DWC_GetLastErrorEx(&errorCode, &errorKind);
        wfc->dwcErrorCode = errorCode;
        DWC_ClearError();
        DWC_CleanupInet();
        wfc->state = 55;
        switch (errorKind) {
        case 1:
        case 2:
            wfc->state = 55;
            break;
        case 4:
            DWC_ShutdownGHTTP();
            wfc->state = 55;
            break;
        case 6:
            wfc->state = 55;
            break;
        case 3:
            DWC_ShutdownFriendsMatch();
            wfc->state = 55;
            break;
        case 5:
        case 7:
            CommState_ShowRestartErrorAndHalt();
            break;
        }
        if (errorCode < -20000 && errorCode >= -29999) {
            wfc->state = 55;
        }
        break;
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_InitHttp(NintendoWfcUnused *wfc) {
    if (Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(wfc->args->sysInfo, (u32)wfc->args->dwcProfileId);
    }
    u32 profileId = Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo);
    u64 friendKey = DWC_CreateFriendKey(wfc->args->dwcUserData);
    NintendoWfcHttp_Init(profileId, (u32)friendKey, (u32)(friendKey >> 32));
    wfc->state = 7;
    return 3;
}

int NintendoWfcUnused_RequestInfo(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestInfo();
    wfc->state = 8;
    wfc->timeoutCounter = 0;
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitInfo(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        *(volatile int *)&wfc->timeoutCounter = 0; // the original accessed this counter as volatile here
        switch (result) {
        case 0:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            switch (wfc->args->mode) {
            case 0:
                NintendoWfcUnused_SetState(wfc, 11, 13);
                break;
            case 1:
                wfc->state = 25;
                break;
            case 2:
                wfc->state = 29;
                break;
            }
            break;
        case 1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        (*(volatile int *)&wfc->timeoutCounter)++;
        if (*(volatile int *)&wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_RequestSetProfile(NintendoWfcUnused *wfc) {
    Save_WiiMessage_GetServerProfile(wfc->args->saveData, &wfc->profile);
    NintendoWfcHttp_RequestSetProfile(&wfc->profile, wfc->profileResponse);
    wfc->state = 10;
    wfc->timeoutCounter = 0;
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitSetProfile(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            switch (wfc->profileResponse[0]) {
            case 0:
                switch (wfc->profileResponse[1]) {
                case 0:
                    switch (wfc->args->mode) {
                    case 0:
                        NintendoWfcUnused_SetState(wfc, 11, 13);
                        break;
                    case 1:
                        wfc->state = 25;
                        break;
                    case 2:
                        wfc->state = 29;
                        break;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    wfc->httpResult = result;
                    wfc->state = 53;
                    NintendoWfcUnused_HideWaitingIcon(wfc);
                    break;
                }
                break;
            case 1:
            case 2:
                wfc->httpResult = result;
                wfc->state = 53;
                NintendoWfcUnused_HideWaitingIcon(wfc);
                break;
            default:
                NintendoWfcUnused_HideWaitingIcon(wfc);
                CommState_ShowRestartErrorAndHalt();
                break;
            }
            break;
        case 1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_RequestRoomCount(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestRoomCount(wfc->rank);
    wfc->state = 12;
    wfc->timeoutCounter = 0;
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitRoomCount(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        *(volatile int *)&wfc->timeoutCounter = 0; // the original accessed this counter as volatile here
        switch (result) {
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = 53;
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        default:
            if (result > 0) {
                wfc->roomCount = result;
                wfc->state = wfc->nextState;
            }
            break;
        }
        NintendoWfcUnused_HideWaitingIcon(wfc);
    } else {
        (*(volatile int *)&wfc->timeoutCounter)++;
        if (*(volatile int *)&wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_AskChallengeRoom(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 0, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 14);
    return 3;
}

int NintendoWfcUnused_StartChallengeRoomInput(NintendoWfcUnused *wfc) {
    DrawFrameAndWindow1(&wfc->roomWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
    NintendoWfcUnused_SetArrowsVisible(wfc, 0, TRUE);
    CopyWindowToVram(&wfc->roomWindow);
    wfc->state = 15;
    return 3;
}

// Chooses the room with UP/DOWN; A: download it (or show it if it was downloaded already), B: back.
int NintendoWfcUnused_HandleChallengeRoomInput(NintendoWfcUnused *wfc) {
    u32 time[3];
    u32 date[4];

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        wfc->room++;
        if ((int)wfc->room > wfc->roomCount) {
            wfc->room = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        wfc->room--;
        if ((int)wfc->room < 1) {
            wfc->room = wfc->roomCount;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        DWC_GetDateTime(date, time);
        // BattleRoomDownload_IsRoomMarked takes u32 group/entry; the original call truncated them to u8
        if (BattleRoomDownload_IsRoomMarked((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date)) {
            wfc->state = 20;
        } else {
            wfc->state = 16;
        }
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = 21;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return 3;
}

int NintendoWfcUnused_PrintReceivingTrainers(NintendoWfcUnused *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, 1, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 17);
    MIi_CpuClearFast(0, (u32 *)wfc->downloadBuffer, sizeof(wfc->downloadBuffer));
    return 3;
}

int NintendoWfcUnused_PrintRoomUnavailable(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, 2, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 13);
    return 3;
}

int NintendoWfcUnused_RequestTrainersDownload(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->ownRank, wfc->room, wfc->downloadBuffer);
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    wfc->state = 18;
    wfc->timeoutCounter = 0;
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitTrainersDownload(NintendoWfcUnused *wfc) {
    int result;
    u32 time[3];
    u32 date[4];

    if (NintendoWfcHttp_IsDone()) {
        result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            // BattleRoomDownload_SetTrainers / BattleRoomDownload_MarkRoom take u32; the original calls truncated rank and room to u8
            BattleRoomDownload_SetTrainers((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer, (u8)wfc->ownRank, (u8)wfc->room);
            NintendoWfcUnused_SetState(wfc, 39, 19);
            DWC_GetDateTime(date, time);
            BattleRoomDownload_MarkRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
        case -4:
            wfc->httpResult = result;
            wfc->state = 11;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_PrintTrainersReceived(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 3, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 45);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

int NintendoWfcUnused_PrintSendingRecord(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 7, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 26);
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return 3;
}

int NintendoWfcUnused_RequestUpload(NintendoWfcUnused *wfc) {
    int record = FrontierData_GetWifiWinCount(wfc->args->frontierData);
    u8 rankRoom[4];
    BattleRoomDownload_GetTrainersRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, rankRoom);
    sub_02069528(wfc->args->saveData, 1, &wfc->uploadRecord);
    NintendoWfcHttp_RequestUpload(rankRoom[0], rankRoom[1], record, &wfc->uploadRecord);
    wfc->state = 27;
    wfc->timeoutCounter = 0;
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitUpload(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        *(volatile int *)&wfc->timeoutCounter = 0; // the original accessed this counter as volatile here
        switch (result) {
        case 0:
            FrontierData_FlagAction(wfc->args->frontierData, 5, 2);
            NintendoWfcUnused_SetState(wfc, 39, 28);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        (*(volatile int *)&wfc->timeoutCounter)++;
        if (*(volatile int *)&wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_PrintRecordSent(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 8, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 45);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

int NintendoWfcUnused_AskLeadersRank(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 5, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 30);
    return 3;
}

int NintendoWfcUnused_StartLeadersRankInput(NintendoWfcUnused *wfc) {
    wfc->rank = 1;
    DrawFrameAndWindow1(&wfc->rankWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, wfc->rank, 2);
    NintendoWfcUnused_SetArrowsVisible(wfc, 1, TRUE);
    CopyWindowToVram(&wfc->rankWindow);
    wfc->state = 31;
    return 3;
}

#pragma force_active on

// Chooses the rank (1-10) with UP/DOWN; A: continue, B: back.
int NintendoWfcUnused_HandleLeadersRankInput(NintendoWfcUnused *wfc) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        (*(volatile u32 *)&wfc->rank)++; // the original accessed the rank as volatile here
        if (*(volatile int *)&wfc->rank > 10) {
            *(volatile u32 *)&wfc->rank = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, *(volatile u32 *)&wfc->rank, 2);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        (*(volatile u32 *)&wfc->rank)--;
        if (*(volatile int *)&wfc->rank < 1) {
            *(volatile u32 *)&wfc->rank = 10;
        }
        NintendoWfcUnused_PrintNumber(&wfc->rankWindow, wfc->rankString, *(volatile u32 *)&wfc->rank, 2);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        NintendoWfcUnused_SetState(wfc, 11, 32);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = 23;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_AskLeadersRoom(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 6, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 33);
    return 3;
}

int NintendoWfcUnused_StartLeadersRoomInput(NintendoWfcUnused *wfc) {
    wfc->room = 1;
    DrawFrameAndWindow1(&wfc->roomWindow, FALSE, 31, 14);
    NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, wfc->room, 3);
    NintendoWfcUnused_SetArrowsVisible(wfc, 0, TRUE);
    CopyWindowToVram(&wfc->roomWindow);
    wfc->state = 34;
    return 3;
}

#pragma force_active on

// Chooses the room (1-roomCount) with UP/DOWN; A: continue, B: back.
int NintendoWfcUnused_HandleLeadersRoomInput(NintendoWfcUnused *wfc) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        (*(volatile u32 *)&wfc->room)++; // the original accessed the room as volatile here
        if (*(volatile int *)&wfc->room == wfc->roomCount + 1) {
            *(volatile u32 *)&wfc->room = 1;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, *(volatile u32 *)&wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        (*(volatile u32 *)&wfc->room)--;
        if (*(volatile int *)&wfc->room < 1) {
            *(volatile u32 *)&wfc->room = wfc->roomCount;
        }
        NintendoWfcUnused_PrintNumber(&wfc->roomWindow, wfc->roomString, *(volatile u32 *)&wfc->room, 3);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = 35;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfcUnused_SetArrowsVisible(wfc, 0, FALSE);
        wfc->state = 29;
        PlaySE(SEQ_SE_DP_SELECT);
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_PrintReceivingLeaders(NintendoWfcUnused *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintFormattedMessage(wfc, wfc->msgDataTower, 1, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 36);
    wfc->state = 36;
    return 3;
}

int NintendoWfcUnused_RequestLeadersDownload(NintendoWfcUnused *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->rank, wfc->room, wfc->downloadBuffer);
    wfc->state = 37;
    wfc->timeoutCounter = 0;
    NintendoWfcUnused_ShowWaitingIcon(wfc);
    return 3;
}

#pragma force_active on

int NintendoWfcUnused_WaitLeadersDownload(NintendoWfcUnused *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            // BattleRoomDownload_SetLeaders takes u32; the original call truncated rank and room to u8
            BattleRoomDownload_SetLeaders((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer + 0x63C, (u8)wfc->rank, (u8)wfc->room);
            NintendoWfcUnused_SetState(wfc, 39, 38);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = 11;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -4:
        case -7:
            wfc->state = 11;
            wfc->httpResult = result;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfcUnused_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        if (++wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfcUnused_PrintLeadersReceived(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 3, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 45);
    NintendoWfcUnused_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

int NintendoWfcUnused_StartDisconnect(NintendoWfcUnused *wfc) {
    wfc->state = 41;
    return 3;
}

int NintendoWfcUnused_StartSave(NintendoWfcUnused *wfc) {
    Save_PrepareForAsyncWrite(wfc->args->saveData, 2);
    wfc->state = 40;
    return 3;
}

int NintendoWfcUnused_Save(NintendoWfcUnused *wfc) {
    if (Save_WriteFileAsync(wfc->args->saveData) == 2) {
        wfc->state = wfc->nextState;
        NintendoWfcUnused_HideWaitingIcon(wfc);
    }
    return 3;
}

int NintendoWfcUnused_AskCancelChallenge(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 4, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 48, 22);
    return 3;
}

int NintendoWfcUnused_HandleCancelChallengeChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = 13;
        } else {
            wfc->state = 41;
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

int NintendoWfcUnused_AskCancelDownload(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 9, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 48, 24);
    return 3;
}

int NintendoWfcUnused_HandleCancelDownloadChoice(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = 29;
        } else {
            wfc->state = 41;
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

int NintendoWfcUnused_PrintDisconnecting(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, 26, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 42);
    return 3;
}

int NintendoWfcUnused_CloseConnection(NintendoWfcUnused *wfc) {
    DWC_CleanupInet();
    NintendoWfcUnused_SetNextMode(wfc, 0, 0);
    wfc->state = 43;
    return 3;
}

int NintendoWfcUnused_PrintDisconnected(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataError, 27, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 49, 46);
    wfc->messageTimer = 0;
    return 3;
}

int ov73_021E9D10(void) {
    return 3;
}

int NintendoWfcUnused_StartExit(NintendoWfcUnused *wfc) {
    WirelessStatusIcon_Destroy();
    CommState_FreeWfcSession();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 8, 1, HEAP_ID_150);
    wfc->state = 0;
    return 4;
}

int ov73_021E9D44(NintendoWfcUnused *wfc) {
    int response = NintendoWfcUnused_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response) {
        if (response == 2) {
            wfc->state = 0;
        } else {
            wfc->state = 41;
        }
    }
    return 3;
}

int NintendoWfcUnused_ShowHttpError(NintendoWfcUnused *wfc) {
    int msgNo = 0;
    switch (wfc->httpResult) {
    case -6:
    case -2:
        msgNo = 13;
        break;
    case 1:
        msgNo = 11;
        break;
    case -1:
    case 2:
        msgNo = 12;
        break;
    case -7:
    case -4:
        msgNo = 14;
        break;
    case -3:
        msgNo = 15;
        break;
    }
    NintendoWfcUnused_HideWaitingIcon(wfc);
    NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, msgNo, 1, 0xF0F);
    NintendoWfcUnused_SetState(wfc, 47, 54);
    return 3;
}

int NintendoWfcUnused_Disconnect(NintendoWfcUnused *wfc) {
    switch (wfc->subState) {
    case 0:
        NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 16, 1, 0xF0F);
        wfc->subState++;
        break;
    case 1:
        if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
            CommState_FreeWfcSession();
            DWC_CleanupInet();
            wfc->subState++;
        }
        break;
    case 2:
        NintendoWfcUnused_PrintMessage(wfc, wfc->msgDataTower, 17, 1, 0xF0F);
        wfc->subState++;
        break;
    case 3:
        if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
            wfc->subState++;
        }
        break;
    default:
        wfc->subTimer++;
        if (wfc->subTimer > 30) {
            NintendoWfcUnused_SetNextMode(wfc, 0, 0);
            wfc->state = 46;
        }
        break;
    }
    return 3;
}

int NintendoWfcUnused_WaitMessage(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->state = wfc->nextState;
    }
    return 3;
}

int NintendoWfcUnused_WaitMessageAndDelay(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        if (wfc->messageTimer > 30) {
            wfc->state = wfc->nextState;
        }
        ++wfc->messageTimer;
    }
    return 3;
}

int NintendoWfcUnused_OpenYesNoPrompt(NintendoWfcUnused *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->yesNoPrompt = NintendoWfcUnused_CreateYesNoPrompt(wfc->bgConfig, 10, 0x244);
        wfc->state = wfc->nextState;
    }
    return 3;
}

void NintendoWfcUnused_PrintFormattedMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    String *string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(wfc->msgFormat, wfc->msgString, string);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

int NintendoWfcUnused_ShowDwcError(NintendoWfcUnused *wfc) {
    int msgNo = ov73_021E6A70(-wfc->dwcErrorCode, wfc->dwcErrorType);
    NintendoWfcUnused_ShowErrorMessage(wfc, msgNo, -wfc->dwcErrorCode);
    wfc->state = 56;
    return 3;
}

int NintendoWfcUnused_WaitErrorDismiss(NintendoWfcUnused *wfc) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
        EraseFrameAndWindow1(&wfc->errorWindow, FALSE);
        wfc->state = 0;
    }
    return 3;
}

void NintendoWfcUnused_PrintMessage(NintendoWfcUnused *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    ReadMsgDataIntoString(msgData, msgNo, wfc->msgString);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
}

int NintendoWfcUnused_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId) {
    int width;
    switch (alignment) {
    case 1:
        width = FontID_String_GetWidth(fontId, string, 0);
        x = ((window->width << 3) - width) / 2;
        break;
    case 2:
        width = FontID_String_GetWidth(fontId, string, 0);
        x = (window->width << 3) - width;
        break;
    }
    return x;
}

void NintendoWfcUnused_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = NintendoWfcUnused_GetAlignedTextX(window, string, x, alignment, color, 1);
    AddTextPrinterParameterizedWithColor(window, 1, string, x, y, 0, color, NULL);
}

void NintendoWfcUnused_PrintAlignedTextSmall(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = NintendoWfcUnused_GetAlignedTextX(window, string, x, alignment, color, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, x, y, 0, color, NULL);
}

void NintendoWfcUnused_PrintNumber(Window *window, String *string, int number, int digits) {
    FillWindowPixelBuffer(window, 15);
    String16_FormatInteger(string, number, digits, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfcUnused_PrintAlignedTextSmall(window, string, 0, 1, 1, MAKE_TEXT_COLOR(1, 2, 0));
}

void NintendoWfcUnused_CreateWirelessIcon(NintendoWfcUnused *wfc) {
    WirelessStatusIcon_Create();
}

void NintendoWfcUnused_SetArrowsVisible(NintendoWfcUnused *wfc, u32 which, BOOL visible) {
    if (which <= 1) {
        Sprite_SetDrawFlag(wfc->arrows[0], visible);
        Sprite_SetDrawFlag(wfc->arrows[1], visible);
    }
}

void NintendoWfcUnused_ShowWaitingIcon(NintendoWfcUnused *wfc) {
    if (!wfc->waitingIcon) {
        wfc->waitingIcon = WaitingIcon_New(&wfc->msgWindow, 1);
    }
}

void NintendoWfcUnused_HideWaitingIcon(NintendoWfcUnused *wfc) {
    if (wfc->waitingIcon) {
        WaitingIcon_Delete(wfc->waitingIcon);
        wfc->waitingIcon = NULL;
    }
}

#pragma force_active on

void NintendoWfcUnused_PrintErrorMessage(NintendoWfcUnused *wfc, int msgNo) {
    String *string = String_New(256, HEAP_ID_150);

    ReadMsgDataIntoString(wfc->msgDataError, msgNo, string);
    StringExpandPlaceholders(wfc->msgFormat, wfc->errorString, string);
    FillWindowPixelBuffer(&wfc->errorWindow, 15);
    DrawFrameAndWindow1(&wfc->errorWindow, TRUE, 31, 11);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->errorWindow, 1, wfc->errorString, 0, 0, 0, NULL);
    String_Delete(string);
}

#pragma force_active reset

void NintendoWfcUnused_ShowErrorMessage(NintendoWfcUnused *wfc, int msgNo, int errorCode) {
    if (msgNo == -1) {
        msgNo = 11;
    }
    BufferIntegerAsString(wfc->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&wfc->msgWindow, TRUE);
    NintendoWfcUnused_PrintErrorMessage(wfc, msgNo);
}
