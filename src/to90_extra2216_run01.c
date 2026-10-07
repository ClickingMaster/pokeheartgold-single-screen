#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void*NARC_New(int,int);void NARC_Delete(void*);void GfGfxLoader_GXLoadPalFromOpenNarc(void*,int,int,int,int,int);void LoadFontPal1(int,int,int);int Options_GetFrame(void*);void LoadUserFrameGfx2(void*,int,int,int,u8,int);void LoadUserFrameGfx1(void*,int,int,int,int,int);void GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int);void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int);void ov70_02239CF8(void*);
void ov70_0223B8E0(u8*w);
void ov70_0223B8E0(u8*w){void*bg=P(w,4);void*narc=NARC_New(100,61);
 GfGfxLoader_GXLoadPalFromOpenNarc(narc,3,0,0,96,61);GfGfxLoader_GXLoadPalFromOpenNarc(narc,5,4,0,256,61);LoadFontPal1(0,416,61);LoadUserFrameGfx2(bg,0,1,14,(u8)Options_GetFrame(P(P(w,0),0x24)),61);LoadUserFrameGfx1(bg,0,31,11,0,61);
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,14,bg,1,0,2560,1,61);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,29,bg,1,0,1536,1,61);GfGfxLoader_LoadCharDataFromOpenNarc(narc,15,bg,2,0,1536,1,61);LoadFontPal1(4,32,61);ov70_02239CF8(w);NARC_Delete(narc);
}
