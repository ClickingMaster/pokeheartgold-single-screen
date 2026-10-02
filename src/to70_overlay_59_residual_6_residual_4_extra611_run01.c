#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;u8 pad[0x46-4];u8 frame;u8 pad2[0x54-0x47];void *bg;u8 pad3[0x290-0x58];void *narc;} State;
extern void *NARC_New(int,int);extern void GfGfxLoader_GXLoadPalFromOpenNarc(void *,int,int,int,int,int),GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),LoadFontPal1(int,int,int),G2x_SetBlendAlpha_(u32,int,int,int,int);
void ov59_02239604(State *);
void ov59_02239604(State *s){void *n=NARC_New(220,s->heap);GfGfxLoader_GXLoadPalFromOpenNarc(n,3,0,0,0,s->heap);GfGfxLoader_LoadCharDataFromOpenNarc(n,4,s->bg,3,0,0,0,s->heap);GfGfxLoader_LoadScrnDataFromOpenNarc(n,5,s->bg,3,0,0,0,s->heap);GfGfxLoader_GXLoadPalFromOpenNarc(n,0,4,0,0,s->heap);GfGfxLoader_LoadCharDataFromOpenNarc(n,1,s->bg,7,0,0,0,s->heap);GfGfxLoader_LoadScrnDataFromOpenNarc(n,2,s->bg,7,0,0,0,s->heap);s->narc=n;LoadUserFrameGfx2(s->bg,0,1,13,s->frame,s->heap);LoadFontPal1(0,384,s->heap);LoadFontPal1(4,384,s->heap);G2x_SetBlendAlpha_(0x4000050,0,8,6,26);}
