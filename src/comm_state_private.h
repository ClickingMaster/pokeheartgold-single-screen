#ifndef POKEHEARTGOLD_DWCACCOUNT_H
#define POKEHEARTGOLD_DWCACCOUNT_H

// This module predates the typed declarations in the current public headers.
// Keep its verified DWC and Wi-Fi interfaces private until those headers are
// reconciled for every caller.
#include "global.h"

typedef struct DWCUserData DWCUserData;

typedef struct DWCFriendData {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} DWCFriendData;

BOOL DWC_CheckValidConsole(DWCUserData *userData);
BOOL DWC_CheckHasProfile(DWCUserData *userData);
int DWC_IsValidFriendData(DWCFriendData *friendData);
void DWC_CreateExchangeToken(DWCUserData *, DWCFriendData *);
void DWC_CreateFriendKeyToken(DWCFriendData *friendData, u64 a1);
int DWC_CheckFriendKey(DWCUserData *userData, u64 a1);
int DWC_GetGsProfileId(DWCUserData *userData, DWCFriendData *friendData);
void DWC_CreateUserData(DWCUserData *userData, s32 a1);
void DWC_ClearDirtyFlag(DWCUserData *userData);
int DWC_CheckUserData(DWCUserData *userData);
int DWC_Init(int);
int DWC_IsEqualFriendData(DWCFriendData *friendData0, DWCFriendData *friendData1);

#endif // POKEHEARTGOLD_DWCACCOUNT_H

#ifndef POKEHEARTGOLD_FRIEND_ROSTER_H
#define POKEHEARTGOLD_FRIEND_ROSTER_H

#include "pm_string.h"
#include "save.h"

typedef struct FriendRoster FriendRoster;
typedef FriendRoster WiFiList;

void Save_FriendRoster_LoadFromSave(SaveData *saveData);
void Save_FriendRoster_StoreToSave(SaveData *saveData);
WiFiList *Save_FriendRoster_Get(SaveData *saveData);
int FriendRoster_CountFriends(WiFiList *a0);
DWCUserData *FriendRoster_GetUserData(WiFiList *a0);
u16 *sub_0202C254(WiFiList *a0, s32 a1);
u16 *sub_0202C298(WiFiList *a0, s32 a1);
s32 FriendRoster_GetEntryField(WiFiList *a0, s32 a1, s32 a2);
void FriendRoster_Compact(WiFiList *a0);
void FriendRoster_UpdateDate(WiFiList *a0, s32 a1);
u16 FriendRoster_GetUsedSlotCount(WiFiList *a0);
void FriendRoster_SetEntryField(WiFiList *a0, int a1, int a2, u32 a3);
void Save_FriendRoster_Init(WiFiList *a0);
void sub_0202C2B4(WiFiList *a0, int a1, String *a2);
void sub_0202C270(WiFiList *a0, int a1, String *a2);
DWCFriendData *FriendRoster_GetDwcFriendData(WiFiList *a0, int a1);

#endif // POKEHEARTGOLD_FRIEND_ROSTER_H

#ifndef POKEHEARTGOLD_PR488_PRIVATE_H
#define POKEHEARTGOLD_PR488_PRIVATE_H

#include "overlay_44_02232E9C.h"
#include "save.h"
#include "save_wifi_history.h"
#include "friend_roster.h"

BOOL Comm_IsNetIdConnected(u16 arg0);
int Comm_CountConnected(void);
BOOL Comm_IsActive(void);
void Comm_EnableKeySync(void);
u16 Comm_GetLocalNetId(void);
BOOL Comm_HasLinkError(void);

typedef struct {
    u64 x;
    u64 mul;
    u64 add;
} MATHRandContext32;

