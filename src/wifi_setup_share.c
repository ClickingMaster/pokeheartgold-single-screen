#pragma force_active on
#include "wifi_setup_share.h"

#include "global.h"

#include "WM.h"
#include "wifi_setup_access_points.h"
#include "wifi_setup_app.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Local wireless session of the access-point setup (see include/wifi_setup_share.h): Download Play
// parent queries, the WM handler (WifiSetupWh_*), the session state machine (WifiSetupShare_*) and
// the mount of the utility archive.

#define WH_LOG_LEVEL 0x8000000

extern char *gWifiSetupWhStateNames[]; // state names, see WIFI_SETUP_WH_STATE_*
extern char gWifiSetupWhFmt_StateFrom[];  // "%s -> "
extern char gWifiSetupWhFmt_StateTo[];  // "%s\n"
extern char gWifiSetupWhStr_ChildConnected[];  // "StartParent - new child (aid %x) connected\n"
extern char gWifiSetupWhStr_ChildDisconnected[];  // "StartParent - child (aid %x) disconnected\n"
extern char gWifiSetupWhStr_UnknownIndicate[];  // "unknown indicate, state = %d\n"
extern char gWifiSetupWhStr_StartParentKeyShareFailed[];  // "DWCi_MOV_WH_StateInStartParentKeyShare failed\n"
extern char gWifiSetupWhStr_EndParentFailed[];  // "DWCi_MOV_WH_StateInEndParent failed\n"
extern char gWifiSetupWhStr_RecvBufferSize[];  // "recv buffer size = %d\n"
extern char gWifiSetupWhStr_SendBufferSize[];  // "send buffer size = %d\n"
extern char gWifiSetupWhStr_ChannelBratio[];  // "channel %d bratio = %x\n"
extern char gWifiSetupWhStr_DecidedChannel[];  // "decided channel = %d\n"
extern char gWifiSetupWhStr_UnknownConnectMode[];  // "unknown connect mode %d\n"
extern char gWifiSetupWhStr_StepNoChild[];  // "DWCi_MOV_WH_StepDataSharing - Warning No Child\n"
extern char gWifiSetupWhStr_StepNoDataSet[];  // "DWCi_MOV_WH_StepDataSharing - Warning No DataSet\n"
extern char gWifiSetupWhStr_AlreadyIdle[];  // "already DWCi_MOV_WH_SYSSTATE_IDLE\n"
extern char gWifiSetupWhStr_Finalize[];  // "DWCi_MOV_WH_Finalize, state = %d\n"
extern char gWifiSetupArchivePath[];  // "rom:/dwc/utility.bin"
extern char gWifiSetupArchivePrefixFormat[];  // "%s:/"
extern char gWifiSetupArchiveName[];  // archive name

extern WifiSetupArchiveState *gWifiSetupArchive;

extern int WM_GetAllowedChannel(void);
extern int WM_SetParentParameter(WifiSetupWhCallback callback, WifiSetupWhParentParam *param);
extern int WM_SetWEPKey(WifiSetupWhCallback callback, int mode, u8 *key);
extern int WM_StartParent(WifiSetupWhCallback callback);
extern int WM_StartMP(WifiSetupWhCallback callback, void *recvBuf, u16 recvBufSize, void *sendBuf, u16 sendBufSize, u16 mpFreq);
extern int WM_StartKeySharing(void *buf, u16 port);
extern int WM_EndKeySharing(void *buf);
extern int WM_EndMP(WifiSetupWhCallback callback);
extern int WM_EndParent(WifiSetupWhCallback callback);
extern int WM_Disconnect(WifiSetupWhCallback callback, u16 aid);
extern int WM_Reset(WifiSetupWhCallback callback);
extern int WM_End(WifiSetupWhCallback callback);
extern int WM_MeasureChannel(WifiSetupWhCallback callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
extern int WM_Initialize(void *work, WifiSetupWhCallback callback, int dmaNo);
extern int WM_SetIndCallback(WifiSetupWhCallback callback);
extern void *WM_GetSharedDataAddress(void *info, void *dataSet, u16 aid);
extern int WM_StepDataSharing(void *info, void *data, void *dataSet);
extern int WM_StartDataSharing(void *info, u16 port, u16 aidBitmap, u16 dataLength, int doubleMode);
extern u16 WM_GetNextTgid(void);
extern u16 WM_GetDispersionBeaconPeriod(void);
extern void *MB_CommGetChildUser(u16 aid);
extern void MB_End(void);
extern void DWCi_BACKUPlRead(void *buf);
extern FSResult WifiSetupArchive_Proc(FSFile *file, FSCommandType command);
extern FSResult WifiSetupArchive_Read(FSArchive *arc, void *dst, u32 pos, u32 size);
extern FSResult WifiSetupArchive_Write(FSArchive *arc, const void *src, u32 pos, u32 size);

int WifiSetupMbParent_GetChildState(int aid) {
    WifiSetupMbParentState state;
    OSIntrMode old = OS_DisableInterrupts();
    u16 mask = 1 << aid;
    if (!(gWifiSetupMbParent->head.connectedMask & mask)) {
        OS_RestoreInterrupts(old);
        return 0;
    }
    MI_CpuCopy8(gWifiSetupMbParent, &state, sizeof(state));
    OS_RestoreInterrupts(old);
    if (state.requestedMask & mask) {
        return 2;
    }
    if (state.waitingMask & mask) {
        return 3;
    }
    if (state.sendingMask & mask) {
        return 4;
    }
    if (state.sentMask & mask) {
        return 5;
    }
    if (state.bootableMask & mask) {
        return 6;
    }
    return 1;
}

WifiSetupMbParentChild *WifiSetupMbParent_GetChild(int aid) {
    if (!(gWifiSetupMbParent->head.connectedMask & (1 << aid))) {
        return NULL;
    }
    return &gWifiSetupMbParent->children[aid - 1];
}

static inline int IsSameMacAddr(const u8 *mac, const u8 *other) {
    return mac[0] == other[0] && mac[1] == other[1] && mac[2] == other[2] && mac[3] == other[3] && mac[4] == other[4] && mac[5] == other[5];
}

u16 WifiSetupMbParent_FindAid(const u8 *macAddr) {
    u16 i;
    for (i = 1; i < 2; i++) {
        if (gWifiSetupMbParent->head.connectedMask & (1 << i)) {
            if (IsSameMacAddr(macAddr, gWifiSetupMbParent->children[i - 1].macAddr)) {
                return gWifiSetupMbParent->children[i - 1].aid;
            }
        }
    }
    return 0;
}

void WifiSetupWh_Init(u8 *work) {
    gWifiSetupWh.work = (WifiSetupWhWork *)work;
    gWifiSetupWh.work->state = WIFI_SETUP_WH_STATE_STOP;
    gWifiSetupWh.work->unk_13A8 = 0;
    gWifiSetupWh.work->getWepKey = NULL;
    gWifiSetupWh.work->unk_13B0 = 0;
}

void WifiSetupWh_SetState(u32 state) {
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhFmt_StateFrom, gWifiSetupWhStateNames[gWifiSetupWh.work->state]);
    }
    gWifiSetupWh.work->state = state;
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhFmt_StateTo, gWifiSetupWhStateNames[gWifiSetupWh.work->state]);
    }
}

