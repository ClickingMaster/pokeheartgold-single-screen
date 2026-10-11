	.include "asm/macros.inc"
	.include "overlay_48.inc"
	.include "global.inc"

	.text
	.public sVisitorProfilesBgAnimPeriods
	.public VisitorProfiles_Init
	.public VisitorProfiles_Main
	.public VisitorProfiles_Exit
	.public VisitorProfilesPlaces_Load
	.public VisitorProfilesPlaces_Set
	.public VisitorProfiles_CalcGlobeMatrix
	.public VisitorProfiles_CalcPlaceMatrix
	.public VisitorProfiles_GetTouchDrag
	.public VisitorProfiles_HandleGlobeInput
	.public VisitorProfiles_WrapAngles
	.public VisitorProfiles_Update
	.public VisitorProfiles_HandleInput
	.public VisitorProfiles_FocusStartPlace
	.public VisitorProfiles_PickPlace
	.public VisitorProfiles_FindNearestPlace
	.public VisitorProfiles_CalcLocalHour
	.public VisitorProfiles_FocusPlace
	.public VisitorProfiles_InitSettings
	.public VisitorProfiles_InitGraphics
	.public VisitorProfiles_InitBgs
	.public VisitorProfiles_FreeBgs
	.public VisitorProfiles_InitSprites
	.public VisitorProfiles_Init3D
	.public VisitorProfiles_Setup3D
	.public VisitorProfilesPlaces_Init
	.public VisitorProfilesPlaces_Draw
	.public VisitorProfiles_GetVisitorMarker
	.public VisitorProfilesPlaces_FindIndex
	.public VisitorProfilesCamera_Init
	.public VisitorProfilesCamera_UpdateZoom
	.public VisitorProfilesTouch_Init
	.public VisitorProfilesTouch_Update
	.public VisitorProfilesExitPrompt_Init
	.public VisitorProfilesExitPrompt_Open
	.public VisitorProfilesExitPrompt_HandleInput
	.public VisitorProfilesPanel_Init
	.public VisitorProfilesPanel_Free
	.public VisitorProfilesPanel_Update
	.public VisitorProfilesPanel_AddPlace
	.public VisitorProfilesPanel_PushHistory
	.public VisitorProfilesPanel_AnimateBgs
	.public VisitorProfilesPanel_StartReveal
	.public ov48_0225A428
	.public VisitorProfilesPanel_Reveal
	.public VisitorProfilesPanel_UpdateRow
	.public VisitorProfilesPanel_CopyBaseRow
	.public VisitorProfilesWindowMask_Init
	.public VisitorProfilesWindowMask_Task
	.public VisitorProfilesPanel_AddWindows
	.public VisitorProfilesPanel_RemoveWindows
	.public VisitorProfilesPanel_PrintPlace
	.public VisitorProfilesPanel_TransferRowPixels
	.public VisitorProfilesBgAnim_Init
	.public VisitorProfilesBgAnim_Free
	.public VisitorProfilesBgAnim_Update
	.public VisitorProfilesBgAnim_SetEnabled
	.public VisitorProfilesBgAnim_CopyRow
	.public VisitorProfilesHourSprites_Init
	.public VisitorProfilesHourSprites_Free
	.public VisitorProfilesHourSprites_Reset
	.public VisitorProfilesHourSprites_Set
	.public VisitorProfilesHourSprites_Update
	.public VisitorProfilesHourSprites_Start
	.public VisitorProfilesHourSprites_Step
	.public VisitorProfilesHourSpriteMotion_CalcPosition
	.public VisitorProfilesHourSpriteMotion_UpdateWindowMask
	.public VisitorProfilesHourSprites_UpdateSprite
	.public sVisitorProfilesCameraPos
	.public sVisitorProfilesMarkerModelFiles
	.public sVisitorProfilesCameraTarget
	.public sVisitorProfilesGraphicsModes
	.public sVisitorProfilesObjCharTransferTemplate
	.public sVisitorProfilesBgLayers
	.public sVisitorProfilesYesNoTemplate
	.public sVisitorProfilesPlaceNameColors
	.public sVisitorProfilesIdentityMtx
	.public sVisitorProfilesGraphicsBanks
	.public sVisitorProfilesHourAnims
	.public ov48_0225B239
	.public sVisitorProfilesBgAnimTemplates
	.public sVisitorProfilesBgTemplates
	.public sVisitorProfilesHourSpritePaths
	.public VisitorProfiles_UpdatePanel
	.public VisitorProfiles_Draw
	.public VisitorProfiles_VBlankCB
	.public VisitorProfiles_AngleDistance
	.public VisitorProfiles_FreeGraphics
	.public VisitorProfiles_DrawSprites
	.public VisitorProfiles_GraphicsVBlank
	.public VisitorProfiles_FreeSprites
	.public VisitorProfiles_Free3D
	.public VisitorProfilesGlobe_Init
	.public VisitorProfilesGlobe_Free
	.public VisitorProfilesGlobe_Draw
	.public VisitorProfilesGlobe_GetPosition
	.public VisitorProfilesGlobe_GetMatrix
	.public VisitorProfilesGlobe_GetRotation
	.public VisitorProfilesGlobe_SetRotation
	.public VisitorProfilesPlaces_Free
	.public VisitorProfilesPlaces_Find
	.public VisitorProfilesPlaces_GetCoords
	.public VisitorProfilesPlaces_GetMarker
	.public VisitorProfilesPlaces_GetCountry
	.public VisitorProfilesPlaces_GetRegion
	.public VisitorProfilesPlaces_GetCount
	.public VisitorProfilesCamera_Free
	.public VisitorProfilesCamera_Apply
	.public VisitorProfilesCamera_ToggleZoom
	.public VisitorProfilesCamera_GetZoomMode
	.public VisitorProfilesTouch_Free
	.public VisitorProfilesTouch_Get
	.public VisitorProfilesTouch_HideExitButton
	.public VisitorProfilesTouch_ShowExitButton
	.public VisitorProfilesExitPrompt_Free
	.public VisitorProfilesExitPrompt_Close
	.public VisitorProfilesPanel_HideHint
	.public VisitorProfilesPanel_ShowHint
	.public VisitorProfilesPanel_HasPlace
	.public VisitorProfilesPanel_InitHistory
	.public VisitorProfilesPanel_ClearHistory
	.public VisitorProfilesPanel_PopHistory
	.public VisitorProfilesPanel_GetHistory
	.public VisitorProfilesPanel_RevealSlow
	.public VisitorProfilesPanel_RevealFast
	.public VisitorProfilesPanel_ActivateRow
	.public VisitorProfilesWindowMask_Free
	.public VisitorProfilesWindowMask_SetWindow0
	.public VisitorProfilesWindowMask_SetWindow1
	.public VisitorProfilesPanel_GetRowPixels
	.public VisitorProfilesPanel_GetBlankRowPixels
	.public VisitorProfilesBgAnim_DisableAll
	.public VisitorProfilesHourSprites_HideAll
	.public VisitorProfilesHourSpriteMotion_Init
	.public VisitorProfilesHourSpriteMotion_SetJoinNext
	.public VisitorProfilesHourSpriteMotion_SetJoinPrev
	.public VisitorProfilesHourSpriteMotion_Step
	.public VisitorProfilesHourSpriteMotion_Start
	.public VisitorProfilesHourSpriteMotion_GetPosition
	.public VisitorProfiles_GetHourBgAnim
	.public VisitorProfiles_GetHourSprite
	.public VisitorProfilesMsg_Init
	.public VisitorProfilesMsg_Free
	.public VisitorProfilesMsg_Get
	.public VisitorProfilesMsg_Read
	.public VisitorProfilesMsg_GetCountryName
	.public VisitorProfilesMsg_GetRegionName
	.public VisitorProfiles_FindFirstVisitor

	thumb_func_start VisitorProfilesHourSpriteMotion_CalcPosition
