#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35113_GPIO=0x8000} w35113_Interface;extern void EXIi_SelectRcnt(w35113_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
void RTC_WriteTime(const void*data);
// @fn RTC_WriteTime
void RTC_WriteTime(const void*data){EXIi_SelectRcnt(w35113_GPIO);RTCi_GpioStart();RTCi_GpioSendCommand(6,96);RTCi_GpioSendData(data,3);RTCi_GpioEnd();}
