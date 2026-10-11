#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_INDEX_HELPERS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_INDEX_HELPERS_PRIVATE_H

#include "global.h"

int PokedexApp_GetListCursorIndex(void *);
u16 PokedexApp_GetCursorSpecies(void *);
u32 PokedexApp_FindSpeciesInResults(void *, u16);
void PokedexApp_BuildListEntries(void *, int);
u32 PokedexApp_GetListLength(void *, int);
u32 PokedexApp_GetLastListPage(void *, int);
int Pokedex_ConvertToCurrentDexNo(u8, u16);

#endif
