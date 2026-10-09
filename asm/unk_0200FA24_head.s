	.include "asm/macros.inc"
	.include "unk_0200FA24.inc"
	.include "global.inc"

	.rodata

	.public sFadeFuncPtrs
sFadeFuncPtrs:
	.word FadeFunc_00
	.word FadeFunc_01
	.word FadeFunc_02
	.word FadeFunc_03
	.word FadeFunc_04
	.word FadeFunc_05
	.word FadeFunc_06
	.word FadeFunc_07
	.word FadeFunc_08
	.word FadeFunc_09
	.word FadeFunc_10
	.word FadeFunc_11
	.word FadeFunc_12
	.word FadeFunc_13
	.word FadeFunc_14
	.word FadeFunc_15
	.word FadeFunc_16
	.word FadeFunc_17
	.word FadeFunc_18
	.word FadeFunc_19
	.word FadeFunc_20
	.word FadeFunc_21
	.word FadeFunc_22
	.word FadeFunc_23
	.word FadeFunc_24
	.word FadeFunc_25
	.word FadeFunc_26
	.word FadeFunc_27
	.word FadeFunc_28
	.word FadeFunc_29
	.word FadeFunc_30
	.word FadeFunc_31
	.word FadeFunc_32
	.word FadeFunc_33
	.word FadeFunc_34
	.word FadeFunc_35
	.word FadeFunc_36
	.word FadeFunc_37
	.word FadeFunc_38
	.word FadeFunc_39
	.word FadeFunc_40
	.word FadeFunc_41
	.word FadeFunc_42

	.text
	.public BeginNormalPaletteFade
	.public HandleFadeUpdateFrame
	.public IsPaletteFadeFinished
	.public sub_0200FB70
	.public ResetVisibleHardwareWindows
	.public SetMasterBrightnessNeutral
	.public sub_0200FBF4
	.public sub_0200FC20
	.public sub_0200FC60
	.public sub_0200FCDC
	.public SetMasterBrightness
	.public HandleEndFade
	.public HandleFadeUpdateFrame
	.public IsPaletteFadeFinished
	.public ResetVisibleHardwareWindows
	.public SetMasterBrightnessNeutral
	.public sub_0200FBF4
	.public sub_0200FC20
	.public sub_0200FC60
	.public sub_0200FCDC
	.public SetMasterBrightness
	.public HandleEndFade
	; BeginNormalPaletteFade is in src/palette_fade_reset.c (owner of _021D0EF4)
