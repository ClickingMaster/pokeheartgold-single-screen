	.extern sub_02094C6C
#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_020932E0.inc"
	.include "global.inc"

.text
	thumb_func_start sub_02094A90
sub_02094A90: ; 0x02094A90
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r0, #0
	cmp r1, #0
	beq _02094B1A
	ldr r2, _02094BF0 ; =0x0000469C
	ldr r0, [r4, r2]
	cmp r0, #2
	bne _02094AE8
	cmp r1, #2
	bne _02094AC0
	add r0, r2, #0
	sub r0, #0x14
	add r5, r4, r0
	ldmia r5!, {r0, r1}
	add r3, sp, #0
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	sub r2, #0x18
	str r0, [r3]
	ldr r0, [r4, r2]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	b _02094B0E
_02094AC0:
	add r0, r2, #0
	add r0, #0x14
	ldr r0, [r4, r0]
	add r2, #0x14
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r0, #0x35
	lsl r0, r0, #0xe
	str r0, [sp]
	add r1, r1, #1
	mov r0, #0x28
	mul r0, r1
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r0, [r4, r2]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	b _02094B0E
_02094AE8:
	add r0, r2, #0
	add r0, #0x14
	ldr r0, [r4, r0]
	add r2, #0x14
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	mov r0, #0x35
	lsl r0, r0, #0xe
	str r0, [sp]
	add r1, r1, #1
	mov r0, #0x28
	mul r0, r1
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r0, [r4, r2]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
_02094B0E:
	ldr r0, _02094BF4 ; =0x000046B8
	mov r1, #3
	ldr r0, [r4, r0]
	bl sub_02095D40
	b _02094B5C
_02094B1A:
	ldr r2, _02094BF8 ; =0x00004688
	add r3, sp, #0
	add r5, r4, r2
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	str r0, [r3]
	add r0, r2, #0
	add r0, #0x14
	ldr r0, [r4, r0]
	cmp r0, #2
	bne _02094B48
	add r0, r2, #0
	sub r2, r2, #4
	ldr r2, [r4, r2]
	add r0, #0x30
	lsl r2, r2, #0x18
	ldr r0, [r4, r0]
	mov r1, #3
	lsr r2, r2, #0x18
	bl sub_02095D40
	b _02094B5C
_02094B48:
	add r0, r2, #0
	sub r2, r2, #4
	ldr r2, [r4, r2]
	add r0, #0x30
	lsl r2, r2, #0x18
	ldr r0, [r4, r0]
	mov r1, #2
	lsr r2, r2, #0x18
	bl sub_02095D40
_02094B5C:
	ldr r1, _02094BFC ; =0x000046A0
	add r0, r1, #0
	sub r0, #0x1c
	ldr r2, [r4, r0]
	ldr r3, [r4, r1]
	mov r0, #0x34
	mul r0, r2
	sub r1, #0x18
	ldr r0, [r3, r0]
	add r1, r4, r1
	bl Sprite_SetMatrix
	ldr r0, _02094BFC ; =0x000046A0
	ldr r2, [r4, r0]
	sub r0, #0x1c
	ldr r1, [r4, r0]
	mov r0, #0x34
	mul r0, r1
	ldr r0, [r2, r0]
	mov r1, #6
	bl Sprite_SetDrawPriority
	mov r0, #0x23
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	add r1, sp, #0
	bl Sprite_SetMatrix
	mov r0, #0x23
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	mov r1, #0x2d
	bl Sprite_SetAnimCtrlSeq
	ldr r0, _02094C00 ; =0x000046BC
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _02094BAC
	mov r1, #0
	str r1, [r4, r0]
_02094BAC:
	ldr r0, _02094C04 ; =0x00004680
	mov r1, #0
	str r1, [r4, r0]
	sub r3, r1, #1
	add r2, r0, #4
	str r3, [r4, r2]
	add r2, r0, #0
	add r2, #8
	str r1, [r4, r2]
	add r2, r0, #0
	add r2, #0xc
	str r1, [r4, r2]
	add r2, r0, #0
	add r2, #0x10
	str r1, [r4, r2]
	add r2, r0, #0
	add r2, #0x14
	str r1, [r4, r2]
	add r2, r0, #0
	add r2, #0x18
	str r1, [r4, r2]
	add r2, r0, #0
	add r2, #0x20
	str r1, [r4, r2]
	add r0, #0x1c
	str r1, [r4, r0]
	mov r0, #0x7b
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_02094BF0: .word 0x0000469C
_02094BF4: .word 0x000046B8
_02094BF8: .word 0x00004688
_02094BFC: .word 0x000046A0
_02094C00: .word 0x000046BC
_02094C04: .word 0x00004680
	thumb_func_end sub_02094A90
