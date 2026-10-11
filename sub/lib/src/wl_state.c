#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

int WSetRateSet(const u16 *rates) {
    WlWork *w = wlWorkPlain;
    {
        WlRateSet *rateSet = &w->bss.rateSet;
        rateSet->basic = rates[0];
        rateSet->supported = rates[1] | rates[0];
    }
    WSetTxTimeStampOffset();
    return 0;
}

// Transmit time stamp offset: flash value, corrected for the management frame rate and preamble.
void WSetTxTimeStampOffset(void) {
    u32 value = 0xE2E2;
    FLASH_Read(0x58, 2, &value);
    value += 0x202;
    if (WCalcManRate() == 20) {
        value -= 0x6161;
        if (WL_REG(0x0BC) & 2) {
            value -= 0x6060;
        }
    }
    WL_REG(0x140) = value;
}

int WSetPowerMgtMode(u32 mode) {
    WlWork *w = wlWorkPlain;
    WlParam *param = &w->param;
    w->bss.powerMgtMode = mode;
    if (mode && param->mode != 1) {
        WL_REG(0x006) |= 0x40;
    } else {
        WL_REG(0x006) &= ~0x40;
        WSetActiveZoneTime(param->activeZoneTime, 0);
    }
    return 0;
}

int WSetPowerState(u32 state) {
    wlWorkPlain->bss.powerState = state >> 1;
    WL_REG(0x03C) = state;
    return 0;
}

int WSetForcePowerState(u32 state) {
    WL_REG(0x040) = state;
    return 0;
}

void WShutdown(void) {
    if (wlWorkPlain->rf.type == 2) {
        RF_Write(0xC008);
    }
    WL_REG(0x158) = 0x601E;
    WaitLoop_BbpAccess();
    BBP_Write(30, WL_REG(0x15C) | 0x3F);
    WL_REG(0x168) = 0x800D;
    WL_REG(0x036) = 1;
}

void WWakeUp(void) {
    WL_REG(0x036) = 0;
    WaitLoop_Waitus(8000, TimeoutDummy);
    WL_REG(0x168) = 0;
    switch (wlWorkPlain->rf.type) {
    case 2: {
        u32 value;
        WL_REG(0x158) = 0x6001;
        WaitLoop_BbpAccess();
        value = WL_REG(0x15C);
        BBP_Write(1, value & 0x7F);
        BBP_Write(1, value);
        WaitLoop_Waitus(40000, TimeoutDummy);
        InitRF();
        break;
    }
    case 3:
        InitRF();
        break;
    }
}

// Frame life time (0xFFFF: unlimited); also kept in beacon periods.
int WSetFrameLifeTime(u32 time) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    u32 value;
    if (time == 0xFFFF) {
        param->frameLifeTime = 0xFFFF;
        bss->unk_8c = 0xFFFF;
    } else {
        value = __VENEER__u32_div_f(time * bss->beaconPeriod, 100);
        if (value > 0x10000) {
            return 5;
        }
        param->frameLifeTime = time;
        bss->unk_8c = value;
    }
    return 0;
}

void WDisableTmpttPowerSave(void) {
    wlWorkPlain->bss.tmpttPowerSaveOff = TRUE;
    if (wlWorkPlain->txDesc[3].active == 0) {
        WL_REG(0x038) &= ~2;
        WL_REG(0x048) = 0;
    }
}

void WEnableTmpttPowerSave(void) {
    wlWorkPlain->bss.tmpttPowerSaveOff = FALSE;
    WL_REG(0x038) |= 2;
}

// Game info carried in the own beacon (at most 128 bytes).
int WInitGameInfo(u32 length, const void *data) {
    WlBss *bss = &wlWorkPlain->bss;
    if (length > 128) {
        return 4;
    }
    __VENEER_MIi_CpuCopy16(data, bss->gameInfo, length + 1);
    bss->gameInfoLength = length;
    return 0;
}

int WSetGameInfo(u32 length, const u8 *data) {
    WlBss *bss = &wlWorkPlain->bss;
    if (length > 128) {
        return 4;
    }
    if (length) {
        if (bss->unk_a2 & 1) {
            u8 *dest = bss->gameInfo;
            u32 i;
            WL_WriteByte(dest, 0xFF);
            dest++;
            for (i = 0; i < length; i++) {
                WL_WriteByte(dest, WL_ReadByte(data));
                dest++;
                data++;
            }
        } else {
            __VENEER_MIi_CpuCopy16(data, bss->gameInfo, length + 1);
        }
    }
    bss->gameInfoLength = length;
    bss->unk_a4 = 1;
    return 0;
}

void WSetAids(u16 aid) {
    wlWorkPlain->bss.aid = aid;
    WL_REG(0x02A) = aid;
    if (wlWorkPlain->param.flags.bits.nullKeyMode) {
        WL_REG(0x028) = aid;
    }
}

void WClearAids(void) {
    WlBss *bss = &wlWorkPlain->bss;
    bss->aid = 0;
    WaitLoop_ClrAid();
    WL_REG(0x02A) = 0;
    if (bss->unk_88) {
        DeleteTxFrames(bss->unk_88);
        CAM_SetStaState(bss->unk_88, 32);
        bss->unk_88 = 0;
    }
}

void WSetKSID(u32 unused) {
    WL_REG(0x028) = wlWorkPlain->bss.aid;
}

void WClearKSID(u32 unused) {
    if (!(WL_REG(0x094) & 0x8000)) {
        WaitLoop_ClrAid();
    }
}

// Station state (0: shut down, 0x10: idle, 0x40: own BSS running with the periodic interval timer).
void WSetStaState(u32 state) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    if (bss->state == state) {
        return;
    }
    if (bss->state == 0x40) {
        __VENEER_OS_CancelAlarm(&w->timeOutAlarm);
    }
    switch (state) {
    case 0:
        WShutdown();
        break;
    case 0x10:
        WL_REG(0x040) = 0;
        WStop();
        WWakeUp();
        break;
    case 0x40:
        if (bss->mode == 2) {
            WEnableTmpttPowerSave();
        }
        SetupPeriodicTimeOut(100, (OSAlarmHandler)WIntervalTimer);
        break;
    }
    bss->state = state;
}

void WSetMacAdrs1(u16 *dest, const u16 *addr) {
    dest[0] = addr[0];
    dest[1] = addr[1];
    dest[2] = addr[2];
}

void WSetMacAdrs2(u16 *dest, const u16 *addr1, const u16 *addr2) {
    dest[0] = addr1[0];
    dest[1] = addr1[1];
    dest[2] = addr1[2];
    dest[3] = addr2[0];
    dest[4] = addr2[1];
    dest[5] = addr2[2];
}

void WSetMacAdrs3(u16 *dest, const u16 *addr1, const u16 *addr2, const u16 *addr3) {
    dest[0] = addr1[0];
    dest[1] = addr1[1];
    dest[2] = addr1[2];
    dest[3] = addr2[0];
    dest[4] = addr2[1];
    dest[5] = addr2[2];
    dest[6] = addr3[0];
    dest[7] = addr3[1];
    dest[8] = addr3[2];
}
