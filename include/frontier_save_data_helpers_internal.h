#ifndef POKEHEARTGOLD_FRONTIER_SAVE_DATA_HELPERS_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_SAVE_DATA_HELPERS_INTERNAL_H

#include "global.h"

typedef struct Overlay80Bits {
    u8 padding[0x10];
    u8 unused : 1;
    u8 value : 2;
} Overlay80Bits;

typedef struct Overlay80Counter {
    u8 padding_00[0xD];
    u8 count;
    u8 padding_0E[0xC];
    u16 base;
} Overlay80Counter;

u32 FrontierTrainer_GetClassSprite(u8 value);
u32 BattleTower_NextRandom(u32 value);
void BattleTower_GenerateTrainerMons(void *data, void *array, u32 value, u32 count, void *values1, void *values2, u32 zero, u32 word);
void BattleTower_GetRoomTrainer(void *saveData, void *array, u32 index);

void BattleTower_GenerateOpponents(void *data, void *saveData);
u32 BattleTower_GetTrainerSprite(void *data, u32 index);
void BattleTower_SetPalmerDefeated(void *data, u32 value);
u32 BattleTower_IncBattleNumber(void *data);
u32 ov80_022358E8(void *data);

#endif
