	.include "asm/macros.inc"
	.include "global.inc"

	.extern __VENEER_OS_SetAlarm
	.extern __VENEER__ll_udiv

.text

	arm_func_start WaitLoop_Waitus
WaitLoop_Waitus: ; 0x0600EF54
	stmdb sp!, {r3, r4, r5, lr}
	sub sp, sp, #8
	ldr r2, _0600EFD4 ; =0x000082EA
	mov r3, #0
	umull lr, ip, r0, r2
	mla ip, r0, r3, ip
	mov r0, r3
	mla ip, r0, r2, ip
	mov r0, lr, lsr #6
	mov r4, #1
	mov r5, r1
	mov r1, ip, lsr #6
	orr r0, r0, ip, lsl #26
	mov r2, #0x3e8
	str r4, [sp, #4]
	bl __VENEER__ll_udiv
	add r3, sp, #4
	str r3, [sp]
	ldr r2, _0600EFD8 ; =0x0380FFF4
	mov r3, r5
	ldr r2, [r2]
	add ip, r2, #0x234
	mov r2, r1
	mov r1, r0
	add r0, ip, #0x400
	bl __VENEER_OS_SetAlarm
_0600EFBC:
	ldr r0, [sp, #4]
	cmp r0, #0
	bne _0600EFBC
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, lr}
	bx lr
	.align 2, 0
_0600EFD4: .word 0x000082EA
_0600EFD8: .word 0x0380FFF4
	arm_func_end WaitLoop_Waitus
