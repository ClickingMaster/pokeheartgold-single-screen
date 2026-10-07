#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
u64 FX_SinFx64c_internal(u64 x);
u64 FX_CosFx64c_internal(u64 x);
// @fn FX_SinFx64c_internal
u64 FX_SinFx64c_internal(u64 x) {
    u64 x2, r;
    if (x == 0x100000000ULL) {
        return 0xB504F334;
    }
    x2 = (x * x) >> 32;
    r = 0x100000000ULL - ((x2 * 0x02317888) >> 32);
    r = 0x100000000ULL - ((r * ((x2 * 0x03C2857C) >> 32)) >> 32);
    r = 0x100000000ULL - ((r * ((x2 * 0x07E54B84) >> 32)) >> 32);
    r = 0xC90FDAA2ULL - ((r * ((x2 * 0x14ABBCE6) >> 32)) >> 32);
    return (r * x) >> 32;
}
// @fn FX_CosFx64c_internal
u64 FX_CosFx64c_internal(u64 x) {
    u64 x2, r;
    if (x == 0x100000000ULL) {
        return 0xB504F334;
    }
    x2 = (x * x) >> 32;
    r = 0x100000000ULL - ((x2 * 0x02D1E41D) >> 32);
    r = 0x100000000ULL - ((r * ((x2 * 0x054387AD) >> 32)) >> 32);
    r = 0x100000000ULL - ((r * ((x2 * 0x0D28D331) >> 32)) >> 32);
    r = 0x100000000ULL - ((r * ((x2 * 0x4EF4F327) >> 32)) >> 32);
    return r;
}
