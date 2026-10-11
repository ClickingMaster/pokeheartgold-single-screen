#include "msgdata/msg/msg_0252.h"
	.include "asm/macros.inc"
	.include "overlay_trainer_card_signature.inc"
	.include "global.inc"

	.text
	.public TrainerCardSignature_Exit
	.public TrainerCardSignature_Init
	.public TrainerCardSignature_Main
	.public sTrainerCardSignatureDoneButtonPos
	.public TrainerCardSignature_InitBgLayers
	.public TrainerCardSignature_InitStrings
	.public TrainerCardSignature_LoadBgGraphics
	.public TrainerCardSignature_InitSpriteResources
	.public TrainerCardSignature_CreateDoneButtonSprite
	.public TrainerCardSignature_PrintCentered
	.public TrainerCardSignature_CreateWindows
	.public TrainerCardSignature_HandleTouch
	.public TrainerCardSignature_State_ConfirmPrompt
	.public TrainerCardSignature_State_ErasePrompt
	.public TrainerCardSignature_BlitClipped
	.public TrainerCardSignature_DrawLine
	.public TrainerCardSignature_SavePenPositions
	.public TrainerCardSignature_DrawStrokes
	.public TrainerCardSignature_TakeNewStroke
	.public TrainerCardSignature_PrintMessage
	.public TrainerCardSignature_PackCanvas
	.public TrainerCardSignature_UpdatePenSound
	.public sTrainerCardSignatureCanvasHitbox
	.public sTrainerCardSignatureButtonHitboxes
	.public sTrainerCardSignatureGraphicsModes
	.public sTrainerCardSignatureObjCharTransferTemplate
	.public sTrainerCardSignatureSubBg0Template
	.public sTrainerCardSignatureSubBg1Template
	.public sTrainerCardSignatureMainBg0Template
	.public sTrainerCardSignatureMainBg1Template
	.public sTrainerCardSignatureMainBg2Template
	.public sTrainerCardSignatureGraphicsBanks
	.public sTrainerCardSignaturePenBitmaps
	.public sTrainerCardSignatureStateFuncs
	.public TrainerCardSignature_VBlankCB
	.public TrainerCardSignature_SetGfxBanks
	.public TrainerCardSignature_FreeStrings
	.public TrainerCardSignature_FreeBgLayers
	.public TrainerCardSignature_InitObjTransfer
	.public TrainerCardSignature_RemoveWindows
	.public ov52_021E8BD8
	.public ov52_021E8CBC
	.public TrainerCardSignature_SetDoneButtonPressed
	.public TrainerCardSignature_State_Draw
	.public TrainerCardSignature_UpdateCanvas
	.public TrainerCardSignature_ShowYesNoPrompt
	.public TrainerCardSignature_State_WaitConfirmMessage
	.public TrainerCardSignature_State_PrintEraseMessage
	.public TrainerCardSignature_State_WaitEraseMessage
	.public ov52_021E9218
	.public TrainerCardSignature_AnimateButtonPalette
	.public ov52_021E925C
	.public TrainerCardSignature_IsPrinterDone
	.public TrainerCardSignature_PlayPenSoundOnTurn
	.public TrainerCardSignature_SetDoneButtonHidden
	.public TrainerCardSignature_PrintCentered


	thumb_func_start TrainerCardSignature_CreateWindows
