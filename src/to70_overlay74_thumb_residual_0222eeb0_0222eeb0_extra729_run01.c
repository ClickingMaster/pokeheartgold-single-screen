#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x354];u32 day;}Card;
typedef struct{u8 pad[0x2a00];void*fmt;u8 pad2[0x1b0];Card*card;}Work;
extern BOOL RTC_ConvertDayToDate(RTCDate*,u32);
extern void BufferIntegerAsString(void*,int,int,int,int,int);
int ov74_0222EEB0(Work*);
int ov74_0222EEB0(Work*w){RTCDate date;RTC_ConvertDayToDate(&date,w->card->day);BufferIntegerAsString(w->fmt,0,date.year+2000,4,2,1);BufferIntegerAsString(w->fmt,1,date.month,2,2,1);BufferIntegerAsString(w->fmt,2,date.day,2,2,1);return 1;}
