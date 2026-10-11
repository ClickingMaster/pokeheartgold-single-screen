#include "to55_r2_t13_private.h"

typedef struct CoordBlob {
    s16 x;
    s16 y;
    u8 rest[0x30];
} CoordBlob;

extern void BattleAnimSystem_GetSpriteTemplate(CoordBlob *, void *);

void ov07_0222C4C0(void *owner, void *a, void *b, void *sprite) {
    int i;
    int idx;
    int v;
    CoordBlob blob;
    MainWork *w;

    w = Heap_Alloc(BattleAnimSystem_GetHeapID(owner), 0x100);
    w->owner = owner;
    w->unk4 = a;
    w->spriteSystem = b;
    w->timer = 8;
    w->index = 0;
    w->state = 0;
    idx = BattleAnimSystem_GetAttacker(owner);
    v = ov07_02222004(owner, idx);
    w->sprite = sprite;
    w->unk50 = 0;
    BattleAnimUtil_InitSinMotionFromStep(&w->sin, 0x3fff, 0xbfff, 0x3fff, 0xbfff, -(v << 17), 0xffff8000, 0x71c);
    BattleAnimUtil_InitCosMotion(&w->linear, 0, 0xffff, 0xfffff000, w->sin.duration);
    w->baseX = w->sin.stepX;
    w->unk54 = 1;
    w->x = BattleAnimSystem_GetBattlerScreenCoord(owner, idx, 0);
    w->y = BattleAnimSystem_GetBattlerScreenCoord(owner, idx, 1);
    ManagedSprite_SetPositionXY(w->sprite, w->x, w->y);
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    BattleAnimSystem_GetSpriteTemplate(&blob, w->owner);
    blob.x = w->x;
    blob.y = w->y;
    for (i = 0; i < 2; i++) {
        ov07_0222C18C(&w->child[i], w->unk4, w->spriteSystem, &blob, &w->sprite);
    }
    w->blend = 0;
    BattleAnimUtil_SetBlendWithBattleBg(w->owner, 0, 31);
    ManagedSprite_SetOamMode(w->sprite, 1);
    for (i = 0; i < 2; i++) {
        ManagedSprite_SetOamMode(w->child[i].sprite, 1);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(w->owner, ov07_0222C2D4, w);
}
