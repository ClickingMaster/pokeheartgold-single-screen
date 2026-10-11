#include "overlay_80_022310C4_opus_partial_internal.h"
void ov80_02231930(void *save, u8 a, s32 b, u8 c);
u32 FrontierSave_SetStat(void *, u32, u32, u16);

void ov80_02231930(void *save, u8 a, s32 b, u8 c) {
    void *frontier;
    u32 stat;
    u16 lo;
    u16 hi;
    u8 packed;
    u8 rem;
    ov80_022318D0(save, a, b, &lo, &hi);
    packed = (u8)((hi << 4) | lo);
    rem = (u8)(b % 2);
    if (rem == 0) {
        packed &= 0xF0;
    } else {
        packed &= 0xF;
    }
    packed |= (u8)(c << (rem * 4));
    frontier = Save_Frontier_GetStatic(save);
    stat = sub_0205C0A0(a, b);
    FrontierSave_SetStat(frontier, stat, sub_0205C268(sub_0205C0A0(a, b)), packed);
}
