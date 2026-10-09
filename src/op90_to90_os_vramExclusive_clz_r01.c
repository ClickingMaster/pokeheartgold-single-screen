#pragma force_active on
#include <nitro/types.h>
u32 OsCountZeroBits(register u32 bitmap);

#include <nitro/code32.h>

// @fn OsCountZeroBits
asm u32 OsCountZeroBits(register u32 bitmap) {
    clz r0, r0
    bx lr
}
