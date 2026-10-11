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

	thumb_func_start GlobalTerminalNet_GetSequence
GlobalTerminalNet_GetSequence: ; 0x02227DE4
	mov r1, #0xef
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	bx lr
	thumb_func_end GlobalTerminalNet_GetSequence


	thumb_func_start GlobalTerminalNet_CreateOsHeap
GlobalTerminalNet_CreateOsHeap: ; 0x02227DEC
	push {r4, r5, r6, lr}
	mov r1, #2
	lsl r1, r1, #0xc
	bl Heap_Alloc
	add r5, r0, #0
	mov r6, #2
	lsl r6, r6, #0xc
	mov r0, #0
	add r1, r5, #0
	add r2, r5, r6
	mov r3, #1
	bl OS_InitAlloc
	add r4, r0, #0
	mov r0, #0
	add r1, r4, #0
	bl OS_SetArenaLo
	add r2, r5, r6
	add r4, #0x1f
	mov r0, #0x1f
	bic r4, r0
	add r2, #0x1f
	bic r2, r0
	mov r0, #0
	add r1, r4, #0
	bl OS_CreateHeap
	add r1, r0, #0
	ldr r0, _02227E38 ; =sGlobalTerminalNetOsHeap
	str r1, [r0]
	mov r0, #0
	bl OS_SetCurrentHeap
	add r0, r5, #0
	pop {r4, r5, r6, pc}
	nop
_02227E38: .word sGlobalTerminalNetOsHeap
	thumb_func_end GlobalTerminalNet_CreateOsHeap
