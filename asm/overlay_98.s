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

	thumb_func_start PokeathlonUtil_CreateSpriteRow
PokeathlonUtil_CreateSpriteRow: ; 0x0221E6F0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x3c
	str r0, [sp]
	ldr r0, [sp, #0x58]
	add r5, r2, #0
	str r0, [sp, #0x58]
	ldr r0, [sp, #0x5c]
	add r4, r1, #0
	str r0, [sp, #0x5c]
	mov r0, #0
	add r2, sp, #8
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	str r0, [r2]
	add r0, r3, #0
	str r3, [sp, #4]
	cmp r0, #0xa
	bls _0221E722
	bl GF_AssertFail
_0221E722:
	add r1, sp, #0x40
	mov r0, #0x14
	ldrsh r6, [r1, r0]
	ldr r0, [sp, #0x50]
	mov r7, #0
	lsl r0, r0, #2
	add r4, r4, r0
_0221E730:
	ldr r0, [sp, #4]
	cmp r7, r0
	bhs _0221E73A
	mov r1, #1
	b _0221E73C
_0221E73A:
	mov r1, #0
_0221E73C:
	add r0, sp, #8
	strh r1, [r0, #6]
	ldr r0, [r5]
	str r0, [sp, #0x1c]
	ldr r0, [r5, #4]
	str r0, [sp, #0x20]
	ldr r0, [r5, #8]
	str r0, [sp, #0x24]
	ldr r0, [r5, #0xc]
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x5c]
	cmp r0, #0
	beq _0221E75A
	mov r0, #1
	b _0221E75C
_0221E75A:
	mov r0, #2
_0221E75C:
	str r0, [sp, #0x18]
	add r0, sp, #8
	strh r6, [r0]
	ldr r1, [sp, #0x58]
	strh r1, [r0, #2]
	ldr r0, [sp]
	add r1, sp, #8
	bl PokeathlonUtil_NewSprite
	mov r1, #1
	stmia r4!, {r0}
	bl ManagedSprite_SetAnimateFlag
	add r7, r7, #1
	add r6, #0x10
	cmp r7, #0xa
	blo _0221E730
	add sp, #0x3c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokeathlonUtil_CreateSpriteRow
