#ifndef POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H
#define POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H

#include "script.h"

void TaskManager_CallFrontierPartySelect(TaskManager *taskManager, void *a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void TaskManager_CallFrontierWifiConnection(TaskManager *taskManager, u32 a1, u32 a2, u16 a3);
void TaskManager_CallFrontierLinkPartnerCheck(TaskManager *taskManager, u32 a1, u32 a2);
u32 sub_020672D8(SaveData *saveData);
u32 sub_02067398(SaveData *saveData);
void sub_02067484(FieldSystem *fieldSystem, void *);
u32 BattleTower_NextRandom(u32 a0);
void BattleTower_InitSeed(SaveData *saveData);
u32 BattleTower_AdvanceSeed(SaveData *saveData);
u32 BattleTower_RestoreRandom(SaveData *saveData);
BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem);
void Save_DullBadges(SaveData *saveData, int a1);

#endif // POKEHEARTGOLD_FRONTIER_FIELD_SUPPORT_H
