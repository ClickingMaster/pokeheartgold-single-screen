#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0221F5B0(void *work, int battler) {
    if (((u32) * (u8 *)((u8 *)work + battler * 0x50 + 0x1B) << 24) >> 31 == 0) {
        ov08_0221DF80(work, battler, battler, 0, 0x20);
    }
}
