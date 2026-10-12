#include "frontier/battle_hall_data.h"

#include "global.h"

#include "constants/pokemon.h"

#include "frontier/battle_hall_setup.h"
#include "frontier/frontier_link_messages.h"

#include "frontier_save_access.h"
#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sys_vars.h"

// The Battle Hall's script data: the challenge (type, streak, round, the cell picked on the board and its rank),
// saving and resuming it, the trainers and sets of the next battle and the opponents' level.
// The rank of each type cell (0-10) is kept as 4 bits: in the data (typeRanks, HallTypeRanks_Get) and in the frontier
// stats (BattleHall_GetSavedTypeRank).

// HallChallenge fields
#define HALL_CHALLENGE_TYPE        0
#define HALL_CHALLENGE_BATTLE_NO   1
#define HALL_CHALLENGE_TRAINER_IDS 2
#define HALL_CHALLENGE_PARTY_SLOTS 3
#define HALL_CHALLENGE_SET_IDS     4

#define HALL_FLAGS_HAS_STREAK 5
#define HALL_MAX_WIN_STREAK   9999
#define HALL_MAX_ROUND        18

// The trainers of a battle in BattleHallData.trainerIds.
typedef struct BattleHallTrainerPair {
    u16 trainerId;
    u16 partnerTrainerId;
} BattleHallTrainerPair;

extern const u8 gBattleHallBattlePoints[];
extern const u8 gBattleHallMultiBattlePoints[];

int FrontierSpeciesRecords_UpdateFromStat(void *saveData, int stat, int stat2, int substat, int stat3, int heapID, int *out1, int *out2);
void HallChallenge_Clear(void *challenge);
u32 HallChallenge_GetField(void *challenge, int field, u8 index, int unused, int unused2);
void HallChallenge_SetField(void *challenge, int field, u8 index, int unused, void *value);
void HallChallenge_SetSaved(void *challenge, int value);
u32 HallTypeRanks_Get(u8 cell, u8 *ranks);
void HallTypeRanks_Set(u8 cell, u8 *ranks, u8 rank);
void HallTypeRanks_Clear(u8 *ranks);
u32 BattleHall_GetTypeRankStat(u32 type, u32 cell);
u32 BattleHall_GetWinStreakStat(u8 type);
int BattleHall_GetBestStreakStat(int type);
int BattleHall_GetSpeciesStat(int type);
int BattleHall_GetSpeciesRecordsType(int type);
u32 FrontierStat_GetFriendIndex(u32 stat);
void *FrontierTrainer_Load(BattleHallTrainerRecord *record, u32 trainerId, enum HeapID heapID, u32 narcId);
void FrontierTrainer_GetClassSprite(u8 trainerClass);
void ov80_0222A840(void *saveData);
// Defined in battle_hall_setup.c with other types (returning u8 and int); declared as these callers use them.
int BattleHall_GetLevelRank(BattleHallData *data);
u8 BattleHall_GetPartySize(u8 type);
// Not in battle_hall_data.h: frontier_cmd_hall.c uses their (undefined) return values.
void BattleHallData_GetTrainerSprite(BattleHallData *data, u32 index);
void BattleHallData_SendLinkMessage(BattleHallData *data, u32 kind, u32 value);

