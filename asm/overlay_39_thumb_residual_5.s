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

	thumb_func_start GlobalTerminalNet_RequestRankings
GlobalTerminalNet_RequestRankings: ; 0x022274D4
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r1, #0
	add r4, r2, #0
	bl GlobalTerminalNet_IsIdle
	cmp r0, #0
	bne _022274E8
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_022274E8:
	ldr r0, [r5]
	bl Save_PlayerData_GetIGTAddr
	mov r1, #0x66
	lsl r1, r1, #2
	add r1, r5, r1
	mov r2, #4
	bl MI_CpuCopy8
	mov r2, #0x19
	lsl r2, r2, #4
	ldr r0, [r5]
	add r1, r6, #0
	add r2, r5, r2
	bl ov39_022271CC
	mov r0, #0x67
	lsl r0, r0, #2
	mov r6, #0
	add r7, r5, #0
	add r1, r0, #4
_02227512:
	ldr r3, [r4]
	ldr r2, [r4, #4]
	add r6, r6, #1
	str r3, [r7, r0]
	str r2, [r7, r1]
	add r4, #8
	add r7, #8
	cmp r6, #3
	blt _02227512
	mov r0, #0xfa
	ldr r1, _02227530 ; =0x000055F1
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227530: .word 0x000055F1
	thumb_func_end GlobalTerminalNet_RequestRankings


	thumb_func_start GlobalTerminalNet_RequestVideoUpload
GlobalTerminalNet_RequestVideoUpload: ; 0x02227534
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl GlobalTerminalNet_IsIdle
	cmp r0, #0
	bne _02227546
	mov r0, #0
	pop {r3, r4, r5, pc}
_02227546:
	bl BattleRecord_sizeof
	sub r1, r0, #4
	ldr r0, _02227584 ; =0x00001D4C
	cmp r1, r0
	beq _02227556
	bl GF_AssertFail
_02227556:
	bl sub_0202FC74
	mov r1, #0x19
	lsl r1, r1, #4
	str r0, [r5, r1]
	ldr r0, [r5]
	bl VsRecorder_EncryptRecord
	bl BattleRecord_GetProfile
	add r1, r0, #0
	add r0, r4, #0
	mov r2, #0x80
	bl MI_CpuCopy8
	ldr r0, _02227588 ; =0x00000411
	mov r1, #0x3c
	strb r1, [r5, r0]
	ldr r1, _0222758C ; =0x000059D8
	sub r0, #0x29
	str r1, [r5, r0]
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02227584: .word 0x00001D4C
_02227588: .word 0x00000411
_0222758C: .word 0x000059D8
	thumb_func_end GlobalTerminalNet_RequestVideoUpload


	thumb_func_start GlobalTerminalNet_RequestVideoSearch
GlobalTerminalNet_RequestVideoSearch: ; 0x02227590
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	add r7, r3, #0
	bl GlobalTerminalNet_IsIdle
	cmp r0, #0
	bne _022275A6
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_022275A6:
	mov r0, #0x19
	lsl r0, r0, #4
	add r0, r5, r0
	mov r1, #0
	mov r2, #0x10
	bl MI_CpuFill8
	mov r1, #0x19
	lsl r1, r1, #4
	strh r4, [r5, r1]
	add r0, r1, #2
	strb r6, [r5, r0]
	add r0, r1, #3
	strb r7, [r5, r0]
	add r0, sp, #8
	ldrb r2, [r0, #0x10]
	add r0, r1, #4
	strb r2, [r5, r0]
	add r0, r1, #0
	sub r0, #0x50
	add r1, #8
	str r0, [r5, r1]
	mov r0, #0x3f
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r5, r0]
	ldr r1, _022275E4 ; =0x000059D9
	sub r0, #8
	str r1, [r5, r0]
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022275E4: .word 0x000059D9
	thumb_func_end GlobalTerminalNet_RequestVideoSearch


	thumb_func_start ov39_022275E8
ov39_022275E8: ; 0x022275E8
	push {r4, lr}
	add r4, r0, #0
	bl GlobalTerminalNet_IsIdle
	cmp r0, #0
	bne _022275F8
	mov r0, #0
	pop {r4, pc}
_022275F8:
	mov r0, #0x19
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0
	mov r2, #0x10
	bl MI_CpuFill8
	mov r0, #0x19
	ldr r1, _02227640 ; =0x0000FFFF
	lsl r0, r0, #4
	strh r1, [r4, r0]
	mov r1, #0xfe
	add r0, r1, #0
	add r0, #0x94
	strb r1, [r4, r0]
	mov r1, #0xff
	add r0, r1, #0
	add r0, #0x94
	strb r1, [r4, r0]
	add r0, r1, #0
	add r0, #0x95
	strb r1, [r4, r0]
	add r0, r1, #0
	add r0, #0x41
	add r1, #0x99
	str r0, [r4, r1]
	mov r0, #0x3f
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r4, r0]
	ldr r1, _02227644 ; =0x000059D9
	sub r0, #8
	str r1, [r4, r0]
	mov r0, #1
	pop {r4, pc}
	nop
_02227640: .word 0x0000FFFF
_02227644: .word 0x000059D9
	thumb_func_end ov39_022275E8


	thumb_func_start ov39_02227648
ov39_02227648: ; 0x02227648
	push {r4, lr}
	add r4, r0, #0
	bl GlobalTerminalNet_IsIdle
	cmp r0, #0
	bne _02227658
	mov r0, #0
	pop {r4, pc}
_02227658:
	mov r0, #0x19
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0
	mov r2, #0x10
	bl MI_CpuFill8
	mov r0, #0x19
	ldr r1, _0222769C ; =0x0000FFFF
	lsl r0, r0, #4
	strh r1, [r4, r0]
	mov r1, #0xff
	add r0, r1, #0
	add r0, #0x93
	strb r1, [r4, r0]
	add r0, r1, #0
	add r0, #0x94
	strb r1, [r4, r0]
	add r0, r1, #0
	add r0, #0x95
	strb r1, [r4, r0]
	add r0, r1, #0
	add r0, #0x41
	add r1, #0x99
	str r0, [r4, r1]
	mov r0, #0x3f
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r4, r0]
	ldr r1, _022276A0 ; =0x000059D9
	sub r0, #8
	str r1, [r4, r0]
	mov r0, #1
	pop {r4, pc}
	.balign 4, 0
_0222769C: .word 0x0000FFFF
_022276A0: .word 0x000059D9
	thumb_func_end ov39_02227648
