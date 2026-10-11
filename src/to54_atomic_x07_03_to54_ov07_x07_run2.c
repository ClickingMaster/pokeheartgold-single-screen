#include "to54_atomic_x07_private.h"
BOOL ov07_0222D590(X07ScaleSprite *work);
void ov07_0222D56C(X07ScaleSprite *work);
void ov07_0222D578(X07ScaleSprite *work);
void ov07_0222D60C(void *task, X07Group *work);
BOOL ov07_0222DBD8(X07PicPulse *work);
BOOL ov07_0222DC54(X07SpritePulse *work);

void ov07_0222DA60(void *task, X07Popup *work) {
    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateSinMotionSprite(work->motion, work->x, work->y - work->attr, work->sprite) == FALSE) {
            work->state++;
            work->timer = 16;
        }
        break;
    case 1:
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(work->anim, 0, 16, 16, 0, 32);
        }
        break;
    case 2:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(work->anim)) {
            ManagedSprite_SetDrawFlag(work->sprite, TRUE);
            Pokepic_SetAttr(work->pokepic, 6, 0);
            work->state++;
        }
        break;
    case 3:
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
