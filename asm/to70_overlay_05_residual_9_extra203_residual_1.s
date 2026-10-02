	.include "asm/macros.inc"
	.include "overlay_05.inc"
	.include "global.inc"

.text
	thumb_func_start ov05_0221D140
ov05_0221D140: ; 0x0221D140
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	ldr r0, _0221D21C ; =0x00000BA8
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0221D164
	ldr r0, [r4]
	cmp r0, #0
	beq _0221D164
	ldr r1, [r0]
	cmp r1, #0
	beq _0221D164
	mov r0, #0x13
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	cmp r0, #0
	bne _0221D168
_0221D164:
	bl GF_AssertFail
_0221D168:
	ldr r0, [r4]
	ldr r1, [r0]
	mov r0, #0x13
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldr r0, [r4]
	mov r1, #0
	ldr r0, [r0, #0x24]
	mov r2, #1
	str r0, [sp, #4]
	ldr r0, [r4, #0xc]
	mov r3, #0xf
	bl LoadUserFrameGfx2
	ldr r0, [r4, #8]
	mov r1, #0
	mov r2, #0xf0
	mov r3, #0x20
	bl PaletteData_LoadPaletteSlotFromHardware
	mov r1, #0
	str r1, [sp]
	ldr r0, [r4]
	mov r2, #0x1f
	ldr r0, [r0, #0x24]
	mov r3, #0xe
	str r0, [sp, #4]
	ldr r0, [r4, #0xc]
	bl LoadUserFrameGfx1
	ldr r0, [r4, #8]
	mov r1, #0
	mov r2, #0xe0
	mov r3, #0x20
	bl PaletteData_LoadPaletteSlotFromHardware
	ldr r2, [r4]
	mov r1, #0x1a
	ldr r2, [r2, #0x24]
	mov r0, #0
	lsl r1, r1, #4
	bl LoadFontPal0
	ldr r0, [r4, #8]
	mov r1, #0
	mov r2, #0xd0
	mov r3, #0x20
	bl PaletteData_LoadPaletteSlotFromHardware
	mov r0, #0x13
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x28
	ldr r1, _0221D220 ; =0x00000B88
	str r0, [sp, #0x10]
	ldr r0, [r4, #0xc]
	add r1, r4, r1
	mov r2, #0
	mov r3, #2
	bl AddWindowParameterized
	mov r0, #0x13
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x28
	ldr r1, _0221D224 ; =0x00000B98
	str r0, [sp, #0x10]
	ldr r0, [r4, #0xc]
	add r1, r4, r1
	mov r2, #0
	mov r3, #2
	bl AddWindowParameterized
	add sp, #0x14
	pop {r3, r4, pc}
	.balign 4, 0
_0221D21C: .word 0x00000BA8
_0221D220: .word 0x00000B88
_0221D224: .word 0x00000B98
	thumb_func_end ov05_0221D140
