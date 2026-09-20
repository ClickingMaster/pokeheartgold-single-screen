#include "overlay112_014_ir_private.h"

void ov112_021E7398(void *unused) {
    for (;;) {
        OS_LockMutex(&ov112_021FFB08);
        if (ov112_021E5B98() == 1) {
            ov112_021E5E18(2);
        }
        OS_UnlockMutex(&ov112_021FFB08);
        OS_Sleep(4);
    }
}

void ov112_021E73C8(void *stack, u32 stackSize, u32 priority) {
    ov112_021FFAA4.field18 = 1;
    ov112_021FFAA4.field14 = 1;
    ov112_021FFAA4.field44 = 0;
    ov112_021FFAA4.field48 = 0;
    ov112_021E5964();
    ov112_021E5D5C(1);
    ov112_021E5D38(ov112_021E6A6C);
    ov112_021E5D44(ov112_021E6F60);
    ov112_021E5D50(ov112_021E6BDC);
    CARD_SpiWaitGetStatus();
    if (CARD_SpiWaitGetStatus() == 0xAA) {
        CARD_SetSpiWriteWaitCycles(0);
        CARD_SetSpiReadWaitCycles(0);
    }
    OS_InitMutex(&ov112_021FFB08);
    OS_InitMessageQueue(&ov112_021FFB20, ov112_021FFAC0, 1);
    OS_CreateThread(&ov112_021FFBD8, ov112_021E7398, NULL, (u8 *)stack + stackSize, stackSize, priority);
    OS_WakeupThreadDirect(&ov112_021FFBD8);
}
