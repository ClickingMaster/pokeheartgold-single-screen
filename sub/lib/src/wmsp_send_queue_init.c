#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Links all 32 send entries into the free queue and empties the other queues.
void WMSP_InitSendQueue(void) {
    WmspStatus *status = wmspW.status;
    u16 i;
    __VENEER_OS_LockMutex(&status->sendQueueMutex);
    __VENEER_MIi_CpuClear16(0, status->sendEntry, sizeof(status->sendEntry));
    for (i = 0; i < 31; i++) {
        status->sendEntry[i].next = i + 1;
    }
    status->sendEntry[i].next = 0xffff;
    status->sendFreeQueue.head = 0;
    status->sendFreeQueue.tail = i;
    for (i = 0; i < 4; i++) {
        status->sendQueue[i].head = 0xffff;
        status->sendQueue[i].tail = 0xffff;
        status->unk_6fc[i].head = 0xffff;
        status->unk_6fc[i].tail = 0xffff;
    }
    __VENEER_OS_UnlockMutex(&status->sendQueueMutex);
}

// clang-format off
asm void __VENEER_OS_LockMutex(OSMutex *mutex) {
    ldr pc, =OS_LockMutex
}
// clang-format on

// clang-format off
asm void __VENEER_OS_UnlockMutex(OSMutex *mutex) {
    ldr pc, =OS_UnlockMutex
}
// clang-format on
