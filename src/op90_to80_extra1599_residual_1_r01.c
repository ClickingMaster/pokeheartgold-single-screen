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
    Pokepic *pics[4];
    ManagedSprite *sprite;
    u8 unk28[0x24];
    s16 x;
    s16 y;
    u8 unk50[4];
    int alpha;
    u8 unk58[4];
} Work;
void *BattleAnimUtil_AllocWork(void *owner, u32 size);
u32 BattleAnimSystem_GetDefender(void *owner);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, u32 battler);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void *func, void *data);
void ov07_0222E108(void *task, Work *work);
void ov07_0222E2EC(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_0222E2EC
void ov07_0222E2EC(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = BattleAnimUtil_AllocWork(owner, sizeof(Work));
    int i;
    work->owner = owner;
    work->a = a;
    work->b = b;
    work->pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetDefender(owner));
    for (i = 0; i < 4; i++) {
        work->pics[i] = BattleAnimSystem_GetBattlerPokepic(work->owner, i);
    }
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_GetPositionXY(work->sprite, &work->x, &work->y);
    work->x -= 40;
    work->y -= 40;
    ManagedSprite_SetPositionXY(work->sprite, work->x, work->y);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    work->alpha = -16;
    G2_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 31, 26);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222E108, work);
}
