#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetBeaconType(u16);
u32 PARAMSET_BeaconTypeReqCmd(u16 *p, u16 *c);

u32 PARAMSET_BeaconTypeReqCmd(u16 *p, u16 *c) {
    u32 result = 1;
    u16 state;
    c[1] = 1;
    state = *(u16 *)(*(u8 *volatile *)0x0380fff4 + 0x34c);
    if (state <= 32) {
        result = WSetBeaconType(p[8]);
    }
    return result;
}
