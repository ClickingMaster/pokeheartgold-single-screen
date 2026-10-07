#pragma force_active on
#include <nitro/types.h>
#include <nitro/exi/ARM7/genPort.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void RTCi_GpioStart(void);void RTCi_GpioSendCommand(u32,u32);void RTCi_GpioSendData(const void*,u32);void RTCi_GpioEnd(void);void RtcGpioTransfer(u32,u32,void*,u32);
typedef union {u16 value;struct {u16 reset:1,other:15;} bits;} ResetStatus;
void RTC_Reset(void);
void RTC_Reset(void){ResetStatus status;EXIi_SelectRcnt(EXI_GPIOIF_GPIO);status.bits.reset=1;RTCi_GpioStart();RTCi_GpioSendCommand(6,0);RTCi_GpioSendData(&status,1);RTCi_GpioEnd();}
