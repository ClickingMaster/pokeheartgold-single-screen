#include "global.h"

#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "frontier/battle_castle.h"
#include "frontier/overlay_80_022340E8.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0031.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "message_printer.h"
#include "move.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "render_window.h"
#include "save.h"
#include "save_frontier.h"
#include "sound_playback.h"
#include "sprite.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen_list_menu.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "oam_manager.h"
#include "unk_0200CE7C.h"
#include "unk_0200FA24.h"
#include "unk_02019BA4.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_0203A3B0.h"
#include "unk_0205BFF0.h"
#include "unk_0208805C.h"
#include "unk_02096910.h"
#include "use_item_on_mon.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_80);

// Battle Castle party app (overlay 83, first half): manage your own party with
// Castle Points (HEAL / RENTAL / RESTORE HP / RANK UP). Texts: msg_0031.

// Main loop states (*state of Main)
#define CASTLE_PARTY_MAIN_FADE_IN     0
#define CASTLE_PARTY_MAIN_MENU        1
#define CASTLE_PARTY_MAIN_LINK_ACTION 2
#define CASTLE_PARTY_MAIN_LINK_EXIT   3
#define CASTLE_PARTY_MAIN_FADE_OUT    4

// Menu states (app->subState in BattleCastlePartyApp_HandleMenu)
#define CASTLE_PARTY_STATE_SELECT_MON           0
#define CASTLE_PARTY_STATE_MAIN_MENU            1
#define CASTLE_PARTY_STATE_HEAL_MENU            2
#define CASTLE_PARTY_STATE_CONFIRM_HEAL         3
#define CASTLE_PARTY_STATE_CONFIRM_HEAL_RANK_UP 4
#define CASTLE_PARTY_STATE_HEAL_RANK_UP         5
#define CASTLE_PARTY_STATE_HEAL_RANK_UP_DONE    6
#define CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE    7
#define CASTLE_PARTY_STATE_RENTAL_MENU          8
#define CASTLE_PARTY_STATE_SELECT_ITEM          9
#define CASTLE_PARTY_STATE_ITEM_BUTTON          10
#define CASTLE_PARTY_STATE_CONFIRM_RENTAL       11
#define CASTLE_PARTY_STATE_CONFIRM_RENT_RANK_UP 12
#define CASTLE_PARTY_STATE_RENTAL_RANK_UP       13
#define CASTLE_PARTY_STATE_RENTAL_RANK_UP_DONE  14
#define CASTLE_PARTY_STATE_RENTAL_MENU_MESSAGE  15
#define CASTLE_PARTY_STATE_ITEM_MESSAGE         16
#define CASTLE_PARTY_STATE_HELD_ITEM_MESSAGE    17
#define CASTLE_PARTY_STATE_CONFIRM_SWAP_ITEM    18
#define CASTLE_PARTY_STATE_APPLY_ACTION         19
#define CASTLE_PARTY_STATE_ACTION_DONE          20
#define CASTLE_PARTY_STATE_STATS_SUMMARY        21
#define CASTLE_PARTY_STATE_MOVES_SUMMARY        22

// List menu item values / app->action
#define CASTLE_PARTY_ACTION_HEAL           0
#define CASTLE_PARTY_ACTION_RESTORE_HP     1
#define CASTLE_PARTY_ACTION_RESTORE_PP     2
#define CASTLE_PARTY_ACTION_RESTORE_ALL    3
#define CASTLE_PARTY_ACTION_HEAL_RANK_UP   4
#define CASTLE_PARTY_ACTION_RENTAL         5
#define CASTLE_PARTY_ACTION_RENT_BERRY     6
#define CASTLE_PARTY_ACTION_RENT_ITEM      7
#define CASTLE_PARTY_ACTION_RENTAL_RANK_UP 8
#define CASTLE_PARTY_ACTION_STATS          9
#define CASTLE_PARTY_ACTION_MOVES          10
#define CASTLE_PARTY_ACTION_CANCEL         11

// Party grid: positions 0-3 are the party, then the EXIT button
#define CASTLE_PARTY_GRID_EXIT 4

// Rental item grid: 6 items per page, then the page arrows and RETURN
#define CASTLE_ITEMS_PER_PAGE      6
#define CASTLE_ITEM_GRID_PREV_PAGE 6
#define CASTLE_ITEM_GRID_NEXT_PAGE 7
#define CASTLE_ITEM_GRID_RETURN    8

// BattleCastle_GetRank kinds
#define CASTLE_RANK_HEAL   0
#define CASTLE_RANK_RENTAL 1
#define CASTLE_RANK_MAX    3

// BattleCastlePartyApp_SendLinkCommand link commands
#define CASTLE_LINK_CMD_START  10
#define CASTLE_LINK_CMD_ACTION 11
#define CASTLE_LINK_CMD_CURSOR 12
#define CASTLE_LINK_CMD_EXIT   13

// sub_02037AC0 / sub_02037B38 link sync ids
#define CASTLE_PARTY_SYNC_START  215
#define CASTLE_PARTY_SYNC_ACTION 130
#define CASTLE_PARTY_SYNC_EXIT   131

#define CASTLE_ITEM_GRID_NUM_ITEMS   27 // rentable held items
#define CASTLE_ITEM_GRID_NUM_BERRIES 32 // rentable berries

// app->listCursorPos before the list menu reported a position (never equals a real one)
#define CASTLE_PARTY_LIST_POS_NONE (-2)

// Party panel layout (windows[0] / windows[2]): one 64 px column per mon,
// shifted right by 32 px when only the player's own three mons are shown.
#define CASTLE_PARTY_COLUMN_WIDTH 64

// Windows (app->windows)
#define CASTLE_PARTY_WIN_POINTS  0
#define CASTLE_PARTY_WIN_MESSAGE 6
// Summary panel of the selected mon (labels on the even windows 20-38, values next to them)
#define CASTLE_PARTY_WIN_SUMMARY_FIRST 18
#define CASTLE_PARTY_WIN_NICKNAME      18
#define CASTLE_PARTY_WIN_GENDER        19
#define CASTLE_PARTY_WIN_LEVEL_LABEL   20
#define CASTLE_PARTY_WIN_LEVEL         21
#define CASTLE_PARTY_WIN_ABILITY_LABEL 22
#define CASTLE_PARTY_WIN_ABILITY       23
#define CASTLE_PARTY_WIN_NATURE_LABEL  24
#define CASTLE_PARTY_WIN_NATURE        25
#define CASTLE_PARTY_WIN_ITEM_LABEL    26
#define CASTLE_PARTY_WIN_ITEM          27
#define CASTLE_PARTY_WIN_HP_LABEL      28
#define CASTLE_PARTY_WIN_HP            29
#define CASTLE_PARTY_WIN_ATTACK_LABEL  30
#define CASTLE_PARTY_WIN_ATTACK        31
#define CASTLE_PARTY_WIN_SP_ATK_LABEL  32
#define CASTLE_PARTY_WIN_SP_ATK        33
#define CASTLE_PARTY_WIN_DEFENSE_LABEL 34
#define CASTLE_PARTY_WIN_DEFENSE       35
#define CASTLE_PARTY_WIN_SP_DEF_LABEL  36
#define CASTLE_PARTY_WIN_SP_DEF        37
#define CASTLE_PARTY_WIN_SPEED_LABEL   38
#define CASTLE_PARTY_WIN_SPEED         39
#define CASTLE_PARTY_WIN_MOVE_NAMES    40 // 40-43
#define CASTLE_PARTY_WIN_MOVE_PP       44 // 44-47
#define CASTLE_PARTY_WIN_SUMMARY_LAST  47
// Rental item grid
#define CASTLE_PARTY_WIN_ITEM_GRID        48 // 48-53, one per item button
#define CASTLE_PARTY_WIN_ITEM_NAME        54
#define CASTLE_PARTY_WIN_ITEM_COST        55
#define CASTLE_PARTY_WIN_PAGE_NUMBER      56
#define CASTLE_PARTY_WIN_RETURN           57
#define CASTLE_PARTY_WIN_GRID_TITLE       58
#define CASTLE_PARTY_WIN_GRID_PLAYER_NAME 59
#define CASTLE_PARTY_WIN_GRID_POINTS      60
#define CASTLE_PARTY_WIN_ITEM_DESC        61
// Selected mon's info panel next to the item grid (62-69)
#define CASTLE_PARTY_WIN_MON_NICKNAME   62
#define CASTLE_PARTY_WIN_MON_GENDER     63
#define CASTLE_PARTY_WIN_MON_LV         64
#define CASTLE_PARTY_WIN_MON_LEVEL      65
#define CASTLE_PARTY_WIN_MON_HP_LABEL   66
#define CASTLE_PARTY_WIN_MON_HP         67
#define CASTLE_PARTY_WIN_MON_ITEM_LABEL 68
#define CASTLE_PARTY_WIN_MON_ITEM       69

#define CASTLE_PARTY_TEXT_COLOR           MAKE_TEXT_COLOR(1, 2, 0)
#define CASTLE_PARTY_TEXT_COLOR_MALE      MAKE_TEXT_COLOR(5, 6, 0) // male symbol
#define CASTLE_PARTY_TEXT_COLOR_FEMALE    MAKE_TEXT_COLOR(3, 4, 0) // female symbol and names
#define CASTLE_PARTY_TEXT_COLOR_MALE_NAME MAKE_TEXT_COLOR(7, 8, 0)

// BattleCastlePartyApp_PrintExpandedMessage / BattleCastle_PrintString / BattleCastle_PrintMsg alignment
#define CASTLE_ALIGN_LEFT   0
#define CASTLE_ALIGN_RIGHT  1
#define CASTLE_ALIGN_CENTER 2

// Members of the open NARC (app->narc)
#define CASTLE_PARTY_NARC_ITEM_GRID_SCREEN   38
#define CASTLE_PARTY_NARC_ITEM_DETAIL_SCREEN 39

typedef struct BattleCastlePartyAppArgs {
    SaveData *saveData; // 0x00
    u8 frontierType;    // 0x04
    u8 unk_05[0x13];    // 0x05
    Party *party;       // 0x18
    u8 unk_1C[4];       // 0x1C
    u16 selection;      // 0x20
    u8 unk_22[6];       // 0x22
    u16 partnerPoints;  // 0x28
} BattleCastlePartyAppArgs;

// Entry of the HEAL / RENTAL list menus
typedef struct BattleCastleMenuOption {
    u32 minRank; // required rank for RESTORE / rental options
    int msgId;
    int value;
} BattleCastleMenuOption;

// Entry of the main (HEAL / RENTAL / CANCEL) list menu
typedef struct BattleCastleMenuItem {
    int msgId;
    int value;
} BattleCastleMenuItem;

// Tables still defined in the overlay's data asm
extern const u16 _02247D0C[];         // heal result messages, by action - 1; bytes 0x1E.. hold the list menu template
extern const u16 ov83_02247D12[];     // number of rentable berries, by rental rank - 1
extern const u16 ov83_02247D18[];     // CP cost of RESTORE HP / PP / ALL
extern const u16 ov83_02247D1E[];     // main menu help messages, by cursor position
extern const u16 ov83_02247D24[];     // number of rentable items, by rental rank - 1
extern const u16 ov83_02247D48[2][3]; // CP cost of the next rank, by rank kind (CASTLE_RANK_*) and rank
extern const u16 ov83_02247D4E[];     // = ov83_02247D48[CASTLE_RANK_RENTAL] (referenced by its own label)
extern const u16 ov83_02247D5A[2][3]; // "gained the ability to..." messages, by rank kind and rank
extern const BattleCastleMenuItem ov83_02247D6C[];
extern const BattleCastleMenuOption ov83_02247EB0[]; // BERRIES / ITEMS / RANK UP / CANCEL
extern const u16 ov83_02247EE0[];                    // rentable items
extern const u16 ov83_02247F16[];                    // item costs in CP
extern const BattleCastleMenuOption ov83_02247F4C[]; // RESTORE HP / PP / ALL / RANK UP / CANCEL
extern const u16 ov83_02247F88[];                    // rentable berries
extern const u16 ov83_02247FC8[];                    // berry costs in CP
extern const u8 ov83_02248008[];                     // list menu y, by number of entries
extern const GraphicsModes ov83_02247D38;
extern const BgTemplate ov83_02247D84;
extern const BgTemplate ov83_02247DA0;
extern const BgTemplate ov83_02247DBC;
extern const BgTemplate ov83_02247DD8;
extern const BgTemplate ov83_02247DF4;
extern const BgTemplate ov83_02247E10;
extern const BgTemplate ov83_02247E2C;
extern const BgTemplate ov83_02247E48;
extern const GraphicsBanks ov83_02247E88;

// Frontier overlay (80) helpers
u32 ov80_0222A43C(u16 hp, u16 maxHp);
void ov80_0222A7CC(MessageFormat *msgFormat, int value);
int ov80_02237B24(u8 frontierType, int kind);
void ov80_02237FA4(FrontierSave *frontierSave, int frontierType, int points);

