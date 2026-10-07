	.include "asm/macros.inc"
	.include "global.inc"

	.extern NVRAM_ReadDataBytes
	.extern SPI_Lock
	.extern SPI_Unlock

.text

	arm_func_start __VENEER_SPI_Lock
__VENEER_SPI_Lock: ; 0x060180AC
	ldr pc, _060180B0 ; =SPI_Lock
	.align 2, 0
_060180B0: .word SPI_Lock
	arm_func_end __VENEER_SPI_Lock

	arm_func_start __VENEER_NVRAM_ReadDataBytes
__VENEER_NVRAM_ReadDataBytes: ; 0x060180B4
	ldr pc, _060180B8 ; =NVRAM_ReadDataBytes
	.align 2, 0
_060180B8: .word NVRAM_ReadDataBytes
	arm_func_end __VENEER_NVRAM_ReadDataBytes

	arm_func_start __VENEER_SPI_Unlock
__VENEER_SPI_Unlock: ; 0x060180BC
	ldr pc, _060180C0 ; =SPI_Unlock
	.align 2, 0
_060180C0: .word SPI_Unlock
	arm_func_end __VENEER_SPI_Unlock
