#ifndef POKEHEARTGOLD_WIFI_SETUP_SHARE_H
#define POKEHEARTGOLD_WIFI_SETUP_SHARE_H

#include <nitro/wm/wm_internal.h>

#include "global.h"

#include "wifi_setup/wifi_setup_app.h"
#include "wifi_setup/wifi_setup_local_wireless.h"
#include "wifi_setup/wifi_setup_ui.h"

// Local wireless session of the access-point setup: DS Download Play of a program to one child, then data sharing
// with that child, sending the connection settings in 64-byte blocks.

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

// download state of one child (WifiSetupMbParent_GetChildState)
#define WIFI_SETUP_MB_CHILD_NONE      0
#define WIFI_SETUP_MB_CHILD_CONNECTED 1
#define WIFI_SETUP_MB_CHILD_REQUESTED 2 // asked for the download
#define WIFI_SETUP_MB_CHILD_WAITING   3
#define WIFI_SETUP_MB_CHILD_SENDING   4
#define WIFI_SETUP_MB_CHILD_SENT      5
#define WIFI_SETUP_MB_CHILD_BOOTABLE  6

// requests handled in WIFI_SETUP_SHARE_STATE_IDLE (gWifiSetupShare->request)
#define WIFI_SETUP_SHARE_REQUEST_NONE  0
#define WIFI_SETUP_SHARE_REQUEST_START 1
#define WIFI_SETUP_SHARE_REQUEST_END   2

// session states (gWifiSetupShare->state, WifiSetupShare_Update)
#define WIFI_SETUP_SHARE_STATE_NONE                0
#define WIFI_SETUP_SHARE_STATE_IDLE                1
#define WIFI_SETUP_SHARE_STATE_MEASURE_CHANNEL     2
#define WIFI_SETUP_SHARE_STATE_SETUP_DOWNLOAD      3
#define WIFI_SETUP_SHARE_STATE_WAIT_ENTRY          4
#define WIFI_SETUP_SHARE_STATE_ENTRY_REQUESTED     5 // a child asked for the download
#define WIFI_SETUP_SHARE_STATE_DOWNLOADING         6
#define WIFI_SETUP_SHARE_STATE_START_SHARING       7 // the child booted the program
#define WIFI_SETUP_SHARE_STATE_SHARING             8
#define WIFI_SETUP_SHARE_STATE_CHILD_READY         9
#define WIFI_SETUP_SHARE_STATE_SENDING_SETTINGS    10
#define WIFI_SETUP_SHARE_STATE_SETTINGS_RECEIVED   11
#define WIFI_SETUP_SHARE_STATE_DONE                12
#define WIFI_SETUP_SHARE_STATE_ENTRY_CANCELLED     13 // the child left before the download
#define WIFI_SETUP_SHARE_STATE_CANCEL_DOWNLOAD      16
#define WIFI_SETUP_SHARE_STATE_CANCEL_DOWNLOAD_WAIT 17
#define WIFI_SETUP_SHARE_STATE_DOWNLOAD_ABORTED    18
#define WIFI_SETUP_SHARE_STATE_STOP_WAIT           19
#define WIFI_SETUP_SHARE_STATE_STOPPED             20
#define WIFI_SETUP_SHARE_STATE_CHILD_ABORT         21
#define WIFI_SETUP_SHARE_STATE_ABORT_WAIT          22
#define WIFI_SETUP_SHARE_STATE_ABORTED             23
#define WIFI_SETUP_SHARE_STATE_CHILD_FAILED        24
#define WIFI_SETUP_SHARE_STATE_FAIL_WAIT           25
#define WIFI_SETUP_SHARE_STATE_FAILED              26
#define WIFI_SETUP_SHARE_STATE_SHARING_ERROR       27
#define WIFI_SETUP_SHARE_STATE_ERROR_WAIT          28
#define WIFI_SETUP_SHARE_STATE_ERROR               29
#define WIFI_SETUP_SHARE_STATE_FINALIZE            30
#define WIFI_SETUP_SHARE_STATE_FATAL               31
#define WIFI_SETUP_SHARE_STATE_CANCEL_SHARING      32
#define WIFI_SETUP_SHARE_STATE_CANCEL_SHARING_WAIT 33
#define WIFI_SETUP_SHARE_STATE_END                 34 // the session can be freed

typedef void (*WifiSetupWhLogFunc)(int level, char *format, ...);

typedef BOOL (*WifiSetupWhJudgeFunc)(WMStartParentCallback *arg);

typedef struct WifiSetupWhWork {
    WmParentParam parentParam;
    u32 state;       // WIFI_SETUP_WH_STATE_*
    u32 connectMode; // WIFI_SETUP_WH_MODE_*
    u32 unk_48;
    WifiSetupWhJudgeFunc judgeChild; // may refuse a new child
    u16 unk_50;
    u16 connectedMask; // bit per AID, bit 0 the parent
    u32 errorCode;
    u32 randomSeed;
    u16 channel; // decided by the measurement
    u16 minBusyRatio;
    u16 quietChannels; // channels with the lowest busy ratio
    u8 unk_62[0x1E];
    u8 wmWork[0xF00];
    u8 sendBuffer[0xE0];
    u8 recvBuffer[0x240];
    u32 sendBufferSize;
    u32 recvBufferSize;
    u8 unk_12A8[0x100];
    u32 unk_13A8;
    int (*getWepKey)(u8 *key, struct WifiSetupWhWork *work); // returns the WEP mode; NULL: no WEP
    u32 unk_13B0;
    u8 unk_13B4[0xC];
    u8 wepKey[0x20];
    WMDataSharingInfo dataSharingInfo;
    WMDataSet dataSet;
    WMKeySetBuf keySharing;
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
    } send;

    union {
        WifiSetupSharePacket packets[2]; // by AID
        u8 raw[0x100];
    } received;

    u16 blockIndex; // settings block sent next (0-35, mod 16)
    u16 unk_202;
    u32 errorCount;        // step failures without the child's answer
    u32 receivedFlags[16]; // by AID
    u8 unk_248[0x400];
    u16 tgid;
    u16 channel;
    u8 backup[0x404];
    u8 firstBlock[0x40];
    u8 state;
    u8 prevState;
    u8 languageCode; // sent to the child
    u8 sendingData;  // the child answered, settings blocks are sent
    u32 frame;
    u32 counter;
    u32 timer;
    void *childUsers[1]; // by AID - 1
    u8 *data;            // the connection settings (WifiSetupConnections_Get)
    u32 unk_AA8;
    u8 request;
    WifiSetupSharePacket *sendPacket; // = &send.packet
    WifiSetupSessionParams params;
    u8 unk_AD0[0x30];
    u8 mbParent[0x1E280 - 0xB00]; // WifiSetupMbParent
} WifiSetupShare;

extern WifiSetupWh gWifiSetupWh;
extern WifiSetupShare *gWifiSetupShare;

int WifiSetupMbParent_GetChildState(int aid);

void WifiSetupWh_Init(u8 *work);
int WifiSetupWh_StartConnection(u32 mode, u16 tgid, u16 channel);

void WifiSetupShare_Init(WifiSetupShare *share, const WifiSetupSessionParams *params);
int WifiSetupShare_Cancel(void);
int WifiSetupShare_Start(void);
int WifiSetupShare_StartDownload(void);
void WifiSetupShare_Update(void);
void WifiSetupShare_GetState(u8 *state, u8 *changed);
void *WifiSetupShare_GetChildUser(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SHARE_H
