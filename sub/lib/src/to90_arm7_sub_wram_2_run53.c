#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35121_GPIO = 0x8000
} w35121_Interface;

extern void EXIi_SelectRcnt(w35121_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);
void RTC_WriteStatus1(const void *data);

void RTC_WriteStatus1(const void *data) {
    EXIi_SelectRcnt(w35121_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 0);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
}
