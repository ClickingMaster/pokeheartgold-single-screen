#include "wireless_manager.h"

#include <stddef.h>
#include <string.h>

#include "global.h"

#include "WM.h"

// Local wireless manager (ARM9 static 0x02032844-0x02033AE0): drives the WM library for the communication layer.
// A parent sets its parent parameters, starts the parent and MP communication and accepts or refuses the
// children that connect; a child scans for a parent (all allowed channels in turn, or one), connects to it and
// starts MP communication. Every request is asynchronous: the state is WIRELESS_STATE_BUSY until its callback
// arrives. Errors of the library are kept in error and move the state to WIRELESS_STATE_ERROR (recoverable by a
// reset) or WIRELESS_STATE_FATAL.

#define WIRELESS_STATE_STOPPED  0
#define WIRELESS_STATE_IDLE     1
#define WIRELESS_STATE_SCANNING 2
#define WIRELESS_STATE_BUSY     3
#define WIRELESS_STATE_PARENT   4 // MP communication running
#define WIRELESS_STATE_5        5
#define WIRELESS_STATE_CHILD    6
#define WIRELESS_STATE_MEASURED 7 // all channels measured, Wireless_SelectChannel picks one
#define WIRELESS_STATE_8        8
#define WIRELESS_STATE_ERROR    9
#define WIRELESS_STATE_FATAL    10

#define WM_OK 2 // asynchronous request accepted

typedef void (*WMCallback)(void *arg);
typedef void (*WirelessSentFunc)(BOOL success);
typedef void (*WirelessReceiveFunc)(u16 aid, void *data, u16 length);

// Common head of the WM callback arguments.
typedef struct WirelessEvent {
    u16 apiid;
    u16 errcode;
    u16 state;
} WirelessEvent;

// WM_StartParent callback argument.
typedef struct WirelessParentEvent {
    u16 apiid;
    u16 errcode;
    u8 unk_04[4];
    u16 state; // 0x08: 0 parent started, 2 beacon sent, 7 child connected, 9 child disconnected
    u8 unk_0A[6];
    u16 aid; // 0x10
    u8 unk_12[2];
    u8 connectData[4]; // 0x14: the child's WM_StartConnectEx data
} WirelessParentEvent;

// WM_StartConnectEx callback argument.
typedef struct WirelessConnectEvent {
    u16 apiid;
    u16 errcode;
    u8 unk_04[4];
    u16 state; // 0x08: 7 connected, 9 disconnected
    u16 aid;   // 0x0A
} WirelessConnectEvent;

// Port callback argument (WM_SetPortCallback).
typedef struct WirelessPortEvent {
    u16 apiid;
    u16 errcode;
    u16 state; // 0x04: 9 disconnected, 0x15 data received, 0x19 callback set
    u8 unk_06[6];
    void *data; // 0x0C
    u16 length; // 0x10
    u16 aid;    // 0x12: sender
} WirelessPortEvent;

// WM_SetMPDataToPortEx callback argument.
typedef struct WirelessSendEvent {
    u16 apiid;
    u16 errcode;
    u8 unk_04[0x1C];
    WirelessSentFunc callback; // 0x20: the request's argument
} WirelessSendEvent;

// Parent parameters handed to WM_SetParentParameter (0x40 bytes).
typedef struct WirelessParentParam {
    void *gameInfo;     // 0x00
    u16 gameInfoLength; // 0x04
    u16 unk_06;
    u32 ggid;      // 0x08
    u16 tgid;      // 0x0C
    u16 entryFlag; // 0x0E
    u16 maxEntry;  // 0x10
    u16 unk_12;
    u16 keySharing; // 0x14: 1 in mode 2
    u16 unk_16;
    u16 beaconPeriod; // 0x18
    u8 unk_1A[0x18];
    u16 channel;       // 0x32
    u16 parentMaxSize; // 0x34
    u16 childMaxSize;  // 0x36
    u8 unk_38[8];
} WirelessParentParam;

// WM_StartScan parameters.
typedef struct WirelessScanParam {
    void *buffer; // 0x12E0: parentInfo
    u16 channel;  // 0x12E4
    u16 time;     // 0x12E6
    u16 bssid[3]; // 0x12E8: address of the parent looked for
} WirelessScanParam;

// The work buffer (Wireless_Init aligns it to 32 bytes; Wireless_GetWorkSize).
typedef struct WirelessWork {
    WirelessParentParam parentParam; // 0x0000
    u8 systemBuffer[0xF00];          // 0x0040: WM_Initialize
    u8 sendBuffer[0xE0];             // 0x0F40: MP send buffer
    u8 receiveBuffer[0x200];         // 0x1020: MP receive buffer
    u8 parentInfo[0xC0];             // 0x1220: the parent found by the scan / connected to
    WirelessScanParam scanParam;     // 0x12E0
    u8 unk_12F0[0x10];
    void (*parentFoundCallback)(void *parentInfo); // 0x1300
    u32 sendBufferSize;                            // 0x1304
    u32 receiveBufferSize;                         // 0x1308
    u16 scanChannel;                               // 0x130C: 0: every allowed channel in turn
    u16 connectAfterScan;                          // 0x130E
    u32 state;                                     // 0x1310: WIRELESS_STATE_*
    u32 mode;                                      // 0x1314: 0, 2, 4 parent; 1, 3, 5 child
    WirelessReceiveFunc receiveCallback;           // 0x1318
    u32 unk_131C;
    void (*beaconCallback)(u32 ggid, u8 info); // 0x1320: every beacon seen while scanning
    void (*childLeftCallback)(u16 aid);        // 0x1324
    void (*childJoinedCallback)(u16 aid);      // 0x1328
    u16 aid;                                   // 0x132C: own AID (0: parent)
    u16 connectedBitmap;                       // 0x132E: bit per connected AID, the parent included
    u32 error;                                 // 0x1330
    u8 maxEntries;                             // 0x1334
    u8 connectBlocked;                         // 0x1335
    u8 unk_1336[2];
    u32 random;        // 0x1338
    u16 channel;       // 0x133C
    u16 minBusyRatio;  // 0x133E
    u16 quietChannels; // 0x1340: channels with minBusyRatio
    u8 entryLocked;    // 0x1342
    u8 entryClosed;    // 0x1343
    u8 entrySet;       // 0x1344
    u8 beaconCount;    // 0x1345
    u8 unk_1346[0x3A];
} WirelessWork;

