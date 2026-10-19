#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H

#include "global.h"

#include "constants/species.h"

#include "pokeathlon/pokeathlon_util.h"

#include "bg_window.h"
#include "filesystem.h"
#include "grid_input_handler.h"
#include "overlay_99.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "touchscreen.h"

// Pokeathlon record screens: course records, trophies, medals and event records.

// Main steps of the record screens
enum PokeathlonRecordScreenState {
    RECORD_SCREEN_STATE_FADE_IN,
    RECORD_SCREEN_STATE_WAIT_FADE_IN,
    RECORD_SCREEN_STATE_INPUT,
    RECORD_SCREEN_STATE_WAIT_FADE_OUT,
    RECORD_SCREEN_STATE_EXIT,
};

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
} PokeathlonCourseRecordEntry;

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
} PokeathlonCourseRecord;

// Trophies app

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
} PokeathlonTrophiesSummary;

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
} PokeathlonTrophies;

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
} PokeathlonMedals;

void PokeathlonMedals_OnButton(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnSwitchToTouchMode(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnKeyMove(PokeathlonMedals *work, int newTarget, int prevTarget);
void PokeathlonMedals_OnTouch(PokeathlonMedals *work, int newTarget, int prevTarget);

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
} PokeathlonEventRecords;

typedef struct PokeathlonEventRecordDefaultEntry {
    u16 unk0;
    u16 unk2;
    PokeathlonEventRecordTeam team;
} PokeathlonEventRecordDefaultEntry;

typedef struct PokeathlonEventRecordData {
    PokeathlonEventRecordArgs *args;
    PokeathlonEventRecords records[10]; // solo records with the trophy target inserted at its rank
    String *string;
} PokeathlonEventRecordData;

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
} PokeathlonEventRecord;

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RECORD_SCREENS_H
