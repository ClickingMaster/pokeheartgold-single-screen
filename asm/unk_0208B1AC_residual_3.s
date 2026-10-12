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

	thumb_func_start PokemonSummary_UpdateStatusSprite
PokemonSummary_UpdateStatusSprite: ; 0x0208B4EC
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0xa
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	lsl r0, r0, #4
	lsr r0, r0, #4
	cmp r0, #7
	bne _0208B514
	ldr r0, _0208B554 ; =0x00000454
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _0208B558 ; =0x00000524
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	pop {r4, pc}
_0208B514:
	cmp r0, #0
	ldr r0, _0208B558 ; =0x00000524
	beq _0208B540
	ldr r0, [r4, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, _0208B554 ; =0x00000454
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	mov r1, #0xa
	lsl r1, r1, #6
	ldr r0, _0208B554 ; =0x00000454
	ldr r1, [r4, r1]
	ldr r0, [r4, r0]
	lsl r1, r1, #4
	lsr r1, r1, #4
	bl Sprite_SetAnimCtrlSeq
	pop {r4, pc}
_0208B540:
	ldr r0, [r4, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, _0208B554 ; =0x00000454
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	pop {r4, pc}
	.balign 4, 0
_0208B554: .word 0x00000454
_0208B558: .word 0x00000524
	thumb_func_end PokemonSummary_UpdateStatusSprite
