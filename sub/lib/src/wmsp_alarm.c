#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Internal requests queued for WMSP_RequestThread.
#define WMSP_REQ_KICK_MP_PARENT 0x2b
#define WMSP_REQ_KICK_MP_CHILD  0x2c
#define WMSP_REQ_RESUME_MP      0x2d

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void WMSP_RequestResumeMP(void) {
    WmspStatus *status = wmspW.status;
    u32 *request = WMSP_GetInternalRequestBuf();
    BOOL ok;
    if (request) {
        request[0] = WMSP_REQ_RESUME_MP;
        request[1] = status->prevPollBitmap;
        ok = __VENEER_OS_SendMessage(&wmspW.pxiQueue, request, OS_MESSAGE_NOBLOCK);
    } else {
        ok = FALSE;
    }
    if (ok) {
        status->resumePending = 1;
    } else {
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = 0x80;
        result[1] = WM_ERRCODE_FIFO_ERROR;
        result[2] = 0x16;
        result[3] = WMSP_REQ_RESUME_MP;
        __VENEER_WMSP_ReturnResult2Wm9(result);
    }
}

void WmspMPParentIntervalAlarmCallback(void *arg) {
    WmspKickMPParent((u16)(u32)arg);
}

#pragma opt_treetransformation off

void WmspKickMPParent(u16 bitmap) {
    u32 *request = WMSP_GetInternalRequestBuf();
    WmspWork *work = &wmspW;
    BOOL ok;
    if (request) {
        request[0] = WMSP_REQ_KICK_MP_PARENT;
        request[1] = bitmap;
        ok = __VENEER_OS_SendMessage(&work->pxiQueue, request, OS_MESSAGE_NOBLOCK);
    } else {
        ok = FALSE;
    }
    if (!ok && work->arm9Buf) {
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = 0x80;
        result[1] = WM_ERRCODE_FIFO_ERROR;
        result[2] = 0x16;
        result[3] = WMSP_REQ_KICK_MP_PARENT;
        __VENEER_WMSP_ReturnResult2Wm9(result);
    }
}

// Posts a WL "multi-poll acknowledgement" indication (0x185) to WMSP_IndicateThread.
void WmspMaMultiPollAckAlarmCallback(void *arg) {
    WmspWork *work = &wmspW;
    u16 *msg = __VENEER_OS_AllocFromHeap(work->heapArena, work->heapHandle, 64);
    (void)arg;
    msg[6] = 0x185;
    msg[7] = 0;
    if (!__VENEER_OS_SendMessage(&work->indicateQueue, msg, OS_MESSAGE_NOBLOCK)) {
        WmspFreeBufOfWL(msg);
        if (work->arm9Buf) {
            u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            result[0] = 0x80;
            result[1] = WM_ERRCODE_FIFO_ERROR;
            result[2] = 0x16;
            result[3] = 0x80;
            __VENEER_WMSP_ReturnResult2Wm9(result);
        }
    }
}

#pragma opt_treetransformation reset

// clang-format off
asm void *__VENEER_OS_AllocFromHeap(OSArenaId id, OSHeapHandle heap, u32 size) {
    ldr pc, =OS_AllocFromHeap
}
// clang-format on

void WmspMPChildIntervalAlarmCallback(void *arg) {
    WmspKickMPChild(arg);
}

#pragma opt_treetransformation off

void WmspKickMPChild(void *arg) {
    u32 *request = WMSP_GetInternalRequestBuf();
    WmspWork *work;
    WmspStatus *status = wmspW.status;
    BOOL ok = FALSE;
    (void)arg;
    status->unk_5e = 0;
    status->unk_60 = 0;
    status->unk_88 = 0;
    work = &wmspW;
    if (request) {
        request[0] = WMSP_REQ_KICK_MP_CHILD;
        ok = __VENEER_OS_SendMessage(&work->pxiQueue, request, OS_MESSAGE_NOBLOCK);
    }
    if (!ok && work->arm9Buf) {
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = 0x80;
        result[1] = WM_ERRCODE_FIFO_ERROR;
        result[2] = 0x16;
        result[3] = WMSP_REQ_KICK_MP_CHILD;
        __VENEER_WMSP_ReturnResult2Wm9(result);
    }
}

#pragma opt_treetransformation reset

void WMSP_InitAlarm(void) {
    __VENEER_OS_CreateAlarm(&wmspMPIntervalAlarm);
    __VENEER_OS_CreateAlarm(&wmspMPAckAlarm);
}

// clang-format off
asm void __VENEER_OS_CreateAlarm(OSAlarm *alarm) {
    ldr pc, =OS_CreateAlarm
}
// clang-format on

void WMSP_CancelAllAlarms(void) {
    __VENEER_OS_CancelAlarm(&wmspMPIntervalAlarm);
    __VENEER_OS_CancelAlarm(&wmspMPAckAlarm);
}

#pragma dont_inline reset
