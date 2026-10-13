#include "frontier/battle_hall_setup.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/pokemon.h"

#include "battle/battle_setup.h"
#include "frontier/battle_hall_data.h"
#include "frontier/frontier.h"

#include "communication_player_state.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "communication_core.h"
#include "use_item_on_mon.h"

// Battle Hall trainers and battles: the trainers and sets of the next battle (from the cell's type and its rank),
// the battle setup of a Hall battle and the opponents' level.

#define HALL_TRAINER_COUNT_TOTAL 300
#define HALL_SET_COUNT           477
#define HALL_MATRON_SILVER       307 // trainer ids of the Hall Matron, battles 50 and 170 of the single battle streak
#define HALL_MATRON_GOLD         308
// trainerId is HALL_MATRON_SILVER or HALL_MATRON_GOLD (0xFECD is -HALL_MATRON_SILVER as a u16)
#define IS_HALL_MATRON(trainerId) ((u16)((trainerId) + 0xFECD) <= 1)

typedef struct BattleHallSetRange {
    u16 min;
    u16 max;
} BattleHallSetRange;

typedef struct BattleHallSetTypes {
    u16 type1;
    u16 type2;
} BattleHallSetTypes;

extern const BattleHallSetRange gBattleHallMatronSetRanges[];
extern const BattleHallSetRange gBattleHallGoldMatronSetRange;
extern const u8 gBattleHallRankIVs[];
extern const BattleHallSetRange gBattleHallRankSetRanges[];
extern const u16 gBattleHallCellTrainerClasses[];
extern const u16 gBattleHallRankTrainerClasses[];
extern const u16 gBattleHallTrainerClasses[];
extern const u16 gBattleHallSetSpecies[];
extern const BattleHallSetTypes gBattleHallSetTypes[];
extern const u8 gBattleHallCellTypes[];

u32 HallTypeRanks_Get(u32 cell, void *ranks);
fx32 FX_Sqrt(fx32 x);

// Not in battle_hall_setup.h: battle_hall_data.c and frontier_cmd_hall.c declare them with the types they call them
// with.
int BattleHall_GetPartySize(int type);
u8 BattleHall_GetLevelRank(BattleHallData *data);
void *FrontierTrainer_Load(BattleHallTrainerRecord *record, u32 trainerId, enum HeapID heapID, u32 narcId);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, u8 level);
void FrontierTrainer_SetBattleTrainer(BattleSetup *setup, BattleHallTrainerRecord *record, int count, int battler, enum HeapID heapID);
void FrontierMon_GenerateWithRandomOtId(FrontierMon *frontierMon, u16 setId, int index, u8 ivs, int pid, enum HeapID heapID, u32 narcId);

// Picks a trainer class for each of the count trainers of the battle: from the classes of the rank (2 in 3) or of the
// cell. BattleHall_PickTrainerIds turns them into trainer ids.
void BattleHall_PickTrainerClasses(u32 cell, int count, u32 rank, u32 battleNo, u16 *trainerIds) {
    int i;
    u32 clampedRank;
    u16 *dst;
    const u16 *rankClasses;
    const u16 *cellClasses;

    i = 0;
    clampedRank = BattleHall_ClampRank(rank);
    dst = trainerIds + (battleNo & 0x7F) * 2;
    rankClasses = &gBattleHallRankTrainerClasses[(u8)clampedRank * 8];
    cellClasses = &gBattleHallCellTrainerClasses[cell * 4];
    do {
        u16 r = LCRandom() % 12;
        if (r < 8) {
            *dst = rankClasses[r];
        } else {
            *dst = cellClasses[r - 8];
        }
        i++;
        dst++;
    } while (i < count);
}

