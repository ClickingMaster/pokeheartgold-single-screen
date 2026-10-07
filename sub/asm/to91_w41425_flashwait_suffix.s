	.include "asm/macros.inc"
	.include "global.inc"

	.extern NVRAM_ReadStatusRegister
	.extern NVRAM_SoftwareReset

.text



	arm_func_start __VENEER_NVRAM_ReadStatusRegister
__VENEER_NVRAM_ReadStatusRegister: ; 0x06017FEC
	ldr pc, _06017FF0 ; =NVRAM_ReadStatusRegister
	.align 2, 0
_06017FF0: .word NVRAM_ReadStatusRegister
	arm_func_end __VENEER_NVRAM_ReadStatusRegister

	arm_func_start __VENEER_NVRAM_SoftwareReset
__VENEER_NVRAM_SoftwareReset: ; 0x06017FF4
	ldr pc, _06017FF8 ; =NVRAM_SoftwareReset
	.align 2, 0
_06017FF8: .word NVRAM_SoftwareReset
	arm_func_end __VENEER_NVRAM_SoftwareReset
