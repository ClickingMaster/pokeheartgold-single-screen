#include "nintendo_wfc_connect.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "nnsys/fnd/expheap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_00_thumb.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "frontier_data_access.h"
#include "save_wii_message.h"
#include "comm_wireless.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "frontier_wifi_upload_record.h"
#include "network_overlay_loader.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Touch YES/NO menu on the sub screen (texts from message bank 191).
typedef struct NintendoWfcYesNoMenu {
    BgConfig *bgConfig;   // 0x00
    Sprite *cursorSprite; // 0x04
    Window yesWindow;     // 0x08
    Window noWindow;      // 0x18
    enum HeapID heapID;   // 0x28
    u8 initialChoice;     // 0x2C
    u8 state;             // 0x2D, index into sNintendoWfcYesNoStates
    u8 bgPltt;            // 0x2E
    u8 bgLayer;           // 0x2F
    u8 fontPltt;          // 0x30
    u8 baseTile;          // 0x31
    u8 textBgLayer;       // 0x32
    u8 choice;            // 0x33, 0: yes, 1: no
    int cursorOffsetY;    // 0x34
} NintendoWfcYesNoMenu;   // size: 0x38

// Number pad on the sub screen (rank / Battle Room number entry).
typedef struct NintendoWfcNumberInput {
    u8 unk_0;             // 0x0
    u8 maxValue;          // 0x1, 100: three digits
    s8 value;             // 0x2, entered value, -1: quit
    u8 cursor;            // 0x3, pad key under the cursor
    s16 digitPos;         // 0x4
    u8 state;             // 0x6, index into sNintendoWfcNumberInputStates
    u8 nextState;         // 0x7
    s8 digits[3];         // 0x8, -1: empty
    u8 unk_B;             // 0xB
} NintendoWfcNumberInput; // size: 0xC

// Brief palette flash of a pressed button: a sprite, or a rectangle of a bg layer.
typedef struct NintendoWfcButtonFlash {
    u8 isBgRect;          // 0x0
    u8 state;             // 0x1
    u8 target;            // 0x2, sprite index or bg layer
    u8 timer;             // 0x3
    u8 width;             // 0x4
    u8 height;            // 0x5
    u8 x;                 // 0x6
    u8 y;                 // 0x7
    u16 pressedPltt;      // 0x8
    u16 normalPltt;       // 0xA
} NintendoWfcButtonFlash; // size: 0xC

// Background palette cycling (VBlank task).
typedef struct NintendoWfcPaletteCycle {
    SysTask *task;          // 0x000
    int active;             // 0x004
    u16 basePltt[64];       // 0x008
    u16 cyclePltt[21 * 16]; // 0x088
    s16 index;              // 0x328
    s8 direction;           // 0x32A
    u8 tick;                // 0x32B
    u32 screens;            // 0x32C, 0: both, 1: main only, 2: sub only
} NintendoWfcPaletteCycle;  // size: 0x330

#define NINTENDO_WFC_SPRITE_DIGIT_0       0
#define NINTENDO_WFC_SPRITE_PAD_CURSOR    3
#define NINTENDO_WFC_SPRITE_DIGIT_CURSOR  4
#define NINTENDO_WFC_SPRITE_BUTTON_0      5
#define NINTENDO_WFC_SPRITE_YES_NO_CURSOR 8
#define NINTENDO_WFC_SPRITE_COUNT         9

typedef struct NintendoWfcConnect {
    NintendoWifiConnectArgs *args;              // 0x0000
    BgConfig *bgConfig;                         // 0x0004
    u8 filler_0008[8];                          // 0x0008
    int mode;                                   // 0x0010, index into the sNintendoWfcMode* tables
    int nextMode;                               // 0x0014
    int unk_0018;                               // 0x0018
    int state;                                  // 0x001C, index into sNintendoWfcStates
    int nextState;                              // 0x0020
    void *heapMem;                              // 0x0024
    NNSFndHeapHandle expHeap;                   // 0x0028
    u8 connection[0x64];                          // 0x002C, passed to DWC_InitInetEx
    u32 ownRank;                                // 0x0090
    u32 rank;                                   // 0x0094
    int roomCount;                              // 0x0098
    u32 room;                                   // 0x009C
    u8 downloadBuffer[0xA38];                   // 0x00A0
    FrontierWifiUploadRecord uploadRecord;            // 0x0AD8
    u8 filler_0BBC[0x14];                       // 0x0BBC
    MessageFormat *msgFormat;                   // 0x0BD0
    MsgData *msgDataTower;                      // 0x0BD4, Wi-Fi Battle Tower (bank 774)
    MsgData *msgDataWfc;                        // 0x0BD8, Nintendo WFC (bank 778)
    MsgData *msgDataError;                      // 0x0BDC, Nintendo WFC errors (bank 800)
    String *msgString;                          // 0x0BE0
    String *titleString;                        // 0x0BE4
    String *unk_0BE8;                           // 0x0BE8
    String *unk_0BEC;                           // 0x0BEC
    String *errorString;                        // 0x0BF0
    u32 textPrinterId;                          // 0x0BF4
    SpriteList *spriteList;                     // 0x0BF8
    G2dRenderer renderer;                       // 0x0BFC
    GF_2DGfxResMan *resMan[4];                  // 0x0D24
    SpriteResource *resObjs[12];                // 0x0D34
    SpriteResourcesHeader spriteHeaders[3];     // 0x0D64
    u8 filler_0DD0[0x24];                       // 0x0DD0
    Sprite *sprites[NINTENDO_WFC_SPRITE_COUNT]; // 0x0DF4
    Window msgWindow;                           // 0x0E18
    Window unk_0E28;                            // 0x0E28
    Window unk_0E38;                            // 0x0E38
    Window titleWindow;                         // 0x0E48
    Window errorWindow;                         // 0x0E58
    u8 filler_0E68[0xE4];                       // 0x0E68
    WaitingIcon *waitingIcon;                   // 0x0F4C
    int dwcError;                               // 0x0F50
    int dwcErrorCode;                           // 0x0F54
    int dwcErrorType;                           // 0x0F58
    int httpResult;                             // 0x0F5C
    int messageTimer;                           // 0x0F60
    WifiServerProfile profile;                        // 0x0F64
    u32 profileResponse[2];                     // 0x0FC8
    s16 subState;                               // 0x0FD0
    s16 subTimer;                               // 0x0FD2
    int timeoutCounter;                         // 0x0FD4
    NintendoWfcPaletteCycle paletteCycle;       // 0x0FD8
    YesNoPrompt *yesNoPrompt;                   // 0x1308
    NintendoWfcNumberInput numberInput;         // 0x130C
    Window numberInputWindow;                   // 0x1318
    Window buttonWindows[3];                    // 0x1328
    NintendoWfcButtonFlash buttonFlash;         // 0x1358
    u16 padColumn[2];                           // 0x1364, pad column to return to from the bottom rows
    u32 twoDigits;                              // 0x1368
    NintendoWfcYesNoMenu yesNoMenu;             // 0x136C
} NintendoWfcConnect;                           // size: 0x13A4

// The HTTP request state of the Wi-Fi Battle Tower (gamestats) requests.
typedef struct NintendoWfcHttp {
    volatile s32 state; // 0x00, 1: idle, 2/4/6/8/10: request running, 12: done
    s32 result;         // 0x04
    u32 profileId;      // 0x08
    u64 friendKey;      // 0x0C
    u8 rank;            // 0x14, first byte of the request buffer gNintendoWfcHttpRequestBuffer
    u8 room;            // 0x15
} NintendoWfcHttp;

typedef struct NintendoWfcHttpResponse {
    u8 unk_0[3];
    volatile u8 status; // 0x3, first byte of the response buffer ov72_0223B923
    u32 unk_4;
    u8 *buffer; // 0x8
} NintendoWfcHttpResponse;

typedef struct NintendoWfcGlobal {
    NNSFndHeapHandle heap;   // 0x0, heap of the DWC allocator callbacks
    NintendoWfcConnect *wfc; // 0x4
    u8 unk_8[0xC];
} NintendoWfcGlobal;

typedef int (*NintendoWfcModeFunc)(NintendoWfcConnect *wfc, int state);

// One row per mode: {init, main, exit}. Each column is addressed through its own label.
typedef struct NintendoWfcModeRow {
    NintendoWfcModeFunc func[3];
} NintendoWfcModeRow;

typedef struct BattleRoomDownload BattleRoomDownload;

// Defined in OVY_0 / OVY_38 (DWC and HTTP wrappers); not declared in any header.
int DWC_GetLastError(void *code);
int DWC_GetLastErrorEx(int *code, u32 *category);
void DWC_ClearError(void);
void DWC_SetMemFunc(void *(*alloc)(void *, u32, int), void (*freeFunc)(u32, void *));
void DWC_InitInetEx(void *connection, int priority, int unused, int sslPriority);
void DWC_ConnectInetAsync(void);
int DWC_CheckInet(void);
void DWC_ProcessInet(void);
int DWC_GetInetStatus(void);
void DWC_CleanupInet(void);
int DWC_GetLinkLevel(void);
void DWC_GetApInfo(void *info);
void DWC_UpdateConnection(void);
void DWC_GetDateTime(void *date, void *time);
void DWC_NASLoginAsync(void);
int DWC_NASLoginProcess(void);
void DWC_ShutdownFriendsMatch(void);
void DWC_ShutdownGHTTP(void);
void GameStats_Start(void);
int GameStats_PrepareRequest(const char *url, u32 profileId, void *request, int requestSize, void *response, int responseSize);
int GameStats_Update(void);
void GameStats_Shutdown(void);
int GameStats_GetError(void);
int GameStats_GetResponseLength(void);

