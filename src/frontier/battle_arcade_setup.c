#include "frontier/battle_arcade_setup.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/frontier.h"
#include "frontier/battle_arcade_data.h"

#include "assert.h"
#include "communication_player_state.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "unk_02035900.h"
#include "use_item_on_mon.h"

// Battle Arcade trainers and battles: the trainers of a round, the battle setup of an arcade battle (with the weather
// and party swap events of the game board), the opponents' party and the game board event colors.

#define ARCADE_EVENT_SWAP_PARTIES 27
#define ARCADE_EVENT_28           28

// A trainer as FrontierTrainer_Load reads it.
typedef struct BattleArcadeTrainerRecord {
    u8 data[0x30];
} BattleArcadeTrainerRecord;

extern const u16 gBattleArcadeRoundTrainerRanges[];
extern const u16 ov80_0223D516[];
extern const u16 ov80_0223D518[];
extern const u16 ov80_0223D51A[];

void *FrontierTrainer_Load(BattleArcadeTrainerRecord *record, u16 trainerId, enum HeapID heapID, int narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *setup, BattleArcadeTrainerRecord *record, int count, int battler, enum HeapID heapID);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
// not in the header: battle_arcade_data.c declares it with an unsigned count
void BattleArcade_GetRoundTrainerIds(u32 type, u32 round, u16 *trainerIds, int count);

u32 BattleArcade_GetRandomTrainerId(u32 type, u32 round, u32 battleNo) {
    u16 min;
    s32 span;

    if (type == 0) {
        u32 battle = battleNo + 1;
        battle += round * 7;
        if (battle == 21) {
            return 311;
        }
        if (battle == 49) {
            return 312;
        }
    }
    if (round >= 8) {
        round = 7;
    }
    if (battleNo == 6 || battleNo == 13) {
        min = ov80_0223D518[round * 4];
        span = ov80_0223D51A[round * 4] - min;
    } else {
        min = gBattleArcadeRoundTrainerRanges[round * 4];
        span = ov80_0223D516[round * 4] - min;
    }
    return min + LCRandom() % span;
}

#pragma force_active on
void BattleArcade_GetRoundTrainerIds(u32 type, u32 round, u16 *trainerIds, int count) {
    int i = 0;
    u16 *trainerId = trainerIds;

    do {
        int j;
        *trainerId = BattleArcade_GetRandomTrainerId(type, round, i);
        for (j = 0; j < i; j++) {
            if (trainerIds[j] == trainerIds[i]) {
                break;
            }
        }
        if (j == i) {
            trainerId++;
            i++;
        }
    } while (i < count);
}
#pragma force_active reset

u8 BattleArcade_GetMonCount(u8 type, int both) {
    switch (type) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        if (both == FALSE) {
            return 2;
        }
        return 4;
    default:
        GF_AssertFail();
        return 3;
    }
}

u8 BattleArcade_GetOpponentMonCount(u8 type, int both) {
    switch (type) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        if (both == FALSE) {
            return 2;
        }
        return 4;
    default:
        GF_AssertFail();
        return 3;
    }
}

#pragma force_active on
BattleSetup *BattleArcade_NewBattleSetup(ArcadeContext *ctx, FrontierLaunchArgs *args) {
    BattleArcadeTrainerRecord trainer;
    int i;
    Party *playerParty;
    Party *opponentParty;
    u8 playerCount;
    u8 opponentCount;
    BattleSetup *setup;
    Pokemon *mon;
    int slot;

    playerCount = BattleArcade_GetMonCount(ctx->type, FALSE);
    opponentCount = BattleArcade_GetOpponentMonCount(ctx->type, FALSE);
    setup = BattleSetup_New(HEAP_ID_FIELD2, BattleArcade_GetBattleType(ctx->type));
    sub_02051D18(setup, NULL, args->saveData, args->mapId, args->bagCursor, args->unk1C);
    setup->battleBg = BATTLE_BG_BATTLE_ARCADE;
    setup->terrain = TERRAIN_BATTLE_ARCADE;
    setup->weatherType = ctx->weather;
    opponentParty = ctx->opponentParty;
    playerParty = ctx->playerParty;
    if (ctx->eventId == ARCADE_EVENT_SWAP_PARTIES) {
        playerParty = ctx->opponentParty;
        opponentParty = ctx->playerParty;
    }
    Party_InitWithMaxSize(setup->party[BATTLER_PLAYER], playerCount);
    // in link battles the second player's Pokemon follow the first player's in the party
    if (Comm_GetLocalNetId() == FALSE) {
        slot = 0;
    } else {
        slot = 2;
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < playerCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(playerParty, slot), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_PLAYER);
        slot++;
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(&trainer, ctx->trainerIds[ctx->battleNo], HEAP_ID_FIELD2, 204));
    FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY, HEAP_ID_FIELD2);
    Party_InitWithMaxSize(setup->party[BATTLER_ENEMY], BattleArcade_GetOpponentMonCount(ctx->type, FALSE));
    for (i = 0; i < BATTLER_MAX; i++) {
        setup->trainer[i].data.aiFlags = BattleArcade_GetTrainerAIFlags(ctx);
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < opponentCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(opponentParty, i), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY);
    }
    Heap_Free(mon);
    switch (ctx->type) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(setup);
        PlayerProfile_Copy(CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId()), setup->profile[BATTLER_PLAYER2]);
        Heap_Free(FrontierTrainer_Load(&trainer, ctx->trainerIds[ctx->battleNo + 7], HEAP_ID_FIELD2, 204));
        FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY2, HEAP_ID_FIELD2);
        Party_InitWithMaxSize(setup->party[BATTLER_ENEMY2], BattleArcade_GetOpponentMonCount(ctx->type, FALSE));
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        for (i = 0; i < opponentCount; i++) {
            CopyPokemonToPokemon(Party_GetMonByIndex(opponentParty, opponentCount + i), mon);
            BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY2);
        }
        Heap_Free(mon);
        break;
    }
    HealParty(ctx->playerParty);
    HealParty(ctx->opponentParty);
    return setup;
}
#pragma force_active reset