void WifiSetupWh_SetError(u32 code) {
    // kept while in WIFI_SETUP_WH_STATE_ERROR or WIFI_SETUP_WH_STATE_FATAL
    if ((u32)(gWifiSetupWh.work->state - WIFI_SETUP_WH_STATE_ERROR) > 1) {
        gWifiSetupWh.work->errorCode = code;
    }
}

int WifiSetupWh_SetParentParameter(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_SetParentParameter((WifiSetupWhCallback)WifiSetupWh_SetParentParameterCallback, &gWifiSetupWh.work->parentParam);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    return 0;
}

void WifiSetupWh_SetParentParameterCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return;
    }
    if (gWifiSetupWh.work->getWepKey != NULL) {
        if (WifiSetupWh_SetWepKey() == 0) {
            WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        }
    } else if (WifiSetupWh_StartParent() == 0) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    }
}

int WifiSetupWh_SetWepKey(void) {
    int mode, result;
    int (*getWepKey)(u8 *, WifiSetupWhWork *);
    WifiSetupWhWork *work;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    work = gWifiSetupWh.work;
    getWepKey = work->getWepKey;
    mode = getWepKey(work->wepKey, work);
    result = WM_SetWEPKey((WifiSetupWhCallback)WifiSetupWh_SetWepKeyCallback, mode, gWifiSetupWh.work->wepKey);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    return 0;
}

void WifiSetupWh_SetWepKeyCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return;
    }
    if (WifiSetupWh_StartParent() == 0) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    }
}

int WifiSetupWh_StartParent(void) {
    int result;
    // already connected, data sharing or key sharing
    if ((u32)(gWifiSetupWh.work->state - WIFI_SETUP_WH_STATE_CONNECTED) <= 2) {
        return 1;
    }
    result = WM_StartParent((WifiSetupWhCallback)WifiSetupWh_StartParentCallback);
    if (result != WM_ERRCODE_OPERATING) {
        WifiSetupWh_SetError(result);
        return 0;
    }
    gWifiSetupWh.work->unk_50 = 0;
    gWifiSetupWh.work->connectedMask = 1;
    return 1;
}

void WifiSetupWh_StartParentCallback(WifiSetupWhParentArg *arg) {
    struct {
        WifiSetupWhParentArg *arg;
        u16 mask;
        u32 aid;
    } frame;

    frame.arg = arg;
    frame.aid = frame.arg->aid;
    {
        u32 one = 1;
        frame.mask = one << frame.aid;
    }
    if (frame.arg->result) {
        WifiSetupWh_SetError(frame.arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return;
    }
    switch (frame.arg->state) {
    case 7: // child connected
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_ChildConnected, frame.aid);
        }
        if (gWifiSetupWh.work->judgeChild && gWifiSetupWh.work->judgeChild(frame.arg) == 0) {
            int result = WM_Disconnect(NULL, frame.arg->aid);
            if (result == WM_ERRCODE_OPERATING) {
                return;
            }
            WifiSetupWh_SetError(result);
            WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
            return;
        }
        gWifiSetupWh.work->connectedMask |= frame.mask;
        return;
    case 9: // child disconnected
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_ChildDisconnected, frame.aid);
        }
        gWifiSetupWh.work->connectedMask &= ~frame.mask;
        return;
    case 0: // parent started
        if (!WifiSetupWh_StartMp()) {
            WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        }
        return;
    case 2:
        return;
    default:
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_UnknownIndicate, frame.arg->state);
        }
        return;
    }
}