// Entry points and link receive callbacks (their names are referenced from asm)
BOOL BattleCastlePartyApp_Init(OverlayManager *manager, int *state);
int BattleCastlePartyApp_Main(OverlayManager *manager, int *state);
BOOL BattleCastlePartyApp_Exit(OverlayManager *manager);
void ov83_0223FAF0(void);
void BattleCastlePartyApp_ReceiveRanks(int netId, void *unused, void *data, BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveAction(int netId, int unused, u16 *data, BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveCursor(int netId, int unused, u16 *data, BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReceiveExit(int netId, int unused, u16 *data, BattleCastlePartyApp *app);

BOOL BattleCastlePartyApp_FadeIn(BattleCastlePartyApp *app);
void BattleCastlePartyApp_DrawScreen(BattleCastlePartyApp *app);
BOOL BattleCastlePartyApp_HandleMenu(BattleCastlePartyApp *app);
BOOL BattleCastlePartyApp_LinkAction(BattleCastlePartyApp *app);
BOOL BattleCastlePartyApp_LinkExit(BattleCastlePartyApp *app);
BOOL BattleCastlePartyApp_FadeOut(BattleCastlePartyApp *app);
void BattleCastlePartyApp_FreeResources(BattleCastlePartyApp *app);
void BattleCastlePartyApp_DisableDisplay(void);
void BattleCastlePartyApp_InitGraphics(BattleCastlePartyApp *app);
void BattleCastlePartyApp_InitBgGraphics(BattleCastlePartyApp *app);
void BattleCastlePartyApp_InitSpriteGfx(BattleCastlePartyApp *app);
void BattleCastlePartyApp_FreeBgLayers(BgConfig *bgConfig);
void BattleCastlePartyApp_VBlankCallback(void *arg);
void BattleCastlePartyApp_SetGraphicsBanks(void);
void BattleCastlePartyApp_InitBgLayers(BgConfig *bgConfig);
void BattleCastlePartyApp_LoadPartyBg(BattleCastlePartyApp *app, GFBgLayer bgId);
void BattleCastlePartyApp_LoadBgPalette(void);
void BattleCastlePartyApp_LoadStatsSummaryBg(BattleCastlePartyApp *app, GFBgLayer bgId);
void BattleCastlePartyApp_LoadMovesSummaryBg(BattleCastlePartyApp *app, GFBgLayer bgId);
void BattleCastlePartyApp_LoadItemGridBg(BattleCastlePartyApp *app);
void BattleCastlePartyApp_LoadSubScreenBg(BattleCastlePartyApp *app, GFBgLayer bgId);
int BattleCastlePartyApp_PrintMessage(BattleCastlePartyApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId);
int BattleCastlePartyApp_AddMessage(BattleCastlePartyApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId);
int BattleCastlePartyApp_ShowMessage(BattleCastlePartyApp *app, int msgNo, u8 fontId);
void BattleCastlePartyApp_PrintStatsSummary(BattleCastlePartyApp *app, Window *window, Pokemon *mon);
int BattleCastlePartyApp_PrintSummaryText(BattleCastlePartyApp *app, Window *window, int msgNo, int x, u16 y);
void BattleCastlePartyApp_PrintMovesSummary(BattleCastlePartyApp *app, Window *window, Pokemon *mon);
void BattleCastlePartyApp_PrintMove(BattleCastlePartyApp *app, Window *window, int moveSlot, int msgNo, int ppMsgNo, Pokemon *mon, int moveField, int ppField, int maxPpField);
void BattleCastlePartyApp_PrintPartyHp(BattleCastlePartyApp *app, Window *window);
void BattleCastlePartyApp_PrintMonHp(BattleCastlePartyApp *app, Window *window, int slot, int mode);
void BattleCastlePartyApp_PrintPartyLevels(BattleCastlePartyApp *app, Window *window);
void BattleCastlePartyApp_PrintMonLevelAndGender(BattleCastlePartyApp *app, Window *window, u32 slot, u8 mode);
void BattleCastlePartyApp_RedrawMonHp(BattleCastlePartyApp *app, Window *window, int slot, int hp);
void BattleCastlePartyApp_PrintChoosePrompt(BattleCastlePartyApp *app);
void BattleCastlePartyApp_HideChoosePrompt(BattleCastlePartyApp *app);
void BattleCastlePartyApp_OpenMainMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_CloseMainMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_OpenHealMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_CloseHealMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_OpenRentalMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_CloseRentalMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_OpenItemGrid(BattleCastlePartyApp *app, int mode);
void BattleCastlePartyApp_CloseItemGrid(BattleCastlePartyApp *app);
void BattleCastlePartyApp_ShowYesNoPrompt(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintItemGridHeader(BattleCastlePartyApp *app, int mode);
void BattleCastlePartyApp_PrintItemNames(BattleCastlePartyApp *app);
void BattleCastlePartyApp_DrawItemIcons(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintItemPageNumber(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintReturnButton(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintItemDescription(BattleCastlePartyApp *app, u32 item);
void BattleCastlePartyApp_PrintSelectedMonInfo(BattleCastlePartyApp *app);
void BattleCastlePartyApp_SetupItemGrid(BattleCastlePartyApp *app, int mode);
void BattleCastlePartyApp_BufferNumber(BattleCastlePartyApp *app, int field, int value, int digits, int mode);
void BattleCastlePartyApp_BufferMonNickname(BattleCastlePartyApp *app, int field, BoxPokemon *boxMon);
void BattleCastlePartyApp_BufferPlayerName(BattleCastlePartyApp *app, int field);
void BattleCastlePartyApp_PrintPlayerName(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId);
void BattleCastlePartyApp_PrintPartnerName(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId);
void BattleCastlePartyApp_PrintGenderSymbol(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId, u8 gender);
void BattleCastlePartyApp_SetMainState(BattleCastlePartyApp *app, int *state, u32 newState);
void BattleCastlePartyApp_MoveCursor(BattleCastlePartyApp *app);
void BattleCastlePartyApp_DrawCursor(BattleCastlePartyApp *app, u32 index, int isPartner);
void BattleCastlePartyApp_GetCursorPos(BattleCastlePartyApp *app, int *x, int *y, u32 index);
u16 BattleCastlePartyApp_GetRentalCost(BattleCastlePartyApp *app, int index, int kind);
u16 BattleCastlePartyApp_GetRentalCostByItemId(u16 itemId);
u16 BattleCastlePartyApp_GetRentalItemId(BattleCastlePartyApp *app, int index, int kind);
void BattleCastlePartyApp_GetTrainerNamePositions(BattleCastlePartyApp *app, u16 *x1, u16 *y1, u16 *x2, u16 *y2);
int BattleCastlePartyApp_PlayActionEffect(BattleCastlePartyApp *app, u32 selected, u32 action);
int BattleCastlePartyApp_GetMonIconHpPalette(BattleCastlePartyApp *app, u32 hpBarColor);
int BattleCastlePartyApp_GetHpBarAnimSeq(BattleCastlePartyApp *app, u32 hpBarColor);
void BattleCastlePartyApp_ScrollStatsSummary(BattleCastlePartyApp *app, int delta);
void BattleCastlePartyApp_ScrollMovesSummary(BattleCastlePartyApp *app, int delta);
void BattleCastlePartyApp_CloseAll(BattleCastlePartyApp *app);
int BattleCastlePartyApp_IsAnyPpUsed(Pokemon *mon);
void BattleCastlePartyApp_CloseWindow(Window *window);
BOOL BattleCastlePartyApp_SendLinkCommand(BattleCastlePartyApp *app, int command, int param);
void BattleCastlePartyApp_PrepareLinkRanks(BattleCastlePartyApp *app, int command);
void BattleCastlePartyApp_PrepareLinkAction(BattleCastlePartyApp *app, int command, int slot);
void BattleCastlePartyApp_PrepareLinkCursor(BattleCastlePartyApp *app, int command);
void BattleCastlePartyApp_PrepareLinkExit(BattleCastlePartyApp *app, int command);
void BattleCastlePartyApp_UseItemOnMon(Pokemon *mon, u16 itemId);
void BattleCastlePartyApp_PrintTrainerNames(BattleCastlePartyApp *app, Window *window);
void BattleCastlePartyApp_HealMon(BattleCastlePartyApp *app, int slot, int action);
void BattleCastlePartyApp_GiveRentalItem(BattleCastlePartyApp *app, int slot, u16 itemId);
void BattleCastlePartyApp_CloseMenus(BattleCastlePartyApp *app);
void BattleCastlePartyApp_ReturnToItemList(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintCastlePoints(BattleCastlePartyApp *app, Window *window);
void BattleCastlePartyApp_ApplyAction(BattleCastlePartyApp *app, u32 slot, u8 action);
void BattleCastlePartyApp_ShowMonStats(BattleCastlePartyApp *app, u32 slot);
void BattleCastlePartyApp_ShowMonMoves(BattleCastlePartyApp *app, u32 slot);
void BattleCastlePartyApp_HideSummaryPanel(BattleCastlePartyApp *app);
void BattleCastlePartyApp_UpdateMonIcons(BattleCastlePartyApp *app);
void BattleCastlePartyApp_RankUp(BattleCastlePartyApp *app, u8 slot, u16 action);
void BattleCastlePartyApp_PrintExpandedMessage(BattleCastlePartyApp *app, Window *window, MsgData *msgData, int msgNo, int x, int y, int fontId, int color, int alignment);
void BattleCastlePartyApp_LoadMonSummary(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintSummaryLabels(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintMonSummary(BattleCastlePartyApp *app, int scheduleCopy);
void BattleCastlePartyApp_RefreshMonSummary(BattleCastlePartyApp *app, int scheduleCopy);
void BattleCastlePartyApp_ShowMonSummary(BattleCastlePartyApp *app, int show);
void BattleCastlePartyApp_GetItemButtonPos(u32 slot, s16 *x, s16 *y);
void BattleCastlePartyApp_ShowSelectedItem(BattleCastlePartyApp *app, u32 selected);
void BattleCastlePartyApp_ShowItemGrid(BattleCastlePartyApp *app);
void BattleCastlePartyApp_SetCursorPos(BattleCastlePartyApp *app, u32 cursorPos, u32 prevCursorPos);
void BattleCastlePartyApp_SetItemCursor(BattleCastlePartyApp *app, int x, int y, int seq);
void BattleCastlePartyApp_ShowItemDescription(BattleCastlePartyApp *app, int button);
void BattleCastlePartyApp_CreateListMenu(BattleCastlePartyApp *app, int count, u8 x, u8 y, u8 width);
void BattleCastlePartyApp_CreateMainMenuList(BattleCastlePartyApp *app);
void BattleCastlePartyApp_CreateHealMenuList(BattleCastlePartyApp *app);
void BattleCastlePartyApp_CreateRentalMenuList(BattleCastlePartyApp *app);
void BattleCastlePartyApp_DestroyListMenu(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintMainMenuHelp(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintHealMenuHelp(BattleCastlePartyApp *app);
void BattleCastlePartyApp_PrintRentalMenuHelp(BattleCastlePartyApp *app);
void BattleCastlePartyApp_StartButtonAnim(BattleCastlePartyApp *app, u16 button);

// BattleCastlePartyApp init
BOOL BattleCastlePartyApp_Init(OverlayManager *manager, int *state) {
    BattleCastlePartyApp *app;
    BattleCastlePartyAppArgs *args;
    int i;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleCastlePartyApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_107, 0x30000);
    app = OverlayManager_CreateAndGetData(manager, sizeof(BattleCastlePartyApp), HEAP_ID_107);
    memset(app, 0, sizeof(BattleCastlePartyApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_107);
    app->manager = manager;
    args = OverlayManager_GetArgs(manager);
    app->saveData = args->saveData;
    app->unk_510 = sub_02030CC8(app->saveData);
    app->unk_514 = sub_02030E08(app->saveData);
    app->frontierType = args->frontierType;
    app->selectionOut = &args->selection;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->party = args->party;
    app->unk_012 = 0xFF;
    app->partnerPoints = args->partnerPoints;
    app->frontierSave = Save_Frontier_GetStatic(app->saveData);
    for (i = 0; i < 3; i++) {
        app->partnerRanks[i] = 1;
    }
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        app->numMons = 3;
    } else {
        app->numMons = 4;
    }
    app->unk_015 = 4;
    app->prevCursorPos = app->unk_015 - 1;
    BattleCastle_ClearBgToggleRequests(&app->bgToggleRequests);
    BattleCastlePartyApp_InitGraphics(app);
    if (ov80_02237D8C(app->frontierType) == TRUE) {
        sub_02096910(app);
    }
    return TRUE;
}

#pragma force_active on
// BattleCastlePartyApp main
int BattleCastlePartyApp_Main(OverlayManager *manager, int *state) {
    BattleCastlePartyApp *app = OverlayManager_GetData(manager);

    if (app->unk_7FE == 1) {
        if (*state == CASTLE_PARTY_MAIN_MENU) {
            app->unk_7FE = 0;
            BattleCastlePartyApp_CloseAll(app);
            if (app->effectSprite != NULL) {
                BattleCastleSprite_Delete(app->effectSprite);
                app->flags.bits.effectActive = FALSE;
            }
            BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
            ov80_0222A7CC(app->msgFormat, 0);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00008, 1);
            BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_LINK_EXIT);
        }
    } else if (app->unk_012 != 0xFF && (*state == CASTLE_PARTY_MAIN_MENU || *state == CASTLE_PARTY_MAIN_LINK_EXIT)) {
        app->unk_7FE = 0;
        BattleCastlePartyApp_CloseAll(app);
        BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_LINK_ACTION);
    }

    switch (*state) {
    case CASTLE_PARTY_MAIN_FADE_IN:
        if (BattleCastlePartyApp_FadeIn(app) == TRUE) {
            BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_MENU);
        }
        break;
    case CASTLE_PARTY_MAIN_MENU:
        if (BattleCastlePartyApp_HandleMenu(app) == TRUE) {
            if (app->flags.bits.linkSyncPending == TRUE) {
                BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_LINK_ACTION);
            } else if (ov80_02237D8C(app->frontierType) == TRUE) {
                BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_LINK_EXIT);
            } else {
                BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_FADE_OUT);
            }
        }
        break;
    case CASTLE_PARTY_MAIN_LINK_ACTION:
        if (BattleCastlePartyApp_LinkAction(app) == TRUE) {
            BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_MENU);
        }
        break;
    case CASTLE_PARTY_MAIN_LINK_EXIT:
        if (BattleCastlePartyApp_LinkExit(app) == TRUE) {
            BattleCastlePartyApp_SetMainState(app, state, CASTLE_PARTY_MAIN_FADE_OUT);
        }
        break;
    case CASTLE_PARTY_MAIN_FADE_OUT:
        if (BattleCastlePartyApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    }
    BattleCastlePartyApp_UpdateMonIcons(app);
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    return FALSE;
}
#pragma force_active reset

// BattleCastlePartyApp exit
BOOL BattleCastlePartyApp_Exit(OverlayManager *manager) {
    BattleCastlePartyApp *app = OverlayManager_GetData(manager);

    *app->selectionOut = app->cursorPos;
    G2_BlendNone();
    GF_DestroyVramTransferManager();
    BattleCastlePartyApp_FreeResources(app);
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_107);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

#pragma force_active on
BOOL BattleCastlePartyApp_FadeIn(BattleCastlePartyApp *app) {
    switch (app->subState) {
    case 0:
        if (ov80_02237D8C(app->frontierType) == TRUE) {
            sub_02037BEC();
            sub_02037AC0(CASTLE_PARTY_SYNC_START);
        }
        app->subState++;
        break;
    case 1:
        if (ov80_02237D8C(app->frontierType) == TRUE) {
            if (sub_02037B38(CASTLE_PARTY_SYNC_START) == TRUE) {
                sub_02037BEC();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        if (ov80_02237D8C(app->frontierType) == TRUE) {
            if (BattleCastlePartyApp_SendLinkCommand(app, CASTLE_LINK_CMD_START, 0) == TRUE) {
                app->subState++;
            }
        } else {
            BattleCastlePartyApp_DrawScreen(app);
            BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_107);
            app->subState++;
        }
        break;
    case 3:
        if (ov80_02237D8C(app->frontierType) == TRUE) {
            if (app->linkRecvCount >= 2) {
                app->linkRecvCount = 0;
                BattleCastlePartyApp_DrawScreen(app);
                BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_107);
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

void BattleCastlePartyApp_DrawScreen(BattleCastlePartyApp *app) {
    Window *window = &app->windows[0];

    FillWindowPixelBuffer(window, 0);
    BattleCastlePartyApp_PrintTrainerNames(app, window);
    BattleCastlePartyApp_PrintCastlePoints(app, window);
    BattleCastlePartyApp_PrintPartyHp(app, &app->windows[3]);
    BattleCastlePartyApp_PrintPartyLevels(app, &app->windows[2]);
    BattleCastlePartyApp_PrintChoosePrompt(app);
    GfGfx_BothDispOn();
}

#pragma force_active on
BOOL BattleCastlePartyApp_HandleMenu(BattleCastlePartyApp *app) {
    u32 input;
    u32 rank;
    int cpStat;
    u32 points;
    Pokemon *mon;
    int resume;
    int healOption;

    switch (app->subState) {
    case CASTLE_PARTY_STATE_SELECT_MON:
        resume = app->flags.bits.linkResume;
        if (resume == 1) {
            BattleCastlePartyApp_OpenHealMenu(app);
            ov83_02247630(app->unk_778, 204, 100);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
            app->flags.bits.linkResume = 0;
            return FALSE;
        }
        if (resume == 2) {
            BattleCastlePartyApp_OpenRentalMenu(app);
            ov83_02247630(app->unk_778, 211, 106);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
            app->flags.bits.linkResume = 0;
            return FALSE;
        }
        input = BattleCastle_HandlePartyGridInput(app->partyGrid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
            BattleCastlePartyApp_HideChoosePrompt(app);
            BattleCastlePartyApp_OpenMainMenu(app);
            app->subState = CASTLE_PARTY_STATE_MAIN_MENU;
            break;
        case CASTLE_PARTY_GRID_EXIT:
            PlaySE(SEQ_SE_DP_SELECT);
            return TRUE;
        case GRID_MENU_CANCEL:
            if (app->cursorPos != app->unk_015) {
                ov83_02247B04(app->partyGrid);
                BattleCastlePartyApp_SetCursorPos(app, CASTLE_PARTY_GRID_EXIT, app->cursorPos);
            }
            break;
        }
        break;
    case CASTLE_PARTY_STATE_MAIN_MENU:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        BattleCastlePartyApp_PrintMainMenuHelp(app);
        switch (input) {
        case LIST_NOTHING_CHOSEN:
            break;
        case CASTLE_PARTY_ACTION_HEAL:
            BattleCastlePartyApp_CloseMainMenu(app);
            BattleCastlePartyApp_OpenHealMenu(app);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
            break;
        case CASTLE_PARTY_ACTION_RENTAL:
            BattleCastlePartyApp_CloseMainMenu(app);
            BattleCastlePartyApp_OpenRentalMenu(app);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
            break;
        case CASTLE_PARTY_ACTION_CANCEL:
        case LIST_CANCEL:
            BattleCastlePartyApp_CloseMainMenu(app);
            BattleCastlePartyApp_PrintChoosePrompt(app);
            app->subState = CASTLE_PARTY_STATE_SELECT_MON;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_HEAL_MENU:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        BattleCastlePartyApp_PrintHealMenuHelp(app);
        switch (input) {
        case LIST_NOTHING_CHOSEN:
            break;
        case CASTLE_PARTY_ACTION_RESTORE_HP:
        case CASTLE_PARTY_ACTION_RESTORE_PP:
        case CASTLE_PARTY_ACTION_RESTORE_ALL:
            app->action = input;
            BattleCastlePartyApp_CloseHealMenu(app);
            BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
            if (rank < ov83_02247F4C[app->listCursorPos].minRank) {
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00033, 1);
                app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                break;
            }
            BattleCastlePartyApp_BufferNumber(app, 0, ov83_02247D18[app->listCursorPos], 3, 0);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00055, 1);
            BattleCastlePartyApp_ShowYesNoPrompt(app);
            app->subState = CASTLE_PARTY_STATE_CONFIRM_HEAL;
            break;
        case CASTLE_PARTY_ACTION_HEAL_RANK_UP:
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
            if (rank == CASTLE_RANK_MAX) {
                StopSE(SEQ_SE_DP_SELECT, 0);
                PlaySE(SEQ_SE_DP_BOX03);
                break;
            }
            app->action = input;
            BattleCastlePartyApp_CloseHealMenu(app);
            cpStat = sub_0205C1F0(app->frontierType);
            points = FrontierSave_GetStat(app->frontierSave, cpStat, sub_0205C268(sub_0205C1F0(app->frontierType)));
            BattleCastlePartyApp_BufferNumber(app, 0, ov83_02247D48[CASTLE_RANK_HEAL][rank], 4, 0);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00038, 1);
            BattleCastlePartyApp_ShowYesNoPrompt(app);
            app->subState = CASTLE_PARTY_STATE_CONFIRM_HEAL_RANK_UP;
            break;
        case CASTLE_PARTY_ACTION_CANCEL:
        case LIST_CANCEL:
            BattleCastlePartyApp_CloseHealMenu(app);
            BattleCastlePartyApp_OpenMainMenu(app);
            app->subState = CASTLE_PARTY_STATE_MAIN_MENU;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_CONFIRM_HEAL:
        input = YesNoPrompt_HandleInput(app->yesNoPrompt.prompt);
        switch (input) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, app->cursorPos));
            cpStat = sub_0205C1F0(app->frontierType);
            points = FrontierSave_GetStat(app->frontierSave, cpStat, sub_0205C268(sub_0205C1F0(app->frontierType)));
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
            healOption = app->listCursorPos;
            if (rank < ov83_02247F4C[healOption].minRank) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00033, 1);
                app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                break;
            }
            if (points < ov83_02247D18[healOption]) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00032, 1);
                app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                break;
            }
            if (healOption == 0) {
                u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
                if (hp == GetMonData(mon, MON_DATA_MAX_HP, NULL)) {
                    app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00037, 1);
                    app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                    break;
                }
            } else if (healOption == 1) {
                if (!BattleCastlePartyApp_IsAnyPpUsed(mon)) {
                    app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00037, 1);
                    app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                    break;
                }
            } else {
                u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
                if (hp == GetMonData(mon, MON_DATA_MAX_HP, NULL) && !BattleCastlePartyApp_IsAnyPpUsed(mon)) {
                    app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00037, 1);
                    app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                    break;
                }
            }
            if (!ov80_02237D8C(app->frontierType)) {
                ov80_02237FA4(app->frontierSave, app->frontierType, ov83_02247D18[app->action - 1]);
                BattleCastlePartyApp_PrintCastlePoints(app, &app->windows[0]);
                BattleCastlePartyApp_HealMon(app, app->cursorPos, app->action);
                app->subState = CASTLE_PARTY_STATE_APPLY_ACTION;
            } else {
                app->flags.bits.linkSyncPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_OpenHealMenu(app);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_CONFIRM_HEAL_RANK_UP:
        input = YesNoPrompt_HandleInput(app->yesNoPrompt.prompt);
        switch (input) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            cpStat = sub_0205C1F0(app->frontierType);
            points = FrontierSave_GetStat(app->frontierSave, cpStat, sub_0205C268(sub_0205C1F0(app->frontierType)));
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
            if (points < ov83_02247D48[CASTLE_RANK_HEAL][rank]) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00041, 1);
                app->subState = CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE;
                break;
            }
            if (!ov80_02237D8C(app->frontierType)) {
                BattleCastlePartyApp_RankUp(app, app->cursorPos, CASTLE_PARTY_ACTION_HEAL_RANK_UP);
                app->subState = CASTLE_PARTY_STATE_HEAL_RANK_UP;
            } else {
                app->flags.bits.linkSyncPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_OpenHealMenu(app);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_HEAL_RANK_UP:
        if (BattleCastlePartyApp_PlayActionEffect(app, app->cursorPos, app->action) == TRUE) {
            app->subState = CASTLE_PARTY_STATE_HEAL_RANK_UP_DONE;
        }
        break;
    case CASTLE_PARTY_STATE_HEAL_RANK_UP_DONE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_OpenHealMenu(app);
            ov83_02247630(app->unk_778, 204, 100);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
        }
        break;
    case CASTLE_PARTY_STATE_HEAL_MENU_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
            BattleCastlePartyApp_OpenHealMenu(app);
            app->subState = CASTLE_PARTY_STATE_HEAL_MENU;
        }
        break;
    case CASTLE_PARTY_STATE_RENTAL_MENU:
        BattleCastle_RequestBgToggle(GF_BG_LYR_MAIN_2, 0, &app->bgToggleRequests);
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        BattleCastlePartyApp_PrintRentalMenuHelp(app);
        switch (input) {
        case LIST_NOTHING_CHOSEN:
            break;
        case LIST_CANCEL:
            BattleCastlePartyApp_CloseRentalMenu(app);
            BattleCastlePartyApp_OpenMainMenu(app);
            app->subState = CASTLE_PARTY_STATE_MAIN_MENU;
            break;
        case CASTLE_PARTY_ACTION_RENT_BERRY:
            app->action = input;
            BattleCastlePartyApp_CloseRentalMenu(app);
            BattleCastlePartyApp_OpenItemGrid(app, CASTLE_PARTY_ACTION_RENT_BERRY);
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        case CASTLE_PARTY_ACTION_RENT_ITEM:
            app->action = input;
            BattleCastlePartyApp_CloseRentalMenu(app);
            if (BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL) == 1) {
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00054, 1);
                app->subState = CASTLE_PARTY_STATE_RENTAL_MENU_MESSAGE;
                return FALSE;
            }
            BattleCastlePartyApp_OpenItemGrid(app, CASTLE_PARTY_ACTION_RENT_ITEM);
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        case CASTLE_PARTY_ACTION_RENTAL_RANK_UP:
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
            if (rank == CASTLE_RANK_MAX) {
                StopSE(SEQ_SE_DP_SELECT, 0);
                PlaySE(SEQ_SE_DP_BOX03);
                break;
            }
            app->action = input;
            BattleCastlePartyApp_CloseRentalMenu(app);
            BattleCastlePartyApp_BufferNumber(app, 0, ov83_02247D4E[rank], 4, 0);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00038, 1);
            BattleCastlePartyApp_ShowYesNoPrompt(app);
            app->subState = CASTLE_PARTY_STATE_CONFIRM_RENT_RANK_UP;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_SELECT_ITEM:
        input = BattleCastle_HandleItemGridInput(app->itemGrid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            app->unk_860 = input + app->itemPage * CASTLE_ITEMS_PER_PAGE;
            if (app->unk_860 < app->numItems) {
                PlaySE(SEQ_SE_DP_DECIDE);
                BattleCastlePartyApp_StartButtonAnim(app, input);
                app->subState = CASTLE_PARTY_STATE_ITEM_BUTTON;
            }
            break;
        case CASTLE_ITEM_GRID_PREV_PAGE:
            PlaySE(SEQ_SE_DP_SELECT78);
            BattleCastlePartyApp_StartButtonAnim(app, CASTLE_ITEM_GRID_PREV_PAGE);
            app->subState = CASTLE_PARTY_STATE_ITEM_BUTTON;
            break;
        case CASTLE_ITEM_GRID_NEXT_PAGE:
            PlaySE(SEQ_SE_DP_SELECT78);
            BattleCastlePartyApp_StartButtonAnim(app, CASTLE_ITEM_GRID_NEXT_PAGE);
            app->subState = CASTLE_PARTY_STATE_ITEM_BUTTON;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            break;
        case CASTLE_ITEM_GRID_RETURN:
        case GRID_MENU_CANCEL:
            PlaySE(SEQ_SE_DP_DECIDE);
            BattleCastlePartyApp_StartButtonAnim(app, CASTLE_ITEM_GRID_RETURN);
            app->subState = CASTLE_PARTY_STATE_ITEM_BUTTON;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_ITEM_BUTTON:
        if (BattleCastlePartyApp_AnimatePressedButton(app) == TRUE) {
            break;
        }
        input = app->pressedButton;
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            BattleCastlePartyApp_ShowSelectedItem(app, input);
            BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
            BattleCastlePartyApp_BufferNumber(app, 0, BattleCastlePartyApp_GetRentalCost(app, app->unk_860, app->action), 3, 0);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00055, 1);
            BattleCastlePartyApp_ShowYesNoPrompt(app);
            app->subState = CASTLE_PARTY_STATE_CONFIRM_RENTAL;
            break;
        case CASTLE_ITEM_GRID_PREV_PAGE:
            app->itemPage--;
            if (app->itemPage < 0) {
                app->itemPage = (app->numItems - 1) / CASTLE_ITEMS_PER_PAGE;
            }
            BattleCastlePartyApp_PrintItemNames(app);
            BattleCastlePartyApp_DrawItemIcons(app);
            BattleCastlePartyApp_PrintItemPageNumber(app);
            BattleCastlePartyApp_PrintItemDescription(app, GridInputHandler_GetNextInput(app->itemGrid));
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        case CASTLE_ITEM_GRID_NEXT_PAGE:
            app->itemPage++;
            if ((app->numItems - 1) / CASTLE_ITEMS_PER_PAGE < app->itemPage) {
                app->itemPage = 0;
            }
            BattleCastlePartyApp_PrintItemNames(app);
            BattleCastlePartyApp_DrawItemIcons(app);
            BattleCastlePartyApp_PrintItemPageNumber(app);
            BattleCastlePartyApp_PrintItemDescription(app, GridInputHandler_GetNextInput(app->itemGrid));
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        case CASTLE_ITEM_GRID_RETURN:
            BattleCastlePartyApp_CloseItemGrid(app);
            BattleCastlePartyApp_OpenRentalMenu(app);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_CONFIRM_RENTAL:
        input = YesNoPrompt_HandleInput(app->yesNoPrompt.prompt);
        switch (input) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            cpStat = sub_0205C1F0(app->frontierType);
            points = FrontierSave_GetStat(app->frontierSave, cpStat, sub_0205C268(sub_0205C1F0(app->frontierType)));
            if (points < BattleCastlePartyApp_GetRentalCost(app, app->unk_860, app->action)) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00032, 1);
                app->subState = CASTLE_PARTY_STATE_ITEM_MESSAGE;
                break;
            }
            mon = app->summary.mon;
            if (!GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) {
                if (!ov80_02237D8C(app->frontierType)) {
                    BattleCastlePartyApp_CloseItemGrid(app);
                    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                    ov80_02237FA4(app->frontierSave, app->frontierType, BattleCastlePartyApp_GetRentalCost(app, app->unk_860, app->action));
                    BattleCastlePartyApp_PrintCastlePoints(app, &app->windows[0]);
                    BattleCastlePartyApp_GiveRentalItem(app, app->cursorPos, BattleCastlePartyApp_GetRentalItemId(app, app->unk_860, app->action));
                    app->subState = CASTLE_PARTY_STATE_APPLY_ACTION;
                } else {
                    app->selectedItem = BattleCastlePartyApp_GetRentalItemId(app, app->unk_860, app->action);
                    BattleCastlePartyApp_CloseItemGrid(app);
                    BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
                    app->flags.bits.linkSyncPending = TRUE;
                    return TRUE;
                }
            } else {
                BattleCastlePartyApp_BufferMonNickname(app, 0, Mon_GetBoxMon(mon));
                BufferItemNameWithIndefArticle(app->msgFormat, 1, GetMonData(mon, MON_DATA_HELD_ITEM, NULL));
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00060, 1);
                app->subState = CASTLE_PARTY_STATE_HELD_ITEM_MESSAGE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_ReturnToItemList(app);
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_CONFIRM_RENT_RANK_UP:
        input = YesNoPrompt_HandleInput(app->yesNoPrompt.prompt);
        switch (input) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            cpStat = sub_0205C1F0(app->frontierType);
            points = FrontierSave_GetStat(app->frontierSave, cpStat, sub_0205C268(sub_0205C1F0(app->frontierType)));
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
            if (points < ov83_02247D4E[rank]) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00041, 1);
                app->subState = CASTLE_PARTY_STATE_RENTAL_MENU_MESSAGE;
                break;
            }
            if (!ov80_02237D8C(app->frontierType)) {
                BattleCastlePartyApp_RankUp(app, app->cursorPos, CASTLE_PARTY_ACTION_RENTAL_RANK_UP);
                app->subState = CASTLE_PARTY_STATE_RENTAL_RANK_UP;
            } else {
                app->flags.bits.linkSyncPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_OpenRentalMenu(app);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_RENTAL_RANK_UP:
        if (BattleCastlePartyApp_PlayActionEffect(app, app->cursorPos, app->action) == TRUE) {
            app->subState = CASTLE_PARTY_STATE_RENTAL_RANK_UP_DONE;
        }
        break;
    case CASTLE_PARTY_STATE_RENTAL_RANK_UP_DONE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_OpenRentalMenu(app);
            ov83_02247630(app->unk_778, 211, 106);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
        }
        break;
    case CASTLE_PARTY_STATE_RENTAL_MENU_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
            BattleCastlePartyApp_OpenRentalMenu(app);
            app->subState = CASTLE_PARTY_STATE_RENTAL_MENU;
        }
        break;
    case CASTLE_PARTY_STATE_ITEM_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_ReturnToItemList(app);
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
        }
        break;
    case CASTLE_PARTY_STATE_HELD_ITEM_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            app->printerId = BattleCastlePartyApp_ShowMessage(app, msg_0031_00061, 1);
            BattleCastlePartyApp_ShowYesNoPrompt(app);
            app->subState = CASTLE_PARTY_STATE_CONFIRM_SWAP_ITEM;
        }
        break;
    case CASTLE_PARTY_STATE_CONFIRM_SWAP_ITEM:
        input = YesNoPrompt_HandleInput(app->yesNoPrompt.prompt);
        switch (input) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_PrintItemNames(app);
            if (!ov80_02237D8C(app->frontierType)) {
                BattleCastlePartyApp_CloseItemGrid(app);
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
                ov80_02237FA4(app->frontierSave, app->frontierType, BattleCastlePartyApp_GetRentalCost(app, app->unk_860, app->action));
                BattleCastlePartyApp_PrintCastlePoints(app, &app->windows[0]);
                BattleCastlePartyApp_GiveRentalItem(app, app->cursorPos, BattleCastlePartyApp_GetRentalItemId(app, app->unk_860, app->action));
                app->subState = CASTLE_PARTY_STATE_APPLY_ACTION;
            } else {
                app->selectedItem = BattleCastlePartyApp_GetRentalItemId(app, app->unk_860, app->action);
                BattleCastlePartyApp_CloseItemGrid(app);
                BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
                app->flags.bits.linkSyncPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastlePartyApp_ReturnToItemList(app);
            app->subState = CASTLE_PARTY_STATE_SELECT_ITEM;
            break;
        }
        break;
    case CASTLE_PARTY_STATE_APPLY_ACTION:
        BattleCastle_RequestBgToggle(GF_BG_LYR_MAIN_2, 0, &app->bgToggleRequests);
        if (BattleCastlePartyApp_PlayActionEffect(app, app->cursorPos, app->action) == TRUE) {
            app->subState = CASTLE_PARTY_STATE_ACTION_DONE;
        }
        break;
    case CASTLE_PARTY_STATE_ACTION_DONE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
            BattleCastlePartyApp_PrintChoosePrompt(app);
            app->subState = CASTLE_PARTY_STATE_SELECT_MON;
        }
        break;
    case CASTLE_PARTY_STATE_STATS_SUMMARY:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            BattleCastlePartyApp_ScrollStatsSummary(app, -1);
        } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
            BattleCastlePartyApp_ScrollStatsSummary(app, 1);
        } else if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_HideSummaryPanel(app);
            BattleCastlePartyApp_OpenMainMenu(app);
            app->subState = CASTLE_PARTY_STATE_MAIN_MENU;
        }
        break;
    case CASTLE_PARTY_STATE_MOVES_SUMMARY:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            BattleCastlePartyApp_ScrollMovesSummary(app, -1);
        } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
            BattleCastlePartyApp_ScrollMovesSummary(app, 1);
        } else if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastlePartyApp_HideSummaryPanel(app);
            BattleCastlePartyApp_OpenMainMenu(app);
            app->subState = CASTLE_PARTY_STATE_MAIN_MENU;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL BattleCastlePartyApp_LinkAction(BattleCastlePartyApp *app) {
    switch (app->subState) {
    case 0:
        app->flags.bits.linkResume = 0;
        BattleCastle_RequestBgToggle(GF_BG_LYR_MAIN_2, 0, &app->bgToggleRequests);
        if (BattleCastlePartyApp_SendLinkCommand(app, CASTLE_LINK_CMD_ACTION, app->cursorPos) == TRUE) {
            app->flags.bits.linkSyncPending = FALSE;
            app->subState++;
        }
        break;
    case 1:
        if (app->unk_012 != 0xFF) {
            u8 action;

            app->linkRecvCount = 0;
            action = app->action;
            if (action == CASTLE_PARTY_ACTION_HEAL_RANK_UP || action == CASTLE_PARTY_ACTION_RENTAL_RANK_UP) {
                BattleCastlePartyApp_RankUp(app, app->unk_012, action);
            } else {
                BattleCastlePartyApp_ApplyAction(app, app->unk_012, action);
            }
            app->subState++;
        }
        break;
    case 2:
        if (BattleCastlePartyApp_PlayActionEffect(app, BattleCastle_GetSideSlot(app->unk_015, app->unk_012), app->action) == TRUE) {
            app->timer = 30;
            app->subState++;
        }
        break;
    case 3:
        app->timer--;
        if (app->timer == 0) {
            sub_02037BEC();
            sub_02037AC0(CASTLE_PARTY_SYNC_ACTION);
            app->subState++;
        }
        break;
    case 4:
        if (sub_02037B38(CASTLE_PARTY_SYNC_ACTION) == TRUE) {
            sub_02037BEC();
            sub_020379A0(HEAP_ID_107);
            app->unk_012 = 0xFF;
            if (app->flags.bits.linkResume == 0) {
                BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
                BattleCastlePartyApp_PrintChoosePrompt(app);
            }
            app->unk_7FE = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleCastlePartyApp_LinkExit(BattleCastlePartyApp *app) {
    switch (app->subState) {
    case 0:
        if (BattleCastlePartyApp_SendLinkCommand(app, CASTLE_LINK_CMD_EXIT, 0) == TRUE) {
            app->timer = 30;
            app->subState++;
        }
        break;
    case 1:
        if (app->timer != 0) {
            app->timer--;
        }
        if (app->timer == 0) {
            sub_02037BEC();
            sub_02037AC0(CASTLE_PARTY_SYNC_EXIT);
            app->subState++;
        }
        break;
    case 2:
        if (sub_02037B38(CASTLE_PARTY_SYNC_EXIT) == TRUE) {
            sub_02037BEC();
            BattleCastlePartyApp_CloseWindow(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleCastlePartyApp_FadeOut(BattleCastlePartyApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_107);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on
void BattleCastlePartyApp_FreeResources(BattleCastlePartyApp *app) {
    int numMons;
    int i;

    BattleCastlePrompt_Free(&app->yesNoPrompt);
    BattleCastle_DestroyListMenuSpawner(app->listMenuSpawner);
    BattleCastle_FreeGrid(app->partyGrid);
    BattleCastleSprite_Delete(app->unk_734);
    BattleCastleSprite_Delete(app->unk_738);
    BattleCastleSprite_Delete(app->unk_778);
    BattleCastleSprite_Delete(app->cursor);
    BattleCastleSprite_Delete(app->partnerCursor);
    BattleCastleSprite_Delete(app->itemCursor);
    BattleCastleSprite_Delete(app->monPic);
    BattleCastleSprite_Delete(app->selectedItemIcon);
    numMons = ov80_02237B24(app->frontierType, 1);
    for (i = 0; i < numMons; i++) {
        BattleCastleSprite_Delete(app->monIcons[i]);
        BattleCastleSprite_Delete(app->heldItemIcons[i]);
        BattleCastleSprite_Delete(app->hpBars[i]);
    }
    for (i = 0; i < 6; i++) {
        BattleCastleSprite_Delete(app->itemIcons[i]);
    }
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleCastle_FreeSpriteGfx(&app->spriteGfx);
    DestroyMsgData(app->msgData);
    DestroyMsgData(app->itemDescMsgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    MessagePrinter_Delete(app->msgPrinter);
    FontID_Release(4);
    for (i = 0; i < 3; i++) {
        String_Delete(app->strings[i]);
    }
    BattleCastle_RemoveWindows(app->windows, 0);
    BattleCastlePartyApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
}
#pragma force_active reset

void BattleCastlePartyApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

void BattleCastlePartyApp_InitGraphics(BattleCastlePartyApp *app) {
    Pokemon *mon;
    int i;
    int itemX;
    int numMons;
    int iconX;
    int iconAnim;
    u32 hp;
    u32 maxHp;
    int cursorX;
    int cursorY;
    u16 namePos[4];
    s16 itemPos[2];

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_107);
    BattleCastlePartyApp_InitBgGraphics(app);
    BattleCastlePartyApp_InitSpriteGfx(app);
    FontID_Alloc(4, HEAP_ID_107);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0031_bin, HEAP_ID_107);
    app->itemDescMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0221_bin, HEAP_ID_107);
    app->msgFormat = MessageFormat_New(HEAP_ID_107);
    app->expandedString = String_New(600, HEAP_ID_107);
    app->rawString = String_New(600, HEAP_ID_107);
    for (i = 0; i < 3; i++) {
        app->strings[i] = String_New(32, HEAP_ID_107);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_107);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_107);
    app->msgPrinter = MessagePrinter_New(1, 2, 0, HEAP_ID_107);
    BattleCastle_AddWindows(app->bgConfig, app->windows, 0);
    BattleCastlePartyApp_GetTrainerNamePositions(app, &namePos[3], &namePos[2], &namePos[1], &namePos[0]);
    app->unk_734 = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 4, 160, 10, 0, 0);
    app->unk_738 = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 5, 160, 124, 0, 0);
    BattleCastleSprite_SetDrawFlag(app->unk_734, FALSE);
    BattleCastleSprite_SetDrawFlag(app->unk_738, FALSE);
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        iconX = 64;
        itemX = 72;
    } else {
        iconX = 32;
        itemX = 40;
    }
    numMons = ov80_02237B24(app->frontierType, 1);
    for (i = 0; i < numMons; i++, itemX += 64, iconX += 64) {
        app->heldItemIcons[i] = BattleCastleSprite_New(&app->spriteGfx, 1, 1, 1, 0, itemX, 62, 2, 0);
        mon = Party_GetMonByIndex(app->party, i);
        if (GetMonData(mon, MON_DATA_HELD_ITEM, NULL) == ITEM_NONE) {
            BattleCastleSprite_SetDrawFlag(app->heldItemIcons[i], FALSE);
        }
        hp = GetMonData(mon, MON_DATA_HP, NULL);
        maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        iconAnim = BattleCastlePartyApp_GetMonIconHpPalette(app, CalculateHpBarColor(hp, maxHp, 48));
        app->hpBars[i] = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, BattleCastlePartyApp_GetHpBarAnimSeq(app, CalculateHpBarColor(hp, maxHp, 48)), iconX, 78, 3, 0);
        app->monIcons[i] = BattleCastleSprite_New(&app->spriteGfx, i + 10, 10, 5, iconAnim, iconX, 58, 2, 0);
        BattleCastleSprite_SetMonIconPalette(app->monIcons[i], Party_GetMonByIndex(app->party, i));
    }
    app->selectedItemIcon = BattleCastleSprite_NewSubScreen(&app->spriteGfx, 3, 3, 3, 0, 16, 160, 0, 0);
    BattleCastleSprite_SetDrawFlag(app->selectedItemIcon, FALSE);
    for (i = 0; i < 6; i++) {
        BattleCastlePartyApp_GetItemButtonPos(i, &itemPos[1], &itemPos[0]);
        app->itemIcons[i] = BattleCastleSprite_New(&app->spriteGfx, i + 4, i + 4, 4, 0, itemPos[1], itemPos[0], 0, 0);
        BattleCastleSprite_SetDrawFlag(app->itemIcons[i], FALSE);
    }
    BattleCastlePartyApp_GetCursorPos(app, &cursorX, &cursorY, 0);
    app->cursor = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 1, cursorX, cursorY, 2, 0);
    app->partnerCursor = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 2, cursorX, cursorY, 2, 0);
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        BattleCastleSprite_SetDrawFlag(app->partnerCursor, FALSE);
    }
    app->unk_778 = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 11, 20, 20, 0, 0);
    BattleCastleSprite_SetDrawFlag(app->unk_778, FALSE);
    app->itemCursor = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 3, 20, 20, 1, 0);
    BattleCastleSprite_SetDrawFlag(app->itemCursor, FALSE);
    BattleCastlePartyApp_LoadMonSummary(app);
    BattleCastlePartyApp_PrintSummaryLabels(app);
    BattleCastlePartyApp_PrintMonSummary(app, TRUE);
    app->monPic = BattleCastleSprite_NewSubScreen(&app->spriteGfx, 2, 2, 2, 0, 48, 40, 0, 0);
    BattleCastleSprite_LoadMonPic(app->monPic, app->summary.boxMon, app->summary.species, app->summary.personality);
    app->partyGrid = BattleCastlePartyApp_CreatePartyGrid(app, 1, app->numMons);
    app->listMenuSpawner = BattleCastle_CreateListMenuSpawner(app->spriteGfx.spriteList, app->paletteData);
    BattleCastlePrompt_Init(&app->yesNoPrompt);
    if (sub_02037474()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 6, 10);
    Main_SetVBlankIntrCB(BattleCastlePartyApp_VBlankCallback, app);
}

