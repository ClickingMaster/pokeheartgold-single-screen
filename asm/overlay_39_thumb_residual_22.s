#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_39_thumb.inc"
	.include "global.inc"

	.text
	.public sGlobalTerminalNetOsHeap
	.public GlobalTerminalNet_PackBoxData
	.public ov39_022271CC
	.public GlobalTerminalNet_Init
	.public GlobalTerminalNet_Free
	.public GlobalTerminalNet_RequestDressUpUpload
	.public GlobalTerminalNet_RequestBoxUpload
	.public GlobalTerminalNet_RequestRankings
	.public GlobalTerminalNet_RequestVideoUpload
	.public GlobalTerminalNet_RequestVideoSearch
	.public ov39_022275E8
	.public ov39_02227648
	.public GlobalTerminalNet_Update
	.public GlobalTerminalNet_SendPendingRequest
	.public GlobalTerminalNet_PollStatus
	.public GlobalTerminalNet_SaveUploadedVideoNumber
	.public GlobalTerminalNet_HandleResponse
	.public GlobalTerminalNet_ShowErrorMessage
	.public GlobalTerminalNet_GetSequence
	.public GlobalTerminalNet_CreateOsHeap
	.public GlobalTerminalNet_GetDressUpList
	.public GlobalTerminalNet_GetBoxList
	.public GlobalTerminalNet_GetUploadedVideoNumber
	.public GlobalTerminalNet_GetVideoList
	.public GlobalTerminalNet_GetDownloadedVideo
	.public GlobalTerminalConnect_Init
	.public GlobalTerminalConnect_Main
	.public GlobalTerminalConnect_Exit
	.public GlobalTerminalConnect_VBlankCB
	.public GlobalTerminalConnect_InitBgs
	.public GlobalTerminalConnect_LoadGraphics
	.public GlobalTerminalConnect_PaletteCycleTask
	.public GlobalTerminalConnect_CreateWindows
	.public GlobalTerminalConnect_GetAlignedX
	.public GlobalTerminalConnect_PrintAligned
	.public GlobalTerminalConnect_ShowErrorWindow
	.public GlobalTerminalConnect_ShowErrorCode
	.public GlobalTerminalConnect_CreateYesNoPrompt
	.public GlobalTerminalConnect_PrintMessage
	.public GlobalTerminalConnect_StateAskConnect
	.public GlobalTerminalConnect_StateStartConnect
	.public GlobalTerminalConnect_StateWaitConnect
	.public ov39_02228DA0
	.public GlobalTerminalConnect_StateSaveProfileId
	.public GlobalTerminalConnect_StateWaitErrorAck
	.public GlobalTerminalConnect_StateExit
	.public GlobalTerminalConnect_StateShowServerError
	.public GlobalTerminalConnect_StateServerErrorDisconnect
	.public GlobalTerminalConnect_StateDisconnect
	.public GlobalTerminalNet_DressUpToUploadForm
	.public GlobalTerminalNet_UploadFormToDressUp
	.public sGlobalTerminalNetNoSequence
	.public sGlobalTerminalNetSequences
	.public sGlobalTerminalNetErrorSequence
	.public sGlobalTerminalConnectGraphicsModes
	.public sGlobalTerminalConnectGraphicsBanks
	.public sGlobalTerminalConnectMainBgTemplates
	.public sGlobalTerminalConnectSubBgTemplates
	.public sGlobalTerminalConnectStates
	.public GlobalTerminalNet_PackDressUpPhoto
	.public GlobalTerminalNet_UnpackDressUpPhoto
	.public GlobalTerminalNet_SealBoxData
	.public GlobalTerminalNet_SetRequestHeader
	.public GlobalTerminalNet_CopyRequestHeader
	.public GlobalTerminalNet_SeqStopClient
	.public GlobalTerminalNet_SeqDisconnect
	.public GlobalTerminalNet_SeqShowError
	.public GlobalTerminalNet_SeqWaitButton
	.public GlobalTerminalNet_RequestDressUpDownload
	.public GlobalTerminalNet_RequestBoxDownload
	.public ov39_022274B4
	.public ov39_022276A4
	.public ov39_022276E0
	.public GlobalTerminalNet_RequestVideoDownload
	.public ov39_0222774C
	.public ov39_02227B1C
	.public ov39_02227B20
	.public ov39_02227B50
	.public ov39_02227B54
	.public ov39_02227B58
	.public GlobalTerminalNet_GetError
	.public GlobalTerminalNet_PrintMessage
	.public GlobalTerminalNet_IsIdle
	.public GlobalTerminalNet_ClearOsHeap
	.public GlobalTerminalNet_CheckDressUpUploadResult
	.public GlobalTerminalNet_CheckDressUpDownloadResult
	.public GlobalTerminalNet_CheckBoxUploadResult
	.public GlobalTerminalNet_CheckBoxDownloadResult
	.public GlobalTerminalNet_Check22000Result
	.public ov39_02227F74
	.public GlobalTerminalNet_CheckRankingsResult
	.public GlobalTerminalNet_GetRankings
	.public GlobalTerminalNet_CheckVideoUploadResult
	.public GlobalTerminalNet_CheckVideoSearchResult
	.public GlobalTerminalNet_CheckVideoDownloadResult
	.public GlobalTerminalNet_Check23003Result
	.public GlobalTerminalConnect_FreeBgs
	.public GlobalTerminalConnect_RemoveWindows
	.public GlobalTerminalConnect_IsPrinting
	.public GlobalTerminalConnect_ShowWaitingIcon
	.public GlobalTerminalConnect_HideWaitingIcon
	.public GlobalTerminalConnect_SetStateAndNext
	.public GlobalTerminalConnect_ErrorAllowsRetry
	.public ov39_02228D90
	.public GlobalTerminalConnect_StateShowError
	.public GlobalTerminalConnect_StateWaitMessage

	thumb_func_start GlobalTerminalConnect_StateWaitErrorAck