#pragma force_active on
BattleHallData *BattleHallData_Alloc(SaveData *saveData, BOOL resume, u8 type, u8 slot0, u8 slot1) {
    BattleHallData *data;
    BattleHallData *data0;
    BattleHallData *data1;
    BattleHallData *data2;
    BattleHallData *data5;
    BattleHallData *data6;
    void *challenge;
    int flags;
    u8 partySize;
    u16 i;
    u8 hasStreak;
    FrontierSave *frontierSave;
    u32 stat;
    u16 low;
    u16 high;
    static BattleHallData *sBattleHallData;

    sBattleHallData = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleHallData));
    MI_CpuFill8(sBattleHallData, 0, sizeof(BattleHallData));
    data0 = sBattleHallData;
    data0->challenge = Save_Frontier_GetHallChallenge(saveData);
    data0->saveData = saveData;
    data0->heapId = HEAP_ID_FIELD2;
    data1 = sBattleHallData;
    data1->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    data1->partnerMon = AllocMonZeroed(HEAP_ID_FIELD2);
    challenge = data1->challenge;
    flags = Save_Frontier_GetHallFlags(saveData);
    if (resume == FALSE) {
        data2 = sBattleHallData;
        data2->type = type;
        partySize = BattleHall_GetPartySize(data2->type);
        sBattleHallData->battleNo = 0;
        HallChallenge_Clear(challenge);
        if (sBattleHallData->type == 3) {
            hasStreak = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(sBattleHallData->saveData));
        } else {
            hasStreak = HallFlags_GetField(flags, HALL_FLAGS_HAS_STREAK, sBattleHallData->type, 0, 0);
        }
        if (hasStreak == TRUE) {
            sBattleHallData->winStreak = FrontierSave_GetStat(Save_Frontier_GetStatic(sBattleHallData->saveData), BattleHall_GetWinStreakStat(sBattleHallData->type), FrontierStat_GetFriendIndex(BattleHall_GetWinStreakStat(sBattleHallData->type)));
        } else {
            sBattleHallData->winStreak = 0;
            for (i = 0; i < HALL_CELL_COUNT; i++) {
                BattleHall_SetSavedTypeRank(sBattleHallData->saveData, sBattleHallData->type, (u8)i, 0);
            }
        }
        data = sBattleHallData;
        data->partySlots[0] = slot0;
        data->partySlots[1] = slot1;
    } else {
        data = sBattleHallData;
        data->type = HallChallenge_GetField(challenge, HALL_CHALLENGE_TYPE, 0, 0, 0);
        partySize = BattleHall_GetPartySize(data->type);
        data5 = sBattleHallData;
        data5->battleNo = HallChallenge_GetField(challenge, HALL_CHALLENGE_BATTLE_NO, 0, 0, 0);
        frontierSave = Save_Frontier_GetStatic(sBattleHallData->saveData);
        stat = BattleHall_GetWinStreakStat(sBattleHallData->type);
        sBattleHallData->winStreak = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetWinStreakStat(data5->type)));
        for (i = 0; i < partySize; i++) {
            sBattleHallData->partySlots[i] = HallChallenge_GetField(challenge, HALL_CHALLENGE_PARTY_SLOTS, i, 0, 0);
        }
        for (i = 0; i < HALL_TRAINER_COUNT; i++) {
            sBattleHallData->trainerIds[i] = HallChallenge_GetField(challenge, HALL_CHALLENGE_TRAINER_IDS, i, 0, 0);
        }
        for (i = 0; i < HALL_TRAINER_COUNT; i++) {
            data6 = sBattleHallData;
            data6->setIds[i] = (u8)HallChallenge_GetField(challenge, HALL_CHALLENGE_SET_IDS, i, 0, 0);
        }
    }
    for (i = 0; i < partySize; i++) {
        Pokemon *mon;
        data = sBattleHallData;
        mon = Party_GetMonByIndex(SaveArray_Party_Get(data->saveData), data->partySlots[i]);
        data = sBattleHallData;
        data->heldItems[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    data = sBattleHallData;
    data->unk_010 = 0;
    data->round = data->winStreak / 10;
    if (data->type == 2) {
        for (i = 0; i < HALL_CELL_COUNT; i++) {
            HallTypeRanks_Set(i, sBattleHallData->typeRanks[2], 9);
        }
    } else {
        for (i = 0; i < HALL_CELL_COUNT; i++) {
            u16 rank = BattleHall_GetSavedTypeRank(saveData, sBattleHallData->type, (u8)i, &low, &high);
            HallTypeRanks_Set(i, sBattleHallData->typeRanks[sBattleHallData->type], rank);
        }
    }
    if (BattleHall_IsMultiType(sBattleHallData->type) == TRUE) {
        ov80_0222A840(sBattleHallData->saveData);
    }
    return sBattleHallData;
}
#pragma force_active reset

void BattleHallData_Init(BattleHallData *data, u32 unused) {
    BattleHallData_GenerateOpponents(data);
}

#pragma force_active on
// Picks the trainers and the sets of the next battle; the Hall Matron's sets depend on the player's level instead of
// the cell's rank.
void BattleHallData_GenerateOpponents(BattleHallData *data) {
    int matron = 0;
    int count = 1;
    int rank;
    BattleHallTrainerPair *battles;
    int battleNo;

    if (data->type) {
        count = 2;
    }
    rank = HallTypeRanks_Get(data->cell, data->typeRanks[data->type]);
    data->opponentLevel = BattleHallData_CalcOpponentLevel(data);
    BattleHall_PickTrainerClasses(data->cell, count, rank, data->battleNo, data->trainerIds);
    BattleHall_PickTrainerIds(data->type, count, data->round, rank, data->battleNo, data->trainerIds);
    battles = (BattleHallTrainerPair *)data->trainerIds;
    battleNo = data->battleNo & 0x7F;
    if ((u16)(battles[battleNo].trainerId + 0xFECD) <= 1) {
        rank = (u8)BattleHall_GetLevelRank(data);
    }
    if (battles[battleNo].trainerId == 307) {
        matron = 1;
    }
    if (battles[battleNo].trainerId == 308) {
        matron = 2;
    }
    BattleHall_PickSetIds(1, data->cellType, rank, data->battleNo, (u16)GetMonData(Party_GetMonByIndex(data->party, 0), MON_DATA_SPECIES, NULL), data->setIds, matron);
}
#pragma force_active reset

void BattleHallData_Free(BattleHallData *data) {
    if (data != NULL) {
        if (data->party != NULL) {
            Heap_Free(data->party);
        }
        if (data->partnerMon != NULL) {
            Heap_Free(data->partnerMon);
        }
        MI_CpuFill8(data, 0, sizeof(BattleHallData));
        Heap_Free(data);
    }
}

void BattleHallData_ReadAppResult(BattleHallData *data, BattleHallAppArgs *args) {
    data->cursor = BattleHallAppArgs_GetResult(args, 0);
    data->cell = data->cursor;
    data->cellType = BattleHall_GetCellType((u8)data->cursor);
    if (data->cell >= HALL_CELL_COUNT - 1) {
        data->cell = HALL_CELL_COUNT - 1;
    }
}

u16 BattleHallAppArgs_GetResult(BattleHallAppArgs *args, u32 unused) {
    return args->result;
}

void BattleHallData_UpdateSpeciesRecord(BattleHallData *data, u16 *result) {
    int type = data->type;
    int unused1;
    int unused2;
    int stat1;
    int stat2;
    int substat;

    if (type != 3) {
        stat1 = BattleHall_GetBestStreakStat(type);
        stat2 = BattleHall_GetSpeciesStat(data->type);
        substat = FrontierStat_GetFriendIndex(BattleHall_GetBestStreakStat(data->type));
        *result = FrontierSpeciesRecords_UpdateFromStat(data->saveData, stat1, stat2, substat, BattleHall_GetSpeciesRecordsType(type), HEAP_ID_FIELD2, &unused1, &unused2);
    } else {
        *result = 0;
    }
}

#pragma force_active on
void BattleHallData_Save(BattleHallData *data, int mode) {
    struct {
        u16 value;
        u8 filler_02[6];
        u8 byte;
        u8 filler_09[3];
    } temp;
    FrontierSave *frontierSave;
    Pokemon *mon;
    int flags;
    u16 species, savedSpecies, i;
    u8 type;
    int stat;

    flags = Save_Frontier_GetHallFlags(data->saveData);
    frontierSave = Save_Frontier_GetStatic(data->saveData);
    temp.byte = data->type;
    HallChallenge_SetField(data->challenge, HALL_CHALLENGE_TYPE, 0, 0, &temp.byte);
    HallChallenge_SetSaved(data->challenge, 1);
    mon = Party_GetMonByIndex(SaveArray_Party_Get(data->saveData), data->partySlots[0]);
    species = (u16)GetMonData(mon, MON_DATA_SPECIES, NULL);
    stat = BattleHall_GetSpeciesStat(data->type);
    savedSpecies = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetSpeciesStat(data->type)));
    temp.byte = data->battleNo;
    HallChallenge_SetField(data->challenge, HALL_CHALLENGE_BATTLE_NO, 0, 0, &temp.byte);
    stat = BattleHall_GetWinStreakStat(data->type);
    FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetWinStreakStat(data->type)), data->winStreak);
    if (mode != 2) {
        type = data->type;
        if (type != 3) {
            stat = BattleHall_GetBestStreakStat(type);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetBestStreakStat(type)), data->winStreak);
        } else if (savedSpecies != species) {
            // the link streak record only grows with the same Pokemon
            stat = BattleHall_GetBestStreakStat(type);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetBestStreakStat(type)), data->winStreak);
        } else {
            stat = BattleHall_GetBestStreakStat(type);
            FrontierSave_RaiseStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetBestStreakStat(type)), data->winStreak);
        }
        temp.byte = data->hasStreak;
        HallFlags_SetField(flags, HALL_FLAGS_HAS_STREAK, data->type, 0, &temp.byte);
        if (data->type == 3) {
            FrontierSave_SetStat(frontierSave, 106, FrontierStat_GetFriendIndex(106), data->hasStreak);
        }
    }
    for (i = 0; i < HALL_TRAINER_COUNT; i++) {
        temp.value = data->trainerIds[i];
        HallChallenge_SetField(data->challenge, HALL_CHALLENGE_TRAINER_IDS, (u8)i, 0, &temp.value);
    }
    for (i = 0; i < 2; i++) {
        temp.byte = data->partySlots[i];
        HallChallenge_SetField(data->challenge, HALL_CHALLENGE_PARTY_SLOTS, (u8)i, 0, &temp.byte);
    }
    for (i = 0; i < HALL_CELL_COUNT; i++) {
        temp.byte = HallTypeRanks_Get((u8)i, data->typeRanks[data->type]);
        BattleHall_SetSavedTypeRank(data->saveData, data->type, (u8)i, temp.byte);
    }
    for (i = 0; i < HALL_TRAINER_COUNT; i++) {
        temp.value = data->setIds[i];
        HallChallenge_SetField(data->challenge, HALL_CHALLENGE_SET_IDS, (u8)i, 0, &temp.value);
    }
    stat = BattleHall_GetSpeciesStat(data->type);
    {
        int substat = FrontierStat_GetFriendIndex(BattleHall_GetSpeciesStat(data->type));
        FrontierSave_SetStat(frontierSave, stat, substat, (u16)GetMonData(mon, MON_DATA_SPECIES, NULL));
    }
}
#pragma force_active reset

