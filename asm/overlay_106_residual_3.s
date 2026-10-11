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

	thumb_func_start LegendaryCinematic_StartCapture
LegendaryCinematic_StartCapture: ; 0x021E5E0C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _021E5E54 ; =LegendaryCinematic_CaptureTask
	ldr r3, [r5, #0x28]
	mov r1, #0x38
	mov r2, #5
	bl CreateSysTaskAndEnvironment
	add r6, r0, #0
	bl SysTask_GetData
	add r4, r0, #0
	add r2, r4, #4
	mov r3, #5
_021E5E28:
	ldmia r5!, {r0, r1}
	stmia r2!, {r0, r1}
	sub r3, r3, #1
	bne _021E5E28
	ldr r0, [r5]
	str r0, [r2]
	str r6, [r4, #0x34]
	mov r0, #0
	str r0, [r4, #0x30]
	bl GX_GetBankForLCDC
	str r0, [r4]
	add r0, r4, #4
	bl LegendaryCinematic_InitCapture
	ldr r0, _021E5E58 ; =LegendaryCinematic_CaptureStartTask
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnVWaitQueue
	add r0, r4, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021E5E54: .word LegendaryCinematic_CaptureTask
_021E5E58: .word LegendaryCinematic_CaptureStartTask
	thumb_func_end LegendaryCinematic_StartCapture


	thumb_func_start LegendaryCinematic_StopCapture
LegendaryCinematic_StopCapture: ; 0x021E5E5C
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	add r1, r2, #0
	add r2, r3, #0
	bl GX_SetGraphicsMode
	ldr r0, [r4]
	bl GX_SetBankForLCDC
	ldr r0, [r4, #4]
	cmp r0, #0xa
	bgt _021E5E88
	bge _021E5EAC
	cmp r0, #6
	bgt _021E5EC8
	cmp r0, #2
	blt _021E5EC8
	beq _021E5E8E
	cmp r0, #6
	beq _021E5E9E
	b _021E5EC8
_021E5E88:
	cmp r0, #0xe
	beq _021E5EBA
	b _021E5EC8
_021E5E8E:
	mov r1, #0x1a
	mov r2, #2
	mov r0, #0
	lsl r1, r1, #0x16
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5ECE
_021E5E9E:
	mov r2, #2
	ldr r1, _021E5ED8 ; =0x06820000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5ECE
_021E5EAC:
	mov r2, #2
	ldr r1, _021E5EDC ; =0x06840000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5ECE
_021E5EBA:
	mov r2, #2
	ldr r1, _021E5EE0 ; =0x06860000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5ECE
_021E5EC8:
	mov r0, #0
	bl GX_SetBankForLCDC
_021E5ECE:
	ldr r0, [r4, #0x34]
	bl DestroySysTaskAndEnvironment
	pop {r4, pc}
	nop
_021E5ED8: .word 0x06820000
_021E5EDC: .word 0x06840000
_021E5EE0: .word 0x06860000
	thumb_func_end LegendaryCinematic_StopCapture


	thumb_func_start LegendaryCinematic_CaptureTask
LegendaryCinematic_CaptureTask: ; 0x021E5EE4
	push {r4, r5, r6, r7}
	ldr r0, [r1, #0x30]
	cmp r0, #0
	beq _021E5F1C
	ldr r6, [r1, #0x20]
	ldr r5, [r1, #0x10]
	ldr r4, [r1, #0x1c]
	ldr r3, [r1, #0x18]
	ldr r2, [r1, #0x14]
	ldr r0, [r1, #0x24]
	ldr r1, [r1, #0x28]
	lsl r5, r5, #0x14
	lsl r7, r1, #8
	lsl r1, r3, #0x18
	lsl r3, r2, #0x1d
	mov r2, #2
	lsl r2, r2, #0x1e
	orr r3, r2
	lsl r2, r4, #0x19
	orr r2, r3
	orr r1, r2
	lsl r6, r6, #0x10
	orr r1, r5
	orr r1, r6
	orr r1, r7
	orr r1, r0
	ldr r0, _021E5F20 ; =0x04000064
	str r1, [r0]
_021E5F1C:
	pop {r4, r5, r6, r7}
	bx lr
	.balign 4, 0
_021E5F20: .word 0x04000064
	thumb_func_end LegendaryCinematic_CaptureTask


	thumb_func_start LegendaryCinematic_CaptureStartTask
LegendaryCinematic_CaptureStartTask: ; 0x021E5F24
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #4]
	cmp r0, #0xa
	bgt _021E5F42
	bge _021E5F58
	cmp r0, #6
	bgt _021E5F68
	cmp r0, #2
	blt _021E5F68
	beq _021E5F48
	cmp r0, #6
	beq _021E5F50
	b _021E5F68
_021E5F42:
	cmp r0, #0xe
	beq _021E5F60
	b _021E5F68
_021E5F48:
	mov r0, #1
	bl GX_SetBankForLCDC
	b _021E5F6E
_021E5F50:
	mov r0, #2
	bl GX_SetBankForLCDC
	b _021E5F6E
_021E5F58:
	mov r0, #4
	bl GX_SetBankForLCDC
	b _021E5F6E
_021E5F60:
	mov r0, #8
	bl GX_SetBankForLCDC
	b _021E5F6E
_021E5F68:
	mov r0, #0
	bl GX_SetBankForLCDC
_021E5F6E:
	ldr r0, [r4, #4]
	ldr r1, [r4, #8]
	ldr r2, [r4, #0xc]
	bl GX_SetGraphicsMode
	mov r0, #1
	str r0, [r4, #0x30]
	add r0, r5, #0
	bl SysTask_Destroy
	pop {r3, r4, r5, pc}
	thumb_func_end LegendaryCinematic_CaptureStartTask


	thumb_func_start LegendaryCinematic_InitCapture
LegendaryCinematic_InitCapture: ; 0x021E5F84
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r0, [r4]
	cmp r0, #0xa
	bgt _021E5FA0
	bge _021E5FC4
	cmp r0, #6
	bgt _021E5FDE
	cmp r0, #2
	blt _021E5FDE
	beq _021E5FA6
	cmp r0, #6
	beq _021E5FB6
	b _021E5FDE
_021E5FA0:
	cmp r0, #0xe
	beq _021E5FD2
	b _021E5FDE
_021E5FA6:
	mov r1, #0x1a
	mov r2, #2
	mov r0, #0
	lsl r1, r1, #0x16
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5FDE
_021E5FB6:
	mov r2, #2
	ldr r1, _021E600C ; =0x06820000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5FDE
_021E5FC4:
	mov r2, #2
	ldr r1, _021E6010 ; =0x06840000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
	b _021E5FDE
_021E5FD2:
	mov r2, #2
	ldr r1, _021E6014 ; =0x06860000
	mov r0, #0
	lsl r2, r2, #0x10
	bl MIi_CpuClearFast
_021E5FDE:
	ldr r5, [r4, #0x10]
	ldr r1, [r4, #0x1c]
	ldr r3, [r4, #0x18]
	ldr r6, [r4, #0x14]
	ldr r0, [r4, #0xc]
	mov r4, #2
	lsl r2, r1, #0x10
	lsl r1, r0, #0x14
	lsl r5, r5, #0x1d
	lsl r4, r4, #0x1e
	orr r4, r5
	lsl r3, r3, #0x19
	lsl r0, r6, #0x18
	orr r3, r4
	orr r0, r3
	orr r0, r1
	add r1, r2, #0
	orr r1, r0
	mov r0, #0x10
	orr r1, r0
	ldr r0, _021E6018 ; =0x04000064
	str r1, [r0]
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021E600C: .word 0x06820000
_021E6010: .word 0x06840000
_021E6014: .word 0x06860000
_021E6018: .word 0x04000064
	thumb_func_end LegendaryCinematic_InitCapture
