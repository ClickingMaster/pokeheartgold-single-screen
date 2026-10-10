#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetDiversity(u16, u16);
u32 PARAMSET_DiversityReqCmd(u16 *p, u16 *c);

u32 PARAMSET_DiversityReqCmd(u16 *p, u16 *c) {
    c[1] = 1;
    if (*(u16 *)(*(u8 *volatile *)0x0380fff4 + 0x34c) < 16) {
        return 1;
    }
    return WSetDiversity(p[8], p[9]);
}
