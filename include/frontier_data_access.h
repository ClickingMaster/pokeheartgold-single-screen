#ifndef POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H
#define POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H

#include "frontier_data.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "frontier_wifi_upload_record.h"

void BattleTowerChallenge_Clear(u32 challenge);
u32 BattleTowerChallenge_GetField(u32 challenge, u32 field, void *out);
void BattleTowerChallenge_SetField(u32 challenge, u32 field, void *value);
void BattleTowerChallenge_AddWifiScoreCounters(u32 challenge, u8 a, u32 b, u32 c);
u32 BattleTowerChallenge_GetSaved(u32 record);
void BattleTowerChallenge_SetSaved(u32 challenge, u32 value);
u32 FrontierData_WifiStreakAction(FrontierData *frontierData, u32 action);
u32 FrontierData_WifiRankAction(FrontierData *frontierData, u32 action);
void FrontierData_SetWifiTeam(FrontierData *frontierData, u32 index, void *src);
void FrontierData_GetWifiTeam(FrontierData *frontierData, s32 index, FrontierWifiUploadRecord *dst);
u16 FrontierData_CalcWifiScore(FrontierData *frontierData, u32 challenge);
u16 FrontierData_GetWifiScore(FrontierData *frontierData);
u32 FrontierData_TowerRoundAction(void *ptr, u32 towerMode, u32 action);
u32 FrontierData_SetTowerRound(FrontierData *frontierData, u32 towerMode, u16 round);
u32 FrontierData_FlagAction(FrontierData *frontierData, u16 flag, u32 action);
void Save_Frontier_SetTowerMessage(SaveData *saveData, int index, MailMessage *mailMessage);
u32 BattleRoomDownload_HasTrainers(u32 download);
u32 BattleRoomDownload_HasLeaders(u32 download);
void BattleRoomDownload_GetLeadersRoom(u32 addr, u8 *out);
u32 Save_BattleTowerChallenge_Get(SaveData *saveData);
u32 Save_BattleRoomDownload_Get(SaveData *saveData);
MailMessage *Save_Frontier_GetTowerMessage(SaveData *saveData, u32 index);

#endif // POKEHEARTGOLD_FRONTIER_DATA_ACCESS_H
