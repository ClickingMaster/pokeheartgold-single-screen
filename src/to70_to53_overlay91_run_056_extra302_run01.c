#pragma force_active on
#include "global.h"
#pragma force_active on
void GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *,int,int,int,int,int),BgTilemapRectChangePalette(void *,int,int,int,int,int,u8),BgCommitTilemapBufferToVram(void *,int);
extern const u8 ov91_02261BEC[];void ov91_0225FDE0(void **,void *,int,int);
void ov91_0225FDE0(void **w,void *narc,int n,int heap){int pal;
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,12,w[0],4,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,14,w[0],7,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,15,w[0],6,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,16,w[0],5,0,0,0,heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,17,w[0],4,0,0,0,heap);
 GfGfxLoader_GXLoadPalFromOpenNarc(narc,13,4,0,384,heap);BgTilemapRectChangePalette(w[0],7,0,0,32,32,ov91_02261BEC[n]);pal=n*2;BgTilemapRectChangePalette(w[0],6,0,0,32,32,(u8)(pal+2));pal=n*2+1;BgTilemapRectChangePalette(w[0],5,0,0,32,32,(u8)pal);BgTilemapRectChangePalette(w[0],4,0,0,32,32,(u8)pal);
 BgCommitTilemapBufferToVram(w[0],4);BgCommitTilemapBufferToVram(w[0],5);BgCommitTilemapBufferToVram(w[0],6);BgCommitTilemapBufferToVram(w[0],7);
}
