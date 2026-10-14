#ifndef POKEHEARTGOLD_FRIEND_ROSTER_H
#define POKEHEARTGOLD_FRIEND_ROSTER_H

#include "save.h"

typedef struct FriendRoster FriendRoster;

void Save_FriendRoster_LoadFromSave(SaveData *saveData);
void Save_FriendRoster_StoreToSave(SaveData *saveData);
FriendRoster *Save_FriendRoster_Get(SaveData *saveData);
int FriendRoster_CountFriends(struct FriendRoster *data);
void *FriendRoster_GetUserData(FriendRoster *data);
u16 *FriendRoster_GetPlayerName(struct FriendRoster *data, s32 index);
u16 *FriendRoster_GetGroupName(struct FriendRoster *data, s32 index);
s32 FriendRoster_GetEntryField(struct FriendRoster *data, s32 index, s32 field);
void FriendRoster_Compact(struct FriendRoster *data);
void FriendRoster_UpdateDate(struct FriendRoster *data, s32 index);
int FriendRoster_GetUsedSlotCount(struct FriendRoster *data);
void FriendRoster_SetEntryField(FriendRoster *data, int index, int field, u8 value);
void Save_FriendRoster_Init(struct FriendRoster *data);

#endif // POKEHEARTGOLD_FRIEND_ROSTER_H
