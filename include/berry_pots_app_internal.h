#ifndef POKEHEARTGOLD_BERRY_POTS_APP_INTERNAL_H
#define POKEHEARTGOLD_BERRY_POTS_APP_INTERNAL_H

#include "game_stats.h"
#include "launch_application.h"
#include "berry_pots.h"
#include "sprite.h"
#include "touchscreen_list_menu.h"

enum BerryPotsAppMessage {
    BERRY_POTS_APP_MSG_TOUCH_POT,
    BERRY_POTS_APP_MSG_NO_BERRIES,
    BERRY_POTS_APP_MSG_NO_FERTILIZER,
    BERRY_POTS_APP_MSG_WATERING_HELP,
    BERRY_POTS_APP_MSG_SOFT_SOIL,
    BERRY_POTS_APP_MSG_MULCH_LAID,
    BERRY_POTS_APP_MSG_PLANTED,
    BERRY_POTS_APP_MSG_SPROUTED,
    BERRY_POTS_APP_MSG_GROWING,
    BERRY_POTS_APP_MSG_BLOOM,
    BERRY_POTS_APP_MSG_HAS_BERRIES,
    BERRY_POTS_APP_MSG_MULCH_SCATTERED,
    BERRY_POTS_APP_MSG_BERRY_PLANTED,
    BERRY_POTS_APP_MSG_WATERED,
    BERRY_POTS_APP_MSG_PICKED,
    BERRY_POTS_APP_MSG_PUT_AWAY,
    BERRY_POTS_APP_MSG_BAG_FULL,
};

typedef struct BerryPotsAppArgs_UnkSub4 {
    u8 unk0[4];
    MenuInputStateMgr *menuInputPtr;
    u8 unk8[4];
    SaveData *savedata;
} BerryPotsAppArgs_UnkSub4;

typedef struct BerryPotsAppArgs {
    u8 unk0[4];
    BerryPotsAppArgs_UnkSub4 *launchArgs;
    u8 unk8[0xC];
    u8 itemAction;
    u8 potIndex;
    u16 itemId;
} BerryPotsAppArgs;

typedef struct BerryPotsAppPot {
    u8 berryId;
    u8 growthStage;
    u8 health;
    u8 moisture;
    u8 mulch;
    u8 quantityOrYieldMaybe;
    u16 minutesToNextStage;
    u16 drainMinutes;
    u8 soilState;
    u8 prevBerryId;
    u8 prevGrowthStage;
    u8 prevSoilState;
    Sprite *soilSpriteMaybe;
} BerryPotsAppPot;

typedef struct BerryPotsAppData {
    enum HeapID heapID;
    BerryPotsAppArgs *args;
    void *unk8;
    Bag *bag;
    GameStats *stats;
    BerryPot *berryPots;
    struct GF_RTC_DateTime *berryDatetime;
    BerryGrowthProperties *berryGrowthProperties;
    BerryPotsAppPot pots[MAX_BERRY_POT];
    u16 state70;
    u16 state72;
    MenuInputState fieldMenuState;
    u8 textFrameDelay;
    u8 frame;
    u8 messageTimer;
    u8 appliedItemAction;
    u8 selectedPot;
    u8 runningTasks;
    u8 harvestBerriesLeft;
    u8 waitFrames;
    u8 wateringPot;
    u8 nextWateringPot;
    u16 totalBerryQuantity;
    u16 totalMulchQuantity;
    BgConfig *bgConfig;
    TouchscreenListMenuSpawner *menuSpawner;
    u8 unk90[12];
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *currentStatusString;
    String *cancelString;
    u8 unkAC[4];
    String *statusStrings[17];
    u8 unkF4[8];
    Window windows[3];
    void *yesNoPrompt;
    SpriteSystem *spriteRenderer;
    SpriteManager *spriteGfxHandler1;
    SpriteManager *spriteGfxHandler2;
    u8 unk13C[40];
    Sprite *sprites[11];
    NARC *itemIconNarc;
    NARC *berryPotsAppNarc;
    ListMenuItem *listMenuItems[4];
    TouchscreenListMenu *menu;
} BerryPotsAppData;

void BerryPotsApp_UpdateCursor(BerryPotsAppData *data);
void BerryPotsApp_DrawCancelButton(BerryPotsAppData *data, u32 seqNo);
void BerryPotsApp_WaterPot(BerryPotsAppData *data, u8 index);
ManagedSprite *BerryPotsApp_CreateBerryIconSprite(BerryPotsAppData *data, s16 x, s16 y, u8 unused);
void BerryPotsApp_UpdatePlantSprite(BerryPotsAppData *data, int index);
void BerryPotsApp_StartWateringCanMove(BerryPotsAppData *data);
void BerryPotsApp_StartHarvestBerryAnim(BerryPotsAppData *data);
void BerryPotsApp_FormatMessage(BerryPotsAppData *data, enum BerryPotsAppMessage action, u8 potIndex);
void BerryPotsApp_UpdateGrowthFromRTC(BerryPotsAppData *data, BOOL startAnims);
void BerryPotsApp_RefreshPotData(BerryPotsAppData *data);
void BerryPotsApp_StartGrowthAnims(BerryPotsAppData *data);
u32 BerryPotsApp_HandleSelectPotInput(BerryPotsAppData *data);
BOOL BerryPotsApp_HandleWateringInput(BerryPotsAppData *data);
u32 BerryPotsApp_SelectPotAction(BerryPotsAppData *data, u32 selection);
u32 BerryPotsApp_HandleSelectPotKeys(BerryPotsAppData *data);
u32 BerryPotsApp_HandleSelectPotTouch(BerryPotsAppData *data, BOOL *touched);
BOOL BerryPotsApp_HandleWateringKeys(BerryPotsAppData *data);
BOOL BerryPotsApp_HandleWateringTouch(BerryPotsAppData *data, BOOL *touched);
int BerryPotsApp_GetTouchedPot(BerryPotsAppData *data);
int BerryPotsApp_GetSelectPotTouch(BerryPotsAppData *data);
u32 BerryPotsApp_GetPotMenuState(BerryPotsAppData *data);

#endif // POKEHEARTGOLD_BERRY_POTS_APP_INTERNAL_H
