#include "w16_private.h"

void ov12_0226311C(BattleSystem *, int, u32);
typedef struct W16Pair {
    u16 first;
    u16 second;
} W16Pair;
void ov12_022632C0(BattleSystem *, int, W16Pair);
void ov12_02262FE0(BattleSystem *, int, u32);
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
        ov12_0226430C(*(BattleSystem **)work, work[battlerOffset], work[commandOffset]); \
        Heap_Free(work);                                                                 \
        SysTask_Destroy(task);                                                           \
    }

void ov12_0225E6FC(SysTask *task, void *data) {
    u8 *work = data;
    int value = ov12_022581D4(*(BattleSystem **)work, BattleSystem_GetBattleContext(*(BattleSystem **)work), 0xB, work[0xD]) + 1;
    GF_ASSERT(value < 5);
    ov12_0226311C(*(BattleSystem **)work, work[0xD], value);
    ov12_0226430C(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(work);
    SysTask_Destroy(task);
}


DEFINE_TASK_CLEANUP(ov12_0225E740, 0xC, 0xD)

