#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 sSearchAreaHitboxes[],sSearchAreaDpadBoxes[],sSearchAreaGridCallbacks[];
void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,u8,int);
void PokedexApp_SetSpriteVisible(void*,int,int),PokedexApp_UpdateSearchAreaCursor(void*,int);void PokedexApp_CreateSearchAreaGrid(void*);
void PokedexApp_CreateSearchAreaGrid(void*p){*(void**)((u8*)p+0x1864)=GridInputHandler_Create(sSearchAreaHitboxes,sSearchAreaDpadBoxes,sSearchAreaGridCallbacks,p,1,0,37);PokedexApp_SetSpriteVisible(p,0,1);PokedexApp_UpdateSearchAreaCursor(p,0);}
