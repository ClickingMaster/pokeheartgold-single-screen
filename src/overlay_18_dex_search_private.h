#ifndef POKEHEARTGOLD_OVERLAY_18_DEX_SEARCH_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY_18_DEX_SEARCH_PRIVATE_H

#define ov18_021F7ED4 ov18_021F7ED4_UpstreamDecl
#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#undef ov18_021F7ED4

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

BOOL ov18_021F7ED4(PokedexAppData *pokedexApp, u8 natDexFlag, u32 order, u32 letter, u32 type1, u32 type2, u32 heightMin, u32 heightMax, u32 weightMin, u32 weightMax, u32 areaMask, u32 bodyType);
void *ov18_021F8168(u32 fileIdx, u32 *countOut);
void ov18_021F8198(u16 *dst, u32 *count, Pokedex *pokedex, u16 *src, u32 srcCount);
void ov18_021F81D8(PokedexAppData_UnkSub0878 *result, Pokedex *pokedex, u16 *src, u32 count);
void ov18_021F822C(u32 order, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F82CC(u32 letter, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F831C(u32 type, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F8468(u32 bodyType, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F8584(PokedexAppData *pokedexApp, u32 areaMask, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F8640(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F86D0(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F8764(u16 *dst, u32 *count, u16 *lookup, u32 lookupCount, u16 *src, u32 srcCount, BOOL skipCaughtCheck, Pokedex *pokedex);
void ov18_021F89F8(SysTask *task, void *data);

#endif
