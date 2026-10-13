#ifndef POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_INTERNAL_H
#define POKEHEARTGOLD_COMMUNICATION_PLAYER_STATE_INTERNAL_H

#include "global.h"

typedef struct CommPlayerInfoEntry {
    /* 0x00 */ u8 rules[0x20];
    /* 0x20 */ u8 profile[0x20];
    /* 0x40 */ u8 token[0xC];
    /* 0x4C */ u8 unk4C[0x10];
    /* 0x5C */ u8 mac[6];
    /* 0x62 */ u8 index;
    /* 0x63 */ u8 country;
    /* 0x64 */ u8 region;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 pad66[2];
} CommPlayerInfoEntry;

typedef struct CommPlayerRecord {
    /* 0x0 */ u16 wins;
    /* 0x2 */ u16 losses;
    /* 0x4 */ u16 trades;
} CommPlayerRecord;

typedef struct CommPlayerInfo {
    /* 0x000 */ void *unk00;
    /* 0x004 */ void *rules;
    /* 0x008 */ void *save;
    /* 0x00C */ CommPlayerInfoEntry entries[8];
    /* 0x34C */ void *profiles[8];
    /* 0x36C */ CommPlayerRecord scores[8];
    /* 0x39C */ u8 status[8];
    /* 0x3A4 */ u8 unk3A4;
    /* 0x3A5 */ u8 unk3A5;
    /* 0x3A6 */ u8 curIdx;
    /* 0x3A7 */ u8 unk3A7;
} CommPlayerInfo;

void *Heap_Alloc(u32 heapId, u32 size);
void Heap_Free(void *ptr);
void GF_AssertFail(void);
void *Save_PlayerData_GetProfile(void *save);
u32 PlayerProfile_sizeof(void);
void PlayerProfile_Copy(void *src, void *dst);
void PlayerProfile_Init(void *profile);
BOOL PlayerProfile_IsNameEmpty(void *profile);
void LinkBattleRuleset_Copy(void *src, void *dst);
void *FriendRoster_GetUserData(void *a);
void FriendRoster_AddWinsLossesTrades(void *a, int b, int c, int d, int e);
void *Save_FriendRoster_Get(void *save);
void *Save_FriendGroup_GetName(void *a, int b, int c);
void *Save_FriendGroup_Get(void *save);
void *Save_WiFiHistory_Get(void *save);
u8 WifiHistory_GetPlayerCountry(void *wifi);
u8 WiFiHistory_GetPlayerRegion(void *wifi);
void CommWireless_SetMacAddress(void *a, int b);
void Comm_SendCommand(int a, void *b, int c);
BOOL Comm_IsNetIdConnected(u16 netId);
int Comm_CountConnected(void);
void sub_0203753C(int a, void *b, int c);
int Comm_GetLocalNetId(void);
int sub_02037824(int a);
int sub_020378AC(int netId);
int Comm_FindFriendSlot(void *a, void *b, int *c);
u8 Comm_FindFriendIndexOfPlayer(void *save, int a);
void DWC_CreateExchangeToken(void *a, void *b);

extern CommPlayerInfo *gCommPlayerInfo;

void CommPlayerInfo_Init(void *save, void *rules);
void CommPlayerInfo_Free(void);
BOOL CommPlayerInfo_Exists(void);
void CommPlayerInfo_SendLocal(void);
int CommPlayerInfo_GetEntrySize(void);
void sub_02034524(void);
void CommPlayerInfo_RecvEntry(int netId, int unused, void *packet);
void CommPlayerInfo_RecvPlayerEntry(int netId, int unused, void *src);
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
void *CommPlayerInfo_GetProfile(int netId);
void *CommPlayerInfo_GetExchangeToken(int netId);
u8 sub_02034870(int a);
void *CommPlayerInfo_GetGroupName(int netId);
u8 CommPlayerInfo_GetCountry(int netId);
u8 CommPlayerInfo_GetRegion(int netId);
BOOL CommPlayerInfo_RulesetsMatch(void);
void CommPlayerInfo_AddToRecords(int mode, int delta);
void CommPlayerInfo_SaveRecords(void *save);
void CommPlayerInfo_RecordBattleResult(void *save, int b);
void CommPlayerInfo_RecordTrades(void *save, int b);
void CommPlayerInfo_SetProfile(void *a);

#endif
