	.include "asm/macros.inc"
	.extern OS_UnlockCartridge
	.public OS_UnLockCartridge

.text
	.type OS_UnLockCartridge, @function
	arm_func_start OS_UnLockCartridge
OS_UnLockCartridge:
	ldr r1, [pc, #0]
	bx r1
	.word OS_UnlockCartridge
	arm_func_end OS_UnLockCartridge
	.type $d, @function
