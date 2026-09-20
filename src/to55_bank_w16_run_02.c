#include "to55_bank_w16_private.h"

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov07_0222150C(void *a, void *p) {
    int count = ov07_02221CB4();
    int i;
    for (i = 0; i < count; i++) {
        int mask = ov07_02221C8C(i);
        if ((U32(p, 0x1c) & mask) == mask) {
            ov07_02221C74(i, a, p);
        }
    }
}
