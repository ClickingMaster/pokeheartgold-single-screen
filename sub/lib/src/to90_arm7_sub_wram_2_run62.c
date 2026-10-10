#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35130_GPIO = 0x8000
} w35130_Interface;

extern void EXIi_SelectRcnt(w35130_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);
void RtcGpioTransfer(u32 command, u32 part, void *data, u32 size);

void RtcGpioTransfer(u32 command, u32 part, void *data, u32 size) {
    RTCi_GpioStart();
    RTCi_GpioSendCommand(command, part);
    switch (command) {
    case 0x86:
        RTCi_GpioReceiveData(data, size);
        break;
    case 6:
        RTCi_GpioSendData(data, size);
        break;
    }
    RTCi_GpioEnd();
}
