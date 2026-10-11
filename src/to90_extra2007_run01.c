#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int),SpriteSystem_LoadPaletteBufferFromOpenNarc(void*,int,void*,void*,void*,int,int,int,int,int),SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int),SpriteSystem_LoadAnimResObjFromOpenNarc(void*,void*,void*,int,int,int);
extern int sub_02074490(void),sub_0207449C(void),sub_020744A8(void);
void PokedexApp_LoadAreaSpriteResources(void*);
#pragma force_active on
void PokedexApp_LoadAreaSpriteResources(void *w){
 SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),76,1,2,0xc550);
 SpriteSystem_LoadPaletteBufferFromOpenNarc(P(w,0x850),3,P(w,0x668),P(w,0x66c),P(w,0x858),sub_02074490(),0,3,2,0xc551);
 SpriteSystem_LoadCellResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x858),sub_0207449C(),0,0xc550);
 SpriteSystem_LoadAnimResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x858),sub_020744A8(),0,0xc550);
 SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),115,1,2,0xc59c);
 SpriteSystem_LoadPaletteBufferFromOpenNarc(P(w,0x850),3,P(w,0x668),P(w,0x66c),P(w,0x854),118,0,1,2,0xc55e);
 SpriteSystem_LoadCellResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),116,1,0xc55a);
 SpriteSystem_LoadAnimResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),117,1,0xc55a);
 SpriteSystem_LoadCharResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),119,1,2,0xc59a);
 SpriteSystem_LoadPaletteBufferFromOpenNarc(P(w,0x850),3,P(w,0x668),P(w,0x66c),P(w,0x854),122,0,2,2,0xc55c);
 SpriteSystem_LoadCellResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),120,1,0xc559);
 SpriteSystem_LoadAnimResObjFromOpenNarc(P(w,0x668),P(w,0x66c),P(w,0x854),121,1,0xc559);
}
