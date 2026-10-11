	.include "asm/macros.inc"
	.public PokeathlonUtil_GetSpriteSystem
	.public PokeathlonUtil_GetSpriteManager
	.public PokeathlonUtil_CreateSpriteContext
	.public PokeathlonUtil_FreeSpriteContext
	.public PokeathlonUtil_DrawSprites
	.public PokeathlonUtil_NewSprite
	.public PokeathlonUtil_CreateSpriteRow
	.public ov98_0221E784
	.public PokeathlonUtil_LoadMonPicSprite
	.public PokeathlonUtil_LoadMonIconResources
	.public PokeathlonUtil_SetMonIconSprite
	.public PokeathlonUtil_LoadSpriteChars
	.public PokeathlonUtil_LoadSpritePalette
	.public PokeathlonUtil_LoadCharDataFromNarc
	.public PokeathlonUtil_CreateTextContext
	.public PokeathlonUtil_FreeTextContext
	.public ov98_0221EBD8
	.public ov98_0221EBEC
	.public ov98_0221EC08
	.public PokeathlonUtil_PrintMessageCentered
	.public PokeathlonUtil_PrintMessageEx
	.public PokeathlonUtil_PrintMessageWithNumber
	.public PokeathlonUtil_PrintMessage
	.public PokeathlonUtil_PrintMessageAt
	.public PokeathlonUtil_BufferNumber
	.public PokeathlonUtil_PrintMessageWithPlayerName
	.public PokeathlonUtil_PrintSpeciesName
	.public PokeathlonUtil_ClearAndCopyWindow
	.public PokeathlonUtil_PrintString
	.public PokeathlonUtil_ClearWindowTilemap
	.public PokeathlonUtil_SetWindowY
	.public PokeathlonUtil_GetMessageFormat
	.public PokeathlonUtil_ClearWindow
	.public PokeathlonUtil_CheckExitInput
	.public ov98_0221EF64
	.public ov98_0221EF80
	.public ov98_0221EFA4
	.public ov98_0221EFB4
	.public ov98_0221EFE8
	.public ov98_0221F01C
	.public PokeathlonUtil_CountSpeciesWithAllMedals
	.public ov98_0221F058
	.public PokeathlonUtil_ResetDisplay
	.public PokeathlonUtil_DisableDisplay
	.public PokeathlonUtil_GetDecimalDigit
	.public PokeathlonUtil_CountDecimalDigits
	.public PokeathlonUtil_SetGfxBanks
	.public _0221F194
	.public ov98_0221F19C
	.public ov98_0221F1AC
	.public ov98_0221F1C0
	.public ov98_0221F1E0
	.public ov98_0221F1E4
	.public ov98_0221F1F8
	.public ov98_0221F220
	.public ov98_0221F248
	.include "overlay_98.inc"
	.include "global.inc"

	.text

	thumb_func_start PokeathlonUtil_PrintMessageAt
PokeathlonUtil_PrintMessageAt: ; 0x0221ED48
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r4, r0, #0
	ldr r0, [r4, #8]
	add r5, r1, #0
	add r1, r2, #0
	add r6, r3, #0
	bl NewString_ReadMsgData
	add r7, r0, #0
	ldr r0, [r4, #0x10]
	ldr r1, [r4, #0x14]
	add r2, r7, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	add r1, r5, #0
	bl PokeathlonUtil_ClearWindow
	add r0, sp, #0x18
	ldrb r0, [r0, #0x10]
	mov r1, #0
	lsl r5, r5, #4
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0221EDA0 ; =0x00010200
	add r3, r6, #0
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4, #4]
	ldr r2, [r4, #0x14]
	add r0, r0, r5
	bl AddTextPrinterParameterizedWithColor
	ldr r0, [r4, #4]
	add r0, r0, r5
	bl ScheduleWindowCopyToVram
	add r0, r7, #0
	bl String_Delete
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0221EDA0: .word 0x00010200
	thumb_func_end PokeathlonUtil_PrintMessageAt
