	.include "asm/macros.inc"
	.include "overlay_72.inc"
	.include "global.inc"

	.text
	.public NintendoWfc_Exit
	.public NintendoWfc_Init
	.public NintendoWfc_Main
	.public _0223B330
	.public sNintendoWfcRoomNumUrl
	.public sNintendoWfcHttp
	.public NintendoWfcHttp_Update
	.public NintendoWfcHttp_IsDone
	.public NintendoWfcHttp_RequestRoomCount
	.public NintendoWfcHttp_RequestDownload
	.public NintendoWfcHttp_RequestUpload
	.public NintendoWfcHttp_RequestInfo
	.public NintendoWfcHttp_RequestSetProfile
	.public NintendoWfcHttp_StartRequest
	.public NintendoWfcHttp_ConvertError
	.public NintendoWfc_VBlankCB
	.public NintendoWfc_SetGfxBanks
	.public NintendoWfc_InitObjTransfer
	.public NintendoWfc_InitSpriteResources
	.public NintendoWfc_CreateSprites
	.public NintendoWfc_SetSpritePosition
	.public NintendoWfc_MovePadCursor
	.public NintendoWfc_MoveDigitCursor
	.public NintendoWfc_SetDigitSpritePosition
	.public NintendoWfc_Alloc
	.public NintendoWfc_InitGraphics
	.public NintendoWfc_FreeGraphics
	.public NintendoWfc_SessionInit
	.public NintendoWfc_SessionMain
	.public NintendoWfc_SessionExit
	.public NintendoWfc_InitBgs
	.public NintendoWfc_LoadSubScreenBg
	.public NintendoWfc_LoadBgGraphics
	.public NintendoWfc_PaletteCycleTask
	.public NintendoWfc_AddWindows
	.public NintendoWfc_InitStrings
	.public ov72_02239108
	.public ov72_02239198
	.public ov72_022391E8
	.public ov72_02239220
	.public ov72_022392BC
	.public ov72_02239370
	.public ov72_022393CC
	.public ov72_022394A4
	.public ov72_022394D8
	.public ov72_02239638
	.public ov72_022396E8
	.public ov72_02239760
	.public ov72_02239784
	.public ov72_022397E8
	.public ov72_022398BC
	.public ov72_02239A3C
	.public ov72_02239A88
	.public ov72_02239BEC
	.public ov72_02239C14
	.public ov72_02239CB4
	.public ov72_02239CD8
	.public ov72_02239D6C
	.public ov72_02239E58
	.public ov72_02239F04
	.public ov72_02239F60
	.public ov72_02239F8C
	.public ov72_02239FD8
	.public NintendoWfc_StartExit
	.public ov72_0223A044
	.public NintendoWfc_ShowHttpError
	.public NintendoWfc_Disconnect
	.public NintendoWfc_WaitMessage
	.public NintendoWfc_WaitMessageAndDelay
	.public NintendoWfc_OpenYesNoMenu
	.public NintendoWfc_PrintFormattedMessage
	.public NintendoWfc_ShowDwcError
	.public NintendoWfc_WaitErrorDismiss
	.public NintendoWfc_PrintMessage
	.public NintendoWfc_GetAlignedTextX
	.public NintendoWfc_PrintAlignedText
	.public NintendoWfc_ShowWaitingIcon
	.public NintendoWfc_PrintErrorMessage
	.public NintendoWfc_ShowErrorMessage
	.public NintendoWfc_StartNumberInput
	.public NintendoWfc_RunNumberInput
	.public NintendoWfc_NumberInput_FadeOut
	.public NintendoWfc_NumberInput_Setup
	.public NintendoWfc_ShowNumberInputSprites
	.public NintendoWfc_GetTouchedPadKey
	.public NintendoWfc_GetPadNeighbor
	.public NintendoWfc_GetEnteredNumber
	.public NintendoWfc_AreDigitsComplete
	.public NintendoWfc_PressPadKey
	.public NintendoWfc_HandlePadButtons
	.public NintendoWfc_NumberInput_Main
	.public NintendoWfc_UpdateDigitSprites
	.public NintendoWfc_NumberInput_StartFadeOut
	.public NintendoWfc_NumberInput_Teardown
	.public NintendoWfc_AddNumberInputWindows
	.public NintendoWfc_PrintButtonLabel
	.public NintendoWfc_RemoveNumberInputWindows
	.public NintendoWfc_HideNumberInput
	.public NintendoWfc_SetButtonFlashPltt
	.public NintendoWfc_UpdateButtonFlash
	.public NintendoWfc_StartButtonFlash
	.public NintendoWfcYesNo_Init
	.public NintendoWfcYesNo_Run
	.public NintendoWfcYesNo_HandleInput
	.public NintendoWfcYesNo_WaitClose
	.public NintendoWfcYesNo_LoadGraphics
	.public NintendoWfcYesNo_AddWindows
	.public NintendoWfcYesNo_RemoveWindows
	.public NintendoWfcYesNo_MoveCursor
	.public ov72_0223B338
	.public sNintendoWfcObjCharTransferTemplate
	.public sNintendoWfcGraphicsModes
	.public ov72_0223B364
	.public ov72_0223B366
	.public sNintendoWfcGraphicsBanks
	.public ov72_0223B3A4
	.public ov72_0223B3A6
	.public sNintendoWfcBgTemplate_Sub1
	.public sNintendoWfcBgTemplate_Sub2
	.public sNintendoWfcBgTemplate_Main1
	.public sNintendoWfcBgTemplate_Main0
	.public sNintendoWfcBgTemplate_Sub0
	.public ov72_0223B464
	.public ov72_0223B466
	.public ov72_0223B468
	.public ov72_0223B46A
	.public ov72_0223B46C
	.public ov72_0223B478
	.public ov72_0223B479
	.public ov72_0223B48C
	.public ov72_0223B4C0
	.public ov72_0223B4C2
	.public ov72_0223B4C8
	.public sNintendoWfcDownloadUrl
	.public sNintendoWfcUploadUrl
	.public sNintendoWfcInfoUrl
	.public sNintendoWfcSetProfileUrl
	.public sNintendoWfcModeInit
	.public sNintendoWfcModeMain
	.public sNintendoWfcModeExit
	.public sNintendoWfcStates
	.public sNintendoWfcNumberInputStates
	.public ov72_0223B774
	.public ov72_0223B7B8
	.public sNintendoWfcYesNoStates
	.public ov72_0223B82C
	.public ov72_0223B834
	.public ov72_0223B900
	.public ov72_0223B91B
	.public ov72_0223B920
	.public ov72_0223B923
	.public sNintendoWfcGlobal

	.public NintendoWfcHttp_Init
	.public NintendoWfcHttp_GetResult
	.public NintendoWfc_InitArgs
	.public ov72_02238160
	.public NintendoWfc_InitSpriteTemplate
	.public NintendoWfc_HandleYesNoPrompt
	.public NintendoWfc_SetState
	.public NintendoWfc_Free
	.public NintendoWfc_GetWifiIconLevel
	.public NintendoWfc_SetNextMode
	.public NintendoWfc_FreeBgs
	.public NintendoWfc_RemoveWindows
	.public NintendoWfc_FreeStrings
	.public ov72_022390D8
	.public ov72_02239168
	.public ov72_022392AC
	.public ov72_022393B0
	.public ov72_02239614
	.public ov72_0223985C
	.public ov72_0223988C
	.public ov72_022399CC
	.public ov72_02239A08
	.public ov72_02239B5C
	.public ov72_02239B98
	.public ov72_02239BC8
	.public ov72_02239C8C
	.public ov72_02239D3C
	.public ov72_02239E94
	.public NintendoWfc_StartSave
	.public NintendoWfc_Save
	.public ov72_02239ED4
	.public ov72_02239F30
	.public ov72_02239FBC
	.public ov72_0223A010
	.public ov72_0223A414
	.public ov72_0223A41C
	.public NintendoWfc_HideWaitingIcon
	.public NintendoWfc_NumberInput_WaitFadeIn
	.public NintendoWfc_GetCursorAfterDigit
	.public ov72_0223ABF4
	.public ov72_0223ABF8
	.public ov72_0223ABFC
	.public NintendoWfc_NumberInput_WaitButtonFlash
	.public NintendoWfc_NumberInput_WaitFadeOut
	.public NintendoWfc_NumberInput_Finish
	.public NintendoWfcYesNo_Start
	.public ov72_0223B084
	.public NintendoWfcYesNo_ShowCursor

	thumb_func_start NintendoWfc_InitGraphics
