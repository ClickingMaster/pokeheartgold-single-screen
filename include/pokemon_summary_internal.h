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
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02088288.h"

// Pokemon summary screen work (0x7D8 bytes), used by src/pokemon_summary*.c.

#define POKEMON_SUMMARY_SPRITE_COUNT 115

// The shown Pokemon, read once from the Pokemon or box Pokemon (PokemonSummary_ReadMon).
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
    void *camera;                       // 0x29C
    PokepicManager *pokepicManager;     // 0x2A0
    u8 unk2A4[0x2CC - 0x2A4];           // 0x2A4
    void *pokepicAnimManager;           // 0x2CC
    Pokepic *pokepic;                   // 0x2D0
    u8 unk2D4[0x3FC - 0x2D4];           // 0x2D4
    SpriteSystem *spriteSystem;         // 0x3FC
    SpriteManager *spriteManager;       // 0x400
    Sprite *sprites[POKEMON_SUMMARY_SPRITE_COUNT]; // 0x404: see pokemon_summary_sprites.c for what each one shows
    u8 unk5D0[0x79C - 0x5D0];           // 0x5D0
    MessagePrinter *numberPrinter;      // 0x79C
    MsgData *msgData;                   // 0x7A0
    MsgData *ribbonMsgData;             // 0x7A4
    MessageFormat *msgFormat;           // 0x7A8
    String *buffer;                     // 0x7AC
    String *playerName;                 // 0x7B0
    MsgData *moveNames;                 // 0x7B4
    NARC *pokepicAnimNarc;              // 0x7B8
    s8 page;                            // 0x7BC
    u8 moveSlot : 4;                    // 0x7BD: the two move slots being swapped
    u8 swapMoveSlot : 4;
    u8 unk7BE;                          // 0x7BE: condition stats that changed (bits 0-4); the button flash step
    u8 unk7BF_0 : 4;                    // 0x7BF
    u8 exitRequested : 4;               // 1: leave the screen
    u8 sparkleState;                    // 0x7C0: sheen sparkles
    u8 numSparkles;                     // 0x7C1
    u8 unk7C2;                          // 0x7C2
    u8 unk7C3;                          // 0x7C3
    u8 ribbonCursor;                    // 0x7C4
    u8 ribbonRow;                       // 0x7C5
    u8 ribbonCount;                     // 0x7C6
    u8 ribbonId;                        // 0x7C7
    u8 flashX;                          // 0x7C8: the touched button being flashed (PokemonSummary_FlashButton)
    u8 flashY;                          // 0x7C9
    u8 flashWidth;                      // 0x7CA
    u8 flashHeight;                     // 0x7CB
    u8 flashPalette : 4;                // 0x7CC
    u8 restorePalette : 4;
    u8 flashBgId;                       // 0x7CD
    u8 flashTimer;                      // 0x7CE
    u8 flashNextState;                  // 0x7CF
    BgCellManager *cellManager;         // 0x7D0
    u32 boxIconSprite;                  // 0x7D4: which of sprites 73/74 shows the box Pokemon's icon next
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
void PokemonSummary_PrintPokeathlonStatLabels(PokemonSummary *summary);
void PokemonSummary_PrintRibbonCount(PokemonSummary *summary);
void PokemonSummary_PrintFlavorPreference(PokemonSummary *summary);
void PokemonSummary_PrintRibbonInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveSlot(PokemonSummary *summary, int slot);
void PokemonSummary_PrintMoveInfo(PokemonSummary *summary, u16 move);
void PokemonSummary_ClearMoveInfo(PokemonSummary *summary);
void PokemonSummary_PrintMoveToLearn(PokemonSummary *summary);
void PokemonSummary_ScheduleMainBg1Transfer(PokemonSummary *summary);
void PokemonSummary_PrintSwappedMoves(PokemonSummary *summary);
void PokemonSummary_PrintCantForgetHMMessage(PokemonSummary *summary);
void PokemonSummary_ShowForgetLabel(PokemonSummary *summary, int show);
void PokemonSummary_ShowSwitchLabel(PokemonSummary *summary, int show);
void PokemonSummary_PrintConditionChangeMessage(PokemonSummary *summary, int result);

