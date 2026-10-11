	.include "asm/macros.inc"
	.public UnionChatLog_PrintMenuButton
	.public UnionChatLog_Update
	.public UnionChatLog_Task
	.public UnionChatLog_InitText
	.public UnionChatLog_AddWindows
	.public UnionChatLog_RemoveWindows
	.public UnionChatLog_Init
	.public UnionChatLog_Exit
	.public UnionChatLog_FreeBgs
	.public UnionChatLog_InitBgs
	.public UnionChatLog_InitSpriteResources
	.public UnionChatLog_CreateSprites
	.public UnionChatLog_NextLogIndex
	.public UnionChatLog_AddLogIndex
	.public UnionChatLog_DrawRow
	.public UnionChatLog_DrawRows
	.public UnionChatLog_SetThumbY
	.public UnionChatLog_UpdateScrollbar
	.public UnionChatLog_HandleTouchButtons
	.public UnionChatLog_HandleTouchScrollbar
	.public UnionChatLog_HandleScrollKeys
	.public UnionChatLog_HandleInput
	.public UnionChatLog_UpdateRowPress
	.public UnionChatLog_GetFriendOfText
	.public UnionChatLog_AddEntry
	.public UnionChatLog_IsNewMessage
	.public UnionChatLog_AppendMessage
	.public UnionChatLog_CollectMessages
	.public UnionChatLog_RequestRedraw
	.public UnionChatLog_InitTouchRepeat
	.public UnionChatLog_UpdateTouchRepeat
	.public UnionChatLog_IsTouchRepeatTick
	.public UnionChatLog_SetThumbFollowsScroll
	.public UnionChatLog_GetThumbFollowsScroll
	.public UnionChatLog_PressArrow
	.public UnionChatLog_UpdateArrows
	.public sUnionChatLogRowPressBgOffsets
	.public sUnionChatLogSpriteYOffsets
	.public sUnionChatLogScrollbarHitboxes
	.public sUnionChatLogBgTemplateSub2
	.public sUnionChatLogBgTemplateSub3
	.public sUnionChatLogBgTemplateSub1
	.public sUnionChatLogBgTemplateSub0
	.public sUnionChatLogButtonHitboxes
	.include "overlay_34.inc"
	.include "global.inc"

	.text

	thumb_func_start UnionChatLog_DrawRow
UnionChatLog_DrawRow: ; 0x0225DC18
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r7, r1, #0
	add r5, r0, #0
	mov r0, #0x38
	add r4, r7, #0
	mul r4, r0
	mov r0, #0x7e
	lsl r0, r0, #2
	add r6, r2, #0
	add r1, r5, r0
	ldr r0, [r6, #0x10]
	mov r3, #7
	str r0, [r1, r4]
	mov r0, #0x20
	str r0, [sp]
	str r3, [sp, #4]
	ldr r2, [r5, #0x24]
	add r2, #0xc
	str r2, [sp, #8]
	mov r2, #0
	str r2, [sp, #0xc]
	ldr r2, [r1, r4]
	mov r1, #0x18
	mul r1, r2
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x10]
	add r1, r3, #0
	mul r3, r7
	str r0, [sp, #0x14]
	mov r0, #0x30
	add r3, r3, #2
	str r0, [sp, #0x18]
	lsl r3, r3, #0x18
	ldr r0, [r5, #0x14]
	mov r2, #0
	lsr r3, r3, #0x18
	bl CopyToBgTilemapRect
	mov r0, #0x72
	lsl r0, r0, #2
	add r7, r5, r0
	add r0, r7, r4
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x76
	lsl r0, r0, #2
	add r0, r5, r0
	str r0, [sp, #0x1c]
	add r0, r0, r4
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x7a
	lsl r0, r0, #2
	add r5, r5, r0
	add r0, r5, r4
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r1, #1
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0225DCFC ; =0x000F0200
	mov r3, #0
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	ldr r2, [r6]
	add r0, r7, r4
	bl AddTextPrinterParameterizedWithColor
	mov r3, #0
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0225DD00 ; =0x00010200
	mov r1, #1
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	ldr r0, [sp, #0x1c]
	ldr r2, [r6, #4]
	add r0, r0, r4
	bl AddTextPrinterParameterizedWithColor
	add r0, r7, r4
	bl ScheduleWindowCopyToVram
	ldr r0, [sp, #0x1c]
	add r0, r0, r4
	bl ScheduleWindowCopyToVram
	ldr r2, [r6, #8]
	cmp r2, #0
	beq _0225DCF0
	mov r1, #1
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0225DCFC ; =0x000F0200
	mov r3, #0
	str r0, [sp, #8]
	add r0, r5, r4
	str r3, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
_0225DCF0:
	add r0, r5, r4
	bl ScheduleWindowCopyToVram
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225DCFC: .word 0x000F0200
_0225DD00: .word 0x00010200
	thumb_func_end UnionChatLog_DrawRow


	thumb_func_start UnionChatLog_DrawRows
UnionChatLog_DrawRows: ; 0x0225DD04
	push {r3, r4, r5, r6, r7, lr}
	mov r1, #0x27
	add r5, r0, #0
	lsl r1, r1, #4
	add r0, r1, #0
	ldr r2, [r5, r1]
	add r1, #0x18
	add r0, #0xdc
	ldrh r1, [r5, r1]
	ldr r0, [r2, r0]
	bl UnionChatLog_AddLogIndex
	add r6, r0, #0
	mov r0, #0x27
	lsl r0, r0, #4
	ldr r1, [r5, r0]
	add r0, #0xd8
	ldr r7, [r1, r0]
	cmp r7, #3
	ble _0225DD2E
	mov r7, #3
_0225DD2E:
	mov r0, #0xa2
	lsl r0, r0, #2
	ldrh r1, [r5, r0]
	add r0, r0, #4
	ldr r0, [r5, r0]
	cmp r1, r0
	beq _0225DD4C
	add r0, r5, #0
	bl UnionChatLog_RequestRedraw
	mov r0, #0xa2
	lsl r0, r0, #2
	ldrh r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
_0225DD4C:
	mov r0, #0xa5
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #1
	bne _0225DDB6
	mov r4, #0
	cmp r7, #0
	ble _0225DD84
_0225DD5C:
	mov r2, #0x27
	lsl r2, r2, #4
	mov r3, #0x1c
	ldr r2, [r5, r2]
	mul r3, r6
	add r0, r5, #0
	add r1, r4, #0
	add r2, r2, r3
	bl UnionChatLog_DrawRow
	mov r0, #0x27
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl UnionChatLog_NextLogIndex
	add r4, r4, #1
	add r6, r0, #0
	cmp r4, r7
	blt _0225DD5C
_0225DD84:
	ldr r0, [r5, #0x14]
	mov r1, #7
	bl GetBgTilemapBuffer
	mov r1, #6
	lsl r1, r1, #8
	bl DC_FlushRange
	ldr r0, [r5, #0x14]
	mov r1, #7
	bl GetBgTilemapBuffer
	add r2, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r3, #6
	ldr r0, [r5, #0x14]
	mov r1, #7
	lsl r3, r3, #8
	bl BgCopyOrUncompressTilemapBufferRangeToVram
	mov r0, #0xa5
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r5, r0]
_0225DDB6:
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end UnionChatLog_DrawRows
