#ifndef POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_H
#define POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_H

#include "battle/battle_setup.h"
#include "credits/credits.h"
#include "pokeathlon/pokeathlon.h"
#include "voltorb_flip/voltorb_flip.h"

#include "alph_puzzle.h"
#include "bag.h"
#include "certificates_app.h"
#include "choose_starter_app.h"
#include "fashion_case.h"
#include "mail.h"
#include "mail_misc.h"
#include "move_relearner.h"
#include "overlay_02.h"
#include "mailbox_app.h"
#include "safari_area_customizer.h"
#include "photo_album_app.h"
#include "bug_contest_swap_mon.h"
#include "unown_report.h"
#include "overlay_14.h"
#include "berry_pots.h"
#include "overlay_18.h"
#include "overlay_41.h"
#include "overlay_43.h"
#include "overlay_54.h"
#include "overlay_58.h"
#include "battle_room_results.h"
#include "nintendo_wfc_connect.h"
#include "record_mixing.h"
#include "overlay_86.h"
#include "overlay_87.h"
#include "overlay_99.h"
#include "overlay_trainer_card.h"
#include "party_menu.h"
#include "pokemon_types_def.h"
#include "register_hall_of_fame.h"
#include "sav_system_info.h"
#include "save_palpad.h"
#include "save_special_ribbons.h"
#include "save_wifi_history.h"
#include "script.h"
#include "pokemon_summary.h"
#include "gear_phone_ring_manager.h"
#include "legendary_cinematic_manager.h"
#include "view_rankings_task.h"

typedef enum NameScreenType {
    NAME_SCREEN_PLAYER,
    NAME_SCREEN_POKEMON,
    NAME_SCREEN_BOX,
    NAME_SCREEN_RIVAL,
    NAME_SCREEN_UNK4,
    NAME_SCREEN_GROUP,
    NAME_SCREEN_UNK6,
    NAME_SCREEN_UNK7,
} NameScreenType;

typedef enum PocketType {
    POCKET_TYPE_ITEMS,
    POCKET_TYPE_BERRIES,
} PocketType;

typedef struct GlobalTerminalLaunchArgs {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    int connectDirectly;
    u8 unkC;
} GlobalTerminalLaunchArgs;

struct UnkStruct_ScrCmd230 {
    u8 filler_00[0x30];
    u8 unk_30[6];
};

typedef struct UnkStruct_0203E600 {
    u8 unk0[0x14];
    u8 unk14;
    u8 unk15;
    u8 unk16;
} UnkStruct_0203E600;

typedef struct EasyChatResult {
    u16 changed;
    u16 unk2;
} EasyChatResult;

static inline void InitUnkStructScrCmd408(BattleRoomResultsArgs *data, u16 a1, u16 a2, ScriptContext *ctx) {
    MI_CpuClear8(data, sizeof(BattleRoomResultsArgs));
    data->showLeaders = a1;
    data->unk_6 = a2;
    data->saveData = ctx->fieldSystem->saveData;
}

static inline PCBoxArgs *PCBoxAppData_New(ScriptContext *ctx) {
    PCBoxArgs *ret = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PCBoxArgs));
    ret->saveData = ctx->fieldSystem->saveData;
    ret->mode = ScriptReadByte(ctx);
    ret->menuInputStatePtr = &ctx->fieldSystem->menuInputState;
    return ret;
}

