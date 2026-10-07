#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct{u64 state;u64 mul;u64 inc;} Random;
extern void GF_RTC_CopyDateTime(RTCDate *,RTCTime *);
void sub_0203778C(Random *);
// @fn sub_0203778C
void sub_0203778C(Random *r){RTCDate date;RTCTime time;GF_RTC_CopyDateTime(&date,&time);r->state=(time.second+gSystem.vblankCounter)+((time.minute+((time.hour+((date.day+((date.month+((u64)date.year<<4))<<5))<<5))<<6))<<6);r->mul=0x5D588B656C078965ULL;r->inc=0x269EC3;}