u8 BattleHallData_NextBattle(BattleHallData *data) {
    data->battleNo++;
    return data->battleNo;
}

u8 BattleHallData_GetBattleNo(BattleHallData *data) {
    return data->battleNo;
}

void BattleHallData_GetTrainerSprite(BattleHallData *data, u32 index) {
    BattleHallTrainerRecord trainer;
    Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[(u8)(index + data->battleNo * 2)], HEAP_ID_FIELD2, 204));
    FrontierTrainer_GetClassSprite((u8)trainer.trainerClass);
}

void BattleHallData_ClearRanks(BattleHallData *data) {
    HallTypeRanks_Clear(data->typeRanks[data->type]);
    BattleHallData_Save(data, 1);
}

void BattleHallData_EndRound(BattleHallData *data) {
    data->hasStreak = TRUE;
    if (data->round < HALL_MAX_ROUND) {
        data->round++;
    }
    data->battleNo = 0;
    BattleHallData_Save(data, 0);
}

void BattleHallData_SendLinkMessage(BattleHallData *data, u32 kind, u32 value) {
    switch (kind) {
    case 0:
        ov80_0222AF10(data);
        break;
    case 1:
        BattleHallLink_SendTrainerIds(data);
        break;
    case 2:
        BattleHallLink_SendSetIds(data);
        break;
    case 3:
        ov80_0222B024(data, (u8)value);
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        BattleHallLink_SendPartyMon(data);
        break;
    }
}

