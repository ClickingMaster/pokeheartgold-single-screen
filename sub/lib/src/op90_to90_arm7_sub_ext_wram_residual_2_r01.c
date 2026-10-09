#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_FreeToHeap(void);
void __VENEER_OS_FreeToHeap(void);
// @fn __VENEER_OS_FreeToHeap
asm void __VENEER_OS_FreeToHeap(void)
{
    ldr pc, =OS_FreeToHeap
}
