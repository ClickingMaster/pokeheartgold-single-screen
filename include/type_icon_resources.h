#ifndef POKEHEARTGOLD_TYPE_ICON_RESOURCES_H
#define POKEHEARTGOLD_TYPE_ICON_RESOURCES_H

#include "sprite_system.h"

int TypeIcon_GetCharFile(int moveType);
NarcId TypeIcon_GetNarc(void);
u32 TypeIcon_GetPalette(u32 type);
void TypeIcon_LoadPaletteBuffer(PaletteData *paletteData, int buffer, SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int resource);
void TypeIcon_LoadCellAnim(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int cell, int animation);
void TypeIcon_LoadChar(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NNS_G2D_VRAM_TYPE vramType, int type, int resource); // load type icon graphics?
void TypeIcon_LoadPltt(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int resId);
void TypeIcon_UnloadChar(SpriteManager *spriteManager, int resource);
void TypeIcon_UnloadPltt(SpriteManager *spriteManager, int resource);
void TypeIcon_UnloadCellAnim(SpriteManager *spriteManager, int cell, int animation);
ManagedSprite *TypeIcon_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int type, ManagedSpriteTemplate *spriteTemplate);
void thunk_ManagedSprite_DeleteAndFreeResources(ManagedSprite *managedSprite);
void MoveCategoryIcon_DeleteSprite(ManagedSprite *managedSprite);
int MoveCategoryIcon_GetCharFile(u32 category);
u32 MoveCategoryIcon_GetPalette(u32 category);
void MoveCategoryIcon_LoadChar(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int category, int resId);
void MoveCategoryIcon_UnloadChar(SpriteManager *spriteManager, int resource);

#endif // POKEHEARTGOLD_TYPE_ICON_RESOURCES_H
