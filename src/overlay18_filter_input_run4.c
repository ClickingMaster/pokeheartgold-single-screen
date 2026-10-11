#include "overlay18_filter_input_internal.h"

#define U8_AT(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U16_AT(p, n) (*(u16 *)((u8 *)(p) + (n)))

typedef struct Ov18Flags5 {
    u8 value : 5;
    u8 : 3;
} Ov18Flags5;

void PokedexApp_ApplySortOrder(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex) {
    u16 *resource;
    u32 resourceCount;
    BOOL seenOnly = FALSE;

    switch (index) {
    case 0:
        memcpy(dst, filter, filterCount * sizeof(u16));
        *dstCount = filterCount;
        return;
    case 1:
        resource = PokedexApp_LoadSortData(2, &resourceCount);
        seenOnly = TRUE;
        break;
    case 2:
        resource = PokedexApp_LoadSortData(3, &resourceCount);
        break;
    case 3:
        resource = PokedexApp_LoadSortData(4, &resourceCount);
        break;
    case 4:
        resource = PokedexApp_LoadSortData(5, &resourceCount);
        break;
    case 5:
        resource = PokedexApp_LoadSortData(6, &resourceCount);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokedexApp_IntersectSpeciesLists(dst, dstCount, filter, filterCount, resource, resourceCount, seenOnly, pokedex);
    Heap_Free(resource);
}

void PokedexApp_FilterByLetter(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex) {
    u16 *resource;
    u32 resourceCount;

    if (index == 26) {
        memcpy(dst, filter, filterCount * sizeof(u16));
        *dstCount = filterCount;
        return;
    }
    resource = PokedexApp_LoadSortData(index + 7, &resourceCount);
    PokedexApp_IntersectSpeciesLists(dst, dstCount, resource, resourceCount, filter, filterCount, TRUE, pokedex);
    Heap_Free(resource);
}
