	.include "asm/macros.inc"
	.include "overlay_67.inc"
	.include "global.inc"

.text
	thumb_func_start ov67_021E6A40
ov67_021E6A40: ; 0x021E6A40
	push {r3, r4, lr}
	sub sp, #0xc
	ldr r3, _021E6B60 ; =0x00000496
	add r4, r0, #0
	ldrb r0, [r4, r3]
	cmp r0, #0
	beq _021E6A58
	cmp r0, #1
	beq _021E6AC2
	cmp r0, #2
	beq _021E6B44
	b _021E6B5A
_021E6A58:
	sub r0, r3, #2
	ldrb r0, [r4, r0]
	lsl r1, r0, #0x1f
	lsr r1, r1, #0x1f
	bne _021E6A7E
	lsl r0, r0, #0x18
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r1, r4, r0
	add r0, r3, #0
	sub r0, #0x12
	ldr r0, [r1, r0]
	sub r1, r3, #1
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x1c
	lsr r1, r1, #0x1c
	bl ManagedSprite_SetPaletteOverride
	b _021E6AB8
_021E6A7E:
	add r0, r3, #4
	ldrb r0, [r4, r0]
	sub r1, r3, #2
	add r2, r3, #2
	str r0, [sp]
	add r0, r3, #5
	ldrb r0, [r4, r0]
	str r0, [sp, #4]
	sub r0, r3, #1
	ldrb r0, [r4, r0]
	add r3, r3, #3
	lsl r0, r0, #0x1c
	lsr r0, r0, #0x1c
	str r0, [sp, #8]
	ldrb r1, [r4, r1]
	ldrb r2, [r4, r2]
	ldrb r3, [r4, r3]
	lsl r1, r1, #0x18
	ldr r0, [r4, #0x10]
	lsr r1, r1, #0x19
	bl BgTilemapRectChangePalette
	ldr r1, _021E6B64 ; =0x00000494
	ldr r0, [r4, #0x10]
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x19
	bl ScheduleBgTilemapBufferTransfer
_021E6AB8:
	ldr r0, _021E6B60 ; =0x00000496
	ldrb r1, [r4, r0]
	add r1, r1, #1
	strb r1, [r4, r0]
	b _021E6B5A
_021E6AC2:
	add r0, r3, #1
	ldrb r0, [r4, r0]
	add r1, r0, #1
	add r0, r3, #1
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #4
	bne _021E6B5A
	sub r0, r3, #2
	ldrb r0, [r4, r0]
	lsl r1, r0, #0x1f
	lsr r1, r1, #0x1f
	bne _021E6AF8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r1, r4, r0
	add r0, r3, #0
	sub r0, #0x12
	ldr r0, [r1, r0]
	sub r1, r3, #1
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1c
	bl ManagedSprite_SetPaletteOverride
	b _021E6B32
_021E6AF8:
	add r0, r3, #4
	ldrb r0, [r4, r0]
	sub r1, r3, #2
	add r2, r3, #2
	str r0, [sp]
	add r0, r3, #5
	ldrb r0, [r4, r0]
	str r0, [sp, #4]
	sub r0, r3, #1
	ldrb r0, [r4, r0]
	add r3, r3, #3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1c
	str r0, [sp, #8]
	ldrb r1, [r4, r1]
	ldrb r2, [r4, r2]
	ldrb r3, [r4, r3]
	lsl r1, r1, #0x18
	ldr r0, [r4, #0x10]
	lsr r1, r1, #0x19
	bl BgTilemapRectChangePalette
	ldr r1, _021E6B64 ; =0x00000494
	ldr r0, [r4, #0x10]
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x19
	bl ScheduleBgTilemapBufferTransfer
_021E6B32:
	ldr r0, _021E6B68 ; =0x00000497
	mov r1, #0
	strb r1, [r4, r0]
	sub r1, r0, #1
	ldrb r1, [r4, r1]
	sub r0, r0, #1
	add r1, r1, #1
	strb r1, [r4, r0]
	b _021E6B5A
_021E6B44:
	add r0, r3, #1
	ldrb r0, [r4, r0]
	add r1, r0, #1
	add r0, r3, #1
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #2
	bne _021E6B5A
	add sp, #0xc
	mov r0, #0
	pop {r3, r4, pc}
_021E6B5A:
	mov r0, #1
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
_021E6B60: .word 0x00000496
_021E6B64: .word 0x00000494
_021E6B68: .word 0x00000497
	thumb_func_end ov67_021E6A40
