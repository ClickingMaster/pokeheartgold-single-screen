#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// WEP key slots in the wireless RAM (4 keys, 0x20 bytes apart).
#define WL_WEP_KEY_RAM 0x04805F80

// The setters below return 0, or 5 for an out-of-range value.
#define PARAM_OK    0
#define PARAM_RANGE 5

// Bit 0 of `value` moved to bit `bit` (how the setters below store into WlParamFlags.raw).
#define FLAG_BIT(value, bit) (((u32)(value) << 31) >> (31 - (bit)))

extern u8 def_WepKey[];
extern u16 def_SsidMask[];
// Rate set of WSetDefaultParameters (a function-local static, hence the generated name).
#pragma dollar_identifiers on
extern u16 c_RateSet$4351[];

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Clears the parameters and the BSS state (keeping the BSS buffer) and takes the station table.
void InitializeParam(void *camTable, u32 camTableSize) {
    WlWork *w = wlWork;
    u8 *buf = w->bss.gameInfo;
    __VENEER_MIi_CpuClear16(0, &w->param, 0x28); // all of WlParam
    __VENEER_MIi_CpuClear16(0, &wlWork->bss, sizeof(WlBss));
    wlWork->param.camTable = camTable;
    wlWork->param.camTableSize = camTableSize;
    wlWork->param.maxSta = camTableSize;
    wlWork->bss.gameInfo = buf;
}

// Own MAC address (must not be a group address).
u32 WSetMacAdrs(const u16 *addr) {
    if (*addr & 1) {
        return PARAM_RANGE;
    }
    WSetMacAdrs1(wlWork->param.macAddress, addr);
    WSetMacAdrs1((u16 *)&WL_REG(0x018), addr);
    {
        WlWork *w = wlWork;
        w->param.unk_340 |= 2;
    }
    return PARAM_OK;
}

u32 WSetRetryLimit(u16 limit) {
    if (limit > 255) {
        return PARAM_RANGE;
    }
    wlWork->param.retryLimit = limit;
    WL_REG(0x02C) = limit;
    return PARAM_OK;
}

// Bit mask of the usable channels 1..14.
u32 WSetEnableChannel(u32 channels) {
    if (!(channels & 0x7FFE)) {
        return PARAM_RANGE;
    }
    wlWork->param.enableChannel = channels;
    {
        WlWork *w = wlWork;
        w->param.unk_340 |= 4;
    }
    return PARAM_OK;
}

u32 WSetMode(u16 mode) {
    if (mode > 3) {
        return PARAM_RANGE;
    }
    wlWork->param.mode = mode;
    wlWork->bss.mode = mode;
    WL_REG(0x006) = (WL_REG(0x006) & 0xFFF8) | mode;
    WSetPowerMgtMode(wlWork->bss.powerMgtMode);
    {
        WlWork *w = wlWork;
        w->param.unk_340 |= 8;
    }
    return PARAM_OK;
}

u32 WSetRate(u32 rate) {
    if (rate > 2) {
        return PARAM_RANGE;
    }
    wlWork->param.rate = rate;
    WSetTxTimeStampOffset();
    return PARAM_OK;
}

// 0: no WEP; also updates the privacy bit of an own beacon.
u32 WSetWepMode(u32 mode) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    if (mode > 3) {
        return PARAM_RANGE;
    }
    w->param.wepMode = mode;
    if (mode == 0) {
        bss->capability &= ~0x10;
        bss->unk_8a &= ~0x4000;
    } else {
        bss->capability |= 0x10;
        bss->unk_8a |= 0x4000;
    }
    if (bss->state == 0x40 && mode == 1) {
        *(u16 *)(wlWork->txCtrl.beacon.frame + 0x2E) = bss->capability;
    }
    if (mode == 0) {
        mode = 1;
    }
    WL_REG(0x006) = (WL_REG(0x006) & 0xFFC7) | (mode << 3);
    return PARAM_OK;
}

u32 WSetWepKeyId(u32 id) {
    if (id > 3) {
        return PARAM_RANGE;
    }
    wlWork->param.wepKeyId = id;
    return PARAM_OK;
}

// Four 20-byte keys.
u32 WSetWepKey(const u8 *keys) {
    __VENEER_MIi_CpuCopy16(keys, (void *)WL_WEP_KEY_RAM, 20);
    __VENEER_MIi_CpuCopy16(keys + 20, (void *)(WL_WEP_KEY_RAM + 0x20), 20);
    __VENEER_MIi_CpuCopy16(keys + 40, (void *)(WL_WEP_KEY_RAM + 0x40), 20);
    __VENEER_MIi_CpuCopy16(keys + 60, (void *)(WL_WEP_KEY_RAM + 0x60), 20);
    return PARAM_OK;
}

