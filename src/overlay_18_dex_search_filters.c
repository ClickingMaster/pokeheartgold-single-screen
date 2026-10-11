#include "overlay_18_dex_search_private.h"

void PokedexApp_FilterByArea(PokedexAppData *pokedexApp, u32 areaMask, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    if (areaMask & (1 << DEX_SEARCH_AREA_ALL)) {
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    }
    if (areaMask == ((1 << DEX_SEARCH_AREA_JOHTO) | (1 << DEX_SEARCH_AREA_KANTO))) {
        u32 i;
        for (i = 0; i < srcCount; i++) {
            if ((areaMask & pokedexApp->unk_1854[src[i]]) && !(pokedexApp->unk_1854[src[i]] & (1 << DEX_SEARCH_AREA_UNKNOWN)) && Pokedex_CheckMonSeenFlag(pokedex, src[i])) {
                dst[*count] = src[i];
                (*count)++;
            }
        }
    } else {
        u32 i;
        for (i = 0; i < srcCount; i++) {
            if ((pokedexApp->unk_1854[src[i]] & areaMask) && Pokedex_CheckMonSeenFlag(pokedex, src[i])) {
                dst[*count] = src[i];
                (*count)++;
            }
        }
    }
}

void PokedexApp_FilterByHeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    u32 i;
    if (min == 0 && max == DEX_SEARCH_SLIDER_MAX) {
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    }
    for (i = 0; i < srcCount; i++) {
        s32 height = ((s32 *)pokedexApp->heights)[src[i]];
        if (height >= pokedexApp->unk_1850[min].unk_0 && height <= pokedexApp->unk_1850[max].unk_0 && Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
            dst[*count] = src[i];
            (*count)++;
        }
    }
}

void PokedexApp_FilterByWeight(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    u32 i;
    if (min == 0 && max == DEX_SEARCH_SLIDER_MAX) {
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    }
    for (i = 0; i < srcCount; i++) {
        s32 weight = ((s32 *)pokedexApp->weights)[src[i]];
        if (weight >= pokedexApp->unk_1850[min].unk_2 && weight <= pokedexApp->unk_1850[max].unk_2 && Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
            dst[*count] = src[i];
            (*count)++;
        }
    }
}

void PokedexApp_IntersectSpeciesLists(u16 *dst, u32 *count, u16 *lookup, u32 lookupCount, u16 *src, u32 srcCount, BOOL skipCaughtCheck, Pokedex *pokedex) {
    u32 i;
    u32 j;
    *count = 0;
    i = 0;
    if (skipCaughtCheck == TRUE) {
        for (; i < srcCount; i++) {
            for (j = 0; j < lookupCount; j++) {
                if (src[i] == lookup[j]) {
                    dst[*count] = src[i];
                    (*count)++;
                    break;
                }
            }
        }
    } else {
        for (; i < srcCount; i++) {
            for (j = 0; j < lookupCount; j++) {
                if (src[i] == lookup[j] && Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
                    dst[*count] = src[i];
                    (*count)++;
                    break;
                }
            }
        }
    }
}
