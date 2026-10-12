#ifndef POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H
#define POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "unk_02088288.h"

// Pokemon summary screen work (0x7D8 bytes). Only the parts the summary text code uses are typed so far.

// The shown Pokemon, read once from the Pokemon or box Pokemon (sub_0208981C).
typedef struct PokemonSummaryMon {
    String *speciesName;  // 0x00
    String *nickname;     // 0x04
    String *otName;       // 0x08
    u16 species;          // 0x0C
    u16 heldItem;         // 0x0E
    u8 type1;             // 0x10
    u8 type2;             // 0x11
    u8 level : 7;         // 0x12
    u8 noPrintGender : 1;
    u8 gender : 2; // 0x13
    u8 pokeball : 6;
    u32 otId;             // 0x14
    u32 exp;              // 0x18
    u32 curLevelExp;      // 0x1C
    u32 nextLevelExp;     // 0x20
    u16 hp;               // 0x24
    u16 maxHp;            // 0x26
    u16 attack;           // 0x28
    u16 defense;          // 0x2A
    u16 spAttack;         // 0x2C
    u16 spDefense;        // 0x2E
    u16 speed;            // 0x30
    u8 ability;           // 0x32
    u8 nature;            // 0x33
    u16 moves[4];         // 0x34
    u8 pp[4];             // 0x3C
    u8 maxPP[4];          // 0x40
    u8 otGender;          // 0x44
    u8 contestStats[6];   // 0x45: cool, beauty, cute, smart, tough, sheen
    u8 favoriteFlavor;    // 0x4B: 5 = none
    u16 markings;         // 0x4C
    u16 form;             // 0x4E
    u32 status : 28;      // 0x50
    u32 isEgg : 1;
    u32 shiny : 1;
    u32 pokerus : 2;
    u32 ribbons[4];       // 0x54: one bit per ribbon
    u8 shinyLeaves[6];    // 0x64
} PokemonSummaryMon;

typedef struct PokemonSummary {
    BgConfig *bgConfig;                 // 0x000
    Window windows[34];                 // 0x004
    Window *pageWindows;                // 0x224: the current page's windows
    u32 numPageWindows;                 // 0x228
    PokemonSummaryArgs *args;           // 0x22C
    PokemonSummaryMon mon;              // 0x230
    u8 unk29C[0x79C - 0x29C];           // 0x29C
    MessagePrinter *numberPrinter;      // 0x79C
    MsgData *msgData;                   // 0x7A0
    MsgData *ribbonMsgData;             // 0x7A4
    MessageFormat *msgFormat;           // 0x7A8
    String *buffer;                     // 0x7AC
    String *playerName;                 // 0x7B0
    MsgData *moveNames;                 // 0x7B4
    u8 unk7B8[4];                       // 0x7B8
    s8 page;                            // 0x7BC
    u8 moveSlot : 4;                    // 0x7BD: the two move slots being swapped
    u8 swapMoveSlot : 4;
    u8 unk7BE[6];                       // 0x7BE
    u8 ribbonCursor;                    // 0x7C4
    u8 ribbonRow;                       // 0x7C5
    u8 ribbonCount;                     // 0x7C6
    u8 ribbonId;                        // 0x7C7
    u8 unk7C8[0x7D8 - 0x7C8];           // 0x7C8
} PokemonSummary; // size: 0x7D8

// pokemon_summary_text.c
void PokemonSummary_AddWindows(PokemonSummary *summary);
void PokemonSummary_AddPageWindows(PokemonSummary *summary);
void PokemonSummary_RemovePageWindows(PokemonSummary *summary);
void PokemonSummary_RemoveWindows(PokemonSummary *summary);
void PokemonSummary_PrintNickname(PokemonSummary *summary);
void PokemonSummary_PrintLevel(PokemonSummary *summary);
void PokemonSummary_PrintHeldItem(PokemonSummary *summary);
int PokemonSummary_IsPlayersMon(PokemonSummary *summary);
void PokemonSummary_PrintBuffer(PokemonSummary *summary, Window *window, u32 color, int alignment);
void PokemonSummary_PrintStatLabel(PokemonSummary *summary, int window, int msgNo, int stat, int alignment);
void PokemonSummary_PrintMessage(PokemonSummary *summary, int window, int msgNo, u32 color, int alignment);
void PokemonSummary_FormatNumber(PokemonSummary *summary, int msgNo, int value, int digits, u8 mode);
void PokemonSummary_PrintValuePair(PokemonSummary *summary, int window, int labelMsg, int firstMsg, int secondMsg, u16 firstValue, u16 secondValue, volatile u8 digits, u8 centerX, u8 y);
void PokemonSummary_PrintLabels(PokemonSummary *summary);
void PokemonSummary_PrintPage(PokemonSummary *summary);
void PokemonSummary_PrintInfo(PokemonSummary *summary);
void PokemonSummary_PrintTrainerMemo(Window *window, void *mon, int isMine);
int PokemonSummary_CountShinyLeaves(PokemonSummary *summary);
void PokemonSummary_PrintMemo(PokemonSummary *summary);
void PokemonSummary_PrintStats(PokemonSummary *summary);
void PokemonSummary_PrintMoves(PokemonSummary *summary);
void sub_0208D520(PokemonSummary *summary);
void PokemonSummary_PrintRibbonCount(PokemonSummary *summary);
void PokemonSummary_PrintFlavorPreference(PokemonSummary *summary);
void PokemonSummary_PrintRibbonInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveSlot(PokemonSummary *summary, int slot);
void PokemonSummary_PrintMoveInfo(PokemonSummary *summary, u16 move);
void PokemonSummary_ClearMoveInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveToLearn(PokemonSummary *summary);
void sub_0208DB58(PokemonSummary *summary);
void PokemonSummary_PrintSwappedMoves(PokemonSummary *summary);
void sub_0208DBF0(PokemonSummary *summary);
void sub_0208DC68(PokemonSummary *summary, int show);
void sub_0208DD20(PokemonSummary *summary, int show);
void sub_0208DDA0(PokemonSummary *summary, int result);

#endif // POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H
