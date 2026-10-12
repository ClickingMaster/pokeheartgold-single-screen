	.include "asm/macros.inc"
	.include "unk_0201010C.inc"
	.include "global.inc"
	.text
	.public sub_02011130
	.public RectWipe_Begin
	.public RectWipe_Update
	.public DoubleRectWipe_Begin
	.public DoubleRectWipe_Update
	.public RectWipe_Init
	.public RectWipe_Step
	.public CircleWipe_Begin
	.public CircleWipe_Update
	.public CircleWipe_Init
	.public CircleWipe_Step
	.public CircleWipe_GetScanlineSpan
	.public CircleWipe_BuildScanlines
	.public WedgeWipe_Begin
	.public WedgeWipe_Update
	.public WedgeWipe_Init
	.public WedgeWipe_Step
	.public WedgeWipe_BuildScanlines
	.public SlopeWipe_Begin
	.public SlopeWipe_Update
	.public SlopeWipe_Init
	.public SlopeWipe_Step
	.public SlopeWipe_BuildScanlines
	.public MultiRectWipe_Begin
	.public MultiRectWipe_Update
	.public MultiRectWipe_Init
	.public MultiRectWipe_Step
	thumb_func_start RectWipe_Init
RectWipe_Init: ; 0x020117A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r4, r1, #0
	add r1, r4, #4
	add r5, r0, #0
	add r6, r2, #0
	str r1, [sp]
	add r1, r5, #0
	add r2, r5, #0
	add r7, r3, #0
	add r1, #0x20
	add r2, #0x10
	add r3, r4, #0
	str r6, [sp, #4]
	bl sub_02010AB0
	ldr r1, [sp, #0x30]
	str r1, [r5, #0x30]
	ldrb r0, [r4, #8]
	str r0, [r5, #0x34]
	str r6, [r5, #0x38]
	str r7, [r5, #0x3c]
	mov r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #0x34]
	str r0, [r5, #0x48]
	ldrb r2, [r4, #0xb]
	str r2, [r5, #0x44]
	str r1, [sp]
	ldrb r1, [r4]
	str r1, [sp, #4]
	ldrb r1, [r4, #1]
	str r1, [sp, #8]
	ldrb r1, [r4, #2]
	str r1, [sp, #0xc]
	ldrb r1, [r4, #3]
	str r1, [sp, #0x10]
	ldr r1, [r5, #0x44]
	str r1, [sp, #0x14]
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #0xa]
	ldrb r3, [r4, #8]
	bl sub_02010F84
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end RectWipe_Init
