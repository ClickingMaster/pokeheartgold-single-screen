#ifndef TO30_SOL_R4_OVERLAY_12_BATTLE_CONTROLLER_OPPONENT_PRIVATE_H
#define TO30_SOL_R4_OVERLAY_12_BATTLE_CONTROLLER_OPPONENT_PRIVATE_H

#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller_opponent.h"
#include "battle/battle_cursor.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/party_gauge.h"

#include "player_data.h"
#include "pokepic.h"
#include "sound.h"

void ov12_0226430C(BattleSystem *battleSystem, int battlerId, int value);

void ov12_02258EB0(BattleSystem *battleSystem, OpponentData *opponentData);
u8 ov12_02258BA0(BattleSystem *battleSystem, int battlerId);
void ov12_02258F44(BattleSystem *battleSystem, OpponentData *opponentData);
void ov12_022594F4(BattleSystem *battleSystem, OpponentData *opponentData);
void ov12_02259694(BattleSystem *battleSystem, OpponentData *opponentData);
void ov12_02261EB8(BattleSystem *battleSystem);
void ov12_02261ED4(BattleSystem *battleSystem);
void ov12_0225E134(SysTask *task, void *data);
void ov12_0225E1D4(SysTask *task, void *data);
void ov12_0225E4CC(SysTask *task, void *data);
void ov12_0225E740(SysTask *task, void *data);
void ov12_0225F960(SysTask *task, void *data);
void ov12_0225FC80(SysTask *task, void *data);
void ov12_0225FCA0(SysTask *task, void *data);
void ov12_0225B434(BattleSystem *battleSystem, OpponentData *opponentData, u8 *buffer);
void ov12_02262F24(BattleSystem *battleSystem, int battlerId, u32 value);
u8 ov12_02261258(OpponentData *opponentData);
u32 ov12_02261264(OpponentData *opponentData);
void *ov12_02261270(OpponentData *opponentData);
BattleHpBar *OpponentData_GetHpBar(OpponentData *opponentData);
BattleCursorPosition *ov12_02261280(OpponentData *opponentData);
void ov12_02261284(OpponentData *opponentData);
void ov12_02261294(OpponentData *opponentData, s32 drawFlag);
void ov12_02259928(OpponentData *opponentData);

#define DECL_OPPONENT_HANDLER(name) void name(BattleSystem *, OpponentData *)
DECL_OPPONENT_HANDLER(ov12_02258EE0);
DECL_OPPONENT_HANDLER(ov12_02258EF4);
DECL_OPPONENT_HANDLER(ov12_02258F08);
DECL_OPPONENT_HANDLER(ov12_02258F1C);
DECL_OPPONENT_HANDLER(ov12_02258F30);
DECL_OPPONENT_HANDLER(ov12_02258F68);
DECL_OPPONENT_HANDLER(ov12_02258F7C);
DECL_OPPONENT_HANDLER(ov12_02258F90);
DECL_OPPONENT_HANDLER(ov12_02258FA0);
DECL_OPPONENT_HANDLER(ov12_02258FB4);
DECL_OPPONENT_HANDLER(ov12_02258FC8);
DECL_OPPONENT_HANDLER(ov12_02259000);
DECL_OPPONENT_HANDLER(ov12_02259014);
DECL_OPPONENT_HANDLER(ov12_02259028);
DECL_OPPONENT_HANDLER(ov12_0225903C);
DECL_OPPONENT_HANDLER(ov12_02259050);
DECL_OPPONENT_HANDLER(ov12_02259064);
DECL_OPPONENT_HANDLER(ov12_02259078);
DECL_OPPONENT_HANDLER(ov12_0225908C);
DECL_OPPONENT_HANDLER(ov12_022590D4);
DECL_OPPONENT_HANDLER(ov12_022590E8);
DECL_OPPONENT_HANDLER(ov12_022590FC);
DECL_OPPONENT_HANDLER(ov12_02259110);
DECL_OPPONENT_HANDLER(ov12_02259124);
DECL_OPPONENT_HANDLER(ov12_02259134);
DECL_OPPONENT_HANDLER(ov12_02259148);
DECL_OPPONENT_HANDLER(ov12_0225915C);
DECL_OPPONENT_HANDLER(ov12_02259170);
DECL_OPPONENT_HANDLER(ov12_02259184);
DECL_OPPONENT_HANDLER(ov12_02259198);
DECL_OPPONENT_HANDLER(ov12_022591A8);
DECL_OPPONENT_HANDLER(ov12_022591BC);
DECL_OPPONENT_HANDLER(ov12_022591CC);
DECL_OPPONENT_HANDLER(ov12_022591E0);
DECL_OPPONENT_HANDLER(ov12_022593D4);
DECL_OPPONENT_HANDLER(ov12_022593E8);
DECL_OPPONENT_HANDLER(ov12_022595B8);
DECL_OPPONENT_HANDLER(ov12_022595CC);
DECL_OPPONENT_HANDLER(ov12_02259724);
DECL_OPPONENT_HANDLER(ov12_02259738);
DECL_OPPONENT_HANDLER(ov12_02259748);
DECL_OPPONENT_HANDLER(ov12_02259758);
DECL_OPPONENT_HANDLER(ov12_022597B0);
DECL_OPPONENT_HANDLER(ov12_022597C4);
DECL_OPPONENT_HANDLER(ov12_022597D8);
DECL_OPPONENT_HANDLER(ov12_02259930);
#undef DECL_OPPONENT_HANDLER

