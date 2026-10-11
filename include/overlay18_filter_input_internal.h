#ifndef POKEHEARTGOLD_OVERLAY18_FILTER_INPUT_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY18_FILTER_INPUT_INTERNAL_H

#include "global.h"

#include "dex_mon_measures.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokedex.h"
#include "system.h"
#include "touchscreen.h"

extern const TouchscreenHitbox ov18_021FB704[];
extern const TouchscreenHitbox ov18_021FB934[];
extern const TouchscreenHitbox ov18_021FB804[];
extern const TouchscreenHitbox ov18_021FB7E0[];
extern const TouchscreenHitbox ov18_021FB630[];
extern const int ov18_021FB698[];
extern const int ov18_021FB904[];
extern const int ov18_021FB760[];
extern const int ov18_021FB7C0[];

int PokedexApp_HandleEntryTabTouch(void *work, int *touched);
int PokedexApp_HandleAreaInput(void *work, int *touched);
int PokedexApp_HandleSizeCheckInput(void *work, int *touched);
int PokedexApp_HandleFormsInput(void *work, int *touched);
int PokedexApp_HandleFormCompareInput(void *work, int *touched);
int ov18_021F7E70(void *work, int *touched);
void *PokedexApp_LoadSortData(u32 index, u32 *count);
void PokedexApp_FilterSeen(u16 *dst, u32 *count, const Pokedex *pokedex, const u16 *src, u32 length);
void PokedexApp_StoreSearchResults(void *dst, const Pokedex *pokedex, const u16 *src, u32 count);
void PokedexApp_ApplySortOrder(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex);
void PokedexApp_FilterByLetter(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex);
void PokedexApp_FilterByType(void *work, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex);
void PokedexApp_FilterByBodyType(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex);
void PokedexApp_FilterByArea(void *work, u32 flags, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByHeight(void *work, u16 lowerIndex, u16 upperIndex, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_FilterByWeight(void *work, u16 lowerIndex, u16 upperIndex, u16 *dst, u32 *dstCount, const u16 *src, u32 srcCount, const Pokedex *pokedex);
void PokedexApp_IntersectSpeciesLists(u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const u16 *src, u32 srcCount, BOOL seenOnly, const Pokedex *pokedex);
int ov18_021F8970(void *work);

#endif
