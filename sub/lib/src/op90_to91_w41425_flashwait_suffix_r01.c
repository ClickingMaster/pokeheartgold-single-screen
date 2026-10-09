#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void NVRAM_ReadStatusRegister(void);
void __VENEER_NVRAM_ReadStatusRegister(void);
void NVRAM_SoftwareReset(void);
void __VENEER_NVRAM_SoftwareReset(void);
// @fn __VENEER_NVRAM_ReadStatusRegister
asm void __VENEER_NVRAM_ReadStatusRegister(void)
{
    ldr pc, =NVRAM_ReadStatusRegister
}
// @fn __VENEER_NVRAM_SoftwareReset
asm void __VENEER_NVRAM_SoftwareReset(void)
{
    ldr pc, =NVRAM_SoftwareReset
}
