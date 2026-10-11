#include "overlay18_scroll_math_private.h"

BOOL PokedexApp_IsTouchOnListScrollbar(PokedexAppData *pokedexApp, int index, u32 x, u32 y) {
    s16 spriteX;
    s16 spriteY;
    u32 width;

    ManagedSprite_GetPositionXY(POKEDEX_SPRITE(pokedexApp, index), &spriteX, &spriteY);
    width = PokedexApp_GetListScrollbarLength(pokedexApp, index);

    if (x >= spriteX - 11 && x <= spriteX + 11 && y >= spriteY - (width >> 1) && y <= spriteY + (width >> 1)) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokedexApp_DragListScrollbar(PokedexAppData *pokedexApp, int index, u32 unused, u32 y, int mode) {
    s16 spriteX;
    s16 spriteY;
    u32 lower;
    u32 step;
    u32 count;

    ManagedSprite_GetPositionXY(POKEDEX_SPRITE(pokedexApp, index), &spriteX, &spriteY);
    lower = PokedexApp_GetListScrollbarMinY(pokedexApp, index);
    if (y < lower) {
        y = lower;
    }
    step = PokedexApp_GetListScrollbarMaxY(pokedexApp, index);
    if (y > step) {
        y = step;
    }
    ManagedSprite_SetPositionXY(POKEDEX_SPRITE(pokedexApp, index), spriteX, (s16)y);

    lower = PokedexApp_GetListScrollbarMinY(pokedexApp, index);
    step = PokedexApp_GetListScrollbarMaxY(pokedexApp, index) - lower;
    count = PokedexApp_GetLastListPage(pokedexApp, mode);
    step = (step << 8) / count;
    {
        u32 offset = (y - lower) << 8;
        u32 i = 0;
        u32 left = i;
        u32 right = i;

        while (i <= count) {
            if (offset >= left && offset < right + step) {
                if (POKEDEX_PAGE(pokedexApp) == i) {
                    break;
                }
                POKEDEX_PAGE(pokedexApp) = i;
                return TRUE;
            }
            left += step;
            right += step;
            i++;
        }
    }
    return FALSE;
}

int PokedexApp_GetListScrollbarYForPage(PokedexAppData *pokedexApp, int page, int index, int mode) {
    int lower = PokedexApp_GetListScrollbarMinY(pokedexApp, index);
    int result = PokedexApp_GetListScrollbarMaxY(pokedexApp, index);
    u32 count = PokedexApp_GetLastListPage(pokedexApp, mode);

    if (page != count) {
        u32 step = ((result - lower) << 8) / count;
        u32 scaled = step;
        scaled *= page;
        result = lower + (scaled >> 8);
    }
    return result;
}

u32 ov18_021F2E14(PokedexAppData *pokedexApp, int index, u32 position, u32 divisor) {
    s16 spriteX;
    s16 spriteY;

    ManagedSprite_GetPositionXY(POKEDEX_SPRITE(pokedexApp, index), &spriteX, &spriteY);
    if (position >= spriteY) {
        return (position - spriteY) / divisor;
    }
    return (spriteY - position) / divisor;
}

void PokedexApp_MoveListScrollbar(PokedexAppData *pokedexApp, int index, int delta) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(POKEDEX_SPRITE(pokedexApp, index), &x, &y);
    ManagedSprite_SetPositionXY(POKEDEX_SPRITE(pokedexApp, index), x, y + delta);
}

void PokedexApp_SyncListScrollbarToPage(PokedexAppData *pokedexApp, int index, int mode) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(POKEDEX_SPRITE(pokedexApp, index), &x, &y);
    ManagedSprite_SetPositionXY(POKEDEX_SPRITE(pokedexApp, index), x, PokedexApp_GetListScrollbarYForPage(pokedexApp, POKEDEX_PAGE(pokedexApp), index, mode));
}
