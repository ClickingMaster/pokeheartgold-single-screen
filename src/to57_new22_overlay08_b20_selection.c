#include "to57_new22_overlay08_b20_private.h"
void ov08_022220AC(u8 *work, unsigned int value) {
    work[0x1FA3] &= ~0x70;
    if (value <= 5 && ov08_0221D5DC(work, value) == 2) {
        work[0x1FA3] = (work[0x1FA3] & ~0x70) | 0x10;
    }
    work[0x1FA0] = 0;
    work[0x1FA1] = 0;
    work[0x1FA2] = value;
    work[0x1FA3] |= 0x80;
}
