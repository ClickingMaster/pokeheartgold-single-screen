#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0221DD70(void *work, int mode) {
    switch (mode) {
    case 0:
        ov08_0221F4A4(work);
        break;
    case 1:
        ov08_0221F5D0(work);
        break;
    case 2:
        ov08_0221F900(work);
        break;
    case 3:
        ov08_0221F7C0(work);
        break;
    case 4:
        ov08_0221FB18(work);
        break;
    case 5:
        ov08_0221FF70(work);
        break;
    case 6:
    case 8:
        ov08_0221FC7C(work);
        break;
    case 7:
        ov08_0221FDA4(work);
        break;
    case 9:
        ov08_02220084(work);
        break;
    }
}
