#ifndef POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
#define POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H

#include "player_data.h"

void CommPlayerInfo_Init(SaveData *saveData, int a1);
void CommPlayerInfo_Free(void);
BOOL CommPlayerInfo_Exists(void);
void CommPlayerInfo_SendLocal(void);
void CommPlayerInfo_HostRedistribute(void);
int CommPlayerInfo_HasNewEntries(void);
void CommPlayerInfo_ClearPlayer(int arg0);
int CommPlayerInfo_IsReceived(int arg0);
int CommPlayerInfo_IsReceivedOrAccepted(int arg0);
int CommPlayerInfo_IsAccepted(int arg0);
void CommPlayerInfo_Accept(int arg0);
int CommPlayerInfo_FindReceived(void);
int CommPlayerInfo_CountAccepted(void);
PlayerProfile *CommPlayerInfo_GetProfile(u32 a0);
u8 CommPlayerInfo_GetFriendIndex(int arg0);
s32 CommPlayerInfo_RulesetsMatch();
void CommPlayerInfo_RecordBattleResult(SaveData *saveData, int a1);

#endif // POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_H
