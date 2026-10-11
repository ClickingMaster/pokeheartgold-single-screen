#include "to57_new36_private.h"
#include "to91_ov07_motion_owner.h"
typedef Ov07MotionWork W36;
void ov07_0222FD14(void *, W36 *);
void ov07_0222FE04(void *, void *, void *, void *);
void ov07_0222FD14(void *taskOwner, W36 *work) {
    int i;
    switch (work->state) {
    case 0:
        BattleAnimUtil_SetBlendWithBattleBg(work->owner, 1, 15);
        BattleAnimUtil_StartBlendAlphaMotion((u8 *)work + 0x108, 1, 16, 15, 0, 10);
        ov07_0222FBEC(work, work->amplitude);
        ov07_0222FC44(work);
        work->state++;
        break;
    case 1:
        ov07_0222FC44(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone((u8 *)work + 0x108)) {
            work->state++;
            work->timer = 28;
        }
        break;
    case 2:
        ov07_0222FC44(work);
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion((u8 *)work + 0x108, 16, 1, 0, 15, 10);
        }
        break;
    case 3:
        ov07_0222FC44(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone((u8 *)work + 0x108)) {
            work->state++;
        }
        break;
    case 4:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->owner, taskOwner);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void ov07_0222FE04(void *owner, void *spriteSystem, void *spriteManager, void *firstSprite) {
    u8 template[0x34];
    W36 *work = BattleAnimUtil_AllocWork(owner, sizeof(W36));
    void *pic;
    int i;
    work->owner = owner;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(owner));
    work->x = Pokepic_GetAttr(pic, 0);
    work->y = Pokepic_GetAttr(pic, 1);
    BattleAnimSystem_GetSpriteTemplate(template, owner);
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->sprites[i] = firstSprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetOamMode(work->sprites[i], 1);
        ManagedSprite_SetAnimateFlag(work->sprites[i], 1);
    }
    work->amplitude = ov07_02222004(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222FD14, work);
}
