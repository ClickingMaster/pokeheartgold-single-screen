#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0221F550(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    FillWindowPixelRect(&((Window *)work->windowArray)[battler], 0, 0x38, 0x20, 0x18, 8);
    FillWindowPixelRect(&((Window *)work->windowArray)[battler], 0, 0x40, 0x18, 0x40, 8);
    ov08_0221DFC8(work, battler, battler, 0x38, 0x20);
    ov08_0221E048(work, battler, battler, 0x40, 0x18);
}
