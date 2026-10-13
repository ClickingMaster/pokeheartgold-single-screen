#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Low half-words of the wireless TSF timer.
#define REG_WL_TSF0 (*(vu16 *)0x048080F8)
#define REG_WL_TSF1 (*(vu16 *)0x048080FA)

// VSync time stamp (TSF / 64): stored by the parent (WmspParentAdjustVSync), read by the child
// (WmspChildAdjustVSync1).
#define WMSP_VSYNC_TIME (*(vu16 *)0x0380FFF0)

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void WMSP_InitVAlarm(void) {
    OS_CreateVAlarm(&wmspVAlarm);
}

void WMSP_CancelVAlarm(void) {
    OS_CancelVAlarm(&wmspVAlarm);
}

// Starts the MP timing: parent (unk_e6 1) at VCOUNT 203, child (unk_e6 2) at VCOUNT 200.
void WMSP_SetVAlarm(void) {
    OSVAlarm *alarm = &wmspVAlarm;
    WmspStatus *status = wmspW.status;
    if (status->mode == 1) {
        if (alarm->handler) {
            __VENEER_OS_CancelVAlarm(alarm);
        }
        __VENEER_OS_SetVAlarm(&wmspVAlarm, 203, 263, WmspParentAdjustVSync, (void *)3);
    } else if (status->mode == 2) {
        status->vsyncFlag = 0;
        if (alarm->handler) {
            __VENEER_OS_CancelVAlarm(alarm);
        }
        __VENEER_OS_SetVAlarm(&wmspVAlarm, 200, 263, WmspChildAdjustVSync1, (void *)1);
        status->vsyncAdjust = 0;
    }
}

// clang-format off
asm void __VENEER_OS_CancelVAlarm(OSVAlarm *alarm) {
    ldr pc, =OS_CancelVAlarm
}
// clang-format on

// clang-format off
asm void __VENEER_OS_SetVAlarm(OSVAlarm *alarm, s16 count, s16 delay, OSVAlarmHandler handler, void *arg) {
    ldr pc, =OS_SetVAlarm
}
// clang-format on

#pragma dont_inline reset

// Sets vsyncAdjust to the time from now to the parent's next VSync (vsyncTime, then advanced by
// whole frames), 0 when it is already past or too far.
static inline void WmspChildCalcVSyncAdjust(void) {
    WmspStatus *status = wmspW.status;
    u16 tsfLo, tsfLo2;
    u32 tsfHi;
    u32 now;
    int i;
    status->vsyncTime <<= 6;
    tsfLo = REG_WL_TSF0;
    tsfHi = REG_WL_TSF1;
    tsfLo2 = REG_WL_TSF0;
    if (tsfLo > tsfLo2) {
        tsfHi = REG_WL_TSF1;
    }
    tsfHi = (tsfLo2 | (tsfHi << 16)) & 0x3fffc0;
    now = ((263 - reg_GX_VCOUNT) * 127 + (tsfHi << 1)) >> 1 & 0x3fffc0;
    if (status->vsyncTime > now) {
        status->vsyncAdjust = 0;
        return;
    }
    for (i = 1; i < 30; i++) {
        status->vsyncTime += 0x414b;
        if (status->vsyncTime > now) {
            status->vsyncAdjust = status->vsyncTime - now;
            if (status->vsyncAdjust > 0x400e) {
                status->vsyncAdjust = 0;
            }
            return;
        }
    }
    status->vsyncAdjust = 0;
}

// Child VAlarm at VCOUNT 200: adjusts VCOUNT (WmspChildAdjustVSync2) while the parent's VSync is
// more than 127 away, else schedules the MP VAlarm.
void WmspChildAdjustVSync1(void *arg) {
    WmspStatus *status = wmspW.status;
    u32 vsyncTime = WMSP_VSYNC_TIME;
    status->vsyncTime = vsyncTime;
    if (status->lastVSyncWord != vsyncTime) {
        status->lastVSyncWord = vsyncTime;
        WmspChildCalcVSyncAdjust();
    }
    if (status->vsyncAdjust > 127) {
        __VENEER_OS_SetVAlarm(&wmspVAlarm, 208, 263, WmspChildAdjustVSync2, (void *)2);
    } else {
        status->vsyncFlag = 1;
        __VENEER_OS_SetVAlarm(&wmspVAlarm, status->childVAlarmLine, 263, WmspChildVAlarmMP, (void *)4);
    }
}

