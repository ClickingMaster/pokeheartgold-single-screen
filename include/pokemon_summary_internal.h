#ifndef POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H
#define POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H

#include "global.h"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pokemon_summary.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"

// Pokemon summary screen work.

#define POKEMON_SUMMARY_SPRITE_COUNT 115

enum PokemonSummaryState {
    POKEMON_SUMMARY_STATE_START_FADE_IN,
    POKEMON_SUMMARY_STATE_WAIT_FADE_IN,
    POKEMON_SUMMARY_STATE_INPUT,
    POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO,
    POKEMON_SUMMARY_STATE_SLIDE_OUT_MOVE_INFO,
    POKEMON_SUMMARY_STATE_MOVE_SELECT_INPUT,
    POKEMON_SUMMARY_STATE_MOVE_SWAP_INPUT,
    POKEMON_SUMMARY_STATE_START_MOVE_SWAP,
    POKEMON_SUMMARY_STATE_FORGET_MOVE_INPUT,
    POKEMON_SUMMARY_STATE_FORGET_MOVE_CONFIRM_INPUT,
    POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST,
    POKEMON_SUMMARY_STATE_CLOSE_RIBBON_LIST,
    POKEMON_SUMMARY_STATE_RIBBON_INPUT,
    POKEMON_SUMMARY_STATE_RIBBON_PAGE_UP,
    POKEMON_SUMMARY_STATE_RIBBON_PAGE_DOWN,
    POKEMON_SUMMARY_STATE_FLASH_BUTTON,
    POKEMON_SUMMARY_STATE_UPDATE_CONDITION,
    POKEMON_SUMMARY_STATE_SHOW_CONDITION_CHANGES,
    POKEMON_SUMMARY_STATE_WAIT_CONDITION_EXIT,
    POKEMON_SUMMARY_STATE_SHOW_POKEPIC,
    POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON,
    POKEMON_SUMMARY_STATE_START_FADE_OUT,
    POKEMON_SUMMARY_STATE_WAIT_FADE_OUT,
};

// The shown Pokemon, read once from the Pokemon or box Pokemon (PokemonSummary_ReadMon).
typedef struct PokemonSummaryMon {
    String *speciesName; // 0x00
    String *nickname;    // 0x04
    String *otName;      // 0x08
    u16 species;         // 0x0C
    u16 heldItem;        // 0x0E
    u8 type1;            // 0x10
    u8 type2;            // 0x11
    u8 level : 7;        // 0x12
    u8 noPrintGender : 1;
    u8 gender : 2; // 0x13
    u8 pokeball : 6;
    u32 otId;           // 0x14
    u32 exp;            // 0x18
    u32 curLevelExp;    // 0x1C
    u32 nextLevelExp;   // 0x20
    u16 hp;             // 0x24
    u16 maxHp;          // 0x26
    u16 attack;         // 0x28
    u16 defense;        // 0x2A
    u16 spAttack;       // 0x2C
    u16 spDefense;      // 0x2E
    u16 speed;          // 0x30
    u8 ability;         // 0x32
    u8 nature;          // 0x33
    u16 moves[4];       // 0x34
    u8 pp[4];           // 0x3C
    u8 maxPP[4];        // 0x40
    u8 otGender;        // 0x44
    u8 contestStats[6]; // 0x45: cool, beauty, cute, smart, tough, sheen
    u8 favoriteFlavor;  // 0x4B: 5 = none
    u16 markings;       // 0x4C
    u16 form;           // 0x4E
    u32 status : 28;    // 0x50
    u32 isEgg : 1;
    u32 shiny : 1;
    u32 pokerus : 2;
    u32 ribbons[4];    // 0x54: one bit per ribbon
    u8 shinyLeaves[6]; // 0x64
} PokemonSummaryMon;

