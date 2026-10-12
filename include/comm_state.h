#ifndef POKEHEARTGOLD_COMM_STATE_H
#define POKEHEARTGOLD_COMM_STATE_H

#include "overlay_44_02232E9C.h"
#include "save.h"
#include "save_wifi_history.h"
#include "friend_roster.h"

BOOL CommState_Exists(void);
void CommState_SetEntry(u32 arg0);
void sub_02037FF0(void);
void sub_020380B0(void);
void sub_020380CC(void);
u32 *CommState_GetWifiError(void);
BOOL sub_020393C8(void);
u8 CommState_GetLinkMode(void);
u8 sub_02039954(void);
int CommState_IsWifi(void);
int sub_02039FFC(int a0);
BOOL sub_02039AA4(void);
u8 CommState_GetErrorScreen(void);
BOOL sub_02038D90(void);
BOOL sub_0203A05C(SaveData *saveData);
void sub_02039F68(SaveWiFiHistory *);
BOOL CommState_SetError(int);
void *sub_0203A040(FriendRoster *a0);
void CommState_SetErrorWatch(s8 a0, s8 a1);
void sub_02039B58(void);
void sub_02039358(void);
BOOL sub_020390C4(void);
void sub_020378E4(int a0);
void sub_02039B18(void);
void sub_02039AF8(void);
s32 sub_02039080(s32);
u8 sub_02039264(void);
UnkStruct_ov44_0223197C *sub_020398C8(void);
void sub_020393B4(void);
BOOL sub_02039274(void);
void sub_02039330(void);
void sub_0203A1C4(SaveData *saveData, void *a1, enum HeapID heapID);
void sub_02039B7C(void);
BOOL sub_020392A0(void);
UnkStruct_ov44_0223197C *sub_020396FC(SaveData *saveData, u32 a1);
void sub_02039378(void);
int sub_02039FD8(enum HeapID heapID);
void sub_02039E9C(void);
void sub_02039FB8(SaveWiFiHistory *history, s32 country, s32 region, u8 language);

#endif // POKEHEARTGOLD_COMM_STATE_H
