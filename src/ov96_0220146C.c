#include "ov96_0220146C_private.h"

void *ov96_0220146C(Work17 *work) {
    void *sorted[16];
    void *input[16];
    int i;
    s32 count;
    u8 index;

    for (i = 0; i < 16; i++) {
        input[i] = &work->entries[i];
    }
    count = ov96_022014A4(input, sorted);
    index = (u8)((s32)LCRandom() % count);
    return sorted[index];
}
