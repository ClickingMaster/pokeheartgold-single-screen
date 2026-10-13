#include "global.h"

#include "assert.h"
#include "record_mixing_exchange.h"
#include "save.h"
#include "save_arrays.h"
#include "communication_player_state.h"
#include "communication_core.h"

// Merges the records received from the other members into the save, record type by record type (a separate TU from
// record_mixing_exchange.c: the overlay's data lies between them).

#define RECORD_MIXING_MAX_MEMBERS 5

// Defined in save_block_23.c; not declared in a header.
void sub_0202E43C(UnkStruct_0202E474 *data);
void sub_0202E474(UnkStruct_0202E474 *data);

void RecordMixing_MergeReceivedRecords(SaveData *saveData, u8 (*buffers)[0xBC0]);

#pragma force_active on

void RecordMixing_MergeReceivedRecords(SaveData *saveData, u8 (*buffers)[0xBC0]) {
    RecordMixingMergeArgs args;
    void *records[RECORD_MIXING_MAX_MEMBERS];
    void *records2[RECORD_MIXING_MAX_MEMBERS];
    u32 offsets[3];
    u32 size;
    UnkStruct_0202E474 *block23;
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
    block23 = sub_020270C4(saveData);
    sub_0202E43C(block23);
    sub_0202E474(block23);
}

#pragma force_active reset
