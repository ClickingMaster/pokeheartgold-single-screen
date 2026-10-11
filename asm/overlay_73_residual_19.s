#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

	.text
	.public sRecordMixingButtonX
	.public sRecordMixingStateFuncs
	.public _021EA940
	.public RecordMixing_Init
	.public RecordMixing_Main
	.public RecordMixing_Exit
	.public RecordMixing_VBlankTask
	.public RecordMixing_VBlankCB
	.public RecordMixing_SetGfxBanks
	.public RecordMixing_InitBgs
	.public RecordMixing_InitResources
	.public RecordMixing_FreeResources
	.public RecordMixing_LoadBgGraphics
	.public RecordMixing_InitObjTransfer
	.public RecordMixing_InitSpriteGraphics
	.public RecordMixing_CreateSprites
	.public RecordMixing_AddWindows
	.public RecordMixing_PrintCenteredString
	.public ov73_021E6444
	.public ov73_021E6484
	.public RecordMixing_UpdateMenuInputState
	.public RecordMixing_HandleInput
	.public ov73_021E66F4
	.public RecordMixing_State_WaitText
	.public ov73_021E6778
	.public RecordMixing_ShowYesNo
	.public RecordMixing_HideYesNo
	.public ov73_021E6890
	.public ov73_021E68AC
	.public ov73_021E69E8
	.public ov73_021E6A70
	.public ov73_021E6A90
	.public ov73_021E6AAC
	.public ov73_021E6B6C
	.public ov73_021E6BC8
	.public ov73_021E6C50
	.public ov73_021E6CC0
	.public ov73_021E6CDC
	.public ov73_021E6CFC
	.public ov73_021E6D38
	.public ov73_021E6D54
	.public ov73_021E6DF8
	.public ov73_021E6E48
	.public ov73_021E6E6C
	.public ov73_021E6E8C
	.public ov73_021E6ECC
	.public ov73_021E6EF8
	.public ov73_021E6F1C
	.public ov73_021E6FAC
	.public ov73_021E6FE8
	.public ov73_021E705C
	.public ov73_021E7120
	.public RecordMixing_SetButtonState
	.public RecordMixing_PrintButtons
	.public RecordMixing_PulsePalette
	.public RecordMixing_PrintMemberList
	.public RecordMixing_UpdateMembers
	.public RecordMixing_PrintMessage
	.public RecordMixing_UpdateMemberSprites
	.public RecordMixing_LoadAvatarGraphics
	.public RecordMixing_LoadAvatar
	.public ov73_021E781C
	.public ov73_021E7870
	.public RecordMixing_InsertGroup
	.public RecordMixing_RemoveGroup
	.public RecordMixing_MergeGroups
	.public ov73_021E7A8C
	.public ov73_021E7AC0
	.public ov73_021E7B3C
	.public ov73_021E7B84
	.public ov73_021E7BC8
	.public ov73_021E7C0C
	.public ov73_021E7C50
	.public ov73_021E7C94
	.public ov73_021E7CD8
	.public ov73_021E7D74
	.public ov73_021E7E24
	.public NintendoWfcUnused_Init
	.public NintendoWfcUnused_Main
	.public NintendoWfcUnused_Exit
	.public NintendoWfcUnused_VBlankCB
	.public NintendoWfcUnused_SetGfxBanks
	.public NintendoWfcUnused_InitObjTransfer
	.public NintendoWfcUnused_InitSpriteResources
	.public NintendoWfcUnused_CreateSprites
	.public NintendoWfcUnused_CreateYesNoPrompt
	.public NintendoWfcUnused_FreeGraphics
	.public NintendoWfcUnused_SessionInit
	.public NintendoWfcUnused_SessionMain
	.public NintendoWfcUnused_SessionExit
	.public NintendoWfcUnused_InitBgs
	.public NintendoWfcUnused_LoadBgGraphics
	.public NintendoWfcUnused_PaletteCycleTask
	.public NintendoWfcUnused_AddWindows
	.public NintendoWfcUnused_RemoveWindows
	.public NintendoWfcUnused_InitStrings
	.public NintendoWfcUnused_FreeStrings
	.public ov73_021E8CBC
	.public ov73_021E8D2C
	.public ov73_021E8DC8
	.public ov73_021E8E7C
	.public ov73_021E8ED8
	.public ov73_021E8FE4
	.public ov73_021E9144
	.public ov73_021E9224
	.public ov73_021E926C
	.public ov73_021E9378
	.public ov73_021E944C
	.public ov73_021E95CC
	.public ov73_021E9618
	.public ov73_021E9758
	.public ov73_021E97A8
	.public ov73_021E98AC
	.public ov73_021E98FC
	.public ov73_021E9A6C
	.public NintendoWfcUnused_StartExit
	.public NintendoWfcUnused_ShowHttpError
	.public NintendoWfcUnused_Disconnect
	.public NintendoWfcUnused_WaitMessageAndDelay
	.public NintendoWfcUnused_PrintFormattedMessage
	.public NintendoWfcUnused_ShowDwcError
	.public NintendoWfcUnused_WaitErrorDismiss
	.public NintendoWfcUnused_PrintMessage
	.public NintendoWfcUnused_GetAlignedTextX
	.public NintendoWfcUnused_PrintAlignedText
	.public NintendoWfcUnused_PrintAlignedTextSmall
	.public NintendoWfcUnused_PrintNumber
	.public NintendoWfcUnused_PrintErrorMessage
	.public NintendoWfcUnused_ShowErrorMessage
	.public RecordMixing_FriendGroup_Load
	.public RecordMixing_Fashion_Load
	.public RecordMixing_Fashion_Merge
	.public RecordMixing_Rankings_Merge
	.public RecordMixing_BuildSendBuffer
	.public RecordMixing_MergeReceivedRecords
	.public sRecordMixingAvatarVramOffsets
	.public sRecordMixingFlashDurations
	.public sRecordMixingFlashPalettes
	.public sRecordMixingButtonHitboxes
	.public sRecordMixingGraphicsModes
	.public sRecordMixingObjCharTransferTemplate
	.public sRecordMixingSpritePositions
	.public sRecordMixingBgTemplate_Main0
	.public sRecordMixingBgTemplate_Main1
	.public sRecordMixingBgTemplate_Sub0
	.public sRecordMixingBgTemplate_Sub1
	.public sRecordMixingGraphicsBanks
	.public ov73_021EA610
	.public ov73_021EA618
	.public ov73_021EA624
	.public ov73_021EA630
	.public ov73_021EA640
	.public ov73_021EA650
	.public ov73_021EA664
	.public sNintendoWfcUnusedArrowPositions
	.public sNintendoWfcUnusedGraphicsModes
	.public sNintendoWfcUnusedObjCharTransferTemplate
	.public sNintendoWfcUnusedGraphicsBanks
	.public sNintendoWfcUnusedBgTemplate_Sub1
	.public sNintendoWfcUnusedBgTemplate_Main1
	.public sNintendoWfcUnusedBgTemplate_Main0
	.public sNintendoWfcUnusedBgTemplate_Sub0
	.public sRecordMixingHandlers
	.public sNintendoWfcUnusedModeInit
	.public sNintendoWfcUnusedModeMain
	.public sNintendoWfcUnusedModeExit
	.public sNintendoWfcUnusedStates

	.public RecordMixing_FreeBgs
	.public RecordMixing_FreeButtonScreen
	.public RecordMixing_RemoveWindows
	.public ov73_021E66F0
	.public RecordMixing_SetStateAfterText
	.public ov73_021E69D0
	.public RecordMixing_StartFlash
	.public ov73_021E6BF8
	.public ov73_021E6C20
	.public ov73_021E7340
	.public ov73_021E7468
	.public RecordMixing_CountMembers
	.public ov73_021E7488
	.public RecordMixing_IsPrinterDone
	.public RecordMixing_ClearMessageWindow
	.public RecordMixing_FreeAvatarGraphics
	.public RecordMixing_GetTextSpeed
	.public RecordMixing_GetGroupRecordSize
	.public RecordMixing_GetGroupRecord
	.public ov73_021E7A74
	.public ov73_021E7A7C
	.public ov73_021E7A84
	.public ov73_021E7D54
	.public NintendoWfcUnused_InitArgs
	.public ov73_021E8164
	.public NintendoWfcUnused_InitSpriteTemplate
	.public NintendoWfcUnused_HandleYesNoPrompt
	.public NintendoWfcUnused_SetState
	.public NintendoWfcUnused_Alloc
	.public NintendoWfcUnused_Free
	.public NintendoWfcUnused_GetWifiIconLevel
	.public NintendoWfcUnused_SetNextMode
	.public NintendoWfcUnused_InitGraphics
	.public NintendoWfcUnused_FreeBgs
	.public ov73_021E8BFC
	.public ov73_021E8C2C
	.public ov73_021E8C8C
	.public ov73_021E8D0C
	.public ov73_021E8DB8
	.public ov73_021E8EBC
	.public ov73_021E8FB0
	.public ov73_021E9120
	.public ov73_021E91F4
	.public ov73_021E93EC
	.public ov73_021E941C
	.public ov73_021E955C
	.public ov73_021E9598
	.public ov73_021E96EC
	.public ov73_021E9728
	.public ov73_021E987C
	.public ov73_021E99D4
	.public ov73_021E9A3C
	.public ov73_021E9B58
	.public ov73_021E9B94
	.public NintendoWfcUnused_StartSave
	.public NintendoWfcUnused_Save
	.public ov73_021E9BD4
	.public ov73_021E9C04
	.public ov73_021E9C30
	.public ov73_021E9C60
	.public ov73_021E9C8C
	.public ov73_021E9CBC
	.public ov73_021E9CD8
	.public ov73_021E9D10
	.public ov73_021E9D44
	.public NintendoWfcUnused_WaitMessage
	.public NintendoWfcUnused_OpenYesNoPrompt
	.public NintendoWfcUnused_CreateWirelessIcon
	.public NintendoWfcUnused_SetArrowsVisible
	.public NintendoWfcUnused_ShowWaitingIcon
	.public NintendoWfcUnused_HideWaitingIcon
	.public RecordMixing_FriendGroup_GetSize
	.public RecordMixing_FriendGroup_Merge
	.public RecordMixing_Fashion_GetSize
	.public ov73_021EA2FC
	.public RecordMixing_Rankings_Load
	.public ov73_021EA348
	.public ov73_021EA370

	thumb_func_start NintendoWfcUnused_FreeGraphics
