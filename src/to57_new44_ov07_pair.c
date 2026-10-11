#include "to57_new44_private.h"

void ov07_0222D6E0(void *owner, void *spriteSystem, void *spriteManager, void *resource) {
    X07ManagedSpriteTemplate template;
    X07Group *work;
    int y;
    int flip;
    int i;

    work = BattleAnimUtil_AllocWork(owner, sizeof(X07Group));
    work->owner = owner;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = ov07_0221FA48(work->owner, ov07_0221C470(owner));
    work->x = Pokepic_GetAttr(work->pokepic, 0);
    work->y = Pokepic_GetAttr(work->pokepic, 1);
    ov07_022227A8(work->motion, 4, 0, 1, 1);
    y = work->y + 16;
    flip = 0;
    ov07_0221F9E8(&template, work->owner);
    for (i = flip; i < 4; i++) {
        if (i == 0) {
            ov07_0222D4F4(&work->sprites[i], resource, work->x, y, flip);
        } else {
            void *sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
            ov07_0222D4F4(&work->sprites[i], sprite, work->x, y, flip);
        }
        flip ^= 1;
        y -= 10;
    }
    ov07_0221C410(work->owner, (void (*)(void *, void *))ov07_0222D60C, work);
}

void ov07_0222D7A4(void *task, X07PairWork *work) {
    switch (work->state) {
    case 0:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic1, work->x1, work->y1);
            return;
        }
        ov07_02222268(work->movement, *(s16 *)work->movement, 0, 0, 0, 2);
        work->state++;
        return;
    case 1:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic1, work->x1, work->y1);
            return;
        }
        Pokepic_SetAttr(work->pic1, 0, work->x1);
        Pokepic_SetAttr(work->pic1, 1, work->y1);
        ov07_02222268(work->movement, 0, -(work->distance << 5), 0, 0, 4);
        work->state++;
        return;
    case 2:
        if (!ov07_022223CC(work->linear, work->sine, work->sprite)) {
            ov07_02222338(work->linear, work->sine, work->x2, work->x1, work->y2, work->y1, 10, -2 << 16);
            work->state++;
        }
        break;
    case 3:
        if (!ov07_022223CC(work->linear, work->sine, work->sprite)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    case 4:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic1, work->x1, work->y1);
        } else {
            ov07_02222268(work->movement, *(s16 *)work->movement, 0, 0, 0, 4);
            work->state++;
        }
        break;
    case 5:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic1, work->x1, work->y1);
        } else {
            Pokepic_SetAttr(work->pic1, 0, work->x1);
            Pokepic_SetAttr(work->pic1, 1, work->y1);
            work->state++;
        }
        break;
    case 6:
        Sprite_DeleteAndFreeResources(work->sprite);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}
