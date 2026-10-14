#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H

#include "global.h"

#include "constants/species.h"

#include "pokeathlon/pokeathlon_util.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overlay_99.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "grid_input_handler.h"

// Pokeathlon record screens (OVY_99): course records, trophies, medals and event records.

// NitroSDK MATH (bit count); no repo header declares it
u32 MATH_CountPopulation(u32 value);

// Course record app

typedef struct PokeathlonRecordMon {
    PokeathlonMonInfo info;
    u32 unk8;
} PokeathlonRecordMon;

typedef struct PokeathlonCourseRecordEntry {
    u16 eventScores[3];
    u16 totalScore; // event scores plus bonus points
    PokeathlonRecordMon mons[3];
} PokeathlonCourseRecordEntry; // size: 0x2C

typedef struct PokeathlonCourseRecordBgTemplates {
    BgTemplate templates[8];
} PokeathlonCourseRecordBgTemplates;

typedef struct PokeathlonCourseRecordSpritePos {
    u32 sprite;
    s32 x;
    s32 y;
} PokeathlonCourseRecordSpritePos;

typedef struct PokeathlonCourseRecordSelection {
    s8 cursor; // 0-4: course, 5: return button
    BOOL scrollForward;
    int page;
    BOOL scrolling;
    BOOL pageChanged;
} PokeathlonCourseRecordSelection;

typedef struct PokeathlonCourseRecord {
    PokeathlonCourseRecordArgs *args; // heap copy of the launch args
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    PokeathlonTextContext *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[26];
    PokeathlonCourseRecordSelection selection;
} PokeathlonCourseRecord; // size: 0x94