// Defined in other ARM9 sources; not declared in their headers.
BOOL LoadOVY38(void);
void UnloadOVY38(void);
void CommWireless_Terminate(void);
void CommState_InitWfcSession(SaveData *saveData);
void CommState_FreeWfcSession(void);
void CommState_ShowRestartErrorAndHalt(void);
void WirelessStatusIcon_SetLevel(s32 level);
int WCM_GetPhase(void);
u64 DWC_CreateFriendKey(const void *data);
u8 FrontierData_GetWifiWinCount(FrontierData *data);
void BattleRoomDownload_MarkRoom(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
BOOL BattleRoomDownload_IsRoomMarked(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
void BattleRoomDownload_SetTrainers(BattleRoomDownload *data, void *records, u32 a, u32 b);
void BattleRoomDownload_GetTrainersRoom(BattleRoomDownload *data, u8 *out);
void BattleRoomDownload_SetLeaders(BattleRoomDownload *data, void *records, u32 a, u32 b);

extern NintendoWfcHttp sNintendoWfcHttp;
extern u8 ov72_0223B82C[];
extern u8 gNintendoWfcHttpRequestBuffer[];
extern u8 ov72_0223B900[];
extern u8 ov72_0223B91B[];
extern NintendoWfcHttpResponse gNintendoWfcHttpResponse;
extern u8 ov72_0223B923[];
extern NintendoWfcGlobal sNintendoWfcGlobal;
extern const char sNintendoWfcRoomNumUrl[];
extern const char sNintendoWfcDownloadUrl[];
extern const char sNintendoWfcUploadUrl[];
extern const char sNintendoWfcInfoUrl[];
extern const char sNintendoWfcSetProfileUrl[];
extern const NintendoWfcModeRow sNintendoWfcModeInit[];
extern const NintendoWfcModeRow sNintendoWfcModeMain[];
extern const NintendoWfcModeRow sNintendoWfcModeExit[];
extern int (*const sNintendoWfcStates[])(NintendoWfcConnect *wfc);
extern int (*const sNintendoWfcNumberInputStates[])(NintendoWfcConnect *wfc);
extern int (*const sNintendoWfcYesNoStates[])(NintendoWfcYesNoMenu *menu);
extern const ObjCharTransferTemplate sNintendoWfcObjCharTransferTemplate;
extern const GraphicsModes sNintendoWfcGraphicsModes;
extern const GraphicsBanks sNintendoWfcGraphicsBanks;
extern const BgTemplate sNintendoWfcBgTemplate_Sub1;
extern const BgTemplate sNintendoWfcBgTemplate_Sub2;
extern const BgTemplate sNintendoWfcBgTemplate_Main1;
extern const BgTemplate sNintendoWfcBgTemplate_Main0;
extern const BgTemplate sNintendoWfcBgTemplate_Sub0;
extern const u16 gNintendoWfcButtonSpritePositions[][2];
extern const u16 gNintendoWfcDigitSpritePositions[][3][2];
extern const u16 ov72_0223B366[][3][2];
extern const u16 gNintendoWfcPadCursorPositions[][2];
extern const u16 ov72_0223B3A6[][2];
extern const u8 ov72_0223B464[];
extern const u8 ov72_0223B466[];
extern const u8 ov72_0223B468[];
extern const u8 ov72_0223B46A[];
extern const int gNintendoWfcNumberInputWindowX[];
extern const u8 gNintendoWfcPadKeyFlashes[];
extern const u8 ov72_0223B479[];
extern const u8 gNintendoWfcPadNeighbors[][4];
extern const TouchscreenHitbox gNintendoWfcYesNoHitboxes[];
extern const TouchscreenHitbox gNintendoWfcPadHitboxes_Max100[];
extern const TouchscreenHitbox gNintendoWfcPadHitboxes[];

void NintendoWfcHttp_Update(void);
s32 NintendoWfcHttp_GetResult(void);
BOOL NintendoWfcHttp_StartRequest(const char *url, void *request, int requestSize, void *response, int responseSize);
int NintendoWfcHttp_ConvertError(int error);
void NintendoWfc_VBlankCB(void *arg);
void NintendoWfc_SetGfxBanks(void);
void NintendoWfc_InitArgs(NintendoWfcConnect *wfc, OverlayManager *man);
void NintendoWfc_ExitDummy(NintendoWfcConnect *wfc);
void NintendoWfc_InitObjTransfer(void);
void NintendoWfc_InitSpriteResources(NintendoWfcConnect *wfc);
void NintendoWfc_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcConnect *wfc, SpriteResourcesHeader *header, u32 whichScreen);
void NintendoWfc_CreateSprites(NintendoWfcConnect *wfc);
int NintendoWfc_HandleYesNoPrompt(YesNoPrompt *prompt);
void NintendoWfc_SetState(NintendoWfcConnect *wfc, int state, int nextState);
void NintendoWfc_SetSpritePosition(Sprite *sprite, int x, int y);
void NintendoWfc_MovePadCursor(Sprite *sprite, int key);
void NintendoWfc_MoveDigitCursor(Sprite *sprite, int index, int twoDigits);
void NintendoWfc_SetDigitSpritePosition(Sprite *sprite, int index, int twoDigits);
void *NintendoWfc_Alloc(void *unused, u32 size, int alignment);
void NintendoWfc_Free(u32 unused, void *ptr);
int NintendoWfc_GetWifiIconLevel(void);
void NintendoWfc_SetNextMode(NintendoWfcConnect *wfc, int mode, int a2);
void NintendoWfc_InitGraphics(NintendoWfcConnect *wfc);
void NintendoWfc_FreeGraphics(NintendoWfcConnect *wfc);
void NintendoWfc_InitBgs(BgConfig *bgConfig);
void NintendoWfc_FreeBgs(BgConfig *bgConfig);
void NintendoWfc_LoadSubScreenBg(NintendoWfcConnect *wfc, int numberPad);
void NintendoWfc_LoadBgGraphics(NintendoWfcConnect *wfc);
void NintendoWfc_PaletteCycleTask(SysTask *task, void *data);
void NintendoWfc_AddWindows(NintendoWfcConnect *wfc);
void NintendoWfc_RemoveWindows(NintendoWfcConnect *wfc);
void NintendoWfc_InitStrings(NintendoWfcConnect *wfc);
void NintendoWfc_FreeStrings(NintendoWfcConnect *wfc);
void NintendoWfc_PrintFormattedMessage(NintendoWfcConnect *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
void NintendoWfc_PrintMessage(NintendoWfcConnect *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused);
int NintendoWfc_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId);
void NintendoWfc_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color);
void NintendoWfc_CreateWirelessIcon(NintendoWfcConnect *wfc);
void NintendoWfc_NumberInputDummy(NintendoWfcConnect *wfc, int a1, int a2);
void NintendoWfc_ShowWaitingIcon(NintendoWfcConnect *wfc);
void NintendoWfc_HideWaitingIcon(NintendoWfcConnect *wfc);
void NintendoWfc_PrintErrorMessage(NintendoWfcConnect *wfc, int msgNo);
void NintendoWfc_ShowErrorMessage(NintendoWfcConnect *wfc, int msgNo, int errorCode);
int NintendoWfc_StartNumberInput(NintendoWfcConnect *wfc, int a1, int maxValue);
int NintendoWfc_RunNumberInput(NintendoWfcConnect *wfc);
void NintendoWfc_ShowNumberInputSprites(NintendoWfcConnect *wfc);
int NintendoWfc_GetTouchedPadKey(NintendoWfcConnect *wfc);
u8 NintendoWfc_GetPadNeighbor(u32 key, u32 direction, u16 *padColumn);
int NintendoWfc_GetEnteredNumber(NintendoWfcNumberInput *input, int newDigit);
BOOL NintendoWfc_AreDigitsComplete(NintendoWfcNumberInput *input);
u8 NintendoWfc_GetCursorAfterDigit(NintendoWfcNumberInput *input);
void NintendoWfc_PressPadKey(NintendoWfcConnect *wfc, int key);
BOOL NintendoWfc_HandlePadButtons(NintendoWfcConnect *wfc);
void NintendoWfc_UpdateDigitSprites(NintendoWfcConnect *wfc);
void NintendoWfc_AddNumberInputWindows(NintendoWfcConnect *wfc);
void NintendoWfc_PrintButtonLabel(Window *window, MsgData *msgData, int msgNo);
void NintendoWfc_RemoveNumberInputWindows(NintendoWfcConnect *wfc);
void NintendoWfc_HideNumberInput(NintendoWfcConnect *wfc);
void NintendoWfc_SetButtonFlashPltt(NintendoWfcConnect *wfc, u8 pltt);
BOOL NintendoWfc_UpdateButtonFlash(NintendoWfcConnect *wfc);
void NintendoWfc_StartButtonFlash(NintendoWfcConnect *wfc, int x, int y, int nextState);
void NintendoWfcYesNo_Init(NintendoWfcYesNoMenu *menu);
int NintendoWfcYesNo_Run(NintendoWfcYesNoMenu *menu);
void NintendoWfcYesNo_LoadGraphics(NintendoWfcYesNoMenu *menu);
void NintendoWfcYesNo_AddWindows(NintendoWfcYesNoMenu *menu);
void NintendoWfcYesNo_RemoveWindows(NintendoWfcYesNoMenu *menu);
void NintendoWfcYesNo_ShowCursor(NintendoWfcYesNoMenu *menu);
void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoMenu *menu, int choice);
void NintendoWfcHttp_Init(u32 profileId, u64 friendKey);
BOOL NintendoWfcHttp_IsDone(void);
void NintendoWfcHttp_RequestRoomCount(int rank);
void NintendoWfcHttp_RequestDownload(int rank, int room, void *buffer);
void NintendoWfcHttp_RequestUpload(int rank, int room, int winCount, FrontierWifiUploadRecord *record);
void NintendoWfcHttp_RequestInfo(void);
void NintendoWfcHttp_RequestSetProfile(WifiServerProfile *profile, u32 *response);
BOOL NintendoWfc_Init(OverlayManager *man, int *state);
BOOL NintendoWfc_Main(OverlayManager *man, int *state);
BOOL NintendoWfc_Exit(OverlayManager *man, int *state);
int NintendoWfc_SessionInit(NintendoWfcConnect *wfc);
int NintendoWfc_SessionMain(NintendoWfcConnect *wfc);
int NintendoWfc_SessionExit(NintendoWfcConnect *wfc);
int NintendoWfc_AskConnect(NintendoWfcConnect *wfc);
int NintendoWfc_HandleConnectChoice(NintendoWfcConnect *wfc);
int NintendoWfc_AskLogOut(NintendoWfcConnect *wfc);
int NintendoWfc_HandleLogOutChoice(NintendoWfcConnect *wfc);
int NintendoWfc_StartConnect(NintendoWfcConnect *wfc);
int NintendoWfc_WaitConnect(NintendoWfcConnect *wfc);
int NintendoWfc_StartAuth(NintendoWfcConnect *wfc);
int NintendoWfc_WaitAuth(NintendoWfcConnect *wfc);
int NintendoWfc_InitHttp(NintendoWfcConnect *wfc);
int NintendoWfc_RequestInfo(NintendoWfcConnect *wfc);
int NintendoWfc_WaitInfo(NintendoWfcConnect *wfc);
int NintendoWfc_RequestSetProfile(NintendoWfcConnect *wfc);
int NintendoWfc_WaitSetProfile(NintendoWfcConnect *wfc);
int NintendoWfc_RequestRoomCount(NintendoWfcConnect *wfc);
int NintendoWfc_WaitRoomCount(NintendoWfcConnect *wfc);
int NintendoWfc_AskChallengeRoom(NintendoWfcConnect *wfc);
int NintendoWfc_StartChallengeRoomInput(NintendoWfcConnect *wfc);
int NintendoWfc_HandleChallengeRoomInput(NintendoWfcConnect *wfc);
int NintendoWfc_PrintReceivingTrainers(NintendoWfcConnect *wfc);
int NintendoWfc_PrintRoomUnavailable(NintendoWfcConnect *wfc);
int NintendoWfc_RequestTrainersDownload(NintendoWfcConnect *wfc);
int NintendoWfc_WaitTrainersDownload(NintendoWfcConnect *wfc);
int NintendoWfc_PrintTrainersReceived(NintendoWfcConnect *wfc);
int NintendoWfc_PrintSendingRecord(NintendoWfcConnect *wfc);
int NintendoWfc_RequestUpload(NintendoWfcConnect *wfc);
int NintendoWfc_WaitUpload(NintendoWfcConnect *wfc);
int NintendoWfc_PrintRecordSent(NintendoWfcConnect *wfc);
int NintendoWfc_AskLeadersRank(NintendoWfcConnect *wfc);
int NintendoWfc_StartLeadersRankInput(NintendoWfcConnect *wfc);
int NintendoWfc_HandleLeadersRankInput(NintendoWfcConnect *wfc);
int NintendoWfc_AskLeadersRoom(NintendoWfcConnect *wfc);
int NintendoWfc_StartLeadersRoomInput(NintendoWfcConnect *wfc);
int NintendoWfc_HandleLeadersRoomInput(NintendoWfcConnect *wfc);
int NintendoWfc_PrintReceivingLeaders(NintendoWfcConnect *wfc);
int NintendoWfc_RequestLeadersDownload(NintendoWfcConnect *wfc);
int NintendoWfc_WaitLeadersDownload(NintendoWfcConnect *wfc);
int NintendoWfc_PrintLeadersReceived(NintendoWfcConnect *wfc);
int NintendoWfc_StartDisconnect(NintendoWfcConnect *wfc);
int NintendoWfc_StartSave(NintendoWfcConnect *wfc);
int NintendoWfc_Save(NintendoWfcConnect *wfc);
int NintendoWfc_AskCancelChallenge(NintendoWfcConnect *wfc);
int NintendoWfc_HandleCancelChallengeChoice(NintendoWfcConnect *wfc);
int NintendoWfc_AskCancelDownload(NintendoWfcConnect *wfc);
int NintendoWfc_HandleCancelDownloadChoice(NintendoWfcConnect *wfc);
int NintendoWfc_PrintDisconnecting(NintendoWfcConnect *wfc);
int NintendoWfc_CloseConnection(NintendoWfcConnect *wfc);
int NintendoWfc_PrintDisconnected(NintendoWfcConnect *wfc);
int NintendoWfc_UnusedState_Idle(void);
int NintendoWfc_StartExit(NintendoWfcConnect *wfc);
int NintendoWfc_UnusedState_HandleYesNo(NintendoWfcConnect *wfc);
int NintendoWfc_ShowHttpError(NintendoWfcConnect *wfc);
int NintendoWfc_Disconnect(NintendoWfcConnect *wfc);
int NintendoWfc_WaitMessage(NintendoWfcConnect *wfc);
int NintendoWfc_WaitMessageAndDelay(NintendoWfcConnect *wfc);
int NintendoWfc_OpenYesNoMenu(NintendoWfcConnect *wfc);
int NintendoWfc_ShowDwcError(NintendoWfcConnect *wfc);
int NintendoWfc_WaitErrorDismiss(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_FadeOut(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_Setup(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_WaitFadeIn(NintendoWfcConnect *wfc);
BOOL NintendoWfc_NumberInput_Main(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_UnusedState4(void);
int NintendoWfc_NumberInput_UnusedState5(void);
int NintendoWfc_NumberInput_UnusedState6(void);
int NintendoWfc_NumberInput_StartFadeOut(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_Teardown(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_WaitButtonFlash(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_WaitFadeOut(NintendoWfcConnect *wfc);
int NintendoWfc_NumberInput_Finish(void);
int NintendoWfcYesNo_Start(NintendoWfcYesNoMenu *menu);
int NintendoWfcYesNo_HandleInput(NintendoWfcYesNoMenu *menu);
int NintendoWfcYesNo_UnusedState2(void);
BOOL NintendoWfcYesNo_WaitClose(NintendoWfcYesNoMenu *menu);

void NintendoWfcHttp_Init(u32 profileId, u64 friendKey) {
    sNintendoWfcHttp.state = 1;
    sNintendoWfcHttp.result = 0;
    sNintendoWfcHttp.profileId = profileId;
    sNintendoWfcHttp.friendKey = friendKey;
}

#pragma force_active on

void NintendoWfcHttp_Update(void) {
    switch (sNintendoWfcHttp.state) {
    case 0:
    case 1:
    case 12:
        break;
    case 2:
        switch (GameStats_Update()) {
        case 1:
            sNintendoWfcHttp.state = 12;
            sNintendoWfcHttp.result = NintendoWfcHttp_ConvertError(GameStats_GetError());
            GameStats_Shutdown();
            break;
        case 7:
            sNintendoWfcHttp.state = 12;
            if (GameStats_GetResponseLength() > 0) {
                sNintendoWfcHttp.result = gNintendoWfcHttpResponse.status;
            } else {
                sNintendoWfcHttp.result = -2;
            }
            GameStats_Shutdown();
            break;
        }
        break;
    case 4:
        switch (GameStats_Update()) {
        case 1:
            sNintendoWfcHttp.state = 12;
            sNintendoWfcHttp.result = NintendoWfcHttp_ConvertError(GameStats_GetError());
            GameStats_Shutdown();
            break;
        case 7:
            sNintendoWfcHttp.state = 12;
            if (GameStats_GetResponseLength() == 2616) {
                sNintendoWfcHttp.result = 0;
            } else {
                switch (*gNintendoWfcHttpResponse.buffer) {
                case 2:
                    sNintendoWfcHttp.result = -3;
                    break;
                case 5:
                    sNintendoWfcHttp.result = -2;
                    break;
                default:
                    sNintendoWfcHttp.result = -5;
                    break;
                }
            }
            GameStats_Shutdown();
            break;
        }
        break;
    case 6:
        switch (GameStats_Update()) {
        case 1:
            sNintendoWfcHttp.state = 12;
            sNintendoWfcHttp.result = NintendoWfcHttp_ConvertError(GameStats_GetError());
            GameStats_Shutdown();
            break;
        case 7:
            sNintendoWfcHttp.state = 12;
            switch (gNintendoWfcHttpResponse.status) {
            case 1:
                sNintendoWfcHttp.result = 0;
                break;
            case 2:
                sNintendoWfcHttp.result = -3;
                break;
            case 4:
                sNintendoWfcHttp.result = -1;
                break;
            case 5:
                sNintendoWfcHttp.result = -2;
                break;
            default:
                sNintendoWfcHttp.result = -5;
                break;
            }
            GameStats_Shutdown();
            break;
        }
        break;
    case 8:
        switch (GameStats_Update()) {
        case 1:
            sNintendoWfcHttp.state = 12;
            sNintendoWfcHttp.result = NintendoWfcHttp_ConvertError(GameStats_GetError());
            GameStats_Shutdown();
            break;
        case 7:
            sNintendoWfcHttp.state = 12;
            switch (gNintendoWfcHttpResponse.status) {
            case 1:
                sNintendoWfcHttp.result = 0;
                break;
            case 3:
                sNintendoWfcHttp.result = 1;
                break;
            case 4:
                sNintendoWfcHttp.result = 2;
                break;
            case 5:
                sNintendoWfcHttp.result = -2;
                break;
            default:
                sNintendoWfcHttp.result = -5;
                break;
            }
            GameStats_Shutdown();
            break;
        }
        break;
    case 10:
        switch (GameStats_Update()) {
        case 1:
            sNintendoWfcHttp.state = 12;
            sNintendoWfcHttp.result = NintendoWfcHttp_ConvertError(GameStats_GetError());
            GameStats_Shutdown();
            break;
        case 7:
            sNintendoWfcHttp.state = 12;
            if (GameStats_GetResponseLength() == 8) {
                sNintendoWfcHttp.result = 0;
            } else {
                switch (gNintendoWfcHttpResponse.status) {
                case 1:
                    sNintendoWfcHttp.result = 0;
                    break;
                case 3:
                    sNintendoWfcHttp.result = 1;
                    break;
                case 4:
                    sNintendoWfcHttp.result = 2;
                    break;
                case 5:
                    sNintendoWfcHttp.result = -2;
                    break;
                default:
                    sNintendoWfcHttp.result = -5;
                    break;
                }
            }
            GameStats_Shutdown();
            break;
        }
        break;
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -4;
        GameStats_Shutdown();
        break;
    }
}

#pragma force_active reset

BOOL NintendoWfcHttp_IsDone(void) {
    switch (sNintendoWfcHttp.state) {
    case 1:
        return TRUE;
    case 12:
        sNintendoWfcHttp.state = 1;
        return TRUE;
    default:
        return FALSE;
    }
}

s32 NintendoWfcHttp_GetResult(void) {
    return sNintendoWfcHttp.result;
}

void NintendoWfcHttp_RequestRoomCount(int rank) {
    GameStats_Start();
    sNintendoWfcHttp.rank = rank - 1;
    if (NintendoWfcHttp_StartRequest(sNintendoWfcRoomNumUrl, gNintendoWfcHttpRequestBuffer, 1, ov72_0223B923, 2)) {
        sNintendoWfcHttp.state = 2;
    } else {
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -5;
        GameStats_Shutdown();
    }
}

void NintendoWfcHttp_RequestDownload(int rank, int room, void *buffer) {
    gNintendoWfcHttpResponse.buffer = buffer;
    GameStats_Start();
    sNintendoWfcHttp.rank = rank - 1;
    sNintendoWfcHttp.room = room - 1;
    if (NintendoWfcHttp_StartRequest(sNintendoWfcDownloadUrl, gNintendoWfcHttpRequestBuffer, 2, buffer, 0xA38)) {
        sNintendoWfcHttp.state = 4;
    } else {
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -5;
        GameStats_Shutdown();
    }
}

#pragma force_active on

void NintendoWfcHttp_RequestUpload(int rank, int room, int winCount, FrontierWifiUploadRecord *record) {
    u8 *dst;
    u8 *src;
    u32 n;
    GameStats_Start();
    memcpy(gNintendoWfcHttpRequestBuffer, record, sizeof(FrontierWifiUploadRecord));
    // the request continues after the record: rank, room, winCount and the friend key
    ov72_0223B900[0x18] = rank - 1;
    ov72_0223B900[0x19] = room - 1;
    ov72_0223B900[0x1A] = winCount;
    dst = ov72_0223B91B;
    src = ov72_0223B82C;
    n = 8;
    do {
        *dst++ = *src++;
    } while (--n);
    if (NintendoWfcHttp_StartRequest(sNintendoWfcUploadUrl, gNintendoWfcHttpRequestBuffer, 239, ov72_0223B923, 2)) {
        sNintendoWfcHttp.state = 6;
    } else {
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -5;
        GameStats_Shutdown();
    }
}

#pragma force_active reset

void NintendoWfcHttp_RequestInfo(void) {
    GameStats_Start();
    if (NintendoWfcHttp_StartRequest(sNintendoWfcInfoUrl, gNintendoWfcHttpRequestBuffer, 0, ov72_0223B923, 2)) {
        sNintendoWfcHttp.state = 8;
    } else {
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -5;
        GameStats_Shutdown();
    }
}

void NintendoWfcHttp_RequestSetProfile(WifiServerProfile *profile, u32 *response) {
    OS_GetMacAddress(profile->macAddress);
    memcpy(gNintendoWfcHttpRequestBuffer, profile, sizeof(WifiServerProfile));
    gNintendoWfcHttpResponse.buffer = (u8 *)response;
    GameStats_Start();
    if (NintendoWfcHttp_StartRequest(sNintendoWfcSetProfileUrl, gNintendoWfcHttpRequestBuffer, sizeof(WifiServerProfile), gNintendoWfcHttpResponse.buffer, 8)) {
        sNintendoWfcHttp.state = 10;
    } else {
        sNintendoWfcHttp.state = 12;
        sNintendoWfcHttp.result = -5;
        GameStats_Shutdown();
    }
}

BOOL NintendoWfcHttp_StartRequest(const char *url, void *request, int requestSize, void *response, int responseSize) {
    int result = GameStats_PrepareRequest(url, sNintendoWfcHttp.profileId, request, requestSize, response, responseSize);
    switch (result) {
    case 0:
        return TRUE;
    case 1:
        break;
    case 2:
        break;
    }
    return FALSE;
}

int NintendoWfcHttp_ConvertError(int error) {
    int ret;
    switch (error) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
    case 14:
    case 20:
    case 21:
    case 22:
    case 23:
    case 32:
        ret = ~4;
        break;
    case 2:
    case 8:
    case 25:
        ret = ~4;
        break;
    case 11:
        ret = ~6;
        break;
    case 12:
    case 13:
        ret = ~6;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 26:
    case 27:
    case 28:
    case 29:
    case 31:
        ret = ~1;
        break;
    case 15:
    case 30:
        ret = ~1;
        break;
    default:
        ret = ~4;
        break;
    }
    if (ret != ~4) {
        if (WCM_GetPhase() != 9) {
            ret = ~5;
        }
        DWC_ClearError();
    }
    return ret;
}

#pragma force_active on

BOOL NintendoWfc_Init(OverlayManager *man, int *state) {
    NintendoWfcConnect *wfc;
    switch (*state) {
    case 0: {
        GraphicsModes modes;
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        Heap_Create(HEAP_ID_3, HEAP_ID_67, 0x50000);
        // the definition returns BOOL; unk_020915B0.h declares it void, which changes the scheduling here
        ((BOOL(*)(void))LoadDwcOverlay)();
        LoadOVY38();
        Comm_InitDwc(HEAP_ID_67);
        wfc = OverlayManager_CreateAndGetData(man, sizeof(NintendoWfcConnect), HEAP_ID_67);
        memset(wfc, 0, sizeof(NintendoWfcConnect));
        wfc->bgConfig = BgConfig_Alloc(HEAP_ID_67);
        sNintendoWfcGlobal.wfc = wfc;
        modes = sNintendoWfcGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
        wfc->msgFormat = MessageFormat_New_Custom(11, 32, HEAP_ID_67);
        wfc->msgDataTower = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0774_bin, HEAP_ID_67);
        wfc->msgDataWfc = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_67);
        wfc->msgDataError = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_67);
        SetKeyRepeatTimers(4, 8);
        NintendoWfc_InitArgs(wfc, man);
        NintendoWfc_InitGraphics(wfc);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        wfc->heapMem = Heap_Alloc(HEAP_ID_67, 0x20020);
        wfc->expHeap = NNS_FndCreateExpHeapEx((void *)(((u32)wfc->heapMem + 31) & ~31), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        *state = 1;
        break;
    }
    case 1:
        CommWireless_StartUp();
        FontID_Alloc(4, HEAP_ID_67);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

BOOL NintendoWfc_Main(OverlayManager *man, int *state) {
    NintendoWfcConnect *wfc = OverlayManager_GetData(man);
    DWC_UpdateConnection();
    NintendoWfcHttp_Update();
    DWC_UpdateConnection();
    switch (*state) {
    case 0:
        if (CommWireless_IsStartedUp()) {
            sNintendoWfcGlobal.heap = wfc->expHeap;
            DWC_SetMemFunc(NintendoWfc_Alloc, NintendoWfc_Free);
            *state = 1;
        }
        break;
    case 1:
        *state = sNintendoWfcModeInit[wfc->mode].func[0](wfc, *state);
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *state = 3;
        }
        break;
    case 3:
        *state = sNintendoWfcModeMain[wfc->mode].func[0](wfc, *state);
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            *state = sNintendoWfcModeExit[wfc->mode].func[0](wfc, *state);
        }
        break;
    case 5:
        return TRUE;
    }
    if (wfc->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(wfc->spriteList);
    }
    return FALSE;
}

BOOL NintendoWfc_Exit(OverlayManager *man, int *state) {
    NintendoWfcConnect *wfc = OverlayManager_GetData(man);
    if (!IsPaletteFadeFinished()) {
        return FALSE;
    }
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    FontID_Release(4);
    Heap_Free(wfc->heapMem);
    UnloadOVY38();
    UnloadDwcOverlay();
    NintendoWfc_FreeGraphics(wfc);
    DestroyMsgData(wfc->msgDataError);
    DestroyMsgData(wfc->msgDataWfc);
    DestroyMsgData(wfc->msgDataTower);
    MessageFormat_Delete(wfc->msgFormat);
    NintendoWfc_ExitDummy(wfc);
    CommWireless_Terminate();
    Heap_Free(wfc->bgConfig);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_67);
    return TRUE;
}

void NintendoWfc_VBlankCB(void *arg) {
    volatile u32 *irqCheck = (volatile u32 *)0x027E0000; // the OS IRQ check flag lives at +0x3FF8
    NintendoWfcConnect *wfc = arg;
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(wfc->bgConfig);
    OamManager_ApplyAndResetBuffers();
    irqCheck[0x3FF8 / 4] |= 1;
}

void NintendoWfc_SetGfxBanks(void) {
    GraphicsBanks banks = sNintendoWfcGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void NintendoWfc_InitArgs(NintendoWfcConnect *wfc, OverlayManager *man) {
    wfc->args = OverlayManager_GetArgs(man);
    wfc->mode = 0;
    NintendoWfc_SetNextMode(wfc, 0, 0);
}

void NintendoWfc_ExitDummy(NintendoWfcConnect *wfc) {
}

void NintendoWfc_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sNintendoWfcObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_67);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active on

void NintendoWfc_InitSpriteResources(NintendoWfcConnect *wfc) {
    NARC *narcA237 = NARC_New(NARC_a_2_3_7, HEAP_ID_67);
    NARC *narcA236 = NARC_New(NARC_a_2_3_6, HEAP_ID_67);
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_67);
    wfc->spriteList = G2dRenderer_Init(20, &wfc->renderer, HEAP_ID_67);
    G2dRenderer_SetSubSurfaceCoords(&wfc->renderer, 0, 256 << FX32_SHIFT);
    for (i = 0; i < 4; i++) {
        wfc->resMan[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_67);
    }
    wfc->resObjs[0] = AddCharResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CHAR], narcA236, 1, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_67);
    wfc->resObjs[1] = AddPlttResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_PLTT], narcA236, 0, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, 3, HEAP_ID_67);
    wfc->resObjs[2] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CELL], narcA236, 2, FALSE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    wfc->resObjs[3] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_ANIM], narcA236, 3, FALSE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    wfc->resObjs[4] = AddCharResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CHAR], narcA236, 5, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_67);
    wfc->resObjs[5] = AddPlttResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_PLTT], narcA236, 4, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 3, HEAP_ID_67);
    wfc->resObjs[6] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CELL], narcA236, 6, FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    wfc->resObjs[7] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_ANIM], narcA236, 7, FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    wfc->resObjs[8] = AddCharResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CHAR], narcA237, 12, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_67);
    wfc->resObjs[9] = AddPlttResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_PLTT], narcA237, 11, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_67);
    wfc->resObjs[10] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CELL], narcA237, 13, FALSE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_67);
    wfc->resObjs[11] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_ANIM], narcA237, 14, FALSE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_67);
    SpriteTransfer_CreateCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_CreateCharTransferTask(wfc->resObjs[4]);
    SpriteTransfer_CreateCharTransferTask(wfc->resObjs[8]);
    SpriteTransfer_CreateExtPlttTransferTask(wfc->resObjs[1]);
    SpriteTransfer_CreateExtPlttTransferTask(wfc->resObjs[5]);
    SpriteTransfer_CreateExtPlttTransferTask(wfc->resObjs[9]);
    NARC_Delete(narcA236);
    NARC_Delete(narcA237);
}

