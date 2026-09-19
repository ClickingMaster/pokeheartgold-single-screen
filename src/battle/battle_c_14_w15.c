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

void ov12_0225CDB8(SysTask *task, PokepicMoveTask *work)
{
    switch (work->state) {
    case 0:
        Pokepic_AddAttr(work->pokepic, 12, -32);
        Pokepic_AddAttr(work->pokepic, 13, -32);
        if (work->delay != 0) {
            work->delay--;
            Pokepic_AddAttr(work->pokepic, 1, -1);
        }
        if (Pokepic_GetAttr(work->pokepic, 12) <= 0) {
            Pokepic_Delete(work->pokepic);
            work->state++;
        }
        break;
    case 1:
        ov12_0226430C(work->battleSystem, work->battlerId, work->command);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}
