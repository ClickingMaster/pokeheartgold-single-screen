	.include "asm/macros.inc"
	.include "global.inc"

	.extern SVC_ResetSoundBias

.text

	arm_func_start __VENEER_SVC_ResetSoundBias
__VENEER_SVC_ResetSoundBias: ; 0x037FB6E8
	ldr ip, _037FB6F0 ; =SVC_ResetSoundBias
	bx ip
	.align 2, 0
_037FB6F0: .word SVC_ResetSoundBias
	arm_func_end __VENEER_SVC_ResetSoundBias
