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
    u8 unk14[0x48];
    ManagedSprite *sprite;
    u8 unk60[0x28];
    s16 x;
    s16 y;
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
u32 BattleAnimSystem_GetAttacker(void *owner);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, u32 battler);
s16 BattleAnimSystem_GetBattlerScreenCoord(void *owner, u32 battler, int coord);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void *func, void *data);
void ov07_0222F210(void *task, Work *work);
void ov07_0222F378(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_0222F378
void ov07_0222F378(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    work->owner = owner;
    work->a = a;
    work->b = b;
    work->pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(owner));
    Pokepic_SetAttr(work->pic, 6, 0);
    work->x = BattleAnimSystem_GetBattlerScreenCoord(owner, BattleAnimSystem_GetAttacker(owner), 0);
    work->y = BattleAnimSystem_GetBattlerScreenCoord(owner, BattleAnimSystem_GetAttacker(owner), 1);
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetPositionXY(work->sprite, work->x, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222F210, work);
}
