#include "w15_private.h"

typedef struct PokepicMoveTask {
    BattleSystem *battleSystem;
    Pokepic *pokepic;
    u8 command;
    u8 battlerId;
    u8 unusedA;
    u8 state;
    u8 delay;
} PokepicMoveTask;

typedef struct ManagedSpriteMoveTask {
    BattleSystem *battleSystem;
    ManagedSprite *sprite;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 direction;
    s16 target;
} ManagedSpriteMoveTask;

typedef struct ManagedSpriteDeleteTask {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 direction;
} ManagedSpriteDeleteTask;

void ov12_0225D890(SysTask *task, ManagedSpriteDeleteTask *work)
{
    s16 pos[2];
    SpriteManager *spriteManager;

    switch (work->state) {
    case 0:
        if (work->direction == 2) {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, 5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &pos[1], &pos[0]);
            if (pos[1] >= 0x128) {
                work->state++;
            }
        } else {
            ManagedSprite_OffsetPositionXY(work->opponentData->managedSprite, -5, 0);
            ManagedSprite_GetPositionXY(work->opponentData->managedSprite, &pos[1], &pos[0]);
            if (pos[1] <= -40) {
                work->state++;
            }
        }
        break;
    case 1:
        spriteManager = BattleSystem_GetSpriteManager(work->battleSystem);
        Sprite_DeleteAndFreeResources(work->opponentData->managedSprite);
        work->opponentData->managedSprite = NULL;
        SpriteManager_UnloadCharObjById(spriteManager, work->opponentData->battlerType + 0x4E2F);
        SpriteManager_UnloadPlttObjById(spriteManager, work->opponentData->battlerType + 0x4E2A);
        SpriteManager_UnloadCellObjById(spriteManager, work->opponentData->battlerType + 0x4E27);
        SpriteManager_UnloadAnimObjById(spriteManager, work->opponentData->battlerType + 0x4E27);
        ov12_0226430C(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_0225D990(SysTask *task, ManagedSpriteMoveTask *work)
{
    s16 pos[2];

    switch (work->state) {
    case 0:
        ManagedSprite_GetPositionXY(work->sprite, &pos[1], &pos[0]);
        if (work->direction == 0) {
            pos[1] += 5;
            if (pos[1] >= work->target) {
                pos[1] = work->target;
                work->state++;
            }
        } else {
            pos[1] -= 5;
            if (pos[1] <= work->target) {
                pos[1] = work->target;
                work->state++;
            }
        }
        ManagedSprite_SetPositionXY(work->sprite, pos[1], pos[0]);
        break;
    case 1:
        ov12_0226430C(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_0225DA18(SysTask *task, BattleHpBar *hpBar)
{
    switch (hpBar->script) {
    case 0:
        if (hpBar->unk4D != 0) {
            hpBar->unk4D--;
            break;
        }
        ov12_02264FB0(hpBar, FALSE);
        BattleHpBar_SetEnabled(hpBar, TRUE);
        hpBar->script++;
        break;
    case 1:
        if (hpBar->unk_4F_1 == 1) {
            hpBar->script++;
        }
        break;
    default:
        ov12_0226430C(hpBar->battleSystem, hpBar->battlerId, hpBar->unk4C);
        hpBar->unk10 = NULL;
        SysTask_Destroy(task);
        break;
    }
}

void ov12_0225DA8C(SysTask *task, BattleHpBar *hpBar)
{
    if (hpBar->script == 0) {
        if (hpBar->unk_4F_1 == 1) {
            hpBar->script++;
        }
    } else {
        ov12_0226430C(hpBar->battleSystem, hpBar->battlerId, hpBar->unk4C);
        hpBar->unk10 = NULL;
        SysTask_Destroy(task);
        BattleHpBar_SetEnabled(hpBar, FALSE);
    }
}
