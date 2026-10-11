#include "overlay18_sol_partial_internal.h"

void PokedexApp_SwapMonPic(void *, int, int, int);
u32 PokedexApp_GetListScrollbarLength(void *, int);
void PokedexApp_LoadSearchSpriteResources(void *);
void PokedexApp_CreateSearchSprites(void *);
void PokedexApp_UnloadSearchSpriteResources(void *);
void ov18_021F26E4(void *);
void PokedexApp_UpdateSearchMenuCursor(void *);
void PokedexApp_UpdateSearchOrderCursor(void *);
void PokedexApp_UpdateSearchHeightCursor(void *);
void PokedexApp_UpdateSearchWeightCursor(void *);
void PokedexApp_UpdateSearchAreaCursor(void *);
void PokedexApp_DrawFormList(void *);
void PokedexApp_DrawFormsTitle(void *);
void PokedexApp_DrawSelectedFormName(void *);
void PokedexApp_ClearFormsWindows(void *);
void PokedexApp_UnloadAreaSpriteResources(void *);
void PokedexApp_UnloadSizeCheckResources(void *);
void PokedexApp_LoadMonIconResources(void *, int);
void PokedexApp_LoadMonPicResources(void *);
void ov18_021F193C(void *);
void PokedexApp_CreateFormsSprites(void *);
void PokedexApp_UpdateSearchNameCursor(void *, int);
void PokedexApp_UpdateSearchTypeCursor(void *, int);
void PokedexApp_UpdateSearchFormCursor(void *, int);
void PokedexApp_CreateSpriteSystem(void *);
void PokedexApp_UnloadMonPicResources(void *);
void PokedexApp_UnloadFootprintResources(void *);
void PokedexApp_UnloadTypeIconResources(void *);
void ov18_021F281C(void *);
void PokedexApp_UnloadLanguageIconResourcesSub(void *);
void PokedexApp_UnloadMonIconResources(void *, int);
void ov18_021F19EC(void *);
void PokedexEntryPopup_FreeSprites(void *);
void PokedexApp_UnloadLanguageIconResources(void *);
void PokedexEntryPopup_UnloadFootprintResources(void *);
void PokedexEntryPopup_UnloadTypeIconResources(void *);
void ov18_021F9054(void *);
void PokedexEntryPopup_DestroyObjCharTransfer(void);

void ov18_021F6984(void *p, int a1, int a2);

void PokedexApp_ShowMonPic(void *p, int a1, int a2, int a3) {
    PokedexApp_SwapMonPic(p, a1, a2, a3);
}
