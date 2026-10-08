	.extern OS_Sleep
	.extern __VENEER_MATH_CountPopulation
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_LockMutex
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_UnlockMutex
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW
	.include "asm/macros.inc"
	.include "global.inc"

.text
	arm_func_start WmspGetTmptt
WmspGetTmptt: ; 0x06007DA4
	stmdb sp!, {r4, r5, r6, lr}
	mov r6, r1
	mov r5, r0
	tst r6, #0x8000
	ldrne r0, _06007E40 ; =0x00007FFF
	mov r4, r3
	andne r6, r6, r0
	addeq r0, r6, #0x1c
	moveq r0, r0, lsl #2
	addeq r6, r0, #0x66
	mov r0, r2
	bl __VENEER_MATH_CountPopulation
	mul r2, r6, r0
	ldr r1, _06007E44 ; =0x04000006
	add r0, r5, #0x22
	mov r3, r0, lsl #2
	ldrh r1, [r1]
	sub r0, r4, #2
	add r3, r3, #0x60
	add r2, r2, #0x388
	add r4, r3, r2
	subs r3, r0, r1
	bpl _06007E0C
_06007E00:
	add r0, r3, #7
	adds r3, r0, #0x100
	bmi _06007E00
_06007E0C:
	ldr r2, _06007E48 ; =0x66666667
	rsb r1, r3, r3, lsl #7
	mov r0, r1, lsr #0x1f
	smull r1, r3, r2, r1
	add r3, r0, r3, asr #3
	mov r0, #0xa
	mul r0, r3, r0
	cmp r0, r4
	movlo r3, #0
	mov r0, r3, lsl #0x10
	mov r0, r0, lsr #0x10
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_06007E40: .word 0x00007FFF
_06007E44: .word 0x04000006
_06007E48: .word 0x66666667
	arm_func_end WmspGetTmptt
