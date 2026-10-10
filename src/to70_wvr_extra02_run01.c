#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef void (*WvrAsyncCallback)(void *, int);

typedef struct {
    volatile u16 lock;
    volatile u16 bank;
    void *arg;
    WvrAsyncCallback cb;
} WvrState;

extern WvrState wvrLockId;
int WVR_StartUpAsync(int bank, WvrAsyncCallback cb, void *arg);
int WVR_TerminateAsync(WvrAsyncCallback cb, void *arg);
void WvrReceiveCallback(PXIFifoTag tag, u32 data, BOOL err);
void WvrDummyAsyncCallback(void *arg, int result);

int WVR_StartUpAsync(int bank, WvrAsyncCallback cb, void *arg) {
    OSIntrMode intr;
    PXI_Init();
    if (!PXI_IsCallbackReady(15, PXI_PROC_ARM7)) {
        return 2;
    }
    if (wvrLockId.lock == 0) {
        do {
            int id = OS_GetLockID();
            if (id == -3) {
                return 7;
            }
            wvrLockId.lock = id;
        } while (wvrLockId.lock == 0);
    }
    intr = OS_DisableInterrupts();
    if (wvrLockId.cb) {
        OS_RestoreInterrupts(intr);
        return 5;
    }
    if (wvrLockId.bank) {
        OS_RestoreInterrupts(intr);
        return 5;
    }
    switch (bank) {
    case 4:
        if (!OSi_TryLockVram(4, wvrLockId.lock)) {
            OS_RestoreInterrupts(intr);
            return 6;
        }
        wvrLockId.bank = 4;
        *(vu8 *)0x04000242 = 0x82;
        break;
    case 8:
        if (!OSi_TryLockVram(8, wvrLockId.lock)) {
            OS_RestoreInterrupts(intr);
            return 6;
        }
        wvrLockId.bank = 8;
        *(vu8 *)0x04000243 = 0x82;
        break;
    case 12:
        if (!OSi_TryLockVram(12, wvrLockId.lock)) {
            OS_RestoreInterrupts(intr);
            return 6;
        }
        wvrLockId.bank = 12;
        *(vu8 *)0x04000242 = 0x82;
        *(vu8 *)0x04000243 = 0x8a;
        break;
    default:
        OS_RestoreInterrupts(intr);
        return 3;
    }
    if (!PXI_IsCallbackReady(15, PXI_PROC_ARM9)) {
        PXI_SetFifoRecvCallback(15, WvrReceiveCallback);
    }
    if (!cb) {
        wvrLockId.cb = WvrDummyAsyncCallback;
    } else {
        wvrLockId.cb = cb;
    }
    wvrLockId.arg = arg;
    if (PXI_SendWordByFifo(15, 0x10000, FALSE) < 0) {
        OSi_UnlockVram(wvrLockId.bank, wvrLockId.lock);
        wvrLockId.bank = 0;
        wvrLockId.cb = 0;
        OS_RestoreInterrupts(intr);
        return 4;
    }
    OS_RestoreInterrupts(intr);
    return 1;
}

int WVR_TerminateAsync(WvrAsyncCallback cb, void *arg) {
    OSIntrMode intr;
    PXI_Init();
    if (!PXI_IsCallbackReady(15, PXI_PROC_ARM7)) {
        return 2;
    }
    intr = OS_DisableInterrupts();
    if (wvrLockId.cb) {
        OS_RestoreInterrupts(intr);
        return 5;
    }
    if (!PXI_IsCallbackReady(15, PXI_PROC_ARM9)) {
        PXI_SetFifoRecvCallback(15, WvrReceiveCallback);
    }
    if (!cb) {
        wvrLockId.cb = WvrDummyAsyncCallback;
    } else {
        wvrLockId.cb = cb;
    }
    wvrLockId.arg = arg;
    if (PXI_SendWordByFifo(15, 0x20000, FALSE) < 0) {
        wvrLockId.cb = 0;
        OS_RestoreInterrupts(intr);
        return 4;
    }
    OS_RestoreInterrupts(intr);
    return 1;
}

void WvrReceiveCallback(PXIFifoTag tag, u32 data, BOOL err) {
    WvrAsyncCallback cb = wvrLockId.cb;
    void *arg = wvrLockId.arg;
    int result = data & 255;
    switch (data & 0xffff0000) {
    case 0x10000:
        if (result == 7 && wvrLockId.bank && wvrLockId.lock) {
            OSi_UnlockVram(wvrLockId.bank, wvrLockId.lock);
            wvrLockId.bank = 0;
        }
        break;
    case 0x20000:
        if (result == 0 && wvrLockId.bank && wvrLockId.lock) {
            OSi_UnlockVram(wvrLockId.bank, wvrLockId.lock);
            wvrLockId.bank = 0;
        }
        PXI_SetFifoRecvCallback(15, 0);
        break;
    }
    if (cb) {
        wvrLockId.cb = 0;
        wvrLockId.arg = 0;
        cb(arg, result);
    }
}

void WvrDummyAsyncCallback(void *arg, int result) {
}
