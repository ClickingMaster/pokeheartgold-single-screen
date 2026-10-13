#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Unsigned access to a signed status field (the code reads and writes these as u16).
#define AS_U16(field) (*(u16 *)&(field))

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// request[1]: entry flag of the parent parameters; the game info is rebuilt with it.
void WMSP_SetEntry(u32 *request) {
    u32 buf[128], parent[32];
    WmspStatus *status = wmspW.status;
    u16 *result, *cb;
    status->parentParam.entryFlag = request[1];
    WMSP_CopyParentParam((u8 *)parent, (u8 *)&status->parentParam);
    result = WMSP_WL_ParamSetGameInfo(buf, (u16)(status->parentParam.userGameInfoLength + 16), parent);
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x21;
    if (!result[2]) {
        cb[1] = 0;
    } else {
        cb[1] = 1;
        cb[2] = 0x245;
        cb[3] = result[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request + 4: MAC address of the station to deauthenticate (retried once while the WL driver is busy).
void WMSP_AutoDeAuth(void *request) {
    u8 addr[8];
    u32 buf[128];
    int i;
    u16 *result, *cb;
    __VENEER_MI_CpuCopy8((u8 *)request + 4, addr, 6);
    for (i = 0; i < 2; i++) {
        result = WMSP_WL_MlmeDeAuthenticate((u16 *)buf, addr, 19);
        if (result[2] == 0 || (result[2] != 7 && result[2] != 12)) {
            break;
        }
    }
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x22;
    if (!result[2]) {
        cb[1] = 0;
    } else {
        cb[1] = 1;
        cb[2] = 5;
        cb[3] = result[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Takes over the ARM9's buffer and status block (state 0 -> 1).
void WMSP_Enable(void *request) {
    WmspStatus *status;
    WmspArm9Buf *arm9Buf;
    WmspEnableRequest *req = request;
    arm9Buf = req->arm9Buf;
    wmspW.arm9Buf = arm9Buf;
    status = req->status;
    wmspW.status = status;
    arm9Buf->status = status;
    arm9Buf->callbackBuf = req->callbackBuf;
    WMSPi_CommonInit(req->flags);
    {
        u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 3;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

// Resets the status block to its defaults (state 1); flags bit 1: keep the LED pattern.
void WMSPi_CommonInit(u32 flags) {
    WmspArm9Buf *arm9Buf;
    int clean = 0, i;
    OSIntrMode irq;
    WmspStatus *status = wmspW.status;
    arm9Buf = wmspW.arm9Buf;
    irq = __VENEER_OS_DisableInterrupts();
    if (status->mpActive == 1) {
        status->mpActive = 0;
        clean = 1;
        WMSP_CancelVAlarm();
        WMSP_SetThreadPriorityLow();
    }
    status->childBitmap = 0;
    status->readyBitmap = 0;
    status->unk_14 = 0;
    status->dcfActive = 0;
    status->vsyncFlag = 0;
    status->vAlarmRequestQueued = 0;
    status->beaconIndicateFlag = 0;
    status->minFreq = 1;
    status->freq = 1;
    status->maxFreq = 6;
    status->defaultRetryCount = 0;
    status->minPollBmpMode = 0;
    status->singlePacketMode = 0;
    status->ignoreFatalErrorMode = 0;
    status->ignoreSizePrecheckMode = 0;
    status->currentMinFreq = status->minFreq;
    status->currentFreq = status->freq;
    status->currentMaxFreq = status->maxFreq;
    status->sendRetryCount = status->defaultRetryCount;
    status->currentMinPollBmpMode = status->minPollBmpMode;
    status->currentSinglePacketMode = status->singlePacketMode;
    status->currentIgnoreFatalErrorMode = status->ignoreFatalErrorMode;
    status->wepEnabled = 0;
    status->wepMode = 0;
    __VENEER_MI_CpuFill8(status->wepKey, 0, 0x50);
    WMSP_ResetSizeVars();
    status->parentVAlarmLine = 0x104;
    status->childVAlarmLine = 0xf0;
    status->parentInterval = 1000;
    status->childInterval = 0;
    status->parentIntervalTick = 0x20b;
    status->childIntervalTick = 0;
    status->pwrMgtMode = 0;
    status->preamble = 1;
    status->miscFlags = flags;
    __VENEER_OS_RestoreInterrupts(irq);
    if (clean) {
        WMSP_CleanSendQueue(0xffff);
    }
    for (i = 0; i < 32; i++) {
        arm9Buf->requestBuf[i][0] = 0x8000; // free
    }
    __VENEER_MIi_CpuClear16(1, status->portSeqNo, 0x100);
    WMSP_InitAlarm();
    __VENEER_OS_InitMutex(&status->sendQueueMutex);
    WMSP_InitVAlarm();
    if (!(flags & 2)) {
        __VENEER_PM_SetLEDPattern(15);
    }
    status->state = 1;
}

void WMSP_Disable(void *request) {
    WmspStatus *status = wmspW.status;
    u16 *cb;
    if (status->state != 1) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 4;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    __VENEER_PM_SetLEDPattern(1);
    status->state = 0;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 4;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// State 1 -> 2: starts the WL driver (WMSPi_CommonWlIdle).
void WMSP_PowerOn(void *request) {
    u16 command, wlResult;
    WmspStatus *status = wmspW.status;
    u16 *cb;
    if (status->state != 1) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 5;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    if (!WMSPi_CommonWlIdle(&command, &wlResult)) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 5;
        cb[1] = 1;
        cb[2] = command;
        cb[3] = wlResult;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status->state = 2;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 5;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Restarts the WL driver into idle and reads its channels, version and MAC address. On failure
// stores the failing WL command and its result and returns FALSE.
BOOL WMSPi_CommonWlIdle(u16 *command, u16 *wlResult) {
    u32 buf[128];
    WmspStatus *status = wmspW.status;
    u16 *result;
    result = WMSP_WL_DevRestart(buf);
    if (result[2]) {
        *command = 0x304;
        *wlResult = result[2];
        return FALSE;
    }
    result = WMSP_WL_DevIdle(buf);
    if (result[2]) {
        *command = 0x302;
        *wlResult = result[2];
        return FALSE;
    }
    *(vu16 *)0x4808124 = 200;
    *(vu16 *)0x4808128 = 2000;
    *(vu16 *)0x4808150 = 0x202;
    result = WMSP_WL_ParamGetEnableChannel(buf);
    if (result[2]) {
        *command = 0x283;
        *wlResult = result[2];
        return FALSE;
    }
    {
        u16 channels = result[3];
        status->enableChannel = channels;
        status->allowedChannel = __VENEER_WMSP_GetAllowedChannel((u16)(channels >> 1));
    }
    WMSP_WL_ParamSetLifeTime(buf, 0xffff, 40, 5);
    status->recvTimeout = 0x1ff621;
    status->rate = 2;
    status->preamble = 1;
    result = WMSP_WL_DevGetVersion(buf);
    if (result[2]) {
        *command = 0x306;
        *wlResult = result[2];
        return FALSE;
    }
    __VENEER_MIi_CpuCopy16(result + 3, status->wlVersion, 8);
    status->unk_28 = result[7];
    status->unk_2c = result[8];
    status->unk_2e = result[9];
    status->unk_2a = result[10];
    result = WMSP_WL_ParamGetMacAddress(buf);
    if (result[2]) {
        *command = 0x281;
        *wlResult = result[2];
        return FALSE;
    }
    __VENEER_MI_CpuCopy8(result + 3, status->macAddress, 6);
    result = WMSP_WL_ParamSetBeaconSendRecvInd(buf, 1);
    if (result[2]) {
        *command = 0x215;
        *wlResult = result[2];
        return FALSE;
    }
    return TRUE;
}

// clang-format off
asm u16 __VENEER_WMSP_GetAllowedChannel(u16 channels) {
    ldr pc, =WMSP_GetAllowedChannel
}
// clang-format on

// State 2 -> 1: shuts the WL driver down.
void WMSP_PowerOff(void *request) {
    u32 buf[128];
    WmspStatus *status = wmspW.status;
    u16 *cb, *result;
    if (status->state != 2) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 6;
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    result = WMSP_WL_DevShutdown(buf);
    if (result[2]) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 6;
        cb[1] = 1;
        cb[2] = 0x301;
        cb[3] = result[2];
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status->state = 1;
    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 6;
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// request[1..]: WmspMPParam; the callback carries the previous parameters.
void WMSP_SetMPParameter(u32 *request) {
    WmspMPParam old;
    u32 result = WMSP_SetMPParameterCore((WmspMPParam *)(request + 1), &old);
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x23;
    cb[1] = result;
    *(u32 *)(cb + 2) = request[1];
    __VENEER_MI_CpuCopy8(&old, cb + 4, 0x1c);
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Sets the MP parameters selected by input->mask (bit n: field n) and stores the previous ones in
// `old` (may be NULL). Returns 0, 3 (size or mode fields during MP) or 6 (value out of range).
u32 WMSP_SetMPParameterCore(WmspMPParam *input, WmspMPParam *old) {
    OSIntrMode irq;
    u32 result = 0;
    u32 mask;
    WmspStatus *status = wmspW.status;
    mask = input->mask;
    if ((u16)(status->state + (u16)-9) <= 1 && (mask & 0x2c00)) {
        mask &= ~0x2c00;
        result = 3;
    }
    irq = __VENEER_OS_DisableInterrupts();
    if (old) {
        old->mask = 0x3fff;
        old->unk_04 = status->minFreq;
        old->unk_06 = status->freq;
        old->unk_08 = status->maxFreq;
        old->parentSize = status->parentSize;
        old->childSize = status->childSize;
        old->parentInterval = status->parentInterval;
        old->childInterval = status->childInterval;
        old->parentVAlarmLine = AS_U16(status->parentVAlarmLine);
        old->childVAlarmLine = AS_U16(status->childVAlarmLine);
        old->retryCount = status->defaultRetryCount;
        old->unk_18 = status->minPollBmpMode;
        old->unk_19 = status->singlePacketMode;
        old->unk_1a = status->ignoreFatalErrorMode;
        old->unk_1b = status->ignoreSizePrecheckMode;
    }
    if (mask & 1) {
        u16 value = input->unk_04;
        if (value == 0) {
            value = 16;
        }
        status->minFreq = value;
        status->currentMinFreq = value;
    }
    if (mask & 2) {
        u16 value = input->unk_06;
        if (value == 0) {
            value = 16;
        }
        status->freq = value;
        AS_U16(status->currentFreq) = value;
        if (status->mpCount > (s32)value) {
            status->mpCount = value;
        }
    }
    if (mask & 4) {
        u16 value = input->unk_08;
        if (value == 0) {
            value = 16;
        }
        status->maxFreq = value;
        status->currentMaxFreq = value;
        if (status->mpCount > (s32)value) {
            status->mpCount = value;
        }
    }
    if (mask & 8) {
        u16 value = input->parentSize;
        if (status->parentMaxSize < (u16)((value + 1) & ~1)) {
            result = 6;
        } else {
            WMSP_SetParentSize(value);
        }
    }
    if (mask & 16) {
        u16 value = input->childSize;
        if ((u16)((value + 1) & ~1) > 512) {
            result = 6;
        } else {
            WMSP_SetChildSize(value);
        }
    }
    if (mask & 32) {
        u32 value = input->parentInterval;
        if (value > 10000) {
            result = 6;
        } else {
            status->parentInterval = value;
            status->parentIntervalTick = ((u64)value * 0x82ea / 64) / 1024; // microseconds -> ticks
        }
    }
    if (mask & 64) {
        u32 value = input->childInterval;
        if (value > 10000) {
            result = 6;
        } else {
            status->childInterval = value;
            status->childIntervalTick = ((u64)value * 0x82ea / 64) / 1024;
        }
    }
    if (mask & 128) {
        u32 value = input->parentVAlarmLine;
        if (value <= 190 || (value >= 220 && value <= 262)) {
            status->parentVAlarmLine = value;
        } else {
            result = 6;
        }
    }
    if (mask & 256) {
        u32 value = input->childVAlarmLine;
        if (value <= 190 || (value >= 220 && value <= 262)) {
            status->childVAlarmLine = value;
        } else {
            result = 6;
        }
    }
    if (mask & 512) {
        u32 value = input->retryCount;
        status->defaultRetryCount = value;
        status->sendRetryCount = value;
    }
    if (mask & 1024) {
        status->minPollBmpMode = input->unk_18;
    }
    if (mask & 2048) {
        status->singlePacketMode = input->unk_19;
    }
    if (mask & 4096) {
        status->ignoreFatalErrorMode = input->unk_1a;
        status->currentIgnoreFatalErrorMode = status->ignoreFatalErrorMode;
    }
    if (mask & 8192) {
        status->ignoreSizePrecheckMode = input->unk_1b;
    }
    __VENEER_OS_RestoreInterrupts(irq);
    return result;
}

void WMSP_SetBeaconPeriod(u32 *request) {
    u32 buf[128];
    u16 error = WMSP_WL_ParamSetBeaconPeriod(buf, (u16)request[1])[2];
    u16 *cb;
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 0x24;
        cb[1] = 1;
        cb[2] = 0x242;
        cb[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    } else {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 0x24;
        cb[1] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}

void WMSP_AutoDisconnect(void *request) {
    WMSP_DisconnectCore(request, 1, 0);
}

// request[1]: 1 to enter power save mode (child, state 11); a null data frame tells the parent.
void WMSP_SetPowerSaveMode(void *request) {
    WlMaDataFrame frame;
    u16 bufArr[256];
    u16 *buf = bufArr;
    u32 *arg = request;
    WmspStatus *status = wmspW.status;
    u16 *cb;
    u16 *result;

    cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 0x28;
    if (status->state != 11) {
        cb[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    result = WMSP_WL_MlmePowerManagement(buf, arg[1] == 1, 0, 1);
    if (result[2] != 0) {
        cb[1] = 1;
        cb[2] = 1;
        cb[3] = result[2];
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    status = wmspW.status;
    __VENEER_MI_CpuCopy8(status->bssid, status->dcfDestAddress, 6);
    status->dcfData = bufArr;
    status->dcfDataSize = 0;
    status->dcfSendFlag = 1;
    __VENEER_MIi_CpuClear16(0, &frame, sizeof(frame));
    frame.unk_00 = 0;
    frame.length = 0;
    frame.rate = (status->rate == 2) ? 20 : 10;
    __VENEER_MI_CpuCopy8(status->bssid, frame.destAddress, 6);
    __VENEER_MI_CpuCopy8(status->macAddress, frame.srcAddress, 6);
    frame.data = bufArr;
    result = WMSP_WL_MaData(bufArr, (u16 *)&frame);
    if (result[2] != 0) {
        cb[1] = 1;
        cb[2] = 0x100;
        cb[3] = result[2];
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    cb[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

// Test receive mode is not supported: both requests answer 4.
void WMSP_StartTestRxMode(void) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 41;
    cb[1] = 4;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

void WMSP_StopTestRxMode(void) {
    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    cb[0] = 42;
    cb[1] = 4;
    __VENEER_WMSP_ReturnResult2Wm9(cb);
}

#pragma dont_inline reset
