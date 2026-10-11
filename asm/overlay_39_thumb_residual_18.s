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

	thumb_func_start GlobalTerminalConnect_GetAlignedX
GlobalTerminalConnect_GetAlignedX: ; 0x02228964
	push {r4, lr}
	add r4, r0, #0
	cmp r3, #1
	beq _02228972
	cmp r3, #2
	beq _02228988
	b _02228996
_02228972:
	ldr r0, [sp, #0xc]
	mov r2, #0
	bl FontID_String_GetWidth
	ldrb r1, [r4, #7]
	lsl r1, r1, #3
	sub r1, r1, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r2, r0, #1
	b _02228996
_02228988:
	ldr r0, [sp, #0xc]
	mov r2, #0
	bl FontID_String_GetWidth
	ldrb r1, [r4, #7]
	lsl r1, r1, #3
	sub r2, r1, r0
_02228996:
	add r0, r2, #0
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end GlobalTerminalConnect_GetAlignedX


	thumb_func_start GlobalTerminalConnect_PrintAligned
GlobalTerminalConnect_PrintAligned: ; 0x0222899C
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r4, r3, #0
	ldr r3, [sp, #0x24]
	add r5, r0, #0
	str r3, [sp]
	mov r3, #1
	str r3, [sp, #4]
	ldr r3, [sp, #0x20]
	add r6, r1, #0
	bl GlobalTerminalConnect_GetAlignedX
	add r3, r0, #0
	str r4, [sp]
	mov r1, #0
	ldr r0, [sp, #0x24]
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	add r0, r5, #0
	mov r1, #1
	add r2, r6, #0
	bl AddTextPrinterParameterizedWithColor
	add sp, #0x10
	pop {r4, r5, r6, pc}
	thumb_func_end GlobalTerminalConnect_PrintAligned


	thumb_func_start GlobalTerminalConnect_ShowErrorWindow
GlobalTerminalConnect_ShowErrorWindow: ; 0x022289D0
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #1
	add r6, r1, #0
	lsl r0, r0, #8
	mov r1, #0x7c
	bl String_New
	add r4, r0, #0
	ldr r0, [r5, #0x2c]
	add r1, r6, #0
	add r2, r4, #0
	bl ReadMsgDataIntoString
	ldr r0, [r5, #0x20]
	ldr r1, [r5, #0x3c]
	add r2, r4, #0
	bl StringExpandPlaceholders
	add r0, r5, #0
	add r0, #0x64
	mov r1, #0xf
	bl FillWindowPixelBuffer
	add r0, r5, #0
	add r0, #0x64
	mov r1, #1
	mov r2, #0x1f
	mov r3, #0xb
	bl DrawFrameAndWindow1
	mov r3, #0
	str r3, [sp]
	str r3, [sp, #4]
	str r3, [sp, #8]
	add r0, r5, #0
	ldr r2, [r5, #0x3c]
	add r0, #0x64
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r0, #0xff
	str r0, [r5, #0x40]
	add r0, r4, #0
	bl String_Delete
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end GlobalTerminalConnect_ShowErrorWindow


	thumb_func_start GlobalTerminalConnect_ShowErrorCode
GlobalTerminalConnect_ShowErrorCode: ; 0x02228A34
	push {r3, r4, r5, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0
	add r4, r1, #0
	mvn r0, r0
	cmp r4, r0
	bne _02228A46
	mov r4, #0xb
_02228A46:
	mov r0, #2
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [r5, #0x20]
	mov r1, #0
	mov r3, #5
	bl BufferIntegerAsString
	add r0, r5, #0
	add r0, #0x44
	mov r1, #1
	bl ClearFrameAndWindow2
	add r0, r5, #0
	add r1, r4, #0
	bl GlobalTerminalConnect_ShowErrorWindow
	add sp, #8
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end GlobalTerminalConnect_ShowErrorCode
