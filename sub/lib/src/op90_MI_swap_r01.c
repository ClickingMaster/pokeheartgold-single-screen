#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
u32 MI_SwapWord(u32 setData, volatile u32 *destp);
// @fn MI_SwapWord
asm u32 MI_SwapWord(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
