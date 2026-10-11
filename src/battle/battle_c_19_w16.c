#include "w16_private.h"

void BattleController_ReplyTargetSelection(BattleSystem *, int, u32);
typedef struct W16Pair {
    u16 first;
    u16 second;
} W16Pair;
void BattleController_ReplyItemSelection(BattleSystem *, int, W16Pair);
void BattleController_ReplyMoveSelection(BattleSystem *, int, u32);
void ov12_0225E1FC(SysTask *, void *);
void ov12_0225E154(SysTask *, void *);
void ov12_0225E4EC(SysTask *, void *);
void ov12_0225E6FC(SysTask *, void *);
void ov12_0225F3A4(SysTask *, void *);
void ov12_0225F434(SysTask *, void *);
extern u8 ov12_0226D140[][2];
extern u8 ov12_0226D141[][2];

#define DEFINE_TASK_CLEANUP(name, commandOffset, battlerOffset)                          \
    void name(SysTask *task, void *data) {                                               \
        u8 *work = data;                                                                 \
        BattleController_EmitCommandComplete(*(BattleSystem **)work, work[battlerOffset], work[commandOffset]); \
        Heap_Free(work);                                                                 \
        SysTask_Destroy(task);                                                           \
    }

void ov12_0225F3A4(SysTask *task, void *data) {
    u8 *work = data;
    W16Pair value;
    value.first = 1;
    BattleController_ReplyItemSelection(*(BattleSystem **)work, work[0xD], value);
    BattleController_EmitCommandComplete(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(**(void ***)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)(work + 8));
    Heap_Free(work);
    SysTask_Destroy(task);
}

void ov12_0225F3FC(SysTask *task, void *data) {
    u8 *work = data;
    BattleController_EmitCommandComplete(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(**(void ***)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)(work + 8));
    Heap_Free(work);
    SysTask_Destroy(task);
}

void ov12_0225F434(SysTask *task, void *data) {
    u8 *work = data;
    struct {
        u8 current;
        u8 padding;
        u16 first;
        u8 secondLow;
        u8 secondHigh;
    } local;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[0xD], &local.current) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    local.first = local.current;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[0xD], &local.current) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    local.first |= local.current << 8;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[0xD], &local.current) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    local.secondLow = local.current & 0xF;
    local.secondHigh = 0;
    BattleController_ReplyItemSelection(*(BattleSystem **)work, work[0xD], *(W16Pair *)&local.first);
    BattleController_EmitCommandComplete(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(**(void ***)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)((u8 *)*(void **)(work + 8) + 4));
    Heap_Free(*(void **)(work + 8));
    Heap_Free(work);
    SysTask_Destroy(task);
}
