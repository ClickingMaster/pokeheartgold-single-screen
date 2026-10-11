	.extern sBattleBagItemEntryMsgIds
	.extern ov08_02225BE4
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.extern sBattleBagItemEntryMsgIds
	.extern ov08_02225BE4
	.public BattleBagMenu_PrintItemName
	.public BattleBagMenu_PrintItemQuantity

.text
	.force_active on