int WifiSetupWh_StartMp(void) {
    int result;
    if ((u32)(gWifiSetupWh.work->state - WIFI_SETUP_WH_STATE_CONNECTED) <= 2) {
        return 1;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_CONNECTED);
    result = WM_StartMP((WifiSetupWhCallback)WifiSetupWh_StartMpCallback, gWifiSetupWh.work->recvBuffer, (u16)gWifiSetupWh.work->recvBufferSize, gWifiSetupWh.work->sendBuffer, (u16)gWifiSetupWh.work->sendBufferSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_StartMpCallback(WifiSetupWhMpArg *arg) {
    int result, state;
    if (arg->result) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return;
    }
    state = arg->state;
    switch (state) {
    case 10: // MP started
        if (gWifiSetupWh.work->connectMode == WIFI_SETUP_WH_MODE_KS_PARENT) {
            if (gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_CONNECTED) {
                if (WifiSetupWh_StartKeySharing()) {
                    return;
                }
                if (gWifiSetupWh.log) {
                    gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_StartParentKeyShareFailed);
                }
                WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
                return;
            } else if (gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_KEYSHARING) {
                return;
            }
        } else if (gWifiSetupWh.work->connectMode == WIFI_SETUP_WH_MODE_DS_PARENT) {
            result = WM_StartDataSharing(gWifiSetupWh.work->dataSharingInfo, 13, 7, 68, 1);
            if (result) {
                WifiSetupWh_SetError(result);
                WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
                return;
            }
            WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_DATASHARING);
            return;
        }
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_CONNECTED);
        return;
    case 11:
        break;
    case 12:
    case 13:
    default:
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_UnknownIndicate, state);
        }
        break;
    }
}

int WifiSetupWh_StartKeySharing(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_KEYSHARING);
    result = WM_StartKeySharing(gWifiSetupWh.work->keySharing, 13);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

int WifiSetupWh_EndKeySharing(void) {
    int result;
    result = WM_EndKeySharing(gWifiSetupWh.work->keySharing);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

int WifiSetupWh_EndMp(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_EndMP((WifiSetupWhCallback)WifiSetupWh_EndMpCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_EndMpCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_TryReset();
        return;
    }
    if (WifiSetupWh_EndParent() != 0) {
        return;
    }
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_EndParentFailed);
    }
    WifiSetupWh_TryReset();
}

int WifiSetupWh_EndParent(void) {
    int result;
    result = WM_EndParent((WifiSetupWhCallback)WifiSetupWh_EndParentCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_EndParentCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_IDLE);
}

int WifiSetupWh_StopKeySharing(void) {
    int result;
    if (gWifiSetupWh.work->state != WIFI_SETUP_WH_STATE_KEYSHARING) {
        return 0;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_EndKeySharing(gWifiSetupWh.work->keySharing);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

int WifiSetupWh_EndMpForDisconnect(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_EndMP((WifiSetupWhCallback)WifiSetupWh_EndMpForDisconnectCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_EndMpForDisconnectCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_Finalize();
        return;
    }
    if (WifiSetupWh_Disconnect() == 0) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    }
}

int WifiSetupWh_Disconnect(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_Disconnect((WifiSetupWhCallback)WifiSetupWh_DisconnectCallback, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    WifiSetupWh_TryReset();
    return 0;
}

void WifiSetupWh_DisconnectCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_IDLE);
}

int WifiSetupWh_Reset(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_Reset((WifiSetupWhCallback)WifiSetupWh_ResetCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_ResetCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        WifiSetupWh_SetError(arg->result);
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_IDLE);
}

void WifiSetupWh_EndCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_FATAL);
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_STOP);
}

void WifiSetupWh_SetGgid(u32 ggid) {
    gWifiSetupWh.work->parentParam.ggid = ggid;
}

u16 WifiSetupWh_GetConnectedMask(void) {
    return gWifiSetupWh.work->connectedMask;
}

u32 WifiSetupWh_GetState(void) {
    return gWifiSetupWh.work->state;
}

int WifiSetupWh_StartMeasureChannel(void) {
    WifiSetupWhWork *work;
    u16 status;
    u16 macAddr[3];
    OS_GetMacAddress((u8 *)macAddr);
    gWifiSetupWh.work->randomSeed = OS_GetVBlankCount() + macAddr[0] + macAddr[1] + macAddr[2];
    work = gWifiSetupWh.work;
    work->randomSeed = work->randomSeed * 69069u + 12345u;
    gWifiSetupWh.work->channel = 0;
    gWifiSetupWh.work->minBusyRatio = 101;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    status = WifiSetupWh_MeasureNextChannel(1);
    if (status == 24) {
        WifiSetupWh_SetError(24);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return 0;
    }
    if (status == 2) {
        return 1;
    }
    WifiSetupWh_SetError(status);
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    return 0;
}

// returns 2 when a measurement started, 24 when there is no channel left
u16 WifiSetupWh_MeasureNextChannel(u16 channel) {
    int allowed = WM_GetAllowedChannel();
    if (allowed == 0x8000) {
        WifiSetupWh_SetError(3);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return 3;
    }
    if (allowed == 0) {
        WifiSetupWh_SetError(22);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return 24;
    }
    while (!(allowed & (1 << (channel - 1)))) {
        channel++;
        if (channel > 16) {
            return 24;
        }
    }
    return WifiSetupWh_MeasureChannel((WifiSetupWhCallback)WifiSetupWh_MeasureChannelCallback, channel);
}

