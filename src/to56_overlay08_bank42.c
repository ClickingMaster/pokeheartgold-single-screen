#include "to56_overlay08_bank42_private.h"
BOOL ov08_02224BFC(Ov08MenuWork *work) {
    if (work->done == 1) {
        return TRUE;
    }
    if (gSystem.newKeys & 0xF3) {
        work->done = 1;
        ov12_0226BAFC(work->callbackArg,
            work->entries[work->index].values[0],
            work->entries[work->index].values[2],
            work->entries[work->index].values[1],
            work->entries[work->index].values[3]);
        PlaySE(0x5DC);
    }
    return FALSE;
}

BOOL ov08_02224C48(Ov08MenuEntry *entry, u32 direction) {
    switch (direction) {
    case 0:
        if (entry->flags[1] & 0x80) {
            return TRUE;
        }
        break;
    case 1:
        if (entry->flags[0] & 0x80) {
            return TRUE;
        }
        break;
    case 2:
        if (entry->flags[3] & 0x80) {
            return TRUE;
        }
        break;
    case 3:
        if (entry->flags[2] & 0x80) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
