#include "application/main_menu/ranger_link.h"

#include "global.h"

// Wireless link to Pokemon Ranger (Manaphy Egg transfer), built directly on the WM library.
// HeartGold is either the parent (it measures the channels, starts a parent and sends the egg data to
// every child that connected) or a child (it scans for parents, the player picks one, and it connects
// and receives). Every packet starts with a RangerLinkPacket header and is checked with a CRC; the
// payload may be scrambled with a seed from the header.

// WM callbacks
typedef void (*RangerLinkWmCallback)(void *arg);

int WM_Init(void *workBuffer, u16 dmaNo);
int WM_SetIndCallback(RangerLinkWmCallback callback);
int WM_Enable(RangerLinkWmCallback callback);
int WM_Disable(RangerLinkWmCallback callback);
int WM_PowerOn(RangerLinkWmCallback callback);
int WM_PowerOff(RangerLinkWmCallback callback);
int WM_Reset(RangerLinkWmCallback callback);
int WM_End(RangerLinkWmCallback callback);
int WM_Finish(void);
int WM_MeasureChannel(RangerLinkWmCallback callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
int WM_SetParentParameter(RangerLinkWmCallback callback, void *parentParam);
int WM_StartParent(RangerLinkWmCallback callback);
int WM_StartScan(RangerLinkWmCallback callback, void *scanParam);
int WM_EndScan(RangerLinkWmCallback callback);
int WM_StartConnectEx(RangerLinkWmCallback callback, void *bssDesc, void *ssid, int powerSave, u16 authMode);
int WM_SetPortCallback(u16 port, RangerLinkWmCallback callback, void *arg);
int WM_StartMP(RangerLinkWmCallback callback, void *recvBuf, u16 recvSize, void *sendBuf, u16 sendSize, u16 mode);
int WM_SetMPDataToPortEx(RangerLinkWmCallback callback, void *arg, void *data, u16 size, u32 destBitmap, u16 port, int priority);
u16 SVC_GetCRC16(u32 value, const void *data, u32 size);
u16 WM_GetAllowedChannel(void);
u16 WM_GetDispersionBeaconPeriod(void);
u16 WM_GetDispersionScanPeriod(void);
u16 WM_GetNextTgid(void);
int WM_GetLinkLevel(void);

#define WM_OK        0
#define WM_OPERATING 2

// RangerLinkWork.state
enum {
    RANGER_LINK_STATE_STOPPED,    // 0
    RANGER_LINK_STATE_ENABLED,    // 1
    RANGER_LINK_STATE_POWERED_ON, // 2
    RANGER_LINK_STATE_BUSY,       // 3
    RANGER_LINK_STATE_ERROR,      // 4
    RANGER_LINK_STATE_5,          // 5
    RANGER_LINK_STATE_SCANNING,   // 6
    RANGER_LINK_STATE_SCAN_ENDED, // 7
    RANGER_LINK_STATE_PARENT,     // 8
    RANGER_LINK_STATE_CONNECTED,  // 9
    RANGER_LINK_STATE_PARENT_MP,  // 10
    RANGER_LINK_STATE_CHILD_MP,   // 11
    RANGER_LINK_STATE_ENDING,     // 12
};

// RangerLinkWork.mode
#define RANGER_LINK_MODE_CHILD  1
#define RANGER_LINK_MODE_PARENT 2

#define RANGER_LINK_MAX_PEERS     8
#define RANGER_LINK_PACKET_SIZE   0x40
#define RANGER_LINK_COMMAND_LIST  0xFD // parent: announce to the connected children
#define RANGER_LINK_COMMAND_REPLY 0xFE

// one header word: who sent the packet / who it is for (the GGID-like id of the game and its version)
typedef struct RangerLinkHeaderWord {
    u32 gameCode : 8;
    u32 version : 4;
    u32 encrypted : 4;
    u32 id : 16; // sender word: the scramble seed
} RangerLinkHeaderWord;

// every packet starts with this (0x10 bytes); headerSize is the payload offset
typedef struct RangerLinkPacket {
    RangerLinkHeaderWord sender;
    RangerLinkHeaderWord receiver;
    u32 command : 8;
    u32 sequence : 8;
    u32 crc : 16;
    u32 headerSize : 8;
    u32 length : 24;
} RangerLinkPacket;

// user game info of the beacon (0x38 bytes)
typedef struct RangerLinkHeaderPair {
    RangerLinkHeaderWord sender;
    RangerLinkHeaderWord receiver;
} RangerLinkHeaderPair;

typedef struct RangerLinkGameInfo {
    RangerLinkHeaderPair header;
    u16 name1[12];
    u16 name2[12];
} RangerLinkGameInfo;

// gRangerLinkParentParam (asm data): parameters for WM_SetParentParameter
typedef struct RangerLinkParentParam {
    RangerLinkGameInfo *gameInfo;
    u16 gameInfoLength;
    u8 unk06[6];
    u16 tgid;
    u8 unk0E[8];
    u16 noFixedFrequency;
    u16 beaconPeriod;
    u8 unk1A[0x18];
    u16 channel;
    u16 maxSendSize;
    u8 unk36[0xA];
} RangerLinkParentParam;

// parameters for WM_StartScan
typedef struct RangerLinkScanParam {
    void *scanBuffer;
    u16 channel;
    u16 maxChannelTime;
    u8 bssid[6];
} RangerLinkScanParam;

// parent mode: one connected child (active: connected with a matching SSID, acknowledged: answered the list packet)
typedef struct RangerLinkChild {
    u32 macAddressTail;
    u16 macAddressHead;
    u16 aid;
    u8 acknowledged;
    u8 listSendsLeft;
    u8 active;
    u8 unk0B;
} RangerLinkChild;

// parent mode peer table, at RangerLinkWork.peers
typedef struct RangerLinkParentPeers {
    RangerLinkChild children[RANGER_LINK_MAX_PEERS];
    u8 measureState;
    u8 lowestBusyRatio;
    u16 channelsToMeasure;
    u8 quietestChannel;
} RangerLinkParentPeers;

// child mode: one parent found while scanning
typedef struct RangerLinkFoundParent {
    u32 macAddressTail;
    u16 name1[12];
    u16 name2[12];
    u16 macAddressHead;
    u8 linkLevel;
    u8 unk37;
} RangerLinkFoundParent;

// child mode peer table, at RangerLinkWork.peers
typedef struct RangerLinkChildPeers {
    RangerLinkFoundParent parents[RANGER_LINK_MAX_PEERS];
    u16 scanTimer;
    u8 beaconsSeen;
    u8 selected : 4;
    u8 scanPhase : 4;
} RangerLinkChildPeers;

// send / receive progress, at RangerLinkWork.transfer
typedef struct RangerLinkTransfer {
    const void *data;
    const void *dest;
    RangerLinkPacket *sendBuffer;
    RangerLinkPacket *recvBuffer;
    int size;
    int sent;
    u8 sendMode;
    u8 state;
    u8 timer;
    u8 retries;
    u8 command;
    u8 unk1D[3];
} RangerLinkTransfer;

// gRangerLinkWork
typedef struct RangerLinkWork {
    u8 state;
    u8 requestedState;
    u8 mode;
    u8 port;
    u32 ggid;
    u8 unk08[4];
    u16 aid;
    u8 unk0E[2];
    u32 indCallbackSet;
    BOOL connected;
    BOOL disconnected;
    BOOL resetDone;
    BOOL finished;
    RangerLinkScanParam *scanParam;
    void *bssDesc;
    u32 sendBufferSize;
    u32 recvBufferSize;
    void *wmBuffer;
    void *sendBuffer;
    void *recvBuffer;
    RangerLinkGameInfo gameInfo;
    RangerLinkHeaderPair ssid;
    u8 unk80[0x10];

    union {
        RangerLinkParentPeers parent;
        RangerLinkChildPeers child;
        u8 raw[0x1C4];
    } peers;

    RangerLinkTransfer transfer;
} RangerLinkWork;

// argument of the WM callbacks (common head)
typedef struct RangerLinkWmResult {
    u16 apiId;
    u16 errCode;
    u16 wlCmdId;
    u16 wlResult;
    u16 state;
    u16 aid;
    u16 unk0C;
} RangerLinkWmResult;

typedef struct RangerLinkConfig {
    RangerLinkHeaderWord receiver;
    u16 *name2;
    u16 *name1;
} RangerLinkConfig;

typedef struct RangerLinkSsidCheck {
    u32 gameCode : 8;
    u32 version : 4;
    u32 unused : 20;
    u32 receiver;
} RangerLinkSsidCheck;

// WM_StartParent callback argument
typedef struct RangerLinkParentResult {
    u16 apiId;
    u16 errCode;
    u32 unk04;
    u16 state;
    u8 macAddress[6];
    u16 aid;
    u16 unk12;
    RangerLinkSsidCheck ssid;
} RangerLinkParentResult;

typedef struct RangerLinkMacTail {
    u32 value;
} RangerLinkMacTail;

typedef struct RangerLinkMacHead {
    u16 value;
} RangerLinkMacHead;

typedef struct RangerLinkName {
    u16 chars[12];
} RangerLinkName;

// child mode, a parent found while scanning (field view of RangerLinkFoundParent)
typedef struct RangerLinkFoundParentEntry {
    RangerLinkMacTail macAddressTail;
    RangerLinkName name1;
    RangerLinkName name2;
    RangerLinkMacHead macAddressHead;
    u8 linkLevel;
    u8 unk37;
} RangerLinkFoundParentEntry;

// WM_StartScan callback argument
typedef struct RangerLinkScanResult {
    u8 unk00[0x12];
    u16 linkLevel;
} RangerLinkScanResult;

// beacon (WM_StartScan result buffer)
typedef struct RangerLinkBssDesc {
    u32 unk00;
    u8 macAddress[0x54];
    RangerLinkName name1;
    RangerLinkName name2;
} RangerLinkBssDesc;

typedef struct RangerLinkBeaconCheck {
    u32 gameCode : 8;
    u32 version : 4;
    u32 unused : 20;
    u32 receiverLow : 16;
    u32 receiverId : 16;
} RangerLinkBeaconCheck;

typedef struct RangerLinkBeacon {
    u8 unk00[0x44];
    u32 ggid;
    u8 unk48[8];
    RangerLinkBeaconCheck gameInfo;
} RangerLinkBeacon;

typedef struct RangerLinkScanList {
    RangerLinkFoundParent parents[RANGER_LINK_MAX_PEERS];
    u16 scanTimer;
    volatile u8 beaconsSeen;
} RangerLinkScanList;

typedef struct RangerLinkScanFound {
    u8 unk00[0x36];
    u16 gameInfoLength;
} RangerLinkScanFound;

typedef union RangerLinkCheckWord {
    u32 raw;

    struct {
        u32 gameCode : 8;
        u32 version : 4;
        u32 unused : 20;
    };
} RangerLinkCheckWord;

typedef struct RangerLinkSelectedBeacon {
    u8 unk00[0x3C];
    u16 gameInfoLength;
    u16 unk3E;
    u32 unk40;
    u32 ggid;
    u32 unk48[2];
    RangerLinkCheckWord sender;

    union {
        u32 receiver;

        struct {
            u32 receiverLow : 16;
            u32 receiverId : 16;
        };
    };
} RangerLinkSelectedBeacon;

typedef struct RangerLinkSelectList {
    u32 parents[RANGER_LINK_MAX_PEERS][14];
    u8 unk1C0[3];
    u8 selected : 4;
    u8 scanPhase : 4;
} RangerLinkSelectList;

// port receive callback argument
typedef struct RangerLinkRecvResult {
    u16 apiId;
    u16 errCode;
    u16 state;
    u16 port;
    u32 unk08;
    void *data;
    u16 length;
    u16 aid;
} RangerLinkRecvResult;

typedef struct RangerLinkRecvTransfer {
    const void *data;
    void *dest;
    RangerLinkPacket *sendBuffer;
    u32 *recvBuffer;
    u8 unk10[9];
    volatile u8 state;
    u8 timer;
    u8 retries;
    u8 command;
} RangerLinkRecvTransfer;

typedef struct RangerLinkPhase {
    u8 selected : 4;
    u8 scanPhase : 4;
} RangerLinkPhase;

#define FOUND_PARENT_SIZE 0x38

// the packet header as seen when unscrambling
typedef struct RangerLinkPacketTail {
    u32 receiver;
    u32 commandAndCrc;
    u32 headerSize : 8;
    u32 length : 24;
} RangerLinkPacketTail;

typedef struct RangerLinkScrambledPacket {
    u32 senderCode : 12;
    u32 encrypted : 4;
    u32 seed : 16;
    RangerLinkPacketTail tail;
    u32 data[1];
} RangerLinkScrambledPacket;

extern RangerLinkParentParam gRangerLinkParentParam;
extern RangerLinkWork gRangerLinkWork;

void RangerLink_Abort(void);
u32 RangerLink_GetMacAddressTail(u8 *macAddress);
int RangerLink_GetMacAddressHead(u8 *macAddress);
BOOL RangerLink_HandleEndRequest(void);
void RangerLink_RemoveChild(u16 aid);
int RangerLink_NextChannelToMeasure(void);
BOOL RangerLink_InitChannelMeasure(void);
void RangerLink_SetBeaconTiming(void);
void RangerLink_InitScanParam(void);
void RangerLink_NextScanChannel(void);
void RangerLink_IndicationCallback(void *arg);
void RangerLink_SetMPDataCallback(void *arg);
void RangerLink_EnableCallback(void *arg);
void RangerLink_DisableCallback(void *arg);
void RangerLink_MeasureChannelCallback(u16 *arg);
void RangerLink_SetParentParameterCallback(void *arg);
void RangerLink_StartParentCallback(void *arg);
int RangerLink_AddFoundParent(RangerLinkScanResult *result, RangerLinkBssDesc *bssDesc);
int RangerLink_HandleBeacon(RangerLinkScanFound *result);
void RangerLink_ScanNextChannel(void);
void RangerLink_ScanCallback(void *arg);
int RangerLink_HandleSelectedBeacon(void);
void RangerLink_ScanSelectedCallback(void *arg);
void RangerLink_EndScanCallback(void *arg);
void RangerLink_StartConnectCallback(void *arg);
void RangerLink_StartAfterPowerOn(void);
void RangerLink_StartMPCallback(void *arg);
void RangerLink_ReceiveAck(void *arg);
void RangerLink_ReceiveListPacket(void *arg);
void RangerLink_ReceiveCommand(RangerLinkRecvResult *recv);
void RangerLink_PortCallback(void *arg);
void RangerLink_PowerOnCallback(void *arg);
void RangerLink_PowerOffCallback(void *arg);
void RangerLink_ResetCallback(void *arg);
void RangerLink_EndCallback(void *arg);
int RangerLink_SetIndicationCallback(void);
void RangerLink_Begin(void);
void RangerLink_SendData(void *data, u32 size, u32 destBitmap);
u32 RangerLink_GetSendBufferSize(void);
u32 RangerLink_GetRecvBufferSize(void);
void RangerLink_UpdateSend(void);
void RangerLink_UpdateCommand(void);
void RangerLink_UpdateTransfer(void);
void RangerLink_UpdateChild(void);
void RangerLink_UpdateParent(void);
int RangerLink_CountFoundParents(void);
int RangerLink_CountActiveChildren(void);
RangerLinkParentParam *RangerLink_GetParentParam(void);
RangerLinkWork *RangerLink_GetWork(void);
u8 RangerLink_GetState(void);
void RangerLink_SetState(u8 state);
u8 RangerLink_GetRequestedState(void);
void RangerLink_SetRequestedState(u8 state);
u8 RangerLink_GetMode(void);
void RangerLink_SetMode(u8 mode);
int RangerLink_GetPort(void);
RangerLinkScanParam *RangerLink_GetScanParam(void);
u32 RangerLink_GetSendSize(void);
u32 RangerLink_GetRecvSize(void);
void *RangerLink_GetSendBuffer(void);
void *RangerLink_GetRecvBuffer(void);
void *RangerLink_GetBssDesc(void);
void RangerLink_SetAid(u16 aid);
BOOL RangerLink_IsConnectedToParent(void);
void RangerLink_SetConnected(BOOL connected);
void RangerLink_SetDisconnected(BOOL disconnected);
void RangerLink_SetFinished(BOOL finished);
void *RangerLink_GetPeers(void);
RangerLinkChildPeers *RangerLink_GetFoundParents(void);
RangerLinkParentPeers *RangerLink_GetChildren(void);
void RangerLink_ClearFoundParents(void);
RangerLinkTransfer *RangerLink_GetTransfer(void);
void RangerLink_ResetTransfer(void);
u32 RangerLink_GetGgid(void);
u8 RangerLink_GetGameCode(void);
u32 RangerLink_GetVersion(void);
u32 RangerLink_GetReceiverId(void);
u32 RangerLink_GetGameInfoLength(void);
RangerLinkGameInfo *RangerLink_GetGameInfo(void);
void *RangerLink_GetSsid(void);
s16 RangerLink_ReadMacAddressHead(u8 *macAddress);
u32 RangerLink_GetPacketHeaderSize(void);
u32 RangerLink_NewSeed(void);
u32 RangerLink_Scramble(u32 seed, u32 size, u32 *data);
void RangerLink_BuildPacket(RangerLinkPacket *packet, const void *data, u32 length, int sequence, int command);
void RangerLink_Unscramble(RangerLinkPacket *packet);
BOOL RangerLink_CheckCrc(u32 *packet);
void RangerLink_DebugError();
void RangerLink_DebugState(u16 value);
void RangerLink_DebugScan(void);
void RangerLink_DebugFoundParents(RangerLinkChildPeers *peers);
void RangerLink_DebugChildren(RangerLinkParentPeers *peers);
void RangerLink_DebugPacket(RangerLinkPacket *packet);
BOOL RangerLink_SetMPData(void *data, int size, u32 destBitmap);
BOOL RangerLink_WmEnable(void);
BOOL RangerLink_WmDisable(void);
BOOL RangerLink_WmMeasureChannel(u32 channel);
BOOL RangerLink_WmSetParentParameter(void);
BOOL RangerLink_WmStartParent(void);
BOOL RangerLink_WmStartScan(void);
BOOL RangerLink_WmStartScanSelected(void);
BOOL RangerLink_WmEndScan(void);
BOOL RangerLink_WmStartConnect(void);
BOOL RangerLink_WmStartMP(void);
BOOL RangerLink_WmPowerOn(void);
BOOL RangerLink_WmPowerOff(void);
BOOL RangerLink_WmReset(void);
BOOL RangerLink_WmEnd(void);

static inline RangerLinkMacTail GetMacAddressTail(RangerLinkParentResult *result) {
    RangerLinkMacTail tail;
    tail.value = RangerLink_GetMacAddressTail(result->macAddress);
    return tail;
}

static inline RangerLinkMacHead GetMacAddressHead(RangerLinkParentResult *result) {
    RangerLinkMacHead head;
    head.value = RangerLink_GetMacAddressHead(result->macAddress);
    return head;
}

static inline RangerLinkMacTail GetBeaconMacAddressTail(RangerLinkBssDesc *bssDesc) {
    RangerLinkMacTail tail;
    tail.value = RangerLink_GetMacAddressTail(bssDesc->macAddress);
    return tail;
}

static inline RangerLinkMacHead GetBeaconMacAddressHead(RangerLinkBssDesc *bssDesc) {
    RangerLinkMacHead head;
    head.value = RangerLink_GetMacAddressHead(bssDesc->macAddress);
    return head;
}

void RangerLink_Abort(void) {
    RangerLink_SetState(RANGER_LINK_STATE_ERROR);
    RangerLink_WmReset();
}

#pragma force_active on

u32 RangerLink_GetMacAddressTail(u8 *macAddress) {
    volatile u32 result = RangerLink_ReadMacAddressTail(macAddress);
}

int RangerLink_GetMacAddressHead(u8 *macAddress) {
    volatile s16 result = RangerLink_ReadMacAddressHead(macAddress);
    return result;
}

#pragma force_active reset

BOOL RangerLink_HandleEndRequest(void) {
    RangerLinkWork *work;
    if (RangerLink_GetRequestedState() == RANGER_LINK_STATE_ENDING) {
        work = RangerLink_GetWork();
        if (work->resetDone == FALSE) {
            RangerLink_WmReset();
            work->resetDone = TRUE;
        }
        return TRUE;
    }
    return FALSE;
}

void RangerLink_RemoveChild(u16 aid) {
    RangerLinkChild *child = RangerLink_GetChildren()->children;
    int i;
    for (i = 0; i < RANGER_LINK_MAX_PEERS; i++, child++) {
        if (child->macAddressTail != 0 && aid == child->aid) {
            child->acknowledged = FALSE;
            child->active = FALSE;
        }
    }
}

int RangerLink_NextChannelToMeasure(void) {
    RangerLinkParentPeers *peers = RangerLink_GetChildren();
    u16 channels = peers->channelsToMeasure;
    if (channels & (1 << 0)) {
        peers->channelsToMeasure &= ~(1 << 0);
        return 1;
    }
    if (channels & (1 << 6)) {
        peers->channelsToMeasure &= ~(1 << 6);
        return 7;
    }
    if (channels & (1 << 12)) {
        peers->channelsToMeasure &= ~(1 << 12);
        return 13;
    }
    return 0;
}

BOOL RangerLink_InitChannelMeasure(void) {
    RangerLinkParentPeers *peers = RangerLink_GetChildren();
    u16 allowed;
    peers->measureState = 1;
    allowed = WM_GetAllowedChannel();
    peers->channelsToMeasure = allowed;
    if (peers->channelsToMeasure == 0 || peers->channelsToMeasure == 0x8000) {
        return FALSE;
    }
    peers->lowestBusyRatio = 102;
    peers->quietestChannel = 0;
    return TRUE;
}

void RangerLink_SetBeaconTiming(void) {
    RangerLinkParentParam *param = RangerLink_GetParentParam();
    param->beaconPeriod = WM_GetDispersionBeaconPeriod();
    param->tgid = WM_GetNextTgid();
}

void RangerLink_InitScanParam(void) {
    RangerLinkScanParam *scanParam = RangerLink_GetScanParam();
    scanParam->scanBuffer = RangerLink_GetBssDesc();
    scanParam->channel = RangerLink_GetParentParam()->channel;
    scanParam->maxChannelTime = WM_GetDispersionScanPeriod();
    scanParam->bssid[0] = 0xFF;
    scanParam->bssid[1] = 0xFF;
    scanParam->bssid[2] = 0xFF;
    scanParam->bssid[3] = 0xFF;
    scanParam->bssid[4] = 0xFF;
    scanParam->bssid[5] = 0xFF;
}

#pragma force_active on

// next allowed channel after the current scan channel
void RangerLink_NextScanChannel(void) {
    int i, start, channel;
    u32 allowed;
    RangerLinkScanParam *scanParam = RangerLink_GetScanParam();
    allowed = WM_GetAllowedChannel();
    if (allowed) {
        start = scanParam->channel;
        channel = start;
        for (i = 0; i < 16; i++, channel++) {
            if (allowed & (1 << (channel % 16))) {
                scanParam->channel = (start + i) % 16 + 1;
                return;
            }
        }
    }
}

#pragma force_active reset

// WM_SetIndCallback
void RangerLink_IndicationCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    RangerLink_DebugState(result->wlCmdId);
    if (result->errCode == 8) {
        RangerLink_DebugError();
        RangerLink_Abort();
    }
}

