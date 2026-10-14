#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_DATA_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_DATA_H

#include "battle/battle_setup.h"
#include "frontier/frontier_system.h"
#include "frontier/frontier_trainers.h"

#include "bg_window.h"
#include "party.h"
#include "player_data.h"
#include "save.h"
#include "scrcmd_9.h"

#define CASTLE_PARTY_SIZE       3
#define CASTLE_OPPONENT_COUNT   4
#define CASTLE_LINK_MON_BUFFER_SIZE 0x200

// The Battle Castle's script data (BattleCastleData_Alloc, Frontier_GetData while the castle's scripts run).
typedef struct BattleCastleData {
    enum HeapID heapId;                                       // 0x000
    SaveData *saveData;                                       // 0x004
    void *challenge;                                          // 0x008, Save_Frontier_GetCastleChallenge
    BattleSetup *battleSetup;                                 // 0x00C
    u8 type;                                                  // 0x010, single / double / multi / link multi
    u8 battleNo;                                              // 0x011, battle in the current round (0-6)
    u8 monFainted;                                            // 0x012, set by BattleCastleData_CalcPointsEarned
    u8 unk_013;                                               // 0x013
    u16 winStreak;                                            // 0x014
    u16 round;                                                // 0x016, winStreak / 7, at most 8
    u32 unk_018;                                              // 0x018
    BOOL battleWon;                                           // 0x01C
    u16 points;                                               // 0x020, castle points when the challenge started
    u16 savedPoints;                                          // 0x022
    u8 partySlots[CASTLE_PARTY_SIZE];                         // 0x024
    u8 hasStreak;                                             // 0x027, saved in the castle flags
    Party *playerParty;                                       // 0x028
    Party *opponentParty;                                     // 0x02C
    u16 trainerIds[14];                                       // 0x030, 7 per round, the second half for the partner
    FrontierTrainerData trainers[2];                          // 0x04C, the current opponents
    u16 opponentSetIds[CASTLE_OPPONENT_COUNT];                // 0x26C
    u8 opponentIVs[CASTLE_OPPONENT_COUNT];                    // 0x274
    u32 opponentPersonalities[CASTLE_OPPONENT_COUNT];         // 0x278
    FrontierMon opponentMons[CASTLE_OPPONENT_COUNT];          // 0x288
    u8 filler_368[2];                                         // 0x368
    u16 heldItems[CASTLE_PARTY_SIZE];                         // 0x36A, of the party, given back after the challenge
    u8 unk_370[4];                                            // 0x370, the castle apps' result (BattleCastle_OnAppExit)
    u8 unk_374[4];                                            // 0x374
    u8 unk_378[4];                                            // 0x378
    u8 unk_37C[4];                                            // 0x37C
    u16 selections[6];                                           // 0x380
    u8 filler_38C[8];                                         // 0x38C
    u16 startPP[4][4];                                        // 0x394, PP of each move when the battle started
    u8 filler_3B4[0xC];                                       // 0x3B4
    u16 linkBuffer[40];                                       // 0x3C0
    u8 linkMonBuffer[CASTLE_LINK_MON_BUFFER_SIZE];            // 0x410
    u8 linkRecvMon[2][CASTLE_LINK_MON_BUFFER_SIZE];           // 0x610
    u8 unk_A10;                                               // 0xA10
    u8 unk_A11;                                               // 0xA11
    u16 partnerRound;                                         // 0xA12
    u8 filler_A14[2];                                         // 0xA14
    u16 partnerWinStreak;                                     // 0xA16
    u8 choice;                                                // 0xA18
    u8 partnerChoice;                                         // 0xA19
    u8 linkMessageCount;                                      // 0xA1A
    u8 decidedChoice;                                         // 0xA1B, + 6 when the partner's choice was taken
    u16 partnerPoints;                                        // 0xA1C
    u8 filler_A1E[2];                                         // 0xA1E
    u16 *resultVar;                                           // 0xA20
    u32 unk_A24;                                              // 0xA24
} BattleCastleData; // size 0xA28

// Arguments of the castle party / opponent apps (BattleCastle_FillAppArgs).
typedef struct BattleCastleAppArgs {
    SaveData *saveData;      // 0x00
    u8 type;                 // 0x04
    u8 filler_05[3];         // 0x05
    u8 unk_08[4];            // 0x08
    u8 unk_0C[4];            // 0x0C
    u8 unk_10[4];            // 0x10
    u8 unk_14[4];            // 0x14
    Party *playerParty;      // 0x18
    Party *opponentParty;    // 0x1C
    u16 selection;           // 0x20, written by the party app
    u8 filler_22[2];         // 0x22
    BattleCastleData *data;  // 0x24
    u16 partnerPoints;       // 0x28
    u8 filler_2A[6];         // 0x2A
} BattleCastleAppArgs; // size 0x30

BattleCastleData *BattleCastleData_Alloc(SaveData *saveData, BOOL resume, u8 type, int slot0, u16 slot1, u16 slot2, u16 *resultVar);
void BattleCastleData_Init(BattleCastleData *data, u32 resume);
void BattleCastleData_Free(BattleCastleData *data);
void BattleCastleData_ReadAppResult(BattleCastleData *data, BattleCastleAppArgs *args);
void BattleCastleData_Save(BattleCastleData *data, int mode);
u8 BattleCastleData_NextBattle(BattleCastleData *data);
u8 BattleCastleData_GetBattleNo(BattleCastleData *data);
u32 BattleCastleData_GetTrainerSprite(BattleCastleData *data, u32 index);
void BattleCastleData_ResetRanks(BattleCastleData *data);
void BattleCastleData_EndRound(BattleCastleData *data);
void BattleCastleData_SetPartyBeforeBattle(BattleCastleData *data);
void BattleCastleData_GenerateOpponents(BattleCastleData *data);
void BattleCastleData_RecordLinkPartyPP(BattleCastleData *data);
int BattleCastleData_CalcPointsEarned(BattleCastleData *data);
void BattleCastle_AddPoints(SaveData *saveData, u32 type, s32 amount);
BOOL BattleCastleData_SendLinkMessage(BattleCastleData *data, u32 kind, u16 value);
void BattleCastle_ShowPointsWindows(FrontierSystem *system, BattleCastleData *data);
void BattleCastle_HidePointsWindows(FrontierSystem *system, BattleCastleData *data);
void BattleCastle_PrintPoints(FrontierSystem *system, BattleCastleData *data);
u32 BattleCastleData_GetRoundTableValue(BattleCastleData *data);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_DATA_H
