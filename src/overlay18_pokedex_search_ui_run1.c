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

BOOL PokedexApp_DragFormScrollbar(void *p, int unused, u32 v);
BOOL PokedexApp_DragAreaScrollbar(void *p, int unused, u32 v);
u32 PokedexApp_GetFormScrollbarY(void *p, int idx);
u32 PokedexApp_GetAreaScrollbarY(void *p, int idx);
u32 ov18_021F6540(void *p, int idx, u32 v, u32 div);
void PokedexApp_MoveScrollbar(void *p, int idx, s16 dy);
void PokedexApp_SyncFormScrollbar(void *p);
void PokedexApp_SyncAreaScrollbar(void *p);
void PokedexApp_ShowFormListSprites(void *p);
void PokedexApp_HideFormListSprites(void *p);
void PokedexApp_ShowFormCompareSprites(void *p);
void PokedexApp_HideFormCompareSprites(void *p);
void ov18_021F6844(void *p, int idx, int value);
void PokedexApp_ShowFormCompareMonPic(void *p, int a1, int a2, int a3);
void ov18_021F6984(void *p, int a1, int a2);
void PokedexApp_UpdateFormCompareSelector(void *p);
void PokedexApp_SetPlayerMarkerAnim(void *p, int variant);
void PokedexApp_SetMonPicBgAffine(void *p, int a1, int a2);
void PokedexApp_CreateSearchMenuGrid(void *p, int a1);
void PokedexApp_HandleSearchMenuInput(void *p);
void PokedexApp_UpdateSearchMenuCursor(void *p, int a1);

BOOL PokedexApp_IsTouchOnFormScrollbar(void *p, u32 x, u32 y) {
    s16 sx;
    s16 sy;
    u32 w;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), &sx, &sy);
    w = PokedexApp_GetScrollbarLength(p, 5, ov18_021FA310);
    if (x >= sx - 11 && x <= sx + 11 && y >= sy - (w >> 1) && y <= sy + (w >> 1)) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokedexApp_IsTouchOnAreaScrollbar(void *p, u32 x, u32 y) {
    s16 sx;
    s16 sy;
    u32 w;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + 0x684), &sx, &sy);
    w = PokedexApp_GetScrollbarLength(p, 5, ov18_021FA304);
    if (x >= sx - 11 && x <= sx + 11 && y >= sy - (w >> 1) && y <= sy + (w >> 1)) {
        return TRUE;
    }
    return FALSE;
}
