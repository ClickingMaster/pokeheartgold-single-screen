#include "to55_ov07_3061c_private.h"
void ov07_02230714(void *task, Ov07W10Large *work) {
    BOOL done;
    switch (work->state) {
    case 0:
        ov07_02222590(work->motion, (s16)work->field5C, (s16)work->field60, (s16)work->field64, (s16)work->field68, (s16)work->field6C, work->field74 >> 16);
        ov07_0222283C(work->anim, work->y, work->x, work->pokepic);
        ov07_02222788(work->motion, work->pokepic);
        ov07_022226C4(work->pokepic, work->x, work->field0C, *(int *)((u8 *)work + 0x28), 0);
        work->state++;
        return;
    case 1:
        ov07_0222283C(work->anim, work->y, work->x, work->pokepic);
        done = ov07_02222788(work->motion, work->pokepic);
        ov07_022226C4(work->pokepic, work->x, work->field0C, *(int *)((u8 *)work + 0x28), 0);
        if (!done) {
            work->state++;
            return;
        }
        break;
    case 2:
        ov07_02222590(work->motion, (s16)work->field60, (s16)work->field5C, (s16)work->field68, (s16)work->field64, (s16)work->field6C, (u16)work->field74);
        ov07_0222283C(work->anim, work->y, work->x, work->pokepic);
        ov07_02222788(work->motion, work->pokepic);
        ov07_022226C4(work->pokepic, work->x, work->field0C, *(int *)((u8 *)work + 0x28), 0);
        work->state++;
        return;
    case 3:
        ov07_0222283C(work->anim, work->y, work->x, work->pokepic);
        done = ov07_02222788(work->motion, work->pokepic);
        ov07_022226C4(work->pokepic, work->x, work->field0C, *(int *)((u8 *)work + 0x28), 0);
        if (!done) {
            if (--work->timer <= 0) {
                work->state++;
            } else {
                work->state = 0;
            }
            return;
        }
        break;
    case 4:
        Pokepic_SetAttr(work->pokepic, 0, work->y);
        Pokepic_SetAttr(work->pokepic, 1, work->x);
        Pokepic_SetAttr(work->pokepic, 12, 0x100);
        Pokepic_SetAttr(work->pokepic, 13, 0x100);
        Heap_Free(work);
        ov07_0221C448(work->owner, task);
        break;
    }
}

void ov07_022308E4(void *owner) {
    Ov07W10Large *work = ov07_022324D8(owner, sizeof(Ov07W10Large));
    work->owner = owner;
    work->field0C = ov07_0221FAA0(work->owner, ov07_0221C470(owner));
    work->pokepic = ov07_0221FA48(work->owner, ov07_0221C470(work->owner));
    work->x = Pokepic_GetAttr(work->pokepic, 1);
    work->y = Pokepic_GetAttr(work->pokepic, 0);
    work->field5C = 10;
    work->field60 = 7;
    work->field64 = 10;
    work->field68 = 30;
    work->field6C = 10;
    work->timer = 1;
    work->field74 = 0x40003;
    ov07_022227A8(work->anim, 2, 0, 1, 2);
    ov07_0221C410(work->owner, ov07_02230714, work);
}
