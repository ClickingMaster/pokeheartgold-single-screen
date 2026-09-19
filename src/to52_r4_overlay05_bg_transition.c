#include "to52_r4_overlay05_bg_transition_private.h"

BOOL ov05_0221C5A8(W07Ov05Work *work) {
    if (work->direction == 0) {
        return ov05_0221C5C4(work);
    }
    return ov05_0221C6C8(work);
}

BOOL ov05_0221C5C4(W07Ov05Work *work) {
    ScheduleSetBgPosText(work->bgConfig, 2, BG_POS_OP_SUB_X, ov05_0221EA6C[work->frame * 2]);
    ScheduleSetBgPosText(work->bgConfig, 3, BG_POS_OP_SUB_X, ov05_0221EA6C[work->frame * 2]);
    ov05_0221D3AC(work, 0, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 1, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 2, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 3, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 0, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 1, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 2, ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 3, ov05_0221EA6D[work->frame * 2], 0);
    if (work->frame == 0) {
        PlaySE(0x853);
    }
    if (work->frame == 5) {
        work->frame = 0;
        return TRUE;
    }
    work->frame++;
    return FALSE;
}

BOOL ov05_0221C6C8(W07Ov05Work *work) {
    ScheduleSetBgPosText(work->bgConfig, 2, BG_POS_OP_ADD_X, ov05_0221EA6C[work->frame * 2]);
    ScheduleSetBgPosText(work->bgConfig, 3, BG_POS_OP_ADD_X, ov05_0221EA6C[work->frame * 2]);
    ov05_0221D3AC(work, 0, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 1, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 2, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D3AC(work, 3, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 0, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 1, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 2, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    ov05_0221D664(work, 3, (s16)-ov05_0221EA6D[work->frame * 2], 0);
    if (work->frame == 0) {
        PlaySE(0x853);
    }
    if (work->frame == 5) {
        work->frame = 0;
        return TRUE;
    }
    work->frame++;
    return FALSE;
}
