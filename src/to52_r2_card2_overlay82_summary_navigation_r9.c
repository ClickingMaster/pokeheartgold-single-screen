#include "overlay82_summary_navigation_r9_internal.h"

void ov82_0223F300(Ov82Work *work) {
    BOOL changed = FALSE;
    u8 kind;

    if (gSystem.newKeys & 0x20) {
        if (ov80_02237920(work->unk00D) != 0xfe) {
            work->unk00C = work->unk00D;
        }
        if (work->unk00D % 4 == 0) {
            work->unk00D += 3;
        } else if (ov80_02237920(work->unk00D) == 0xfe) {
            work->unk00D = 0x10;
        } else {
            work->unk00D--;
        }
        changed = TRUE;
    }
    if (gSystem.newKeys & 0x10) {
        if (ov80_02237920(work->unk00D) != 0xfe) {
            work->unk00C = work->unk00D;
        }
        if (work->unk00D % 4 == 3) {
            work->unk00D -= 3;
        } else if (ov80_02237920(work->unk00D) == 0xfe) {
            work->unk00D = 0x13;
        } else {
            work->unk00D++;
        }
        changed = TRUE;
    }
    if (gSystem.newKeys & 0x40) {
        if (ov80_02237920(work->unk00D) != 0xfe) {
            work->unk00C = work->unk00D;
        }
        if (work->unk00D < 4) {
            work->unk00D += 0x10;
        } else if (ov80_02237920(work->unk00D) == 0xfe) {
            if (work->unk00C == 0x10) {
                work->unk00D = 0xd;
            } else if (work->unk00C == 0x13) {
                work->unk00D = 0xe;
            } else if (work->unk00C == 0xd || work->unk00C == 1) {
                work->unk00D = 0xd;
            } else if (work->unk00C == 0xe || work->unk00C == 2) {
                work->unk00D = 0xe;
            } else {
                work->unk00D = 0xd;
            }
        } else {
            work->unk00D -= 4;
        }
        changed = TRUE;
    }
    if (gSystem.newKeys & 0x80) {
        if (ov80_02237920(work->unk00D) != 0xfe) {
            work->unk00C = work->unk00D;
        }
        if (work->unk00D >= 0x10) {
            work->unk00D -= 0x10;
        } else {
            work->unk00D += 4;
        }
        changed = TRUE;
    }
    if (changed == TRUE) {
        PlaySE(0x5dc);
        ov82_0223FCBC(work->spr204, ov82_0223F558(work), ov82_0223F570(work));
    }
    kind = ov80_02237920(work->unk00D);
    if (kind == 0xfe) {
        ov82_0223FCFC(work->spr204, 2);
        ov82_0223FCBC(work->spr204, 0x80, 0xa8);
    } else {
        ov82_0223FCFC(work->spr204, 1);
    }
}
