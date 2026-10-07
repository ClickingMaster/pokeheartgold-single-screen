#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void EXIi_SelectRcnt(u32);void RTCi_GpioStart(void);void RTCi_GpioSendCommand(u32,u32);void RTCi_GpioSendData(void*,u32);void RTCi_GpioEnd(void);
void RTC_WriteDateTime(void *p);
// @fn RTC_WriteDateTime
void RTC_WriteDateTime(void *p){EXIi_SelectRcnt(0x8000);RTCi_GpioStart();RTCi_GpioSendCommand(6,32);RTCi_GpioSendData(p,7);RTCi_GpioEnd();}
