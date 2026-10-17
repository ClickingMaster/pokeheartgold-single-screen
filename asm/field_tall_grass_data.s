	.public TallGrassEffect_Init
	.public TallGrassEffect_Destroy
	.public TallGrassEffect_Update
	.public TallGrassEffect_Draw
	.public TallGrassUnusedEffect_Init
	.public TallGrassUnusedEffect_Destroy
	.public TallGrassUnusedEffect_Update
	.public TallGrassUnusedEffect_Draw
	.public gTallGrassEffectTemplate
	.public sTallGrassUnusedEffectTemplate
	.include "asm/macros.inc"
	.include "field_tall_grass_data.inc"
	.include "global.inc"

	.public gTallGrassEffectTemplate
	.public sTallGrassUnusedEffectTemplate
.rodata

gTallGrassEffectTemplate: ; 0x02209138
	.word 0x00000040
	.word TallGrassEffect_Init
	.word TallGrassEffect_Destroy
	.word TallGrassEffect_Update
	.word TallGrassEffect_Draw

sTallGrassUnusedEffectTemplate:
	.word 0x00000028
	.word TallGrassUnusedEffect_Init
	.word TallGrassUnusedEffect_Destroy
	.word TallGrassUnusedEffect_Update
	.word TallGrassUnusedEffect_Draw
