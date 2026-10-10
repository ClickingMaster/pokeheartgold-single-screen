#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 head[0x1500];
    u8 scanBuffer[0x2000 - 0x1500];
    u8 gap[0x264];
    u32 options;
    u16 period;
    u8 pad[0x284 - 0x26a];
    u32 zero;
    void *buffer;
    u16 size, channels, duration;
    u8 address[6];
    u16 scanType, length;
    u8 ssid[32];
} Work;

extern Work *_021D5FEC;
extern u8 _02108FC0[];
extern u8 _02108FC8[];
extern void WCM_UpdateOption(u32);
extern int WcmGetNextScanChannel(int);
extern u16 WM_GetDispersionScanPeriod(void);

void WcmEditScanExParam(const u8 *address, const u8 *ssid, u32 option);

void WcmEditScanExParam(const u8 *address, const u8 *ssid, u32 option) {
    u16 period;
    WCM_UpdateOption(option);
    _021D5FEC->buffer = _021D5FEC->scanBuffer;
    _021D5FEC->size = 0x400;
    _021D5FEC->channels = (1 << WcmGetNextScanChannel(0)) >> 1;
    period = _021D5FEC->period;
    if (period == 0) {
        period = WM_GetDispersionScanPeriod();
    }
    _021D5FEC->duration = period;
    _021D5FEC->scanType = (_021D5FEC->options & 0x300000) != 0x300000;
    if (address == 0) {
        MI_CpuCopy8(_02108FC0, _021D5FEC->address, 6);
    } else {
        MI_CpuCopy8(address, _021D5FEC->address, 6);
    }
    if (ssid == 0 || ssid == _02108FC8) {
        MI_CpuCopy8(_02108FC8, _021D5FEC->ssid, 32);
        _021D5FEC->length = 0;
    } else {
        int n;
        MI_CpuCopy8(ssid, _021D5FEC->ssid, 32);
        for (n = 0; n < 32; n++, ssid++) {
            if (*ssid == 0) {
                break;
            }
        }
        _021D5FEC->length = n;
    }
    _021D5FEC->zero = 0;
}
