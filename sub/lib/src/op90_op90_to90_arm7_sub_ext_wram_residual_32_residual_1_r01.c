#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void _ll_udiv(void);
void __VENEER__ll_udiv(void);
// @fn __VENEER__ll_udiv
asm void __VENEER__ll_udiv(void)
{
    ldr pc, =_ll_udiv
}
