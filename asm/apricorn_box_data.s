#include "constants/items.h"
	.include "asm/macros.inc"
	.include "unk_02031B0C.inc"
	.include "global.inc"

	.rodata

	.public gApricornBallItems
gApricornBallItems:
	.short ITEM_LEVEL_BALL
	.short ITEM_MOON_BALL
	.short ITEM_LURE_BALL
	.short ITEM_FRIEND_BALL
	.short ITEM_LOVE_BALL
	.short ITEM_FAST_BALL
	.short ITEM_HEAVY_BALL
	.public gApricornFlavorDeltas
gApricornFlavorDeltas:
	.byte  4, -2,  0,  0
	.byte  0,  0,  4, -2
	.byte  0,  0,  0,  0
	.byte  4, -2,  0,  0
	.byte  0,  0,  4, -2
	.byte -2,  0,  0,  0
	.byte  4, -2, -2, -2
	.byte -2, -2,  2,  2
	.byte  2,  2,  2,  0
	.public gApricornJuicePresets
gApricornJuicePresets:
	.byte 0x00, 0x00, 0x74, 0x16, 0x0A, 0x00, 0x00, 0x00
	.byte 0x96, 0x00, 0x78, 0x00, 0x14, 0x0E
	.byte 0x00, 0x00, 0x1C, 0x02, 0x7D, 0x00, 0x00, 0x1C
	.byte 0x14, 0x00, 0xC8, 0x00, 0x88, 0x00, 0x00, 0x00
	.byte 0x18, 0x0E, 0x96, 0x00, 0x68, 0x0C, 0x00, 0x00, 0x00, 0x14
