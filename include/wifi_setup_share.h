#ifndef POKEHEARTGOLD_WIFI_SETUP_SHARE_H
#define POKEHEARTGOLD_WIFI_SETUP_SHARE_H

#include "global.h"

#include "wifi_setup_app.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_ui.h"

// Local wireless session of the access-point setup (ARM code in overlay 13, src/wifi_setup_share.c):
//  - WifiSetupWh_*: a small handler around the WM parent functions (initialise, parent parameters,
//    WEP key, start parent, MP, key sharing / data sharing, channel measurement, end, reset). Its log
//    strings call it "DWCi_MOV_WH"; the state names are in the table ov13_02247120.
//  - WifiSetupShare_*: the session that runs on it: DS Download Play of a program to one child
//    (WifiSetupMbParent_*), then data sharing with that child, sending the connection settings
//    (WifiSetupConnections_Get) in 64-byte blocks.
//  - WifiSetupArchive_Load (wifi_setup_ui.h): mounts the "rom:/dwc/utility.bin" archive.

// gWifiSetupWh.work->state (names from the log table)
#define WIFI_SETUP_WH_STATE_STOP           0
#define WIFI_SETUP_WH_STATE_IDLE           1
#define WIFI_SETUP_WH_STATE_SCANNING       2
#define WIFI_SETUP_WH_STATE_BUSY           3
#define WIFI_SETUP_WH_STATE_CONNECTED      4
#define WIFI_SETUP_WH_STATE_DATASHARING    5
#define WIFI_SETUP_WH_STATE_KEYSHARING     6
#define WIFI_SETUP_WH_STATE_MEASURECHANNEL 7
#define WIFI_SETUP_WH_STATE_CONNECT_FAIL   8
#define WIFI_SETUP_WH_STATE_ERROR          9
#define WIFI_SETUP_WH_STATE_FATAL          10 // WM could not be reset or ended

// connect modes (WifiSetupWh_StartConnection, gWifiSetupWh.work->connectMode)
#define WIFI_SETUP_WH_MODE_MP_PARENT 0
#define WIFI_SETUP_WH_MODE_MP_CHILD  1
#define WIFI_SETUP_WH_MODE_KS_PARENT 2 // key sharing
#define WIFI_SETUP_WH_MODE_KS_CHILD  3
#define WIFI_SETUP_WH_MODE_DS_PARENT 4 // data sharing
#define WIFI_SETUP_WH_MODE_DS_CHILD  5

// packet kinds of the data sharing (WifiSetupSharePacket.kind)
#define WIFI_SETUP_SHARE_PACKET_HELLO 188 // parent: language code, waiting for the child
#define WIFI_SETUP_SHARE_PACKET_READY 189 // child: ready for the settings blocks

typedef void (*WifiSetupWhLogFunc)(int level, char *format, ...);

// Head of the argument of every WM callback.
typedef struct WifiSetupWhCallbackArg {
    u16 apiId;
    u16 result; // 0 success
} WifiSetupWhCallbackArg;

typedef struct WifiSetupWhMpArg {
    u16 apiId;
    u16 result;
    u16 state;
} WifiSetupWhMpArg;

typedef struct WifiSetupWhParentArg {
    u16 apiId;
    u16 result;
    u8 unk_04[4];
    u16 state;
    u8 macAddr[6]; // 0x0A
    u16 aid;       // 0x10
} WifiSetupWhParentArg;

typedef struct WifiSetupWhMeasureArg {
    u16 apiId;
    u16 result;
    u8 unk_04[4];
    u16 channel;   // 0x08
    u16 busyRatio; // 0x0A
} WifiSetupWhMeasureArg;

typedef void (*WifiSetupWhCallback)(void *arg);
typedef BOOL (*WifiSetupWhJudgeFunc)(WifiSetupWhParentArg *arg);

// Parent parameters handed to WM_SetParentParameter.
typedef struct WifiSetupWhParentParam {
    void *userData;     // 0x00
    u16 userDataLength; // 0x04
    u16 unk_06;
    u32 ggid;           // 0x08
    u16 tgid;           // 0x0C
    u16 entry;          // 0x0E, accept new children
    u16 maxEntries;     // 0x10
    u16 multiBoot;      // 0x12
    u16 keySharing;     // 0x14
    u16 continuousSend; // 0x16
    u16 beaconPeriod;   // 0x18
    u16 unk_1A[12];     // 0x1A
    u16 channel;        // 0x32
    u16 parentMaxSize;  // 0x34
    u16 childMaxSize;   // 0x36
    u16 unk_38[4];      // 0x38
} WifiSetupWhParentParam;

