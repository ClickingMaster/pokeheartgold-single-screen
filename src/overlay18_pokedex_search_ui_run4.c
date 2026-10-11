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
void PokedexApp_CreateSearchMenuGrid(void *p, int a1);
void PokedexApp_HandleSearchMenuInput(void *p);
void PokedexApp_UpdateSearchMenuCursor(void *p, int a1);

void PokedexApp_SetMonPicBgAffine(void *p, int a1, int a2) {
    fx32 mtx[4];
    fx32 v = FX_Inv(FX_Div(0x100000, a2 << 12));
    mtx[0] = v;
    mtx[3] = v;
    mtx[1] = 0;
    mtx[2] = 0;
    Bg_SetTextDimAndAffineParams(*(void **)((u8 *)p + 4), 7, 0, 0x38, mtx, 0x80, 0x80);
    Bg_SetTextDimAndAffineParams(*(void **)((u8 *)p + 4), 7, 3, 0x10 - a1, mtx, 0x80, 0x80);
}
