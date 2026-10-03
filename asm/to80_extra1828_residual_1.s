	.extern ov40_0224320C
	.extern ov40_0224355C
	.extern ov40_02243614
	.extern ov40_02243E80
	.extern ov40_02243EEC
	.extern ov40_02243F38
	.extern ov40_02244054
	.extern ov40_02245C40
	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"
	.extern ov40_022437C0

.text
	thumb_func_start ov40_022436D4
ov40_022436D4: ; 0x022436D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x64
	mov r1, #0x8e
	add r4, r0, #0
	lsl r1, r1, #2
	mov r0, #0
	add r1, r4, r1
	add r2, r4, #0
_022436E4:
	add r0, r0, #1
	str r1, [r2, #0x10]
	add r1, r1, #4
	add r2, #0x1c
	cmp r0, #0xc
	blt _022436E4
	ldr r5, _022437AC ; =ov40_02245C40
	add r3, sp, #4
	mov r2, #0x30
_022436F6:
	ldrh r1, [r5]
	add r5, r5, #2
	strh r1, [r3]
	add r3, r3, #2
	sub r2, r2, #1
	bne _022436F6
	cmp r0, #0x18
	bge _02243764
	add r2, sp, #4
	lsl r1, r0, #3
	add r1, r2, r1
	lsl r2, r0, #2
	mov r7, #0x5b
	ldr r3, _022437B0 ; =0x0000023B
	add r2, r4, r2
	mvn r7, r7
_02243716:
	mov r5, #0x5d
	mov r6, #0x59
	mvn r5, r5
	mvn r6, r6
	ldrsh r5, [r1, r5]
	ldrsh r6, [r1, r6]
	add r0, r0, #1
	sub r6, r5, r6
	mov r5, #0x8e
	lsl r5, r5, #2
	strb r6, [r2, r5]
	mov r5, #0x5f
	mov r6, #0x5b
	mvn r5, r5
	mvn r6, r6
	ldrsh r5, [r1, r5]
	ldrsh r6, [r1, r6]
	sub r6, r5, r6
	ldr r5, _022437B4 ; =0x0000023A
	strb r6, [r2, r5]
	mov r5, #0x5d
	mov r6, #0x59
	mvn r5, r5
	mvn r6, r6
	ldrsh r5, [r1, r5]
	ldrsh r6, [r1, r6]
	add r6, r5, r6
	ldr r5, _022437B8 ; =0x00000239
	strb r6, [r2, r5]
	mov r5, #0x5f
	mvn r5, r5
	ldrsh r6, [r1, r5]
	ldrsh r5, [r1, r7]
	add r1, #8
	add r5, r6, r5
	strb r5, [r2, r3]
	add r2, r2, #4
	cmp r0, #0x18
	blt _02243716
_02243764:
	mov r0, #0x29
	lsl r0, r0, #4
	mov r3, #0x98
	strb r3, [r4, r0]
	mov r2, #0x20
	add r1, r0, #2
	strb r2, [r4, r1]
	mov r2, #0xb8
	add r1, r0, #1
	strb r2, [r4, r1]
	mov r1, #0x80
	add r5, r0, #3
	strb r1, [r4, r5]
	add r5, r0, #4
	strb r3, [r4, r5]
	add r3, r0, #6
	strb r1, [r4, r3]
	add r1, r0, #5
	strb r2, [r4, r1]
	add r1, r0, #7
	mov r2, #0xe0
	strb r2, [r4, r1]
	mov r1, #0x6d
	sub r0, #0x58
	str r1, [sp]
	ldr r2, _022437BC ; =ov40_022437C0
	add r0, r4, r0
	mov r1, #0x18
	add r3, r4, #0
	bl TouchHitboxController_Create
	mov r1, #0x8d
	lsl r1, r1, #2
	str r0, [r4, r1]
	add sp, #0x64
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022437AC: .word ov40_02245C40
_022437B0: .word 0x0000023B
_022437B4: .word 0x0000023A
_022437B8: .word 0x00000239
_022437BC: .word ov40_022437C0
	thumb_func_end ov40_022436D4
