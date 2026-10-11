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

void PokedexApp_UnloadLanguageIconResources(void *p) {
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC597);
    SpriteManager_UnloadPlttObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC559);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC556);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC556);
}

void PokedexApp_LoadLanguageIconResourcesSub(void *p) {
    SpriteSystem_LoadCharResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 53, 1, 2, 0xC598);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(*(PaletteData **)((u8 *)p + 0x850), (PaletteBufferId)3, *(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 56, 0, 1, 2, 0xC55A);
    SpriteSystem_LoadCellResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 54, 1, 0xC557);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), *(NARC **)((u8 *)p + 0x854), 55, 1, 0xC557);
}

void PokedexApp_UnloadLanguageIconResourcesSub(void *p) {
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC598);
    SpriteManager_UnloadPlttObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC55A);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC557);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC557);
}

void PokedexApp_CreateSpriteInSlot(void *p, int idx, const ManagedSpriteTemplate *tmpl) {
    if (tmpl->vramTransfer == 1) {
        *(ManagedSprite **)((u8 *)p + idx * 4 + 0x670) = SpriteSystem_NewSprite(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), tmpl);
    } else {
        *(ManagedSprite **)((u8 *)p + idx * 4 + 0x670) = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), tmpl, 0x200000);
    }
}
