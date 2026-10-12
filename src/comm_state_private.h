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
BOOL sub_02037474(void);
void sub_020374E4(void);
u16 Comm_GetLocalNetId(void);
BOOL sub_02037700(void);

typedef struct {
    u64 x;
    u64 mul;
    u64 add;
} MATHRandContext32;

void CommRandom_Init(MATHRandContext32 *arg0);
void Comm_FreeCommandTable(void);
void CommWireless_Terminate(void);
void sub_02036274(void);
int CommWireless_Stop(void);
void sub_02036024(void);
void Wireless_SetEntry(int arg0);
void Wireless_SetEntryLocked(int arg0);
void Wireless_SetMaxEntries(int arg0);
void Wireless_SetEntryClosed(int arg0);
void WirelessStatusIcon_SetLevel(int arg0);
int ov00_021EC9D4(void);
s32 WM_GetLinkLevel(void);
int CommWireless_IsIdle(void);
void CommWireless_SetManagerEnded(int arg0);
void sub_0203622C(void);
void CommWireless_Init(PlayerProfile *playerProfile, int arg1);
int sub_02035ED8(int arg0, int arg1, int arg2, int arg3);
void sub_02035FD8(void);
int sub_02035F14(int arg0, int arg1, int arg2);
void CommWireless_UpdateParents(void);
int sub_020360E4(int arg0);
void sub_0203624C(void);
int CommWireless_HasSentBeacons(void);
int sub_020376F8(void);
int CommWireless_GetParentMemberCount(int arg0);
void sub_020376E0(int arg0, u8 *arg1);
void sub_02035FE4(void);
void sub_02037184(int arg0, u8 *arg1);
void Wireless_SetBeaconCallback(void *arg0);
void sub_02037988(void);
void WifiConnection_Shutdown(void);
int WifiConnection_Update(int arg0);
void sub_0203786C(int arg0);
u32 WifiConnection_StartMatchmaking(s32 arg0, s32 arg1, s32 arg2);
s32 sub_02033FC4(u8 arg0);
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
int ov45_0222F274(int arg0);
void ov45_0222F028(int arg0, int arg1);
void ov45_0222F350(int arg0);
void CommWireless_SetDetectDrops(int arg0);
s32 sub_02034044(void);
BOOL CommWireless_IsManagerEnded(void);
BOOL CommWireless_IsInitialized(void);
int Wireless_HasFatalError(void);
void sub_020378E4(int arg0);
int ov45_0222E8F4(void);
int WiFiPlazaLobbyClient_Update(void);
int ov45_0222E944(void);
int CommPlayerInfo_GetCountry(int arg0);
int CommPlayerInfo_GetRegion(int arg0);
void LoadOVY38(void);
void UnloadOVY38(void);
void CommPlayerInfo_SaveRecords(SaveData *saveData);

s32 sub_02037C94(void);
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
void sub_02037F18(SaveData *saveData);
void CommState_JoinParent(s8 arg0);
s32 CommState_GetJoinResult(void);
s32 CommState_IsAlternateLinked(void);
s32 sub_02038070(void);
void sub_02038094(void);
void CommState_SetEntry(u32 arg0);
void sub_02037FF0(void);
void sub_020380B0(void);
void sub_020380CC(void);
void sub_020380F4(void);
void sub_02038104(void);
void sub_02038114(SaveData *saveData, s8 arg1);
void sub_02038148(SaveData *saveData, s8 arg1);
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
void sub_02038418(void);
void sub_02038460(void);
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
void sub_020388A8(void);
void sub_020388E0(void);
void sub_02038918(s8 arg0);
void sub_0203893C(void);
void sub_0203894C(s8 arg0);
void sub_0203897C(void);
void sub_0203898C(s8 arg0);
void sub_020389BC(void);
void sub_020389D4(void);
void sub_02038A10(void);
void sub_02038A94(void);
void CommState_RecvJoinRequest(u8 arg0, u8 arg1, u8 *arg2);
void CommState_RecvJoinReply(u32 arg0, u32 arg1, u8 *arg2);
s32 sub_02038C18(void);
void CommState_SetMaxEntries(s8 arg0);
void sub_02038C34(void);
void CommState_OnBeacon(u32 arg0, s32 arg1);
void sub_02038CC4(void);
void sub_02038CE8(void);
void sub_02038D28(SaveData *saveData);
void sub_02038D64(void);
u8 CommState_GetSeenGames(void);
BOOL sub_02038D90(void);
void sub_02038E00(void);
void sub_02038E48(void);
void sub_02038E90(void);
void sub_02038E94(void);
void sub_02038EB0(void);
void sub_02038EB4(void);
void sub_02038EB8(void);

