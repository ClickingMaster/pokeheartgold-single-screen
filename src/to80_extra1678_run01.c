#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 _021D601C;
extern u8 _021D6020[];
u8 WCMi_GetRssiAverage(void);

u8 WCMi_GetRssiAverage(void) {
    int total = 0, i, count = _021D601C;
    u8 *p;
    if ((u32)count > 16) {
        p = _021D6020;
        i = 0;
        do {
            total += *p++;
            i++;
        } while (i < 16);
        total /= 16;
    } else if (count) {
        i = 0;
        if (count > 0) {
            p = _021D6020;
            do {
                total += *p++;
                i++;
            } while (i < count);
        }
        total /= count;
    }
    return (u8)total;
}
