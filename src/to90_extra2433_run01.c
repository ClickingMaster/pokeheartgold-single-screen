#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31683Parent {u8 pad[0x24];void *options;} W31683Parent;
typedef struct W31683 {W31683Parent *parent;void *bg;} W31683;
extern void *NARC_New(int,int);
extern void NARC_Delete(void *),GfGfxLoader_GXLoadPalFromOpenNarc(void *,int,int,int,int,int),LoadFontPal1(int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),LoadUserFrameGfx1(void *,int,int,int,int,int),GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int),ov70_02239CF8(W31683 *);
extern int Options_GetFrame(void *);
void ov70_02239FA4(W31683 *);
void ov70_02239FA4(W31683 *ctx){void *bg=ctx->bg;void *narc=NARC_New(100,61);GfGfxLoader_GXLoadPalFromOpenNarc(narc,8,0,0,96,61);GfGfxLoader_GXLoadPalFromOpenNarc(narc,5,4,0,256,61);LoadFontPal1(0,416,61);LoadUserFrameGfx2(bg,0,1,14,(u8)Options_GetFrame(ctx->parent->options),61);LoadUserFrameGfx1(bg,0,31,11,0,61);GfGfxLoader_LoadCharDataFromOpenNarc(narc,20,bg,1,0,2560,1,61);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,28,bg,1,0,1536,1,61);LoadFontPal1(4,32,61);ov70_02239CF8(ctx);NARC_Delete(narc);}
