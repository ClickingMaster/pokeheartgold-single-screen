#pragma force_active on
#include "global.h"
#pragma force_active on

extern void*String_New(u32,u32);extern void String_Delete(void*),String16_FormatInteger(void*,int,u32,int,BOOL),FillWindowPixelBuffer(void*,u8),ScheduleWindowCopyToVram(void*);
extern u8 AddTextPrinterParameterizedWithColor(void*,u32,void*,u32,u32,u32,u32,void*);
void ov15_021FEDEC(u8*,u32);
void ov15_021FEDEC(u8*w,u32 digits){void*text;u32 divisor,value,i;if(*(s16*)(w+0x680)>=1000)GF_AssertFail();text=String_New(2,6);if(digits==2)divisor=10;else divisor=100;value=(u32)*(s16*)(w+0x680);for(i=0;i<digits;i++){u32 digit=value/divisor;void*win;String16_FormatInteger(text,(int)digit,1,0,TRUE);value-=digit*divisor;divisor/=10;win=w+0xb4+((i+17)<<4);FillWindowPixelBuffer(win,0);AddTextPrinterParameterizedWithColor(win,0,text,0,4,255,0x10200,0);ScheduleWindowCopyToVram(win);}String_Delete(text);}
