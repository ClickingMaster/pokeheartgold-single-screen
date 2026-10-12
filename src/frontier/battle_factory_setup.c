#include "frontier/battle_factory_setup.h"

#include "global.h"

#include "constants/battle.h"

#include "battle/battle_setup.h"
#include "frontier/battle_factory_data.h"
#include "frontier/frontier.h"

#include "assert.h"
#include "communication_player_state.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "unk_02035900.h"
#include "use_item_on_mon.h"

// Battle Factory trainers, sets and battles: the trainers of a round, the ranges of set ids their Pokemon and the
// rentals are drawn from, and the battle setup of a factory battle.

#define FACTORY_SETS_NARC 205

// Battle Factory set (narc member).
typedef struct BattleFactorySet {
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 evMask;
    u8 nature;
    u16 item;
    u16 form;
} BattleFactorySet;

// Trainer ids of a round: regular trainers, and the 7th battle of a round.
typedef struct BattleFactoryTrainerRange {
    u16 min;
    u16 max;
    u16 lastBattleMin;
    u16 lastBattleMax;
} BattleFactoryTrainerRange;

// A trainer as FrontierTrainer_Load reads it.
typedef struct BattleFactoryTrainerRecord {
    u8 data[0x30];
} BattleFactoryTrainerRecord;

extern const int gBattleFactoryTradeCountTiers[5];
extern const BattleFactoryTrainerRange gBattleFactoryRoundTrainerRanges[];
extern const BattleFactorySetRange gBattleFactoryOpenLevelRentalRanges[];
extern const BattleFactorySetRange gBattleFactoryLevel50SetRanges[];
extern const BattleFactorySetRange gBattleFactoryOpenLevelTrainerSetRanges[];
extern const BattleFactorySetRange *gBattleFactoryTrainerSetRanges;

void FrontierData_ReadMember(void *dest, s32 fileId, int narcId);
void *FrontierTrainer_Load(BattleFactoryTrainerRecord *record, u16 trainerId, enum HeapID heapID, int narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *setup, BattleFactoryTrainerRecord *record, int count, int battler, enum HeapID heapID);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);

int BattleFactory_GetRandomTrainerId(int type, u32 round, int battleNo) {
    u16 min;
    int span;

    if (round >= 8) {
        round = 7;
    }
    if (type == 0) {
        int battle = (battleNo + 1) + round * 7;
        if (battle == 21) {
            return 309;
        }
        if (battle == 49) {
            return 310;
        }
    }
    if (battleNo == 6 || battleNo == 13) {
        min = gBattleFactoryRoundTrainerRanges[round].lastBattleMin;
        span = gBattleFactoryRoundTrainerRanges[round].lastBattleMax - min;
    } else {
        min = gBattleFactoryRoundTrainerRanges[round].min;
        span = gBattleFactoryRoundTrainerRanges[round].max - min;
    }
    return min + LCRandom() % span;
}

