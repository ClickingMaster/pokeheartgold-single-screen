#include "to56_r2_w06_private.h"
struct Ov07R2Work {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[4];
    SpriteSystem *spriteSystem;
    u8 pad10[0x0C];
    ManagedSprite *sprite;
    u32 value20;
    int duration;
    u32 value28;
    u32 value2C;
    u32 enabled;
    u32 value34;
    u32 counter;
    u32 threshold;
};

void ov07_02226F80(void *owner) {
    Ov07R2Work *work;
    int type;
    int first;
    int second;

    work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07R2Work));
    BattleAnimUtil_InitCommonWork(owner, work);
    work->value20 = (u32)(BattleAnimSystem_GetArg(owner, 0));
    work->duration = (int)(BattleAnimSystem_GetArg(owner, 1));
    work->value28 = (u32)(BattleAnimSystem_GetArg(owner, 2));
    work->value2C = (u32)(BattleAnimSystem_GetArg(owner, 3));
    work->enabled = (u32)(BattleAnimSystem_GetArg(owner, 5));
    work->value34 = (u32)(BattleAnimSystem_GetArg(owner, 6));
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->owner, (void *)work->value20);
    work->counter = 0;

    if (work->value28 != 0xFF) {
        ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBgPriority(owner, work->value28));
    }
    if (work->value2C != 0xFF) {
        ManagedSprite_SetDrawPriority(work->sprite, work->value2C);
    }

    if (BattleAnimSystem_IsDoubleBattle(owner) != 1) {
        if ((u32)(BattleAnimSystem_GetArg(owner, 4)) == 2 || (u32)(BattleAnimSystem_GetArg(owner, 4)) == 3) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            Heap_Free(work);
            return;
        }
    }

    type = (int)(BattleAnimSystem_GetArg(owner, 4));
    first = BattleAnimSystem_GetAttacker(owner);
    second = BattleAnimSystem_GetDefender(owner);
    if (work->value2C != 0xFF) {
        first = BattleAnimUtil_GetBattlerType(owner, first);
        second = BattleAnimUtil_GetBattlerType(owner, second);
        switch (type) {
        case 0:
            switch (first) {
            case 2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            }
            break;
        case 2:
            switch (first) {
            case 2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            }
            break;
        case 1:
            switch (second) {
            case 2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            }
            break;
        case 3:
            switch (second) {
            case 2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case 3:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 4:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case 5:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            }
            break;
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02226F04, work);
}
