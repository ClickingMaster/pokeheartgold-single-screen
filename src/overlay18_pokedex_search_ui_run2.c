#include "overlay18_pokedex_search_ui_private.h"

BOOL PokedexApp_IsTouchOnFormScrollbar(void *p, u32 x, u32 y);
BOOL PokedexApp_IsTouchOnAreaScrollbar(void *p, u32 x, u32 y);
BOOL PokedexApp_DragFormScrollbar(void *p, int unused, u32 v);
BOOL PokedexApp_DragAreaScrollbar(void *p, int unused, u32 v);
u32 ov18_021F6540(void *p, int idx, u32 v, u32 div);
void PokedexApp_MoveScrollbar(void *p, int idx, s16 dy);
void PokedexApp_ShowFormListSprites(void *p);
void PokedexApp_HideFormListSprites(void *p);
void PokedexApp_ShowFormCompareSprites(void *p);
void PokedexApp_HideFormCompareSprites(void *p);
void ov18_021F6984(void *p, int a1, int a2);
void PokedexApp_SetPlayerMarkerAnim(void *p, int variant);
void PokedexApp_SetMonPicBgAffine(void *p, int a1, int a2);
void PokedexApp_CreateSearchMenuGrid(void *p, int a1);
void PokedexApp_HandleSearchMenuInput(void *p);

BOOL PokedexApp_IsTouchOnFormScrollbar(void *p, u32 x, u32 y);
BOOL PokedexApp_IsTouchOnAreaScrollbar(void *p, u32 x, u32 y);
BOOL PokedexApp_DragFormScrollbar(void *p, int unused, u32 v);
BOOL PokedexApp_DragAreaScrollbar(void *p, int unused, u32 v);
void PokedexApp_ShowFormCompareMonPic(void *p, int a1, int a2, int a3);
void ov18_021F6984(void *p, int a1, int a2);
void PokedexApp_UpdateFormCompareSelector(void *p);
void PokedexApp_SetPlayerMarkerAnim(void *p, int variant);
void PokedexApp_SetMonPicBgAffine(void *p, int a1, int a2);
void PokedexApp_CreateSearchMenuGrid(void *p, int a1);
void PokedexApp_HandleSearchMenuInput(void *p);
void PokedexApp_UpdateSearchMenuCursor(void *p, int a1);

u32 PokedexApp_GetFormScrollbarY(void *p, int idx) {
    u32 lo;
    u32 v;
    int n;
    u32 q;
    u32 t;
    lo = PokedexApp_GetScrollbarMinY(p, 5, ov18_021FA310, 0x40, 0x56);
    v = PokedexApp_GetScrollbarMaxY(p, 5, ov18_021FA310, 0x40, 0x56);
    n = *(s8 *)((u8 *)p + 0x18C4) - 1;
    if (idx != n) {
        q = ((v - lo) << 8) / n;
        t = q * idx;
        v = lo + (t >> 8);
    }
    return v;
}

u32 PokedexApp_GetAreaScrollbarY(void *p, int idx) {
    u32 lo;
    u32 v;
    int n;
    u32 q;
    u32 t;
    lo = PokedexApp_GetScrollbarMinY(p, 5, ov18_021FA304, 0x60, 0x56);
    v = PokedexApp_GetScrollbarMaxY(p, 5, ov18_021FA304, 0x60, 0x56);
    n = *(int *)((u8 *)p + 0x1900) - 1;
    if (idx != n) {
        q = ((v - lo) << 8) / n;
        t = q * idx;
        v = lo + (t >> 8);
    }
    return v;
}

u32 ov18_021F6540(void *p, int idx, u32 v, u32 div) {
    s16 sx;
    s16 sy;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + idx * 4 + 0x670), &sx, &sy);
    if (v >= sy) {
        return (v - sy) / div;
    } else {
        return (sy - v) / div;
    }
}

void PokedexApp_MoveScrollbar(void *p, int idx, s16 dy) {
    s16 sx;
    s16 sy;
    ManagedSprite **base = (ManagedSprite **)((u8 *)p + 0x670);
    ManagedSprite_GetPositionXY(base[idx], &sx, &sy);
    ManagedSprite_SetPositionXY(base[idx], sx, sy + dy);
}

void PokedexApp_SyncFormScrollbar(void *p) {
    s16 sx;
    s16 sy;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), &sx, &sy);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), sx, PokedexApp_GetFormScrollbarY(p, *(s8 *)((u8 *)p + 0x18C5)));
}

