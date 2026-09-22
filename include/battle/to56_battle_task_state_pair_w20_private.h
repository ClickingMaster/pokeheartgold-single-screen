#ifndef TO56_R3_W20_PRIVATE_H
#define TO56_R3_W20_PRIVATE_H
#include "global.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "pokepic.h"
#include "sys_task_api.h"

typedef struct W20TaskSmall {
    BattleSystem *battleSystem;
    u8 pad04[8];
    u8 command;
    u8 battlerId;
} W20TaskSmall;

typedef struct W20BattleSource {
    u8 pad00[0xE];
    u16 active : 1;
    u16 flagsRest : 15;
    u8 pad10[0x3C];
    u32 mode4C;
    u32 value50;
} W20BattleSource;

typedef struct W20BattleSourceLocal {
    W20BattleSource source;
    u32 pad54;
} W20BattleSourceLocal;

typedef struct W20BattleParams {
    u32 value0;
    u32 value4;
    void *battlerValues[4];
    void *objects[4];
    u16 values28[4];
    u8 values30[4];
    u8 values34[4];
    u8 values38[4];
    u32 values3C[4];
    u8 mapping[4];
} W20BattleParams;

typedef struct W20TaskLarge {
    BattleSystem *battleSystem;
    void *context;
    Pokepic *pokepic;
    void *object;
    W20BattleSource source;
    u8 pad64[4];
    u8 value68;
    u8 battlerId;
    u8 state;
    u8 disableHpBar;
    u8 updateG3;
} W20TaskLarge;

void ov12_02261ED4(BattleSystem *);
void ov12_02261EB8(BattleSystem *);
void ov12_02262F24(BattleSystem *, u8, u8);
void ov12_0226430C(BattleSystem *, u8, u8);
void ov12_022643C8(BattleSystem *, void *, void *, int, int, int, int, int);
void ov12_02261B80(BattleSystem *, void *, void *, void *);
void ov12_02261CA8(BattleSystem *, W20BattleSource *, W20BattleParams *, u8);
void ov07_0223494C(void *, int);
void ov07_02234A20(void *, int);
void ov07_0221C394(void *);
BOOL ov07_0221C3B0(void *);
void ov07_0221C3C0(void *);
void ov12_0225FCC0(SysTask *, W20TaskSmall *);
void ov12_0225FD14(SysTask *, W20TaskLarge *);
#endif
