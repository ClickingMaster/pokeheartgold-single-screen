#ifndef POKEHEARTGOLD_BATTLE_CONTROLLER_OPPONENT_H
#define POKEHEARTGOLD_BATTLE_CONTROLLER_OPPONENT_H

#include "battle/battle.h"

// The display side of a battler: OpponentData_RunCommand runs the command the battle controller sent
// (BattleController_Emit*); the input commands run the tasks OpponentData_SetInputTasks chose
// (local player, AI, link partner, recorded battle, Safari Zone, Pal Park).

OpponentData *OpponentData_New(BattleSystem *battleSystem, const u8 *data);
void OpponentData_CreateBallData(BattleSystem *battleSystem, OpponentData *opponentData, u16 ball, int a3);
void OpponentData_RunCommand(BattleSystem *battleSystem, OpponentData *opponentData);
void OpponentData_Free(BattleSystem *battleSystem, OpponentData *opponentData, int mode);
void OpponentData_SetInputTasks(BattleSystem *battleSystem, OpponentData *opponentData);

u8 OpponentData_GetBattlerType(OpponentData *opponentData);
void *OpponentData_GetPokepic(OpponentData *opponentData);
u32 OpponentData_GetInputMode(OpponentData *opponentData);
BattleHpBar *OpponentData_GetHpBar(OpponentData *opponentData);
BattleCursorPosition *OpponentData_GetCursorPosition(OpponentData *opponentData);
void OpponentData_SetSpriteDrawFlag(OpponentData *opponentData, s32 drawFlag);

int TrainerAI_GetMoveType(BattleSystem *battleSystem, BattleContext *ctx, Pokemon *mon, int move);
int TrainerAI_PostKOSwitchIn(BattleSystem *battleSystem, int battler);

// Starts the pokepic animation and plays the cry of a battler that appeared (its own file, after the data asm).
void BattleControllerOpponent_PlayMonAnimAndCry(BattleSystem *battleSystem, int battlerId, int battlerType, Pokepic *pokepic, u32 *narc, int species, int form, int facing, int cryParam);

#endif
