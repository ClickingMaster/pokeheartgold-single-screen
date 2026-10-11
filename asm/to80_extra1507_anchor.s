	.extern EasyChatApp_Handler_WordMenu
	.extern EasyChatApp_Handler_MessageMenu
	.extern EasyChatApp_MoveCategoryGridCursor
	.extern EasyChatApp_MoveInitialGridCursor
	.extern EasyChatAppView_Cmd_Load
	.extern EasyChatAppView_Cmd_Unload
	.extern EasyChatAppView_Cmd_FadeIn
	.extern EasyChatAppView_Cmd_RedrawSentence
	.extern EasyChatAppView_Cmd_Nop
	.extern EasyChatAppView_Cmd_ShowCategoryCursor
	.extern EasyChatAppView_Cmd_ShowWordListCursor
	.extern EasyChatAppView_Cmd_FocusMenu
	.extern EasyChatAppView_Cmd_FocusSentence
	.extern EasyChatAppView_Cmd_OpenCategories
	.extern ov102_021E9A24
	.extern EasyChatAppView_Cmd_SwitchListMode
	.extern EasyChatAppView_Cmd_CloseCategories
	.extern EasyChatAppView_Cmd_OpenWordList
	.extern EasyChatAppView_Cmd_WordListToCategories
	.extern EasyChatAppView_Cmd_CloseWordList
	.extern EasyChatAppView_Cmd_WordListToMenu
	.extern EasyChatAppView_Cmd_MoveMenuCursor
	.extern EasyChatAppView_Cmd_SetMenuCursor
	.extern EasyChatAppView_Cmd_SetCategoryCursor
	.extern EasyChatAppView_Cmd_SetWordListCursor
	.extern EasyChatAppView_Cmd_ScrollWordList
	.extern EasyChatAppView_Cmd_HideMessage
	.extern EasyChatAppView_Cmd_ConfirmFinish
	.extern EasyChatAppView_Cmd_ConfirmQuit
	.extern EasyChatAppView_Cmd_ShowIncompleteMessage
	.extern ov102_021EA144
	.extern EasyChatAppView_Cmd_CloseYesNo
	.extern EasyChatAppView_Cmd_PressPageUp
	.extern EasyChatAppView_Cmd_PressPageDown
	.extern EasyChatAppView_Cmd_ReleasePageUp
	.extern EasyChatAppView_Cmd_ReleasePageDown
	.public ov102_021EC820
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
	.include "asm/macros.inc"
	.include "overlay_102.inc"
	.include "global.inc"

.text
	.rodata

_021EC5D8:
	.word EasyChatApp_MoveCategoryGridCursor
	.word EasyChatApp_MoveInitialGridCursor

ov102_021EC5E0: ; 0x021EC5E0
	.byte 0x40, 0x80, 0x14, 0xD7, 0xFF, 0x00, 0x00, 0x00

ov102_021EC5E8: ; 0x021EC5E8
	.word EasyChatApp_Handler_WordMenu
	.word EasyChatApp_Handler_WordMenu
	.word EasyChatApp_Handler_MessageMenu

ov102_021EC5F4: ; 0x021EC5F4
	.byte 0x30, 0x68, 0xE4, 0xFC, 0x68, 0xA0, 0xE4, 0xFC, 0xFF, 0x00, 0x00, 0x00

ov102_021EC600: ; 0x021EC600
	.byte 0xA4, 0xBA, 0xC4, 0xFC, 0x00, 0x2A, 0x10, 0x68, 0x00, 0x2A, 0x98, 0xF0, 0xFF, 0x00, 0x00, 0x00

ov102_021EC610: ; 0x021EC610
	.byte 0x70, 0x80, 0xC0, 0x00, 0x88, 0x98, 0xC0, 0x00, 0x10, 0x20, 0x50, 0xB0, 0xFF, 0x00, 0x00, 0x00

ov102_021EC620: ; 0x021EC620
	.byte 0x70, 0x80, 0xC0, 0x00, 0x88, 0x98, 0xC0, 0x00, 0x10, 0x20, 0x18, 0x78, 0x10, 0x20, 0x88, 0xE8
	.byte 0xFF, 0x00, 0x00, 0x00

