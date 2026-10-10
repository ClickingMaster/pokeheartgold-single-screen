#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_GetCRC16(void);
void __Veneer_SVC_GetCRC16(void);
void SVC_Halt(void);
void __Veneer_SVC_Halt(void);
// @fn __Veneer_SVC_GetCRC16
void __Veneer_SVC_GetCRC16(void)
{
    SVC_GetCRC16();
}
// @fn __Veneer_SVC_Halt
void __Veneer_SVC_Halt(void)
{
    SVC_Halt();
}
