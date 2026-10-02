#pragma force_active on
#include "global.h"
#pragma force_active on
void GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int);
void GfGfxLoader_GXLoadPal(int,int,int,int,int,int);void ScheduleSetBgPosText(void *,int,int,int);void LoadFontPal0(int,int,int);
void ov96_0220EE8C(u8 *);
#define P(o) (*(void **)(w+(o)))
#define W(o) (*(int *)(w+(o)))
void ov96_0220EE8C(u8 *w){
GfGfxLoader_LoadCharData(237,4,P(8),0,0,0,1,W(0));
GfGfxLoader_LoadCharData(237,2,P(8),1,0,0,1,W(0));
GfGfxLoader_LoadScrnData(237,5,P(8),0,0,0,1,W(0));
GfGfxLoader_LoadScrnData(237,1,P(8),1,0,0,1,W(0));
GfGfxLoader_GXLoadPal(237,0,0,0,0,W(0));ScheduleSetBgPosText(P(8),1,4,8);LoadFontPal0(0,480,W(0));
GfGfxLoader_LoadCharData(237,4,P(8),4,0,0,1,W(0));
GfGfxLoader_LoadCharData(237,11,P(8),5,0,0,1,W(0));
GfGfxLoader_LoadCharData(237,11,P(8),6,0,0,1,W(0));
GfGfxLoader_LoadScrnData(237,5,P(8),4,0,0,1,W(0));
GfGfxLoader_LoadScrnData(237,12,P(8),5,0,0,1,W(0));
GfGfxLoader_LoadScrnData(237,13,P(8),6,0,0,1,W(0));
GfGfxLoader_GXLoadPal(237,3,4,0,0,W(0));}
