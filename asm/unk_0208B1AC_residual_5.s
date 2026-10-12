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

	thumb_func_start PokemonSummary_SetPokeathlonStatStars
PokemonSummary_SetPokeathlonStatStars: ; 0x0208B780
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r7, r0, #0
	ldr r0, [sp, #0x2c]
	add r5, r1, #0
	add r1, r2, #0
	add r2, r3, #0
	str r0, [sp]
	ldr r3, [sp, #0x28]
	add r0, sp, #8
	bl PokemonSummary_GetPokeathlonStarStates
	str r0, [sp, #4]
	lsl r0, r5, #2
	mov r6, #0
	add r4, sp, #8
	add r5, r7, r0
_0208B7A2:
	ldrb r0, [r4]
	cmp r0, #0
	bne _0208B7B2
	ldr r0, [r5]
	mov r1, #0
	bl Sprite_SetDrawFlag
	b _0208B808
_0208B7B2:
	cmp r0, #1
	bne _0208B7C8
	ldr r0, [r5]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, [r5]
	mov r1, #3
	bl Sprite_SetAnimCtrlSeq
	b _0208B808
_0208B7C8:
	cmp r0, #2
	bne _0208B7DE
	ldr r0, [r5]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, [r5]
	mov r1, #0
	bl Sprite_SetAnimCtrlSeq
	b _0208B808
_0208B7DE:
	cmp r0, #3
	bne _0208B7F4
	ldr r0, [r5]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, [r5]
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	b _0208B808
_0208B7F4:
	cmp r0, #4
	bne _0208B808
	ldr r0, [r5]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, [r5]
	mov r1, #2
	bl Sprite_SetAnimCtrlSeq
_0208B808:
	add r6, r6, #1
	add r4, r4, #1
	add r5, r5, #4
	cmp r6, #5
	blt _0208B7A2
	ldr r0, [sp, #4]
	cmp r0, #0
	bne _0208B828
	ldr r0, [sp, #0x30]
	mov r1, #0
	lsl r0, r0, #2
	ldr r0, [r7, r0]
	bl Sprite_SetDrawFlag
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
_0208B828:
	ble _0208B842
	ldr r0, [sp, #0x30]
	mov r1, #1
	lsl r4, r0, #2
	ldr r0, [r7, r4]
	bl Sprite_SetDrawFlag
	ldr r0, [r7, r4]
	mov r1, #4
	bl Sprite_SetAnimCtrlSeq
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
_0208B842:
	bge _0208B858
	ldr r0, [sp, #0x30]
	mov r1, #1
	lsl r4, r0, #2
	ldr r0, [r7, r4]
	bl Sprite_SetDrawFlag
	ldr r0, [r7, r4]
	mov r1, #5
	bl Sprite_SetAnimCtrlSeq
_0208B858:
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end PokemonSummary_SetPokeathlonStatStars
