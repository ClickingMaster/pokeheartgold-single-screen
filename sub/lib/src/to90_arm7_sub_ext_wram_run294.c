#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetBeaconPeriod(u16);
u32 PARAMSET_BeaconPeriodReqCmd(u16 *p, u16 *c);

u32 PARAMSET_BeaconPeriodReqCmd(u16 *p, u16 *c) {
    c[1] = 1;
    if (*(u16 *)(*(u8 *volatile *)0x0380fff4 + 0x32e) != 1) {
        return 11;
    }
    return WSetBeaconPeriod(p[8]);
}
