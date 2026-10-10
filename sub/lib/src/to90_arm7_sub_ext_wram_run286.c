#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 WSetFrameLifeTime(u16);
u32 PARAMSET_LifeTimeReqCmd(u16 *p, u16 *c);

u32 PARAMSET_LifeTimeReqCmd(u16 *p, u16 *c) {
    u8 *table = *(u8 **)(*(u8 *volatile *)0x0380fff4 + 0x31c);
    u32 i;
    u16 index;
    c[1] = 1;
    index = p[8];
    if (index >= *(u16 *)(*(u8 *volatile *)0x0380fff4 + 0x322) && index != 65535) {
        return 5;
    }
    if (p[10] > 63 && p[10] != 65535) {
        return 5;
    }
    if (index == 65535) {
        for (i = 1; i < *(u16 *)(*(u8 *volatile *)0x0380fff4 + 0x322); i++) {
            u16 *entry = (u16 *)(table + i * 28);
            entry[13] = p[9];
            if (entry[12]) {
                entry[12] = p[9];
            }
        }
    } else if (index) {
        u8 *active = table + 24;
        *(u16 *)(table + index * 28 + 26) = p[9];
        if (*(u16 *)(active + p[8] * 28)) {
            *(u16 *)(active + p[8] * 28) = p[9];
        }
    }
    if (p[10]) {
        WSetFrameLifeTime(p[10]);
    }
    return 0;
}
