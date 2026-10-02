#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x64];u8 window[0xf4];void*msg;} Work;
extern void *NewString_ReadMsgData(void*,int);extern void ClearWindowTilemapAndCopyToVram(void*);extern void FillWindowPixelBuffer(void*,u8);extern u32 FontID_String_GetWidth(int,void*,int);extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);extern void ScheduleWindowCopyToVram(void*);extern void String_Delete(void*);

void ov31_0225D684(Work*w,int mode);

void ov31_0225D684(Work*w,int mode){void*s=NewString_ReadMsgData(w->msg,17);ClearWindowTilemapAndCopyToVram(w->window);FillWindowPixelBuffer(w->window,0);if(mode==0){u32 width=FontID_String_GetWidth(0,s,0);AddTextPrinterParameterizedWithColor(w->window,0,s,((48-width)>>1)+8,0,255,0xf0e00,0);}else AddTextPrinterParameterizedWithColor(w->window,0,s,5,0,255,0xf0e00,0);ScheduleWindowCopyToVram(w->window);String_Delete(s);}
