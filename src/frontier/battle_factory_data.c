#include "frontier/battle_factory_data.h"

#include "global.h"

#include "frontier/frontier_link_messages.h"

#include "error_handling.h"
#include "game_stats.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
#include "frontier_save_access.h"
#include "frontier_stat_ids_legacy.h"

// The Battle Factory's script data: rentals and opponents generated from Battle Factory sets, the player's rental
// party, trades and the record kept in the FactoryChallenge save.

// A trainer as FrontierTrainer_Load reads it.
typedef struct BattleFactoryTrainer {
    u32 id;
    u16 trainerClass;
    u8 filler_06[0x2A];
} BattleFactoryTrainer;

extern BattleFactoryData *gBattleFactoryData;
extern const u8 gBattleFactoryBattlePoints[];
extern const u8 gBattleFactoryMultiBattlePoints[];

void *Save_Frontier_GetFactoryChallenge(SaveData *saveData);
void FactoryChallenge_Clear(void *challenge);
u32 FactoryChallenge_GetField(void *challenge, int field, u8 index, int a3);
void FactoryChallenge_SetField(void *challenge, int field, u8 index, void *value);
void FactoryChallenge_SetSaved(void *challenge, int saved);
int FactoryFlags_GetField(int flags, int field, u8 index, int a3);
u32 BattleFactory_GetBestStreakStat(u8 type, u8 levelMode);
u32 BattleFactory_GetBestTradeCountStat(u8 type, u8 levelMode);
u32 FrontierStat_GetFriendIndex(u32 stat);
void Frontier_RecordLinkPlayerLocations(SaveData *saveData);
void *FrontierTrainer_Load(BattleFactoryTrainer *trainer, int trainerId, enum HeapID heapId, int narcId);
int FrontierTrainer_GetClassSprite(int trainerClass);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
void Frontier_AddRentalMonToParty(SaveData *saveData, Party *party, Pokemon *mon);
int BattleFactory_GetPartySize(int type);
// opponent Pokemon count of a type (defined returning int; the count is read as a u8 here)
u8 BattleFactory_GetOpponentMonCount(int type, int both);
void BattleFactory_GetRoundTrainerIds(u8 type, u16 round, u16 *trainerIds, int count);
void BattleFactory_GenerateRentalSets(u16 round, u8 levelMode, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *personalities, u16 tradeCount, u16 *excludedSetIds, u16 *excludedItems);
void BattleFactory_GenerateTrainerSets(int count, u16 trainerId, u8 levelMode, FrontierMon *candidates, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *personalities, int candidateCount);
const void *BattleFactory_GetTrainerSetRange(u16 trainerId, u8 levelMode);
int BattleFactory_PickSetIds(const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapId, const void *range, u16 tradeCount, u8 *ivs);
int BattleFactory_GetMonLevel(BattleFactoryData *data);
BOOL BattleFactory_IsMultiType(int type);
u16 BattleFactory_GetRound(BattleFactoryData *data);

#pragma force_active on

