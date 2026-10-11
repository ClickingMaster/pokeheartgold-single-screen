#include "global.h"

#include "battle/battle.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"
#include "sound_playback.h"

typedef struct HealthbarUpdateMessageR4W06 {
    u8 command;
    u8 level;
    s16 hp;
    u16 maxHp;
    u8 pad06[2];
    s32 gainedHp;
} HealthbarUpdateMessageR4W06;

typedef struct ExpUpdateMessageR4W06 {
    u8 command;
    u8 pad01[3];
    s32 exp;
    s32 newExp;
    s32 maxExp;
} ExpUpdateMessageR4W06;

typedef struct CommandSetMessageR4W06 {
    u8 command;
    u8 partySlot;
    u16 value02;
    u32 value04;
    u8 value08;
    u8 value09;
    u8 value0A;
    u8 pad0B;
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 values1C[4];
    u32 values20[4];
} CommandSetMessageR4W06;

typedef struct CommandSetWorkR4W06 {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    u8 pad0C[0x18];
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 values34[4];
    u32 values38[4];
    u8 pad48[0x1C];
    u8 command;
    u8 battlerId;
    u8 state;
    u8 face;
    u16 value68;
    u8 partySlot;
    u8 value6B;
    u32 value6C;
    u16 value70;
    u16 value72;
} CommandSetWorkR4W06;

void ov12_0225FFDC(SysTask *, void *);
void ov12_02260030(SysTask *, void *);
void ov12_022600F0(SysTask *, void *);
void ov12_0226430C(BattleSystem *, int, int);
void ov12_0225A9E0(BattleSystem *, OpponentData *, HealthbarUpdateMessageR4W06 *);
void ov12_0225AA6C(BattleSystem *, OpponentData *, ExpUpdateMessageR4W06 *);
void ov12_0225AAE0(BattleSystem *, OpponentData *, CommandSetMessageR4W06 *);
void ov12_0225ABB8(BattleSystem *, OpponentData *, u8 *);

void ov12_0225A9E0(BattleSystem *battleSystem, OpponentData *opponentData, HealthbarUpdateMessageR4W06 *message) {
    BattleHpBar *hpBar;

    GF_ASSERT(opponentData->hpBar.boxObj != NULL);
    hpBar = &opponentData->hpBar;
    MI_CpuFill8(&hpBar->script, 0, sizeof(hpBar->script));
    hpBar->battleSystem = battleSystem;
    hpBar->unk4C = message->command;
    hpBar->battlerId = opponentData->unk194;
    hpBar->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(opponentData->battlerType, BattleSystem_GetBattleType(battleSystem));
    hpBar->hp = message->hp;
    hpBar->maxHp = message->maxHp;
    hpBar->gainedHp = message->gainedHp;
    hpBar->level = message->level;
    if (message->gainedHp == 0x7FFF) {
        hpBar->hp = 0;
        hpBar->gainedHp = 0;
    }
    hpBar->unk10 = SysTask_CreateOnMainQueue(ov12_0225FFDC, hpBar, 1000);
}

void ov12_0225AA6C(BattleSystem *battleSystem, OpponentData *opponentData, ExpUpdateMessageR4W06 *message) {
    BattleHpBar *hpBar;

    GF_ASSERT(opponentData->hpBar.boxObj != NULL);
    hpBar = &opponentData->hpBar;
    MI_CpuFill8(&hpBar->script, 0, sizeof(hpBar->script));
    hpBar->battleSystem = battleSystem;
    hpBar->unk4C = message->command;
    hpBar->battlerId = opponentData->unk194;
    hpBar->exp = message->exp;
    hpBar->maxExp = message->maxExp;
    hpBar->gainedExp = message->newExp - hpBar->exp;
    if (opponentData->battlerType == 0) {
        hpBar->unk10 = SysTask_CreateOnMainQueue(ov12_02260030, hpBar, 1000);
    } else {
        ov12_0226430C(hpBar->battleSystem, hpBar->battlerId, hpBar->unk4C);
    }
}

void ov12_0225AAE0(BattleSystem *battleSystem, OpponentData *opponentData, CommandSetMessageR4W06 *message) {
    CommandSetWorkR4W06 *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(CommandSetWorkR4W06));
    int i;

    work->face = (opponentData->battlerType & 1) ? 2 : 0;
    work->state = 0;
    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = message->command;
    work->battlerId = opponentData->unk194;
    work->pokepic = opponentData->pokepic;
    work->value68 = message->value02;
    work->partySlot = message->partySlot;
    work->value6B = message->value08;
    work->value6C = message->value04;
    work->value70 = message->value09;
    work->value72 = message->value0A;
    for (i = 0; i < 4; i++) {
        work->moves[i] = message->moves[i];
        work->curPP[i] = message->curPP[i];
        work->maxPP[i] = message->maxPP[i];
        work->values34[i] = message->values1C[i];
        work->values38[i] = message->values20[i];
    }
    SysTask_CreateOnMainQueue(ov12_022600F0, work, 0);
}

void ov12_0225ABB8(BattleSystem *battleSystem, OpponentData *opponentData, u8 *message) {
    s32 pan;

    if (opponentData->battlerType & 1) {
        pan = 0x75;
    } else {
        pan = 1 - 0x76;
    }
    ov12_0226430C(battleSystem, opponentData->unk194, message[0]);
    PlaySE_SetPan(*(u16 *)&message[2], pan);
}