// Replaces the trainer classes by random trainers of these classes, not used in the round yet. The Hall Matron is the
// opponent of battles 50 and 170 of the single battle streak.
void BattleHall_PickTrainerIds(int type, int count, int round, u32 rank, u8 battleNo, u16 *trainerIds) {
    u16 start;
    int picked;
    u16 *dst;
    u32 index;
    u8 clampedRank;

    picked = 0;
    index = (battleNo & 0x7F) * 2;
    clampedRank = BattleHall_ClampRank(rank);
    if (type == 0) {
        if (battleNo + 1 + round * 10 == 50) {
            trainerIds[index] = HALL_MATRON_SILVER;
            return;
        }
        if (battleNo + 1 + round * 10 == 170) {
            trainerIds[index] = HALL_MATRON_GOLD;
            return;
        }
    }
    dst = &trainerIds[index];
    do {
        u16 trainerId = (u32)LCRandom() % HALL_TRAINER_COUNT_TOTAL;
        start = trainerId;
        int pos = index + picked;
        for (;;) {
            u16 trainerClass = trainerIds[pos];
            if (trainerClass == gBattleHallTrainerClasses[trainerId]) {
                int j;
                for (j = 0; j < pos; j++) {
                    if (trainerId == trainerIds[j]) {
                        break;
                    }
                }
                if (j == pos) {
                    *dst = trainerId;
                    dst++;
                    picked++;
                    break;
                }
            }
            trainerId = (u16)(trainerId + 1);
            if (trainerId >= HALL_TRAINER_COUNT_TOTAL) {
                trainerId = 0;
            }
            if (trainerId == start) {
                // no unused trainer of the class left: pick another class of the rank
                const u16 *rankClasses = gBattleHallRankTrainerClasses;
                rankClasses += clampedRank << 3;
                u16 newClass;
                do {
                    newClass = rankClasses[(u16)((s32)LCRandom() % 8)];
                } while (trainerClass == newClass);
                trainerIds[pos] = newClass;
            }
        }
    } while (picked < count);
}

#pragma force_active on
// Picks count sets of the cell's type (or the Hall Matron's sets) that are not the player's species and not used by
// the earlier battles of the round.
void BattleHall_PickSetIds(u16 count, u16 cellType, int rank, u8 battleNo, u16 playerSpecies, u16 *setIds, int matron) {
    u16 i, j;
    u16 found;
    u16 span;
    u16 index;
    const BattleHallSetRange *range;
    BOOL wrapped;
    u16 start;
    u8 base;
    u8 clampedRank;
    int picked;

    picked = 0;
    base = battleNo * 2;
    wrapped = FALSE;
    clampedRank = BattleHall_ClampRank(rank);
    if (matron != 0) {
        for (i = 0; i < HALL_SET_COUNT; i++) {
            if (playerSpecies == gBattleHallSetSpecies[i]) {
                found = i;
                break;
            }
        }
        if (i == HALL_SET_COUNT) {
            found = 376;
        }
        for (i = 0; i < 4; i++) {
            if (found < gBattleHallMatronSetRanges[i].max) {
                break;
            }
        }
        if (i == 4) {
            i = 3;
        }
        if (matron == 2) {
            range = &gBattleHallGoldMatronSetRange;
        } else {
            range = &gBattleHallMatronSetRanges[i];
        }
    } else {
        range = &gBattleHallRankSetRanges[clampedRank];
    }
    span = range->max - range->min + 1;
    index = range->min + LCRandom() % span;
    index--;
    start = index;
    while (picked < count) {
        if (wrapped == FALSE) {
            for (j = 0; j < base; j++) {
                if (index + 1 == setIds[j]) {
                    break;
                }
            }
        } else if (index + 1 == setIds[base - 2]) {
            j = 0;
        } else {
            j = base;
        }
        if (j == base) {
            if (matron != 0) {
                if (playerSpecies != gBattleHallSetSpecies[index]) {
                    setIds[base + picked] = index + 1;
                    picked++;
                }
            } else if (cellType == gBattleHallSetTypes[index].type1 || cellType == gBattleHallSetTypes[index].type2) {
                if (playerSpecies != gBattleHallSetSpecies[index]) {
                    setIds[base + picked] = index + 1;
                    picked++;
                }
            }
        }
        index++;
        if (index + 1 >= range->max) {
            index = range->min - 1;
        }
        if (index == start) {
            wrapped = TRUE;
        }
    }
}