TrainerCardSignature_CreateWindows: ; 0x021E89D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	mov r0, #1
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x28
	mov r1, #0xb5
	str r0, [sp, #0x10]
	lsl r1, r1, #2
	ldr r0, [r5]
	add r1, r5, r1
	mov r2, #0
	mov r3, #2
	bl AddWindowParameterized
	mov r0, #0xb5
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #9
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r0, #8
	mov r1, #0xb1
	lsl r1, r1, #2
	str r0, [sp, #8]
	mov r2, #1
	str r2, [sp, #0xc]
	str r2, [sp, #0x10]
	ldr r0, [r5]
	add r1, r5, r1
	mov r3, #4
	bl AddWindowParameterized
	mov r0, #0xb1
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #2
	bl FillWindowPixelBuffer
	mov r0, #0x15
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	str r0, [sp, #0xc]
	mov r0, #0xc1
	mov r1, #0xb9
	str r0, [sp, #0x10]
	lsl r1, r1, #2
	ldr r0, [r5]
	add r1, r5, r1
	mov r2, #1
	mov r3, #0x1a
	bl AddWindowParameterized
	mov r0, #0xb9
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, _021E8B84 ; =0x000E0702
	mov r2, #1
	str r0, [sp]
	mov r0, #0xb9
	lsl r0, r0, #2
	ldr r1, [r5, #0x2c]
	add r0, r5, r0
	mov r3, #2
	bl TrainerCardSignature_PrintCentered
	mov r1, #2
	lsl r1, r1, #8
	bl DC_FlushRange
	mov r4, #0
	ldr r0, _021E8B88 ; =0x00005B9C
	str r4, [sp, #0x18]
	add r7, r4, #0
	add r6, r5, r0
_021E8A8A:
	mov r0, #0
	str r0, [sp]
	mov r0, #0xb9
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #4
	mov r2, #2
	add r3, r4, #0
	str r6, [sp, #4]
	bl FontOAM_CopyWindowChars
	mov r1, #1
	add r0, r6, #0
	lsl r1, r1, #8
	bl DC_FlushRange
	mov r2, #1
	add r0, r6, #0
	add r1, r7, #0
	lsl r2, r2, #8
	bl GX_LoadOBJ
	mov r0, #1
	lsl r0, r0, #8
	add r7, r7, r0
	ldr r0, [sp, #0x18]
	add r4, r4, #4
	add r0, r0, #1
	str r0, [sp, #0x18]
	cmp r0, #2
	blt _021E8A8A
	mov r3, #2
	str r3, [sp]
	mov r0, #0x1c
	str r0, [sp, #4]
	mov r1, #0xbd
	lsl r1, r1, #2
	str r3, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0xd1
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r1, r5, r1
	mov r2, #1
	bl AddWindowParameterized
	ldr r1, [r5, #0x30]
	mov r0, #1
	mov r2, #0
	bl FontID_String_GetWidth
	add r4, r0, #0
	mov r0, #0xbd
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	mov r3, #0xe0
	sub r4, r3, r4
	lsr r3, r4, #0x1f
	add r3, r4, r3
	ldr r0, _021E8B8C ; =0x00010200
	str r1, [sp, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	mov r0, #0xbd
	lsl r0, r0, #2
	ldr r2, [r5, #0x30]
	add r0, r5, r0
	mov r1, #1
	asr r3, r3, #1
	bl AddTextPrinterParameterizedWithColor
	mov r0, #0
	str r0, [sp, #0x14]
	mov r0, #0x9d
	lsl r0, r0, #2
	mov r7, #1
	mov r6, #3
	add r4, r5, r0
_021E8B32:
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	lsl r0, r7, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r1, r4, #0
	mov r2, #4
	mov r3, #5
	bl AddWindowParameterized
	add r0, r4, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	ldr r0, [sp, #0x14]
	add r7, #0x14
	add r0, r0, #1
	add r6, r6, #4
	add r4, #0x10
	str r0, [sp, #0x14]
	cmp r0, #5
	blt _021E8B32
	mov r0, #0x9d
	lsl r0, r0, #2
	ldr r2, _021E8B90 ; =0x000E0D0F
	add r0, r5, r0
	mov r1, #0
	add r3, r5, #0
	bl ov52_021E925C
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_021E8B84: .word 0x000E0702
_021E8B88: .word 0x00005B9C
_021E8B8C: .word 0x00010200
_021E8B90: .word 0x000E0D0F
	thumb_func_end TrainerCardSignature_CreateWindows
