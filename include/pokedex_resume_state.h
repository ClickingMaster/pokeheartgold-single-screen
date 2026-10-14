#ifndef POKEHEARTGOLD_POKEDEX_RESUME_STATE_H
#define POKEHEARTGOLD_POKEDEX_RESUME_STATE_H

#include "heap.h"

typedef struct PokedexResumeState {
    u16 species;
    u16 dexMode;
} PokedexResumeState;

PokedexResumeState *PokedexResumeState_New(enum HeapID heapID);
void PokedexResumeState_Free(PokedexResumeState *data);
void PokedexResumeState_Set(PokedexResumeState *data, u16 species, u16 dexMode);
u16 PokedexResumeState_GetDexMode(PokedexResumeState *data);
u32 PokedexResumeState_GetSpecies(PokedexResumeState *data);

#endif // POKEHEARTGOLD_POKEDEX_RESUME_STATE_H