typedef struct WifiSetupWhWork {
    WifiSetupWhParentParam parentParam;                      // 0x0000
    u32 state;                                               // 0x0040, WIFI_SETUP_WH_STATE_*
    u32 connectMode;                                         // 0x0044, WIFI_SETUP_WH_MODE_*
    u32 unk_48;                                              // 0x0048
    WifiSetupWhJudgeFunc judgeChild;                         // 0x004C, may refuse a new child
    u16 unk_50;                                              // 0x0050
    u16 connectedMask;                                       // 0x0052, bit per AID, bit 0 the parent
    u32 errorCode;                                           // 0x0054
    u32 randomSeed;                                          // 0x0058
    u16 channel;                                             // 0x005C, decided by the measurement
    u16 minBusyRatio;                                        // 0x005E
    u16 quietChannels;                                       // 0x0060, channels with the lowest busy ratio
    u8 unk_62[0x1E];                                         // 0x0062
    u8 wmWork[0xF00];                                        // 0x0080, WM_Initialize
    u8 sendBuffer[0xE0];                                     // 0x0F80
    u8 recvBuffer[0x240];                                    // 0x1060
    u32 sendBufferSize;                                      // 0x12A0
    u32 recvBufferSize;                                      // 0x12A4
    u8 unk_12A8[0x100];                                      // 0x12A8
    u32 unk_13A8;                                            // 0x13A8
    int (*getWepKey)(u8 *key, struct WifiSetupWhWork *work); // 0x13AC, returns the WEP mode; NULL: no WEP
    u32 unk_13B0;                                            // 0x13B0
    u8 unk_13B4[0xC];                                        // 0x13B4
    u8 wepKey[0x20];                                         // 0x13C0
    u8 dataSharingInfo[0x820];                               // 0x13E0
    u8 dataSet[0x200];                                       // 0x1C00
    u8 keySharing[4];                                        // 0x1E00, size unknown
} WifiSetupWhWork;

typedef struct WifiSetupWh {
    WifiSetupWhLogFunc log; // NULL: no logging
    WifiSetupWhWork *work;
} WifiSetupWh;

typedef struct WifiSetupSharePacket {
    u16 kind; // WIFI_SETUP_SHARE_PACKET_* or a settings block
    u16 index;
    u8 data[0x40];
} WifiSetupSharePacket;

// Session work: the start of WifiSetupSession (wifi_setup_app.h).
typedef struct WifiSetupShare {
    union {
        WifiSetupSharePacket packet;
        u8 raw[0x100];
    } send; // 0x000

    union {
        WifiSetupSharePacket packets[2]; // by AID
        u8 raw[0x100];
    } received; // 0x100

    u16 blockIndex;                   // 0x200, settings block sent next (0-35, mod 16)
    u16 unk_202;                      // 0x202
    u32 errorCount;                   // 0x204, step failures without the child's answer
    u32 receivedFlags[16];            // 0x208, by AID
    u8 unk_248[0x400];                // 0x248
    u16 tgid;                         // 0x648
    u16 channel;                      // 0x64A
    u8 backup[0x404];                 // 0x64C, DWCi_BACKUPlRead
    u8 firstBlock[0x40];              // 0xA50
    u8 state;                         // 0xA90
    u8 prevState;                     // 0xA91
    u8 languageCode;                  // 0xA92, sent to the child
    u8 sendingData;                   // 0xA93, the child answered, settings blocks are sent
    u32 frame;                        // 0xA94
    u32 counter;                      // 0xA98
    u32 timer;                        // 0xA9C
    void *childUsers[1];              // 0xAA0, by AID - 1
    u8 *data;                         // 0xAA4, the connection settings (WifiSetupConnections_Get)
    u32 unk_AA8;                      // 0xAA8
    u8 request;                       // 0xAAC
    WifiSetupSharePacket *sendPacket; // 0xAB0, = &send.packet
    WifiSetupSessionParams params;    // 0xAB4
    u8 unk_AD0[0x30];                 // 0xAD0
    u8 mbParent[0x1E280 - 0xB00];     // 0xB00, WifiSetupMbParent, WifiSetupWhWork at 0x1B160
} WifiSetupShare;

