#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetAuthAlgo(u16);
u32 PARAMSET_AuthAlgoReqCmd(u16 *p, u16 *c);

u32 PARAMSET_AuthAlgoReqCmd(u16 *p, u16 *c) {
    c[1] = 1;
    return WSetAuthAlgo(p[8]);
}
