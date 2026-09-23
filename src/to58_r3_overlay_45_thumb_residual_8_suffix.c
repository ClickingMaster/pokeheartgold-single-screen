#include "global.h"
#pragma require_prototypes off
void GF_AssertFail(void);

void *ov45_0222A964(u8 *p, u32 index) {
    int seen, i;
    if (index >= 12) {
        GF_AssertFail();
    }
    seen = 0;
    for (i = 0; i < 12; i++) {
        u8 v = p[0x4C + i];
        if (v < 0x18) {
            seen++;
        }
        if (seen - 1 == index) {
            return *(void **)(p + 0x58 + i * 4);
        }
    }
    return (void *)-1;
}
