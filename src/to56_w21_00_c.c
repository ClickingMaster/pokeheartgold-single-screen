#include "to56_w21_private.h"

int ov08_0221D5DC(u8 *work, int index) {
    if (*(u16 *)(work + index * 0x50 + 8) == 0) {
        return 0;
    }
    if (index == 0) {
        goto return_one;
    }
    if (ov08_0221DAE4(work)) {
        goto check_one;
    }
    if (!ov08_0221DB04(work)) {
        goto return_two;
    }
check_one:
    if (index != 1) {
        goto return_two;
    }
return_one:
    return 1;
return_two:
    return 2;
}
