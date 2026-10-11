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

	thumb_func_start VisitorProfilesExitPrompt_Open
VisitorProfilesExitPrompt_Open: ; 0x02259F48
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	add r0, #0x10
	mov r1, #0xf
	bl FillWindowPixelBuffer
	add r0, r4, #0
	mov r1, #1
	add r0, #0x10
	add r2, r1, #0
	mov r3, #2
	bl DrawFrameAndWindow2
	add r0, r4, #0
	add r0, #0x10
	bl ScheduleWindowCopyToVram
	mov r3, #0
	str r3, [sp]
	ldr r0, [r4, #8]
	mov r1, #1
	str r0, [sp, #4]
	str r3, [sp, #8]
	add r0, r4, #0
	ldr r2, [r4, #0xc]
	add r0, #0x10
	bl AddTextPrinterParameterized
	str r0, [r4, #4]
	mov r0, #0
	str r0, [r4]
	add sp, #0xc
	pop {r3, r4, pc}
	thumb_func_end VisitorProfilesExitPrompt_Open


	thumb_func_start VisitorProfilesExitPrompt_HandleInput
VisitorProfilesExitPrompt_HandleInput: ; 0x02259F8C
	push {r4, r5, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, [r5]
	cmp r0, #0
	beq _02259F9E
	cmp r0, #1
	beq _02259FD8
	b _02259FE0
_02259F9E:
	ldr r0, [r5, #4]
	mov r4, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _02259FE0
	ldr r3, _02259FE8 ; =sVisitorProfilesYesNoTemplate
	add r2, sp, #0
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	add r0, r5, #0
	add r0, #0x10
	bl GetWindowBgConfig
	str r0, [sp]
	ldr r0, [r5, #0x20]
	add r1, sp, #0
	bl YesNoPrompt_InitFromTemplate
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _02259FE0
_02259FD8:
	ldr r0, [r5, #0x20]
	bl YesNoPrompt_HandleInput
	add r4, r0, #0
_02259FE0:
	add r0, r4, #0
	add sp, #0x14
	pop {r4, r5, pc}
	nop
_02259FE8: .word sVisitorProfilesYesNoTemplate
	thumb_func_end VisitorProfilesExitPrompt_HandleInput
