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
    u16 setIds[FACTORY_RENTAL_COUNT];        // 0x00
    u8 ivs[FACTORY_RENTAL_COUNT];            // 0x0C
    u32 personalities[FACTORY_RENTAL_COUNT]; // 0x14
    FrontierMon mons[FACTORY_RENTAL_COUNT];  // 0x2C
} BattleFactoryMonSet; // size 0x17C

// The Battle Factory's script data (BattleFactoryData_Alloc, Frontier_GetData while the factory's scripts run).
typedef struct BattleFactoryData {
    u32 heapId;                                     // 0x000
    u8 type;                                        // 0x004, single / double / multi / link multi
    u8 levelMode;                                   // 0x005, level 50 / open level
    u8 battleNo;                                    // 0x006, battle in the current round (0-6)
    u8 unk_007;
    u16 tradeCount;                                 // 0x008
    u8 hasStreak;                                   // 0x00A, a streak is in progress (saved in the factory flags)
    u8 unk_00B;
    u16 winStreak;                                  // 0x00C
    u16 round;                                      // 0x00E, winStreak / 7, at most 8
    u32 unk_010;
    BOOL battleWon;                                 // 0x014
    u16 trainerIds[14];                             // 0x018, 7 per round, the second half for the partner
    FrontierTrainerData trainers[2];                // 0x034, the current opponents
    BattleFactoryMonSet rentals;                    // 0x254, the player's rentals / trade candidates
    u8 filler_3D0[2];
    u16 opponentSetIds[FACTORY_OPPONENT_COUNT];     // 0x3D2
    u8 opponentIVs[FACTORY_OPPONENT_COUNT];         // 0x3DA
    u32 opponentPersonalities[FACTORY_OPPONENT_COUNT]; // 0x3E0
    FrontierMon opponentMons[FACTORY_OPPONENT_COUNT];  // 0x3F0
    u8 filler_4D0[4];
    Party *playerParty;                             // 0x4D4
    Party *opponentParty;                           // 0x4D8
    u16 selection[6];                               // 0x4DC, result of the factory app (picked rentals / trade slots)
    u16 partySetIds[6];                             // 0x4E8, set ids of the player party's Pokemon
    void *challenge;                                // 0x4F4, Save_Frontier_GetFactoryChallenge
    SaveData *saveData;                             // 0x4F8
    BattleSetup *battleSetup;                       // 0x4FC
    SysTask *scrollTask;                            // 0x500
    u16 linkBuffer[30];                             // 0x504, outgoing link message
    u8 filler_540[0x3C];
    u8 unk_57C;                                     // 0x57C, partner's value of link message 0x1A
    u8 unk_57D;                                     // 0x57D, partner's value of link message 0x1B
    u16 partnerRound;                               // 0x57E
    u16 partnerTradeCount;                          // 0x580
    u16 partnerWinStreak;                           // 0x582
    BattleFactoryMonSet partnerRentals;             // 0x584, generated for the partner in link multi battles
    u8 filler_700[2];
    u8 linkMessageCount;                            // 0x702
    u8 filler_703[5];
} BattleFactoryData; // size 0x708

// Arguments of the factory app (FrtCmd_FactoryLaunchRentalApp / FrtCmd_FactoryLaunchTradeApp).
typedef struct BattleFactoryAppLaunchArgs {
    SaveData *saveData;       // 0x00
    u8 type;                  // 0x04
    u8 levelMode;             // 0x05
    u8 screen;                // 0x06, 0 = pick rentals, 1 = trade
    u8 unk_07;
    Party *playerParty;       // 0x08
    Party *opponentParty;     // 0x0C
    u16 result[6];            // 0x10, copied to BattleFactoryData.selection when the app exits
    BattleFactoryData *data;  // 0x1C
    u32 unk_20;
} BattleFactoryAppLaunchArgs; // size 0x24

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
void ov80_022307C8(BattleFactoryData *data);
void BattleFactory_CompleteRound(BattleFactoryData *data);
void BattleFactory_SetRentalParty(BattleFactoryData *data);
void BattleFactory_GenerateOpponentMons(BattleFactoryData *data);
void BattleFactory_TradeMon(BattleFactoryData *data);
void BattleFactory_SetOpponentParty(BattleFactoryData *data);
void BattleFactory_IncTradeCount(BattleFactoryData *data);
u16 BattleFactory_SendBuffer(BattleFactoryData *data, u32 command, int value);
int BattleFactory_GetWonBattlePoints(BattleFactoryData *data);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_DATA_H
