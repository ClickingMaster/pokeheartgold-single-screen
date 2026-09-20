#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.extern ov112_021F35A4
	.extern ov112_021F35C8
	.extern ov112_021F3608
	.extern ov112_021F3630

	.text

	thumb_func_start ov112_021F328C
ov112_021F328C: ; 0x021F328C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	str r1, [sp, #4]
	mov r1, #0
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	str r0, [sp]
	ldr r5, [sp, #0x10]
	bl Save_TrainerHouse_Get
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	bl TrainerHouseSet_CheckHasData
	cmp r0, #0
	bne _021F32B6
	ldr r0, [sp, #8]
	bl ov112_021F35A4
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_021F32B6:
	ldr r0, [sp, #4]
	ldr r1, _021F3368 ; =0x000001F2
	bl GF_CalcCRC16
	ldr r2, _021F3368 ; =0x000001F2
	ldr r1, [sp, #4]
	ldrh r1, [r1, r2]
	cmp r1, r0
	bne _021F3364
	ldr r0, [sp, #8]
	bl ov112_021F35A4
	str r0, [sp, #0xc]
	ldr r0, [sp]
	add r1, sp, #0x18
	bl ov112_021F3608
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	add r2, sp, #0x14
	bl ov112_021F35C8
	add r4, r0, #0
	ldr r0, [sp, #0xc]
	cmp r0, #0xa
	bge _021F32F0
	ldr r0, [sp, #0x14]
	cmp r0, #0
	beq _021F32FE
_021F32F0:
	ldr r0, [sp, #0xc]
	mov r5, #1
	sub r0, r0, #1
	str r0, [sp, #0xc]
	add r0, sp, #0x18
	ldrb r0, [r0, r4]
	str r0, [sp, #0x10]
_021F32FE:
	cmp r5, #0
	beq _021F3342
	ldr r0, [sp, #0xc]
	cmp r4, r0
	bge _021F3342
	cmp r4, r0
	bge _021F3342
	mov r0, #6
	lsl r0, r0, #6
	add r1, r4, #0
	mul r1, r0
	ldr r0, [sp, #8]
	add r5, r0, r1
	add r0, sp, #0x18
	add r6, r0, r4
_021F331C:
	mov r0, #6
	lsl r0, r0, #6
	add r3, r5, r0
	add r7, r5, #0
	mov r2, #0x30
_021F3326:
	ldmia r3!, {r0, r1}
	stmia r7!, {r0, r1}
	sub r2, r2, #1
	bne _021F3326
	ldrb r0, [r6, #1]
	add r4, r4, #1
	strb r0, [r6]
	mov r0, #6
	lsl r0, r0, #6
	add r5, r5, r0
	ldr r0, [sp, #0xc]
	add r6, r6, #1
	cmp r4, r0
	blt _021F331C
_021F3342:
	ldr r1, [sp, #0xc]
	mov r2, #6
	add r3, r1, #0
	lsl r2, r2, #6
	ldr r1, [sp, #8]
	mul r3, r2
	ldr r0, [sp, #4]
	add r1, r1, r3
	bl MI_CpuCopy8
	ldr r2, [sp, #0x10]
	ldr r0, [sp, #0xc]
	add r1, sp, #0x18
	strb r2, [r1, r0]
	ldr r0, [sp]
	bl ov112_021F3630
_021F3364:
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021F3368: .word 0x000001F2
	thumb_func_end ov112_021F328C
