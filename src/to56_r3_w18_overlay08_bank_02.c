#include "to56_r3_w18_overlay08_bank_02_private.h"
#pragma force_active on
int ov08_0221C954(Ov08WorkR3W18 *work) {
    if (work->mode == 0) {
        ov08_0221D840(work, 6);
    } else {
        ov08_0221D840(work, 8);
    }
    return 19;
}
int ov08_0221C978(Ov08WorkR3W18 *work) {
    ov08_022213C8(work);
    if (work->mode == 0) {
        ov08_0221D840(work, 7);
    } else {
        ov08_0221D840(work, 9);
    }
    return 20;
}
int ov08_0221C9A4(Ov08OwnerR3W18 **work) {
    ov08_0221D840(work, 5);
    if (GetItemAttr((*work)->item, 0x25, (*work)->heapId) != 0) {
        return 24;
    }
    return 21;
}
int ov08_0221C9C8(Ov08WorkR3W18 *work) {
    ov08_02220C5C(work, work->selection);
    ov08_0221DD70(work, work->selection);
    ov08_022221CC(work, work->selection);
    ov08_0221D6CC(work, work->selection);
    if (work->selection == 2) {
        return 3;
    }
    return 4;
}