void BattleCastlePartyApp_InitBgGraphics(BattleCastlePartyApp *app) {
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    BattleCastlePartyApp_SetGraphicsBanks();
    BattleCastlePartyApp_InitBgLayers(app->bgConfig);
    app->paletteData = PaletteData_Init(HEAP_ID_107);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_107);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_107);
    BattleCastlePartyApp_LoadPartyBg(app, GF_BG_LYR_MAIN_3);
    BattleCastlePartyApp_LoadBgPalette();
    BattleCastlePartyApp_LoadStatsSummaryBg(app, GF_BG_LYR_MAIN_2);
    ov83_0223FAF0();
    BattleCastle_RequestBgToggle(GF_BG_LYR_MAIN_2, 0, &app->bgToggleRequests);
    BattleCastlePartyApp_LoadSubScreenBg(app, GF_BG_LYR_SUB_0);
}

void BattleCastlePartyApp_InitSpriteGfx(BattleCastlePartyApp *app) {
    BattleCastle_InitSpriteGfx(&app->spriteGfx, app->party, ov80_02237D8C(app->frontierType));
}

void BattleCastlePartyApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK; // GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB)
}

void BattleCastlePartyApp_VBlankCallback(void *arg) {
    // OS_SetIrqCheckFlag(OS_IE_V_BLANK) with the DTCM address written out
    vu32 *dtcm = (vu32 *)0x027E0000;
    BattleCastlePartyApp *app = arg;

    if (app->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(app->paletteData);
    }
    DoScheduledBgGpuUpdates(app->bgConfig);
    BattleCastle_ApplyBgToggleRequests(&app->bgToggleRequests);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
}

void BattleCastlePartyApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = ov83_02247E88;
    GfGfx_SetBanks(&banks);
}

void BattleCastlePartyApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main1;
    BgTemplate main0;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;
    BgTemplate sub2;
    BgTemplate sub3;

    modes = ov83_02247D38;
    SetBothScreensModesAndDisable(&modes);
    main1 = ov83_02247DA0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main0 = ov83_02247DBC;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main2 = ov83_02247DF4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = ov83_02247E10;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = ov83_02247E2C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = ov83_02247E48;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    sub2 = ov83_02247D84;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    sub3 = ov83_02247DD8;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void BattleCastlePartyApp_LoadPartyBg(BattleCastlePartyApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x22, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x23, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    } else {
        GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x24, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    }
}

void BattleCastlePartyApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9C, &plttData, HEAP_ID_107);

    DC_FlushRange(plttData->pRawData, 0xE0);
    GX_LoadBGPltt(plttData->pRawData, 0, 0xE0);
    Heap_Free(file);
}

void BattleCastlePartyApp_LoadStatsSummaryBg(BattleCastlePartyApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x22, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x2A, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
}

// Reloads the first 4 palettes of the same file as BattleCastlePartyApp_LoadBgPalette
void ov83_0223FAF0(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9C, &plttData, HEAP_ID_107);

    DC_FlushRange(plttData->pRawData, 0x80);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x80);
    Heap_Free(file);
}

void BattleCastlePartyApp_LoadMovesSummaryBg(BattleCastlePartyApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x22, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x2B, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
}

void BattleCastlePartyApp_LoadItemGridBg(BattleCastlePartyApp *app) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x22, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x26, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x28, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x29, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_107);
}

void BattleCastlePartyApp_LoadSubScreenBg(BattleCastlePartyApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x28, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x93, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_107);
}

int BattleCastlePartyApp_PrintMessage(BattleCastlePartyApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    int printerId;

    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgNo, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    printerId = AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, speed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    ScheduleWindowCopyToVram(window);
    return printerId;
}

// Same as BattleCastlePartyApp_PrintMessage without clearing the window first
int BattleCastlePartyApp_AddMessage(BattleCastlePartyApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    int printerId;

    ReadMsgDataIntoString(app->msgData, msgNo, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    printerId = AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, speed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    ScheduleWindowCopyToVram(window);
    return printerId;
}

int BattleCastlePartyApp_ShowMessage(BattleCastlePartyApp *app, int msgNo, u8 fontId) {
    int printerId = BattleCastlePartyApp_PrintMessage(app, &app->windows[CASTLE_PARTY_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, fontId);

    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_MESSAGE]);
    return printerId;
}

void BattleCastlePartyApp_PrintStatsSummary(BattleCastlePartyApp *app, Window *window, Pokemon *mon) {
    u32 value;

    FillWindowPixelBuffer(window, 0);
    value = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    BufferItemName(app->msgFormat, 0, value);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00070, 1, 8);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00071, 64, 8);
    value = GetMonNature(mon);
    BufferNatureName(app->msgFormat, 0, value);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00072, 1, 24);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00073, 64, 24);
    value = GetMonData(mon, MON_DATA_ABILITY, NULL);
    BufferAbilityName(app->msgFormat, 0, value);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00074, 1, 40);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00075, 64, 40);
    value = GetMonData(mon, MON_DATA_ATK, NULL);
    BattleCastlePartyApp_BufferNumber(app, 0, value, 3, 1);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00076, 1, 56);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00077, 56, 56);
    value = GetMonData(mon, MON_DATA_DEF, NULL);
    BattleCastlePartyApp_BufferNumber(app, 0, value, 3, 1);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00078, 88, 56);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00079, 144, 56);
    value = GetMonData(mon, MON_DATA_SP_ATK, NULL);
    BattleCastlePartyApp_BufferNumber(app, 0, value, 3, 1);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00080, 1, 72);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00081, 56, 72);
    value = GetMonData(mon, MON_DATA_SP_DEF, NULL);
    BattleCastlePartyApp_BufferNumber(app, 0, value, 3, 1);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00082, 88, 72);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00083, 144, 72);
    value = GetMonData(mon, MON_DATA_SPEED, NULL);
    BattleCastlePartyApp_BufferNumber(app, 0, value, 3, 1);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00084, 1, 88);
    BattleCastlePartyApp_PrintSummaryText(app, window, msg_0031_00085, 56, 88);
    ScheduleWindowCopyToVram(window);
}

int BattleCastlePartyApp_PrintSummaryText(BattleCastlePartyApp *app, Window *window, int msgNo, int x, u16 y) {
    return BattleCastlePartyApp_AddMessage(app, window, msgNo, x, y, TEXT_SPEED_NOTRANSFER, 1, 2, 0, 0);
}

void BattleCastlePartyApp_PrintMovesSummary(BattleCastlePartyApp *app, Window *window, Pokemon *mon) {
    FillWindowPixelBuffer(window, 0);
    BattleCastlePartyApp_PrintMove(app, window, 0, msg_0031_00096, msg_0031_00100, mon, MON_DATA_MOVE1, MON_DATA_MOVE1_PP, MON_DATA_MOVE1_MAX_PP);
    BattleCastlePartyApp_PrintMove(app, window, 1, msg_0031_00097, msg_0031_00100, mon, MON_DATA_MOVE2, MON_DATA_MOVE2_PP, MON_DATA_MOVE2_MAX_PP);
    BattleCastlePartyApp_PrintMove(app, window, 2, msg_0031_00098, msg_0031_00100, mon, MON_DATA_MOVE3, MON_DATA_MOVE3_PP, MON_DATA_MOVE3_MAX_PP);
    BattleCastlePartyApp_PrintMove(app, window, 3, msg_0031_00099, msg_0031_00100, mon, MON_DATA_MOVE4, MON_DATA_MOVE4_PP, MON_DATA_MOVE4_MAX_PP);
    ScheduleWindowCopyToVram(window);
}

void BattleCastlePartyApp_PrintMove(BattleCastlePartyApp *app, Window *window, int moveSlot, int msgNo, int ppMsgNo, Pokemon *mon, int moveField, int ppField, int maxPpField) {
    u32 value;
    u16 y;

    value = GetMonData(mon, moveField, NULL);
    BufferMoveName(app->msgFormat, moveSlot, value);
    y = moveSlot * 24 + 12;
    app->printerId = BattleCastlePartyApp_AddMessage(app, window, msgNo, 24, y, TEXT_SPEED_NOTRANSFER, 1, 2, 0, 0);
    value = GetMonData(mon, ppField, NULL);
    BattleCastlePartyApp_BufferNumber(app, 4, value, 3, 1);
    value = GetMonData(mon, maxPpField, NULL);
    BattleCastlePartyApp_BufferNumber(app, 5, value, 3, 0);
    app->printerId = BattleCastlePartyApp_AddMessage(app, window, ppMsgNo, 96, y, TEXT_SPEED_NOTRANSFER, 1, 2, 0, 0);
}