extern WifiSetupWh gWifiSetupWh;
extern WifiSetupShare *gWifiSetupShare;

int WifiSetupMbParent_GetChildState(int aid);
WifiSetupMbParentChild *WifiSetupMbParent_GetChild(int aid);
u16 WifiSetupMbParent_FindAid(const u8 *macAddr);

void WifiSetupWh_Init(u8 *work);
void WifiSetupWh_SetState(u32 state);
void WifiSetupWh_SetError(u32 code);
int WifiSetupWh_SetParentParameter(void);
void WifiSetupWh_SetParentParameterCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_SetWepKey(void);
void WifiSetupWh_SetWepKeyCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_StartParent(void);
void WifiSetupWh_StartParentCallback(WifiSetupWhParentArg *arg);
int WifiSetupWh_StartMp(void);
void WifiSetupWh_StartMpCallback(WifiSetupWhMpArg *arg);
int WifiSetupWh_StartKeySharing(void);
int WifiSetupWh_EndKeySharing(void);
int WifiSetupWh_EndMp(void);
void WifiSetupWh_EndMpCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_EndParent(void);
void WifiSetupWh_EndParentCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_StopKeySharing(void);
int WifiSetupWh_EndMpForDisconnect(void);
void WifiSetupWh_EndMpForDisconnectCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_Disconnect(void);
void WifiSetupWh_DisconnectCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_Reset(void);
void WifiSetupWh_ResetCallback(WifiSetupWhCallbackArg *arg);
void WifiSetupWh_EndCallback(WifiSetupWhCallbackArg *arg);
void WifiSetupWh_SetGgid(u32 ggid);
u16 WifiSetupWh_GetConnectedMask(void);
u32 WifiSetupWh_GetState(void);
int WifiSetupWh_StartMeasureChannel(void);
u16 WifiSetupWh_MeasureNextChannel(u16 channel);
void WifiSetupWh_MeasureChannelCallback(WifiSetupWhMeasureArg *arg);
int WifiSetupWh_MeasureChannel(WifiSetupWhCallback callback, u16 channel);
u16 WifiSetupWh_DecideChannel(void);
s16 WifiSetupWh_PickChannel(u16 mask);
int WifiSetupWh_Start(void);
void WifiSetupWh_IndicationCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_InitializeWm(void);
void WifiSetupWh_InitializeCallback(WifiSetupWhCallbackArg *arg);
int WifiSetupWh_StartConnection(u32 mode, u16 tgid, u16 channel);
void WifiSetupWh_SetJudgeCallback(WifiSetupWhJudgeFunc judge);
void *WifiSetupWh_GetSharedData(u16 aid);
int WifiSetupWh_StepDataSharing(void *data);
void WifiSetupWh_TryReset(void);
void WifiSetupWh_Finalize(void);
int WifiSetupWh_End(void);

void WifiSetupShare_Init(WifiSetupShare *share, const WifiSetupSessionParams *params);
int WifiSetupShare_Cancel(void);
void WifiSetupShare_Reset(void);
int WifiSetupShare_Start(void);
int WifiSetupShare_StartDownload(void);
void WifiSetupShare_Update(void);
void WifiSetupShare_GetState(u8 *state, u8 *changed);
void *WifiSetupShare_GetChildUser(void);
void WifiSetupShare_UpdateChannelScan(void);
void WifiSetupShare_SetupMbParent(void);
void WifiSetupShare_UpdateDownload(void);
void WifiSetupShare_StartSharing(void);
void WifiSetupShare_UpdateSharing(void);
void WifiSetupShare_SetDone(void);
int WifiSetupShare_EndWireless(void);
BOOL WifiSetupShare_JudgeChild(WifiSetupWhParentArg *arg);
void WifiSetupShare_ClearPackets(void);
void WifiSetupShare_SendPacket(u16 kind, u16 index, void *data);
void WifiSetupShare_UpdateChild(void);
u16 WifiSetupShare_GetChildKind(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SHARE_H
