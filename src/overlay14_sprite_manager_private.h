#ifndef POKEHEARTGOLD_OVERLAY14_SPRITE_MANAGER_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_SPRITE_MANAGER_PRIVATE_H

#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))

typedef struct R50Counts {
    u32 word[6];
} R50Counts;

typedef struct R50Transfer {
    u32 word[8];
} R50Transfer;

typedef struct R50Oam {
    u32 word[5];
} R50Oam;

extern const R50Counts ov14_021F80D4;
extern const R50Transfer ov14_021F80EC;
extern const R50Oam ov14_021F80A8;

extern void ManagedSprite_SetDrawPriority(void *, u32);
extern void ManagedSprite_SetOamMode(void *, u32);
extern void *SpriteSystem_Alloc(u32);
extern void *SpriteManager_New(void *);
extern void SpriteSystem_Init(void *, const R50Transfer *, const R50Oam *, u32);
extern void SpriteSystem_InitSprites(void *, void *, u32);
extern void SpriteSystem_InitManagerWithCapacities(void *, void *, const R50Counts *);
extern void *SpriteSystem_GetRenderer(void *);
extern void G2dRenderer_SetSubSurfaceCoords(void *, s32, s32);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern void SpriteSystem_Free(void *);
extern void Sprite_DeleteAndFreeResources(void *);

extern void PCBox_LoadTrayBoxResources(void *);
extern void PCBox_LoadWallpaperListResources(void *);
extern void PCBox_LoadMonIconResources(void *);
extern void PCBox_LoadUiSpriteResources(void *);
extern void PCBox_LoadItemIconResources(void *);
extern void PCBox_LoadMonPicResources(void *);
extern void PCBox_LoadTypeIconResources(void *);
extern void PCBox_CreateTrayBoxSprites(void *);
extern void PCBox_CreateWallpaperListSprites(void *);
extern void PCBox_CreateMonIconSprites(void *);
extern void PCBox_CreateUiSprites(void *);
extern void PCBox_CreateItemIconSprites(void *);
extern void PCBox_CreateMonPicSprites(void *);
extern void PCBox_CreateTypeIconSprites(void *);

void PCBox_SetSpriteDrawPriority(void *, u32, u32);
void PCBox_SetSpriteTranslucent(void *, u32, u32);
void PCBox_InitSpriteSystem(void *);
void PCBox_FreeSpriteSystem(void *);
void PCBox_LoadSpriteResources(void *);
void PCBox_CreateSprites(void *);
void PCBox_DeleteSprite(void *, u32);

#endif
