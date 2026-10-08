#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <nitro/rtc.h>
s32 RTCi_ConvertTimeToSecond(const RTCTime*);
// @fn RTC_ConvertDateTimeToSecond
s64 RTC_ConvertDateTimeToSecond(const RTCDate*d,const RTCTime*t){s32 day=RTC_ConvertDateToDay(d);s32 sec;if(day==-1)return -1;sec=RTCi_ConvertTimeToSecond(t);if(sec==-1)return -1;return (s64)day*86400+sec;}
