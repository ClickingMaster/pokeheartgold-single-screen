#pragma force_active on
#include "global.h"
typedef struct {u8 a[0xc8];u8 window[16];void*str;u32 printer;u32 speed;} Work;
typedef struct {u8 a[0x50];void*format;void*msg;} Other;
extern void FillWindowPixelBuffer(void*,int);extern void*String_New(int,int);extern void ReadMsgDataIntoString(void*,int,void*);extern void StringExpandPlaceholders(void*,void*,void*);extern void TextFlags_SetCanTouchSpeedUpPrint(int);extern u32 AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);extern void DrawFrameAndWindow2(void*,int,int,int);extern void ScheduleWindowCopyToVram(void*);extern void String_Delete(void*);extern int TextPrinterCheckActive(u8);extern int TextFlags_GetIsTouchSpeedingUpPrint(void);

void ov43_0222C550(Work*w,Other*o,int message,int heap);
int ov43_0222C5D8(Work*w,int*out);

void ov43_0222C550(Work*w,Other*o,int message,int heap){void*s;FillWindowPixelBuffer(w->window,15);s=String_New(128,heap);ReadMsgDataIntoString(o->msg,message,s);StringExpandPlaceholders(o->format,w->str,s);TextFlags_SetCanTouchSpeedUpPrint(1);w->printer=AddTextPrinterParameterizedWithColor(w->window,1,w->str,0,0,w->speed,0x10200,0);DrawFrameAndWindow2(w->window,1,70,12);ScheduleWindowCopyToVram(w->window);String_Delete(s);}
int ov43_0222C5D8(Work*w,int*out){if(TextPrinterCheckActive((u8)w->printer)==0){TextFlags_SetCanTouchSpeedUpPrint(0);*out=TextFlags_GetIsTouchSpeedingUpPrint();return 1;}return 0;}
