	.include "asm/macros.inc"
	.include "overlay_12_battle_controller.inc"
	.include "global.inc"

.text
	thumb_func_start ov12_022643C8
ov12_022643C8: ; 0x022643C8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r1, [sp, #8]
	ldr r1, [sp, #0x24]
	add r4, r2, #0
	str r1, [sp, #0x24]
	ldr r1, [sp, #0x28]
	str r0, [sp, #4]
	str r1, [sp, #0x28]
	mov r1, #0x16
	strb r1, [r4]
	add r1, sp, #0x10
	ldrh r1, [r1, #0x1c]
	strh r1, [r4, #2]
	ldr r1, [sp, #0x24]
	strh r1, [r4, #0x14]
	ldr r1, [sp, #0x28]
	strh r1, [r4, #0x16]
	ldr r1, [sp, #0x20]
	str r3, [r4, #0x4c]
	str r1, [r4, #0x50]
	bl BattleSystem_GetTerrainId
	str r0, [r4, #0x54]
	ldrh r1, [r4, #0xe]
	mov r0, #4
	bic r1, r0
	strh r1, [r4, #0xe]
	ldrh r1, [r4, #0xe]
	mov r0, #8
	bic r1, r0
	ldr r0, [sp, #8]
	strh r1, [r4, #0xe]
	cmp r0, #0
	bne _02264410
	b _0226459A
_02264410:
	ldr r1, _022645A0 ; =0x00002144
	ldr r0, [r0, r1]
	add r1, #0x10
	str r0, [r4, #4]
	ldr r0, [sp, #8]
	ldr r0, [r0, r1]
	cmp r0, #0
	beq _02264422
	b _02264430
_02264422:
	add r0, sp, #0x10
	ldrh r0, [r0, #0x1c]
	lsl r1, r0, #4
	ldr r0, [sp, #8]
	add r1, r0, r1
	ldr r0, _022645A4 ; =0x000003E1
	ldrb r0, [r1, r0]
_02264430:
	strh r0, [r4, #8]
	ldr r0, [sp, #0x24]
	mov r1, #0xc0
	mul r1, r0
	ldr r0, [sp, #8]
	mov r2, #8
	add r1, r0, r1
	ldr r0, _022645A8 ; =0x00002D75
	mov r3, #0
	ldrb r0, [r1, r0]
	ldr r1, [sp, #8]
	strh r0, [r4, #0xc]
	mov r0, #0xd
	str r0, [sp]
	ldr r0, [sp, #4]
	bl CheckAbilityActive
	cmp r0, #0
	bne _02264476
	mov r0, #0x4c
	str r0, [sp]
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	mov r2, #8
	mov r3, #0
	bl CheckAbilityActive
	cmp r0, #0
	bne _02264476
	mov r1, #6
	ldr r0, [sp, #8]
	lsl r1, r1, #6
	ldr r0, [r0, r1]
	str r0, [r4, #0x10]
	b _0226447A
_02264476:
	mov r0, #0
	str r0, [r4, #0x10]
_0226447A:
	ldr r1, _022645AC ; =0x00002164
	ldr r0, [sp, #8]
	ldr r0, [r0, r1]
	ldr r1, _022645B0 ; =0x00002DB0
	strh r0, [r4, #0xa]
	ldr r0, [sp, #8]
	add r2, r0, r1
	ldr r0, [sp, #0x24]
	mov r1, #0xc0
	mul r1, r0
	mov r0, #1
	ldr r3, [r2, r1]
	lsl r0, r0, #0x18
	tst r0, r3
	beq _0226449C
	mov r5, #1
	b _0226449E
_0226449C:
	mov r5, #0
_0226449E:
	ldrh r0, [r4, #0xe]
	mov r3, #1
	bic r0, r3
	lsl r3, r5, #0x10
	lsr r5, r3, #0x10
	mov r3, #1
	and r5, r3
	orr r0, r5
	strh r0, [r4, #0xe]
	ldr r1, [r2, r1]
	lsl r0, r3, #0x15
	tst r0, r1
	bne _022644BA
	mov r3, #0
_022644BA:
	ldrh r0, [r4, #0xe]
	mov r1, #2
	ldr r5, _022645B4 ; =0x00002D66
	bic r0, r1
	lsl r1, r3, #0x10
	lsr r1, r1, #0x10
	lsl r1, r1, #0x1f
	lsr r1, r1, #0x1e
	orr r0, r1
	ldr r1, [sp, #8]
	strh r0, [r4, #0xe]
	add r2, r1, #0
	add r6, r2, r5
	add r5, #0x58
	add r3, r1, #0
	mov r0, #0
	mov ip, r4
	add r2, r4, #0
	add r7, r3, r5
_022644E0:
	mov r3, #0xb5
	lsl r3, r3, #6
	ldrh r5, [r1, r3]
	mov r3, ip
	strh r5, [r3, #0x18]
	ldrb r3, [r6]
	lsl r3, r3, #0x1a
	lsr r5, r3, #0x1f
	add r3, r4, r0
	add r3, #0x24
	strb r5, [r3]
	ldrb r3, [r6]
	lsl r3, r3, #0x1b
	lsr r5, r3, #0x1b
	add r3, r4, r0
	add r3, #0x28
	strb r5, [r3]
	mov r3, #0xb7
	lsl r3, r3, #6
	ldr r3, [r1, r3]
	str r3, [r2, #0x3c]
	ldr r3, _022645B0 ; =0x00002DB0
	ldr r5, [r1, r3]
	mov r3, #2
	lsl r3, r3, #0x14
	tst r3, r5
	beq _02264524
	ldr r3, _022645B8 ; =0x00002DFA
	ldrh r5, [r1, r3]
	add r3, r4, r0
	add r3, #0x20
	strb r5, [r3]
	ldr r3, _022645BC ; =0x00002DE4
	b _02264532
_02264524:
	ldrb r3, [r7]
	lsl r3, r3, #0x1c
	lsr r5, r3, #0x1c
	add r3, r4, r0
	add r3, #0x20
	strb r5, [r3]
	ldr r3, _022645C0 ; =0x00002DA8
_02264532:
	ldr r3, [r1, r3]
	add r0, r0, #1
	str r3, [r2, #0x2c]
	mov r3, ip
	add r3, r3, #2
	add r1, #0xc0
	mov ip, r3
	add r6, #0xc0
	add r2, r2, #4
	add r7, #0xc0
	cmp r0, #4
	blt _022644E0
	ldr r0, [sp, #0x24]
	cmp r0, #0xff
	beq _02264572
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x24]
	bl ov12_0223C140
	cmp r0, #0xff
	beq _02264572
	ldr r2, [sp, #8]
	ldr r1, [sp, #0x24]
	add r2, r2, r1
	ldr r1, _022645C4 ; =0x0000219C
	ldrb r1, [r2, r1]
	cmp r0, r1
	bne _02264572
	ldrh r1, [r4, #0xe]
	mov r0, #4
	orr r0, r1
	strh r0, [r4, #0xe]
_02264572:
	ldr r0, [sp, #0x28]
	cmp r0, #0xff
	beq _0226459A
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x28]
	bl ov12_0223C140
	cmp r0, #0xff
	beq _0226459A
	ldr r2, [sp, #8]
	ldr r1, [sp, #0x28]
	add r2, r2, r1
	ldr r1, _022645C4 ; =0x0000219C
	ldrb r1, [r2, r1]
	cmp r0, r1
	bne _0226459A
	ldrh r1, [r4, #0xe]
	mov r0, #8
	orr r0, r1
	strh r0, [r4, #0xe]
_0226459A:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_022645A0: .word 0x00002144
_022645A4: .word 0x000003E1
_022645A8: .word 0x00002D75
_022645AC: .word 0x00002164
_022645B0: .word 0x00002DB0
_022645B4: .word 0x00002D66
_022645B8: .word 0x00002DFA
_022645BC: .word 0x00002DE4
_022645C0: .word 0x00002DA8
_022645C4: .word 0x0000219C
	thumb_func_end ov12_022643C8