BattleFactoryData *BattleFactoryData_Alloc(SaveData *saveData, int resume, u8 type, u8 levelMode) {
    BattleFactoryData *data;
    void *challenge;
    int flags;

    gBattleFactoryData = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactoryData));
    MI_CpuFill8(gBattleFactoryData, 0, sizeof(BattleFactoryData));
    data = gBattleFactoryData;
    data->challenge = Save_Frontier_GetFactoryChallenge(saveData);
    data->saveData = saveData;
    data->heapId = HEAP_ID_FIELD2;
    data = gBattleFactoryData;
    data->playerParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    data->opponentParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    challenge = data->challenge;
    flags = Save_Frontier_GetFactoryFlags(saveData);
    if (!resume) {
        u8 hasStreak;
        BattleFactoryData *first;
        BattleFactoryData *second;
        FrontierSave *frontier;
        int stat;
        int tradeCount;
        {
            BattleFactoryData *initial = gBattleFactoryData;
            initial->type = type;
            initial->levelMode = levelMode;
            initial->battleNo = 0;
        }
        FactoryChallenge_Clear(challenge);
        if (gBattleFactoryData->type == 3) {
            hasStreak = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(gBattleFactoryData->saveData));
        } else {
            hasStreak = FactoryFlags_GetField(flags, 10, (u8)(gBattleFactoryData->levelMode * 4 + gBattleFactoryData->type), 0);
        }
        if (hasStreak == 1) {
            first = gBattleFactoryData;
            frontier = Save_Frontier_GetStatic(first->saveData);
            stat = BattleFactory_GetWinStreakStat(first->levelMode, first->type);
            first->winStreak = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(BattleFactory_GetWinStreakStat(first->levelMode, first->type)));
            second = gBattleFactoryData;
            frontier = Save_Frontier_GetStatic(second->saveData);
            stat = BattleFactory_GetTradeCountStat(second->levelMode, second->type);
            tradeCount = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(BattleFactory_GetTradeCountStat(first->levelMode, first->type)));
        } else {
            second = gBattleFactoryData;
            second->winStreak = 0;
            tradeCount = 0;
        }
        second->tradeCount = tradeCount;
        second->unk_010 = 0;
    } else {
        BattleFactoryData *first;
        BattleFactoryData *second;
        BattleFactoryData *initial;
        FrontierSave *frontier;
        int stat;
        u8 savedType = FactoryChallenge_GetField(challenge, 1, 0, 0);
        initial = gBattleFactoryData;
        initial->type = savedType;
        initial->levelMode = FactoryChallenge_GetField(challenge, 0, 0, 0);
        initial->battleNo = FactoryChallenge_GetField(challenge, 2, 0, 0);
        first = gBattleFactoryData;
        frontier = Save_Frontier_GetStatic(first->saveData);
        stat = BattleFactory_GetWinStreakStat(first->levelMode, first->type);
        first->winStreak = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(BattleFactory_GetWinStreakStat(initial->levelMode, initial->type)));
        second = gBattleFactoryData;
        frontier = Save_Frontier_GetStatic(second->saveData);
        stat = BattleFactory_GetTradeCountStat(second->levelMode, second->type);
        second->tradeCount = FrontierSave_GetStat(frontier, stat, FrontierStat_GetFriendIndex(BattleFactory_GetTradeCountStat(first->levelMode, first->type)));
    }
    data = gBattleFactoryData;
    data->round = data->winStreak / 7;
    if (BattleFactory_IsMultiType(data->type) == TRUE) {
        Frontier_RecordLinkPlayerLocations(gBattleFactoryData->saveData);
    }
    return gBattleFactoryData;
}

#pragma force_active reset

void BattleFactoryData_Init(BattleFactoryData *data, int resume) {
    if (resume == 0) {
        BattleFactoryData_InitNew(data);
    } else {
        BattleFactoryData_InitResume(data);
    }
}

#pragma force_active on

void BattleFactoryData_InitNew(BattleFactoryData *data) {
    FrontierMon candidates[12];
    u16 setIds[FACTORY_RENTAL_COUNT];
    u16 items[FACTORY_RENTAL_COUNT];
    int count;
    int i;

    BattleFactory_GetRoundTrainerIds(data->type, BattleFactory_GetRound(data), data->trainerIds, 14);
    BattleFactory_GenerateRentalSets(BattleFactory_GetRound(data), data->levelMode, data->rentals.setIds, data->rentals.mons, data->rentals.ivs, data->rentals.personalities, data->tradeCount, NULL, NULL);
    BattleFactory_SwapRentalWithRandom(data, 4, 0);
    BattleFactory_SwapRentalWithRandom(data, 5, 0);
    count = FACTORY_RENTAL_COUNT;
    {
        u8 *p;
        i = 0;
        p = (u8 *)data;
        for (; i < FACTORY_RENTAL_COUNT; i++) {
            candidates[i] = ((BattleFactoryData *)p)->rentals.mons[0];
            p += sizeof(FrontierMon);
        }
    }
    // Link multi battles: the partner's rentals are generated here too, without the player's species and items.
    if (BattleFactory_IsMultiType(data->type) == TRUE) {
        struct {
            FrontierMon *mon;
            u16 *setId;
            BattleFactoryData *item;
            u16 *excludedItem;
        } v;
        i = 0;
        v.mon = data->rentals.mons;
        v.setId = setIds;
        v.item = data;
        v.excludedItem = items;
        for (; i < FACTORY_RENTAL_COUNT; i++) {
            *v.setId++ = v.mon->species;
            *v.excludedItem++ = v.item->rentals.mons[0].item;
            v.mon++;
            v.item = (BattleFactoryData *)((u8 *)v.item + sizeof(FrontierMon));
        }
        BattleFactory_GenerateRentalSets(BattleFactory_GetRound(data), data->levelMode, data->partnerRentals.setIds, data->partnerRentals.mons, data->partnerRentals.ivs, data->partnerRentals.personalities, data->partnerTradeCount, setIds, items);
        BattleFactory_SwapRentalWithRandom(data, 4, 1);
        BattleFactory_SwapRentalWithRandom(data, 5, 1);
        count = 12;
        {
            u8 *p;
            i = 0;
            p = (u8 *)data;
            for (; i < FACTORY_RENTAL_COUNT; i++) {
                candidates[i + FACTORY_RENTAL_COUNT] = ((BattleFactoryData *)p)->partnerRentals.mons[0];
                p += sizeof(FrontierMon);
            }
        }
    }
    BattleFactory_GenerateTrainerSets(BattleFactory_GetOpponentMonCount(data->type, 1), data->trainerIds[data->battleNo], data->levelMode, candidates, data->opponentSetIds, data->opponentMons, data->opponentIVs, data->opponentPersonalities, count);
    for (i = 0; i < FACTORY_RENTAL_COUNT; i++) {
        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
        FrontierMon_ToPokemon(&data->rentals.mons[i], mon, BattleFactory_GetMonLevel(data));
        Frontier_AddRentalMonToParty(data->saveData, data->playerParty, mon);
        Heap_Free(mon);
    }
    for (i = 0; i < FACTORY_RENTAL_COUNT; i++) {
        Party_GetMonByIndex(data->playerParty, i);
    }
}

