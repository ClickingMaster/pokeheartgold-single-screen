	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

thumb_func_start ov03_02254BEC
ov03_02254BEC: ; 0x02254BEC
	push {lr}
	sub sp, #0x54
	ldr r3, _02254C94 ; =ov03_0225931E
	add r2, sp, #0
	mov r1, #0x29
_02254BF6:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _02254BF6
	ldr r0, _02254C98 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x91
	ldrb r0, [r0]
	cmp r0, #0x29
	blo _02254C12
	bl GF_AssertFail
_02254C12:
	ldr r0, _02254C98 ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0x91
	ldrb r0, [r0]
	cmp r0, #8
	bne _02254C4E
	bl sub_020347A0
	cmp r0, #1
	ble _02254C3A
	ldr r0, _02254C98 ; =ov03_022598A0
	mov r1, #0
	ldr r0, [r0]
	add r0, #0x92
	ldrb r0, [r0]
	add r0, #0x7f
	bl ov03_02253E20
	add sp, #0x54
	pop {pc}
_02254C3A:
	ldr r0, _02254C98 ; =ov03_022598A0
	mov r1, #0
	ldr r0, [r0]
	add r0, #0x92
	ldrb r0, [r0]
	add r0, #0x75
	bl ov03_02253E20
	add sp, #0x54
	pop {pc}
_02254C4E:
	cmp r0, #0x28
	bne _02254C82
	bl sub_020347A0
	cmp r0, #1
	ble _02254C6E
	ldr r0, _02254C98 ; =ov03_022598A0
	mov r1, #0
	ldr r0, [r0]
	add r0, #0x92
	ldrb r0, [r0]
	add r0, #0x9a
	bl ov03_02253E20
	add sp, #0x54
	pop {pc}
_02254C6E:
	ldr r0, _02254C98 ; =ov03_022598A0
	mov r1, #0
	ldr r0, [r0]
	add r0, #0x92
	ldrb r0, [r0]
	add r0, #0x8e
	bl ov03_02253E20
	add sp, #0x54
	pop {pc}
_02254C82:
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
	add sp, #0x54
	pop {pc}
	nop
_02254C94: .word ov03_0225931E
_02254C98: .word ov03_022598A0
	thumb_func_end ov03_02254BEC