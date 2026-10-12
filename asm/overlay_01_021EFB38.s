#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.public Task_RunEncounterEffect
	.public EncounterEffect_LoadOverlay
	.public EncounterEffect_UnloadOverlay
	.public Task_EncounterEffectFlash
	.public EncounterEffectQuadInterp_Init
	.public EncounterEffectQuadInterp_Update
	.public StripeWipe_New
	.public StripeWipe_Start
	.public Task_StripeWipe_Update
	.public StripeWipe_End
	.public HBlank_StripeWipe
	.public SplitWipe_New
	.public SplitWipe_Start
	.public Task_SplitWipe_Update
	.public HBlank_SplitWipe
	.public SplitWipe_End
	.public EncounterEffect_LoadBgGraphics
	.public EncounterEffect_LoadBgGraphicsMirrored
	.public EncounterEffect_LoadSpriteResources
	.public RectTrail_Update
	.public MovingRect_Update
	.public BlockWipe_Update
	.public ZigzagWipe_Start
	.public ZigzagWipe_End
	.public EncounterEffect_GetDefaultEffect
	.public EncounterEffect_StartParticleMode
	.public EncounterEffect_LoadParticles
	.public Task_ParticleSetup_Main
	.public EncounterEffect_SetupParticleGraphics
	.public EncounterEffectParticles_AllocTexVram
	.public EncounterEffectParticles_AllocPlttVram
	.public EncounterEffectHBlankWave_AlternateDirection
	.include "overlay_01_021EFB38.inc"
	.include "global.inc"

	.text
	.public ov01_022067D0
	.public sParticleBaseScale
	.public sParticleGraphicsModes
	.public sParticleCaptureBgTemplate
	.public sParticleGraphicsBanks
	.public sEncounterEffectFuncs
	.public sBlockWipeColumnOrder
	.public gEncounterEffectState
	.public EncounterEffect_Start
	.public EncounterEffect_Finish
	.public EncounterEffect_StartFlash
	.public ov01_021EFE30
	.public EncounterEffectInterp_Init
	.public EncounterEffectInterp_Update
	.public EncounterEffectInterp_InitFx
	.public EncounterEffectInterp_UpdateFx
	.public EncounterEffect_SetMasterBrightness
	.public BrightnessInterp_Init
	.public BrightnessInterp_Update
	.public Task_ApplyBrightness
	.public StripeWipe_Free
	.public Task_StripeWipe_Begin
	.public SplitWipe_Free
	.public Task_SplitWipe_Begin
	.public EncounterEffect_InitSprites
	.public EncounterEffect_FreeSprites
	.public EncounterEffect_FreeSpriteResources
	.public EncounterEffect_CreateSprite
	.public EncounterEffect_SetVecFx32
	.public RectTrail_New
	.public RectTrail_Free
	.public RectTrail_Start
	.public MovingRect_New
	.public MovingRect_Free
	.public MovingRect_Start
	.public EncounterEffect_FillWindowRect
	.public BlockWipe_New
	.public BlockWipe_Free
	.public BlockWipe_Start
	.public ZigzagWipe_New
	.public ZigzagWipe_Free
	.public Task_ZigzagWipe_Begin
	.public Task_ZigzagWipe_Update
	.public HBlank_ZigzagWipe
	.public Task_ApplyMainBrightness
	.public EncounterEffect_ApplyMainBrightness
	.public EncounterEffect_RequestParticleSetup
	.public EncounterEffect_IsParticleSetupDone
	.public EncounterEffect_DrawParticles
	.public EncounterEffect_EndParticleMode
	.public EncounterEffect_CreateEmitters
	.public EncounterEffect_UpdateParticles
	.public EncounterEffect_AreParticlesDone
	.public EncounterEffect_FreeParticles
	.public Task_ParticleSetup_VWait
	.public EncounterEffect_FreeParticleGraphics
	.public EncounterEffectHBlankWave_StartTask
	.public Task_EncounterEffectHBlankWave
	.public EncounterEffectHBlankWave_Init
	.public EncounterEffectHBlankWave_Free
	.public EncounterEffectHBlankWave_Start

	thumb_func_start Task_RunEncounterEffect
