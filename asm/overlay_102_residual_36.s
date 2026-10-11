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
	.public EasyChatSentenceView_Create
	.public EasyChatSentenceView_Delete
	.public EasyChatSentenceView_Load
	.public EasyChatSentenceView_HBlankCallback
	.public EasyChatSentenceView_ToggleScrollWindows
	.public EasyChatSentenceView_StartScroll
	.public EasyChatSentenceView_UpdateScroll
	.public EasyChatSentenceView_LoadPalettes
	.public EasyChatSentenceView_ApplyPalette
	.public EasyChatSentenceView_InitSlotPositions
	.public EasyChatSentenceView_CreateSprites
	.public EasyChatSentenceView_DrawSentence
	.public EasyChatSentenceView_GetSlotHitbox
	.public EasyChatSentenceView_DrawMessage
	.public EasyChatSentenceView_UpdateSliderThumb
	.public EasyChatMessageParser_Init
	.public EasyChatMessageParser_Free
	.public EasyChatMessageParser_Next
	.public EasyChatSentenceView_GetSlotTextPos
	.public EasyChatSentenceView_GetSlotCursorPos
	.public EasyChatSentenceView_ClearSlot
	.public EasyChatSentenceView_DrawWord
	.public EasyChatSentenceView_GetNumSlots
	.public EasyChatSentenceView_SelectMessageWindow
	.public EasyChatSentenceView_PrintMessage
	.public EasyChatSentenceView_DeactivateCursor
	.public EasyChatSentenceView_ActivateCursor
	.public EasyChatSentenceView_SetCursorVisible
	.public EasyChatSentenceView_HideSlider
	.public EasyChatSentenceView_ShowSlider
	.public EasyChatSentenceView_GetSliderThumb
	.public EasyChatSentenceView_MoveCursorToSlot
	.public EasyChatSentenceView_SetCursorActive
	.public EasyChatSentenceView_StartYesNo
	.public EasyChatSentenceView_HandleYesNo
	.public EasyChatSentenceView_DrawYesNoCursor
	.public EasyChatMenuView_Create
	.public EasyChatMenuView_Delete
	.public EasyChatMenuView_Load
	.public EasyChatMenuView_Unload
	.public EasyChatMenuView_DrawMainButtons
	.public EasyChatMenuView_DrawListButton
	.public EasyChatMenuView_AddWindows
	.public EasyChatMenuView_CreateCursor
	.public EasyChatMenuView_DeactivateCursor
	.public EasyChatMenuView_ActivateCursor
	.public EasyChatMenuView_SetCursorVisible
	.public EasyChatMenuView_MoveCursor
	.public EasyChatMenuView_DrawModeButton
	.public EasyChatMenuView_ModeSwitchTask
	.public EasyChatMenuView_StartModeSwitch
	.public EasyChatMenuView_IsModeSwitchDone
	.public EasyChatMenuView_ShowListButton
	.public EasyChatMenuView_ShowMainButtons
	.public EasyChatCategoryView_Create
	.public EasyChatCategoryView_Delete
	.public EasyChatCategoryView_Load
	.public EasyChatCategoryView_DrawCategories
	.public EasyChatCategoryView_DrawInitials
	.public ov102_021EB94C
	.public EasyChatCategoryView_CreateCursor
	.public EasyChatCategoryView_SetCursorVisible
	.public EasyChatCategoryView_MoveCursor
	.public EasyChatCategoryView_StartOpen
	.public EasyChatCategoryView_UpdateOpen
	.public EasyChatCategoryView_StartClose
	.public EasyChatCategoryView_UpdateClose
	.public EasyChatCategoryView_ResetScrollY
	.public EasyChatCategoryView_StartDim
	.public EasyChatCategoryView_UpdateDim
	.public EasyChatCategoryView_StartFadeOut
	.public EasyChatCategoryView_UpdateFadeOut
	.public EasyChatCategoryView_StartFadeIn
	.public EasyChatCategoryView_UpdateFadeIn
	.public EasyChatCategoryView_ResetScrollX
	.public EasyChatCategoryView_StartModeScroll
	.public EasyChatCategoryView_UpdateModeScroll
	.public EasyChatWordListView_Create
	.public EasyChatWordListView_Delete
	.public EasyChatWordListView_Load
	.public EasyChatWordListView_CreateSprites
	.public EasyChatWordListView_DrawWords
	.public EasyChatWordListView_StartFadeIn
	.public EasyChatWordListView_UpdateFadeIn
	.public EasyChatWordListView_StartFadeOut
	.public EasyChatWordListView_UpdateFadeOut
	.public EasyChatWordListView_SetCursorVisible
	.public EasyChatWordListView_MoveCursor
	.public EasyChatWordListView_StartScroll
	.public EasyChatWordListView_UpdateScroll
	.public EasyChatWordListView_ClearRows
	.public EasyChatWordListView_ResetScroll
	.public EasyChatWordListView_DrawWord
	.public EasyChatPageButtonsView_Create
	.public EasyChatPageButtonsView_Delete
	.public EasyChatPageButtonsView_Load
	.public EasyChatPageButtonsView_SetEnabled
	.public EasyChatPageButtonsView_SetPressed
	.public EasyChatBgScroll_Start
	.public EasyChatBgScroll_Update
	.public EasyChatBgScroll_Task
	.public EasyChatBlendFade_StartAlpha
	.public EasyChatBlendFade_IsAlphaDone
	.public EasyChatBlendFade_AlphaTask
	.public EasyChatBlendFade_StartBrightness
	.public EasyChatBlendFade_IsBrightnessDone
	.public EasyChatBlendFade_BrightnessTask
	.public EasyChatInitialGrid_GetCount
	.public EasyChatInitialGrid_GetLetter
	.public EasyChatInitialGrid_GetPosition
	.public EasyChatInitialGrid_GetUp
	.public EasyChatInitialGrid_GetDown
	.public EasyChatInitialGrid_GetLeft
	.public EasyChatInitialGrid_GetRight
	.public EasyChatInitialGrid_GetBelowButton
	.public EasyChatInitialGrid_GetAboveButton
	.public EasyChatInitialGrid_FindCellAt
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
	.public EasyChatSentenceView_GetSlotHitbox
	.public EasyChatSentenceView_UpdateSliderThumb
	.public EasyChatMessageParser_Free
	.public EasyChatSentenceView_GetSlotTextPos
	.public EasyChatSentenceView_GetSlotCursorPos
	.public EasyChatSentenceView_ClearSlot
	.public EasyChatSentenceView_GetNumSlots
	.public EasyChatSentenceView_DeactivateCursor
	.public EasyChatSentenceView_ActivateCursor
	.public EasyChatSentenceView_SetCursorVisible
	.public EasyChatSentenceView_HideSlider
	.public EasyChatSentenceView_GetSliderThumb
	.public EasyChatMenuView_Create
	.public EasyChatMenuView_Delete
	.public EasyChatMenuView_DeactivateCursor
	.public EasyChatMenuView_ActivateCursor
	.public EasyChatMenuView_SetCursorVisible
	.public EasyChatMenuView_StartModeSwitch
	.public EasyChatMenuView_IsModeSwitchDone
	.public EasyChatMenuView_ShowListButton
	.public EasyChatMenuView_ShowMainButtons
	.public EasyChatCategoryView_Create
	.public EasyChatCategoryView_Delete
	.public ov102_021EB94C
	.public EasyChatCategoryView_StartOpen
	.public EasyChatCategoryView_StartClose
	.public EasyChatCategoryView_ResetScrollY
	.public EasyChatCategoryView_StartDim
	.public EasyChatCategoryView_UpdateDim
	.public EasyChatCategoryView_StartFadeOut
	.public EasyChatCategoryView_UpdateFadeOut
	.public EasyChatCategoryView_UpdateFadeIn
	.public EasyChatCategoryView_ResetScrollX
	.public EasyChatCategoryView_UpdateModeScroll
	.public EasyChatWordListView_UpdateFadeIn
	.public EasyChatWordListView_StartFadeOut
	.public EasyChatWordListView_ResetScroll
	.public EasyChatPageButtonsView_Create
	.public EasyChatPageButtonsView_Delete
	.public EasyChatPageButtonsView_SetEnabled
	.public EasyChatPageButtonsView_SetPressed
	.public EasyChatInitialGrid_GetCount
	.public EasyChatInitialGrid_GetLetter
	.public EasyChatInitialGrid_GetPosition
	.public EasyChatInitialGrid_GetUp
	.public EasyChatInitialGrid_GetDown
	.public EasyChatInitialGrid_GetLeft
	.public EasyChatInitialGrid_GetRight
	.public EasyChatInitialGrid_GetBelowButton
	.public EasyChatInitialGrid_GetAboveButton

	thumb_func_start EasyChatWordListView_Create
