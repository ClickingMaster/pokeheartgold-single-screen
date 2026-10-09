#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_DisableInterrupts(void);
void __VENEER_OS_DisableInterrupts(void);
void OS_DisableScheduler(void);
void __VENEER_OS_DisableScheduler(void);
void OS_SetThreadPriority(void);
void __VENEER_OS_SetThreadPriority(void);
void OS_EnableScheduler(void);
void __VENEER_OS_EnableScheduler(void);
void OS_RestoreInterrupts(void);
void __VENEER_OS_RestoreInterrupts(void);
// @fn __VENEER_OS_DisableInterrupts
asm void __VENEER_OS_DisableInterrupts(void)
{
    ldr pc, =OS_DisableInterrupts
}
// @fn __VENEER_OS_DisableScheduler
asm void __VENEER_OS_DisableScheduler(void)
{
    ldr pc, =OS_DisableScheduler
}
// @fn __VENEER_OS_SetThreadPriority
asm void __VENEER_OS_SetThreadPriority(void)
{
    ldr pc, =OS_SetThreadPriority
}
// @fn __VENEER_OS_EnableScheduler
asm void __VENEER_OS_EnableScheduler(void)
{
    ldr pc, =OS_EnableScheduler
}
// @fn __VENEER_OS_RestoreInterrupts
asm void __VENEER_OS_RestoreInterrupts(void)
{
    ldr pc, =OS_RestoreInterrupts
}
