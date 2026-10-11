#ifndef POKEHEARTGOLD_OVERLAY18_ICON_GRID_RESOURCES_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY18_ICON_GRID_RESOURCES_INTERNAL_H

#include "global.h"

#include "gf_gfx_loader.h"
#include "pokedex.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"

#define PTR(p, off, T) (*(T **)((u8 *)(p) + (off)))
#define SPR(p, i)      (*(ManagedSprite **)((u8 *)(p) + 0x670 + (i) * 4))

void Heap_Free(void *p);
void PokedexApp_LoadSpriteCharData(void *p, int idx, void *data, int size, int kind);
void PokedexApp_SetSpriteTranslucent(void *p, int idx, int draw);
void PokedexApp_UnloadMonIconResources(void *p, u32 count);
void *PokedexApp_LoadMonIconCharData(void *p, u32 species, u32 form, NNSG2dCharacterData **out);
void PokedexApp_SetMonIconGraphics(void *p, void *data, int idx, int palette);
void PokedexApp_SetMonIcon(void *p, u32 species, u32 form, int idx);
void PokedexApp_SetMonIconSub(void *p, u32 species, u32 form, int idx);
void PokedexApp_LoadMonPicResources(void *p);
void PokedexApp_UnloadMonPicResources(void *p);
void ov18_021F193C(void *p);
void ov18_021F19EC(void *p);
void PokedexApp_LoadMonIconResources(void *p, u32 count);
void PokedexApp_CreateMonIconSprites(void *p, int first);
void PokedexApp_CreateMonPicSprites(void *p, int idx);
void PokedexApp_SetListIcon(void *p, int entry, int spriteIdx);
void PokedexApp_DrawListIconGrid(void *p, int base);
extern const ManagedSpriteTemplate sMonIconSpriteTemplate;
extern const ManagedSpriteTemplate ov18_021FABC0;
extern const ManagedSpriteTemplate ov18_021FABF4;

#endif
