#ifndef POKEHEARTGOLD_RECORD_MIXING_EXCHANGE_H
#define POKEHEARTGOLD_RECORD_MIXING_EXCHANGE_H

#include "heap.h"
#include "save.h"

// The record types record mixing exchanges (record_mixing_exchange.c builds what is sent, record_mixing_merge.c
// merges what was received).

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

extern const RecordMixingHandler sRecordMixingHandlers[RECORD_MIXING_NUM_HANDLERS];

#endif // POKEHEARTGOLD_RECORD_MIXING_EXCHANGE_H
