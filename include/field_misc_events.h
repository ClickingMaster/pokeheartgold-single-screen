#ifndef POKEHEARTGOLD_FIELD_MISC_EVENTS_H
#define POKEHEARTGOLD_FIELD_MISC_EVENTS_H

#include "npc_trade.h"
#include "pokemon_types_def.h"
#include "script.h"
#include "task.h"

BOOL FieldSystem_TryPushIceBlock(FieldSystem *fieldSystem);
BOOL MonIsInGameTradePoke(Pokemon *mon, NpcTradeNum tradeNum);
BOOL MonIsFromTogepiEgg(Pokemon *mon, SaveData *saveData);
BugContest *FieldSystem_BugContest_Get(FieldSystem *fieldSystem);
u16 *BugContest_GetSportBallsAddr(BugContest *contest);
void FieldSystem_IncrementBugContestTimer(FieldSystem *fieldSystem, int duration);
void FieldSystem_InitMystriStageGymmick(FieldSystem *fieldSystem);
void BugContest_WarpToJudging(TaskManager *taskman, FieldSystem *fieldSystem);
void BugContest_PromptSwapPokemon(TaskManager *man, Pokemon *mon);

#endif // POKEHEARTGOLD_FIELD_MISC_EVENTS_H
