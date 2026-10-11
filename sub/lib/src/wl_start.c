#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// The pair of registers at 0x04808008 / 0x0480800A that WStart clears and sets per mode.
typedef struct WlRegPair {
    vu16 tx;
    vu16 rx;
} WlRegPair;

#pragma opt_propagation off

// Programs the next target beacon time: the current TSF rounded up to the next beacon period.
static inline void ProgramNextBeacon(const WlBss *bss) {
    union {
        u64 value;
        u16 part[4];
    } stamp;

    u16 *words = stamp.part;
    u64 period;
    words[0] = WL_REG(0x0F8);
    words[1] = WL_REG(0x0FA);
    words[2] = WL_REG(0x0FC);
    words[3] = WL_REG(0x0FE);
    period = (u32)bss->beaconPeriod << 10;
    stamp.value = __VENEER__ll_udiv(stamp.value, (u64)period) + 1;
    stamp.value *= period;
    WL_REG(0x0F6) = words[3];
    WL_REG(0x0F4) = words[2];
    WL_REG(0x0F2) = words[1];
    WL_REG(0x0F0) = words[0] | 1;
}

// Restarts the MAC in the current mode (0 .. 4; 1: own BSS with beacons).
void WStart(void) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    u32 a, b, c;
    WlRegPair *enable;
    WStop();
    a = WL_REG(0x044);
    b = WL_REG(0x044);
    c = WL_REG(0x044);
    RND_Init(a + (b << 8), c);
    bss->capability = 1;
    if (param->flags.bits.preambleType == 1) {
        bss->capability |= 0x20;
    }
    if (param->wepMode) {
        bss->capability |= 0x10;
    }
    bss->unk_12 = 0;
    WL_REG(0x032) = 0x8000;
    WL_REG(0x134) = 0xFFFF;
    WL_REG(0x02A) = 0;
    WL_REG(0x028) = 0;
    WL_REG(0x038) = 15;
    InitCAM();
    InitApList();
    InitTxCtrl();
    InitRxCtrl();
    WL_REG(0x030) = 0x8000;
    WL_REG(0x010) = 0xFFFF;
    WL_REG(0x1AE) = 0x1FFF;
    if (wlWorkPlain->unk_690 & 8) {
        WL_REG(0x1AA) = 0x400;
    } else {
        WL_REG(0x1AA) = 0;
    }
    enable = (WlRegPair *)&WL_REG(0x008);
    enable->tx = 0;
    enable->rx = 0;
    switch (bss->mode) {
    case 0:
        WL_REG(0x012) = 0x3F;
        WL_REG(0x0D0) = 0xFFFF;
        WL_REG(0x0E0) = 8;
        enable->tx = 0;
        enable->rx = 0;
        WL_REG(0x0E8) = 0;
        WL_REG(0x004) = 1;
        break;
    case 1: {
        WL_REG(0x012) = 0x703F;
        WL_REG(0x1AE) = 0x1FFF;
        WL_REG(0x0D0) = 0x301;
        WL_REG(0x0E0) = 13;
        enable->tx = 0xE000;
        WL_REG(0x004) = 1;
        ProgramNextBeacon(bss);
        WL_REG(0x0E8) = 1;
        WL_REG(0x0EA) = 1;
        WSetStaState(64);
        StartBeaconFrame();
        WL_REG(0x0AE) = 2;
        break;
    }
    case 2:
        WL_REG(0x012) = 0xE0BF;
        if (wlWorkPlain->unk_690 & 0x20) {
            WL_REG(0x012) |= 0x40;
            WL_REG(0x1AA) |= 0x68;
        }
        if (bss->bssid[0] & 1) {
            WL_REG(0x0D0) = 0x581;
        } else {
            WL_REG(0x0D0) = 0x181;
        }
        WL_REG(0x0E0) = 11;
        WL_REG(0x004) = 1;
        WL_REG(0x0E8) = 1;
        WL_REG(0x0EA) = 1;
        WSetStaState(32);
        break;
    case 3:
        WL_REG(0x010) = 0xFFFF;
        WL_REG(0x012) = 0xC03F;
        if (bss->bssid[0] & 1) {
            WL_REG(0x0D0) = 0x401;
        } else {
            WL_REG(0x0D0) = 1;
        }
        WL_REG(0x0E0) = 11;
        WL_REG(0x004) = 1;
        WL_REG(0x0E8) = 1;
        WL_REG(0x0EA) = 1;
        WL_REG(0x048) = 0;
        WSetStaState(32);
        break;
    case 4:
        WL_REG(0x012) = 0;
        WL_REG(0x1AE) = 0;
        WL_REG(0x004) = 1;
        WSetStaState(32);
        break;
    }
    WL_REG(0x048) = 0;
    WDisableTmpttPowerSave();
    WL_REG(0x0AE) = 2;
    if (bss->powerMgtMode == 1) {
        WSetPowerState(2);
    }
    WaitLoop_Rxpe();
}

#pragma opt_propagation reset
