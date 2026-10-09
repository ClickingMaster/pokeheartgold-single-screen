#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void _s32_div_f(void);
void __VENEER__s32_div_f(void);
// @fn __VENEER__s32_div_f
asm void __VENEER__s32_div_f(void)
{
    ldr pc, =_s32_div_f
}
