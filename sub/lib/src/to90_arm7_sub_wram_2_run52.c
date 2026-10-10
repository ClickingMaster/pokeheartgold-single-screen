#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35120_GPIO = 0x8000
} w35120_Interface;

extern void EXIi_SelectRcnt(w35120_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);
void RTC_ReadStatus1(void *data);

void RTC_ReadStatus1(void *data) {
    EXIi_SelectRcnt(w35120_GPIO);
    RtcGpioTransfer(0x86, 0, data, 1);
}
