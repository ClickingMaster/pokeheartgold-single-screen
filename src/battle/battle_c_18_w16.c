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

void ov12_0225E6FC(SysTask *task, void *data) {
    u8 *work = data;
    int value = ov12_022581D4(*(BattleSystem **)work, BattleSystem_GetBattleContext(*(BattleSystem **)work), 0xB, work[0xD]) + 1;
    GF_ASSERT(value < 5);
    BattleController_ReplyTargetSelection(*(BattleSystem **)work, work[0xD], value);
    BattleController_EmitCommandComplete(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(work);
    SysTask_Destroy(task);
}


DEFINE_TASK_CLEANUP(ov12_0225E740, 0xC, 0xD)

