#pragma force_active on
#include "global.h"
#include <nitro/rtc.h>
typedef struct {u32 low,high;u32 entropy[8];} Clock;
typedef struct {u8 pad[16];Clock clock;u8 pad2[40];u32 seconds;} Work;
typedef struct {u8 pad[32];Work*w;} Global;
extern Global ov112_021FFAA4;
extern int RTC_GetTime(RTCTime*);extern int RTC_GetDate(RTCDate*);

void ov112_021E5E28(void);
void ov112_021E5E48(void);

void ov112_021E5E28(void){Work*w=ov112_021FFAA4.w;Clock*c=&w->clock;s64 offset=OS_GetOwnerRtcOffset();w->clock.low=(u32)offset;c->high=offset>>32;OS_GetLowEntropyData(c->entropy);}
void ov112_021E5E48(void){RTCTime t;RTCDate d;if(RTC_GetTime(&t)==0){if(RTC_GetDate(&d)==0){s64 sec=RTC_ConvertDateTimeToSecond(&d,&t);u32 s;if(sec>0xffffffffLL)sec=0xffffffffLL;if(sec<0)sec=0;s=(u32)sec;ov112_021FFAA4.w->seconds=(u8)(s>>24)|((s>>8)&0xff00)|((s<<8)&0xff0000)|((s<<24)&0xff000000);}}}
