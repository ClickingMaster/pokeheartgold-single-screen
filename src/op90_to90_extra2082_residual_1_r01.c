#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
typedef struct Work {
    void *owner;
    int a;
    int b;
    u8 unk0C[4];
    Pokepic *pic;
    ManagedSprite *sprite;
    u8 unk18[0x4C];
    s16 x;
    s16 y;
    u8 unk68[8];
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
u32 ov07_0221C470(void *owner);
Pokepic *ov07_0221FA48(void *owner, u32 battler);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_0222DEEC(void *task, Work *work);
void ov07_0222E020(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_0222E020
void ov07_0222E020(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    work->owner = owner;
    work->a = a;
    work->b = b;
    work->pic = ov07_0221FA48(work->owner, ov07_0221C470(owner));
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_GetPositionXY(work->sprite, &work->x, &work->y);
    ov07_0221C410(work->owner, ov07_0222DEEC, work);
}
