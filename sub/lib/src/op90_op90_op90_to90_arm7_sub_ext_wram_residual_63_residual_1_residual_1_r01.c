#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MI_WaitDma(void);
void __VENEER_MI_WaitDma(void);
// @fn __VENEER_MI_WaitDma
asm void __VENEER_MI_WaitDma(void)
{
    ldr pc, =MI_WaitDma
}
