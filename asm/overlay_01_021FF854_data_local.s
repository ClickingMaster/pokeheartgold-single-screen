	.public VeryTallGrassEffect_Init
	.public VeryTallGrassEffect_Destroy
	.public VeryTallGrassEffect_Update
	.public VeryTallGrassEffect_Draw
	.public ov01_021FFCA8
	.public ov01_021FFD64
	.public ov01_021FFD70
	.public ov01_021FFE98
	.public ov01_02209218
	.public ov01_0220922C

	.rodata

ov01_02209218:
	.word 0x00000040
	.word VeryTallGrassEffect_Init
	.word VeryTallGrassEffect_Destroy
	.word VeryTallGrassEffect_Update
	.word VeryTallGrassEffect_Draw

ov01_0220922C:
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00

ov01_02209244:
	.word 0x00000040
	.word ov01_021FFCA8
	.word ov01_021FFD64
	.word ov01_021FFD70
	.word ov01_021FFE98
