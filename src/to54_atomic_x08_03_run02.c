#include "to54_atomic_x08_private.h"

void ov07_0222F210(void *task, X08Work2 *work) {
    switch (work->state) {
    case 0:
        ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
        work->state++;
        break;
    case 1:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            ManagedSprite_SetAnimateFlag(work->sprite, FALSE);
            work->state++;
            ov07_02222508(work->anim0, 10, 10, 0, 8);
            int x = Pokepic_GetAttr(work->pokepic, 0);
            int y = Pokepic_GetAttr(work->pokepic, 1);
            ov07_02222268(work->anim1, x, work->x, y, work->y, 8);
        }
        break;
    case 2:
        ov07_02222314(work->anim1, work->pokepic);
        if (!ov07_02222768(work->anim0, work->pokepic)) {
            Pokepic_SetAttr(work->pokepic, 6, 1);
            Pokepic_SetAttr(work->pokepic, 12, 256);
            Pokepic_SetAttr(work->pokepic, 13, 256);
            ManagedSprite_SetAnim(work->sprite, 1);
            ManagedSprite_SetAnimSpeed(work->sprite, 0x1000);
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
            work->state++;
        }
        break;
    case 3:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            ManagedSprite_SetAnimateFlag(work->sprite, FALSE);
            work->state++;
            ov07_02222268(work->movement, 0, 0, work->y, 0, 8);
        }
        break;
    case 4:
        if (ov07_022222B4(work->movement)) {
            ManagedSprite_SetPositionXY(work->sprite, work->x, *(s16 *)&work->movement[2]);
        } else {
            work->state++;
        }
        break;
    case 5:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}
