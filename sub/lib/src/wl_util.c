#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Bit of each rate (500 kbit/s units, 1..120) in a WlRateSet; 0xFF: no bit.
extern const u16 RateElement2Bit[];

// -ipa file would otherwise inline WUpdateCounter into WInitCounter.
#pragma dont_inline on

void WInitCounter(void) {
    WUpdateCounter();
    __VENEER_MIi_CpuClear32(0, wlWorkPlain->counter, sizeof(wlWorkPlain->counter));
}

#pragma dont_inline reset

// clang-format off
asm void __VENEER_MIi_CpuClear32(u32 data, void *dest, u32 size) {
    ldr pc, =MIi_CpuClear32
}
// clang-format on

// Adds the hardware statistics counters (0x048081B0.., one count per byte) to WlWork.counter.
void WUpdateCounter(void) {
    WlWork *w = wlWorkPlain;
    vu16 *reg = &WL_REG(0x1B0);
    int value;
    value = reg[0];
    w->counter[20] += value & 0xFF;
    value = reg[1];
    w->counter[19] += value >> 8;
    w->counter[23] += value & 0xFF;
    value = reg[2];
    w->counter[22] += value >> 8;
    w->counter[21] += value & 0xFF;
    value = reg[3];
    w->counter[24] += value >> 8;
    w->counter[14] += value & 0xFF;
    value = reg[4];
    w->counter[18] += value & 0xFF;
    value = reg[5];
    w->counter[8] += value & 0xFF;
    value = reg[6];
    w->counter[12] += value >> 8;
    w->counter[17] += value & 0xFF;
    value = reg[7];
    w->counter[15] += value & 0xFF;
    w->counter[16] += value >> 8;
    value = reg[8];
    w->counter[3] += value & 0xFF;
    value = reg[16];
    w->counter[30] += value >> 8;
    value = reg[17];
    w->counter[31] += value & 0xFF;
    w->counter[32] += value >> 8;
    value = reg[18];
    w->counter[33] += value & 0xFF;
    w->counter[34] += value >> 8;
    value = reg[19];
    w->counter[35] += value & 0xFF;
    w->counter[36] += value >> 8;
    value = reg[20];
    w->counter[37] += value & 0xFF;
    w->counter[38] += value >> 8;
    value = reg[21];
    w->counter[39] += value & 0xFF;
    w->counter[40] += value >> 8;
    value = reg[22];
    w->counter[41] += value & 0xFF;
    w->counter[42] += value >> 8;
    value = reg[23];
    w->counter[43] += value & 0xFF;
    w->counter[44] += value >> 8;
}

// Whether an SSID matches the own one under the SSID mask (an empty own SSID matches any).
int WCheckSSID(u32 length, const u8 *ssid) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    u32 ownLength;
    u8 *own;
    u8 *mask;
    u32 i;
    if (length > 32) {
        return 0;
    }
    ownLength = bss->ssidLength;
    if (!ownLength) {
        return 1;
    }
    if (w->mlme.state == 0x13) {
        if (length < ownLength) {
            return 0;
        }
        length = ownLength;
    } else if (length != ownLength) {
        return 0;
    }
    own = bss->ssid;
    mask = (u8 *)bss->ssidMask;
    for (i = 0; i < length; i++) {
        u32 maskByte = WL_ReadByte(mask);
        mask++;
        u32 byte = WL_ReadByte(ssid);
        ssid++;
        u32 ownByte = WL_ReadByte(own);
        own++;
        if ((byte | maskByte) != (ownByte | maskByte)) {
            return 0;
        }
    }
    return 1;
}

int MatchMacAdrs(const u16 *addr1, const u16 *addr2) {
    return addr1[2] == addr2[2] && addr1[1] == addr2[1] && addr1[0] == addr2[0];
}

u32 CheckEnableChannel(u32 channel) {
    return wlWorkPlain->param.enableChannel & (1u << channel);
}

// Supported rates information element: id, length, rates (bit 7: basic rate).
typedef struct WlRatesElement {
    u8 id;
    u8 length;
    u8 rates[];
} WlRatesElement;

static inline void WlRateSet_Add(WlRateSet *rateSet, u32 rate) {
    u32 index = (rate & 0x7F) - 1;
    u32 bit;
    if (index < 120 && (bit = RateElement2Bit[index]) != 0xFF) {
        rateSet->supported |= 1u << bit;
        if (rate & 0x80) {
            rateSet->basic |= 1u << bit;
        }
    } else {
        rateSet->supported |= 0x8000;
        if (rate & 0x80) {
            rateSet->basic |= 0x8000;
        }
    }
}

void WElement2RateSet(const u8 *element, WlRateSet *rateSet) {
    const u8 *rates;
    u32 i, count;
    const WlRatesElement *ratesElement;
    WlRateSet *out;
    ratesElement = (const WlRatesElement *)element;
    out = rateSet;
    out->basic = 0;
    out->supported = 0;
    count = WL_ReadByte(&ratesElement->length);
    rates = ratesElement->rates;
    for (i = 0; i < count; i++) {
        WlRateSet_Add(out, WL_ReadByte(rates + i));
    }
}

// Rate of management frames: 10 (1 Mbit/s) or 20 (2 Mbit/s).
u32 WCalcManRate(void) {
    WlWork *w = wlWorkPlain;
    switch (w->param.rate) {
    case 0:
        if (w->bss.rateSet.basic & 1) {
            return 10;
        }
        break;
    case 1:
        return 10;
    }
    return 20;
}
