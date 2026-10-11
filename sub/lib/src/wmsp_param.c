#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void WMSP_EndMP(void *request) {
    u8 buf[512];
    int clean = 0;
    OSIntrMode irq;
    WmspStatus *status = wmspW.status;
    u16 *cb, error;
    if (status->state != 9 && status->state != 10) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 16;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    irq = __VENEER_OS_DisableInterrupts();
    if (status->mpActive == 1) {
        clean = 1;
    }
    status->mpActive = 0;
    WMSP_CancelVAlarm();
    WMSP_SetThreadPriorityLow();
    if (status->state == 10) {
        status->state = 8;
    } else if (status->state == 9) {
        status->state = 7;
    }
    __VENEER_OS_RestoreInterrupts(irq);
    error = WMSP_WL_ParamSetNullKeyResponseMode(buf, 0)[2];
    if (error) {
        WmspError_06005328(0x216, error);
        return;
    }
    error = WMSP_WL_MaClearData(buf, 7)[2];
    if (error) {
        WmspError_06005328(0x104, error);
        return;
    }
    status->unk_8a = 0;
    if (clean) {
        WMSP_CleanSendQueue(0xffff);
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 16;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Reports the failed WL command `command` (driver result `result`) of WMSP_EndMP.
void WmspError_06005328(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 16;
    cb[1] = 1;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request[1]: receive buffer (two halves of request[2] bytes).
void WMSP_StartDCF(u32 *request) {
    WmspStatus *status = wmspW.status;
    u32 raw = request[2];
    u8 *buffer = (u8 *)request[1];
    u16 size = raw;
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    status->rxBuf[0] = buffer;
    status->rxBufSize = size;
    status->rxBuf[1] = buffer + size;
    status->rxBufIndex = 0;
    status->dcfData = NULL;
    status->dcfDataSize = 0;
    status->unk_18 = 0;
    status->state = 11;
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 17;
    cb[1] = 0;
    cb[2] = 14;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
    status->dcfActive = 1;
    __VENEER_OS_RestoreInterrupts(irq);
}

// MA data frame given to WMSP_WL_MaData.
typedef struct WlMaDataFrame {
    u16 unk_00;
    u16 unk_02[2];
    u16 length;
    u8 unk_08[6];
    u8 rate; // 10: 1 Mbps, 20: 2 Mbps
    u8 unk_0f[0x18 - 0x0f];
    u8 destAddress[6];
    u8 srcAddress[6];
    u8 unk_24[0x2c - 0x24];
    u16 *data;
} WlMaDataFrame;

// request[1..2]: destination MAC address, request[3]: data, request[4]: its size.
void WMSP_SetDCFData(u32 *request) {
    WlMaDataFrame frame;
    u8 buf[512];
    WmspStatus *status = wmspW.status;
    __VENEER_MI_CpuCopy8(request + 1, status->dcfDestAddress, 6);
    status->dcfData = (u16 *)request[3];
    status->dcfDataSize = request[4];
    status->unk_18 = 1;
    __VENEER_MIi_CpuClear16(0, &frame, sizeof(frame));
    frame.unk_00 = 0;
    frame.length = request[4];
    frame.rate = status->unk_1ec == 2 ? 20 : 10;
    __VENEER_MI_CpuCopy8(request + 1, frame.destAddress, 6);
    __VENEER_MI_CpuCopy8(status->macAddress, frame.srcAddress, 6);
    frame.data = (u16 *)request[3];
    u16 *reply = WMSP_WL_MaData((u16 *)buf, (u16 *)&frame);
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 18;
    if (reply[2] == 0) {
        cb[1] = 0;
    } else {
        cb[1] = 1;
    }
    if (reply[2]) {
        cb[2] = 0x100;
        cb[3] = reply[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_EndDCF(void *request) {
    u8 buf[512];
    WmspStatus *status = wmspW.status;
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    u16 *cb, error;
    if (status->state != 11) {
        __VENEER_OS_RestoreInterrupts(irq);
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 19;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        status->dcfActive = 0;
        status->state = 8;
        __VENEER_OS_RestoreInterrupts(irq);
        error = WMSP_WL_MaClearData(buf, 7)[2];
        if (error) {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 19;
            cb[1] = 1;
            cb[2] = 0x104;
            cb[3] = error;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        } else {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 19;
            cb[1] = 0;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}

// request[1]: WEP mode (0: off), request[2]: the 0x50-byte key block.
void WMSP_SetWEPKey(u32 *request) {
    WmspStatus *status = wmspW.status;
    status->wepMode = request[1];
    switch (status->wepMode) {
    case 0:
        status->wepEnabled = 0;
        break;
    case 1:
    case 2:
    case 3:
        status->wepEnabled = 1;
        break;
    default:
        status->wepEnabled = 0;
        break;
    }
    if (status->wepEnabled == 1) {
        __VENEER_MI_CpuCopy8((void *)request[2], status->wepKey, 0x50);
    } else {
        __VENEER_MI_CpuFill8(status->wepKey, 0, 0x50);
    }
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 20;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// As WMSP_SetWEPKey, request[3]: key id. Reports success also after a failed key id command.
void WMSP_SetWEPKeyEx(u32 *request) {
    u8 buf[512];
    WmspStatus *status = wmspW.status;
    status->wepMode = request[1];
    switch (status->wepMode) {
    case 0:
        status->wepEnabled = 0;
        break;
    case 1:
    case 2:
    case 3:
        status->wepEnabled = 1;
        break;
    default:
        status->wepEnabled = 0;
        break;
    }
    if (status->wepEnabled == 1) {
        __VENEER_MI_CpuCopy8((void *)request[2], status->wepKey, 0x50);
    } else {
        __VENEER_MI_CpuFill8(status->wepKey, 0, 0x50);
    }
    u16 error;
    u16 *cb;
    status->wepKeyId = request[3];
    error = WMSP_WL_ParamSetWepKeyId(buf, status->wepKeyId)[2];
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 20;
        cb[1] = 1;
        cb[2] = 0x207;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 39;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request[1]: user game info, [2]: its length, [3]: ggid, [4]: tgid, [5]: attribute bits.
void WMSP_SetGameInfo(u32 *request) {
    u8 buf[512];
    u8 param[128];
    WmspStatus *status = wmspW.status;
    u8 flags;
    u16 *reply, *cb;
    status->parentParam.userGameInfo = (void *)request[1];
    status->parentParam.userGameInfoLength = request[2];
    status->parentParam.ggid = request[3];
    status->parentParam.tgid = request[4];
    flags = request[5];
    status->parentParam.unk_0e = (flags & 1) ? 1 : 0;
    status->parentParam.unk_12 = (flags & 2) ? 1 : 0;
    status->parentParam.entryFlag = (flags & 4) ? 1 : 0;
    status->parentParam.unk_16 = (flags & 8) ? 1 : 0;
    WMSP_CopyParentParam(param, (u8 *)&status->parentParam);
    reply = WMSP_WL_ParamSetGameInfo(buf, (u16)(status->parentParam.userGameInfoLength + 16), param);
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 24;
    if (reply[2] == 0) {
        cb[1] = 0;
    } else {
        cb[1] = 1;
    }
    if (reply[2]) {
        cb[2] = 0x245;
        cb[3] = reply[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request[1]: 1 to get beacon send/receive indications.
void WMSP_SetBeaconTxRxInd(u32 *request) {
    u8 buf[512];
    u16 error = WMSP_WL_ParamSetBeaconSendRecvInd(buf, (u16)request[1])[2];
    u16 *cb;
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 25;
        cb[1] = 1;
        cb[2] = 0x215;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 25;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

// Test mode is not supported: both report result 4.
void WMSP_StartTestMode(void *request) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 26;
    cb[1] = 4;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_StopTestMode(void *request) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 27;
    cb[1] = 4;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

#pragma opt_lifetimes off

// request[1..3]: WMSP_WL_ParamSetLifeTime arguments, request[4]: receive timeout of the children
// in units of 100 ms (0xFFFF: none), stored as recvTimeout.
void WMSP_SetLifeTime(u32 *request) {
    u8 buf[512];
    u16 first = request[1], second = request[2], third = request[3], timeout = request[4];
    WmspStatus *status = wmspW.status;
    u16 error = WMSP_WL_ParamSetLifeTime(buf, first, second, third)[2];
    u16 *cb;
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 29;
        cb[1] = 1;
        cb[2] = 0x211;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        OSTick ticks;
        int i;
        if (timeout != 0xffff) {
            if (!timeout) {
                ticks = 1;
            } else {
                // milliseconds to ticks: 33514 ticks / 64 per millisecond
                ticks = ((u64)(s32)(timeout * 100) * 33514) >> 6;
            }
            status->recvTimeout = ticks;
        } else {
            status->recvTimeout = 0;
        }
        ticks = __VENEER_OS_GetTick() | 1;
        for (i = 0; i < 16; i++) {
            status->lastRecvTick[i] = ticks;
        }
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 29;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

#pragma opt_lifetimes reset

// request[1]: CCA mode, [2]: ED threshold, [3]: channel, [4]: measure time. Replies the CCA busy
// ratio and the channel.
void WMSP_MeasureChannel(u16 *request) {
    u16 *req = request;
    u8 channelList[16];
    u16 bufArr[256];
    u16 *buf = bufArr;
    WmspStatus *status;
    u16 *reply;
    u16 *cb;
    u16 ccaMode, edThreshold, measureTime;
    u16 ccaBusyRatio, channel;
    if (wmspW.status->state != 2) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 30;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status = wmspW.status;
    reply = WMSP_WL_DevGetStationState(buf);
    if (reply[2] != 0) {
        WmspError_06005BB8(0x308, reply[2]);
        return;
    }
    {
        u16 stationState = reply[3];
        status->unk_e6 = 2;
        if (stationState == 0x10) {
            if (!WMSP_SetAllParams(10, (u8 *)buf)) {
                return;
            }
            reply = WMSP_WL_DevClass1(buf);
            if (reply[2] != 0) {
                WmspError_06005BB8(0x303, reply[2]);
                return;
            }
            status->state = 3;
            reply = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);
            if (reply[2] != 0) {
                WmspError_06005BB8(1, reply[2]);
                return;
            }
            status->pwrMgtMode = 1;
        }
    }
    {
        u16 ch;
        ccaMode = req[1];
        edThreshold = req[2];
        ch = req[3];
        measureTime = req[4];
        __VENEER_MI_CpuFill8(channelList, 0, 16);
        channelList[0] = ch;
    }
    reply = WMSP_WL_MlmeMeasureChannel(buf, ccaMode, edThreshold, measureTime, channelList);
    if (reply[2] != 0) {
        WmspError_06005BB8(10, reply[2]);
        return;
    }
    ccaBusyRatio = reply[4] & 0xff;
    channel = reply[4] >> 8;
    reply = WMSP_WL_DevIdle(buf);
    if (reply[2] != 0) {
        WmspError_06005BB8(0x302, reply[2]);
        return;
    }
    status->state = 2;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 30;
    cb[1] = 0;
    cb[4] = ccaBusyRatio;
    cb[5] = channel;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Reports the failed WL command `command` (driver result `result`) of WMSP_MeasureChannel.
void WmspError_06005BB8(u16 command, u16 result) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 30;
    cb[1] = 1;
    cb[2] = command;
    cb[3] = result;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_InitWirelessCounter(void *request) {
    u8 buf[512];
    u16 error = WMSP_WL_DevSetInitializeWirelessCounter(buf)[2];
    u16 *cb;
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 31;
        cb[1] = 1;
        cb[2] = 0x305;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 31;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

// Replies the 0xB4 bytes of wireless counters.
void WMSP_GetWirelessCounter(void *request) {
    u8 buf[512];
    u16 *reply = WMSP_WL_DevGetWirelessCounter(buf);
    u16 error = reply[2];
    u16 *cb;
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 32;
        cb[1] = 1;
        cb[2] = 0x307;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 32;
        cb[1] = 0;
        __VENEER_MIi_CpuCopy16(reply + 4, cb + 4, 0xb4);
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

#pragma dont_inline reset
