#pragma force_active on
#include "global.h"
typedef struct{void*x;u8 gap[0x64];u8 win[16];u8 gap2[0x88];void*str;u8 gap3[0xb5];u8 printer;}Work;extern void FillWindowPixelBuffer(void*,int),DrawFrameAndWindow2(void*,int,int,int),ov68_021E6A2C(Work*,int),TextFlags_SetCanABSpeedUpPrint(int),ov68_021E6C8C(void*);extern int Options_GetTextFrameDelay(void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,void(*)(void*));
void ov68_021E6C14(Work*w,int id);
void ov68_021E6C14(Work*w,int id){FillWindowPixelBuffer(w->win,15);DrawFrameAndWindow2(w->win,0,61,13);ov68_021E6A2C(w,id);TextFlags_SetCanABSpeedUpPrint(1);w->printer=AddTextPrinterParameterized(w->win,1,w->str,0,0,Options_GetTextFrameDelay(*(void**)((u8*)w->x+8)),ov68_021E6C8C);}