// WM_SetMPDataToPortEx
void RangerLink_SetMPDataCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    RangerLink_DebugState(result->state);
    if (result->errCode != WM_OK) {
        RangerLink_DebugError();
    }
}

// WM_Enable
void RangerLink_EnableCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        RangerLink_SetState(RANGER_LINK_STATE_POWERED_ON);
        if (RangerLink_GetRequestedState() == RANGER_LINK_STATE_ENDING) {
            RangerLink_WmDisable();
            RangerLink_SetState(RANGER_LINK_STATE_POWERED_ON);
        } else if (!RangerLink_WmPowerOn()) {
            RangerLink_WmDisable();
            RangerLink_SetState(RANGER_LINK_STATE_POWERED_ON);
        }
    } else {
        RangerLink_DebugError();
        RangerLink_WmDisable();
    }
}

// WM_Disable
void RangerLink_DisableCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        if (RangerLink_GetRequestedState() == RANGER_LINK_STATE_ENDING) {
            WM_Finish();
            RangerLink_SetState(RANGER_LINK_STATE_ENDING);
            RangerLink_SetFinished(TRUE);
        } else {
            RangerLink_SetState(RANGER_LINK_STATE_ENABLED);
        }
    } else {
        RangerLink_DebugError();
        RangerLink_SetState(RANGER_LINK_STATE_ERROR);
    }
}

