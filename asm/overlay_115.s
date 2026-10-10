	.include "asm/macros.inc"
	.public _02260374
	.public ScaleSprites_Init
	.public ScaleSprites_Delete
	.public ScaleSprites_Update
	.public LeadersEffect_GetTrainerNameString
	.public LeadersEffect_GetRivalNameString
	.public LeadersEffect_RunGymLeaderIntro
	.public EncounterEffect_LeaderFalkner
	.public EncounterEffect_LeaderBugsy
	.public EncounterEffect_LeaderWhitney
	.public EncounterEffect_LeaderMorty
	.public EncounterEffect_LeaderJasmine
	.public EncounterEffect_LeaderChuck
	.public EncounterEffect_LeaderPryce
	.public EncounterEffect_LeaderClair
	.public EncounterEffect_LeaderBrock
	.public EncounterEffect_LeaderMisty
	.public EncounterEffect_LeaderLtSurge
	.public EncounterEffect_LeaderErika
	.public EncounterEffect_LeaderJanine
	.public EncounterEffect_LeaderSabrina
	.public EncounterEffect_LeaderBlaine
	.public EncounterEffect_LeaderBlue
	.public EncounterEffect_Rival
	.public LeadersEffect_GetPlayerGender
	.public LeadersEffect_RunEliteFourIntro
	.public LeadersEffect_LoadBlendedSpritePalette
	.public EncounterEffect_EliteFourWill
	.public EncounterEffect_EliteFourKoga
	.public EncounterEffect_EliteFourBruno
	.public EncounterEffect_EliteFourKaren
	.public EncounterEffect_Champion
	.public ov115_02260388
	.public ov115_02260390
	.public ov115_02260398
	.public ov115_022603A0
	.public ov115_022603A8
	.public ov115_022603B0
	.public ov115_022603C4
	.public ov115_022603D8
	.public ov115_022603EC
	.public ov115_02260400
	.public ov115_02260414
	.public ov115_02260428
	.public ov115_0226043C
	.public ov115_02260450
	.public ov115_02260464
	.public ov115_02260478
	.public ov115_0226048C
	.public ov115_022604A0
	.public ov115_022604B4
	.public ov115_022604C8
	.public ov115_022604DC
	.include "overlay_115.inc"
	.include "global.inc"

	.text
	.public ScaleSprites_Delete
	.public LeadersEffect_GetTrainerNameString
	.public LeadersEffect_GetRivalNameString
	.public EncounterEffect_LeaderFalkner
	.public EncounterEffect_LeaderBugsy
	.public EncounterEffect_LeaderWhitney
	.public EncounterEffect_LeaderMorty
	.public EncounterEffect_LeaderJasmine
	.public EncounterEffect_LeaderChuck
	.public EncounterEffect_LeaderPryce
	.public EncounterEffect_LeaderClair
	.public EncounterEffect_LeaderBrock
	.public EncounterEffect_LeaderMisty
	.public EncounterEffect_LeaderLtSurge
	.public EncounterEffect_LeaderErika
	.public EncounterEffect_LeaderJanine
	.public EncounterEffect_LeaderSabrina
	.public EncounterEffect_LeaderBlaine
	.public EncounterEffect_LeaderBlue
	.public EncounterEffect_Rival
	.public LeadersEffect_GetPlayerGender
	.public EncounterEffect_EliteFourWill
	.public EncounterEffect_EliteFourKoga
	.public EncounterEffect_EliteFourBruno
	.public EncounterEffect_EliteFourKaren
	.public EncounterEffect_Champion

	thumb_func_start ScaleSprites_Init
ScaleSprites_Init: ; 0x0225F020
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	mov r6, #0
	strh r6, [r5]
	add r4, r5, #0
	ldr r7, [sp, #0x28]
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	str r3, [sp, #0x10]
	strh r6, [r5, #2]
	add r4, #0x14
_0225F038:
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x10]
	add r3, r7, #0
	bl ov01_021F0718
	str r0, [r5, #4]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #4]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	cmp r6, #3
	beq _0225F07E
	ldr r0, [r5, #4]
	mov r1, #2
	bl Sprite_SetAffineOverwriteMode
	ldr r0, [r5, #4]
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	mov r1, #2
	lsl r1, r1, #0xc
	add r0, r4, #0
	lsr r2, r1, #1
	mov r3, #6
	bl ov01_021EFE70
	b _0225F08C
_0225F07E:
	mov r1, #1
	lsl r1, r1, #0xc
	add r0, r4, #0
	add r2, r1, #0
	mov r3, #6
	bl ov01_021EFE70
_0225F08C:
	add r6, r6, #1
	add r5, r5, #4
	add r4, #0x14
	cmp r6, #4
	blt _0225F038
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ScaleSprites_Init
