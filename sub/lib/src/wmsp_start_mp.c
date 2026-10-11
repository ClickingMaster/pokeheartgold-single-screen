#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Checks the ARM9's MP buffers, sets the MP parameters and starts MP (state 7 -> 9 on the parent,
// 8 -> 10 on a child) with the VAlarm timing.
void WMSP_StartMP(WmspStartMPRequest *request) {
    WmspWork *work = &wmspW;
    u8 *recvBuf = request->recvBuf;
    u32 recvBufSize = request->recvBufSize;
    u8 *sendBuf = request->sendBuf;
    u32 sendBufSize = request->sendBufSize;
    u32 error = 0;
    u32 flags;
    WmspStatus *status = wmspW.status;
    WmspStatus *settings;
    u32 irq;
    u32 buffer[128];
    u16 *cb;
    u16 *result;
    if (!status->unk_9c) {
        u32 needed = (status->sendMaxSize + 31) & ~31u;
        if (sendBufSize < needed) {
            error = 6;
        }
        if (status->aid == 0) {
            needed = (status->recvMaxSize + 12) * status->parentParam.maxEntry + 0x29;
        } else {
            needed = status->recvMaxSize + 0x51;
        }
        needed &= ~31u;
        if (recvBufSize < needed) {
            error = 6;
        }
    }
    if (status->unk_e6 == 2) {
        if (!(status->allowedChannel & ((1 << work->arm9Buf->bssDesc.channel) >> 1))) {
            error = 6;
        }
    }
    if (error) {
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 14;
        cb[1] = error;
        cb[2] = 10;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        return;
    }
    {
        u32 clean = 0;
        if (status->mpActive) {
            status->mpActive = 0;
            clean = 1;
        }
        if (clean) {
            WMSP_CleanSendQueue(0xffff);
        }
    }
    WMSP_InitSendQueue();
    irq = __VENEER_OS_DisableInterrupts();
    WMSP_SetMPParameterCore(request->param, NULL);
    {
        flags = request->flags;
        settings = wmspW.status;
        if (settings->state != 9 && settings->state != 10) {
            u32 old = __VENEER_OS_DisableInterrupts();
            // the original addresses these three fields from settings + 0x700
            u8 *mp = (u8 *)settings + 0x700;
            u32 max = (flags & 4) ? request->unk_38 : settings->unk_5c;
            u32 a, b;
            if (!max) {
                max = 16;
            }
            a = (flags & 1) ? request->unk_34 : settings->unk_58;
            if (!a) {
                a = 16;
            }
            if (a > max) {
                a = max;
            }
            b = (flags & 2) ? request->unk_36 : settings->unk_5a;
            ((WmspStatus *)(mp - 0x700))->unk_7c4 = max;
            if (!b) {
                b = 16;
            }
            ((WmspStatus *)(mp - 0x700))->unk_7c0 = a;
            if (b > max) {
                b = max;
            }
            ((WmspStatus *)(mp - 0x700))->unk_7c2 = b;
            if (settings->unk_62 > (s32)max) {
                settings->unk_62 = max;
            }
            settings->sendRetryCount = (flags & 0x200) ? request->unk_3a : settings->unk_98;
            settings->unk_7c6 = (flags & 0x400) ? request->unk_3c : settings->unk_92;
            settings->unk_7c8 = (flags & 0x800) ? request->unk_3d : settings->unk_94;
            settings->unk_7cc = (flags & 0x1000) ? request->unk_3e : settings->unk_9a;
            __VENEER_OS_RestoreInterrupts((OSIntrMode)old);
        }
    }
    // 7: parent, 8: child
    if ((u16)(status->state + 0xfff9) <= 1) {
        OSTick tick;
        int i;
        status->mpAckAlarmSet = 0;
        status->unk_5e = 0;
        status->unk_60 = 1;
        status->unk_88 = 0;
        status->unk_9e = 0;
        status->unk_a0 = 60;
        status->sendInFlight = 0;
        status->unk_8a = 0;
        status->unk_8c = 0;
        status->unk_8e = 0;
        status->unk_90 = 0;
        status->resumePending = 0;
        status->mpBuf[0] = recvBuf;
        status->mpBufSize = recvBufSize;
        status->mpBuf[1] = recvBuf + recvBufSize;
        status->mpBufIndex = 0;
        status->sendBuf = sendBuf;
        status->sendBufSize = sendBufSize;
        status->unk_62 = 0;
        status->unk_64 = 0;
        status->unk_68 = 0;
        status->unk_6a = 0;
        status->minRssi = 0xffff;
        status->linkLevelCountdown = 1;
        tick = __VENEER_OS_GetTick() | 1;
        for (i = 0; i < 16; i++) {
            status->lastRecvTick[i] = tick;
        }
        WMSP_SetThreadPriorityHigh();
        status->vAlarmRequestQueued = 0;
        WMSP_SetVAlarm();
        if (status->state == 8) {
            status->state = 10;
        } else if (status->state == 7) {
            status->state = 9;
        }
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 14;
        cb[1] = 0;
        cb[2] = 10;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
        status->mpActive = 1;
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
        result = WMSP_WL_ParamSetNullKeyResponseMode(buffer, 1);
        if (result[2]) {
            cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 14;
            cb[1] = 1;
            cb[2] = 0x216;
            cb[3] = result[2];
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    } else {
        __VENEER_OS_RestoreInterrupts((OSIntrMode)irq);
        cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        cb[0] = 14;
        cb[1] = 3;
        cb[2] = 10;
        __VENEER_WMSP_ReturnResult2Wm9(cb);
    }
}
