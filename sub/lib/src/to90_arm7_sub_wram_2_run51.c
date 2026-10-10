#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35119_GPIO = 0x8000
} w35119_Interface;

extern void EXIi_SelectRcnt(w35119_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);

typedef struct {
    u16 mode : 4, unused : 2, alarm2 : 1, pad : 9;
} w35119_Status;

int RTC_WriteAlarm2(void *data);

int RTC_WriteAlarm2(void *data) {
    w35119_Status status;
    EXIi_SelectRcnt(w35119_GPIO);
    RtcGpioTransfer(0x86, 0x40, &status, 1);
    if (!status.alarm2) {
        return 0;
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 80);
    RTCi_GpioSendData(data, 3);
    RTCi_GpioEnd();
    return 1;
}
