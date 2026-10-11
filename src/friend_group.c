#include "friend_group.h"

#include "global.h"

#include "math_util.h"
#include "string_util.h"

// The definitions' own types; friend_group_access.h has the (partly different) caller view.
void Save_FriendGroup_CopyGroup(SAV_FRIEND_GRP *data, int src, int dst);
void Save_FriendGroup_AdvanceSeeds(SAV_FRIEND_GRP *data, u32 days);
u32 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *data, int index);
void Save_FriendGroup_SetGroupId(SAV_FRIEND_GRP *data, int index, u32 id);
void Save_FriendGroup_SetName(SAV_FRIEND_GRP *data, int index, int type, String *str);
void Save_FriendGroup_SetLeaderGender(SAV_FRIEND_GRP *data, int index, u8 gender);
void Save_FriendGroup_SetLanguage(SAV_FRIEND_GRP *data, int index, u8 language);
BOOL Save_FriendGroup_IsGroupValid(SAV_FRIEND_GRP *data, int index);
BOOL Save_FriendGroup_IsCurrentGroup(SAV_FRIEND_GRP *data, int index);
BOOL Save_FriendGroup_IsGroupNameTaken(SAV_FRIEND_GRP *data, u16 *name);
BOOL FriendGroup_IsEmpty(FRIEND_GROUP *group);
BOOL FriendGroup_Equals(FRIEND_GROUP *a, FRIEND_GROUP *b);

extern SAV_FRIEND_GRP *gFriendGroupSave; // asm/friend_group_data.s

u32 Save_FriendGroup_sizeof(void) {
    return sizeof(SAV_FRIEND_GRP);
}

void Save_FriendGroup_CopyGroup(SAV_FRIEND_GRP *data, int src, int dst) {
    data->groups[dst] = data->groups[src];
}

void Save_FriendGroup_Init(SAV_FRIEND_GRP *data) {
    int i;
    MI_CpuClearFast(data, sizeof(*data));
    for (i = 0; i < 6; i++) {
        data->groups[i].groupName[0] = 0xFFFF;
        data->groups[i].leaderName[0] = 0xFFFF;
    }
    gFriendGroupSave = data;
}

void Save_FriendGroup_AdvanceSeeds(SAV_FRIEND_GRP *data, u32 days) {
    u32 i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < days; j++) {
            data->groups[i].seed = PRandom(data->groups[i].seed);
        }
    }
}

u32 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *data, int index) {
    return data->groups[index].id;
}

void Save_FriendGroup_SetGroupId(SAV_FRIEND_GRP *data, int index, u32 id) {
    data->groups[index].id = id;
    data->groups[index].seed = PRandom(id);
}

u32 Save_FriendGroup_GetCurrentSeed(SAV_FRIEND_GRP *data) {
    return data->groups[1].seed;
}

u16 *Save_FriendGroup_GetName(SAV_FRIEND_GRP *data, int index, int type) {
    if (type == 0) {
        return data->groups[index].groupName;
    } else {
        return data->groups[index].leaderName;
    }
}

void Save_FriendGroup_SetName(SAV_FRIEND_GRP *data, int index, int type, String *str) {
    if (type == 0) {
        CopyStringToU16Array(str, data->groups[index].groupName, 8);
    } else {
        CopyStringToU16Array(str, data->groups[index].leaderName, 8);
    }
}

void Save_FriendGroup_SetLeaderGender(SAV_FRIEND_GRP *data, int index, u8 gender) {
    data->groups[index].leaderGender = gender;
}

u8 Save_FriendGroup_GetLeaderGender(SAV_FRIEND_GRP *data, int index) {
    return data->groups[index].leaderGender;
}

u8 Save_FriendGroup_GetLanguage(SAV_FRIEND_GRP *data, int index) {
    return data->groups[index].language;
}

void Save_FriendGroup_SetLanguage(SAV_FRIEND_GRP *data, int index, u8 language) {
    data->groups[index].language = language;
}

SAV_FRIEND_GRP *Save_FriendGroup_Get(SaveData *save) {
    return SaveArray_Get(save, 14);
}

BOOL Save_FriendGroup_IsGroupValid(SAV_FRIEND_GRP *data, int index) {
    return !FriendGroup_IsEmpty(&data->groups[index]);
}

BOOL Save_FriendGroup_IsCurrentGroup(SAV_FRIEND_GRP *data, int index) {
    return FriendGroup_Equals(&data->groups[1], &data->groups[index]);
}

BOOL Save_FriendGroup_IsGroupNameTaken(SAV_FRIEND_GRP *data, u16 *name) {
    int i;
    if (name[0] == 0xFFFF) {
        return FALSE;
    }
    for (i = 0; i < 6; i++) {
        if (!StringNotEqualN(name, data->groups[i].groupName, 8)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FriendGroup_IsEmpty(FRIEND_GROUP *group) {
    if (group->groupName[0] == 0xFFFF) {
        return TRUE;
    }
    if (group->leaderName[0] == 0xFFFF) {
        return TRUE;
    }
    return FALSE;
}

BOOL FriendGroup_Equals(FRIEND_GROUP *a, FRIEND_GROUP *b) {
    if (StringNotEqualN(a->leaderName, b->leaderName, 8)) {
        return FALSE;
    }
    if (StringNotEqualN(a->groupName, b->groupName, 8)) {
        return FALSE;
    }
    if (a->leaderGender != b->leaderGender) {
        return FALSE;
    }
    if (a->language != b->language) {
        return FALSE;
    }
    if (a->id == b->id) {
        return TRUE;
    }
    return FALSE;
}