NintendoWfc_InitGraphics: ; 0x022387D8
	push {r4, lr}
	add r4, r0, #0
	bl NintendoWfc_SetGfxBanks
	bl NintendoWfc_InitObjTransfer
	add r0, r4, #0
	bl NintendoWfc_InitSpriteResources
	add r0, r4, #0
	bl NintendoWfc_CreateSprites
	ldr r0, _022387FC ; =NintendoWfc_VBlankCB
	add r1, r4, #0
	bl Main_SetVBlankIntrCB
	pop {r4, pc}
	nop
_022387FC: .word NintendoWfc_VBlankCB
	thumb_func_end NintendoWfc_InitGraphics


	thumb_func_start NintendoWfc_FreeGraphics
NintendoWfc_FreeGraphics: ; 0x02238800
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r0, _02238854 ; =0x00000D34
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeleteCharTransferTask
	ldr r0, _02238858 ; =0x00000D44
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeleteCharTransferTask
	ldr r0, _0223885C ; =0x00000D38
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeletePlttTransferTask
	ldr r0, _02238860 ; =0x00000D48
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeletePlttTransferTask
	ldr r7, _02238864 ; =0x00000D24
	mov r4, #0
	add r5, r6, #0
_0223882A:
	ldr r0, [r5, r7]
	bl Destroy2DGfxResObjMan
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #4
	blt _0223882A
	ldr r0, _02238868 ; =0x00000BF8
	ldr r0, [r6, r0]
	bl SpriteList_Delete
	ldr r0, _02238868 ; =0x00000BF8
	mov r1, #0
	str r1, [r6, r0]
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02238854: .word 0x00000D34
_02238858: .word 0x00000D44
_0223885C: .word 0x00000D38
_02238860: .word 0x00000D48
_02238864: .word 0x00000D24
_02238868: .word 0x00000BF8
	thumb_func_end NintendoWfc_FreeGraphics


	thumb_func_start NintendoWfc_SessionInit
