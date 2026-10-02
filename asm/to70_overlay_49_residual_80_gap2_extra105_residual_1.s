	.extern ov49_0225E3B8
	.extern ov49_0225E47C
	.extern ov49_0225E4A4
	.extern ov49_022693A4
	.extern ov49_022693D4
	.extern ov49_022693F8
	.extern ov49_0226A8DC
	.include "asm/macros.inc"
	.include "overlay_49.inc"
	.include "global.inc"

.text
	thumb_func_start ov49_02269240
ov49_02269240: ; 0x02269240
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r7, r0, #0
	ldr r0, [sp, #0x30]
	str r1, [sp]
	str r0, [sp, #0x30]
	ldr r0, [r7, #4]
	str r2, [sp, #4]
	add r5, r3, #0
	bl ov45_0222AD3C
	add r6, r0, #0
	ldr r0, [r7, #4]
	bl ov45_0222AD2C
	ldr r0, [sp]
	mov r4, #0
	lsl r1, r0, #1
	add r0, r0, r1
	str r0, [sp, #0x10]
	ldr r0, [sp]
	mov r1, #3
	bl _u32_div_f
	str r1, [sp, #8]
	mov r0, #0xc
	ldr r1, _02269398 ; =ov49_0226A8DC
	mul r0, r6
	add r0, r1, r0
	str r0, [sp, #0xc]
	add r0, r7, #0
	str r0, [sp, #0x14]
	add r0, #0x2c
	str r0, [sp, #0x14]
	ldr r0, [sp]
	mov r1, #0xe
	mul r1, r0
	str r1, [sp, #0x18]
_0226928C:
	ldr r1, [sp, #0x10]
	ldr r0, [r7, #4]
	add r1, r4, r1
	bl ov45_0222ADE8
	mov r6, #0
	cmp r5, #1
	bne _022692A4
	ldr r1, [sp, #0x30]
	cmp r4, r1
	bne _022692A4
	mov r6, #1
_022692A4:
	cmp r0, #1
	bne _0226938A
	cmp r4, #0
	beq _022692B6
	cmp r4, #1
	beq _0226931A
	cmp r4, #2
	beq _02269350
	b _0226938A
_022692B6:
	ldr r0, [r7, #8]
	ldr r1, [sp]
	bl ov49_0225E47C
	cmp r0, #1
	bne _0226938A
	ldr r0, [sp, #8]
	cmp r0, #2
	bne _022692EC
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #4]
	ldrh r3, [r3, #2]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693D4
	ldr r0, [sp, #0xc]
	ldrh r3, [r0, #8]
	ldr r0, _0226939C ; =0x0000FFFE
	cmp r3, r0
	beq _0226930E
	ldr r0, [sp, #4]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693A4
	b _0226930E
_022692EC:
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #4]
	ldrh r3, [r3]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693D4
	ldr r0, [sp, #0xc]
	ldrh r3, [r0, #4]
	ldr r0, _0226939C ; =0x0000FFFE
	cmp r3, r0
	beq _0226930E
	ldr r0, [sp, #4]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693A4
_0226930E:
	ldr r1, [sp, #0x10]
	ldr r0, [r7, #4]
	add r1, r4, r1
	bl ov45_0222ADF8
	b _0226938A
_0226931A:
	ldr r0, [sp, #8]
	cmp r0, #2
	bne _02269324
	mov r1, #1
	b _02269326
_02269324:
	mov r1, #0
_02269326:
	ldr r2, [sp, #0x14]
	ldr r0, [sp, #0x18]
	ldr r3, [sp]
	add r0, r2, r0
	ldr r2, [r7, #8]
	bl ov49_022693F8
	cmp r0, #1
	bne _0226938A
	ldr r1, [sp, #0x10]
	ldr r0, [r7, #4]
	add r1, r4, r1
	bl ov45_0222ADF8
	ldr r0, [sp, #4]
	ldr r3, _022693A0 ; =0x000005C6
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693A4
	b _0226938A
_02269350:
	ldr r0, [r7, #8]
	ldr r1, [sp]
	bl ov49_0225E4A4
	cmp r0, #1
	bne _0226938A
	ldr r1, [sp, #0x10]
	ldr r0, [r7, #4]
	add r1, r4, r1
	bl ov45_0222ADF8
	ldr r0, [sp, #8]
	cmp r0, #2
	bne _0226937C
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #4]
	ldrh r3, [r3, #0xa]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693A4
	b _0226938A
_0226937C:
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #4]
	ldrh r3, [r3, #6]
	add r1, r5, #0
	add r2, r6, #0
	bl ov49_022693A4
_0226938A:
	add r4, r4, #1
	cmp r4, #3
	bge _02269392
	b _0226928C
_02269392:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_02269398: .word ov49_0226A8DC
_0226939C: .word 0x0000FFFE
_022693A0: .word 0x000005C6
	thumb_func_end ov49_02269240
