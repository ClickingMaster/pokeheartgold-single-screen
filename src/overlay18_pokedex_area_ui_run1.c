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

void PokedexApp_UnloadFootprintResources(void *p) {
    SpriteManager_UnloadCharObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC599);
    SpriteManager_UnloadPlttObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC55B);
    SpriteManager_UnloadCellObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC558);
    SpriteManager_UnloadAnimObjById(*(SpriteManager **)((u8 *)p + 0x66C), 0xC558);
}

void PokedexApp_CreateFootprintSprite(void *p, int idx) {
    u8 *base = (u8 *)p + 0x670;
    int offset = idx * 4;
    *(ManagedSprite **)(base + offset) = SpriteSystem_NewSpriteWithYOffset(*(SpriteSystem **)((u8 *)p + 0x668), *(SpriteManager **)((u8 *)p + 0x66C), &sFootprintSpriteTemplate, 0x200000);
    ManagedSprite_SetPaletteOverride(*(ManagedSprite **)(base + offset), SpriteManager_FindPlttResourceOffset(*(SpriteManager **)((u8 *)p + 0x66C), 0xC55B, 2));
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(base + offset), FALSE);
}

void PokedexApp_UpdateFootprintSprite(void *p, int species, int idx, int spriteIdx) {
    void *str;
    if (*(u8 *)((u8 *)p + 0x185C) != 2 || species == 0 || *(u16 *)((u8 *)p + 0x1032 + idx * 4) == 1 || (species == 0x1E7 && Pokedex_GetSeenFormByIdx(*(void **)*(void **)p, species, 0) == 1)) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x670 + spriteIdx * 4), FALSE);
    } else {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)p + 0x670 + spriteIdx * 4), TRUE);
        str = PokedexApp_LoadFootprintGraphics(species, 37);
        PokedexApp_LoadSpriteCharData(p, spriteIdx, str, 128, 2);
        Heap_Free(str);
    }
}
