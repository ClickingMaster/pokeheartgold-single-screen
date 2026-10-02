#pragma force_active on
#include "global.h"
#pragma force_active on
struct Inner{u8 pad[8];void*options;};struct S{struct Inner*inner;void*bg;u8 gap[0x1c0-8];void*data;void*screen;};
void BgConfig_LoadAssetFromOpenNarc(void*,int,void*,int,int,int,int,int,int);void*GfGfxLoader_GetScrnDataFromOpenNarc(void*,int,int,void*,int);void LoadFontPal1(int,int,int);void LoadFontPal0(int,int,int);int Options_GetFrame(void*);void LoadUserFrameGfx2(void*,int,int,int,int,int);void ov68_021E5D24(struct S*,void*);
void ov68_021E5D24(struct S*s,void*n){
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,1,3,0,0,0);
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,4,3,1,2048,0);
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,0,3,2,0,0);
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,1,7,0,0,0);
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,2,7,1,2048,0);
BgConfig_LoadAssetFromOpenNarc(s->bg,66,n,110,0,7,2,0,0);
s->data=GfGfxLoader_GetScrnDataFromOpenNarc(n,3,0,&s->screen,66);
LoadFontPal1(4,448,66);LoadFontPal0(0,480,66);LoadFontPal0(4,480,66);
LoadUserFrameGfx2(s->bg,4,61,13,(u8)Options_GetFrame(s->inner->options),66);
}
