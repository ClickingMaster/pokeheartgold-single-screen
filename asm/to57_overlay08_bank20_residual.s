#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.public ov08_02221500
	.public ov08_022213E4
	.public ov08_022214BC
	.public ov08_02220A8C
	.text

	thumb_func_start ov08_02221500
ov08_02221500: ; 0x02221500
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r0, [r6]
	add r5, r6, #4
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	add r4, r1, #0
	mul r4, r0
	ldr r0, _02221618 ; =0x00002024
	mov r1, #0
	ldr r0, [r6, r0]
	bl ov08_022213E4
	ldr r0, _0222161C ; =0x00002028
	mov r1, #1
	ldr r0, [r6, r0]
	bl ov08_022213E4
	ldr r0, _02221620 ; =0x0000202C
	mov r1, #3
	ldr r0, [r6, r0]
	bl ov08_022213E4
	ldr r0, _02221624 ; =0x00002030
	mov r1, #4
	ldr r0, [r6, r0]
	bl ov08_022213E4
	ldr r0, _02221628 ; =0x00002034
	mov r1, #2
	ldr r0, [r6, r0]
	bl ov08_022213E4
	add r0, r5, r4
	add r0, #0x28
	mov r1, #0x90
	ldrb r0, [r0]
	add r2, r1, #0
	bl ov08_022214BC
	add r7, r0, #0
	add r0, r5, r4
	add r0, #0x28
	ldrb r0, [r0]
	mov r1, #2
	mov r2, #0x18
	bl ov08_022214BC
	add r2, r0, #0
	ldr r0, _02221618 ; =0x00002024
	add r1, r7, #0
	ldr r0, [r6, r0]
	bl ov08_02220A8C
	add r0, r5, r4
	add r0, #0x29
	ldrb r0, [r0]
	mov r1, #0xa4
	mov r2, #0x90
	bl ov08_022214BC
	add r7, r0, #0
	add r0, r5, r4
	add r0, #0x29
	ldrb r0, [r0]
	mov r1, #0x10
	mov r2, #0x18
	bl ov08_022214BC
	add r2, r0, #0
	ldr r0, _0222161C ; =0x00002028
	add r1, r7, #0
	ldr r0, [r6, r0]
	bl ov08_02220A8C
	add r0, r5, r4
	add r0, #0x2a
	ldrb r0, [r0]
	mov r1, #0x9c
	mov r2, #0x90
	bl ov08_022214BC
	add r7, r0, #0
	add r0, r5, r4
	add r0, #0x2a
	ldrb r0, [r0]
	mov r1, #0x29
	mov r2, #0x18
	bl ov08_022214BC
	add r2, r0, #0
	ldr r0, _02221620 ; =0x0000202C
	add r1, r7, #0
	ldr r0, [r6, r0]
	bl ov08_02220A8C
	add r0, r5, r4
	add r0, #0x2b
	ldrb r0, [r0]
	mov r1, #0x83
	mov r2, #0x8f
	bl ov08_022214BC
	add r7, r0, #0
	add r0, r5, r4
	add r0, #0x2b
	ldrb r0, [r0]
	mov r1, #0x29
	mov r2, #0x18
	bl ov08_022214BC
	add r2, r0, #0
	ldr r0, _02221624 ; =0x00002030
	add r1, r7, #0
	ldr r0, [r6, r0]
	bl ov08_02220A8C
	add r0, r5, r4
	add r0, #0x2c
	ldrb r0, [r0]
	mov r1, #0x7b
	mov r2, #0x8f
	bl ov08_022214BC
	add r7, r0, #0
	add r0, r5, r4
	add r0, #0x2c
	ldrb r0, [r0]
	mov r1, #0x10
	mov r2, #0x18
	bl ov08_022214BC
	add r2, r0, #0
	ldr r0, _02221628 ; =0x00002034
	add r1, r7, #0
	ldr r0, [r6, r0]
	bl ov08_02220A8C
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02221618: .word 0x00002024
_0222161C: .word 0x00002028
_02221620: .word 0x0000202C
_02221624: .word 0x00002030
_02221628: .word 0x00002034
	thumb_func_end ov08_02221500