void BattleCastlePartyApp_PrintPartyHp(BattleCastlePartyApp *app, Window *window) {
    int numMons;
    int i;

    FillWindowPixelBuffer(window, 0);
    numMons = ov80_02237B24(app->frontierType, 1);
    for (i = 0; i < numMons; i++) {
        // the slot is narrowed to u8 at this call site
        BattleCastlePartyApp_PrintMonHp(app, window, (u8)i, 0);
    }
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on
// mode 0: in the party panel column of the slot; otherwise at the start of a single-mon window
void BattleCastlePartyApp_PrintMonHp(BattleCastlePartyApp *app, Window *window, int slot, int mode) {
    u16 maxHpX;
    u16 slashX;
    Pokemon *mon;
    u16 baseX;
    u16 hpX;
    u8 y;

    mon = Party_GetMonByIndex(app->party, slot);

    if (ov80_02237D8C(app->frontierType) == FALSE) {
        baseX = 36;
    } else {
        baseX = 4;
    }
    if (mode == 0) {
        hpX = baseX + slot * CASTLE_PARTY_COLUMN_WIDTH;
        y = 1;
        slashX = baseX + 24 + slot * CASTLE_PARTY_COLUMN_WIDTH;
        maxHpX = baseX + 32 + slot * CASTLE_PARTY_COLUMN_WIDTH;
    } else {
        slashX = 28;
        hpX = 4;
        y = 0;
        maxHpX = 36;
    }
    PrintUIntOnWindow(app->msgPrinter, GetMonData(mon, MON_DATA_HP, NULL), 3, PRINTING_MODE_RIGHT_ALIGN, window, hpX, y);
    sub_0200CDAC(app->msgPrinter, 0, window, slashX, y); // "/"
    PrintUIntOnWindow(app->msgPrinter, GetMonData(mon, MON_DATA_MAX_HP, NULL), 3, PRINTING_MODE_LEFT_ALIGN, window, maxHpX, y);
}
#pragma force_active reset

void BattleCastlePartyApp_PrintPartyLevels(BattleCastlePartyApp *app, Window *window) {
    int numMons = ov80_02237B24(app->frontierType, 1);
    int i;

    for (i = 0; i < numMons; i++) {
        BattleCastlePartyApp_PrintMonLevelAndGender(app, window, (u8)i, 0);
    }
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on
// mode 0: in the party panel column of the slot; otherwise at the start of a single-mon window
void BattleCastlePartyApp_PrintMonLevelAndGender(BattleCastlePartyApp *app, Window *window, u32 slot, u8 mode) {
    unsigned int levelBaseX;
    unsigned int genderX;
    unsigned int levelX;
    unsigned int y;
    Pokemon *mon;

    if (ov80_02237D8C(app->frontierType) == FALSE) {
        levelBaseX = 40;
        genderX = 80;
    } else {
        levelBaseX = 8;
        genderX = 48;
    }
    mon = Party_GetMonByIndex(app->party, slot);
    if (mode == 0) {
        levelX = slot * CASTLE_PARTY_COLUMN_WIDTH + levelBaseX;
        y = 1;
        genderX = genderX + slot * CASTLE_PARTY_COLUMN_WIDTH;
    } else {
        levelX = 4;
        y = 0;
        genderX = 48;
    }
    sub_0200CE7C(app->msgPrinter, 1, GetMonData(mon, MON_DATA_LEVEL, NULL), 3, PRINTING_MODE_LEFT_ALIGN, window, levelX, y); // "Lv. {n}"
    BattleCastlePartyApp_PrintGenderSymbol(app, window, genderX, y, 0, GetMonData(mon, MON_DATA_GENDER, NULL));
}
#pragma force_active reset

// Redraws the current HP of the mon in the party panel column of the slot
void BattleCastlePartyApp_RedrawMonHp(BattleCastlePartyApp *app, Window *window, int slot, int hp) {
    int x = ov80_02237D8C(app->frontierType) == FALSE ? 36 : 4;

    x += slot * CASTLE_PARTY_COLUMN_WIDTH;
    FillWindowPixelRect(window, 0, (u16)x, 0, 24, 9);
    PrintUIntOnWindow(app->msgPrinter, hp, 3, PRINTING_MODE_RIGHT_ALIGN, window, x, 1);
    ScheduleWindowCopyToVram(window);
}

void BattleCastlePartyApp_PrintChoosePrompt(BattleCastlePartyApp *app) {
    app->printerId = BattleCastlePartyApp_PrintMessage(app, &app->windows[1], 6, 0, 5, 0xFF, 1, 2, 0, 0); // "EXIT"
    BattleCastle_DrawWindowFrame(&app->windows[7], Options_GetFrame(app->options));
    app->printerId = BattleCastlePartyApp_PrintMessage(app, &app->windows[7], 5, 1, 1, 0xFF, 1, 2, 15, 1); // "Please choose your Pokémon."
}

void BattleCastlePartyApp_HideChoosePrompt(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_CloseWindow(&app->windows[7]);
}

#pragma force_active on
// HEAL / RENTAL / CANCEL
void BattleCastlePartyApp_OpenMainMenu(BattleCastlePartyApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
    app->listCursorPos = CASTLE_PARTY_LIST_POS_NONE;
    BattleCastlePartyApp_CreateMainMenuList(app);
    BattleCastlePartyApp_PrintMainMenuHelp(app);
}
#pragma force_active reset

void BattleCastlePartyApp_CloseMainMenu(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_CloseWindow(&app->windows[6]);
    BattleCastlePartyApp_DestroyListMenu(app);
}

#pragma force_active on
// RESTORE HP / RESTORE PP / RESTORE ALL / RANK UP / CANCEL
void BattleCastlePartyApp_OpenHealMenu(BattleCastlePartyApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
    app->listCursorPos = CASTLE_PARTY_LIST_POS_NONE;
    BattleCastlePartyApp_CreateHealMenuList(app);
    BattleCastlePartyApp_PrintHealMenuHelp(app);
}
#pragma force_active reset

void BattleCastlePartyApp_CloseHealMenu(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_DestroyListMenu(app);
}

#pragma force_active on
void BattleCastlePartyApp_OpenRentalMenu(BattleCastlePartyApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
    app->listCursorPos = CASTLE_PARTY_LIST_POS_NONE;
    BattleCastlePartyApp_CreateRentalMenuList(app);
    BattleCastlePartyApp_PrintRentalMenuHelp(app);
}
#pragma force_active reset

void BattleCastlePartyApp_CloseRentalMenu(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_DestroyListMenu(app);
}

// mode: the rental action (6 or 7) the item grid is opened for
void BattleCastlePartyApp_OpenItemGrid(BattleCastlePartyApp *app, int mode) {
    app->flags.bits.itemMenuOpen = TRUE;
    BattleCastlePartyApp_CloseWindow(&app->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[9]);
    BattleCastlePartyApp_LoadItemGridBg(app);
    BattleCastle_RequestBgToggle(2, 1, &app->bgToggleRequests);
    BattleCastle_RequestBgToggle(6, 1, &app->bgToggleRequests);
    BattleCastle_RequestBgToggle(7, 1, &app->bgToggleRequests);
    app->unk_860 = 0;
    app->itemPage = 0;
    BattleCastlePartyApp_SetupItemGrid(app, mode);
}

#pragma force_active on
void BattleCastlePartyApp_CloseItemGrid(BattleCastlePartyApp *app) {
    if (app->flags.bits.itemMenuOpen == TRUE) {
        u32 i;

        for (i = 0; i < 6; i++) {
            ClearWindowTilemapAndScheduleTransfer(&app->windows[48 + i]);
        }
        ClearWindowTilemapAndScheduleTransfer(&app->windows[54]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[55]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[58]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[59]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[60]);
        BattleCastle_RequestBgToggle(2, 0, &app->bgToggleRequests);
        BattleCastle_RequestBgToggle(6, 0, &app->bgToggleRequests);
        BattleCastle_RequestBgToggle(7, 0, &app->bgToggleRequests);
        BattleCastle_FreeGrid(app->itemGrid);
        ListMenuItems_Delete(app->itemList);
        BattleCastleSprite_SetDrawFlag(app->itemCursor, FALSE);
        BattleCastleSprite_SetDrawFlag(app->selectedItemIcon, FALSE);
        {
            u32 j;

            for (j = 0; j < 6; j++) {
                BattleCastleSprite_SetDrawFlag(app->itemIcons[j], FALSE);
            }
        }
        BattleCastleSprite_SetPositionSubScreen(app->monPic, 48, 40);
        app->flags.bits.itemMenuOpen = FALSE;
    }
}
#pragma force_active reset

void BattleCastlePartyApp_ShowYesNoPrompt(BattleCastlePartyApp *app) {
    BattleCastlePrompt_Show(&app->yesNoPrompt, app->bgConfig);
}

// Title ("Rental/Berry" or "Rental/Item"), the player's name and their Castle Points
void BattleCastlePartyApp_PrintItemGridHeader(BattleCastlePartyApp *app, int mode) {
    PlayerProfile *profile;
    String *name;
    u32 color;
    int width;

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_GRID_TITLE], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_GRID_PLAYER_NAME], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_GRID_POINTS], 0);
    if (mode == CASTLE_PARTY_ACTION_RENT_BERRY) {
        BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_GRID_TITLE], app->msgData, msg_0031_00106, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    } else {
        BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_GRID_TITLE], app->msgData, msg_0031_00107, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    }
    profile = Save_PlayerData_GetProfile(app->saveData);
    name = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_107);
    CopyU16ArrayToString(name, PlayerProfile_GetNamePtr(profile));
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = CASTLE_PARTY_TEXT_COLOR_MALE_NAME;
    } else {
        color = CASTLE_PARTY_TEXT_COLOR_FEMALE;
    }
    BattleCastle_PrintString(&app->windows[CASTLE_PARTY_WIN_GRID_PLAYER_NAME], name, 0, 0, 0, color, CASTLE_ALIGN_LEFT);
    String_Delete(name);
    BattleCastlePartyApp_BufferNumber(app, 0, FrontierSave_GetStat(app->frontierSave, sub_0205C1F0(app->frontierType), sub_0205C268(sub_0205C1F0(app->frontierType))), 4, PRINTING_MODE_RIGHT_ALIGN);
    width = GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_GRID_POINTS]) * 8;
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_GRID_POINTS], app->msgData, msg_0031_00002, width, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT); // "{n} CP"
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_GRID_TITLE]);
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_GRID_PLAYER_NAME]);
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_GRID_POINTS]);
}

// Names and costs of the items on the current page
void BattleCastlePartyApp_PrintItemNames(BattleCastlePartyApp *app) {
    u16 first;
    u16 i;
    int width;

    first = (u16)(app->itemPage * CASTLE_ITEMS_PER_PAGE);
    for (i = 0; i < CASTLE_ITEMS_PER_PAGE; i++) {
        FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i], 0);
        if ((int)(first + i) < app->numItems) {
            AddTextPrinterParameterizedWithColor(&app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i], 0, (&app->itemList[first])[i].text, 4, 4, TEXT_SPEED_NOTRANSFER, CASTLE_PARTY_TEXT_COLOR, NULL);
            BattleCastlePartyApp_BufferNumber(app, 0, BattleCastlePartyApp_GetRentalCost(app, (u16)(&app->itemList[first])[i].value, app->action), 2, PRINTING_MODE_LEFT_ALIGN);
            width = GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i]) * 8 - 4;
            BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i], app->msgData, msg_0031_00104, width, 20, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT); // "{n}CP"
        }
        ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i]);
    }
}

#pragma force_active on
// Icons of the items on the current page
void BattleCastlePartyApp_DrawItemIcons(BattleCastlePartyApp *app) {
    // The list entry offset and the icon slot (iconSlot->itemIcons[0] == app->itemIcons[i])
    // are stepped by hand: keeps the original codegen
    u32 itemOffset = 0;
    u32 pageOffset;
    u32 i = 0;
    u32 index = (u16)(app->itemPage * CASTLE_ITEMS_PER_PAGE);
    BattleCastlePartyApp *iconSlot = app;

    pageOffset = index * sizeof(ListMenuItem);
    for (; i < CASTLE_ITEMS_PER_PAGE; i++, index++, itemOffset += sizeof(ListMenuItem), iconSlot = (BattleCastlePartyApp *)((u8 *)iconSlot + sizeof(BattleCastleSprite *))) {
        if (index < app->numItems) {
            ListMenuItem *item = (ListMenuItem *)(itemOffset + (pageOffset + (u32)app->itemList));
            u16 itemId = BattleCastlePartyApp_GetRentalItemId(app, (u16)item->value, app->action);
            struct {
                u16 y;
                u16 x;
            } pos;

            BattleCastle_LoadItemIconChar(&app->spriteGfx, i + 4, itemId);
            BattleCastle_LoadItemIconPalette(&app->spriteGfx, i + 4, itemId);
            BattleCastlePartyApp_GetItemButtonPos(i, &pos.x, &pos.y);
            BattleCastleSprite_SetPosition(iconSlot->itemIcons[0], pos.x, pos.y);
            BattleCastleSprite_SetDrawFlag(iconSlot->itemIcons[0], TRUE);
        } else {
            BattleCastleSprite_SetDrawFlag(iconSlot->itemIcons[0], FALSE);
        }
    }
}
#pragma force_active reset

// "{page}/{numPages}"
void BattleCastlePartyApp_PrintItemPageNumber(BattleCastlePartyApp *app) {
    int x;

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_PAGE_NUMBER], 0);
    BattleCastlePartyApp_BufferNumber(app, 0, app->itemPage + 1, 1, PRINTING_MODE_LEFT_ALIGN);
    BattleCastlePartyApp_BufferNumber(app, 1, (app->numItems - 1) / CASTLE_ITEMS_PER_PAGE + 1, 1, PRINTING_MODE_LEFT_ALIGN);
    x = GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_PAGE_NUMBER]) * 8 / 2;
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_PAGE_NUMBER], app->msgData, msg_0031_00103, x, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_CENTER);
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_PAGE_NUMBER]);
}

void BattleCastlePartyApp_PrintReturnButton(BattleCastlePartyApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_RETURN], 0);
    width = GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_RETURN]);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_RETURN], app->msgData, msg_0031_00105, width * 8 / 2, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_CENTER); // "RETURN"
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_RETURN]);
}

// Description and icon of the item under the cursor (item: position on the page)
void BattleCastlePartyApp_PrintItemDescription(BattleCastlePartyApp *app, u32 item) {
    u16 index;
    u16 itemId;

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM_DESC], 0);
    index = app->itemPage * CASTLE_ITEMS_PER_PAGE + item;
    BattleCastleSprite_SetDrawFlag(app->selectedItemIcon, FALSE);
    if (item < CASTLE_ITEMS_PER_PAGE && index < app->numItems) {
        itemId = BattleCastlePartyApp_GetRentalItemId(app, index, app->action);
        BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_ITEM_DESC], app->itemDescMsgData, itemId, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastle_LoadItemIconChar(&app->spriteGfx, 3, itemId);
        BattleCastle_LoadItemIconPalette(&app->spriteGfx, 3, itemId);
        BattleCastleSprite_SetDrawFlag(app->selectedItemIcon, TRUE);
    }
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_ITEM_DESC]);
}

#pragma force_active on
// Nickname, gender, level, HP and held item of the mon the item is for
void BattleCastlePartyApp_PrintSelectedMonInfo(BattleCastlePartyApp *app) {
    u32 i;

    for (i = CASTLE_PARTY_WIN_MON_NICKNAME; i <= CASTLE_PARTY_WIN_MON_ITEM; i++) {
        FillWindowPixelBuffer(&app->windows[i], 0);
    }
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_MON_LV], app->msgData, msg_0031_00088, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);         // "Lv."
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_MON_ITEM_LABEL], app->msgData, msg_0031_00070, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT); // "Item"
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_MON_HP_LABEL], app->msgData, msg_0031_00089, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);   // "HP"
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.hp, 3, PRINTING_MODE_LEFT_ALIGN);
    BattleCastlePartyApp_BufferNumber(app, 1, app->summary.maxHp, 3, PRINTING_MODE_LEFT_ALIGN);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_MON_HP], app->msgData, msg_0031_00095, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_MON_HP]) * 8 / 2, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_CENTER); // "{hp}/{maxHp}"
    BufferBoxMonNickname(app->msgFormat, 0, Mon_GetBoxMon(app->summary.mon));
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_MON_NICKNAME], app->msgData, msg_0031_00091, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    if (!app->summary.unk_0A_7) {
        if (app->summary.gender == MON_MALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_MON_GENDER], app->msgData, msg_0031_00086, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR_MALE, CASTLE_ALIGN_LEFT); // "♂"
        } else if (app->summary.gender == MON_FEMALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_MON_GENDER], app->msgData, msg_0031_00087, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR_FEMALE, CASTLE_ALIGN_LEFT); // "♀"
        }
    }
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.level, 3, PRINTING_MODE_LEFT_ALIGN);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_MON_LEVEL], app->msgData, msg_0031_00094, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BufferItemName(app->msgFormat, 0, app->summary.heldItem);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_MON_ITEM], app->msgData, msg_0031_00071, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    for (i = CASTLE_PARTY_WIN_MON_NICKNAME; i <= CASTLE_PARTY_WIN_MON_ITEM; i++) {
        ScheduleWindowCopyToVram(&app->windows[i]);
    }
}
#pragma force_active reset

#pragma force_active on
// Builds the rental list for the player's rental rank and draws the item grid
void BattleCastlePartyApp_SetupItemGrid(BattleCastlePartyApp *app, int mode) {
    MsgData *itemNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0222_bin, HEAP_ID_107);
    int rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
    const u16 *items;
    u16 i;

    if (mode == CASTLE_PARTY_ACTION_RENT_BERRY) {
        app->numItems = ov83_02247D12[rank - 1];
        items = ov83_02247F88;
    } else {
        app->numItems = ov83_02247D24[rank - 1];
        items = ov83_02247EE0;
    }
    app->itemList = ListMenuItems_New(app->numItems, HEAP_ID_107);
    for (i = 0; i < app->numItems; i++) {
        ListMenuItems_AppendFromMsgData(app->itemList, itemNames, items[i], i);
    }
    DestroyMsgData(itemNames);
    app->itemGrid = BattleCastlePartyApp_CreateItemGrid(app);
    BattleCastleSprite_SetDrawFlag(app->itemCursor, TRUE);
    BattleCastlePartyApp_PrintItemGridHeader(app, mode);
    BattleCastlePartyApp_PrintItemNames(app);
    BattleCastlePartyApp_DrawItemIcons(app);
    BattleCastlePartyApp_PrintItemPageNumber(app);
    BattleCastlePartyApp_PrintReturnButton(app);
    BattleCastlePartyApp_PrintItemDescription(app, 0);
    BattleCastlePartyApp_PrintSelectedMonInfo(app);
    BattleCastleSprite_SetPositionSubScreen(app->monPic, 48, 72);
}
#pragma force_active reset

void BattleCastlePartyApp_BufferNumber(BattleCastlePartyApp *app, int field, int value, int digits, int mode) {
    BufferIntegerAsString(app->msgFormat, field, value, digits, (PrintingMode)mode, TRUE);
}

void BattleCastlePartyApp_BufferMonNickname(BattleCastlePartyApp *app, int field, BoxPokemon *boxMon) {
    BufferBoxMonNickname(app->msgFormat, field, boxMon);
}

void BattleCastlePartyApp_BufferPlayerName(BattleCastlePartyApp *app, int field) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    BufferPlayersName(app->msgFormat, field, profile);
}

void BattleCastlePartyApp_PrintPlayerName(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    String *name = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_107);
    u32 color;

    CopyU16ArrayToString(name, PlayerProfile_GetNamePtr(profile));
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = CASTLE_PARTY_TEXT_COLOR_MALE_NAME;
    } else {
        color = CASTLE_PARTY_TEXT_COLOR_FEMALE;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, name, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(name);
}

