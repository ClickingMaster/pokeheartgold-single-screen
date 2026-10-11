#include "to54_atomic_x07_private.h"
void ov07_0222D56C(X07ScaleSprite *work);
void ov07_0222D578(X07ScaleSprite *work);
void ov07_0222DA60(void *task, X07Popup *work);
BOOL ov07_0222DBD8(X07PicPulse *work);
BOOL ov07_0222DC54(X07SpritePulse *work);

BOOL ov07_0222D590(X07ScaleSprite *work) {
    f32 x, y;
    if (BattleAnimUtil_UpdateScaleMotion(work->scaleAnim) == TRUE) {
        BattleAnimUtil_GetScaleFloat(work->scaleAnim, &x, &y);
        if (work->flip != 0) {
            ManagedSprite_SetAffineScale(work->sprite, _fsub(0, x), 1.0f);
        } else {
            ManagedSprite_SetAffineScale(work->sprite, x, 1.0f);
        }
    } else if (work->delay > 0) {
        work->delay--;
        BattleAnimUtil_InitScaleMotion(work->scaleAnim, (s16)work->start, 10, (s16)work->end, 8);
        {
            int tmp = work->start;
            work->start = work->end;
            work->end = tmp;
        }
    } else {
        return FALSE;
    }
    return TRUE;
}

void ov07_0222D60C(void *task, X07Group *work) {
    int i;
    switch (work->state) {
    case 0:
        if (work->timer >= 4) {
            work->timer = 0;
            ov07_0222D578(&work->sprites[work->index]);
            work->index++;
            if (work->index >= 4) {
                work->state++;
            }
        }
        work->timer++;
        break;
    case 1:
        if (BattleAnimUtil_UpdateShakePokepic(work->motion, work->x, work->y, work->pokepic) == FALSE) {
            work->state++;
        }
        break;
    case 2: {
        BOOL running;
        for (i = 0; i < 4; i++) {
            running = ov07_0222D590(&work->sprites[i]);
        }
        if (running == FALSE) {
            work->state++;
        }
        break;
    }
    case 3:
        Pokepic_SetAttr(work->pokepic, 0, work->x);
        Pokepic_SetAttr(work->pokepic, 1, work->y);
        for (i = 0; i < 4; i++) {
            ov07_0222D56C(&work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