#pragma dont_inline on

// Child VAlarm at VCOUNT 208: moves VCOUNT back by up to 6 lines (63 per line) towards the parent.
void WmspChildAdjustVSync2(void *arg) {
    int i;
    s32 line = reg_GX_VCOUNT;
    WmspStatus *status = wmspW.status;
    if (line >= 208 && line < 210 && status->vsyncAdjust >= 127) {
        for (i = 1; i < 7; i++) {
            if (*(vu32 *)&status->vsyncAdjust < 127 + i * 63) {
                break;
            }
        }
        reg_GX_VCOUNT = line + (1 - i);
        status->vsyncAdjust -= i * 63;
    }
    if (status->vsyncAdjust >= 127) {
        status->vsyncFlag = 0;
    }
    __VENEER_OS_SetVAlarm(&wmspVAlarm, status->childVAlarmLine, 263, WmspChildVAlarmMP, (void *)4);
}

// Child MP VAlarm: re-arms the VSync check and kicks the MP request, or queues a disconnect
// (request 37) when the parent was silent for longer than recvTimeout.
void WmspChildVAlarmMP(void *arg) {
    WmspStatus *status = wmspW.status;
    if (status->mpActive == 1) {
        __VENEER_OS_SetVAlarm(&wmspVAlarm, 200, 263, WmspChildAdjustVSync1, (void *)1);
        if (status->recvTimeout) {
            OSTick now = __VENEER_OS_GetTick() | 1;
            if (status->lastRecvTick[0] && now - status->lastRecvTick[0] > status->recvTimeout) {
                u32 *request;
                int sent;
                status->lastRecvTick[0] = 0;
                request = WMSP_GetInternalRequestBuf();
                if (request) {
                    request[0] = 37;
                    request[1] = 0;
                    request[2] = 0x8001;
                    sent = __VENEER_OS_SendMessage(&wmspW.pxiQueue, request, OS_MESSAGE_NOBLOCK);
                } else {
                    sent = 0;
                }
                if (!sent) {
                    u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                    cb[0] = 128;
                    cb[1] = 8;
                    cb[2] = 22;
                    cb[3] = 37;
                    __VENEER_WMSP_ReturnResult2Wm9(cb);
                }
                return;
            }
        }
        WmspFromVAlarmToWmspThread();
    }
}

// Parent VAlarm at VCOUNT 203: rewrites VCOUNT every 60 frames, publishes the VSync time stamp and
// schedules the MP VAlarm.
void WmspParentAdjustVSync(void *arg) {
    WmspStatus *status = wmspW.status;
    s32 line;
    u32 tsfLo, tsfHi, tsfLo2;
    if (status->vcountResyncCount >= 60) {
        line = reg_GX_VCOUNT;
        if (line >= 203 && line < 210) {
            reg_GX_VCOUNT = line;
            status->vcountResyncCount = 0;
        }
    } else {
        status->vcountResyncCount++;
    }
    line = reg_GX_VCOUNT;
    tsfLo = REG_WL_TSF0;
    tsfHi = REG_WL_TSF1;
    tsfLo2 = REG_WL_TSF0;
    if (tsfLo > tsfLo2) {
        tsfHi = REG_WL_TSF1;
    }
    WMSP_VSYNC_TIME = ((tsfLo2 | (tsfHi << 16)) * 2 - (u32)line * 127) >> 7;
    __VENEER_OS_SetVAlarm(&wmspVAlarm, status->parentVAlarmLine, 263, WmspParentVAlarmMP, (void *)5);
}