#pragma force_active reset

void NintendoWfc_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcConnect *wfc, SpriteResourcesHeader *header, u32 whichScreen) {
    template->spriteList = wfc->spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 1;
    template->whichScreen = (NNS_G2D_VRAM_TYPE)whichScreen;
    template->heapID = HEAP_ID_67;
}

#pragma force_active on

void NintendoWfc_CreateSprites(NintendoWfcConnect *wfc) {
    SpriteTemplate template;
    int i;
    CreateSpriteResourcesHeader(&wfc->spriteHeaders[0], 0, 0, 0, 0, -1, -1, 0, 0, wfc->resMan[0], wfc->resMan[1], wfc->resMan[2], wfc->resMan[3], NULL, NULL);
    CreateSpriteResourcesHeader(&wfc->spriteHeaders[1], 1, 1, 1, 1, -1, -1, 0, 0, wfc->resMan[0], wfc->resMan[1], wfc->resMan[2], wfc->resMan[3], NULL, NULL);
    CreateSpriteResourcesHeader(&wfc->spriteHeaders[2], 2, 2, 2, 2, -1, -1, 0, 0, wfc->resMan[0], wfc->resMan[1], wfc->resMan[2], wfc->resMan[3], NULL, NULL);
    NintendoWfc_InitSpriteTemplate(&template, wfc, &wfc->spriteHeaders[0], NNS_G2D_VRAM_TYPE_2DSUB);
    for (i = 0; i < 3; i++) {
        template.position.x = gNintendoWfcDigitSpritePositions[0][i][0] << FX32_SHIFT;
        template.position.y = (gNintendoWfcDigitSpritePositions[0][i][1] << FX32_SHIFT) + (256 << FX32_SHIFT);
        wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], TRUE);
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], 0);
        Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], FALSE);
    }
    NintendoWfc_InitSpriteTemplate(&template, wfc, &wfc->spriteHeaders[1], NNS_G2D_VRAM_TYPE_2DSUB);
    template.position.x = 32 << FX32_SHIFT;
    template.position.y = 0x151000;
    wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR] = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], TRUE);
    Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], 1);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], FALSE);
    template.position.x = 96 << FX32_SHIFT;
    template.position.y = 0x129000;
    wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR] = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], TRUE);
    Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], 0);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], FALSE);
    for (i = 0; i < 3; i++) {
        template.position.x = gNintendoWfcButtonSpritePositions[i][0] << FX32_SHIFT;
        template.position.y = (gNintendoWfcButtonSpritePositions[i][1] << FX32_SHIFT) + (256 << FX32_SHIFT);
        wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + i], TRUE);
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + i], 3);
        Sprite_SetPriority(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + i], 1);
        Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + i], FALSE);
    }
    NintendoWfc_InitSpriteTemplate(&template, wfc, &wfc->spriteHeaders[2], NNS_G2D_VRAM_TYPE_2DSUB);
    template.position.x = 128 << FX32_SHIFT;
    template.position.y = 0x151000;
    wfc->sprites[NINTENDO_WFC_SPRITE_YES_NO_CURSOR] = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(wfc->sprites[NINTENDO_WFC_SPRITE_YES_NO_CURSOR], TRUE);
    Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_YES_NO_CURSOR], 1);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_YES_NO_CURSOR], FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    WirelessStatusIcon_Create();
}