void BattleCastlePartyApp_PrintPartnerName(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId) {
    PlayerProfile *profile = sub_02034818(Comm_GetLocalNetId() ^ 1);
    u32 color;

    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = CASTLE_PARTY_TEXT_COLOR_MALE_NAME;
    } else {
        color = CASTLE_PARTY_TEXT_COLOR_FEMALE;
    }
    BufferPlayersName(app->msgFormat, 0, profile);
    BattleCastlePartyApp_AddMessage(app, window, msg_0031_00001, x, y, TEXT_SPEED_NOTRANSFER, (u8)(color >> 16), (color >> 8) & 0xFF, color & 0xFF, 0);
}

void BattleCastlePartyApp_PrintGenderSymbol(BattleCastlePartyApp *app, Window *window, u32 x, u32 y, u8 fontId, u8 gender) {
    u32 msgNo;
    u8 fg;
    u8 shadow;
    u8 bg;

    if (gender == MON_MALE) {
        msgNo = msg_0031_00086; // "♂"
        fg = 7;
        shadow = 8;
        bg = 0;
    } else if (gender == MON_FEMALE) {
        msgNo = msg_0031_00087; // "♀"
        fg = 3;
        shadow = 4;
        bg = 0;
    } else {
        return;
    }
    BattleCastlePartyApp_AddMessage(app, window, msgNo, x, y, TEXT_SPEED_NOTRANSFER, fg, shadow, bg, fontId);
}

void BattleCastlePartyApp_SetMainState(BattleCastlePartyApp *app, int *state, u32 newState) {
    app->subState = 0;
    *state = newState;
}

void BattleCastlePartyApp_MoveCursor(BattleCastlePartyApp *app) {
    PlaySE(SEQ_SE_DP_SELECT);
    if (ov80_02237D8C(app->frontierType) == TRUE) {
        BattleCastlePartyApp_SendLinkCommand(app, CASTLE_LINK_CMD_CURSOR, app->cursorPos);
    }
    BattleCastlePartyApp_DrawCursor(app, app->cursorPos, FALSE);
}

// Moves the player's (or the partner's) cursor to a party slot, or onto EXIT
void BattleCastlePartyApp_DrawCursor(BattleCastlePartyApp *app, u32 index, int isPartner) {
    BattleCastleSprite *cursor;
    int x;
    int y;
    int seq;
    int exitSeq;

    if (isPartner == FALSE) {
        cursor = app->cursor;
        seq = 1;
        exitSeq = 0;
    } else {
        cursor = app->partnerCursor;
        seq = 2;
        exitSeq = 17;
    }

    if (index >= app->unk_015) {
        BattleCastleSprite_SetAnim(cursor, exitSeq);
        BattleCastleSprite_SetPosition(cursor, 224, 160);
        if (isPartner == FALSE) {
            BattleCastlePartyApp_ShowMonSummary(app, FALSE);
        }
        return;
    }

    BattleCastleSprite_SetAnim(cursor, seq);
    BattleCastlePartyApp_GetCursorPos(app, &x, &y, index);
    BattleCastleSprite_SetPosition(cursor, (u16)x, (u16)y);
    if (isPartner == FALSE) {
        if (app->prevCursorPos >= app->unk_015) {
            BattleCastlePartyApp_ShowMonSummary(app, TRUE);
        }
        BattleCastlePartyApp_RefreshMonSummary(app, 0);
    }
}

void BattleCastlePartyApp_GetCursorPos(BattleCastlePartyApp *app, int *x, int *y, u32 index) {
    if (ov80_02237D8C(app->frontierType) == TRUE) {
        if (index == 0) {
            *x = 40;
        } else if (index == 1) {
            *x = 104;
        } else if (index == 2) {
            *x = 168;
        } else {
            *x = 232;
        }
    } else {
        if (index == 0) {
            *x = 72;
        } else if (index == 1) {
            *x = 136;
        } else {
            *x = 200;
        }
    }
    *y = 88;
}

// Cost in CP of a rental entry
u16 BattleCastlePartyApp_GetRentalCost(BattleCastlePartyApp *app, int index, int kind) {
    BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
    if (kind == CASTLE_PARTY_ACTION_RENT_BERRY) {
        return ov83_02247FC8[index];
    }
    return ov83_02247F16[index];
}

u16 BattleCastlePartyApp_GetRentalCostByItemId(u16 itemId) {
    u32 i;

    for (i = 0; i < CASTLE_ITEM_GRID_NUM_ITEMS; i++) {
        if (itemId == ov83_02247EE0[i]) {
            return ov83_02247F16[i];
        }
    }
    for (i = 0; i < CASTLE_ITEM_GRID_NUM_BERRIES; i++) {
        if (itemId == ov83_02247F88[i]) {
            return ov83_02247FC8[i];
        }
    }
    GF_AssertFail();
    return 0;
}

// Item id of a rental entry
u16 BattleCastlePartyApp_GetRentalItemId(BattleCastlePartyApp *app, int index, int kind) {
    BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
    if (kind == CASTLE_PARTY_ACTION_RENT_BERRY) {
        return ov83_02247F88[index];
    }
    return ov83_02247EE0[index];
}

// Positions of the player's and the partner's name
void BattleCastlePartyApp_GetTrainerNamePositions(BattleCastlePartyApp *app, u16 *x1, u16 *y1, u16 *x2, u16 *y2) {
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        *x1 = 40;
        *y1 = 0;
        *x2 = 0;
        *y2 = 0;
    } else {
        *x1 = 0;
        *y1 = 0;
        *x2 = 128;
        *y2 = 0;
    }
}

// Plays the effect of the chosen action on the mon; returns TRUE once it is done
int BattleCastlePartyApp_PlayActionEffect(BattleCastlePartyApp *app, u32 selected, u32 action) {
    u32 slot;
    u32 hp;
    u32 iconPalette;
    Pokemon *mon;
    u32 maxHp;
    u32 hpBarSeq;

    slot = BattleCastle_GetMonIndex(app->numMons, selected);
    mon = Party_GetMonByIndex(app->party, slot);
    hp = GetMonData(mon, MON_DATA_HP, NULL);
    maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    iconPalette = BattleCastlePartyApp_GetMonIconHpPalette(app, CalculateHpBarColor(hp, maxHp, 48));
    hpBarSeq = BattleCastlePartyApp_GetHpBarAnimSeq(app, CalculateHpBarColor(hp, maxHp, 48));

    switch (action) {
    case CASTLE_PARTY_ACTION_HEAL_RANK_UP:
    case CASTLE_PARTY_ACTION_RENTAL_RANK_UP:
        app->flags.bits.effectActive = FALSE;
        return TRUE;
    case CASTLE_PARTY_ACTION_RESTORE_HP:
    case CASTLE_PARTY_ACTION_RESTORE_PP:
    case CASTLE_PARTY_ACTION_RESTORE_ALL:
        if (app->flags.bits.effectActive == FALSE) {
            s16 x;

            app->flags.bits.effectActive = TRUE;
            if (ov80_02237D8C(app->frontierType) == FALSE) {
                x = 64;
            } else {
                x = 32;
            }
            app->effectSprite = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 8, x + slot * 64, 62, 0, 0);
        }
        if (app->effectSprite != NULL && BattleCastleSprite_IsAnimated(app->effectSprite) == FALSE) {
            BattleCastleSprite_Delete(app->effectSprite);
            app->effectSprite = NULL;
        }
        if (app->effectSprite == NULL) {
            hp = GetMonData(mon, MON_DATA_HP, NULL);
            BattleCastlePartyApp_RedrawMonHp(app, &app->windows[3], slot, hp);
            if (app->cursorPos == slot) {
                BattleCastlePartyApp_LoadMonSummary(app);
                BattleCastlePartyApp_PrintMonSummary(app, FALSE);
            }
            BattleCastleSprite_SetAnim(app->hpBars[slot], hpBarSeq);
            ov83_02247600(app->monIcons[slot], (u8)iconPalette);
            app->flags.bits.effectActive = FALSE;
            return TRUE;
        }
        break;
    case CASTLE_PARTY_ACTION_RENT_BERRY:
    case CASTLE_PARTY_ACTION_RENT_ITEM:
        if (app->flags.bits.effectActive == FALSE) {
            s16 x;

            app->flags.bits.effectActive = TRUE;
            if (ov80_02237D8C(app->frontierType) == FALSE) {
                x = 64;
            } else {
                x = 32;
            }
            app->effectSprite = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 16, x + slot * 64, 62, 0, 0);
        }
        if (BattleCastleSprite_IsAnimated(app->effectSprite) == FALSE) {
            BattleCastleSprite_Delete(app->effectSprite);
            app->effectSprite = NULL;
            app->flags.bits.effectActive = FALSE;
            return TRUE;
        }
        break;
    case CASTLE_PARTY_ACTION_STATS:
        app->flags.bits.effectActive = FALSE;
        return TRUE;
    case CASTLE_PARTY_ACTION_MOVES:
        app->flags.bits.effectActive = FALSE;
        return TRUE;
    }
    return FALSE;
}

int BattleCastlePartyApp_GetMonIconHpPalette(BattleCastlePartyApp *app, u32 hpBarColor) {
    switch (hpBarColor) {
    case 4:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    case 1:
        return 4;
    default:
        return (int)app; // not reached for a valid colour
    }
}

int BattleCastlePartyApp_GetHpBarAnimSeq(BattleCastlePartyApp *app, u32 hpBarColor) {
    switch (hpBarColor) {
    case 4:
        return 15;
    case 3:
        return 15;
    case 2:
        return 14;
    case 1:
        return 13;
    default:
        return (int)app; // not reached for a valid colour
    }
}

// LEFT / RIGHT on the stats summary page: next mon, wrapping around
void BattleCastlePartyApp_ScrollStatsSummary(BattleCastlePartyApp *app, int delta) {
    s8 cursorPos = (s8)app->cursorPos + delta;
    Pokemon *mon;

    if (cursorPos < 0) {
        cursorPos = app->numMons - 1;
    } else if (cursorPos >= app->numMons) {
        cursorPos = 0;
    }
    app->cursorPos = cursorPos;
    BattleCastlePartyApp_MoveCursor(app);
    mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, app->cursorPos));
    BattleCastlePartyApp_PrintStatsSummary(app, &app->windows[4], mon);
}

// LEFT / RIGHT on the moves summary page: next mon, wrapping around
void BattleCastlePartyApp_ScrollMovesSummary(BattleCastlePartyApp *app, int delta) {
    s8 cursorPos = (s8)app->cursorPos + delta;
    Pokemon *mon;

    if (cursorPos < 0) {
        cursorPos = app->numMons - 1;
    } else if (cursorPos >= app->numMons) {
        cursorPos = 0;
    }
    app->cursorPos = cursorPos;
    BattleCastlePartyApp_MoveCursor(app);
    mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, app->cursorPos));
    BattleCastlePartyApp_PrintMovesSummary(app, &app->windows[4], mon);
}

// Closes whatever menu or panel is open (the partner left the link challenge)
void BattleCastlePartyApp_CloseAll(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_CloseMenus(app);
    BattleCastlePartyApp_CloseItemGrid(app);
    BattleCastlePartyApp_CloseWindow(&app->windows[6]);
    BattleCastlePartyApp_HideSummaryPanel(app);
    BattleCastle_RequestBgToggle(2, 0, &app->bgToggleRequests);
    BattleCastleSprite_SetDrawFlag(app->unk_778, FALSE);
}

#pragma force_active on
// TRUE if any move has less than its max PP
int BattleCastlePartyApp_IsAnyPpUsed(Pokemon *mon) {
    int used = FALSE;
    u32 pp;

    pp = GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
    if (pp != GetMonData(mon, MON_DATA_MOVE1_MAX_PP, NULL)) {
        used = TRUE;
    }
    pp = GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
    if (pp != GetMonData(mon, MON_DATA_MOVE2_MAX_PP, NULL)) {
        used = TRUE;
    }
    pp = GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
    if (pp != GetMonData(mon, MON_DATA_MOVE3_MAX_PP, NULL)) {
        used = TRUE;
    }
    pp = GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
    if (pp != GetMonData(mon, MON_DATA_MOVE4_MAX_PP, NULL)) {
        used = TRUE;
    }
    return used;
}
#pragma force_active reset

