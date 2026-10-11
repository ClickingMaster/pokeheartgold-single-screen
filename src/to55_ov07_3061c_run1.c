#include "to55_ov07_3061c_private.h"

void ov07_0223061C(void *task, Ov07W10Small *work) {
    switch (work->state) {
    case 0:
        BattleAnimUtil_SetBlendWithBattleBg(work->owner, 0, 16);
        BattleAnimUtil_StartBlendAlphaMotion(&work->motion, 0, 16, 16, 0, 8);
        work->state++;
        break;
    case 1:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->motion)) {
            work->state++;
        }
        break;
    case 2:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->motion, 16, 0, 0, 16, 8);
        }
        break;
    case 3:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->motion)) {
            work->state++;
        }
        break;
    case 4:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
