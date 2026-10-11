#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.include "overlay_01_021FDA14.inc"
	.include "global.inc"

	.public Reflection_New
	.public Reflection_Free
	.public Reflection_LoadGfx
	.public Reflection_FreeGfx
	.public Reflection_Spawn
	.public ReflectionEffect_Init
	.public ReflectionEffect_Destroy
	.public ReflectionEffect_Update
	.public ReflectionEffect_Draw
	.public ReflectionEffect_CalcPosition
	.public ReflectionEffect_CreateBillboard
	.public BillboardReflectionEffect_CreateBillboard
	.public BillboardReflectionEffect_CalcPosition
	.public Reflection_SpawnForBillboard
	.public BillboardReflectionEffect_Init
	.public BillboardReflectionEffect_Destroy
	.public BillboardReflectionEffect_Update
	.public BillboardReflectionEffect_Draw
	.public Reflection_SpawnForFollower
	.public FollowerReflectionEffect_Init
	.public FollowerReflectionEffect_Update
	.public FollowerReflectionEffect_Draw
	.public FollowerReflectionEffect_CreateBillboard
	.public ov01_02208F74
	.public ov01_02208F88
	.public ov01_02208F9C
	.public ov01_02208FB0
	.public ov01_02208FC8

	.rodata

ov01_02208F74: ; 0x02208F74
	.word 0x00000038
	.word FollowerReflectionEffect_Init
	.word ReflectionEffect_Destroy
	.word FollowerReflectionEffect_Update
	.word FollowerReflectionEffect_Draw

ov01_02208F88: ; 0x02208F88
	.word 0x00000038
	.word ReflectionEffect_Init
	.word ReflectionEffect_Destroy
	.word ReflectionEffect_Update
	.word ReflectionEffect_Draw

ov01_02208F9C: ; 0x02208F9C
	.word 0x00000038
	.word BillboardReflectionEffect_Init
	.word BillboardReflectionEffect_Destroy
	.word BillboardReflectionEffect_Update
	.word BillboardReflectionEffect_Draw
