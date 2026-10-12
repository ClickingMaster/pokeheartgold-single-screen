	.include "asm/macros.inc"
	.include "overlay_120.inc"
	.include "global.inc"

	.public ov120_022601BC
	.public ov120_022601D8
	.public ov120_022601F4
	.public ov120_022601F8
	.public ov120_022601FC
	.public ov120_02260200
	.public ov120_022602E4
	.public ov120_022602FC
	.public ov120_02260314
	.public ov120_0226032C

	.public _0226019C

	.text
	.public WildEffectCell_New
	.public WildEffectCell_Free
	.public WildEffectCell_Start
	.public WildEffectCell_Update
	.public WildEffectRandomFill_New
	.public WildEffectRandomFill_Free
	.public WildEffectRandomFill_Start
	.public WildEffectRandomFill_Update
	.public ShuffledIndices_Init
	.public ShuffledIndices_Next
	.public WildEffectCellGrid_New
	.public WildEffectCellGrid_Free
	.public WildEffectCellGrid_Start
	.public WildEffectCellGrid_UpdateRows
	.public WildEffectCellGrid_UpdateDiagonals
	.public WildEffectSine_Init
	.public WildEffectSine_Update
	.public WildEffectStep_Init
	.public WildEffectStep_Run
	.public EncounterEffect_PkmnTrainerRed
	.public RedEffect_FillScreen
	.public RedEffect_FirstFlash
	.public RedEffect_SecondFlash
	.public RedEffect_CreateSprite
	.public EncounterEffect_WildRowsWhite
	.public EncounterEffect_WildRowsBlack
	.public WildEffect_RunCellGridRows
	.public EncounterEffect_WildDiagonalsWhite
	.public EncounterEffect_WildDiagonalsBlack
	.public WildEffect_RunCellGridDiagonals
	.public EncounterEffect_WildWaveWhite
	.public EncounterEffect_WildWaveBlack
	.public WildEffect_LoadCharData
	.public WildEffect_LoadScreen
	.public WildEffectBgLoader_VBlankTask
	.public WildEffectBgLoader_Task
	.public WildEffectBgLoader_Start
	.public WildEffectBgLoader_IsDone
	.public WildEffect_RunWave
	thumb_func_start WildEffectCell_Update
WildEffectCell_Update: ; 0x0225F08C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r0, #0
	add r1, r4, #0
	add r1, #0x30
	ldrb r1, [r1]
	cmp r1, #0
	bne _0225F0A2
	add sp, #8
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0225F0A2:
	bl EncounterEffectInterp_Update
	add r5, r0, #0
	add r0, r4, #0
	add r0, #0x14
	bl EncounterEffectInterp_Update
	add r7, r5, #0
	ldr r1, [r4]
	orr r7, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r5, r0, #1
	mov r0, #0x2c
	ldr r1, [r4, #0x14]
	ldrsh r3, [r4, r0]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r6, r0, #1
	mov r0, #0x2e
	ldrsh r2, [r4, r0]
	add r0, r3, r5
	lsl r0, r0, #0x10
	sub r1, r2, r6
	asr r0, r0, #0x10
	str r0, [sp]
	add r0, r4, #0
	add r0, #0x31
	ldrb r0, [r0]
	add r2, r2, r6
	sub r3, r3, r5
	str r0, [sp, #4]
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	ldr r0, [r4, #0x28]
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	asr r3, r3, #0x10
	bl ov01_021F0960
	add r0, r7, #0
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end WildEffectCell_Update
