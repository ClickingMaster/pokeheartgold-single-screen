	.include "asm/macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"

	.public ov83_02245ACC

	.text

	.extern ov80_02237B24
	.extern GF_AssertFail
	.extern ov83_0224776C
	.extern sub_0203769C
	.extern ov83_02244ABC
	.extern ov83_0224777C
	.extern ov80_02237FA4
	.extern Save_Frontier_GetStatic
	.extern sub_0205C174
	.extern sub_0205C268
	.extern sub_02031108
	.extern ov80_02237D8C
	.extern ov83_022477C4
	.extern ov83_02245390
	.extern ov83_022453DC
	.extern Options_GetFrame
	.extern ov83_02247944
	.extern ov83_022448AC
	.extern ov83_02248054

	thumb_func_start ov83_02245ACC
ov83_02245ACC: ; 0x02245ACC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	ldrb r0, [r5, #9]
	add r4, r1, #0
	mov r1, #0
	add r7, r2, #0
	bl ov80_02237B24
	cmp r7, #5
	bne _02245AE6
	mov r6, #2
	b _02245AEA
_02245AE6:
	bl GF_AssertFail
_02245AEA:
	ldrb r7, [r5, #0x15]
	add r1, r4, #0
	add r0, r7, #0
	bl ov83_0224776C
	bl sub_0203769C
	cmp r0, #0
	bne _02245B9A
	cmp r4, r7
	bhs _02245B7A
	add r0, r5, #0
	mov r1, #5
	bl ov83_02244ABC
	mov r0, #0xaf
	lsl r0, r0, #2
	ldrb r1, [r5, #9]
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl ov83_0224777C
	ldrb r1, [r5, #9]
	ldr r0, [r5, #4]
	mov r2, #0x32
	bl ov80_02237FA4
	mov r0, #0xaf
	lsl r0, r0, #2
	ldrb r1, [r5, #9]
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl ov83_0224777C
	add r4, r0, #0
	mov r0, #0xaf
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Save_Frontier_GetStatic
	add r7, r0, #0
	ldrb r0, [r5, #9]
	add r1, r6, #0
	bl sub_0205C174
	str r0, [sp]
	ldrb r0, [r5, #9]
	add r1, r6, #0
	bl sub_0205C174
	bl sub_0205C268
	add r3, r4, #1
	lsl r3, r3, #0x10
	add r2, r0, #0
	ldr r1, [sp]
	add r0, r7, #0
	lsr r3, r3, #0x10
	bl sub_02031108
	ldrb r0, [r5, #9]
	bl ov80_02237D8C
	cmp r0, #1
	bne _02245C36
	ldrb r1, [r5, #0xf]
	mov r0, #0xf8
	bic r1, r0
	mov r0, #0x10
	orr r0, r1
	strb r0, [r5, #0xf]
	b _02245C36
_02245B7A:
	ldr r0, [r5, #0x24]
	mov r1, #5
	bl ov83_022477C4
	ldr r1, _02245C78 ; =0x000005B7
	add r2, r1, #3
	add r0, r5, r1
	ldrh r2, [r5, r2]
	ldrb r4, [r0, r6]
	add r1, r1, #3
	sub r2, #0x32
	strh r2, [r5, r1]
	ldrb r1, [r0, r6]
	add r1, r1, #1
	strb r1, [r0, r6]
	b _02245C36
_02245B9A:
	cmp r4, r7
	bhs _02245BBE
	ldr r0, [r5, #0x24]
	mov r1, #5
	bl ov83_022477C4
	ldr r1, _02245C78 ; =0x000005B7
	add r2, r1, #3
	add r0, r5, r1
	ldrh r2, [r5, r2]
	ldrb r4, [r0, r6]
	add r1, r1, #3
	sub r2, #0x32
	strh r2, [r5, r1]
	ldrb r1, [r0, r6]
	add r1, r1, #1
	strb r1, [r0, r6]
	b _02245C36
_02245BBE:
	add r0, r5, #0
	mov r1, #5
	bl ov83_02244ABC
	mov r0, #0xaf
	lsl r0, r0, #2
	ldrb r1, [r5, #9]
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl ov83_0224777C
	ldrb r1, [r5, #9]
	ldr r0, [r5, #4]
	mov r2, #0x32
	bl ov80_02237FA4
	mov r0, #0xaf
	lsl r0, r0, #2
	ldrb r1, [r5, #9]
	ldr r0, [r5, r0]
	add r2, r6, #0
	bl ov83_0224777C
	add r4, r0, #0
	mov r0, #0xaf
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Save_Frontier_GetStatic
	add r7, r0, #0
	ldrb r0, [r5, #9]
	add r1, r6, #0
	bl sub_0205C174
	str r0, [sp, #4]
	ldrb r0, [r5, #9]
	add r1, r6, #0
	bl sub_0205C174
	bl sub_0205C268
	add r3, r4, #1
	lsl r3, r3, #0x10
	add r2, r0, #0
	ldr r1, [sp, #4]
	add r0, r7, #0
	lsr r3, r3, #0x10
	bl sub_02031108
	ldrb r0, [r5, #9]
	bl ov80_02237D8C
	cmp r0, #1
	bne _02245C36
	ldrb r1, [r5, #0xf]
	mov r0, #0xf8
	bic r1, r0
	mov r0, #0x10
	orr r0, r1
	strb r0, [r5, #0xf]
_02245C36:
	add r0, r5, #0
	bl ov83_02245390
	add r1, r5, #0
	add r0, r5, #0
	add r1, #0x50
	bl ov83_022453DC
	mov r0, #0xae
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetFrame
	add r1, r0, #0
	add r0, r5, #0
	add r0, #0xc0
	bl ov83_02247944
	mov r1, #6
	add r3, r6, #0
	mul r3, r1
	ldr r1, _02245C7C ; =ov83_02248054
	lsl r2, r4, #1
	add r1, r1, r3
	ldrh r1, [r2, r1]
	add r0, r5, #0
	mov r2, #1
	bl ov83_022448AC
	strb r0, [r5, #0xa]
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02245C78: .word 0x000005B7
_02245C7C: .word ov83_02248054
	thumb_func_end ov83_02245ACC