void WifiSetupWh_MeasureChannelCallback(WifiSetupWhMeasureArg *arg) {
    u16 channel, busyRatio;
    int result;
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
        return;
    }
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_ChannelBratio, arg->channel, arg->busyRatio);
    }
    busyRatio = arg->busyRatio;
    channel = arg->channel;
    if (gWifiSetupWh.work->minBusyRatio > busyRatio) {
        gWifiSetupWh.work->minBusyRatio = busyRatio;
        gWifiSetupWh.work->quietChannels = 1 << (channel - 1);
    } else if (gWifiSetupWh.work->minBusyRatio == busyRatio) {
        gWifiSetupWh.work->quietChannels |= 1 << (channel - 1);
    }
    result = WifiSetupWh_MeasureNextChannel(channel + 1);
    if (result == 24) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_MEASURECHANNEL);
        return;
    }
    if (result == WM_ERRCODE_OPERATING) {
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
}

int WifiSetupWh_MeasureChannel(WifiSetupWhCallback callback, u16 channel) {
    return WM_MeasureChannel(callback, 3, 17, channel, 30);
}

u16 WifiSetupWh_DecideChannel(void) {
    if (gWifiSetupWh.work->state != WIFI_SETUP_WH_STATE_MEASURECHANNEL) {
        OS_Terminate();
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_IDLE);
    gWifiSetupWh.work->channel = WifiSetupWh_PickChannel(gWifiSetupWh.work->quietChannels);
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_DecidedChannel, gWifiSetupWh.work->channel);
    }
    return gWifiSetupWh.work->channel;
}

// a random one of the channels in mask
s16 WifiSetupWh_PickChannel(u16 mask) {
    s16 last = 0, i;
    u16 count = 0, target;
    for (i = 0; i < 16; i++) {
        if (mask & (1 << i)) {
            last = i + 1;
            count++;
        }
    }
    if (count <= 1) {
        return last;
    }
    gWifiSetupWh.work->randomSeed = gWifiSetupWh.work->randomSeed * 69069 + 12345;
    target = (count * (gWifiSetupWh.work->randomSeed & 255)) >> 8;
    for (i = 0; i < 16; i++) {
        if (mask & 1) {
            if (target == 0) {
                return i + 1;
            }
            target--;
        }
        mask >>= 1;
    }
    return 0;
}

#pragma push
#pragma opt_rebuildconditionals off

int WifiSetupWh_Start(void) {
    gWifiSetupWh.work->recvBufferSize = 0;
    gWifiSetupWh.work->sendBufferSize = 0;
    gWifiSetupWh.work->unk_48 = 0;
    gWifiSetupWh.work->unk_50 = 0;
    gWifiSetupWh.work->connectedMask = 1;
    gWifiSetupWh.work->errorCode = 0;
    gWifiSetupWh.work->parentParam.userData = NULL;
    gWifiSetupWh.work->parentParam.userDataLength = 0;
    gWifiSetupWh.work->judgeChild = NULL;
    // called with an argument of 0
    return ((int (*)(int))WifiSetupWh_InitializeWm)(0) == 0 ? 0 : 1;
}

#pragma pop

void WifiSetupWh_IndicationCallback(WifiSetupWhCallbackArg *arg) {
    if (arg->result != WM_ERRCODE_FIFO_ERROR) {
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    OS_Terminate();
}

int WifiSetupWh_InitializeWm(void) {
    int result;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_Initialize(gWifiSetupWh.work->wmWork, (WifiSetupWhCallback)WifiSetupWh_InitializeCallback, 2);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_FATAL);
    return 0;
}

void WifiSetupWh_InitializeCallback(WifiSetupWhCallbackArg *arg) {
    int result;
    if (arg->result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(arg->result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_FATAL);
        return;
    }
    result = WM_SetIndCallback((WifiSetupWhCallback)WifiSetupWh_IndicationCallback);
    if (result != WM_ERRCODE_SUCCESS) {
        WifiSetupWh_SetError(result);
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_FATAL);
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_IDLE);
}

#pragma push
#pragma opt_propagation off
#pragma opt_dead_assignments off

int WifiSetupWh_StartConnection(u32 mode, u16 tgid, u16 channel) {
    (void)((gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_IDLE) || (OS_Terminate(), 0));
    gWifiSetupWh.work->recvBufferSize = 0x180;
    gWifiSetupWh.work->sendBufferSize = 0xE0;
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_RecvBufferSize, gWifiSetupWh.work->recvBufferSize);
    }
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_SendBufferSize, gWifiSetupWh.work->sendBufferSize);
    }
    gWifiSetupWh.work->connectMode = mode;
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    gWifiSetupWh.work->parentParam.tgid = tgid;
    gWifiSetupWh.work->parentParam.channel = channel;
    gWifiSetupWh.work->parentParam.beaconPeriod = WM_GetDispersionBeaconPeriod();
    gWifiSetupWh.work->parentParam.parentMaxSize = 0xD0;
    gWifiSetupWh.work->parentParam.childMaxSize = 0x44;
    gWifiSetupWh.work->parentParam.maxEntries = 2;
    gWifiSetupWh.work->parentParam.continuousSend = 0;
    gWifiSetupWh.work->parentParam.multiBoot = 0;
    gWifiSetupWh.work->parentParam.entry = 1;
    gWifiSetupWh.work->parentParam.keySharing = mode == WIFI_SETUP_WH_MODE_KS_PARENT ? 1 : 0;
    if (mode == WIFI_SETUP_WH_MODE_MP_PARENT || mode == WIFI_SETUP_WH_MODE_KS_PARENT || mode == WIFI_SETUP_WH_MODE_DS_PARENT) {
        return WifiSetupWh_SetParentParameter();
    }
    if (gWifiSetupWh.log) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_UnknownConnectMode, mode);
    }
    return 0;
}