ov102_021EC634: ; 0x021EC634
	.byte 0x70, 0x80, 0xC0, 0x00, 0x88, 0x98, 0xC0, 0x00, 0x34, 0x4C, 0x08, 0x28
	.byte 0x34, 0x4C, 0xD8, 0xF8, 0xFF, 0x00, 0x00, 0x00

ov102_021EC648: ; 0x021EC648
	.byte 0x18, 0x50, 0x18, 0x70, 0x70, 0xA8, 0x18, 0x70
	.byte 0x28, 0x48, 0xB0, 0xD0, 0x78, 0x98, 0xB0, 0xD0, 0xFF, 0x00, 0x00, 0x00

ov102_021EC65C: ; 0x021EC65C
	.byte 0xFE

ov102_021EC65D: ; 0x021EC65D
	.byte 0x03

ov102_021EC65E: ; 0x021EC65E
	.byte 0x02

ov102_021EC65F: ; 0x021EC65F
	.byte 0x01
	.byte 0xFE, 0x04, 0x00, 0x02, 0xFE, 0x05, 0x01, 0x00, 0x00, 0x06, 0x05, 0x04, 0x01, 0x07, 0x03, 0x05
	.byte 0x02, 0x08, 0x04, 0x03, 0x03, 0x09, 0x08, 0x07, 0x04, 0x0A, 0x06, 0x08, 0x05, 0x0B, 0x07, 0x06
	.byte 0x06, 0xFE, 0x0B, 0x0A, 0x07, 0xFE, 0x09, 0x0B, 0x08, 0xFE, 0x0A, 0x09, 0x09, 0x00, 0xFE, 0xFE
	.byte 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00

ov102_021EC698: ; 0x021EC698
	.byte 0x02, 0x00, 0x00, 0x00, 0x80, 0x0C, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00

ov102_021EC6A8: ; 0x021EC6A8
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC6B8: ; 0x021EC6B8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x06, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov102_021EC6D4: ; 0x021EC6D4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x1C, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC6F0: ; 0x021EC6F0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1D, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC70C: ; 0x021EC70C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x02
	.byte 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC728: ; 0x021EC728
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov102_021EC744: ; 0x021EC744
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC760: ; 0x021EC760
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov102_021EC788: ; 0x021EC788
	.word EasyChatAppView_Cmd_Load
	.word EasyChatAppView_Cmd_Unload
	.word EasyChatAppView_Cmd_FadeIn
	.word EasyChatAppView_Cmd_RedrawSentence
	.word EasyChatAppView_Cmd_Nop
	.word EasyChatAppView_Cmd_ShowCategoryCursor
	.word EasyChatAppView_Cmd_ShowWordListCursor
	.word EasyChatAppView_Cmd_FocusMenu
	.word EasyChatAppView_Cmd_FocusSentence
	.word EasyChatAppView_Cmd_OpenCategories
	.word EasyChatAppView_Cmd_CloseCategories
	.word EasyChatAppView_Cmd_OpenWordList
	.word EasyChatAppView_Cmd_WordListToCategories
	.word EasyChatAppView_Cmd_CloseWordList
	.word EasyChatAppView_Cmd_WordListToMenu
	.word EasyChatAppView_Cmd_MoveMenuCursor
	.word EasyChatAppView_Cmd_SetMenuCursor
	.word EasyChatAppView_Cmd_SetCategoryCursor
	.word EasyChatAppView_Cmd_SetWordListCursor
	.word EasyChatAppView_Cmd_ScrollWordList
	.word EasyChatAppView_Cmd_HideMessage
	.word EasyChatAppView_Cmd_ConfirmFinish
	.word EasyChatAppView_Cmd_ConfirmQuit
	.word EasyChatAppView_Cmd_ShowIncompleteMessage
	.word ov102_021EA144
	.word EasyChatAppView_Cmd_CloseYesNo
	.word ov102_021E9A24
	.word EasyChatAppView_Cmd_SwitchListMode
	.word EasyChatAppView_Cmd_PressPageUp
	.word EasyChatAppView_Cmd_PressPageDown
	.word EasyChatAppView_Cmd_ReleasePageUp
	.word EasyChatAppView_Cmd_ReleasePageDown

