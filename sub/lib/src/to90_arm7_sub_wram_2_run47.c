#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35115_GPIO=0x8000} w35115_Interface;extern void EXIi_SelectRcnt(w35115_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
typedef struct{u16 mode:4,unused:2,alarm2:1,pad:9;}w35115_Status;
int RTC_WritePulse(void*data);
// @fn RTC_WritePulse
int RTC_WritePulse(void*data){w35115_Status status;EXIi_SelectRcnt(w35115_GPIO);RtcGpioTransfer(0x86,0x40,&status,1);if((status.mode&11)!=1)return 0;RTCi_GpioStart();RTCi_GpioSendCommand(6,16);RTCi_GpioSendData(data,1);RTCi_GpioEnd();return 1;}
