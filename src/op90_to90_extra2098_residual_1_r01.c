#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Work {
    u8 unk00[4];
    void *owner;
    u8 unk08[0x14];
    ManagedSprite *sprite;
    u8 unk20[0x4C];
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
void BattleAnimUtil_InitCommonWork(void *owner, void *work);
int ov07_0221FB04(void *owner, int a1);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_02226B2C(void *task, Work *work);
void ov07_02226C4C(void *owner, int a1, int a2, ManagedSprite *sprite);

// @fn ov07_02226C4C
void ov07_02226C4C(void *owner, int a1, int a2, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    BattleAnimUtil_InitCommonWork(owner, work);
    work->sprite = sprite;
    ManagedSprite_SetPositionXY(work->sprite, -30, 160);
    ManagedSprite_SetPriority(work->sprite, ov07_0221FB04(work->owner, 2));
    ov07_0221C410(work->owner, ov07_02226B2C, work);
}
