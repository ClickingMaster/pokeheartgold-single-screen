#include "record_mixing/record_mixing_exchange.h"

#include "global.h"

#include "assert.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "fashion_case.h"
#include "friend_group.h"
#include "frontier_wifi_upload_record.h"
#include "heap.h"
#include "save.h"
#include "save_arrays.h"
#include "save_block_23.h"
#include "save_rankings.h"

// The records exchanged by record mixing: per record type, its size, how it is loaded into the send buffer and how
// the copies received from the other members are merged into the save.

#define RECORD_MIXING_MAX_MEMBERS 5

u32 RecordMixing_GetGroupRecordSize(SAV_FRIEND_GRP *friendGroup);
FRIEND_GROUP *RecordMixing_GetGroupRecord(SAV_FRIEND_GRP *friendGroup);
void RecordMixing_MergeGroups(int count, int localId, FRIEND_GROUP *groups, FRIEND_GROUP **received);
void RecordMixing_MergeBlock23(void *saveBlock23, int numPlayers, int localId, void **received, void **received2);
void SaveBlock23_ClearArraysAndReset(SaveBlock23 *data);
void SaveBlock23_Reset(SaveBlock23 *data);

u32 FashionEntry_sizeof(void);
void Save_FashionData_InsertEntries(u8 count, int skip, SaveFashionData *fashionData, SaveFashionDataSub **entries);

u32 RecordMixing_FriendGroup_GetSize(SaveData *saveData);
void *RecordMixing_FriendGroup_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void RecordMixing_FriendGroup_Merge(RecordMixingMergeArgs *args);
u32 RecordMixing_Fashion_GetSize(SaveData *saveData);
void *RecordMixing_Fashion_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void RecordMixing_Fashion_Merge(RecordMixingMergeArgs *args);
void RecordMixing_Block23_Merge(RecordMixingMergeArgs *args);
SaveRankingsEntry *RecordMixing_Rankings_Load(SaveData *saveData, enum HeapID heapID);
void RecordMixing_Rankings_Merge(RecordMixingMergeArgs *args);
void *RecordMixing_BattleRoomTrainer_Load(SaveData *saveData, enum HeapID heapID, u32 size);
void RecordMixing_BattleRoomTrainer_Merge(RecordMixingMergeArgs *args);

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
    Save_FashionData_Get(saveData);
    return FashionEntry_sizeof();
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

void RecordMixing_Block23_Merge(RecordMixingMergeArgs *args) {
    SaveBlock23 *data = Save_Block23_Get(args->saveData);
    RecordMixing_MergeBlock23(data, args->numPlayers, args->localId, args->records, args->records2);
}

SaveRankingsEntry *RecordMixing_Rankings_Load(SaveData *saveData, enum HeapID heapID) {
    return Save_GetPlayerMixingRankingEntry(saveData, heapID);
}

void RecordMixing_Rankings_Merge(RecordMixingMergeArgs *args) {
    Save_Rankings_Get(args->saveData);
    // the local id is passed without truncation to u8
    ((void (*)(SaveData *, int, u8, SaveRankingsEntry **, enum HeapID))Save_UpdateRankingsFromMixing)(args->saveData, args->localId, (u8)args->numPlayers, (SaveRankingsEntry **)args->records, args->heapID);
}

void *RecordMixing_BattleRoomTrainer_Load(SaveData *saveData, enum HeapID heapID, u32 size) {
    void *dest = Heap_AllocAtEnd(heapID, size);
    MI_CpuFill8(dest, 0, size);
    Frontier_FillWifiUploadRecord(saveData, 0, dest);
    return dest;
}

void RecordMixing_BattleRoomTrainer_Merge(RecordMixingMergeArgs *args) {
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

// Merges the records received from the other members into the save, record type by record type.
void RecordMixing_MergeReceivedRecords(SaveData *saveData, u8 (*buffers)[RECORD_MIXING_BUFFER_SIZE]) {
    RecordMixingMergeArgs args;
    void *records[RECORD_MIXING_MAX_MEMBERS];
    void *records2[RECORD_MIXING_MAX_MEMBERS];
    u32 offsets[3];
    u32 size;
    SaveBlock23 *block23;
    u32 remaining;
    int i;
    u32 offset;
    int j;
    u32 *offsetOfEarlier;

    remaining = RECORD_MIXING_SEND_SIZE;
    args.heapID = HEAP_ID_50;
    offset = 0;
    args.saveData = saveData;
    args.localId = Comm_GetLocalNetId();
    args.numPlayers = RECORD_MIXING_MAX_MEMBERS;
    args.records = records;
    args.records2 = records2;
    // offsets of the records of types 2, 3 and 4 in the received buffers
    offsets[0] = 0;
    offsets[1] = 0;
    offsets[2] = 0;
    for (i = 0; i < 2; i++) {
        offsets[0] += sRecordMixingHandlers[i].getSize(saveData);
    }
    for (i = 0; i < 3; i++) {
        offsets[1] += sRecordMixingHandlers[i].getSize(saveData);
    }
    for (i = 0; i < 4; i++) {
        offsets[2] += sRecordMixingHandlers[i].getSize(saveData);
    }
    for (i = 0; (u32)i < RECORD_MIXING_NUM_HANDLERS; i++) {
        size = sRecordMixingHandlers[i].getSize(saveData);
        GF_ASSERT(remaining > size);
        for (j = 0; j < RECORD_MIXING_MAX_MEMBERS; j++) {
            if (CommPlayerInfo_GetProfile(j)) {
                args.records[j] = buffers[j] + offset;
            } else {
                args.records[j] = NULL;
            }
        }
        offsetOfEarlier = &offsets[i]; // only read for types 7-9: offsets[i - 7]
        switch (i) {
        case 7:
        case 8:
        case 9:
            // types 7, 8 and 9 also get the received records of types 2, 3 and 4
            for (j = 0; j < RECORD_MIXING_MAX_MEMBERS; j++) {
                if (CommPlayerInfo_GetProfile(j)) {
                    args.records2[j] = buffers[j] + offsetOfEarlier[-7];
                } else {
                    args.records2[j] = NULL;
                }
            }
            break;
        }
        if (sRecordMixingHandlers[i].merge != NULL) {
            sRecordMixingHandlers[i].merge(&args);
        }
        offset += size;
        remaining -= size;
    }
    block23 = Save_Block23_Get(saveData);
    SaveBlock23_ClearArraysAndReset(block23);
    SaveBlock23_Reset(block23);
}
