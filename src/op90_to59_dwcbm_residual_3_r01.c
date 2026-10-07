#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
u8 DWCi_BACKUPlConvMaskCidr(const u8 *mask);
// @fn DWCi_BACKUPlConvMaskCidr
u8 DWCi_BACKUPlConvMaskCidr(const u8 *mask)
{
    int count;
    int i = 0;
    count = 0;
    for (; i < 4; i++) {
        int j;
        int b = mask[i];
        for (j = 0; j < 8; j++) {
            if ((b >> j) & 1) {
                count++;
            }
        }
    }
    return count;
}
