#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_LockMutex(void);
void __VENEER_OS_LockMutex(void);
void OS_UnlockMutex(void);
void __VENEER_OS_UnlockMutex(void);
// @fn __VENEER_OS_LockMutex
asm void __VENEER_OS_LockMutex(void)
{
    ldr pc, =OS_LockMutex
}
// @fn __VENEER_OS_UnlockMutex
asm void __VENEER_OS_UnlockMutex(void)
{
    ldr pc, =OS_UnlockMutex
}
