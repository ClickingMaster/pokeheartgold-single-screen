#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_0208B1AC.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text
	.public gPokemonSummarySpriteResdatIds
	.public gPokemonSummaryOamTransferParam
	.public gPokemonSummaryOamManagerParam
	.public gPokemonSummarySpriteTemplates
	.public gPokemonSummaryBallPalettes
	.public gPokemonSummaryPartySlotPositions
	.public PokemonSummary_InitSpriteSystem
	.public PokemonSummary_FreeSpriteSystem
	.public PokemonSummary_UpdateSpriteAnims
	.public PokemonSummary_CreateSprites
	.public PokemonSummary_UpdatePartySlotSprite
	.public sub_0208B448
	.public PokemonSummary_LoadBallSprite
	.public PokemonSummary_UpdateStatusSprite
	.public PokemonSummary_LoadTypeAndIconSprites
	.public PokemonSummary_UpdateTypeSprites
	.public PokemonSummary_HidePokeathlonStars
	.public PokemonSummary_InitShinyLeafSprites
	.public PokemonSummary_SetPokeathlonStatStars
	.public PokemonSummary_GetPokeathlonStarStates
	.public PokemonSummary_UpdatePokeathlonStars
	.public PokemonSummary_UpdateShinyLeafSprites
	.public PokemonSummary_SetMonTypeSprites
	.public PokemonSummary_SetTypeSprite
	.public PokemonSummary_FindMoveTypeSprite
	.public PokemonSummary_SwapMoveTypeSprites
	.public PokemonSummary_SetMoveCategorySprite
	.public PokemonSummary_UpdateMoveCursorSprite
	.public PokemonSummary_ShowSwapCursorSprite
	.public sub_0208BCB4
	.public PokemonSummary_InitSheenSparkles
	.public PokemonSummary_UpdateMarkingSprites
	.public sub_0208BDC8
	.public PokemonSummary_LoadMonIcon
	.public PokemonSummary_LoadShownMonIcon
	.public PokemonSummary_LoadPartyIcons
	.public sub_0208BF9C
	.public sub_0208BFD0
	.public PokemonSummary_ShowMonIconSprite
	.public PokemonSummary_GetConditionPoint
	.public PokemonSummary_ShowConditionSprites
	.public PokemonSummary_HideConditionSprites
	.public PokemonSummary_SetRibbonSprite
	.public PokemonSummary_ShowRibbonSprites
	.public PokemonSummary_UpdateRibbonSprites
	.public PokemonSummary_UpdateRibbonCursorSprite
	.public sub_0208C3C0

	thumb_func_start PokemonSummary_LoadPartyIcons
PokemonSummary_LoadPartyIcons: ; 0x0208BECC
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	bl PokemonSummary_GetShownMon
	add r7, r0, #0
	mov r0, #0x8b
	lsl r0, r0, #2
	ldr r1, [r6, r0]
	ldrb r0, [r1, #0x11]
	cmp r0, #0
	beq _0208BEEC
	cmp r0, #1
	beq _0208BEF8
	cmp r0, #2
	beq _0208BEF2
	b _0208BEFC
_0208BEEC:
	mov r0, #1
	str r0, [sp]
	b _0208BEFC
_0208BEF2:
	mov r0, #0
	str r0, [sp]
	b _0208BEFC
_0208BEF8:
	ldrb r0, [r1, #0x13]
	str r0, [sp]
_0208BEFC:
	ldr r0, [sp]
	mov r4, #0
	cmp r0, #0
	ble _0208BF7A
	add r5, r6, #0
_0208BF06:
	mov r0, #0x8b
	lsl r0, r0, #2
	ldr r1, [r6, r0]
	ldrb r0, [r1, #0x11]
	cmp r0, #0
	beq _0208BF1C
	cmp r0, #1
	beq _0208BF4A
	cmp r0, #2
	beq _0208BF36
	b _0208BF64
_0208BF1C:
	add r0, r6, #0
	bl PokemonSummary_GetShownMon
	add r7, r0, #0
	bl Pokemon_GetIconPalette
	add r1, r0, #0
	ldr r0, _0208BF98 ; =0x00000528
	add r1, #0xc
	ldr r0, [r5, r0]
	bl thunk_Sprite_SetPaletteOverride
	b _0208BF64
_0208BF36:
	add r0, r7, #0
	bl Boxmon_GetIconPalette
	add r1, r0, #0
	ldr r0, _0208BF98 ; =0x00000528
	add r1, #0xc
	ldr r0, [r5, r0]
	bl thunk_Sprite_SetPaletteOverride
	b _0208BF64
_0208BF4A:
	ldr r0, [r1]
	add r1, r4, #0
	bl Party_GetMonByIndex
	add r7, r0, #0
	bl Pokemon_GetIconPalette
	add r1, r0, #0
	ldr r0, _0208BF98 ; =0x00000528
	add r1, #0xc
	ldr r0, [r5, r0]
	bl thunk_Sprite_SetPaletteOverride
_0208BF64:
	add r2, r4, #0
	add r0, r6, #0
	add r1, r7, #0
	add r2, #0x49
	bl PokemonSummary_LoadMonIcon
	ldr r0, [sp]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, r0
	blt _0208BF06
_0208BF7A:
	cmp r4, #6
	bge _0208BF96
	lsl r0, r4, #2
	add r5, r6, r0
	ldr r6, _0208BF98 ; =0x00000528
	mov r7, #0
_0208BF86:
	ldr r0, [r5, r6]
	add r1, r7, #0
	bl thunk_Sprite_SetDrawFlag
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #6
	blt _0208BF86
_0208BF96:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0208BF98: .word 0x00000528
	thumb_func_end PokemonSummary_LoadPartyIcons
