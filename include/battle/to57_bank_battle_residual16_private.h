#ifndef TO56_R3_W08_PRIVATE_H
#define TO56_R3_W08_PRIVATE_H

#include "global.h"

#include "battle/battle.h"
#include "battle/battle_02261FD4.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/trainer_ai.h"

#include "heap.h"
#include "pokemon.h"
#include "sound.h"
#include "sys_task_api.h"

typedef struct BattleWork16 {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    u32 input;
    u16 moves[4];
    u8 pp[4];
    u8 ppMax[4];
    u8 command;
    u8 battlerId;
    u8 partySlot;
    u8 battlerType;
    u8 state;
    u8 pad21;
    u16 excludedMask;
} BattleWork16;

typedef struct BattleMoveMenuData16 {
    u16 moves[4];
    u8 pp[4];
    u8 ppMax[4];
    u8 partySlot;
} BattleMoveMenuData16;

void ov12_02262FE0(BattleSystem *, int, u32);
void ov12_0226430C(BattleSystem *, int, int);
void PlaySE(u16);
void ov12_0225E250(SysTask *, BattleWork16 *);
void ov12_0225E404(SysTask *, BattleWork16 *);

#endif
