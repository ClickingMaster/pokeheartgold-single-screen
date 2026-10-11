#ifndef POKEHEARTGOLD_OVERLAY_18_DEX_SEARCH_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY_18_DEX_SEARCH_PRIVATE_H

#define PokedexApp_RunSearch ov18_021F7ED4_UpstreamDecl
#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#undef PokedexApp_RunSearch

#include "filesystem_files_def.h"
#include "heap.h"
#include "pokedex.h"
#include "sprite.h"
#include "sys_task_api.h"

#define DEX_SORT_FILE(member) ((member) - NARC_zukan_data_sort_order_dex_order_national)
#define DEX_SEARCH_SLIDER_MAX 152

typedef struct PokedexEntryPopup {
    u8 copiedPrefix[0x14];
    enum HeapID heapId;
    SysTask *task;
    NARC *gfxNarc;
    u8 padding_020[0x220];
    u16 unk_240;
    u8 padding_242[0x12];
    u32 unk_254;
} PokedexEntryPopup;

typedef char PokedexEntryPopup_size_must_be_0x258[sizeof(PokedexEntryPopup) == 0x258 ? 1 : -1];
typedef char PokedexEntryPopup_heapId_must_be_0x14[offsetof(PokedexEntryPopup, heapId) == 0x14 ? 1 : -1];
typedef char PokedexEntryPopup_task_must_be_0x18[offsetof(PokedexEntryPopup, task) == 0x18 ? 1 : -1];
typedef char PokedexEntryPopup_gfxNarc_must_be_0x1C[offsetof(PokedexEntryPopup, gfxNarc) == 0x1C ? 1 : -1];
typedef char PokedexEntryPopup_unk240_must_be_0x240[offsetof(PokedexEntryPopup, unk_240) == 0x240 ? 1 : -1];
typedef char PokedexEntryPopup_unk254_must_be_0x254[offsetof(PokedexEntryPopup, unk_254) == 0x254 ? 1 : -1];

BOOL PokedexApp_RunSearch(PokedexAppData *pokedexApp, u8 natDexFlag, u32 order, u32 letter, u32 type1, u32 type2, u32 heightMin, u32 heightMax, u32 weightMin, u32 weightMax, u32 areaMask, u32 bodyType);
void *PokedexApp_LoadSortData(u32 fileIdx, u32 *countOut);
void PokedexApp_FilterSeen(u16 *dst, u32 *count, Pokedex *pokedex, u16 *src, u32 srcCount);
void PokedexApp_StoreSearchResults(PokedexAppData_UnkSub0878 *result, Pokedex *pokedex, u16 *src, u32 count);
void PokedexApp_ApplySortOrder(u32 order, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByLetter(u32 letter, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByType(u32 type, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByBodyType(u32 bodyType, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByArea(PokedexAppData *pokedexApp, u32 areaMask, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByHeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_FilterByWeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void PokedexApp_IntersectSpeciesLists(u16 *dst, u32 *count, u16 *lookup, u32 lookupCount, u16 *src, u32 srcCount, BOOL skipCaughtCheck, Pokedex *pokedex);
void PokedexEntryPopup_Task(SysTask *task, void *data);

#endif
