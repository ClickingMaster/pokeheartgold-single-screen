#include "to55_overlay91_merge_private.h"

void ov91_022600FC(u8 *p, void *arg) {
    int i = 0;
    u8 *q = p + 0x168;
    for (; i < 3; i++) {
        ModelResource_Free(q);
        q += 0x10;
    }
    {
        u8 *r;
        int j;
        j = 0;
        r = p + 0x198;
        for (; j < 6; j++) {
            ModelAnimation_Free(r, arg);
            r += 0x14;
        }
    }
    ModelAnimation_Free(p + 0x214, arg);
    memset(p, 0, 0x230);
}
