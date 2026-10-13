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
	.public gFrontierLinkCommands
gFrontierLinkCommands:
	.word BattleFactoryLink_ReceiveRecord, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveRentalMons, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveOpponentMons, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveValueA, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceiveValueB, CommCmd_GetSizeVariable, 0
	.word BattleFactoryLink_ReceivePartyMons, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceivePing, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveRentals, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveUnused, CommCmd_GetSizeVariable, 0
	.word BattleFactory_ReceiveSwap, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceiveUnused, CommCmd_GetSizeVariable, 0
	.word BattleHallLink_ReceiveEmptyRecord, CommCmd_GetSizeVariable, 0
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
	.word BattleCastleLink_ReceiveValueA, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceiveValueB, CommCmd_GetSizeVariable, 0
	.word BattleCastleLink_ReceivePartyMons, CommCmd_GetSizeVariable, BattleCastleLink_GetRecvMonBuffer
	.word BattleCastlePartyApp_ReceiveRanks, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveAction, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveCursor, CommCmd_GetSizeVariable, 0
	.word BattleCastlePartyApp_ReceiveExit, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveRanks, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveAction, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveCursor, CommCmd_GetSizeVariable, 0
	.word BattleCastleOpponentApp_ReceiveExit, CommCmd_GetSizeVariable, 0
	.word FrontierLinkLobby_ReceiveFacility, CommCmd_GetSizeVariable, 0
	.word FrontierLinkLobby_ReceiveChoice, CommCmd_GetSizeVariable, 0
	.word FrontierLinkLobby_ReceiveParty, CommCmd_GetSizeVariable, 0
	.word FrontierLinkLobby_ReceiveValue74, CommCmd_GetSizeVariable, 0
	.word FrontierLinkLobby_ReceiveValue71, CommCmd_GetSizeVariable, 0
	.word BattleTowerLink_ReceivePartnerInfo, CommCmd_GetSizeVariable, 0
	.word BattleTowerLink_ReceiveTrainers, CommCmd_GetSizeVariable, 0
	.word BattleTowerLink_ReceiveFlag, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveWinStreak, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveTrainerIds, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveOpponentMons, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveValueA, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceiveValueB, CommCmd_GetSizeVariable, 0
	.word BattleArcadeLink_ReceivePartyMons, CommCmd_GetSizeVariable, BattleArcadeLink_GetRecvMonBuffer
	.word BattleArcadeGameBoard_ReceiveBoardLayout, CommCmd_GetSizeVariable, 0
	.word BattleArcadeGameBoard_ReceiveStopPos, CommCmd_GetSizeVariable, 0
	.word BattleArcadeGameBoard_ReceiveCursorPos, CommCmd_GetSizeVariable, 0
