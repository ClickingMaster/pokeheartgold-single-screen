#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen


	.text

	thumb_func_start ov112_021F33D8
ov112_021F33D8: ; 0x021F33D8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	add r6, r0, #0
	add r0, r1, #0
	str r1, [sp]
	bl Party_GetCount
	str r0, [sp, #0x18]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bgt _021F33F6
	b _021F3598
_021F33F6:
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r1, [sp, #0x1c]
	bl Party_GetMonByIndex
	mov r1, #3
	mov r2, #0
	add r5, r0, #0
	bl GetMonData
	cmp r0, #0
	bne _021F341E
	add r0, r5, #0
	mov r1, #0x4c
	mov r2, #0
	bl GetMonData
	cmp r0, #0
	beq _021F3420
_021F341E:
	b _021F358A
_021F3420:
	add r0, r5, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	ldrh r1, [r6]
	ldr r2, _021F359C ; =0xFFFFF800
	lsr r3, r0, #0x10
	lsr r0, r2, #0x15
	and r1, r2
	and r0, r3
	orr r0, r1
	strh r0, [r6]
	add r0, r5, #0
	mov r1, #0x70
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r0, r0, #0x1b
	ldrh r2, [r6]
	ldr r1, _021F35A0 ; =0xFFFF07FF
	lsr r0, r0, #0x10
	and r1, r2
	orr r0, r1
	strh r0, [r6]
	ldrh r1, [r6]
	lsl r0, r1, #0x15
	lsr r2, r0, #0x15
	mov r0, #0x7b
	lsl r0, r0, #2
	cmp r2, r0
	bne _021F3482
	lsl r0, r1, #0x10
	lsr r0, r0, #0x1b
	cmp r0, #1
	bne _021F3482
	add r0, r5, #0
	mov r1, #0
	bl Mon_UpdateShayminForm
	ldrh r1, [r6]
	ldr r0, _021F35A0 ; =0xFFFF07FF
	and r0, r1
	strh r0, [r6]
	mov r0, #1
	str r0, [sp, #0xc]
_021F3482:
	add r0, r5, #0
	mov r1, #6
	mov r2, #0
	bl GetMonData
	strh r0, [r6, #2]
	add r0, r5, #0
	mov r1, #7
	mov r2, #0
	bl GetMonData
	mov r1, #0
	str r0, [r6, #0xc]
	add r0, r5, #0
	add r2, r1, #0
	bl GetMonData
	str r0, [r6, #0x10]
	add r0, r5, #0
	mov r1, #0xc
	mov r2, #0
	bl GetMonData
	strb r0, [r6, #0x1f]
	add r0, r5, #0
	mov r1, #0xa
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x20
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #9
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x21
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #0xa1
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x22
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #0x75
	add r2, sp, #0x20
	bl GetMonData
	add r0, r6, #0
	add r0, #0x24
	add r1, sp, #0x20
	mov r2, #0xa
	bl CopyU16StringArrayN
	mov r4, #0
	str r4, [sp, #0x10]
	str r6, [sp, #8]
	add r7, r4, #0
_021F3500:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x36
	mov r2, #0
	bl GetMonData
	ldr r1, [sp, #8]
	mov r2, #0
	strh r0, [r1, #4]
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x3e
	bl GetMonData
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #0x10]
	add r4, r4, #1
	orr r0, r1
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	add r7, r7, #2
	add r0, r0, #2
	str r0, [sp, #8]
	cmp r4, #4
	blt _021F3500
	ldr r0, [sp, #0x10]
	mov r4, #0
	strb r0, [r6, #0x1e]
	str r4, [sp, #4]
	add r7, r4, #0
_021F353E:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x46
	mov r2, #0
	bl GetMonData
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #4]
	mov r2, #0
	orr r0, r1
	add r1, r4, #0
	str r0, [sp, #4]
	add r0, r5, #0
	add r1, #0xd
	bl GetMonData
	add r1, r6, r4
	add r4, r4, #1
	add r7, r7, #5
	strb r0, [r1, #0x18]
	cmp r4, #6
	blt _021F353E
	ldr r0, [sp, #4]
	str r0, [r6, #0x14]
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _021F357E
	add r0, r5, #0
	mov r1, #1
	bl Mon_UpdateShayminForm
_021F357E:
	ldr r0, [sp, #0x14]
	add r6, #0x38
	add r0, r0, #1
	str r0, [sp, #0x14]
	cmp r0, #6
	bge _021F3598
_021F358A:
	ldr r0, [sp, #0x1c]
	add r1, r0, #1
	ldr r0, [sp, #0x18]
	str r1, [sp, #0x1c]
	cmp r1, r0
	bge _021F3598
	b _021F33F6
_021F3598:
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F359C: .word 0xFFFFF800
_021F35A0: .word 0xFFFF07FF
	thumb_func_end ov112_021F33D8
