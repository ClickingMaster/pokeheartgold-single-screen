#include "record_mixing_records.h"

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "player_data.h"
#include "save_arrays.h"
#include "string_util.h"
#include "save_block_23.h"
#include "communication_player_state.h"

#define RECORD_MIXING_MAX_GROUPS_KEPT 4

// friend_group.c
BOOL FriendGroup_IsEmpty(FRIEND_GROUP *group);
BOOL FriendGroup_Equals(FRIEND_GROUP *a, FRIEND_GROUP *b);

extern const u8 *sRecordMixingAllowedEntryKinds[];

u32 RecordMixing_GetGroupRecordSize(SAV_FRIEND_GRP *friendGroup) {
    return sizeof(FRIEND_GROUP);
}

FRIEND_GROUP *RecordMixing_GetGroupRecord(SAV_FRIEND_GRP *friendGroup) {
    return &friendGroup->groups[1];
}

// groups[2..5] hold the groups received by mixing, newest first
void RecordMixing_InsertGroup(FRIEND_GROUP *groups, const FRIEND_GROUP *group) {
    int i;
    for (i = 5; i > 2; --i) {
        groups[i] = groups[i - 1];
    }
    groups[2] = *group;
}

void RecordMixing_RemoveGroup(FRIEND_GROUP *groups, int index) {
    int i;
    if (index < 2 || index > 5) {
        GF_AssertFail();
    }
    for (i = index + 1; i <= 5; ++i) {
        groups[i - 1] = groups[i];
    }
    u8 *last = (u8 *)&groups[5];
    *(u16 *)last = EOS;
    *(u16 *)(last + 0x10) = EOS;
}

void RecordMixing_MergeGroups(int count, int localId, FRIEND_GROUP *groups, FRIEND_GROUP **received) {
    int i;
    for (i = 0; i < count; i++) {
        FRIEND_GROUP *group = received[i];
        if (i == localId || group == NULL || FriendGroup_IsEmpty(group) == TRUE || FriendGroup_Equals(group, groups) != FALSE) {
            continue;
        }
        int j;
        for (j = 0; j < RECORD_MIXING_MAX_GROUPS_KEPT; ++j) {
            if (FriendGroup_Equals(group, &groups[j + 2]) != FALSE) {
                RecordMixing_RemoveGroup(groups, j + 2);
            }
        }
        RecordMixing_InsertGroup(groups, group);
    }
}

u32 RecordMixing_Block23List2_GetSize(SaveData *saveData) {
    return 4 * sizeof(RecordMixingEntry);
}

u32 RecordMixing_Block23List0_GetSize(SaveData *saveData) {
    return 4 * sizeof(RecordMixingEntry);
}

u32 RecordMixing_Block23List1_GetSize(SaveData *saveData) {
    return 4 * sizeof(RecordMixingEntry);
}

#pragma force_active on

BOOL RecordMixing_IsAllowedEntryKind(int idx, int val) {
    int i;
    for (i = 0; sRecordMixingAllowedEntryKinds[idx][i] != 0xFF; i++) {
        if (val == sRecordMixingAllowedEntryKinds[idx][i]) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active reset

#pragma force_active on

void RecordMixing_FillBlock23Entries(SaveData *saveData, RecordMixingEntry *out, RecordMixingEntryData *in, int count, int unused4, int unused5) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    int i;
    for (i = 0; i < count; i++, in++, out++) {
        MI_CpuClearFast(out, sizeof(RecordMixingEntry));
        out->trainerId = PlayerProfile_GetTrainerID(profile);
        out->gender = PlayerProfile_GetTrainerGender(profile);
        out->version = GAME_VERSION;
        out->language = GAME_LANGUAGE;
        CopyU16StringArrayN(out->name, PlayerProfile_GetNamePtr(profile), 8);
        out->data = *in;
        out->data.unk_01 = 0;
        out->data.unk_00 = 0;
    }
}

#pragma force_active reset

void *RecordMixing_Block23List2_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List2_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_18C, 4, 1, 3);
    return out;
}

void *RecordMixing_Block23List0_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List0_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_00C, 4, 1, 4);
    return out;
}

void *RecordMixing_Block23List1_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List1_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_0CC, 4, 1, 5);
    return out;
}

void *RecordMixing_Block23List2_Load2(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List2_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_18C, 4, 0, 0);
    return out;
}

void *RecordMixing_Block23List0_Load2(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List0_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_00C, 4, 0, 1);
    return out;
}

void *RecordMixing_Block23List1_Load2(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *out = Heap_AllocAtEnd(heapID, size);
    SaveBlock23 *data = Save_Block23_Get(saveData);
    if (size != RecordMixing_Block23List1_GetSize(saveData)) {
        GF_AssertFail();
    }
    RecordMixing_FillBlock23Entries(saveData, out, (RecordMixingEntryData *)data->unk_0CC, 4, 0, 2);
    return out;
}

#pragma force_active on

int RecordMixing_InsertBlock23Entry(RecordMixingEntry *entries, int count, RecordMixingEntry *entry, int idx) {
    int i;
    if (entry->data.unk_00 == 0) {
        return 0;
    }
    if (!RecordMixing_IsAllowedEntryKind(idx, entry->data.unk_00)) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        if (entries[i].data.unk_00 == 0) {
            entries[i] = *entry;
            return 1;
        }
    }
    for (i = 0; i < count; i++) {
        if (entries[i].data.unk_01 >= 3) {
            entries[i] = *entry;
            return 1;
        }
    }
    return 0;
}

#pragma force_active reset

// a for partners whose profile has a version set, b otherwise
void *RecordMixing_SelectReceivedBuffer(u32 netId, void *a, void *b) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(netId);
    if (!profile) {
        return NULL;
    }
    if (PlayerProfile_GetVersion(profile)) {
        b = a;
    }
    return b;
}

// every member takes 4 entries, from the other members in turn
#pragma force_active on

void RecordMixing_MergeBlock23List(RecordMixingEntry *entries, int count, int numPlayers, int localId, RecordMixingEntry **received, void *received2, int idx) {
    u8 given[16];
    int j;
    int i;
    int k;
    RecordMixingEntry **list;
    MI_CpuFill8(given, 0, sizeof(given));
    j = 0;
    for (i = 0; i < numPlayers; i++) {
        if (received[i] == NULL) {
            continue;
        }
        for (k = 0; k < 4; k++) {
            while (received[j] == NULL || j == i || given[j] == 4) {
                j = (j + 1) % numPlayers;
            }
            given[j]++;
            if (j != localId) {
                list = RecordMixing_SelectReceivedBuffer(j, received, received2);
                if (list != NULL) {
                    RecordMixing_InsertBlock23Entry(entries, count, &list[j][k], idx);
                }
            }
        }
    }
    SaveSubstruct_UpdateCRC(23);
}

#pragma force_active reset

void RecordMixing_MergeBlock23(void *saveBlock23, int numPlayers, int localId, RecordMixingEntry **received, void *received2) {
    RecordMixing_MergeBlock23List((RecordMixingEntry *)((u8 *)saveBlock23 + 0x90C), 8, numPlayers, localId, received, received2, 0);
}
