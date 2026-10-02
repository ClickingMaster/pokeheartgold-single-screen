	.include "asm/macros.inc"
	.include "overlay_86.inc"
	.include "global.inc"

.text
	thumb_func_start ov86_021E7094
ov86_021E7094: ; 0x021E7094
	push {r3, r4, lr}
	sub sp, #0xc
	ldr r3, _021E71B8 ; =0x0000024E
	add r4, r0, #0
	ldrb r0, [r4, r3]
	cmp r0, #0
	beq _021E70AC
	cmp r0, #1
	beq _021E7118
	cmp r0, #2
	beq _021E719C
	b _021E71B2
_021E70AC:
	sub r0, r3, #2
	ldrb r0, [r4, r0]
	lsl r1, r0, #0x1f
	lsr r1, r1, #0x1f
	bne _021E70D2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r1, r4, r0
	add r0, r3, #0
	sub r0, #0x1a
	ldr r0, [r1, r0]
	sub r1, r3, #1
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x1c
	lsr r1, r1, #0x1c
	bl ManagedSprite_SetPaletteOverride
	b _021E710E
_021E70D2:
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
	ldr r0, [r4, #0xc]
	lsr r1, r1, #0x19
	bl BgTilemapRectChangePalette
	mov r1, #0x93
	lsl r1, r1, #2
	ldrb r1, [r4, r1]
	ldr r0, [r4, #0xc]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x19
	bl ScheduleBgTilemapBufferTransfer
_021E710E:
	ldr r0, _021E71B8 ; =0x0000024E
	ldrb r1, [r4, r0]
	add r1, r1, #1
	strb r1, [r4, r0]
	b _021E71B2
_021E7118:
	add r0, r3, #1
	ldrb r0, [r4, r0]
	add r1, r0, #1
	add r0, r3, #1
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #4
	bne _021E71B2
	sub r0, r3, #2
	ldrb r0, [r4, r0]
	lsl r1, r0, #0x1f
	lsr r1, r1, #0x1f
	bne _021E714E
	lsl r0, r0, #0x18
	lsr r0, r0, #0x19
	lsl r0, r0, #2
	add r1, r4, r0
	add r0, r3, #0
	sub r0, #0x1a
	ldr r0, [r1, r0]
	sub r1, r3, #1
	ldrb r1, [r4, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x1c
	bl ManagedSprite_SetPaletteOverride
	b _021E718A
_021E714E:
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
	ldr r0, [r4, #0xc]
	lsr r1, r1, #0x19
	bl BgTilemapRectChangePalette
	mov r1, #0x93
	lsl r1, r1, #2
	ldrb r1, [r4, r1]
	ldr r0, [r4, #0xc]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x19
	bl ScheduleBgTilemapBufferTransfer
_021E718A:
	ldr r0, _021E71BC ; =0x0000024F
	mov r1, #0
	strb r1, [r4, r0]
	sub r1, r0, #1
	ldrb r1, [r4, r1]
	sub r0, r0, #1
	add r1, r1, #1
	strb r1, [r4, r0]
	b _021E71B2
_021E719C:
	add r0, r3, #1
	ldrb r0, [r4, r0]
	add r1, r0, #1
	add r0, r3, #1
	strb r1, [r4, r0]
	ldrb r0, [r4, r0]
	cmp r0, #2
	bne _021E71B2
	add sp, #0xc
	mov r0, #0
	pop {r3, r4, pc}
_021E71B2:
	mov r0, #1
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
_021E71B8: .word 0x0000024E
_021E71BC: .word 0x0000024F
	thumb_func_end ov86_021E7094
