#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35123_GPIO=0x8000} w35123_Interface;extern void EXIi_SelectRcnt(w35123_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
void RTC_WriteStatus2(const void*data);
// @fn RTC_WriteStatus2
void RTC_WriteStatus2(const void*data){EXIi_SelectRcnt(w35123_GPIO);RTCi_GpioStart();RTCi_GpioSendCommand(6,64);RTCi_GpioSendData(data,1);RTCi_GpioEnd();}