void BattleFactory_GetRoundTrainerIds(int type, int round, u16 *trainerIds, int count) {
    int i = 0;
    int j;
    u16 *trainerId = trainerIds;

    do {
        *trainerId = BattleFactory_GetRandomTrainerId(type, round, i);
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

const BattleFactorySetRange *BattleFactory_GetTrainerSetRange(int trainerId, int levelMode) {
    int count;
    int i;

    if (levelMode == 0) {
        gBattleFactoryTrainerSetRanges = gBattleFactoryLevel50SetRanges;
        count = 10;
    } else {
        gBattleFactoryTrainerSetRanges = gBattleFactoryOpenLevelTrainerSetRanges;
        count = 10;
    }
    for (i = 0; i < count; i++) {
        if (trainerId < gBattleFactoryTrainerSetRanges[i].maxTrainerId) {
            break;
        }
    }
    if (i >= count) {
        GF_AssertFail();
        i = count - 1;
    }
    return &gBattleFactoryTrainerSetRanges[i];
}

const BattleFactorySetRange *BattleFactory_GetRentalSetRange(int round, int levelMode) {
    if (round >= 8) {
        round = 7;
    }
    if (levelMode == 0) {
        return &gBattleFactoryLevel50SetRanges[round];
    }
    return &gBattleFactoryOpenLevelRentalRanges[round];
}

#pragma force_active on
int BattleFactory_PickSetIds(const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID, const BattleFactorySetRange *range, u16 tradeCount, u8 *ivs) {
    BattleFactorySet sets[6];
    int i, j;
    int numNextTier;
    int iv;
    int span;
    int setId;
    BattleFactorySet *set;
    u16 *setIdPtr;

    GF_ASSERT(count <= 6);
    i = 0;
    span = range->maxSetId - range->minSetId;
    numNextTier = 0;
    if (tradeCount != 0) {
        numNextTier = 5;
        for (j = 0; j < NELEMS(gBattleFactoryTradeCountTiers); j++) {
            if (tradeCount <= gBattleFactoryTradeCountTiers[j]) {
                numNextTier = j;
                break;
            }
        }
    }
    if (count != 0) {
        set = sets;
        setIdPtr = setIds;
        do {
            if (i >= count - numNextTier && range->hasNextTier == TRUE) {
                span = range[1].maxSetId - range[1].minSetId;
                setId = range[1].maxSetId - LCRandom() % (span + 1);
                iv = range[1].iv;
            } else {
                setId = range->maxSetId - LCRandom() % (span + 1);
                iv = range->iv;
            }
            FrontierData_ReadMember(set, setId, FACTORY_SETS_NARC);
            for (j = 0; j < i; j++) {
                BattleFactorySet *other = &sets[j];
                if (other->species == set->species || other->item == set->item) {
                    break;
                }
            }
            if (j == i) {
                for (j = 0; j < numExcluded; j++) {
                    if (set->species == excludedSpecies[j] || set->item == excludedItems[j]) {
                        break;
                    }
                }
                if (j == numExcluded) {
                    *setIdPtr = setId;
                    ivs[i] = iv;
                    i++;
                    set++;
                    setIdPtr++;
                }
            }
        } while (i != count);
    }
    return 0;
}
#pragma force_active reset

int BattleFactory_GetPartySize(int type) {
    switch (type) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        return 2;
    }
    return 0;
}

int BattleFactory_GetOpponentMonCount(int type, int both) {
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
    }
    return 0;
}

// The caller passes excluded set ids and excluded items, but both lists are read from excludedSetIds.
void BattleFactory_GenerateRentalSets(int round, int levelMode, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, u16 tradeCount, const u16 *excludedSetIds, const u16 *excludedItems) {
    const BattleFactorySetRange *range = BattleFactory_GetRentalSetRange(round, levelMode);

    if (excludedSetIds == NULL) {
        BattleFactory_PickSetIds(NULL, NULL, 0, 6, setIds, HEAP_ID_FIELD2, range, tradeCount, ivs);
    } else {
        BattleFactory_PickSetIds(excludedSetIds, excludedSetIds, 6, 6, setIds, HEAP_ID_FIELD2, range, tradeCount, ivs);
    }
    FrontierMon_GenerateList(mons, setIds, ivs, NULL, otIds, 6, HEAP_ID_FIELD2, FACTORY_SETS_NARC);
}

#pragma force_active on
void BattleFactory_GenerateTrainerSets(u8 count, u16 trainerId, u8 levelMode, const FrontierMon *partyMons, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, int numPartyMons) {
    FrontierMon partyMon;
    u16 species[12];
    u16 items[12];
    int i;
    const BattleFactorySetRange *range = BattleFactory_GetTrainerSetRange(trainerId, levelMode);

    for (i = 0; i < numPartyMons; i++) {
        partyMon = partyMons[i];
        species[i] = partyMon.species;
        items[i] = partyMon.item;
    }
    BattleFactory_PickSetIds(species, items, numPartyMons, count, setIds, HEAP_ID_FIELD2, range, 0, ivs);
    FrontierMon_GenerateList(mons, setIds, ivs, NULL, otIds, count, HEAP_ID_FIELD2, FACTORY_SETS_NARC);
}