NintendoWfcUnused_FreeGraphics: ; 0x021E847C
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r0, _021E84C0 ; =0x00000D34
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeleteCharTransferTask
	ldr r0, _021E84C4 ; =0x00000D38
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeletePlttTransferTask
	ldr r7, _021E84C8 ; =0x00000D24
	mov r4, #0
	add r5, r6, #0
_021E8496:
	ldr r0, [r5, r7]
	bl Destroy2DGfxResObjMan
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #4
	blt _021E8496
	ldr r0, _021E84CC ; =0x00000BF8
	ldr r0, [r6, r0]
	bl SpriteList_Delete
	ldr r0, _021E84CC ; =0x00000BF8
	mov r1, #0
	str r1, [r6, r0]
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E84C0: .word 0x00000D34
_021E84C4: .word 0x00000D38
_021E84C8: .word 0x00000D24
_021E84CC: .word 0x00000BF8
	thumb_func_end NintendoWfcUnused_FreeGraphics


	thumb_func_start NintendoWfcUnused_SessionInit
NintendoWfcUnused_SessionInit: ; 0x021E84D0
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	bl NintendoWfcUnused_InitStrings
	mov r0, #8
	str r0, [sp]
	mov r1, #1
	str r1, [sp, #4]
	mov r0, #0x96
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldr r0, [r4, #4]
	bl NintendoWfcUnused_InitBgs
	add r0, r4, #0
	bl NintendoWfcUnused_LoadBgGraphics
	add r0, r4, #0
	bl NintendoWfcUnused_AddWindows
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
	ldr r0, [r0, #0xc]
	bl sub_02039418
	add r0, r4, #0
	bl NintendoWfcUnused_CreateWirelessIcon
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
	ldr r0, _021E85A0 ; =0x00000F0C
	mov r1, #0
	str r1, [r4, r0]
	bl ov00_021EC5B4
	cmp r0, #0
	bne _021E8594
	ldr r0, [r4]
	ldr r0, [r0, #0x24]
	cmp r0, #0
	beq _021E858E
	ldr r0, _021E85A4 ; =0x00000F0F
	ldr r1, _021E85A8 ; =0x00000BD8
	str r0, [sp]
	mov r2, #1
	ldr r1, [r4, r1]
	add r0, r4, #0
	add r3, r2, #0
	bl NintendoWfcUnused_PrintMessage
	add r0, r4, #0
	mov r1, #0x2f
	mov r2, #2
	bl NintendoWfcUnused_SetState
	add r0, r4, #0
	bl NintendoWfcUnused_ShowWaitingIcon
	b _021E8598
_021E858E:
	mov r0, #0
	str r0, [r4, #0x1c]
	b _021E8598
_021E8594:
	mov r0, #0x33
	str r0, [r4, #0x1c]
_021E8598:
	mov r0, #2
	add sp, #0xc
	pop {r3, r4, pc}
	nop
_021E85A0: .word 0x00000F0C
_021E85A4: .word 0x00000F0F
_021E85A8: .word 0x00000BD8
	thumb_func_end NintendoWfcUnused_SessionInit


	thumb_func_start NintendoWfcUnused_SessionMain
NintendoWfcUnused_SessionMain: ; 0x021E85AC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl NintendoWfcUnused_GetWifiIconLevel
	bl WirelessStatusIcon_SetLevel
	ldr r4, [r5, #0x1c]
	ldr r1, _021E85D8 ; =sNintendoWfcUnusedStates
	lsl r2, r4, #2
	ldr r1, [r1, r2]
	add r0, r5, #0
	blx r1
	ldr r1, [r5, #0x1c]
	cmp r4, r1
	beq _021E85D6
	mov r1, #0xf9
	mov r2, #0
	lsl r1, r1, #4
	strh r2, [r5, r1]
	add r1, r1, #2
	strh r2, [r5, r1]
_021E85D6:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_021E85D8: .word sNintendoWfcUnusedStates
	thumb_func_end NintendoWfcUnused_SessionMain


	thumb_func_start NintendoWfcUnused_SessionExit
NintendoWfcUnused_SessionExit: ; 0x021E85DC
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _021E8620 ; =0x00000F98
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _021E85F6
	bl SysTask_Destroy
	ldr r0, _021E8620 ; =0x00000F98
	mov r1, #0
	str r1, [r4, r0]
	add r0, r0, #4
	str r1, [r4, r0]
_021E85F6:
	add r0, r4, #0
	bl NintendoWfcUnused_FreeStrings
	add r0, r4, #0
	bl NintendoWfcUnused_RemoveWindows
	ldr r0, [r4, #4]
	bl NintendoWfcUnused_FreeBgs
	ldr r0, _021E8624 ; =0x00000F9C
	mov r1, #0
	str r1, [r4, r0]
	ldr r0, [r4, #0x14]
	str r0, [r4, #0x10]
	cmp r0, #0
	bne _021E861A
	mov r0, #5
	pop {r4, pc}
_021E861A:
	mov r0, #1
	pop {r4, pc}
	nop
_021E8620: .word 0x00000F98
_021E8624: .word 0x00000F9C
	thumb_func_end NintendoWfcUnused_SessionExit


	thumb_func_start NintendoWfcUnused_InitBgs
NintendoWfcUnused_InitBgs: ; 0x021E8628
	push {r3, r4, r5, lr}
	sub sp, #0x70
	ldr r5, _021E86FC ; =sNintendoWfcUnusedBgTemplate_Main0
	add r4, r0, #0
	ldmia r5!, {r0, r1}
	add r3, sp, #0x54
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
	ldr r5, _021E8700 ; =sNintendoWfcUnusedBgTemplate_Main1
	add r3, sp, #0x38
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
	ldr r5, _021E8704 ; =sNintendoWfcUnusedBgTemplate_Sub0
	add r3, sp, #0x1c
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
	ldr r5, _021E8708 ; =sNintendoWfcUnusedBgTemplate_Sub1
	add r3, sp, #0
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
	mov r0, #0
	mov r1, #0x20
	add r2, r0, #0
	mov r3, #0x96
	bl BG_ClearCharDataRange
	mov r0, #4
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x96
	bl BG_ClearCharDataRange
	add sp, #0x70
	pop {r3, r4, r5, pc}
	nop
_021E86FC: .word sNintendoWfcUnusedBgTemplate_Main0
_021E8700: .word sNintendoWfcUnusedBgTemplate_Main1
_021E8704: .word sNintendoWfcUnusedBgTemplate_Sub0
_021E8708: .word sNintendoWfcUnusedBgTemplate_Sub1
	thumb_func_end NintendoWfcUnused_InitBgs