EasyChatWordListView_Create: ; 0x021EBC34
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
	thumb_func_end EasyChatWordListView_Create


	thumb_func_start EasyChatWordListView_Delete
EasyChatWordListView_Delete: ; 0x021EBCBC
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
	thumb_func_end EasyChatWordListView_Delete


	thumb_func_start EasyChatWordListView_Load
EasyChatWordListView_Load: ; 0x021EBD00
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
	bl EasyChatWordListView_CreateSprites
	mov r0, #2
	mov r1, #0
	bl ToggleBgLayer
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end EasyChatWordListView_Load


	thumb_func_start EasyChatWordListView_CreateSprites
EasyChatWordListView_CreateSprites: ; 0x021EBD68
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
	thumb_func_end EasyChatWordListView_CreateSprites


	thumb_func_start EasyChatWordListView_DrawWords
EasyChatWordListView_DrawWords: ; 0x021EBDEC
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r0, #0x10
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #0
	bl EasyChatWordListView_ResetScroll
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
	bl EasyChatWordListView_DrawWord
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
	thumb_func_end EasyChatWordListView_DrawWords


	thumb_func_start EasyChatWordListView_StartFadeIn
EasyChatWordListView_StartFadeIn: ; 0x021EBE3C
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
	bl EasyChatBlendFade_StartAlpha
	add sp, #8
	pop {r4, pc}
	nop
_021EBE7C: .word 0x04000050
	thumb_func_end EasyChatWordListView_StartFadeIn
