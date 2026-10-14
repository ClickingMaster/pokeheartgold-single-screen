#ifndef POKEHEARTGOLD_BATTLE_SETUP_H
#define POKEHEARTGOLD_BATTLE_SETUP_H

#include "constants/battle.h"

#include "bag.h"
#include "field_types_def.h"
#include "game_stats.h"
#include "gf_rtc.h"
#include "map_header.h"
#include "party.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "save_palpad.h"
#include "save_wifi_history.h"
#include "trainer_data.h"
#include "field_warp_state.h"
#include "evolution_scene.h"

struct BattleSetupSub_138 {
    int totalTurns;
    int totalTimesFainted;
    int totalDamage;
};

struct BattleSetup {                     // declared in trainer_data.h
    u32 battleType;                      // 0
    Party *party[BATTLER_MAX];           // 4
    int winFlag;                         // 14
    int trainerId[BATTLER_MAX];          // 18
    Trainer trainer[BATTLER_MAX];        // 28
    PlayerProfile *profile[BATTLER_MAX]; // f8
    Bag *bag;                            // 108
    BagCursor *bagCursor;                // 10c
    Pokedex *pokedex;                    // 110
    PCStorage *storagePC;                // 114
    SOUND_CHATOT *chatot[BATTLER_MAX];   // 118
    void *unk_128;
    SaveWiFiHistory *wifiHistory;
    Options *options; // 130
    struct BattleSetupSub_134 *unk_134;
    struct BattleSetupSub_138 battleTotals;
    GameStats *gameStats; // 144
    SavePalPad *palPad;   // 148
    BattleBg battleBg;    // 14C
    Terrain terrain;
    u32 mapSection;        // 154
    u32 mapNumber;         // 158
    TIMEOFDAY timeOfDay;   // 15C
    u32 evolutionLocation; // 160
    u32 unk_164;
    BOOL metBill;          // 168
    int momsSavingsActive; // 16C
    u32 caughtBattlerId;
    u32 weatherType; // 174
    int levelUpFlag; // 178
    u32 aiFlags[BATTLER_MAX];
    u32 battleSpecial; // 18C
    int safariBalls;   // 190
    BOOL fixedDamaageMovesBanned;
    EvolutionTaskData *evolutionTaskData;
    int randSeed;
    int linkPositions[4];
    u16 networkId;
    u8 linkBattleMode;
    u8 unk1B3;
    int totalTurns;
    void *keyPressedSave;
    u8 chatotVoiceParam[BATTLER_MAX];
    SaveData *saveData; // 1c0
    int unk1C4;
    Pokemon *bugContestMon; // 1c8
    u8 followMonSlot[BATTLER_MAX];
    BOOL isFishing;
};

BattleSetup *BattleSetup_New(enum HeapID heapID, u32 battleFlags);
BattleSetup *BattleSetup_New_SafariZone(enum HeapID heapID, int balls);
BattleSetup *BattleSetup_New_BugContest(enum HeapID heapID, int balls, Pokemon *bugmon);
BattleSetup *BattleSetup_New_PalPark(enum HeapID heapID, int balls);
BattleSetup *BattleSetup_New_Tutorial(enum HeapID heapID, FieldSystem *fieldSystem);
void BattleSetup_Delete(BattleSetup *setup);
void BattleSetup_AddMonToParty(BattleSetup *setup, Pokemon *mon, int battler);
void BattleSetup_InitFromSaveData(BattleSetup *setup, FieldSystem *fieldSystem, SaveData *savedata, u32 mapno, BagCursor *bagCursor, void *arg5);
void BattleSetup_InitFromFieldSystem(BattleSetup *setup,
    FieldSystem *fieldSystem);
void BattleSetup_InitForFixedLevelFacility(BattleSetup *setup,
    FieldSystem *fieldSystem,
    int level);
void BattleSetup_InitForLinkBattleWithParty(BattleSetup *setup, FieldSystem *fieldSystem, Party *party, u8 *partySlots);
void BattleSetup_InitForLinkBattle(BattleSetup *setup, FieldSystem *fieldSystem, void *partySlots);
void BattleSetup_CopyResultsToSaveData(BattleSetup *setup, FieldSystem *fieldSystem);
void BattleSetup_CopyPokedexToSaveData(BattleSetup *setup, FieldSystem *fieldSystem);
void BattleSetup_SetFishing(BattleSetup *setup);
BOOL IsBattleResultWin(u32 result);
BOOL IsBattleResultStaticWildWin(u32 result);
BOOL IsBattleResultLatiCaught(u32 result);
void BattleSetup_SetAllySideBattlersToPlayer(BattleSetup *setup);

#endif // POKEHEARTGOLD_BATTLE_SETUP_H
