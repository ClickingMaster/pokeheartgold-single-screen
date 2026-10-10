#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef enum {
    w35129_GPIO = 0x8000
} w35129_Interface;

extern void EXIi_SelectRcnt(w35129_Interface);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(const void *, u32);
extern void RTCi_GpioReceiveData(void *, u32);
extern void RtcGpioTransfer(u32, u32, void *, u32);

typedef struct {
    u32 low : 8, hour : 6, pm : 1, rest : 17;
} w35129_Alarm;

void RtcChangeAlarmFormat12to24(w35129_Alarm *a);

void RtcChangeAlarmFormat12to24(w35129_Alarm *a) {
    switch (a->hour) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 16:
    case 17:
        if (!a->pm) {
            return;
        }
        a->hour += 0x12;
        return;
    case 8:
    case 9:
        if (!a->pm) {
            return;
        }
        a->hour += 0x18;
        return;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 32:
    case 33:
    case 34:
    case 35:
        a->pm = 1;
        return;
    default:
        a->pm = 0;
        a->hour = 0;
        return;
    }
}
