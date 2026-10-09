#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_AllocFromHeap(void);
void __VENEER_OS_AllocFromHeap(void);
// @fn __VENEER_OS_AllocFromHeap
asm void __VENEER_OS_AllocFromHeap(void)
{
    ldr pc, =OS_AllocFromHeap
}
