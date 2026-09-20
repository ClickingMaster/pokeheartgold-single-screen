#include "overlay08_exact_bank_private.h"

#pragma force_active on

int ov08_0221CA08(u8 *work) {
    ov08_022201C0(work);
    work[0x2079] = 0x10;
    return 0x11;
}

int ov08_0221CA20(u8 *work) {
    ClearFrameAndWindow2(work + 0x2060, FALSE);
    return 2;
}

int ov08_0221CA34(const u8 *work) {
    if (TextPrinterCheckActive(work[0x207B]) == 0) {
        return 0x12;
    }
    return 0x11;
}
