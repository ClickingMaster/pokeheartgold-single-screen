	.public TallGrass_New
	.public TallGrass_Free
	.public TallGrass_LoadGfx
	.public TallGrass_FreeGfx
	.public TallGrass_Spawn
	.public TallGrass_SpawnAt
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
	.public gTallGrassRecord6Params
	.public gTallGrassRecordParams
	.public gTallGrassRecord5Params
	.public gTallGrassRecord7Params
	.include "asm/macros.inc"
	.include "overlay_01_021FEEEC.inc"
	.include "global.inc"

	.public gTallGrassEffectTemplate
	.public sTallGrassUnusedEffectTemplate
	.public gTallGrassRecord6Params
	.public gTallGrassRecordParams
	.public gTallGrassRecord5Params
	.public gTallGrassRecord7Params
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

gTallGrassRecord6Params: ; 0x02209160
	.byte 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

gTallGrassRecordParams: ; 0x02209178
	.byte 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

gTallGrassRecord5Params: ; 0x02209190
	.byte 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

gTallGrassRecord7Params: ; 0x022091A8
	.byte 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
