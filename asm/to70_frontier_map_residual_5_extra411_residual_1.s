	.include "asm/macros.inc"
	.include "overlay_80_02238648.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_0223947C
ov80_0223947C: ; 0x0223947C
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r0, [r6, #8]
	add r5, r1, #0
	bl sub_02096864
	add r7, r0, #0
	ldrh r3, [r5]
	mov r1, #0
	add r2, r7, #0
_02239490:
	ldrh r0, [r2]
	cmp r3, r0
	beq _022394D2
	add r1, r1, #1
	add r2, r2, #4
	cmp r1, #0x18
	blt _02239490
	ldr r0, _022394D4 ; =0x0000FFFF
	mov r4, #0
	add r2, r7, #0
_022394A4:
	ldrh r1, [r2]
	cmp r1, r0
	beq _022394B2
	add r4, r4, #1
	add r2, r2, #4
	cmp r4, #0x18
	blt _022394A4
_022394B2:
	cmp r4, #0x18
	bne _022394BA
	bl GF_AssertFail
_022394BA:
	ldrh r0, [r5]
	lsl r2, r4, #2
	add r1, r7, r2
	strh r0, [r7, r2]
	ldrh r0, [r5, #2]
	mov r3, #0x65
	strh r0, [r1, #2]
	ldrh r1, [r5]
	ldrb r2, [r5, #2]
	ldr r0, [r6, #0x20]
	bl ov42_02228FE0
_022394D2:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022394D4: .word 0x0000FFFF
	thumb_func_end ov80_0223947C
