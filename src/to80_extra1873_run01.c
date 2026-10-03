#pragma force_active on
#include "global.h"
#pragma force_active on
extern void FillWindowPixelBuffer(void *,int);extern u8 GetWindowWidth(void *);extern u32 FontID_String_GetWidth(int,void *,int);extern void AddTextPrinterParameterizedWithColor(void *,int,void *,u32,int,int,u32,void *);extern void ScheduleWindowCopyToVram(void *);
void ov15_021FF758(void *,void **,u8);
void ov15_021FF758(void *w,void **strings,u8 index){FillWindowPixelBuffer(w,0);if(index!=255){void **slot=&strings[index];u32 width;width=GetWindowWidth(w);u32 text=FontID_String_GetWidth(0,*slot,0);AddTextPrinterParameterizedWithColor(w,0,*slot,((width*8)-text)/2,0,255,0xf0e00,0);}ScheduleWindowCopyToVram(w);}