void CommRandom_Init(MATHRandContext32 *arg0);
void Comm_FreeCommandTable(void);
void CommWireless_Terminate(void);
void Comm_ResetWorkAndParents(void);
int CommWireless_Stop(void);
void Comm_Shutdown(void);
void Wireless_SetEntry(int arg0);
void Wireless_SetEntryLocked(int arg0);
void Wireless_SetMaxEntries(int arg0);
void Wireless_SetEntryClosed(int arg0);
void WirelessStatusIcon_SetLevel(int arg0);
int ov00_021EC9D4(void);
s32 WM_GetLinkLevel(void);
int CommWireless_IsIdle(void);
void CommWireless_SetManagerEnded(int arg0);
void Comm_ResetWork(void);
void CommWireless_Init(PlayerProfile *playerProfile, int arg1);
int Comm_StartParent(int arg0, int arg1, int arg2, int arg3);
void sub_02035FD8(void);
int Comm_StartScan(int arg0, int arg1, int arg2);
void CommWireless_UpdateParents(void);
int Comm_ConnectToParent(int arg0);
void sub_0203624C(void);
int CommWireless_HasSentBeacons(void);
int Comm_GetConnectedChildren(void);
int CommWireless_GetParentMemberCount(int arg0);
void Comm_SendFixedCommand(int arg0, u8 *arg1);
void sub_02035FE4(void);
void Comm_SendFixedParentCommand(int arg0, u8 *arg1);
void Wireless_SetBeaconCallback(void *arg0);
void Comm_StopFrameUpdate(void);
void WifiConnection_Shutdown(void);
int WifiConnection_Update(int arg0);
void Comm_SetWifiConnected(int arg0);
u32 WifiConnection_StartMatchmaking(s32 arg0, s32 arg1, s32 arg2);
s32 Comm_GetMaxChildren(u8 arg0);
void WifiConnection_SetReceiveCallbacks(void (*func0)(), void (*func1)());
s32 WifiConnection_CheckError(void);
int WifiConnection_Disconnect(int arg0);
int WifiConnection_FinishDisconnect(void);
int ov00_021EC11C(s32 *arg0, s32 *arg1);
void ov00_021EC210(void);
s32 WifiConnection_UpdateLogin(void);
int ov45_0222E804(int arg0);
void WifiConnection_Init(SaveData *saveData, int arg1, int arg2, int arg3);
void WifiConnection_SetErrorCallback(void (*func)(u32));
int WiFiPlazaLobbyClient_IsMatchActive(int arg0);
void WiFiPlazaLobbyClient_HostMatch(int arg0, int arg1);
void WiFiPlazaLobbyClient_JoinMatch(int arg0);
void CommWireless_SetDetectDrops(int arg0);
s32 Comm_IsWifiLinkMode(void);
BOOL CommWireless_IsManagerEnded(void);
BOOL CommWireless_IsInitialized(void);
int Wireless_HasFatalError(void);
void sub_020378E4(int arg0);
int WiFiPlazaLobbyClient_IsInLobby(void);
int WiFiPlazaLobbyClient_Update(void);
int WiFiPlazaLobbyClient_IsDisconnected(void);
int CommPlayerInfo_GetCountry(int arg0);
int CommPlayerInfo_GetRegion(int arg0);
void LoadOVY38(void);
void UnloadOVY38(void);
void CommPlayerInfo_SaveRecords(SaveData *saveData);

