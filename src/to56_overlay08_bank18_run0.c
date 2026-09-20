#include "../include/to56_overlay08_bank18_private.h"
#pragma force_active on

void ov08_02220C5C(Ov08Work *work, u32 mode) {
    u32 i;
    for (i = 0; i < 38; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
    }
    switch (mode) {
    case 0:
        ov08_02220CF4(work);
        break;
    case 1:
        ov08_02220D90(work);
        break;
    case 2:
        ov08_02220DE8(work);
        break;
    case 3:
        ov08_02220E80(work);
        break;
    case 4:
        ov08_02220F58(work);
        break;
    case 5:
        ov08_0222114C(work);
        break;
    case 6:
        ov08_0222101C(work);
        break;
    case 7:
        ov08_02221088(work);
        break;
    case 8:
        ov08_02221230(work);
        break;
    case 9:
        ov08_022211B8(work);
        break;
    }
}
