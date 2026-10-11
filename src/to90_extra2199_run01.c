#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 sSearchHeightHitboxes[],sSearchHeightDpadBoxes[],sSearchHeightGridCallbacks[];
void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,u8,int);
void PokedexApp_SetSpriteVisible(void*,int,int),PokedexApp_UpdateSearchHeightCursor(void*,int);void PokedexApp_CreateSearchHeightGrid(void*);
void PokedexApp_CreateSearchHeightGrid(void*p){*(void**)((u8*)p+0x1864)=GridInputHandler_Create(sSearchHeightHitboxes,sSearchHeightDpadBoxes,sSearchHeightGridCallbacks,p,1,0,37);PokedexApp_SetSpriteVisible(p,0,1);PokedexApp_UpdateSearchHeightCursor(p,0);}
