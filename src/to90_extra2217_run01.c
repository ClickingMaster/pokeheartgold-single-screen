#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
void ov71_022472C4(void*,int,int,int);void ov71_02247320(void*,void*,NNSG2dImageProxy*,NNSG2dImagePaletteProxy*,int);void*ov71_02247340(void*,void*,int,int,int,int);
u32 GfGfxLoader_LoadImageMapping(int,int,int,int,u32,int,u32,int,NNSG2dImageProxy*);void GfGfxLoader_PartiallyLoadPalette(int,int,int,u32,int,NNSG2dImagePaletteProxy*);
void SetMTRNGSeed(u32);u32 MTRandom(void);void Sprite_SetAnimCtrlSeq(void*,int);void Sprite_SetDrawFlag(void*,int);
void ov71_02248A08(void*);
void ov71_02248A08(void*p){NNSG2dImagePaletteProxy pal;NNSG2dImageProxy img;u32 header[9];int i,x,y;u8*cursor;ov71_022472C4((u8*)p+12,0x59,11,12);NNS_G2dInitImagePaletteProxy(&pal);NNS_G2dInitImageProxy(&img);GfGfxLoader_LoadImageMapping(0x59,13,1,0,0,1,0,0x39,&img);GfGfxLoader_LoadImageMapping(0x59,13,1,0,0,2,0,0x39,&img);GfGfxLoader_PartiallyLoadPalette(0x59,14,1,0,0x39,&pal);GfGfxLoader_PartiallyLoadPalette(0x59,14,2,0,0x39,&pal);ov71_02247320(header,(u8*)p+12,&img,&pal,1);SetMTRNGSeed(0x35947d1);i=0;cursor=p;do{x=MTRandom()%232+12;y=(int)(MTRandom()%452)-28;P(cursor,0x1c)=ov71_02247340(P(p,0),header,x,y,0,1);y+=56;P(cursor,0x20)=ov71_02247340(P(p,0),header,x,y,0,1);Sprite_SetAnimCtrlSeq(P(cursor,0x1c),0);Sprite_SetAnimCtrlSeq(P(cursor,0x20),1);Sprite_SetDrawFlag(P(cursor,0x1c),0);Sprite_SetDrawFlag(P(cursor,0x20),0);cursor+=8;}while(++i<20);}
