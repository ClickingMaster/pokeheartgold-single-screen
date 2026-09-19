#include "to52_r4_overlay90_r24_private.h"

typedef struct Ov90W20 {
    u8 pad00[0x14];
    volatile u8 count;
    u8 pad15[0x247];
    u8 entry[0x38];
    u8 pad294[0xA8];
    u8 target[0x38];
} Ov90W20;

BOOL ov90_0225B8F0(Ov90W20 *work) {
    int i;
    BOOL allDone;
    u8 *entry;
    u8 *iter;
    u8 *target;

    allDone = TRUE;
    i = 0;
    if (i < work->count) {
        entry = work->entry;
        iter = (u8 *)work;
        target = entry;
        do {
            if (*(void **)(iter + 0x26C) != NULL) {
                BOOL done = ov90_02259F7C(entry);
                ov90_0225A134(work->target, target);
                if (!done) {
                    allDone = FALSE;
                }
            } else {
                allDone = FALSE;
            }
            iter += 0x38;
            entry += 0x38;
            target += 0x38;
            i++;
        } while (i < work->count);
    }
    return allDone;
}
