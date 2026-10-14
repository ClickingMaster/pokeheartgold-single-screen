#ifndef POKEHEARTGOLD_COMM_STATE_H
#define POKEHEARTGOLD_COMM_STATE_H

#include "wifi_club_app.h"
#include "save.h"
#include "save_wifi_history.h"
#include "friend_roster.h"

BOOL CommState_Exists(void);
void CommState_SetEntry(u32 enabled);
void CommState_ResumeAlternateSearch(void);
void CommState_RefuseJoins(void);
void CommState_RefuseJoinsAndRestart(void);
u32 *CommState_GetWifiError(void);
BOOL CommState_HasWifiFailed(void);
u8 CommState_GetLinkMode(void);
u8 CommState_GetBeaconId(void);
int CommState_IsWifi(void);
int Comm_LoadAndInitDwc(int heapID);
BOOL CommState_IsLinkDown(void);
u8 CommState_GetErrorScreen(void);
BOOL CommState_StopForReset(void);
BOOL Comm_HasWifiProfile(SaveData *saveData);
void Comm_RecordPlayerLocations(SaveWiFiHistory *);
BOOL CommState_SetError(int);
void *Comm_GetOwnGsProfileId(FriendRoster *wifiList);
void CommState_SetErrorWatch(s8 watchMembers, s8 reportErrors);
void CommState_SetWifiClubLinkMode(void);
void CommState_LeaveWifiMatch(void);
BOOL CommState_GetWifiMatchStatus(void);
void sub_020378E4(int value);
void CommState_SetClubWifiLinkMode(void);
void sub_02039AF8(void);
s32 CommState_StartWifiMatch(s32);
u8 CommState_WasWifiDisconnected(void);
WifiClubStatusData *CommState_GetWifiClubData(void);
void CommState_SendWifiDisconnect(void);
BOOL CommState_IsWifiJoiningLobby(void);
void CommState_EndWifiSession(void);
void Comm_RegisterLinkedFriends(SaveData *saveData, void *results, enum HeapID heapID);
void CommState_SetSingleBattleWifiLinkMode(void);
BOOL CommState_IsWifiInLobby(void);
WifiClubStatusData *CommState_StartWifiClub(SaveData *saveData, u32 dataSize);
void CommState_EndWifiMatch(void);
int Comm_InitDwc(enum HeapID heapID);
void CommState_SetFrontierWifiLinkMode(void);
void Comm_MarkLocationSeen(SaveWiFiHistory *history, s32 country, s32 region, u8 language);

#endif // POKEHEARTGOLD_COMM_STATE_H
