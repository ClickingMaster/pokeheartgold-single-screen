#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x20];void*msg,*fmt,*str,*tmp;}Work;
extern void FillWindowPixelBuffer(void*,u8),ReadMsgDataIntoString(void*,int,void*),StringExpandPlaceholders(void*,void*,void*),ScheduleWindowCopyToVram(void*);extern u8 AddTextPrinterParameterizedWithColor(void*,u8,void*,int,int,int,u32,void*);
u8 ov83_022447E0(Work*,void*,int,int,int,int,u8,u8,u8,u8);u8 ov83_0224484C(Work*,void*,int,int,int,int,u8,u8,u8,u8);
u8 ov83_022447E0(Work*w,void*win,int msg,int x,int y,int speed,u8 fg,u8 shadow,u8 bg,u8 font){u8 r;FillWindowPixelBuffer(win,bg);ReadMsgDataIntoString(w->msg,msg,w->tmp);StringExpandPlaceholders(w->fmt,w->str,w->tmp);r=AddTextPrinterParameterizedWithColor(win,font,w->str,x,y,speed,(((u32)fg<<24)>>8)|(((u32)shadow<<24)>>16)|bg,NULL);ScheduleWindowCopyToVram(win);return r;}
u8 ov83_0224484C(Work*w,void*win,int msg,int x,int y,int speed,u8 fg,u8 shadow,u8 bg,u8 font){u8 r;ReadMsgDataIntoString(w->msg,msg,w->tmp);StringExpandPlaceholders(w->fmt,w->str,w->tmp);r=AddTextPrinterParameterizedWithColor(win,font,w->str,x,y,speed,(((u32)fg<<24)>>8)|(((u32)shadow<<24)>>16)|bg,NULL);ScheduleWindowCopyToVram(win);return r;}
