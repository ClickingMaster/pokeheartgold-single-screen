#ifndef POKEHEARTGOLD_SAVE_BLOCK_23_H
#define POKEHEARTGOLD_SAVE_BLOCK_23_H

#include "global.h"

typedef struct SaveBlock23 {
    u8 filler_000[12];
    u8 unk_00C[192];
    u8 unk_0CC[192];
    u8 unk_18C[192];
    u8 filler_24C[0x97C];
} SaveBlock23;

#endif // POKEHEARTGOLD_SAVE_BLOCK_23_H
