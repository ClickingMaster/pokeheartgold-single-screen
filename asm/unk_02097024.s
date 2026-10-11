#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_02097024.inc"
	.include "global.inc"

	.rodata

	.public _02108EA0
_02108EA0:
	.word SpinTradeGroupApp_Init, SpinTradeGroupApp_Main, SpinTradeGroupApp_Exit, FS_OVERLAY_ID(OVY_85)
	.public _02108EB0
_02108EB0:
	.word SpinTradeApp_Init, SpinTradeApp_Main, SpinTradeApp_Exit, FS_OVERLAY_ID(OVY_85)
	.public _02108EC0
_02108EC0:
	.word sub_020970E4
	.word sub_02097108
	.word sub_02097148
	.word sub_020971AC
	.word sub_020971D4
	.word sub_020971EC