#pragma force_active reset

int NintendoWfc_HandleYesNoPrompt(YesNoPrompt *prompt) {
    int response = YesNoPrompt_HandleInput(prompt);
    if (response) {
        YesNoPrompt_Destroy(prompt);
    }
    return response;
}

void NintendoWfc_SetState(NintendoWfcConnect *wfc, int state, int nextState) {
    wfc->state = state;
    wfc->nextState = nextState;
}

void NintendoWfc_SetSpritePosition(Sprite *sprite, int x, int y) {
    VecFx32 pos;
    float f;
    if (x > 0) {
        f = (float)(x << FX32_SHIFT) + 0.5f;
    } else {
        f = (float)(x << FX32_SHIFT) - 0.5f;
    }
    pos.x = (fx32)f;
    if (y > 0) {
        f = (float)(y << FX32_SHIFT) + 0.5f;
    } else {
        f = (float)(y << FX32_SHIFT) - 0.5f;
    }
    pos.y = (fx32)f;
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}

void NintendoWfc_MovePadCursor(Sprite *sprite, int key) {
    NintendoWfc_SetSpritePosition(sprite, gNintendoWfcPadCursorPositions[key][0], ov72_0223B3A6[key][0] + 256);
    if (key < 10) {
        Sprite_SetAnimCtrlSeq(sprite, 1);
    } else {
        Sprite_SetAnimCtrlSeq(sprite, 2);
    }
}

void NintendoWfc_MoveDigitCursor(Sprite *sprite, int index, int twoDigits) {
    NintendoWfc_SetSpritePosition(sprite, gNintendoWfcDigitSpritePositions[twoDigits][index][0], 0x129);
}

void NintendoWfc_SetDigitSpritePosition(Sprite *sprite, int index, int twoDigits) {
    NintendoWfc_SetSpritePosition(sprite, gNintendoWfcDigitSpritePositions[twoDigits][index][0], ov72_0223B366[twoDigits][index][0] + 256);
}

void *NintendoWfc_Alloc(void *unused, u32 size, int alignment) {
    OSIntrMode enabled = OS_DisableInterrupts();
    void *result = NNS_FndAllocFromExpHeapEx(sNintendoWfcGlobal.heap, size, alignment);
    OS_RestoreInterrupts(enabled);
    return result;
}

void NintendoWfc_Free(u32 unused, void *ptr) {
    OSIntrMode enabled;
    if (!ptr) {
        return;
    }
    enabled = OS_DisableInterrupts();
    NNS_FndFreeToExpHeap(sNintendoWfcGlobal.heap, ptr);
    OS_RestoreInterrupts(enabled);
}

int NintendoWfc_GetWifiIconLevel(void) {
    return 3 - DWC_GetLinkLevel();
}

void NintendoWfc_SetNextMode(NintendoWfcConnect *wfc, int mode, int a2) {
    wfc->nextMode = mode;
    wfc->unk_0018 = a2;
}

void NintendoWfc_InitGraphics(NintendoWfcConnect *wfc) {
    NintendoWfc_SetGfxBanks();
    NintendoWfc_InitObjTransfer();
    NintendoWfc_InitSpriteResources(wfc);
    NintendoWfc_CreateSprites(wfc);
    Main_SetVBlankIntrCB(NintendoWfc_VBlankCB, wfc);
}

void NintendoWfc_FreeGraphics(NintendoWfcConnect *wfc) {
    int i;
    SpriteTransfer_DeleteCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_DeleteCharTransferTask(wfc->resObjs[4]);
    SpriteTransfer_DeletePlttTransferTask(wfc->resObjs[1]);
    SpriteTransfer_DeletePlttTransferTask(wfc->resObjs[5]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(wfc->resMan[i]);
    }
    SpriteList_Delete(wfc->spriteList);
    wfc->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

int NintendoWfc_SessionInit(NintendoWfcConnect *wfc) {
    NintendoWfc_InitStrings(wfc);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 8, 1, HEAP_ID_67);
    NintendoWfc_InitBgs(wfc->bgConfig);
    NintendoWfc_LoadBgGraphics(wfc);
    NintendoWfc_AddWindows(wfc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, TRUE);
    wfc->ownRank = FrontierData_WifiRankAction(wfc->args->frontierData, 0);
    wfc->rank = wfc->ownRank;
    wfc->room = 1;
    wfc->waitingIcon = NULL;
    if (!DWC_CheckInet()) {
        NintendoWifiConnectArgs *args = wfc->args;
        if (args->connectNow != 0) {
            CommState_InitWfcSession(args->saveData);
            NintendoWfc_PrintMessage(wfc, wfc->msgDataWfc, 1, 1, 0xF0F);
            NintendoWfc_SetState(wfc, 47, 2);
            NintendoWfc_ShowWaitingIcon(wfc);
        } else {
            Sys_ClearSleepDisableFlag(4);
            wfc->state = 0;
        }
    } else {
        CommState_InitWfcSession(wfc->args->saveData);
        wfc->state = 51;
    }
    return 2;
}

int NintendoWfc_SessionMain(NintendoWfcConnect *wfc) {
    u32 prevState;
    int result;
    WirelessStatusIcon_SetLevel(NintendoWfc_GetWifiIconLevel());
    prevState = wfc->state;
    result = sNintendoWfcStates[prevState](wfc);
    if (prevState != wfc->state) {
        wfc->subState = 0;
        wfc->subTimer = 0;
    }
    return result;
}

int NintendoWfc_SessionExit(NintendoWfcConnect *wfc) {
    if (wfc->paletteCycle.task != NULL) {
        SysTask_Destroy(wfc->paletteCycle.task);
        wfc->paletteCycle.task = NULL;
        wfc->paletteCycle.active = 0;
    }
    NintendoWfc_FreeStrings(wfc);
    NintendoWfc_RemoveWindows(wfc);
    NintendoWfc_FreeBgs(wfc->bgConfig);
    wfc->paletteCycle.active = 0;
    wfc->mode = wfc->nextMode;
    if (wfc->mode == 0) {
        return 5;
    }
    return 1;
}

void NintendoWfc_InitBgs(BgConfig *bgConfig) {
    BgTemplate main0 = sNintendoWfcBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgTemplate main1 = sNintendoWfcBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
    BgTemplate sub0 = sNintendoWfcBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, FALSE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgTemplate sub1 = sNintendoWfcBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, FALSE);
    BgTemplate sub2 = sNintendoWfcBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, TRUE);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_67);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_67);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_67);
}

