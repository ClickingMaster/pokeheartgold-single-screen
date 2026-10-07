	.include "asm/macros.inc"
	.include "global.inc"

	.extern SVC_SetSoundBias

.text

	arm_func_start __VENEER_SVC_SetSoundBias
__VENEER_SVC_SetSoundBias: ; 0x037FB744
	ldr ip, _037FB74C ; =SVC_SetSoundBias
	bx ip
	.align 2, 0
_037FB74C: .word SVC_SetSoundBias
	arm_func_end __VENEER_SVC_SetSoundBias
