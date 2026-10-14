#ifndef POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
#define POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H

#include "player_data.h"

void CommPlayerInfo_Init(SaveData *saveData, int rules);
void CommPlayerInfo_Free(void);
BOOL CommPlayerInfo_Exists(void);
void CommPlayerInfo_SendLocal(void);
void CommPlayerInfo_HostRedistribute(void);
int CommPlayerInfo_HasNewEntries(void);
void CommPlayerInfo_ClearPlayer(int netId);
int CommPlayerInfo_IsReceived(int netId);
int CommPlayerInfo_IsReceivedOrAccepted(int netId);
int CommPlayerInfo_IsAccepted(int netId);
void CommPlayerInfo_Accept(int netId);
int CommPlayerInfo_FindReceived(void);
int CommPlayerInfo_CountAccepted(void);
PlayerProfile *CommPlayerInfo_GetProfile(u32 netId);
u8 CommPlayerInfo_GetFriendIndex(int a);
s32 CommPlayerInfo_RulesetsMatch();
void CommPlayerInfo_RecordBattleResult(SaveData *saveData, int b);

#endif // POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
