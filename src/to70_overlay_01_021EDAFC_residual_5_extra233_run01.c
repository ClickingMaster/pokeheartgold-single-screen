#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[8];u8 window[16];u8 gap[0x8c-24];void*messages;void*format;};void*String_New(int,int);void ReadMsgDataIntoString(void*,int,void*);void StringExpandPlaceholders(void*,void*,void*);void AddTextPrinterParameterized(void*,int,void*,int,int,int,int);void String_Delete(void*);void ov01_021EE754(struct S*,u16,u8,u8);
void ov01_021EE754(struct S*s,u16 message,u8 x,u8 y){void*a=String_New(90,4);void*b=String_New(90,4);ReadMsgDataIntoString(s->messages,message,a);StringExpandPlaceholders(s->format,b,a);AddTextPrinterParameterized(s->window,0,b,x,y,255,0);String_Delete(a);String_Delete(b);}
