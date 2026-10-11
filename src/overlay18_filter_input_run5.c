#include "overlay18_filter_input_internal.h"

#define U8_AT(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U16_AT(p, n) (*(u16 *)((u8 *)(p) + (n)))

typedef struct Ov18Flags5 {
    u8 value : 5;
    u8 : 3;
} Ov18Flags5;

void PokedexApp_FilterByBodyType(u32 index, u16 *dst, u32 *dstCount, const u16 *filter, u32 filterCount, const Pokedex *pokedex) {
    u16 *resource;
    u32 resourceCount;
    switch (index) {
    case 14:
        memcpy(dst, filter, filterCount * sizeof(u16));
        *dstCount = filterCount;
        return;
    case 7:
        resource = PokedexApp_LoadSortData(0x44, &resourceCount);
        break;
    case 13:
        resource = PokedexApp_LoadSortData(0x45, &resourceCount);
        break;
    case 11:
        resource = PokedexApp_LoadSortData(0x46, &resourceCount);
        break;
    case 5:
        resource = PokedexApp_LoadSortData(0x47, &resourceCount);
        break;
    case 4:
        resource = PokedexApp_LoadSortData(0x48, &resourceCount);
        break;
    case 12:
        resource = PokedexApp_LoadSortData(0x49, &resourceCount);
        break;
    case 9:
        resource = PokedexApp_LoadSortData(0x4A, &resourceCount);
        break;
    case 6:
        resource = PokedexApp_LoadSortData(0x4B, &resourceCount);
        break;
    case 1:
        resource = PokedexApp_LoadSortData(0x4C, &resourceCount);
        break;
    case 2:
        resource = PokedexApp_LoadSortData(0x4D, &resourceCount);
        break;
    case 3:
        resource = PokedexApp_LoadSortData(0x4E, &resourceCount);
        break;
    case 10:
        resource = PokedexApp_LoadSortData(0x4F, &resourceCount);
        break;
    case 0:
        resource = PokedexApp_LoadSortData(0x50, &resourceCount);
        break;
    case 8:
        resource = PokedexApp_LoadSortData(0x51, &resourceCount);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokedexApp_IntersectSpeciesLists(dst, dstCount, resource, resourceCount, filter, filterCount, TRUE, pokedex);
    Heap_Free(resource);
}
