#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
typedef enum{w35116_GPIO=0x8000} w35116_Interface;extern void EXIi_SelectRcnt(w35116_Interface);
extern void RTCi_GpioStart(void),RTCi_GpioEnd(void),RTCi_GpioSendCommand(u32,u32),RTCi_GpioSendData(const void*,u32),RTCi_GpioReceiveData(void*,u32),RtcGpioTransfer(u32,u32,void*,u32);
typedef struct{u16 mode:4,unused:2,alarm2:1,pad:9;}w35116_Status;
int RTC_ReadAlarm1(void*data);
// @fn RTC_ReadAlarm1
int RTC_ReadAlarm1(void*data){w35116_Status status;EXIi_SelectRcnt(w35116_GPIO);RtcGpioTransfer(0x86,0x40,&status,1);if(status.mode!=4)return 0;RtcGpioTransfer(0x86,16,data,3);return 1;}
