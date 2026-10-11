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

	thumb_func_start NintendoWfc_SetSpritePosition
NintendoWfc_SetSpritePosition: ; 0x02238688
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r0, #0
	add r5, r2, #0
	cmp r1, #0
	ble _022386A6
	lsl r0, r1, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _022386B4
_022386A6:
	lsl r0, r1, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_022386B4:
	bl _ffix
	str r0, [sp]
	cmp r5, #0
	ble _022386D0
	lsl r0, r5, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _022386DE
_022386D0:
	lsl r0, r5, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_022386DE:
	bl _ffix
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	add r0, r4, #0
	add r1, sp, #0
	bl Sprite_SetMatrix
	add sp, #0xc
	pop {r4, r5, pc}
	thumb_func_end NintendoWfc_SetSpritePosition


	thumb_func_start NintendoWfc_MovePadCursor
NintendoWfc_MovePadCursor: ; 0x022386F4
	push {r3, r4, r5, lr}
	add r4, r1, #0
	ldr r1, _02238728 ; =ov72_0223B3A4
	lsl r3, r4, #2
	ldr r2, _0223872C ; =ov72_0223B3A6
	ldrh r1, [r1, r3]
	ldrh r3, [r2, r3]
	mov r2, #1
	lsl r2, r2, #8
	add r2, r3, r2
	add r5, r0, #0
	bl NintendoWfc_SetSpritePosition
	cmp r4, #0xa
	bge _0223871C
	add r0, r5, #0
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	pop {r3, r4, r5, pc}
_0223871C:
	add r0, r5, #0
	mov r1, #2
	bl Sprite_SetAnimCtrlSeq
	pop {r3, r4, r5, pc}
	nop
_02238728: .word ov72_0223B3A4
_0223872C: .word ov72_0223B3A6
	thumb_func_end NintendoWfc_MovePadCursor


	thumb_func_start NintendoWfc_MoveDigitCursor
NintendoWfc_MoveDigitCursor: ; 0x02238730
	push {r4, lr}
	add r4, r2, #0
	mov r3, #0xc
	mul r4, r3
	ldr r3, _02238748 ; =ov72_0223B364
	lsl r2, r1, #2
	add r1, r3, r4
	ldrh r1, [r2, r1]
	ldr r2, _0223874C ; =0x00000129
	bl NintendoWfc_SetSpritePosition
	pop {r4, pc}
	.balign 4, 0
_02238748: .word ov72_0223B364
_0223874C: .word 0x00000129
	thumb_func_end NintendoWfc_MoveDigitCursor


	thumb_func_start NintendoWfc_SetDigitSpritePosition
NintendoWfc_SetDigitSpritePosition: ; 0x02238750
	push {r3, r4, r5, lr}
	add r3, r2, #0
	mov r5, #0xc
	lsl r4, r1, #2
	mul r3, r5
	ldr r2, _02238770 ; =ov72_0223B366
	ldr r1, _02238774 ; =ov72_0223B364
	add r2, r2, r3
	add r1, r1, r3
	ldrh r2, [r4, r2]
	add r5, #0xf4
	ldrh r1, [r4, r1]
	add r2, r2, r5
	bl NintendoWfc_SetSpritePosition
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02238770: .word ov72_0223B366
_02238774: .word ov72_0223B364
	thumb_func_end NintendoWfc_SetDigitSpritePosition


	thumb_func_start NintendoWfc_Alloc
NintendoWfc_Alloc: ; 0x02238778
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r4, r2, #0
	bl OS_DisableInterrupts
	add r6, r0, #0
	ldr r0, _0223879C ; =sNintendoWfcGlobal
	add r1, r5, #0
	ldr r0, [r0]
	add r2, r4, #0
	bl NNS_FndAllocFromExpHeapEx
	add r4, r0, #0
	add r0, r6, #0
	bl OS_RestoreInterrupts
	add r0, r4, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0223879C: .word sNintendoWfcGlobal
	thumb_func_end NintendoWfc_Alloc
