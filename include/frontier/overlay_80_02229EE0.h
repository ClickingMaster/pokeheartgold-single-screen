#ifndef POKEHEARTGOLD_OVERLAY_80_02229EE0
#define POKEHEARTGOLD_OVERLAY_80_02229EE0

#include "global.h"

typedef struct FrontierTrainerData {
    u8 unk0[0x18];
    u16 unk18[4];
    u8 unk1C[0xF0];
} FrontierTrainerData;

void FrontierTrainer_LoadRecord(FrontierTrainerData *a0, u16 a1, u32 a2, u32 a3);
void FrontierMon_GenerateList(void *a0, u16 *a1, u8 *a2, u32 *a3, void *a4, u32 a5, u32 a6, u32 a7);

#endif
