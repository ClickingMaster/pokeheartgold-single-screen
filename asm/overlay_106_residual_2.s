#include "constants/mmodel.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "overlay_106.inc"
	.include "global.inc"
	.public _021E6D70
	.public LegendaryCinematic_SetLights
	.public LegendaryCinematic_SetEdgeMarking
	.public LegendaryCinematic_LoadModels
	.public LegendaryCinematic_FreeModels
	.public LegendaryCinematic_DrawModels
	.public LegendaryCinematic_IsModelAnimating
	.public LegendaryCinematic_InitCameraAndLights
	.public LegendaryCinematic_StartCapture
	.public LegendaryCinematic_StopCapture
	.public LegendaryCinematic_CaptureTask
	.public LegendaryCinematic_CaptureStartTask
	.public LegendaryCinematic_InitCapture
	.public LugiaCinematic_Init
	.public LugiaCinematic_Play
	.public ArceusCinematic_InitBgs
	.public ArceusCinematic_InitSprites
	.public ArceusCinematic_DeleteSprite
	.public ArceusCinematic_GetOverworldModel
	.public ArceusCinematic_LoadArceusSprite
	.public ArceusCinematic_Init
	.public ArceusCinematic_Blend
	.public ArceusCinematic_RunScript
	.public ArceusCinematic_LoadBgPalette
	.public ArceusCinematic_TransferBgPalettes
	.public ArceusScriptCmd_WaitFade
	.public ov106_021E6D7C
	.public ov106_021E6D8C
	.public ov106_021E6DB0
	.public ov106_021E6DD8
	.public ov106_021E6E04
	.public ov106_021E6E2C
	.public ov106_021E6E50
	.public ov106_021E6E74
	.public ov106_021E6E9C
	.public ov106_021E6EC4
	.public ov106_021E6EE4
	.public ov106_021E6EFC
	.public ov106_021E6F24
	.public ov106_021E6F48
	.public ov106_021E6F5C
	.public ov106_021E6F74
	.public ov106_021E6F90
	.public ov106_021E6FAC
	.public ov106_021E6FC8
	.public ov106_021E6FE8
	.public ov106_021E7010
	.public ov106_021E7044
	.public ov106_021E7090
	.public ov106_021E70E0
	.public sScriptCinematicSubs_Arceus
	.public sScriptCinematicSubs_HoOh
	.public sScriptCinematicSubs_Lugia


	.text
	.public ScriptCinematic_Arceus
	.public ScriptCinematic_HoOh
	.public ScriptCinematic_Lugia
	.public LegendaryCinematic_Init3D
	.public LegendaryCinematic_Free3D
	.public LegendaryCinematic_FadeInFromBlack
	.public LegendaryCinematic_FadeOutToBlack
	.public LegendaryCinematic_FadeInFromWhite
	.public LegendaryCinematic_FadeOutToWhite
	.public LegendaryCinematic_SetCamera
	.public LegendaryCinematic_AreModelsAnimating
	.public LegendaryCinematic_SetBanks
	.public LegendaryCinematic_SetGraphicsModes
	.public LegendaryCinematic_FreeCamera
	.public LegendaryCinematic_StartMotionBlur
	.public LegendaryCinematic_StopMotionBlur
	.public HoOhCinematic_Init
	.public HoOhCinematic_Exit
	.public HoOhCinematic_WaitFade
	.public HoOhCinematic_PlayFirstScene
	.public HoOhCinematic_LoadSecondScene
	.public HoOhCinematic_PlaySecondScene
	.public LugiaCinematic_Exit
	.public LugiaCinematic_WaitFade
	.public ArceusCinematic_VBlank
	.public ArceusCinematic_SetBanks
	.public ArceusCinematic_FreeBgs
	.public ArceusCinematic_FreeSprites
	.public ArceusCinematic_UpdateSprites
	.public ArceusCinematic_LoadObjChar
	.public ArceusCinematic_LoadObjPltt
	.public ArceusCinematic_Exit
	.public ArceusCinematic_WaitFade
	.public ArceusScriptCmd_Wait
	.public ArceusScriptCmd_FadeInFromBlack
	.public ArceusScriptCmd_FadeOutToBlack
	.public ArceusScriptCmd_FadeInFromWhite
	.public ArceusScriptCmd_FadeOutToWhite
	.public ArceusScriptCmd_LoadBg
	.public ArceusScriptCmd_ToggleBg
	.public ArceusScriptCmd_SetBgPriority
	.public ArceusScriptCmd_SetBlendPlane
	.public ArceusScriptCmd_StartBlend
	.public ArceusScriptCmd_ResetBlend
	.public ArceusScriptCmd_SaveBgm
	.public ArceusScriptCmd_StopBgm
	.public ArceusScriptCmd_PlayBgm
	.public ArceusScriptCmd_SetSpriteVisible
	.public ArceusScriptCmd_SetSpritePosition
	.public ArceusScriptCmd_SetSpritePriority
	.public ArceusScriptCmd_Nop

	thumb_func_start LegendaryCinematic_InitCameraAndLights
LegendaryCinematic_InitCameraAndLights: ; 0x021E5D70
	push {r4, lr}
	sub sp, #8
	add r4, r0, #0
	mov r0, #0x99
	bl Camera_New
	str r0, [r4, #0x18]
	mov r0, #0x99
	bl GF3dRender_InitSimpleManager
	ldr r2, _021E5DF0 ; =0x04000060
	ldr r0, _021E5DF4 ; =0xFFFFCFFF
	ldrh r1, [r2]
	and r1, r0
	mov r0, #0x10
	orr r0, r1
	strh r0, [r2]
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	add r3, r0, #0
	bl G3X_SetFog
	mov r1, #0
	ldr r2, _021E5DF8 ; =0x00007FFF
	str r1, [sp]
	mov r0, #0x1f
	mov r3, #0x3f
	bl G3X_SetClearColor
	ldr r2, [r4]
	ldrh r0, [r2, #0x24]
	ldrh r1, [r2, #0x26]
	ldr r2, [r2, #0x2c]
	bl NNS_G3dGlbMaterialColorDiffAmb
	ldr r2, [r4]
	ldrh r0, [r2, #0x28]
	ldrh r1, [r2, #0x2a]
	ldr r2, [r2, #0x30]
	bl NNS_G3dGlbMaterialColorSpecEmi
	ldr r3, [r4]
	ldr r0, [r3, #0x44]
	str r0, [sp]
	ldr r0, [r3, #0x48]
	str r0, [sp, #4]
	ldr r0, [r3, #0x34]
	ldr r1, [r3, #0x38]
	ldr r2, [r3, #0x3c]
	ldr r3, [r3, #0x40]
	bl NNS_G3dGlbPolygonAttr
	mov r0, #0
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	add sp, #8
	pop {r4, pc}
	nop
_021E5DF0: .word 0x04000060
_021E5DF4: .word 0xFFFFCFFF
_021E5DF8: .word 0x00007FFF
	thumb_func_end LegendaryCinematic_InitCameraAndLights