#pragma force_active on

// WM_MeasureChannel: remember the quietest channel, measure the next one, then set up the parent
void RangerLink_MeasureChannelCallback(u16 *arg) {
    if (arg[1] == WM_OK) {
        RangerLinkParentPeers *peers = RangerLink_GetChildren();
        if (peers->lowestBusyRatio > arg[5]) {
            peers->lowestBusyRatio = arg[5];
            peers->quietestChannel = arg[4];
        }
        u32 channel = RangerLink_NextChannelToMeasure();
        if (channel) {
            if (RangerLink_WmMeasureChannel(channel) == FALSE) {
                RangerLink_Abort();
            }
        } else {
            if (peers->lowestBusyRatio < 102) {
                RangerLink_SetBeaconTiming();
                if (RangerLink_WmSetParentParameter() == FALSE) {
                    RangerLink_Abort();
                }
            } else {
                RangerLink_Abort();
            }
        }
    } else {
        RangerLink_DebugError();
        RangerLink_Abort();
    }
}

#pragma force_active reset

// WM_SetParentParameter
void RangerLink_SetParentParameterCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        if (!RangerLink_HandleEndRequest() && !RangerLink_WmStartParent()) {
            RangerLink_Abort();
        }
    } else {
        RangerLink_DebugError();
        RangerLink_Abort();
    }
}

#pragma force_active on

