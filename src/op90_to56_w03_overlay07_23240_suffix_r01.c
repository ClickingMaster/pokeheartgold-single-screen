#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
typedef struct Work {
    void *owner;
    int a;
    int b;
    u8 unk0C[8];
    ManagedSprite *sprite;
    u8 unk18[0x24];
    int height;
    u8 unk40[4];
    int frames;
    u8 unk48[4];
    s16 x;
    s16 y;
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
u32 ov07_0221C468(void *owner);
u32 ov07_0221C470(void *owner);
int ov07_02222004(void *owner, u32 battler);
Pokepic *ov07_0221FA48(void *owner, u32 battler);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_02223DC4(void *task, Work *work);
void ov07_02223E08(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_02223E08
void ov07_02223E08(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    Pokepic *pic;
    work->owner = owner;
    work->a = a;
    work->b = b;
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
    work->frames = 4;
    work->height = ov07_02222004(work->owner, ov07_0221C468(work->owner));
    pic = ov07_0221FA48(work->owner, ov07_0221C470(owner));
    work->x = Pokepic_GetAttr(pic, 0);
    work->y = Pokepic_GetAttr(pic, 1);
    ov07_0221C410(work->owner, ov07_02223DC4, work);
}
