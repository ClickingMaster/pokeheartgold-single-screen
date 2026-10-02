#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[36];void *options;} Options;
typedef struct {u8 pad[12];Options *config;int pad2;void *bg;} State;
extern void GfGfxLoader_GXLoadPalFromOpenNarc(void *,int,int,int,int,int),GfGfxLoader_GXLoadPal(int,int,int,int,int,int),LoadFontPal1(int,int,int),GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int),GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int),LoadUserFrameGfx2(void *,int,int,int,u8,int),LoadUserFrameGfx1(void *,int,int,int,int,int);extern int Options_GetFrame(void *);
void ov85_021E8E6C(State *,void *);
void ov85_021E8E6C(State *s,void *narc){void *bg=s->bg;GfGfxLoader_GXLoadPalFromOpenNarc(narc,0,0,0,512,102);GfGfxLoader_GXLoadPal(14,71,4,0,32,102);LoadFontPal1(0,416,102);LoadFontPal1(4,416,102);GfGfxLoader_LoadCharData(14,10,bg,6,0,0,1,102);GfGfxLoader_LoadScrnData(14,11,bg,6,0,0,1,102);GfGfxLoader_LoadCharDataFromOpenNarc(narc,2,bg,1,0,8192,1,102);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,3,bg,1,0,1536,1,102);LoadUserFrameGfx2(bg,0,1,10,Options_GetFrame(s->config->options),102);LoadUserFrameGfx1(bg,0,31,11,0,102);}
