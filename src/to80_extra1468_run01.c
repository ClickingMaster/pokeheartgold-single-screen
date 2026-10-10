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

void WcmWmcbConnect(Callback *cb) {
    int result;
    switch (cb->error) {
    case 0:
        switch (cb->state) {
        case 6:
            goto done;
        case 26:
            goto done;
        case 8:
        case 9:
            switch (_021D5FEC->phase) {
            case 10:
                _021D5FEC->aid = 0;
            case 8:
                WcmSetPhase(12);
                return;
            case 9:
                _021D5FEC->aid = 0;
                _021D5FEC->previous = 6;
            case 12:
                WcmWmReset();
                return;
            default:
                goto done;
            }
        case 7:
            if (_021D5FEC->phase == 12) {
                WcmSetPhase(8);
                WcmWmReset();
                return;
            }
            if (cb->aid >= 1 && cb->aid <= 2007) {
                _021D5FEC->aid = cb->aid;
                result = WM_StartDCF(WcmWmcbStartDCF, (u8 *)_021D5FEC + 0x1500, 0x620);
                if (result == 2) {
                    return;
                }
                if (result != 3 && result == 8) {
                    WcmSetPhase(12);
                    WcmNotify(1, _021D5FEC->ap, 0, 0x7ed);
                } else {
                    WcmSetPhase(11);
                    WcmNotify(7, _021D5FEC->ap, 0, 0x7f6);
                }
                return;
            }
            WcmWmReset();
            return;
        default:
            WcmSetPhase(11);
            WcmNotify(7, _021D5FEC->ap, cb->state, 0x804);
            return;
        }
        return;
    case 1:
        _021D5FEC->reason = cb->count;
    case 6:
    case 11:
    case 12:
        WcmSetPhase(8);
        WcmWmReset();
        return;
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
    default:
        WcmSetPhase(11);
        WcmNotify(7, _021D5FEC->ap, 0, 0x81b);
        return;
    }
done:
    return;
}
