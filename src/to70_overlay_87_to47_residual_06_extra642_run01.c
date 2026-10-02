#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x38];void *messages,*format,*string,*scratch;} State;
extern void FillWindowPixelBuffer(void *,u8),ReadMsgDataIntoString(void *,int,void *),StringExpandPlaceholders(void *,void *,void *);
extern u8 AddTextPrinterParameterizedWithColor(void *,u8,void *,u32,u32,u32,u32,void *);extern u32 FontID_String_GetWidth(u8,void *,u32);
void ov87_021E6F38(State *,void *,int,u32,u32,u32,u8,u8,u8,u8);void ov87_021E6F98(State *,void *,int,u32,u32,u32,u8,u8,u8,u8);
void ov87_021E6F38(State *s,void *w,int msg,u32 x,u32 y,u32 speed,u8 fg,u8 shadow,u8 bg,u8 font){FillWindowPixelBuffer(w,bg);ReadMsgDataIntoString(s->messages,msg,s->scratch);StringExpandPlaceholders(s->format,s->string,s->scratch);AddTextPrinterParameterizedWithColor(w,font,s->string,x,y,speed,((fg&255)<<16)|((shadow&255)<<8)|bg,0);}
void ov87_021E6F98(State *s,void *w,int msg,u32 x,u32 y,u32 speed,u8 fg,u8 shadow,u8 bg,u8 font){FillWindowPixelBuffer(w,bg);ReadMsgDataIntoString(s->messages,msg,s->scratch);StringExpandPlaceholders(s->format,s->string,s->scratch);x-=(FontID_String_GetWidth(font,s->string,0)+1)/2;AddTextPrinterParameterizedWithColor(w,font,s->string,x,y,speed,((fg&255)<<16)|((shadow&255)<<8)|bg,0);}
