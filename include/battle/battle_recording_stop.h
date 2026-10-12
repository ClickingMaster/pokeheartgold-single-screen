#ifndef POKEHEARTGOLD_BATTLE_RECORDING_STOP_H
#define POKEHEARTGOLD_BATTLE_RECORDING_STOP_H

#include "battle/battle.h"

#include "sys_task_api.h"

typedef struct BattleRecordingStopWork {
    BattleSystem *battleSystem;
    s16 state;
    s16 delay;
} BattleRecordingStopWork;

SysTask *BattleRecordingStop_CreateTask(BattleSystem *battleSystem);
void BattleRecordingStop_DestroyTask(SysTask *task);

#endif
