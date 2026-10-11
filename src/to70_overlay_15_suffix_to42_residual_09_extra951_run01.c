#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),GF_CreateVramTransferManager(int,int),BagApp_CreateSpriteSystem(void *),BagApp_LoadSpriteResources(void *),BagApp_CreateSprites(void *),G2dRenderer_SetSubSurfaceCoords(void *,int,int);extern void *SpriteSystem_GetRenderer(void *);void BagApp_InitSprites(void *);
void BagApp_InitSprites(void *w){GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);GF_CreateVramTransferManager(32,6);BagApp_CreateSpriteSystem(w);BagApp_LoadSpriteResources(w);BagApp_CreateSprites(w);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*(void**)((u8*)w+0x248)),0,0x100000);}
