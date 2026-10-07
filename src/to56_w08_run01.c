#include "global.h"

#include "to56_w08_private.h"

void ov07_022248F4(void *task, WorkBlink *w) {
    if (w->timer >= w->limit) {
        if (w->count == 0) {
            Pokepic_SetAttr(w->pic, 6, 0);
            Heap_Free(w);
            ov07_0221C448(w->owner, task);
        } else {
            Pokepic_SetAttr(w->pic, 6, 1 ^ Pokepic_GetAttr(w->pic, 6));
            w->count--;
            w->timer = 0;
        }
    } else {
        w->timer++;
    }
}

void ov07_02224948(void *ctx) {
    WorkBlink *w = ov07_022324D8(ctx, sizeof(WorkBlink));
    w->owner = ctx;
    w->count = ((s32)ov07_0221C4A8(ctx, 0)) * 2;
    w->limit = ((s32)ov07_0221C4A8(ctx, 1));
    ov07_02231FE4(ctx, &w->motion);
    w->pic = ov07_0221FA48(ctx, ov07_0221C468(ctx));
    ov07_0221C410(w->owner, (void (*)(void *, void *))ov07_022248F4, w);
}
