	.extern ov99_021E82E4
	.extern ov99_021E84EC
	.extern ov99_021E8544
	.extern ov99_021E88EC
	.extern ov99_021E8C50
	.extern ov99_021E8C88
	.extern ov99_021E8D5C
	.extern ov99_021E8D88
	.extern ov99_021E8EBC
	.extern ov99_021E9020
	.extern ov99_021E945C
	.extern ov99_021E94CC
	.extern ov99_021E94FC
	.extern ov99_021E9508
	.extern ov99_021E9518
	.extern ov99_021EA3AC
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"

.text
	thumb_func_start ov99_021E89EC
ov99_021E89EC: ; 0x021E89EC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	ldr r0, [r5, #0x10]
	bl ov98_0221EEFC
	add r2, r5, #0
	add r2, #0xac
	ldrb r2, [r2]
	mov r1, #0
	bl BufferPokeathlonEventName
	mov r3, #1
	str r3, [sp]
	ldr r0, [r5, #0x10]
	mov r1, #2
	mov r2, #0x3e
	bl ov98_0221EC08
	mov r0, #1
	str r0, [sp]
	mov r3, #0
	str r3, [sp, #4]
	mov r2, #0xac
	ldrsb r2, [r5, r2]
	ldr r0, [r5, #0x10]
	mov r1, #5
	add r2, #0x41
	bl ov98_0221EBEC
	ldr r0, [r5, #0x10]
	bl ov98_0221EEFC
	add r1, r5, #0
	add r1, #0xac
	add r2, r5, #0
	add r2, #0xb0
	add r4, r0, #0
	ldrb r1, [r1]
	ldr r0, [r5]
	ldr r2, [r2]
	bl ov99_021E94FC
	add r2, r0, #0
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #0
	mov r3, #7
	bl BufferIntegerAsString
	mov r0, #0
	str r0, [sp]
	ldr r0, [r5, #0x10]
	mov r1, #3
	mov r2, #0x36
	mov r3, #1
	bl ov98_0221EC08
	mov r4, #0
	add r7, r4, #0
_021E8A68:
	cmp r4, #0
	bne _021E8A7A
	add r0, r5, #0
	add r0, #0xb0
	ldr r0, [r0]
	cmp r0, #0
	bne _021E8A7A
	mov r6, #1
	b _021E8A7C
_021E8A7A:
	mov r6, #0
_021E8A7C:
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl ov99_021E8544
	cmp r0, #0
	bne _021E8AB2
	mov r1, #0xac
	add r3, r5, #0
	add r3, #0xb0
	lsl r2, r4, #0x18
	ldrsb r1, [r5, r1]
	ldr r0, [r5]
	ldr r3, [r3]
	lsr r2, r2, #0x18
	bl ov99_021E94CC
	add r1, r0, #0
	str r6, [sp]
	mov r3, #0xac
	ldrsb r3, [r5, r3]
	add r0, r5, #0
	add r2, r4, #7
	add r3, #0x56
	bl ov99_021E88EC
	b _021E8ACA
_021E8AB2:
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	str r7, [sp, #4]
	mov r2, #0xac
	ldrsb r2, [r5, r2]
	ldr r0, [r5, #0x10]
	add r1, r4, #7
	add r2, #0x60
	add r3, r7, #0
	bl ov98_0221EBEC
_021E8ACA:
	add r4, r4, #1
	cmp r4, #5
	blt _021E8A68
	add r0, r5, #0
	add r0, #0xb0
	ldr r0, [r0]
	cmp r0, #0
	beq _021E8B2A
	ldr r0, [r5, #0x10]
	mov r1, #6
	mov r2, #0x4b
	mov r3, #0
	bl ov98_0221EBD8
	add r0, r5, #0
	bl ov99_021E9020
	mov r4, #0
	mov r6, #1
	add r7, r4, #0
_021E8AF2:
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl ov99_021E8544
	cmp r0, #0
	bne _021E8B20
	add r1, r5, #0
	add r1, #0xac
	ldrb r1, [r1]
	lsl r2, r4, #0x18
	ldr r0, [r5]
	lsr r2, r2, #0x18
	bl ov99_021E9518
	add r1, r4, #0
	str r6, [sp]
	add r2, r0, #0
	ldr r0, [r5, #0x10]
	add r1, #0xc
	add r3, r7, #0
	bl ov98_0221EE9C
_021E8B20:
	add r4, r4, #1
	cmp r4, #5
	blt _021E8AF2
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_021E8B2A:
	mov r0, #0xac
	ldrsb r1, [r5, r0]
	lsl r0, r1, #2
	add r0, r5, r0
	add r0, #0xbc
	ldr r0, [r0]
	cmp r0, #0
	beq _021E8B4C
	ldr r0, [r5, #0x10]
	mov r1, #0x11
	bl ov98_0221EE84
	ldr r0, [r5, #0x10]
	mov r1, #0x12
	bl ov98_0221EE84
	b _021E8B76
_021E8B4C:
	lsl r1, r1, #0x18
	ldr r0, [r5]
	lsr r1, r1, #0x18
	bl ov99_021E82E4
	add r3, r0, #0
	mov r0, #3
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, [r5, #0x10]
	mov r1, #0x11
	mov r2, #0x6b
	bl ov98_0221ECD0
	ldr r0, [r5, #0x10]
	mov r1, #0x12
	mov r2, #0x6a
	mov r3, #0
	bl ov98_0221EBD8
_021E8B76:
	mov r0, #0xac
	ldrsb r0, [r5, r0]
	cmp r0, #0
	bge _021E8B86
	ldr r0, [r5, #0x10]
	mov r1, #6
	bl ov98_0221EE84
_021E8B86:
	ldr r0, [r5]
	bl ov99_021E945C
	cmp r0, #0
	bne _021E8BC8
	mov r0, #0xac
	ldrsb r0, [r5, r0]
	lsl r0, r0, #2
	add r0, r5, r0
	add r0, #0xbc
	ldr r0, [r0]
	cmp r0, #0
	beq _021E8BB0
	ldr r0, [r5, #0x10]
	mov r1, #6
	mov r2, #0x6d
	mov r3, #0
	bl ov98_0221EBD8
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_021E8BB0:
	ldr r0, [r5]
	bl ov99_021E9508
	str r0, [sp]
	ldr r0, [r5, #0x10]
	mov r1, #6
	mov r2, #0x6c
	mov r3, #0
	bl ov98_0221EDC4
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_021E8BC8:
	mov r1, #0xac
	ldrsb r1, [r5, r1]
	add r0, r5, #0
	bl ov99_021E84EC
	cmp r0, #0
	beq _021E8BEE
	ldr r0, [r5]
	bl ov99_021E9508
	str r0, [sp]
	ldr r0, [r5, #0x10]
	mov r1, #6
	mov r2, #0x6c
	mov r3, #0
	bl ov98_0221EDC4
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_021E8BEE:
	mov r2, #0xac
	ldrsb r2, [r5, r2]
	ldr r0, [r5, #0x10]
	mov r1, #6
	add r2, #0x4c
	mov r3, #0
	bl ov98_0221EBD8
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov99_021E89EC
