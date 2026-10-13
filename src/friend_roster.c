#include "friend_roster.h"

#include "global.h"

#include "gf_rtc.h"
#include "pm_string.h"
#include "to90_login_callback_private.h"

typedef struct FriendEntry {
    u16 name[8], name2[8];
    u32 id;
    u16 value24, value26, value28, value2A;
    u8 value2C, value2D, value2E, value2F;
    u16 value30, value32, value34, value36;
} FriendEntry;

typedef struct FriendEntryArray {
    FriendEntry entries[32];
} FriendEntryArray;

struct FriendRoster {
    W41287Account header;
    u32 friendData[32][3];
    FriendEntryArray roster;
};

// asm/friend_roster_data.s
extern FriendRoster gFriendRoster;

void Comm_EnsureDwcUserData(FriendRoster *data);
BOOL DWC_IsValidFriendData(void *data);

u32 Save_FriendRoster_sizeof(void);
void Save_FriendRoster_Init(FriendRoster *data);
void *FriendRoster_GetUserData(FriendRoster *data);
s32 FriendRoster_GetEntryField(FriendRoster *data, s32 index, s32 field);
void FriendRoster_SetEntryField(FriendRoster *data, int index, int field, u8 value);
void *FriendRoster_GetDwcFriendData(FriendRoster *data, int index);
u16 *sub_0202C254(FriendRoster *data, s32 index);
void sub_0202C270(FriendRoster *data, int index, String *name);
u16 *sub_0202C298(FriendRoster *data, s32 index);
void sub_0202C2B4(FriendRoster *data, int index, String *name);
BOOL FriendRoster_IsFriendValid(FriendRoster *data, int index);
int FriendRoster_CountFriends(FriendRoster *data);
int FriendRoster_GetUsedSlotCount(FriendRoster *data);
void FriendRoster_RemoveFriend(FriendRoster *data, int index);
void FriendRoster_MoveFriend(FriendRoster *data, int dest, int src);
void FriendRoster_Compact(FriendRoster *data);
void FriendRoster_UpdateDate(FriendRoster *data, s32 index);
void FriendRoster_AddWinsLossesTrades(FriendRoster *data, s32 index, s32 a, s32 b, s32 c);
void sub_0202C554(FriendRoster *data, s32 index, s32 value);
void sub_0202C584(FriendRoster *data, s32 index, s32 value);
void sub_0202C5B4(FriendRoster *data, s32 index, s32 value);
void FriendRoster_MergeFriend(FriendRoster *data, s32 src, s32 dest);
FriendRoster *Save_FriendRoster_Get(SaveData *saveData);
void Save_FriendRoster_LoadFromSave(SaveData *saveData);
void Save_FriendRoster_StoreToSave(SaveData *saveData);

u32 Save_FriendRoster_sizeof(void) {
    return sizeof(FriendRoster);
}

void Save_FriendRoster_Init(FriendRoster *data) {
    int i;
    MI_CpuClearFast(data, sizeof(*data));
    for (i = 0; i < 32; i++) {
        data->roster.entries[i].name2[0] = 0xFFFF;
        data->roster.entries[i].name[0] = 0xFFFF;
        data->roster.entries[i].value2E = 2;
    }
    Comm_EnsureDwcUserData(data);
    MI_CpuCopy8(data, &gFriendRoster, sizeof(*data));
}

void *FriendRoster_GetUserData(FriendRoster *data) {
    return data;
}

s32 FriendRoster_GetEntryField(FriendRoster *data, s32 index, s32 field) {
    int value;
    GF_ASSERT(index < 32);
    switch (field) {
    case 0:
        value = data->roster.entries[index].id;
        break;
    case 1:
        value = data->roster.entries[index].value24;
        break;
    case 2:
        value = data->roster.entries[index].value26;
        break;
    case 3:
        value = data->roster.entries[index].value28;
        break;
    case 4:
        value = data->roster.entries[index].value2A;
        break;
    case 5:
        value = data->roster.entries[index].value2C;
        break;
    case 6:
        value = data->roster.entries[index].value2D;
        break;
    case 8:
        value = data->roster.entries[index].value2E;
        break;
    case 7:
        value = data->roster.entries[index].value2F;
        break;
    case 9:
        value = data->roster.entries[index].value30;
        break;
    case 10:
        value = data->roster.entries[index].value32;
        break;
    case 11:
        value = data->roster.entries[index].value34;
        break;
    case 12:
        value = data->roster.entries[index].value36;
        break;
    }
    return value;
}

