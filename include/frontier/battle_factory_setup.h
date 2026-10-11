#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_SETUP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_SETUP_H

#include "battle/battle_setup.h"
#include "frontier/battle_factory_data.h"
#include "frontier/frontier.h"

#include "scrcmd_9.h"

// Range of Battle Factory set ids (narc 205). Trainer set ranges are looked up by trainer id; when hasNextTier is set,
// the following entry is the range the last Pokemon come from once enough trades were made.
typedef struct BattleFactorySetRange {
    u16 maxTrainerId;
    u16 minSetId;
    u16 maxSetId;
    u8 iv;
    u8 hasNextTier;
} BattleFactorySetRange;

int BattleFactory_GetRandomTrainerId(int type, u32 round, int battleNo);
void BattleFactory_GetRoundTrainerIds(int type, int round, u16 *trainerIds, int count);
const BattleFactorySetRange *BattleFactory_GetTrainerSetRange(int trainerId, int levelMode);
const BattleFactorySetRange *BattleFactory_GetRentalSetRange(int round, int levelMode);
int BattleFactory_PickSetIds(const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID, const BattleFactorySetRange *range, u16 tradeCount, u8 *ivs);
int BattleFactory_GetPartySize(int type);
int BattleFactory_GetOpponentMonCount(int type, int both);
void BattleFactory_GenerateRentalSets(int round, int levelMode, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, u16 tradeCount, const u16 *excludedSetIds, const u16 *excludedItems);
void BattleFactory_GenerateTrainerSets(u8 count, u16 trainerId, u8 levelMode, const FrontierMon *partyMons, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, int numPartyMons);
BattleSetup *BattleFactory_NewBattleSetup(BattleFactoryData *data, FrontierLaunchArgs *args);
u32 BattleFactory_GetBattleType(int type);
int BattleFactory_GetMonLevel(BattleFactoryData *data);
void BattleFactory_SetPartyToRentals(BattleFactoryData *data);
void BattleFactory_AddPartnerMonsToParty(BattleFactoryData *data);
BOOL BattleFactory_IsMultiType(int type);
int BattleFactory_GetTrainerAIFlags(BattleFactoryData *data);
int BattleFactory_GetRound(BattleFactoryData *data);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_SETUP_H
