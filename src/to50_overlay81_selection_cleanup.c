#include "to50_overlay81_selection_cleanup_internal.h"
#include "unk_02019BA4.h"

void ov81_0223FB3C(u8 *work) {
    u32 offset = 0x3C8;
    u32 zero = 0;

    work[0x13] &= ~4;
    work[0x11]--;
    *(u16 *)(work + offset) = zero;
    *(u32 *)(work + offset + 0xA0) = zero;
    GridInputHandler_SetNextInput(*(GridInputHandler **)(work + offset + 0x9C), zero);
}
