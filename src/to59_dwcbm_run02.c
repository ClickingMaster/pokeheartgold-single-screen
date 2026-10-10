#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern u8 _021D43C8[];
extern int readNvram(u32 address, u32 len, void *dst);
extern u8 Wifi[];

int DWCi_BACKUPlInit(u8 *buf);
int DWCi_BACKUPlRead(void *dst);

int DWCi_BACKUPlInit(u8 *buf) {
    if (!readNvram(0x20, 0x20, buf)) {
        return 0;
    }
    *(u32 *)(_021D43C8 + 8) = (*(u16 *)buf << 3) - 0x400;
    return 1;
}

int DWCi_BACKUPlRead(void *dst) {
    return readNvram(*(u32 *)(_021D43C8 + 8), 0x400, dst) != 0;
}
