#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct W{u8 pad[0x1c];void*msg,*fmt,*str,*tmp;u8 rest[0x94];Window win;}W;extern void ReadMsgDataIntoString(void*,u32,void*),StringExpandPlaceholders(void*,void*,void*),BufferIntegerAsString(void*,u32,s32,u32,u32,u32);extern u8 AddTextPrinterParameterizedWithColor(Window*,u8,void*,u32,u32,u32,u32,void*);u8 ov81_0224080C(W*,Window*,u32,u32,u32,u32,u8,u8,u8,u8),ov81_0224086C(W*,u32);void ov81_022408A0(W*,u32,s32);
u8 ov81_0224080C(W*w,Window*win,u32 msg,u32 x,u32 y,u32 speed,u8 fg,u8 shadow,u8 bg,u8 font){FillWindowPixelBuffer(win,bg);ReadMsgDataIntoString(w->msg,msg,w->tmp);StringExpandPlaceholders(w->fmt,w->str,w->tmp);return AddTextPrinterParameterizedWithColor(win,font,w->str,x,y,speed,(((u32)fg<<24)>>8)|(((u32)shadow<<24)>>16)|bg,0);}
u8 ov81_0224086C(W*w,u32 msg){u8 r=ov81_0224080C(w,&w->win,msg,1,1,255,1,2,15,1);ScheduleWindowCopyToVram(&w->win);return r;}
void ov81_022408A0(W*w,u32 idx,s32 value){BufferIntegerAsString(w->fmt,idx,value,1,0,1);}
