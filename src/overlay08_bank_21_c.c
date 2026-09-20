#include "overlay08_exact_bank_private.h"

#pragma force_active on

int ov08_02222D78(void *work) {
    ov08_02223300(work, 0);
    return 1;
}

int ov08_02222D84(void *work) {
    ov08_02223300(work, 1);
    return 2;
}

int ov08_02222D90(void *work) {
    ov08_02223300(work, 2);
    return 3;
}

int ov08_02222D9C(u8 *work) {
    ClearFrameAndWindow2(work + 0x1C, FALSE);
    return 3;
}

int ov08_02222DAC(const u8 *work) {
    if (TextPrinterCheckActive(work[0x32]) == 0) {
        return 10;
    }
    return 9;
}