u8 BattleHallData_GetWonBattlePoints(BattleHallData *data) {
    u8 type = data->type;
    u16 round = data->round;
    u8 ret;

    if (type <= 1) {
        if (round >= HALL_MAX_ROUND) {
            ret = 12;
        } else {
            ret = gBattleHallBattlePoints[round];
        }
    } else {
        if (round >= HALL_MAX_ROUND) {
            ret = 23;
        } else {
            ret = gBattleHallMultiBattlePoints[round];
        }
    }
    if (type == 0) {
        if (data->winStreak == 50 || data->winStreak == 170) {
            ret = 20;
        }
    } else if (type == 2) {
        ret = 12;
    }
    return ret;
}

static inline u16 BattleHall_ToU16(u32 value) {
    return value;
}

#pragma force_active on
// The ranks of two cells share a byte of the frontier stats: low nibble for the even cell, high nibble for the odd.
u16 BattleHall_GetSavedTypeRank(SaveData *saveData, int type, int cell, u16 *low, u16 *high) {
    FrontierSave *frontierSave = Save_Frontier_GetStatic(saveData);
    int stat = BattleHall_GetTypeRankStat(type, cell);
    u16 value = BattleHall_ToU16((u8)FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetTypeRankStat(type, cell))));
    *low = value & 0xF;
    *high = value >> 4;
    if (cell % 2 == 0) {
        return *low;
    }
    return *high;
}
#pragma force_active reset

