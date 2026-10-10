#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char *_021D5FEC;
u16 WcmGetNextScanChannel(int);

u16 WcmGetNextScanChannel(int channel) {
    int i;
    u32 mask = *(u32 *)(_021D5FEC + 0x2264);
    int next = channel;
    for (i = 0; i < 13; i++, next++) {
        if (mask & (1 << ((next % 13) + 1))) {
            break;
        }
    }
    return ((channel + i) % 13) + 1;
}