u32 BattleArcade_GetBattleType(int type) {
    switch (type) {
    case 0:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER;
    case 1:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    case 2:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    case 3:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    default:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER;
    }
}

int BattleArcade_GetMonLevel(ArcadeContext *ctx) {
    return 50;
}

BOOL BattleArcade_MultiplayerCheck(int type) {
    switch (type) {
    case 2:
    case 3:
        return TRUE;
    default:
        return FALSE;
    }
}

void BattleArcade_SetMonOriginalTrainer(ArcadeContext *ctx, Pokemon *mon) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(ctx->saveData);
    MonSetCatchDataNoHealBall(mon, profile, 4, 0, 0, HEAP_ID_FIELD2);
}

void BattleArcade_AddMonToParty(ArcadeContext *ctx, Party *party, Pokemon *mon) {
    BattleArcade_SetMonOriginalTrainer(ctx, mon);
    Party_AddMon(party, mon);
}

#pragma force_active on
void BattleArcade_SetOpponentParty(ArcadeContext *ctx) {
    int count, i;
    u32 item;
    Pokemon *mon;

    SaveArray_Party_Init(ctx->opponentParty);
    count = BattleArcade_GetOpponentMonCount(ctx->type, TRUE);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        FrontierMon_ToPokemon(&ctx->opponentMons[i], mon, BattleArcade_GetMonLevel(ctx));
        BattleArcade_AddMonToParty(ctx, ctx->opponentParty, mon);
        {
            Pokemon *partyMon = Party_GetMonByIndex(ctx->opponentParty, i);
            item = ITEM_NONE;
            SetMonData(partyMon, MON_DATA_HELD_ITEM, &item);
        }
    }
    Heap_Free(mon);
}
#pragma force_active reset

u32 ov80_02238430(ArcadeContext *ctx, u8 eventId) {
    BattleArcade_GetRound(ctx);
    if (eventId == ARCADE_EVENT_28) {
        return 1;
    }
    return 3;
}

u32 BattleArcade_GetTrainerAIFlags(ArcadeContext *ctx) {
    s32 round;
    s32 aiFlags;

    if (ctx->type == 0) {
        u16 *trainerId = &ctx->trainerIds[ctx->battleNo];
        u16 id = *trainerId;
        if (id == 311 || id == 312) {
            return 7;
        }
    }
    round = BattleArcade_GetRound(ctx);
    aiFlags = 7;
    switch (round) {
    case -1:
        break;
    case 0:
    case 1:
        aiFlags = 0;
        break;
    case 2:
    case 3:
        aiFlags = 1;
        break;
    }
    return aiFlags;
}

u32 BattleArcade_GetRound(ArcadeContext *ctx) {
    u16 round = ctx->round;

    if (BattleArcade_MultiplayerCheck(ctx->type) == TRUE) {
        u16 partnerRound = ctx->partnerRound;
        if (partnerRound > ctx->round) {
            round = partnerRound;
        }
    }
    return round;
}

u32 BattleArcade_GetEventColor(u8 eventId) {
    if (eventId < 9) {
        return 0;
    }
    if (eventId < 18) {
        return 1;
    }
    if (eventId < 27) {
        return 2;
    }
    return 3;
}