void PokedexApp_SyncAreaScrollbar(void *p) {
    s16 sx;
    s16 sy;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), &sx, &sy);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), sx, PokedexApp_GetAreaScrollbarY(p, *(s8 *)((u8 *)p + 0x18CA)));
}

void PokedexApp_ShowFormListSprites(void *p) {
    PokedexApp_ShowFormMonPics(p, *(s8 *)((u8 *)p + 0x18C5), 1);
    PokedexApp_DrawFormListIcons(p);
    PokedexApp_SyncFormScrollbar(p);
    PokedexApp_UpdateScrollArrows(p, *(s8 *)((u8 *)p + 0x18C5), *(s8 *)((u8 *)p + 0x18C4), 6);
    PokedexApp_SetSpriteVisible(p, 5, 1);
    PokedexApp_SetSpriteVisible(p, 6, 1);
    PokedexApp_SetSpriteVisible(p, 7, 1);
    PokedexApp_SetSpriteVisible(p, 8, 1);
}

void PokedexApp_HideFormListSprites(void *p) {
    PokedexApp_SetSpriteVisible(p, 5, 0);
    PokedexApp_SetSpriteVisible(p, 6, 0);
    PokedexApp_SetSpriteVisible(p, 7, 0);
    PokedexApp_SetSpriteVisible(p, 8, 0);
    PokedexApp_SetSpriteVisible(p, 14, 0);
    PokedexApp_SetSpriteVisible(p, 15, 0);
    PokedexApp_SetSpriteVisible(p, 16, 0);
    PokedexApp_SetSpriteVisible(p, 17, 0);
    PokedexApp_SetSpriteVisible(p, 18, 0);
    PokedexApp_SetSpriteVisible(p, 19, 0);
    PokedexApp_SetSpriteVisible(p, 1, 0);
    PokedexApp_SetSpriteVisible(p, 2, 0);
    PokedexApp_SetSpriteVisible(p, 3, 0);
    PokedexApp_SetSpriteVisible(p, 4, 0);
}

void PokedexApp_ShowFormCompareSprites(void *p) {
    if (*(s8 *)((u8 *)p + 0x18C4) >= 3) {
        PokedexApp_SetSpriteVisible(p, 9, 1);
        PokedexApp_SetSpriteVisible(p, 10, 1);
        PokedexApp_SetSpriteVisible(p, 11, 1);
        PokedexApp_SetSpriteVisible(p, 12, 1);
        PokedexApp_SetSpriteVisible(p, 13, 1);
    }
    ov18_021F6844(p, 14, *(s8 *)((u8 *)p + 0x18C5));
    ov18_021F6844(p, 15, *(s8 *)((u8 *)p + 0x18C6));
    PokedexApp_SetSpritePos(p, 14, 0x40, 0x50, 0);
    PokedexApp_SetSpritePos(p, 15, 0xC0, 0x50, 0);
    PokedexApp_ShowFormCompareMonPic(p, 1, *(s8 *)((u8 *)p + 0x18C5), 1);
    PokedexApp_ShowFormCompareMonPic(p, 2, *(s8 *)((u8 *)p + 0x18C6), 1);
    PokedexApp_UpdateFormCompareSelector(p);
    PokedexApp_SetSpriteVisible(p, 14, 1);
    PokedexApp_SetSpriteVisible(p, 15, 1);
}

void PokedexApp_HideFormCompareSprites(void *p) {
    PokedexApp_SetSpriteVisible(p, 9, 0);
    PokedexApp_SetSpriteVisible(p, 10, 0);
    PokedexApp_SetSpriteVisible(p, 11, 0);
    PokedexApp_SetSpriteVisible(p, 12, 0);
    PokedexApp_SetSpriteVisible(p, 13, 0);
    PokedexApp_SetSpriteVisible(p, 14, 0);
    PokedexApp_SetSpriteVisible(p, 15, 0);
    PokedexApp_SetSpriteVisible(p, 1, 0);
    PokedexApp_SetSpriteVisible(p, 2, 0);
    PokedexApp_SetSpriteVisible(p, 3, 0);
    PokedexApp_SetSpriteVisible(p, 4, 0);
}

void ov18_021F6844(void *p, int idx, int value) {
    PokedexApp_SetFormListIcon(p, idx, value);
}
