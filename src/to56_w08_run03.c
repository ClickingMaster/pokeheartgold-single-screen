#include "global.h"

#include "to56_w08_private.h"

void ov07_02224E58(void *ctx) {
    WorkMoveA *w;
    int a;
    w = Heap_Alloc(ov07_0221BFD0(ctx), 0x5c);
    w->pad8 = 0;
    w->state = 0;
    w->owner = ctx;
    w->pic = ov07_0221FA48(w->owner, ov07_0221C468(ctx));
    w->timer = 0;
    w->delay = ov07_0221C4A8(ctx, 4);
    w->baseY = Pokepic_GetAttr(w->pic, 1);
    w->unk04 = ov07_0221FAA0(w->owner, ov07_0221C468(w->owner));
    a = ov07_0221C4A8(ctx, 0);
    ov07_02222508(&w->move, 100, 100, a, ov07_0221C4A8(ctx, 2));
    ov07_022227A8(&w->bounce, 2, 0, 0, 10);
    ov07_0221C410(w->owner, (void (*)(void *, void *))ov07_02224CB8, w);
}
