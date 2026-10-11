	.include "asm/macros.inc"
	.include "overlay_31.inc"
	.include "global.inc"

	.text
	.public sTouchMartTextSkipHitbox
	.public sTouchMartButtonSets
	.public TouchMartApp_Init
	.public TouchMartApp_PrintCancel
	.public TouchMartApp_RunTask
	.public TouchMartApp_HandleAction
	.public TouchMartApp_LoadMainTilemap
	.public TouchMartApp_SetupBgs
	.public TouchMartApp_LoadGraphics
	.public TouchMartApp_AddWindows
	.public TouchMartApp_AddItemWindows
	.public TouchMartApp_PrintItemList
	.public TouchMartApp_PrintPrice
	.public TouchMartApp_PrintMoney
	.public TouchMartApp_PrintPageNumber
	.public TouchMartApp_EditListTilemap
	.public TouchMartApp_PrintSelectedItem
	.public TouchMartApp_PrintInBag
	.public TouchMartApp_PrintQuantity
	.public TouchMartApp_PrintHowMany
	.public TouchMartApp_PrintConfirmPurchase
	.public TouchMartApp_CreateYesNoPrompt
	.public TouchMartApp_PrintPurchaseDone
	.public TouchMartApp_PrintCannotAfford
	.public TouchMartApp_PrintNoRoom
	.public TouchMartApp_PrintNoRoomInBag
	.public TouchMartApp_PrintPremierBallBonus
	.public TouchMartApp_AddDetailWindows
	.public sTouchMartQuantityButtons
	.public sTouchMartItemListButtons
	.public sTouchMartItemWindowPositions
	.public sTouchMartBgTemplate_Sub0
	.public sTouchMartBgTemplate_Sub1
	.public sTouchMartBgTemplate_Sub2
	.public sTouchMartListTilemapEdits
	.public TouchMartApp_SetupText
	.public TouchMartApp_FreeText
	.public TouchMartApp_Exit
	.public TouchMartApp_IsExitDone
	.public TouchMartApp_IsSelectionValid
	.public TouchMartApp_GetTouchedButton
	.public TouchMartApp_FreeWindows
	.public TouchMartApp_RemoveItemWindows
	.public TouchMartApp_PrintItemName
	.public TouchMartApp_LoadListTilemap
	.public ov31_0225E12C
	.public TouchMartApp_PrintBuyButton
	.public TouchMartApp_BufferItemName
	.public TouchMartApp_BufferItemNamePlural
	.public TouchMartApp_BufferItemNameWithIndefArticle
	.public TouchMartApp_HandleYesNoPrompt
	.public TouchMartApp_PurchaseDonePrinterCallback
	.public TouchMartApp_GetCannotAffordMessage
	.public TouchMartApp_GetNoRoomMessage
	.public TouchMartApp_RemoveDetailWindows

	thumb_func_start TouchMartApp_PrintCannotAfford