typedef struct PokemonSummary {
    BgConfig *bgConfig;                            // 0x000
    Window windows[34];                            // 0x004
    Window *pageWindows;                           // 0x224: the current page's windows
    u32 numPageWindows;                            // 0x228
    PokemonSummaryArgs *args;                      // 0x22C
    PokemonSummaryMon mon;                         // 0x230
    void *camera;                                  // 0x29C
    PokepicManager *pokepicManager;                // 0x2A0
    u8 unk2A4[0x2CC - 0x2A4];                      // 0x2A4
    void *pokepicAnimManager;                      // 0x2CC
    Pokepic *pokepic;                              // 0x2D0
    u8 unk2D4[0x3FC - 0x2D4];                      // 0x2D4
    SpriteSystem *spriteSystem;                    // 0x3FC
    SpriteManager *spriteManager;                  // 0x400
    Sprite *sprites[POKEMON_SUMMARY_SPRITE_COUNT]; // 0x404
    u8 unk5D0[0x79C - 0x5D0];                      // 0x5D0
    MessagePrinter *numberPrinter;                 // 0x79C
    MsgData *msgData;                              // 0x7A0
    MsgData *ribbonMsgData;                        // 0x7A4
    MessageFormat *msgFormat;                      // 0x7A8
    String *buffer;                                // 0x7AC
    String *playerName;                            // 0x7B0
    MsgData *moveNames;                            // 0x7B4
    NARC *pokepicAnimNarc;                         // 0x7B8
    s8 page;                                       // 0x7BC
    u8 moveSlot : 4;                               // 0x7BD: the two move slots being swapped
    u8 swapMoveSlot : 4;
    u8 unk7BE;            // 0x7BE: condition stats that changed (bits 0-4); the button flash step
    u8 unk7BF_0 : 4;      // 0x7BF
    u8 exitRequested : 4; // 1: leave the screen
    u8 sparkleState;      // 0x7C0: sheen sparkles
    u8 numSparkles;       // 0x7C1
    u8 unk7C2;            // 0x7C2
    u8 unk7C3;            // 0x7C3
    u8 ribbonCursor;      // 0x7C4
    u8 ribbonRow;         // 0x7C5
    u8 ribbonCount;       // 0x7C6
    u8 ribbonId;          // 0x7C7
    u8 flashX;            // 0x7C8: the touched button being flashed (PokemonSummary_FlashButton)
    u8 flashY;            // 0x7C9
    u8 flashWidth;        // 0x7CA
    u8 flashHeight;       // 0x7CB
    u8 flashPalette : 4;  // 0x7CC
    u8 restorePalette : 4;
    u8 flashBgId;               // 0x7CD
    u8 flashTimer;              // 0x7CE
    u8 flashNextState;          // 0x7CF
    BgCellManager *cellManager; // 0x7D0
    u32 boxIconSprite;          // 0x7D4: which of sprites 73/74 shows the box Pokemon's icon next
} PokemonSummary;               // size: 0x7D8

void PokemonSummary_AddWindows(PokemonSummary *summary);
void PokemonSummary_AddPageWindows(PokemonSummary *summary);
void PokemonSummary_RemovePageWindows(PokemonSummary *summary);
void PokemonSummary_RemoveWindows(PokemonSummary *summary);
void PokemonSummary_PrintNickname(PokemonSummary *summary);
void PokemonSummary_PrintLevel(PokemonSummary *summary);
void PokemonSummary_PrintHeldItem(PokemonSummary *summary);
void PokemonSummary_PrintPage(PokemonSummary *summary);
void PokemonSummary_PrintRibbonInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveInfo(PokemonSummary *summary, u16 move);
void PokemonSummary_ClearMoveInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveToLearn(PokemonSummary *summary);
void PokemonSummary_ScheduleMainBg1Transfer(PokemonSummary *summary);
void PokemonSummary_PrintSwappedMoves(PokemonSummary *summary);
void PokemonSummary_PrintCantForgetHMMessage(PokemonSummary *summary);
void PokemonSummary_ShowForgetLabel(PokemonSummary *summary, int show);
void PokemonSummary_ShowSwitchLabel(PokemonSummary *summary, int show);
void PokemonSummary_PrintConditionChangeMessage(PokemonSummary *summary, int result);

int PokemonSummary_FlashButton(PokemonSummary *summary);
void PokemonSummary_ReadMon(PokemonSummary *summary, Pokemon *mon, PokemonSummaryMon *out);

void *PokemonSummary_GetShownMon(PokemonSummary *summary); // Pokemon or BoxPokemon (args->unk11 == 2)

BOOL PokemonSummary_IsPokeathlonUnlocked(SaveData *saveData);
BOOL PokemonSummary_Init(OverlayManager *manager, int *state);
BOOL PokemonSummary_Main(OverlayManager *manager, int *state);
BOOL PokemonSummary_Exit(OverlayManager *manager, int *state);

int PokemonSummary_GetPokeathlonStarStates(u8 *states, int maxStars, int baseStars, int color, int modifier);

#endif // POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H
