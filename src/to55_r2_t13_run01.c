#include "to55_r2_t13_private.h"

BOOL ov07_0222C214(Child *w) {
    if (!w->active) {
        return TRUE;
    }
    switch (w->state) {
    case 0:
    case 1:
        if (ov07_02222180(&w->sin)) {
            ov07_022224AC(&w->linear);
            w->sin.stepX = w->offX + w->linear.value;
            ManagedSprite_SetPositionXY(w->sprite, w->offY + w->sin.x, w->offZ + w->sin.y);
        } else {
            if (w->state < 1) {
                if (w->state == 0) {
                    ov07_0222212C(&w->sin, w->sin.angleX, 0x7fff, (u16)w->sin.angleX, 0x7fff, w->sin.ampX, w->sin.ampY, 0x71c);
                }
                ov07_02222494(&w->linear, 0, 0xffff, 0xfffff000, w->sin.duration);
            }
            w->state++;
        }
        break;
    case 2:
        return FALSE;
    }
    return TRUE;
}

void ov07_0222C2D4(void *task, MainWork *w) {
    int i;
    BOOL active;
    switch (w->state) {
    case 0:
        ov07_0222C214((Child *)&w->sprite);
        w->state++;
        break;
    case 1:
        w->blend += 10;
        *(volatile u16 *)0x04000052 = ((31 - w->blend / 10) << 8) | (w->blend / 10);
        if (w->blend >= 310) {
            ManagedSprite_SetOamMode(w->sprite, 0);
            w->state++;
        }
        break;
    case 2:
        *(volatile u16 *)0x04000052 = 0xffff;
        w->state++;
        break;
    case 3:
        if (w->timer <= 0) {
            w->child[w->index].active = TRUE;
            ManagedSprite_SetDrawFlag(w->child[w->index].sprite, TRUE);
            w->index++;
            w->timer = 8;
        }
        if (w->index < 2) {
            w->timer--;
        }
        active = ov07_0222C214((Child *)&w->sprite);
        for (i = 0; i < 2; i++) {
            BOOL x = ov07_0222C214(&w->child[i]);
            if (!x) {
                ov07_0222C208(&w->child[i], 0);
            }
            active |= x;
        }
        if (!active) {
            w->state++;
        }
        break;
    case 4:
        ManagedSprite_TickFrame(w->sprite);
        if (!Sprite_IsAnimated(*(void **)w->sprite)) {
            ManagedSprite_SetOamMode(w->sprite, 1);
            w->state++;
        }
        break;
    case 5:
        w->blend -= 10;
        *(volatile u16 *)0x04000052 = ((31 - w->blend / 10) << 8) | (w->blend / 10);
        if (w->blend <= 0) {
            w->state++;
        }
        break;
    case 6:
        for (i = 0; i < 2; i++) {
            ov07_0222C1FC(&w->child[i]);
        }
        Sprite_DeleteAndFreeResources(w->sprite);
        BattleAnimSystem_DestroyTask(w->owner, task);
        Heap_Free(w);
        return;
    }
    SpriteSystem_DrawSprites(w->spriteSystem);
}
