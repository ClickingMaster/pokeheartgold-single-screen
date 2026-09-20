#include "to56_r2_w13_private.h"
#pragma force_active on

void ov07_0222D050(void *arg0, R2W13Transition *w) {
    switch (w->state) {
    case 0:
        ov07_0222CFCC(w);
        if (ManagedSprite_GetDrawFlag(w->sprite) == TRUE) {
            int value = ov07_0221C470(w->owner);
            if (ov07_0221FAC8(w->owner, value) == 0) {
                Pokepic_SetAttr(w->pokepic, 6, 1);
            }
        }
        w->state++;
        break;
    case 1:
        if (ov07_0222CFCC(w)) {
            w->state++;
        }
        break;
    case 2: {
        int bg = ov07_0221FAEC(w->owner, 1);
        int priority = ov07_0221FB04(w->owner, 1);
        SetBgPriority((u8)bg, (u8)priority);
        SetBgPriority(0, (u8)ov07_0221FAE8(w->owner));
        int value = ov07_0221C470(w->owner);
        if (ov07_0221FAC8(w->owner, value) == 0) {
            Pokepic_SetAttr(w->pokepic, 6, 0);
        }
        bg = ov07_0221FAEC(w->owner, 1);
        BG_ClearCharDataRange((u8)bg, 1 << 14, 0, ov07_0221BFD0(w->owner));
        BgClearTilemapBufferAndCommit(ov07_0221C4A0(w->owner), (u8)ov07_0221FAEC(w->owner, 1));
        ov07_0221C448(w->owner, arg0);
        Heap_Free(w);
        break;
    }
    }
}
