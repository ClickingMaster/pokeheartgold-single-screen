	.public VeryTallGrassEffect_Init
	.public VeryTallGrassEffect_Destroy
	.public VeryTallGrassEffect_Update
	.public VeryTallGrassEffect_Draw
	.public UnusedBillboardEffect_Init
	.public UnusedBillboardEffect_Destroy
	.public UnusedBillboardEffect_Update
	.public UnusedBillboardEffect_Draw
	.public sVeryTallGrassEffectTemplate
	.public sVeryTallGrassRecordParams

	.rodata

sVeryTallGrassEffectTemplate:
	.word 0x00000040
	.word VeryTallGrassEffect_Init
	.word VeryTallGrassEffect_Destroy
	.word VeryTallGrassEffect_Update
	.word VeryTallGrassEffect_Draw

sVeryTallGrassRecordParams:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00

sUnusedBillboardEffectTemplate:
	.word 0x00000040
	.word UnusedBillboardEffect_Init
	.word UnusedBillboardEffect_Destroy
	.word UnusedBillboardEffect_Update
	.word UnusedBillboardEffect_Draw
