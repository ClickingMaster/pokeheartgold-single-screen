#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H

#include "frontier/battle_castle_data.h"
#include "frontier/battle_factory_data.h"
#include "frontier/battle_hall_data.h"
#include "frontier/battle_arcade_data.h"

// Link messages of the facilities' script side (frontier_link_messages.c).
typedef struct ArcadeMonsMessage ArcadeMonsMessage;

void BattleHallLink_ReceiveUnused(int netId, int size, void *data, BattleHallData *work);
BOOL BattleHallLink_SendEmptyRecord(BattleHallData *work);
void BattleHallLink_ReceiveEmptyRecord(int netId, int size, void *data, BattleHallData *work);
BOOL BattleHallLink_SendTrainerIds(BattleHallData *work);
void BattleHallLink_ReceiveTrainerIds(int netId, int size, const u16 *data, BattleHallData *work);
BOOL BattleHallLink_SendSetIds(BattleHallData *work);
void BattleHallLink_ReceiveSetIds(int netId, int size, const u16 *data, BattleHallData *work);
BOOL BattleHallLink_SendValue(BattleHallData *work, u16 value);
void BattleHallLink_ReceiveValue(int netId, int size, const u16 *data, BattleHallData *work);
BOOL BattleHallLink_SendPartyMon(BattleHallData *work);
void BattleHallLink_ReceivePartnerMon(int netId, int size, const void *data, BattleHallData *work);
void *BattleHallLink_GetRecvMonBuffer(int netId, BattleHallData *work, int size);
BOOL BattleFactoryLink_SendRecord(BattleFactoryData *work);
void BattleFactoryLink_ReceiveRecord(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendTrainerIds(BattleFactoryData *work);
void BattleFactoryLink_ReceiveTrainerIds(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendRentalMons(BattleFactoryData *work);
void BattleFactoryLink_ReceiveRentalMons(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendOpponentMons(BattleFactoryData *work);
void BattleFactoryLink_ReceiveOpponentMons(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendValueA(BattleFactoryData *work, u16 value);
void BattleFactoryLink_ReceiveValueA(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendValueB(BattleFactoryData *work, u16 value);
void BattleFactoryLink_ReceiveValueB(int netId, int size, const u16 *data, BattleFactoryData *work);
BOOL BattleFactoryLink_SendPartyMons(BattleFactoryData *work);
void BattleFactoryLink_ReceivePartyMons(int netId, int size, u16 *data, BattleFactoryData *work);
BOOL BattleCastleLink_SendRecord(BattleCastleData *work);
void BattleCastleLink_ReceiveRecord(int netId, int size, const u16 *data, BattleCastleData *work);
BOOL BattleCastleLink_SendTrainerIds(BattleCastleData *work);
void BattleCastleLink_ReceiveTrainerIds(int netId, int size, const u16 *data, BattleCastleData *work);
BOOL BattleCastleLink_SendChoice(BattleCastleData *work);
void BattleCastleLink_ReceiveChoice(int netId, int size, void *data, BattleCastleData *work);
BOOL BattleCastleLink_SendOpponentMons(BattleCastleData *work);
void BattleCastleLink_ReceiveOpponentMons(int netId, int size, void *data, BattleCastleData *work);
BOOL BattleCastleLink_SendValueA(BattleCastleData *work, u16 value);
void BattleCastleLink_ReceiveValueA(int netId, int size, const u16 *data, BattleCastleData *work);
BOOL BattleCastleLink_SendValueB(BattleCastleData *work, u16 value);
void BattleCastleLink_ReceiveValueB(int netId, int size, const u16 *data, BattleCastleData *work);
BOOL BattleCastleLink_SendPartyMons(BattleCastleData *work);
void BattleCastleLink_ReceivePartyMons(int netId, int size, void *data, BattleCastleData *work);
void *BattleCastleLink_GetRecvMonBuffer(int netId, BattleCastleData *work, int size);
BOOL BattleArcadeLink_SendWinStreak(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveWinStreak(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendTrainerIds(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveTrainerIds(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendOpponentMons(ArcadeContext *ctx);
void BattleArcadeLink_ReceiveOpponentMons(int netId, int size, ArcadeMonsMessage *msg, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendValueA(ArcadeContext *ctx, u16 value);
void BattleArcadeLink_ReceiveValueA(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendValueB(ArcadeContext *ctx, u16 value);
void BattleArcadeLink_ReceiveValueB(int netId, int size, const u16 *data, ArcadeContext *ctx);
BOOL BattleArcadeLink_SendPartyMons(ArcadeContext *ctx);
void BattleArcadeLink_ReceivePartyMons(int netId, int size, void *data, ArcadeContext *ctx);
void *BattleArcadeLink_GetRecvMonBuffer(int netId, ArcadeContext *ctx, int size);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_LINK_MESSAGES_H
