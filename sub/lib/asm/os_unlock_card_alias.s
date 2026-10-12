	.include "asm/macros.inc"
	.extern OS_UnlockCard
	.public OS_UnLockCard

.text
	.type OS_UnLockCard, @function
	arm_func_start OS_UnLockCard
OS_UnLockCard:
	ldr r1, [pc, #0]
	bx r1
	.word OS_UnlockCard
	arm_func_end OS_UnLockCard
	.type $d, @function
