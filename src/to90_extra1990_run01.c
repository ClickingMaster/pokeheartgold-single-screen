#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void*NARC_New(int,int),*GfGfxLoader_GetScrnDataFromOpenNarc(void*,int,int,void*,int);extern void NARC_Delete(void*),GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int),GfGfxLoader_GXLoadPalFromOpenNarc(void*,int,int,int,int,int),LoadFontPal1(int,int,int),LoadUserFrameGfx2(void*,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void*,int);void ov108_021E9A60(void*);
void ov108_021E9A60(void*w){void*n=NARC_New(225,W(w,0));GfGfxLoader_LoadCharDataFromOpenNarc(n,1,P(w,0x438),3,0,0,0,W(w,0));GfGfxLoader_LoadCharDataFromOpenNarc(n,5,P(w,0x438),7,0,0,0,W(w,0));GfGfxLoader_GXLoadPalFromOpenNarc(n,0,0,0,0,W(w,0));GfGfxLoader_GXLoadPalFromOpenNarc(n,4,4,0,0,W(w,0));LoadFontPal1(0,416,W(w,0));LoadFontPal1(4,416,W(w,0));GfGfxLoader_LoadScrnDataFromOpenNarc(n,2,P(w,0x438),3,0,0,0,W(w,0));GfGfxLoader_LoadScrnDataFromOpenNarc(n,3,P(w,0x438),2,0,0,0,W(w,0));GfGfxLoader_LoadScrnDataFromOpenNarc(n,6,P(w,0x438),7,0,0,0,W(w,0));P(w,0x518)=GfGfxLoader_GetScrnDataFromOpenNarc(n,8,0,(u8*)w+0x51c,W(w,0));P(w,0x520)=GfGfxLoader_GetScrnDataFromOpenNarc(n,6,0,(u8*)w+0x524,W(w,0));P(w,0x528)=GfGfxLoader_GetScrnDataFromOpenNarc(n,7,0,(u8*)w+0x52c,W(w,0));NARC_Delete(n);LoadUserFrameGfx2(P(w,0x438),4,994,12,B(w,25),W(w,0));LoadFontPal1(0,416,W(w,0));ScheduleBgTilemapBufferTransfer(P(w,0x438),7);ScheduleBgTilemapBufferTransfer(P(w,0x438),3);G2x_SetBlendAlpha_(0x4000050,1,30,28,4);}
