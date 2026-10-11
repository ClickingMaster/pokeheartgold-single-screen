	.extern BattleBagMenu_PrintCenteredText
	.extern BattleBagMenu_PrintItemQuantity
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.extern BattleBagMenu_PrintCenteredText
	.extern BattleBagMenu_PrintItemQuantity
	.public BattleBagMenu_PrintUseItemName

.text
	.force_active on
