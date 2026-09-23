#include "global.h"

#pragma require_prototypes off
void ov45_0222DB98(void *, void *);

void ov45_0222C370(u8 *p, u32 value) {
    u32 args[2];
    args[0] = (u32)(p + 0xD4);
    args[1] = value;
    ov45_0222DB98(*(void **)(p + 4), args);
}

void ov45_0222C388(u8 *p) {
    memset(p, 0, 0x194);
    MIi_CpuClear32(0xFFFFFFFF, (u32 *)(p + 0x50), 0x6C);
}
