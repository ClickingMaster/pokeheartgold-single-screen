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

	thumb_func_start UnionChatLog_Task
UnionChatLog_Task: ; 0x0225D5A0
	push {r3, r4, r5, lr}
	add r4, r1, #0
	ldr r0, [r4, #0xc]
	ldr r5, [r4, #0x14]
	add r1, r0, #0
	add r1, #0x84
	ldr r1, [r1]
	cmp r1, #0
	beq _0225D5F4
	ldr r1, [r4]
	cmp r1, #5
	bhi _0225D5F4
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225D5C4: ; jump table
	.short _0225D5F4 - _0225D5C4 - 2 ; case 0
	.short _0225D5F4 - _0225D5C4 - 2 ; case 1
	.short _0225D5D0 - _0225D5C4 - 2 ; case 2
	.short _0225D5E8 - _0225D5C4 - 2 ; case 3
	.short _0225D5F4 - _0225D5C4 - 2 ; case 4
	.short _0225D5F4 - _0225D5C4 - 2 ; case 5
_0225D5D0:
	bl FieldSystem_TaskIsRunning
	cmp r0, #0
	bne _0225D5E0
	add r0, r4, #0
	add r1, r5, #0
	bl UnionChatLog_Update
_0225D5E0:
	ldr r0, [r4, #0x28]
	bl SpriteList_RenderAndAnimateSprites
	pop {r3, r4, r5, pc}
_0225D5E8:
	mov r1, #0
	add r2, r1, #0
	bl ov01_021F6A9C
	mov r0, #4
	str r0, [r4]
_0225D5F4:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end UnionChatLog_Task
