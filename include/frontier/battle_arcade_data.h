#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_ARCADE_DATA_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_ARCADE_DATA_H

#include "frontier/frontier_map.h"
#include "frontier/frontier_trainers.h"

#include "battle_arcade_game_board.h"
#include "save.h"
#include "scrcmd_9.h"

typedef struct ArcadeContext {
    u32 heapId;
    SaveData *saveData;
    u32 challenge; // the ArcadeChallenge save record (Save_Frontier_GetArcadeChallenge)
    BattleSetup *battleSetup;
    u8 type;
    u8 battleNo; // battle number in the current round (0-6)
    u8 randomFlag;
    u8 eventId; // game board event to apply, 0x20 = none
    u32 weather;
    u16 winStreak;
    u16 round; // winStreak / 7, capped at 8
    u8 cursorSpeed;
    u8 bpGain;
    u8 unk1E;
    u8 eventNoEffect; // set when the event affected no Pokemon
    u16 eventRandom;
    u16 unk22;
    u32 unk24;
    BOOL battleWon;
    u8 partySlots[3]; // party indices of the entered Pokemon
    u8 hasStreak;
    ManagedSprite *playerMonSprites[4];
    ManagedSprite *opponentMonSprites[4];
    ManagedSprite *playerItemIcons[4];
    ManagedSprite *opponentItemIcons[4];
    Party *playerParty;
    Party *opponentParty;
    u16 trainerIds[14]; // 7 per round, second half for the partner in multi battles
    u32 savedHp[4];
    u32 savedAtk[4];
    u32 savedDef[4];
    u32 savedSpd[4];
    u32 savedSpAtk[4];
    u32 savedSpDef[4];
    FrontierTrainerData trainers[2]; // 0xF4, the current opponents (trainerIds[battleNo], [battleNo + 7])
    u16 setIds[4];        // 0x314, the opponents' Pokemon sets, with their IVs and personalities
    u8 ivs[4];
    u32 personalities[4];
    FrontierMon opponentMons[4]; // 0x330
    u8 filler_410[2];
    u16 heldItems[3]; // held items of the entered Pokemon, restored afterwards
    u16 gameBoardResults[6];
    u16 linkBuffer[20];      // 0x424, outgoing link message
    u8 filler_44C[0x28];
    u8 linkMonBuffer[0x200]; // 0x474, outgoing Pokemon
    u8 linkRecvMon[0x3FE];   // 0x674, received Pokemon, 0x200 per net id (the second one overlaps unkA72)
    u8 unkA72;
    u8 unkA73;
    u8 unkA74;
    u8 unkA75;
    u16 partnerRound; // the link partner's round
    u16 multiWinStreak;
    u8 mirrorParticles;
    u8 unkA7B;
    u8 linkMessageCount;
} ArcadeContext;

void BattleArcadeData_Init(ArcadeContext *ctx, u32 resume);
void BattleArcadeData_Free(ArcadeContext *ctx);
u32 BattleArcade_GetWonBattlePoints(ArcadeContext *ctx, Party *playerParty, Party *opponentParty, int rank);
void BattleArcade_CopyMonHeldItem(Party *srcParty, Party *dstParty, int srcSlot, int dstSlot);
int BattleCastle_IsMultiType(u8 type);
void BattleArcade_StoreGameBoardResult(ArcadeContext *ctx, GAME_BOARD_ARGS *args);
void BattleArcade_SetPartyBeforeBattle(ArcadeContext *ctx);
void BattleArcade_SetPartyAfterBattle(ArcadeContext *ctx);
void BattleArcadeData_SaveRecord(ArcadeContext *ctx, u32 mode);
u32 BattleArcade_IncBattleNo(ArcadeContext *ctx);
u32 BattleArcade_GetTrainerSprite(ArcadeContext *ctx, u8 partner);
void ov80_022347B8(ArcadeContext *ctx);
void BattleArcade_CompleteRound(ArcadeContext *ctx);
u32 BattleArcade_GetBattleNo(ArcadeContext *ctx);
void BattleArcade_CreateMonSprites(ArcadeContext *ctx, FrontierMap *map, u32 opponent);
void BattleArcade_DeleteMonSprites(ArcadeContext *ctx, FrontierMap *map, u32 opponent);
void BattleArcade_CreateItemIcons(ArcadeContext *ctx, FrontierMap *map, u32 opponent);
void BattleArcade_DeleteItemIcons(ArcadeContext *ctx, FrontierMap *map, u32 opponent);
void BattleArcade_LoadEventBg(ArcadeContext *ctx, FrontierMap *map);
void BattleArcade_LoadMultiBgScreen(ArcadeContext *ctx, FrontierMap *map);
u32 BattleArcade_GetTrainerIndex(ArcadeContext *ctx, u32 partner);
void BattleArcade_SetPlayerMonSpriteVisible(ArcadeContext *ctx, FrontierMap *map, u32 visible, u32 slot);
void BattleArcade_SetOpponentMonSpriteVisible(ArcadeContext *ctx, FrontierMap *map, u32 visible, u32 slot);
void BattleArcade_LoadBlendedBgExtPalette(u32 color, u32 mode);
u32 ov80_02235324(ArcadeContext *ctx);
u16 BattleArcade_SendBuffer(ArcadeContext *ctx, u32 cmd, u32 value);
void BattleArcade_ApplyEvent(ArcadeContext *ctx, u8 event);
void BattleArcade_SetItemIconVisible(ArcadeContext *ctx, FrontierMap *map, u32 opponent, u32 slot, u32 visible);

#endif