// side 0: the player's rentals, 1: the partner's.
void BattleFactory_SwapRentalWithRandom(BattleFactoryData *data, int index, int partner) {
    u16 random = LCRandom() % FACTORY_RENTAL_COUNT;
    if (partner == 0) {
        FrontierMon mon;
        u16 setId = data->rentals.setIds[index];
        u8 iv = data->rentals.ivs[index];
        u32 personality = data->rentals.personalities[index];
        FrontierMon *dest;
        dest = &data->rentals.mons[index];
        mon = *dest;
        data->rentals.setIds[index] = data->rentals.setIds[random];
        data->rentals.ivs[index] = data->rentals.ivs[random];
        data->rentals.personalities[index] = data->rentals.personalities[random];
        *dest = data->rentals.mons[random];
        data->rentals.setIds[random] = setId;
        data->rentals.ivs[random] = iv;
        data->rentals.personalities[random] = personality;
        data->rentals.mons[random] = mon;
    } else {
        FrontierMon *dest;
        FrontierMon mon;
        u16 setId = data->partnerRentals.setIds[index];
        u8 iv = data->partnerRentals.ivs[index];
        u32 personality = data->partnerRentals.personalities[index];
        dest = &data->partnerRentals.mons[index];
        mon = *dest;
        data->partnerRentals.setIds[index] = data->partnerRentals.setIds[random];
        data->partnerRentals.ivs[index] = data->partnerRentals.ivs[random];
        data->partnerRentals.personalities[index] = data->partnerRentals.personalities[random];
        *dest = data->partnerRentals.mons[random];
        data->partnerRentals.setIds[random] = setId;
        data->partnerRentals.ivs[random] = iv;
        data->partnerRentals.personalities[random] = personality;
        data->partnerRentals.mons[random] = mon;
    }
}

#pragma force_active reset

