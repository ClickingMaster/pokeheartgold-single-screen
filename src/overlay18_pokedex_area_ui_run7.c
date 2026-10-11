#include "overlay18_pokedex_area_ui_private.h"

void PokedexApp_UnloadFootprintResources(void *p);
void PokedexApp_CreateFootprintSprite(void *p, int idx);
void PokedexApp_UpdateFootprintSprite(void *p, int species, int idx, int spriteIdx);
void PokedexApp_UnloadTypeIconResources(void *p);
void PokedexApp_CreateTypeIconSprites(void *p, int idx);
void PokedexApp_UnloadLanguageIconResources(void *p);
void PokedexApp_LoadLanguageIconResourcesSub(void *p);
void PokedexApp_UnloadLanguageIconResourcesSub(void *p);
void PokedexApp_CreateSpriteInSlot(void *p, int idx, const ManagedSpriteTemplate *tmpl);
void PokedexApp_CreateLanguageIconSprites(void *p);
void PokedexApp_UpdateCurrentLanguageIcon(void *p, int a1, int idx);
void PokedexApp_UpdateLanguageIcons(void *p, u16 species, int a2);
void PokedexApp_LoadLanguageIconResources(void *p);
void PokedexApp_LoadFootprintResources(void *p);
void PokedexApp_LoadTypeIconResources(void *p);
void PokedexApp_UpdateTypeIcons(void *p, int a1, int a2, int a3);
void ov18_021F2648(void *p);
void ov18_021F26E4(void *p);
void ov18_021F2724(void *p);
void ov18_021F281C(void *p);
void PokedexApp_SetupListSprites(void *p);
void PokedexApp_LoadListSpriteResources(void *p);
void ov18_021F299C(void *p);
void PokedexApp_SetSpritePairVisibleIfInternationalView(void *p, int idx, int draw);
void PokedexApp_SetSpriteVisibleIfInternationalView(void *p, int idx, int draw);
void ov18_021F2AC0(void *p, int idx);
BOOL PokedexApp_IsTouchOnSprite0(void *p, u32 x, u32 y);
void PokedexApp_MoveSpriteX(void *p, int idx, s16 dx);
void PokedexApp_SetSpriteX(void *p, int idx, s16 x);

void PokedexApp_MoveSpriteX(void *p, int idx, s16 dx) {
    s16 sx;
    s16 sy;
    ManagedSprite **base = (ManagedSprite **)((u8 *)p + 0x670);
    ManagedSprite_GetPositionXY(base[idx], &sx, &sy);
    ManagedSprite_SetPositionXY(base[idx], sx + dx, sy);
}

void PokedexApp_SetSpriteX(void *p, int idx, s16 x) {
    s16 sx;
    s16 sy;
    ManagedSprite **base = (ManagedSprite **)((u8 *)p + 0x670);
    ManagedSprite_GetPositionXY(base[idx], &sx, &sy);
    ManagedSprite_SetPositionXY(base[idx], x, sy);
}
