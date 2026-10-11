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

void ov18_021F2648(void *p) {
    SpriteSystem_LoadCharResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 12, 1, 1, 0xC590);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(*(PaletteData **)((u8 *)p + 0x850), (PaletteBufferId)2, *(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 15, 0, 5, 1, 0xC556);
    SpriteSystem_LoadCellResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 13, 1, 0xC552);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 14, 1, 0xC552);
}

void ov18_021F26E4(void *p) {
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC590);
    SpriteManager_UnloadPlttObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC556);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC552);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC552);
}

void ov18_021F2724(void *p) {
    SpriteSystem_LoadCharResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 26, 1, 2, 0xC591);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(*(PaletteData **)((u8 *)p + 0x850), (PaletteBufferId)3, *(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 32, 0, 1, 2, 0xC557);
    SpriteSystem_LoadCellResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 27, 1, 0xC553);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 28, 1, 0xC553);
    SpriteSystem_LoadCharResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 29, 1, 2, 0xC592);
    SpriteSystem_LoadCellResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 30, 1, 0xC554);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 31, 1, 0xC554);
}

void ov18_021F281C(void *p) {
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC591);
    SpriteManager_UnloadPlttObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC557);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC553);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC553);
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC592);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC554);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC554);
}

void PokedexApp_SetupListSprites(void *p) {
    u16 a;
    int b;
    PokedexApp_LoadListSpriteResources(p);
    PokedexApp_CreateMonIconSprites(p, 24);
    PokedexApp_DrawListIconGrid(p, 24);
    ov18_021F299C(p);
    if (*(int *)((u8 *)p + 0x1860) == 0) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x670), FALSE);
    } else {
        ov18_021F2AC0(p, 0);
    }
    PokedexApp_MoveSpriteToListCursor(p, 5);
    PokedexApp_UpdateListPageArrows(p, 2, 1);
    PokedexApp_UpdateListScrollbarSize(p, 1, 1);
    PokedexApp_SyncListScrollbarToPage(p, 1, 1);
    a = PokedexApp_GetCursorSpecies(p);
    b = PokedexApp_GetListCursorIndex(p);
    PokedexApp_CreateMonPicSprites(p, 11);
    PokedexApp_ShowMonPic(p, a, 11, 10);
    PokedexApp_CreateTypeIconSprites(p, 14);
    PokedexApp_UpdateTypeIcons(p, a, b, 14);
    PokedexApp_CreateFootprintSprite(p, 13);
    PokedexApp_UpdateFootprintSprite(p, a, b, 13);
    PokedexApp_UpdateCaughtMarkSprite(p, b, 9);
    PokedexApp_CreateLanguageIconSprites(p);
    PokedexApp_UpdateLanguageIcons(p, a, 18);
    PokedexApp_UpdateCurrentLanguageIcon(p, a, 8);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x690), FALSE);
}

void PokedexApp_LoadListSpriteResources(void *p) {
    PokedexApp_LoadMonIconResources(p, 60);
    ov18_021F2648(p);
    PokedexApp_LoadLanguageIconResources(p);
    PokedexApp_LoadMonPicResources(p);
    PokedexApp_LoadFootprintResources(p);
    PokedexApp_LoadTypeIconResources(p);
    ov18_021F2724(p);
    PokedexApp_LoadLanguageIconResourcesSub(p);
}
