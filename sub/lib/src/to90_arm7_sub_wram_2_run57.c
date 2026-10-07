#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35125_GPIO=0x8000} w35125_Interface;extern void EXIi_SelectRcnt(w35125_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
void RTC_WriteAdjust(const void*data);
// @fn RTC_WriteAdjust
void RTC_WriteAdjust(const void*data){EXIi_SelectRcnt(w35125_GPIO);RTCi_GpioStart();RTCi_GpioSendCommand(6,48);RTCi_GpioSendData(data,1);RTCi_GpioEnd();}