TouchMartApp_PrintCannotAfford: ; 0x0225EA08
	push {r4, r5, lr}
	sub sp, #0xc
	mov r1, #0x56
	add r5, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, #0x14]
	ldr r1, [r5, r1]
	bl TouchMartApp_GetCannotAffordMessage
	mov r1, #0x55
	add r4, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, #0x34
	ldr r1, [r5, r1]
	add r2, r4, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	add r0, r5, #0
	add r0, #0x44
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r1, #4
	str r1, [sp, #4]
	ldr r0, [r5, #4]
	ldr r2, _0225EA98 ; =0x000001B5
	mov r3, #5
	bl LoadUserFrameGfx2
	add r0, r5, #0
	ldr r2, _0225EA98 ; =0x000001B5
	add r0, #0x44
	mov r1, #1
	mov r3, #5
	bl DrawFrameAndWindow2
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	str r0, [sp, #4]
	mov r2, #0x62
	add r0, r5, #0
	str r3, [sp, #8]
	lsl r2, r2, #2
	ldr r2, [r5, r2]
	add r0, #0x44
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r1, #0xa
	ldr r2, [r5, #0x14]
	lsl r1, r1, #6
	strb r0, [r2, r1]
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0225EA98: .word 0x000001B5
	thumb_func_end TouchMartApp_PrintCannotAfford


	thumb_func_start TouchMartApp_PrintNoRoom
TouchMartApp_PrintNoRoom: ; 0x0225EA9C
	push {r4, r5, lr}
	sub sp, #0xc
	mov r1, #0x56
	add r5, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, #0x14]
	ldr r1, [r5, r1]
	bl TouchMartApp_GetNoRoomMessage
	mov r1, #0x55
	add r4, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, #0x34
	ldr r1, [r5, r1]
	add r2, r4, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	add r0, r5, #0
	add r0, #0x44
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r1, #4
	str r1, [sp, #4]
	ldr r0, [r5, #4]
	ldr r2, _0225EB2C ; =0x000001B5
	mov r3, #5
	bl LoadUserFrameGfx2
	add r0, r5, #0
	ldr r2, _0225EB2C ; =0x000001B5
	add r0, #0x44
	mov r1, #1
	mov r3, #5
	bl DrawFrameAndWindow2
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	str r0, [sp, #4]
	mov r2, #0x62
	add r0, r5, #0
	str r3, [sp, #8]
	lsl r2, r2, #2
	ldr r2, [r5, r2]
	add r0, #0x44
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r1, #0xa
	ldr r2, [r5, #0x14]
	lsl r1, r1, #6
	strb r0, [r2, r1]
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0225EB2C: .word 0x000001B5
	thumb_func_end TouchMartApp_PrintNoRoom


	thumb_func_start TouchMartApp_PrintNoRoomInBag
TouchMartApp_PrintNoRoomInBag: ; 0x0225EB30
	push {r4, r5, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #0x56
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0x10
	bl NewString_ReadMsgData
	mov r1, #0x55
	add r4, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, #0x34
	ldr r1, [r5, r1]
	add r2, r4, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	add r0, r5, #0
	add r0, #0x44
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r1, #4
	str r1, [sp, #4]
	ldr r0, [r5, #4]
	ldr r2, _0225EBC0 ; =0x000001B5
	mov r3, #5
	bl LoadUserFrameGfx2
	add r0, r5, #0
	ldr r2, _0225EBC0 ; =0x000001B5
	add r0, #0x44
	mov r1, #1
	mov r3, #5
	bl DrawFrameAndWindow2
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	str r0, [sp, #4]
	mov r2, #0x62
	add r0, r5, #0
	str r3, [sp, #8]
	lsl r2, r2, #2
	ldr r2, [r5, r2]
	add r0, #0x44
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r1, #0xa
	ldr r2, [r5, #0x14]
	lsl r1, r1, #6
	strb r0, [r2, r1]
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0225EBC0: .word 0x000001B5
	thumb_func_end TouchMartApp_PrintNoRoomInBag


	thumb_func_start TouchMartApp_PrintPremierBallBonus
TouchMartApp_PrintPremierBallBonus: ; 0x0225EBC4
	push {r4, r5, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #0x56
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0x14
	bl NewString_ReadMsgData
	mov r1, #0x55
	add r4, r0, #0
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	add r1, #0x34
	ldr r1, [r5, r1]
	add r2, r4, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	bl String_Delete
	add r0, r5, #0
	add r0, #0x44
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r1, #4
	str r1, [sp, #4]
	ldr r0, [r5, #4]
	ldr r2, _0225EC54 ; =0x000001B5
	mov r3, #5
	bl LoadUserFrameGfx2
	add r0, r5, #0
	ldr r2, _0225EC54 ; =0x000001B5
	add r0, #0x44
	mov r1, #1
	mov r3, #5
	bl DrawFrameAndWindow2
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Options_GetTextFrameDelay
	mov r3, #0
	str r3, [sp]
	str r0, [sp, #4]
	mov r2, #0x62
	add r0, r5, #0
	str r3, [sp, #8]
	lsl r2, r2, #2
	ldr r2, [r5, r2]
	add r0, #0x44
	mov r1, #1
	bl AddTextPrinterParameterized
	mov r1, #0xa
	ldr r2, [r5, #0x14]
	lsl r1, r1, #6
	strb r0, [r2, r1]
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0225EC54: .word 0x000001B5
	thumb_func_end TouchMartApp_PrintPremierBallBonus


	thumb_func_start TouchMartApp_AddDetailWindows
TouchMartApp_AddDetailWindows: ; 0x0225EC58
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #7
	str r0, [sp]
	mov r0, #0xb
	str r0, [sp, #4]
	mov r2, #4
	add r1, r4, #0
	str r2, [sp, #8]
	mov r3, #0xc
	str r3, [sp, #0xc]
	mov r0, #0xad
	str r0, [sp, #0x10]
	ldr r0, [r4, #4]
	add r1, #0xe4
	bl AddWindowParameterized
	mov r0, #0xe
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	mov r0, #0xd9
	str r0, [sp, #0x10]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0xf4
	mov r2, #4
	mov r3, #0x10
	bl AddWindowParameterized
	mov r0, #0xe
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	mov r1, #0xdf
	str r1, [sp, #0x10]
	add r1, #0x25
	ldr r0, [r4, #4]
	add r1, r4, r1
	mov r2, #4
	mov r3, #0x14
	bl AddWindowParameterized
	mov r0, #0x15
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	mov r1, #0xe5
	str r1, [sp, #0x10]
	add r1, #0x2f
	ldr r0, [r4, #4]
	add r1, r4, r1
	mov r2, #4
	mov r3, #0xe
	bl AddWindowParameterized
	mov r0, #0xd
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #5
	str r0, [sp, #8]
	mov r0, #0xb
	str r0, [sp, #0xc]
	mov r1, #0xf3
	str r1, [sp, #0x10]
	add r1, #0x31
	ldr r0, [r4, #4]
	add r1, r4, r1
	mov r2, #4
	mov r3, #1
	bl AddWindowParameterized
	mov r0, #0xe
	str r0, [sp]
	mov r0, #8
	str r0, [sp, #4]
	mov r0, #3
	str r0, [sp, #8]
	mov r0, #0xc
	ldr r1, _0225ED98 ; =0x0000011B
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	add r1, #0x19
	ldr r0, [r4, #4]
	add r1, r4, r1
	mov r2, #4
	mov r3, #0x17
	bl AddWindowParameterized
	mov r0, #1
	str r0, [sp]
	mov r0, #0x11
	str r0, [sp, #4]
	mov r2, #4
	str r2, [sp, #8]
	mov r3, #0xc
	ldr r1, _0225ED9C ; =0x00000133
	str r3, [sp, #0xc]
	str r1, [sp, #0x10]
	add r1, #0x11
	ldr r0, [r4, #4]
	add r1, r4, r1
	bl AddWindowParameterized
	add r0, r4, #0
	add r0, #0xe4
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r4, #0
	add r0, #0xf4
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x41
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x45
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x49
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x4d
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r0, #0x51
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #0xf
	bl FillWindowPixelBuffer
	add sp, #0x14
	pop {r3, r4, pc}
	.balign 4, 0
_0225ED98: .word 0x0000011B
_0225ED9C: .word 0x00000133
	thumb_func_end TouchMartApp_AddDetailWindows
