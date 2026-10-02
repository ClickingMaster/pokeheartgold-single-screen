#pragma force_active on
#include "global.h"
#pragma force_active on
int CalculateHpBarColor(u16,u16,int),CalculateHpBarPixelsLength(u16,u16,int);void BG_LoadPlttData(int,void *,int,u16),FillWindowPixelRect(void *,int,int,int,u16,int),ov05_0221E7B8(u8 *,int);
void ov05_0221E7B8(u8 *s,int index){u8 *window=s+16+(index*4+3)*16;u16 *max=(u16 *)(s+540),*hp=(u16 *)(s+538);int offset=index*24;int color=CalculateHpBarColor(*(u16 *)((u8 *)hp+offset),*(u16 *)((u8 *)max+offset),48);switch(color){case 3:case 4:BG_LoadPlttData(1,s+1270,4,(u16)((window[9]*16+9)*2));break;case 2:BG_LoadPlttData(1,s+1302,4,(u16)((window[9]*16+9)*2));break;case 1:BG_LoadPlttData(1,s+1334,4,(u16)((window[9]*16+9)*2));break;}int pixels=CalculateHpBarPixelsLength(*(u16 *)((u8 *)hp+offset),*(u16 *)((u8 *)max+offset),48);FillWindowPixelRect(window,10,0,2,(u16)pixels,1);FillWindowPixelRect(window,9,0,3,(u16)pixels,2);FillWindowPixelRect(window,10,0,5,(u16)pixels,1);}
