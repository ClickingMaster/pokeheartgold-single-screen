#include "to55_ov07_3061c_private.h"

void ov07_0223061C(void *task, Ov07W10Small *work) {
    switch (work->state) {
    case 0:
        BattleAnimUtil_SetBlendWithBattleBg(work->owner, 0, 16);
        ov07_02222AC4(&work->motion, 0, 16, 16, 0, 8);
        work->state++;
        break;
    case 1:
        if (ov07_02222AF4(&work->motion)) {
            work->state++;
        }
        break;
    case 2:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
            ov07_02222AC4(&work->motion, 16, 0, 0, 16, 8);
        }
        break;
    case 3:
        if (ov07_02222AF4(&work->motion)) {
            work->state++;
        }
        break;
    case 4:
        Sprite_DeleteAndFreeResources(work->sprite);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