s32 CommState_GetSyncPacketSize(void);
void CommState_Init(SaveData *saveData, s8 arg1);
void CommState_Free(void);
BOOL CommState_Exists(void);
void CommState_StartParentSession(SaveData *saveData, s8 arg1, s8 arg2, s32 arg3);
void CommState_StartChildSession(SaveData *saveData, s8 arg1, s8 arg2, s32 arg3);
void CommState_ConnectToSlot(s8 arg0);
void CommState_RestartScan(void);
void CommState_ShutdownWait(void);
void CommState_RequestShutdown(void);
s32 CommState_IsLinked(void);
void CommState_StartAlternateSearch(SaveData *saveData);
void CommState_JoinParent(s8 arg0);
s32 CommState_GetJoinResult(void);
s32 CommState_IsAlternateLinked(void);
s32 CommState_IsAlternateSearchReady(void);
void CommState_StopAlternateSearch(void);
void CommState_SetEntry(u32 arg0);
void CommState_ResumeAlternateSearch(void);
void CommState_RefuseJoins(void);
void CommState_RefuseJoinsAndRestart(void);
void CommState_SetUnionAppLinkMode(void);
void CommState_SetUnionRoomLinkMode(void);
void CommState_StartMysteryGiftParent(SaveData *saveData, s8 arg1);
void CommState_StartMysteryGiftChild(SaveData *saveData, s8 arg1);
void CommState_Main(void);
void CommState_SetState(void (*func)(), int arg1);
void CommState_EndWireless(void);
void CommState_StopWireless(void);
void CommState_ParentStart(void);
void CommState_WaitLinked(void);
void CommState_ParentLinked(void);
void CommState_ChildStartScan(void);
void CommState_ChildScanning(void);
void CommState_ChildConnecting(void);
void CommState_ChildWaitLinked(void);
void CommState_ChildRescanStop(void);
void CommState_ChildRescan(void);
void CommState_ChildLinked(void);
void CommState_RestartScanStop(void);
void CommState_RestartScanStart(void);
void CommState_Idle(void);
void CommState_FreeWhenDone(void);
void CommState_ResumeParentWaitAlone(void);
void CommState_ResumeChildStop(void);
void CommState_ShutdownWaitStopped(void);
void CommState_AlternateInit(void);
void CommState_AlternateScan(void);
void CommState_AlternateWaitStopped(void);
void CommState_AlternateStartParent(void);
void CommState_AlternateParentWait(void);
void CommState_AlternateStartScan(void);
void CommState_JoinWaitStopped(void);
void CommState_JoinStartScan(void);
void CommState_JoinConnect(void);
void CommState_JoinWaitLinked(void);
void CommState_JoinWaitReply(void);
void CommState_Joined(void);
void CommState_JoinFailed(void);
void CommState_JoinRetry(void);
void CommState_AlternateLinked(void);
void CommState_RestartIdle(void);
void sub_020388E0(void);
void CommState_ReconnectToParent(s8 arg0);
void CommState_SetRecordMixingLinkMode(void);
void CommState_JoinRecordMixing(s8 arg0);
void CommState_SetSpinTradeLinkMode(void);
void CommState_JoinSpinTrade(s8 arg0);
void CommState_ReconnectStop(void);
void CommState_ReconnectScan(void);
void CommState_ReconnectWaitParent(void);
void CommState_ReconnectWaitLinked(void);
void CommState_RecvJoinRequest(u8 arg0, u8 arg1, u8 *arg2);
void CommState_RecvJoinReply(u32 arg0, u32 arg1, u8 *arg2);
s32 CommState_GetJoinMessageSize(void);
void CommState_SetMaxEntries(s8 arg0);
void CommState_BeaconWatch(void);
void CommState_OnBeacon(u32 arg0, s32 arg1);
void CommState_BeaconWatchScan(void);
void CommState_BeaconWatchInit(void);
void CommState_StartBeaconWatch(SaveData *saveData);
void CommState_StopBeaconWatch(void);
u8 CommState_GetSeenGames(void);
BOOL CommState_StopForReset(void);
void CommState_MysteryGiftParentStart(void);
void CommState_MysteryGiftChildStart(void);
void CommState_WifiFailed(void);
void CommState_WifiTimedOut(void);
void CommState_WifiDisconnected(void);
void CommState_WifiCancelled(void);
void CommState_WifiConnected(void);

void CommState_SetWifiError(u32 arg0, s32 arg1, s32 arg2);
void CommState_WifiLobby(void);
void CommState_WifiStartMatch(void);
s32 CommState_StartWifiMatch(s32 arg0);
s32 CommState_GetWifiMatchStatus(void);
void CommState_WifiJoinLobby(void);
void CommState_WifiDisconnect(void);
void CommState_RecvWifiDisconnect(void);
u8 CommState_WasWifiDisconnected(void);
BOOL CommState_IsWifiJoiningLobby(void);
BOOL CommState_IsWifiInLobby(void);
u32 *CommState_GetWifiError(void);
void CommState_WifiShutdown(void);
void CommState_EndWifiSession(void);
void CommState_LeaveWifiMatch(void);
void CommState_EndWifiMatch(void);
void CommState_SendWifiDisconnect(void);
BOOL CommState_HasWifiFailed(void);
void CommState_InitWfcSession(SaveData *saveData);
void CommState_FreeWfcSession(void);
void CommState_InitMysteryGiftWfcSession(SaveData *saveData);
void CommState_FreeMysteryGiftWfcSession(void);
void CommState_InitWiiMessageWfcSession(SaveData *saveData);
void CommState_FreeWiiMessageWfcSession(void);
BOOL CommState_CheckDwcError(void);
void CommState_WifiLogin(void);
void CommState_WifiClubStart(void);
UnkStruct_ov44_0223197C *CommState_StartWifiClub(SaveData *saveData, u32 arg1);
void CommState_StartWifiPlaza(SaveData *saveData, s32 arg1);
void CommState_LeaveWifiPlaza(void);
u8 CommState_HasPlazaLobbyStarted(void);
BOOL CommState_HasPlazaLobbyFailed(void);
void CommState_StartPlazaMatch(s32 arg0);
void CommState_EndPlazaMatch(void);
s32 CommState_GetPlazaMatchStatus(void);
UnkStruct_ov44_0223197C *CommState_GetWifiClubData(void);
void CommState_SetErrorWatch(s8 arg0, s8 arg1);
BOOL CommState_ReportsErrors(void);
u8 CommState_GetLinkMode(void);
u8 CommState_GetBeaconId(void);
void CommState_SetSelectedOrder(void *arg0);
void CommState_GetSelectedOrder(void *arg0);
int CommState_IsWifi(void);

