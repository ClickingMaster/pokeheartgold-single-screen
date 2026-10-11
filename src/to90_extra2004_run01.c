#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int),SpriteSystem_LoadPaletteBufferFromOpenNarc(void*,int,void*,void*,void*,int,int,int,int,int),SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int),SpriteSystem_LoadAnimResObjFromOpenNarc(void*,void*,void*,int,int,int);void PokedexApp_LoadTypeIconResources(void*);
void PokedexApp_LoadTypeIconResources(void*w){SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),36,1,2,50579);SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),36,1,2,50580);SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),36,1,2,50581);SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),36,1,2,50582);SpriteSystem_LoadPaletteBufferFromOpenNarc(P(w,0x850),3,P(w,0x668),P(w,0x66c),P(w,0x854),35,0,4,2,50520);SpriteSystem_LoadCellResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),33,1,50517);SpriteSystem_LoadAnimResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),34,1,50517);}
