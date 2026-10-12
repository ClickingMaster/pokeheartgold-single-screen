#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_HALL_SETUP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_HALL_SETUP_H

#include "battle/battle_setup.h"
#include "frontier/battle_hall_data.h"
#include "frontier/frontier.h"

#include "scrcmd_9.h"

void BattleHall_PickTrainerClasses(u32 cell, int count, u32 rank, u32 battleNo, u16 *trainerIds);
void BattleHall_PickTrainerIds(int type, int count, int round, u32 rank, u8 battleNo, u16 *trainerIds);
void BattleHall_PickSetIds(u16 count, u16 cellType, int rank, u8 battleNo, u16 playerSpecies, u16 *setIds, int matron);
BattleSetup *BattleHall_NewBattleSetup(BattleHallData *data, FrontierLaunchArgs *args);
u8 BattleHall_RerollPersonality(BattleHallData *data, u32 index);
u32 BattleHall_GetBattleType(u32 type);
int BattleHall_GetOpponentMonCount(int type);
void BattleHall_GenerateOpponentMons(FrontierMon *mons, u32 rank, u32 trainerId, u16 *setIds, int count, enum HeapID heapID, u32 narcId);
u16 BattleHall_GetOpponentLevel(BattleHallData *data, u32 unused);
u8 BattleHall_GetCellType(u32 cell);
BOOL BattleHall_IsMultiType(int type);
u8 BattleHall_GetOpponentIVs(u32 rank);
int BattleHall_GetTrainerAIFlags(BattleHallData *data, u32 index, int rank);
u32 BattleHall_ClampRank(u32 rank);
u16 BattleHall_GetPartyLevel(BattleHallData *data);
u32 BattleHall_SqrtFx(u32 value);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_HALL_SETUP_H