void BattleHall_SetSavedTypeRank(SaveData *saveData, u8 type, s32 cell, u8 rank) {
    FrontierSave *frontierSave;
    u32 stat;
    u16 low;
    u16 high;
    u8 packed;
    u8 shift;

    BattleHall_GetSavedTypeRank(saveData, type, cell, &low, &high);
    packed = (u8)((high << 4) | low);
    shift = (u8)(cell % 2);
    if (shift == 0) {
        packed &= 0xF0;
    } else {
        packed &= 0xF;
    }
    packed |= (u8)(rank << (shift * 4));
    frontierSave = Save_Frontier_GetStatic(saveData);
    stat = BattleHall_GetTypeRankStat(type, cell);
    FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleHall_GetTypeRankStat(type, cell)), packed);
}

void BattleHallData_ResetMaxedRanks(BattleHallData *data) {
    s32 i;

    if (data->type == 2) {
        return;
    }
    for (i = 0; i < HALL_CELL_COUNT - 1; i++) {
        if (HallTypeRanks_Get(i, data->typeRanks[data->type]) < 10) {
            break;
        }
    }
    if (i == HALL_CELL_COUNT - 1) {
        for (i = 0; i < HALL_CELL_COUNT - 1; i++) {
            HallTypeRanks_Set(i, data->typeRanks[data->type], 9);
        }
    }
}

void BattleHallData_UpdateLevelSqrt(BattleHallData *data) {
    data->levelSqrt = BattleHall_SqrtFx(BattleHall_GetPartyLevel(data));
}

static inline int BattleHall_IntToFx32(int value) {
    float result;
    if (value > 0) {
        result = 0.5f + (float)(int)((u32)value << 12);
    } else {
        result = (float)(int)((u32)value << 12) - 0.5f;
    }
    return (int)result;
}

#pragma force_active on
// The opponents' level: grows with the cell's rank relative to the party level, plus half a level per other type
// with a rank; at most the party level and 100. In link multi battles it is the party level.
u16 BattleHallData_CalcOpponentLevel(BattleHallData *data) {
    int rank = HallTypeRanks_Get(data->cell, data->typeRanks[data->type]);
    int remaining;
    int partyLevel = BattleHall_GetPartyLevel(data);
    float divisor, score;
    int i;
    float total;
    int result;

    if (data->type == 2) {
        return (u16)partyLevel;
    }
    {
        float fixed;
        int levelSqrt;
        if (partyLevel > 0) {
            fixed = 0.5f + (float)(int)((u32)partyLevel << 12);
        } else {
            fixed = (float)(int)((u32)partyLevel << 12) - 0.5f;
        }
        levelSqrt = data->levelSqrt;
        remaining = (int)fixed - levelSqrt * 3;
        divisor = (float)((double)((float)levelSqrt / 4096.0f) * 5.0);
    }
    if ((double)((float)partyLevel / divisor) < 1.0) {
        score = (float)rank;
    } else {
        score = ((float)BattleHall_IntToFx32(rank * partyLevel) / 4096.0f) / divisor;
    }
    total = 0.0f;
    for (i = 0; i < HALL_CELL_COUNT; i++) {
        if (i == data->cell) {
            total += 1.0;
        } else if (HallTypeRanks_Get((u8)i, data->typeRanks[data->type]) != 0) {
            total += 1.0;
        }
    }
    if ((double)total != 0.0) {
        total -= 1.0;
    }
    total *= 0.5;
    total = total + ((float)remaining / 4096.0f + score);
    result = (int)total;
    if (total != (float)(int)total) {
        result++;
    }
    if (result > partyLevel) {
        result = partyLevel;
    }
    if (result > 100) {
        result = 100;
    }
    return (u16)result;
}
#pragma force_active reset