void NintendoWfc_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
}

void NintendoWfc_LoadSubScreenBg(NintendoWfcConnect *wfc, int numberPad) {
    NARC *narc;
    BgConfig *bgConfig = wfc->bgConfig;

    if (numberPad == 0) {
        narc = NARC_New(NARC_a_0_8_8, HEAP_ID_67);
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_67);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_67);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_67);
        wfc->paletteCycle.screens = 0;
        NARC_Delete(narc);
        return;
    } else {
        narc = NARC_New(NARC_a_2_3_6, HEAP_ID_67);
        wfc->paletteCycle.screens = 1;
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_67);
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_67);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_67);
        NARC_Delete(narc);
        return;
    }
}

#pragma force_active on

void NintendoWfc_LoadBgGraphics(NintendoWfcConnect *wfc) {
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
    narc = NARC_New(NARC_a_0_8_8, HEAP_ID_67);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_67);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_67);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_67);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_67);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(wfc->args->options), HEAP_ID_67);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_0, 1, 14, Options_GetFrame(wfc->args->options), HEAP_ID_67);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_67);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_67);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, HEAP_ID_67);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_67);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, HEAP_ID_67);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    MI_CpuFill8(&wfc->paletteCycle, 0, sizeof(NintendoWfcPaletteCycle));
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, HEAP_ID_67);
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
    wfc->paletteCycle.task = SysTask_CreateOnVBlankQueue(NintendoWfc_PaletteCycleTask, &wfc->paletteCycle, 20);
    NARC_Delete(narc);
}

#pragma force_active reset

