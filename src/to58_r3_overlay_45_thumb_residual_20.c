#include "global.h"

#pragma require_prototypes off
void ov45_0222EF4C(int, int, void *, int);

void ov45_0222BE08(u8 *p, int arg, u16 val) {
    *(u16 *)(p + 0x1E2) = 2;
    *(u16 *)(p + 0x1E0) = val;
    ov45_0222EF4C(1, arg, p + 0x1E0, 4);
}

void ov45_0222BE28(u8 *p, int arg) {
    p[0x1C6] = 4;
    *(u16 *)(p + 0x1E2) = 4;
    ov45_0222EF4C(2, arg, p + 0x1E0, 4);
}
