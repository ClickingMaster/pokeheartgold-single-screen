#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35127_GPIO=0x8000} w35127_Interface;extern void EXIi_SelectRcnt(w35127_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
void RTC_WriteFree(const void*data);
// @fn RTC_WriteFree
void RTC_WriteFree(const void*data){EXIi_SelectRcnt(w35127_GPIO);RTCi_GpioStart();RTCi_GpioSendCommand(6,112);RTCi_GpioSendData(data,1);RTCi_GpioEnd();}