void WmspParentVAlarmMP(void *arg) {
    WmspStatus *status = wmspW.status;
    if (status->mpActive == 1) {
        __VENEER_OS_SetVAlarm(&wmspVAlarm, 203, 263, WmspParentAdjustVSync, (void *)3);
        WmspFromVAlarmToWmspThread();
    }
}

// Queues request 28 (WMSP_VAlarmSetMPData) once; reports an error when the queue is full.
void WmspFromVAlarmToWmspThread(void) {
    WmspWork *work = &wmspW;
    WmspStatus *status = wmspW.status;
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    u32 *request;
    int sent;
    if (status->vAlarmRequestQueued == 1) {
        __VENEER_OS_RestoreInterrupts(irq);
        return;
    }
    status->vAlarmRequestQueued = 1;
    __VENEER_OS_RestoreInterrupts(irq);
    request = WMSP_GetInternalRequestBuf();
    if (request) {
        request[0] = 28;
        sent = __VENEER_OS_SendMessage(&work->pxiQueue, request, OS_MESSAGE_NOBLOCK);
    } else {
        sent = 0;
    }
    if (!sent) {
        status->vAlarmRequestQueued = 0;
        if (work->arm9Buf) {
            u16 *cb = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb[0] = 128;
            cb[1] = 8;
            cb[2] = 22;
            cb[3] = 28;
            __VENEER_WMSP_ReturnResult2Wm9(cb);
        }
    }
}

// Request 28, once per MP frame: updates the link level, then starts the next MP sequence
// (parent) or sends the key data (child).
void WMSP_VAlarmSetMPData(void *request) {
    WmspStatus *status = wmspW.status;
    status->vAlarmRequestQueued = 0;
    status->unk_88 = 1;
    status->linkLevelCountdown--;
    if (status->linkLevelCountdown == 0) {
        if (status->minRssi == 0xffff) {
            status->minRssi = 4;
        }
        WMSP_AddRssiToList((u8)status->minRssi);
        status->linkLevel = WMSP_GetAverageLinkLevel();
        status->minRssi = 0xffff;
        status->linkLevelCountdown = 1;
    }
    if (status->state == 9) {
        OSIntrMode irq = __VENEER_OS_DisableInterrupts();
        int skip, send;
        if (status->childBitmap == 0) {
            status->mpCount = 0;
            __VENEER_OS_RestoreInterrupts(irq);
            return;
        }
        skip = 1;
        if (status->mpCount > 0 && status->mpLimitCount > 0) {
            skip = 0;
        }
        if (status->mpCount < 0) {
            status->mpCount = 0;
        }
        status->mpCount += status->currentFreq;
        if (status->mpCount > 256) {
            status->mpCount = 256;
        }
        status->mpLimitCount = status->currentMaxFreq;
        if (skip && status->mpCount > 0 && status->mpLimitCount > 0) {
            send = 1;
        } else {
            send = 0;
        }
        __VENEER_OS_RestoreInterrupts(irq);
        if (send) {
            WMSP_SendMaMP(0xffff);
        }
        if (status->currentMinPollBmpMode == 1) {
            status->unk_a0--;
            if (!status->unk_a0) {
                status->unk_9e = 1;
                status->unk_a0 = 60;
            }
        }
    } else if (status->state == 10) {
        int send = 0;
        OSIntrMode irq = __VENEER_OS_DisableInterrupts();
        if (status->sendInFlight != 1) {
            status->unk_60 = 0;
            send = 1;
        }
        __VENEER_OS_RestoreInterrupts(irq);
        if (send == 1) {
            WMSP_SendMaKeyData();
        }
    }
}

// request[1]: bitmap of the children to send to.
void WMSP_KickNextMP_Parent(u32 *request) {
    if (wmspW.status->state == 9) {
        WMSP_SendMaMP((u16)request[1]);
    }
}

void WMSP_KickNextMP_Child(void *request) {
    if (wmspW.status->state == 10) {
        WMSP_SendMaKeyData();
    }
}

void WMSP_KickNextMP_Resume(u32 *request) {
    if (wmspW.status->state == 9) {
        WMSP_ResumeMaMP((u16)request[1]);
    }
}

#pragma dont_inline reset
