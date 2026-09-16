#include "overlay_18_dex_search_private.h"

BOOL ov18_021F7ED4(PokedexAppData *pokedexApp, u8 natDexFlag, u32 order, u32 letter, u32 type1, u32 type2, u32 heightMin, u32 heightMax, u32 weightMin, u32 weightMax, u32 areaMask, u32 bodyType) {
    u16 *buf1;
    u16 *buf2;
    u16 *list;
    u32 listCount;
    u32 count1;
    u32 count2;

    MI_CpuClear32(&pokedexApp->unk_0878, sizeof(PokedexAppData_UnkSub0878));
    buf1 = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, NATIONAL_DEX_COUNT * sizeof(u16));
    GF_ASSERT(buf1 != NULL);
    memset(buf1, 0, NATIONAL_DEX_COUNT * sizeof(u16));
    count1 = 0;
    buf2 = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, NATIONAL_DEX_COUNT * sizeof(u16));
    GF_ASSERT(buf2 != NULL);
    memset(buf2, 0, NATIONAL_DEX_COUNT * sizeof(u16));
    count2 = 0;

    if (natDexFlag == 0) {
        list = ov18_021F8168(DEX_SORT_FILE(NARC_zukan_data_sort_order_dex_order_johto), &listCount);
    } else {
        list = ov18_021F8168(DEX_SORT_FILE(NARC_zukan_data_sort_order_dex_order_national), &listCount);
    }
    ov18_021F8198(buf1, &count1, pokedexApp->args->pokedex, list, listCount);
    ov18_021F822C(order, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F82CC(letter, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;
    ov18_021F831C(type1, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;
    ov18_021F831C(type2, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;
    ov18_021F8468(bodyType, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    if (pokedexApp->unk_1860 == 0 && (areaMask & (1 << DEX_SEARCH_AREA_KANTO))) {
        ov18_021F8584(pokedexApp, (1 << DEX_SEARCH_AREA_JOHTO) | (1 << DEX_SEARCH_AREA_KANTO), buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
        if (count2 != 0) {
            goto filterHeight;
        }
        goto end;
    } else {
        ov18_021F8584(pokedexApp, areaMask, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
        if (count2 == 0) {
            goto end;
        }
    }

filterHeight:
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;
    ov18_021F8640(pokedexApp, heightMin, heightMax, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;
    ov18_021F86D0(pokedexApp, weightMin, weightMax, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    ov18_021F81D8(&pokedexApp->unk_0878, pokedexApp->args->pokedex, buf2, count2);

end:
    Heap_Free(list);
    Heap_Free(buf2);
    Heap_Free(buf1);
    return TRUE;
}
