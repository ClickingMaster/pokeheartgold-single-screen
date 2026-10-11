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

	thumb_func_start ov72_022393CC
ov72_022393CC: ; 0x022393CC
	push {r4, lr}
	add r4, r0, #0
	bl NintendoWfcHttp_IsDone
	cmp r0, #0
	beq _02239484
	bl NintendoWfcHttp_GetResult
	ldr r1, _022394A0 ; =0x00000FD4
	mov r2, #0
	str r2, [r4, r1]
	add r2, r0, #7
	cmp r2, #9
	bhi _02239478
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_022393F4: ; jump table
	.short _02239458 - _022393F4 - 2 ; case 0
	.short _02239468 - _022393F4 - 2 ; case 1
	.short _02239478 - _022393F4 - 2 ; case 2
	.short _02239458 - _022393F4 - 2 ; case 3
	.short _02239478 - _022393F4 - 2 ; case 4
	.short _02239468 - _022393F4 - 2 ; case 5
	.short _02239448 - _022393F4 - 2 ; case 6
	.short _02239408 - _022393F4 - 2 ; case 7
	.short _02239438 - _022393F4 - 2 ; case 8
	.short _02239448 - _022393F4 - 2 ; case 9
_02239408:
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	cmp r0, #0
	beq _02239420
	cmp r0, #1
	beq _0223942C
	cmp r0, #2
	beq _02239432
	b _0223949A
_02239420:
	add r0, r4, #0
	mov r1, #0xb
	mov r2, #0xd
	bl NintendoWfc_SetState
	b _0223949A
_0223942C:
	mov r0, #0x19
	str r0, [r4, #0x1c]
	b _0223949A
_02239432:
	mov r0, #0x1d
	str r0, [r4, #0x1c]
	b _0223949A
_02239438:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _0223949A
_02239448:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _0223949A
_02239458:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _0223949A
_02239468:
	sub r1, #0x78
	str r0, [r4, r1]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _0223949A
_02239478:
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	bl sub_020399EC
	b _0223949A
_02239484:
	ldr r0, _022394A0 ; =0x00000FD4
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	mov r0, #0xe1
	lsl r0, r0, #4
	cmp r1, r0
	bne _0223949A
	bl sub_020399EC
_0223949A:
	mov r0, #3
	pop {r4, pc}
	nop
_022394A0: .word 0x00000FD4
	thumb_func_end ov72_022393CC


	thumb_func_start ov72_022394A4
ov72_022394A4: ; 0x022394A4
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4]
	ldr r1, _022394D0 ; =0x00000F64
	ldr r0, [r0, #0xc]
	add r1, r4, r1
	bl sub_0203189C
	ldr r1, _022394D0 ; =0x00000F64
	add r0, r4, r1
	add r1, #0x64
	add r1, r4, r1
	bl NintendoWfcHttp_RequestSetProfile
	mov r0, #0xa
	str r0, [r4, #0x1c]
	ldr r0, _022394D4 ; =0x00000FD4
	mov r1, #0
	str r1, [r4, r0]
	mov r0, #3
	pop {r4, pc}
	nop
_022394D0: .word 0x00000F64
_022394D4: .word 0x00000FD4
	thumb_func_end ov72_022394A4


	thumb_func_start ov72_022394D8
ov72_022394D8: ; 0x022394D8
	push {r3, r4, r5, lr}
	add r4, r0, #0
	bl NintendoWfcHttp_IsDone
	cmp r0, #0
	bne _022394E6
	b _022395EE
_022394E6:
	bl NintendoWfcHttp_GetResult
	add r5, r0, #0
	ldr r0, _02239608 ; =0x00000FD4
	mov r1, #0
	str r1, [r4, r0]
	add r1, r5, #7
	cmp r1, #9
	bhi _022395E2
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02239504: ; jump table
	.short _022395C2 - _02239504 - 2 ; case 0
	.short _022395D2 - _02239504 - 2 ; case 1
	.short _022395E2 - _02239504 - 2 ; case 2
	.short _022395C2 - _02239504 - 2 ; case 3
	.short _022395E2 - _02239504 - 2 ; case 4
	.short _022395D2 - _02239504 - 2 ; case 5
	.short _022395B2 - _02239504 - 2 ; case 6
	.short _02239518 - _02239504 - 2 ; case 7
	.short _022395A2 - _02239504 - 2 ; case 8
	.short _022395B2 - _02239504 - 2 ; case 9
_02239518:
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	ldr r0, _0223960C ; =0x00000FC8
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _02239530
	cmp r1, #1
	beq _02239586
	cmp r1, #2
	beq _02239586
	b _02239596
_02239530:
	add r1, r0, #4
	ldr r1, [r4, r1]
	cmp r1, #3
	bhi _02239604
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02239544: ; jump table
	.short _0223954C - _02239544 - 2 ; case 0
	.short _02239576 - _02239544 - 2 ; case 1
	.short _02239576 - _02239544 - 2 ; case 2
	.short _02239576 - _02239544 - 2 ; case 3
_0223954C:
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	cmp r0, #0
	beq _0223955E
	cmp r0, #1
	beq _0223956A
	cmp r0, #2
	beq _02239570
	b _02239604
_0223955E:
	add r0, r4, #0
	mov r1, #0xb
	mov r2, #0xd
	bl NintendoWfc_SetState
	b _02239604
_0223956A:
	mov r0, #0x19
	str r0, [r4, #0x1c]
	b _02239604
_02239570:
	mov r0, #0x1d
	str r0, [r4, #0x1c]
	b _02239604
_02239576:
	sub r0, #0x6c
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_02239586:
	ldr r0, _02239610 ; =0x00000F5C
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_02239596:
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	bl sub_020399EC
	b _02239604
_022395A2:
	sub r0, #0x78
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_022395B2:
	sub r0, #0x78
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_022395C2:
	sub r0, #0x78
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_022395D2:
	sub r0, #0x78
	str r5, [r4, r0]
	mov r0, #0x35
	str r0, [r4, #0x1c]
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	b _02239604
_022395E2:
	add r0, r4, #0
	bl NintendoWfc_HideWaitingIcon
	bl sub_020399EC
	b _02239604
_022395EE:
	ldr r0, _02239608 ; =0x00000FD4
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	mov r0, #0xe1
	lsl r0, r0, #4
	cmp r1, r0
	bne _02239604
	bl sub_020399EC
_02239604:
	mov r0, #3
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02239608: .word 0x00000FD4
_0223960C: .word 0x00000FC8
_02239610: .word 0x00000F5C
	thumb_func_end ov72_022394D8