#pragma pop

void WifiSetupWh_SetJudgeCallback(WifiSetupWhJudgeFunc judge) {
    gWifiSetupWh.work->judgeChild = judge;
}

void *WifiSetupWh_GetSharedData(u16 aid) {
    return WM_GetSharedDataAddress(gWifiSetupWh.work->dataSharingInfo, gWifiSetupWh.work->dataSet, aid);
}

int WifiSetupWh_StepDataSharing(void *data) {
    int result = WM_StepDataSharing(gWifiSetupWh.work->dataSharingInfo, data, gWifiSetupWh.work->dataSet);
    if (result == WM_ERRCODE_NO_CHILD) {
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_StepNoChild);
        }
        return 0;
    }
    if (result == WM_ERRCODE_NO_DATASET) {
        if (gWifiSetupWh.log) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_StepNoDataSet);
        }
        WifiSetupWh_SetError(result);
        return 0;
    }
    if (result == WM_ERRCODE_SUCCESS) {
        return 1;
    }
    WifiSetupWh_SetError(result);
    return 0;
}

void WifiSetupWh_TryReset(void) {
    if (WifiSetupWh_Reset() != 0) {
        return;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_FATAL);
}

void WifiSetupWh_Finalize(void) {
    if (gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_IDLE) {
        if (gWifiSetupWh.log != NULL) {
            gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_AlreadyIdle);
        }
        return;
    }
    if (gWifiSetupWh.log != NULL) {
        gWifiSetupWh.log(WH_LOG_LEVEL, gWifiSetupWhStr_Finalize, gWifiSetupWh.work->state);
    }
    if (!(gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_KEYSHARING || gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_DATASHARING || gWifiSetupWh.work->state == WIFI_SETUP_WH_STATE_CONNECTED)) {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
        WifiSetupWh_TryReset();
    } else {
        WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
        switch (gWifiSetupWh.work->connectMode) {
        case WIFI_SETUP_WH_MODE_KS_CHILD:
            if (!WifiSetupWh_StopKeySharing()) {
                WifiSetupWh_TryReset();
            }
            break;
        case WIFI_SETUP_WH_MODE_MP_CHILD:
        case WIFI_SETUP_WH_MODE_DS_CHILD:
            if (!WifiSetupWh_EndMpForDisconnect()) {
                WifiSetupWh_TryReset();
            }
            break;
        case WIFI_SETUP_WH_MODE_KS_PARENT:
            if (!WifiSetupWh_EndKeySharing()) {
                WifiSetupWh_TryReset();
            }
            break;
        case WIFI_SETUP_WH_MODE_MP_PARENT:
        case WIFI_SETUP_WH_MODE_DS_PARENT:
            if (!WifiSetupWh_EndMp()) {
                WifiSetupWh_TryReset();
            }
            break;
        }
    }
}

int WifiSetupWh_End(void) {
    int result;
    if (gWifiSetupWh.work->state != WIFI_SETUP_WH_STATE_IDLE) {
        OS_Terminate();
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_BUSY);
    result = WM_End((WifiSetupWhCallback)WifiSetupWh_EndCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return 1;
    }
    WifiSetupWh_SetState(WIFI_SETUP_WH_STATE_ERROR);
    return 0;
}

void WifiSetupShare_Init(WifiSetupShare *share, const WifiSetupSessionParams *params) {
    gWifiSetupShare = share;
    WifiSetupMbParent_Init(gWifiSetupShare->mbParent);
    gWifiSetupShare->tgid = 0;
    gWifiSetupShare->channel = 0;
    gWifiSetupShare->state = 1;
    gWifiSetupShare->prevState = 1;
    gWifiSetupShare->timer = 0;
    WifiSetupShare_ClearPackets();
    gWifiSetupShare->params.filePath = params->filePath;
    gWifiSetupShare->params.title = params->title;
    gWifiSetupShare->params.description = params->description;
    gWifiSetupShare->params.unk_0C = params->unk_0C;
    gWifiSetupShare->params.unk_10 = params->unk_10;
    gWifiSetupShare->params.ggid = params->ggid;
    gWifiSetupShare->languageCode = params->languageCode;
    gWifiSetupShare->params.languageCode = 2;
    OS_GetTick();
    DWCi_BACKUPlRead(gWifiSetupShare->backup);
    {
        u64 tick = OS_GetTick();
        // called with the tick in the argument registers
        gWifiSetupShare->data = (u8 *)((WifiSetupConnections * (*)(u32, u32)) WifiSetupConnections_Get)((u32)tick, (u32)(tick >> 32));
    }
}

