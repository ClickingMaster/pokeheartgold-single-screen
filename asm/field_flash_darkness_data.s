#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sFlashDarknessTemplate
	.public FlashDarknessEffect_Destroy
	.public FlashDarknessEffect_Draw
	.public FlashDarknessEffect_Init
	.public FlashDarknessEffect_Update

	.rodata

sFlashDarknessTemplate: ; 0x022095B4
	.word 0x00000020
	.word FlashDarknessEffect_Init
	.word FlashDarknessEffect_Destroy
	.word FlashDarknessEffect_Update
	.word FlashDarknessEffect_Draw
