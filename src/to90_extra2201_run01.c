#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 sSearchWeightHitboxes[],sSearchWeightDpadBoxes[],sSearchWeightGridCallbacks[];
void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,u8,int);
void PokedexApp_SetSpriteVisible(void*,int,int),PokedexApp_UpdateSearchWeightCursor(void*,int);void PokedexApp_CreateSearchWeightGrid(void*);
void PokedexApp_CreateSearchWeightGrid(void*p){*(void**)((u8*)p+0x1864)=GridInputHandler_Create(sSearchWeightHitboxes,sSearchWeightDpadBoxes,sSearchWeightGridCallbacks,p,1,0,37);PokedexApp_SetSpriteVisible(p,0,1);PokedexApp_UpdateSearchWeightCursor(p,0);}
