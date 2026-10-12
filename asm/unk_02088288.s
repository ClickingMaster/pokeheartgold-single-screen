#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_02088288.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text

	thumb_func_start PokemonSummary_Init
PokemonSummary_Init: ; 0x02088298
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	bl HBlankInterruptDisable
	bl GfGfx_DisableEngineAPlanes
	bl GfGfx_DisableEngineBPlanes
	mov r1, #1
	lsl r1, r1, #0x1a
	ldr r0, [r1]
	ldr r2, _02088408 ; =0xFFFFE0FF
	and r0, r2
	str r0, [r1]
	ldr r0, _0208840C ; =0x04001000
	ldr r3, [r0]
	and r2, r3
	str r2, [r0]
	ldr r3, [r1]
	ldr r2, _02088410 ; =0xFFFF1FFF
	and r3, r2
	str r3, [r1]
	ldr r3, [r0]
	add r1, #0x50
	and r2, r3
	str r2, [r0]
	mov r2, #0
	strh r2, [r1]
	add r0, #0x50
	strh r2, [r0]
	mov r0, #4
	mov r1, #8
	bl SetKeyRepeatTimers
	mov r2, #0x45
	mov r0, #3
	mov r1, #0x13
	lsl r2, r2, #0xc
	bl Heap_Create
	mov r0, #0x27
	mov r1, #0x13
	bl NARC_New
	add r7, r0, #0
	mov r0, #0xa2
	mov r1, #0x13
	bl NARC_New
	add r5, r0, #0
	ldr r1, _02088414 ; =0x000007D8
	add r0, r6, #0
	mov r2, #0x13
	bl OverlayManager_CreateAndGetData
	ldr r2, _02088414 ; =0x000007D8
	mov r1, #0
	add r4, r0, #0
	bl memset
	add r0, r6, #0
	bl OverlayManager_GetArgs
	mov r1, #0x8b
	lsl r1, r1, #2
	str r0, [r4, r1]
	mov r0, #0x13
	bl BgConfig_Alloc
	str r0, [r4]
	mov r0, #0x8b
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	str r1, [r0, #0x38]
	mov r1, #1
	mov r0, #0x13
	add r2, r1, #0
	bl PokepicAnimManager_New
	mov r1, #0xb3
	lsl r1, r1, #2
	str r0, [r4, r1]
	mov r0, #0xb4
	mov r1, #0x13
	bl NARC_New
	ldr r1, _02088418 ; =0x000007B8
	str r0, [r4, r1]
	mov r0, #0
	mov r1, #0x13
	bl FontID_SetAccessDirect
	bl GF_TouchpadResetSampling
	mov r0, #4
	bl GF_TouchpadStartSampling
	bl PokemonSummary_SetGraphicsBanks
	ldr r0, [r4]
	bl PokemonSummary_InitBgs
	add r0, r4, #0
	add r1, r7, #0
	add r2, r5, #0
	bl PokemonSummary_LoadBgGraphics
	bl PokemonSummary_SetBlend
	add r0, r4, #0
	bl PokemonSummary_Init3D
	mov r0, #4
	mov r1, #0x13
	bl FontID_Alloc
	add r0, r4, #0
	bl PokemonSummary_InitText
	add r0, r4, #0
	add r1, r5, #0
	bl PokemonSummary_InitBgCells
	add r0, r4, #0
	bl PokemonSummary_ReadShownMon
	add r0, r4, #0
	bl PokemonSummary_InitSpriteSystem
	add r0, r4, #0
	bl PokemonSummary_CreateSprites
	add r0, r4, #0
	bl PokemonSummary_CreatePokepic
	add r0, r4, #0
	bl PokemonSummary_LoadBallSprite
	add r0, r4, #0
	bl PokemonSummary_UpdateStatusSprite
	add r0, r4, #0
	bl PokemonSummary_LoadPartyIcons
	add r0, r4, #0
	bl PokemonSummary_AddWindows
	add r0, r4, #0
	bl PokemonSummary_ShowFirstPage
	add r0, r4, #0
	bl PokemonSummary_CreateCamera
	ldr r0, _0208841C ; =PokemonSummary_VBlank
	add r1, r4, #0
	bl Main_SetVBlankIntrCB
	ldr r2, _02088420 ; =0x04000304
	ldrh r1, [r2]
	lsr r0, r2, #0xb
	orr r0, r1
	strh r0, [r2]
	bl GfGfx_BothDispOn
	mov r1, #0
	mov r0, #0x3d
	add r2, r1, #0
	bl Sound_SetSceneAndPlayBGM
	bl WirelessStatusIcon_CreateIfConnected
	add r0, r5, #0
	bl NARC_Delete
	add r0, r7, #0
	bl NARC_Delete
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02088408: .word 0xFFFFE0FF
_0208840C: .word 0x04001000
_02088410: .word 0xFFFF1FFF
_02088414: .word 0x000007D8
_02088418: .word 0x000007B8
_0208841C: .word PokemonSummary_VBlank
_02088420: .word 0x04000304
	thumb_func_end PokemonSummary_Init
	thumb_func_start PokemonSummary_Main
PokemonSummary_Main: ; 0x02088424
	push {r3, r4, r5, lr}
	add r4, r1, #0
	bl OverlayManager_GetData
	ldr r1, [r4]
	add r5, r0, #0
	cmp r1, #0x16
	bls _02088436
	b _0208854A
_02088436:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02088442: ; jump table
	.short _02088470 - _02088442 - 2 ; case 0
	.short _0208848C - _02088442 - 2 ; case 1
	.short _02088494 - _02088442 - 2 ; case 2
	.short _0208849C - _02088442 - 2 ; case 3
	.short _020884A4 - _02088442 - 2 ; case 4
	.short _020884AC - _02088442 - 2 ; case 5
	.short _020884BC - _02088442 - 2 ; case 6
	.short _020884B4 - _02088442 - 2 ; case 7
	.short _020884C4 - _02088442 - 2 ; case 8
	.short _020884CC - _02088442 - 2 ; case 9
	.short _020884D4 - _02088442 - 2 ; case 10
	.short _020884DC - _02088442 - 2 ; case 11
	.short _020884E4 - _02088442 - 2 ; case 12
	.short _020884EC - _02088442 - 2 ; case 13
	.short _020884F4 - _02088442 - 2 ; case 14
	.short _020884FC - _02088442 - 2 ; case 15
	.short _02088504 - _02088442 - 2 ; case 16
	.short _0208850C - _02088442 - 2 ; case 17
	.short _02088514 - _02088442 - 2 ; case 18
	.short _02088530 - _02088442 - 2 ; case 19
	.short _02088544 - _02088442 - 2 ; case 20
	.short _0208851C - _02088442 - 2 ; case 21
	.short _02088524 - _02088442 - 2 ; case 22
_02088470:
	mov r0, #0
	mov r1, #0x13
	bl sub_020880CC
	mov r0, #0x2d
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #6
	mov r2, #0
	bl Pokepic_SetAttr
	mov r0, #1
	str r0, [r4]
	b _0208854A
_0208848C:
	bl PokemonSummary_WaitFadeIn
	str r0, [r4]
	b _0208854A
_02088494:
	bl PokemonSummary_HandleInput
	str r0, [r4]
	b _0208854A
_0208849C:
	bl sub_02088D18
	str r0, [r4]
	b _0208854A
_020884A4:
	bl sub_02088D34
	str r0, [r4]
	b _0208854A
_020884AC:
	bl PokemonSummary_HandleMoveSelectInput
	str r0, [r4]
	b _0208854A
_020884B4:
	bl PokemonSummary_StartMoveSwap
	str r0, [r4]
	b _0208854A
_020884BC:
	bl PokemonSummary_HandleMoveSwapInput
	str r0, [r4]
	b _0208854A
_020884C4:
	bl PokemonSummary_HandleForgetMoveInput
	str r0, [r4]
	b _0208854A
_020884CC:
	bl PokemonSummary_HandleForgetMoveConfirmInput
	str r0, [r4]
	b _0208854A
_020884D4:
	bl PokemonSummary_OpenRibbonList
	str r0, [r4]
	b _0208854A
_020884DC:
	bl PokemonSummary_CloseRibbonList
	str r0, [r4]
	b _0208854A
_020884E4:
	bl PokemonSummary_HandleRibbonInput
	str r0, [r4]
	b _0208854A
_020884EC:
	bl PokemonSummary_WaitRibbonPageUp
	str r0, [r4]
	b _0208854A
_020884F4:
	bl PokemonSummary_WaitRibbonPageDown
	str r0, [r4]
	b _0208854A
_020884FC:
	bl PokemonSummary_FlashButton
	str r0, [r4]
	b _0208854A
_02088504:
	bl PokemonSummary_UpdateCondition
	str r0, [r4]
	b _0208854A
_0208850C:
	bl PokemonSummary_ShowConditionChanges
	str r0, [r4]
	b _0208854A
_02088514:
	bl PokemonSummary_WaitConditionExit
	str r0, [r4]
	b _0208854A
_0208851C:
	bl PokemonSummary_StartFadeOut
	str r0, [r4]
	b _0208854A
_02088524:
	bl PokemonSummary_WaitFadeOut
	cmp r0, #1
	bne _0208854A
	mov r0, #1
	pop {r3, r4, r5, pc}
_02088530:
	mov r0, #0x2d
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #6
	mov r2, #0
	bl Pokepic_SetAttr
	mov r0, #2
	str r0, [r4]
	b _0208854A
_02088544:
	bl PokemonSummary_ChangeMonByButton
	str r0, [r4]
_0208854A:
	add r0, r5, #0
	bl PokemonSummary_UpdateSpriteAnims
	add r0, r5, #0
	bl sub_0208C3C0
	mov r0, #1
	lsl r0, r0, #0xa
	ldr r0, [r5, r0]
	bl SpriteSystem_DrawSprites
	add r0, r5, #0
	bl PokemonSummary_DrawPokepic
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end PokemonSummary_Main
	thumb_func_start PokemonSummary_Exit
PokemonSummary_Exit: ; 0x0208856C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	add r0, r4, #0
	bl PokemonSummary_FreePokepicAndCamera
	add r0, r4, #0
	bl PokemonSummary_FreeSpriteSystem
	add r0, r4, #0
	bl PokemonSummary_RemoveWindows
	ldr r0, [r4]
	bl PokemonSummary_FreeBgs
	bl GF_TouchpadStopSampling
	bl GF_DestroyVramTransferManager
	add r0, r4, #0
	bl PokemonSummary_FreeBgCells
	add r0, r4, #0
	bl PokemonSummary_FreeText
	mov r0, #4
	bl FontID_Release
	ldr r0, _020885D4 ; =0x000007B8
	ldr r0, [r4, r0]
	bl NARC_Delete
	mov r0, #0
	bl FontID_SetAccessLazy
	ldr r0, _020885D8 ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, r5, #0
	bl OverlayManager_FreeData
	mov r0, #0x13
	bl Heap_Destroy
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_020885D4: .word 0x000007B8
_020885D8: .word 0x04000050
	thumb_func_end PokemonSummary_Exit

	.rodata

	.public gPokemonSummaryCancelButtonHitbox
gPokemonSummaryCancelButtonHitbox:
	.byte 0xA5, 0xBC, 0xBE, 0xF9
	.public gPokemonSummarySwapButtonHitbox
gPokemonSummarySwapButtonHitbox:
	.byte 0x88, 0x97, 0x08, 0x57
	.public gPokemonSummaryConfirmButtonHitbox
gPokemonSummaryConfirmButtonHitbox:
	.byte 0x98, 0xB7, 0x08, 0x7F
	.public gPokemonSummaryMonButtonHitboxes
gPokemonSummaryMonButtonHitboxes:
	.byte 0x28, 0x3F, 0xC0, 0xEF, 0x68, 0x7F, 0xC0, 0xEF
	.byte 0xFF, 0x00, 0x00, 0x00
	.public gPokemonSummaryGraphicsModes
gPokemonSummaryGraphicsModes:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00
	.public gPokemonSummaryMoveHitboxes
gPokemonSummaryMoveHitboxes:
	.byte 0x08, 0x27, 0x08, 0x7F, 0x28, 0x47, 0x08, 0x7F, 0x48, 0x67, 0x08, 0x7F
	.byte 0x68, 0x87, 0x08, 0x7F, 0xFF, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate1
gPokemonSummaryBgTemplate1:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x04, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate2
gPokemonSummaryBgTemplate2:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x1B, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate3
gPokemonSummaryBgTemplate3:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1A, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate4
gPokemonSummaryBgTemplate4:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x04
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate5
gPokemonSummaryBgTemplate5:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x1B, 0x02, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryBgTemplate6
gPokemonSummaryBgTemplate6:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gPokemonSummaryGraphicsBanks
gPokemonSummaryGraphicsBanks:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00
	.public gPokemonSummaryPageScreens
gPokemonSummaryPageScreens:
	.word 13, 12, 14
	.word 19, 17, 18
	.word 10, 9, 9
	.word 13, 12, 12
	.public gPokemonSummaryRibbonHitboxes
gPokemonSummaryRibbonHitboxes:
	.byte 0x08, 0x27, 0x10, 0x2F, 0x08, 0x27, 0x30, 0x4F
	.byte 0x08, 0x27, 0x50, 0x70, 0x30, 0x4F, 0x10, 0x2F, 0x30, 0x4F, 0x30, 0x4F, 0x30, 0x4F, 0x50, 0x70
	.byte 0x58, 0x77, 0x10, 0x2F, 0x58, 0x77, 0x30, 0x4F, 0x58, 0x77, 0x50, 0x70, 0x0C, 0x33, 0x74, 0x8B
	.byte 0x4C, 0x73, 0x74, 0x8B, 0xB0, 0xBF, 0xD0, 0xFF, 0xFF, 0x00, 0x00, 0x00

	; File boundary

	.public gOverlayTemplate_PokemonSummary
	.public gOverlayTemplate_PokemonSummary
gOverlayTemplate_PokemonSummary:
	.word PokemonSummary_Init, PokemonSummary_Main, PokemonSummary_Exit, 0xFFFFFFFF
