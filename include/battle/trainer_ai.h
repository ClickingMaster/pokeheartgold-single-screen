#ifndef POKEHEARTGOLD_TRAINER_AI_H
#define POKEHEARTGOLD_TRAINER_AI_H

#include "battle.h"

void TrainerAI_Init(BattleSystem *battleSystem, BattleContext *ctx, u8 battlerId, u8 a3);
u8 TrainerAI_SelectMove(BattleSystem *battleSystem, u8 battlerId);
int TrainerAI_SelectCommand(BattleSystem *battleSystem, int battler);

#endif
