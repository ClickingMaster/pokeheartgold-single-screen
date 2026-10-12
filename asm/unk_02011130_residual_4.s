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
	thumb_func_start CircleWipe_GetScanlineSpan
CircleWipe_GetScanlineSpan: ; 0x02011A44
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r1, [sp]
	asr r1, r0, #6
	lsr r1, r1, #0x19
	add r1, r0, r1
	asr r1, r1, #7
	ldr r5, [sp, #0x20]
	ldr r4, [sp, #0x24]
	sub r0, r3, r2
	bpl _02011A5C
	neg r0, r0
_02011A5C:
	cmp r0, r1
	blt _02011A6A
	mov r0, #0
	str r0, [r5]
	add sp, #0xc
	str r0, [r4]
	pop {r4, r5, r6, r7, pc}
_02011A6A:
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	asr r7, r0, #0x1f
	lsl r0, r1, #0xc
	asr r1, r0, #0x1f
	add r2, r0, #0
	add r3, r1, #0
	bl _ll_mul
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	add r6, r1, #0
	add r1, r7, #0
	add r2, r0, #0
	add r3, r7, #0
	bl _ll_mul
	add r2, r0, #0
	add r3, r1, #0
	mov r0, #2
	ldr r1, [sp, #8]
	mov r7, #0
	lsl r0, r0, #0xa
	add r0, r1, r0
	adc r6, r7
	lsl r1, r6, #0x14
	lsr r0, r0, #0xc
	orr r0, r1
	mov r1, #2
	lsl r1, r1, #0xa
	add r2, r2, r1
	adc r3, r7
	lsl r1, r3, #0x14
	lsr r2, r2, #0xc
	orr r2, r1
	sub r0, r0, r2
	bl FX_Sqrt
	asr r2, r0, #0xc
	ldr r0, [sp]
	sub r0, r0, r2
	str r0, [r5]
	bpl _02011AC4
	add r0, r7, #0
	str r0, [r5]
_02011AC4:
	ldr r1, [r5]
	lsl r0, r2, #1
	add r0, r1, r0
	str r0, [r4]
	cmp r0, #0xff
	ble _02011AD4
	mov r0, #0xff
	str r0, [r4]
_02011AD4:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end CircleWipe_GetScanlineSpan
	thumb_func_start CircleWipe_BuildScanlines
CircleWipe_BuildScanlines: ; 0x02011AD8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	mov r1, #0
	add r5, r0, #0
	bl sub_02010EE0
	add r7, r0, #0
	mov r4, #0
	add r6, r7, #0
_02011AEA:
	ldr r2, [r5, #0x14]
	cmp r4, r2
	bgt _02011B04
	add r0, sp, #0xc
	str r0, [sp]
	add r0, sp, #8
	str r0, [sp, #4]
	ldr r0, [r5, #0xc]
	ldr r1, [r5, #0x10]
	add r3, r4, #0
	bl CircleWipe_GetScanlineSpan
	b _02011B3E
_02011B04:
	lsl r0, r2, #1
	cmp r4, r0
	bgt _02011B2C
	sub r0, r0, r4
	lsl r0, r0, #1
	add r1, r7, r0
	mov r0, #3
	lsl r0, r0, #8
	ldrsh r0, [r1, r0]
	str r0, [sp, #0xc]
	ldr r0, [r5, #0x14]
	lsl r0, r0, #1
	sub r0, r0, r4
	lsl r0, r0, #1
	add r1, r7, r0
	mov r0, #0x12
	lsl r0, r0, #6
	ldrsh r0, [r1, r0]
	str r0, [sp, #8]
	b _02011B3E
_02011B2C:
	add r0, sp, #0xc
	str r0, [sp]
	add r0, sp, #8
	str r0, [sp, #4]
	ldr r0, [r5, #0xc]
	ldr r1, [r5, #0x10]
	add r3, r4, #0
	bl CircleWipe_GetScanlineSpan
_02011B3E:
	mov r0, #3
	ldr r1, [sp, #0xc]
	lsl r0, r0, #8
	strh r1, [r6, r0]
	mov r0, #0x12
	ldr r1, [sp, #8]
	lsl r0, r0, #6
	strh r1, [r6, r0]
	add r4, r4, #1
	add r6, r6, #2
	cmp r4, #0xc0
	blt _02011AEA
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end CircleWipe_BuildScanlines
