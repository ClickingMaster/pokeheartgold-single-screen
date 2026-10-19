#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_TRAINER_RECORD_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_TRAINER_RECORD_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"

// A facility trainer as FrontierTrainer_Load fills it in from the trainer data and its name.
typedef struct FrontierTrainerRecord {
    u32 id;
    u16 trainerClass;
    u16 unk_06;
    u16 name[8];
    u16 introMsgBank;
    u16 introMsgNo;
    u8 introMsgFields[4];
    u16 messages[8]; // win and lose easy chat messages
} FrontierTrainerRecord;

void *FrontierTrainer_Load(FrontierTrainerRecord *, int, enum HeapID, NarcId);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_TRAINER_RECORD_H
