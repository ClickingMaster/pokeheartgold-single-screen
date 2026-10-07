#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
// RTC transfers define one byte. Every named field is in that byte;
// the other bits are representation padding and are never serialized.
typedef struct W40828Status1{u16 :4;u16 alarm1:1;u16 alarm2:1;u16 :10;}W40828Status1;
typedef struct W40828Status2{u16 mode:4;u16 :2;u16 alarm2:1;u16 :9;}W40828Status2;
void RTC_ReadStatus1(void*);void RTC_ReadStatus2(void*);void RTC_WriteStatus2(const void*);void RtcReturnResult(u32,u32);void RtcAlarmIntr(void);
typedef char W40828Assert1[(sizeof(W40828Status1)==2)?1:-1];typedef char W40828Assert2[(sizeof(W40828Status2)==2)?1:-1];
void RtcAlarmIntr(void){W40828Status1 status1;W40828Status2 status2;u32 flags;RTC_ReadStatus1(&status1);if(status1.alarm1||status1.alarm2){RTC_ReadStatus2(&status2);flags=0;if(status1.alarm1){flags|=1;status2.mode=0;}if(status1.alarm2){flags|=2;status2.alarm2=0;}RTC_WriteStatus2(&status2);RtcReturnResult(48,flags);}}