// WM_StartParent: register each child that connects with a matching SSID
void RangerLink_StartParentCallback(void *arg) {
    RangerLinkParentResult *result = arg;
    RangerLinkParentPeers *peers;
    RangerLink_DebugState(result->state);
    if (!result->errCode) {
        peers = RangerLink_GetChildren();
        RangerLink_SetState(RANGER_LINK_STATE_PARENT);
        if (RangerLink_HandleEndRequest()) {
            return;
        }
        if (peers->measureState == 1) {
            peers->measureState = 2;
        }
        switch (result->state) {
        case 0:
            if (!RangerLink_WmStartMP()) {
                RangerLink_Abort();
            } else {
                RangerLink_SetAid(0);
            }
            break;
        case 2:
            break;
        case 7:
            RangerLink_DebugChildren(RangerLink_GetChildren());
            if (peers->measureState == 2) {
                RangerLinkSsidCheck *ssid = &result->ssid;
                if (ssid->gameCode == RangerLink_GetGameCode() && ssid->version <= RangerLink_GetVersion() && (ssid->receiver >> 16) == RangerLink_GetReceiverId()) {
                    peers = RangerLink_GetChildren();
                    RangerLinkMacTail tail = GetMacAddressTail(result);
                    RangerLinkMacHead head = GetMacAddressHead(result);
                    int i;
                    for (i = 0; i < RANGER_LINK_MAX_PEERS; i++) {
                        u32 known = peers->children[i].macAddressTail;
                        if (known == tail.value) {
                            peers->children[i].aid = result->aid;
                            peers->children[i].active = TRUE;
                            return;
                        }
                        if (!known) {
                            peers->children[i].macAddressTail = tail.value;
                            peers->children[i].macAddressHead = head.value;
                            peers->children[i].aid = result->aid;
                            peers->children[i].active = TRUE;
                            return;
                        }
                    }
                }
            }
            break;
        case 9:
            RangerLink_RemoveChild(result->aid);
            break;
        }
    } else {
        RangerLink_DebugError();
        RangerLink_Abort();
    }
}

// returns 1 if the parent is (now) in the list, 0 if the list is full
int RangerLink_AddFoundParent(RangerLinkScanResult *result, RangerLinkBssDesc *bssDesc) {
    RangerLinkFoundParentEntry *parents = (RangerLinkFoundParentEntry *)RangerLink_GetFoundParents();
    RangerLinkMacTail tail = GetBeaconMacAddressTail(bssDesc);
    RangerLinkMacHead head = GetBeaconMacAddressHead(bssDesc);
    int i = 0;
    RangerLinkFoundParentEntry *parent = parents;

    for (; i < RANGER_LINK_MAX_PEERS; i++, parent++) {
        RangerLinkMacHead *knownHead = &parent->macAddressHead;
        if (parent->macAddressTail.value == tail.value && knownHead->value == head.value) {
            return 1;
        }
        if (parent->macAddressTail.value == 0) {
            parents[i].macAddressTail.value = tail.value;
            parents[i].name1 = bssDesc->name1;
            parents[i].name2 = bssDesc->name2;
            parents[i].macAddressHead.value = head.value;
            parents[i].linkLevel = result->linkLevel;
            return 1;
        }
    }
    return 0;
}

#pragma force_active reset

void RangerLink_ScanNextChannel(void) {
    RangerLink_NextScanChannel();
    if (!RangerLink_WmStartScan()) {
        RangerLink_Abort();
    }
}

#pragma force_active on

// a beacon came in while listing the parents; TRUE if it was handled
int RangerLink_HandleBeacon(RangerLinkScanFound *result) {
    RangerLinkBeacon *beacon = RangerLink_GetBssDesc();
    RangerLinkScanList *list = (RangerLinkScanList *)RangerLink_GetFoundParents();
    int found, i;
    RangerLinkFoundParent *parent;
    if (RangerLink_HandleEndRequest()) {
        return 1;
    }
    found = 0;
    i = 0;
    parent = list->parents;
    for (; i < RANGER_LINK_MAX_PEERS; i++, parent++) {
        if (parent->macAddressTail) {
            found = 1;
            break;
        }
    }
    if (found && !list->scanTimer) {
        if (!RangerLink_WmEndScan()) {
            RangerLink_Abort();
        }
        return 1;
    }
    DC_InvalidateRange(beacon, 0xC0);
    RangerLink_DebugFoundParents(RangerLink_GetFoundParents());
    if (result->gameInfoLength >= 8 && RangerLink_GetGgid() == beacon->ggid) {
        RangerLinkBeaconCheck *check = &beacon->gameInfo;
        if (RangerLink_GetGameCode() == check->gameCode && check->version <= RangerLink_GetVersion() && RangerLink_GetReceiverId() == check->receiverId) {
            if (RangerLink_AddFoundParent((RangerLinkScanResult *)result, (RangerLinkBssDesc *)beacon)) {
                list = (RangerLinkScanList *)RangerLink_GetFoundParents();
                ++list->beaconsSeen;
                if (list->beaconsSeen > 28 && !RangerLink_WmEndScan()) {
                    RangerLink_Abort();
                    return 1;
                }
            } else {
                if (!RangerLink_WmEndScan()) {
                    RangerLink_Abort();
                }
                return 1;
            }
        }
    }
    return 0;
}

#pragma force_active reset

// WM_StartScan while listing the parents
void RangerLink_ScanCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    RangerLink_DebugState(result->state);
    if (result->errCode == WM_OK) {
        RangerLink_GetBssDesc();
        RangerLink_DebugScan();
        RangerLink_SetState(RANGER_LINK_STATE_SCANNING);
        if (!RangerLink_HandleEndRequest()) {
            if (RangerLink_GetMode() == RANGER_LINK_MODE_CHILD) {
                u8 *peers = (u8 *)RangerLink_GetFoundParents();
                peers[0x1C3] = (peers[0x1C3] & ~0xF0) | 0x10;
            }
            if (result->state == 4) {
                goto scan_next;
            }
            if (result->state == 5) {
                if (RangerLink_HandleBeacon(arg)) {
                    return;
                }
scan_next:
                RangerLink_ScanNextChannel();
            } else {
                RangerLink_DebugError(result->errCode);
                RangerLink_Abort();
            }
        }
    } else {
        RangerLink_DebugError(result->errCode);
        RangerLink_Abort();
    }
}

#pragma force_active on

// a beacon came in while looking for the selected parent; TRUE if it was handled
int RangerLink_HandleSelectedBeacon(void) {
    RangerLinkSelectedBeacon *beacon;
    RangerLinkCheckWord *check;
    RangerLinkSelectList *list;
    beacon = RangerLink_GetBssDesc();
    list = (RangerLinkSelectList *)RangerLink_GetFoundParents();
    RangerLink_GetParentParam();
    if (RangerLink_HandleEndRequest()) {
        return 1;
    }
    DC_InvalidateRange(beacon, 0xC0);
    RangerLink_DebugFoundParents(RangerLink_GetFoundParents());
    if (beacon->gameInfoLength >= 8 && beacon->ggid == RangerLink_GetGgid()) {
        check = &beacon->sender;
        if (check->gameCode == RangerLink_GetGameCode() && check->version <= RangerLink_GetVersion() && (*(RangerLinkCheckWord *)((u8 *)check + 4)).raw >> 16 == RangerLink_GetReceiverId()) {
            u32 tail = RangerLink_GetMacAddressTail((u8 *)beacon + 4);

            volatile struct {
                u32 a, b;
            } pair;

            pair.a = tail;
            pair.b = tail;
            u32 selectedTail = list->parents[list->selected][0];
            if (selectedTail == tail) {
                if (!RangerLink_WmEndScan()) {
                    RangerLink_Abort();
                }
                return 1;
            }
        }
    }
    return 0;
}

#pragma force_active reset

// WM_StartScan while looking for the selected parent
void RangerLink_ScanSelectedCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    RangerLink_DebugState(result->state);
    if (result->errCode == WM_OK) {
        u8 *peers = (u8 *)RangerLink_GetFoundParents();
        peers[0x1C3] = (peers[0x1C3] & ~0xF0) | 0x20;
        RangerLink_SetState(RANGER_LINK_STATE_SCANNING);
        if (!RangerLink_HandleEndRequest()) {
            if (result->state == 4) {
                goto scan_next;
            } else if (result->state == 5) {
                if (RangerLink_HandleSelectedBeacon()) {
                    return;
                }
scan_next:
                RangerLink_NextScanChannel();
                if (!RangerLink_WmStartScanSelected()) {
                    RangerLink_Abort();
                }
            } else {
                RangerLink_DebugError(result->errCode);
                RangerLink_Abort();
            }
        }
    } else {
        RangerLink_DebugError(result->errCode);
        RangerLink_Abort();
    }
}

// WM_EndScan: connect to the selected parent
void RangerLink_EndScanCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        RangerLink_SetState(RANGER_LINK_STATE_SCAN_ENDED);
        if (!RangerLink_HandleEndRequest() && RangerLink_GetMode() == RANGER_LINK_MODE_CHILD && (((u32)((u8 *)RangerLink_GetFoundParents())[0x1C3] << 24) >> 28) == 2) {
            if (!RangerLink_WmStartConnect()) {
                RangerLink_Abort();
            } else {
                RangerLink_SetState(RANGER_LINK_STATE_BUSY);
            }
        }
    } else {
        RangerLink_DebugError(result->errCode);
        RangerLink_Abort();
    }
}

