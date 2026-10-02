	.extern ov88_02258D90
	.extern ov88_0225918C
	.extern ov88_02259924
	.include "asm/macros.inc"
	.include "overlay_88.inc"
	.include "global.inc"

.text
	thumb_func_start ov88_02258F88
ov88_02258F88: ; 0x02258F88
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r4, r0, #0
	str r1, [sp, #0x10]
	str r2, [sp, #0x14]
	ldrb r1, [r4, #1]
	mov r0, #0x3c
	ldrb r2, [r4, #2]
	mul r0, r1
	ldrb r1, [r4]
	add r2, r2, r0
	mov r0, #0xe1
	lsl r0, r0, #4
	mul r0, r1
	ldr r1, [r4, #8]
	add r0, r2, r0
	bl _u32_div_f
	ldrb r1, [r4, #5]
	add r5, r0, #0
	cmp r1, #0
	beq _02259036
	mov r2, #0
	strh r2, [r4, #6]
	strb r2, [r4, #5]
	mov r0, #4
	str r0, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	mov r0, #8
	str r0, [sp, #8]
	ldr r0, [sp, #0x14]
	str r2, [sp, #0xc]
	ldr r0, [r0]
	mov r1, #3
	mov r3, #5
	bl FillBgTilemapRect
	ldr r0, [sp, #0x14]
	mov r1, #3
	ldr r0, [r0]
	bl ScheduleBgTilemapBufferTransfer
	mov r0, #0
	str r0, [sp, #0x1c]
_02258FE2:
	ldr r0, [sp, #0x1c]
	mov r1, #3
	sub r6, r1, r0
	mov r0, #0xa
	add r7, r6, #0
	mul r7, r0
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	mov r4, #0
	str r0, [sp, #0x20]
_02258FF6:
	mov r0, #9
	sub r3, r0, r4
	add r0, r3, r7
	cmp r0, r5
	blo _02259022
	cmp r0, r5
	bne _02259008
	mov r2, #1
	b _02259012
_02259008:
	cmp r6, #3
	bne _02259010
	mov r2, #2
	b _02259012
_02259010:
	mov r2, #0
_02259012:
	ldr r0, [sp, #0x20]
	lsl r3, r3, #0x18
	str r0, [sp]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	lsr r3, r3, #0x18
	bl ov88_02258D90
_02259022:
	add r4, r4, #1
	cmp r4, #0xa
	blt _02258FF6
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	str r0, [sp, #0x1c]
	cmp r0, #4
	blt _02258FE2
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_02259036:
	cmp r5, #0x28
	bhs _022590B8
	ldrh r1, [r4, #6]
	add r1, r1, #1
	strh r1, [r4, #6]
	ldrh r6, [r4, #6]
	mov r1, #0x1b
	lsr r3, r6, #0x1f
	lsl r2, r6, #0x1b
	sub r2, r2, r3
	ror r2, r1
	mov r1, #0xa
	add r7, r3, r2
	bl _u32_div_f
	str r1, [sp, #0x18]
	add r0, r5, #0
	mov r1, #0xa
	bl _u32_div_f
	cmp r7, #0
	bne _0225909E
	ldr r2, [r4, #8]
	mov r1, #0x1e
	mul r1, r2
	lsr r4, r1, #2
	mov r2, #6
	mov r5, #0
_0225906E:
	add r1, r5, #1
	add r3, r4, #0
	mul r3, r1
	cmp r3, r6
	bls _02259080
	ldr r1, _022590BC ; =ov88_02259924
	lsl r2, r5, #2
	ldr r2, [r1, r2]
	b _02259086
_02259080:
	add r5, r1, #0
	cmp r1, #4
	blt _0225906E
_02259086:
	lsl r0, r0, #0x18
	ldr r3, [sp, #0x18]
	lsr r0, r0, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	lsr r3, r3, #0x18
	bl ov88_02258D90
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_0225909E:
	cmp r7, #0x10
	bne _022590B8
	lsl r0, r0, #0x18
	ldr r3, [sp, #0x18]
	lsr r0, r0, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	mov r2, #3
	lsr r3, r3, #0x18
	bl ov88_02258D90
_022590B8:
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022590BC: .word ov88_02259924
	thumb_func_end ov88_02258F88
