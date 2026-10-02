#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

.text
	thumb_func_start ov12_0225A700
ov12_0225A700: ; 0x0225A700
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	str r1, [sp]
	mov r0, #5
	mov r1, #0x34
	str r2, [sp, #4]
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #5
	mov r1, #0x34
	bl Heap_Alloc
	str r0, [r4, #8]
	mov r0, #5
	mov r1, #0x38
	bl Heap_Alloc
	ldr r1, [r4, #8]
	str r0, [r1, #4]
	mov r0, #5
	bl SaveArray_Party_Alloc
	ldr r1, [r4, #8]
	mov r6, #0
	ldr r1, [r1, #4]
	ldr r3, [sp, #4]
	str r0, [r1]
	strb r6, [r4, #0xe]
	str r5, [r4]
	ldr r0, [sp, #4]
	mov r1, #0x65
	ldrb r0, [r0]
	lsl r1, r1, #2
	add r5, r4, #0
	strb r0, [r4, #0xc]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	add r1, r1, #1
	strb r0, [r4, #0xd]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	strb r0, [r4, #0xf]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #1]
	strb r0, [r4, #0x14]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #2]
	strb r0, [r4, #0x15]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #3]
	strb r0, [r4, #0x16]
_0225A76A:
	ldr r0, [sp, #4]
	ldr r1, [r4, #8]
	add r7, r0, r6
	ldrb r0, [r7, #4]
	add r1, r1, r6
	mov r2, #0
	strb r0, [r1, #0xc]
_0225A778:
	add r0, r3, r2
	ldrb r1, [r0, #8]
	add r0, r5, r2
	add r2, r2, #1
	strb r1, [r0, #0x18]
	cmp r2, #6
	blt _0225A778
	add r7, #0x20
	ldrb r1, [r7]
	add r0, r4, r6
	add r0, #0x30
	add r6, r6, #1
	strb r1, [r0]
	add r3, r3, #6
	add r5, r5, #6
	cmp r6, #4
	blt _0225A76A
	ldr r0, [sp]
	add r1, r4, #0
	ldr r0, [r0, #0xc]
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A700
