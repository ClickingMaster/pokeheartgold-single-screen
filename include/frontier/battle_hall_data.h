#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H

#include "battle/battle_setup.h"
#include "frontier/frontier_trainers.h"

#include "party.h"
#include "pokemon.h"
#include "save.h"
#include "scrcmd_9.h"

#define HALL_TRAINER_COUNT         20 // 2 per battle of a round (the second for the partner), 10 battles
#define HALL_CELL_COUNT            18 // type cells of the board with a rank (the others are the Hall Matron's)
#define HALL_TYPE_RANKS_SIZE       9  // 18 ranks of 4 bits
#define HALL_LINK_MON_BUFFER_SIZE  0x200

typedef struct BattleHallData {
    enum HeapID heapId;
    u8 type; // single / double / multi / link multi
    union {
        u8 battleNo;
        struct {
            u8 battleNoBits : 7;
            u8 unk_005_7 : 1;
        };
    };
    u8 hasStreak; // saved in the Hall flags
    u8 opponentLevel;
    u16 winStreak;
    u16 round;      // winStreak / 10, at most 18
    fx32 levelSqrt; // square root of the party level
    u32 unk_010;
    BOOL battleWon;
    u16 trainerIds[HALL_TRAINER_COUNT]; // [battleNo * 2] and its partner [battleNo * 2 + 1]
    FrontierTrainerData trainers[2];
    u8 partySlots[2];
    u8 padding_262[2];
    Party *party;
    u16 setIds[HALL_TRAINER_COUNT]; // like trainerIds
    FrontierMon opponentMons[HALL_TRAINER_COUNT]; // like trainerIds
    u8 padding_6F0[2];
    u16 cursor;  // board cell picked in the board app
    u8 cellType; // Pokemon type of the cell
    u8 cell;     // cursor clamped to the ranked cells
    u8 padding_6F6[2];
    void *challenge;
    SaveData *saveData;
    BattleSetup *battleSetup;
    u8 typeRanks[4][HALL_TYPE_RANKS_SIZE]; // per type
    u16 heldItems[2];                      // of the party, given back after the challenge
    u16 linkBuffer[22];
    u8 padding_758[0x2C];
    u8 linkMonBuffer[HALL_LINK_MON_BUFFER_SIZE];
    u8 linkRecvMon[2][HALL_LINK_MON_BUFFER_SIZE];
    u16 partnerLevel;
    u8 padding_D86[2];
    u8 partnerValue;
    u8 padding_D89[3];
    Pokemon *partnerMon;
    u8 linkMessageCount;
    u8 padding_D91[7];
} BattleHallData;

typedef struct BattleHallAppArgs {
    SaveData *saveData;
    u8 mode; // BattleHallData.type
    u8 cursor;
    u16 result;        // the picked cell
    void *hallRecords; // the type ranks of the type
    Party *party;
    BattleHallData *data;
    u16 *partnerLevel;
    u16 battleNo; // the win streak
    u8 padding_1A[6];
} BattleHallAppArgs;

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
u8 BattleHallData_GetWonBattlePoints(BattleHallData *data);
u16 BattleHall_GetSavedTypeRank(SaveData *saveData, int type, int cell, u16 *low, u16 *high);
void BattleHall_SetSavedTypeRank(SaveData *saveData, u8 type, s32 cell, u8 rank);
void BattleHallData_ResetMaxedRanks(BattleHallData *data);
void BattleHallData_UpdateLevelSqrt(BattleHallData *data);
u16 BattleHallData_CalcOpponentLevel(BattleHallData *data);
u16 BattleHallData_GetTrainerSprite(BattleHallData *, u32);
BOOL BattleHallData_SendLinkMessage(BattleHallData *, u32, u32);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_HALL_DATA_H
