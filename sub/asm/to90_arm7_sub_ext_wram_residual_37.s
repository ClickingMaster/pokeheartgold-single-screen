	.include "asm/macros.inc"
	.include "global.inc"

	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_SetAlarm
	.extern __VENEER__ll_udiv

.text

	arm_func_start SetupTimeOut
SetupTimeOut: ; 0x0600B7AC
	stmdb sp!, {r3, r4, r5, lr}
	ldr r2, _0600B814 ; =0x0380FFF4
	mov r5, r0
	ldr r0, [r2]
	mov r4, r1
	add r0, r0, #0x234
	add r0, r0, #0x400
	bl __VENEER_OS_CancelAlarm
	mov r3, #0
	ldr r0, _0600B818 ; =0x000082EA
	mov r1, r3
	umull r2, ip, r5, r0
	mla ip, r5, r3, ip
	mla ip, r1, r0, ip
	mov r1, r2, lsr #6
	str r3, [sp]
	ldr r0, _0600B814 ; =0x0380FFF4
	mov r3, r4
	ldr r0, [r0]
	mov r2, ip, lsr #6
	add r0, r0, #0x234
	add r0, r0, #0x400
	orr r1, r1, ip, lsl #26
	bl __VENEER_OS_SetAlarm
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_0600B814: .word 0x0380FFF4
_0600B818: .word 0x000082EA
	arm_func_end SetupTimeOut

	arm_func_start SetupUsTimeOut
SetupUsTimeOut: ; 0x0600B81C
	stmdb sp!, {r3, r4, r5, lr}
	ldr r2, _0600B898 ; =0x0380FFF4
	mov r5, r0
	ldr r0, [r2]
	mov r4, r1
	add r0, r0, #0x234
	add r0, r0, #0x400
	bl __VENEER_OS_CancelAlarm
	ldr r0, _0600B89C ; =0x000082EA
	mov r3, #0
	umull ip, r2, r5, r0
	mla r2, r5, r3, r2
	mov r1, r3
	mla r2, r1, r0, r2
	mov r0, ip, lsr #6
	mov r1, r2, lsr #6
	orr r0, r0, r2, lsl #26
	mov r2, #0x3e8
	bl __VENEER__ll_udiv
	mov r2, r1
	mov r1, r0
	mov r0, #0
	str r0, [sp]
	ldr r0, _0600B898 ; =0x0380FFF4
	mov r3, r4
	ldr r0, [r0]
	add r0, r0, #0x234
	add r0, r0, #0x400
	bl __VENEER_OS_SetAlarm
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_0600B898: .word 0x0380FFF4
_0600B89C: .word 0x000082EA
	arm_func_end SetupUsTimeOut
