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
	.include "overlay_01_data_02208BFC.inc"
	.include "global.inc"

	.rodata

	.public defaultFieldEffectRenderers
defaultFieldEffectRenderers: ; 0x02208BFC
	.word 0x14
	.word 0x13
	.word 0x11
	.word 0x01
	.word 0x10
	.word 0x05
	.word 0x16
	.word 0x08
	.word 0x09
	.word 0x0A
	.word 0x0B
	.word 0x0C
	.word 0x0D
	.word 0x0F
	.word 0x12
	.word 0x00
	.word 0x02
	.word 0x03
	.word 0x04
	.word 0x0E
	.word 0x06
	.word 0x07
	.word 0x15
	.word 0x17

	.public gFieldEffectRenderers
gFieldEffectRenderers: ; 0x02208C5C
	.word 0x00000000, FieldShadow_New, FieldShadow_Free
	.word 0x00000001, ov01_021FDA14, ov01_021FDA30
	.word 0x00000002, ov01_021FE200, ov01_021FE220
	.word 0x00000003, ov01_02200858, ov01_02200874
	.word 0x00000004, ov01_021FE780, ov01_021FE79C
	.word 0x00000005, RockClimb_New, RockClimb_Free
	.word 0x00000006, PuddleSplash_New, PuddleSplash_Free
	.word 0x00000007, MudSplash_New, MudSplash_Free
	.word 0x00000008, TallGrass_New, TallGrass_Free
	.word 0x00000009, ov01_022001E4, ov01_022001F8
	.word 0x0000000A, ShallowWaterRipple_New, ShallowWaterRipple_Free
	.word 0x0000000B, GroundDust_New, GroundDust_Free
	.word 0x0000000C, VeryTallGrass_New, VeryTallGrass_Free
	.word 0x0000000D, ov01_021FFC0C, ov01_021FFC28
	.word 0x0000000E, ov01_021FFECC, ov01_021FFEE8
	.word 0x0000000F, ov01_02200040, ov01_0220005C
	.word 0x00000010, ov01_021FE590, ov01_021FE5A4
	.word 0x00000011, ov01_022031C0, ov01_022031E8
	.word 0x00000012, EmoteBubble_New, EmoteBubble_Free
	.word 0x00000013, ov01_02203E40, ov01_02203E64
	.word 0x00000014, MapObjectModelList_New, MapObjectModelList_Free
	.word 0x00000015, ov01_022006A8, ov01_022006C4
	.word 0x00000016, ov01_022051EC, ov01_02205208
	.word 0x00000017, 0x00000000, 0x00000000
