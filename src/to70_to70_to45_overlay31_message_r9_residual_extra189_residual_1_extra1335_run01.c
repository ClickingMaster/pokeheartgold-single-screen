#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x283];u8 type;u16 thing;s16 count;u8 p2[4];int price;} Input;
typedef struct{u8 p[4];void*bg;u8 p2[12];Input*input;u8 p3[0x12c];u8 window[16];void*fmt,*msg;u8 p4[8];void*options;u8 p5[0x1c];int printer;void*expanded;} Work;
void FillWindowPixelBuffer(void*,int);
void ov31_0225E51C(int,void*,int,int);
void*NewString_ReadMsgData(void*,int);
void BufferIntegerAsString(void*,int,int,int,int,int);
void StringExpandPlaceholders(void*,void*,void*);
void String_Delete(void*);
int Options_GetFrame(void*);
void LoadUserFrameGfx2(void*,int,int,int,u8,int);
void DrawFrameAndWindow2(void*,int,int,int);
int Options_GetTextFrameDelay(void*);
int AddTextPrinterParameterized(void*,int,void*,int,int,int,void*);
void ov31_0225E5FC(Work*);
void ov31_0225E5FC(Work*w){void*s;FillWindowPixelBuffer(w->window,15);
if((u8)(w->input->type+253)<=1){ov31_0225E51C(w->input->type,w->fmt,w->input->thing,0);s=NewString_ReadMsgData(w->msg,46);}else{BufferIntegerAsString(w->fmt,0,w->input->count,2,0,1);BufferIntegerAsString(w->fmt,1,w->input->price*w->input->count,6,0,1);s=NewString_ReadMsgData(w->msg,14);}
StringExpandPlaceholders(w->fmt,w->expanded,s);String_Delete(s);LoadUserFrameGfx2(w->bg,4,437,5,(u8)Options_GetFrame(w->options),4);DrawFrameAndWindow2(w->window,1,437,5);w->printer=AddTextPrinterParameterized(w->window,1,w->expanded,0,0,Options_GetTextFrameDelay(w->options),0);}
