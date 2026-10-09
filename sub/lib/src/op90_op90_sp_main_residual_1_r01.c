#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_GetCRC16(void);
void __Veneer_SVC_GetCRC16(void);
void SVC_Halt(void);
void __Veneer_SVC_Halt(void);
// @fn __Veneer_SVC_GetCRC16
asm void __Veneer_SVC_GetCRC16(void)
{
    ldr r12, =SVC_GetCRC16
    bx r12
}
// @fn __Veneer_SVC_Halt
asm void __Veneer_SVC_Halt(void)
{
    ldr r12, =SVC_Halt
    bx r12
}