// pokemon_summary_states.c
int PokemonSummary_HandleMoveSelectInput(PokemonSummary *summary);
int PokemonSummary_StartMoveSwap(PokemonSummary *summary);
int PokemonSummary_HandleMoveSwapInput(PokemonSummary *summary);
int PokemonSummary_HandleForgetMoveInput(PokemonSummary *summary);
int PokemonSummary_HandleForgetMoveConfirmInput(PokemonSummary *summary);
int PokemonSummary_OpenRibbonList(PokemonSummary *summary);
int PokemonSummary_CloseRibbonList(PokemonSummary *summary);
int PokemonSummary_HandleRibbonInput(PokemonSummary *summary);
int PokemonSummary_WaitRibbonPageUp(PokemonSummary *summary);
int PokemonSummary_WaitRibbonPageDown(PokemonSummary *summary);
int PokemonSummary_UpdateCondition(PokemonSummary *summary);
int PokemonSummary_ShowConditionChanges(PokemonSummary *summary);
int PokemonSummary_WaitConditionExit(PokemonSummary *summary);
int PokemonSummary_StartFadeOut(PokemonSummary *summary);
int PokemonSummary_WaitFadeOut(PokemonSummary *summary);
int PokemonSummary_FlashButton(PokemonSummary *summary);
int PokemonSummary_ChangeMonByButton(PokemonSummary *summary);
void PokemonSummary_ReadShownMon(PokemonSummary *summary);
void PokemonSummary_ReadBoxMon(PokemonSummary *summary, BoxPokemon *boxMon, PokemonSummaryMon *out);
void PokemonSummary_ReadMon(PokemonSummary *summary, Pokemon *mon, PokemonSummaryMon *out);
void PokemonSummary_PlayCry(PokemonSummary *summary);
void PokemonSummary_ShowFirstPage(PokemonSummary *summary);
void PokemonSummary_SetPage(PokemonSummary *summary, int page);
void PokemonSummary_ChangePage(PokemonSummary *summary, int page);
void PokemonSummary_ScrollPage(PokemonSummary *summary, int direction);

// pokemon_summary_ui.c
BOOL PokemonSummary_HasPage(PokemonSummary *summary, int page);
int PokemonSummary_HandleTouchInput(PokemonSummary *summary);
void PokemonSummary_LoadBgScreen(PokemonSummary *summary, BgConfig *bgConfig, int member, int bgId);
void PokemonSummary_LoadPageBgs(PokemonSummary *summary);
void PokemonSummary_DrawHpBar(PokemonSummary *summary);
void PokemonSummary_DrawExpBar(PokemonSummary *summary);
void PokemonSummary_ShowMon(PokemonSummary *summary, u8 slot);
void PokemonSummary_ChangeMon(PokemonSummary *summary, int direction);
int PokemonSummary_FindNextMon(PokemonSummary *summary, int direction);
BOOL PokemonSummary_CanShowMonAt(PokemonSummary *summary, int slot);
BOOL PokemonSummary_CanShowEgg(PokemonSummary *summary);
int PokemonSummary_FindNextMonInArray(PokemonSummary *summary, int direction);
int PokemonSummary_FindNextPartyMon(PokemonSummary *summary, int direction);
int PokemonSummary_FindNextBoxMon(PokemonSummary *summary, int direction);
void *PokemonSummary_GetShownMon(PokemonSummary *summary); // Pokemon or BoxPokemon (args->unk11 == 2)
BOOL PokemonSummary_SlideInMoveInfo(PokemonSummary *summary);
BOOL PokemonSummary_SlideOutMoveInfo(PokemonSummary *summary);
BOOL PokemonSummary_MoveMoveCursor(PokemonSummary *summary, int direction);
void PokemonSummary_ShowSelectedMoveInfo(PokemonSummary *summary);
void PokemonSummary_ShowMoveInfo(PokemonSummary *summary, int move);
void PokemonSummary_SwapMoves(PokemonSummary *summary);
void PokemonSummary_InitForgetMove(PokemonSummary *summary);
void PokemonSummary_UpdateForgetMovePage(PokemonSummary *summary);
BOOL PokemonSummary_SlideInRibbonList(PokemonSummary *summary);
BOOL PokemonSummary_SlideOutRibbonList(PokemonSummary *summary);
void PokemonSummary_MoveRibbonCursor(PokemonSummary *summary, int direction);
u8 PokemonSummary_GetRibbonAt(PokemonSummary *summary, u8 cell);
void PokemonSummary_SetMenuInputState(PokemonSummary *summary, int state);
int PokemonSummary_GetTouchedMove(PokemonSummary *summary);
int PokemonSummary_GetMoveSwapTouch(PokemonSummary *summary);
int PokemonSummary_GetMoveSelectTouch(PokemonSummary *summary);
int PokemonSummary_GetForgetMoveTouch(PokemonSummary *summary);
int PokemonSummary_GetForgetConfirmTouch(PokemonSummary *summary);
int PokemonSummary_GetMonButtonTouch(PokemonSummary *summary);
int PokemonSummary_GetRibbonTouch(PokemonSummary *summary);
int PokemonSummary_ShowForgetConfirm(PokemonSummary *summary);
void PokemonSummary_DrawForgetConfirm(PokemonSummary *summary, int show);
void PokemonSummary_ClearForgetButton(PokemonSummary *summary);
void PokemonSummary_DrawSwitchButton(PokemonSummary *summary, int value);
int PokemonSummary_StartButtonFlash(PokemonSummary *summary, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u8 restorePalette, u8 nextState);
int PokemonSummary_FlashCancelButton(PokemonSummary *summary, u8 nextState);
int PokemonSummary_FlashConfirmButton(PokemonSummary *summary, u8 nextState);
int PokemonSummary_FlashSwapButton(PokemonSummary *summary, u8 nextState);
int PokemonSummary_FlashMonButton(PokemonSummary *summary, int which, u8 nextState);
int PokemonSummary_FlashRibbonCancelButton(PokemonSummary *summary, u8 nextState);
void PokemonSummary_DrawMonButtons(PokemonSummary *summary);

