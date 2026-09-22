#include "global.h"

#include "to57_overlay07_slide_private.h"

void ov07_02224998(void *task, WorkSlideA *w) {
    switch (w->state) {
    case 0:
        ov07_02222268(&w->motion, w->x, w->x + w->delta, w->y, w->y, (u16)w->param);
        w->state++;
        break;
    case 1:
        if (ov07_022222B4(&w->motion) == 0) {
            w->state++;
        }
        Pokepic_SetAttr(w->pic, 0, w->motion.x);
        break;
    default:
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        break;
    }
}

void ov07_02224A20(void *ctx) {
    WorkSlideA *w = ov07_022324D8(ctx, sizeof(WorkSlideA));
    int kind, picno;
    w->owner = ctx;
    w->param = ov07_0221C4A8(ctx, 0);
    w->delta = ov07_0221C4A8(ctx, 1);
    kind = ov07_0221C4A8(ctx, 2);
    picno = 0xff;
    switch (kind) {
    case 0x102:
        picno = ov07_0221C468(ctx);
        break;
    case 0x104:
        if (ov07_0221FAB0(ctx) == 1) {
            picno = ov07_0223197C(ctx, ov07_0221C468(ctx));
        }
        break;
    case 0x108:
        picno = ov07_0221C470(ctx);
        break;
    case 0x110:
        if (ov07_0221FAB0(ctx) == 1) {
            picno = ov07_0223197C(ctx, ov07_0221C470(ctx));
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (picno == 0xff) {
        Heap_Free(w);
        return;
    }
    ov07_02231FE4(ctx, w->cfg);
    w->pic = ov07_0221FA48(ctx, picno);
    ov07_02231FA0(w->pic, &w->x);
    if (ov07_02222004(ctx, picno) <= 0) {
        w->delta *= -1;
    }
    ov07_0221C410(w->owner, (void (*)(void *, void *))ov07_02224998, w);
}

void ov07_02224B14(void *task, WorkSlideB *w) {
    switch (w->state) {
    case 0:
        if (w->pic == NULL) {
            w->state = 0xff;
            return;
        }
        ov07_02222268(&w->motion, w->x, w->x + w->delta, w->y, w->y, (u16)w->param);
        w->state++;
    case 1:
        if (ov07_022222B4(&w->motion) == 0) {
            w->state++;
        }
        Pokepic_SetAttr(w->pic, 0, w->motion.x);
        break;
    default:
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        break;
    }
}

void ov07_02224BAC(void *ctx) {
    WorkSlideB *w = ov07_022324D8(ctx, sizeof(WorkSlideB));
    int kind, picno;
    w->owner = ctx;
    w->param = ov07_0221C4A8(ctx, 0);
    w->delta = ov07_0221C4A8(ctx, 1);
    kind = ov07_0221C4A8(ctx, 2);
    ov07_02231FE4(ctx, w->cfg);
    picno = 0xff;
    switch (kind) {
    case 0x102:
        picno = ov07_0221C468(ctx);
        break;
    case 0x104:
        if (ov07_0221FAB0(ctx) == 1) {
            picno = ov07_0223197C(ctx, ov07_0221C468(ctx));
        }
        break;
    case 0x108:
        picno = ov07_0221C470(ctx);
        break;
    case 0x110:
        if (ov07_0221FAB0(ctx) == 1) {
            picno = ov07_0223197C(ctx, ov07_0221C470(ctx));
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (picno == 0xff) {
        Heap_Free(w);
        return;
    }
    w->pic = ov07_0221FA48(ctx, picno);
    ov07_02231FA0(w->pic, &w->x);
    if (ov07_02222004(ctx, picno) <= 0) {
        w->delta *= -1;
    }
    ov07_0221BFC0(ctx);
    ov07_02224B14(ov07_0221C410(w->owner, (void (*)(void *, void *))ov07_02224B14, w), w);
}