BattleSetup *BattleHall_NewBattleSetup(BattleHallData *data, FrontierLaunchArgs *args) {
    u32 rank;
    BattleSetup *setup;
    Pokemon *mon;
    Party *party;
    u8 index;
    int playerCount;
    int aiFlags;
    BattleHallTrainerRecord trainer;
    int i;
    s32 opponentCount;

    index = data->battleNo * 2;
    playerCount = BattleHall_GetPartySize(data->type);
    opponentCount = BattleHall_GetOpponentMonCount(data->type);
    party = SaveArray_Party_Get(data->saveData);

    HealParty(party);
    setup = BattleSetup_New(HEAP_ID_FIELD2, BattleHall_GetBattleType(data->type));
    BattleSetup_InitFromSaveData(setup, NULL, args->saveData, args->mapId, args->bagCursor, args->unk1C);
    setup->battleBg = BATTLE_BG_BATTLE_HALL;
    setup->terrain = TERRAIN_BATTLE_HALL;
    Party_InitWithMaxSize(setup->party[BATTLER_PLAYER], playerCount);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < playerCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(party, data->partySlots[i]), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_PLAYER);
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[index], HEAP_ID_FIELD2, 204));
    FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY, HEAP_ID_FIELD2);
    Party_InitWithMaxSize(setup->party[BATTLER_ENEMY], opponentCount);
    rank = HallTypeRanks_Get(data->cell, data->typeRanks[data->type]);
    if (data->type == 2) {
        rank = 9;
    }
    aiFlags = BattleHall_GetTrainerAIFlags(data, index, rank);
    for (i = 0; i < BATTLER_MAX; i++) {
        setup->trainer[i].data.aiFlags = aiFlags;
    }
    BattleHall_GenerateOpponentMons((FrontierMon *)((u8 *)data->opponentMons + index * sizeof(FrontierMon)), rank, data->trainerIds[index], &data->setIds[index], opponentCount, HEAP_ID_FIELD2, 206);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < opponentCount; i++) {
        while (BattleHall_RerollPersonality(data, index) != 0) {
        }
        FrontierMon_ToPokemon(&data->opponentMons[index], mon, BattleHall_GetOpponentLevel(data, rank));
        UpdateMonAbility(mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY);
    }
    Heap_Free(mon);
    switch (data->type) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(setup);
        PlayerProfile_Copy(CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId()), setup->profile[BATTLER_PLAYER2]);
        Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[index + 1], HEAP_ID_FIELD2, 204));
        FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY2, HEAP_ID_FIELD2);
        Party_InitWithMaxSize(setup->party[BATTLER_ENEMY2], opponentCount);
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        while (BattleHall_RerollPersonality(data, index) != 0) {
        }
        FrontierMon_ToPokemon((FrontierMon *)((u8 *)data + 0x290 + index * sizeof(FrontierMon)), mon, BattleHall_GetOpponentLevel(data, rank));
        UpdateMonAbility(mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY2);
        Heap_Free(mon);
        break;
    }
    return setup;
}
#pragma force_active reset

// Moves the personality on until the opponent Pokemon is not shiny (the callers loop while it returns TRUE).
u8 BattleHall_RerollPersonality(BattleHallData *data, u32 index) {
    u32 pid = data->opponentMons[index].pid;
    if (pid > 0x3D0A9) {
        data->opponentMons[index].pid = pid - 0x3D0A9;
    } else {
        data->opponentMons[index].pid = pid + 0x3D0A9;
    }
    return CalcShininessByOtIdAndPersonality(data->opponentMons[index].otId, data->opponentMons[index].pid);
}

