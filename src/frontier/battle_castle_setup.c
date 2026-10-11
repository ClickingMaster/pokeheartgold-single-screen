#include "frontier/battle_castle_setup.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/battle_castle_data.h"
#include "frontier/frontier.h"

#include "assert.h"
#include "bg_window.h"
#include "communication_player_state.h"
#include "frontier_save_access.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "unk_02035900.h"
#include "use_item_on_mon.h"

// Battle Castle trainers and battles: the trainers of a round, the battle setup of a castle battle, the party kept
// between battles, the round tiles and spending castle points.

// A trainer as FrontierTrainer_Load reads it.
typedef struct BattleCastleTrainerRecord {
    u8 data[0x30];
} BattleCastleTrainerRecord;

// Trainer ids of a round: regular trainers, and the 7th battle of a round.
typedef struct BattleCastleTrainerRange {
    u16 min;
    u16 max;
    u16 lastBattleMin;
    u16 lastBattleMax;
} BattleCastleTrainerRange;

extern const u16 gBattleCastleRoundTrainerRanges[];
extern const u16 ov80_0223D4D6[];
extern const u16 ov80_0223D4D8[];
extern const u16 ov80_0223D4DA[];

void *FrontierTrainer_Load(BattleCastleTrainerRecord *record, int trainerId, enum HeapID heapID, int narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *setup, BattleCastleTrainerRecord *record, int count, int battler, enum HeapID heapID);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
int sub_0205C1F0(int type);
int sub_0205C218(int type);
void FrontierSave_SubStat(FrontierSave *frontierSave, int stat, int substat, int amount);
void FrontierSave_AddStat(FrontierSave *frontierSave, int stat, int substat, int amount);

int BattleCastle_GetRandomTrainerId(int type, u32 round, int battleNo) {
    int min;
    int span;

    if (type == 0) {
        int battle = battleNo + 1;
        int number = 7 * round + battle;
        if (number == 21) {
            return 313;
        }
        if (number == 49) {
            return 314;
        }
    }
    if (round >= 8) {
        round = 7;
    }
    if (battleNo == 6 || battleNo == 13) {
        min = ov80_0223D4D8[round * 4];
        span = ov80_0223D4DA[round * 4] - min;
    } else {
        min = gBattleCastleRoundTrainerRanges[round * 4];
        span = ov80_0223D4D6[round * 4] - min;
    }
    return min + LCRandom() % span;
}

#pragma force_active on
void BattleCastle_GetRoundTrainerIds(u32 type, u32 round, u16 *trainerIds, int count) {
    int i = 0;
    u16 *trainerId = trainerIds;

    do {
        int j;
        *trainerId = BattleCastle_GetRandomTrainerId(type, round, i);
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

int BattleCastle_GetPartySize(int type, int both) {
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

int BattleCastle_GetOpponentMonCount(int type, int both) {
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
BattleSetup *BattleCastle_NewBattleSetup(BattleCastleData *data, FrontierLaunchArgs *args) {
    int i;
    int playerCount = BattleCastle_GetPartySize(data->type, FALSE);
    int opponentCount = BattleCastle_GetOpponentMonCount(data->type, FALSE);
    int slot;
    int j;
    int count;
    BattleSetup *setup;
    Pokemon *mon;
    BattleCastleTrainerRecord trainer;

    HealParty(data->opponentParty);
    setup = BattleSetup_New(HEAP_ID_FIELD2, BattleCastle_GetBattleType(data->type));
    sub_02051D18(setup, NULL, args->saveData, args->mapId, args->bagCursor, args->unk1C);
    setup->battleBg = BATTLE_BG_BATTLE_CASTLE;
    setup->terrain = TERRAIN_BATTLE_CASTLE;
    Party_InitWithMaxSize(setup->party[BATTLER_PLAYER], playerCount);
    // in link battles the second player's Pokemon follow the first player's in the party
    slot = Comm_GetLocalNetId() == 0 ? 0 : 2;
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < playerCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(data->playerParty, slot), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_PLAYER);
        slot++;
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[data->battleNo], HEAP_ID_FIELD2, 204));
    FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY, HEAP_ID_FIELD2);
    Party_InitWithMaxSize(setup->party[BATTLER_ENEMY], BattleCastle_GetOpponentMonCount(data->type, FALSE));
    for (j = 0; j < BATTLER_MAX; j++) {
        setup->trainer[j].data.aiFlags = BattleCastle_GetTrainerAIFlags(data);
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (slot = 0; slot < opponentCount; slot++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(data->opponentParty, slot), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY);
    }
    Heap_Free(mon);
    switch (data->type) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(setup);
        PlayerProfile_Copy(CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId()), setup->profile[BATTLER_PLAYER2]);
        Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[data->battleNo + 7], HEAP_ID_FIELD2, 204));
        FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY2, HEAP_ID_FIELD2);
        Party_InitWithMaxSize(setup->party[BATTLER_ENEMY2], BattleCastle_GetOpponentMonCount(data->type, FALSE));
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        count = 0;
        if (opponentCount > 0) {
            slot = opponentCount;
            do {
                CopyPokemonToPokemon(Party_GetMonByIndex(data->opponentParty, slot), mon);
                BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY2);
                slot++;
                count++;
            } while (count < opponentCount);
        }
        Heap_Free(mon);
        break;
    }
    return setup;
}
#pragma force_active reset

