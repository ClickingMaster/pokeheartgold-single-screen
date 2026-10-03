	.include "asm/macros.inc"
	.include "render_window.inc"
	.include "global.inc"

.text
	thumb_func_start sub_0200EC84
sub_0200EC84: ; 0x0200EC84
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r1, #0
	add r5, r0, #0
	ldr r1, [sp, #0x20]
	cmp r3, #0
	bne _0200EC98
	add r1, #0x21
	lsl r0, r1, #0x10
	b _0200EC9C
_0200EC98:
	add r0, r1, #2
	lsl r0, r0, #0x10
_0200EC9C:
	lsr r1, r0, #0x10
	mov r0, #3
	str r2, [sp]
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r0, [sp, #0x24]
	add r2, r5, #0
	str r0, [sp, #0xc]
	mov r0, #0x24
	add r3, r4, #0
	bl GfGfxLoader_LoadCharData
	add sp, #0x10
	pop {r3, r4, r5, pc}
	thumb_func_end sub_0200EC84
