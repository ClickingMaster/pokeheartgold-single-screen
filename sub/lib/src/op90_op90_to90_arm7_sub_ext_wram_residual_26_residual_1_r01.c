#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_IsThreadTerminated(void);
void __VENEER_OS_IsThreadTerminated(void);
void OS_JoinThread(void);
void __VENEER_OS_JoinThread(void);
// @fn __VENEER_OS_IsThreadTerminated
asm void __VENEER_OS_IsThreadTerminated(void)
{
    ldr pc, =OS_IsThreadTerminated
}
// @fn __VENEER_OS_JoinThread
asm void __VENEER_OS_JoinThread(void)
{
    ldr pc, =OS_JoinThread
}