int WifiSetupShare_Cancel(void) {
    WifiSetupShare *share = gWifiSetupShare;
    u8 state = share->state;
    if (state == 1 || state == 20 || state == 23 || state == 26 || state == 29) {
        share->state = 34;
        gWifiSetupShare->request = 0;
        return 1;
    }
    if (state == 4 || state == 5 || state == 6 || state == 13) {
        if (state == 4 && share->counter < 6) {
            return 0;
        }
        MB_End();
        gWifiSetupShare->state = 16;
        gWifiSetupShare->request = 2;
        return 1;
    }
    if (state == 9 || state == 10) {
        share->state = 32;
        return 1;
    }
    if (state == 12) {
        share->state = 34;
        return 1;
    }
    return 0;
}

void WifiSetupShare_Reset(void) {
    WifiSetupWh_SetGgid(gWifiSetupShare->params.ggid);
    gWifiSetupShare->state = 1;
    gWifiSetupShare->tgid = WM_GetNextTgid();
    MI_CpuCopy8(gWifiSetupShare->data, gWifiSetupShare->firstBlock, 64);
    gWifiSetupShare->sendingData = 0;
    gWifiSetupShare->errorCount = 0;
    gWifiSetupShare->tgid++;
}

int WifiSetupShare_Start(void) {
    if (gWifiSetupShare->state != 1 && gWifiSetupShare->state != 26 && gWifiSetupShare->state != 29) {
        return 0;
    }
    WifiSetupShare_Reset();
    WifiSetupWh_Start();
    gWifiSetupShare->state = 2;
    return 1;
}

int WifiSetupShare_StartDownload(void) {
    if (gWifiSetupShare->state != 5) {
        return 0;
    }
    gWifiSetupShare->state = 6;
    WifiSetupMbParent_StartDownload();
    return 1;
}

void WifiSetupShare_Update(void) {
    switch (gWifiSetupShare->state) {
    case 1:
        if (gWifiSetupShare->request == 1) {
            gWifiSetupShare->request = 0;
            WifiSetupShare_Start();
            return;
        }
        if (gWifiSetupShare->request == 2) {
            gWifiSetupShare->request = 0;
            gWifiSetupShare->state = 34;
        }
        return;
    case 2:
        WifiSetupShare_UpdateChannelScan();
        return;
    case 3:
        WifiSetupShare_SetupMbParent();
        gWifiSetupShare->state = 4;
        return;
    case 4:
        gWifiSetupShare->counter++;
        WifiSetupShare_UpdateDownload();
        return;
    case 5:
    case 6:
        WifiSetupShare_UpdateDownload();
        return;
    case 7:
        WifiSetupShare_StartSharing();
        return;
    case 8:
    case 9:
    case 10:
        WifiSetupShare_UpdateSharing();
        return;
    case 11:
        WifiSetupShare_SetDone();
        return;
    case 16:
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 17;
        return;
    case 17:
        if (gWifiSetupShare->timer++ > 30) {
            WifiSetupShare_EndWireless();
        }
        return;
    case 18:
        MB_End();
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 22;
        return;
    case 19:
        if (gWifiSetupShare->timer++ <= 30) {
            return;
        }
        if (WifiSetupWh_GetState() == WIFI_SETUP_WH_STATE_IDLE) {
            WifiSetupWh_End();
            gWifiSetupShare->state = 20;
            return;
        }
        return;
    case 21:
        WifiSetupWh_Finalize();
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 22;
        return;
    case 22:
        if (gWifiSetupShare->timer++ <= 30) {
            return;
        }
        if (WifiSetupWh_GetState() == WIFI_SETUP_WH_STATE_IDLE) {
            WifiSetupWh_End();
            gWifiSetupShare->state = 23;
            return;
        }
        return;
    case 24:
        WifiSetupWh_Finalize();
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 25;
        return;
    case 25:
        if (gWifiSetupShare->timer++ <= 30) {
            return;
        }
        if (WifiSetupWh_GetState() == WIFI_SETUP_WH_STATE_IDLE) {
            WifiSetupWh_End();
            gWifiSetupShare->state = 26;
            return;
        }
        return;
    case 27:
        WifiSetupWh_Finalize();
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 28;
        return;
    case 28:
        if (gWifiSetupShare->timer++ <= 30) {
            return;
        }
        if (WifiSetupWh_GetState() == WIFI_SETUP_WH_STATE_IDLE) {
            WifiSetupWh_End();
            gWifiSetupShare->state = 29;
            return;
        }
        if (gWifiSetupShare->timer % 30 == 1 && gWifiSetupShare->timer > 55) {
            WifiSetupWh_Finalize();
        }
        return;
    case 32:
        WifiSetupWh_Finalize();
        gWifiSetupShare->timer = 0;
        gWifiSetupShare->state = 33;
        return;
    case 33:
        if (gWifiSetupShare->timer++ <= 30) {
            return;
        }
        if (WifiSetupWh_GetState() == WIFI_SETUP_WH_STATE_IDLE) {
            WifiSetupWh_End();
            gWifiSetupShare->state = 34;
            return;
        }
        return;
    case 30:
        WifiSetupWh_Finalize();
        return;
    case 0:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 29:
    case 31:
    case 34:
    default:
        goto done;
    }
done:
    return;
}

void WifiSetupShare_GetState(u8 *state, u8 *changed) {
    *state = gWifiSetupShare->state;
    if (gWifiSetupShare->state != gWifiSetupShare->prevState) {
        *changed = 1;
    } else {
        *changed = 0;
    }
    gWifiSetupShare->prevState = gWifiSetupShare->state;
}

