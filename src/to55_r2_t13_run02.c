#include "to55_r2_t13_private.h"

void ov07_0222C610(PicWork *w) {
    f32 x, y;
    Pokepic_SetAttr(w->pokepic, 0, w->baseX + w->sin.x);
    Pokepic_SetAttr(w->pokepic, 1, w->baseY + w->sin.y);
    ManagedSprite_SetPositionXY(w->sprite, w->baseX + w->sin.x, w->baseY + w->sin.y - w->deltaY);
    ov07_02222644(&w->linear, &x, &y);
    x += (f32)w->pad64 * 0.1f;
    ManagedSprite_SetAffineScale(w->sprite, x, y);
    w->pad64 *= -1;
}

void ov07_0222C6A8(void *task, PicWork *w) {
    switch (w->state) {
    case 0: {
        ov07_022227D8(&w->sin);
        BOOL active = ov07_0222260C(&w->linear);
        ov07_0222C610(w);
        if (!active) {
            ov07_02222590(&w->linear, 12, 10, 15, 10, 10, 4);
            w->state++;
        }
        break;
    }
    case 1: {
        ov07_022227D8(&w->sin);
        BOOL active = ov07_0222260C(&w->linear);
        ov07_0222C610(w);
        if (!active) {
            Pokepic_SetAttr(w->pokepic, 0, w->baseX);
            Pokepic_SetAttr(w->pokepic, 1, w->baseY);
            ManagedSprite_SetOamMode(w->sprite, 0);
            ManagedSprite_SetPositionXY(w->sprite, w->baseX, w->baseY - w->deltaY);
            ManagedSprite_SetAffineScale(w->sprite, 1.0f, 1.0f);
            w->state++;
        }
        break;
    }
    case 2:
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        return;
    }
    SpriteSystem_DrawSprites(w->spriteSystem);
}

void ov07_0222C780(void *owner) {
    PicWork *w = Heap_Alloc(ov07_0221BFD0(owner), 0x68);
    w->owner = owner;
    w->spriteSystem = ov07_0221C514(owner);
    w->state = 0;
    w->unkC = 0;
    int idx = ov07_0221C470(w->owner);
    w->pokepic = ov07_0221FA48(w->owner, (void *)idx);
    w->baseX = Pokepic_GetAttr(w->pokepic, 0);
    w->baseY = Pokepic_GetAttr(w->pokepic, 1);
    w->deltaY = Pokepic_GetAttr(w->pokepic, 0x29);
    ov07_022227A8(&w->sin, 2, 0, 1, 6);
    w->sprite = ov07_0221C4E8(w->owner, 0);
    ManagedSprite_SetOamMode(w->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(w->sprite, 2);
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    ov07_02222590(&w->linear, 10, 12, 10, 15, 10, 7);
    w->pad64 = 1;
    ov07_02231E08(w->owner, 28, 15);
    ov07_0221C410(w->owner, ov07_0222C6A8, w);
}