// WM_StartConnectEx
void RangerLink_StartConnectCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    RangerLink_DebugState(result->state);
    if (result->errCode == WM_OK) {
        RangerLink_SetState(RANGER_LINK_STATE_CONNECTED);
        if (!RangerLink_HandleEndRequest()) {
            switch (result->state) {
            case 6:
            case 8:
                break;
            case 7:
                RangerLink_SetAid(result->aid);
                RangerLink_SetConnected(TRUE);
                RangerLink_SetDisconnected(FALSE);
                if (!RangerLink_WmStartMP()) {
                    RangerLink_Abort();
                }
                break;
            case 9:
                if (RangerLink_IsConnectedToParent()) {
                    RangerLink_SetDisconnected(TRUE);
                }
                RangerLink_SetConnected(FALSE);
                break;
            default:
                RangerLink_DebugError(result->errCode);
                RangerLink_Abort();
                break;
            }
        }
    } else {
        RangerLink_DebugError(result->errCode);
        RangerLink_Abort();
    }
}

// powered on: child -> scan for parents, parent -> measure the channels
void RangerLink_StartAfterPowerOn(void) {
    u8 mode = RangerLink_GetMode();
    if (mode != RANGER_LINK_MODE_CHILD) {
        if (mode != RANGER_LINK_MODE_PARENT) {
            return;
        }
        if (RangerLink_InitChannelMeasure()) {
            if (RangerLink_WmMeasureChannel(RangerLink_NextChannelToMeasure())) {
                RangerLink_GetChildren()->measureState = 1;
                RangerLink_SetState(RANGER_LINK_STATE_BUSY);
                return;
            }
        }
        RangerLink_Abort();
    } else {
        RangerLink_InitScanParam();
        if (!RangerLink_WmStartScan()) {
            RangerLink_Abort();
        } else {
            RangerLink_SetState(RANGER_LINK_STATE_BUSY);
        }
    }
}

// WM_StartMP
void RangerLink_StartMPCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    u16 errCode;
    RangerLink_DebugState(result->wlCmdId);
    errCode = result->errCode;
    if (errCode == WM_OK) {
        if (result->apiId != 14 || !RangerLink_HandleEndRequest()) {
            u8 mode = RangerLink_GetMode();
            if (mode != RANGER_LINK_MODE_CHILD) {
                if (mode != RANGER_LINK_MODE_PARENT) {
                    return;
                }
                RangerLink_SetState(RANGER_LINK_STATE_PARENT_MP);
            } else {
                RangerLink_SetState(RANGER_LINK_STATE_CHILD_MP);
            }
        }
    } else if (errCode != 9 && errCode != 13 && errCode != 15) {
        RangerLink_Abort();
    }
}

// parent sending: a child answered
void RangerLink_ReceiveAck(void *arg) {
    RangerLinkRecvResult *recv = arg;
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    if (transfer->state == 1) {
        u32 *words = (u32 *)transfer->recvBuffer;
        DC_FlushRange(recv->data, recv->length);
        MI_CpuCopy8(recv->data, transfer->recvBuffer, recv->length);
        if ((u8)words[0] == RangerLink_GetGameCode() && ((words[0] << 20) >> 28) <= RangerLink_GetVersion()) {
            RangerLink_Unscramble(transfer->recvBuffer);
            if ((u8)words[2] == transfer->command) {
                RangerLink_DebugPacket(transfer->recvBuffer);
                if (RangerLink_CheckCrc((u32 *)transfer->recvBuffer) && ((words[2] << 16) >> 24) != 1) {
                    transfer->state = 2;
                } else {
                    transfer->state = 0;
                }
            }
        }
    }
}

#pragma force_active on

// child: the parent's list packet arrived, mark this child as reached
void RangerLink_ReceiveListPacket(void *arg) {
    RangerLinkRecvResult *recv = arg;
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    u32 *words;
    RangerLinkChild *children;
    if (RangerLink_GetMode() != RANGER_LINK_MODE_PARENT || transfer->state == 2) {
        return;
    }
    words = (u32 *)transfer->recvBuffer;
    DC_FlushRange(recv->data, recv->length);
    MI_CpuCopy8(recv->data, transfer->recvBuffer, recv->length);
    if ((words[0] & 0xFF) != RangerLink_GetGameCode()) {
        return;
    }
    if (((words[0] << 20) >> 28) > RangerLink_GetVersion()) {
        return;
    }
    RangerLink_Unscramble(transfer->recvBuffer);
    if ((words[2] & 0xFF) != RANGER_LINK_COMMAND_LIST) {
        return;
    }
    children = RangerLink_GetChildren()->children;
    {
        int i;
        for (i = 0; i < RANGER_LINK_MAX_PEERS; i++) {
            if (recv->aid == children[i].aid) {
                children[i].acknowledged = TRUE;
                children[i].listSendsLeft = 3;
                transfer->timer = 180;
                transfer->state = 0;
                return;
            }
        }
    }
}

// child receiving
void RangerLink_ReceiveCommand(RangerLinkRecvResult *recv) {
    RangerLinkRecvTransfer *transfer = (RangerLinkRecvTransfer *)RangerLink_GetTransfer();
    u32 *words;
    if (transfer->command == RANGER_LINK_COMMAND_LIST) {
        RangerLink_ReceiveListPacket(recv);
        return;
    }
    if ((transfer->state == 1 || transfer->state == 3) && transfer->retries == 0) {
        words = transfer->recvBuffer;
        DC_FlushRange(recv->data, recv->length);
        MI_CpuCopy8(recv->data, transfer->recvBuffer, recv->length);
        if ((words[0] & 0xFF) != RangerLink_GetGameCode()) {
            return;
        }
        if (((words[0] << 20) >> 28) > RangerLink_GetVersion()) {
            return;
        }
        RangerLink_Unscramble((RangerLinkPacket *)transfer->recvBuffer);
        if ((words[2] & 0xFF) != transfer->command) {
            return;
        }
        RangerLink_DebugPacket((RangerLinkPacket *)transfer->recvBuffer);
        transfer->state = 0;
        if (RangerLink_CheckCrc(transfer->recvBuffer)) {
            if ((words[2] & 0xFF) == RANGER_LINK_COMMAND_LIST) {
                RangerLink_ReceiveListPacket(recv);
            } else {
                MI_CpuCopy8((u8 *)transfer->recvBuffer + (words[3] & 0xFF), transfer->dest, words[3] >> 8);
            }
            words[2] &= 0xFFFF00FF;
            transfer->retries = 4;
        } else {
            words[2] = (words[2] & 0xFFFF00FF) | 0x100;
        }
    }
}

// WM_SetPortCallback (port 4)
void RangerLink_PortCallback(void *arg) {
    RangerLinkRecvResult *recv = arg;
    RangerLink_DebugState(recv->state);
    if (recv->errCode == WM_OK) {
        switch (recv->state) {
        case 21:
            switch (RangerLink_GetState()) {
            case RANGER_LINK_STATE_PARENT_MP:
            case RANGER_LINK_STATE_CHILD_MP:
                switch (RangerLink_GetTransfer()->sendMode) {
                case 1:
                    RangerLink_ReceiveAck(recv);
                    break;
                case 2:
                    RangerLink_ReceiveCommand(recv);
                    break;
                }
                break;
            }
            break;
        case 7:
            RangerLink_SetConnected(TRUE);
            RangerLink_SetDisconnected(FALSE);
            break;
        case 9:
            if (RangerLink_IsConnectedToParent()) {
                RangerLink_SetDisconnected(TRUE);
            }
            switch (RangerLink_GetState()) {
            case RANGER_LINK_STATE_PARENT_MP:
                RangerLink_RemoveChild(recv->aid);
                break;
            case RANGER_LINK_STATE_CHILD_MP:
                RangerLink_SetConnected(FALSE);
                break;
            }
            break;
        }
    }
}

#pragma force_active reset

// WM_PowerOn
void RangerLink_PowerOnCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        if (!RangerLink_HandleEndRequest()) {
            RangerLink_StartAfterPowerOn();
        }
    } else {
        RangerLink_DebugError();
        RangerLink_SetState(RANGER_LINK_STATE_ERROR);
    }
}

// WM_PowerOff
void RangerLink_PowerOffCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        if (RangerLink_WmDisable()) {
            RangerLink_SetState(RANGER_LINK_STATE_POWERED_ON);
        }
    } else {
        RangerLink_DebugError();
        RangerLink_Abort();
    }
}

// WM_Reset
void RangerLink_ResetCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        RangerLink_SetAid(0);
        if (RangerLink_GetRequestedState() == RANGER_LINK_STATE_ENDING) {
            if (RangerLink_WmEnd()) {
                RangerLink_SetState(RANGER_LINK_STATE_BUSY);
            }
        } else if (RangerLink_WmPowerOff()) {
            RangerLink_SetState(RANGER_LINK_STATE_BUSY);
        }
    } else {
        RangerLink_DebugError();
        RangerLink_SetState(RANGER_LINK_STATE_ERROR);
    }
}

// WM_End
void RangerLink_EndCallback(void *arg) {
    RangerLinkWmResult *result = arg;
    if (result->errCode == WM_OK) {
        RangerLink_SetState(RANGER_LINK_STATE_ENDING);
        RangerLink_SetFinished(TRUE);
    } else {
        RangerLink_DebugError();
        RangerLink_SetState(RANGER_LINK_STATE_ERROR);
    }
}

#pragma force_active on