ov102_021EC808: ; 0x021EC808
	.byte 0x00, 0x18, 0x0E, 0x07, 0x02, 0x0D, 0xF1, 0x03
	.byte 0x00, 0x18, 0x11, 0x07, 0x02, 0x0D, 0xE3, 0x03
	.byte 0x00, 0x19, 0x15, 0x06, 0x02, 0x0D, 0xD5, 0x03

ov102_021EC820: ; 0x021EC820
	.byte 0x2B, 0x01, 0x18, 0x08, 0xFE, 0x0A, 0x09, 0x01, 0x00, 0x14
	.byte 0x2C, 0x01, 0x28, 0x08, 0xFE, 0x0B, 0x00, 0x02, 0x01, 0x15
	.byte 0x2D, 0x01, 0x38, 0x08, 0xFE, 0x0C, 0x01, 0x03, 0x02, 0x16
	.byte 0x2E, 0x01, 0x48, 0x08, 0xFE, 0x0D, 0x02, 0x04, 0x03, 0x17
	.byte 0x2F, 0x01, 0x58, 0x08, 0xFE, 0x0E, 0x03, 0x05, 0x04, 0x18
	.byte 0x30, 0x01, 0x68, 0x08, 0xFE, 0x0F, 0x04, 0x06, 0x05, 0x19
	.byte 0x31, 0x01, 0x78, 0x08, 0xFE, 0x10, 0x05, 0x07, 0x06, 0x10
	.byte 0x32, 0x01, 0x88, 0x08, 0xFE, 0x11, 0x06, 0x08, 0x07, 0x11
	.byte 0x33, 0x01, 0x98, 0x08, 0xFE, 0x12, 0x07, 0x09, 0x08, 0x12
	.byte 0x34, 0x01, 0xA8, 0x08, 0xFE, 0x13, 0x08, 0x00, 0x09, 0x1A
	.byte 0x35, 0x01, 0x18, 0x18, 0x00, 0x14, 0x13, 0x0B, 0x00, 0x14
	.byte 0x36, 0x01, 0x28, 0x18, 0x01, 0x15, 0x0A, 0x0C, 0x01, 0x15
	.byte 0x37, 0x01, 0x38, 0x18, 0x02, 0x16, 0x0B, 0x0D, 0x02, 0x16
	.byte 0x38, 0x01, 0x48, 0x18, 0x03, 0x17, 0x0C, 0x0E, 0x03, 0x17
	.byte 0x39, 0x01, 0x58, 0x18, 0x04, 0x18, 0x0D, 0x0F, 0x04, 0x18
	.byte 0x3A, 0x01, 0x68, 0x18, 0x05, 0x19, 0x0E, 0x10, 0x05, 0x19
	.byte 0x3B, 0x01, 0x78, 0x18, 0x06, 0xFE, 0x0F, 0x11, 0x06, 0x10
	.byte 0x3C, 0x01, 0x88, 0x18, 0x07, 0xFE, 0x10, 0x12, 0x07, 0x11
	.byte 0x3D, 0x01, 0x98, 0x18, 0x08, 0xFE, 0x11, 0x13, 0x08, 0x12
	.byte 0x3E, 0x01, 0xA8, 0x18, 0x09, 0x1A, 0x12, 0x0A, 0x09, 0x1A
	.byte 0x3F, 0x01, 0x18, 0x28, 0x0A, 0xFE, 0x1A, 0x15, 0x00, 0x14
	.byte 0x40, 0x01, 0x28, 0x28, 0x0B, 0xFE, 0x14, 0x16, 0x01, 0x15
	.byte 0x41, 0x01, 0x38, 0x28, 0x0C, 0xFE, 0x15, 0x17, 0x02, 0x16
	.byte 0x42, 0x01, 0x48, 0x28, 0x0D, 0xFE, 0x16, 0x18, 0x03, 0x17
	.byte 0x43, 0x01, 0x58, 0x28, 0x0E, 0xFE, 0x17, 0x19, 0x04, 0x18
	.byte 0x44, 0x01, 0x68, 0x28, 0x0F, 0xFE, 0x18, 0x1A, 0x05, 0x19
	.byte 0xAB, 0x01, 0xA8, 0x28, 0x13, 0xFE, 0x19, 0x14, 0x09, 0x1A
	; 0x021EC930
