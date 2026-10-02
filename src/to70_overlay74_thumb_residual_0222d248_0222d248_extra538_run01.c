#pragma force_active on
#include "global.h"
extern void GfGfxLoader_GXLoadPal(int,int,int,int,int,int),GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int),BgTilemapRectChangePalette(void*,int,int,int,int,int,int),BgCommitTilemapBufferToVram(void*,int);
void ov74_0222D248(void*w);
void ov74_0222D248(void*w){GfGfxLoader_GXLoadPal(113,15,0,480,32,85);GfGfxLoader_GXLoadPal(113,20,0,384,32,85);GfGfxLoader_LoadCharData(113,21,w,1,0,3072,1,85);GfGfxLoader_LoadScrnData(113,22,w,1,0,1536,1,85);BgTilemapRectChangePalette(w,1,0,0,32,24,12);BgCommitTilemapBufferToVram(w,1);}