NintendoWfc_SessionInit: ; 0x0223886C
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	bl NintendoWfc_InitStrings
	mov r0, #8
	str r0, [sp]
	mov r1, #1
	str r1, [sp, #4]
	mov r0, #0x43
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldr r0, [r4, #4]
	bl NintendoWfc_InitBgs
	add r0, r4, #0
	bl NintendoWfc_LoadBgGraphics
	add r0, r4, #0
	bl NintendoWfc_AddWindows
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineBTogglePlanes
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r4]
	mov r1, #0
	ldr r0, [r0]
	bl FrontierData_WifiRankAction
	add r1, r4, #0
	add r1, #0x90
	str r0, [r1]
	add r0, r4, #0
	add r0, #0x90
	ldr r1, [r0]
	add r0, r4, #0
	add r0, #0x94
	str r1, [r0]
	add r0, r4, #0
	mov r1, #1
	add r0, #0x9c
	str r1, [r0]
	ldr r0, _02238940 ; =0x00000F4C
	mov r1, #0
	str r1, [r4, r0]
	bl ov00_021EC5B4
	cmp r0, #0
	bne _0223892E
	ldr r1, [r4]
	ldr r0, [r1, #0x24]
	cmp r0, #0
	beq _02238922
	ldr r0, [r1, #0xc]
	bl CommState_InitWfcSession
	ldr r0, _02238944 ; =0x00000F0F
	mov r2, #1
	str r0, [sp]
	ldr r1, _02238948 ; =0x00000BD8
	add r0, r4, #0
	ldr r1, [r4, r1]
	add r3, r2, #0
	bl NintendoWfc_PrintMessage
	add r0, r4, #0
	mov r1, #0x2f
	mov r2, #2
	bl NintendoWfc_SetState
	add r0, r4, #0
	bl NintendoWfc_ShowWaitingIcon
	b _0223893A
_02238922:
	mov r0, #4
	bl Sys_ClearSleepDisableFlag
	mov r0, #0
	str r0, [r4, #0x1c]
	b _0223893A
_0223892E:
	ldr r0, [r4]
	ldr r0, [r0, #0xc]
	bl CommState_InitWfcSession
	mov r0, #0x33
	str r0, [r4, #0x1c]
_0223893A:
	mov r0, #2
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
_02238940: .word 0x00000F4C
_02238944: .word 0x00000F0F
_02238948: .word 0x00000BD8
	thumb_func_end NintendoWfc_SessionInit


	thumb_func_start NintendoWfc_SessionMain
NintendoWfc_SessionMain: ; 0x0223894C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl NintendoWfc_GetWifiIconLevel
	bl WirelessStatusIcon_SetLevel
	ldr r4, [r5, #0x1c]
	ldr r1, _02238978 ; =sNintendoWfcStates
	lsl r2, r4, #2
	ldr r1, [r1, r2]
	add r0, r5, #0
	blx r1
	ldr r1, [r5, #0x1c]
	cmp r4, r1
	beq _02238976
	mov r1, #0xfd
	mov r2, #0
	lsl r1, r1, #4
	strh r2, [r5, r1]
	add r1, r1, #2
	strh r2, [r5, r1]
_02238976:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02238978: .word sNintendoWfcStates
	thumb_func_end NintendoWfc_SessionMain


	thumb_func_start NintendoWfc_SessionExit
NintendoWfc_SessionExit: ; 0x0223897C
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _022389C0 ; =0x00000FD8
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02238996
	bl SysTask_Destroy
	ldr r0, _022389C0 ; =0x00000FD8
	mov r1, #0
	str r1, [r4, r0]
	add r0, r0, #4
	str r1, [r4, r0]
_02238996:
	add r0, r4, #0
	bl NintendoWfc_FreeStrings
	add r0, r4, #0
	bl NintendoWfc_RemoveWindows
	ldr r0, [r4, #4]
	bl NintendoWfc_FreeBgs
	ldr r0, _022389C4 ; =0x00000FDC
	mov r1, #0
	str r1, [r4, r0]
	ldr r0, [r4, #0x14]
	str r0, [r4, #0x10]
	cmp r0, #0
	bne _022389BA
	mov r0, #5
	pop {r4, pc}
_022389BA:
	mov r0, #1
	pop {r4, pc}
	nop
_022389C0: .word 0x00000FD8
_022389C4: .word 0x00000FDC
	thumb_func_end NintendoWfc_SessionExit


	thumb_func_start NintendoWfc_InitBgs
NintendoWfc_InitBgs: ; 0x022389C8
	push {r4, r5, lr}
	sub sp, #0x8c
	ldr r5, _02238AD8 ; =sNintendoWfcBgTemplate_Main0
	add r4, r0, #0
	ldmia r5!, {r0, r1}
	add r3, sp, #0x70
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #0
	str r0, [r3]
	add r0, r4, #0
	add r3, r1, #0
	bl InitBgFromTemplate
	mov r0, #1
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02238ADC ; =sNintendoWfcBgTemplate_Main1
	add r3, sp, #0x54
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #1
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #2
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r5, _02238AE0 ; =sNintendoWfcBgTemplate_Sub0
	add r3, sp, #0x38
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #4
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #1
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	add r0, r4, #0
	mov r1, #4
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02238AE4 ; =sNintendoWfcBgTemplate_Sub1
	add r3, sp, #0x1c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #5
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #2
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	ldr r5, _02238AE8 ; =sNintendoWfcBgTemplate_Sub2
	add r3, sp, #0
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #6
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	add r0, r4, #0
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	mov r0, #0
	mov r1, #0x20
	add r2, r0, #0
	mov r3, #0x43
	bl BG_ClearCharDataRange
	mov r0, #4
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x43
	bl BG_ClearCharDataRange
	mov r0, #6
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x43
	bl BG_ClearCharDataRange
	add sp, #0x8c
	pop {r4, r5, pc}
	nop
_02238AD8: .word sNintendoWfcBgTemplate_Main0
_02238ADC: .word sNintendoWfcBgTemplate_Main1
_02238AE0: .word sNintendoWfcBgTemplate_Sub0
_02238AE4: .word sNintendoWfcBgTemplate_Sub1
_02238AE8: .word sNintendoWfcBgTemplate_Sub2
	thumb_func_end NintendoWfc_InitBgs
