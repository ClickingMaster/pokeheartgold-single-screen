#pragma force_active on
#include "wmsp_internal.h"
// The SDK headers below expect <nitro/types.h> (included by wmsp_internal.h) first.
#include <nitro/os/valarm.h>
#include <nitro/pxi.h>
#include <nitro/pxi/fifo.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Parameter block of WMSP_WL_ParamSetAll.
typedef struct WmspWlParams {
    u8 unk_00[0x10];
    u8 macAddress[6];
    u16 unk_16;
    u16 unk_18;
    u16 unk_1a;
    u16 unk_1c;
    u16 unk_1e;
    u16 wepMode;
    u16 wepKeyId;
    u8 wepKey[0x50];
    u16 unk_74;
    u16 unk_76;
    u16 unk_78;
    u16 unk_7a;
    u16 unk_7c[16];
    u16 unk_9c;
    u16 wepEnabled;
} WmspWlParams;

void *WL_GetThreadStruct(void);
void WL_InitDriver(void *driver);
void SND_BeginSleep(void);
void OS_Terminate(void);

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

#pragma opt_common_subs off

void WMSP_Init(u8 *driver, u32 *config) {
    int i;
    wmspW.unk_1540 = config[0];
    wmspW.heapArena = *(OSArenaId *)(driver + 0x24);
    wmspW.heapHandle = *(OSHeapHandle *)(driver + 0x28);
    wmspW.arm9Buf = NULL;
    wmspW.status = NULL;
    __VENEER_OS_InitMessageQueue(&wmspW.requestQueue, wmspW.requestMsgs, 2);
    __VENEER_OS_InitMessageQueue(&wmspW.indicateQueue, wmspW.indicateMsgs, 4);
    __VENEER_OS_InitMessageQueue(&wmspW.replyQueue, wmspW.replyMsgs, 4);
    __VENEER_OS_InitMessageQueue(&wmspW.pxiQueue, wmspW.pxiMsgs, 32);
    *(void **)(driver + 0x10) = &wmspW.requestQueue;
    *(void **)(driver + 0x14) = &wmspW.indicateQueue;
    wmspW.priorities.indicateHigh = config[2];
    wmspW.priorities.wlHigh = config[6];
    wmspW.priorities.requestHigh = config[4];
    wmspW.priorities.indicateLow = config[1];
    wmspW.priorities.wlLow = config[5];
    wmspW.priorities.requestLow = config[3];
    __VENEER_OS_InitMutex(&wmspW.mutex);
    // Each stack ends where the next member starts.
    __VENEER_OS_CreateThread(&wmspIndicateThread, WMSP_IndicateThread, NULL, &wmspW.mutex, 0x400, config[1]);
    __VENEER_OS_WakeupThreadDirect(&wmspIndicateThread);
    __VENEER_OS_CreateThread(&wmspRequestThread, WMSP_RequestThread, NULL, wmspW.indicateStack, 0x1000, config[3]);
    __VENEER_OS_WakeupThreadDirect(&wmspRequestThread);
    for (i = 0; i < 32; i++) {
        wmspW.rssiList[i] = 0;
    }
    wmspW.rssiIndex = 0;
    if (!__VENEER_OS_IsVAlarmAvailable()) {
        __VENEER_OS_InitVAlarm();
    }
    __VENEER_PXI_Init();
    __VENEER_PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WmspPxiCallback);
    *(u32 *)(driver + 0x18) = 2;
    *(u32 *)(driver + 0xc) = config[5];
    WL_InitDriver(driver);
}

#pragma opt_common_subs on

// clang-format off
asm void __VENEER_OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *msgArray, s32 msgCount) {
    ldr pc, =OS_InitMessageQueue
}
// clang-format on

// clang-format off
asm void __VENEER_OS_InitMutex(OSMutex *mutex) {
    ldr pc, =OS_InitMutex
}
// clang-format on

// clang-format off
asm void __VENEER_OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio) {
    ldr pc, =OS_CreateThread
}
// clang-format on

// clang-format off
asm void __VENEER_OS_WakeupThreadDirect(OSThread *thread) {
    ldr pc, =OS_WakeupThreadDirect
}
// clang-format on

