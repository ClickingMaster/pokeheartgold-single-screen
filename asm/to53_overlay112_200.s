#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.text
	.extern PokewalkerIr_Checksum
	.extern ov112_021E5938
	.extern PokewalkerIr_ResetLink
	.extern PokewalkerIr_InitLink
	.extern PokewalkerIr_IsConnected
	.extern PokewalkerIr_SendPacket
	.extern PokewalkerIr_CloseLink
	.extern PokewalkerIr_Disconnect
	.extern PokewalkerIr_Send
	.extern ov112_021E5A80
	.extern ov112_021E5A84
	.extern PokewalkerIr_CheckTimeout
	.extern PokewalkerIr_VerifyPacket
	.extern PokewalkerIr_Poll
	.extern PokewalkerIr_SetConnectCallback
	.extern PokewalkerIr_SetReceiveCallback
	.extern PokewalkerIr_SetCloseCallback
	.extern ov112_021E5D5C
	.extern PokewalkerIr_SetCompressBuffer
	.extern PokewalkerIr_CompressBlocks
	.extern PokewalkerIr_SetError
	.extern PokewalkerIr_StoreClock
	.extern PokewalkerIr_StoreCurrentTime
	.extern PokewalkerIr_SetIdentities
	.extern PokewalkerIr_BeginTransfer
	.extern PokewalkerIr_SendChunk
	.extern PokewalkerIr_OnSendComplete
	.extern PokewalkerIr_RequestChunk
	.extern PokewalkerIr_OnReceiveComplete
	.extern PokewalkerIr_OnConnect
	.extern PokewalkerIr_RestartTransfer
	.extern PokewalkerIr_ValidatePeer
	.extern PokewalkerIr_OnReceive
	.extern PokewalkerIr_ThreadMain
	.extern PokewalkerIr_Start
	.extern PokewalkerIr_Stop
	.extern ov112_021E7484
	.extern ov112_021E74A8
	.extern ov112_021E7548
	.extern ov112_021E7594
	.extern ov112_021E7620
	.extern ov112_021E7668
	.extern PokewalkerIr_GetChunkProgress
	.extern PokewalkerIr_GetTotalProgress
	.extern PokewalkerIr_GetAndClearError
	.extern PokewalkerApp_Init
	.extern PokewalkerApp_Setup
	.extern PokewalkerApp_Teardown
	.extern PokewalkerApp_Main
	.extern PokewalkerApp_Exit
	.extern PokewalkerApp_GetSaveData
	.extern PokewalkerApp_VBlankCB
	.extern PokewalkerApp_SetGxBanks
	.extern PokewalkerApp_InitBgs
	.extern PokewalkerApp_FreeBgs
	.extern PokewalkerApp_LoadBgGraphics
	.extern PokewalkerApp_InitMessages
	.extern PokewalkerApp_FreeMessages
	.extern PokewalkerApp_LoadTilemap
	.extern PokewalkerApp_StartLink
	.extern ov112_021E7D7C
	.extern ov112_021E7DA4
	.extern ov112_021E7DC0
	.extern PokewalkerApp_BuildUploadImage
	.extern PokewalkerApp_PickRouteMons
	.extern PokewalkerApp_ApplyAdvantageTypes
	.extern PokewalkerApp_BuildStrollData
	.extern PokewalkerApp_PrintPlayerName
	.extern PokewalkerApp_PrintStrollTexts
	.extern PokewalkerApp_LoadMonPicture
	.extern ov112_021E9338
	.extern PokewalkerApp_GetPixel4bpp
	.extern PokewalkerApp_ConvertTo2bpp
	.extern PokewalkerApp_GetBoxMonIconFile
	.extern PokewalkerApp_GetSpeciesIconFile
	.extern PokewalkerApp_GetPictureFile
	.extern PokewalkerApp_UnlockNextCourse
	.extern PokewalkerApp_GetWattsToNextCourse
	.extern PokewalkerApp_ShowYesNoMenu
	.extern PokewalkerApp_ShowSaveEraseStepsMenu
	.extern PokewalkerApp_HandleTwoChoiceMenu
	.extern PokewalkerApp_GetTouchedButton
	.extern PokewalkerApp_LoadSubBg1Tilemap
	.extern PokewalkerApp_ShowSubBg1
	.extern PokewalkerApp_GetPixel2bpp
	.extern PokewalkerApp_SetPixel2bpp
	.extern PokewalkerApp_DrawSpindaSpots
	.extern PokewalkerApp_DrawSpindaSpotsOnPicture
	.extern PokewalkerApp_AddWindowBank
	.extern PokewalkerApp_RemoveWindowBank
	.extern ov112_021E9C94
	.extern PokewalkerApp_LoadStrings
	.extern PokewalkerApp_FreeStrings
	.extern PokewalkerApp_PrintExit
	.extern PokewalkerApp_PrintMessage
	.extern PokewalkerApp_PrintString
	.extern PokewalkerApp_PrintStringDefaultColor
	.extern PokewalkerApp_PrintSubString
	.extern PokewalkerApp_PrintSubMessage
	.extern PokewalkerApp_StartMessageBox
	.extern PokewalkerApp_ShowMessageBox
	.extern PokewalkerApp_CreateWaitingIcon
	.extern PokewalkerApp_DeleteWaitingIcon
	.extern PokewalkerApp_InitSprites
	.extern PokewalkerApp_FreeSprites
	.extern PokewalkerApp_InitSpriteSystem
	.extern PokewalkerApp_LoadSpriteResources
	.extern PokewalkerApp_CreateSprites
	.extern PokewalkerApp_ResetCursor
	.extern PokewalkerApp_SetCursorPosition
	.extern PokewalkerApp_HideCursor
	.extern PokewalkerApp_PressCursor
	.extern ov112_021EA5A4
	.extern ov112_021EA60C
	.extern ov112_021EA64C
	.extern PokewalkerApp_ShowSprite
	.extern PokewalkerApp_HideSprite
	.extern PokewalkerApp_SetSpriteAnim
	.extern PokewalkerApp_SetSpritePosition
	.extern PokewalkerApp_DrawParticle
	.extern PokewalkerApp_SpawnRisingParticle
	.extern PokewalkerApp_SpawnFallingParticle
	.extern PokewalkerApp_AgeParticles
	.extern PokewalkerApp_UpdateParticles
	.extern PokewalkerApp_ClearParticles
	.extern PokewalkerApp_FindFreeParticle
	.extern ov112_021EA9BC
	.extern ov112_021EAA10
	.extern ov112_021EAA34
	.extern ov112_021EAA98
	.extern ov112_021EAAE4
	.extern PokewalkerApp_LoadMonIconPalettes
	.extern PokewalkerApp_LoadMonIcon
	.extern PokewalkerApp_SetMonIconGraphics
	.extern PokewalkerApp_MainMenu_Enter
	.extern PokewalkerApp_MainMenu_Update
	.extern PokewalkerApp_MainMenu_Exit
	.extern PokewalkerApp_MainMenu_Start
	.extern PokewalkerApp_ShowWalkerRecords
	.extern ov112_021EAF48
	.extern ov112_021EAF5C
	.extern ov112_021EAFFC
	.extern PokewalkerApp_MainMenu_PrintWelcome
	.extern PokewalkerApp_MainMenu_WaitWelcome
	.extern PokewalkerApp_MainMenu_PrintStrollReady
	.extern PokewalkerApp_MainMenu_ShowStrollMenu
	.extern PokewalkerApp_MainMenu_HandleStrollMenu
	.extern PokewalkerApp_MainMenu_EndStrollMenu
	.extern PokewalkerApp_MainMenu_PrintReceive
	.extern PokewalkerApp_MainMenu_WaitReceive
	.extern PokewalkerApp_MainMenu_ShowReceiveMenu
	.extern PokewalkerApp_MainMenu_HandleReceiveMenu
	.extern PokewalkerApp_MainMenu_EndReceiveMenu
	.extern PokewalkerApp_MainMenu_PrintStrollReady2
	.extern PokewalkerApp_MainMenu_WaitStrollReady2
	.extern PokewalkerApp_MainMenu_ShowFullMenu
	.extern PokewalkerApp_MainMenu_HandleFullMenu
	.extern PokewalkerApp_MainMenu_EndFullMenu
	.extern PokewalkerApp_MainMenu_PrintBoxesFull
	.extern PokewalkerApp_MainMenu_WaitBoxesFull
	.extern PokewalkerApp_MainMenu_Quit
	.extern PokewalkerApp_MainMenu_PrintNoPokemon
	.extern PokewalkerApp_MainMenu_WaitMessage
	.extern PokewalkerApp_MainMenu_WaitCursor
	.extern PokewalkerApp_MainMenu_End
	.extern ov112_021EB90C
	.extern ov112_021EB950
	.extern PokewalkerApp_IsDownXLHeld
	.extern PokewalkerApp_IsUpSelectRHeld
	.extern PokewalkerApp_Mode1_Enter
	.extern PokewalkerApp_Mode1_Update
	.extern PokewalkerApp_Mode1_Exit
	.extern PokewalkerApp_Mode1_PrintMessage
	.extern PokewalkerApp_Mode1_WaitMessage
	.extern PokewalkerApp_Mode1_WaitInput
	.extern PokewalkerApp_Mode1_End
	.extern PokewalkerApp_EraseRecords_Enter
	.extern PokewalkerApp_EraseRecords_Update
	.extern PokewalkerApp_EraseRecords_Exit
	.extern PokewalkerApp_EraseRecords_PrintCaution
	.extern PokewalkerApp_EraseRecords_WaitCaution
	.extern PokewalkerApp_EraseRecords_HandleCautionYesNo
	.extern PokewalkerApp_EraseRecords_PrintWillErase
	.extern PokewalkerApp_EraseRecords_ShowEraseMenu
	.extern PokewalkerApp_EraseRecords_HandleEraseMenu
	.extern PokewalkerApp_EraseRecords_EndEraseMenu
	.extern PokewalkerApp_EraseRecords_PrintCarryOverSteps
	.extern PokewalkerApp_EraseRecords_WaitCarryOverSteps
	.extern PokewalkerApp_EraseRecords_ShowCarryOverStepsMenu
	.extern PokewalkerApp_EraseRecords_HandleCarryOverStepsMenu
	.extern PokewalkerApp_EraseRecords_PrintConnect
	.extern PokewalkerApp_EraseRecords_WaitMessage
	.extern PokewalkerApp_EraseRecords_PrintPointAtCard
	.extern PokewalkerApp_EraseRecords_WaitPointAtCard
	.extern PokewalkerApp_EraseRecords_PrepareLink
	.extern PokewalkerApp_EraseRecords_StartLink
	.extern PokewalkerApp_EraseRecords_UpdateLink
	.extern PokewalkerApp_ReturnWalkerMonToBox
	.extern PokewalkerApp_EraseRecords_PrintCanceled
	.extern PokewalkerApp_EraseRecords_WaitCanceled
	.extern PokewalkerApp_EraseRecords_PrintInitialized
	.extern PokewalkerApp_EraseRecords_WaitInitialized
	.extern PokewalkerApp_EraseRecords_PrintComeBack
	.extern PokewalkerApp_EraseRecords_WaitComeBack
	.extern PokewalkerApp_EraseRecords_ShowReturnedMon
	.extern PokewalkerApp_EraseRecords_AnimateReturnedMon
	.extern PokewalkerApp_EraseRecords_AfterAnimation
	.extern PokewalkerApp_EraseRecords_PrintArrived
	.extern PokewalkerApp_EraseRecords_WaitArrived
	.extern PokewalkerApp_EraseRecords_PrintSaving
	.extern PokewalkerApp_EraseRecords_WaitSaving
	.extern PokewalkerApp_EraseRecords_Save
	.extern PokewalkerApp_EraseRecords_EndSave
	.extern PokewalkerApp_EraseRecords_WaitCursor
	.extern PokewalkerApp_EraseRecords_End
	.extern PokewalkerApp_SelectMenuItem
	.extern PokewalkerApp_StartMessage
	.extern PokewalkerApp_Link_Enter
	.extern PokewalkerApp_Link_Update
	.extern PokewalkerApp_Link_Exit
	.extern PokewalkerApp_Link_Start
	.extern PokewalkerApp_Link_PrintPointAtCard
	.extern PokewalkerApp_Link_WaitPointAtCard
	.extern PokewalkerApp_Link_FadeOutToSelectMon
	.extern PokewalkerApp_Link_WaitFadeOutToSelectMon
	.extern PokewalkerApp_Link_FadeIn
	.extern PokewalkerApp_Link_WaitFadeIn
	.extern PokewalkerApp_Link_PrintSelectCourse
	.extern PokewalkerApp_Link_WaitSelectCourse
	.extern PokewalkerApp_GetCoursePageCount
	.extern PokewalkerApp_PrintCoursePage
	.extern PokewalkerApp_PrintCourseInfo
	.extern PokewalkerApp_ListUnlockedCourses
	.extern PokewalkerApp_Link_ShowCourseMenu
	.extern PokewalkerApp_UpdatePageArrows
	.extern PokewalkerApp_GetSelectedCourse
	.extern PokewalkerApp_Link_HandleCourseMenu
	.extern PokewalkerApp_Link_EndCourseMenu
	.extern PokewalkerApp_ChangeCoursePage
	.extern PokewalkerApp_Link_PrintConfirmCourse
	.extern PokewalkerApp_Link_WaitConfirmCourse
	.extern PokewalkerApp_Link_ShowConfirmCourseYesNo
	.extern PokewalkerApp_Link_HandleConfirmCourseYesNo
	.extern PokewalkerApp_Link_PrintInstructions
	.extern PokewalkerApp_Link_WaitInstructions
	.extern PokewalkerApp_Link_PrintWillConnectWithMon
	.extern PokewalkerApp_Link_PrintWillConnect
	.extern PokewalkerApp_Link_PrintPointAtCardForStroll
	.extern PokewalkerApp_Link_WaitPointAtCardForStroll
	.extern PokewalkerApp_ShowMonPicture
	.extern PokewalkerApp_FadeInMonPicture
	.extern PokewalkerApp_FadeInMonPictureTask
	.extern PokewalkerApp_ScrollSubBgTask
	.extern PokewalkerApp_Link_PrintPointAtCardForGifts
	.extern PokewalkerApp_Link_WaitPointAtCardForGifts
	.extern PokewalkerApp_ResetLinkError
	.extern PokewalkerApp_Link_StartUpload
	.extern PokewalkerApp_StartSubBgScroll
	.extern PokewalkerApp_PlayTransferSE
	.extern PokewalkerApp_UpdateLinkProgress
	.extern PokewalkerApp_Link_UpdateUpload
	.extern PokewalkerApp_Link_StartReceive
	.extern ov112_021ED530
	.extern ov112_021ED590
	.extern ov112_021ED5DC
	.extern ov112_021ED610
	.extern ov112_021ED640
	.extern ov112_021ED66C
	.extern ov112_021ED7DC
	.extern ov112_021ED8DC
	.extern ov112_021ED918
	.extern ov112_021EDA4C
	.extern ov112_021EDAF4
	.extern ov112_021EDB24
	.extern ov112_021EDCB0
	.extern ov112_021EDD08
	.extern ov112_021EDD30
	.extern ov112_021EDD5C
	.extern ov112_021EDD80
	.extern ov112_021EDDB4
	.extern ov112_021EDDE8
	.extern ov112_021EDE1C
	.extern ov112_021EDE68
	.extern ov112_021EDF08
	.extern ov112_021EDF4C
	.extern ov112_021EDFE8
	.extern ov112_021EE044
	.extern ov112_021EE0F4
	.extern ov112_021EE10C
	.extern ov112_021EE12C
	.extern ov112_021EE178
	.extern ov112_021EE1A4
	.extern ov112_021EE1BC
	.extern ov112_021EE1DC
	.extern ov112_021EE1E8
	.extern ov112_021EE29C
	.extern ov112_021EE2B4
	.extern ov112_021EE2D4
	.extern ov112_021EE31C
	.extern ov112_021EE3E0
	.extern ov112_021EE464
	.extern ov112_021EE4B8
	.extern ov112_021EE4D0
	.extern ov112_021EE510
	.extern ov112_021EE514
	.extern ov112_021EE524
	.extern ov112_021EE52C
	.extern ov112_021EE550
	.extern ov112_021EE570
	.extern ov112_021EE594
	.extern ov112_021EE5B8
	.extern PokewalkerApp_Mode4_Enter
	.extern PokewalkerApp_Mode4_Update
	.extern PokewalkerApp_Mode4_Exit
	.extern PokewalkerApp_StartStroll_Enter
	.extern PokewalkerApp_StartStroll_Update
	.extern PokewalkerApp_StartStroll_Exit
	.extern PokewalkerApp_StartStroll_PrintSaving
	.extern PokewalkerApp_StartStroll_WaitSaving
	.extern PokewalkerApp_StartStroll_Save
	.extern PokewalkerApp_StartStroll_EndSave
	.extern PokewalkerApp_StartStroll_CheckFirstStroll
	.extern PokewalkerApp_StartStroll_PrintRegistered
	.extern PokewalkerApp_StartStroll_WaitRegistered
	.extern PokewalkerApp_StartStroll_End
	.extern PokewalkerApp_ReturnStrollMon
	.extern PokewalkerApp_AddStrollExp
	.extern PokewalkerApp_GiveStarfBerry
	.extern PokewalkerApp_AddStrollFriendship
	.extern PokewalkerApp_CreateWalkerMonWithOT
	.extern PokewalkerApp_CreateWalkerMon
	.extern PokewalkerApp_ReceiveItems
	.extern PokewalkerApp_ReceiveMons
	.extern PokewalkerApp_ReceiveGifts
	.extern PokewalkerApp_LinkSave_Enter
	.extern PokewalkerApp_LinkSave_Update
	.extern PokewalkerApp_LinkSave_Exit
	.extern PokewalkerApp_LinkSave_PrintSaving
	.extern PokewalkerApp_LinkSave_WaitSaving
	.extern PokewalkerApp_LinkSave_Save
	.extern PokewalkerApp_LinkSave_EndSave
	.extern PokewalkerApp_LinkSave_SetNextMode
	.extern PokewalkerApp_LinkSave_PrintSaving2
	.extern PokewalkerApp_LinkSave_WaitSaving2
	.extern PokewalkerApp_LinkSave_Save2
	.extern PokewalkerApp_LinkSave_EndSave2
	.extern PokewalkerApp_LinkSave_SetNextMode2
	.extern PokewalkerApp_LinkSave_End
	.extern PokewalkerApp_SelectMon_Enter
	.extern PokewalkerApp_SelectMon_Update
	.extern PokewalkerApp_SelectMon_Exit
	.extern PokewalkerApp_SelectMon_SetGxBanks
	.extern PokewalkerApp_SelectMon_SetGraphicsModes
	.extern PokewalkerApp_SelectMon_InitObjTransfer
	.extern PokewalkerApp_SelectMon_InitOam
	.extern PokewalkerApp_SelectMon_GetBoxMon
	.extern PokewalkerApp_SelectMon_GetBoxMonIcon
	.extern PokewalkerApp_SelectMon_GetBoxName
	.extern PokewalkerApp_SelectMon_VBlankCB
	.extern PokewalkerApp_SelectMon_StoreSelection
	.extern PokewalkerApp_SelectMon_ShowMonInfo
	.extern PokewalkerApp_EventLog_Enter
	.extern PokewalkerApp_EventLog_Update
	.extern PokewalkerApp_EventLog_Exit
	.extern PokewalkerApp_WaitKey_Enter
	.extern PokewalkerApp_WaitKey_Update
	.extern PokewalkerApp_WaitKey_Exit
	.extern PokewalkerApp_StrollReport_Enter
	.extern PokewalkerApp_StrollReport_Update
	.extern PokewalkerApp_StrollReport_Exit
	.extern PokewalkerApp_StrollReport_Start
	.extern PokewalkerApp_StrollReport_ShowResults
	.extern PokewalkerApp_StrollReport_WaitResultsFade
	.extern PokewalkerApp_StrollReport_PrintMonArrived
	.extern PokewalkerApp_StrollReport_PrintLevelUp
	.extern PokewalkerApp_StrollReport_PrintWatts
	.extern PokewalkerApp_StrollReport_PrintRouteFound
	.extern PokewalkerApp_StrollReport_PrintRouteAvailable
	.extern PokewalkerApp_StrollReport_PrintMilestone
	.extern PokewalkerApp_StrollReport_PrintStarfBerry
	.extern PokewalkerApp_StrollReport_FadeOut
	.extern PokewalkerApp_StrollReport_WaitFadeOut
	.extern PokewalkerApp_CountReceivedMons
	.extern PokewalkerApp_StrollReport_ShowCaughtMons
	.extern PokewalkerApp_StrollReport_WaitCaughtMonsFade
	.extern PokewalkerApp_StrollReport_PrintMonsSent
	.extern PokewalkerApp_StrollReport_CaughtMonsFadeOut
	.extern PokewalkerApp_StrollReport_WaitCaughtMonsFadeOut
	.extern PokewalkerApp_AddItemToList
	.extern PokewalkerApp_CountReceivedItems
	.extern PokewalkerApp_StrollReport_ShowItems
	.extern PokewalkerApp_StrollReport_WaitItemsFade
	.extern PokewalkerApp_StrollReport_PrintItemsPlaced
	.extern PokewalkerApp_StrollReport_ItemsFadeOut
	.extern PokewalkerApp_StrollReport_WaitItemsFadeOut
	.extern PokewalkerApp_StrollReport_GiftsFadeOut
	.extern PokewalkerApp_StrollReport_WaitGiftsFadeOut
	.extern PokewalkerApp_StrollReport_ShowGifts
	.extern PokewalkerApp_StrollReport_WaitGiftsFade
	.extern PokewalkerApp_StrollReport_PrintGiftWatts
	.extern PokewalkerApp_StrollReport_PrintGiftRouteFound
	.extern PokewalkerApp_StrollReport_GiftsDoneFadeOut
	.extern PokewalkerApp_StrollReport_WaitGiftsDoneFadeOut
	.extern PokewalkerApp_StrollReport_RecordsFadeOut
	.extern PokewalkerApp_StrollReport_WaitRecordsFadeOut
	.extern PokewalkerApp_StrollReport_ShowRecords
	.extern PokewalkerApp_StrollReport_WaitRecordsFade
	.extern PokewalkerApp_StrollReport_PrintRecords
	.extern PokewalkerApp_StrollReport_WaitMessage
	.extern PokewalkerApp_StrollReport_End
	.extern PokewalkerBoxDisplay_New
	.extern PokewalkerBoxDisplay_Init
	.extern PokewalkerBoxDisplay_ShowMon
	.extern PokewalkerBoxDisplay_Update
	.extern PokewalkerBoxDisplay_Free
	.extern PokewalkerBoxDisplay_DeleteSprites
	.extern PokewalkerBoxDisplay_LoadBgGraphics
	.extern PokewalkerBoxDisplay_InitBgs
	.extern PokewalkerBoxDisplay_PrintMonInfo
	.extern PokewalkerBoxDisplay_ShowMonSprites
	.extern PokewalkerBoxDisplay_HideSprites
	.extern PokewalkerBoxDisplay_InitSprites
	.extern PokewalkerBoxDisplay_CreateSprites
	.extern PokewalkerBoxDisplay_CreateSprite
	.extern PokewalkerApp_InitSpriteTemplate
	.extern PokewalkerBoxDisplay_GetMonPictureLocation
	.extern PokewalkerBoxDisplay_LoadMonPicture
	.extern PokewalkerBoxDisplay_TransferMonPicture
	.extern ov112_021F0D48
	.extern ov112_021F0DC0
	.extern ov112_021F0DF4
	.extern ov112_021F0E14
	.extern ov112_021F0E60
	.extern ov112_021F0EB4
	.extern ov112_021F0EFC
	.extern ov112_021F0F48
	.extern ov112_021F0F60
	.extern ov112_021F0F70
	.extern ov112_021F0F90
	.extern ov112_021F1288
	.extern ov112_021F1324
	.extern ov112_021F135C
	.extern ov112_021F13BC
	.extern ov112_021F13CC
	.extern ov112_021F1488
	.extern ov112_021F1504
	.extern ov112_021F1548
	.extern ov112_021F1584
	.extern ov112_021F15CC
	.extern ov112_021F15E4
	.extern ov112_021F1624
	.extern ov112_021F178C
	.extern ov112_021F179C
	.extern ov112_021F1814
	.extern ov112_021F1838
	.extern ov112_021F1904
	.extern ov112_021F194C
	.extern ov112_021F196C
	.extern ov112_021F1A08
	.extern ov112_021F1A40
	.extern ov112_021F1AF4
	.extern ov112_021F1B44
	.extern ov112_021F1B94
	.extern ov112_021F1CC8
	.extern ov112_021F1D28
	.extern ov112_021F1D58
	.extern ov112_021F1D70
	.extern ov112_021F1DC0
	.extern ov112_021F1EFC
	.extern ov112_021F1F3C
	.extern ov112_021F1F54
	.extern ov112_021F1F80
	.extern ov112_021F1FD0
	.extern ov112_021F2000
	.extern ov112_021F2098
	.extern ov112_021F2204
	.extern ov112_021F22B0
	.extern ov112_021F22D0
	.extern ov112_021F2328
	.extern ov112_021F2338
	.extern ov112_021F2348
	.extern ov112_021F235C
	.extern ov112_021F238C
	.extern ov112_021F24C4
	.extern ov112_021F24D8
	.extern ov112_021F24F8
	.extern ov112_021F257C
	.extern ov112_021F25FC
	.extern ov112_021F2688
	.extern ov112_021F26E8
	.extern ov112_021F2768
	.extern ov112_021F27B8
	.extern ov112_021F27E0
	.extern ov112_021F2810
	.extern ov112_021F2874
	.extern ov112_021F28DC
	.extern ov112_021F2920
	.extern ov112_021F2944
	.extern ov112_021F298C
	.extern ov112_021F2A78
	.extern ov112_021F2B2C
	.extern ov112_021F2B80
	.extern ov112_021F2CC4
	.extern ov112_021F2CD4
	.extern ov112_021F2DE4
	.extern ov112_021F2E70
	.extern ov112_021F2EB0
	.extern ov112_021F2EF4
	.extern ov112_021F2FAC
	.extern ov112_021F3018
	.extern ov112_021F30BC
	.extern ov112_021F3140
	.extern ov112_021F31BC
	.extern PokewalkerApp_GetTrainerHouseDailyFlags
	.extern PokewalkerApp_SetTrainerHouseDailyFlags
	.extern ov112_021F3664
	.extern ov112_021F366C
	.extern ov112_021F3678
	.extern ov112_021F3684
	.extern ov112_021F3694
	.extern ov112_021F36A4
	.extern ov112_021F36B8
	.extern ov112_021F36DC
	.extern ov112_021F3704
	.extern ov112_021F3720
	.extern ov112_021F373C
	.extern ov112_021F3758
	.extern ov112_021F3774
	.extern ov112_021F3790
	.extern ov112_021F37AC
	.extern ov112_021F3814
	.extern ov112_021F387E
	.extern ov112_021F38E8
	.extern sPokewalkerPictureFiles
	.extern sPokewalkerCourses
	.extern sPokewalkerCourseRouteImageIds
	.extern sStrollTextWindowWidths
	.extern ov112_021F5588
	.extern ov112_021F5598
	.extern ov112_021F55A8
	.extern ov112_021F55B8
	.extern ov112_021F55C8
	.extern ov112_021F55D8
	.extern ov112_021F55E8
	.extern ov112_021F55F8
	.extern ov112_021F5608
	.extern ov112_021F5618
	.extern ov112_021F5628
	.extern ov112_021F5638
	.extern ov112_021F5648
	.extern ov112_021F5658
	.extern ov112_021F5668
	.extern ov112_021F5678
	.extern ov112_021F5688
	.extern ov112_021F5698
	.extern ov112_021F56A8
	.extern ov112_021F56B8
	.extern ov112_021F56C8
	.extern ov112_021F56D8
	.extern ov112_021F56E8
	.extern ov112_021F56F8
	.extern ov112_021F5708
	.extern ov112_021F5718
	.extern ov112_App_MainMenu_SelectOption_ConnectToPokewalker
	.extern ov112_021F5738
	.extern ov112_021F5748
	.extern ov112_021F5758
	.extern ov112_021F5778
	.extern ov112_021F5798
	.extern ov112_021F57B8
	.extern ov112_021F57D8
	.extern ov112_021F57F8
	.extern ov112_021F5818
	.extern ov112_021F5838
	.extern ov112_021F5858
	.extern ov112_021F5878
	.extern ov112_021F5898
	.extern ov112_021F58B8
	.extern ov112_021F58D8
	.extern ov112_021F58F8
	.extern ov112_021F5918
	.extern ov112_021F5938
	.extern ov112_021F5958
	.extern ov112_021F5978
	.extern ov112_021F5998
	.extern ov112_021F59D8
	.extern ov112_021F5A18
	.extern ov112_021F5A58
	.extern ov112_021F5A98
	.extern ov112_021F5AD8
	.extern ov112_021F5B18
	.extern ov112_021F5B58
	.extern ov112_021F5B98
	.extern ov112_021F5BD8
	.extern ov112_021F5C18
	.extern ov112_021F5C58
	.extern ov112_021F5C98
	.extern ov112_021F5CD8
	.extern ov112_021F5D18
	.extern ov112_021F5D58
	.extern ov112_021F5DB8
	.extern ov112_021F5E18
	.extern ov112_021F5E78
	.extern ov112_021F5ED8
	.extern ov112_021F5F38
	.extern ov112_021F5F98
	.extern ov112_021F5FF8
	.extern ov112_021F6058
	.extern ov112_021F60B8
	.extern ov112_021F6118
	.extern ov112_021F6178
	.extern ov112_021F61F8
	.extern ov112_021F6278
	.extern ov112_021F62F8
	.extern ov112_021F6378
	.extern ov112_021F63F8
	.extern ov112_021F6498
	.extern ov112_021F6538
	.extern ov112_021F65D8
	.extern ov112_021F6678
	.extern ov112_021F6718
	.extern ov112_021F67D8
	.extern ov112_021F6898
	.extern ov112_021F6958
	.extern ov112_021F6A18
	.extern ov112_021F6AD8
	.extern ov112_021F6B98
	.extern ov112_021F6C58
	.extern ov112_021F6D18
	.extern ov112_021F6DD8
	.extern ov112_021F6E98
	.extern ov112_021F6F58
	.extern ov112_021F7018
	.extern ov112_021F70D8
	.extern ov112_021F71D8
	.extern ov112_021F72D8
	.extern ov112_021F7418
	.extern ov112_021F7558
	.extern ov112_021F7698
	.extern ov112_021F77D8
	.extern ov112_021F7918
	.extern ov112_021F7A58
	.extern ov112_021F7B98
	.extern ov112_021F7D18
	.extern ov112_021F7E98
	.extern ov112_021F8018
	.extern ov112_021F8198
	.extern ov112_021F8318
	.extern ov112_021F8498
	.extern ov112_021F8618
	.extern ov112_021F8798
	.extern ov112_021F8918
	.extern ov112_021F8A98
	.extern ov112_021F8C18
	.extern ov112_021F8D98
	.extern ov112_021F8F18
	.extern ov112_021F9098
	.extern ov112_021F9218
	.extern ov112_021F9398
	.extern ov112_021F9518
	.extern ov112_021F9698
	.extern ov112_021F9818
	.extern ov112_021F9998
	.extern ov112_021F9B18
	.extern ov112_021F9C98
	.extern ov112_021F9E18
	.extern ov112_021F9F98
	.extern ov112_021FA118
	.extern ov112_021FA298
	.extern ov112_021FA418
	.extern ov112_021FA598
	.extern ov112_021FA718
	.extern ov112_021FA898
	.extern ov112_021FAA18
	.extern ov112_021FAB98
	.extern ov112_021FAD18
	.extern ov112_021FAE98
	.extern ov112_021FB018
	.extern ov112_021FB198
	.extern ov112_021FB318
	.extern ov112_021FB498
	.extern ov112_021FB618
	.extern ov112_021FB798
	.extern ov112_021FB918
	.extern ov112_021FBA98
	.extern ov112_021FBC18
	.extern ov112_021FBD98
	.extern ov112_021FBF18
	.extern ov112_021FC098
	.extern ov112_021FC218
	.extern ov112_021FC398
	.extern ov112_021FC518
	.extern ov112_021FC698
	.extern ov112_021FC818
	.extern ov112_021FC998
	.extern ov112_021FCB18
	.extern ov112_021FCC98
	.extern ov112_021FCE18
	.extern ov112_021FCF98
	.extern ov112_021FD118
	.extern ov112_021FD298
	.extern ov112_021FD418
	.extern ov112_021FD598
	.extern ov112_021FD718
	.extern ov112_021FD898
	.extern ov112_021FDA18
	.extern ov112_021FDB98
	.extern ov112_021FDD18
	.extern ov112_021FDE98
	.extern ov112_021FE018
	.extern ov112_021FE198
	.extern sSpindaSpotMask
	.extern ov112_021FEA98
	.extern ov112_021FEAA0
	.extern ov112_021FEAA8
	.extern ov112_021FEAB0
	.extern ov112_021FEAB8
	.extern ov112_021FEAC0
	.extern ov112_021FEAC8
	.extern ov112_021FEAD0
	.extern ov112_021FEAD8
	.extern ov112_021FEAE0
	.extern ov112_021FEAE8
	.extern ov112_021FEAF0
	.extern ov112_021FEAF8
	.extern ov112_021FEB00
	.extern ov112_021FEB08
	.extern ov112_021FEB10
	.extern ov112_021FEB18
	.extern ov112_021FEB20
	.extern ov112_021FEB28
	.extern ov112_021FEB30
	.extern ov112_021FEB38
	.extern ov112_021FEB40
	.extern ov112_021FEB48
	.extern ov112_021FEB50
	.extern ov112_021FEB58
	.extern ov112_021FEB60
	.extern ov112_021FEB68
	.extern ov112_021FEB70
	.extern ov112_021FEB78
	.extern ov112_021FEB80
	.extern ov112_021FEB88
	.extern ov112_021FEB90
	.extern ov112_021FEB98
	.extern ov112_021FEBA0
	.extern ov112_021FEBA8
	.extern ov112_021FEBB0
	.extern ov112_021FEBB8
	.extern ov112_021FEBC0
	.extern ov112_021FEBC8
	.extern ov112_021FEBD0
	.extern ov112_021FEBD8
	.extern ov112_021FEBE0
	.extern ov112_021FEBE8
	.extern ov112_021FEBF0
	.extern ov112_021FEBF8
	.extern ov112_021FEC00
	.extern ov112_021FEC08
	.extern ov112_021FEC10
	.extern ov112_021FEC18
	.extern ov112_021FEC20
	.extern ov112_021FEC28
	.extern ov112_021FEC30
	.extern ov112_021FEC38
	.extern ov112_021FEC40
	.extern ov112_021FEC48
	.extern ov112_021FEC50
	.extern ov112_021FEC58
	.extern ov112_021FEC60
	.extern ov112_021FEC68
	.extern ov112_021FEC70
	.extern ov112_021FEC78
	.extern sWindowBankScreens
	.extern ov112_021FEC88
	.extern ov112_021FEC98
	.extern ov112_021FECAC
	.extern ov112_021FECC4
	.extern ov112_021FECE4
	.extern ov112_021FECE5
	.extern ov112_021FECE6
	.extern ov112_021FECE7
	.extern ov112_021FED1C
	.extern ov112_021FF028
	.extern ov112_021FF05C
	.extern ov112_021FF090
	.extern ov112_021FF09C
	.extern ov112_021FF0AC
	.extern sCourseMenuCursorPositions
	.extern sSelectMonGraphicsModes
	.extern sSelectMonObjCharTransferTemplate
	.extern sSelectMonGraphicsBanks
	.extern sEventLogAppTemplate
	.extern sBoxDisplayBgTemplateSub1
	.extern sBoxDisplayBgTemplateSub3
	.extern ov112_021FF16C
	.extern ov112_021FF172
	.extern ov112_021FF17A
	.extern ov112_021FF182
	.extern ov112_021FF18A
	.extern ov112_021FF192
	.extern ov112_021FF19A
	.extern ov112_021FF1A4
	.extern ov112_021FF1AE
	.extern ov112_021FF1B8
	.extern ov112_021FF1C6
	.extern ov112_021FF1D8
	.extern ov112_021FF1EC
	.extern ov112_021FF200
	.extern ov112_021FF21C
	.extern ov112_021FF244
	.extern ov112_021FF284
	.extern ov112_021FF2CC
	.extern ov112_021FF2DC
	.extern ov112_021FF2EC
	.extern ov112_021FF2F4
	.extern ov112_021FF304
	.extern ov112_021FF318
	.extern ov112_021FF330
	.extern ov112_021FF350
	.extern ov112_021FF370
	.extern ov112_021FF398
	.extern ov112_021FF3CC
	.extern ov112_021FF400
	.extern ov112_021FF434
	.extern ov112_021FF4C0
	.extern ov112_021FF4C8
	.extern sTrainerHouseDailyFlags
	.extern _021FF500
	.extern sSpindaSpotPixels
	.extern sTouchButtons
	.extern sPokewalkerRouteImages
	.extern ov112_021FF54C
	.extern ov112_021FF550
	.extern ov112_021FF554
	.extern ov112_021FF5D0
	.extern ov112_021FF5D8
	.extern ov112_021FF5F0
	.extern ov112_021FF608
	.extern ov112_021FF624
	.extern ov112_021FF640
	.extern ov112_021FF660
	.extern ov112_021FF684
	.extern sMainMenuStates
	.extern sMode1States
	.extern sEraseRecordsStates
	.extern sLinkStates
	.extern sStartStrollStates
	.extern sLinkSaveStates
	.extern sStrollReportStates
	.extern ov112_021FF9B8
	.extern _021FF9E0
	.extern ov112_021FFA18
	.extern ov112_021FFA20
	.extern ov112_021FFAA4
	.extern ov112_021FFAC0
	.extern ov112_021FFAF0
	.extern ov112_021FFAF8
	.extern ov112_021FFB08
	.extern ov112_021FFB20
	.extern ov112_021FFB24
	.extern ov112_021FFB64
	.extern ov112_021FFBD8


	thumb_func_start PokewalkerApp_IsEventLogInvalid