u32 BattleCastle_GetBattleType(int type) {
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

int BattleCastle_GetMonLevel(BattleCastleData *data) {
    return 50;
}

BOOL BattleCastle_IsMultiType(int type) {
    switch (type) {
    case 2:
    case 3:
        return TRUE;
    default:
        return FALSE;
    }
}

void BattleCastle_ReviveParty(Party *party) {
    int count = Party_GetCount(party);
    int i;

    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        int value;
        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
                value = 1;
                SetMonData(mon, MON_DATA_HP, &value);
            }
            value = 0;
            SetMonData(mon, MON_DATA_STATUS, &value);
        }
    }
}

void BattleCastle_SetMonOriginalTrainer(BattleCastleData *data, Pokemon *mon) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(data->saveData);
    sub_0207217C(mon, profile, 4, 0, 0, HEAP_ID_FIELD2);
}

void BattleCastle_AddMonToParty(BattleCastleData *data, Party *party, Pokemon *mon) {
    BattleCastle_SetMonOriginalTrainer(data, mon);
    Party_AddMon(party, mon);
}

void BattleCastle_SetOpponentParty(BattleCastleData *data) {
    int count;
    Pokemon *mon;
    int i;
    FrontierMon *frontierMon;

    SaveArray_Party_Init(data->opponentParty);
    count = BattleCastle_GetOpponentMonCount(data->type, TRUE);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    i = 0;
    if (count > 0) {
        frontierMon = data->opponentMons;
        do {
            FrontierMon_ToPokemon(frontierMon, mon, BattleCastle_GetMonLevel(data));
            BattleCastle_AddMonToParty(data, data->opponentParty, mon);
            i++;
            frontierMon++;
        } while (i < count);
    }
    Heap_Free(mon);
}

int BattleCastle_GetTrainerAIFlags(BattleCastleData *data) {
    int round;
    int aiFlags;

    if (data->type == 0 && (data->trainerIds[data->battleNo] == 313 || data->trainerIds[data->battleNo] == 314)) {
        return 7;
    }
    round = BattleCastle_GetRound(data);
    round++;
    aiFlags = 7;
    switch (round) {
    case 1:
    case 2:
        aiFlags = 0;
        break;
    case 3:
    case 4:
        aiFlags = 1;
        break;
    }
    return aiFlags;
}

u16 BattleCastle_GetRound(BattleCastleData *data) {
    u16 round = data->round;

    if (BattleCastle_IsMultiType(data->type) == TRUE) {
        u16 partnerRound = data->partnerRound;
        if (partnerRound > data->round) {
            round = partnerRound;
        }
    }
    return round;
}

void BattleCastle_DrawRoundTiles(BgConfig *bgConfig, BattleCastleData *data, int bgId) {
    u16 tilemap[30];

    BattleCastle_BuildRoundTilemap(tilemap, BattleCastle_GetRound(data));
    LoadRectToBgTilemapRect(bgConfig, bgId, tilemap, 11, 6, 10, 3);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

#pragma force_active on
void BattleCastle_BuildRoundTilemap(u16 *tilemap, int round) {
    u8 tiles[10];
    u32 i, x, y;
    int palette = BattleCastle_ClampRound(round);

    // a 10x3 frame mirrored horizontally (tiles 0-4, then 4-0 flipped), 3 tile rows per round color
    for (i = 0; i < 5; i++) {
        tiles[i] = i;
        tiles[5 + i] = 4 - i;
    }
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 10; x++) {
            tilemap[y * 10 + x] = tiles[x] + (palette * 0x60 + 0x10 + y * 0x20);
            if (x >= 5) {
                tilemap[y * 10 + x] |= 0x400;
            }
        }
    }
}
#pragma force_active reset

int BattleCastle_ClampRound(int round) {
    if ((u32)round >= 8) {
        round = 7;
    }
    return round;
}

void BattleCastle_SpendPoints(FrontierSave *frontierSave, int type, int amount) {
    int stat = sub_0205C1F0(type);
    int used;

    FrontierSave_SubStat(frontierSave, stat, sub_0205C268(sub_0205C1F0(type)), amount);
    stat = sub_0205C218(type);
    used = FrontierSave_GetStat(frontierSave, stat, sub_0205C268(sub_0205C218(type)));
    used += amount;
    if (used > 9999) {
        stat = sub_0205C218(type);
        FrontierSave_SetStat(frontierSave, stat, sub_0205C268(sub_0205C218(type)), 9999);
    } else {
        stat = sub_0205C218(type);
        FrontierSave_AddStat(frontierSave, stat, sub_0205C268(sub_0205C218(type)), amount);
    }
}
