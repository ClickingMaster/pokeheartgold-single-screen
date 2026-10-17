	.public UnusedBillboardEffect_Init
	.public UnusedBillboardEffect_Destroy
	.public UnusedBillboardEffect_Update
	.public UnusedBillboardEffect_Draw
	.public sVeryTallGrassRecordParams

	.rodata

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
