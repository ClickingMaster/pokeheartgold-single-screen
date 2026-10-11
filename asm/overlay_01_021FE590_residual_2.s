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
	.include "overlay_01_021FE590.inc"
	.include "global.inc"

	.public ApricornRegrowth_New
	.public ApricornRegrowth_Free
	.public ApricornRegrowth_Retain
	.public ApricornRegrowth_Release
	.public ApricornRegrowth_LoadGfx
	.public ApricornRegrowth_FreeGfx
	.public ApricornRegrowth_LoadGfxIfUnused
	.public ApricornRegrowth_FreeGfxIfUnused
	.public ApricornRegrowth_Spawn
	.public ApricornRegrowthEffect_Init
	.public ApricornRegrowthEffect_Destroy
	.public ApricornRegrowthEffect_Update
	.public ApricornRegrowthEffect_Draw
	.public ov01_02209084
	.public ov01_02209098
	.public ov01_022090AC

	.rodata

ov01_022090AC: ; 0x022090AC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00
