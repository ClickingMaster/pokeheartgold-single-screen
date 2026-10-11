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

	thumb_func_start UnionChatLog_AddWindows
UnionChatLog_AddWindows: ; 0x0225D650
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r7, r0, #0
	mov r0, #0xa0
	str r0, [sp, #0x24]
	mov r0, #0xd0
	str r0, [sp, #0x20]
	mov r0, #5
	str r0, [sp, #0x1c]
	ldr r0, _0225D774 ; =0x00000265
	add r5, r1, #0
	str r2, [sp, #0x14]
	mov r4, #0
	mov r6, #3
	str r0, [sp, #0x18]
_0225D66E:
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x24]
	add r2, r4, #4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r2, r2, #0x18
	str r0, [sp, #0x10]
	add r0, r7, #0
	add r1, r5, #0
	lsr r2, r2, #0x18
	mov r3, #1
	bl AddWindowParameterized
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	bl CopyWindowToVram
	ldr r0, [sp, #0x1c]
	add r2, r4, #4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #5
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x20]
	add r1, r5, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r2, r2, #0x18
	str r0, [sp, #0x10]
	add r0, r7, #0
	add r1, #0x10
	lsr r2, r2, #0x18
	mov r3, #2
	bl AddWindowParameterized
	add r0, r5, #0
	add r0, #0x10
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x10
	bl CopyWindowToVram
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0xf
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x18]
	add r2, r4, #4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r1, r5, #0
	lsl r2, r2, #0x18
	str r0, [sp, #0x10]
	add r0, r7, #0
	add r1, #0x20
	lsr r2, r2, #0x18
	mov r3, #0xc
	bl AddWindowParameterized
	add r0, r5, #0
	add r0, #0x20
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x20
	bl CopyWindowToVram
	ldr r0, [sp, #0x24]
	add r4, r4, #1
	add r0, #0x10
	str r0, [sp, #0x24]
	ldr r0, [sp, #0x20]
	add r6, r6, #7
	add r0, #0x87
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x1c]
	add r5, #0x38
	add r0, r0, #7
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	add r0, #0x1e
	str r0, [sp, #0x18]
	cmp r4, #3
	blt _0225D66E
	mov r0, #0
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	ldr r0, _0225D778 ; =0x000002BF
	ldr r1, [sp, #0x14]
	str r0, [sp, #0x10]
	add r0, r7, #0
	mov r2, #4
	mov r3, #8
	bl AddWindowParameterized
	ldr r0, [sp, #0x14]
	mov r1, #0
	bl FillWindowPixelBuffer
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225D774: .word 0x00000265
_0225D778: .word 0x000002BF
	thumb_func_end UnionChatLog_AddWindows
