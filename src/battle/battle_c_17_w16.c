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


DEFINE_TASK_CLEANUP(ov12_0225E4CC, 0x1C, 0x1D)


void ov12_0225E4EC(SysTask *task, void *data) {
    u8 *work = data;
    u8 value;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[0x1D], &value) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    if (value == 0 || value > 4) {
        ov12_02261EB8(*(BattleSystem **)work);
    } else {
        u16 result = GetBattlerVar(BattleSystem_GetBattleContext(*(BattleSystem **)work), work[0x1D], value + 5, NULL);
        if (result == 0 || result > 0x1D3) {
            ov12_02261EB8(*(BattleSystem **)work);
        }
    }
    ov12_02262FE0(*(BattleSystem **)work, work[0x1D], value);
    ov12_0226430C(*(BattleSystem **)work, work[0x1D], work[0x1C]);
    Heap_Free(work);
    SysTask_Destroy(task);
}
