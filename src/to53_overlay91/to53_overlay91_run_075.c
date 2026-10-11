#include "private_w20.h"

void ov91_02261070(u8 *p, u32 unused, void *arg) {
    int i = 0;
    u8 *q = p + 0x198;
    for (; i < 3; i++, q += 0x14) {
        ModelAnimation_Free(q, arg);
    }
    i = 0;
    q = p + 0x168;
    for (; i < 3; i++, q += 0x10) {
        ModelResource_Free(q);
    }
}