void FriendRoster_SetEntryField(FriendRoster *data, int index, int field, u8 value) {
    GF_ASSERT(index < 32);
    switch (field) {
    case 0:
        data->roster.entries[index].id = value;
        break;
    case 1:
        GF_ASSERT(FALSE);
        break;
    case 2:
        GF_ASSERT(FALSE);
        break;
    case 3:
        GF_ASSERT(FALSE);
        break;
    case 4:
        data->roster.entries[index].value2A = value;
        break;
    case 5:
        data->roster.entries[index].value2C = value;
        break;
    case 6:
        data->roster.entries[index].value2D = value;
        break;
    case 8:
        data->roster.entries[index].value2E = value;
        break;
    case 7:
        data->roster.entries[index].value2F = value;
        break;
    case 9:
        GF_ASSERT(FALSE);
        break;
    }
}

void *FriendRoster_GetDwcFriendData(FriendRoster *data, int index) {
    GF_ASSERT(index < 32);
    return data->friendData[index];
}

u16 *sub_0202C254(FriendRoster *data, s32 index) {
    GF_ASSERT(index < 32);
    return data->roster.entries[index].name2;
}

void sub_0202C270(FriendRoster *data, int index, String *name) {
    GF_ASSERT(index < 32);
    CopyStringToU16Array(name, data->roster.entries[index].name2, 16);
}

u16 *sub_0202C298(FriendRoster *data, s32 index) {
    GF_ASSERT(index < 32);
    return data->roster.entries[index].name;
}

void sub_0202C2B4(FriendRoster *data, int index, String *name) {
    GF_ASSERT(index < 32);
    CopyStringToU16Array(name, data->roster.entries[index].name, 16);
}

BOOL FriendRoster_IsFriendValid(FriendRoster *data, int index) {
    GF_ASSERT(index < 32);
    return DWC_IsValidFriendData(data->friendData[index]);
}

int FriendRoster_CountFriends(FriendRoster *data) {
    int i, count = 0;
    for (i = 0; i < 32; i++) {
        if (FriendRoster_IsFriendValid(data, i)) {
            count++;
        }
    }
    return count;
}

int FriendRoster_GetUsedSlotCount(FriendRoster *data) {
    int end = 0, i;
    for (i = 0; i < 32; i++) {
        if (FriendRoster_IsFriendValid(data, i)) {
            end = i + 1;
        }
    }
    return end;
}

void FriendRoster_RemoveFriend(FriendRoster *data, int index) {
    int i;
    if (index >= 0 && index < 32) {
        for (i = index; i < 31; i++) {
            MI_CpuCopy8(&data->roster.entries[i + 1], &data->roster.entries[i], sizeof(FriendEntry));
            MI_CpuCopy8(data->friendData[i + 1], data->friendData[i], sizeof(data->friendData[i]));
        }
        MI_CpuClearFast((u8 *)&data->roster + 0x6C8, sizeof(FriendEntry));
        MI_CpuClearFast((u8 *)&data->friendData + 0x174, sizeof(data->friendData[31]));
        ((FriendEntry *)((u8 *)&data->roster + 0x6C8))->name2[0] = 0xFFFF;
        ((FriendEntry *)((u8 *)&data->roster + 0x6C8))->name[0] = 0xFFFF;
        ((FriendEntry *)((u8 *)&data->roster + 0x6C8))->value2E = 2;
    }
}

void FriendRoster_MoveFriend(FriendRoster *data, int dest, int src) {
    if (dest >= 0 && dest < 32 && src >= 0 && src < 32) {
        MI_CpuCopy8(&data->roster.entries[src], &data->roster.entries[dest], sizeof(FriendEntry));
        MI_CpuCopy8(data->friendData[src], data->friendData[dest], sizeof(data->friendData[src]));
        MI_CpuClearFast(&data->roster.entries[src], sizeof(FriendEntry));
        MI_CpuClearFast(data->friendData[src], sizeof(data->friendData[src]));
        data->roster.entries[src].name2[0] = 0xFFFF;
        data->roster.entries[src].name[0] = 0xFFFF;
        data->roster.entries[src].value2E = 2;
    }
}

void FriendRoster_Compact(FriendRoster *data) {
    int i, empty = -1;
    for (i = 0; i < 32; i++) {
        if (FriendRoster_IsFriendValid(data, i)) {
            if (empty != -1) {
                FriendRoster_MoveFriend(data, empty, i);
                i = -1;
                empty = -1;
            }
        } else if (empty == -1) {
            empty = i;
        }
    }
}

