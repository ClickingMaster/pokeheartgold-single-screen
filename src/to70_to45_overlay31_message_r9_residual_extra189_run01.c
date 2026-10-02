#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Data{u8 pad0[0x280];u8 printer,pad281[2],code;u16 value;}Data;
typedef struct Work{u8 pad0[4];void*bg;u8 pad8[12];Data*data;u8 pad18[0x12c];u8 window[16];void*format,*msg;u8 pad15c[8];void*options;u8 pad168[32];void*string;}Work;
extern void FillWindowPixelBuffer(void*,int),ov31_0225E4BC(u8,void*,u16,int),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),LoadUserFrameGfx2(void*,int,int,int,u8,int),DrawFrameAndWindow2(void*,int,int,int);extern void*NewString_ReadMsgData(void*,int);extern int Options_GetFrame(void*),Options_GetTextFrameDelay(void*);extern u8 AddTextPrinterParameterized(void*,int,void*,int,int,int,void*);void ov31_0225E54C(Work*);
void ov31_0225E54C(Work*w){FillWindowPixelBuffer(w->window,15);ov31_0225E4BC(w->data->code,w->format,w->data->value,0);void*text=NewString_ReadMsgData(w->msg,12);StringExpandPlaceholders(w->format,w->string,text);String_Delete(text);LoadUserFrameGfx2(w->bg,4,437,5,(u8)Options_GetFrame(w->options),4);DrawFrameAndWindow2(w->window,1,437,5);int speed=Options_GetTextFrameDelay(w->options);w->data->printer=AddTextPrinterParameterized(w->window,1,w->string,0,0,speed,0);}
