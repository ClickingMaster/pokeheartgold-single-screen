#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35114_GPIO = 0x8000
} w35114_Interface;

extern void EXIi_SelectRcnt(w35114_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);

typedef struct {
    u16 mode : 4, unused : 2, alarm2 : 1, pad : 9;
} w35114_Status;

int RTC_ReadPulse(void *data);

int RTC_ReadPulse(void *data) {
    w35114_Status status;
    EXIi_SelectRcnt(w35114_GPIO);
    RtcGpioTransfer(0x86, 0x40, &status, 1);
    if ((status.mode & 11) != 1) {
        return 0;
    }
    RtcGpioTransfer(0x86, 16, data, 1);
    return 1;
}
