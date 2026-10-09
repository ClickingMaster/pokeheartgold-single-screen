#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MATH_CountPopulation(void);
void __VENEER_MATH_CountPopulation(void);
// @fn __VENEER_MATH_CountPopulation
asm void __VENEER_MATH_CountPopulation(void)
{
    ldr pc, =MATH_CountPopulation
}
