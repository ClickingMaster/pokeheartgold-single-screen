#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x2140];
    u8 ap[0x110];
    u8 wepType, wepIndex;
    u8 more[14];
    u32 phase, options;
    u8 z[0x2280 - 0x2268];
    s16 previous;
    u16 aid;
    u32 scans;
    void *buffer;
    u16 size, channel;
    u8 rest[0x22f8 - 0x2290];
    u16 reason;
} Work;

extern Work *_021D5FEC;

typedef struct {
    u16 api, error;
    u16 p[2];
    u16 state, aid;
    u16 q, count;
    void *entries[16];
    u16 values[16];
} Callback;

extern void WcmSetPhase(int);
extern void WcmNotify(int, void *, int, int);
extern void WcmNotifyEx(int, int, void *, void *, int);
extern void WcmWmReset(void);
extern int WM_PowerOn(void *);
extern int WM_Finish(void);
extern int WM_Disable(void *);
extern int WM_SetBeaconIndication(void *, int);
extern int WM_SetWEPKeyEx(void *, int, int, void *);
extern int WM_StartConnectEx(void *, void *, int, int, u16);
extern int WM_StartScanEx(void *, void *);
extern int WM_EndScan(void *);
extern int WM_StartDCF(void *, void *, int);
extern int WM_PowerOff(void *);
extern void WcmWmcbCommon(Callback *);
extern void WcmWmcbScanEx(Callback *);
extern void WcmWmcbConnect(Callback *);
extern void WcmWmcbEndScan(void *);
extern void WcmWmcbStartDCF(void *);
extern void WcmWmcbReset(Callback *);
extern int WcmCountBits(u32);
extern int WcmCountLeadingZero(u32);
extern int WcmGetNextScanChannel(int);
extern void WCMi_EntryApList(void *, u16);
extern void DC_InvalidateRange(void *, u32);

static inline u16 PowerMode(u32 options) {
    return (options & 0xc0000) == 0xc0000;
}

void WcmWmcbScanEx(Callback *cb) {
    int result = 20, i;
    switch (cb->error) {
    case 0:
        if (_021D5FEC->phase == 5) {
            WcmSetPhase(6);
            WcmNotify(0, 0, 0, 0x704);
        }
        switch (_021D5FEC->phase) {
        case 6:
            _021D5FEC->previous = 7;
            if (cb->state == 5) {
                DC_InvalidateRange(_021D5FEC->buffer, _021D5FEC->size);
                for (i = 0; i < cb->count; i++) {
                    WCMi_EntryApList(cb->entries[i], cb->values[i]);
                    WcmNotifyEx(7, 0, cb->entries[i], cb, 0x71a);
                }
            }
            if ((_021D5FEC->options & 0xc00000) == 0xc00000) {
                int count = WcmCountBits(_021D5FEC->options & 0x3ffe);
                if (count) {
                    u32 scans = _021D5FEC->scans;
                    if (scans % count == 0) {
                        WcmNotifyEx(8, 0, (void *)scans, 0, 0x728);
                    }
                }
            }
            _021D5FEC->channel = (1 << WcmGetNextScanChannel((u16)(32 - WcmCountLeadingZero(cb->aid)))) >> 1;
            DC_InvalidateRange(_021D5FEC->buffer, _021D5FEC->size);
            _021D5FEC->scans++;
            result = WM_StartScanEx(WcmWmcbScanEx, &_021D5FEC->buffer);
            break;
        case 7:
            result = WM_EndScan(WcmWmcbEndScan);
            break;
        case 13:
            WcmWmReset();
            return;
        }
        if (result == 2) {
            return;
        }
        if (result != 3 && result == 8) {
            WcmSetPhase(12);
            WcmNotify(1, 0, 0, 0x753);
        } else {
            WcmSetPhase(11);
            WcmNotify(7, 0, 0, 0x75c);
        }
        return;
    case 1:
        WcmWmReset();
        return;
    case 2:
    case 3:
    case 4:
    default:
        WcmSetPhase(11);
        WcmNotify(7, 0, 0, 0x76d);
        return;
    }
}
