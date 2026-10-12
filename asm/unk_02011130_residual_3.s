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
	thumb_func_start CircleWipe_Init
CircleWipe_Init: ; 0x02011918
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r0, #0
	add r4, r1, #0
	mov r0, #0
	mov r1, #2
	ldrsh r0, [r4, r0]
	ldrsh r1, [r4, r1]
	str r2, [sp, #0x18]
	str r3, [sp, #0x1c]
	ldr r6, [sp, #0x38]
	ldr r7, [sp, #0x3c]
	bl sub_02010A6C
	str r0, [sp, #0x20]
	ldrb r1, [r4, #8]
	ldr r3, [sp, #0x44]
	add r0, r5, #0
	add r2, r6, #0
	bl ScanlineBufferController_Init
	mov r0, #0
	ldrsh r1, [r4, r0]
	lsl r1, r1, #7
	str r1, [r5, #0xc]
	mov r1, #4
	ldrsh r1, [r4, r1]
	str r1, [r5, #0x10]
	mov r1, #6
	ldrsh r1, [r4, r1]
	str r1, [r5, #0x14]
	ldr r1, [sp, #0x20]
	str r1, [r5, #0x18]
	ldr r1, [sp, #0x18]
	str r1, [r5, #0x1c]
	ldr r1, [sp, #0x1c]
	str r1, [r5, #0x20]
	str r0, [r5, #0x24]
	ldr r0, [sp, #0x40]
	str r7, [r5, #0x30]
	str r0, [r5, #0x34]
	ldr r0, [sp, #0x44]
	str r0, [r5, #0x28]
	ldrb r0, [r4, #0xb]
	str r0, [r5, #0x2c]
	add r0, r5, #0
	bl CircleWipe_BuildScanlines
	ldr r0, _020119E8 ; =sub_02010F00
	ldr r2, _020119EC ; =0x000003FF
	add r1, r5, #0
	bl SysTask_CreateOnVWaitQueue
	add r0, r5, #0
	mov r1, #0
	bl sub_02010EE0
	mov r1, #3
	str r6, [sp]
	lsl r1, r1, #8
	ldrsh r1, [r0, r1]
	str r1, [sp, #4]
	mov r1, #0
	str r1, [sp, #8]
	mov r1, #0x12
	lsl r1, r1, #6
	ldrsh r0, [r0, r1]
	str r0, [sp, #0xc]
	mov r0, #0xc0
	str r0, [sp, #0x10]
	ldr r0, [r5, #0x2c]
	str r0, [sp, #0x14]
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #0xa]
	ldrb r3, [r4, #8]
	add r0, r7, #0
	bl sub_02010F84
	ldrb r0, [r4, #8]
	cmp r0, #0
	bne _020119C8
	ldr r3, [r5, #0x2c]
	add r0, r7, #0
	mov r1, #1
	add r2, r6, #0
	bl sub_02011068
	b _020119D4
_020119C8:
	ldr r3, [r5, #0x2c]
	add r0, r7, #0
	mov r1, #2
	add r2, r6, #0
	bl sub_02011068
_020119D4:
	ldr r0, [sp, #0x44]
	ldr r2, _020119F0 ; =ScanlineWindow_HBlankCallback
	str r0, [sp]
	ldr r0, [r5, #0x34]
	add r1, r5, #0
	add r3, r6, #0
	bl sub_0200FF88
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_020119E8: .word sub_02010F00
_020119EC: .word 0x000003FF
_020119F0: .word ScanlineWindow_HBlankCallback
	thumb_func_end CircleWipe_Init