void BattleFactoryData_InitResume(BattleFactoryData *data) {
    u16 setIds[FACTORY_RENTAL_COUNT];
    u8 ivs[FACTORY_RENTAL_COUNT];
    u32 personalities[FACTORY_RENTAL_COUNT];
    FrontierMon mons[FACTORY_RENTAL_COUNT];
    Pokemon *mon;
    u16 *trainerId;
    int i;

    BattleFactory_GetPartySize(data->type);
    i = 0;
    trainerId = (u16 *)data;
    do {
        trainerId[12] = FactoryChallenge_GetField(data->challenge, 3, (u8)i, 0);
        i++;
        trainerId++;
    } while (i < 14);

    for (i = 0; i < FACTORY_OPPONENT_COUNT; i++) {
        setIds[i] = FactoryChallenge_GetField(data->challenge, 4, (u8)i, 0);
        personalities[i] = FactoryChallenge_GetField(data->challenge, 6, (u8)i, 0);
        ivs[i] = FactoryChallenge_GetField(data->challenge, 5, (u8)i, 0);
        data->partySetIds[i] = setIds[i];
    }
    FrontierMon_GenerateList(mons, setIds, ivs, personalities, NULL, 4, HEAP_ID_FIELD2, NARC_a_2_0_3);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < FACTORY_OPPONENT_COUNT; i++) {
        FrontierMon_ToPokemon(&mons[i], mon, BattleFactory_GetMonLevel(data));
        Frontier_AddRentalMonToParty(data->saveData, data->playerParty, mon);
    }
    Heap_Free(mon);

    for (i = 0; i < FACTORY_OPPONENT_COUNT; i++) {
        setIds[i] = FactoryChallenge_GetField(data->challenge, 7, (u8)i, 0);
        personalities[i] = FactoryChallenge_GetField(data->challenge, 9, (u8)i, 0);
        ivs[i] = FactoryChallenge_GetField(data->challenge, 8, (u8)i, 0);
        data->opponentSetIds[i] = setIds[i];
    }
    FrontierMon_GenerateList(mons, setIds, ivs, personalities, NULL, 4, HEAP_ID_FIELD2, NARC_a_2_0_3);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < FACTORY_OPPONENT_COUNT; i++) {
        FrontierMon_ToPokemon(&mons[i], mon, BattleFactory_GetMonLevel(data));
        Frontier_AddRentalMonToParty(data->saveData, data->opponentParty, mon);
    }
    Heap_Free(mon);
}

void BattleFactoryData_Free(BattleFactoryData *data) {
    if (data != NULL) {
        if (data->playerParty != NULL) {
            Heap_Free(data->playerParty);
        }
        if (data->opponentParty != NULL) {
            Heap_Free(data->opponentParty);
        }
        MI_CpuFill8(data, 0, sizeof(BattleFactoryData));
        Heap_Free(data);
    }
}

void BattleFactory_StoreAppResult(BattleFactoryData *data, BattleFactoryAppLaunchArgs *args) {
    u8 *p = (u8 *)data;
    int i;
    for (i = 0; i < 6; i++) {
        ((BattleFactoryData *)p)->selection[0] = BattleFactory_GetAppResult(args, (u8)i);
        p += 2;
    }
}

int BattleFactory_GetAppResult(BattleFactoryAppLaunchArgs *args, u32 index) {
    if (index >= 6) {
        GF_AssertFail();
        return 0;
    }
    return args->result[index];
}

#pragma force_active on

// mode 2 only saves the challenge; otherwise the records and the streak flag are updated as well.
void BattleFactoryData_SaveRecord(BattleFactoryData *data, int mode) {
    struct {
        u16 half;
        u8 filler_02[6];
        u8 byte;
        u8 filler_09[3];
        u32 word;
        u8 filler_10[12];
    } value;
    int flags = Save_Frontier_GetFactoryFlags(data->saveData);
    FrontierSave *frontier = Save_Frontier_GetStatic(data->saveData);
    u16 i;
    u32 count;

    BattleFactory_GetPartySize(data->type);
    BattleFactory_GetOpponentMonCount(data->type, 1);

    value.byte = data->levelMode;
    FactoryChallenge_SetField(data->challenge, 0, 0, &value.byte);
    value.byte = data->type;
    FactoryChallenge_SetField(data->challenge, 1, 0, &value.byte);
    FactoryChallenge_SetSaved(data->challenge, 1);
    value.byte = data->battleNo;
    FactoryChallenge_SetField(data->challenge, 2, 0, &value.byte);
    FrontierSave_SetStat(frontier, BattleFactory_GetTradeCountStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetTradeCountStat(data->levelMode, data->type)), data->tradeCount);
    FrontierSave_SetStat(frontier, BattleFactory_GetWinStreakStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetWinStreakStat(data->levelMode, data->type)), data->winStreak);
    if (mode != 2) {
        u32 old = FrontierSave_GetStat(frontier, BattleFactory_GetBestStreakStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetBestStreakStat(data->levelMode, data->type)));
        u32 new;
        FrontierSave_RaiseStat(frontier, BattleFactory_GetBestStreakStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetBestStreakStat(data->levelMode, data->type)), data->winStreak);
        new = FrontierSave_GetStat(frontier, BattleFactory_GetBestStreakStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetBestStreakStat(data->levelMode, data->type)));
        if (data->winStreak == old) {
            FrontierSave_RaiseStat(frontier, BattleFactory_GetBestTradeCountStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetBestTradeCountStat(data->levelMode, data->type)), data->tradeCount);
        } else if (old < new) {
            FrontierSave_SetStat(frontier, BattleFactory_GetBestTradeCountStat(data->levelMode, data->type), FrontierStat_GetFriendIndex(BattleFactory_GetBestTradeCountStat(data->levelMode, data->type)), data->tradeCount);
        }
        value.byte = data->hasStreak;
        FactoryFlags_SetField(flags, 10, (u8)(data->type + data->levelMode * 4), &value.byte);
        if (data->type == 3) {
            int stat;
            if (data->levelMode == 0) {
                stat = 102;
            } else {
                stat = 104;
            }
            FrontierSave_SetStat(frontier, stat, FrontierStat_GetFriendIndex(stat), data->hasStreak);
        }
    }
    for (i = 0; i < 14; i++) {
        value.half = data->trainerIds[i];
        FactoryChallenge_SetField(data->challenge, 3, (u8)i, &value.half);
    }
    count = Party_GetCount(data->playerParty);
    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->playerParty, i);
        value.half = data->partySetIds[i];
        FactoryChallenge_SetField(data->challenge, 4, (u8)i, &value.half);
        value.byte = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        FactoryChallenge_SetField(data->challenge, 5, (u8)i, &value.byte);
        value.word = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        FactoryChallenge_SetField(data->challenge, 6, (u8)i, &value.word);
    }
    count = Party_GetCount(data->opponentParty);
    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->opponentParty, i);
        value.half = data->opponentSetIds[i];
        FactoryChallenge_SetField(data->challenge, 7, (u8)i, &value.half);
        value.byte = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        FactoryChallenge_SetField(data->challenge, 8, (u8)i, &value.byte);
        value.word = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        FactoryChallenge_SetField(data->challenge, 9, (u8)i, &value.word);
    }
}

