#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "heap.h"
#include "sys_task_api.h"

typedef struct TrainerSlideMessage {
    u8 unk0;
    u8 unk1;
    u16 trainerClass;
    int unk4;
} TrainerSlideMessage;

typedef struct TrainerSlideTask {
    BattleSystem *bsys;
    ManagedSprite *sprite;
    u8 unk8;
    u8 unk9;
    u8 state;
    u8 unkB;
    s16 unkC;
} TrainerSlideTask;

typedef struct BattlePos {
    s16 x;
    s16 y;
    s16 z;
} BattlePos;
typedef struct BattlePosTable {
    BattlePos front[6];
    BattlePos back[6];
} BattlePosTable;
extern const BattlePosTable ov07_022377F4;
extern const s16 ov07_022377DC[][2];
ManagedSprite *ov12_02261B2C(BattleSystem *bsys, s32 arg1, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y);
void ov12_0225D990(SysTask *task, void *data);
void ov12_0225A37C(BattleSystem *bsys, OpponentData *opponentData, TrainerSlideMessage *msg);
// @fn ov12_0225A37C
void ov12_0225A37C(BattleSystem *bsys, OpponentData *opponentData, TrainerSlideMessage *msg) {
    TrainerSlideTask *task;

    BattleSystem_GetPokepicManager(bsys);
    task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(TrainerSlideTask));
    task->state = 0;
    if (opponentData->battlerType & 1) {
        task->unkB = 2;
    } else {
        task->unkB = 0;
    }
    opponentData->managedSprite = ov12_02261B2C(bsys, opponentData->battlerType & 1, msg->trainerClass, opponentData->battlerType, FALSE, ov07_022377F4.back[opponentData->battlerType & 1].x, ov07_022377F4.back[opponentData->battlerType & 1].y);
    task->sprite = opponentData->managedSprite;
    task->unkC = ov07_022377DC[opponentData->battlerType & 1][msg->unk4];
    task->bsys = bsys;
    task->unk8 = msg->unk0;
    task->unk9 = opponentData->unk194;
    SysTask_CreateOnMainQueue(ov12_0225D990, task, 0);
}
