#include "to58_overlay91_small_private.h"
void ov91_0225CB64(u8 *base) {
    u8 out[0x14];
    if (ov91_0225D32C(*(void **)(base + 0x34), out) == 1) {
        do {
            ov91_0225CCA8(base, 0x18, out, 0x14);
        } while (ov91_0225D32C(*(void **)(base + 0x34), out) == 1);
    }
}

void ov91_0225CB98(u8 *base) {
    int value = ov91_0225D2E8(*(void **)(base + 0x34));
    if (value != 0 && value % 30 == 0 && *(u32 *)(base + 0x80) == 1) {
        u32 out = ov91_0225D37C(*(void **)(base + 0x34));
        if (!ov91_0225CCA8(base, 0x1c, &out, 4)) {
            GF_AssertFail();
        }
        *(u32 *)(base + 0x80) = 0;
    }
}
