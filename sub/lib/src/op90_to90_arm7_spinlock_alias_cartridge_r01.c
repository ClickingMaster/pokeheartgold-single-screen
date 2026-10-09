#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_UnlockCartridge(u16 lockID);
void OS_UnLockCartridge(u16 lockID);
// @fn OS_UnLockCartridge
asm void OS_UnLockCartridge(u16 lockID)
{
    ldr r1, =OS_UnlockCartridge
    bx r1
}
