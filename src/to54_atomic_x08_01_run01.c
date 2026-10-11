#include "to54_atomic_x08_private.h"

void ov07_0222EF88(void *task, X08Work *work) {
    int i;
    switch (work->state) {
    case 0:
        ov07_0222EDF0(work);
        ov07_0222EF0C(work->sprites[work->index], &work->animStates[work->index], 0x2000);
        work->state++;
        break;
    case 1: {
        BOOL a = ov07_0222EE14(work);
        BOOL b = ov07_0222EF58(work->sprites[work->index], &work->animStates[work->index]);
        if (a == b && a == TRUE) {
            ManagedSprite_SetAnim(work->sprites[work->index], 1);
            work->state++;
        }
        break;
    }
    case 2:
        work->timers[work->index]--;
        if (work->timers[work->index] < 0) {
            work->index++;
            if (work->index < 6) {
                work->state = 0;
            } else {
                work->state++;
            }
        }
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            ov07_0222EF0C(work->sprites[i], &work->animStates[i], 0x2000);
        }
        work->state++;
        break;
    case 4: {
        BOOL done = FALSE;
        for (i = 0; i < 6; i++) {
            done = ov07_0222EF58(work->sprites[i], &work->animStates[i]);
        }
        if (done == TRUE) {
            work->state++;
        }
        break;
    }
    case 5:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