u32 BattleHall_GetBattleType(u32 type) {
    switch (type) {
    case 0:
        return BATTLE_TYPE_TRAINER | BATTLE_TYPE_FRONTIER;
    case 1:
        return BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_FRONTIER;
    case 2:
        return BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER;
    case 3:
        return BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER;
    }
    return BATTLE_TYPE_TRAINER | BATTLE_TYPE_FRONTIER;
}

int BattleHall_GetPartySize(int type) {
    if (type == 1) {
        return 2;
    }
    return 1;
}

int BattleHall_GetOpponentMonCount(int type) {
    if (type == 1) {
        return 2;
    }
    return 1;
}

void BattleHall_GenerateOpponentMons(FrontierMon *mons, u32 rank, u32 trainerId, u16 *setIds, int count, enum HeapID heapID, u32 narcId) {
    int i;
    int ivs;

    if (trainerId == HALL_MATRON_SILVER) {
        ivs = 31;
    } else if (trainerId == HALL_MATRON_GOLD) {
        ivs = 31;
    } else {
        ivs = BattleHall_GetOpponentIVs(rank);
    }
    for (i = 0; i < count; i++) {
        FrontierMon_GenerateWithRandomOtId(&mons[i], setIds[i], i, ivs, 0, heapID, narcId);
    }
}

u16 BattleHall_GetOpponentLevel(BattleHallData *data, u32 unused) {
    if (IS_HALL_MATRON(data->trainerIds[data->battleNoBits * 2])) {
        return BattleHall_GetPartyLevel(data);
    }
    return data->opponentLevel;
}

u8 BattleHall_GetCellType(u32 cell) {
    return gBattleHallCellTypes[cell];
}

BOOL BattleHall_IsMultiType(int type) {
    switch (type) {
    case 2:
    case 3:
        return TRUE;
    }
    return FALSE;
}

u8 BattleHall_GetLevelRank(BattleHallData *data) {
    return (s32)GetMonData(Party_GetMonByIndex(SaveArray_Party_Get(data->saveData), data->partySlots[0]), MON_DATA_LEVEL, NULL) / 10;
}

u8 BattleHall_GetOpponentIVs(u32 rank) {
    return gBattleHallRankIVs[BattleHall_ClampRank(rank) * 4];
}

int BattleHall_GetTrainerAIFlags(BattleHallData *data, u32 index, int rank) {
    int aiFlags;

    rank++;
    if (rank >= 8) {
        aiFlags = 7;
    } else if (rank >= 4) {
        aiFlags = 1;
    } else {
        aiFlags = 0;
    }
    if (data->type == 0) {
        if (IS_HALL_MATRON(data->trainerIds[(index & 0x7F) * 2])) {
            aiFlags = 7;
        }
    }
    if (data->type == 2) {
        aiFlags = 7;
    }
    return aiFlags;
}

u32 BattleHall_ClampRank(u32 rank) {
    if (rank >= 10) {
        rank = 9;
    }
    return rank;
}

// The highest level of the party, and of the partner's Pokemon in multi battles.
u16 BattleHall_GetPartyLevel(BattleHallData *data) {
    Party *party;
    u16 level;
    u16 other;

    party = SaveArray_Party_Get(data->saveData);
    level = GetMonData(Party_GetMonByIndex(party, data->partySlots[0]), MON_DATA_LEVEL, NULL);
    if (BattleHall_GetPartySize(data->type) == 2) {
        other = GetMonData(Party_GetMonByIndex(party, data->partySlots[1]), MON_DATA_LEVEL, NULL);
        if (level <= other) {
            level = other;
        }
        return level;
    }
    if (BattleHall_IsMultiType(data->type) == TRUE) {
        other = data->partnerLevel;
        if (level <= other) {
            level = other;
        }
        return level;
    }
    return level;
}

u32 BattleHall_SqrtFx(u32 value) {
    float v;

    if (value != 0) {
        v = 0.5f + (float)(value << 12);
    } else {
        v = (float)(value << 12) - 0.5f;
    }
    return FX_Sqrt((fx32)v);
}
