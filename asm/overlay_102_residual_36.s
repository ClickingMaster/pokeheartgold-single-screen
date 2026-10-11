	.include "asm/macros.inc"
	.public EasyChat_Exit
	.public EasyChat_Init
	.public EasyChat_Main
	.public _021EC5D8
	.public EasyChatApp_OnTouchButton
	.public EasyChatApp_Create
	.public EasyChatMessageCursor_Init
	.public EasyChatMessageCursor_Next
	.public EasyChatMessageCursor_Prev
	.public EasyChatMessageCursor_SetIndex
	.public EasyChatMessageCursor_GetIndex
	.public EasyChatApp_Delete
	.public EasyChatApp_SetHandler
	.public EasyChatApp_UpdateMenuInputState
	.public EasyChatApp_Handler_Main
	.public EasyChatApp_WordMenuAction
	.public EasyChatApp_WordMenuHandleKeys
	.public EasyChatApp_GetOneWordMenuTouch
	.public EasyChatApp_GetTwoWordMenuTouch
	.public EasyChatApp_WordMenuHandleTouch
	.public EasyChatApp_WordMenuRun
	.public EasyChatApp_Handler_WordMenu
	.public EasyChatApp_MessageMenuAction
	.public EasyChatApp_MessageMenuHandleKeys
	.public EasyChatApp_IsTouchOnSlider
	.public EasyChatApp_IsTouchInSliderRange
	.public EasyChatApp_GetMessageMenuTouch
	.public EasyChatApp_MessageMenuHandleTouch
	.public EasyChatApp_MessageMenuRun
	.public EasyChatApp_Handler_MessageMenu
	.public EasyChatApp_Handler_CategoryMenu
	.public EasyChatApp_CategoryMenuHandleKeys
	.public EasyChatApp_GetListButtonTouch
	.public EasyChatApp_GetCategoryGridTouch
	.public EasyChatApp_GetInitialGridTouch
	.public EasyChatApp_CategoryMenuHandleTouch
	.public EasyChatApp_CategoryMenuHandleInput
	.public EasyChatApp_CategoryHasWords
	.public EasyChatApp_MoveCategoryCursor
	.public EasyChatApp_MoveCategoryGridCursor
	.public EasyChatApp_MoveInitialGridCursor
	.public EasyChatWordPage_Init
	.public EasyChatWordPage_GetCursorPos
	.public EasyChatWordPage_GetWordIndex
	.public EasyChatWordPage_GetScroll
	.public EasyChatWordPage_GetTopRow
	.public EasyChatWordPage_GetMaxTopRow
	.public EasyChatApp_Handler_WordList
	.public EasyChatApp_WordListHandleKeys
	.public EasyChatApp_GetWordListTouch
	.public EasyChatApp_WordListHandleTouch
	.public EasyChatApp_WordListHandleInput
	.public EasyChatWordPage_HandleKeys
	.public EasyChatWordPage_PageUp
	.public EasyChatWordPage_PageDown
	.public EasyChatApp_SetSelectedWord
	.public EasyChatApp_Handler_Exit
	.public EasyChatApp_SetSubHandler
	.public EasyChatApp_EndSubHandler
	.public EasyChatApp_SubHandler_Start
	.public EasyChatApp_SubHandler_ConfirmFinish
	.public EasyChatApp_SubHandler_ConfirmQuit
	.public EasyChatApp_HasChanged
	.public EasyChatApp_IsComplete
	.public ov102_021E8F5C
	.public ov102_021E8F64
	.public EasyChatApp_GetMenuInputStatePtr
	.public EasyChatApp_GetType
	.public ov102_021E8F70
	.public EasyChatApp_GetListMode
	.public EasyChatApp_GetMsgBank
	.public EasyChatApp_GetWord
	.public EasyChatApp_GetMessageString
	.public EasyChatApp_GetMessageIndex
	.public EasyChatApp_GetWordSlot
	.public EasyChatApp_GetMenuCursor
	.public EasyChatApp_GetCategory
	.public EasyChatApp_GetCategoryWordCount
	.public EasyChatApp_GetCategoryWord
	.public EasyChatApp_GetWordListCursor
	.public EasyChatApp_GetWordListScroll
	.public EasyChatApp_CanScrollWordListUp
	.public EasyChatApp_CanScrollWordListDown
	.public ov102_021E9084
	.public ov102_021E9090
	.public EasyChatAppView_Create
	.public EasyChatAppView_Delete
	.public EasyChatAppView_InitCharTransfer
	.public EasyChatAppView_FreeCharTransfer
	.public EasyChatAppView_CreateButtonTexts
	.public EasyChatAppView_DeleteButtonTexts
	.public EasyChatAppView_CreateVBlankTask
	.public EasyChatAppView_MainTask
	.public EasyChatAppView_VBlankTask
	.public EasyChatAppView_StartCommand
	.public EasyChatAppView_AreCommandsDone
	.public EasyChatAppView_IsCommandDone
	.public EasyChatAppView_EndCommand
	.public EasyChatAppView_Cmd_Load
	.public EasyChatAppView_LoadSpriteGraphics
	.public EasyChatAppView_Cmd_Unload
	.public EasyChatAppView_InitBgs
	.public EasyChatAppView_Cmd_FadeIn
	.public EasyChatAppView_Cmd_RedrawSentence
	.public EasyChatAppView_Cmd_Nop
	.public EasyChatAppView_Cmd_ShowCategoryCursor
	.public EasyChatAppView_Cmd_ShowWordListCursor
	.public EasyChatAppView_Cmd_FocusMenu
	.public EasyChatAppView_Cmd_FocusSentence
	.public EasyChatAppView_Cmd_OpenCategories
	.public ov102_021E9A24
	.public EasyChatAppView_Cmd_SwitchListMode
	.public EasyChatAppView_Cmd_CloseCategories
	.public EasyChatAppView_Cmd_OpenWordList
	.public EasyChatAppView_Cmd_WordListToCategories
	.public EasyChatAppView_Cmd_CloseWordList
	.public EasyChatAppView_Cmd_WordListToMenu
	.public EasyChatAppView_Cmd_MoveMenuCursor
	.public EasyChatAppView_Cmd_SetMenuCursor
	.public EasyChatAppView_Cmd_SetCategoryCursor
	.public EasyChatAppView_Cmd_SetWordListCursor
	.public EasyChatAppView_Cmd_ScrollWordList
	.public EasyChatAppView_Cmd_HideMessage
	.public EasyChatAppView_Cmd_ConfirmFinish
	.public EasyChatAppView_Cmd_ConfirmQuit
	.public EasyChatAppView_Cmd_ShowIncompleteMessage
	.public ov102_021EA144
	.public EasyChatAppView_Cmd_CloseYesNo
	.public EasyChatAppView_Cmd_PressPageUp
	.public EasyChatAppView_Cmd_PressPageDown
	.public EasyChatAppView_Cmd_ReleasePageUp
	.public EasyChatAppView_Cmd_ReleasePageDown
	.public EasyChatAppView_GetSliderSprite
	.public EasyChatAppView_GetNumWordSlots
	.public EasyChatAppView_GetWordSlotHitbox
	.public EasyChatAppView_GetYesNoResult
	.public EasyChatAppView_GetBgConfig
	.public EasyChatAppView_GetSpriteList
	.public EasyChatAppView_InitSpriteHeader
	.public EasyChatAppView_CreateSprite
	.public ov102_021EA314
	.public ov102_021EA380
	.public ov102_021EA41C
	.public ov102_021EA644
	.public ov102_021EA678
	.public ov102_021EA71C
	.public ov102_021EA754
	.public ov102_021EA80C
	.public ov102_021EA874
	.public ov102_021EA8C0
	.public ov102_021EA920
	.public ov102_021EAA3C
	.public ov102_021EAB10
	.public ov102_021EAB30
	.public ov102_021EAC20
	.public ov102_021EAC44
	.public ov102_021EAC70
	.public ov102_021EAC7C
	.public ov102_021EAD48
	.public ov102_021EAD5C
	.public ov102_021EAD70
	.public ov102_021EAD98
	.public ov102_021EADF0
	.public ov102_021EADF8
	.public ov102_021EAE40
	.public ov102_021EAF44
	.public ov102_021EAF50
	.public ov102_021EAF5C
	.public ov102_021EAF7C
	.public ov102_021EAFAC
	.public ov102_021EAFEC
	.public ov102_021EAFF0
	.public ov102_021EB088
	.public ov102_021EB0C0
	.public ov102_021EB130
	.public ov102_021EB170
	.public ov102_021EB1A4
	.public ov102_021EB1E0
	.public ov102_021EB1F8
	.public ov102_021EB29C
	.public ov102_021EB2EC
	.public ov102_021EB3B4
	.public ov102_021EB46C
	.public ov102_021EB4D0
	.public ov102_021EB50C
	.public ov102_021EB518
	.public ov102_021EB524
	.public ov102_021EB530
	.public ov102_021EB570
	.public ov102_021EB5B8
	.public ov102_021EB624
	.public ov102_021EB654
	.public ov102_021EB664
	.public ov102_021EB67C
	.public ov102_021EB694
	.public ov102_021EB6C8
	.public ov102_021EB6E0
	.public ov102_021EB77C
	.public ov102_021EB880
	.public ov102_021EB94C
	.public ov102_021EB950
	.public ov102_021EB98C
	.public ov102_021EB9C0
	.public ov102_021EBA44
	.public ov102_021EBA5C
	.public ov102_021EBAAC
	.public ov102_021EBAD0
	.public ov102_021EBB1C
	.public ov102_021EBB2C
	.public ov102_021EBB44
	.public ov102_021EBB50
	.public ov102_021EBB6C
	.public ov102_021EBB88
	.public ov102_021EBBC4
	.public ov102_021EBBD0
	.public ov102_021EBBF4
	.public ov102_021EBC28
	.public ov102_021EBC34
	.public ov102_021EBCBC
	.public ov102_021EBD00
	.public ov102_021EBD68
	.public ov102_021EBDEC
	.public ov102_021EBE3C
	.public ov102_021EBE80
	.public ov102_021EBEA4
	.public ov102_021EBEC8
	.public ov102_021EBEF4
	.public ov102_021EBF38
	.public ov102_021EBFA0
	.public ov102_021EC05C
	.public ov102_021EC090
	.public ov102_021EC11C
	.public ov102_021EC13C
	.public ov102_021EC20C
	.public ov102_021EC240
	.public ov102_021EC248
	.public ov102_021EC290
	.public ov102_021EC294
	.public ov102_021EC298
	.public ov102_021EC2EC
	.public ov102_021EC338
	.public ov102_021EC37C
	.public ov102_021EC3AC
	.public ov102_021EC3D4
	.public ov102_021EC410
	.public ov102_021EC450
	.public ov102_021EC478
	.public ov102_021EC4A4
	.public ov102_021EC4A8
	.public ov102_021EC4CC
	.public ov102_021EC4F8
	.public ov102_021EC514
	.public ov102_021EC530
	.public ov102_021EC54C
	.public ov102_021EC568
	.public ov102_021EC584
	.public ov102_021EC5A0
	.public ov102_021EC5E0
	.public ov102_021EC5E8
	.public ov102_021EC5F4
	.public ov102_021EC600
	.public ov102_021EC610
	.public ov102_021EC620
	.public ov102_021EC634
	.public ov102_021EC648
	.public ov102_021EC65C
	.public ov102_021EC65D
	.public ov102_021EC65E
	.public ov102_021EC65F
	.public ov102_021EC698
	.public ov102_021EC6A8
	.public ov102_021EC6B8
	.public ov102_021EC6D4
	.public ov102_021EC6F0
	.public ov102_021EC70C
	.public ov102_021EC728
	.public ov102_021EC744
	.public ov102_021EC760
	.public ov102_021EC788
	.public ov102_021EC808
	.public ov102_021EC820
	.include "overlay_102.inc"
	.include "global.inc"

	.text
	.public EasyChat_Exit
	.public EasyChatMessageCursor_GetIndex
	.public EasyChatApp_Delete
	.public EasyChatApp_SetHandler
	.public EasyChatApp_GetOneWordMenuTouch
	.public EasyChatApp_GetTwoWordMenuTouch
	.public EasyChatApp_CategoryMenuHandleInput
	.public EasyChatApp_MoveCategoryCursor
	.public EasyChatWordPage_GetCursorPos
	.public EasyChatWordPage_GetWordIndex
	.public EasyChatWordPage_GetScroll
	.public EasyChatWordPage_GetTopRow
	.public EasyChatWordPage_GetMaxTopRow
	.public EasyChatApp_WordListHandleInput
	.public EasyChatApp_Handler_Exit
	.public EasyChatApp_SetSubHandler
	.public EasyChatApp_EndSubHandler
	.public EasyChatApp_SubHandler_Start
	.public EasyChatApp_HasChanged
	.public ov102_021E8F5C
	.public ov102_021E8F64
	.public EasyChatApp_GetMenuInputStatePtr
	.public EasyChatApp_GetType
	.public ov102_021E8F70
	.public EasyChatApp_GetListMode
	.public EasyChatApp_GetMsgBank
	.public EasyChatApp_GetWord
	.public EasyChatApp_GetMessageString
	.public EasyChatApp_GetMessageIndex
	.public EasyChatApp_GetWordSlot
	.public EasyChatApp_GetMenuCursor
	.public EasyChatApp_GetCategory
	.public EasyChatApp_GetCategoryWordCount
	.public EasyChatApp_GetCategoryWord
	.public EasyChatApp_GetWordListCursor
	.public EasyChatApp_GetWordListScroll
	.public EasyChatApp_CanScrollWordListUp
	.public EasyChatApp_CanScrollWordListDown
	.public ov102_021E9084
	.public ov102_021E9090
	.public EasyChatAppView_FreeCharTransfer
	.public EasyChatAppView_CreateVBlankTask
	.public EasyChatAppView_MainTask
	.public EasyChatAppView_AreCommandsDone
	.public EasyChatAppView_IsCommandDone
	.public EasyChatAppView_Cmd_RedrawSentence
	.public EasyChatAppView_Cmd_Nop
	.public EasyChatAppView_Cmd_ShowCategoryCursor
	.public EasyChatAppView_Cmd_ShowWordListCursor
	.public EasyChatAppView_Cmd_FocusSentence
	.public EasyChatAppView_Cmd_SetMenuCursor
	.public EasyChatAppView_Cmd_SetCategoryCursor
	.public EasyChatAppView_Cmd_SetWordListCursor
	.public EasyChatAppView_Cmd_HideMessage
	.public EasyChatAppView_Cmd_ShowIncompleteMessage
	.public ov102_021EA144
	.public EasyChatAppView_Cmd_PressPageUp
	.public EasyChatAppView_Cmd_PressPageDown
	.public EasyChatAppView_Cmd_ReleasePageUp
	.public EasyChatAppView_Cmd_ReleasePageDown
	.public EasyChatAppView_GetSliderSprite
	.public EasyChatAppView_GetNumWordSlots
	.public EasyChatAppView_GetWordSlotHitbox
	.public EasyChatAppView_GetYesNoResult
	.public EasyChatAppView_GetBgConfig
	.public EasyChatAppView_GetSpriteList
	.public ov102_021EAB10
	.public ov102_021EAC20
	.public ov102_021EAC70
	.public ov102_021EAD48
	.public ov102_021EAD5C
	.public ov102_021EAD70
	.public ov102_021EADF0
	.public ov102_021EAF44
	.public ov102_021EAF50
	.public ov102_021EAF5C
	.public ov102_021EAF7C
	.public ov102_021EAFEC
	.public ov102_021EB1A4
	.public ov102_021EB1E0
	.public ov102_021EB50C
	.public ov102_021EB518
	.public ov102_021EB524
	.public ov102_021EB624
	.public ov102_021EB654
	.public ov102_021EB664
	.public ov102_021EB67C
	.public ov102_021EB694
	.public ov102_021EB6C8
	.public ov102_021EB94C
	.public ov102_021EBA44
	.public ov102_021EBAAC
	.public ov102_021EBB1C
	.public ov102_021EBB2C
	.public ov102_021EBB44
	.public ov102_021EBB50
	.public ov102_021EBB6C
	.public ov102_021EBBC4
	.public ov102_021EBBD0
	.public ov102_021EBC28
	.public ov102_021EBE80
	.public ov102_021EBEA4
	.public ov102_021EC11C
	.public ov102_021EC20C
	.public ov102_021EC240
	.public ov102_021EC290
	.public ov102_021EC294
	.public ov102_021EC4A4
	.public ov102_021EC4A8
	.public ov102_021EC4CC
	.public ov102_021EC4F8
	.public ov102_021EC514
	.public ov102_021EC530
	.public ov102_021EC54C
	.public ov102_021EC568
	.public ov102_021EC584

	thumb_func_start ov102_021EBC34
