#pragma force_active on
#include <nitro/types.h>
u32 MI_SwapWord(register u32 setData, register vu32 *destp);

#include <nitro/code32.h>

// @fn MI_SwapWord
asm u32 MI_SwapWord(register u32 setData, register vu32 *destp) {
    swp r0, r0, [r1]
    bx lr
}
