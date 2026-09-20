#include "to55_overlay112_030_private.h"

void ov112_021E8C5C(const u8 *values, void *boxmon, u16 *stats) {
    int first = GetBoxMonData(boxmon, 0xB1, NULL);
    int second = GetBoxMonData(boxmon, 0xB2, NULL);
    int i;
    int found = 0;
    for (i = 0; i < 3; i++) {
        if (first == values[i] || second == values[i]) {
            found = 1;
        }
    }
    if (found) {
        for (i = 0; i < 3; i++) {
            stats[i] = _s32_div_f(stats[i] * 75, 100);
        }
    }
}
