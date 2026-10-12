#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sShayminFormChangeTemplate
	.public ShayminFormChangeEffect_Destroy
	.public ShayminFormChangeEffect_Draw
	.public ShayminFormChangeEffect_Init
	.public ShayminFormChangeEffect_Update

	.text
	.rodata

sShayminFormChangeTemplate: ; 0x022096CC
	.word 0x00000008
	.word ShayminFormChangeEffect_Init
	.word ShayminFormChangeEffect_Destroy
	.word ShayminFormChangeEffect_Update
	.word ShayminFormChangeEffect_Draw
