#ifndef POKEHEARTGOLD_COMM_STATE_H
#define POKEHEARTGOLD_COMM_STATE_H

#include "overlay_44_02232E9C.h"
#include "save.h"
#include "save_wifi_history.h"
#include "friend_roster.h"

BOOL CommState_Exists(void);
void CommState_SetEntry(u32 arg0);
void CommState_ResumeAlternateSearch(void);
void CommState_RefuseJoins(void);
void CommState_RefuseJoinsAndRestart(void);
u32 *CommState_GetWifiError(void);
BOOL CommState_HasWifiFailed(void);
u8 CommState_GetLinkMode(void);
u8 CommState_GetBeaconId(void);
int CommState_IsWifi(void);
int Comm_LoadAndInitDwc(int a0);
BOOL CommState_IsLinkDown(void);
u8 CommState_GetErrorScreen(void);
BOOL CommState_StopForReset(void);
BOOL Comm_HasWifiProfile(SaveData *saveData);
void Comm_RecordPlayerLocations(SaveWiFiHistory *);
BOOL CommState_SetError(int);
void *Comm_GetOwnGsProfileId(FriendRoster *a0);
void CommState_SetErrorWatch(s8 a0, s8 a1);
void CommState_SetWifiClubLinkMode(void);
void CommState_LeaveWifiMatch(void);
BOOL CommState_GetWifiMatchStatus(void);
void sub_020378E4(int a0);
void sub_02039B18(void);
void sub_02039AF8(void);
s32 CommState_StartWifiMatch(s32);
u8 CommState_WasWifiDisconnected(void);
UnkStruct_ov44_0223197C *CommState_GetWifiClubData(void);
void CommState_SendWifiDisconnect(void);
BOOL CommState_IsWifiJoiningLobby(void);
void CommState_EndWifiSession(void);
void Comm_RegisterLinkedFriends(SaveData *saveData, void *a1, enum HeapID heapID);
void sub_02039B7C(void);
BOOL CommState_IsWifiInLobby(void);
UnkStruct_ov44_0223197C *CommState_StartWifiClub(SaveData *saveData, u32 a1);
void CommState_EndWifiMatch(void);
int Comm_InitDwc(enum HeapID heapID);
void sub_02039E9C(void);
void Comm_MarkLocationSeen(SaveWiFiHistory *history, s32 country, s32 region, u8 language);

#endif // POKEHEARTGOLD_COMM_STATE_H