Task_RunEncounterEffect: ; 0x021EFB38
	push {r3, lr}
	ldr r2, _021EFB5C ; =gEncounterEffectState
	ldr r2, [r2, #4]
	lsl r3, r2, #2
	ldr r2, _021EFB60 ; =sEncounterEffectFuncs
	ldr r2, [r2, r3]
	blx r2
	ldr r0, _021EFB5C ; =gEncounterEffectState
	ldr r1, [r0, #8]
	add r1, r1, #1
	str r1, [r0, #8]
	bl OS_GetTick
	mov r0, #0
	add r1, r0, #0
	bl OS_SetTick
	pop {r3, pc}
	.balign 4, 0
_021EFB5C: .word gEncounterEffectState
_021EFB60: .word sEncounterEffectFuncs
	thumb_func_end Task_RunEncounterEffect


	thumb_func_start EncounterEffect_LoadOverlay
EncounterEffect_LoadOverlay: ; 0x021EFB64
	push {r3, lr}
	cmp r0, #0xc
	blt _021EFB78
	cmp r0, #0x21
	bgt _021EFB78
	ldr r0, _021EFBE8 ; =FS_OVERLAY_ID(OVY_115)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFB78:
	add r1, r0, #0
	sub r1, #0x22
	cmp r1, #2
	bhi _021EFB8A
	ldr r0, _021EFBEC ; =FS_OVERLAY_ID(OVY_116)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFB8A:
	cmp r0, #0x27
	blt _021EFB9C
	cmp r0, #0x2c
	bgt _021EFB9C
	ldr r0, _021EFBF0 ; =FS_OVERLAY_ID(OVY_117)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFB9C:
	cmp r0, #0x2d
	bne _021EFBAA
	ldr r0, _021EFBF4 ; =FS_OVERLAY_ID(OVY_118)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFBAA:
	cmp r0, #0
	blt _021EFBBC
	cmp r0, #5
	bgt _021EFBBC
	ldr r0, _021EFBF8 ; =FS_OVERLAY_ID(OVY_120)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFBBC:
	cmp r0, #6
	blt _021EFBCE
	cmp r0, #0xb
	bgt _021EFBCE
	ldr r0, _021EFBFC ; =FS_OVERLAY_ID(OVY_119)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFBCE:
	cmp r0, #0x2e
	bne _021EFBDC
	ldr r0, _021EFBF8 ; =FS_OVERLAY_ID(OVY_120)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
_021EFBDC:
	ldr r0, _021EFC00 ; =FS_OVERLAY_ID(OVY_114)
	mov r1, #2
	bl HandleLoadOverlay
	pop {r3, pc}
	nop
_021EFBE8: .word FS_OVERLAY_ID(OVY_115)
_021EFBEC: .word FS_OVERLAY_ID(OVY_116)
_021EFBF0: .word FS_OVERLAY_ID(OVY_117)
_021EFBF4: .word FS_OVERLAY_ID(OVY_118)
_021EFBF8: .word FS_OVERLAY_ID(OVY_120)
_021EFBFC: .word FS_OVERLAY_ID(OVY_119)
_021EFC00: .word FS_OVERLAY_ID(OVY_114)
	thumb_func_end EncounterEffect_LoadOverlay


	thumb_func_start EncounterEffect_UnloadOverlay
EncounterEffect_UnloadOverlay: ; 0x021EFC04
	push {r3, lr}
	cmp r0, #0xc
	blt _021EFC16
	cmp r0, #0x21
	bgt _021EFC16
	ldr r0, _021EFC78 ; =FS_OVERLAY_ID(OVY_115)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC16:
	add r1, r0, #0
	sub r1, #0x22
	cmp r1, #2
	bhi _021EFC26
	ldr r0, _021EFC7C ; =FS_OVERLAY_ID(OVY_116)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC26:
	cmp r0, #0x27
	blt _021EFC36
	cmp r0, #0x2c
	bgt _021EFC36
	ldr r0, _021EFC80 ; =FS_OVERLAY_ID(OVY_117)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC36:
	cmp r0, #0x2d
	bne _021EFC42
	ldr r0, _021EFC84 ; =FS_OVERLAY_ID(OVY_118)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC42:
	cmp r0, #0
	blt _021EFC52
	cmp r0, #5
	bgt _021EFC52
	ldr r0, _021EFC88 ; =FS_OVERLAY_ID(OVY_120)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC52:
	cmp r0, #6
	blt _021EFC62
	cmp r0, #0xb
	bgt _021EFC62
	ldr r0, _021EFC8C ; =FS_OVERLAY_ID(OVY_119)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC62:
	cmp r0, #0x2e
	bne _021EFC6E
	ldr r0, _021EFC88 ; =FS_OVERLAY_ID(OVY_120)
	bl UnloadOverlayByID
	pop {r3, pc}
_021EFC6E:
	ldr r0, _021EFC90 ; =FS_OVERLAY_ID(OVY_114)
	bl UnloadOverlayByID
	pop {r3, pc}
	nop
_021EFC78: .word FS_OVERLAY_ID(OVY_115)
_021EFC7C: .word FS_OVERLAY_ID(OVY_116)
_021EFC80: .word FS_OVERLAY_ID(OVY_117)
_021EFC84: .word FS_OVERLAY_ID(OVY_118)
_021EFC88: .word FS_OVERLAY_ID(OVY_120)
_021EFC8C: .word FS_OVERLAY_ID(OVY_119)
_021EFC90: .word FS_OVERLAY_ID(OVY_114)
	thumb_func_end EncounterEffect_UnloadOverlay
