	.include "asm/macros.inc"
	.include "overlay_120.inc"
	.include "global.inc"

	.public ov120_022601BC
	.public ov120_022601D8
	.public ov120_022601F4
	.public ov120_022601F8
	.public ov120_022601FC
	.public ov120_02260200
	.public ov120_022602E4
	.public ov120_022602FC
	.public ov120_02260314
	.public ov120_0226032C

	.public _0226019C

	.text
	.public ov120_0225F020
	.public ov120_0225F038
	.public ov120_0225F040
	.public ov120_0225F08C
	.public ov120_0225F0FC
	.public ov120_0225F118
	.public ov120_0225F120
	.public ov120_0225F14C
	.public ov120_0225F1E8
	.public ov120_0225F240
	.public ov120_0225F268
	.public ov120_0225F294
	.public ov120_0225F2B8
	.public ov120_0225F318
	.public ov120_0225F4DC
	.public ov120_0225F6AC
	.public ov120_0225F6BC
	.public ov120_0225F6FC
	.public ov120_0225F704
	.public ov120_0225F714
	.public ov120_0225F8B0
	.public ov120_0225F90C
	.public ov120_0225F970
	.public ov120_0225F9D4
	.public ov120_0225FA5C
	.public ov120_0225FA68
	.public ov120_0225FA74
	.public ov120_0225FBAC
	.public ov120_0225FBB8
	.public ov120_0225FBC4
	.public ov120_0225FCFC
	.public ov120_0225FD08
	.public ov120_0225FD14
	.public ov120_0225FD2C
	.public ov120_0225FDA0
	.public ov120_0225FE08
	.public ov120_0225FECC
	.public ov120_0225FEE8
	.public ov120_0225FF00
	thumb_func_start ov120_0225F08C
ov120_0225F08C: ; 0x0225F08C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r0, #0
	add r1, r4, #0
	add r1, #0x30
	ldrb r1, [r1]
	cmp r1, #0
	bne _0225F0A2
	add sp, #8
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0225F0A2:
	bl ov01_021EFE44
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0x14
	bl ov01_021EFE44
	add r7, r5, #0
	ldr r1, [r4]
	orr r7, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r5, r0, #1
	mov r0, #0x2c
	ldr r1, [r4, #0x14]
	ldrsh r3, [r4, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r6, r0, #1
	mov r0, #0x2e
	ldrsh r2, [r4, r0]
	add r0, r3, r5
	lsl r0, r0, #0x10
	sub r1, r2, r6
	asr r0, r0, #0x10
	str r0, [sp]
	add r0, r4, #0
	add r0, #0x31
	ldrb r0, [r0]
	add r2, r2, r6
	sub r3, r3, r5
	str r0, [sp, #4]
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	ldr r0, [r4, #0x28]
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov01_021F0960
	add r0, r7, #0
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov120_0225F08C
