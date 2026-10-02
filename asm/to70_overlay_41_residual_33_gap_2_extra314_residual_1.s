	.extern ov41_02245EE0
	.extern ov41_022484E8
	.extern ov41_02248584
	.extern ov41_02248B20
	.extern ov41_022499F0
	.extern ov41_02249A50
	.extern ov41_02249AF4
	.extern ov41_02249B94
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_022485DC
ov41_022485DC: ; 0x022485DC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x48
	add r5, r0, #0
	ldr r0, [r5, #0x58]
	add r4, r2, #0
	str r4, [sp, #0x40]
	str r0, [sp, #0x28]
	ldr r0, [r5, #0x44]
	add r6, r1, #0
	mov r1, #0
	str r0, [sp, #0x2c]
	str r1, [sp, #0x38]
	str r1, [sp, #0x3c]
	cmp r6, #0
	beq _02248604
	cmp r6, #1
	beq _02248610
	cmp r6, #2
	beq _0224861E
	b _0224862C
_02248604:
	ldr r0, [r5, #0x50]
	str r4, [sp, #0xc]
	ldrb r0, [r0, r4]
	add r7, r1, #0
	str r0, [sp, #0x44]
	b _02248630
_02248610:
	add r0, r4, #0
	str r0, [sp, #0xc]
	add r0, #0x64
	str r0, [sp, #0xc]
	add r7, r4, #1
	str r1, [sp, #0x44]
	b _02248630
_0224861E:
	add r0, r4, #0
	str r0, [sp, #0xc]
	add r0, #0x64
	str r0, [sp, #0xc]
	add r7, r4, #1
	str r1, [sp, #0x44]
	b _02248630
_0224862C:
	bl GF_AssertFail
_02248630:
	ldr r0, [sp, #0xc]
	ldr r1, [r5, #0x48]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	lsl r1, r7, #2
	str r0, [sp, #0x30]
	ldr r2, [r5, #0x4c]
	cmp r0, #0
	ldr r1, [r2, r1]
	str r1, [sp, #0x34]
	bne _0224864A
	bl GF_AssertFail
_0224864A:
	ldr r0, [sp, #0x34]
	cmp r0, #0
	bne _02248654
	bl GF_AssertFail
_02248654:
	ldr r2, [r5, #4]
	add r0, r6, #0
	add r1, r4, #0
	bl ov41_022484E8
	add r7, r0, #0
	add r0, sp, #0x28
	bl ov41_02245EE0
	str r0, [sp, #0x10]
	ldr r0, [r5, #8]
	ldr r1, [sp, #0x10]
	add r2, r6, #0
	bl ov41_022499F0
	mov r1, #0xc
	mul r1, r6
	add r1, r5, r1
	ldr r2, [r1, #0xc]
	lsl r1, r7, #4
	add r1, r2, r1
	ldr r1, [r1, #0xc]
	str r0, [sp, #0x14]
	bl ov41_02249A50
	ldr r0, [sp, #0x14]
	add r1, sp, #0x1c
	add r2, sp, #0x18
	bl ov41_02249B94
	ldr r0, [sp, #0x1c]
	add r1, r4, #0
	str r0, [sp]
	ldr r0, [sp, #0x18]
	add r2, sp, #0x24
	str r0, [sp, #4]
	ldr r0, [r5, #4]
	add r3, sp, #0x20
	str r0, [sp, #8]
	add r0, r6, #0
	bl ov41_02248584
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x24]
	ldr r2, [sp, #0x20]
	bl ov41_02249AF4
	ldr r1, [sp, #0x10]
	add r0, r5, #0
	add r2, r6, #0
	add r3, r7, #0
	bl ov41_02248B20
	add sp, #0x48
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov41_022485DC