GlobalTerminalConnect_StateWaitErrorAck: ; 0x02228EC8
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _02228F14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	bne _02228EE4
	mov r0, #2
	tst r0, r1
	bne _02228EE4
	bl System_GetTouchNew
	cmp r0, #1
	bne _02228F0E
_02228EE4:
	add r0, r4, #0
	add r0, #0x64
	mov r1, #0
	bl EraseFrameAndWindow1
	add r0, r4, #0
	mov r1, #0
	add r0, #0x94
	str r1, [r0]
	ldr r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	neg r0, r0
	bl GlobalTerminalConnect_ErrorAllowsRetry
	cmp r0, #1
	bne _02228F0A
	mov r0, #0
	str r0, [r4, #8]
	b _02228F0E
_02228F0A:
	mov r0, #8
	str r0, [r4, #8]
_02228F0E:
	mov r0, #0
	pop {r4, pc}
	nop
_02228F14: .word gSystem
	thumb_func_end GlobalTerminalConnect_StateWaitErrorAck


	thumb_func_start GlobalTerminalConnect_StateExit
GlobalTerminalConnect_StateExit: ; 0x02228F18
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #9
	beq _02228F28
	bl CommState_FreeWfcSession
_02228F28:
	bl WirelessStatusIcon_Destroy
	add r0, r4, #0
	bl GlobalTerminalConnect_HideWaitingIcon
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x7c
	str r0, [sp, #8]
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	mov r0, #0
	str r0, [r4, #8]
	mov r0, #1
	add sp, #0xc
	pop {r3, r4, pc}
	thumb_func_end GlobalTerminalConnect_StateExit


	thumb_func_start GlobalTerminalConnect_StateShowServerError
GlobalTerminalConnect_StateShowServerError: ; 0x02228F54
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	ldr r0, [r4, #0x10]
	mov r2, #0
	add r0, #0xf
	cmp r0, #0x11
	bhi _02228FA2
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02228F70: ; jump table
	.short _02228FA0 - _02228F70 - 2 ; case 0
	.short _02228F9C - _02228F70 - 2 ; case 1
	.short _02228FA2 - _02228F70 - 2 ; case 2
	.short _02228FA0 - _02228F70 - 2 ; case 3
	.short _02228FA2 - _02228F70 - 2 ; case 4
	.short _02228FA2 - _02228F70 - 2 ; case 5
	.short _02228FA2 - _02228F70 - 2 ; case 6
	.short _02228FA2 - _02228F70 - 2 ; case 7
	.short _02228FA2 - _02228F70 - 2 ; case 8
	.short _02228FA2 - _02228F70 - 2 ; case 9
	.short _02228FA0 - _02228F70 - 2 ; case 10
	.short _02228FA2 - _02228F70 - 2 ; case 11
	.short _02228FA0 - _02228F70 - 2 ; case 12
	.short _02228F9C - _02228F70 - 2 ; case 13
	.short _02228F98 - _02228F70 - 2 ; case 14
	.short _02228FA2 - _02228F70 - 2 ; case 15
	.short _02228F94 - _02228F70 - 2 ; case 16
	.short _02228F98 - _02228F70 - 2 ; case 17
_02228F94:
	mov r2, #0x96
	b _02228FA2
_02228F98:
	mov r2, #0x97
	b _02228FA2
_02228F9C:
	mov r2, #0x9c
	b _02228FA2
_02228FA0:
	mov r2, #0x9b
_02228FA2:
	ldr r0, _02228FC4 ; =0x00000F0F
	add r3, r4, #0
	str r0, [sp]
	add r3, #0x90
	ldr r1, [r4, #0x24]
	ldr r3, [r3]
	add r0, r4, #0
	bl GlobalTerminalConnect_PrintMessage
	add r0, r4, #0
	mov r1, #0xc
	mov r2, #0xb
	bl GlobalTerminalConnect_SetStateAndNext
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
_02228FC4: .word 0x00000F0F
	thumb_func_end GlobalTerminalConnect_StateShowServerError


	thumb_func_start GlobalTerminalConnect_StateServerErrorDisconnect
GlobalTerminalConnect_StateServerErrorDisconnect: ; 0x02228FC8
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	add r1, r4, #0
	add r1, #0x94
	ldr r1, [r1]
	cmp r1, #3
	bhi _02229064
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02228FE4: ; jump table
	.short _02228FEC - _02228FE4 - 2 ; case 0
	.short _0222900C - _02228FE4 - 2 ; case 1
	.short _0222902C - _02228FE4 - 2 ; case 2
	.short _0222904C - _02228FE4 - 2 ; case 3
_02228FEC:
	ldr r1, _02229088 ; =0x00000F0F
	add r3, r4, #0
	str r1, [sp]
	add r3, #0x90
	ldr r1, [r4, #0x24]
	ldr r3, [r3]
	mov r2, #0xba
	bl GlobalTerminalConnect_PrintMessage
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02229080
_0222900C:
	ldr r0, [r4, #0x40]
	bl GlobalTerminalConnect_IsPrinting
	cmp r0, #0
	bne _02229080
	bl CommState_FreeWfcSession
	bl ov00_021EC8D8
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02229080
_0222902C:
	ldr r1, _02229088 ; =0x00000F0F
	add r3, r4, #0
	str r1, [sp]
	add r3, #0x90
	ldr r1, [r4, #0x24]
	ldr r3, [r3]
	mov r2, #0xbb
	bl GlobalTerminalConnect_PrintMessage
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02229080
_0222904C:
	ldr r0, [r4, #0x40]
	bl GlobalTerminalConnect_IsPrinting
	cmp r0, #0
	bne _02229080
	add r0, r4, #0
	add r0, #0x94
	ldr r0, [r0]
	add r4, #0x94
	add r0, r0, #1
	str r0, [r4]
	b _02229080
_02229064:
	add r0, r4, #0
	add r0, #0x98
	ldr r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x98
	str r1, [r0]
	add r0, r4, #0
	add r0, #0x98
	ldr r0, [r0]
	cmp r0, #0x1e
	ble _02229080
	mov r0, #8
	str r0, [r4, #8]
_02229080:
	mov r0, #0
	add sp, #4
	pop {r3, r4, pc}
	nop
_02229088: .word 0x00000F0F
	thumb_func_end GlobalTerminalConnect_StateServerErrorDisconnect
