#ifndef POKEHEARTGOLD_FRONTIER_SAVE_DATA_HELPERS_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_SAVE_DATA_HELPERS_INTERNAL_H

#include "global.h"

#include "scrcmd_9.h"

u32 FrontierTrainer_GetClassSprite(u8 value);
u32 BattleTower_NextRandom(u32 value);
void BattleTower_GenerateTrainerMons(void *data, void *array, u32 value, u32 count, void *values1, void *values2, u32 zero, u32 word);
void BattleTower_GetRoomTrainer(void *saveData, void *array, u32 index);

void BattleTower_GenerateOpponents(void *data, void *saveData);
u32 BattleTower_GetTrainerSprite(void *data, u32 index);
void BattleTower_SetPalmerDefeated(void *data, u32 value);
u32 BattleTower_IncBattleNumber(void *data);
u32 BattleTower_GetWinStreak(void *data);

#endif
