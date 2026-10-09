#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u64 x;
    u64 mul;
    u64 add;
} Ov00RandContext;
Ov00RandContext ov00_0221A76C;
void OS_GetMacAddress(u8 *mac);
u64 OS_GetTick(void);
u32 ov00_021ED5E4(u32 max);
// @fn ov00_021ED5E4
u32 ov00_021ED5E4(u32 max) {
    if (ov00_0221A76C.x == 0 && ov00_0221A76C.mul == 0 && ov00_0221A76C.add == 0) {
        u64 seed;
        OS_GetMacAddress((u8 *)&seed);
        seed = ((seed >> 24) & 0xffffff) | (OS_GetTick() << 24);
        ov00_0221A76C.x = seed;
        ov00_0221A76C.mul = 0x5D588B656C078965ULL;
        ov00_0221A76C.add = 0x269EC3;
    }
    ov00_0221A76C.x = ov00_0221A76C.mul * ov00_0221A76C.x + ov00_0221A76C.add;
    if (max == 0) {
        return (u32)(ov00_0221A76C.x >> 32);
    }
    return (u32)(((ov00_0221A76C.x >> 32) * max) >> 32);
}
