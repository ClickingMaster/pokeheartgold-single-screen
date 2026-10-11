#ifndef TO56_R2_W09_PRIVATE_H
#define TO56_R2_W09_PRIVATE_H
#include "global.h"

#include "battle/battle.h"
#include "battle/battle_02261FD4.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"

#include "filesystem.h"
#include "heap.h"
#include "sound.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"

typedef struct CommandSetData {
    BattleSystem *battleSystem;
    BattleHpBar *hpBar;
    u8 command;
    u8 battlerId;
    u8 state;
    s8 unusedB;
    u32 input;
    u8 ballStatus[2][6];
    u8 expPercents[6];
    u8 unused22;
    u8 partySlot;
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 battlerType;
    u8 msgIdx;
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingMask;
} CommandSetData;

typedef struct ActionSelectData {
    u8 battlerType;
    u8 battlerId;
    u8 partySlot;
    u8 ballStatusBattler;
    s16 curHP;
    u16 maxHP;
    s8 waitingForPartner;
    u8 pad[3];
} ActionSelectData;

void ov12_0225DAD4(SysTask *, CommandSetData *);
void BattleController_EmitCommandComplete(BattleSystem *, int, int);
void BattleController_ReplyCommandSelection(BattleSystem *, int, u32);
void PlaySE(u16);
#endif
