#include <nitro/types.h>

#include <nitro/pxi/fifo_shared.h>

#include "wmsp_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// ARM7 side of the wireless-driver loader (WVR): the ARM9 asks over PXI to start or stop the wireless
// manager; a helper thread copies the driver image into EXT_WRAM, starts WMSP and later shuts it down.

#define REG_POWCNT2  (*(volatile u16 *)0x04000304)
#define POWCNT2_WIFI 2

#define WVR_COMMAND_START          0x10000
#define WVR_COMMAND_TERMINATE      0x20000
#define WVR_RESULT_ILLEGAL_STATE   5
#define WVR_RESULT_INVALID_COMMAND 7

enum {
    WVR_STATE_IDLE = 1,
    WVR_STATE_STARTING,
    WVR_STATE_ACTIVE,
    WVR_STATE_TERMINATING,
};

typedef struct WvrStatus {
    u8 state;
    u8 unk_01[3];
    void *image; // driver image copied to EXT_WRAM (0x06000000), or NULL
    void *callback;
    u32 imageSize;
} WvrStatus;

// Parameter block of WMSP_Init.
typedef struct WvrInitParam {
    void *work;
    void *stack;
    u32 stackSize;
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 unk_20;
    u32 unk_24;
    void *callback;
    void *staElement;
    u32 unk_30;
} WvrInitParam;

extern u8 WVR_BSS_START[]; // +0 initialized flag, +8 heap handle, +16 0xA4 bytes cleared by WVR_Init
extern WvrStatus wvrStatus;
extern OSThread wvrThread;
extern u8 wvrWlWork[];
extern u8 wvrWlStack[0x600];
extern u8 wvrWlStaElement[];

typedef void (*WvrPxiCallback)(int tag, u32 data, int error);

void PXI_SetFifoRecvCallback(int tag, void *callback);
int PXI_SendWordByFifo(int tag, u32 data, int error);
void PMi_SetLED(int led);
void OS_Terminate(void);
void WL_Terminate(void);

void WVR_Init(s32 heapHandle);
void WVR_Shutdown(void);
void WvrBegin(void *callback);
void __VENEER_WMSP_Init(WvrInitParam *param, u32 *config);
void WvrPxiWmSubstituteCallback(int tag, void *buf, int error);
int WvrCheckThreadRunning(OSThread *thread);
void WvrPxiReceiveCallback(int tag, u32 word, int error);
void WvrStartUpThread(void *arg);
void WvrTerminateThread(void *arg);
void __VENEER_WMSP_CancelVAlarm(void);
void __VENEER_WMSP_CancelAllAlarms(void);
OSThread *__VENEER_WMSP_GetRequestThread(void);
void __VENEER_WL_Terminate(void);
OSThread *__VENEER_WMSP_GetIndicateThread(void);

#pragma force_active on

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void WVR_Init(s32 heapHandle) {
    *(s32 *)(WVR_BSS_START + 8) = heapHandle;
    *WVR_BSS_START = 1;
    MI_CpuFill8(WVR_BSS_START + 16, 0, 0xA4);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, WvrPxiReceiveCallback);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WvrPxiWmSubstituteCallback);
}

void WVR_Shutdown(void) {
    REG_POWCNT2 &= ~POWCNT2_WIFI;
    PM_SetLEDPattern(1);
    PMi_SetLED(1);
}

void WvrBegin(void *callback) {
    WvrInitParam param;
    u32 config[7];
    param.work = wvrWlWork;
    param.stack = wvrWlStack + sizeof(wvrWlStack);
    param.stackSize = sizeof(wvrWlStack);
    param.unk_0c = 4;
    param.unk_20 = 0;
    param.unk_24 = 8;
    param.staElement = wvrWlStaElement;
    param.unk_30 = 0x1C0;
    config[0] = 3;
    param.unk_1c = 0x40;
    config[2] = 3;
    config[6] = 4;
    config[4] = 5;
    config[1] = 7;
    config[5] = 8;
    config[3] = 9;
    param.callback = callback;
    __VENEER_WMSP_Init(&param, config);
}

// clang-format off
asm void __VENEER_WMSP_Init(WvrInitParam *param, u32 *config)
{
    ldr pc, =WMSP_Init
}
// clang-format on

// Answers every WM request of the ARM9 while the wireless manager is not running.
void WvrPxiWmSubstituteCallback(int tag, void *buf, int error) {
    u16 *p = buf;
    u16 command = *p;
    if (p) {
        MIi_CpuClear32(0, (u32 *)p, 0x100);
        p[0] = command;
        p[1] = WM_ERRCODE_WM_DISABLE;
        while (PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)p, 0) < 0) {}
    }
}

int WvrCheckThreadRunning(OSThread *thread) {
    OSThread *p;
    if (!thread) {
        return 0;
    }
    for (p = OSi_ThreadInfo.list; p; p = p->next) {
        if (p == thread) {
            switch (thread->state) {
            case OS_THREAD_STATE_WAITING:
            case OS_THREAD_STATE_READY:
                return 1;
            case OS_THREAD_STATE_TERMINATED:
            default:
                return 0;
            }
        }
    }
    return 0;
}

