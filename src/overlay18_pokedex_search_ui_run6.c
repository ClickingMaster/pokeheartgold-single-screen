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

void PokedexApp_UpdateSearchMenuCursor(void *p, int a1) {
    u8 x;
    u8 y;
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(*(void **)((u8 *)p + 0x1864), a1), &x, &y);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)p + 0x670), x, y);
    PokedexApp_SetSpriteAnim(p, 0, sSearchMenuCursorAnims[a1]);
}
