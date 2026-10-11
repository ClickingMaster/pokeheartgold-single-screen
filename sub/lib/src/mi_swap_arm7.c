#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u32 MI_SwapWord(u32 setData, volatile u32 *destp);

// clang-format off
asm u32 MI_SwapWord(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
// clang-format on
