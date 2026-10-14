#ifndef POKEHEARTGOLD_FIELD_WARP_RESET_H
#define POKEHEARTGOLD_FIELD_WARP_RESET_H

#include "field/field_effect_manager.h"
#include "npc_trade.h"
#include "script.h"

void FieldSystem_ClearFollowingTrainer(FieldSystem *fieldSystem);
void FieldSystem_ResetStateOnWarp(FieldSystem *fieldSystem);
void FieldSystem_ResetStateOnMapConnection(FieldSystem *fieldSystem);
void FieldSystem_ResetStateOnFlyWarp(FieldSystem *fieldSystem);
void FieldSystem_ClearSafariAndRandomizeRoamers(FieldSystem *fieldSystem);
void FieldSystem_ClearSafariSysFlag(FieldSystem *fieldSystem);
void FieldSystem_RandomizeRoamers(FieldSystem *fieldSystem);
BOOL FieldEffect_IsActive(FieldEffect *unk);
void CallTask_NPCTrade(TaskManager *taskManager, NPCTradeAppData *tradeWork, u16 arg, enum HeapID heapID);

#endif // POKEHEARTGOLD_FIELD_WARP_RESET_H
