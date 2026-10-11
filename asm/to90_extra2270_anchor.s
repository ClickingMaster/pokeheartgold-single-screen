	.extern BattlePartyMenu_ShowSpriteAt
	.extern BattlePartyMenu_ShowStatusIcon
	.extern BattlePartyMenu_ShowHeldItemIcon
	.extern BattlePartyMenu_ShowCapsuleIcon
	.extern sBattlePartyListIconPos
	.extern sBattlePartyListStatusIconPos
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.public BattlePartyMenu_ShowPartyListSprites
	.extern BattlePartyMenu_ShowSpriteAt
	.extern BattlePartyMenu_ShowStatusIcon
	.extern BattlePartyMenu_ShowHeldItemIcon
	.extern BattlePartyMenu_ShowCapsuleIcon
	.extern sBattlePartyListIconPos
	.extern sBattlePartyListStatusIconPos

.text
	.force_active on