void FriendRoster_UpdateDate(FriendRoster *data, s32 index) {
    RTCDate date;
    GF_RTC_CopyDate(&date);
    if (index >= 0 && index < 32) {
        data->roster.entries[index].value2A = date.year + 2000;
        data->roster.entries[index].value2C = date.month;
        data->roster.entries[index].value2D = date.day;
    }
}

void FriendRoster_AddWinsLossesTrades(FriendRoster *data, s32 index, s32 a, s32 b, s32 c) {
    if (index >= 0 && index < 32) {
        data->roster.entries[index].value24 += a;
        if (data->roster.entries[index].value24 > 9999) {
            data->roster.entries[index].value24 = 9999;
        }
        data->roster.entries[index].value26 += b;
        if (data->roster.entries[index].value26 > 9999) {
            data->roster.entries[index].value26 = 9999;
        }
        data->roster.entries[index].value28 += c;
        if (data->roster.entries[index].value28 > 9999) {
            data->roster.entries[index].value28 = 9999;
        }
        FriendRoster_UpdateDate(data, index);
    }
}

void sub_0202C554(FriendRoster *data, s32 index, s32 value) {
    if (index >= 0 && index < 32) {
        data->roster.entries[index].value32 += value;
        if (data->roster.entries[index].value32 > 9999) {
            data->roster.entries[index].value32 = 9999;
        }
        FriendRoster_UpdateDate(data, index);
    }
}

void sub_0202C584(FriendRoster *data, s32 index, s32 value) {
    if (index >= 0 && index < 32) {
        data->roster.entries[index].value34 += value;
        if (data->roster.entries[index].value34 > 9999) {
            data->roster.entries[index].value34 = 9999;
        }
        FriendRoster_UpdateDate(data, index);
    }
}

void sub_0202C5B4(FriendRoster *data, s32 index, s32 value) {
    if (index >= 0 && index < 32) {
        data->roster.entries[index].value36 += value;
        if (data->roster.entries[index].value36 > 9999) {
            data->roster.entries[index].value36 = 9999;
        }
        FriendRoster_UpdateDate(data, index);
    }
}

void FriendRoster_MergeFriend(FriendRoster *data, s32 src, s32 dest) {
    if (dest >= 0 && dest < 32 && src >= 0 && src < 32) {
        data->roster.entries[dest].value24 += data->roster.entries[src].value24;
        if (data->roster.entries[dest].value24 > 9999) {
            data->roster.entries[dest].value24 = 9999;
        }
        data->roster.entries[dest].value26 += data->roster.entries[src].value26;
        if (data->roster.entries[dest].value26 > 9999) {
            data->roster.entries[dest].value26 = 9999;
        }
        data->roster.entries[dest].value28 += data->roster.entries[src].value28;
        if (data->roster.entries[dest].value28 > 9999) {
            data->roster.entries[dest].value28 = 9999;
        }
        data->roster.entries[dest].value30 += data->roster.entries[src].value30;
        if (data->roster.entries[dest].value30 > 9999) {
            data->roster.entries[dest].value30 = 9999;
        }
        data->roster.entries[dest].value32 += data->roster.entries[src].value32;
        if (data->roster.entries[dest].value32 > 9999) {
            data->roster.entries[dest].value32 = 9999;
        }
        data->roster.entries[dest].value34 += data->roster.entries[src].value34;
        if (data->roster.entries[dest].value34 > 9999) {
            data->roster.entries[dest].value34 = 9999;
        }
        data->roster.entries[dest].value36 += data->roster.entries[src].value36;
        if (data->roster.entries[dest].value36 > 9999) {
            data->roster.entries[dest].value36 = 9999;
        }
        MI_CpuCopyFast(data->roster.entries[src].name, data->roster.entries[dest].name, sizeof(data->roster.entries[src].name));
        MI_CpuClearFast(&data->roster.entries[src], sizeof(FriendEntry));
        data->roster.entries[src].name2[0] = 0xFFFF;
        data->roster.entries[src].name[0] = 0xFFFF;
        data->roster.entries[src].value2E = 2;
    }
}

FriendRoster *Save_FriendRoster_Get(SaveData *saveData) {
    return &gFriendRoster;
}

void Save_FriendRoster_LoadFromSave(SaveData *saveData) {
    FriendRoster *data = SaveArray_Get(saveData, 25);
    MI_CpuCopy8(data, &gFriendRoster, sizeof(FriendRoster));
}

void Save_FriendRoster_StoreToSave(SaveData *saveData) {
    FriendRoster *data = SaveArray_Get(saveData, 25);
    MI_CpuCopy8(&gFriendRoster, data, sizeof(FriendRoster));
}
