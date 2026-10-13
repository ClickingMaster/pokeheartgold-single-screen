#include "global.h"

#include "filesystem.h"
#include "gf_rtc.h"
#include "berry_pots.h"

typedef struct BerryFile {
    u16 sizeInMillimeters;
    u8 firmness;
    u8 yieldMultiplier; // yield multiplier?
    u8 defaultGrowthInterval;
    u8 defaultDrainRate;
    u8 spiciness;
    u8 dryness;
    u8 sweetness;
    u8 bitterness;
    u8 sourness;
    u8 smoothness;
} BerryFile; // size: 0xc

enum BerryAttr {
    BERRY_ATTR_SIZE,
    BERRY_ATTR_FIRMNESS,
    BERRY_ATTR_YIELD_MULTIPLIER,
    BERRY_ATTR_GROWTH_INTERVAL,
    BERRY_ATTR_DRAIN_RATE,
    BERRY_ATTR_SPICINESS,
    BERRY_ATTR_DRYNESS,
    BERRY_ATTR_SWEETNESS,
    BERRY_ATTR_BITTERNESS,
    BERRY_ATTR_SOURNESS,
    BERRY_ATTR_SMOOTHNESS,
};

static void BerryPot_Clear(BerryPot *berryPot);
static u32 BerryPot_CalcYield(BerryPot *berryPot, BerryGrowthProperties *growthProperties);
static u32 CalculateBerryPotGrowthInterval(BerryGrowthProperties *growthProperties, int berryId, int mulch);
static u32 CalculateBerryPotDrainRate(BerryGrowthProperties *growthProperties, int berryId, int mulch);
static u32 BerryPot_GetRipeDurationFactor(BerryPot *berryPot);
static u32 BerryPot_GetMaxRegrowths(BerryPot *berryPot);
static u32 BerryPot_GetLifetimeInIntervals(BerryPot *berryPot);
static void BerryPot_AdvanceGrowthStage(BerryPot *berryPot, BerryGrowthProperties *growthProperties);
static void BerryPot_DrainMoisture(BerryPot *berryPot, BerryGrowthProperties *growthProperties, int minutes);
static NARC *GetBerryInfoNarc(enum HeapID heapID);
static BerryFile *GetBerryFileFromNarc(NARC *narc, s32 fileId, enum HeapID heapID);
static void FreeBerryInfoNarc(NARC *narc);
static u16 GetBerryAttr(BerryFile *unk, enum BerryAttr attr);

BerryGrowthProperties *BerryPots_LoadGrowthProperties(enum HeapID heapID) {
    BerryGrowthProperties *unk;
    NARC *narc;

    narc = GetBerryInfoNarc(heapID);
    unk = Heap_Alloc(heapID, NUM_BERRIES * sizeof(BerryGrowthProperties));

    for (int fileId = 0; fileId < NUM_BERRIES; fileId++) {
        BerryFile *berryFile = GetBerryFileFromNarc(narc, fileId, heapID);
        unk[fileId].defaultGrowthTimeInHours = GetBerryAttr(berryFile, BERRY_ATTR_GROWTH_INTERVAL);
        unk[fileId].defaultDrainRate = GetBerryAttr(berryFile, BERRY_ATTR_DRAIN_RATE);
        unk[fileId].yieldMultiplier = GetBerryAttr(berryFile, BERRY_ATTR_YIELD_MULTIPLIER);

        Heap_Free(berryFile);
    }

    FreeBerryInfoNarc(narc);

    return unk;
}

static void BerryPot_Clear(BerryPot *berryPot) {
    berryPot->berryId = BERRY_NONE;
    berryPot->growthStage = BERRY_POT_GROWTH_STAGE_NONE;
    berryPot->minutesToNextStage = 0;
    berryPot->drainMinutes = 0;
    berryPot->regrowthCount = 0;
    berryPot->yield = 0;
    berryPot->moisture = 0;
    berryPot->health = 0;
    berryPot->mulch = MULCH_NONE;
}