// clang-format off
asm BOOL __VENEER_OS_IsVAlarmAvailable(void) {
    ldr pc, =OS_IsVAlarmAvailable
}
// clang-format on

// clang-format off
asm void __VENEER_OS_InitVAlarm(void) {
    ldr pc, =OS_InitVAlarm
}
// clang-format on

// clang-format off
asm void __VENEER_PXI_Init(void) {
    ldr pc, =PXI_Init
}
// clang-format on

// clang-format off
asm void __VENEER_PXI_SetFifoRecvCallback(int tag, void (*callback)(int, u16 *, BOOL)) {
    ldr pc, =PXI_SetFifoRecvCallback
}
// clang-format on

void *WMSP_WlRequest(void *request) {
    u8 *reply;
    __VENEER_OS_SendMessage(&wmspW.requestQueue, request, OS_MESSAGE_BLOCK);
    __VENEER_OS_ReceiveMessage(&wmspW.replyQueue, (OSMessage *)&reply, OS_MESSAGE_BLOCK);
    // Result code 14 from the WL driver: report and stop the ARM7.
    if (*(u16 *)(reply + *(u16 *)(reply + 0xe) * 2 + 0x14) == 14) {
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = 0x80;
        result[1] = WM_ERRCODE_FLASH_ERROR;
        result[2] = 0x18;
        __VENEER_WMSP_ReturnResult2Wm9(result);
        __VENEER_SND_BeginSleep();
        __VENEER_OS_Terminate();
    }
    return reply;
}

// clang-format off
asm BOOL __VENEER_OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags) {
    ldr pc, =OS_SendMessage
}
// clang-format on

// clang-format off
asm BOOL __VENEER_OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags) {
    ldr pc, =OS_ReceiveMessage
}
// clang-format on

// clang-format off
asm u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void) {
    ldr pc, =WMSP_GetBuffer4Callback2Wm9
}
// clang-format on

// clang-format off
asm void __VENEER_WMSP_ReturnResult2Wm9(void *result) {
    ldr pc, =WMSP_ReturnResult2Wm9
}
// clang-format on

// clang-format off
asm void __VENEER_SND_BeginSleep(void) {
    ldr pc, =SND_BeginSleep
}
// clang-format on

// clang-format off
asm void __VENEER_OS_Terminate(void) {
    ldr pc, =OS_Terminate
}
// clang-format on

void WmspPxiCallback(int tag, u16 *request, BOOL error) {
    if (!error && !__VENEER_OS_SendMessage(&wmspW.pxiQueue, request, OS_MESSAGE_NOBLOCK) && wmspW.arm9Buf) {
        // The command queue is full.
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = request[0];
        result[1] = WM_ERRCODE_FIFO_ERROR;
        result[2] = 0;
        result[3] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(result);
    }
}

BOOL WMSP_CheckMacAddress(const u8 *address) {
    u8 *own = wmspW.status->macAddress;
    return address[0] == own[0] && address[1] == own[1] && address[2] == own[2] && address[3] == own[3] && address[4] == own[4] && address[5] == own[5];
}

void WMSP_CopyParentParam(u8 *dest, const u8 *src) {
    u16 length;
    *(u32 *)(dest + 4) = *(u32 *)(src + 8);
    *(u16 *)(dest + 8) = *(u16 *)(src + 12);
    dest[11] = (*(u16 *)(src + 14) ? 1 : 0) | (*(u16 *)(src + 18) ? 2 : 0) | (*(u16 *)(src + 20) ? 4 : 0);
    dest[10] = *(u16 *)(src + 4);
    *(u16 *)(dest + 0) = 1;
    dest[2] = 1;
    dest[3] = 0;
    *(u16 *)(dest + 12) = *(u16 *)(src + 52);
    if (*(u16 *)(src + 18) && *(u16 *)(src + 54) >= 8) {
        length = 8;
    } else {
        length = *(u16 *)(src + 54);
    }
    *(u16 *)(dest + 14) = length;
    if (dest[10]) {
        __VENEER_MI_CpuCopy8(*(void *const *)src, dest + 16, (dest[10] + 1) & ~1);
    }
}

