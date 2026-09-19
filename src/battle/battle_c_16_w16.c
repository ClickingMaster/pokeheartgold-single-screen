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

void ov12_0225E104(SysTask *task, void *data) {
    u8 *work = data;
    u32 value = ov10_022205BC(*(BattleSystem **)work, work[9]);
    ov12_02262F24(*(BattleSystem **)work, work[9], value);
    ov12_0226430C(*(BattleSystem **)work, work[9], work[8]);
    Heap_Free(work);
    SysTask_Destroy(task);
}


DEFINE_TASK_CLEANUP(ov12_0225E134, 8, 9)


void ov12_0225E154(SysTask *task, void *data) {
    u8 *work = data;
    int index = ov12_0223B694(*(BattleSystem **)work);
    u16 species = GetBattlerVar(BattleSystem_GetBattleContext(*(BattleSystem **)work), work[9], 0, NULL);
    int stat = GetMonBaseStat(species, 0x1A);
    int numerator = ov12_0226D140[index][0] * stat;
    int chance = numerator / ov12_0226D141[index][0];
    if (BattleSystem_Random(*(BattleSystem **)work) % 0xFF <= chance) {
        ov12_02262F24(*(BattleSystem **)work, work[9], 4);
    } else {
        ov12_02262F24(*(BattleSystem **)work, work[9], 5);
    }
    ov12_0226430C(*(BattleSystem **)work, work[9], work[8]);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void ov12_0225E1D4(SysTask *task, void *data) {
    u8 *work = data;
    ov12_02262F24(*(BattleSystem **)work, work[9], 4);
    ov12_0226430C(*(BattleSystem **)work, work[9], work[8]);
    Heap_Free(work);
    SysTask_Destroy(task);
}

void ov12_0225E1FC(SysTask *task, void *data) {
    u8 *work = data;
    u8 value;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[9], &value) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    if (value == 0 || value > 4 || value == 2) {
        ov12_02261EB8(*(BattleSystem **)work);
    }
    ov12_02262F24(*(BattleSystem **)work, work[9], value);
    ov12_0226430C(*(BattleSystem **)work, work[9], work[8]);
    Heap_Free(work);
    SysTask_Destroy(task);
}