BattleSetup *BattleFactory_NewBattleSetup(BattleFactoryData *data, FrontierLaunchArgs *args) {
    int playerCount = BattleFactory_GetPartySize(data->type);
    int opponentCount = BattleFactory_GetOpponentMonCount(data->type, FALSE);
    BattleSetup *setup;
    Pokemon *mon;
    int i, j;
    BattleFactoryTrainerRecord trainer;

    HealParty(data->playerParty);
    HealParty(data->opponentParty);
    setup = BattleSetup_New(HEAP_ID_FIELD2, BattleFactory_GetBattleType(data->type));
    BattleSetup_InitFromSaveData(setup, NULL, args->saveData, args->mapId, args->bagCursor, args->unk1C);
    setup->battleBg = BATTLE_BG_BATTLE_FACTORY;
    setup->terrain = TERRAIN_BATTLE_FACTORY;
    Party_InitWithMaxSize(setup->party[BATTLER_PLAYER], playerCount);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < playerCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(data->playerParty, i), mon);
        BattleSetup_AddMonToParty(setup, mon, BATTLER_PLAYER);
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(&trainer, data->trainerIds[data->battleNo], HEAP_ID_FIELD2, 204));
    FrontierTrainer_SetBattleTrainer(setup, &trainer, opponentCount, BATTLER_ENEMY, HEAP_ID_FIELD2);
    Party_InitWithMaxSize(setup->party[BATTLER_ENEMY], BattleFactory_GetOpponentMonCount(data->type, FALSE));
    for (i = 0; i < BATTLER_MAX; i++) {
        setup->trainer[i].data.aiFlags = BattleFactory_GetTrainerAIFlags(data);
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < opponentCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(data->opponentParty, i), mon);
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
        Party_InitWithMaxSize(setup->party[BATTLER_ENEMY2], BattleFactory_GetOpponentMonCount(data->type, FALSE));
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        j = 0;
        if (opponentCount > 0) {
            i = opponentCount;
            do {
                CopyPokemonToPokemon(Party_GetMonByIndex(data->opponentParty, i), mon);
                BattleSetup_AddMonToParty(setup, mon, BATTLER_ENEMY2);
                i++;
                j++;
            } while (j < opponentCount);
        }
        Heap_Free(mon);
        break;
    }
    return setup;
}
#pragma force_active reset

u32 BattleFactory_GetBattleType(int type) {
    switch (type) {
    case 0:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER;
    case 1:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    case 2:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    case 3:
        return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_MULTI | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TRAINER;
    }
    return BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER;
}

int BattleFactory_GetMonLevel(BattleFactoryData *data) {
    if (data->levelMode == 0) {
        return 50;
    }
    return 100;
}

void BattleFactory_SetPartyToRentals(BattleFactoryData *data) {
    int i;
    Pokemon *mon;
    FrontierMon *frontierMon;

    FrontierMon_GenerateList(data->rentals.mons, data->rentals.setIds, data->rentals.ivs, data->rentals.personalities, NULL, FACTORY_RENTAL_COUNT, HEAP_ID_FIELD2, FACTORY_SETS_NARC);
    SaveArray_Party_Init(data->playerParty);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0, frontierMon = data->rentals.mons; i < FACTORY_RENTAL_COUNT; i++, frontierMon++) {
        FrontierMon_ToPokemon(frontierMon, mon, BattleFactory_GetMonLevel(data));
        ov80_0222A3BC(data->saveData, data->playerParty, mon);
    }
    Heap_Free(mon);
}

void BattleFactory_AddPartnerMonsToParty(BattleFactoryData *data) {
    int i;
    int count;
    Pokemon *mon;
    FrontierMon partnerMons[2];

    count = Party_GetCount(data->playerParty);
    while (count > 2) {
        Party_RemoveMon(data->playerParty, count - 1);
        count--;
    }
    FrontierMon_GenerateList(partnerMons, data->partnerRentals.setIds, data->partnerRentals.ivs, data->partnerRentals.personalities, NULL, 2, HEAP_ID_FIELD2, FACTORY_SETS_NARC);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < 2; i++) {
        FrontierMon_ToPokemon(&partnerMons[i], mon, BattleFactory_GetMonLevel(data));
        ov80_0222A3BC(data->saveData, data->playerParty, mon);
        data->partySetIds[2 + i] = data->partnerRentals.setIds[i];
    }
    Heap_Free(mon);
}

BOOL BattleFactory_IsMultiType(int type) {
    switch (type) {
    case 2:
    case 3:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
int BattleFactory_GetTrainerAIFlags(BattleFactoryData *data) {
    int aiFlags, round;

    if (data->type == 0 && (data->trainerIds[data->battleNo] == 309 || data->trainerIds[data->battleNo] == 310)) {
        return 7;
    }
    round = BattleFactory_GetRound(data);
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
#pragma force_active reset

int BattleFactory_GetRound(BattleFactoryData *data) {
    u16 round = data->round;

    if (BattleFactory_IsMultiType(data->type) == TRUE) {
        if (data->partnerRound > data->round) {
            round = data->partnerRound;
        }
    }
    return round;
}
