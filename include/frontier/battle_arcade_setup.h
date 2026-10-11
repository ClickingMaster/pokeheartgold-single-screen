#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_ARCADE_SETUP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_ARCADE_SETUP_H

#include "battle/battle_setup.h"
#include "frontier/frontier.h"
#include "frontier/overlay_80_022340E8.h"

#include "party.h"
#include "pokemon.h"

u32 BattleArcade_GetRandomTrainerId(u32 type, u32 round, u32 battleNo);
u8 BattleArcade_GetMonCount(u8 type, int both);
u8 BattleArcade_GetOpponentMonCount(u8 type, int both);
BattleSetup *BattleArcade_NewBattleSetup(ArcadeContext *ctx, FrontierLaunchArgs *args);
u32 BattleArcade_GetBattleType(int type);
int BattleArcade_GetMonLevel(ArcadeContext *ctx);
BOOL BattleArcade_MultiplayerCheck(int type);
void BattleArcade_SetMonOriginalTrainer(ArcadeContext *ctx, Pokemon *mon);
void BattleArcade_AddMonToParty(ArcadeContext *ctx, Party *party, Pokemon *mon);
void BattleArcade_SetOpponentParty(ArcadeContext *ctx);
u32 ov80_02238430(ArcadeContext *ctx, u8 eventId);
u32 BattleArcade_GetTrainerAIFlags(ArcadeContext *ctx);
u32 BattleArcade_GetRound(ArcadeContext *ctx);
u32 BattleArcade_GetEventColor(u8 eventId);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_ARCADE_SETUP_H