static u32 BerryPot_CalcYield(BerryPot *berryPot, BerryGrowthProperties *growthProperties) {
    return berryPot->health * growthProperties[berryPot->berryId - 1].yieldMultiplier;
}

static u32 CalculateBerryPotGrowthInterval(BerryGrowthProperties *growthProperties, int berryId, int mulch) {
    s32 growthTimeInMinutes = growthProperties[berryId - 1].defaultGrowthTimeInHours * 60;
    if (mulch == MULCH_IDX(ITEM_GROWTH_MULCH)) {
        return (growthTimeInMinutes * 3) / 4;
    } else if (mulch == MULCH_IDX(ITEM_DAMP_MULCH)) {
        growthTimeInMinutes += (growthTimeInMinutes / 2);
    }
    return growthTimeInMinutes;
}

static u32 CalculateBerryPotDrainRate(BerryGrowthProperties *growthProperties, int berryId, int mulch) {
    s32 drainRate = growthProperties[berryId - 1].defaultDrainRate;
    if (mulch == MULCH_IDX(ITEM_DAMP_MULCH)) {
        return drainRate / 2;
    } else if (mulch == MULCH_IDX(ITEM_GROWTH_MULCH)) {
        drainRate += (drainRate / 2);
    }
    return drainRate;
}

static u32 BerryPot_GetRipeDurationFactor(BerryPot *berryPot) {
    return (berryPot->mulch == MULCH_IDX(ITEM_STABLE_MULCH)) ? 6 : 4;
}

static u32 BerryPot_GetMaxRegrowths(BerryPot *berryPot) {
    return (berryPot->mulch == MULCH_IDX(ITEM_GOOEY_MULCH)) ? 15 : 10;
}

static u32 BerryPot_GetLifetimeInIntervals(BerryPot *berryPot) {
    u32 unk1 = BerryPot_GetRipeDurationFactor(berryPot);
    u32 unk2 = BerryPot_GetMaxRegrowths(berryPot);
    return (unk1 + 3) * unk2 + 1;
}

u8 BerryPots_GetPotGrowthStage(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].growthStage;
}

u8 BerryPots_GetPotBerryId(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].berryId;
}

u32 BerryPots_GetSoilState(BerryPot *berryPots, u32 idx) {
    s32 moisture = berryPots[idx].moisture;
    if (moisture == 0) {
        return 0;
    } else if (moisture <= 50) {
        return 1;
    } else {
        return 2;
    }
}

u8 BerryPots_GetPotMoisture(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].moisture;
}

u8 BerryPots_GetPotHealth(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].health;
}

u16 BerryPots_GetPotMinutesToNextStage(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].minutesToNextStage;
}

u16 BerryPots_GetPotDrainMinutes(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].drainMinutes;
}

void BerryPots_PlantBerry(BerryPot *berryPots, u32 idx, BerryGrowthProperties *growthProperties, u32 berryId) {
    berryPots[idx].berryId = berryId;
    berryPots[idx].growthStage = BERRY_POT_GROWTH_STAGE_PLANTED;
    berryPots[idx].minutesToNextStage = CalculateBerryPotGrowthInterval(growthProperties, berryId, berryPots[idx].mulch);
    berryPots[idx].drainMinutes = 0;
    berryPots[idx].regrowthCount = 0;
    berryPots[idx].yield = 0;
    berryPots[idx].moisture = 100;
    berryPots[idx].health = 5;
}

void BerryPots_ResetPotMoisture(BerryPot *berryPots, u32 idx) {
    berryPots[idx].moisture = 100;
}

u8 BerryPots_GetPotMulch(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].mulch;
}

void BerryPots_SetPotMulch(BerryPot *berryPots, u32 idx, u32 mulch) {
    berryPots[idx].mulch = mulch;
}

