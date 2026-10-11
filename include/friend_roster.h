#ifndef POKEHEARTGOLD_FRIEND_ROSTER_H
#define POKEHEARTGOLD_FRIEND_ROSTER_H

#include "save.h"

typedef struct FriendRoster FriendRoster;

void Save_FriendRoster_LoadFromSave(SaveData *saveData);
void Save_FriendRoster_StoreToSave(SaveData *saveData);
FriendRoster *Save_FriendRoster_Get(SaveData *saveData);
int FriendRoster_CountFriends(struct FriendRoster *a0);
void *FriendRoster_GetUserData(FriendRoster *a0);
u16 *sub_0202C254(struct FriendRoster *a0, s32 a1);
u16 *sub_0202C298(struct FriendRoster *a0, s32 a1);
s32 FriendRoster_GetEntryField(struct FriendRoster *a0, s32 a1, s32 a2);
void FriendRoster_Compact(struct FriendRoster *a0);
void FriendRoster_UpdateDate(struct FriendRoster *a0, s32 a1);
int FriendRoster_GetUsedSlotCount(struct FriendRoster *a0);
void FriendRoster_SetEntryField(FriendRoster *a0, int a1, int a2, u8 a3);
void Save_FriendRoster_Init(struct FriendRoster *a0);

#endif // POKEHEARTGOLD_FRIEND_ROSTER_H
