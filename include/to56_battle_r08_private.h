#ifndef TO56_R4_W19_PRIVATE_H
#define TO56_R4_W19_PRIVATE_H

#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"

typedef struct R4Work {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 pad08[0x18];
    u16 values20[4];
    u8 values28[4];
    u8 values2C[4];
    u8 values30[4];
    u32 values34[11];
    u8 command;
    u8 battler;
    u8 state;
    u8 param;
    u32 value64;
} R4Work;

typedef struct R4TextWork {
    BattleSystem *battleSystem;
    u8 command;
    u8 battler;
    u8 printer;
    u8 unused;
} R4TextWork;

typedef struct R4Command {
    u8 command;
    u8 param;
    u8 value;
    u8 pad03;
    u16 values04[4];
    u8 values0C[4];
    u8 values10[4];
    u8 values14[4];
    u32 values18[4];
} R4Command;

void ov12_02260418(SysTask *, R4Work *);
void ov12_022605D0(SysTask *, R4TextWork *);
void BattleController_EmitCommandComplete(BattleSystem *, int, int);
void ov12_02261390(BattleSystem *, OpponentData *, const u8 *, BattleMessage *);
void ov12_02261464(BattleSystem *, OpponentData *, const u8 *, BattleMessage *);
void ov12_02261544(BattleSystem *, OpponentData *, BattleMessage *);
void ov12_022615F0(BattleSystem *, OpponentData *, const u8 *, BattleMessage *);
void ov12_0225AC1C(BattleSystem *, OpponentData *, const u8 *);
void ov12_0225ACB0(BattleSystem *, OpponentData *, const u8 *);
void ov12_0225ACE8(BattleSystem *, OpponentData *, const u8 *);
void ov12_0225AD44(BattleSystem *, OpponentData *, const u8 *);
void ov12_0225AD9C(BattleSystem *, OpponentData *, const u8 *);
void ov12_0225ADF4(BattleSystem *, OpponentData *);
void ov12_0225AE48(BattleSystem *, OpponentData *, const u8 *);

#endif
