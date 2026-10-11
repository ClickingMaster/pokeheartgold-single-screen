#include "overlay18_pokedex_status_sprites_r11_private.h"

void PokedexApp_UpdateListPageArrows(PokedexAppData *pokedexApp, int sprite, int mode) {
    if (pokedexApp->unk_1859 == 0) {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite, 7);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite, 5);
    }
    if (pokedexApp->unk_1859 == PokedexApp_GetLastListPage(pokedexApp, mode)) {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite + 1, 10);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, sprite + 1, 8);
    }
}

void PokedexApp_UpdateListScrollbarSize(PokedexAppData *pokedexApp, int sprite, BOOL mode) {
    PokedexApp_SetSpriteAnim(pokedexApp, sprite, PokedexApp_GetListScrollbarSizeAnim(pokedexApp, mode));
}

int PokedexApp_GetListScrollbarSizeAnim(PokedexAppData *pokedexApp, BOOL mode) {
    u32 value = PokedexApp_GetListLength(pokedexApp, mode);
    u32 i = 0;

    do {
        if (value <= sListScrollbarSizeThresholds[i]) {
            break;
        }
        i++;
    } while (i < 12);
    return i + 11;
}

u32 PokedexApp_GetListScrollbarLength(PokedexAppData *pokedexApp, int index) {
    ManagedSprite *sprite = *(ManagedSprite **)((u8 *)pokedexApp + 0x670 + index * sizeof(ManagedSprite *));

    return (ov18_021FA310 + 1)[ManagedSprite_GetActiveAnim(sprite)];
}
