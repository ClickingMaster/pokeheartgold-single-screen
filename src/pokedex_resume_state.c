#include "pokedex_resume_state.h"

#include "global.h"

PokedexResumeState *PokedexResumeState_New(enum HeapID heapID) {
    PokedexResumeState *ret = Heap_Alloc(heapID, sizeof(PokedexResumeState));
    PokedexResumeState_Set(ret, 0, 2);
    return ret;
}

void PokedexResumeState_Free(PokedexResumeState *data) {
    Heap_Free(data);
}

void PokedexResumeState_Set(PokedexResumeState *data, u16 species, u16 dexMode) {
    data->species = species;
    data->dexMode = dexMode;
}

u16 PokedexResumeState_GetDexMode(PokedexResumeState *data) {
    return data->dexMode;
}

u32 PokedexResumeState_GetSpecies(PokedexResumeState *data) {
    return data->species;
}
