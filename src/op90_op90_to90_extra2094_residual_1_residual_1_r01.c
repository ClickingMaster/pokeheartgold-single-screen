#pragma force_active on
#include "global.h"
u32 ov74_02231264(void);
// @fn ov74_02231264
u32 ov74_02231264(void) {
    u16 v = reg_GX_VCOUNT;
    u64 t = OS_GetTick();
    u32 x = t + v;
    return (x & ~1) + 1;
}
