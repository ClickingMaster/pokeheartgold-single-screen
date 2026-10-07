#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35120_GPIO=0x8000} w35120_Interface;extern void EXIi_SelectRcnt(w35120_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
void RTC_ReadStatus1(void*data);
// @fn RTC_ReadStatus1
void RTC_ReadStatus1(void*data){EXIi_SelectRcnt(w35120_GPIO);RtcGpioTransfer(0x86,0,data,1);}
