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

#pragma force_active on

void WcmWmcbCommon(Callback *cb) {
    int result = 20;
    switch (cb->error) {
    case 0:
        switch (cb->api) {
        case 3:
            result = WM_PowerOn(WcmWmcbCommon);
            break;
        case 4:
            result = WM_Finish();
            switch (result) {
            case 0: {
                WcmSetPhase(1);
                WcmNotify(0, 0, 0, 0x663);
                return;
            }
            case 4:
            default: {
                WcmSetPhase(11);
                WcmNotify(7, 0, 0, 0x66c);
                return;
            }
            }
        case 5:
            WcmSetPhase(3);
            WcmNotify(0, 0, 0, 0x673);
            return;
        case 6:
            result = WM_Disable(WcmWmcbCommon);
            break;
        case 29:
            result = WM_SetBeaconIndication(WcmWmcbCommon, 0);
            break;
        case 25:
            result = WM_SetWEPKeyEx(WcmWmcbCommon, _021D5FEC->wepType, _021D5FEC->wepIndex, (u8 *)_021D5FEC + 0x2200);
            break;
        case 39: {
            u16 power;
            u32 auth;
            power = PowerMode(_021D5FEC->options);
            auth = (_021D5FEC->options & 0x30000) == 0x30000 ? 0 : 1;
            result = WM_StartConnectEx(WcmWmcbConnect, _021D5FEC->ap, 0, auth, power);
            break;
        }
        }
        if (result == 2) {
            return;
        }
        if (result != 3 && result == 8) {
            WcmSetPhase(12);
            WcmNotify(1, _021D5FEC->previous == 5 ? _021D5FEC->ap : 0, 0, 0x6af);
        } else {
            WcmSetPhase(11);
            WcmNotify(7, _021D5FEC->previous == 5 ? _021D5FEC->ap : 0, 0, 0x6b8);
        }
        return;
    case 1:
        WcmSetPhase(12);
        WcmNotify(1, _021D5FEC->previous == 5 ? _021D5FEC->ap : 0, 0, 0x6de);
        return;
    case 2:
    case 3:
    case 4:
    default:
        WcmSetPhase(11);
        WcmNotify(7, _021D5FEC->previous == 5 ? _021D5FEC->ap : 0, 0, 0x6e8);
        return;
    }
}