u32 WSetBeaconType(u32 type) {
    if (type > 1) {
        return PARAM_RANGE;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.raw = (w->param.flags.raw & ~1u) | (type & 1);
    }
    return PARAM_OK;
}

// Whether to answer probe requests for the broadcast SSID.
u32 WSetBcSsidResponse(u32 enable) {
    if (enable > 1) {
        return PARAM_RANGE;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.raw = (w->param.flags.raw & ~2u) | FLAG_BIT(enable, 1);
    }
    return PARAM_OK;
}

u32 WSetBeaconLostThreshold(u32 threshold) {
    if (threshold > 255) {
        return PARAM_RANGE;
    }
    wlWork->bss.beaconLostCount = 0;
    wlWork->bss.beaconLostThreshold = threshold;
    return PARAM_OK;
}

// Also written to the hardware when `write` is set, and into the own beacon in power-save mode 1.
u32 WSetActiveZoneTime(u16 time, u32 write) {
    u8 *tim;
    WlWork *w;
    if (time < 10) {
        return PARAM_RANGE;
    }
    wlWork->param.activeZoneTime = time;
    if (write) {
        WL_REG(0x134) = time;
    }
    w = wlWork;
    if (w->txCtrl.beacon.active) {
        tim = w->txCtrl.beacon.frame + 0x24 + w->bss.gameInfoOffset;
        if (w->bss.powerMgtMode == 1) {
            WL_WriteByte(tim + 6, time & 0xFF);
            WL_WriteByte(tim + 7, (time >> 8) & 0xFF);
        } else {
            WL_WriteByte(tim + 6, 0xFF);
            WL_WriteByte(tim + 7, 0xFF);
        }
    }
    return PARAM_OK;
}

u32 WSetSsidMask(const u16 *mask) {
    u16 *dest = wlWork->bss.ssidMask;
    u32 i = 0;
    do {
        *dest++ = *mask++;
        i++;
    } while (i < 16);
    return PARAM_OK;
}

// 1: short preamble; also updates the capability of an own beacon.
u32 WSetPreambleType(u32 type) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    if (type > 1) {
        return PARAM_RANGE;
    }
    w->param.flags.raw = (w->param.flags.raw & ~4u) | FLAG_BIT(type, 2);
    if (!type) {
        bss->capability &= ~0x20;
    } else {
        bss->capability |= 0x20;
    }
    if (bss->state == 0x40) {
        w = wlWork;
        if (w->param.mode == 1) {
            *(u16 *)(w->txCtrl.beacon.frame + 0x2E) = bss->capability;
        }
    }
    if (!type) {
        WL_REG(0x0BC) &= ~6;
    } else {
        WL_REG(0x0BC) |= 6;
    }
    WSetTxTimeStampOffset();
    return PARAM_OK;
}

u32 WSetAuthAlgo(u32 algo) {
    if (algo > 1) {
        return PARAM_RANGE;
    }
    wlWork->param.authAlgo = algo;
    return PARAM_OK;
}

// Clear channel assessment mode and energy detection threshold (baseband registers 0x13, 0x35).
u32 WSetCCA_ED(u32 mode, u32 threshold) {
    if (mode > 3) {
        return PARAM_RANGE;
    }
    if (threshold > 63) {
        return PARAM_RANGE;
    }
    BBP_Write(0x13, mode);
    BBP_Write(0x35, threshold);
    return PARAM_OK;
}

u32 WSetMainAntenna(u32 antenna) {
    if (antenna > 1) {
        return PARAM_RANGE;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.bits.mainAntenna = (u16)antenna;
    }
    {
        WlWork *w = wlWork;
        WL_REG(0x290) = w->param.flags.bits.otherAntenna ^ w->param.flags.bits.mainAntenna;
    }
    return PARAM_OK;
}

// Diversity needs mode 1 (11 otherwise); without it `otherAntenna` selects the antenna.
u32 WSetDiversity(u32 enable, u32 otherAntenna) {
    if (enable > 1 || otherAntenna > 1) {
        return PARAM_RANGE;
    }
    switch (enable) {
    case 0: {
        WlWork *w = wlWork;
        w->param.flags.bits.otherAntenna = (u16)otherAntenna;
    } break;
    case 1: {
        WlWork *w = wlWork;
        if (w->param.mode != 1) {
            return 11;
        }
        w->param.flags.bits.otherAntenna = 0;
    } break;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.bits.diversity = (u16)enable;
    }
    {
        WlWork *w = wlWork;
        WL_REG(0x290) = w->param.flags.bits.otherAntenna ^ w->param.flags.bits.mainAntenna;
    }
    return PARAM_OK;
}

