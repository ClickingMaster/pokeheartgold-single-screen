	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_SetPeriodicAlarm
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_GetTick

.text

	arm_func_start SetupPeriodicTimeOut
SetupPeriodicTimeOut: ; 0x0600B674
	stmdb sp!, {r3, r4, r5, r6, lr}
	sub sp, sp, #0xc
	ldr r2, _0600B6FC ; =0x0380FFF4
	mov r5, r0
	ldr r0, [r2]
	mov r4, r1
	add r0, r0, #0x208
	add r0, r0, #0x400
	bl __VENEER_OS_CancelAlarm
	ldr r0, _0600B700 ; =0x000082EA
	mov r1, #0
	umull r3, r2, r5, r0
	mla r2, r5, r1, r2
	mov r5, r1
	mla r2, r5, r0, r2
	mov r6, r3, lsr #6
	orr r6, r6, r2, lsl #26
	bl __VENEER_OS_GetTick
	adds r0, r6, r0
	adc r2, r1, #0
	mov r1, r0
	str r4, [sp, #4]
	mov r0, r5
	str r0, [sp, #8]
	ldr r0, _0600B6FC ; =0x0380FFF4
	str r5, [sp]
	ldr r0, [r0]
	mov r3, r6
	add r0, r0, #0x208
	add r0, r0, #0x400
	bl __VENEER_OS_SetPeriodicAlarm
	add sp, sp, #0xc
	ldmia sp!, {r3, r4, r5, r6, lr}
	bx lr
	.align 2, 0
_0600B6FC: .word 0x0380FFF4
_0600B700: .word 0x000082EA
	arm_func_end SetupPeriodicTimeOut

	arm_func_start __VENEER_OS_SetPeriodicAlarm
__VENEER_OS_SetPeriodicAlarm: ; 0x0600B704
	ldr pc, _0600B708 ; =OS_SetPeriodicAlarm
	.align 2, 0
_0600B708: .word OS_SetPeriodicAlarm
	arm_func_end __VENEER_OS_SetPeriodicAlarm