typedef char WirelessWorkSizeCheck[sizeof(WirelessWork) == 0x1380 ? 1 : -1];
typedef char WirelessWorkStateCheck[offsetof(WirelessWork, state) == 0x1310 ? 1 : -1];
typedef char WirelessWorkBeaconCheck[offsetof(WirelessWork, beaconCount) == 0x1345 ? 1 : -1];

typedef struct WirelessManager {
    u32 unk_0;
    WirelessWork *work;
} WirelessManager;

extern WirelessManager gWirelessManager;
extern const u8 gWirelessConnectTag[3];

int WM_Initialize(void *work, WMCallback callback, int dma);
int WM_InitializeForListening(void *work, WMCallback callback, int dma, int flags);
int WM_SetIndCallback(WMCallback callback);
int WM_SetLifeTime(void (*callback)(void), u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);
int WM_SetParentParameter(WMCallback callback, void *param);
int WM_StartParent(WMCallback callback);
int WM_EndParent(WMCallback callback);
int WM_StartScan(WMCallback callback, void *param);
int WM_EndScan(WMCallback callback);
int WM_StartConnectEx(WMCallback callback, void *bssDesc, void *ssid, int powerSave, int authMode);
int WM_Disconnect(WMCallback callback, u16 aid);
int WM_StartMP(WMCallback callback, void *recvBuf, int recvSize, void *sendBuf, int sendSize, int frequency);
int WM_EndMP(WMCallback callback);
int WM_SetMPDataToPortEx(WMCallback callback, u32 arg, const void *data, u32 length, u16 aidBitmap, u16 port, u32 priority);
int WM_SetPortCallback(u16 port, WMCallback callback, void *arg);
int WM_Reset(WMCallback callback);
int WM_End(WMCallback callback);
int WM_SetGameInfo(void *callback, const void *data, u16 length, u32 ggid, u16 tgid, u32 attr);
int WM_SetEntry(WMCallback callback, BOOL enabled);
int WM_MeasureChannel(WMCallback callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
u16 WM_GetAllowedChannel(void);
u16 WM_GetDispersionScanPeriod(void);
void *WMi_GetStatusAddress(void);

int CommState_GetLinkMode(void);
int CommState_ReportsErrors(void);
void CommState_SetError(int);
BOOL Comm_IsCommClubLinkMode(int);

void Wireless_SetState(int state);
void Wireless_SetError(int error);
BOOL Wireless_SendParentParameter(void);
void Wireless_OnParentParameterSet(void *arg);
BOOL Wireless_StartParentRun(void);
void Wireless_OnParentEvent(void *arg);
BOOL Wireless_StartParentMP(void);
void Wireless_OnParentMPEvent(void *arg);
BOOL Wireless_EndParent(void);
void Wireless_OnParentMPEnded(void *arg);
BOOL Wireless_EndParentRun(void);
void Wireless_OnParentEnded(void *arg);
BOOL Wireless_ScanAndConnect(int mode, const u16 *bssid, u16 channel);
BOOL Wireless_StartScan(void *callback, const u16 *bssid, u16 channel);
BOOL Wireless_ScanNextChannel(void);
void Wireless_OnScanEvent(void *arg);
BOOL Wireless_StopScan(void);
BOOL Wireless_EndScan(void);
void Wireless_OnScanEnded(void *arg);
BOOL Wireless_StartConnect(void);
void Wireless_OnConnectEvent(void *arg);
BOOL Wireless_StartChildMP(void);
void Wireless_OnChildMPEvent(void *arg);
BOOL Wireless_EndChild(void);
void Wireless_OnChildMPEnded(void *arg);
BOOL Wireless_Disconnect(void);
void Wireless_OnDisconnected(void *arg);
BOOL Wireless_Reset(void);
void Wireless_OnReset(void *arg);
BOOL Wireless_SetSendData(const void *data, u32 length, int port, WirelessSentFunc callback);
void Wireless_OnDataSent(void *arg);
void Wireless_OnPortEvent(void *arg);
void Wireless_OnEnded(void *arg);
void Wireless_SetGgid(u32 ggid);
void Wireless_SetParentGameInfo(void *gameInfo, u16 length);
u16 Wireless_GetConnectedBitmap(void);
u16 Wireless_CountConnected(void);
u32 Wireless_GetState(void);
u32 Wireless_GetError(void);
BOOL Wireless_StartMeasureChannels(void);
u16 Wireless_MeasureFromChannel(u16 channel);
void Wireless_OnChannelMeasured(void *arg);
int Wireless_MeasureChannel(WMCallback callback, u16 channel);
u16 Wireless_SelectChannel(void);
s16 Wireless_PickChannel(u16 channels);
BOOL Wireless_Init(void *buffer, int mode);
u32 Wireless_GetWorkSize(void);
void Wireless_OnIndication(void *arg);
BOOL Wireless_Initialize(int mode);
void Wireless_OnInitialized(void *arg);
void Wireless_OnLifeTimeSet(void);
BOOL Wireless_StartParent(int mode, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, u32 entryFlag);
BOOL Wireless_StartChild(int mode, const void *parentInfo);
void Wireless_SetReceiveCallback(void *callback, int port);
BOOL Wireless_SendData(const void *data, u32 length, int port, WirelessSentFunc callback);
void Wireless_ForceReset(void);
void Wireless_Stop(void);
BOOL Wireless_End(void);
u16 Wireless_GetAid(void);
void Wireless_SetMaxEntries(u8 maxEntries);
BOOL Wireless_IsIdle(void);
BOOL Wireless_IsBusy(void);
BOOL Wireless_HasError(void);
BOOL Wireless_HasFatalError(void);
BOOL Wireless_IsScanning(void);
void Wireless_UpdateGameInfo(const void *gameInfo, int length, u32 ggid, int tgid);
void Wireless_OnEntrySet(void *arg);
BOOL Wireless_SetEntry(BOOL enabled);
BOOL Wireless_HasSentBeacons(void);
void Wireless_ClearBeaconCount(void);
void Wireless_SetBeaconCallback(void *callback);
void Wireless_SetChildJoinedCallback(void *callback);
void Wireless_SetEntryLocked(u8 locked);
u8 Wireless_IsEntryLocked(void);
void Wireless_SetEntryClosed(u8 closed);

void Wireless_SetState(int state) {
    gWirelessManager.work->state = state;
}

void Wireless_SetError(int error) {
    int state = gWirelessManager.work->state;
    if ((u32)(state - WIRELESS_STATE_ERROR) > 1) {
        gWirelessManager.work->error = error;
    }
}

BOOL Wireless_SendParentParameter(void) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    result = WM_SetParentParameter(Wireless_OnParentParameterSet, gWirelessManager.work);
    if (result != WM_OK) {
        Wireless_SetError(result);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnParentParameterSet(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        Wireless_SetState(WIRELESS_STATE_ERROR);
    } else if (!Wireless_StartParentRun()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
    }
}

BOOL Wireless_StartParentRun(void) {
    u8 *status;
    int result;
    if ((u32)(gWirelessManager.work->state - WIRELESS_STATE_PARENT) <= 2) {
        return TRUE;
    }
    status = WMi_GetStatusAddress();
    DC_InvalidateRange(status + 0x198, 4);
    *(u32 *)(status + 0x198) = 0;
    DC_FlushRange(status + 0x198, 4);
    result = WM_StartParent(Wireless_OnParentEvent);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    gWirelessManager.work->aid = 0;
    gWirelessManager.work->connectedBitmap = 1;
    return TRUE;
}

#pragma force_active on

// A child connecting is refused when entry is locked or closed, the parent is full, or its connect data
// (CommState_GetLinkMode(), gWirelessConnectTag) differ from ours.
void Wireless_OnParentEvent(void *arg) {
    WirelessParentEvent *event = arg;
    u16 mask = 1 << event->aid;
    if (event->errcode) {
        Wireless_SetError(event->errcode);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    switch (event->state) {
    case 2:
        gWirelessManager.work->beaconCount++;
        break;
    case 7:
        if (gWirelessManager.work->entryClosed == 1 || gWirelessManager.work->entryLocked == 1 || Wireless_CountConnected() >= gWirelessManager.work->maxEntries || event->connectData[0] != CommState_GetLinkMode() || memcmp(gWirelessConnectTag, &event->connectData[1], 3) != 0) {
            int result = WM_Disconnect(NULL, event->aid);
            if (result != WM_OK) {
                Wireless_SetError(result);
                Wireless_SetState(WIRELESS_STATE_ERROR);
            }
        } else {
            gWirelessManager.work->connectedBitmap |= mask;
            if (gWirelessManager.work->childJoinedCallback) {
                gWirelessManager.work->childJoinedCallback(event->aid);
            }
        }
        break;
    case 9:
        gWirelessManager.work->connectedBitmap &= ~mask;
        if (gWirelessManager.work->childLeftCallback) {
            gWirelessManager.work->childLeftCallback(event->aid);
        }
        break;
    case 0:
        if (!Wireless_StartParentMP()) {
            Wireless_SetState(WIRELESS_STATE_ERROR);
        }
        break;
    case 26:
        break;
    }
}

#pragma force_active off

BOOL Wireless_StartParentMP(void) {
    WirelessWork *work;
    int result;

    if ((u32)(gWirelessManager.work->state - WIRELESS_STATE_PARENT) <= 2) {
        return TRUE;
    }
    Wireless_SetState(WIRELESS_STATE_PARENT);
    work = gWirelessManager.work;
    result = WM_StartMP(Wireless_OnParentMPEvent,
        work->receiveBuffer,
        (u16)work->receiveBufferSize,
        work->sendBuffer,
        (u16)work->sendBufferSize,
        1);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnParentMPEvent(void *arg) {
    WirelessEvent *event = arg;
    u16 error = event->errcode;
    u16 state;

    if (error != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(error);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    state = event->state;
    switch (state) {
    case 10:
        if (gWirelessManager.work->mode == 2) {
            if (gWirelessManager.work->state != WIRELESS_STATE_PARENT) {
                if (gWirelessManager.work->state == WIRELESS_STATE_CHILD) {
                    break;
                }
            }
        }
        Wireless_SetState(WIRELESS_STATE_PARENT);
        break;
    case 11:
    case 12:
    case 13:
    default:
        break;
    }
}

BOOL Wireless_EndParent(void) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    result = WM_EndMP(Wireless_OnParentMPEnded);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnParentMPEnded(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        Wireless_ForceReset();
    } else if (!Wireless_EndParentRun()) {
        Wireless_ForceReset();
    }
}

BOOL Wireless_EndParentRun(void) {
    int result = WM_EndParent(Wireless_OnParentEnded);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnParentEnded(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
    } else {
        Wireless_SetState(WIRELESS_STATE_IDLE);
    }
}

// Scans for the parent with address bssid (on channel, or every allowed channel when 0), then connects to it.
BOOL Wireless_ScanAndConnect(int mode, const u16 *bssid, u16 channel) {
    gWirelessManager.work->receiveBufferSize = 0x200;
    gWirelessManager.work->sendBufferSize = 0x40;
    Wireless_SetState(WIRELESS_STATE_SCANNING);
    *(u16 *)(gWirelessManager.work->parentInfo + 0x36) = 1;
    gWirelessManager.work->scanParam.bssid[2] = bssid[2];
    gWirelessManager.work->scanParam.bssid[1] = bssid[1];
    gWirelessManager.work->scanParam.bssid[0] = bssid[0];
    gWirelessManager.work->mode = mode;
    gWirelessManager.work->parentFoundCallback = NULL;
    gWirelessManager.work->scanChannel = channel;
    gWirelessManager.work->scanParam.channel = 0;
    gWirelessManager.work->connectAfterScan = 1;
    if (!Wireless_ScanNextChannel()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// Scans for parents; callback gets each one found.
BOOL Wireless_StartScan(void *callback, const u16 *bssid, u16 channel) {
    Wireless_SetState(WIRELESS_STATE_SCANNING);
    gWirelessManager.work->parentFoundCallback = callback;
    gWirelessManager.work->scanChannel = channel;
    gWirelessManager.work->scanParam.channel = 0;
    gWirelessManager.work->connectAfterScan = 0;
    gWirelessManager.work->scanParam.bssid[2] = bssid[2];
    gWirelessManager.work->scanParam.bssid[1] = bssid[1];
    gWirelessManager.work->scanParam.bssid[0] = bssid[0];
    if (!Wireless_ScanNextChannel()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on
#pragma optimization_level 2
#pragma opt_common_subs off

BOOL Wireless_ScanNextChannel(void) {
    u16 channels = WM_GetAllowedChannel();
    if (channels == 0x8000) {
        Wireless_SetError(3);
        CommState_SetError(1);
        return FALSE;
    }
    if (channels == 0) {
        Wireless_SetError(22);
        CommState_SetError(1);
        return FALSE;
    }
    if (gWirelessManager.work->scanChannel == 0) {
        for (;;) {
            gWirelessManager.work->scanParam.channel++;
            if (gWirelessManager.work->scanParam.channel > 16) {
                gWirelessManager.work->scanParam.channel = 1;
            }
            if (channels & (1u << (gWirelessManager.work->scanParam.channel - 1))) {
                goto channelSelected;
            } else {
                continue;
            }
        }
    } else {
        gWirelessManager.work->scanParam.channel = gWirelessManager.work->scanChannel;
    }
channelSelected:
    gWirelessManager.work->scanParam.time = WM_GetDispersionScanPeriod() / 3;
    gWirelessManager.work->scanParam.buffer = gWirelessManager.work->parentInfo;
    {
        int error = WM_StartScan(Wireless_OnScanEvent, &gWirelessManager.work->scanParam);
        if (error != WM_OK) {
            Wireless_SetError(error);
            return FALSE;
        }
    }
    return TRUE;
}

#pragma opt_common_subs reset
#pragma optimization_level reset

// WM_StartScan callback argument.
typedef struct WirelessScanEvent {
    u16 apiid;
    u16 errcode;
    u8 unk_04[4];
    u16 state; // 0x08: 3 nothing found, 4 channel done, 5 parent found
    u8 unk_0A[0x2C];
    u16 gameInfoLength; // 0x36
    u8 unk_38[4];
    u32 ggid; // 0x3C
    u8 unk_40[3];
    u8 attribute; // 0x43
    u8 unk_44[4];
    u8 gameInfo[5]; // 0x48
} WirelessScanEvent;

#pragma opt_propagation off

void Wireless_OnScanEvent(void *input) {
    WirelessScanEvent *event = input;
    WirelessWork *work;
    u16 errcode = event->errcode;

    if (errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(errcode);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    work = gWirelessManager.work;
    if (work->state != WIRELESS_STATE_SCANNING) {
        work->connectAfterScan = 0;
        if (!Wireless_EndScan()) {
            Wireless_SetState(WIRELESS_STATE_ERROR);
        }
        return;
    }
    switch (event->state) {
    case 3:
        return;
    case 5:
        DC_InvalidateRange(work->parentInfo, 0xC0);
        work = gWirelessManager.work;
        if (work->beaconCallback != NULL && event->gameInfoLength >= 8) {
            u8 *gameInfo = event->gameInfo;
            work->beaconCallback(event->ggid, gameInfo[4]);
        }
        if (event->gameInfoLength >= 8) {
            work = gWirelessManager.work;
            if (event->ggid == work->parentParam.ggid && (event->attribute & 3) == 1) {
                if (work->parentFoundCallback != NULL) {
                    work->parentFoundCallback(work->parentInfo);
                }
                work = gWirelessManager.work;
                if (work->connectAfterScan != 0) {
                    if (!Wireless_EndScan()) {
                        Wireless_SetState(WIRELESS_STATE_ERROR);
                    }
                    return;
                }
            }
        }
        break;
    case 4:
    default:
        break;
    }
    if (!Wireless_ScanNextChannel()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
    }
}

#pragma opt_propagation reset
#pragma force_active off

// The scan callback ends the scan once the state left WIRELESS_STATE_SCANNING.
BOOL Wireless_StopScan(void) {
    if (gWirelessManager.work->state != WIRELESS_STATE_SCANNING) {
        return FALSE;
    }
    Wireless_SetState(WIRELESS_STATE_BUSY);
    return TRUE;
}

BOOL Wireless_EndScan(void) {
    int result = WM_EndScan(Wireless_OnScanEnded);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnScanEnded(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        return;
    }
    Wireless_SetState(WIRELESS_STATE_IDLE);
    if (gWirelessManager.work->connectAfterScan != 0 && !Wireless_StartConnect()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
    }
}

// Connects to the parent in parentInfo; the connect data are CommState_GetLinkMode() and gWirelessConnectTag.
BOOL Wireless_StartConnect(void) {
    u8 data[0x20];
    int result;
    if ((u32)(gWirelessManager.work->state - WIRELESS_STATE_PARENT) <= 2) {
        return TRUE;
    }
    Wireless_SetState(WIRELESS_STATE_BUSY);
    MI_CpuCopy8(gWirelessConnectTag, data + 1, 3);
    data[0] = CommState_GetLinkMode();
    result = WM_StartConnectEx(Wireless_OnConnectEvent, gWirelessManager.work->parentInfo, data, 1, 0);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnConnectEvent(void *arg) {
    WirelessConnectEvent *event = arg;
    u16 code;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        if (event->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            Wireless_SetState(WIRELESS_STATE_ERROR);
            return;
        }
        if (event->errcode == WM_ERRCODE_NO_ENTRY) {
            Wireless_SetState(WIRELESS_STATE_ERROR);
            return;
        }
        if (event->errcode == WM_ERRCODE_FAILED) {
            if (CommState_ReportsErrors()) {
                Wireless_SetState(WIRELESS_STATE_ERROR);
                return;
            }
            Wireless_SetState(WIRELESS_STATE_8);
            return;
        }
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    code = event->state;
    if (code == 8) {
        return;
    }
    if (code == 7) {
        WirelessWork *work = gWirelessManager.work;
        if (work->connectBlocked) {
            Wireless_SetError(20);
            Wireless_SetState(WIRELESS_STATE_ERROR);
            return;
        }
        Wireless_SetState(WIRELESS_STATE_PARENT);
        if (!Wireless_StartChildMP()) {
            Wireless_SetState(WIRELESS_STATE_BUSY);
            return;
        }
        gWirelessManager.work->aid = event->aid;
        return;
    }
    if (code == 6) {
        return;
    }
    if (code == 9) {
        Wireless_SetError(20);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    if (code == 26) {
        return;
    }
    Wireless_SetState(WIRELESS_STATE_ERROR);
}

BOOL Wireless_StartChildMP(void) {
    WirelessWork *work = gWirelessManager.work;
    int result = WM_StartMP(Wireless_OnChildMPEvent, work->receiveBuffer, (u16)work->receiveBufferSize, work->sendBuffer, (u16)work->sendBufferSize, 1);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnChildMPEvent(void *arg) {
    WirelessEvent *event = arg;
    u16 error = event->errcode;
    u16 state;
    if (error != WM_ERRCODE_SUCCESS) {
        if (error == WM_ERRCODE_SEND_FAILED || error == WM_ERRCODE_TIMEOUT || error == WM_ERRCODE_INVALID_POLLBITMAP) {
            return;
        }
        Wireless_SetError(error);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return;
    }
    state = event->state;
    switch (state) {
    case 10:
        if (gWirelessManager.work->mode != 3 || gWirelessManager.work->state != WIRELESS_STATE_CHILD) {
            Wireless_SetState(WIRELESS_STATE_PARENT);
        }
        break;
    case 11:
    case 12:
    case 13:
    default:
        break;
    }
}

BOOL Wireless_EndChild(void) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    result = WM_EndMP(Wireless_OnChildMPEnded);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnChildMPEnded(void *arg) {
    WirelessEvent *event = arg;
    u16 error = event->errcode;
    if (error != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(error);
        Wireless_Stop();
    } else if (!Wireless_Disconnect()) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
    }
}

BOOL Wireless_Disconnect(void) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    result = WM_Disconnect(Wireless_OnDisconnected, 0);
    if (result != WM_OK) {
        Wireless_SetError(result);
        Wireless_ForceReset();
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnDisconnected(void *arg) {
    WirelessEvent *event = arg;
    u16 error = event->errcode;
    if (error != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(error);
    } else {
        Wireless_SetState(WIRELESS_STATE_IDLE);
    }
}

BOOL Wireless_Reset(void) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    result = WM_Reset(Wireless_OnReset);
    if (result != WM_OK) {
        Wireless_SetError(result);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnReset(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        Wireless_SetError(event->errcode);
    } else {
        Wireless_SetState(WIRELESS_STATE_IDLE);
    }
}

// Sends data to every AID on port; callback (the request's argument) learns whether it was sent.
BOOL Wireless_SetSendData(const void *data, u32 length, int port, WirelessSentFunc callback) {
    int result;
    DC_FlushRange(gWirelessManager.work->sendBuffer, gWirelessManager.work->sendBufferSize);
    result = WM_SetMPDataToPortEx(Wireless_OnDataSent, (u32)callback, data, length, 0xFFFF, port, 2);
    if (result == WM_OK) {
        return TRUE;
    }
    return FALSE;
}

void Wireless_OnDataSent(void *arg) {
    WirelessSendEvent *event = arg;
    u16 error = event->errcode;
    if (error != WM_ERRCODE_SUCCESS && error != WM_ERRCODE_SEND_FAILED) {
        Wireless_SetError(error);
        return;
    }
    if (event->callback != NULL) {
        (event->callback)(error == WM_ERRCODE_SUCCESS);
    }
}

void Wireless_OnPortEvent(void *arg) {
    WirelessPortEvent *event = arg;
    u16 error = event->errcode;
    WirelessReceiveFunc callback;
    if (error != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(error);
        return;
    }
    callback = gWirelessManager.work->receiveCallback;
    if (callback == NULL) {
        return;
    }
    if (event->state == 0x19) {
        return;
    }
    if (event->state == 0x15) {
        callback(event->aid, event->data, event->length);
    } else if (event->state == 9) {
        callback(event->aid, NULL, 0);
    }
}

void WVR_TerminateAsync(void (*callback)(void *), void *arg);

void Wireless_OnEnded(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetState(WIRELESS_STATE_FATAL);
    } else {
        WVR_TerminateAsync(NULL, NULL);
        Wireless_SetState(WIRELESS_STATE_STOPPED);
    }
}

void Wireless_SetGgid(u32 ggid) {
    gWirelessManager.work->parentParam.ggid = ggid;
}

void Wireless_SetParentGameInfo(void *gameInfo, u16 length) {
    gWirelessManager.work->parentParam.gameInfo = gameInfo;
    gWirelessManager.work->parentParam.gameInfoLength = length;
}

u16 Wireless_GetConnectedBitmap(void) {
    return gWirelessManager.work->connectedBitmap;
}

u16 Wireless_CountConnected(void) {
    int count = 0;
    int i = count;
    u16 bits = gWirelessManager.work->connectedBitmap;
    for (; i < 16; i++) {
        if (bits & 1) {
            count++;
        }
        bits >>= 1;
    }
    return count;
}

u32 Wireless_GetState(void) {
    return gWirelessManager.work->state;
}

u32 Wireless_GetError(void) {
    return gWirelessManager.work->error;
}

// WM_MeasureChannel callback argument.
typedef struct WirelessMeasureEvent {
    u16 apiid;
    u16 errcode;
    u16 unk_04;
    u16 unk_06;
    u16 channel;
    u16 busyRatio;
} WirelessMeasureEvent;

// Measures how busy every allowed channel is (WIRELESS_STATE_MEASURED when done).
BOOL Wireless_StartMeasureChannels(void) {
    u8 macAddress[6];
    u32 seed;
    u16 ret;

    OS_GetMacAddress(macAddress);
    seed = *(u32 *)0x027FFC3C;
    seed = ((u16 *)macAddress)[2] + (((u16 *)macAddress)[1] + (((u16 *)macAddress)[0] + seed));
    gWirelessManager.work->random = seed;
    gWirelessManager.work->random = gWirelessManager.work->random * 0x10DCD + 0x3039;
    gWirelessManager.work->channel = 0;
    gWirelessManager.work->minBusyRatio = 101;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    ret = Wireless_MeasureFromChannel(1);
    if (ret == 0x18) {
        Wireless_SetError(0x18);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        CommState_SetError(1);
        return FALSE;
    }
    if (ret != WM_OK) {
        Wireless_SetError(ret);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

// Measures the first allowed channel from channel on; 0x18 when there is none.
u16 Wireless_MeasureFromChannel(u16 channel) {
    int allowed;

    allowed = WM_GetAllowedChannel();
    if (allowed == 0x8000) {
        Wireless_SetError(3);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        CommState_SetError(1);
        return 3;
    }
    if (allowed == 0) {
        Wireless_SetError(0x16);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        CommState_SetError(1);
        return 0x18;
    }
    while (!((1 << (channel - 1)) & allowed)) {
        channel++;
        if (channel > 16) {
            return 0x18;
        }
    }
    return (u16)Wireless_MeasureChannel(Wireless_OnChannelMeasured, channel);
}

void Wireless_OnChannelMeasured(void *arg) {
    WirelessMeasureEvent *event = arg;
    u16 channel;
    u16 ratio;
    u16 ret;

    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        Wireless_SetState(WIRELESS_STATE_ERROR);
        CommState_SetError(1);
        return;
    }
    channel = event->channel;
    ratio = event->busyRatio;
    if (gWirelessManager.work->minBusyRatio > ratio) {
        gWirelessManager.work->minBusyRatio = ratio;
        gWirelessManager.work->quietChannels = 1 << (channel - 1);
    } else if (gWirelessManager.work->minBusyRatio == ratio) {
        gWirelessManager.work->quietChannels |= 1 << (channel - 1);
    }
    ret = Wireless_MeasureFromChannel((u16)(channel + 1));
    if (ret == 0x18) {
        Wireless_SetState(WIRELESS_STATE_MEASURED);
        return;
    }
    if (ret != WM_OK) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
    }
}

int Wireless_MeasureChannel(WMCallback callback, u16 channel) {
    return WM_MeasureChannel(callback, 3, 0x11, channel, 30);
}

// Picks one of the least busy channels.
u16 Wireless_SelectChannel(void) {
    Wireless_SetState(WIRELESS_STATE_IDLE);
    gWirelessManager.work->channel = Wireless_PickChannel(gWirelessManager.work->quietChannels);
    return gWirelessManager.work->channel;
}

#pragma force_active on

// A random channel of the bitmap channels (1-16).
s16 Wireless_PickChannel(u16 channels) {
    s16 result = 0, i;
    u16 count = 0;
    for (i = 0; i < 16; i++) {
        if (channels & (1 << i)) {
            result = i + 1;
            count++;
        }
    }
    if (count > 1) {
        u16 selected;
        gWirelessManager.work->random = gWirelessManager.work->random * 0x10dcd + 0x3039;
        selected = (count * ((u8)gWirelessManager.work->random)) >> 8;
        for (i = 0; i < 16; i++) {
            if (channels & 1) {
                if (selected == 0) {
                    return i + 1;
                }
                selected--;
            }
            channels >>= 1;
        }
        return 0;
    }
    return result;
}

#pragma force_active off

// buffer: Wireless_GetWorkSize() bytes + 32 for the alignment. mode 1: WM_Initialize, else for listening only.
BOOL Wireless_Init(void *buffer, int mode) {
    u32 p = (u32)buffer;
    u32 mod;

    mod = p << 27;
    mod = mod >> 27;
    if (mod != 0) {
        p += 32 - mod;
    }
    gWirelessManager.work = (WirelessWork *)p;
    gWirelessManager.work->receiveBufferSize = 0;
    gWirelessManager.work->sendBufferSize = 0;
    gWirelessManager.work->receiveCallback = NULL;
    gWirelessManager.work->aid = 0;
    gWirelessManager.work->connectedBitmap = 1;
    gWirelessManager.work->error = 0;
    gWirelessManager.work->state = WIRELESS_STATE_STOPPED;
    gWirelessManager.work->parentParam.gameInfo = NULL;
    gWirelessManager.work->parentParam.gameInfoLength = 0;
    gWirelessManager.work->unk_131C = 0;
    gWirelessManager.work->maxEntries = 8;
    gWirelessManager.work->connectBlocked = 0;
    gWirelessManager.work->entryLocked = 0;
    if (!Wireless_Initialize(mode)) {
        return FALSE;
    }
    return TRUE;
}

u32 Wireless_GetWorkSize(void) {
    return sizeof(WirelessWork);
}

void Wireless_OnIndication(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode == WM_ERRCODE_FIFO_ERROR) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        Wireless_SetError(25);
    }
}

BOOL Wireless_Initialize(int mode) {
    int result;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    if (mode == 1) {
        result = WM_Initialize(gWirelessManager.work->systemBuffer, Wireless_OnInitialized, 2);
    } else {
        result = WM_InitializeForListening(gWirelessManager.work->systemBuffer, Wireless_OnInitialized, 2, 0);
    }
    if (result != WM_OK) {
        Wireless_SetError(result);
        Wireless_SetState(WIRELESS_STATE_FATAL);
        return FALSE;
    }
    return TRUE;
}

void Wireless_OnInitialized(void *arg) {
    WirelessEvent *event = arg;
    int result;
    if (event->errcode != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(event->errcode);
        Wireless_SetState(WIRELESS_STATE_FATAL);
        CommState_SetError(5);
        return;
    }
    result = WM_SetIndCallback(Wireless_OnIndication);
    if (result != WM_ERRCODE_SUCCESS) {
        Wireless_SetError(result);
        Wireless_SetState(WIRELESS_STATE_FATAL);
        CommState_SetError(5);
        return;
    }
    Wireless_SetState(WIRELESS_STATE_IDLE);
}

void Wireless_OnLifeTimeSet(void) {
}

// Starts a parent (mode 0, 2 or 4).
BOOL Wireless_StartParent(int mode, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, u32 entryFlag) {
    int value;
    if (Comm_IsCommClubLinkMode(CommState_GetLinkMode())) {
        WM_SetLifeTime(Wireless_OnLifeTimeSet, 0xFFFF, 100, 5, 100);
    }
    gWirelessManager.work->receiveBufferSize = 0x1C0;
    gWirelessManager.work->sendBufferSize = 0xE0;
    gWirelessManager.work->mode = mode;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    gWirelessManager.work->parentParam.tgid = tgid;
    gWirelessManager.work->parentParam.channel = channel;
    gWirelessManager.work->parentParam.beaconPeriod = beaconPeriod;
    switch (mode) {
    case 0:
        gWirelessManager.work->parentParam.parentMaxSize = 0xC0;
        if (maxEntry >= 5) {
            gWirelessManager.work->parentParam.childMaxSize = 0xC;
        } else {
            gWirelessManager.work->parentParam.childMaxSize = 0x26;
        }
        break;
    case 4:
        gWirelessManager.work->parentParam.parentMaxSize = 0x64;
        gWirelessManager.work->parentParam.childMaxSize = 0xC;
        break;
    }
    gWirelessManager.work->parentParam.maxEntry = maxEntry;
    value = 0;
    gWirelessManager.work->parentParam.unk_16 = value;
    gWirelessManager.work->parentParam.unk_12 = value;
    gWirelessManager.work->parentParam.entryFlag = entryFlag;
    if (mode == 2) {
        value = 1;
    }
    gWirelessManager.work->parentParam.keySharing = value;
    if (mode == 0 || mode == 2 || mode == 4) {
        return Wireless_SendParentParameter();
    }
    return FALSE;
}

// Connects as a child (mode 1, 3 or 5) to the parent described by parentInfo (0xC0 bytes).
BOOL Wireless_StartChild(int mode, const void *parentInfo) {
    if (Comm_IsCommClubLinkMode(CommState_GetLinkMode())) {
        WM_SetLifeTime(Wireless_OnLifeTimeSet, 0xFFFF, 100, 5, 100);
    }
    gWirelessManager.work->receiveBufferSize = 0x200;
    gWirelessManager.work->sendBufferSize = 0x40;
    gWirelessManager.work->mode = mode;
    Wireless_SetState(WIRELESS_STATE_BUSY);
    if (mode == 1 || mode == 3 || mode == 5) {
        MI_CpuCopy8(parentInfo, gWirelessManager.work->parentInfo, 0xC0);
        DC_FlushRange(gWirelessManager.work->parentInfo, 0xC0);
        DC_WaitWriteBufferEmpty();
        return Wireless_StartConnect();
    }
    return FALSE;
}

void Wireless_SetReceiveCallback(void *callback, int port) {
    int result;
    gWirelessManager.work->receiveCallback = callback;
    result = WM_SetPortCallback((u16)port, Wireless_OnPortEvent, NULL);
    if (result != WM_ERRCODE_SUCCESS) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        for (;;) {}
    }
}

// A parent sends only while a child is connected.
BOOL Wireless_SendData(const void *data, u32 length, int port, WirelessSentFunc callback) {
    if (Wireless_GetAid() == 0) {
        if ((Wireless_GetConnectedBitmap() & 0xFE) == 0) {
            return FALSE;
        }
    }
    return Wireless_SetSendData(data, length, port, callback);
}

void Wireless_ForceReset(void) {
    if (gWirelessManager.work->state == WIRELESS_STATE_SCANNING) {
        for (;;) {}
    }
    if (!Wireless_Reset()) {
        Wireless_SetState(WIRELESS_STATE_FATAL);
    }
}

// Ends MP communication and the parent / the connection.
void Wireless_Stop(void) {
    int state = gWirelessManager.work->state;
    if (state == WIRELESS_STATE_IDLE) {
        return;
    }
    if (state != WIRELESS_STATE_CHILD && state != WIRELESS_STATE_5 && state != WIRELESS_STATE_PARENT) {
        Wireless_SetState(WIRELESS_STATE_BUSY);
        Wireless_ForceReset();
        return;
    }
    Wireless_SetState(WIRELESS_STATE_BUSY);
    switch (gWirelessManager.work->mode) {
    case 1:
    case 5:
        if (!Wireless_EndChild()) {
            Wireless_ForceReset();
        }
        break;
    case 0:
    case 4:
        if (!Wireless_EndParent()) {
            Wireless_ForceReset();
        }
        break;
    case 2:
    case 3:
        break;
    }
}

BOOL Wireless_End(void) {
    Wireless_SetState(WIRELESS_STATE_BUSY);
    if (WM_End(Wireless_OnEnded) != WM_OK) {
        Wireless_SetState(WIRELESS_STATE_ERROR);
        return FALSE;
    }
    return TRUE;
}

u16 Wireless_GetAid(void) {
    return gWirelessManager.work->aid;
}

void Wireless_SetMaxEntries(u8 maxEntries) {
    if (gWirelessManager.work != NULL) {
        gWirelessManager.work->maxEntries = maxEntries;
    }
}

BOOL Wireless_IsIdle(void) {
    return gWirelessManager.work->state == WIRELESS_STATE_IDLE;
}

BOOL Wireless_IsBusy(void) {
    return gWirelessManager.work->state == WIRELESS_STATE_BUSY;
}

BOOL Wireless_HasError(void) {
    return gWirelessManager.work->state == WIRELESS_STATE_ERROR;
}

BOOL Wireless_HasFatalError(void) {
    return gWirelessManager.work->state == WIRELESS_STATE_FATAL;
}

BOOL Wireless_IsScanning(void) {
    WirelessWork *work = gWirelessManager.work;
    if (work != NULL) {
        if (work->state == WIRELESS_STATE_SCANNING) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void Wireless_UpdateGameInfo(const void *gameInfo, int length, u32 ggid, int tgid) {
    if (gWirelessManager.work->state == WIRELESS_STATE_PARENT) {
        WM_SetGameInfo(NULL, gameInfo, (u16)length, ggid, (u16)tgid, 1);
    }
}

void Wireless_OnEntrySet(void *arg) {
    WirelessEvent *event = arg;
    if (event->errcode == WM_ERRCODE_SUCCESS) {
        gWirelessManager.work->entrySet = 1;
    }
}

BOOL Wireless_SetEntry(BOOL enabled) {
    gWirelessManager.work->entrySet = 0;
    if (gWirelessManager.work->state == WIRELESS_STATE_PARENT && WM_SetEntry(Wireless_OnEntrySet, enabled) == WM_OK) {
        return TRUE;
    }
    return FALSE;
}

BOOL Wireless_HasSentBeacons(void) {
    WirelessWork *work = gWirelessManager.work;
    if (work != NULL) {
        if (work->beaconCount >= 6) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void Wireless_ClearBeaconCount(void) {
    gWirelessManager.work->beaconCount = 0;
}

void Wireless_SetBeaconCallback(void *callback) {
    gWirelessManager.work->beaconCallback = callback;
}

void Wireless_SetChildJoinedCallback(void *callback) {
    gWirelessManager.work->childJoinedCallback = callback;
}

void Wireless_SetEntryLocked(u8 locked) {
    gWirelessManager.work->entryLocked = locked;
}

u8 Wireless_IsEntryLocked(void) {
    return gWirelessManager.work->entryLocked;
}

void Wireless_SetEntryClosed(u8 closed) {
    gWirelessManager.work->entryClosed = closed;
}
