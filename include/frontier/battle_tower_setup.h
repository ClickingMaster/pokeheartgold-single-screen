#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_TOWER_SETUP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_TOWER_SETUP_H

#include "battle/battle_setup.h"
#include "frontier/frontier.h"

#include "filesystem.h"
#include "scrcmd_9.h"

typedef struct BattleTowerTrainerSets BattleTowerTrainerSets;

u32 BattleTower_GenerateTrainerMons(FrontierFieldSystem *tower, FrontierTrainer *trainer, u16 trainerId, u32 count, u16 *partySpecies, u16 *partyItems, FrontierMultiBattleAllyData *allyData, enum HeapID heapID);
u32 BattleTower_PickTrainerMonSets(FrontierFieldSystem *tower, BattleTowerTrainerSets *sets, u16 trainerId, FrontierMon *mons, u8 count, u16 *partySpecies, u16 *partyItems, FrontierMultiBattleAllyData *allyData, enum HeapID heapID);
void BattleTower_GetRoomTrainer(SaveData *saveData, FrontierTrainer *trainer, int index);
void BattleTower_GetSampleRoomTrainer(FrontierTrainer *trainer, int index);
u32 BattleTower_GenerateFrontierMon(FrontierFieldSystem *tower, FrontierMon *mon, u16 setId, u32 otId, u32 pid, u8 iv, u8 itemIndex, BOOL replaceItem, enum HeapID heapID);
BattleSetup *BattleTower_NewBattleSetup(FrontierFieldSystem *tower, FrontierLaunchArgs *args);
void BattleTower_AddTrainerToBattle(BattleSetup *setup, FrontierTrainer *trainer, int count, int battler, enum HeapID heapID);
u32 BattleTower_GetBattleType(u32 towerMode);
void ov80_02236ABC(FrontierFieldSystem *tower, u16 value);
BOOL ov80_02236AD8(FrontierFieldSystem *tower, const u16 *remote);
NarcId BattleTower_GetMonSetNarc(u32 towerMode);
NarcId BattleTower_GetTrainerNarc(u32 towerMode);
u32 BattleTower_GetTrainerMessageBank(u32 towerMode);
BOOL BattleTower_UsesNewSetNarcs(u32 towerMode);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_TOWER_SETUP_H