VisitorProfilesHourSpriteMotion_CalcPosition: ; 0x0225AEDC
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldrh r0, [r5, #0xa]
	ldr r2, [r5, #0xc]
	cmp r0, #0
	bne _0225AEF4
	mov r0, #6
	ldrsh r0, [r2, r0]
	sub r0, #0x10
	lsl r0, r0, #0x10
	asr r7, r0, #0x10
	b _0225AEF8
_0225AEF4:
	mov r0, #6
	ldrsh r7, [r2, r0]
_0225AEF8:
	ldrh r0, [r5, #8]
	cmp r0, #0
	bne _0225AF0A
	mov r0, #2
	ldrsh r0, [r2, r0]
	add r0, #0x10
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	b _0225AF0E
_0225AF0A:
	mov r0, #2
	ldrsh r4, [r2, r0]
_0225AF0E:
	mov r0, #0
	ldrsh r6, [r2, r0]
	mov r0, #4
	ldrsh r1, [r5, r0]
	ldrsh r0, [r2, r0]
	sub r0, r0, r6
	mul r0, r1
	mov r1, #8
	ldrsh r1, [r2, r1]
	bl _s32_div_f
	add r0, r6, r0
	strh r0, [r5]
	mov r0, #4
	ldrsh r1, [r5, r0]
	sub r0, r7, r4
	ldr r2, [r5, #0xc]
	mul r0, r1
	mov r1, #8
	ldrsh r1, [r2, r1]
	bl _s32_div_f
	add r0, r4, r0
	strh r0, [r5, #2]
	mov r0, #6
	ldrsh r1, [r5, r0]
	ldr r0, _0225AFAC ; =0x0000FFFF
	ldr r4, [r5, #0xc]
	mul r0, r1
	mov r1, #0xc
	ldrsh r1, [r4, r1]
	bl _s32_div_f
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	asr r0, r0, #4
	lsl r1, r0, #2
	ldr r0, _0225AFB0 ; =FX_SinCosTable_
	ldrsh r6, [r0, r1]
	mov r0, #0xa
	ldrsh r0, [r4, r0]
	cmp r0, #0
	ble _0225AF76
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0225AF84
_0225AF76:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0225AF84:
	bl _ffix
	asr r1, r0, #0x1f
	asr r3, r6, #0x1f
	add r2, r6, #0
	bl _ll_mul
	mov r3, #2
	mov r4, #0
	lsl r3, r3, #0xa
	add r3, r0, r3
	adc r1, r4
	lsl r0, r1, #0x14
	lsr r1, r3, #0xc
	orr r1, r0
	ldrsh r2, [r5, r4]
	asr r0, r1, #0xc
	add r0, r2, r0
	strh r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225AFAC: .word 0x0000FFFF
_0225AFB0: .word FX_SinCosTable_
	thumb_func_end VisitorProfilesHourSpriteMotion_CalcPosition


	thumb_func_start VisitorProfilesHourSpriteMotion_UpdateWindowMask
VisitorProfilesHourSpriteMotion_UpdateWindowMask: ; 0x0225AFB4
	push {r3, r4, r5, lr}
	add r4, r1, #0
	mov r1, #0
	add r5, r0, #0
	add r0, r4, #0
	add r2, r1, #0
	bl VisitorProfilesWindowMask_SetWindow0
	ldrh r0, [r5, #8]
	cmp r0, #0
	bne _0225AFE8
	ldr r1, [r5, #0xc]
	mov r0, #2
	ldrsh r1, [r1, r0]
	ldrsh r2, [r5, r0]
	add r0, r1, #0
	sub r0, #0x10
	cmp r2, r0
	ble _0225AFE8
	add r2, r1, #0
	add r2, #0x20
	lsl r2, r2, #0x10
	add r0, r4, #0
	asr r2, r2, #0x10
	bl VisitorProfilesWindowMask_SetWindow0
_0225AFE8:
	ldrh r0, [r5, #0xa]
	cmp r0, #0
	bne _0225B00C
	ldr r1, [r5, #0xc]
	mov r0, #6
	ldrsh r2, [r1, r0]
	mov r0, #2
	ldrsh r0, [r5, r0]
	sub r0, #0x10
	cmp r0, r2
	bgt _0225B00C
	add r1, r2, #0
	sub r1, #0x20
	lsl r1, r1, #0x10
	add r0, r4, #0
	asr r1, r1, #0x10
	bl VisitorProfilesWindowMask_SetWindow0
_0225B00C:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end VisitorProfilesHourSpriteMotion_UpdateWindowMask


	thumb_func_start VisitorProfilesHourSprites_UpdateSprite
VisitorProfilesHourSprites_UpdateSprite: ; 0x0225B010
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	lsl r0, r4, #4
	add r0, r5, r0
	add r1, sp, #0
	bl VisitorProfilesHourSpriteMotion_GetPosition
	add r0, r5, r4
	add r0, #0x38
	ldrb r0, [r0]
	add r1, sp, #0
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x3c]
	bl Sprite_SetMatrix
	add sp, #0xc
	pop {r4, r5, pc}
	thumb_func_end VisitorProfilesHourSprites_UpdateSprite