void PokeathlonCourseRecord_CopyArgs(PokeathlonCourseRecord *work, const PokeathlonCourseRecordArgs *args);
BOOL PokeathlonCourseRecord_AreFlags0EFTo0F1Set(PokeathlonCourseRecordArgs *args);
void PokeathlonCourseRecord_FreeArgs(PokeathlonCourseRecordArgs *args);
PokeathlonRecordMon *PokeathlonCourseRecord_GetMon(PokeathlonCourseRecordArgs *args, int course, int slot);
u16 PokeathlonCourseRecord_GetTotalScore(PokeathlonCourseRecordArgs *args, int course);
BOOL PokeathlonCourseRecord_HasRecord(PokeathlonCourseRecordArgs *args, int course);
u16 PokeathlonCourseRecord_GetScore(PokeathlonCourseRecordArgs *args, int course, int event);
int PokeathlonCourseRecord_GetEventId(PokeathlonCourseRecordArgs *args, s32 course, u32 event);
u16 PokeathlonCourseRecord_GetMonSpecies(PokeathlonCourseRecordArgs *args, u32 course, u32 slot);
void PokeathlonCourseRecord_InitBgs(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_FreeBgs(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_LoadBgGraphics(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_StartPageScroll(PokeathlonCourseRecord *work, int course, BOOL forward);
void PokeathlonCourseRecord_UpdatePageScroll(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_ShowCoursePanel(PokeathlonCourseRecord *work, u32 course);
void PokeathlonCourseRecord_PrintStaticText(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_PrintPage(PokeathlonCourseRecord *work, u32 course);
void PokeathlonCourseRecord_PrintEventScores(PokeathlonCourseRecord *work, int course);
void PokeathlonCourseRecord_PrintMonNames(PokeathlonCourseRecord *work, BOOL show);
void PokeathlonCourseRecord_PrintCourseName(PokeathlonCourseRecord *work, int course);
void PokeathlonCourseRecord_PrintTotalLabel(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_VBlankCallback(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_LoadSpriteResources(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_LoadMonPicResources(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_CreateSprites(PokeathlonCourseRecord *work);
void PokeathlonCourseRecord_SetMonSpritesVisible(PokeathlonCourseRecord *work, BOOL visible);
void PokeathlonCourseRecord_UpdatePageSprites(PokeathlonCourseRecord *work, int course);
BOOL PokeathlonCourseRecord_HandleInput(PokeathlonCourseRecord *work);

// Trophies app (PokeathlonTrophies_Init / PokeathlonTrophies_Main / PokeathlonTrophies_Exit)

typedef struct PokeathlonTrophiesSummary {
    PlayerProfile *profile;
    u16 courseRecordTotal;
    u16 eventRecordTotal;
    u16 totalPoints;
    u16 medalTotal;
    u16 courseScores[5];
    u8 medalistCount; // species that won all five medals
    u8 potentialTrophies;
    u8 gender;
} PokeathlonTrophiesSummary; // size: 0x1C

typedef struct PokeathlonTrophiesBgTemplates {
    BgTemplate templates[4];
} PokeathlonTrophiesBgTemplates;

typedef struct PokeathlonTrophies {
    PokeathlonTrophiesSummary *summary;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    PokeathlonTextContext *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[67];
} PokeathlonTrophies; // size: 0x124

void PokeathlonTrophies_VBlankCallback(PokeathlonTrophies *work);
void PokeathlonTrophies_InitBgs(PokeathlonTrophies *work);
void PokeathlonTrophies_LoadBgGraphics(PokeathlonTrophies *work);
void PokeathlonTrophies_FreeBgs(PokeathlonTrophies *work);
void PokeathlonTrophies_LoadSpriteResources(PokeathlonTrophies *work);
void PokeathlonTrophies_CreatePlayerSprite(PokeathlonTrophies *work);
void PokeathlonTrophies_CreateNumberSprites(PokeathlonTrophies *work, s32 value, u32 index, u32 count, s16 x, s16 y, u8 spacing, BOOL mainScreen);
void PokeathlonTrophies_PrintTitle(PokeathlonTrophies *work, u32 trophyCount);
void PokeathlonTrophies_PrintNextFriendshipTrophy(PokeathlonTrophies *work, u32 friendshipTrophies);
void PokeathlonTrophies_DrawScreen(PokeathlonTrophies *work);
u16 PokeathlonTrophies_CalcEventRecordTotal(PokeathlonSave *pokeathlon);
u16 PokeathlonTrophies_CountMedals(const u8 *medals);
void PokeathlonTrophies_CreateSummary(PokeathlonTrophies *work, PokeathlonTrophiesArgs *args);
void PokeathlonTrophies_FreeSummary(PokeathlonTrophiesSummary *summary);
u8 PokeathlonTrophies_GetPotentialTrophyCount(PokeathlonTrophiesSummary *summary);
u8 PokeathlonTrophies_GetMedalistCount(PokeathlonTrophiesSummary *summary);
u8 PokeathlonTrophies_GetGender(PokeathlonTrophiesSummary *summary);
PlayerProfile *PokeathlonTrophies_GetProfile(PokeathlonTrophiesSummary *summary);
u16 PokeathlonTrophies_GetMedalTotal(PokeathlonTrophiesSummary *summary);
u16 PokeathlonTrophies_GetEventRecordTotal(PokeathlonTrophiesSummary *summary);
u16 PokeathlonTrophies_GetCourseRecordTotal(PokeathlonTrophiesSummary *summary);
u16 PokeathlonTrophies_GetTotalPoints(PokeathlonTrophiesSummary *summary);
u16 PokeathlonTrophies_GetCourseScore(PokeathlonTrophiesSummary *summary, int course);
u8 PokeathlonTrophies_CountFriendshipTrophies(int totalPoints);
u8 PokeathlonTrophies_CountCollectiveTrophies(PokeathlonTrophies *work);

// Medals app

typedef struct PokeathlonMedalsFlags {
    u32 maxPage : 5;
    u32 speciesCount : 9;
    u32 page : 5;
    u32 cursor : 8; // 0-29: species icon, 30: return button
    u32 scrollBackward : 1;
    u32 scrolling : 1;
    u32 refreshSpecies : 1;
    u32 unused : 2;
} PokeathlonMedalsFlags;

typedef struct PokeathlonMedalsBgTemplates {
    BgTemplate templates[7];
} PokeathlonMedalsBgTemplates;

typedef struct PokeathlonMedals {
    BgConfig *bgConfig;
    NARC *narc;
    NARC *zukanNarc;
    enum HeapID heapId;
    PokeathlonMedalsArgs *args;
    u16 species[MAX_SPECIES + 1]; // species with at least one medal, in Pokedex order
    u16 *speciesOrder;
    PokeathlonMedalsFlags flags;
    u32 scrollFrame : 8;
    u32 unused_3F8 : 24;
    GridInputHandler *grid;
    PokeathlonTextContext *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[50];
} PokeathlonMedals; // size: 0x4D0

BOOL PokeathlonMedals_GetSpeciesOrderFileIndex(void);
BOOL PokeathlonMedals_AreFlags0EFTo0F1Set(PokeathlonMedals *work);
u8 PokeathlonMedals_GetPageSpeciesCount(PokeathlonMedals *work);
u8 PokeathlonMedals_GetSpeciesMedals(PokeathlonMedals *work, int species);
u32 PokeathlonMedals_CountMedalists(PokeathlonMedals *work);
BOOL PokeathlonMedals_HasAllMedals(PokeathlonMedals *work, int species);
u16 PokeathlonMedals_GetSelectedSpecies(PokeathlonMedals *work);
Pokedex *PokeathlonMedals_GetPokedex(PokeathlonMedals *work);
int PokeathlonMedals_GetSeenForm(PokeathlonMedals *work, u16 species);
int PokeathlonMedals_GetSeenGender(PokeathlonMedals *work, u16 species);
u32 PokeathlonMedals_GetSpindaPersonality(PokeathlonMedals *work);
u8 PokeathlonMedals_CountTrustTrophies(PokeathlonMedals *work);
void PokeathlonMedals_ShowSpeciesMedals(PokeathlonMedals *work, u16 species);
u16 *PokeathlonMedals_LoadSpeciesOrder(PokeathlonMedals *work, u32 *count);
void PokeathlonMedals_BuildSpeciesList(PokeathlonMedals *work);
void PokeathlonMedals_DrawMedalistTiles(PokeathlonMedals *work, s16 x, s16 y);
void PokeathlonMedals_DrawMedalistMark(PokeathlonMedals *work, int slot, int species);
void PokeathlonMedals_SetIconsVisible(PokeathlonMedals *work, BOOL visible);
void PokeathlonMedals_ShowPage(PokeathlonMedals *work);
void PokeathlonMedals_UpdatePageIndicator(PokeathlonMedals *work);
BOOL PokeathlonMedals_UpdatePageScroll(PokeathlonMedals *work);
void PokeathlonMedals_LoadMonPic(PokeathlonMedals *work, u16 species);
void PokeathlonMedals_PrintSpeciesName(PokeathlonMedals *work, u16 species);
void PokeathlonMedals_ShowSelectedSpecies(PokeathlonMedals *work);
void PokeathlonMedals_HideSelectedSpecies(PokeathlonMedals *work);
void PokeathlonMedals_VBlankCallback(PokeathlonMedals *work);
void PokeathlonMedals_CreateGridInput(PokeathlonMedals *work);
void PokeathlonMedals_FreeGridInput(PokeathlonMedals *work);
void PokeathlonMedals_ChangePage(PokeathlonMedals *work, BOOL forward, BOOL touch);
BOOL PokeathlonMedals_HandleInput(PokeathlonMedals *work);
void PokeathlonMedals_MoveCursor(PokeathlonMedals *work, u32 target);
void PokeathlonMedals_OnButton(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnSwitchToTouchMode(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnKeyMove(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnTouch(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnCursorMoved(PokeathlonMedals *work, u32 newTarget, int prevTarget, BOOL touch);
void PokeathlonMedals_InitBgs(PokeathlonMedals *work);
void PokeathlonMedals_LoadBgGraphics(PokeathlonMedals *work);
void PokeathlonMedals_FreeBgs(PokeathlonMedals *work);
void PokeathlonMedals_CreateSprites(PokeathlonMedals *work);
void PokeathlonMedals_LoadMonPicResources(PokeathlonMedals *work);
void PokeathlonMedals_LoadSpriteResources(PokeathlonMedals *work);
void PokeathlonMedals_CreateUiSprites(PokeathlonMedals *work);
void PokeathlonMedals_CreateMedalSprites(PokeathlonMedals *work);
void PokeathlonMedals_CreateIconSprites(PokeathlonMedals *work);
void PokeathlonMedals_PrintStaticText(PokeathlonMedals *work);

// Event record app

typedef struct PokeathlonEventRecordTeam {
    u16 mons[3]; // bits 0-9: species, bits 10-14: form
} PokeathlonEventRecordTeam;

typedef struct PokeathlonEventRecordEntry {
    u16 score;
    PokeathlonEventRecordTeam team;
} PokeathlonEventRecordEntry;

// Same layout as the start of Pokeathlon_RecordsSolo / Pokeathlon_RecordsLink.
typedef struct PokeathlonEventRecords {
    PokeathlonEventRecordEntry entries[5];
    u32 timesJoined;
} PokeathlonEventRecords; // size: 0x2C

typedef struct PokeathlonEventRecordDefaultEntry {
    u16 unk0;
    u16 unk2;
    PokeathlonEventRecordTeam team;
} PokeathlonEventRecordDefaultEntry;

typedef struct PokeathlonEventRecordData {
    PokeathlonEventRecordArgs *args;
    PokeathlonEventRecords records[10]; // solo records with the trophy target inserted at its rank
    String *string;
} PokeathlonEventRecordData; // size: 0x1C0

typedef struct PokeathlonEventRecordBgTemplates {
    BgTemplate templates[6];
} PokeathlonEventRecordBgTemplates;

typedef struct PokeathlonEventRecord {
    PokeathlonEventRecordData *data;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    PokeathlonTextContext *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[37];
    s8 cursor; // 0-9: event, -1: single/link mode button, 10: return button
    s8 lastColumn;
    BOOL linkMode;
    BOOL modeSwitchRequested;
    BOOL eventChanged;
    BOOL noRecord[10];
} PokeathlonEventRecord; // size: 0xE4

u16 PokeathlonEventRecord_GetEventTotal(PokeathlonEventRecordData *data, u32 event);
BOOL PokeathlonEventRecord_RecordReachesThreshold(PokeathlonEventRecord *work, int event);
BOOL PokeathlonEventRecord_SoloRecordReachesThreshold(PokeathlonEventRecord *work, s8 event, u32 column);
BOOL PokeathlonEventRecord_IsRankEmpty(PokeathlonEventRecord *work, u8 rank);
void PokeathlonEventRecord_VBlankCallback(PokeathlonEventRecord *work);
void PokeathlonEventRecord_InitBgs(PokeathlonEventRecord *work);
void PokeathlonEventRecord_LoadBgGraphics(PokeathlonEventRecord *work);
void PokeathlonEventRecord_DrawEventIcon(PokeathlonEventRecord *work, u8 event, s16 x, s16 y, BOOL grayed, BOOL subScreen);
void PokeathlonEventRecord_FreeBgs(PokeathlonEventRecord *work);
void PokeathlonEventRecord_ShowEvent(PokeathlonEventRecord *work);
void PokeathlonEventRecord_LoadModePalette(PokeathlonEventRecord *work);
void PokeathlonEventRecord_DrawModeScreen(PokeathlonEventRecord *work);
void PokeathlonEventRecord_SwitchMode(PokeathlonEventRecord *work);
void PokeathlonEventRecord_PrintReturnButton(PokeathlonEventRecord *work);
void PokeathlonEventRecord_PrintRecordValue(PokeathlonEventRecord *work, int value, int window, int unused, int y);
void PokeathlonEventRecord_PrintModeTitle(PokeathlonEventRecord *work);
void PokeathlonEventRecord_PrintEventRecord(PokeathlonEventRecord *work);
void PokeathlonEventRecord_CreateSprites(PokeathlonEventRecord *work);
void PokeathlonEventRecord_ShowEventRecordMons(PokeathlonEventRecord *work);
void PokeathlonEventRecord_LoadSpriteResources(PokeathlonEventRecord *work);
void PokeathlonEventRecord_CreateUiSprites(PokeathlonEventRecord *work);
void PokeathlonEventRecord_CreateEventSprites(PokeathlonEventRecord *work);
void PokeathlonEventRecord_CreateMonIconSprites(PokeathlonEventRecord *work);
void PokeathlonEventRecord_ShowRecordMonIcons(PokeathlonEventRecord *work);
void PokeathlonEventRecord_SetMonIconsVisible(PokeathlonEventRecord *work, BOOL visible);
void PokeathlonEventRecord_ClearNameWindows(PokeathlonEventRecord *work);
void PokeathlonEventRecord_UpdateEventGrid(PokeathlonEventRecord *work);
BOOL PokeathlonEventRecord_HandleInput(PokeathlonEventRecord *work);
void PokeathlonEventRecord_CopyRecords(PokeathlonEventRecord *work, const PokeathlonEventRecords *src, PokeathlonEventRecords *dst);
void PokeathlonEventRecord_CreateRecordData(PokeathlonEventRecord *work, PokeathlonEventRecordArgs *args);
void PokeathlonEventRecord_FreeRecordData(PokeathlonEventRecordData *data);
PokeathlonEventRecords *PokeathlonEventRecord_GetRecords(PokeathlonEventRecordData *data, u32 event, BOOL link);
BOOL PokeathlonEventRecord_AreFlags0EFTo0F1Set(PokeathlonEventRecordData *data);
void PokeathlonEventRecord_GetRecordMon(PokeathlonEventRecordData *data, PokeathlonMonInfo *mon, int event, int rank, int slot, BOOL link);
u16 PokeathlonEventRecord_GetRecordScore(PokeathlonEventRecordData *data, int event, u8 rank, BOOL link);
u32 PokeathlonEventRecord_GetTimesJoined(PokeathlonEventRecordData *data, u32 event, BOOL link);
PlayerProfile *PokeathlonEventRecord_GetProfile(PokeathlonEventRecordData *data);
String *PokeathlonEventRecord_GetLinkPlayerName(PokeathlonEventRecordData *data, int event, int rank);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H