void BattleSystem_SetRandTemp(BattleSystem *, u32);
void ov12_02259944(BattleSystem *, OpponentData *);
void ov12_0223BB6C(BattleSystem *, u8);
void ov12_0225A524(BattleSystem *, OpponentData *, u8 *);
void ov12_0225A9B0(BattleSystem *, OpponentData *);
u32 BattleSystem_GetBattleType(BattleSystem *);
void BattleInput_StartMenuScrollHorizontalTask(void *, s32, int);
void ov12_02264EB4(void *);
void ov12_02262014(OpponentData *);
void ov12_0225B120(BattleSystem *, OpponentData *, u8 *);
void ov12_0225B16C(BattleSystem *, OpponentData *, u8 *);
void ov12_0225B1A8(BattleSystem *, OpponentData *);
void ov12_0225B2F8(BattleSystem *, OpponentData *, u8 *);
void ov12_0225B34C(BattleSystem *, OpponentData *);

void ov12_02258EB4(BattleSystem *, OpponentData *);
void ov12_02258FD8(BattleSystem *, OpponentData *);
void ov12_022590A0(BattleSystem *, OpponentData *);
void ov12_022592D0(BattleSystem *, OpponentData *);
void ov12_02259328(BattleSystem *, OpponentData *);
void ov12_022595E0(BattleSystem *, OpponentData *);
void ov12_0225961C(BattleSystem *, OpponentData *);
void ov12_02259658(BattleSystem *, OpponentData *);
void ov12_02259700(BattleSystem *, OpponentData *);
void ov12_02259768(BattleSystem *, OpponentData *);
void ov12_0225978C(BattleSystem *, OpponentData *);

void *ov12_02258D74(BattleSystem *, const u8 *);
void ov12_022598F8(BattleSystem *, OpponentData *);
void ov12_02259944(BattleSystem *, OpponentData *);
void ov12_02260668(SysTask *, void *);

typedef void (*OpponentDispatchFunc)(BattleSystem *, OpponentData *);
extern OpponentDispatchFunc ov12_0226D010[];
void BattleHpBar_FreeResources(BattleHpBar *);
void ov12_02258E54(BattleSystem *, OpponentData *);
void ov12_02258E7C(BattleSystem *, OpponentData *, int);
void ov12_022596B8(BattleSystem *, OpponentData *);

u32 ov10_022205BC(BattleSystem *, int);
void ov12_0225E104(SysTask *, void *);
void ov12_0225F3FC(SysTask *, void *);

#endif
