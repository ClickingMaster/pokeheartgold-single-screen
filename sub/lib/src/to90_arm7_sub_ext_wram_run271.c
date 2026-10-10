#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetRetryLimit(u16);
u32 PARAMSET_RetryReqCmd(u16 *p, u16 *c);

u32 PARAMSET_RetryReqCmd(u16 *p, u16 *c) {
    c[1] = 1;
    return WSetRetryLimit(p[8]);
}