void *WifiSetupShare_GetChildUser(void) {
    return MB_CommGetChildUser(1);
}

void WifiSetupShare_UpdateChannelScan(void) {
    switch (WifiSetupWh_GetState()) {
    case WIFI_SETUP_WH_STATE_IDLE:
        WifiSetupWh_StartMeasureChannel();
        break;
    case WIFI_SETUP_WH_STATE_BUSY:
        break;
    case WIFI_SETUP_WH_STATE_MEASURECHANNEL:
        gWifiSetupShare->channel = WifiSetupWh_DecideChannel();
        gWifiSetupShare->counter = 0;
        gWifiSetupShare->state = 3;
        break;
    case WIFI_SETUP_WH_STATE_STOP:
        gWifiSetupShare->counter = 0;
        gWifiSetupShare->state = 3;
        break;
    case WIFI_SETUP_WH_STATE_ERROR:
        WifiSetupWh_TryReset();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void WifiSetupShare_SetupMbParent(void) {
    WifiSetupMbParent_Setup(gWifiSetupShare->params.ggid, gWifiSetupShare->tgid);
}

void WifiSetupShare_UpdateDownload(void) {
    int state;
    switch (WifiSetupMbParent_GetState()) {
    case 1:
        WifiSetupMbParent_Start((WifiSetupMbParentFile *)&gWifiSetupShare->params, gWifiSetupShare->channel);
        break;
    case 2:
        if (WifiSetupMbParent_GetMask(2)) {
            gWifiSetupShare->state = 5;
            return;
        }
        if (WifiSetupMbParent_GetMask(3) || WifiSetupMbParent_GetMask(4)) {
            gWifiSetupShare->state = 6;
            return;
        }
        if (gWifiSetupShare->state == 5 && !WifiSetupMbParent_GetMask(2)) {
            gWifiSetupShare->state = 13;
        }
        break;
    case 3:
        if (WifiSetupMbParent_AreAllBootable()) {
            WifiSetupMbParent_BootChildren();
            return;
        }
        if ((u8)(gWifiSetupShare->state + 250) <= 1 && !WifiSetupMbParent_GetMask(3)) { // states 6 and 7
            gWifiSetupShare->state = 18;
        }
        break;
    case 5:
        gWifiSetupShare->state = 7;
        break;
    case 7:
        WifiSetupMbParent_End();
        gWifiSetupShare->state = 1;
        break;
    case 0:
        state = WifiSetupWh_GetState();
        switch (state) {
        case WIFI_SETUP_WH_STATE_IDLE:
            WifiSetupWh_End();
            break;
        case WIFI_SETUP_WH_STATE_STOP:
            gWifiSetupShare->state = 31;
            break;
        case WIFI_SETUP_WH_STATE_BUSY:
            break;
        default:
            gWifiSetupShare->state = 31;
            break;
        }
        break;
    }
}

void WifiSetupShare_StartSharing(void) {
    WifiSetupShare_ClearPackets();
    WifiSetupWh_SetJudgeCallback(WifiSetupShare_JudgeChild);
    gWifiSetupShare->state = 8;
}

void WifiSetupShare_UpdateSharing(void) {
    switch (WifiSetupWh_GetState()) {
    case WIFI_SETUP_WH_STATE_IDLE:
        WifiSetupWh_StartConnection(WIFI_SETUP_WH_MODE_DS_PARENT, gWifiSetupShare->tgid + 1, gWifiSetupShare->channel);
        return;
    case WIFI_SETUP_WH_STATE_CONNECTED:
    case WIFI_SETUP_WH_STATE_DATASHARING:
    case WIFI_SETUP_WH_STATE_KEYSHARING:
        WifiSetupShare_SendPacket(0, gWifiSetupShare->blockIndex, gWifiSetupShare->data + (gWifiSetupShare->blockIndex % 16) * 64);
        WifiSetupShare_UpdateChild();
        if (gWifiSetupShare->state == 27) {
            return;
        }
        if (gWifiSetupShare->errorCount > 480) {
            gWifiSetupShare->state = 27;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 16 || WifiSetupShare_GetChildKind() == 32) {
            WifiSetupShare_GetChildKind();
            gWifiSetupShare->state = 10;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 64) {
            gWifiSetupShare->state = 11;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 255) {
            gWifiSetupShare->state = 27;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 80) {
            gWifiSetupShare->state = 21;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 96) {
            gWifiSetupShare->state = 24;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 112) {
            gWifiSetupShare->state = 27;
            return;
        }
        if (WifiSetupShare_GetChildKind() == 0) {
            gWifiSetupShare->state = 8;
            return;
        }
        if (WifiSetupShare_GetChildKind() == WIFI_SETUP_SHARE_PACKET_READY) {
            gWifiSetupShare->state = 9;
            return;
        }
        gWifiSetupShare->state = 31;
        return;
    default:
        return;
    }
}

void WifiSetupShare_SetDone(void) {
    gWifiSetupShare->state = 12;
}

int WifiSetupShare_EndWireless(void) {
    if (WifiSetupWh_GetState() != WIFI_SETUP_WH_STATE_IDLE) {
        return 0;
    }
    WifiSetupWh_End();
    gWifiSetupShare->state = 1;
    return 1;
}

// only the child that Download Play booted may connect
BOOL WifiSetupShare_JudgeChild(WifiSetupWhParentArg *arg) {
    u16 aid = WifiSetupMbParent_FindAid(arg->macAddr);
    if (aid == 0) {
        return FALSE;
    }
    gWifiSetupShare->childUsers[aid - 1] = WifiSetupMbParent_GetChild(aid);
    return TRUE;
}

void WifiSetupShare_ClearPackets(void) {
    MI_CpuFill8(gWifiSetupShare->received.raw, 0, sizeof(gWifiSetupShare->received));
    MI_CpuFill8(gWifiSetupShare->send.raw, 0, sizeof(gWifiSetupShare->send));
    gWifiSetupShare->sendPacket = &gWifiSetupShare->send.packet;
}

void WifiSetupShare_SendPacket(u16 kind, u16 index, void *data) {
    u16 i;
    if (gWifiSetupShare->sendingData == 1) {
        gWifiSetupShare->sendPacket->kind = kind;
        gWifiSetupShare->sendPacket->index = index;
        MI_CpuCopy8(data, gWifiSetupShare->sendPacket->data, 64);
    } else {
        gWifiSetupShare->errorCount++;
        gWifiSetupShare->sendPacket->kind = WIFI_SETUP_SHARE_PACKET_HELLO;
        gWifiSetupShare->sendPacket->data[0] = gWifiSetupShare->languageCode;
    }
    if (WifiSetupWh_GetState() != WIFI_SETUP_WH_STATE_DATASHARING) {
        return;
    }
    if (WifiSetupWh_StepDataSharing(gWifiSetupShare) == 0) {
        gWifiSetupShare->errorCount += 4;
        return;
    }
    if (gWifiSetupShare->sendingData == 0) {
        gWifiSetupShare->errorCount++;
    } else {
        gWifiSetupShare->errorCount = 0;
        if (WifiSetupWh_GetConnectedMask() != 3) {
            gWifiSetupShare->state = 27;
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        void *shared = WifiSetupWh_GetSharedData(i);
        if (shared != NULL) {
            MI_CpuCopy8(shared, &gWifiSetupShare->received.packets[i], sizeof(WifiSetupSharePacket));
            gWifiSetupShare->receivedFlags[i] = 1;
        } else {
            gWifiSetupShare->receivedFlags[i] = 0;
        }
    }
}

// follows the child's answers: READY starts the settings blocks, then the block index advances
// every other frame while the child keeps answering 16
void WifiSetupShare_UpdateChild(void) {
    int i, stride = sizeof(WifiSetupSharePacket);
    for (i = 0; i < 16; i++) {
        WifiSetupShare *share = gWifiSetupShare;
        if (share->receivedFlags[i]) {
            u32 offset = (u32)i * (u32)stride;
            u16 *kind = (u16 *)((u32)share + 0x100 + offset); // received.packets[i].kind
            if (i == 1) {
                if (share->sendingData == 1) {
                    if (*kind != 16) {
                        return;
                    }
                    share->frame++;
                    if (!(gWifiSetupShare->frame & 1)) {
                        gWifiSetupShare->blockIndex++;
                        if (gWifiSetupShare->blockIndex >= 36) {
                            gWifiSetupShare->blockIndex = 0;
                        }
                    }
                } else {
                    share->unk_202 = WIFI_SETUP_SHARE_PACKET_HELLO;
                    if (*kind == WIFI_SETUP_SHARE_PACKET_READY) {
                        gWifiSetupShare->sendingData = 1;
                        gWifiSetupShare->blockIndex = 0;
                        gWifiSetupShare->frame = 0;
                    }
                }
            }
        }
    }
}

u16 WifiSetupShare_GetChildKind(void) {
    return gWifiSetupShare->received.packets[1].kind;
}

void WifiSetupArchive_Load(void) {
    FSFile file;
    char path[128];
    u32 fnt[2], fat[2], base, size;
    gWifiSetupArchive = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupArchiveState), 4);
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, gWifiSetupArchivePath)) {
        OS_Terminate();
    }
    gWifiSetupArchive->lockId = OS_GetLockID();
    base = file.prop.file.top;
    FS_ReadFile(&file, fnt, 8);
    FS_ReadFile(&file, fat, 8);
    FS_CloseFile(&file);
    FS_InitArchive(&gWifiSetupArchive->arc);
    if (!FS_RegisterArchiveName(&gWifiSetupArchive->arc, gWifiSetupArchiveName, 3)) {
        OS_Terminate();
    }
    FS_SetArchiveProc(&gWifiSetupArchive->arc, WifiSetupArchive_Proc, 0x602);
    if (!FS_LoadArchive(&gWifiSetupArchive->arc, base, fat[0], fat[1], fnt[0], fnt[1], WifiSetupArchive_Read, WifiSetupArchive_Write)) {
        OS_Terminate();
    }
    size = FS_LoadArchiveTables(&gWifiSetupArchive->arc, NULL, 0);
    gWifiSetupArchive->tables = WifiSetupHeap_AllocZeroed(size, 4);
    FS_LoadArchiveTables(&gWifiSetupArchive->arc, gWifiSetupArchive->tables, size);
    gWifiSetupArchive->filePool = WifiSetupPool_NewWithItems(32, gWifiSetupArchive->poolItems, 4);
    OS_SPrintf(path, gWifiSetupArchivePrefixFormat, gWifiSetupArchiveName);
    FS_ChangeDir(path);
}
