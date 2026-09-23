#include "global.h"

#pragma require_prototypes off

void ov45_0222BE5C(u8 *p) {
    if (*(u16 *)(p + 0x24) == 1) {
        int v = *(s16 *)(p + 0x26) + 1;
        if (v <= 0x384) {
            *(u16 *)(p + 0x26) = v;
        }
    }
}

BOOL ov45_0222BE74(u8 *p) {
    if (*(u16 *)(p + 0x24) == 0) {
        return TRUE;
    }
    return *(s16 *)(p + 0x26) < 0x384;
}
