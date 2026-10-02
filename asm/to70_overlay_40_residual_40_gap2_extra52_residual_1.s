	.extern ov40_0224320C
	.extern ov40_0224355C
	.extern ov40_02243E80
	.extern ov40_02243EEC
	.extern ov40_02243F38
	.extern ov40_02244054
	.extern ov40_02245C40
	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"
	.extern ov40_02243614

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

	thumb_func_start ov40_022437C0
ov40_022437C0: ; 0x022437C0
	push {r3, r4, r5, r6, r7, lr}
	add r4, r2, #0
	mov r2, #0x7d
	lsl r2, r2, #2
	add r5, r0, #0
	ldr r0, [r4, r2]
	cmp r0, #1
	beq _022437D2
	b _02243950
_022437D2:
	add r0, r2, #0
	add r0, #0xa4
	ldr r0, [r4, r0]
	cmp r0, #1
	beq _022437E2
	mov r0, #1
	add r2, #0xa4
	str r0, [r4, r2]
_022437E2:
	cmp r1, #0
	beq _022437E8
	b _02243950
_022437E8:
	cmp r5, #0xb
	bhi _0224381C
	mov r0, #0x1c
	mul r0, r5
	add r2, r4, r0
	ldr r0, [r2, #8]
	cmp r0, #1
	bne _02243806
	mov r0, #0xa7
	mov r1, #2
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r0, #4
	str r5, [r4, r0]
	b _02243814
_02243806:
	mov r0, #0xa7
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r4, r0]
	ldr r1, [r2, #4]
	add r0, r0, #4
	str r1, [r4, r0]
_02243814:
	ldr r0, _02243954 ; =0x0000057B
	bl PlaySE
	pop {r3, r4, r5, r6, r7, pc}
_0224381C:
	cmp r5, #0x16
	bne _02243830
	mov r0, #0x6e
	mov r1, #0
	lsl r0, r0, #2
	strh r1, [r4, r0]
	mov r1, #2
	add r0, r0, #2
	strh r1, [r4, r0]
	b _02243862
_02243830:
	cmp r5, #0x17
	bne _02243844
	mov r0, #0x6e
	mov r1, #3
	lsl r0, r0, #2
	strh r1, [r4, r0]
	mov r1, #2
	add r0, r0, #2
	strh r1, [r4, r0]
	b _02243862
_02243844:
	add r0, r5, #0
	sub r0, #0xc
	mov r1, #5
	bl _u32_div_f
	mov r0, #0x6e
	lsl r0, r0, #2
	strh r1, [r4, r0]
	add r0, r5, #0
	sub r0, #0xc
	mov r1, #5
	bl _u32_div_f
	ldr r1, _02243958 ; =0x000001BA
	strh r0, [r4, r1]
_02243862:
	cmp r5, #0xc
	blo _02243934
	cmp r5, #0x15
	bhi _02243934
	mov r0, #0x82
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02243950
	ldr r0, _02243954 ; =0x0000057B
	bl PlaySE
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r7, [r4, r0]
	mov r0, #0x1c
	add r6, r7, #0
	mul r6, r0
	add r0, r5, #0
	sub r0, #0xb
	str r0, [r4, r6]
	add r1, r4, r6
	ldr r0, [r4, r6]
	ldr r1, [r1, #8]
	bl ov40_02244054
	add r1, r0, #0
	add r0, r4, r6
	ldr r0, [r0, #0xc]
	bl ManagedSprite_SetAnim
	mov r1, #1
	add r0, r4, #0
	add r2, r1, #0
	bl ov40_02243E80
	sub r5, #0xc
	add r0, r4, #0
	add r1, r5, #0
	bl ov40_02243EEC
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov40_02243E80
	add r0, r4, #0
	mov r1, #2
	mov r2, #1
	bl ov40_02243E80
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #2
	bl ov40_02243F38
	mov r0, #0x73
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #3
	bl ManagedSprite_SetAnim
	add r0, r4, r6
	ldr r2, [r0, #4]
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r3, [r4, r0]
	add r1, r7, #1
	cmp r1, r3
	bne _02243904
	add r1, r0, #0
	mov r2, #1
	add r1, #0x98
	str r2, [r4, r1]
	add r1, r0, #0
	mov r2, #0
	add r1, #0x9c
	str r2, [r4, r1]
	add r0, #0xa0
	str r2, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_02243904:
	mov r3, #0x1c
	mul r3, r1
	add r3, r4, r3
	ldr r3, [r3, #4]
	cmp r2, r3
	beq _02243926
	add r1, r0, #0
	mov r2, #1
	add r1, #0x98
	str r2, [r4, r1]
	add r1, r0, #0
	add r1, #0x9c
	str r3, [r4, r1]
	mov r1, #0
	add r0, #0xa0
	str r1, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_02243926:
	add r2, r0, #0
	mov r3, #2
	add r2, #0x98
	str r3, [r4, r2]
	add r0, #0x9c
	str r1, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_02243934:
	cmp r5, #0x16
	ldr r0, _02243954 ; =0x0000057B
	bne _02243946
	bl PlaySE
	add r0, r4, #0
	bl ov40_02243614
	pop {r3, r4, r5, r6, r7, pc}
_02243946:
	bl PlaySE
	add r0, r4, #0
	bl ov40_0224355C
_02243950:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02243954: .word 0x0000057B
_02243958: .word 0x000001BA
	thumb_func_end ov40_022437C0
