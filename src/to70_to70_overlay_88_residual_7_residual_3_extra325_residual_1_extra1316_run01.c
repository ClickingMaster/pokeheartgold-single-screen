#pragma force_active on
#include "global.h"
typedef struct {u8 pad[12];u8 *data;} Palette;
typedef struct {void *bg;u8 pad[316];void *narc;} Context;
typedef struct {u8 sprites[96];u8 windows[3][16];void *string,*raw;Palette *pltt;} Work;
extern u8 _02259904[];
extern void ov88_02259508(void*,Context*,int,int);
extern void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),FillWindowPixelBuffer(void*,u8),ScheduleWindowCopyToVram(void*);
extern void *String_New(int,int),*GfGfxLoader_GetPlttDataFromOpenNarc(void*,int,Palette**,int);
extern void BG_LoadPlttData(u32,const void*,u16,u16);
void ov88_022592EC(Work*,Context*,int);
void ov88_022592EC(Work *w,Context *ctx,int heap){int i=0;u8 *sprite=(u8*)w;int base=768;u8 *window=w->windows[0];u8 *height=_02259904;
for(;i<3;i++){ov88_02259508(sprite,ctx,i,heap);AddWindowParameterized(ctx->bg,window,3,1,*height,30,2,i+7,base);FillWindowPixelBuffer(window,0);ScheduleWindowCopyToVram(window);sprite+=32;base+=60;height++;window+=16;}
w->string=String_New(256,heap);w->raw=GfGfxLoader_GetPlttDataFromOpenNarc(ctx->narc,0,&w->pltt,heap);BG_LoadPlttData(0,w->pltt->data+238,4,250);BG_LoadPlttData(0,w->pltt->data+238,4,282);BG_LoadPlttData(0,w->pltt->data+238,4,314);}
