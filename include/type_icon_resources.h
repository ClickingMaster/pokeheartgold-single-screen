#ifndef POKEHEARTGOLD_TYPE_ICON_RESOURCES_H
#define POKEHEARTGOLD_TYPE_ICON_RESOURCES_H

#include "sprite_system.h"

int TypeIcon_GetCharFile(int moveType);
NarcId TypeIcon_GetNarc(void);
void TypeIcon_LoadPaletteBuffer(PaletteData *paletteData, int a1, SpriteSystem *spriteSystem, SpriteManager *spriteManager, int a4, int a5);
void TypeIcon_LoadCellAnim(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int a2, int a3);
void TypeIcon_LoadChar(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NNS_G2D_VRAM_TYPE vramType, int type, int a4); // load type icon graphics?
void TypeIcon_UnloadChar(SpriteManager *spriteManager, int a1);
void TypeIcon_UnloadPltt(SpriteManager *spriteManager, int a1);
void TypeIcon_UnloadCellAnim(SpriteManager *spriteManager, int a1, int a2);
ManagedSprite *TypeIcon_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int type, ManagedSpriteTemplate *spriteTemplate);
void thunk_ManagedSprite_DeleteAndFreeResources(ManagedSprite *managedSprite);
void MoveCategoryIcon_DeleteSprite(ManagedSprite *managedSprite);
void MoveCategoryIcon_UnloadChar(SpriteManager *spriteManager, int a1);

#endif // POKEHEARTGOLD_TYPE_ICON_RESOURCES_H
