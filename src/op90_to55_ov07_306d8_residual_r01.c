#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Work {
    void *owner;
    int a;
    int b;
    u8 unk0C[4];
    ManagedSprite *sprite;
    u8 unk14[0x28];
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void *func, void *data);
void ov07_0223061C(void *task, Work *work);
void ov07_022306D8(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_022306D8
void ov07_022306D8(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    work->owner = owner;
    work->a = a;
    work->b = b;
    work->sprite = sprite;
    ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0223061C, work);
}
