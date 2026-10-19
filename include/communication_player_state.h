#ifndef POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
#define POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H

#include "player_data.h"
#include "save_link_ruleset.h"

void CommPlayerInfo_Init(SaveData *saveData, LinkBattleRuleset *rules);
void CommPlayerInfo_Free(void);
BOOL CommPlayerInfo_Exists(void);
void CommPlayerInfo_SendLocal(void);
u32 CommPlayerInfo_GetEntrySize(void);
void CommPlayerInfo_RecvSetUnk3A4(int netId, int size, void *data, void *context);
void CommPlayerInfo_RecvEntry(int netId, int size, void *packet, void *context);
void CommPlayerInfo_RecvPlayerEntry(int netId, int size, void *src, void *context);
BOOL CommPlayerInfo_HostRedistribute(void);
u8 CommPlayerInfo_HasNewEntries(void);
void CommPlayerInfo_ClearPlayer(int netId);
BOOL CommPlayerInfo_IsReceived(int netId);
BOOL CommPlayerInfo_IsReceivedOrAccepted(int netId);
BOOL CommPlayerInfo_IsAccepted(int netId);
void CommPlayerInfo_Accept(int netId);
int CommPlayerInfo_FindReceived(void);
int CommPlayerInfo_CountAccepted(void);
BOOL CommPlayerInfo_DropDisconnected(void);
PlayerProfile *CommPlayerInfo_GetProfile(int netId);
void *CommPlayerInfo_GetExchangeToken(int netId);
u8 CommPlayerInfo_GetFriendIndex(int a);
u16 *CommPlayerInfo_GetGroupName(int netId);
u32 CommPlayerInfo_GetCountry(int netId);
u32 CommPlayerInfo_GetRegion(int netId);
BOOL CommPlayerInfo_RulesetsMatch(void);
void CommPlayerInfo_SaveRecords(SaveData *saveData);
void CommPlayerInfo_RecordBattleResult(SaveData *saveData, int b);
void CommPlayerInfo_RecordTrades(SaveData *saveData, int count);
void CommPlayerInfo_SetProfile(PlayerProfile *profile);

#endif // POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