int RangerLink_SetIndicationCallback(void) {
    if (WM_SetIndCallback(RangerLink_IndicationCallback) != WM_OK) {
        RangerLink_SetState(RANGER_LINK_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

void RangerLink_Begin(void) {
    switch (RangerLink_GetState()) {
    case RANGER_LINK_STATE_STOPPED:
    case RANGER_LINK_STATE_ENABLED:
    case RANGER_LINK_STATE_POWERED_ON:
        RangerLink_WmEnable();
        break;
    }
}

void RangerLink_SendData(void *data, u32 size, u32 destBitmap) {
    if (size <= RangerLink_GetParentParam()->maxSendSize) {
        DC_FlushRange(data, size);
        RangerLink_SetMPData(data, size, destBitmap);
    }
}

#pragma force_active reset

u32 RangerLink_GetSendBufferSize(void) {
    return 0x5C0;
}

u32 RangerLink_GetRecvBufferSize(void) {
    return 0x5C0;
}

// carves the WM buffers out of the caller's buffer and sets up the beacon info
void RangerLink_Init(const void *configArg, void *buffer) {
    const RangerLinkConfig *config = configArg;
    RangerLinkWork *work;
    u16 *name1;
    u16 *name2;
    u32 i;
    u32 addr = (u32)buffer;
    RangerLink_GetParentParam();
    work = RangerLink_GetWork();
    if (addr & 0x1F) {
        addr += 0x20 - (addr & 0x1F);
    }
    work->state = RANGER_LINK_STATE_STOPPED;
    work->requestedState = RANGER_LINK_STATE_STOPPED;
    work->mode = 0;
    work->port = 4;
    work->ggid = 0x00400131;
    work->aid = 0;
    work->wmBuffer = (void *)addr;
    addr += 0xF00;
    work->sendBuffer = (void *)addr;
    work->sendBufferSize = RangerLink_GetSendBufferSize();
    addr += work->sendBufferSize;
    work->recvBuffer = (void *)addr;
    work->recvBufferSize = RangerLink_GetRecvBufferSize();
    addr += work->recvBufferSize;
    work->bssDesc = (void *)addr;
    work->scanParam = (RangerLinkScanParam *)(addr + 0xC0);
    addr += 0xE0;
    WM_Init(work->wmBuffer, 2);
    work->indCallbackSet = RangerLink_SetIndicationCallback();
    work->connected = FALSE;
    work->disconnected = FALSE;
    work->resetDone = FALSE;
    work->finished = FALSE;
    work->gameInfo.header.sender.gameCode = 15;
    work->gameInfo.header.sender.version = 1;
    work->gameInfo.header.sender.encrypted = 1;
    work->gameInfo.header.receiver.gameCode = config->receiver.gameCode;
    work->gameInfo.header.receiver.version = config->receiver.version;
    work->gameInfo.header.receiver.encrypted = config->receiver.encrypted;
    work->gameInfo.header.receiver.id = config->receiver.id;
    name1 = config->name1;
    name2 = config->name2;
    for (i = 0; name1 != NULL && i < 12; i++, name1++, name2++) {
        work->gameInfo.name1[i] = *name1;
        work->gameInfo.name2[i] = *name2;
    }
    work->ssid = work->gameInfo.header;
    RangerLink_ResetTransfer();
    RangerLink_GetTransfer()->sendBuffer = (RangerLinkPacket *)addr;
    addr += work->sendBufferSize;
    RangerLink_GetTransfer()->recvBuffer = (RangerLinkPacket *)addr;
    WM_GetNextTgid();
}

// parent: send the data packet every 60 frames until the children answered
void RangerLink_UpdateSend(void) {
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    u32 size;
    if (transfer->state == 1) {
        transfer->timer--;
        if (transfer->timer != 0) {
            return;
        }
        transfer->state = 0;
    }
    if (transfer->state == 0) {
        size = RANGER_LINK_PACKET_SIZE - RangerLink_GetPacketHeaderSize();
        RangerLink_BuildPacket(transfer->sendBuffer, transfer->dest, size, 3, transfer->command);
        RangerLink_SendData(transfer->sendBuffer, size + RangerLink_GetPacketHeaderSize(), 0xFFFF);
        transfer->state = 1;
        transfer->timer = 60;
    }
}

// child: answer the parent; parent with the list command: notify every connected child
void RangerLink_UpdateCommand(void) {
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    if (transfer->state != 0) {
        return;
    }
    if (transfer->command == RANGER_LINK_COMMAND_LIST) {
        RangerLinkChild *children = RangerLink_GetChildren()->children;
        s32 i;
        for (i = 0; i < RANGER_LINK_MAX_PEERS; i++) {
            if (children[i].listSendsLeft != 0) {
                u16 destBitmap = (u16)(1 << children[i].aid);
                RangerLink_BuildPacket(transfer->sendBuffer, transfer->dest, 0, 0, RANGER_LINK_COMMAND_LIST);
                RangerLink_SendData(transfer->sendBuffer, RangerLink_GetPacketHeaderSize(), destBitmap);
                children[i].listSendsLeft--;
                return;
            }
        }
        transfer->timer--;
        if (transfer->timer == 0) {
            transfer->state = 2;
        }
        return;
    }
    RangerLink_GetPacketHeaderSize();
    RangerLink_GetSendBufferSize();
    {
        u32 sequence = (((u32 *)transfer->recvBuffer)[2] << 16) >> 24;
        if (sequence == 0) {
            if (transfer->retries == 0) {
                transfer->state = 2;
                return;
            }
            transfer->retries--;
        }
        RangerLink_BuildPacket(transfer->sendBuffer, transfer->dest, 0, sequence, transfer->command);
        RangerLink_SendData(transfer->sendBuffer, RangerLink_GetPacketHeaderSize(), 0xFFFF);
    }
}

void RangerLink_UpdateTransfer(void) {
    switch (RangerLink_GetTransfer()->sendMode) {
    case 1:
        RangerLink_UpdateSend();
        break;
    case 2:
        RangerLink_UpdateCommand();
        break;
    }
}

// child mode, every frame
void RangerLink_UpdateChild(void) {
    RangerLinkChildPeers *peers = RangerLink_GetFoundParents();
    switch (RangerLink_GetState()) {
    case RANGER_LINK_STATE_SCANNING:
        if (peers->scanTimer != 0) {
            peers->scanTimer--;
        }
        break;
    case RANGER_LINK_STATE_CHILD_MP:
        RangerLink_UpdateTransfer();
        break;
    }
}

// parent mode, every frame
void RangerLink_UpdateParent(void) {
    if (RangerLink_GetState() == RANGER_LINK_STATE_PARENT_MP) {
        RangerLink_UpdateTransfer();
    }
}

#pragma force_active on

void RangerLink_Update(void) {
    if (RangerLink_GetState() != RANGER_LINK_STATE_ENDING) {
        if (RangerLink_GetRequestedState() == RANGER_LINK_STATE_ENDING) {
            if (RangerLink_GetState() == RANGER_LINK_STATE_SCAN_ENDED) {
                RangerLink_WmReset();
            }
        } else {
            switch (RangerLink_GetMode()) {
            case RANGER_LINK_MODE_CHILD:
                RangerLink_UpdateChild();
                break;
            case RANGER_LINK_MODE_PARENT:
                RangerLink_UpdateParent();
                break;
            default:
                RangerLink_GetWork();
                RangerLink_GetState();
                break;
            }
        }
    }
}

#pragma force_active reset

void RangerLink_StartChild(void) {
    RangerLink_ClearFoundParents();
    RangerLink_SetMode(RANGER_LINK_MODE_CHILD);
    RangerLink_Begin();
}

#pragma force_active on

int RangerLink_CountFoundParents(void) {
    int mode = RangerLink_GetMode();
    int count = 0;
    if (mode == RANGER_LINK_MODE_CHILD) {
        u8 *p = (u8 *)RangerLink_GetFoundParents();
        if (((RangerLinkPhase *)(p + 0x1C3))->scanPhase == 1) {
            u32 i = 0;
            do {
                if (*(int *)p) {
                    count++;
                }
                i++;
                p += FOUND_PARENT_SIZE;
            } while (i < RANGER_LINK_MAX_PEERS);
        }
    }
    return count;
}

BOOL RangerLink_CanSelectParent(int index) {
    if (RangerLink_GetMode() == RANGER_LINK_MODE_CHILD && RangerLink_GetState() == RANGER_LINK_STATE_SCAN_ENDED) {
        u8 *p = (u8 *)RangerLink_GetFoundParents();
        if (((RangerLinkPhase *)(p + 0x1C3))->scanPhase == 1 && *(int *)(p + index * FOUND_PARENT_SIZE)) {
            return TRUE;
        }
    }
    return FALSE;
}

void RangerLink_SelectParent(int index) {
    if (RangerLink_GetMode() == RANGER_LINK_MODE_CHILD && RangerLink_GetState() == RANGER_LINK_STATE_SCAN_ENDED) {
        u8 *p = (u8 *)RangerLink_GetFoundParents();
        if (((RangerLinkPhase *)(p + 0x1C3))->scanPhase == 1 && *(int *)(p + index * FOUND_PARENT_SIZE)) {
            ((RangerLinkPhase *)(p + 0x1C3))->selected = (u8)index;
            *(u16 *)(p + 0x1C0) = 120;
            RangerLink_WmStartScanSelected();
        }
    }
}

int RangerLink_CountActiveChildren(void) {
    int mode = RangerLink_GetMode();
    int count = 0;
    if (mode == RANGER_LINK_MODE_PARENT) {
        u8 *p = (u8 *)RangerLink_GetChildren();
        if (p[0x60] == 2) {
            u32 i = 0;
            do {
                if (*(int *)p && p[10]) {
                    count++;
                }
                i++;
                p += sizeof(RangerLinkChild);
            } while (i < RANGER_LINK_MAX_PEERS);
        }
    }
    return count;
}

#pragma force_active reset

int RangerLink_GetLinkLevel(void) {
    if (RangerLink_GetWork()->finished != FALSE) {
        return 0;
    }
    return WM_GetLinkLevel();
}

int RangerLink_CountPeers(void) {
    switch (RangerLink_GetMode()) {
    case RANGER_LINK_MODE_CHILD:
        return RangerLink_CountFoundParents();
    case RANGER_LINK_MODE_PARENT:
        return RangerLink_CountActiveChildren();
    default:
        return 0;
    }
}

void RangerLink_StartSendList(void) {
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    u8 *gameInfo = (u8 *)RangerLink_GetGameInfo();
    if (RangerLink_GetMode() == RANGER_LINK_MODE_CHILD) {
        transfer->sendMode = 1;
        transfer->state = 0;
        transfer->data = gameInfo + 8;
        transfer->dest = gameInfo + 8;
        transfer->size = 0;
        transfer->sent = 0;
        transfer->timer = 120;
        transfer->retries = 0;
        transfer->command = RANGER_LINK_COMMAND_LIST;
    }
}

void RangerLink_StartSendNames(void) {
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    u8 *gameInfo = (u8 *)RangerLink_GetGameInfo();
    transfer->sendMode = 1;
    transfer->state = 0;
    transfer->data = gameInfo + 8;
    transfer->dest = gameInfo + 8;
    transfer->size = 0x30;
    transfer->sent = 0x30;
    transfer->timer = 120;
    transfer->retries = 0;
    transfer->command = RANGER_LINK_COMMAND_REPLY;
}

void RangerLink_StartCommand(const void *data, int size, int command) {
    RangerLinkTransfer *transfer = RangerLink_GetTransfer();
    if (command <= 0xF0) {
        transfer->sendMode = 2;
        transfer->state = 3;
        transfer->data = data;
        transfer->dest = data;
        transfer->size = size;
        transfer->sent = 0;
        transfer->timer = 120;
        transfer->retries = 0;
        transfer->command = command;
    }
}

BOOL RangerLink_IsSendDone(void) {
    u32 mode = RangerLink_GetMode();
    u32 state;
    if (mode == RANGER_LINK_MODE_CHILD) {
        goto mode_ok;
    }
    if (mode != RANGER_LINK_MODE_PARENT) {
        goto fail;
    }
mode_ok:
    state = RangerLink_GetState();
    if (state == RANGER_LINK_STATE_PARENT_MP) {
        goto state_ok;
    }
    if (state != RANGER_LINK_STATE_CHILD_MP) {
        goto fail;
    }
state_ok:
    if (RangerLink_GetTransfer()->state != 2) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL RangerLink_IsCommandDone(void) {
    u32 mode = RangerLink_GetMode();
    u32 state;
    if (mode == RANGER_LINK_MODE_CHILD) {
        goto mode_ok;
    }
    if (mode != RANGER_LINK_MODE_PARENT) {
        goto fail;
    }
mode_ok:
    state = RangerLink_GetState();
    if (state == RANGER_LINK_STATE_PARENT_MP) {
        goto state_ok;
    }
    if (state != RANGER_LINK_STATE_CHILD_MP) {
        goto fail;
    }
state_ok:
    if (RangerLink_GetTransfer()->state != 2) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL RangerLink_IsCommandAcknowledged(void) {
    u32 mode = RangerLink_GetMode();
    u32 state;
    RangerLinkTransfer *transfer;
    if (mode == RANGER_LINK_MODE_CHILD) {
        goto mode_ok;
    }
    if (mode != RANGER_LINK_MODE_PARENT) {
        goto fail;
    }
mode_ok:
    state = RangerLink_GetState();
    if (state == RANGER_LINK_STATE_PARENT_MP) {
        goto state_ok;
    }
    if (state != RANGER_LINK_STATE_CHILD_MP) {
        goto fail;
    }
state_ok:
    transfer = RangerLink_GetTransfer();
    if (transfer->state != 0) {
        goto fail;
    }
    if ((((u32 *)transfer->recvBuffer)[2] << 16) >> 24) {
        goto fail;
    }
    if (transfer->retries >= 4) {
        goto fail;
    }
    return TRUE;
fail:
    return FALSE;
}

BOOL RangerLink_IsConnected(void) {
    if (RangerLink_GetMode() == RANGER_LINK_MODE_PARENT) {
        RangerLinkChild *child = RangerLink_GetChildren()->children;
        int i;
        for (i = 0; i < RANGER_LINK_MAX_PEERS; i++, child++) {
            if (child->macAddressTail != 0 && child->active != 0) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return RangerLink_IsConnectedToParent();
}

void RangerLink_RequestEnd(void) {
    u32 state;
    RangerLink_SetRequestedState(RANGER_LINK_STATE_ENDING);
    state = RangerLink_GetState();
    switch (state) {
    case RANGER_LINK_STATE_STOPPED:
    case RANGER_LINK_STATE_ENABLED:
        WM_Finish();
        RangerLink_SetFinished(TRUE);
        break;
    case RANGER_LINK_STATE_CONNECTED:
        if (RangerLink_IsConnectedToParent() == FALSE) {
            RangerLinkWork *work = RangerLink_GetWork();
            RangerLink_WmReset();
            work->resetDone = TRUE;
        }
        break;
    }
}

BOOL RangerLink_IsFinished(void) {
    return RangerLink_GetWork()->finished;
}

RangerLinkParentParam *RangerLink_GetParentParam(void) {
    return &gRangerLinkParentParam;
}

RangerLinkWork *RangerLink_GetWork(void) {
    return &gRangerLinkWork;
}

u8 RangerLink_GetState(void) {
    return RangerLink_GetWork()->state;
}

void RangerLink_SetState(u8 state) {
    RangerLink_GetWork()->state = state;
}

u8 RangerLink_GetRequestedState(void) {
    return RangerLink_GetWork()->requestedState;
}

void RangerLink_SetRequestedState(u8 state) {
    RangerLink_GetWork()->requestedState = state;
}

u8 RangerLink_GetMode(void) {
    return RangerLink_GetWork()->mode;
}

void RangerLink_SetMode(u8 mode) {
    RangerLink_GetWork()->mode = mode;
}

int RangerLink_GetPort(void) {
    return RangerLink_GetWork()->port;
}

void RangerLink_SetChannel(u16 channel) {
    RangerLink_GetParentParam()->channel = channel;
}

RangerLinkScanParam *RangerLink_GetScanParam(void) {
    return RangerLink_GetWork()->scanParam;
}

u32 RangerLink_GetSendSize(void) {
    return RangerLink_GetWork()->sendBufferSize;
}

u32 RangerLink_GetRecvSize(void) {
    return RangerLink_GetWork()->recvBufferSize;
}

void *RangerLink_GetSendBuffer(void) {
    return RangerLink_GetWork()->sendBuffer;
}

void *RangerLink_GetRecvBuffer(void) {
    return RangerLink_GetWork()->recvBuffer;
}

void *RangerLink_GetBssDesc(void) {
    return RangerLink_GetWork()->bssDesc;
}

void RangerLink_SetAid(u16 aid) {
    RangerLink_GetWork()->aid = aid;
}

BOOL RangerLink_IsConnectedToParent(void) {
    return RangerLink_GetWork()->connected;
}

void RangerLink_SetConnected(BOOL connected) {
    RangerLink_GetWork()->connected = connected;
}

void RangerLink_SetDisconnected(BOOL disconnected) {
    RangerLink_GetWork()->disconnected = disconnected;
}

void RangerLink_SetFinished(BOOL finished) {
    RangerLink_GetWork()->finished = finished;
}

void *RangerLink_GetPeers(void) {
    return &RangerLink_GetWork()->peers;
}

RangerLinkChildPeers *RangerLink_GetFoundParents(void) {
    return RangerLink_GetPeers();
}

RangerLinkParentPeers *RangerLink_GetChildren(void) {
    return RangerLink_GetPeers();
}

#pragma force_active on

void RangerLink_ClearFoundParents(void) {
    RangerLinkChildPeers *peers = RangerLink_GetFoundParents();
    MI_CpuFill8(peers, 0, sizeof(RangerLinkChildPeers));
    peers->scanTimer = 280;
}

#pragma force_active reset

RangerLinkTransfer *RangerLink_GetTransfer(void) {
    return &RangerLink_GetWork()->transfer;
}

void RangerLink_ResetTransfer(void) {
    RangerLink_GetTransfer()->sendMode = 0;
}

u32 RangerLink_GetGgid(void) {
    return RangerLink_GetWork()->ggid;
}

u8 RangerLink_GetGameCode(void) {
    return *(u32 *)&RangerLink_GetWork()->gameInfo.header.sender;
}

u32 RangerLink_GetVersion(void) {
    return (*(u32 *)&RangerLink_GetWork()->gameInfo.header.sender << 20) >> 28;
}

u32 RangerLink_GetReceiverId(void) {
    return *(u32 *)&RangerLink_GetWork()->gameInfo.header.receiver >> 16;
}

u32 RangerLink_GetGameInfoLength(void) {
    return sizeof(RangerLinkGameInfo);
}

RangerLinkGameInfo *RangerLink_GetGameInfo(void) {
    return &RangerLink_GetWork()->gameInfo;
}

void *RangerLink_GetSsid(void) {
    return &RangerLink_GetWork()->ssid;
}

#pragma force_active on

// bytes 2-5 of a MAC address
u32 RangerLink_ReadMacAddressTail(u8 *macAddress) {
    union {
        u32 value;
        u8 bytes[4];
    } v;

    u32 i;
    u8 *out;
    v.value = 0;
    out = v.bytes;
    for (i = 0; i < 4; i++, out++) {
        *out = macAddress[i + 2];
    }
    return v.value;
}

// bytes 0-1 of a MAC address
s16 RangerLink_ReadMacAddressHead(u8 *macAddress) {
    union {
        s16 value;
        u8 bytes[2];
    } v;

    u32 i;
    u8 *out;
    v.value = 0;
    out = v.bytes;
    for (i = 0; i < 2; i++, out++) {
        *out = macAddress[i];
    }
    return v.value;
}

// splits a number into its low and high 4 decimal digits
void RangerLink_SplitDecimal(u32 value, u32 *low, u32 *high) {
    *low = value % 10000;
    *high = (value / 10000) % 10000;
}

#pragma force_active reset

u32 RangerLink_GetPacketHeaderSize(void) {
    return sizeof(RangerLinkPacket);
}

#pragma force_active on

// odd scramble seed
u32 RangerLink_NewSeed(void) {
    u16 vcount = reg_GX_VCOUNT;
    u64 tick = OS_GetTick();
    u32 x = tick + vcount;
    return (x & ~1) + 1;
}

// XORs the data with an LCG stream, returns the next seed
u32 RangerLink_Scramble(u32 seed, u32 size, u32 *data) {
    u32 i;
    u16 hi, lo;
    for (i = 0; i < size / 4; i++) {
        seed = seed * 0x5D588B65 + 0x269EC3;
        hi = seed >> 16;
        seed = seed * 0x5D588B65 + 0x269EC3;
        lo = seed >> 16;
        data[i] ^= (hi << 16) | lo;
    }
    return seed;
}

// builds a packet: header, CRC of the payload, payload, then scrambles it if the sender asks for it
void RangerLink_BuildPacket(RangerLinkPacket *packet, const void *data, u32 length, int sequence, int command) {
    RangerLinkHeaderWord *info = (RangerLinkHeaderWord *)RangerLink_GetGameInfo();
    packet->sender.gameCode = info[0].gameCode;
    packet->sender.version = info[0].version;
    packet->sender.encrypted = info[0].encrypted;
    packet->sender.id = (u16)RangerLink_NewSeed();
    packet->receiver.gameCode = info[1].gameCode;
    packet->receiver.version = info[1].version;
    packet->receiver.encrypted = info[1].encrypted;
    packet->receiver.id = 0;
    packet->command = (u8)command;
    packet->sequence = sequence;
    packet->crc = SVC_GetCRC16(0, data, length);
    packet->headerSize = (u8)RangerLink_GetPacketHeaderSize();
    packet->length = length;
    if (length) {
        MI_CpuCopy8(data, (u8 *)packet + packet->headerSize, length);
    }
    RangerLink_DebugPacket(packet);
    if (info[0].encrypted == 1) {
        RangerLink_Scramble(packet->sender.id, length + packet->headerSize - 4, (u32 *)((u8 *)packet + 4));
    }
}

void RangerLink_Unscramble(RangerLinkPacket *packetArg) {
    RangerLinkScrambledPacket *packet = (RangerLinkScrambledPacket *)packetArg;
    u32 seed;
    RangerLink_GetGameInfo();
    if (packet->encrypted == 1) {
        seed = RangerLink_Scramble(packet->seed, sizeof(RangerLinkPacketTail), (u32 *)&packet->tail);
        RangerLink_Scramble(seed, packet->tail.headerSize + packet->tail.length - 16, packet->data);
    }
}

#pragma force_active reset

BOOL RangerLink_CheckCrc(u32 *packet) {
    return (packet[2] >> 16) == SVC_GetCRC16(0, (u8 *)packet + (packet[3] & 0xFF), packet[3] >> 8);
}

// debug hooks, empty in this build
void RangerLink_DebugError() {
}

void RangerLink_DebugState(u16 value) {
}

void RangerLink_DebugScan(void) {
}

void RangerLink_DebugFoundParents(RangerLinkChildPeers *peers) {
}

void RangerLink_DebugChildren(RangerLinkParentPeers *peers) {
}

void RangerLink_DebugPacket(RangerLinkPacket *packet) {
}

BOOL RangerLink_SetMPData(void *data, int size, u32 destBitmap) {
    u16 port = RangerLink_GetPort();
    if (WM_SetMPDataToPortEx(RangerLink_SetMPDataCallback, 0, data, (u16)size, destBitmap, port, 2) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmEnable(void) {
    if (WM_Enable(RangerLink_EnableCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmDisable(void) {
    if (WM_Disable(RangerLink_DisableCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        OS_Terminate();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmMeasureChannel(u32 channel) {
    if (WM_MeasureChannel((RangerLinkWmCallback)RangerLink_MeasureChannelCallback, 3, 0x11, (u16)channel, 30) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmSetParentParameter(void) {
    RangerLinkParentParam *param = RangerLink_GetParentParam();
    RangerLinkGameInfo *gameInfo = RangerLink_GetGameInfo();
    RangerLinkParentPeers *peers = RangerLink_GetChildren();
    param->channel = peers->quietestChannel;
    param->gameInfo = gameInfo;
    param->gameInfoLength = RangerLink_GetGameInfoLength();
    if (WM_SetParentParameter(RangerLink_SetParentParameterCallback, param) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmStartParent(void) {
    if (WM_StartParent(RangerLink_StartParentCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmStartScan(void) {
    if (WM_StartScan(RangerLink_ScanCallback, RangerLink_GetScanParam()) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

// scan for the selected parent only
BOOL RangerLink_WmStartScanSelected(void) {
    RangerLinkScanParam *scanParam = RangerLink_GetScanParam();
    volatile u8 *source = (volatile u8 *)RangerLink_GetFoundParents();
    scanParam->bssid[0] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 0x34];
    scanParam->bssid[1] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 0x35];
    scanParam->bssid[2] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 0];
    scanParam->bssid[3] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 1];
    scanParam->bssid[4] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 2];
    scanParam->bssid[5] = source[(((u32)source[0x1C3] << 28) >> 28) * FOUND_PARENT_SIZE + 3];
    if (WM_StartScan(RangerLink_ScanSelectedCallback, RangerLink_GetScanParam()) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmEndScan(void) {
    if (WM_EndScan(RangerLink_EndScanCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmStartConnect(void) {
    void *ssid = RangerLink_GetSsid();
    void *bssDesc = RangerLink_GetBssDesc();
    if (WM_StartConnectEx(RangerLink_StartConnectCallback, bssDesc, ssid, 1, 0) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmStartMP(void) {
    int mode;
    void *recvBuffer, *sendBuffer;
    int recvSize;
    if (WM_SetPortCallback(4, RangerLink_PortCallback, 0) != WM_OK) {
        RangerLink_DebugError();
        return FALSE;
    }
    mode = RangerLink_GetParentParam()->noFixedFrequency == 0;
    recvBuffer = RangerLink_GetRecvBuffer();
    recvSize = RangerLink_GetRecvSize();
    sendBuffer = RangerLink_GetSendBuffer();
    if (WM_StartMP(RangerLink_StartMPCallback, recvBuffer, (u16)recvSize, sendBuffer, (u16)RangerLink_GetSendSize(), (u16)mode) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmPowerOn(void) {
    if (WM_PowerOn(RangerLink_PowerOnCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmPowerOff(void) {
    if (WM_PowerOff(RangerLink_PowerOffCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        OS_Terminate();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmReset(void) {
    if (WM_Reset(RangerLink_ResetCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        OS_Terminate();
        return FALSE;
    }
    return TRUE;
}

BOOL RangerLink_WmEnd(void) {
    if (WM_End(RangerLink_EndCallback) != WM_OPERATING) {
        RangerLink_DebugError();
        OS_Terminate();
        return FALSE;
    }
    return TRUE;
}
