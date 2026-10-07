#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void EXIi_SelectRcnt(u32);
void RtcGpioTransfer(u32,u32,void *,u32);
void RTC_ReadDateTime(void *);
// @fn RTC_ReadDateTime
void RTC_ReadDateTime(void *p) {EXIi_SelectRcnt(0x8000);RtcGpioTransfer(0x86,32,p,7);}
