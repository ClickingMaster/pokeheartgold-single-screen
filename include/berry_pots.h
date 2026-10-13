#ifndef POKEHEARTGOLD_BERRY_POTS_H
#define POKEHEARTGOLD_BERRY_POTS_H

#include "bag.h"
#include "overlay_manager.h"
#include "save_misc_data.h"

typedef struct BerryPotsArgs {
    void *unk0;
    MenuInputStateMgr *menuInputStatePtr;
    BagCursor *cursor;
    SaveData *saveData;
} BerryPotsArgs;

typedef struct BerryGrowthProperties {
    u8 defaultGrowthTimeInHours;
    u8 defaultDrainRate;
    u8 yieldMultiplier;               // yield multiplier?
} BerryGrowthProperties; // size: 0x3

BerryGrowthProperties *BerryPots_LoadGrowthProperties(enum HeapID heapID);
u8 BerryPots_GetPotGrowthStage(BerryPot *berryPots, u32 idx);
u8 BerryPots_GetPotBerryId(BerryPot *berryPots, u32 idx);
u32 BerryPots_GetSoilState(BerryPot *berryPots, u32 idx);
u8 BerryPots_GetPotMoisture(BerryPot *berryPots, u32 idx);
u8 BerryPots_GetPotHealth(BerryPot *berryPots, u32 idx);
u16 BerryPots_GetPotMinutesToNextStage(BerryPot *berryPots, u32 idx);
u16 BerryPots_GetPotDrainMinutes(BerryPot *berryPots, u32 idx);
void BerryPots_PlantBerry(BerryPot *berryPots, u32 idx, BerryGrowthProperties *a2, u32 a3);
void BerryPots_ResetPotMoisture(BerryPot *berryPots, u32 idx);
u8 BerryPots_GetPotMulch(BerryPot *berryPots, u32 idx);
void BerryPots_SetPotMulch(BerryPot *berryPots, u32 idx, u32 mulch);
u16 BerryPots_GetPotYield(BerryPot *berryPots, u32 idx);
u16 BerryPots_HarvestPot(BerryPot *berryPots, u32 idx);
void BerryPots_UpdateGrowth(BerryPot *berryPots, BerryGrowthProperties *a1, s32 a2);
void BerryPots_SetBerryDatetime(struct GF_RTC_DateTime *dest, RTCDate srcDate, RTCTime srcTime);
void BerryPots_CopyBerryDatetime(struct GF_RTC_DateTime *datetime, RTCDate *date, RTCTime *time);
u16 BerryIdToItemId(u16 berryId);
u16 ItemIdToBerryId(u16 itemId);
u16 MulchIdToItemId(u16 mulchId);
u32 ItemIdToMulchId(u16 itemId);
u16 GetTotalBerryQuantity(Bag *bag, enum HeapID heapID);
u16 GetTotalMulchQuantity(Bag *bag, enum HeapID heapID);

BOOL BerryPots_Init(OverlayManager *manager, int *state);
BOOL BerryPots_Main(OverlayManager *manager, int *state);
BOOL BerryPots_Exit(OverlayManager *manager, int *state);

#endif
