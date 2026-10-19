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

typedef struct BattleCastleData {
    enum HeapID heapId;
    SaveData *saveData;
    void *challenge;
    BattleSetup *battleSetup;
    u8 type;     // single / double / multi / link multi
    u8 battleNo; // battle in the current round (0-6)
    u8 monFainted;
    u8 unk_013;
    u16 winStreak;
    u16 round; // winStreak / 7, at most 8
    u32 unk_018;
    BOOL battleWon;
    u16 points; // castle points when the challenge started
    u16 savedPoints;
    u8 partySlots[CASTLE_PARTY_SIZE];
    u8 hasStreak; // saved in the castle flags
    Party *playerParty;
    Party *opponentParty;
    u16 trainerIds[14]; // 7 per round, the second half for the partner
    FrontierTrainerData trainers[2];
    u16 opponentSetIds[CASTLE_OPPONENT_COUNT];
    u8 opponentIVs[CASTLE_OPPONENT_COUNT];
    u32 opponentPersonalities[CASTLE_OPPONENT_COUNT];
    FrontierMon opponentMons[CASTLE_OPPONENT_COUNT];
    u8 filler_368[2];
    u16 heldItems[CASTLE_PARTY_SIZE]; // of the party, given back after the challenge
    u8 opponentChecked[4];            // the castle apps' result
    u8 opponentLevelChanges[4];
    u8 opponentStatsBought[4];
    u8 opponentMovesBought[4];
    u16 selections[6];
    u8 filler_38C[8];
    u16 startPP[4][4]; // PP of each move when the battle started
    u8 filler_3B4[0xC];
    u16 linkBuffer[40];
    u8 linkMonBuffer[CASTLE_LINK_MON_BUFFER_SIZE];
    u8 linkRecvMon[2][CASTLE_LINK_MON_BUFFER_SIZE];
    u8 partnerValueA;
    u8 partnerValueB;
    u16 partnerRound;
    u8 filler_A14[2];
    u16 partnerWinStreak;
    u8 choice;
    u8 partnerChoice;
    u8 linkMessageCount;
    u8 decidedChoice; // + 6 when the partner's choice was taken
    u16 partnerPoints;
    u8 filler_A1E[2];
    u16 *resultVar;
    u32 unk_A24;
} BattleCastleData;

typedef struct BattleCastleAppArgs {
    SaveData *saveData;
    u8 type;
    u8 filler_05[3];
    u8 checked[4];
    u8 levelChanges[4];
    u8 statsBought[4];
    u8 movesBought[4];
    Party *playerParty;
    Party *opponentParty;
    u16 selection; // written by the party app
    u8 filler_22[2];
    BattleCastleData *data;
    u16 partnerPoints;
    u8 filler_2A[6];
} BattleCastleAppArgs;

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
