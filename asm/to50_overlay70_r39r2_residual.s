#include "config.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.public FillWindowPixelBuffer
	.public ov70_02242FC4
	.public CopyWindowPixelsToVram_TextMode
	.public sub_02019A60
	.text
	thumb_func_start ov70_022434C0
ov70_022434C0: ; 0x022434C0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	lsl r7, r1, #2
	mov r4, #0
	add r5, r0, #0
	lsl r0, r7, #3
	str r2, [sp, #4]
	str r4, [sp, #0xc]
	add r6, r4, #0
	str r0, [sp, #8]
_022434D4:
	ldr r0, [r5, #4]
	mov r1, #0x22
	add r0, r0, r4
	bl FillWindowPixelBuffer
	ldr r0, [sp, #4]
	cmp r7, r0
	bge _022434FE
	ldr r0, _02243528 ; =0x000F0E02
	ldr r2, [sp, #8]
	str r0, [sp]
	ldr r3, [r5, #0x34]
	ldr r1, [r5, #4]
	add r2, r2, r3
	ldr r0, [r5, #0x1c]
	ldr r2, [r6, r2]
	add r1, r1, r4
	mov r3, #0
	bl ov70_02242FC4
	b _02243512
_022434FE:
	ldr r0, [r5, #4]
	add r0, r0, r4
	bl CopyWindowPixelsToVram_TextMode
	ldr r2, [r5, #4]
	ldr r0, [r5, #0x1c]
	mov r1, #0
	add r2, r2, r4
	bl sub_02019A60
_02243512:
	ldr r0, [sp, #0xc]
	add r4, #0x10
	add r0, r0, #1
	add r7, r7, #1
	add r6, #8
	str r0, [sp, #0xc]
	cmp r0, #4
	blt _022434D4
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02243528: .word 0x000F0E02
	thumb_func_end ov70_022434C0
	thumb_func_start ov70_0224352C
ov70_0224352C: ; 0x0224352C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	lsl r0, r1, #2
	mov r4, #0
	add r7, r1, r0
	lsl r0, r7, #3
	str r2, [sp, #4]
	str r4, [sp, #0xc]
	add r6, r4, #0
	str r0, [sp, #8]
_02243542:
	ldr r0, [r5, #4]
	mov r1, #0x22
	add r0, r0, r4
	bl FillWindowPixelBuffer
	ldr r0, [sp, #4]
	cmp r7, r0
	bge _0224356C
	ldr r0, _02243594 ; =0x000F0E02
	ldr r2, [sp, #8]
	str r0, [sp]
	ldr r3, [r5, #0x34]
	ldr r1, [r5, #4]
	add r2, r2, r3
	ldr r0, [r5, #0x1c]
	ldr r2, [r6, r2]
	add r1, r1, r4
	mov r3, #0
	bl ov70_02242FC4
	b _02243580
_0224356C:
	ldr r0, [r5, #4]
	add r0, r0, r4
	bl CopyWindowPixelsToVram_TextMode
	ldr r2, [r5, #4]
	ldr r0, [r5, #0x1c]
	mov r1, #0
	add r2, r2, r4
	bl sub_02019A60
_02243580:
	ldr r0, [sp, #0xc]
	add r4, #0x10
	add r0, r0, #1
	add r7, r7, #1
	add r6, #8
	str r0, [sp, #0xc]
	cmp r0, #5
	blt _02243542
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02243594: .word 0x000F0E02
	thumb_func_end ov70_0224352C
