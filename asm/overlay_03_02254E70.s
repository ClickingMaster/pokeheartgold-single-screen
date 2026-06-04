	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_02254E70
ov03_02254E70: ; 0x02254E70
	push {r4, r5, r6, r7, lr}
	sub sp, #0x5c
	str r0, [sp]
	ldr r0, [r1, #0x68]
	str r1, [sp, #4]
	bl ov01_021F6B10
	cmp r0, #1
	beq _02254E84
	b _02255058
_02254E84:
	ldr r0, [sp, #4]
	bl ov03_02254C9C
	mov r7, #1
	ldr r4, _0225505C ; =ov03_022598A0
	mov r5, #0
	add r6, r7, #0
_02254E92:
	add r0, r5, #0
	bl sub_02034714
	cmp r0, #0
	beq _02254EB0
	cmp r5, #0
	beq _02254EB0
	ldr r0, [r4]
	add r0, r0, r5
	add r0, #0x80
	strb r6, [r0]
	ldr r0, [r4]
	add r0, #0x98
	strb r6, [r0]
	b _02254EFA
_02254EB0:
	add r0, r5, #0
	bl sub_02034818
	cmp r0, #0
	beq _02254ED4
	ldr r1, [r4]
	add r0, r1, r5
	add r0, #0x80
	ldrb r0, [r0]
	cmp r0, #0
	bne _02254ED4
	add r0, r1, r5
	add r0, #0x80
	strb r7, [r0]
	ldr r0, [r4]
	add r0, #0x98
	strb r7, [r0]
	b _02254EFA
_02254ED4:
	add r0, r5, #0
	bl sub_02034818
	cmp r0, #0
	bne _02254EFA
	ldr r1, [r4]
	add r0, r1, r5
	add r0, #0x80
	ldrb r0, [r0]
	cmp r0, #0
	beq _02254EFA
	add r1, r1, r5
	add r1, #0x80
	mov r0, #0
	strb r0, [r1]
	ldr r1, [r4]
	mov r0, #1
	add r1, #0x98
	strb r0, [r1]
_02254EFA:
	add r5, r5, #1
	cmp r5, #8
	blt _02254E92
	ldr r1, _0225505C ; =ov03_022598A0
	ldr r2, [r1]
	add r0, r2, #0
	add r0, #0x98
	ldrb r0, [r0]
	cmp r0, #0
	beq _02254F52
	mov r0, #0
	add r2, #0x98
	strb r0, [r2]
	ldr r0, [r1]
	ldr r0, [r0, #0x5c]
	bl RedrawListMenu
	ldr r0, _0225505C ; =ov03_022598A0
	mov r1, #3
	ldr r0, [r0]
	mov r5, #0
	ldr r0, [r0, #0x5c]
	bl ListMenuGetTemplateField
	cmp r0, #0
	bls _02254F52
	ldr r4, _0225505C ; =ov03_022598A0
	add r6, r5, #0
	mov r7, #3
_02254F34:
	ldr r0, [r4]
	lsl r2, r5, #0x18
	ldr r0, [r0, #0x5c]
	add r1, r6, #0
	lsr r2, r2, #0x18
	bl ov03_02254A54
	ldr r0, [r4]
	add r1, r7, #0
	ldr r0, [r0, #0x5c]
	add r5, r5, #1
	bl ListMenuGetTemplateField
	cmp r5, r0
	blo _02254F34
_02254F52:
	ldr r0, [sp, #4]
	ldr r0, [r0, #0x5c]
	bl ListMenu_ProcessInput
	add r4, r0, #0
	bl sub_02034780
	cmp r0, #0xff
	beq _02254F92
	bl sub_02034780
	ldr r1, [sp, #4]
	add r1, #0x95
	strb r0, [r1]
	ldr r0, _02255060 ; =0x0000060C
	bl PlaySE
	ldr r0, [sp, #4]
	mov r1, #1
	add r0, #0xa1
	strb r1, [r0]
	ldr r0, [sp, #4]
	ldr r1, [sp, #4]
	add r0, #0x95
	ldrb r0, [r0]
	bl ov03_02254D24
	ldr r0, _02255064 ; =ov03_02255714
	bl ov03_02254B4C
	add sp, #0x5c
	pop {r4, r5, r6, r7, pc}
_02254F92:
	bl sub_02037700
	cmp r0, #0
	beq _02254FB4
	bl ov03_02254B44
	ldr r0, [sp]
	ldr r1, [sp, #4]
	bl ov03_0225574C
	ldr r0, _0225505C ; =ov03_022598A0
	mov r1, #3
	ldr r0, [r0]
	add sp, #0x5c
	add r0, #0x88
	str r1, [r0]
	pop {r4, r5, r6, r7, pc}
_02254FB4:
	mov r0, #0
	mvn r0, r0
	cmp r4, r0
	bne _02254FEE
	bl ov03_02255B84
	cmp r0, #2
	ble _02254FDE
	ldr r0, _0225505C ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0xa2
	ldrh r0, [r0]
	cmp r0, #0
	bne _02254FD4
	mov r4, #0
	b _02254FEE
_02254FD4:
	cmp r0, #1
	bne _02254FEE
	mov r4, #1
	mvn r4, r4
	b _02254FEE
_02254FDE:
	ldr r0, _0225505C ; =ov03_022598A0
	ldr r0, [r0]
	add r0, #0xa2
	ldrh r0, [r0]
	cmp r0, #0
	bne _02254FEE
	mov r4, #1
	mvn r4, r4
_02254FEE:
	mov r0, #1
	mvn r0, r0
	cmp r4, r0
	beq _02254FFE
	add r0, r0, #1
	cmp r4, r0
	beq _02255058
	b _0225500E
_02254FFE:
	ldr r0, _02255068 ; =0x000005DC
	bl PlaySE
	ldr r0, _0225506C ; =ov03_0225558C
	bl ov03_02254B4C
	add sp, #0x5c
	pop {r4, r5, r6, r7, pc}
_0225500E:
	bl ov03_02255B70
	add r4, r0, #0
	bl sub_020347A0
	cmp r4, r0
	bgt _02255058
	ldr r3, _02255070 ; =ov03_02259184
	add r2, sp, #8
	mov r1, #0x29
_02255022:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _02255022
	ldr r0, _02255068 ; =0x000005DC
	bl PlaySE
	bl sub_020347A0
	ldr r1, [sp, #4]
	add r1, #0x90
	strb r0, [r1]
	ldr r0, [sp, #4]
	add r0, #0x91
	str r0, [sp, #4]
	ldrb r0, [r0]
	lsl r1, r0, #1
	add r0, sp, #8
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
	ldr r0, _02255074 ; =ov03_022553C8
	bl ov03_02254B4C
_02255058:
	add sp, #0x5c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225505C: .word ov03_022598A0
_02255060: .word 0x0000060C
_02255064: .word ov03_02255714
_02255068: .word 0x000005DC
_0225506C: .word ov03_0225558C
_02255070: .word ov03_02259184
_02255074: .word ov03_022553C8
	thumb_func_end ov03_02254E70
	