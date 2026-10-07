	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_Init
	.extern wvrWlStaElement
	.extern wvrWlWork

.text

	arm_func_start WvrBegin
WvrBegin: ; 0x03805A6C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	sub sp, sp, #0x50
	str r0, [sp, #0x44]
	ldr r0, _03805B04 ; =wvrWlWork
	mov sb, #4
	mov r7, #8
	mov r4, #3
	ldr r1, _03805B08 ; =wvrWlWork
	ldr r6, _03805B0C ; =wvrWlStaElement
	mov sl, #0x600
	mov r8, #0
	mov r5, #0x1c0
	mov lr, #0x40
	mov ip, #5
	mov r3, #7
	mov r2, #9
	str r0, [sp, #0x1c]
	str r1, [sp, #0x20]
	add r0, sp, #0x1c
	add r1, sp, #0
	str sl, [sp, #0x24]
	str sb, [sp, #0x28]
	str r8, [sp, #0x3c]
	str r7, [sp, #0x40]
	str r6, [sp, #0x48]
	str r5, [sp, #0x4c]
	str r4, [sp]
	str lr, [sp, #0x38]
	str r4, [sp, #8]
	str sb, [sp, #0x18]
	str ip, [sp, #0x10]
	str r3, [sp, #4]
	str r7, [sp, #0x14]
	str r2, [sp, #0xc]
	bl __VENEER_WMSP_Init
	add sp, sp, #0x50
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	.align 2, 0
_03805B04: .word wvrWlWork
_03805B08: .word wvrWlWork
_03805B0C: .word wvrWlStaElement
	arm_func_end WvrBegin

	arm_func_start __VENEER_WMSP_Init
__VENEER_WMSP_Init: ; 0x03805B10
	ldr pc, _03805B14 ; =WMSP_Init
	.align 2, 0
_03805B14: .word WMSP_Init
	arm_func_end __VENEER_WMSP_Init
