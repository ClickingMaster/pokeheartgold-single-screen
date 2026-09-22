#include "to55_r2_t13_private.h"

typedef struct CoordBlob {
    s16 x;
    s16 y;
    u8 rest[0x30];
} CoordBlob;

extern void ov07_0221F9E8(CoordBlob *, void *);

void ov07_0222C4C0(void *owner, void *a, void *b, void *sprite) {
    int i;
    int idx;
    int v;
    CoordBlob blob;
    MainWork *w;

    w = Heap_Alloc(ov07_0221BFD0(owner), 0x100);
    w->owner = owner;
    w->unk4 = a;
    w->spriteSystem = b;
    w->timer = 8;
    w->index = 0;
    w->state = 0;
    idx = ov07_0221C468(owner);
    v = ov07_02222004(owner, idx);
    w->sprite = sprite;
    w->unk50 = 0;
    ov07_0222212C(&w->sin, 0x3fff, 0xbfff, 0x3fff, 0xbfff, -(v << 17), 0xffff8000, 0x71c);
    ov07_02222494(&w->linear, 0, 0xffff, 0xfffff000, w->sin.duration);
    w->baseX = w->sin.stepX;
    w->unk54 = 1;
    w->x = ov07_02221F80(owner, idx, 0);
    w->y = ov07_02221F80(owner, idx, 1);
    ManagedSprite_SetPositionXY(w->sprite, w->x, w->y);
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    ov07_0221F9E8(&blob, w->owner);
    blob.x = w->x;
    blob.y = w->y;
    for (i = 0; i < 2; i++) {
        ov07_0222C18C(&w->child[i], w->unk4, w->spriteSystem, &blob, &w->sprite);
    }
    w->blend = 0;
    ov07_02231E08(w->owner, 0, 31);
    ManagedSprite_SetOamMode(w->sprite, 1);
    for (i = 0; i < 2; i++) {
        ManagedSprite_SetOamMode(w->child[i].sprite, 1);
    }
    ov07_0221C410(w->owner, ov07_0222C2D4, w);
}
