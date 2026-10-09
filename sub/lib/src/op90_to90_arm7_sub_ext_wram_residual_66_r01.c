#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SPI_Lock(void);
void __VENEER_SPI_Lock(void);
void NVRAM_ReadDataBytes(void);
void __VENEER_NVRAM_ReadDataBytes(void);
void SPI_Unlock(void);
void __VENEER_SPI_Unlock(void);
// @fn __VENEER_SPI_Lock
asm void __VENEER_SPI_Lock(void)
{
    ldr pc, =SPI_Lock
}
// @fn __VENEER_NVRAM_ReadDataBytes
asm void __VENEER_NVRAM_ReadDataBytes(void)
{
    ldr pc, =NVRAM_ReadDataBytes
}
// @fn __VENEER_SPI_Unlock
asm void __VENEER_SPI_Unlock(void)
{
    ldr pc, =SPI_Unlock
}