void NintendoWfc_PaletteCycleTask(SysTask *task, void *data) {
    NintendoWfcPaletteCycle *cycle = data;
    if (cycle->active != 0) {
        cycle->tick ^= 1;
        if (cycle->tick & 1) {
            return;
        }
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

void NintendoWfc_AddWindows(NintendoWfcConnect *wfc) {
    AddWindowParameterized(wfc->bgConfig, &wfc->errorWindow, GF_BG_LYR_MAIN_0, 4, 4, 23, 16, 13, 0xA4);
    FillWindowPixelBuffer(&wfc->errorWindow, 0);
    AddWindowParameterized(wfc->bgConfig, &wfc->unk_0E38, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x94);
    FillWindowPixelBuffer(&wfc->unk_0E38, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->unk_0E28, GF_BG_LYR_MAIN_0, 26, 14, 4, 2, 13, 0x9C);
    FillWindowPixelBuffer(&wfc->unk_0E28, 15);
    AddWindowParameterized(wfc->bgConfig, &wfc->titleWindow, GF_BG_LYR_MAIN_0, 4, 1, 24, 2, 13, 0x214);
    FillWindowPixelBuffer(&wfc->titleWindow, 0);
    NintendoWfc_PrintAlignedText(&wfc->titleWindow, wfc->titleString, 0, 1, 1, MAKE_TEXT_COLOR(15, 14, 0));
    AddWindowParameterized(wfc->bgConfig, &wfc->msgWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
}

void NintendoWfc_RemoveWindows(NintendoWfcConnect *wfc) {
    RemoveWindow(&wfc->msgWindow);
    RemoveWindow(&wfc->titleWindow);
    RemoveWindow(&wfc->unk_0E28);
    RemoveWindow(&wfc->unk_0E38);
    RemoveWindow(&wfc->errorWindow);
}

#pragma force_active on

void NintendoWfc_InitStrings(NintendoWfcConnect *wfc) {
    wfc->msgString = String_New(180, HEAP_ID_67);
    wfc->unk_0BE8 = String_New(4, HEAP_ID_67);
    wfc->unk_0BEC = String_New(3, HEAP_ID_67);
    wfc->titleString = NewString_ReadMsgData(wfc->msgDataTower, 12);
    wfc->errorString = String_New(256, HEAP_ID_67);
}

#pragma force_active reset

void NintendoWfc_FreeStrings(NintendoWfcConnect *wfc) {
    String_Delete(wfc->errorString);
    String_Delete(wfc->titleString);
    String_Delete(wfc->unk_0BEC);
    String_Delete(wfc->unk_0BE8);
    String_Delete(wfc->msgString);
}

int NintendoWfc_AskConnect(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataError, 17, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 48, 1);
    return 3;
}

#pragma force_active on

int NintendoWfc_HandleConnectChoice(NintendoWfcConnect *wfc) {
    int choice = NintendoWfcYesNo_Run(&wfc->yesNoMenu);
    if (choice) {
        if (choice == 2) {
            CommState_FreeWfcSession();
            NintendoWfc_SetNextMode(wfc, 0, 0);
            wfc->state = 46;
        } else {
            NintendoWfc_PrintMessage(wfc, wfc->msgDataWfc, 1, 1, 0xF0F);
            NintendoWfc_SetState(wfc, 47, 2);
            NintendoWfc_ShowWaitingIcon(wfc);
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfc_AskLogOut(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataWfc, 12, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 48, 52);
    return 3;
}

int NintendoWfc_HandleLogOutChoice(NintendoWfcConnect *wfc) {
    int result = NintendoWfcYesNo_Run(&wfc->yesNoMenu);
    if (result != 0) {
        if (result == 2) {
            if (DWC_CheckInet() == 0) {
                wfc->state = 0;
            } else {
                wfc->state = 41;
            }
        } else {
            if (DWC_CheckInet() == 0) {
                NintendoWfc_SetNextMode(wfc, 0, 0);
                wfc->state = 46;
            } else {
                wfc->state = 41;
            }
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

int NintendoWfc_StartConnect(NintendoWfcConnect *wfc) {
    CommState_InitWfcSession(wfc->args->saveData);
    Sys_SetSleepDisableFlag(4);
    NintendoWfc_CreateWirelessIcon(wfc);
    DWC_InitInetEx(wfc->connection, 2, 1, 20);
    DWC_SetAuthServer(2);
    DWC_ConnectInetAsync();
    wfc->state = 3;
    return 3;
}

int NintendoWfc_WaitConnect(NintendoWfcConnect *wfc) {
    u8 buf[0x44];
    DWC_ProcessInet();
    if (DWC_CheckInet() != 0) {
        switch (DWC_GetInetStatus()) {
        case 7:
            wfc->dwcError = DWC_GetLastError(buf + 4);
            wfc->dwcErrorCode = *(u32 *)(buf + 4);
            DWC_ClearError();
            DWC_CleanupInet();
            NintendoWfc_HideWaitingIcon(wfc);
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
            DWC_GetLastError(buf);
            NintendoWfc_HideWaitingIcon(wfc);
            wfc->state = 53;
            wfc->httpResult = -2;
            break;
        case 4:
            DWC_GetApInfo(buf + 8);
            wfc->state = 4;
            break;
        }
    }
    return 3;
}

int NintendoWfc_StartAuth(NintendoWfcConnect *wfc) {
    DWC_NASLoginAsync();
    wfc->state = 5;
    return 3;
}

#pragma force_active on

int NintendoWfc_WaitAuth(NintendoWfcConnect *wfc) {
    int value;
    u32 category;
    switch (DWC_NASLoginProcess()) {
    case 3:
        wfc->state = 6;
        break;
    case 0:
    case 4:
    case 5:
        NintendoWfc_HideWaitingIcon(wfc);
        wfc->dwcError = DWC_GetLastErrorEx(&value, &category);
        wfc->dwcErrorCode = value;
        DWC_ClearError();
        DWC_CleanupInet();
        wfc->state = 55;
        switch (category) {
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
        if (value < -20000 && value >= -29999) {
            wfc->state = 55;
        }
        break;
    }
    return 3;
}

int NintendoWfc_InitHttp(NintendoWfcConnect *wfc) {
    u32 profileId;
    u64 friendKey;
    if (Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo) == 0) {
        Save_SysInfo_SetDwcProfileId(wfc->args->sysInfo, (s32)wfc->args->dwcProfileId);
    }
    profileId = Save_SysInfo_GetDwcProfileId(wfc->args->sysInfo);
    friendKey = DWC_CreateFriendKey(wfc->args->dwcUserData);
    NintendoWfcHttp_Init(profileId, friendKey);
    wfc->state = 7;
    return 3;
}

#pragma force_active reset

int NintendoWfc_RequestInfo(NintendoWfcConnect *wfc) {
    NintendoWfcHttp_RequestInfo();
    wfc->state = 8;
    wfc->timeoutCounter = 0;
    return 3;
}

int NintendoWfc_WaitInfo(NintendoWfcConnect *wfc) {
    int result;
    if (NintendoWfcHttp_IsDone()) {
        result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            NintendoWfc_HideWaitingIcon(wfc);
            switch (wfc->args->mode) {
            case 0:
                NintendoWfc_SetState(wfc, 11, 13);
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
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfc_HideWaitingIcon(wfc);
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

int NintendoWfc_RequestSetProfile(NintendoWfcConnect *wfc) {
    Save_WiiMessage_GetServerProfile(wfc->args->saveData, &wfc->profile);
    NintendoWfcHttp_RequestSetProfile(&wfc->profile, wfc->profileResponse);
    wfc->state = 10;
    wfc->timeoutCounter = 0;
    return 3;
}

int NintendoWfc_WaitSetProfile(NintendoWfcConnect *wfc) {
    int result;
    if (NintendoWfcHttp_IsDone()) {
        result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            NintendoWfc_HideWaitingIcon(wfc);
            switch (wfc->profileResponse[0]) {
            case 0:
                switch (wfc->profileResponse[1]) {
                case 0:
                    switch (wfc->args->mode) {
                    case 0:
                        NintendoWfc_SetState(wfc, 11, 13);
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
                    NintendoWfc_HideWaitingIcon(wfc);
                    break;
                }
                break;
            case 1:
            case 2:
                wfc->httpResult = result;
                wfc->state = 53;
                NintendoWfc_HideWaitingIcon(wfc);
                break;
            default:
                NintendoWfc_HideWaitingIcon(wfc);
                CommState_ShowRestartErrorAndHalt();
                break;
            }
            break;
        case 1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -1:
        case 2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -6:
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        default:
            NintendoWfc_HideWaitingIcon(wfc);
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

int NintendoWfc_RequestRoomCount(NintendoWfcConnect *wfc) {
    NintendoWfcHttp_RequestRoomCount(wfc->rank);
    wfc->state = 12;
    wfc->timeoutCounter = 0;
    NintendoWfc_ShowWaitingIcon(wfc);
    return 3;
}

int NintendoWfc_WaitRoomCount(NintendoWfcConnect *wfc) {
    int value;

    if (NintendoWfcHttp_IsDone()) {
        value = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (value) {
        case -1:
            wfc->httpResult = value;
            wfc->state = 53;
            break;
        case -2:
            wfc->httpResult = value;
            wfc->state = 53;
            break;
        case -3:
            wfc->httpResult = value;
            wfc->state = 53;
            break;
        case -6:
            wfc->httpResult = value;
            wfc->state = 53;
            break;
        case -7:
        case -4:
            wfc->httpResult = value;
            wfc->state = 53;
            break;
        case -5:
            wfc->httpResult = value;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        default:
            if (value > 0) {
                wfc->roomCount = value;
                wfc->state = wfc->nextState;
            }
            break;
        }
        NintendoWfc_HideWaitingIcon(wfc);
    } else {
        wfc->timeoutCounter++;
        if (wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

int NintendoWfc_AskChallengeRoom(NintendoWfcConnect *wfc) {
    int roomCount = wfc->roomCount;

    if (roomCount >= 100) {
        BufferIntegerAsString(wfc->msgFormat, 0, roomCount, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
        NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 11, 1, 0xF0F);
    } else {
        BufferIntegerAsString(wfc->msgFormat, 0, roomCount, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 0, 1, 0xF0F);
    }
    NintendoWfc_SetState(wfc, 47, 14);
    return 3;
}

int NintendoWfc_StartChallengeRoomInput(NintendoWfcConnect *wfc) {
    NintendoWfc_StartNumberInput(wfc, 1, wfc->roomCount);
    wfc->state = 15;
    NintendoWfc_NumberInputDummy(wfc, 0, 1);
    return 3;
}

int NintendoWfc_HandleChallengeRoomInput(NintendoWfcConnect *wfc) {
    u32 time[3];
    u32 date[4];
    int value = NintendoWfc_RunNumberInput(wfc);

    if (value != 0) {
        if (value > 0) {
            DWC_GetDateTime(date, time);
            wfc->room = value;
            // BattleRoomDownload_IsRoomMarked takes u32 group/entry; the original call truncated them to u8
            if (BattleRoomDownload_IsRoomMarked((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date)) {
                wfc->state = 20;
            } else {
                wfc->state = 16;
            }
        } else {
            NintendoWfc_NumberInputDummy(wfc, 0, 0);
            wfc->state = 21;
        }
    }
    return 3;
}

int NintendoWfc_PrintReceivingTrainers(NintendoWfcConnect *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 1, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 17);
    MIi_CpuClearFast(0, (u32 *)wfc->downloadBuffer, sizeof(wfc->downloadBuffer));
    return 3;
}

int NintendoWfc_PrintRoomUnavailable(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 2, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 13);
    return 3;
}

int NintendoWfc_RequestTrainersDownload(NintendoWfcConnect *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->ownRank, wfc->room, wfc->downloadBuffer);
    NintendoWfc_ShowWaitingIcon(wfc);
    wfc->state = 18;
    wfc->timeoutCounter = 0;
    return 3;
}

#pragma force_active on

int NintendoWfc_WaitTrainersDownload(NintendoWfcConnect *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0: {
            u32 time[3];
            u32 date[4];
            // BattleRoomDownload_SetTrainers/BattleRoomDownload_MarkRoom take u32; the original calls truncated rank and room to u8
            BattleRoomDownload_SetTrainers((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer, (u8)wfc->ownRank, (u8)wfc->room);
            NintendoWfc_SetState(wfc, 39, 19);
            DWC_GetDateTime(date, time);
            BattleRoomDownload_MarkRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, (u8)wfc->rank, (u8)wfc->room, date);
        } break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -3:
        case -4:
            wfc->httpResult = result;
            wfc->state = 11;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -7:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
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

int NintendoWfc_PrintTrainersReceived(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 3, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 45);
    NintendoWfc_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

int NintendoWfc_PrintSendingRecord(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 7, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 26);
    NintendoWfc_ShowWaitingIcon(wfc);
    return 3;
}

#pragma force_active on

int NintendoWfc_RequestUpload(NintendoWfcConnect *wfc) {
    u8 buf[4];
    int winCount = FrontierData_GetWifiWinCount(wfc->args->frontierData);
    BattleRoomDownload_GetTrainersRoom((BattleRoomDownload *)wfc->args->battleRoomDownload, buf);
    Frontier_FillWifiUploadRecord(wfc->args->saveData, 1, &wfc->uploadRecord);
    NintendoWfcHttp_RequestUpload(buf[0], buf[1], winCount, &wfc->uploadRecord);
    wfc->state = 27;
    wfc->timeoutCounter = 0;
    return 3;
}

int NintendoWfc_WaitUpload(NintendoWfcConnect *wfc) {
    int result;
    if (NintendoWfcHttp_IsDone()) {
        result = NintendoWfcHttp_GetResult();
        wfc->timeoutCounter = 0;
        switch (result) {
        case 0:
            FrontierData_FlagAction(wfc->args->frontierData, 5, 2);
            NintendoWfc_SetState(wfc, 39, 28);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
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

int NintendoWfc_PrintRecordSent(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 8, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 45);
    NintendoWfc_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

int NintendoWfc_AskLeadersRank(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 5, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 30);
    return 3;
}

int NintendoWfc_StartLeadersRankInput(NintendoWfcConnect *wfc) {
    wfc->rank = 1;
    NintendoWfc_NumberInputDummy(wfc, 1, 1);
    NintendoWfc_StartNumberInput(wfc, 1, 10);
    wfc->state = 31;
    return 3;
}

#pragma force_active on

int NintendoWfc_HandleLeadersRankInput(NintendoWfcConnect *wfc) {
    int value = NintendoWfc_RunNumberInput(wfc);
    if (value != 0) {
        if (value > 0) {
            wfc->rank = value;
            NintendoWfc_SetState(wfc, 11, 32);
        } else {
            wfc->state = 23;
        }
    }
    return 3;
}

int NintendoWfc_AskLeadersRoom(NintendoWfcConnect *wfc) {
    if (wfc->roomCount >= 100) {
        BufferIntegerAsString(wfc->msgFormat, 0, wfc->roomCount, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
        NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 10, 1, 0xF0F);
    } else {
        BufferIntegerAsString(wfc->msgFormat, 0, wfc->roomCount, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 6, 1, 0xF0F);
    }
    NintendoWfc_SetState(wfc, 47, 33);
    return 3;
}

#pragma force_active reset

int NintendoWfc_StartLeadersRoomInput(NintendoWfcConnect *wfc) {
    wfc->room = 1;
    NintendoWfc_StartNumberInput(wfc, 1, wfc->roomCount);
    NintendoWfc_NumberInputDummy(wfc, 0, 1);
    wfc->state = 34;
    return 3;
}

#pragma force_active on

int NintendoWfc_HandleLeadersRoomInput(NintendoWfcConnect *wfc) {
    int value = NintendoWfc_RunNumberInput(wfc);
    if (value != 0) {
        if (value > 0) {
            wfc->room = value;
            wfc->state = 35;
        } else {
            wfc->state = 29;
        }
    }
    return 3;
}

int NintendoWfc_PrintReceivingLeaders(NintendoWfcConnect *wfc) {
    BufferIntegerAsString(wfc->msgFormat, 0, wfc->room, 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(wfc->msgFormat, 1, wfc->rank, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    NintendoWfc_PrintFormattedMessage(wfc, wfc->msgDataTower, 1, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 36);
    return 3;
}

#pragma force_active reset

int NintendoWfc_RequestLeadersDownload(NintendoWfcConnect *wfc) {
    NintendoWfcHttp_RequestDownload(wfc->rank, wfc->room, wfc->downloadBuffer);
    wfc->state = 37;
    wfc->timeoutCounter = 0;
    NintendoWfc_ShowWaitingIcon(wfc);
    return 3;
}

#pragma force_active on

int NintendoWfc_WaitLeadersDownload(NintendoWfcConnect *wfc) {
    if (NintendoWfcHttp_IsDone()) {
        int result = NintendoWfcHttp_GetResult();
        *(volatile int *)&wfc->timeoutCounter = 0; // the original accessed this counter as volatile here
        switch (result) {
        case 0:
            // BattleRoomDownload_SetLeaders takes u32; the original call truncated rank and room to u8
            BattleRoomDownload_SetLeaders((BattleRoomDownload *)wfc->args->battleRoomDownload, wfc->downloadBuffer + 0x63C, (u8)wfc->rank, (u8)wfc->room);
            NintendoWfc_SetState(wfc, 39, 38);
            break;
        case -1:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -2:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -3:
            wfc->httpResult = result;
            wfc->state = 11;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -6:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -7:
        case -4:
            wfc->state = 11;
            wfc->httpResult = result;
            NintendoWfc_HideWaitingIcon(wfc);
            break;
        case -5:
            wfc->httpResult = result;
            wfc->state = 53;
            NintendoWfc_HideWaitingIcon(wfc);
            CommState_ShowRestartErrorAndHalt();
            break;
        }
    } else {
        ++*(volatile int *)&wfc->timeoutCounter;
        if (*(volatile int *)&wfc->timeoutCounter == 3600) {
            CommState_ShowRestartErrorAndHalt();
        }
    }
    return 3;
}

int NintendoWfc_PrintLeadersReceived(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 3, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 45);
    NintendoWfc_HideWaitingIcon(wfc);
    wfc->args->incomplete = 0;
    return 3;
}

#pragma force_active reset

int NintendoWfc_StartDisconnect(NintendoWfcConnect *wfc) {
    wfc->state = 41;
    return 3;
}

int NintendoWfc_StartSave(NintendoWfcConnect *wfc) {
    Save_PrepareForAsyncWrite(wfc->args->saveData, 2);
    wfc->state = 40;
    return 3;
}

int NintendoWfc_Save(NintendoWfcConnect *wfc) {
    if (Save_WriteFileAsync(wfc->args->saveData) == 2) {
        wfc->state = wfc->nextState;
        NintendoWfc_HideWaitingIcon(wfc);
    }
    return 3;
}

int NintendoWfc_AskCancelChallenge(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 4, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 48, 22);
    return 3;
}

#pragma force_active on

int NintendoWfc_HandleCancelChallengeChoice(NintendoWfcConnect *wfc) {
    int result = NintendoWfcYesNo_Run(&wfc->yesNoMenu);
    if (result) {
        if (result == 2) {
            wfc->state = 13;
        } else {
            wfc->state = 41;
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

#pragma force_active reset

int NintendoWfc_AskCancelDownload(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 9, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 48, 24);
    return 3;
}

#pragma force_active on

int NintendoWfc_HandleCancelDownloadChoice(NintendoWfcConnect *wfc) {
    int result = NintendoWfcYesNo_Run(&wfc->yesNoMenu);
    if (result) {
        if (result == 2) {
            wfc->state = 29;
        } else {
            wfc->state = 41;
            wfc->args->incomplete = 1;
        }
    }
    return 3;
}

int NintendoWfc_PrintDisconnecting(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataError, 26, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 42);
    return 3;
}

#pragma force_active reset

int NintendoWfc_CloseConnection(NintendoWfcConnect *wfc) {
    DWC_CleanupInet();
    NintendoWfc_SetNextMode(wfc, 0, 0);
    wfc->state = 43;
    return 3;
}

#pragma force_active on

int NintendoWfc_PrintDisconnected(NintendoWfcConnect *wfc) {
    NintendoWfc_PrintMessage(wfc, wfc->msgDataError, 27, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 49, 46);
    wfc->messageTimer = 0;
    return 3;
}

#pragma force_active reset

int NintendoWfc_UnusedState_Idle(void) {
    return 3;
}

int NintendoWfc_StartExit(NintendoWfcConnect *wfc) {
    WirelessStatusIcon_Destroy();
    CommState_FreeWfcSession();
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 8, 1, HEAP_ID_67);
    wfc->state = 0;
    return 4;
}

int NintendoWfc_UnusedState_HandleYesNo(NintendoWfcConnect *wfc) {
    int response = NintendoWfc_HandleYesNoPrompt(wfc->yesNoPrompt);
    if (response != 0) {
        if (response == 2) {
            wfc->state = 0;
        } else {
            wfc->state = 41;
        }
    }
    return 3;
}

int NintendoWfc_ShowHttpError(NintendoWfcConnect *wfc) {
    int msgNo = 0;
    switch (wfc->httpResult) {
    case -6:
    case -2:
        msgNo = 15;
        break;
    case 1:
        msgNo = 13;
        break;
    case -1:
    case 2:
        msgNo = 14;
        break;
    case -7:
    case -4:
        msgNo = 16;
        break;
    case -3:
        msgNo = 17;
        break;
    }
    NintendoWfc_HideWaitingIcon(wfc);
    NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, msgNo, 1, 0xF0F);
    NintendoWfc_SetState(wfc, 47, 54);
    return 3;
}

#pragma force_active on

int NintendoWfc_Disconnect(NintendoWfcConnect *wfc) {
    switch (wfc->subState) {
    case 0:
        NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 18, 1, 0xF0F);
        wfc->subState++;
        break;
    case 1:
        if (TextPrinterCheckActive((u8)wfc->textPrinterId) == 0) {
            CommState_FreeWfcSession();
            DWC_CleanupInet();
            wfc->subState++;
        }
        break;
    case 2:
        NintendoWfc_PrintMessage(wfc, wfc->msgDataTower, 19, 1, 0xF0F);
        wfc->subState++;
        break;
    case 3:
        if (TextPrinterCheckActive((u8)wfc->textPrinterId) == 0) {
            wfc->subState++;
        }
        break;
    default:
        // the original accessed this timer as volatile here
        (*(volatile s16 *)&wfc->subTimer)++;
        if (*(volatile s16 *)&wfc->subTimer > 30) {
            NintendoWfc_SetNextMode(wfc, 0, 0);
            wfc->state = 46;
        }
        break;
    }
    return 3;
}

#pragma force_active reset

int NintendoWfc_WaitMessage(NintendoWfcConnect *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->state = wfc->nextState;
    }
    return 3;
}

int NintendoWfc_WaitMessageAndDelay(NintendoWfcConnect *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        if (wfc->messageTimer > 30) {
            wfc->state = wfc->nextState;
        }
        wfc->messageTimer++;
    }
    return 3;
}

int NintendoWfc_OpenYesNoMenu(NintendoWfcConnect *wfc) {
    if (!TextPrinterCheckActive((u8)wfc->textPrinterId)) {
        wfc->yesNoMenu.cursorSprite = wfc->sprites[NINTENDO_WFC_SPRITE_YES_NO_CURSOR];
        wfc->yesNoMenu.bgConfig = wfc->bgConfig;
        wfc->yesNoMenu.heapID = HEAP_ID_67;
        wfc->yesNoMenu.initialChoice = 0;
        wfc->yesNoMenu.bgPltt = 10;
        wfc->yesNoMenu.bgLayer = GF_BG_LYR_SUB_2;
        wfc->yesNoMenu.textBgLayer = GF_BG_LYR_SUB_0;
        wfc->yesNoMenu.fontPltt = 11;
        wfc->yesNoMenu.baseTile = 1;
        wfc->yesNoMenu.cursorOffsetY = 256;
        NintendoWfcYesNo_Init(&wfc->yesNoMenu);
        wfc->state = wfc->nextState;
    }
    return 3;
}

void NintendoWfc_PrintFormattedMessage(NintendoWfcConnect *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    String *string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(wfc->msgFormat, wfc->msgString, string);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

int NintendoWfc_ShowDwcError(NintendoWfcConnect *wfc) {
    int msgNo = WifiConnection_ClassifyErrorCode(-wfc->dwcErrorCode, wfc->dwcErrorType);
    NintendoWfc_ShowErrorMessage(wfc, msgNo, -wfc->dwcErrorCode);
    wfc->state = 56;
    return 3;
}

int NintendoWfc_WaitErrorDismiss(NintendoWfcConnect *wfc) {
    if ((gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B) || gSystem.touchNew) {
        EraseFrameAndWindow1(&wfc->errorWindow, FALSE);
        wfc->state = 0;
    }
    return 3;
}

void NintendoWfc_PrintMessage(NintendoWfcConnect *wfc, MsgData *msgData, int msgNo, int textSpeed, int unused) {
    ReadMsgDataIntoString(msgData, msgNo, wfc->msgString);
    FillWindowPixelBuffer(&wfc->msgWindow, 15);
    DrawFrameAndWindow2(&wfc->msgWindow, FALSE, 1, 14);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->msgWindow, 1, wfc->msgString, 0, 0, textSpeed, NULL);
}

int NintendoWfc_GetAlignedTextX(const Window *window, String *string, int x, int alignment, int unused, int fontId) {
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

void NintendoWfc_PrintAlignedText(Window *window, String *string, int x, int y, int alignment, u32 color) {
    x = NintendoWfc_GetAlignedTextX(window, string, x, alignment, color, 1);
    AddTextPrinterParameterizedWithColor(window, 1, string, x, y, 0, color, NULL);
}

void NintendoWfc_CreateWirelessIcon(NintendoWfcConnect *wfc) {
    WirelessStatusIcon_Create();
}

void NintendoWfc_NumberInputDummy(NintendoWfcConnect *wfc, int a1, int a2) {
}

#pragma force_active on

void NintendoWfc_ShowWaitingIcon(NintendoWfcConnect *wfc) {
    if (!wfc->waitingIcon) {
        wfc->waitingIcon = WaitingIcon_New(&wfc->msgWindow, 1);
    }
}

#pragma force_active reset

void NintendoWfc_HideWaitingIcon(NintendoWfcConnect *wfc) {
    WaitingIcon *waitingIcon = wfc->waitingIcon;
    if (waitingIcon) {
        WaitingIcon_Delete(waitingIcon);
        wfc->waitingIcon = NULL;
    }
}

void NintendoWfc_PrintErrorMessage(NintendoWfcConnect *wfc, int msgNo) {
    String *string = String_New(256, HEAP_ID_67);

    ReadMsgDataIntoString(wfc->msgDataError, msgNo, string);
    StringExpandPlaceholders(wfc->msgFormat, wfc->errorString, string);
    FillWindowPixelBuffer(&wfc->errorWindow, 15);
    DrawFrameAndWindow1(&wfc->errorWindow, TRUE, 31, 11);
    wfc->textPrinterId = AddTextPrinterParameterized(&wfc->errorWindow, 1, wfc->errorString, 0, 0, 0, NULL);
    String_Delete(string);
}

void NintendoWfc_ShowErrorMessage(NintendoWfcConnect *wfc, int msgNo, int errorCode) {
    if (msgNo == -1) {
        msgNo = 11;
    }
    BufferIntegerAsString(wfc->msgFormat, 0, errorCode, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ClearFrameAndWindow2(&wfc->msgWindow, TRUE);
    NintendoWfc_PrintErrorMessage(wfc, msgNo);
}

#pragma force_active on

int NintendoWfc_StartNumberInput(NintendoWfcConnect *wfc, int a1, int maxValue) {
    wfc->numberInput.unk_0 = a1;
    wfc->numberInput.maxValue = maxValue;
    wfc->numberInput.value = 0;
    wfc->numberInput.cursor = 0;
    if (maxValue != 100) {
        wfc->numberInput.digitPos = 1;
        wfc->twoDigits = 1;
    } else {
        wfc->numberInput.digitPos = 0;
        wfc->twoDigits = 0;
    }
    wfc->numberInput.state = 0;
    wfc->numberInput.digits[0] = -1;
    wfc->numberInput.digits[1] = -1;
    wfc->numberInput.digits[2] = -1;
    wfc->padColumn[0] = 0;
    wfc->padColumn[1] = 1;
    if (wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR]) {
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], 0);
    }
    return 1;
}

#pragma force_active reset

int NintendoWfc_RunNumberInput(NintendoWfcConnect *wfc) {
    if (sNintendoWfcNumberInputStates[wfc->numberInput.state](wfc) == 1) {
        return wfc->numberInput.value;
    }
    return 0;
}

int NintendoWfc_NumberInput_FadeOut(NintendoWfcConnect *wfc) {
    BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 4, 1, HEAP_ID_67);
    wfc->numberInput.state = 1;
    return 0;
}

int NintendoWfc_NumberInput_Setup(NintendoWfcConnect *wfc) {
    if (IsPaletteFadeFinished() == TRUE) {
        NintendoWfc_ShowNumberInputSprites(wfc);
        NintendoWfc_MoveDigitCursor(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], wfc->numberInput.digitPos, wfc->twoDigits);
        NintendoWfc_LoadSubScreenBg(wfc, 1);
        NintendoWfc_AddNumberInputWindows(wfc);
        NintendoWfc_PrintButtonLabel(&wfc->buttonWindows[0], wfc->msgDataTower, 20);
        NintendoWfc_PrintButtonLabel(&wfc->buttonWindows[1], wfc->msgDataTower, 21);
        NintendoWfc_PrintButtonLabel(&wfc->buttonWindows[2], wfc->msgDataTower, 22);
        ToggleBgLayer(GF_BG_LYR_SUB_0, TRUE);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 4, 1, HEAP_ID_67);
        wfc->numberInput.state = 2;
    }
    return 0;
}

void NintendoWfc_ShowNumberInputSprites(NintendoWfcConnect *wfc) {
    int i;

    if (wfc->numberInput.maxValue == 100) {
        Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0], TRUE);
    }
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + 1], TRUE);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + 2], TRUE);
    for (i = 0; i < 3; i++) {
        NintendoWfc_SetDigitSpritePosition(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], i, wfc->twoDigits);
    }
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], TRUE);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], TRUE);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0], TRUE);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + 1], TRUE);
    Sprite_SetDrawFlag(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + 2], TRUE);
}

