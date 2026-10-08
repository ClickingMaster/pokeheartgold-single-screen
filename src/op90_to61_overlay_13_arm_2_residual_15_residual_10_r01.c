#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern u8 *ov13_0224F5C8;
extern void MB_End(void);
int ov13_0223E8FC(void);
#define B ov13_0224F5C8
#define B32(o) (*(u32 *)(B + (o)))
#define B8(o) (B[(o)])
// @fn ov13_0223E8FC
int ov13_0223E8FC(void) {
    u8 *b = B;
    u8 state = b[0xa90];
    if (state == 1 || state == 20 || state == 23 || state == 26 || state == 29) {
        b[0xa90] = 34;
        B8(0xaac) = 0;
        return 1;
    }
    if (state == 4 || state == 5 || state == 6 || state == 13) {
        if (state == 4 && *(u32 *)(b + 0xa98) < 6) {
            return 0;
        }
        MB_End();
        B8(0xa90) = 16;
        B8(0xaac) = 2;
        return 1;
    }
    if (state == 9 || state == 10) {
        b[0xa90] = 32;
        return 1;
    }
    if (state == 12) {
        b[0xa90] = 34;
        return 1;
    }
    return 0;
}
