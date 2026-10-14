#ifndef POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H
#define POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H

#include "script.h"

void TaskManager_CallFrontierPartySelect(TaskManager *taskManager, void *runningApp, u32 context, u32 summaryMode, u32 minMons, u32 maxMons, u32 maxLevel, u32 partySlot);
void TaskManager_CallFrontierWifiConnection(TaskManager *taskManager, u32 a1, u32 outputVar, u16 a3);
void TaskManager_CallFrontierLinkPartnerCheck(TaskManager *taskManager, u32 kind, u32 outputVar);
u32 BattleTower_ClaimRecordMilestone(SaveData *saveData);
u32 BattleTower_GetRecordMilestone(SaveData *saveData);
void sub_02067484(FieldSystem *fieldSystem, void *);
u32 BattleTower_NextRandom(u32 value);
void BattleTower_InitSeed(SaveData *saveData);
u32 BattleTower_AdvanceSeed(SaveData *saveData);
u32 BattleTower_RestoreRandom(SaveData *saveData);
BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem);
void Save_DullBadges(SaveData *saveData, int days);

#endif // POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H