UseMailArgs *UseMail_CreateArgs(FieldSystem *fieldSystem, int n, u8 mailId, enum HeapID heapID);
ApricornBoxArgs *ApricornBox_LaunchApp(FieldSystem *fieldSystem, int mode);
PalPadArgs *PalPad_LaunchApp(FieldSystem *fieldSystem, SaveData *saveData, enum HeapID heapID);
BerryPotsArgs *BerryPots_LaunchApp(FieldSystem *fieldSystem);
UnownReportArgs *UnownReport_LaunchApp(FieldSystem *fieldSystem);
PartyMenuArgs *PartyMenu_LaunchApp_Gracidea(FieldSystem *fieldSystem, enum HeapID heapID, u16 itemId);
PartyMenuArgs *PartyMenu_LaunchApp_PartySelect(enum HeapID heapID, FieldSystem *fieldSystem);        // todo: party select screen
PartyMenuArgs *PartyMenu_LaunchApp_InGameTrade(enum HeapID heapID, FieldSystem *fieldSystem); // todo: party trade screen
PartyMenuArgs *SelectPartyMonAndLearnMove(TaskManager *taskManager, enum HeapID heapID);      // todo: union party select screen
int PartyMenuArgs_GetSlot(struct PartyMenuArgs *partyWork);
int PartyMenuArgs_GetSelectedAction(struct PartyMenuArgs *partyWork);
void VsRecorder_LaunchApp(FieldSystem *fieldSystem, SaveData *saveData);
SafariDecorationArgs *SafariDecoration_LaunchApp(FieldSystem *fieldSystem);
void GeonetGlobe_LaunchApp(FieldSystem *fieldSystem);
void ChooseStarter_LaunchApp(FieldSystem *fieldSystem, ChooseStarterArgs *args);
u16 sub_0203E864(UnkStruct_0203E600 *a0);
u16 sub_0203E600(UnkStruct_0203E600 *a0);
PokemonSummaryArgs *LearnForgetMove_LaunchApp(enum HeapID heapID, FieldSystem *fieldSystem, u8 partySlot, u16 moveToLearn);
ApricornBoxArgs *sub_0203ED80(FieldSystem *fieldSystem, u32 a1, u16 *juiceUsedVar);
BugContestSwapMonArgs *BugContestSwapMon_LaunchApp(FieldSystem *fieldSystem, Pokemon *newlyCaughtMon, Pokemon *currentMon, BOOL noPokemonCaught);
PokemonSummaryArgs *PokemonSummary_LaunchApp(enum HeapID heapID, FieldSystem *fieldSystem, u16 partySlot, u16 moveToLearn);
void CallTask_DressUp(TaskManager *taskManager, u16 *ret, SaveData *saveData, u16 partyIdx, u16 a4);
BOOL AccessoryPortrait_LaunchApp(FieldSystem *fieldSystem, FashionAppData *fashionData);
void BattleRoomResults_LaunchApp(FieldSystem *fieldSystem, BattleRoomResultsArgs *unk);
void PCBox_LaunchApp(FieldSystem *fieldSystem, PCBoxArgs *pcBoxData);
void CallTask_WirelessTrade(TaskManager *taskManager);
void DrawingSession_LaunchApp(FieldSystem *fieldSystem);
RecordMixingArgs *RecordMixing_LaunchApp(FieldSystem *fieldSystem);
HallOfFame *HallOfFameShowcase_LaunchApp(FieldSystem *fieldSystem);
void Gts_LaunchApp(FieldSystem *fieldSystem, u16 connectDirectly);
BagView *Bag_LaunchApp_WithPocket(FieldSystem *fieldSystem, u8 pocketType);
int BagView_SelectResult(BagView *bagView);
void MoveRelearner_LaunchApp(FieldSystem *fieldSystem, MoveRelearnerArgs *moveRelearner);
BagView *Bag_LaunchApp_AllPockets(FieldSystem *fieldSystem, ItemCheckUseData *itemCheckUseData);
void Bag_LaunchApp(FieldSystem *fieldSystem, BagView *bagView);
void PokemonSummary_LearnForget_LaunchApp(FieldSystem *fieldSystem, PokemonSummaryArgs *pokemonSummayArgs);
PokemonSummaryArgs *PokemonSummary_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID, int mode);
PartyMenuArgs *TaskManager_LaunchPartyMenu_UnionRoomBattleSelect(TaskManager *taskman, enum HeapID heapID);
PartyMenuArgs *PartyMenu_LaunchApp_SpinTrade(FieldSystem *fieldSystem, int partySlot);
void EasyChat_LaunchApp(FieldSystem *fieldSystem, EasyChatArgs *args);
PokegearArgs *PokegearTownMap_LaunchApp(FieldSystem *fieldSystem, int kind);
SafariAreaCustomizerArgs *SafariAreaCustomizer_LaunchApp(FieldSystem *fieldSystem);
PhotoAlbumArgs *PhotoAlbum_LaunchApp(FieldSystem *fieldSystem, int unused, int initialCursorPos);
OptionsMenuArgs *OptionsMenu_LaunchApp(FieldSystem *fieldSystem);
UseMailArgs *UseMail_LaunchApp_Compose(FieldSystem *fieldSystem, u16 a1, u8 partyIdx, u8 mailType, enum HeapID heapID);
UseMailArgs *UseMail_LaunchApp_ReadMonMail(FieldSystem *fieldSystem, Pokemon *mon, enum HeapID heapID);
void TrainerCard_LaunchApp(FieldSystem *fieldSystem, TrainerCardAppArgs *args);
int Pokedex_LaunchApp(FieldSystem *fieldSystem, PokedexArgs *args);
NintendoWifiConnectArgs *NintendoWifiConnection_LaunchApp(FieldSystem *fieldSystem, u16 mode, u16 connectNow);
void PokeathlonCourse_LaunchApp(FieldSystem *fieldSystem, PokeathlonCourseArgs *args);
void MagnetTrainCutscene_LaunchApp(FieldSystem *fieldSystem, void *args);
void ShipDepartureCutscene_LaunchApp(FieldSystem *fieldSystem, void *args);
LegendaryCinematicArgs *LegendaryCinematic_LaunchApp(FieldSystem *fieldSystem, UnkStruct_0203FCC4 *modelAttributes, u16 scene, u16 timeOfDay, enum HeapID heapID);
void LaunchApp_ViewRankings(FieldSystem *fieldSystem, ViewRankingsArgs *args);
PartyMenuArgs *PartyMenu_LaunchApp_Field(FieldSystem *fieldSystem, FieldMoveCheckData *fieldMoveCheckData, u8 partySlot);
void CallTask_NamingScreen(TaskManager *taskManager, NameScreenType type, int species, int maxLen, int initPos, const u16 *defaultStr, u16 *retVar);
void CallTask_EasyChat(TaskManager *taskman, int kind, EasyChatResult *result, u16 *word1, u16 *word2);
void TrainerCardSignature_LaunchApp(FieldSystem *fieldSystem);
void HatchEggInParty(FieldSystem *fieldSystem);
MailboxAppArgs *Mailbox_LaunchApp(FieldSystem *fieldSystem, enum HeapID heapID);
CertificatesArgs *Certificates_LaunchApp(FieldSystem *fieldSystem, enum HeapID heapID, u16 certificateId);
PartyMenuArgs *PartyMenu_LaunchApp_DayCare(enum HeapID heapID, FieldSystem *fieldSystem, u16 partySlot);
FrontierRecordsAppArgs *FrontierRecords_LaunchApp(FieldSystem *fieldSystem, u8 mode, u8 facility, u16 species, enum HeapID heapID);
ScratchOffCardsArgs *ScratchOffCards_LaunchApp(FieldSystem *fieldSystem, enum HeapID heapID);
void GlobalTerminal_LaunchApp(FieldSystem *fieldSystem, u16 connectDirectly, u16 a2);
PokegearArgs *PokegearPhone_LaunchApp(FieldSystem *fieldSystem);
PokeathlonMedalsArgs *PokeathlonMedals_LaunchApp(FieldSystem *fieldSystem);
PokeathlonEventRecordArgs *PokeathlonEventRecord_LaunchApp(FieldSystem *fieldSystem);
PokeathlonTrophiesArgs *PokeathlonTrophies_LaunchApp(FieldSystem *fieldSystem);
PokeathlonCourseRecordArgs *PokeathlonCourseRecord_LaunchApp(FieldSystem *fieldSystem);
AlphPuzzleArgs *AlphPuzzle_LaunchApp(FieldSystem *fieldSystem, u8 puzzle);
void RegisterHallOfFame_LaunchApp(FieldSystem *fieldSystem, RegisterHallOfFameArgs *args);
void Credits_LaunchApp(FieldSystem *fieldSystem, CreditsArgs *args);
void Battle_LaunchApp(FieldSystem *fieldSystem, BattleSetup *battleSetup);
VoltorbFlipArgs *VoltorbFlip_LaunchApp(FieldSystem *fieldSystem, u32 luck);

BOOL Battle_Init(OverlayManager *man, int *state);
BOOL Battle_Main(OverlayManager *man, int *state);
BOOL Battle_Exit(OverlayManager *man, int *state);

void LaunchApplication_DeadstrippedFunction(FieldSystem *fieldSystem);
extern const OverlayManagerTemplate gOverlayTemplate_UnownReport;
extern const OverlayManagerTemplate gOverlayTemplate_Battle;

#endif // POKEHEARTGOLD_FIELD_LAUNCH_APPLICATION_H
