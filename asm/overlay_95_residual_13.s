	.include "asm/macros.inc"
	.include "overlay_95.inc"
	.include "global.inc"
	.public HatchEggApp_Exit
	.public HatchEggApp_Init
	.public HatchEggApp_Main
	.public HatchEggApp_Create3DVramMan
	.public HatchEggApp_Init3D
	.public HatchEggApp_InitParticleVram
	.public HatchEggApp_InitBgs
	.public HatchEggApp_LoadWindowGfx
	.public HatchEggApp_CreateMessageWindow
	.public HatchEggApp_PrintMessage
	.public HatchEggApp_LoadBackground
	.public HatchEggApp_AllocParticleTexVram
	.public HatchEggApp_AllocParticlePlttVram
	.public HatchEggApp_CreateParticleSystem
	.public HatchEggApp_CreateParticleSystemFromNarc
	.public HatchEggParticles_New
	.public HatchEggParticles_Emit
	.public HatchEggApp_LoadSpriteResources
	.public HatchEggApp_CreateSprites
	.public HatchEggApp_StepEggShake
	.public HatchEggApp_InitSpriteSystem
	.public HatchEggApp_CreatePokepic
	.public HatchEggApp_StartPokepicAnim
	.public HatchEggApp_RunEggScene
	.public HatchEggApp_RunManaphyEggScene
	.public HatchEggApp_ShakeEggOnce
	.public HatchEggApp_ShakeEggTwice
	.public HatchEggApp_ShakeEggCrack
	.public HatchEggApp_HatchEgg
	.public HatchEggApp_HatchManaphyEgg
	.public HatchEggYesNoMenu_New
	.public HatchEggYesNoMenu_LoadGfx
	.public HatchEggYesNoMenu_Run
	.public HatchEggYesNoMenu_RemoveWindows
	.public HatchEggYesNoMenu_PrintButton
	.public HatchEggYesNoMenu_CreateCursor
	.public HatchEggYesNoMenu_UpdateCursor
	.public HatchEggYesNoMenu_HandleInput
	.public ov95_021E7538
	.public ov95_021E7548
	.public ov95_021E755C
	.public ov95_021E7574
	.public ov95_021E7594
	.public ov95_021E75B4
	.public ov95_021E75DC
	.public ov95_021E762C
	.public ov95_021E767C
	.public ov95_021E76D0
	.public ov95_021E7770
	.public ov95_021E7810
	.public ov95_021E7818
	.public ov95_021E7820
	.public ov95_021E782C
	.public ov95_021E7860


	.text
	.public HatchEggApp_HideLayers
	.public HatchEggApp_InitBlend
	.public HatchEggApp_VBlankCB
	.public HatchEggApp_Draw3D
	.public HatchEggApp_FadeIn
	.public HatchEggApp_FadeOut
	.public HatchEggApp_RemoveMessageWindow
	.public HatchEggApp_DestroyParticleSystem
	.public HatchEggParticles_EmitterCB
	.public HatchEggParticles_IsRunning
	.public HatchEggParticles_Free
	.public HatchEggApp_UpdateEggShake
	.public HatchEggApp_DeleteSprites
	.public HatchEggApp_FreeSpriteSystem
	.public HatchEggApp_DeletePokepic
	.public HatchEggApp_SetPokepicVanished
	.public HatchEggApp_RunScene
	.public HatchEggApp_WaitBeforeShake
	.public HatchEggYesNoMenu_InitBgs
	.public HatchEggYesNoMenu_Free
	.public HatchEggYesNoMenu_Show
	.public HatchEggYesNoMenu_AddWindows
	.public HatchEggYesNoMenu_DeleteCursor
	.public HatchEggYesNoMenu_IsPressAnimFinished

	thumb_func_start HatchEggYesNoMenu_RemoveWindows
HatchEggYesNoMenu_RemoveWindows: ; 0x021E7308
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bne _021E7312
	bl GF_AssertFail
_021E7312:
	mov r4, #0
	add r5, #0x1c
_021E7316:
	add r0, r5, #0
	bl RemoveWindow
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #2
	blt _021E7316
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end HatchEggYesNoMenu_RemoveWindows


	thumb_func_start HatchEggYesNoMenu_PrintButton
HatchEggYesNoMenu_PrintButton: ; 0x021E7328
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r3, [r5]
	add r4, r1, #0
	add r6, r2, #0
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0xbb
	bl NewMsgDataFromNarc
	add r7, r0, #0
	ldr r0, [r5, #0x14]
	ldr r3, [r5]
	add r1, r7, #0
	add r2, r6, #0
	bl ReadMsgData_ExpandPlaceholders
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r3, #0
	str r3, [sp]
	ldr r0, _021E7384 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	add r0, r4, #0
	mov r1, #4
	add r2, r5, #0
	str r3, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r4, #0
	bl CopyWindowToVram
	add r0, r7, #0
	bl DestroyMsgData
	add r0, r5, #0
	bl String_Delete
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E7384: .word 0x00010200
	thumb_func_end HatchEggYesNoMenu_PrintButton


	thumb_func_start HatchEggYesNoMenu_CreateCursor
HatchEggYesNoMenu_CreateCursor: ; 0x021E7388
	push {r4, r5, r6, r7, lr}
	sub sp, #0x3c
	ldr r4, _021E7400 ; =ov95_021E782C
	str r2, [sp]
	str r3, [sp, #4]
	add r5, r0, #0
	add r7, r1, #0
	add r3, sp, #8
	mov r2, #6
_021E739A:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _021E739A
	ldr r0, [r4]
	cmp r5, #0
	str r0, [r3]
	ldr r6, [r5, #4]
	ldr r4, [r5, #8]
	bne _021E73B2
	bl GF_AssertFail
_021E73B2:
	cmp r6, #0
	bne _021E73BA
	bl GF_AssertFail
_021E73BA:
	cmp r4, #0
	bne _021E73C2
	bl GF_AssertFail
_021E73C2:
	mov r1, #0x80
	add r0, sp, #8
	strh r1, [r0]
	mov r1, #0x48
	strh r1, [r0, #2]
	mov r1, #1
	strh r1, [r0, #6]
	ldr r0, [sp]
	add r1, r4, #0
	str r0, [sp, #0x20]
	ldr r0, [sp, #4]
	add r2, sp, #8
	str r0, [sp, #0x24]
	ldr r0, [sp, #0x50]
	str r7, [sp, #0x1c]
	str r0, [sp, #0x28]
	add r0, r6, #0
	bl SpriteSystem_NewSprite
	add r4, r0, #0
	mov r1, #1
	bl ManagedSprite_SetAnimateFlag
	add r0, r4, #0
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	str r4, [r5, #0x10]
	add sp, #0x3c
	pop {r4, r5, r6, r7, pc}
	nop
_021E7400: .word ov95_021E782C
	thumb_func_end HatchEggYesNoMenu_CreateCursor
