#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35113_GPIO = 0x8000
} w35113_Interface;

extern void EXIi_SelectRcnt(w35113_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);
void RTC_WriteTime(const void *data);

void RTC_WriteTime(const void *data) {
    EXIi_SelectRcnt(w35113_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 96);
    RTCi_GpioSendData(data, 3);
    RTCi_GpioEnd();
}