PokewalkerApp_IsEventLogInvalid: ; 0x021F31D8
	push {r3, r4, r5, r6}
	ldr r4, _021F323C ; =0x000001ED
	ldr r5, _021F3240 ; =0x0000FFF9
	mov r2, #0
	add r3, r4, #0
_021F31E2:
	add r1, r0, #0
	add r1, #0x84
	ldrb r1, [r1]
	cmp r1, #0
	beq _021F322E
	cmp r1, #0x1e
	blo _021F31F6
	mov r0, #1
	pop {r3, r4, r5, r6}
	bx lr
_021F31F6:
	ldrh r1, [r0, #0xa]
	cmp r1, #0
	bne _021F3202
	mov r0, #1
	pop {r3, r4, r5, r6}
	bx lr
_021F3202:
	ldrh r6, [r0, #8]
	add r6, r6, r5
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	cmp r6, #1
	bhi _021F321E
	cmp r1, r4
	bhi _021F3218
	ldrh r1, [r0, #0xc]
	cmp r1, r3
	bls _021F321E
_021F3218:
	mov r0, #1
	pop {r3, r4, r5, r6}
	bx lr
_021F321E:
	add r1, r0, #0
	add r1, #0x76
	ldrb r1, [r1]
	cmp r1, #8
	blo _021F322E
	mov r0, #1
	pop {r3, r4, r5, r6}
	bx lr
_021F322E:
	add r2, r2, #1
	add r0, #0x88
	cmp r2, #0x18
	blt _021F31E2
	mov r0, #0
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
_021F323C: .word 0x000001ED
_021F3240: .word 0x0000FFF9
	thumb_func_end PokewalkerApp_IsEventLogInvalid

	thumb_func_start PokewalkerApp_BuildTrainerHouseData
PokewalkerApp_BuildTrainerHouseData: ; 0x021F3244
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl Save_PlayerData_GetProfile
	add r0, r5, #0
	bl SaveArray_Party_Get
	mov r2, #0x7d
	add r6, r0, #0
	mov r0, #0
	add r1, r4, #0
	lsl r2, r2, #2
	bl MIi_CpuClearFast
	add r0, r4, #0
	bl TrainerHouseSet_SetZero
	add r0, r4, #0
	add r1, r5, #0
	bl PokewalkerApp_FillTrainerHouseTrainer
	add r0, r4, #0
	add r0, #0x30
	add r1, r6, #0
	bl PokewalkerApp_FillTrainerHouseParty
	ldr r1, _021F3288 ; =0x000001F2
	add r0, r4, #0
	bl GF_CalcCRC16
	ldr r1, _021F3288 ; =0x000001F2
	strh r0, [r4, r1]
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021F3288: .word 0x000001F2
	thumb_func_end PokewalkerApp_BuildTrainerHouseData

	thumb_func_start PokewalkerApp_SaveTrainerHouseData
PokewalkerApp_SaveTrainerHouseData: ; 0x021F328C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	str r1, [sp, #4]
	mov r1, #0
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	str r0, [sp]
	ldr r5, [sp, #0x10]
	bl Save_TrainerHouse_Get
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	bl TrainerHouseSet_CheckHasData
	cmp r0, #0
	bne _021F32B6
	ldr r0, [sp, #8]
	bl PokewalkerApp_CountTrainerHouseSets
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_021F32B6:
	ldr r0, [sp, #4]
	ldr r1, _021F3368 ; =0x000001F2
	bl GF_CalcCRC16
	ldr r2, _021F3368 ; =0x000001F2
	ldr r1, [sp, #4]
	ldrh r1, [r1, r2]
	cmp r1, r0
	bne _021F3364
	ldr r0, [sp, #8]
	bl PokewalkerApp_CountTrainerHouseSets
	str r0, [sp, #0xc]
	ldr r0, [sp]
	add r1, sp, #0x18
	bl PokewalkerApp_GetTrainerHouseDailyFlags
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	add r2, sp, #0x14
	bl PokewalkerApp_FindTrainerHouseSet
	add r4, r0, #0
	ldr r0, [sp, #0xc]
	cmp r0, #0xa
	bge _021F32F0
	ldr r0, [sp, #0x14]
	cmp r0, #0
	beq _021F32FE
_021F32F0:
	ldr r0, [sp, #0xc]
	mov r5, #1
	sub r0, r0, #1
	str r0, [sp, #0xc]
	add r0, sp, #0x18
	ldrb r0, [r0, r4]
	str r0, [sp, #0x10]
_021F32FE:
	cmp r5, #0
	beq _021F3342
	ldr r0, [sp, #0xc]
	cmp r4, r0
	bge _021F3342
	cmp r4, r0
	bge _021F3342
	mov r0, #6
	lsl r0, r0, #6
	add r1, r4, #0
	mul r1, r0
	ldr r0, [sp, #8]
	add r5, r0, r1
	add r0, sp, #0x18
	add r6, r0, r4
_021F331C:
	mov r0, #6
	lsl r0, r0, #6
	add r3, r5, r0
	add r7, r5, #0
	mov r2, #0x30
_021F3326:
	ldmia r3!, {r0, r1}
	stmia r7!, {r0, r1}
	sub r2, r2, #1
	bne _021F3326
	ldrb r0, [r6, #1]
	add r4, r4, #1
	strb r0, [r6]
	mov r0, #6
	lsl r0, r0, #6
	add r5, r5, r0
	ldr r0, [sp, #0xc]
	add r6, r6, #1
	cmp r4, r0
	blt _021F331C
_021F3342:
	ldr r1, [sp, #0xc]
	mov r2, #6
	add r3, r1, #0
	lsl r2, r2, #6
	ldr r1, [sp, #8]
	mul r3, r2
	ldr r0, [sp, #4]
	add r1, r1, r3
	bl MI_CpuCopy8
	ldr r2, [sp, #0x10]
	ldr r0, [sp, #0xc]
	add r1, sp, #0x18
	strb r2, [r1, r0]
	ldr r0, [sp]
	bl PokewalkerApp_SetTrainerHouseDailyFlags
_021F3364:
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021F3368: .word 0x000001F2
	thumb_func_end PokewalkerApp_SaveTrainerHouseData

	thumb_func_start PokewalkerApp_FillTrainerHouseTrainer
PokewalkerApp_FillTrainerHouseTrainer: ; 0x021F336C
	push {r4, r5, r6, lr}
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	bl Save_PlayerData_GetProfile
	add r4, r0, #0
	bl PlayerProfile_GetTrainerID
	str r0, [r5]
	add r0, r4, #0
	bl PlayerProfile_GetTrainerGender
	strb r0, [r5, #7]
	add r0, r4, #0
	bl PlayerProfile_GetLanguage
	strb r0, [r5, #5]
	add r0, r4, #0
	bl PlayerProfile_GetVersion
	strb r0, [r5, #6]
	add r0, r4, #0
	bl PlayerProfile_GetAvatar
	strb r0, [r5, #4]
	add r0, r4, #0
	bl PlayerProfile_GetNamePtr
	add r1, r0, #0
	add r0, r5, #0
	add r0, #8
	mov r2, #7
	bl CopyU16StringArrayN
	mov r4, #0
	add r5, #0x18
_021F33B6:
	add r0, r6, #0
	add r1, r4, #0
	bl Save_Frontier_GetTowerMessage
	add r0, r6, #0
	add r1, r4, #0
	bl Save_Frontier_GetTowerMessage
	add r1, r0, #0
	add r0, r5, #0
	bl MailMsg_Copy
	add r4, r4, #1
	add r5, #8
	cmp r4, #3
	blt _021F33B6
	pop {r4, r5, r6, pc}
	thumb_func_end PokewalkerApp_FillTrainerHouseTrainer

	thumb_func_start PokewalkerApp_FillTrainerHouseParty
PokewalkerApp_FillTrainerHouseParty: ; 0x021F33D8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	add r6, r0, #0
	add r0, r1, #0
	str r1, [sp]
	bl Party_GetCount
	str r0, [sp, #0x18]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bgt _021F33F6
	b _021F3598
_021F33F6:
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r1, [sp, #0x1c]
	bl Party_GetMonByIndex
	mov r1, #3
	mov r2, #0
	add r5, r0, #0
	bl GetMonData
	cmp r0, #0
	bne _021F341E
	add r0, r5, #0
	mov r1, #0x4c
	mov r2, #0
	bl GetMonData
	cmp r0, #0
	beq _021F3420
_021F341E:
	b _021F358A
_021F3420:
	add r0, r5, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	ldrh r1, [r6]
	ldr r2, _021F359C ; =0xFFFFF800
	lsr r3, r0, #0x10
	lsr r0, r2, #0x15
	and r1, r2
	and r0, r3
	orr r0, r1
	strh r0, [r6]
	add r0, r5, #0
	mov r1, #0x70
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r0, r0, #0x1b
	ldrh r2, [r6]
	ldr r1, _021F35A0 ; =0xFFFF07FF
	lsr r0, r0, #0x10
	and r1, r2
	orr r0, r1
	strh r0, [r6]
	ldrh r1, [r6]
	lsl r0, r1, #0x15
	lsr r2, r0, #0x15
	mov r0, #0x7b
	lsl r0, r0, #2
	cmp r2, r0
	bne _021F3482
	lsl r0, r1, #0x10
	lsr r0, r0, #0x1b
	cmp r0, #1
	bne _021F3482
	add r0, r5, #0
	mov r1, #0
	bl Mon_UpdateShayminForm
	ldrh r1, [r6]
	ldr r0, _021F35A0 ; =0xFFFF07FF
	and r0, r1
	strh r0, [r6]
	mov r0, #1
	str r0, [sp, #0xc]
_021F3482:
	add r0, r5, #0
	mov r1, #6
	mov r2, #0
	bl GetMonData
	strh r0, [r6, #2]
	add r0, r5, #0
	mov r1, #7
	mov r2, #0
	bl GetMonData
	mov r1, #0
	str r0, [r6, #0xc]
	add r0, r5, #0
	add r2, r1, #0
	bl GetMonData
	str r0, [r6, #0x10]
	add r0, r5, #0
	mov r1, #0xc
	mov r2, #0
	bl GetMonData
	strb r0, [r6, #0x1f]
	add r0, r5, #0
	mov r1, #0xa
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x20
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #9
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x21
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #0xa1
	mov r2, #0
	bl GetMonData
	add r1, r6, #0
	add r1, #0x22
	strb r0, [r1]
	add r0, r5, #0
	mov r1, #0x75
	add r2, sp, #0x20
	bl GetMonData
	add r0, r6, #0
	add r0, #0x24
	add r1, sp, #0x20
	mov r2, #0xa
	bl CopyU16StringArrayN
	mov r4, #0
	str r4, [sp, #0x10]
	str r6, [sp, #8]
	add r7, r4, #0
_021F3500:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x36
	mov r2, #0
	bl GetMonData
	ldr r1, [sp, #8]
	mov r2, #0
	strh r0, [r1, #4]
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x3e
	bl GetMonData
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #0x10]
	add r4, r4, #1
	orr r0, r1
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	add r7, r7, #2
	add r0, r0, #2
	str r0, [sp, #8]
	cmp r4, #4
	blt _021F3500
	ldr r0, [sp, #0x10]
	mov r4, #0
	strb r0, [r6, #0x1e]
	str r4, [sp, #4]
	add r7, r4, #0
_021F353E:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x46
	mov r2, #0
	bl GetMonData
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #4]
	mov r2, #0
	orr r0, r1
	add r1, r4, #0
	str r0, [sp, #4]
	add r0, r5, #0
	add r1, #0xd
	bl GetMonData
	add r1, r6, r4
	add r4, r4, #1
	add r7, r7, #5
	strb r0, [r1, #0x18]
	cmp r4, #6
	blt _021F353E
	ldr r0, [sp, #4]
	str r0, [r6, #0x14]
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _021F357E
	add r0, r5, #0
	mov r1, #1
	bl Mon_UpdateShayminForm
_021F357E:
	ldr r0, [sp, #0x14]
	add r6, #0x38
	add r0, r0, #1
	str r0, [sp, #0x14]
	cmp r0, #6
	bge _021F3598
_021F358A:
	ldr r0, [sp, #0x1c]
	add r1, r0, #1
	ldr r0, [sp, #0x18]
	str r1, [sp, #0x1c]
	cmp r1, r0
	bge _021F3598
	b _021F33F6
_021F3598:
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F359C: .word 0xFFFFF800
_021F35A0: .word 0xFFFF07FF
	thumb_func_end PokewalkerApp_FillTrainerHouseParty

	thumb_func_start PokewalkerApp_CountTrainerHouseSets
PokewalkerApp_CountTrainerHouseSets: ; 0x021F35A4
	push {r3, r4, r5, r6, r7, lr}
	mov r4, #0
	mov r7, #6
	add r5, r0, #0
	add r6, r4, #0
	lsl r7, r7, #6
_021F35B0:
	add r0, r5, #0
	bl TrainerHouseSet_CheckHasData
	cmp r0, #0
	beq _021F35C4
	add r4, r4, #1
	add r5, r5, r7
	add r6, r6, #1
	cmp r4, #0xa
	blt _021F35B0
_021F35C4:
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end PokewalkerApp_CountTrainerHouseSets

	thumb_func_start PokewalkerApp_FindTrainerHouseSet
PokewalkerApp_FindTrainerHouseSet: ; 0x021F35C8
	push {r3, r4, r5, r6, r7, lr}
	mov r7, #6
	add r5, r0, #0
	add r6, r1, #0
	str r2, [sp]
	mov r4, #0
	lsl r7, r7, #6
_021F35D6:
	add r0, r5, #0
	bl TrainerHouseSet_CheckHasData
	cmp r0, #0
	beq _021F35F6
	add r0, r5, #0
	add r1, r6, #0
	bl TrainerHouseTrainer_Compare
	cmp r0, #0
	beq _021F35F6
	ldr r0, [sp]
	mov r1, #1
	str r1, [r0]
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
_021F35F6:
	add r4, r4, #1
	add r5, r5, r7
	cmp r4, #0xa
	blt _021F35D6
	ldr r1, [sp]
	mov r0, #0
	str r0, [r1]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokewalkerApp_FindTrainerHouseSet