u32 WSetBeaconSendRecvIndicate(u32 enable) {
    if (enable > 1) {
        return PARAM_RANGE;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.bits.beaconIndicate = (u16)enable;
    }
    return PARAM_OK;
}

u32 WSetNullKeyMode(u32 mode) {
    if (mode > 1) {
        return PARAM_RANGE;
    }
    {
        WlWork *w = wlWork;
        w->param.flags.bits.nullKeyMode = (u16)mode;
    }
    if (mode == 1) {
        WL_REG(0x028) = WL_REG(0x02A);
    }
    return PARAM_OK;
}

// A group BSSID turns the BSSID filter (register 0x0D0 bit 10) off.
u32 WSetBssid(const u16 *bssid) {
    WSetMacAdrs1(wlWork->bss.bssid, bssid);
    WSetMacAdrs1((u16 *)&WL_REG(0x020), bssid);
    if (*bssid & 1) {
        WL_REG(0x0D0) &= ~0x400;
    } else {
        WL_REG(0x0D0) |= 0x400;
    }
    return PARAM_OK;
}

// While the own BSS runs (mode 1) only an SSID of the same length is taken (6 otherwise); it is
// copied into the beacon too.
u32 WSetSsid(u32 length, const u8 *ssid) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    u32 i;
    int changed = FALSE;
    if (length > 32) {
        return PARAM_RANGE;
    }
    if (bss->state == 0x40 && w->param.mode == 1) {
        if (bss->ssidLength != length) {
            return 6;
        }
        if (bss->ssidOffset) {
            changed = TRUE;
        }
    }
    for (i = 0; i < length; i++) {
        u32 byte = WL_ReadByte(ssid);
        WL_WriteByte(bss->ssid + i, byte);
        ssid++;
    }
    for (; i < 32; i++) {
        WL_WriteByte(bss->ssid + i, 0);
    }
    bss->ssidLength = length;
    if (changed) {
        u8 *dest = wlWork->txCtrl.beacon.frame + 0x26 + bss->ssidOffset;
        for (i = 0; i < length; i++) {
            WL_WriteByte(dest + i, WL_ReadByte(bss->ssid + i));
        }
    }
    return PARAM_OK;
}

// In TU (1024 us), 10..1000.
u32 WSetBeaconPeriod(u16 period) {
    if (period < 10 || period > 1000) {
        return PARAM_RANGE;
    }
    wlWork->bss.beaconPeriod = period;
    WL_REG(0x08C) = period;
    WSetFrameLifeTime(wlWork->param.frameLifeTime);
    return PARAM_OK;
}

int WSetDTIMPeriod(u16 period) {
    if (period < 1 || period > 255) {
        return PARAM_RANGE;
    }
    wlWork->bss.dtimPeriod = period;
    WL_REG(0x08E) = period;
    WL_REG(0x088) = 0;
    return PARAM_OK;
}

int WSetListenInterval(u32 interval) {
    if (interval < 1 || interval > 255) {
        return PARAM_RANGE;
    }
    wlWork->bss.listenInterval = interval;
    return PARAM_OK;
}

// MAC address and channel mask from the flash user settings, the rest fixed; seeds the random
// generator from the hardware random register.
void WSetDefaultParameters(void) {
    union {
        struct {
            u16 channels;
            u16 mac[3];
        } fields;

        u32 align[2];
    } flash;

    u32 a, b, c;
    FLASH_Read(0x36, 6, flash.fields.mac);
    FLASH_Read(0x3C, 2, &flash.fields.channels);
    WSetMacAdrs(flash.fields.mac);
    WSetRetryLimit(7);
    WSetEnableChannel((u16)(flash.fields.channels & 0x7FFE));
    WSetMode(2);
    WSetRate(0);
    WSetWepMode(0);
    WSetWepKeyId(0);
    WSetWepKey(def_WepKey);
    WSetBeaconPeriod(500);
    WSetBeaconType(0);
    WSetBcSsidResponse(0);
    WSetBeaconLostThreshold(16);
    WSetActiveZoneTime(0xFFFF, FALSE);
    WSetSsidMask(def_SsidMask);
    WSetPreambleType(1);
    WSetAuthAlgo(0);
    WSetRateSet(c_RateSet$4351);
    WSetCCA_ED(0, 31);
    WSetFrameLifeTime(5);
    WSetDiversity(0, 0);
    WSetMainAntenna(0);
    WSetBeaconSendRecvIndicate(0);
    WSetNullKeyMode(0);
    a = WL_REG(0x044);
    b = WL_REG(0x044);
    c = WL_REG(0x044);
    RND_Init(a + (b << 8), c);
    wlWork->bss.receiveDtims = 1;
}