int NintendoWfc_NumberInput_WaitFadeIn(NintendoWfcConnect *wfc) {
    if (IsPaletteFadeFinished() == TRUE) {
        wfc->numberInput.state = 3;
    }
    return 0;
}

int NintendoWfc_GetTouchedPadKey(NintendoWfcConnect *wfc) {
    if (wfc->numberInput.maxValue == 100) {
        return TouchscreenHitbox_FindRectAtTouchNew(gNintendoWfcPadHitboxes_Max100);
    }
    return TouchscreenHitbox_FindRectAtTouchNew(gNintendoWfcPadHitboxes);
}

u8 NintendoWfc_GetPadNeighbor(u32 key, u32 direction, u16 *padColumn) {
    u8 next = gNintendoWfcPadNeighbors[key][direction];

    if (next == 13) {
        next = ov72_0223B464[padColumn[0]];
    } else if (next == 14) {
        next = ov72_0223B466[padColumn[0]];
    } else if (next == 15) {
        next = ov72_0223B468[padColumn[1]];
    } else if (next == 16) {
        next = ov72_0223B46A[padColumn[1]];
    } else if (next == 10) {
        if (key == 5 || key == 0) {
            padColumn[0] = 0;
        } else if (key == 1 || key == 6) {
            padColumn[0] = 1;
        }
    } else if (next == 12) {
        if (key == 3 || key == 8) {
            padColumn[1] = 0;
        } else if (key == 4 || key == 9) {
            padColumn[1] = 1;
        }
    }
    return next;
}

int NintendoWfc_GetEnteredNumber(NintendoWfcNumberInput *input, int newDigit) {
    int digits[3];
    int selected[3];
    int placeValue = 100;
    int digitCount = 3;
    int outputCount;
    int started;
    int i;
    int sum;

    for (i = 0; i < 3; i++) {
        digits[i] = input->digits[i];
    }
    if (newDigit != -1) {
        digits[input->digitPos] = newDigit;
    }
    outputCount = 0;
    started = FALSE;
    for (i = 0; i < 3; i++) {
        if ((input->digits[i] == 0 && started == FALSE) || digits[i] < 0) {
            placeValue /= 10;
            digitCount--;
        } else {
            started = TRUE;
            selected[outputCount++] = digits[i];
        }
    }
    sum = 0;
    for (i = 0; i < digitCount; i++) {
        sum += selected[i] * placeValue;
        placeValue /= 10;
    }
    if (sum > input->maxValue) {
        return -1;
    }
    return sum;
}

BOOL NintendoWfc_AreDigitsComplete(NintendoWfcNumberInput *input) {
    if (input->maxValue != 100) {
        if (input->digits[1] >= 0 && input->digits[2] >= 0) {
            return TRUE;
        }
    } else {
        if (input->digits[0] >= 0 && input->digits[1] >= 0 && input->digits[2] >= 0) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 NintendoWfc_GetCursorAfterDigit(NintendoWfcNumberInput *input) {
    if (NintendoWfc_AreDigitsComplete(input)) {
        return 11;
    }
    return input->cursor;
}

#pragma force_active on

void NintendoWfc_PressPadKey(NintendoWfcConnect *wfc, int key) {
    switch (key) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        if (NintendoWfc_GetEnteredNumber(&wfc->numberInput, key - 3) != -1) {
            wfc->numberInput.digits[wfc->numberInput.digitPos] = key - 3;
            if (wfc->numberInput.digitPos < 2) {
                wfc->numberInput.digitPos++;
            }
            NintendoWfc_MoveDigitCursor(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], wfc->numberInput.digitPos, wfc->twoDigits);
            wfc->numberInput.cursor = key - 3;
            wfc->numberInput.cursor = NintendoWfc_GetCursorAfterDigit(&wfc->numberInput);
            NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
            PlaySE(SEQ_SE_DP_SELECT);
            NintendoWfc_StartButtonFlash(wfc, gNintendoWfcPadKeyFlashes[(key - 3) * 2], ov72_0223B479[(key - 3) * 2], 3);
        } else {
            PlaySE(SEQ_SE_DP_CUSTOM06);
        }
        break;
    case 13:
        wfc->numberInput.digits[wfc->numberInput.digitPos] = -1;
        wfc->numberInput.digitPos--;
        if (wfc->numberInput.maxValue == 100) {
            if (wfc->numberInput.digitPos < 0) {
                wfc->numberInput.digitPos = 0;
            }
        } else if (wfc->numberInput.digitPos < 1) {
            wfc->numberInput.digitPos = 1;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        NintendoWfc_MoveDigitCursor(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_CURSOR], wfc->numberInput.digitPos, wfc->twoDigits);
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0], 5);
        break;
    case 14:
        wfc->numberInput.cursor = key - 3;
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        wfc->numberInput.value = NintendoWfc_GetEnteredNumber(&wfc->numberInput, -1);
        if (wfc->numberInput.value <= 0) {
            wfc->numberInput.value = 1;
        }
        wfc->numberInput.state = 7;
        PlaySE(SEQ_SE_DP_SELECT);
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + 1], 5);
        break;
    case 15:
        wfc->numberInput.cursor = key - 3;
        wfc->numberInput.value = -1;
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        wfc->numberInput.state = 7;
        PlaySE(SEQ_SE_DP_SELECT);
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_BUTTON_0 + 2], 5);
        break;
    }
}

#pragma force_active reset

