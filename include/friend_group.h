#ifndef POKEHEARTGOLD_FRIEND_GROUP_H
#define POKEHEARTGOLD_FRIEND_GROUP_H

#include "pm_string.h"
#include "save.h"

// Hey, you wanna join a GROUP?

#define FGRP_MAX (6)

typedef struct FriendGroup {
    u16 groupName[8];
    u16 leaderName[8];
    u8 leaderGender;
    u8 language;
    u32 id;
    u32 seed; // advanced once per day, used as the daily random value
} FRIEND_GROUP;

// groups[0]: the group the player made, groups[1]: the group the player is in,
// groups[2..5]: groups received through record mixing
typedef struct SaveFriendGroup {
    FRIEND_GROUP groups[FGRP_MAX];
} SAV_FRIEND_GRP;

u32 Save_FriendGroup_sizeof(void);
void Save_FriendGroup_Init(SAV_FRIEND_GRP *savFriendGrp);
SAV_FRIEND_GRP *Save_FriendGroup_Get(SaveData *saveData);
u8 Save_FriendGroup_GetLeaderGender(SAV_FRIEND_GRP *savFriendGrp, int grpno);
u8 Save_FriendGroup_GetLanguage(SAV_FRIEND_GRP *savFriendGrp, int grpno);
u16 *Save_FriendGroup_GetName(SAV_FRIEND_GRP *savFriendGrp, int grpno, int nameType);
u32 Save_FriendGroup_GetCurrentSeed(SAV_FRIEND_GRP *savFriendGrp);

#endif // POKEHEARTGOLD_FRIEND_GROUP_H
