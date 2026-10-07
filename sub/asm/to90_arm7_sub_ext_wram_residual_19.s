	.include "asm/macros.inc"
	.include "global.inc"

	.extern OS_CancelVAlarm
	.extern OS_SetVAlarm
	.extern WmspChildAdjustVSync2
	.extern WmspChildVAlarmMP
	.extern wmspVAlarm
	.extern wmspW

.text

	arm_func_start __VENEER_OS_CancelVAlarm
__VENEER_OS_CancelVAlarm: ; 0x06005DB0
	ldr pc, _06005DB4 ; =OS_CancelVAlarm
	.align 2, 0
_06005DB4: .word OS_CancelVAlarm
	arm_func_end __VENEER_OS_CancelVAlarm

	arm_func_start __VENEER_OS_SetVAlarm
__VENEER_OS_SetVAlarm: ; 0x06005DB8
	ldr pc, _06005DBC ; =OS_SetVAlarm
	.align 2, 0
_06005DBC: .word OS_SetVAlarm
	arm_func_end __VENEER_OS_SetVAlarm

	arm_func_start WmspChildAdjustVSync1
WmspChildAdjustVSync1: ; 0x06005DC0
	stmdb sp!, {r3, r4, lr}
	sub sp, sp, #4
	ldr r1, _06005EFC ; =wmspW + 0x1000
	ldr r2, _06005F00 ; =0x0380FFF0
	ldr r0, [r1, #0x550]
	ldrh r3, [r2]
	str r3, [r0, #0xd0]
	ldr r2, [r0, #0xd4]
	cmp r2, r3
	beq _06005EA0
	str r3, [r0, #0xd4]
	ldr r1, [r1, #0x550]
	ldr r3, _06005F04 ; =0x048080F8
	ldr r2, [r1, #0xd0]
	mov r2, r2, lsl #6
	str r2, [r1, #0xd0]
	ldrh r2, [r3]
	ldrh lr, [r3, #2]
	ldrh r4, [r3]
	cmp r2, r4
	ldrhih lr, [r3, #2]
	ldr r3, _06005F08 ; =0x04000006
	ldr r2, _06005F0C ; =0x00000107
	ldrh ip, [r3]
	ldr r3, _06005F10 ; =0x003FFFC0
	sub ip, r2, ip
	orr lr, r4, lr, lsl #16
	ldr r2, [r1, #0xd0]
	and lr, lr, r3
	rsb ip, ip, ip, lsl #7
	add ip, ip, lr, lsl #1
	and lr, r3, ip, lsr #1
	cmp r2, lr
	movhi r2, #0
	strhi r2, [r1, #0xd8]
	bhi _06005EA0
	mov ip, #1
	b _06005E90
_06005E58:
	ldr r2, [r1, #0xd0]
	add r2, r2, #0x4b
	add r3, r2, #0x4100
	str r3, [r1, #0xd0]
	cmp r3, lr
	bls _06005E8C
	ldr r2, _06005F14 ; =0x0000400E
	sub r3, r3, lr
	cmp r3, r2
	str r3, [r1, #0xd8]
	movhi r2, #0
	strhi r2, [r1, #0xd8]
	b _06005EA0
_06005E8C:
	add ip, ip, #1
_06005E90:
	cmp ip, #0x1e
	blt _06005E58
	mov r2, #0
	str r2, [r1, #0xd8]
_06005EA0:
	ldr r1, [r0, #0xd8]
	cmp r1, #0x7f
	bls _06005ECC
	ldr r0, _06005F18 ; =wmspVAlarm
	mov r1, #0xd0
	mov ip, #2
	ldr r3, _06005F1C ; =WmspChildAdjustVSync2
	add r2, r1, #0x37
	str ip, [sp]
	bl __VENEER_OS_SetVAlarm
	b _06005EF0
_06005ECC:
	mov r2, #1
	str r2, [r0, #0x1c]
	mov r1, #4
	str r1, [sp]
	ldrsh r1, [r0, #0x42]
	ldr r0, _06005F18 ; =wmspVAlarm
	ldr r3, _06005F20 ; =WmspChildVAlarmMP
	rsb r2, r2, #0x108
	bl __VENEER_OS_SetVAlarm
_06005EF0:
	add sp, sp, #4
	ldmia sp!, {r3, r4, lr}
	bx lr
	.align 2, 0
_06005EFC: .word wmspW + 0x1000
_06005F00: .word 0x0380FFF0
_06005F04: .word 0x048080F8
_06005F08: .word 0x04000006
_06005F0C: .word 0x00000107
_06005F10: .word 0x003FFFC0
_06005F14: .word 0x0000400E
_06005F18: .word wmspVAlarm
_06005F1C: .word WmspChildAdjustVSync2
_06005F20: .word WmspChildVAlarmMP
	arm_func_end WmspChildAdjustVSync1
