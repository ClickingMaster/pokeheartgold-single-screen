#include "overlay_18_dex_search_private.h"

void PokedexApp_StoreSearchResults(PokedexAppData_UnkSub0878 *result, Pokedex *pokedex, u16 *src, u32 count) {
    u32 i;
    result->unk_7B4 = count;
    result->unk_7B6 = 0;
    for (i = 0; i < count; i++) {
        result->unk_000[i][0] = src[i];
        if (Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
            result->unk_000[i][1] = 2;
            result->unk_7B6++;
        } else {
            result->unk_000[i][1] = 1;
        }
    }
}
