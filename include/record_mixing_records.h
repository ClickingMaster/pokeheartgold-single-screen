#ifndef POKEHEARTGOLD_RECORD_MIXING_RECORDS_H
#define POKEHEARTGOLD_RECORD_MIXING_RECORDS_H

#include "friend_group.h"
#include "save.h"

// Records of the save block 23 lists as they are mixed: the sender's trainer data + one list entry.
typedef struct RecordMixingEntryData {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02[2];
    u32 unk_04[11];
} RecordMixingEntryData;

typedef struct RecordMixingEntry {
    u32 trainerId;
    u8 gender;
    u8 version;
    u8 language;
    u8 unk_07;
    u16 name[8];
    RecordMixingEntryData data;
} RecordMixingEntry;

u32 RecordMixing_GetGroupRecordSize(SAV_FRIEND_GRP *friendGroup);
FRIEND_GROUP *RecordMixing_GetGroupRecord(SAV_FRIEND_GRP *friendGroup);
void RecordMixing_InsertGroup(FRIEND_GROUP *groups, const FRIEND_GROUP *group);
void RecordMixing_RemoveGroup(FRIEND_GROUP *groups, int index);
void RecordMixing_MergeGroups(int count, int localId, FRIEND_GROUP *groups, FRIEND_GROUP **received);
u32 RecordMixing_Block23List2_GetSize(SaveData *saveData);
u32 RecordMixing_Block23List0_GetSize(SaveData *saveData);
u32 RecordMixing_Block23List1_GetSize(SaveData *saveData);
BOOL RecordMixing_IsAllowedEntryKind(int idx, int val);
void RecordMixing_FillBlock23Entries(SaveData *saveData, RecordMixingEntry *out, RecordMixingEntryData *in, int count, int unused4, int unused5);
void *RecordMixing_Block23List2_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void *RecordMixing_Block23List0_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void *RecordMixing_Block23List1_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void *RecordMixing_Block23List2_Load2(SaveData *saveData, enum HeapID heapID, u32 size);
void *RecordMixing_Block23List0_Load2(SaveData *saveData, enum HeapID heapID, u32 size);
void *RecordMixing_Block23List1_Load2(SaveData *saveData, enum HeapID heapID, u32 size);
int RecordMixing_InsertBlock23Entry(RecordMixingEntry *entries, int count, RecordMixingEntry *entry, int idx);
void *RecordMixing_SelectReceivedBuffer(u32 netId, void *a, void *b);
void RecordMixing_MergeBlock23List(RecordMixingEntry *entries, int count, int numPlayers, int localId, RecordMixingEntry **received, void *received2, int idx);
void RecordMixing_MergeBlock23(void *saveBlock23, int numPlayers, int localId, RecordMixingEntry **received, void *received2);

#endif // POKEHEARTGOLD_RECORD_MIXING_RECORDS_H
