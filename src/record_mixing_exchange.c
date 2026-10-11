#include "global.h"

#include "assert.h"
#include "fashion_case.h"
#include "friend_group.h"
#include "heap.h"
#include "save.h"
#include "save_arrays.h"
#include "save_rankings.h"
#include "unk_0202E41C.h"
#include "unk_02069528.h"

#define RECORD_MIXING_SEND_SIZE    3000
#define RECORD_MIXING_NUM_HANDLERS 10

// What the merge function of a record type gets: the received records of every member.
typedef struct RecordMixingMergeArgs {
    enum HeapID heapID;
    SaveData *saveData;
    int numPlayers;
    int localId;
    void **records;
    void **records2;
} RecordMixingMergeArgs;

// One record type: size and copy of the local data to send, merge of the received data.
typedef struct RecordMixingHandler {
    u32 (*getSize)(SaveData *saveData);
    void *(*load)(SaveData *saveData, enum HeapID heapID, u32 size);
    void (*merge)(RecordMixingMergeArgs *args);
} RecordMixingHandler;

// record_mixing_records.c
u32 RecordMixing_GetGroupRecordSize(SAV_FRIEND_GRP *friendGroup);
FRIEND_GROUP *RecordMixing_GetGroupRecord(SAV_FRIEND_GRP *friendGroup);
void RecordMixing_MergeGroups(int count, int localId, FRIEND_GROUP *groups, FRIEND_GROUP **received);
void ov73_021E7E24(void *saveBlock23, int numPlayers, int localId, void **received, void **received2);

// No public header declares these.
u32 FashionEntry_sizeof(void);
void Save_FashionData_InsertEntries(u8 count, int skip, SaveFashionData *fashionData, SaveFashionDataSub **entries);

u32 RecordMixing_FriendGroup_GetSize(SaveData *saveData);
void *RecordMixing_FriendGroup_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void RecordMixing_FriendGroup_Merge(RecordMixingMergeArgs *args);
u32 RecordMixing_Fashion_GetSize(SaveData *saveData);
void *RecordMixing_Fashion_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void RecordMixing_Fashion_Merge(RecordMixingMergeArgs *args);
void ov73_021EA2FC(RecordMixingMergeArgs *args);
SaveRankingsEntry *RecordMixing_Rankings_Load(SaveData *saveData, enum HeapID heapID);
void RecordMixing_Rankings_Merge(RecordMixingMergeArgs *args);
void *ov73_021EA348(SaveData *saveData, enum HeapID heapID, u32 size);
void ov73_021EA370(RecordMixingMergeArgs *args);
void RecordMixing_BuildSendBuffer(SaveData *saveData, void *dest);

extern const RecordMixingHandler sRecordMixingHandlers[RECORD_MIXING_NUM_HANDLERS];

u32 RecordMixing_FriendGroup_GetSize(SaveData *saveData) {
    return RecordMixing_GetGroupRecordSize(Save_FriendGroup_Get(saveData));
}

void *RecordMixing_FriendGroup_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    SAV_FRIEND_GRP *friendGroup = Save_FriendGroup_Get(saveData);
    void *copy = Heap_AllocAtEnd(heapID, size);
    void *src = RecordMixing_GetGroupRecord(friendGroup);
    MIi_CpuCopyFast(src, copy, size);
    return copy;
}

void RecordMixing_FriendGroup_Merge(RecordMixingMergeArgs *args) {
    RecordMixing_MergeGroups(args->numPlayers, args->localId, Save_FriendGroup_Get(args->saveData)->groups, (FRIEND_GROUP **)args->records);
}

u32 RecordMixing_Fashion_GetSize(SaveData *saveData) {
    // called with the fashion data, but the definition takes no argument
    return ((u32(*)(SaveFashionData *))FashionEntry_sizeof)(Save_FashionData_Get(saveData));
}

void *RecordMixing_Fashion_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    SaveFashionData *fashionData = Save_FashionData_Get(saveData);
    void *copy = Heap_AllocAtEnd(heapID, size);
    void *src = Save_FashionData_GetEntry(fashionData, 0);
    MIi_CpuCopyFast(src, copy, size);
    return copy;
}

void RecordMixing_Fashion_Merge(RecordMixingMergeArgs *args) {
    SaveData *saveData = args->saveData;
    SaveFashionData *fashionData = Save_FashionData_Get(saveData);
    Save_FashionData_InsertEntries((u8)args->numPlayers, args->localId, fashionData, (SaveFashionDataSub **)args->records);
}

void ov73_021EA2FC(RecordMixingMergeArgs *args) {
    UnkStruct_0202E474 *data = sub_020270C4(args->saveData);
    ov73_021E7E24(data, args->numPlayers, args->localId, args->records, args->records2);
}

SaveRankingsEntry *RecordMixing_Rankings_Load(SaveData *saveData, enum HeapID heapID) {
    return Save_GetPlayerMixingRankingEntry(saveData, heapID);
}

void RecordMixing_Rankings_Merge(RecordMixingMergeArgs *args) {
    Save_Rankings_Get(args->saveData);
    // the local id is passed without truncation to u8 (the original prototype took a wider type)
    ((void (*)(SaveData *, int, u8, SaveRankingsEntry **, enum HeapID))Save_UpdateRankingsFromMixing)(args->saveData, args->localId, (u8)args->numPlayers, (SaveRankingsEntry **)args->records, args->heapID);
}

void *ov73_021EA348(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *dest = Heap_AllocAtEnd(heapID, size);
    MI_CpuFill8(dest, 0, size);
    sub_02069528(saveData, 0, dest);
    return dest;
}

void ov73_021EA370(RecordMixingMergeArgs *args) {
}

// Copies the data of every record type into the send buffer.
void RecordMixing_BuildSendBuffer(SaveData *saveData, void *dest) {
    u32 remaining = RECORD_MIXING_SEND_SIZE;
    int i;

    for (i = 0; i < RECORD_MIXING_NUM_HANDLERS; i++) {
        u32 size = sRecordMixingHandlers[i].getSize(saveData);
        void *buffer = sRecordMixingHandlers[i].load(saveData, HEAP_ID_50, size);
        GF_ASSERT(remaining > size);
        MIi_CpuCopyFast(buffer, dest, size);
        Heap_Free(buffer);
        remaining -= size;
        dest = (u8 *)dest + size;
    }
}