void WvrPxiReceiveCallback(int tag, u32 word, int error) {
    u32 result;
    int ok;
    OSIntrMode irq;
    switch (word) {
    case WVR_COMMAND_START:
        irq = OS_DisableInterrupts();
        if (WvrCheckThreadRunning(&wvrThread)) {
            OS_RestoreInterrupts(irq);
            ok = 0;
        } else if (wvrStatus.state != WVR_STATE_IDLE) {
            OS_RestoreInterrupts(irq);
            ok = 0;
        } else {
            wvrStatus.state = WVR_STATE_STARTING;
            OS_RestoreInterrupts(irq);
            OS_CreateThread(&wvrThread, WvrStartUpThread, 0, wvrWlStack, 0x400, 10);
            OS_WakeupThreadDirect(&wvrThread);
            ok = 1;
        }
        if (ok) {
            return;
        }
        result = word | WVR_RESULT_ILLEGAL_STATE;
        break;
    case WVR_COMMAND_TERMINATE:
        irq = OS_DisableInterrupts();
        if (WvrCheckThreadRunning(&wvrThread)) {
            OS_RestoreInterrupts(irq);
            ok = 0;
        } else if (wvrStatus.state != WVR_STATE_ACTIVE) {
            OS_RestoreInterrupts(irq);
            ok = 0;
        } else {
            wvrStatus.state = WVR_STATE_TERMINATING;
            OS_RestoreInterrupts(irq);
            PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WvrPxiWmSubstituteCallback);
            OS_CreateThread(&wvrThread, WvrTerminateThread, 0, wvrWlStack, 0x400, 2);
            OS_WakeupThreadDirect(&wvrThread);
            ok = 1;
        }
        if (ok) {
            return;
        }
        result = word | WVR_RESULT_ILLEGAL_STATE;
        break;
    default:
        result = word | WVR_RESULT_INVALID_COMMAND;
        break;
    }
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, result, 0) < 0) {}
}

void WvrStartUpThread(void *arg) {
    if (wvrStatus.image) {
        MIi_CpuCopyFast(wvrStatus.image, (u32 *)0x06000000, wvrStatus.imageSize);
    }
    WvrBegin(wvrStatus.callback);
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_COMMAND_START, 0) < 0) {}
    OS_DisableInterrupts();
    wvrStatus.state = WVR_STATE_ACTIVE;
    OS_ExitThread();
}

// wmspW.arm9Buf, read as volatile.
#define ARM9_BUF(w) (*(WmspArm9Buf *volatile *)&(w)->arm9Buf)

void WvrTerminateThread(void *arg) {
    WmspWork *w = &wmspW;
    OSMessage msg;
    WmspArm9Buf *arm9Buf;
    int a, b, c;
    if (ARM9_BUF(w) && ARM9_BUF(w)->status->state) {
        __VENEER_WMSP_CancelVAlarm();
        __VENEER_WMSP_CancelAllAlarms();
    }
    for (;;) {
        if (!OS_ReceiveMessage(&w->pxiQueue, &msg, OS_MESSAGE_NOBLOCK)) {
            break;
        }
        if (ARM9_BUF(w)) {
            u16 *buf = WMSP_GetBuffer4Callback2Wm9();
            MIi_CpuClear32(0, (u32 *)buf, 0x100);
            buf[0] = *(u16 *)msg;
            buf[1] = WM_ERRCODE_WM_DISABLE;
            WMSP_ReturnResult2Wm9(buf);
            *(u16 *)msg |= 0x8000;
        }
    }
    if (WvrCheckThreadRunning(__VENEER_WMSP_GetRequestThread())) {
        OS_SendMessage(&w->pxiQueue, 0, OS_MESSAGE_BLOCK);
        OS_JoinThread(__VENEER_WMSP_GetRequestThread());
    }
    a = OS_ReceiveMessage(&w->pxiQueue, &msg, OS_MESSAGE_NOBLOCK);
    b = OS_ReceiveMessage(&w->replyQueue, &msg, OS_MESSAGE_NOBLOCK);
    c = OS_ReceiveMessage(&w->requestQueue, &msg, OS_MESSAGE_NOBLOCK);
    if (a | b | c) {
        OS_Terminate();
    }
    __VENEER_WL_Terminate();
    while (OS_ReceiveMessage(&w->indicateQueue, &msg, OS_MESSAGE_NOBLOCK)) {}
    if (WvrCheckThreadRunning(__VENEER_WMSP_GetIndicateThread())) {
        OS_SendMessage(&w->indicateQueue, 0, OS_MESSAGE_BLOCK);
        OS_JoinThread(__VENEER_WMSP_GetIndicateThread());
    }
    PM_SetLEDPattern(1);
    PMi_SetLED(1);
    arm9Buf = ARM9_BUF(w);
    if (arm9Buf && arm9Buf->status->state) {
        arm9Buf->status->state = 0;
    }
    if (wvrStatus.image) {
        MIi_CpuClearFast(0, (u32 *)0x06000000, wvrStatus.imageSize);
    }
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_COMMAND_TERMINATE, 0) < 0) {}
    OS_DisableInterrupts();
    wvrStatus.state = WVR_STATE_IDLE;
    OS_ExitThread();
}

#pragma dont_inline reset

// Veneers to the wireless manager in EXT_WRAM.
// clang-format off
asm void __VENEER_WMSP_CancelVAlarm(void)
{
    ldr pc, =WMSP_CancelVAlarm
}
// clang-format on

// clang-format off
asm void __VENEER_WMSP_CancelAllAlarms(void)
{
    ldr pc, =WMSP_CancelAllAlarms
}
// clang-format on

// clang-format off
asm OSThread *__VENEER_WMSP_GetRequestThread(void)
{
    ldr pc, =WMSP_GetRequestThread
}
// clang-format on

// clang-format off
asm void __VENEER_WL_Terminate(void)
{
    ldr pc, =WL_Terminate
}
// clang-format on

// clang-format off
asm OSThread *__VENEER_WMSP_GetIndicateThread(void)
{
    ldr pc, =WMSP_GetIndicateThread
}
// clang-format on