#pragma force_active reset

u8 BattleFactory_IncBattleNo(BattleFactoryData *data) {
    data->battleNo++;
    return data->battleNo;
}

u8 BattleFactory_GetBattleNo(BattleFactoryData *data) {
    return data->battleNo;
}

int BattleFactory_GetTrainerSprite(BattleFactoryData *data, int partner) {
    BattleFactoryTrainer trainer;
    void *raw;
    int index = (u8)(data->battleNo + partner * 7);
    raw = FrontierTrainer_Load(&trainer, data->trainerIds[index], HEAP_ID_FIELD2, NARC_a_2_0_2);
    Heap_Free(raw);
    return FrontierTrainer_GetClassSprite((u8)trainer.trainerClass);
}

void BattleFactoryData_SaveRecordMode1(BattleFactoryData *data) {
    BattleFactoryData_SaveRecord(data, 1);
}

void BattleFactory_CompleteRound(BattleFactoryData *data) {
    data->hasStreak = TRUE;
    if (data->round < 8) {
        data->round++;
    }
    data->battleNo = 0;
    BattleFactoryData_SaveRecord(data, 0);
}

#pragma force_active on

void BattleFactory_SetRentalParty(BattleFactoryData *data) {
    FrontierMon *rentals;
    int count = BattleFactory_GetPartySize(data->type);
    int opponentCount = BattleFactory_GetOpponentMonCount(data->type, 1);
    Pokemon *mon;
    int i;
    u8 *p;

    SaveArray_Party_Init(data->playerParty);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    i = 0;
    if (count > 0) {
        p = (u8 *)data;
        rentals = data->rentals.mons;
        do {
            FrontierMon_ToPokemon(&rentals[((BattleFactoryData *)p)->selection[0]], mon, BattleFactory_GetMonLevel(data));
            Frontier_AddRentalMonToParty(data->saveData, data->playerParty, mon);
            ((BattleFactoryData *)p)->partySetIds[0] = data->rentals.setIds[((BattleFactoryData *)p)->selection[0]];
            i++;
            p += 2;
        } while (i < count);
    }
    i = 0;
    if (opponentCount > 0) {
        FrontierMon *opponent = data->opponentMons;
        do {
            FrontierMon_ToPokemon(opponent, mon, BattleFactory_GetMonLevel(data));
            Frontier_AddRentalMonToParty(data->saveData, data->opponentParty, mon);
            i++;
            opponent++;
        } while (i < opponentCount);
    }
    Heap_Free(mon);
}