void CommState_SetWifiError(u32 arg0, s32 arg1, s32 arg2);
void sub_02038F9C(void);
void sub_0203900C(void);
s32 sub_02039080(s32 arg0);
s32 sub_020390C4(void);
void sub_02039144(void);
void sub_020391D0(void);
void sub_02039220(void);
u8 sub_02039264(void);
BOOL sub_02039274(void);
BOOL sub_020392A0(void);
u32 *CommState_GetWifiError(void);
void sub_020392F4(void);
void sub_02039330(void);
void sub_02039358(void);
void sub_02039378(void);
void sub_020393B4(void);
BOOL sub_020393C8(void);
void sub_02039418(SaveData *saveData);
void sub_0203946C(void);
void sub_020394A0(SaveData *saveData);
void sub_020394F4(void);
void sub_02039528(SaveData *saveData);
void sub_0203957C(void);
BOOL sub_020395B0(void);
void sub_0203960C(void);
void sub_02039694(void);
UnkStruct_ov44_0223197C *sub_020396FC(SaveData *saveData, u32 arg1);
void sub_0203976C(SaveData *saveData, s32 arg1);
void sub_020397C8(void);
u8 sub_020397E4(void);
BOOL sub_020397FC(void);
void sub_0203981C(s32 arg0);
void sub_0203986C(void);
s32 sub_0203988C(void);
UnkStruct_ov44_0223197C *sub_020398C8(void);
void CommState_SetErrorWatch(s8 arg0, s8 arg1);
BOOL CommState_ReportsErrors(void);
u8 CommState_GetLinkMode(void);
u8 sub_02039954(void);
void sub_0203996C(void *arg0);
void sub_02039980(void *arg0);
int CommState_IsWifi(void);

void CommState_SetErrorScreen(s8 arg0);
u8 CommState_GetErrorScreen(void);
void CommState_ShowErrorAndHalt(u32 errorCode);
void sub_020399EC(void);
void sub_020399FC(void);
void CommState_CheckLinkError(void);
BOOL sub_02039AA4(void);
BOOL CommState_SetError(int arg0);
void sub_02039AF8(void);
void sub_02039B18(void);
void sub_02039B38(void);
void sub_02039B58(void);
void sub_02039B7C(void);
void sub_02039BA0(void);
void sub_02039C14(void);
void sub_02039C60(void);
s32 sub_02039C6C(s32 arg0);
s32 sub_02039D08(void);
int sub_02039D3C(void);
void sub_02039D54(s32 *arg0);
void sub_02039D78(void);
void sub_02039D8C(void);
void sub_02039DC0(void);
void sub_02039DC4(void);
void sub_02039DE4(void);
void sub_02039E30(void);
void sub_02039E7C(void);
void sub_02039E9C(void);
void sub_02039EAC(s32 *arg0);
s32 sub_02039EB4(SaveData *saveData, s32 arg1, u32 *arg2);
void sub_02039F68(SaveWiFiHistory *arg0);
void sub_02039FB8(SaveWiFiHistory *arg0, s32 arg1, s32 arg2, u8 arg3);
s32 sub_02039FD8(enum HeapID heapID);
s32 sub_02039FFC(enum HeapID heapID);
void sub_0203A01C(WiFiList *arg0);
int sub_0203A040(WiFiList *arg0);
BOOL sub_0203A05C(SaveData *saveData);
s32 sub_0203A084(SaveData *saveData, DWCFriendData *arg1, s32 *arg2);
int sub_0203A128(SaveData *saveData, u64 arg1, int *arg2);
s32 sub_0203A1C4(SaveData *saveData, s32 *arg1, enum HeapID heapID);
void sub_0203A280(SaveData *saveData, s32 arg1, s32 arg2, enum HeapID heapID, u32 arg4);
s32 sub_0203A378(SaveData *saveData, s32 arg1);

int ov45_0222F464(void);
void ov45_0222F154(void);
void ov45_0222F70C(void);
void ov45_0222E91C(void);
int ov45_0222F254(void);

DWCFriendData *CommPlayerInfo_GetExchangeToken(int arg0);
u16 *CommPlayerInfo_GetGroupName(int arg0);
void sub_020367A8(void);
void sub_02036904(void);
void sub_020369CC(void);

#endif // POKEHEARTGOLD_PR488_PRIVATE_H
