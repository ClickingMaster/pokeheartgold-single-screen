#include "global.h"

#include "to56_w08_private.h"

void ov07_02224CB8(void *task, WorkMoveA *w) {
    int a, b, c;
    switch (w->state) {
    case 0:
        if (ov07_02222558(&w->move) == 1) {
            ov07_022227D8(&w->bounce);
            Pokepic_SetAttr(w->pic, 12, w->move.x);
            Pokepic_SetAttr(w->pic, 13, w->move.y);
            Pokepic_AddAttr(w->pic, 0, w->bounce.x);
            Pokepic_SetAttr(w->pic, 1, w->baseY + ov07_02222674(w->baseY, w->unk04, w->move.unk14));
            return;
        }
        w->state++;
        return;
    case 1:
        if (++w->timer <= w->delay) {
            return;
        }
        a = ov07_0221C4A8(w->owner, 0);
        b = ov07_0221C4A8(w->owner, 1);
        c = ov07_0221C4A8(w->owner, 3);
        ov07_02222508(&w->move, a, 100, b, c);
        w->state++;
        return;
    case 2:
        if (ov07_02222558(&w->move) == 1) {
            Pokepic_SetAttr(w->pic, 12, w->move.x);
            Pokepic_SetAttr(w->pic, 13, w->move.y);
            Pokepic_SetAttr(w->pic, 1, w->baseY + ov07_02222674(w->baseY, w->unk04, w->move.unk14));
            return;
        }
        w->state++;
        return;
    case 3:
        if (ov07_02222558(&w->move) == 1) {
            Pokepic_SetAttr(w->pic, 12, w->move.x);
            Pokepic_SetAttr(w->pic, 13, w->move.y);
            Pokepic_SetAttr(w->pic, 1, w->baseY + ov07_02222674(w->baseY, w->unk04, w->move.unk14));
            return;
        }
        Pokepic_SetAttr(w->pic, 12, 256);
        Pokepic_SetAttr(w->pic, 13, 256);
        Pokepic_SetAttr(w->pic, 1, w->baseY);
        w->state++;
        return;
    default:
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        return;
    }
}