// clang-format off
asm void __VENEER_MI_CpuCopy8(const void *src, void *dest, u32 size) {
    ldr pc, =MI_CpuCopy8
}
// clang-format on

BOOL WMSP_SetAllParams(u32 command, u8 *buf) {
    WmspStatus *status = wmspW.status;
    u16 error;
    WmspWlParams *params = (WmspWlParams *)buf;
    __VENEER_MI_CpuCopy8(status->macAddress, params->macAddress, 6);
    params->unk_16 = 7;
    params->unk_18 = status->enableChannel;
    params->unk_1e = status->rate;
    params->unk_1c = status->mode;
    if (!status->wepEnabled) {
        params->wepMode = 0;
        params->wepKeyId = 0;
        __VENEER_MIi_CpuClear16(0, params->wepKey, 0x50);
        params->wepEnabled = 0;
    } else {
        params->wepMode = status->wepMode;
        params->wepKeyId = status->wepKeyId;
        __VENEER_MI_CpuCopy8(status->wepKey, params->wepKey, 0x50);
        params->wepEnabled = 1;
    }
    params->unk_74 = 1;
    params->unk_76 = 1;
    params->unk_78 = status->mode == 1 ? 0 : 16;
    params->unk_7a = 10;
    if (command == 0x26) {
        __VENEER_MIi_CpuClear16(0, params->unk_7c, 32);
    } else {
        __VENEER_MIi_CpuClear16(0, params->unk_7c, 8);
        __VENEER_MIi_CpuClear16(0xffff, params->unk_7c + 4, 24);
    }
    params->unk_9c = status->preamble;
    error = WMSP_WL_ParamSetAll(buf)[2];
    if (!error) {
        return TRUE;
    }
    {
        u16 *result = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        result[0] = command;
        result[1] = WM_ERRCODE_FAILED;
        result[2] = 0x200;
        result[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(result);
    }
    return FALSE;
}

// clang-format off
asm void __VENEER_MIi_CpuClear16(u16 data, void *dest, u32 size) {
    ldr pc, =MIi_CpuClear16
}
// clang-format on

#pragma opt_common_subs off

void WMSP_AddRssiToList(u32 rssi) {
    u32 x;
    wmspW.rssiList[wmspW.rssiIndex] = rssi;
    wmspW.rssiIndex = (wmspW.rssiIndex + 1) & 31;
    // Also stirs the RSSI into the random seed at 0x027FFF98.
    x = rssi ^ ((u32) * (volatile u16 *)0x027fff98 << 1);
    *(volatile u16 *)0x027fff98 = x ^ (x >> 16);
}

#pragma opt_common_subs on

void WMSP_FillRssiIntoList(u8 rssi) {
    int i;
    for (i = 0; i < 32; i++) {
        wmspW.rssiList[i] = rssi;
    }
    wmspW.rssiIndex = 0;
}

u32 WMSP_GetAverageLinkLevel(void) {
    int i;
    u32 sum = 0;
    for (i = 0; i < 32; i++) {
        sum += wmspW.rssiList[i];
    }
    return WMSP_GetLinkLevel(sum >> 5);
}

u32 WMSP_GetLinkLevel(u32 rssi) {
    if (wmspW.arm9Buf->unk_53 == 8) {
        if (rssi < 22) {
            return 0;
        }
        if (rssi < 28) {
            return 1;
        }
        if (rssi < 34) {
            return 2;
        }
        return 3;
    } else {
        if (rssi < 8) {
            return 0;
        }
        if (rssi < 14) {
            return 1;
        }
        if (rssi < 20) {
            return 2;
        }
        return 3;
    }
}

#pragma opt_common_subs off

void WMSP_SetThreadPriorityLow(void) {
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    __VENEER_OS_DisableScheduler();
    __VENEER_OS_SetThreadPriority(&wmspRequestThread, wmspW.priorities.requestLow);
    __VENEER_OS_SetThreadPriority(WL_GetThreadStruct(), wmspW.priorities.wlLow);
    __VENEER_OS_SetThreadPriority(&wmspIndicateThread, wmspW.priorities.indicateLow);
    __VENEER_OS_EnableScheduler();
    __VENEER_OS_RestoreInterrupts(irq);
}

#pragma opt_common_subs on

// clang-format off
asm OSIntrMode __VENEER_OS_DisableInterrupts(void) {
    ldr pc, =OS_DisableInterrupts
}
// clang-format on

// clang-format off
asm u32 __VENEER_OS_DisableScheduler(void) {
    ldr pc, =OS_DisableScheduler
}
// clang-format on

// clang-format off
asm BOOL __VENEER_OS_SetThreadPriority(OSThread *thread, u32 prio) {
    ldr pc, =OS_SetThreadPriority
}
// clang-format on

// clang-format off
asm u32 __VENEER_OS_EnableScheduler(void) {
    ldr pc, =OS_EnableScheduler
}
// clang-format on

// clang-format off
asm OSIntrMode __VENEER_OS_RestoreInterrupts(OSIntrMode state) {
    ldr pc, =OS_RestoreInterrupts
}
// clang-format on

void WMSP_SetThreadPriorityHigh(void) {
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    __VENEER_OS_DisableScheduler();
    __VENEER_OS_SetThreadPriority(&wmspIndicateThread, wmspW.priorities.indicateHigh);
    __VENEER_OS_SetThreadPriority(WL_GetThreadStruct(), wmspW.priorities.wlHigh);
    __VENEER_OS_SetThreadPriority(&wmspRequestThread, wmspW.priorities.requestHigh);
    __VENEER_OS_EnableScheduler();
    __VENEER_OS_RestoreInterrupts(irq);
}

OSThread *WMSP_GetRequestThread(void) {
    return &wmspRequestThread;
}

OSThread *WMSP_GetIndicateThread(void) {
    return &wmspIndicateThread;
}

void *WMSP_GetInternalRequestBuf(void) {
    u8 *result = NULL;
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    u8 *p = (u8 *)wmspW.arm9Buf;
    if (p) {
        int i;
        for (i = 0; i < 32; i++) {
            if (*(u32 *)(p + i * 16 + 0xd0) & 0x8000) {
                p += 0xd0;
                result = p + i * 16;
                *(u32 *)(p + i * 16) &= ~0x8000;
                break;
            }
        }
    }
    __VENEER_OS_RestoreInterrupts(irq);
    return result;
}

void WMSP_ResetSizeVars(void) {
    WmspStatus *status = wmspW.status;
    status->sendSize = 0;
    status->recvSize = 0;
    status->parentSize = 0;
    status->childSize = 0;
    status->sendMaxSize = 0;
    status->recvMaxSize = 0;
    status->parentMaxSize = 0;
    status->childMaxSize = 0;
}

void WMSP_SetParentMaxSize(u16 size) {
    WmspStatus *status = wmspW.status;
    if (size > 512) {
        size = 512;
    }
    status->parentSize = size;
    status->parentMaxSize = size;
    if (status->aid == 0) {
        u16 x = size + 4;
        status->sendMaxSize = x;
        status->sendSize = x;
    } else {
        u16 x = size + 4;
        status->recvMaxSize = x;
        status->recvSize = x;
    }
}

void WMSP_SetChildMaxSize(u16 size) {
    WmspStatus *status = wmspW.status;
    if (size > 512) {
        size = 512;
    }
    status->childMaxSize = size;
    status->childSize = size;
    if (status->aid == 0) {
        u16 x = size + 2;
        status->recvMaxSize = x;
        status->recvSize = x;
    } else {
        u16 x = size + 2;
        status->sendMaxSize = x;
        status->sendSize = x;
    }
}

void WMSP_SetParentSize(u32 size) {
    WmspStatus *status = wmspW.status;
    status->parentSize = size;
    if (status->aid == 0) {
        status->sendSize = size + 4;
    } else {
        status->recvSize = size + 4;
    }
}

void WMSP_SetChildSize(u32 size) {
    WmspStatus *status = wmspW.status;
    status->childSize = size;
    if (status->aid == 0) {
        status->recvSize = size + 2;
    } else {
        status->sendSize = size + 2;
    }
}

#pragma dont_inline reset