void BattleCastlePartyApp_CloseWindow(Window *window) {
    ClearFrameAndWindow2(window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(window);
}

BOOL BattleCastlePartyApp_SendLinkCommand(BattleCastlePartyApp *app, int command, int param) {
    int cmd;

    switch (command) {
    case 10:
        cmd = 0x31;
        BattleCastlePartyApp_PrepareLinkRanks(app, command);
        break;
    case 11:
        cmd = 0x32;
        BattleCastlePartyApp_PrepareLinkAction(app, command, param);
        break;
    case 12:
        cmd = 0x33;
        BattleCastlePartyApp_PrepareLinkCursor(app, command);
        break;
    case 13:
        cmd = 0x34;
        BattleCastlePartyApp_PrepareLinkExit(app, command);
        break;
    }
    if (Comm_SendCommand(cmd, app->linkSendBuf, sizeof(app->linkSendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void BattleCastlePartyApp_PrepareLinkRanks(BattleCastlePartyApp *app, int command) {
    PlayerProfile *profile;
    int i;
    u8 *dest;

    profile = Save_PlayerData_GetProfile(app->saveData);
    app->linkSendBuf[0] = command;
    app->linkSendBuf[1] = PlayerProfile_GetTrainerGender(profile);
    // linkSendBuf[2 + i], addressed from a cursor that steps through the app (keeps the original codegen)
    for (i = 0, dest = (u8 *)app + 2 * sizeof(u16); i < 3; i++, dest += sizeof(u16)) {
        *(u16 *)(dest + offsetof(BattleCastlePartyApp, linkSendBuf)) = BattleCastle_GetRank(app->saveData, app->frontierType, i);
    }
}

// Link receive: the partner's three Castle ranks (sent by BattleCastlePartyApp_PrepareLinkRanks)
void BattleCastlePartyApp_ReceiveRanks(int netId, void *unused, void *data, BattleCastlePartyApp *app) {
    int i;
    u16 *rank;

    app->linkRecvCount++;
    if (netId != Comm_GetLocalNetId()) {
        for (i = 0, rank = (u16 *)data + 2; i < 3; i++, rank++) {
            app->partnerRanks[i] = *rank;
        }
    }
}

void BattleCastlePartyApp_PrepareLinkAction(BattleCastlePartyApp *app, int command, int slot) {
    app->linkSendBuf[0] = command;
    app->linkSendBuf[1] = slot;
    if (Comm_GetLocalNetId() == 0 && app->unk_012 == 0xFF) {
        app->unk_012 = slot;
    }
    app->linkSendBuf[2] = app->unk_012;
    app->linkSendBuf[4] = app->selectedItem;
    app->linkSendBuf[5] = app->action;
}

#pragma force_active on
// Link receive: the partner's menu action (sent by BattleCastlePartyApp_PrepareLinkAction).
// The parent decides whose action wins; the child takes the parent's decision.
void BattleCastlePartyApp_ReceiveAction(int netId, int unused, u16 *data, BattleCastlePartyApp *app) {
    app->linkRecvCount++;
    if (netId != Comm_GetLocalNetId()) {
        app->unk_7FD = data[1];
        if (Comm_GetLocalNetId() == 0) {
            if (app->unk_012 != 0xFF) {
                app->unk_7FD = 0;
                return;
            }
            app->unk_012 = app->unk_7FD + app->unk_015;
            app->selectedItem = data[4];
            app->action = data[5];
        } else {
            app->unk_012 = data[2];
            app->selectedItem = data[4];
            app->action = data[5];
        }
    }
}
#pragma force_active reset

void BattleCastlePartyApp_PrepareLinkCursor(BattleCastlePartyApp *app, int command) {
    app->linkSendBuf[0] = command;
    app->linkSendBuf[1] = app->cursorPos;
}

#pragma force_active on
// Link receive: the partner's cursor position (sent by BattleCastlePartyApp_PrepareLinkCursor)
void BattleCastlePartyApp_ReceiveCursor(int netId, int unused, u16 *data, BattleCastlePartyApp *app) {
    if (netId != Comm_GetLocalNetId()) {
        app->partnerCursorPos = data[1];
        BattleCastlePartyApp_DrawCursor(app, app->partnerCursorPos, TRUE);
    }
}
#pragma force_active reset

void BattleCastlePartyApp_PrepareLinkExit(BattleCastlePartyApp *app, int command) {
    app->linkSendBuf[0] = 1;
}

// Link receive: the partner chose to exit (sent by BattleCastlePartyApp_PrepareLinkExit)
void BattleCastlePartyApp_ReceiveExit(int netId, int unused, u16 *data, BattleCastlePartyApp *app) {
    if (netId != Comm_GetLocalNetId()) {
        app->unk_7FE = data[0];
    }
}

void BattleCastlePartyApp_UseItemOnMon(Pokemon *mon, u16 itemId) {
    UseItemOnPokemon(mon, itemId, 0, 0, HEAP_ID_107);
}

#pragma force_active on
void BattleCastlePartyApp_PrintTrainerNames(BattleCastlePartyApp *app, Window *window) {
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;

    BattleCastlePartyApp_GetTrainerNamePositions(app, &x1, &y1, &x2, &y2);
    if (!ov80_02237D8C(app->frontierType)) {
        BattleCastlePartyApp_PrintPlayerName(app, window, (u16)(x1 + 4), (u16)(y1 + 1), 0);
    } else if (!Comm_GetLocalNetId()) {
        int y = y1; // widened copy of y1 keeps the original register allocation
        y = (u16)(y + 1);
        BattleCastlePartyApp_PrintPlayerName(app, window, x1, y, 0);
        BattleCastlePartyApp_PrintPartnerName(app, window, x2, (u16)(y2 + 1), 0);
    } else {
        int y = y1;
        y = (u16)(y + 1);
        BattleCastlePartyApp_PrintPartnerName(app, window, x1, y, 0);
        BattleCastlePartyApp_PrintPlayerName(app, window, x2, (u16)(y2 + 1), 0);
    }
    ScheduleWindowCopyToVram(window);
}
#pragma force_active reset

void BattleCastlePartyApp_HealMon(BattleCastlePartyApp *app, int slot, int action) {
    Pokemon *mon;
    int index = BattleCastle_GetMonIndex(app->numMons, slot);

    mon = Party_GetMonByIndex(app->party, index);
    BattleCastle_GetRank(app->saveData, app->frontierType, 0);
    BattleCastlePartyApp_BufferMonNickname(app, 0, Mon_GetBoxMon(mon));
    BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
    app->printerId = BattleCastlePartyApp_ShowMessage(app, _02247D0C[action - 1], 1);
    switch (action) {
    case 1:
        BattleCastlePartyApp_UseItemOnMon(mon, ITEM_MAX_POTION);
        break;
    case 2:
        BattleCastlePartyApp_UseItemOnMon(mon, ITEM_MAX_ELIXIR);
        break;
    case 3:
        BattleCastlePartyApp_UseItemOnMon(mon, ITEM_MAX_POTION);
        BattleCastlePartyApp_UseItemOnMon(mon, ITEM_MAX_ELIXIR);
        break;
    default:
        GF_AssertFail();
        break;
    }
    PlaySE(SEQ_SE_DP_KAIFUKU);
}

void BattleCastlePartyApp_GiveRentalItem(BattleCastlePartyApp *app, int slot, u16 itemId) {
    Pokemon *mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot));

    SetMonData(mon, MON_DATA_HELD_ITEM, &itemId);
    if (app->cursorPos == slot) {
        BattleCastlePartyApp_LoadMonSummary(app);
        BattleCastlePartyApp_PrintMonSummary(app, FALSE);
    }
    BattleCastleSprite_SetDrawFlag(app->heldItemIcons[BattleCastle_GetMonIndex(app->numMons, slot)], TRUE);
    BattleCastlePartyApp_BufferMonNickname(app, 0, Mon_GetBoxMon(mon));
    BufferItemName(app->msgFormat, 1, itemId);
    app->printerId = BattleCastlePartyApp_ShowMessage(app, 59, 1); // "{mon} was given the {item} to hold."
    PlaySE(SEQ_SE_DP_UG_020);
}

void BattleCastlePartyApp_CloseMenus(BattleCastlePartyApp *app) {
    if (app->flags.bits.listMenuOpen == TRUE) {
        TouchscreenListMenu_DestroyButtons(app->listMenu);
        BattleCastlePartyApp_DestroyListMenu(app);
    }
    BattleCastlePrompt_Hide(&app->yesNoPrompt);
}

void BattleCastlePartyApp_ReturnToItemList(BattleCastlePartyApp *app) {
    BattleCastlePartyApp_CloseWindow(&app->windows[6]);
    BattleCastlePartyApp_ShowItemGrid(app);
}

void BattleCastlePartyApp_PrintCastlePoints(BattleCastlePartyApp *app, Window *window) {
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    int points;
    u16 y;
    u16 x;

    BattleCastlePartyApp_GetTrainerNamePositions(app, &x1, &y1, &x2, &y2);
    if (ov80_02237D8C(app->frontierType) == FALSE) {
        x = x1 + 72;
        FillWindowPixelRect(window, 0, x, y = y1 + 1, 48, 16);
        points = sub_0205C1F0(app->frontierType);
        points = FrontierSave_GetStat(app->frontierSave, points, sub_0205C268(sub_0205C1F0(app->frontierType)));
        BattleCastlePartyApp_BufferNumber(app, 0, points, 4, 0);
        app->printerId = BattleCastlePartyApp_AddMessage(app, window, 2, x, y, 0xFF, 1, 2, 0, 0); // "{points} CP"
    } else {
        int rightPoints;
        int leftPoints;

        FillWindowPixelRect(window, 0, 64, 0, 48, 16);
        FillWindowPixelRect(window, 0, 192, 0, 48, 16);
        if (Comm_GetLocalNetId() == 0) {
            points = sub_0205C1F0(app->frontierType);
            leftPoints = FrontierSave_GetStat(app->frontierSave, points, sub_0205C268(sub_0205C1F0(app->frontierType)));
            rightPoints = app->partnerPoints;
        } else {
            leftPoints = app->partnerPoints;
            points = sub_0205C1F0(app->frontierType);
            rightPoints = FrontierSave_GetStat(app->frontierSave, points, sub_0205C268(sub_0205C1F0(app->frontierType)));
        }
        BattleCastlePartyApp_BufferNumber(app, 0, leftPoints, 4, 0);
        BattleCastlePartyApp_PrintExpandedMessage(app, window, app->msgData, 2, 112, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
        BattleCastlePartyApp_BufferNumber(app, 0, rightPoints, 4, 0);
        BattleCastlePartyApp_PrintExpandedMessage(app, window, app->msgData, 3, 240, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), 1);
    }
    ScheduleWindowCopyToVram(window);
}

// Spends the Castle Points for a heal (1-3), rental item (6, 7) or summary page (9, 10)
// once both players agreed on it; the cost is paid by whoever owns the mon.
void BattleCastlePartyApp_ApplyAction(BattleCastlePartyApp *app, u32 slot, u8 action) {
    u32 monSlot;
    u32 numOwnMons;
    int cost;

    PlaySE(SEQ_SE_DP_PIRORIRO2);
    numOwnMons = app->unk_015;
    monSlot = BattleCastle_GetSideSlot(numOwnMons, slot);
    switch (action) {
    case 1:
    case 2:
    case 3:
        cost = ov83_02247D18[action - 1];
        break;
    case 6:
    case 7:
        cost = BattleCastlePartyApp_GetRentalCostByItemId(app->selectedItem);
        break;
    case 9:
    case 10:
        cost = 0;
        break;
    }

    if (Comm_GetLocalNetId() == 0) {
        if (slot < numOwnMons) {
            BattleCastlePartyApp_BufferPlayerName(app, 5);
            ov80_02237FA4(app->frontierSave, app->frontierType, cost);
        } else {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            app->partnerPoints -= cost;
        }
    } else {
        if (slot < numOwnMons) {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            app->partnerPoints -= cost;
        } else {
            BattleCastlePartyApp_BufferPlayerName(app, 5);
            ov80_02237FA4(app->frontierSave, app->frontierType, cost);
        }
    }

    BattleCastlePartyApp_PrintCastlePoints(app, &app->windows[0]);
    BattleCastlePartyApp_CloseMenus(app);
    BattleCastlePartyApp_HideSummaryPanel(app);
    BattleCastlePartyApp_CloseItemGrid(app);
    BattleCastle_RequestBgToggle(2, 0, &app->bgToggleRequests);
    BattleCastlePartyApp_CloseWindow(&app->windows[6]);

    switch (action) {
    case 1:
    case 2:
    case 3:
        BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
        BattleCastlePartyApp_HealMon(app, monSlot, action);
        break;
    case 6:
    case 7:
        BattleCastle_DrawWindowFrame(&app->windows[6], Options_GetFrame(app->options));
        BattleCastlePartyApp_GiveRentalItem(app, monSlot, app->selectedItem);
        break;
    case 9:
        BattleCastlePartyApp_ShowMonStats(app, monSlot);
        break;
    case 10:
        BattleCastlePartyApp_ShowMonMoves(app, monSlot);
        break;
    }
}

void BattleCastlePartyApp_ShowMonStats(BattleCastlePartyApp *app, u32 slot) {
    Pokemon *mon;

    BattleCastlePartyApp_LoadStatsSummaryBg(app, GF_BG_LYR_MAIN_2);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[15]);
    mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot));
    BattleCastlePartyApp_PrintStatsSummary(app, &app->windows[4], mon);
    BattleCastle_RequestBgToggle(2, 1, &app->bgToggleRequests);
}

void BattleCastlePartyApp_ShowMonMoves(BattleCastlePartyApp *app, u32 slot) {
    Pokemon *mon;

    BattleCastlePartyApp_LoadMovesSummaryBg(app, GF_BG_LYR_MAIN_2);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[15]);
    mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot));
    BattleCastlePartyApp_PrintMovesSummary(app, &app->windows[4], mon);
    BattleCastle_RequestBgToggle(2, 1, &app->bgToggleRequests);
}

void BattleCastlePartyApp_HideSummaryPanel(BattleCastlePartyApp *app) {
    BattleCastle_RequestBgToggle(2, 0, &app->bgToggleRequests);
}

void BattleCastlePartyApp_UpdateMonIcons(BattleCastlePartyApp *app) {
    int count = ov80_02237B24(app->frontierType, 1);
    int i;

    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(app->party, i);
        u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
        u32 maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        u32 hpState = ov80_0222A43C(hp, maxHp);

        if (app->monIcons[i] != NULL) {
            ov83_02247600(app->monIcons[i], hpState);
            if (i != BattleCastle_GetMonIndex(app->numMons, app->cursorPos) || app->cursorPos >= app->unk_015) {
                ov83_0224760C(app->monIcons[i], 0);
            } else {
                ov83_0224760C(app->monIcons[i], 1);
            }
        }
    }
}

#pragma force_active on
// Raises the heal (action 4) or rental rank of whoever owns the mon in slot and pays for it
void BattleCastlePartyApp_RankUp(BattleCastlePartyApp *app, u8 slot, u16 action) {
    u8 kind;
    u8 numOwnMons;
    int rank;

    ov80_02237B24(app->frontierType, 0);
    kind = action != CASTLE_PARTY_ACTION_HEAL_RANK_UP ? CASTLE_RANK_RENTAL : CASTLE_RANK_HEAL;
    numOwnMons = app->unk_015;
    BattleCastle_GetSideSlot(numOwnMons, slot);
    if (Comm_GetLocalNetId() == 0) {
        if (slot < numOwnMons) {
            FrontierSave *frontierSave;
            int stat;

            BattleCastlePartyApp_BufferPlayerName(app, 5);
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            ov80_02237FA4(app->frontierSave, app->frontierType, ov83_02247D48[kind][rank]);
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            frontierSave = Save_Frontier_GetStatic(app->saveData);
            stat = sub_0205C174(app->frontierType, kind);
            sub_02031108(frontierSave, stat, sub_0205C268(sub_0205C174(app->frontierType, kind)), (u16)(rank + 1));
            if (ov80_02237D8C(app->frontierType) == TRUE) {
                if (action == CASTLE_PARTY_ACTION_HEAL_RANK_UP) {
                    app->flags.bits.linkResume = 1;
                } else {
                    app->flags.bits.linkResume = 2;
                }
            }
        } else {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            rank = app->partnerRanks[kind];
            app->partnerPoints -= ov83_02247D48[kind][rank];
            app->partnerRanks[kind]++;
        }
    } else {
        if (slot < numOwnMons) {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            rank = app->partnerRanks[kind];
            app->partnerPoints -= ov83_02247D48[kind][rank];
            app->partnerRanks[kind]++;
        } else {
            FrontierSave *frontierSave;
            int stat;

            BattleCastlePartyApp_BufferPlayerName(app, 5);
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            ov80_02237FA4(app->frontierSave, app->frontierType, ov83_02247D48[kind][rank]);
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            frontierSave = Save_Frontier_GetStatic(app->saveData);
            stat = sub_0205C174(app->frontierType, kind);
            sub_02031108(frontierSave, stat, sub_0205C268(sub_0205C174(app->frontierType, kind)), (u16)(rank + 1));
            if (ov80_02237D8C(app->frontierType) == TRUE) {
                if (action == CASTLE_PARTY_ACTION_HEAL_RANK_UP) {
                    app->flags.bits.linkResume = 1;
                } else {
                    app->flags.bits.linkResume = 2;
                }
            }
        }
    }
    BattleCastlePartyApp_CloseMenus(app);
    BattleCastlePartyApp_PrintCastlePoints(app, &app->windows[CASTLE_PARTY_WIN_POINTS]);
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_PARTY_WIN_MESSAGE], Options_GetFrame(app->options));
    app->printerId = BattleCastlePartyApp_ShowMessage(app, ov83_02247D5A[kind][rank], 1);
}
#pragma force_active reset

void BattleCastlePartyApp_PrintExpandedMessage(BattleCastlePartyApp *app, Window *window, MsgData *msgData, int msgNo, int x, int y, int fontId, int color, int alignment) {
    String *string = NewString_ReadMsgData(msgData, msgNo);

    StringExpandPlaceholders(app->msgFormat, app->expandedString, string);
    BattleCastle_PrintString(window, app->expandedString, x, y, fontId, color, alignment);
    String_Delete(string);
}

#pragma force_active on
void BattleCastlePartyApp_LoadMonSummary(BattleCastlePartyApp *app) {
    Pokemon *mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, app->cursorPos));
    BOOL lock = AcquireMonLock(mon);
    u16 i;

    app->summary.mon = mon;
    app->summary.boxMon = Mon_GetBoxMon(mon);
    app->summary.species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    app->summary.level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    app->summary.ability = GetMonData(mon, MON_DATA_ABILITY, NULL);
    app->summary.nature = GetMonNature(mon);
    app->summary.heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    app->summary.hp = GetMonData(mon, MON_DATA_HP, NULL);
    app->summary.maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    app->summary.attack = GetMonData(mon, MON_DATA_ATK, NULL);
    app->summary.spAttack = GetMonData(mon, MON_DATA_SP_ATK, NULL);
    app->summary.defense = GetMonData(mon, MON_DATA_DEF, NULL);
    app->summary.spDefense = GetMonData(mon, MON_DATA_SP_DEF, NULL);
    app->summary.speed = GetMonData(mon, MON_DATA_SPEED, NULL);
    app->summary.form = GetMonData(mon, MON_DATA_FORM, NULL);
    app->summary.personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == 1) {
        app->summary.unk_0A_7 = 0;
    } else {
        app->summary.unk_0A_7 = 1;
    }
    app->summary.gender = GetMonGender(mon);
    for (i = 0; i < 4; i++) {
        app->summary.moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
        app->summary.pp[i] = GetMonData(mon, MON_DATA_MOVE1_PP + i, NULL);
        {
            u16 ppUps = GetMonData(mon, MON_DATA_MOVE1_PP_UPS + i, NULL);
            app->summary.maxPp[i] = GetMoveMaxPP(app->summary.moves[i], ppUps);
        }
    }
    ReleaseMonLock(mon, lock);
}
#pragma force_active reset

void BattleCastlePartyApp_PrintSummaryLabels(BattleCastlePartyApp *app) {
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_LEVEL_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ABILITY_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_NATURE_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_HP_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ATTACK_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SP_ATK_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_DEFENSE_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SP_DEF_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SPEED_LABEL], 0);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_LEVEL_LABEL], app->msgData, msg_0031_00088, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_ABILITY_LABEL], app->msgData, msg_0031_00074, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_NATURE_LABEL], app->msgData, msg_0031_00072, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_ITEM_LABEL], app->msgData, msg_0031_00070, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_HP_LABEL], app->msgData, msg_0031_00089, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_ATTACK_LABEL], app->msgData, msg_0031_00076, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_SP_ATK_LABEL], app->msgData, msg_0031_00080, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_DEFENSE_LABEL], app->msgData, msg_0031_00078, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_SP_DEF_LABEL], app->msgData, msg_0031_00082, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_SPEED_LABEL], app->msgData, msg_0031_00084, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_LEVEL_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ABILITY_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_NATURE_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ITEM_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_HP_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ATTACK_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SP_ATK_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_DEFENSE_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SP_DEF_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SPEED_LABEL]);
}

