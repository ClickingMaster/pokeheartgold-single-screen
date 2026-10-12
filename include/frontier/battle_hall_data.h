#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H

#include "battle/battle_setup.h"
#include "frontier/overlay_80_02229EE0.h"

#include "party.h"
#include "pokemon.h"
#include "save.h"
#include "scrcmd_9.h"

#define HALL_TRAINER_COUNT         20 // 2 per battle of a round (the second for the partner), 10 battles
#define HALL_OPPONENT_COUNT        4
#define HALL_CELL_COUNT            18 // type cells of the board with a rank (the others are the Hall Matron's)
#define HALL_TYPE_RANKS_SIZE       9  // 18 ranks of 4 bits
#define HALL_LINK_MON_BUFFER_SIZE  0x200

// A trainer as FrontierTrainer_Load reads it.
typedef struct BattleHallTrainerRecord {
    u32 unk_00;            // 0x00
    u16 trainerClass;      // 0x04
    u16 unk_06;            // 0x06
    u8 filler_08[0x28];    // 0x08
} BattleHallTrainerRecord; // size 0x30

// The Battle Hall's script data (BattleHallData_Alloc, Frontier_GetData while the Hall's scripts run).
typedef struct BattleHallData {
    enum HeapID heapId;                                  // 0x000
    u8 type;                                             // 0x004, single / double / multi / link multi
    union {
        u8 battleNo;                                     // 0x005, battle in the current round
        struct {
            u8 battleNoBits : 7;                         // 0x005, the same (BattleHall_GetOpponentLevel reads it so)
            u8 unk_005_7 : 1;
        };
    };
    u8 hasStreak;                                        // 0x006, saved in the Hall flags
    u8 opponentLevel;                                    // 0x007
    u16 winStreak;                                       // 0x008
    u16 round;                                           // 0x00A, winStreak / 10, at most 18
    fx32 levelSqrt;                                      // 0x00C, square root of the party level
    u32 unk_010;                                         // 0x010
    BOOL battleWon;                                      // 0x014
    u16 trainerIds[HALL_TRAINER_COUNT];                  // 0x018, [battleNo * 2] and its partner [battleNo * 2 + 1]
    FrontierTrainerData trainers[2];                     // 0x040, the current opponents
    u8 partySlots[2];                                    // 0x260
    u8 filler_262[2];                                    // 0x262
    Party *party;                                        // 0x264
    u16 setIds[HALL_TRAINER_COUNT];                      // 0x268, like trainerIds
    FrontierMon opponentMons[HALL_OPPONENT_COUNT];       // 0x290
    u8 filler_370[0x382];                                // 0x370
    u16 cursor;                                          // 0x6F2, board cell picked in the board app
    u8 cellType;                                         // 0x6F4, Pokemon type of the cell
    u8 cell;                                             // 0x6F5, cursor clamped to the ranked cells
    u8 filler_6F6[2];                                    // 0x6F6
    void *challenge;                                     // 0x6F8, Save_Frontier_GetHallChallenge
    SaveData *saveData;                                  // 0x6FC
    BattleSetup *battleSetup;                            // 0x700
    u8 typeRanks[4][HALL_TYPE_RANKS_SIZE];               // 0x704, per type (HallTypeRanks_Get)
    u16 heldItems[2];                                    // 0x728, of the party, given back after the challenge
    u16 linkBuffer[22];                                  // 0x72C
    u8 filler_758[0x2C];                                 // 0x758
    u8 linkMonBuffer[HALL_LINK_MON_BUFFER_SIZE];         // 0x784
    u8 linkRecvMon[2][HALL_LINK_MON_BUFFER_SIZE];        // 0x984
    u16 partnerLevel;                                    // 0xD84
    u8 filler_D86[2];                                    // 0xD86
    u8 partnerValue;                                     // 0xD88, received by ov80_0222B048
    u8 filler_D89[3];                                    // 0xD89
    Pokemon *partnerMon;                                 // 0xD8C
    u8 linkMessageCount;                                 // 0xD90
    u8 filler_D91[7];                                    // 0xD91
} BattleHallData; // size 0xD98

// Arguments of the Battle Hall board app (OVY_82, FrtCmd_HallLaunchApp).
typedef struct BattleHallAppArgs {
    SaveData *saveData;   // 0x00
    u8 mode;              // 0x04, BattleHallData.type
    u8 cursor;            // 0x05
    u16 result;           // 0x06, the picked cell
    void *hallRecords;    // 0x08, the type ranks of the type
    Party *party;         // 0x0C
    BattleHallData *data; // 0x10
    u16 *partnerLevel;    // 0x14
    u16 battleNo;         // 0x18, the win streak
    u8 filler_1A[6];      // 0x1A
} BattleHallAppArgs; // size 0x20

BattleHallData *BattleHallData_Alloc(SaveData *saveData, BOOL resume, u8 type, u8 slot0, u8 slot1);
void BattleHallData_Init(BattleHallData *data, u32 unused);
void BattleHallData_GenerateOpponents(BattleHallData *data);
void BattleHallData_Free(BattleHallData *data);
void BattleHallData_ReadAppResult(BattleHallData *data, BattleHallAppArgs *args);
u16 BattleHallAppArgs_GetResult(BattleHallAppArgs *args, u32 unused);
void BattleHallData_UpdateSpeciesRecord(BattleHallData *data, u16 *result);
void BattleHallData_Save(BattleHallData *data, int mode);
u8 BattleHallData_NextBattle(BattleHallData *data);
u8 BattleHallData_GetBattleNo(BattleHallData *data);
void BattleHallData_ClearRanks(BattleHallData *data);
void BattleHallData_EndRound(BattleHallData *data);
u8 ov80_02231888(BattleHallData *data);
u16 BattleHall_GetSavedTypeRank(SaveData *saveData, int type, int cell, u16 *low, u16 *high);
void BattleHall_SetSavedTypeRank(SaveData *saveData, u8 type, s32 cell, u8 rank);
void BattleHallData_ResetMaxedRanks(BattleHallData *data);
void BattleHallData_UpdateLevelSqrt(BattleHallData *data);
u16 BattleHallData_CalcOpponentLevel(BattleHallData *data);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H