void CommState_SetErrorScreen(s8 arg0);
u8 CommState_GetErrorScreen(void);
void CommState_ShowErrorAndHalt(u32 errorCode);
void CommState_ShowRestartErrorAndHalt(void);
void sub_020399FC(void);
void CommState_CheckLinkError(void);
BOOL CommState_IsLinkDown(void);
BOOL CommState_SetError(int arg0);
void sub_02039AF8(void);
void CommState_SetClubWifiLinkMode(void);
void CommState_SetPlazaLinkMode(void);
void CommState_SetWifiClubLinkMode(void);
void CommState_SetSingleBattleWifiLinkMode(void);
void CommState_PlazaStart(void);
void CommState_PlazaWaitLobby(void);
void CommState_PlazaLobby(void);
s32 CommState_PlazaCheckWifiResult(s32 arg0);
s32 CommState_PlazaUpdateLobby(void);
int CommState_PlazaUpdate(void);
void CommState_PlazaUpdateMatch(s32 *arg0);
void CommState_PlazaLogout(void);
void CommState_PlazaWaitLogout(void);
void CommState_PlazaLobbyFailed(void);
void CommState_PlazaMatchConnected(void);
void CommState_PlazaMatching(void);
void CommState_PlazaEndMatch(void);
void CommState_PlazaTimedOut(void);
void CommState_SetFrontierWifiLinkMode(void);
void Comm_InitSyncedSave(s32 *arg0);
s32 Comm_SyncedSave(SaveData *saveData, s32 arg1, u32 *arg2);
void Comm_RecordPlayerLocations(SaveWiFiHistory *arg0);
void Comm_MarkLocationSeen(SaveWiFiHistory *arg0, s32 arg1, s32 arg2, u8 arg3);
s32 Comm_InitDwc(enum HeapID heapID);
s32 Comm_LoadAndInitDwc(enum HeapID heapID);
void Comm_EnsureDwcUserData(WiFiList *arg0);
int Comm_GetOwnGsProfileId(WiFiList *arg0);
BOOL Comm_HasWifiProfile(SaveData *saveData);
s32 Comm_FindFriendSlot(SaveData *saveData, DWCFriendData *arg1, s32 *arg2);
int Comm_FindFriendSlotByKey(SaveData *saveData, u64 arg1, int *arg2);
s32 Comm_RegisterLinkedFriends(SaveData *saveData, s32 *arg1, enum HeapID heapID);
void Comm_StoreFriendFromPlayer(SaveData *saveData, s32 arg1, s32 arg2, enum HeapID heapID, u32 arg4);
s32 Comm_FindFriendIndexOfPlayer(SaveData *saveData, s32 arg1);

int WiFiPlazaLobbyClient_IsMatchHost(void);
void ov45_0222F154(void);
void WiFiPlazaLobbyClient_EndMatch(void);
void ov45_0222E91C(void);
int ov45_0222F254(void);

DWCFriendData *CommPlayerInfo_GetExchangeToken(int arg0);
u16 *CommPlayerInfo_GetGroupName(int arg0);
void Comm_RecvParentFrameCallback(void);
void Comm_RecvChildFrameCallback(void);
void Comm_RecvPeerFrameCallback(void);

#endif // POKEHEARTGOLD_PR488_PRIVATE_H
