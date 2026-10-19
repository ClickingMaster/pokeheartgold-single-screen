#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_DATA_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_DATA_H

#include "battle/battle_setup.h"
#include "frontier/frontier_trainers.h"

#include "party.h"
#include "save.h"
#include "scrcmd_9.h"
#include "sys_task_api.h"

#define FACTORY_RENTAL_COUNT   6
#define FACTORY_OPPONENT_COUNT 4

// Rental Pokemon offered to a player: generated from Battle Factory set ids, with their attack IVs and personalities.
typedef struct BattleFactoryMonSet {
    u16 setIds[FACTORY_RENTAL_COUNT];
    u8 ivs[FACTORY_RENTAL_COUNT];
    u32 personalities[FACTORY_RENTAL_COUNT];
    FrontierMon mons[FACTORY_RENTAL_COUNT];
} BattleFactoryMonSet;

typedef struct BattleFactoryData {
    u32 heapId;
    u8 type;      // single / double / multi / link multi
    u8 levelMode; // level 50 / open level
    u8 battleNo;  // battle in the current round (0-6)
    u8 unk_007;
    u16 tradeCount;
    u8 hasStreak; // a streak is in progress (saved in the factory flags)
    u8 unk_00B;
    u16 winStreak;
    u16 round; // winStreak / 7, at most 8
    u32 unk_010;
    BOOL battleWon;
    u16 trainerIds[14]; // 7 per round, the second half for the partner
    FrontierTrainerData trainers[2];
    BattleFactoryMonSet rentals; // the player's rentals / trade candidates
    u8 filler_3D0[2];
    u16 opponentSetIds[FACTORY_OPPONENT_COUNT];
    u8 opponentIVs[FACTORY_OPPONENT_COUNT];
    u32 opponentPersonalities[FACTORY_OPPONENT_COUNT];
    FrontierMon opponentMons[FACTORY_OPPONENT_COUNT];
    u8 filler_4D0[4];
    Party *playerParty;
    Party *opponentParty;
    u16 selection[6]; // result of the factory app (picked rentals / trade slots)
    u16 partySetIds[6];
    void *challenge;
    SaveData *saveData;
    BattleSetup *battleSetup;
    SysTask *scrollTask;
    u16 linkBuffer[30]; // outgoing link message
    u8 filler_540[0x3C];
    u8 partnerValueA; // partner's value of link message 0x1A
    u8 partnerValueB; // partner's value of link message 0x1B
    u16 partnerRound;
    u16 partnerTradeCount;
    u16 partnerWinStreak;
    BattleFactoryMonSet partnerRentals; // generated for the partner in link multi battles
    u8 filler_700[2];
    u8 linkMessageCount;
    u8 filler_703[5];
} BattleFactoryData;

typedef struct BattleFactoryAppLaunchArgs {
    SaveData *saveData;
    u8 type;
    u8 levelMode;
    u8 screen; // 0 = pick rentals, 1 = trade
    u8 unk_07;
    Party *playerParty;
    Party *opponentParty;
    u16 result[6]; // copied to BattleFactoryData.selection when the app exits
    BattleFactoryData *data;
    u32 unk_20;
} BattleFactoryAppLaunchArgs;

BattleFactoryData *BattleFactoryData_Alloc(SaveData *saveData, int resume, u8 type, u8 levelMode);
void BattleFactoryData_Init(BattleFactoryData *data, int resume);
void BattleFactoryData_InitNew(BattleFactoryData *data);
void BattleFactoryData_InitResume(BattleFactoryData *data);
void BattleFactory_SwapRentalWithRandom(BattleFactoryData *data, int index, int partner);
void BattleFactoryData_Free(BattleFactoryData *data);
void BattleFactory_StoreAppResult(BattleFactoryData *data, BattleFactoryAppLaunchArgs *args);
int BattleFactory_GetAppResult(BattleFactoryAppLaunchArgs *args, u32 index);
void BattleFactoryData_SaveRecord(BattleFactoryData *data, int mode);
u8 BattleFactory_IncBattleNo(BattleFactoryData *data);
u8 BattleFactory_GetBattleNo(BattleFactoryData *data);
int BattleFactory_GetTrainerSprite(BattleFactoryData *data, int partner);
void BattleFactoryData_SaveRecordMode1(BattleFactoryData *data);
void BattleFactory_CompleteRound(BattleFactoryData *data);
void BattleFactory_SetRentalParty(BattleFactoryData *data);
void BattleFactory_GenerateOpponentMons(BattleFactoryData *data);
void BattleFactory_TradeMon(BattleFactoryData *data);
void BattleFactory_SetOpponentParty(BattleFactoryData *data);
void BattleFactory_IncTradeCount(BattleFactoryData *data);
u16 BattleFactory_SendBuffer(BattleFactoryData *data, u32 command, int value);
int BattleFactory_GetWonBattlePoints(BattleFactoryData *data);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_DATA_H
