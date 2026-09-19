	.include "asm/macros.inc"
	.public ov80_0222E268
	.public ov80_0222E2B8
	.public ov80_0222E328
	.public FrontierSystem_GetFrontierMap
	.public Frontier_GetLaunchArgs
	.public Options_GetTextFrameDelay
	.public ov80_0222E344
	.text

	thumb_func_start ov80_0222E268
ov80_0222E268:
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	add r6, r1, #0
	add r7, r2, #0
	str r3, [sp, #4]
	ldr r4, [sp, #0x20]
	bl ov80_0222E2B8
	add r0, r5, #0
	add r1, r6, #0
	add r2, r7, #0
	bl ov80_0222E328
	cmp r4, #0
	bne _0222E2A2_r31
	add r0, r5, #0
	bl FrontierSystem_GetFrontierMap
	ldr r0, [r0, #8]
	bl Frontier_GetLaunchArgs
	ldr r0, [r0, #4]
	bl Options_GetTextFrameDelay
	add r2, r0, #0
	mov r0, #0
	mov r1, #1
	b _0222E2A8_r31
_0222E2A2_r31:
	ldrb r2, [r4]
	ldrb r0, [r4, #1]
	ldrb r1, [r4, #2]
_0222E2A8_r31:
	str r0, [sp]
	ldr r3, [sp, #4]
	add r0, r5, #0
	bl ov80_0222E344
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov80_0222E268