u16 BerryPots_GetPotYield(BerryPot *berryPots, u32 idx) {
    return berryPots[idx].yield;
}

u16 BerryPots_HarvestPot(BerryPot *berryPots, u32 idx) {
    u16 unk = berryPots[idx].yield;
    BerryPot_Clear(&berryPots[idx]);
    return unk;
}

static void BerryPot_AdvanceGrowthStage(BerryPot *berryPot, BerryGrowthProperties *growthProperties) {
    switch (berryPot->growthStage) {
    case BERRY_POT_GROWTH_STAGE_NONE:
        GF_ASSERT(FALSE);
        return;
    case BERRY_POT_GROWTH_STAGE_PLANTED:
    case BERRY_POT_GROWTH_STAGE_SPROUTED:
    case BERRY_POT_GROWTH_STAGE_GROWING:
        berryPot->growthStage++;
        return;
    case BERRY_POT_GROWTH_STAGE_BLOOM:
        berryPot->yield = BerryPot_CalcYield(berryPot, growthProperties);
        if (berryPot->yield < 2) {
            berryPot->yield = 2;
        }
        berryPot->growthStage++;
        return;
    case BERRY_POT_GROWTH_STAGE_BERRIES:
        berryPot->yield = 0;
        berryPot->growthStage = BERRY_POT_GROWTH_STAGE_SPROUTED;
        berryPot->health = 5;
        berryPot->regrowthCount++;
        if (berryPot->regrowthCount == BerryPot_GetMaxRegrowths(berryPot)) {
            BerryPot_Clear(berryPot);
        }
        return;
    }
}

static void BerryPot_DrainMoisture(BerryPot *berryPot, BerryGrowthProperties *growthProperties, int minutes) {
    if (berryPot->growthStage == BERRY_POT_GROWTH_STAGE_BERRIES) {
        return;
    }

    int drainRate = CalculateBerryPotDrainRate(growthProperties, berryPot->berryId, berryPot->mulch);
    int r4 = (minutes + berryPot->drainMinutes) / 60;
    berryPot->drainMinutes = (minutes + berryPot->drainMinutes) % 60;
    if (r4 == 0) {
        return;
    }

    if (berryPot->moisture >= (drainRate * r4)) {
        berryPot->moisture -= (r4 * drainRate);
        return;
    }

    if (berryPot->moisture != 0) {
        r4 -= (berryPot->moisture + (drainRate - 1)) / drainRate;
        berryPot->moisture = 0;
    }

    if (berryPot->health > r4) {
        berryPot->health -= r4;
        return;
    }

    berryPot->health = 0;
}

void BerryPots_UpdateGrowth(BerryPot *berryPots, BerryGrowthProperties *growthProperties, s32 minutesDelta) {
    for (s32 i = 0; i < MAX_BERRY_POT; i++) {
        if (berryPots[i].berryId == BERRY_NONE || berryPots[i].growthStage == BERRY_POT_GROWTH_STAGE_NONE) {
            continue;
        }

        s32 growthInterval = CalculateBerryPotGrowthInterval(growthProperties, berryPots[i].berryId, berryPots[i].mulch);
        s32 unk = BerryPot_GetLifetimeInIntervals(&berryPots[i]);

        if (minutesDelta >= growthInterval * unk) {
            BerryPot_Clear(&berryPots[i]);
            continue;
        }

        s32 minutesLeft = minutesDelta;

        while (berryPots[i].growthStage != BERRY_POT_GROWTH_STAGE_NONE && minutesLeft != 0) {
            if (berryPots[i].minutesToNextStage > minutesLeft) {
                BerryPot_DrainMoisture(&berryPots[i], growthProperties, minutesLeft);
                berryPots[i].minutesToNextStage -= minutesLeft;
                break;
            }

            BerryPot_DrainMoisture(&berryPots[i], growthProperties, berryPots[i].minutesToNextStage);
            BerryPot_AdvanceGrowthStage(&berryPots[i], growthProperties);
            minutesLeft -= berryPots[i].minutesToNextStage;
            berryPots[i].minutesToNextStage = CalculateBerryPotGrowthInterval(growthProperties, berryPots[i].berryId, berryPots[i].mulch);
            if (berryPots[i].growthStage == BERRY_POT_GROWTH_STAGE_BERRIES) {
                berryPots[i].minutesToNextStage *= BerryPot_GetRipeDurationFactor(&berryPots[i]);
            }
        }
    }
}

