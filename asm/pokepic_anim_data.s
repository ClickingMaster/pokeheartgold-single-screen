	.public gPokepicMotionTypes
	.public gPokepicMotionTypeArgCounts
	.public gPokepicMotionTypeTargetArgs
	.public gPokepicAnimCommands
	.include "asm/macros.inc"
	.include "pokepic_anim_data.inc"
	.include "global.inc"

	.rodata

gPokepicMotionTypes:
	.word PokepicMotion_UpdateWave
gPokepicMotionTypeArgCounts:
	.word 0x00000006
gPokepicMotionTypeTargetArgs:
	.word 0x00000001
	.word PokepicMotion_UpdateWaveOverDuration, 0x00000006, 0x00000001
	.word PokepicMotion_UpdateAccel, 0x00000004, 0x00000000
	.word PokepicMotion_UpdateLinear, 0x00000003, 0x00000000
	.word PokepicMotion_UpdateAccelToLimit, 0x00000004, 0x00000000
gPokepicAnimCommands:
	.word PokepicScriptAnimCmd_End
	.word PokepicScriptAnimCmd_Yield
	.word PokepicScriptAnimCmd_ResetTransform
	.word PokepicScriptAnimCmd_SetVarIfCompare
	.word PokepicScriptAnimCmd_SetVar
	.word PokepicScriptAnimCmd_CopyVar
	.word PokepicScriptAnimCmd_AddVar
	.word PokepicScriptAnimCmd_MulVar
	.word PokepicScriptAnimCmd_SubVar
	.word PokepicScriptAnimCmd_DivVar
	.word PokepicScriptAnimCmd_ModVar
	.word PokepicScriptAnimCmd_LoopStart
	.word PokepicScriptAnimCmd_LoopEnd
	.word PokepicScriptAnimCmd_SetAttrFromVar
	.word PokepicScriptAnimCmd_AddAttrFromVar
	.word PokepicScriptAnimCmd_SetOrAddAttr
	.word PokepicScriptAnimCmd_Sin
	.word PokepicScriptAnimCmd_Cos
	.word PokepicScriptAnimCmd_SetPosFromVar
	.word PokepicScriptAnimCmd_AddPosFromVar
	.word PokepicScriptAnimCmd_SetOrAddTransform
	.word PokepicScriptAnimCmd_ApplyPosition
	.word PokepicScriptAnimCmd_ApplyTransform
	.word PokepicScriptAnimCmd_SetOffsetFromVar
	.word PokepicScriptAnimCmd_RunMotions
	.word PokepicScriptAnimCmd_SetScaleMode
	.word PokepicScriptAnimCmd_StartWaveMotion
	.word PokepicScriptAnimCmd_StartWaveMotionOverDuration
	.word PokepicScriptAnimCmd_StartAccelMotion
	.word PokepicScriptAnimCmd_StartLinearMotion
	.word PokepicScriptAnimCmd_StartAccelMotionToLimit
	.word PokepicScriptAnimCmd_Wait
	.word PokepicScriptAnimCmd_StartPaletteFade
	.word PokepicScriptAnimCmd_WaitPaletteFade

.text
