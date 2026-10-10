#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetSsid(u16, const u16 *);
u32 PARAMSET_SSIDReqCmd(u16 *p, u16 *c);

u32 PARAMSET_SSIDReqCmd(u16 *p, u16 *c) {
    c[1] = 1;
    return WSetSsid(p[8], p + 9);
}