void BattleFactory_GenerateOpponentMons(BattleFactoryData *data) {
    u16 species[8];
    u16 items[8];
    int i;
    int playerCount;
    u8 num;
    Pokemon *mon;
    u32 opponentCount;

    for (i = 0; i < 8; i++) {
        species[i] = 0;
        items[i] = 0;
    }
    num = BattleFactory_GetOpponentMonCount(data->type, 1);
    playerCount = Party_GetCount(data->playerParty);
    for (i = 0; i < playerCount; i++) {
        mon = Party_GetMonByIndex(data->playerParty, i);
        species[i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
        items[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    opponentCount = Party_GetCount(data->opponentParty);
    for (i = 0; i < (int)opponentCount; i++) {
        Pokemon *opponentMon = Party_GetMonByIndex(data->opponentParty, i);
        species[playerCount + i] = GetMonData(opponentMon, MON_DATA_SPECIES, NULL);
        items[playerCount + i] = GetMonData(opponentMon, MON_DATA_HELD_ITEM, NULL);
        data->rentals.setIds[i] = data->opponentSetIds[i];
    }
    BattleFactory_PickSetIds(species, items, playerCount + opponentCount, num, data->opponentSetIds, HEAP_ID_FIELD2, BattleFactory_GetTrainerSetRange(data->trainerIds[data->battleNo], data->levelMode), 0, data->opponentIVs);
    // no personalities given: new ones are generated and returned through the last array
    FrontierMon_GenerateList(data->opponentMons, data->opponentSetIds, data->opponentIVs, NULL, data->opponentPersonalities, num, HEAP_ID_FIELD2, NARC_a_2_0_3);
}

#pragma force_active reset

void BattleFactory_TradeMon(BattleFactoryData *data) {
    if (data->selection[0] != 0xFF) {
        Pokemon *mon = Party_GetMonByIndex(data->opponentParty, data->selection[1]);
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(data->playerParty, data->selection[0], mon);
        data->partySetIds[data->selection[0]] = data->rentals.setIds[data->selection[1]];
        BattleFactory_IncTradeCount(data);
        GameStats_Inc(Save_GameStats_Get(data->saveData), GAME_STAT_UNK65);
    }
}

void BattleFactory_SetOpponentParty(BattleFactoryData *data) {
    FrontierMon *opponent;
    int count;
    int i;
    Pokemon *mon;

    BattleFactory_GetPartySize(data->type);
    count = BattleFactory_GetOpponentMonCount(data->type, 1);
    SaveArray_Party_Init(data->opponentParty);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    i = 0;
    if (count > 0) {
        opponent = data->opponentMons;
        do {
            FrontierMon_ToPokemon(opponent, mon, BattleFactory_GetMonLevel(data));
            Frontier_AddRentalMonToParty(data->saveData, data->opponentParty, mon);
            opponent++;
            i++;
        } while (i < count);
    }
    Heap_Free(mon);
    for (i = 0; i < count; i++) {
        Party_GetMonByIndex(data->opponentParty, i);
    }
}

void BattleFactory_IncTradeCount(BattleFactoryData *data) {
    u16 tradeCount = data->tradeCount;
    if (tradeCount < 9999) {
        data->tradeCount = tradeCount + 1;
    }
}

u16 BattleFactory_SendBuffer(BattleFactoryData *data, u32 command, int value) {
    switch (command) {
    case 0:
        BattleFactoryLink_SendRecord(data);
        break;
    case 1:
        BattleFactoryLink_SendTrainerIds(data);
        break;
    case 2:
        BattleFactoryLink_SendRentalMons(data);
        break;
    case 3:
        BattleFactoryLink_SendOpponentMons(data);
        break;
    case 4:
        BattleFactoryLink_SendValueA(data, (u8)value);
        break;
    case 5:
        BattleFactoryLink_SendValueB(data, (u8)value);
        break;
    case 6:
        BattleFactoryLink_SendPartyMons(data);
        break;
    }
}

int BattleFactory_GetWonBattlePoints(BattleFactoryData *data) {
    u8 type = data->type;
    u16 round = data->round;
    int points;
    if (type <= 1) {
        points = round >= 8 ? 9 : gBattleFactoryBattlePoints[round];
    } else {
        points = round >= 8 ? 21 : gBattleFactoryMultiBattlePoints[round];
    }
    // the Factory Head's battles (21st and 49th)
    if (type == 0) {
        u16 winStreak = data->winStreak;
        if (winStreak == 21 || winStreak == 49) {
            points = 20;
        }
    }
    return points;
}
