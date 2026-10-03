#pragma force_active on
#include "global.h"
typedef struct {int heap;u8 pad[0x29fc-4];void*bg;}State;
extern void GfGfxLoader_GXLoadPal(int,int,int,int,u32,int),GfGfxLoader_LoadCharData(int,int,void*,int,u32,u32,int,int),GfGfxLoader_LoadScrnData(int,int,void*,int,u32,u32,int,int),ov74_0222F2D4(State*,int,int,int),BgTilemapRectChangePalette(void*,int,int,int,int,int,int),BgCommitTilemapBufferToVram(void*,int);
void ov74_0222F314(State*,int);
void ov74_0222F314(State*s,int kind){GfGfxLoader_GXLoadPal(113,7,0,0,256,s->heap);GfGfxLoader_LoadCharData(113,10,s->bg,1,0,0x3000,1,s->heap);switch(kind){case 0:GfGfxLoader_LoadScrnData(113,8,s->bg,1,0,0x600,1,s->heap);break;case 1:GfGfxLoader_LoadScrnData(113,9,s->bg,1,0,0x600,1,s->heap);break;}GfGfxLoader_GXLoadPal(113,0,0,0x160,32,s->heap);GfGfxLoader_LoadCharData(113,1,s->bg,3,0,512,1,s->heap);ov74_0222F2D4(s,2,3,0x600);BgTilemapRectChangePalette(s->bg,3,0,0,32,24,11);BgCommitTilemapBufferToVram(s->bg,3);}