// pokemon_summary.c
BOOL PokemonSummary_IsPokeathlonUnlocked(SaveData *saveData);
BOOL PokemonSummary_Init(OverlayManager *manager, int *state);
BOOL PokemonSummary_Main(OverlayManager *manager, int *state);
BOOL PokemonSummary_Exit(OverlayManager *manager, int *state);
void PokemonSummary_VBlank(void *data);
void PokemonSummary_SetGraphicsBanks(void);
void PokemonSummary_InitBgs(BgConfig *bgConfig);
void PokemonSummary_FreeBgs(BgConfig *bgConfig);
void PokemonSummary_LoadBgGraphics(PokemonSummary *summary, NARC *unused, NARC *narc);
void PokemonSummary_SetBlend(void);
void PokemonSummary_InitText(PokemonSummary *summary);
void PokemonSummary_FreeText(PokemonSummary *summary);
void PokemonSummary_InitBgCells(PokemonSummary *summary, NARC *narc);
void PokemonSummary_FreeBgCells(PokemonSummary *summary);
int PokemonSummary_WaitFadeIn(PokemonSummary *summary);
int PokemonSummary_HandleInput(PokemonSummary *summary);
int PokemonSummary_WaitSlideInMoveInfo(PokemonSummary *summary);
int PokemonSummary_WaitSlideOutMoveInfo(PokemonSummary *summary);

// pokemon_summary_sprites.c
void PokemonSummary_InitSpriteSystem(PokemonSummary *summary);
void PokemonSummary_FreeSpriteSystem(PokemonSummary *summary);
void PokemonSummary_UpdateSpriteAnims(PokemonSummary *summary);
void PokemonSummary_CreateSprites(PokemonSummary *summary);
void PokemonSummary_UpdatePartySlotSprite(PokemonSummary *summary);
void sub_0208B448(PokemonSummary *summary);
void PokemonSummary_LoadBallSprite(PokemonSummary *summary);
void PokemonSummary_UpdateStatusSprite(PokemonSummary *summary);
void PokemonSummary_LoadTypeAndIconSprites(PokemonSummary *summary);
void PokemonSummary_UpdateTypeSprites(PokemonSummary *summary);
void PokemonSummary_HidePokeathlonStars(PokemonSummary *summary);
void PokemonSummary_InitShinyLeafSprites(PokemonSummary *summary);
void PokemonSummary_SetPokeathlonStatStars(Sprite **sprites, int firstStar, int maxStars, int baseStars, int color, int modifier, int modifierSprite);
int PokemonSummary_GetPokeathlonStarStates(u8 *states, int maxStars, int baseStars, u8 color, int modifier);
void PokemonSummary_UpdatePokeathlonStars(PokemonSummary *summary);
void PokemonSummary_UpdateShinyLeafSprites(PokemonSummary *summary);
void PokemonSummary_SetMonTypeSprites(PokemonSummary *summary);
void PokemonSummary_SetTypeSprite(PokemonSummary *summary, u8 sprite, u8 resId, u8 type);
void PokemonSummary_FindMoveTypeSprite(PokemonSummary *summary, u8 *slot, s16 *x, s16 *y);
void PokemonSummary_SwapMoveTypeSprites(PokemonSummary *summary, u8 slotA, u8 slotB, int unused);
void PokemonSummary_SetMoveCategorySprite(PokemonSummary *summary, u16 move);
void PokemonSummary_UpdateMoveCursorSprite(PokemonSummary *summary);
void PokemonSummary_ShowSwapCursorSprite(PokemonSummary *summary);
void PokemonSummary_ResetSwapCursorSprite(PokemonSummary *summary);
void PokemonSummary_InitSheenSparkles(PokemonSummary *summary);
void PokemonSummary_UpdateMarkingSprites(PokemonSummary *summary);
void PokemonSummary_ShowConditionChangeSprites(PokemonSummary *summary);
void PokemonSummary_LoadMonIcon(PokemonSummary *summary, void *mon, int sprite);
void PokemonSummary_LoadShownMonIcon(PokemonSummary *summary);
void PokemonSummary_LoadPartyIcons(PokemonSummary *summary);
void PokemonSummary_SetBoxMonIconPos(PokemonSummary *summary);
void PokemonSummary_AlternateBoxMonIcon(PokemonSummary *summary);
void PokemonSummary_ShowMonIconSprite(PokemonSummary *summary);
s16 PokemonSummary_GetConditionPoint(int stat, s16 tip, s16 center);
void PokemonSummary_ShowConditionSprites(PokemonSummary *summary);
void PokemonSummary_HideConditionSprites(PokemonSummary *summary);
void PokemonSummary_SetRibbonSprite(PokemonSummary *summary, int ribbon, int index);
void PokemonSummary_ShowRibbonSprites(PokemonSummary *summary);
void PokemonSummary_UpdateRibbonSprites(PokemonSummary *summary);
void PokemonSummary_UpdateRibbonCursorSprite(PokemonSummary *summary);
void sub_0208C3C0(PokemonSummary *summary);

#endif // POKEHEARTGOLD_POKEMON_SUMMARY_INTERNAL_H
