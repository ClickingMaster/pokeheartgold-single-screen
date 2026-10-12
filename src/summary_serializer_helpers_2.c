#include "summary_serializer_helpers_internal.h"

void sub_02089C50(void *v) {
    u8 *w = v;
    u8 value;
    if ((*(u32 *)(w + 0x280) << 3) >> 31 == 0) {
        sub_020729A4(*(void **)(w + 0x7b8), &value, *(u16 *)(w + 0x23c), 1);
        if (*(u16 *)(w + 0x23c) == 0x1b9) {
            sub_02006EA0(*(void **)(*(u8 **)(w + 0x22c) + 0x28), 0, 100, 0, value);
        } else {
            PlayCryDelayed(*(u16 *)(w + 0x23c), value, *(u16 *)(w + 0x27e));
        }
    }
}

void sub_02089CB4(void *v) {
    u8 *w = v;
    switch (*((u8 *)*(void **)(w + 0x22c) + 0x12)) {
    case 0:
    case 1:
        w[0x7bc] = 0;
        break;
    case 2:
        w[0x7bc] = 1;
        break;
    }
    sub_0208B448(v);
    PokemonSummary_UpdateTypeSprites(v);
    PokemonSummary_ShowRibbonSprites(v);
    PokemonSummary_UpdateMarkingSprites(v);
    PokemonSummary_InitSheenSparkles(v);
    PokemonSummary_AddPageWindows(v);
    PokemonSummary_PrintPage(v);
    sub_02089F98(v);
    PokemonSummary_UpdateShinyLeafSprites(v);
    if (*((u8 *)*(void **)(w + 0x22c) + 0x12) == 2) {
        sub_0208A8F4(v);
        return;
    }
    PokemonSummary_UpdatePartySlotSprite(v);
    sub_0208BF9C(v);
    sub_0208BFD0(v);
}
