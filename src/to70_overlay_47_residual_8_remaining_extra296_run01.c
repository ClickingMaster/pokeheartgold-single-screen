#pragma force_active on
#include "global.h"
#pragma force_active on
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int),FillWindowPixelBuffer(void *,int),CopyWindowToVram(void *),ov47_0225999C(void *,int,int,int);
void ov47_02259C3C(u8 *,int,u8 *,int);
void ov47_02259C3C(u8 *win,int msg,u8 *w,int heap){int i;u8 *p;GfGfxLoader_LoadScrnDataFromOpenNarc(*(void **)(w+320),203,*(void **)w,0,0,0,0,heap);p=win;for(i=0;i<6;i++,p+=16){FillWindowPixelBuffer(p,0);CopyWindowToVram(p);}ov47_0225999C(win,msg,119,0x50600);}