BOOL NintendoWfc_HandlePadButtons(NintendoWfcConnect *wfc) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        wfc->numberInput.cursor = NintendoWfc_GetPadNeighbor(wfc->numberInput.cursor, 0, wfc->padColumn);
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        wfc->numberInput.cursor = NintendoWfc_GetPadNeighbor(wfc->numberInput.cursor, 1, wfc->padColumn);
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        wfc->numberInput.cursor = NintendoWfc_GetPadNeighbor(wfc->numberInput.cursor, 2, wfc->padColumn);
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        wfc->numberInput.cursor = NintendoWfc_GetPadNeighbor(wfc->numberInput.cursor, 3, wfc->padColumn);
        NintendoWfc_MovePadCursor(wfc->sprites[NINTENDO_WFC_SPRITE_PAD_CURSOR], wfc->numberInput.cursor);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        NintendoWfc_PressPadKey(wfc, wfc->numberInput.cursor + 3);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        NintendoWfc_PressPadKey(wfc, 13);
    }
    return FALSE;
}

BOOL NintendoWfc_NumberInput_Main(NintendoWfcConnect *wfc) {
    int key = NintendoWfc_GetTouchedPadKey(wfc);

    if (key == -1) {
        NintendoWfc_HandlePadButtons(wfc);
    } else {
        NintendoWfc_PressPadKey(wfc, key);
    }
    NintendoWfc_UpdateDigitSprites(wfc);
    return FALSE;
}

void NintendoWfc_UpdateDigitSprites(NintendoWfcConnect *wfc) {
    int i;

    for (i = 0; i < 3; i++) {
        s8 digit = wfc->numberInput.digits[i];
        if (digit == -1) {
            Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], 0);
        } else if (digit >= 0 && digit <= 9) {
            Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], digit + 1);
        }
    }
}

int NintendoWfc_NumberInput_UnusedState4(void) {
    return 0;
}

int NintendoWfc_NumberInput_UnusedState5(void) {
    return 0;
}

int NintendoWfc_NumberInput_UnusedState6(void) {
    return 0;
}

int NintendoWfc_NumberInput_StartFadeOut(NintendoWfcConnect *wfc) {
    BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 4, 1, HEAP_ID_67);
    wfc->numberInput.state = 8;
    return 0;
}

#pragma force_active on

int NintendoWfc_NumberInput_Teardown(NintendoWfcConnect *wfc) {
    if (IsPaletteFadeFinished() == TRUE) {
        NintendoWfc_HideNumberInput(wfc);
        NintendoWfc_LoadSubScreenBg(wfc, 0);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 4, 1, HEAP_ID_67);
        wfc->numberInput.state = 9;
    }
    return 0;
}

#pragma force_active reset

int NintendoWfc_NumberInput_WaitButtonFlash(NintendoWfcConnect *wfc) {
    if (!NintendoWfc_UpdateButtonFlash(wfc)) {
        wfc->numberInput.state = wfc->numberInput.nextState;
    }
    return 0;
}

int NintendoWfc_NumberInput_WaitFadeOut(NintendoWfcConnect *wfc) {
    if (IsPaletteFadeFinished() == TRUE) {
        wfc->numberInput.state = 11;
    }
    return 0;
}

int NintendoWfc_NumberInput_Finish(void) {
    return 1;
}

#pragma force_active on

void NintendoWfc_AddNumberInputWindows(NintendoWfcConnect *wfc) {
    int i;
    int baseTile;
    const int *x = gNintendoWfcNumberInputWindowX;
    Window *window;
    i = 0;
    baseTile = 31;
    window = wfc->buttonWindows;
    for (; i < 3; baseTile += 64, x++, window++, i++) {
        AddWindowParameterized(wfc->bgConfig, window, GF_BG_LYR_SUB_0, (u8)*x, 21, 8, 2, 13, (u16)baseTile);
    }
    AddWindowParameterized(wfc->bgConfig, &wfc->numberInputWindow, GF_BG_LYR_SUB_0, 2, 19, 27, 4, 13, 79);
}

#pragma force_active reset

void NintendoWfc_PrintButtonLabel(Window *window, MsgData *msgData, int msgNo) {
    String *string = NewString_ReadMsgData(msgData, msgNo);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 4, string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    CopyWindowToVram(window);
    String_Delete(string);
}

void NintendoWfc_RemoveNumberInputWindows(NintendoWfcConnect *wfc) {
    int i;
    Window *window = wfc->buttonWindows;
    for (i = 0; i < 3; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
    RemoveWindow(&wfc->numberInputWindow);
}

#pragma force_active on

void NintendoWfc_HideNumberInput(NintendoWfcConnect *wfc) {
    int i;
    NintendoWfc_RemoveNumberInputWindows(wfc);
    Sprite_SetDrawFlag(wfc->sprites[0], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[1], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[2], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[3], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[4], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[5], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[6], FALSE);
    Sprite_SetDrawFlag(wfc->sprites[7], FALSE);
    for (i = 0; i < 3; i++) {
        Sprite_SetAnimCtrlSeq(wfc->sprites[NINTENDO_WFC_SPRITE_DIGIT_0 + i], 0);
    }
}

#pragma force_active reset

void NintendoWfc_SetButtonFlashPltt(NintendoWfcConnect *wfc, u8 pltt) {
    NintendoWfcButtonFlash *flash = &wfc->buttonFlash;
    if (flash->isBgRect == 0) {
        Sprite_SetAnimCtrlSeq(wfc->sprites[flash->target], pltt);
    } else {
        BgTilemapRectChangePalette(wfc->bgConfig, flash->target, flash->x, flash->y, flash->width, flash->height, pltt);
        ScheduleBgTilemapBufferTransfer(wfc->bgConfig, flash->target);
    }
}

BOOL NintendoWfc_UpdateButtonFlash(NintendoWfcConnect *wfc) {
    NintendoWfcButtonFlash *flash = &wfc->buttonFlash;
    switch (flash->state) {
    case 0:
        NintendoWfc_SetButtonFlashPltt(wfc, (u8)flash->pressedPltt);
        flash->state++;
        break;
    case 1:
        flash->timer++;
        if (flash->timer == 4) {
            NintendoWfc_SetButtonFlashPltt(wfc, (u8)flash->normalPltt);
            flash->timer = 0;
            flash->state++;
        }
        break;
    case 2:
        flash->timer++;
        if (flash->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

#pragma force_active on

void NintendoWfc_StartButtonFlash(NintendoWfcConnect *wfc, int x, int y, int nextState) {
    wfc->buttonFlash.isBgRect = TRUE;
    wfc->buttonFlash.timer = 0;
    wfc->buttonFlash.state = 0;
    wfc->buttonFlash.target = GF_BG_LYR_SUB_1;
    wfc->buttonFlash.pressedPltt = 1;
    wfc->buttonFlash.normalPltt = 0;
    wfc->buttonFlash.x = x;
    wfc->buttonFlash.y = y;
    wfc->buttonFlash.width = 6;
    wfc->buttonFlash.height = 6;
    wfc->numberInput.nextState = nextState;
    wfc->numberInput.state = 10;
}

#pragma force_active reset

void NintendoWfcYesNo_Init(NintendoWfcYesNoMenu *menu) {
    menu->state = 0;
    menu->choice = menu->initialChoice;
    NintendoWfcYesNo_LoadGraphics(menu);
    NintendoWfcYesNo_AddWindows(menu);
    NintendoWfcYesNo_ShowCursor(menu);
}

int NintendoWfcYesNo_Run(NintendoWfcYesNoMenu *menu) {
    int result = sNintendoWfcYesNoStates[menu->state](menu);
    if (result == 1) {
        return menu->choice + 1;
    }
    return 0;
}

int NintendoWfcYesNo_Start(NintendoWfcYesNoMenu *menu) {
    menu->state = 1;
    return 0;
}

#pragma force_active on

int NintendoWfcYesNo_HandleInput(NintendoWfcYesNoMenu *menu) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(gNintendoWfcYesNoHitboxes);
    if (hit != -1) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (hit == 0) {
            menu->choice = 0;
            NintendoWfcYesNo_MoveCursor(menu, menu->choice);
            Sprite_SetAnimCtrlSeq(menu->cursorSprite, 3);
            menu->state = 3;
        } else if (hit == 1) {
            menu->choice = 1;
            NintendoWfcYesNo_MoveCursor(menu, menu->choice);
            Sprite_SetAnimCtrlSeq(menu->cursorSprite, 3);
            menu->state = 3;
        }
    } else {
        if (gSystem.newKeys & PAD_KEY_UP) {
            menu->choice = 0;
            NintendoWfcYesNo_MoveCursor(menu, menu->choice);
            PlaySE(SEQ_SE_DP_SELECT);
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            menu->choice = 1;
            NintendoWfcYesNo_MoveCursor(menu, menu->choice);
            PlaySE(SEQ_SE_DP_SELECT);
        } else if (gSystem.newKeys & PAD_BUTTON_A) {
            Sprite_SetAnimCtrlSeq(menu->cursorSprite, 3);
            menu->state = 3;
            PlaySE(SEQ_SE_DP_SELECT);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            menu->choice = 1;
            NintendoWfcYesNo_MoveCursor(menu, menu->choice);
            menu->state = 3;
            Sprite_SetAnimCtrlSeq(menu->cursorSprite, 3);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return 0;
}

#pragma force_active reset

int NintendoWfcYesNo_UnusedState2(void) {
    return 0;
}

BOOL NintendoWfcYesNo_WaitClose(NintendoWfcYesNoMenu *menu) {
    if (Sprite_IsAnimated(menu->cursorSprite)) {
        return FALSE;
    }
    ToggleBgLayer(menu->bgLayer, FALSE);
    ToggleBgLayer(menu->textBgLayer, FALSE);
    NintendoWfcYesNo_RemoveWindows(menu);
    Sprite_SetDrawFlag(menu->cursorSprite, FALSE);
    return TRUE;
}

#pragma force_active on

void NintendoWfcYesNo_LoadGraphics(NintendoWfcYesNoMenu *menu) {
    NARC *narc = NARC_New(NARC_a_2_3_7, menu->heapID);
    void *buffer;
    NNSG2dScreenData *screen;
    int i;
    u16 *src;
    u16 *dst;
    if (menu->bgLayer <= GF_BG_LYR_MAIN_3) {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(menu->bgPltt * 32), 32, menu->heapID);
        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(menu->fontPltt * 32), menu->heapID);
    } else {
        GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(menu->bgPltt * 32), 32, menu->heapID);
        LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(menu->fontPltt * 32), menu->heapID);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, menu->bgConfig, (GFBgLayer)menu->bgLayer, 0, 0, FALSE, menu->heapID);
    buffer = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 10, FALSE, &screen, menu->heapID);
    dst = GetBgTilemapBuffer(menu->bgConfig, menu->bgLayer);
    i = 0;
    src = (u16 *)screen->rawData;
    do {
        int tile = *src;
        int pltt = menu->bgPltt;
        i++;
        tile &= 0xFFF;
        *dst = (pltt << 12) | tile;
        src++;
        dst++;
    } while (i < 32 * 24);
    BgCommitTilemapBufferToVram(menu->bgConfig, menu->bgLayer);
    Heap_Free(buffer);
    NARC_Delete(narc);
    ToggleBgLayer(menu->bgLayer, TRUE);
    ToggleBgLayer(menu->textBgLayer, TRUE);
}

#pragma force_active reset

void NintendoWfcYesNo_AddWindows(NintendoWfcYesNoMenu *menu) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0191_bin, menu->heapID);
    String *yes;
    String *no;

    AddWindowParameterized(menu->bgConfig, &menu->yesWindow, menu->textBgLayer, 12, 8, 8, 2, menu->fontPltt, menu->baseTile);
    FillWindowPixelBuffer(&menu->yesWindow, 0);
    AddWindowParameterized(menu->bgConfig, &menu->noWindow, menu->textBgLayer, 12, 14, 8, 2, menu->fontPltt, (u16)(menu->baseTile + 16));
    FillWindowPixelBuffer(&menu->noWindow, 0);
    yes = NewString_ReadMsgData(msgData, 46);
    no = NewString_ReadMsgData(msgData, 47);
    AddTextPrinterParameterizedWithColor(&menu->yesWindow, 4, yes, 0, 0, 0, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    AddTextPrinterParameterizedWithColor(&menu->noWindow, 4, no, 0, 0, 0, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    CopyWindowToVram(&menu->yesWindow);
    CopyWindowToVram(&menu->noWindow);
    String_Delete(no);
    String_Delete(yes);
    DestroyMsgData(msgData);
}

void NintendoWfcYesNo_RemoveWindows(NintendoWfcYesNoMenu *menu) {
    ClearWindowTilemapAndCopyToVram(&menu->noWindow);
    ClearWindowTilemapAndCopyToVram(&menu->yesWindow);
    RemoveWindow(&menu->noWindow);
    RemoveWindow(&menu->yesWindow);
}

void NintendoWfcYesNo_ShowCursor(NintendoWfcYesNoMenu *menu) {
    Sprite_SetDrawFlag(menu->cursorSprite, TRUE);
    NintendoWfcYesNo_MoveCursor(menu, menu->initialChoice);
}
