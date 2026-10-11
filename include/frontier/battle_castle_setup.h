#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_SETUP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_SETUP_H

#include "battle/battle_setup.h"
#include "frontier/battle_castle_data.h"
#include "frontier/frontier.h"

#include "bg_window.h"
#include "frontier_save_access.h"
#include "party.h"
#include "pokemon.h"

int BattleCastle_GetRandomTrainerId(int type, u32 round, int battleNo);
void BattleCastle_GetRoundTrainerIds(u32 type, u32 round, u16 *trainerIds, int count);
int BattleCastle_GetPartySize(int type, int both);
int BattleCastle_GetOpponentMonCount(int type, int both);
BattleSetup *BattleCastle_NewBattleSetup(BattleCastleData *data, FrontierLaunchArgs *args);
u32 BattleCastle_GetBattleType(int type);
int BattleCastle_GetMonLevel(BattleCastleData *data);
BOOL BattleCastle_IsMultiType(int type);
void BattleCastle_ReviveParty(Party *party);
void BattleCastle_SetMonOriginalTrainer(BattleCastleData *data, Pokemon *mon);
void BattleCastle_AddMonToParty(BattleCastleData *data, Party *party, Pokemon *mon);
void BattleCastle_SetOpponentParty(BattleCastleData *data);
int BattleCastle_GetTrainerAIFlags(BattleCastleData *data);
u16 BattleCastle_GetRound(BattleCastleData *data);
void BattleCastle_DrawRoundTiles(BgConfig *bgConfig, BattleCastleData *data, int bgId);
void BattleCastle_BuildRoundTilemap(u16 *tilemap, int round);
int BattleCastle_ClampRound(int round);
void BattleCastle_SpendPoints(FrontierSave *frontierSave, int type, int amount);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_SETUP_H
