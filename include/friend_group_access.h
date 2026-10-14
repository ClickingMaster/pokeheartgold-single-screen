#ifndef POKEHEARTGOLD_FRIEND_GROUP_ACCESS_H
#define POKEHEARTGOLD_FRIEND_GROUP_ACCESS_H

#include "friend_group.h"

// Prototypes of src/friend_group.c as its callers were compiled; several parameter and return
// types differ from the definitions (e.g. the group id is read back as a u8), so the definition
// file does not include this header.

void Save_FriendGroup_SetGroupId(SAV_FRIEND_GRP *saveGrp, u32 grpIdx, u32 id);
void Save_FriendGroup_AdvanceSeeds(SAV_FRIEND_GRP *saveGrp, int days);
BOOL Save_FriendGroup_IsGroupValid(SAV_FRIEND_GRP *group, u16 index);
BOOL Save_FriendGroup_IsCurrentGroup(SAV_FRIEND_GRP *group, u16 index);
void Save_FriendGroup_CopyGroup(SAV_FRIEND_GRP *group, u16 src, int dst);

// return group->groups[a1].id;
u8 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *group, int index);
void Save_FriendGroup_SetName(SAV_FRIEND_GRP *group, int index, int type, String *str);
void Save_FriendGroup_SetLeaderGender(SAV_FRIEND_GRP *group, int index, u32 gender);
void Save_FriendGroup_SetLanguage(SAV_FRIEND_GRP *group, int index, int language);
BOOL Save_FriendGroup_IsGroupNameTaken(SAV_FRIEND_GRP *group, u16 *name);

#endif // POKEHEARTGOLD_FRIEND_GROUP_ACCESS_H
