#include "overlay18_pokedex_result_helpers_private.h"

void PokedexApp_MoveFormListIcons(void *work, int value) {
    u32 i;

    for (i = 0; i < 6; i++) {
        PokedexApp_MoveSprite(work, i + 0xe, 0, value, 0);
    }
}

void PokedexApp_UpdateScrollArrows(void *work, int index, int count, int sprite) {
    if (index == 0) {
        PokedexApp_SetSpriteAnim(work, sprite, 7);
    } else {
        PokedexApp_SetSpriteAnim(work, sprite, 5);
    }
    if (index == count - 1) {
        PokedexApp_SetSpriteAnim(work, sprite + 1, 10);
    } else {
        PokedexApp_SetSpriteAnim(work, sprite + 1, 8);
    }
}

void PokedexApp_SetScrollbarSizeAnim(void *work, int sprite, int value, const u16 *table, u32 count) {
    PokedexApp_SetSpriteAnim(work, sprite, PokedexApp_GetScrollbarSizeAnim(work, value, table, count));
}

int PokedexApp_GetScrollbarSizeAnim(void *unused, int value, const u16 *table, u32 count) {
    u32 i = 0;

    if (count > i) {
        do {
            u16 threshold = *table;
            if (value <= threshold) {
                break;
            }
            i++;
            table++;
        } while (i < count);
    }
    return i + 0xe;
}
