#pragma force_active on
#include "global.h"
extern void FillWindowPixelBuffer(void *,int);
extern void *NewString_ReadMsgData(void *,int);
extern void BufferIntegerAsString(void *,int,int,int,int,int);
extern void StringExpandPlaceholders(void *,void *,void *);
extern int FontID_String_GetWidth(int,void *,int);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int);
extern void ScheduleWindowCopyToVram(void *);
extern void String_Delete(void *);
typedef struct {u8 p[0x224];u8 window[0xcc];void *msg,*format;u8 p2[0x5e4-0x2f8];void *string;u8 p3[0x680-0x5e8];s16 count;u16 pad;int price;} Work;
void BagApp_PrintSellPrice(Work *);
void BagApp_PrintSellPrice(Work *w){void *s;u32 offset=0x224;FillWindowPixelBuffer((void*)((u32)w+offset),0);s=NewString_ReadMsgData(*(void**)((u32)w+offset+0xcc),83);BufferIntegerAsString(*(void**)((u32)w+offset+0xd0),0,w->price*w->count,6,1,1);StringExpandPlaceholders(*(void**)((u32)w+offset+0xd0),w->string,s);FontID_String_GetWidth(0,w->string,0);AddTextPrinterParameterizedWithColor((void*)((u32)w+offset),0,w->string,0,4,255,0x10200,0);ScheduleWindowCopyToVram((void*)((u32)w+offset));String_Delete(s);}
