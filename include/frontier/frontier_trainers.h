#ifndef POKEHEARTGOLD_FRONTIER_TRAINERS_H
#define POKEHEARTGOLD_FRONTIER_TRAINERS_H

#include "global.h"

typedef struct FrontierTrainerData {
    u8 unk0[0x18];
    u16 unk18[4];
    u8 unk1C[0xF0];
} FrontierTrainerData;

void FrontierTrainer_LoadRecord(FrontierTrainerData *record, u16 trainerId, u32 heapID, u32 narcId);
void FrontierMon_GenerateList(void *mons, u16 *setIds, u8 *ivs, u32 *pids, void *otIds, u32 count, u32 heapID, u32 narcId);

#endif
