#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct T07 {
    u8 pad[0x48];
    u32 state;
    struct T07 *next;
} T07;

typedef struct S07 {
    u8 state, pad[3];
    void *copy;
    u32 begin, size;
} S07;

extern S07 wvrStatus;
extern T07 wvrThread;
extern u8 wvrWlStack[];
extern int PXI_SendWordByFifo(int, u32, int);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32), OS_ExitThread(void), OS_WakeupThreadDirect(T07 *), OS_CreateThread(T07 *, void (*)(void *), void *, void *, u32, int);
extern int WvrCheckThreadRunning(T07 *);
extern void WvrStartUpThread(void *);
extern void WvrTerminateThread(void *);
extern void WvrPxiWmSubstituteCallback(int, void *, int);
extern void MIi_CpuClear32(u32, void *, u32);
extern void PM_SetLEDPattern(int);
extern void PMi_SetLED(int);

typedef struct {
    u8 first[0x28], indicate[0x30], other[0x30], request[0x154c - 0x88];
    u16 **volatile state;
} Work07;

extern Work07 wmspW;
extern void __VENEER_WMSP_CancelVAlarm(void);
extern void __VENEER_WMSP_CancelAllAlarms(void);
extern void WMSP_ReturnResult2Wm9(void *);
extern void OS_JoinThread(T07 *);
extern void OS_Terminate(void);
extern void __VENEER_WL_Terminate(void);
extern void MIi_CpuClearFast(u32, void *, u32);
extern T07 *__VENEER_WMSP_GetRequestThread(void);
extern T07 *__VENEER_WMSP_GetIndicateThread(void);
extern u16 *WMSP_GetBuffer4Callback2Wm9(void);
extern int OS_ReceiveMessage(void *, void **, int);
extern int OS_SendMessage(void *, void *, int);

void WvrTerminateThread(void *arg);

void WvrTerminateThread(void *arg) {
    Work07 *w = &wmspW;
    void *msg;
    u16 **state;
    int a, b, c;
    if (w->state && **w->state) {
        __VENEER_WMSP_CancelVAlarm();
        __VENEER_WMSP_CancelAllAlarms();
    }
    for (;;) {
        if (!OS_ReceiveMessage(w->request, &msg, 0)) {
            break;
        }
        if (w->state) {
            u16 *buf = WMSP_GetBuffer4Callback2Wm9();
            MIi_CpuClear32(0, buf, 0x100);
            buf[0] = *(u16 *)msg;
            buf[1] = 4;
            WMSP_ReturnResult2Wm9(buf);
            *(u16 *)msg |= 0x8000;
        }
    }
    if (WvrCheckThreadRunning(__VENEER_WMSP_GetRequestThread())) {
        OS_SendMessage(w->request, 0, 1);
        OS_JoinThread(__VENEER_WMSP_GetRequestThread());
    }
    a = OS_ReceiveMessage(w->request, &msg, 0);
    b = OS_ReceiveMessage(w->other, &msg, 0);
    c = OS_ReceiveMessage(w->first, &msg, 0);
    if (a | b | c) {
        OS_Terminate();
    }
    __VENEER_WL_Terminate();
    while (OS_ReceiveMessage(w->indicate, &msg, 0)) {}
    if (WvrCheckThreadRunning(__VENEER_WMSP_GetIndicateThread())) {
        OS_SendMessage(w->indicate, 0, 1);
        OS_JoinThread(__VENEER_WMSP_GetIndicateThread());
    }
    PM_SetLEDPattern(1);
    PMi_SetLED(1);
    state = w->state;
    if (state && **state) {
        **state = 0;
    }
    if (wvrStatus.copy) {
        MIi_CpuClearFast(0, (void *)0x6000000, wvrStatus.size);
    }
    while (PXI_SendWordByFifo(15, 0x20000, 0) < 0) {}
    OS_DisableInterrupts();
    wvrStatus.state = 1;
    OS_ExitThread();
}
