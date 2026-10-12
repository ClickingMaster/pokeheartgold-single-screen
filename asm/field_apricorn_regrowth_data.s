#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sApricornRegrowthTemplate
	.public ApricornRegrowthEffect_Destroy
	.public ApricornRegrowthEffect_Draw
	.public ApricornRegrowthEffect_Init
	.public ApricornRegrowthEffect_Update

	.rodata

sApricornRegrowthTemplate: ; 0x02209084
	.word 0x0000001C
	.word ApricornRegrowthEffect_Init
	.word ApricornRegrowthEffect_Destroy
	.word ApricornRegrowthEffect_Update
	.word ApricornRegrowthEffect_Draw
