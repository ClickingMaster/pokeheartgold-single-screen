#ifndef POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H
#define POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H

#include "frontier_data.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_02069528.h"

void BattleTowerChallenge_Clear(u32 a0);
u32 BattleTowerChallenge_GetField(u32 a0, u32 a1, void *a2);
void BattleTowerChallenge_SetField(u32 a0, u32 a1, void *a2);
void BattleTowerChallenge_AddWifiScoreCounters(u32 a0, u8 a1, u32 a2, u32 a3);
u32 BattleTowerChallenge_GetSaved(u32 record);
void BattleTowerChallenge_SetSaved(u32 a0, u32 a1);
u32 FrontierData_WifiStreakAction(FrontierData *frontierData, u32 a1);
u32 FrontierData_WifiRankAction(FrontierData *frontierData, u32 a1);
void FrontierData_SetWifiTeam(FrontierData *frontierData, u32 a1, void *a2);
void FrontierData_GetWifiTeam(FrontierData *frontierData, s32 a1, FrontierWifiUploadRecord *a2);
u16 FrontierData_CalcWifiScore(FrontierData *frontierData, u32 a1);
u16 FrontierData_GetWifiScore(FrontierData *frontierData);
u32 FrontierData_TowerRoundAction(void *a0, u32 a1, u32 a2);
u32 FrontierData_SetTowerRound(FrontierData *frontierData, u32 a1, u16 a2);
u32 FrontierData_FlagAction(FrontierData *frontierData, u16 a1, u32 a2);
void Save_Frontier_SetTowerMessage(SaveData *saveData, int a1, MailMessage *mailMessage);
u32 BattleRoomDownload_HasTrainers(u32 a0);
u32 BattleRoomDownload_HasLeaders(u32 a0);
void BattleRoomDownload_GetLeadersRoom(u32 a0, u8 *a1);
u32 Save_BattleTowerChallenge_Get(SaveData *saveData);
u32 Save_BattleRoomDownload_Get(SaveData *saveData);
MailMessage *Save_Frontier_GetTowerMessage(SaveData *saveData, u32 a1);

#endif // POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H
