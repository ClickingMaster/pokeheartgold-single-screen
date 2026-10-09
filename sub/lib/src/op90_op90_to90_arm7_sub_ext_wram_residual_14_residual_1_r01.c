#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void _u32_div_f(void);
void __VENEER__u32_div_f(void);
// @fn __VENEER__u32_div_f
asm void __VENEER__u32_div_f(void)
{
    ldr pc, =_u32_div_f
}
