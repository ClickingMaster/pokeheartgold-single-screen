#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_02096910.inc"
	.include "global.inc"

	.rodata
	.public _02108594
_02108594:
	.word BattleFactoryLink_ReceiveRecord, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveRentalMons, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveOpponentMons, CommCmd_GetSizeVariable, 0
	.word ov80_0222B3D4, CommCmd_GetSizeVariable, 0
	.word ov80_0222B420, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceivePartyMons, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceivePing, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveRentals, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveUnused, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveSwap, CommCmd_GetSizeVariable, 0
	.word ov80_0222AEF8, CommCmd_GetSizeVariable, 0
	.word ov80_0222AF3C, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceiveSetIds, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceiveValue, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceivePartnerMon, CommCmd_GetSizeVariable, BattleHallLink_GetRecvMonBuffer
	.word BattleHall_ReceivePing, CommCmd_GetSizeVariable, 0
	.word BattleHall_ReceiveChoice, CommCmd_GetSizeVariable, 0
	.word BattleHall_ReceiveAnswer, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceiveRecord, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceiveChoice, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceiveOpponentMons, CommCmd_GetSizeVariable, 0
	.word ov80_0222B8F8, CommCmd_GetSizeVariable, 0
	.word ov80_0222B940, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceivePartyMons, CommCmd_GetSizeVariable, BattleCastleLink_GetRecvMonBuffer
	.word BattleCastlePartyApp_ReceiveRanks, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveAction, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveCursor, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveExit, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveRanks, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveAction, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveCursor, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveExit, CommCmd_GetSizeVariable, 0
	.word sub_02096A7C, CommCmd_GetSizeVariable, 0
	.word sub_02096ACC, CommCmd_GetSizeVariable, 0
	.word sub_02096BB8, CommCmd_GetSizeVariable, 0
	.word sub_02096C18, CommCmd_GetSizeVariable, 0
	.word sub_02096C60, CommCmd_GetSizeVariable, 0
	.word sub_02096924, CommCmd_GetSizeVariable, 0
	.word sub_020969C4, CommCmd_GetSizeVariable, 0
	.word sub_020969F8, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveWinStreak, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveOpponentMons, CommCmd_GetSizeVariable, 0
	.word ov80_0222BC6C, CommCmd_GetSizeVariable, 0
	.word ov80_0222BCB8, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceivePartyMons, CommCmd_GetSizeVariable, BattleArcadeLink_GetRecvMonBuffer
	.word BattleArcadeGameBoard_ReceiveBoardLayout, CommCmd_GetSizeVariable, 0
	.word BattleArcadeGameBoard_ReceiveStopPos, CommCmd_GetSizeVariable, 0
	.word BattleArcadeGameBoard_ReceiveCursorPos, CommCmd_GetSizeVariable, 0
