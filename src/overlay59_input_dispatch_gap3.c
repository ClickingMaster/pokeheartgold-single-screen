#include "overlay59_input_dispatch_gap3_private.h"
int ov59_0223B358(Ov59R10Gap3Work *work) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        *(u32 *)&work->raw[0x44] = 0;
    }
    keys = gSystem.newKeys;
    if (keys & 2) {
        return ov59_0223B2B4(work, 1);
    }
    if (keys & 1) {
        return ov59_0223B2B4(work, 0);
    }
    if (keys & 0x40) {
        return ov59_0223B2B4(work, 2);
    }
    if (keys & 0x80) {
        return ov59_0223B2B4(work, 3);
    }
    return 2;
}
