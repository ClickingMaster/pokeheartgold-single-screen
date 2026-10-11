#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern const u8 sSearchOrderHitboxes[],sSearchOrderDpadBoxes[],sSearchOrderGridCallbacks[];void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,int,int);void PokedexApp_SetSpriteVisible(void*,int,int);void PokedexApp_UpdateSearchOrderCursor(void*,int);
void PokedexApp_CreateSearchOrderGrid(u8*w);
void PokedexApp_CreateSearchOrderGrid(u8*w){P(w,0x1864)=GridInputHandler_Create(sSearchOrderHitboxes,sSearchOrderDpadBoxes,sSearchOrderGridCallbacks,w,1,0,37);PokedexApp_SetSpriteVisible(w,0,1);PokedexApp_UpdateSearchOrderCursor(w,0);}
