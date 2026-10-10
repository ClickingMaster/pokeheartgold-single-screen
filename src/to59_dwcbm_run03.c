#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern u8 _021D43C8[];
extern int readNvram(u32 address, u32 len, void *dst);
extern u8 Wifi[];

void DWCi_BACKUPlSetWiFi(const u8 *src);

void DWCi_BACKUPlSetWiFi(const u8 *src) {
    MI_CpuCopy8(src, Wifi, 14);
}