ov102_021EBC34: ; 0x021EBC34
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	add r6, r1, #0
	mov r0, #0x23
	mov r1, #0x98
	add r7, r2, #0
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	str r6, [r4, #4]
	str r7, [r4, #8]
	add r0, r5, #0
	bl EasyChatAppView_GetBgConfig
	str r0, [r4, #0xc]
	mov r0, #0x20
	mov r1, #0x23
	bl String_New
	str r0, [r4, #0x3c]
	mov r0, #0
	str r0, [sp]
	mov r0, #0x1a
	str r0, [sp, #4]
	mov r0, #0x20
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	add r1, r4, #0
	ldr r0, [r4, #0xc]
	add r1, #0x10
	mov r2, #2
	mov r3, #4
	bl AddWindowParameterized
	mov r3, #0
	str r3, [sp]
	mov r1, #0xc
	str r1, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	add r1, r4, #0
	ldr r0, [r4, #0xc]
	add r1, #0x20
	mov r2, #2
	bl AddWindowParameterized
	mov r0, #0
	str r0, [r4, #0x30]
	str r0, [r4, #0x34]
	str r0, [r4, #0x38]
	ldr r0, [r4, #4]
	bl EasyChatApp_GetMenuInputStatePtr
	add r1, r4, #0
	add r1, #0x94
	str r0, [r1]
	add r0, r4, #0
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov102_021EBC34


	thumb_func_start ov102_021EBCBC
ov102_021EBCBC: ; 0x021EBCBC
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x30]
	cmp r0, #0
	beq _021EBCCA
	bl Sprite_Delete
_021EBCCA:
	ldr r0, [r4, #0x34]
	cmp r0, #0
	beq _021EBCD4
	bl Sprite_Delete
_021EBCD4:
	ldr r0, [r4, #0x38]
	cmp r0, #0
	beq _021EBCDE
	bl Sprite_Delete
_021EBCDE:
	ldr r0, [r4, #0x3c]
	cmp r0, #0
	beq _021EBCE8
	bl String_Delete
_021EBCE8:
	add r0, r4, #0
	add r0, #0x20
	bl RemoveWindow
	add r0, r4, #0
	add r0, #0x10
	bl RemoveWindow
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	thumb_func_end ov102_021EBCBC


	thumb_func_start ov102_021EBD00
ov102_021EBD00: ; 0x021EBD00
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, [r5]
	bl EasyChatAppView_GetBgConfig
	add r4, r0, #0
	mov r0, #2
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x23
	bl BG_ClearCharDataRange
	mov r2, #0
	str r2, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	add r0, r4, #0
	mov r1, #2
	add r3, r2, #0
	bl FillBgTilemapRect
	add r0, r5, #0
	add r0, #0x10
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x10
	bl PutWindowTilemap
	add r0, r5, #0
	add r0, #0x10
	bl CopyWindowPixelsToVram_TextMode
	add r0, r4, #0
	mov r1, #2
	bl BgCommitTilemapBufferToVram
	add r0, r5, #0
	bl ov102_021EBD68
	mov r0, #2
	mov r1, #0
	bl ToggleBgLayer
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov102_021EBD00


	thumb_func_start ov102_021EBD68
ov102_021EBD68: ; 0x021EBD68
	push {r3, r4, lr}
	sub sp, #0x2c
	add r4, r0, #0
	mov r2, #0
	ldr r0, [r4]
	add r1, sp, #8
	add r3, r2, #0
	bl EasyChatAppView_InitSpriteHeader
	mov r0, #3
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #8
	mov r2, #0x48
	mov r3, #0x38
	bl EasyChatAppView_CreateSprite
	str r0, [r4, #0x30]
	mov r1, #8
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r4, #0x30]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #4
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #8
	mov r2, #0xf0
	mov r3, #0x50
	bl EasyChatAppView_CreateSprite
	str r0, [r4, #0x34]
	mov r1, #0x18
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r4, #0x34]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #4
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [r4]
	add r1, sp, #8
	mov r2, #0xf0
	mov r3, #0x80
	bl EasyChatAppView_CreateSprite
	str r0, [r4, #0x38]
	mov r1, #0x1a
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [r4, #0x38]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add sp, #0x2c
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end ov102_021EBD68


	thumb_func_start ov102_021EBDEC
ov102_021EBDEC: ; 0x021EBDEC
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r0, #0x10
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #0
	bl ov102_021EC11C
	ldr r0, [r6, #4]
	bl EasyChatApp_GetCategoryWordCount
	add r7, r0, #0
	cmp r7, #0xa
	bls _021EBE0C
	mov r7, #0xa
_021EBE0C:
	add r0, r6, #0
	add r0, #0x8c
	mov r5, #0
	ldr r4, [r0]
	cmp r7, #0
	bls _021EBE30
_021EBE18:
	add r0, r6, #0
	add r1, r5, #0
	add r2, r4, #0
	bl ov102_021EC13C
	mov r0, #1
	tst r0, r5
	beq _021EBE2A
	add r4, #0x18
_021EBE2A:
	add r5, r5, #1
	cmp r5, r7
	blo _021EBE18
_021EBE30:
	add r6, #0x10
	add r0, r6, #0
	bl CopyWindowPixelsToVram_TextMode
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov102_021EBDEC


	thumb_func_start ov102_021EBE3C
ov102_021EBE3C: ; 0x021EBE3C
	push {r4, lr}
	sub sp, #8
	add r4, r0, #0
	mov r0, #0x10
	str r0, [sp]
	ldr r0, _021EBE7C ; =0x04000050
	mov r1, #4
	mov r2, #0x3f
	mov r3, #0
	bl G2x_SetBlendAlpha_
	mov r0, #2
	mov r1, #1
	bl ToggleBgLayer
	add r0, r4, #0
	mov r3, #0
	add r0, #0x80
	str r3, [r0]
	mov r0, #0x7c
	str r0, [sp]
	mov r0, #0xc
	add r4, #0x40
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #4
	mov r2, #0x3f
	bl ov102_021EC37C
	add sp, #8
	pop {r4, pc}
	nop
_021EBE7C: .word 0x04000050
	thumb_func_end ov102_021EBE3C
