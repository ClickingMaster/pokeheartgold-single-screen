#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sFollowerReflectionTemplate
	.public sMapObjectReflectionTemplate
	.public sBillboardReflectionTemplate
	.public BillboardReflectionEffect_Destroy
	.public BillboardReflectionEffect_Draw
	.public BillboardReflectionEffect_Init
	.public BillboardReflectionEffect_Update
	.public FollowerReflectionEffect_Draw
	.public FollowerReflectionEffect_Init
	.public FollowerReflectionEffect_Update
	.public ReflectionEffect_Destroy
	.public ReflectionEffect_Draw
	.public ReflectionEffect_Init
	.public ReflectionEffect_Update

	.rodata

sFollowerReflectionTemplate: ; 0x02208F74
	.word 0x00000038
	.word FollowerReflectionEffect_Init
	.word ReflectionEffect_Destroy
	.word FollowerReflectionEffect_Update
	.word FollowerReflectionEffect_Draw

sMapObjectReflectionTemplate: ; 0x02208F88
	.word 0x00000038
	.word ReflectionEffect_Init
	.word ReflectionEffect_Destroy
	.word ReflectionEffect_Update
	.word ReflectionEffect_Draw

sBillboardReflectionTemplate: ; 0x02208F9C
	.word 0x00000038
	.word BillboardReflectionEffect_Init
	.word BillboardReflectionEffect_Destroy
	.word BillboardReflectionEffect_Update
	.word BillboardReflectionEffect_Draw
