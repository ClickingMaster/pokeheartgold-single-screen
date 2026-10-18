#ifndef POKEHEARTGOLD_FRIEND_ROSTER_H
#define POKEHEARTGOLD_FRIEND_ROSTER_H

#include "save.h"

typedef struct FriendRoster FriendRoster;

void Save_FriendRoster_LoadFromSave(SaveData *saveData);
void Save_FriendRoster_StoreToSave(SaveData *saveData);
FriendRoster *Save_FriendRoster_Get(SaveData *saveData);
int FriendRoster_CountFriends(FriendRoster *data);
void *FriendRoster_GetUserData(FriendRoster *data);
u16 *FriendRoster_GetPlayerName(FriendRoster *data, s32 index);
u16 *FriendRoster_GetGroupName(FriendRoster *data, s32 index);
BOOL FriendRoster_IsFriendValid(FriendRoster *, int);
s32 FriendRoster_GetEntryField(FriendRoster *data, s32 index, s32 field);
void FriendRoster_Compact(FriendRoster *data);
void FriendRoster_UpdateDate(FriendRoster *data, s32 index);
void FriendRoster_AddWinsLossesTrades(FriendRoster *, s32, s32, s32, s32);
void FriendRoster_AddPlazaGame0Plays(FriendRoster *, s32, s32);
void FriendRoster_AddPlazaGame1Plays(FriendRoster *, s32, s32);
void FriendRoster_AddPlazaGame2Plays(FriendRoster *, s32, s32);
int FriendRoster_GetUsedSlotCount(FriendRoster *data);
void FriendRoster_RemoveFriend(FriendRoster *, int);
void FriendRoster_SetEntryField(FriendRoster *data, int index, int field, u8 value);
void Save_FriendRoster_Init(FriendRoster *data);
u32 Save_FriendRoster_sizeof(void);

#endif // POKEHEARTGOLD_FRIEND_ROSTER_H
