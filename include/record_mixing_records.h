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
u32 ov73_021E7A74(SaveData *saveData);
u32 ov73_021E7A7C(SaveData *saveData);
u32 ov73_021E7A84(SaveData *saveData);
BOOL ov73_021E7A8C(int idx, int val);
void ov73_021E7AC0(SaveData *saveData, RecordMixingEntry *out, RecordMixingEntryData *in, int count, int unused4, int unused5);
void *ov73_021E7B3C(SaveData *saveData, enum HeapID heapID, u32 size);
void *ov73_021E7B84(SaveData *saveData, enum HeapID heapID, u32 size);
void *ov73_021E7BC8(SaveData *saveData, enum HeapID heapID, u32 size);
void *ov73_021E7C0C(SaveData *saveData, enum HeapID heapID, u32 size);
void *ov73_021E7C50(SaveData *saveData, enum HeapID heapID, u32 size);
void *ov73_021E7C94(SaveData *saveData, enum HeapID heapID, u32 size);
int ov73_021E7CD8(RecordMixingEntry *entries, int count, RecordMixingEntry *entry, int idx);
void *ov73_021E7D54(u32 netId, void *a, void *b);
void ov73_021E7D74(RecordMixingEntry *entries, int count, int numPlayers, int localId, RecordMixingEntry **received, void *received2, int idx);
void ov73_021E7E24(void *saveBlock23, int numPlayers, int localId, RecordMixingEntry **received, void *received2);

#endif // POKEHEARTGOLD_RECORD_MIXING_RECORDS_H
