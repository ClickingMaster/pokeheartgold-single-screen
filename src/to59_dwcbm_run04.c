#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern u8 _021D43C8[];
extern int readNvram(u32 address, u32 len, void *dst);
extern u8 Wifi[];

void DWCi_BACKUPlConvMaskAddr(u32 cidr, u8 *out);
int DWC_BACKUPlCheckSsid(const u8 *ssid);
int DWC_BACKUPlCheckIp(const u8 *addr, const u8 *mask);
int DWC_BACKUPlCheckAddress(const u8 *addr);

void DWCi_BACKUPlConvMaskAddr(u32 cidr, u8 *out) {
    u32 mask = ~0u ^ (~0u >> cidr);
    int i = 0;
    int shift = 0;
    for (; i < 4; i++) {
        out[i] = mask >> (24 - shift);
        shift += 8;
    }
}

int DWC_BACKUPlCheckSsid(const u8 *ssid) {
    int i;
    for (i = 0; i < 32; i++) {
        if (ssid[i] != 0) {
            return 1;
        }
    }
    return 0;
}

int DWC_BACKUPlCheckIp(const u8 *addr, const u8 *mask) {
    u32 addrValue;
    u32 maskValue;
    if (!DWC_BACKUPlCheckAddress(addr)) {
        return 0;
    }
    MI_CpuCopy8(addr, &addrValue, 4);
    MI_CpuCopy8(mask, &maskValue, 4);
    if ((addrValue | maskValue) == ~1u) {
        return 0;
    }
    return (addrValue & ~maskValue) != 0;
}

int DWC_BACKUPlCheckAddress(const u8 *addr) {
    u8 n = addr[0];
    if (n == 127) {
        return 0;
    }
    if (n < 1) {
        return 0;
    }
    if (n <= 223) {
        return 1;
    }
    return 0;
}
