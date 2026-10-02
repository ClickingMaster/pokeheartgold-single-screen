#pragma force_active on
#include "global.h"
extern void*String_New(int,int);extern void ReadMsgDataIntoString(void*,int,void*),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),FillWindowPixelBuffer(void*,int),DrawFrameAndWindow2(void*,int,int,int);extern int Options_GetTextFrameDelay(void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,int);typedef struct{u8 p[12];void*options,*fmt,*msg;u8 gap[28];void*str;int printer;u8 gap2[0x298];u8 window[16];}Work;
void ov52_021E9260(u8*w);
void ov52_021E927C(Work*w,int id);
static inline void copyBytes(u8*d,u8*s,u32 n){do{*d++=*s++;}while(--n);}
void ov52_021E927C(Work*w,int id){void*s=String_New(80,39);ReadMsgDataIntoString(w->msg,id,s);StringExpandPlaceholders(w->fmt,w->str,s);String_Delete(s);FillWindowPixelBuffer(w->window,15);DrawFrameAndWindow2(w->window,0,1,10);w->printer=AddTextPrinterParameterized(w->window,1,w->str,0,0,Options_GetTextFrameDelay(w->options),0);}