void BerryPots_SetBerryDatetime(struct GF_RTC_DateTime *dest, RTCDate srcDate, RTCTime srcTime) {
    dest->date = srcDate;
    dest->time = srcTime;
}

void BerryPots_CopyBerryDatetime(struct GF_RTC_DateTime *datetime, RTCDate *date, RTCTime *time) {
    *date = datetime->date;
    *time = datetime->time;
}

static NARC *GetBerryInfoNarc(enum HeapID heapID) {
    return NARC_New(NARC_a_0_6_6, heapID);
}

static BerryFile *GetBerryFileFromNarc(NARC *narc, s32 fileId, enum HeapID heapID) {
    return NARC_AllocAndReadWholeMember(narc, fileId, heapID);
}

static void FreeBerryInfoNarc(NARC *narc) {
    NARC_Delete(narc);
}

static u16 GetBerryAttr(BerryFile *unk, enum BerryAttr attr) {
    switch (attr) {
    case BERRY_ATTR_SIZE:
        return unk->sizeInMillimeters;
    case BERRY_ATTR_FIRMNESS:
        return unk->firmness;
    case BERRY_ATTR_YIELD_MULTIPLIER:
        return unk->yieldMultiplier;
    case BERRY_ATTR_GROWTH_INTERVAL:
        return unk->defaultGrowthInterval;
    case BERRY_ATTR_DRAIN_RATE:
        return unk->defaultDrainRate;
    case BERRY_ATTR_SPICINESS:
        return unk->spiciness;
    case BERRY_ATTR_DRYNESS:
        return unk->dryness;
    case BERRY_ATTR_SWEETNESS:
        return unk->sweetness;
    case BERRY_ATTR_BITTERNESS:
        return unk->bitterness;
    case BERRY_ATTR_SOURNESS:
        return unk->sourness;
    case BERRY_ATTR_SMOOTHNESS:
        return unk->smoothness;
    default:
        return 0;
    }
}

u16 BerryIdToItemId(u16 berryId) {
    if (berryId == BERRY_NONE) {
        return ITEM_NONE;
    }

    return berryId + FIRST_BERRY_IDX - 1;
}

u16 ItemIdToBerryId(u16 itemId) {
    if (itemId == ITEM_NONE) {
        return BERRY_NONE;
    }

    return itemId - FIRST_BERRY_IDX + 1;
}

u16 MulchIdToItemId(u16 mulchId) {
    if (mulchId == MULCH_NONE) {
        return ITEM_NONE;
    }

    return mulchId + ITEM_GROWTH_MULCH - 1;
}

u32 ItemIdToMulchId(u16 itemId) {
    if (itemId == ITEM_NONE) {
        return MULCH_NONE;
    }

    return itemId - ITEM_GROWTH_MULCH + 1;
}

u16 GetTotalBerryQuantity(Bag *bag, enum HeapID heapID) {
    s32 i;
    u16 total;
    for (total = 0, i = 0; i < NUM_BAG_BERRIES; i++) {
        total += Bag_GetQuantity(bag, FIRST_BERRY_IDX + i, heapID);
    }
    return total;
}

u16 GetTotalMulchQuantity(Bag *bag, enum HeapID heapID) {
    s32 i;
    u16 total;
    for (total = 0, i = 0; i < NUM_MULCHES; i++) {
        total += Bag_GetQuantity(bag, ITEM_GROWTH_MULCH + i, heapID);
    }
    return total;
}