#pragma force_active on
#pragma opt_strength_reduction off
// Prints the values of the summary panel (scheduleCopy == TRUE: schedule the copy, else copy now)
void BattleCastlePartyApp_PrintMonSummary(BattleCastlePartyApp *app, int scheduleCopy) {
    u32 i;
    u32 move;
    u16 halfSlashWidth;
    u16 center;
    u8 *cursor;
    String *slash;
    String *format;

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_NICKNAME], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_GENDER], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_LEVEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ABILITY], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_NATURE], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_HP], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ATTACK], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SP_ATK], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_DEFENSE], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SP_DEF], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_SPEED], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 0], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 1], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 2], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 3], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 0], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 1], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 2], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 3], 0);

    BufferBoxMonNickname(app->msgFormat, 0, Mon_GetBoxMon(app->summary.mon));
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_NICKNAME], app->msgData, msg_0031_00091, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    if (app->summary.unk_0A_7 == 0) {
        if (app->summary.gender == MON_MALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_GENDER], app->msgData, msg_0031_00086, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR_MALE, CASTLE_ALIGN_LEFT);
        } else if (app->summary.gender == MON_FEMALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_PARTY_WIN_GENDER], app->msgData, msg_0031_00087, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR_FEMALE, CASTLE_ALIGN_LEFT);
        }
    }
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.level, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_LEVEL], app->msgData, msg_0031_00094, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BufferAbilityName(app->msgFormat, 0, app->summary.ability);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_ABILITY], app->msgData, msg_0031_00075, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BufferNatureName(app->msgFormat, 0, app->summary.nature);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_NATURE], app->msgData, msg_0031_00073, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BufferItemName(app->msgFormat, 0, app->summary.heldItem);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_ITEM], app->msgData, msg_0031_00071, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);

    // stats are right-aligned
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.hp, 3, 0);
    BattleCastlePartyApp_BufferNumber(app, 1, app->summary.maxHp, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_HP], app->msgData, msg_0031_00095, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_HP]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.attack, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_ATTACK], app->msgData, msg_0031_00077, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_ATTACK]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.spAttack, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_SP_ATK], app->msgData, msg_0031_00081, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_SP_ATK]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.defense, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_DEFENSE], app->msgData, msg_0031_00079, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_DEFENSE]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.spDefense, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_SP_DEF], app->msgData, msg_0031_00083, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_SP_DEF]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    BattleCastlePartyApp_BufferNumber(app, 0, app->summary.speed, 3, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_SPEED], app->msgData, msg_0031_00085, GetWindowWidth(&app->windows[CASTLE_PARTY_WIN_SPEED]) * 8, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_RIGHT);

    // moves: name, then "pp / maxPp" centered on the slash ("--" for an empty slot)
    // (summary.moves[move] is read through a cursor stepping over the app; keeps the original codegen)
    for (move = 0, cursor = (u8 *)app; move < 4; move++, cursor += sizeof(u16)) {
        BufferMoveName(app->msgFormat, move, *(u16 *)(cursor + offsetof(BattleCastlePartyApp, summary.moves)));
        BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + move], app->msgData, msg_0031_00096 + move, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        if (*(u16 *)(cursor + offsetof(BattleCastlePartyApp, summary.moves)) == MOVE_NONE) {
            Window *window = &app->windows[CASTLE_PARTY_WIN_MOVE_PP + move];
            int width = GetWindowWidth(window);

            BattleCastle_PrintMsg(window, app->msgData, msg_0031_00102, (width * 8) / 2, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_CENTER);
        } else {
            Window *window;

            slash = NewString_ReadMsgData(app->msgData, msg_0031_00090);
            halfSlashWidth = (u16)(FontID_String_GetWidth(0, slash, 0) / 2);
            window = &app->windows[CASTLE_PARTY_WIN_MOVE_PP + move];
            center = (u16)((GetWindowWidth(window) * 8) / 2);
            BattleCastle_PrintString(window, slash, center, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_CENTER);
            String_Delete(slash);

            BattleCastlePartyApp_BufferNumber(app, 0, app->summary.pp[move], 2, 0);
            format = NewString_ReadMsgData(app->msgData, msg_0031_00101);
            StringExpandPlaceholders(app->msgFormat, app->expandedString, format);
            BattleCastle_PrintString(window, app->expandedString, (s32)center - halfSlashWidth - FontID_String_GetWidth(0, app->expandedString, 0), 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
            String_Delete(format);
            BattleCastlePartyApp_BufferNumber(app, 0, app->summary.maxPp[move], 2, 0);
            BattleCastlePartyApp_PrintExpandedMessage(app, window, app->msgData, msg_0031_00101, (s32)center + halfSlashWidth, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        }
    }

    if (scheduleCopy == TRUE) {
        Window *window;

        for (i = CASTLE_PARTY_WIN_SUMMARY_FIRST, window = &app->windows[CASTLE_PARTY_WIN_SUMMARY_FIRST]; i <= CASTLE_PARTY_WIN_SUMMARY_LAST; i++, window++) {
            ScheduleWindowCopyToVram(window);
        }
    } else {
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_NICKNAME]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_GENDER]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_LEVEL]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ABILITY]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_NATURE]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ITEM]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_HP]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_ATTACK]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SP_ATK]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_DEFENSE]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SP_DEF]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_SPEED]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 0]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 1]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 2]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_NAMES + 3]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 0]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 1]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 2]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_PARTY_WIN_MOVE_PP + 3]);
    }
}
#pragma opt_strength_reduction reset
#pragma force_active reset

void BattleCastlePartyApp_RefreshMonSummary(BattleCastlePartyApp *app, int scheduleCopy) {
    BattleCastlePartyApp_LoadMonSummary(app);
    BattleCastlePartyApp_PrintMonSummary(app, scheduleCopy);
    BattleCastleSprite_LoadMonPic(app->monPic, app->summary.boxMon, app->summary.species, app->summary.personality);
}

void BattleCastlePartyApp_ShowMonSummary(BattleCastlePartyApp *app, int show) {
    if (show == TRUE) {
        u32 i;
        Window *window;

        for (i = CASTLE_PARTY_WIN_SUMMARY_FIRST, window = &app->windows[CASTLE_PARTY_WIN_SUMMARY_FIRST]; i <= CASTLE_PARTY_WIN_SUMMARY_LAST; i++, window++) {
            ScheduleWindowCopyToVram(window);
        }
        BattleCastleSprite_SetDrawFlag(app->monPic, TRUE);
    } else {
        Window *window;
        u32 i;

        for (i = CASTLE_PARTY_WIN_SUMMARY_FIRST, window = &app->windows[CASTLE_PARTY_WIN_SUMMARY_FIRST]; i <= CASTLE_PARTY_WIN_SUMMARY_LAST; i++, window++) {
            ClearWindowTilemapAndScheduleTransfer(window);
        }
        BattleCastleSprite_SetDrawFlag(app->monPic, FALSE);
    }
}

// Item buttons: two columns, three rows
void BattleCastlePartyApp_GetItemButtonPos(u32 slot, s16 *x, s16 *y) {
    *x = (slot & 1) * 128 + 20;
    *y = (slot >> 1) * 40 + 56;
}

// Rental item chosen: hide the other buttons and show its name and cost
void BattleCastlePartyApp_ShowSelectedItem(BattleCastlePartyApp *app, u32 selected) {
    void *screenRaw;
    NNSG2dScreenData *screen;
    u32 i;
    u32 item;
    u32 cost;

    for (i = 0; i < CASTLE_ITEMS_PER_PAGE; i++) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[CASTLE_PARTY_WIN_ITEM_GRID + i]);
        if (i != selected) {
            BattleCastleSprite_SetDrawFlag(app->itemIcons[i], FALSE);
        } else {
            BattleCastleSprite_SetPosition(app->itemIcons[i], 68, 76);
        }
    }
    BattleCastleSprite_SetDrawFlag(app->itemCursor, FALSE);
    screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(app->narc, CASTLE_PARTY_NARC_ITEM_DETAIL_SCREEN, TRUE, &screen, HEAP_ID_107);
    LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, 0, 0, 32, 24);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
    Heap_Free(screenRaw);

    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM_NAME], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_PARTY_WIN_ITEM_COST], 0);
    item = CASTLE_ITEMS_PER_PAGE * app->itemPage;
    item += selected;
    AddTextPrinterParameterizedWithColor(&app->windows[CASTLE_PARTY_WIN_ITEM_NAME], 0, app->itemList[item].text, 0, 0, TEXT_SPEED_NOTRANSFER, CASTLE_PARTY_TEXT_COLOR, NULL);
    cost = BattleCastlePartyApp_GetRentalCost(app, (u16)app->itemList[item].value, app->action);
    BattleCastlePartyApp_BufferNumber(app, 0, cost, 2, 0);
    BattleCastlePartyApp_PrintExpandedMessage(app, &app->windows[CASTLE_PARTY_WIN_ITEM_COST], app->msgData, msg_0031_00104, 0, 0, 0, CASTLE_PARTY_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_ITEM_NAME]);
    ScheduleWindowCopyToVram(&app->windows[CASTLE_PARTY_WIN_ITEM_COST]);
}

// Back from BattleCastlePartyApp_ShowSelectedItem to the item grid
void BattleCastlePartyApp_ShowItemGrid(BattleCastlePartyApp *app) {
    void *screenRaw;
    NNSG2dScreenData *screen;
    u32 i;
    u32 firstItem;
    u16 pos[2];

    ClearWindowTilemapAndScheduleTransfer(&app->windows[CASTLE_PARTY_WIN_ITEM_NAME]);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[CASTLE_PARTY_WIN_ITEM_COST]);
    screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(app->narc, CASTLE_PARTY_NARC_ITEM_GRID_SCREEN, TRUE, &screen, HEAP_ID_107);
    LoadRectToBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, 0, 0, 32, 24);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
    Heap_Free(screenRaw);

    firstItem = CASTLE_ITEMS_PER_PAGE * app->itemPage;
    for (i = 0; i < CASTLE_ITEMS_PER_PAGE; i++) {
        BattleCastlePartyApp_GetItemButtonPos(i, &pos[1], &pos[0]);
        BattleCastleSprite_SetPosition(app->itemIcons[i], pos[1], pos[0]);
        if (firstItem + i < app->numItems) {
            BattleCastleSprite_SetDrawFlag(app->itemIcons[i], TRUE);
        }
    }
    BattleCastleSprite_SetDrawFlag(app->itemCursor, TRUE);
    BattleCastlePartyApp_PrintItemNames(app);
    BattleCastlePartyApp_PrintItemPageNumber(app);
    BattleCastlePartyApp_PrintReturnButton(app);
}

void BattleCastlePartyApp_SetCursorPos(BattleCastlePartyApp *app, u32 cursorPos, u32 prevCursorPos) {
    app->cursorPos = cursorPos;
    app->prevCursorPos = prevCursorPos;
    BattleCastlePartyApp_MoveCursor(app);
}

#pragma force_active on
void BattleCastlePartyApp_SetItemCursor(BattleCastlePartyApp *app, int x, int y, int seq) {
    BattleCastleSprite_SetPosition(app->itemCursor, x, y);
    BattleCastleSprite_SetAnim(app->itemCursor, seq);
}
#pragma force_active reset

void BattleCastlePartyApp_ShowItemDescription(BattleCastlePartyApp *app, int button) {
    BattleCastlePartyApp_PrintItemDescription(app, (u16)button);
}

void BattleCastlePartyApp_CreateListMenu(BattleCastlePartyApp *app, int count, u8 x, u8 y, u8 width) {
    TouchscreenListMenuTemplate template;
    TouchscreenListMenuHeader header;
    u16 tileStart;
    u16 tileCount;
    int numMons;

    BattleCastle_GetMenuTiles(&tileStart, &tileCount);
    template = *(const TouchscreenListMenuTemplate *)&_02247D0C[15];
    template.baseTile = tileStart;
    template.charOffset = tileStart + tileCount - 27;
    template.unkA = tileCount;
    header.template = template;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = count;
    app->listMenu = BattleCastle_CreateListMenu(app->listMenuSpawner, &header, x, y, width);
    app->flags.bits.listMenuOpen = TRUE;
    numMons = ov80_02237B24(app->frontierType, 1);
    BattleCastle_SetSpritesTranslucent(app->monIcons, numMons, 1);
    BattleCastle_SetSpritesTranslucent(app->heldItemIcons, numMons, 1);
}

void BattleCastlePartyApp_CreateMainMenuList(BattleCastlePartyApp *app) {
    u32 i;

    app->listMenuItems = ListMenuItems_New(3, HEAP_ID_107);
    for (i = 0; i < 3; i++) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247D6C[i].msgId, ov83_02247D6C[i].value);
    }
    BattleCastlePartyApp_CreateListMenu(app, 3, 17, 8, 13);
}

// Options above the current heal rank are left out; RANK UP is left out at the max rank
void BattleCastlePartyApp_CreateHealMenuList(BattleCastlePartyApp *app) {
    u8 count;
    u8 rank;
    u8 i;

    app->listMenuItems = ListMenuItems_New(5, HEAP_ID_107);
    rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
    count = 0;
    for (i = 0; i < 5; i++) {
        switch (ov83_02247F4C[i].value) {
        case 1:
        case 2:
        case 3:
            if (rank >= ov83_02247F4C[i].minRank) {
                ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247F4C[i].msgId, ov83_02247F4C[i].value);
                count++;
            }
            break;
        case CASTLE_PARTY_ACTION_HEAL_RANK_UP:
            if (rank != CASTLE_RANK_MAX) {
                ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247F4C[i].msgId, ov83_02247F4C[i].value);
                count++;
            }
            break;
        default:
            ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247F4C[i].msgId, ov83_02247F4C[i].value);
            count++;
            break;
        }
    }
    BattleCastlePartyApp_CreateListMenu(app, count, 17, ov83_02248008[count], 13);
}

// Options above the current rental rank are left out; RANK UP is left out at the max rank
void BattleCastlePartyApp_CreateRentalMenuList(BattleCastlePartyApp *app) {
    u8 count;
    u8 rank;
    u8 i;

    app->listMenuItems = ListMenuItems_New(4, HEAP_ID_107);
    rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
    count = 0;
    for (i = 0; i < 4; i++) {
        switch (ov83_02247EB0[i].value) {
        case CASTLE_PARTY_ACTION_RENT_BERRY:
        case CASTLE_PARTY_ACTION_RENT_ITEM:
            if (rank >= ov83_02247EB0[i].minRank) {
                ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247EB0[i].msgId, ov83_02247EB0[i].value);
                count++;
            }
            break;
        case CASTLE_PARTY_ACTION_RENTAL_RANK_UP:
            if (rank != CASTLE_RANK_MAX) {
                ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247EB0[i].msgId, ov83_02247EB0[i].value);
                count++;
            }
            break;
        default:
            ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, ov83_02247EB0[i].msgId, ov83_02247EB0[i].value);
            count++;
            break;
        }
    }
    BattleCastlePartyApp_CreateListMenu(app, count, 17, ov83_02248008[count], 13);
}

void BattleCastlePartyApp_DestroyListMenu(BattleCastlePartyApp *app) {
    int numMons = ov80_02237B24(app->frontierType, 1);

    BattleCastle_SetSpritesTranslucent(app->monIcons, numMons, 0);
    BattleCastle_SetSpritesTranslucent(app->heldItemIcons, numMons, 0);
    BattleCastle_DestroyListMenu(app->listMenu);
    ListMenuItems_Delete(app->listMenuItems);
    app->flags.bits.listMenuOpen = FALSE;
}

// Help text for the entry under the cursor, reprinted when the cursor moves
void BattleCastlePartyApp_PrintMainMenuHelp(BattleCastlePartyApp *app) {
    u8 cursorPos = app->listMenu->cursorPos;

    if (app->listCursorPos != cursorPos) {
        BattleCastlePartyApp_PrintMessage(app, &app->windows[CASTLE_PARTY_WIN_MESSAGE], ov83_02247D1E[cursorPos], 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
        app->listCursorPos = app->listMenu->cursorPos;
    }
}

void BattleCastlePartyApp_PrintHealMenuHelp(BattleCastlePartyApp *app) {
    u8 cursorPos = app->listMenu->cursorPos;
    u8 rank;
    int msgNo;

    if (app->listCursorPos != cursorPos) {
        switch ((u32)app->listMenuItems[cursorPos].value) {
        case CASTLE_PARTY_ACTION_HEAL_RANK_UP:
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_HEAL);
            if (rank == CASTLE_RANK_MAX) {
                msgNo = msg_0031_00027; // "Your rank cannot be raised any higher."
            } else if (rank == 1) {
                msgNo = msg_0031_00025; // "...restore PP by going up in rank."
            } else {
                msgNo = msg_0031_00026; // "...restore both HP and PP by going up in rank."
            }
            break;
        case (u32)LIST_CANCEL:
            msgNo = msg_0031_00028; // "Return to previous selections."
            break;
        default:
            msgNo = msg_0031_00024; // "Which would you like to use?"
            break;
        }
        BattleCastlePartyApp_PrintMessage(app, &app->windows[CASTLE_PARTY_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
        app->listCursorPos = app->listMenu->cursorPos;
    }
}

void BattleCastlePartyApp_PrintRentalMenuHelp(BattleCastlePartyApp *app) {
    u8 cursorPos = app->listMenu->cursorPos;
    u8 rank;
    int msgNo;

    if (app->listCursorPos != cursorPos) {
        switch (app->listMenuItems[cursorPos].value) {
        case CASTLE_PARTY_ACTION_RENT_BERRY:
            msgNo = msg_0031_00048; // "You may rent a Berry for this Pokémon."
            break;
        case CASTLE_PARTY_ACTION_RENT_ITEM:
            msgNo = msg_0031_00049; // "You may rent an item for this Pokémon."
            break;
        case CASTLE_PARTY_ACTION_RENTAL_RANK_UP:
            rank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_RENTAL);
            if (rank == CASTLE_RANK_MAX) {
                msgNo = msg_0031_00052; // "Your rank cannot be raised any higher."
            } else if (rank == 1) {
                msgNo = msg_0031_00050; // "...rent an item ... by going up in rank."
            } else {
                msgNo = msg_0031_00051; // "...choices ... will be expanded by going up in rank."
            }
            break;
        default:
            msgNo = msg_0031_00053; // "Return to previous selections."
            break;
        }
        BattleCastlePartyApp_PrintMessage(app, &app->windows[CASTLE_PARTY_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
        app->listCursorPos = app->listMenu->cursorPos;
    }
}

void BattleCastlePartyApp_StartButtonAnim(BattleCastlePartyApp *app, u16 button) {
    app->pressedButton = button;
    app->buttonAnimState = 0;
    app->buttonAnimTimer = 0;
}
