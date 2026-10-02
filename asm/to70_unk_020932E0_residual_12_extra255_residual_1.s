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
	thumb_func_start sub_02094F5C
sub_02094F5C: ; 0x02094F5C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r6, r1, #0
	add r5, r0, #0
	lsl r7, r6, #4
	ldr r1, _02095010 ; =0x0000088C
	add r0, r5, r7
	ldr r2, [r0, r1]
	mov r0, #0xc
	add r4, r2, #0
	add r1, #0x44
	mul r4, r0
	ldr r0, [r5, r1]
	add r2, r0, r4
	ldr r1, [r2, #8]
	ldr r0, [r0, r4]
	str r1, [sp, #4]
	ldr r1, [r2, #4]
	str r1, [sp]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x8d
	lsl r0, r0, #4
	ldr r2, [r5, r0]
	mov r1, #0
	mvn r1, r1
	add r2, r2, r4
	str r1, [r2, #4]
	ldrb r1, [r5, #0xe]
	sub r1, r1, #1
	strb r1, [r5, #0xe]
	ldrb r2, [r5, #0xf]
	ldr r1, [sp, #4]
	cmp r1, r2
	bne _02094FBA
	ldr r0, [sp]
	mov r1, #0x34
	mul r1, r0
	mov r0, #0x66
	add r1, r5, r1
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #0
	bl Sprite_SetOamMode
	b _02094FE6
_02094FBA:
	mov r1, #0x34
	mul r1, r6
	add r1, r5, r1
	sub r0, #0xe8
	ldr r0, [r1, r0]
	bl Sprite_GetMatrixPtr
	add r1, r0, #0
	ldr r0, _02095014 ; =0x000008C4
	ldr r0, [r5, r0]
	bl Sprite_SetMatrix
	ldr r0, _02095014 ; =0x000008C4
	mov r1, #1
	ldr r0, [r5, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _02095014 ; =0x000008C4
	mov r1, #0x32
	ldr r0, [r5, r0]
	bl Sprite_SetAnimCtrlSeq
_02094FE6:
	mov r2, #0x89
	mov r1, #0
	add r0, r5, r7
	lsl r2, r2, #4
	str r1, [r0, r2]
	mov r0, #0x34
	mul r0, r6
	add r0, r5, r0
	sub r2, #0xa8
	ldr r0, [r0, r2]
	bl Sprite_SetDrawFlag
	ldr r0, _02095018 ; =0x000046A4
	mov r2, #0
	ldr r1, [r5, r0]
	lsl r0, r6, #3
	add r0, r1, r0
	str r2, [r0, #4]
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02095010: .word 0x0000088C
_02095014: .word 0x000008C4
_02095018: .word 0x000046A4
	thumb_func_end sub_02094F5C
