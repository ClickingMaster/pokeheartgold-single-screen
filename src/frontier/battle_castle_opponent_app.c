#include "global.h"

#include "constants/moves.h"
#include "constants/sndseq.h"

#include "frontier/battle_arcade_data.h"
#include "frontier/battle_castle.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0033.h"

#include "battle_frontier_communication.h"
#include "bg_window.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "font.h"
#include "frontier_save_access.h"
#include "frontier_stat_ids_legacy.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "link_synchronization.h"
#include "message_format.h"
#include "message_printer.h"
#include "message_printer_number.h"
#include "move.h"
#include "msgdata.h"
#include "oam_manager.h"
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
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite.h"
#include "string_util.h"
#include "system.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

FS_EXTERN_OVERLAY(OVY_80);

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
#define TEXT_SPEED_NOTRANSFER       0xFF

// Battle Castle opponent app: spend Castle Points on
// information about the opponent's party (CHECK / LEVEL / SUMMARY). Texts: msg_0033.
// The file ends with the sprite, prompt, window and grid helpers both Battle Castle apps use.

// Main loop states (*state of Main)
#define CASTLE_OPP_MAIN_FADE_IN     0
#define CASTLE_OPP_MAIN_MENU        1
#define CASTLE_OPP_MAIN_LINK_ACTION 2
#define CASTLE_OPP_MAIN_LINK_EXIT   3
#define CASTLE_OPP_MAIN_FADE_OUT    4

// Menu states (app->subState in BattleCastleOpponentApp_HandleMenu)
#define CASTLE_OPP_STATE_RESUME               0
#define CASTLE_OPP_STATE_SELECT_MON           1
#define CASTLE_OPP_STATE_MAIN_MENU            2
#define CASTLE_OPP_STATE_CONFIRM_CHECK        3
#define CASTLE_OPP_STATE_LEVEL_MENU           4
#define CASTLE_OPP_STATE_CONFIRM_LEVEL        5
#define CASTLE_OPP_STATE_SUMMARY_MENU         6
#define CASTLE_OPP_STATE_CONFIRM_STATS        7
#define CASTLE_OPP_STATE_CONFIRM_MOVES        8
#define CASTLE_OPP_STATE_CONFIRM_RANK_UP      9
#define CASTLE_OPP_STATE_RANK_UP              10
#define CASTLE_OPP_STATE_CHECK_DONE           11
#define CASTLE_OPP_STATE_ACTION_EFFECT        12
#define CASTLE_OPP_STATE_BUY_EFFECT           13
#define CASTLE_OPP_STATE_SHOW_BOUGHT_PAGE     14
#define CASTLE_OPP_STATE_SUMMARY_MENU_MESSAGE 15
#define CASTLE_OPP_STATE_MESSAGE              16
#define CASTLE_OPP_STATE_STATS_PAGE           17
#define CASTLE_OPP_STATE_MOVES_PAGE           18

// List menu item values / app->action
#define CASTLE_OPP_ACTION_CHECK   0
#define CASTLE_OPP_ACTION_LEVEL   1
#define CASTLE_OPP_ACTION_SUMMARY 2
#define CASTLE_OPP_ACTION_STATS   3
#define CASTLE_OPP_ACTION_MOVES   4
#define CASTLE_OPP_ACTION_RANK_UP 5

// LEVEL menu item values
#define CASTLE_OPP_LEVEL_MENU_UP     0
#define CASTLE_OPP_LEVEL_MENU_DOWN   1
#define CASTLE_OPP_LEVEL_MENU_CANCEL 2

// Level change of an opponent mon (app->levelChanges[], app->levelChange)
#define CASTLE_LEVEL_NORMAL 0 // level 50
#define CASTLE_LEVEL_UP     1 // level 55
#define CASTLE_LEVEL_DOWN   2 // level 45

// Party grid: positions 0-3 are the party, then the EXIT button
#define CASTLE_OPP_GRID_EXIT 4

// app->listCursorPos before the list menu reported a position (never equals a real one)
#define CASTLE_OPP_LIST_POS_NONE (-2)

// BattleCastle_GetRank kinds
#define CASTLE_RANK_INFO 2

// CP costs
#define CASTLE_CP_CHECK   1
#define CASTLE_CP_STATS   2
#define CASTLE_CP_MOVES   5
#define CASTLE_CP_RANK_UP 50

// BattleCastleOpponentApp_SendLinkCommand link commands
#define CASTLE_OPP_LINK_CMD_START  20
#define CASTLE_OPP_LINK_CMD_ACTION 21
#define CASTLE_OPP_LINK_CMD_CURSOR 22
#define CASTLE_OPP_LINK_CMD_EXIT   23

// LinkSync_StartSync / LinkSync_IsSynced link sync ids
#define CASTLE_OPP_SYNC_START  216
#define CASTLE_OPP_SYNC_ACTION 133
#define CASTLE_OPP_SYNC_EXIT   134

// app->linkResume values: menu to reopen after a link exchange
#define CASTLE_OPP_RESUME_NONE         0
#define CASTLE_OPP_RESUME_SELECT_MON   1
#define CASTLE_OPP_RESUME_SUMMARY_MENU 2
#define CASTLE_OPP_RESUME_BOUGHT_PAGE  3

// Windows (app->windows, window set 1)
#define CASTLE_OPP_WIN_POINTS        0
#define CASTLE_OPP_WIN_TRAINER_NAMES 1
#define CASTLE_OPP_WIN_LEVELS        2
#define CASTLE_OPP_WIN_HP            3
#define CASTLE_OPP_WIN_MESSAGE       7
#define CASTLE_OPP_WIN_CHOOSE_PROMPT 8

// Summary panel of the selected mon (12-34)
#define CASTLE_OPP_WIN_SUMMARY_FIRST 12
#define CASTLE_OPP_WIN_SPECIES       12
#define CASTLE_OPP_WIN_GENDER        13
#define CASTLE_OPP_WIN_LEVEL_LABEL   14
#define CASTLE_OPP_WIN_LEVEL         15
#define CASTLE_OPP_WIN_ABILITY_LABEL 16
#define CASTLE_OPP_WIN_ABILITY       17
#define CASTLE_OPP_WIN_NATURE_LABEL  18
#define CASTLE_OPP_WIN_NATURE        19
#define CASTLE_OPP_WIN_ITEM_LABEL    20
#define CASTLE_OPP_WIN_ITEM          21
#define CASTLE_OPP_WIN_HP_LABEL      22
#define CASTLE_OPP_WIN_SUMMARY_HP    23
#define CASTLE_OPP_WIN_ATTACK_LABEL  24
#define CASTLE_OPP_WIN_ATTACK        25
#define CASTLE_OPP_WIN_SP_ATK_LABEL  26
#define CASTLE_OPP_WIN_SP_ATK        27
#define CASTLE_OPP_WIN_DEFENSE_LABEL 28
#define CASTLE_OPP_WIN_DEFENSE       29
#define CASTLE_OPP_WIN_SP_DEF_LABEL  30
#define CASTLE_OPP_WIN_SP_DEF        31
#define CASTLE_OPP_WIN_SPEED_LABEL   32
#define CASTLE_OPP_WIN_SPEED         33
#define CASTLE_OPP_WIN_MOVES         34
#define CASTLE_OPP_WIN_SUMMARY_LAST  34

#define CASTLE_OPP_TEXT_COLOR           MAKE_TEXT_COLOR(1, 2, 0)
#define CASTLE_OPP_TEXT_COLOR_MALE      MAKE_TEXT_COLOR(5, 6, 0) // male symbol
#define CASTLE_OPP_TEXT_COLOR_FEMALE    MAKE_TEXT_COLOR(3, 4, 0) // female symbol and names
#define CASTLE_OPP_TEXT_COLOR_MALE_NAME MAKE_TEXT_COLOR(7, 8, 0)

typedef struct BattleCastleOpponentAppArgs {
    SaveData *saveData; // 0x00
    u8 frontierType;    // 0x04
    u8 unk_05[3];       // 0x05
    u8 checked[4];      // 0x08
    u8 levelChanges[4]; // 0x0C
    u8 statsBought[4];  // 0x10
    u8 movesBought[4];  // 0x14
    u8 unk_18[4];       // 0x18
    Party *party;       // 0x1C
    u16 selection;      // 0x20
    u8 unk_22[6];       // 0x22
    u16 partnerPoints;  // 0x28
} BattleCastleOpponentAppArgs;

// Data tables in assembly
extern const u8 gBattleCastlePartyAppButtonRects[][4]; // party app button rects (x, y, width, height in tiles)
extern const u8 gBattleCastleOpponentAppListMenuY[];   // list menu y, by number of entries
extern const TouchscreenListMenuTemplate gBattleCastleOpponentAppListMenuTemplate;
extern const u16 gBattleCastleOpponentAppStatsHelpMessages[2]; // SUMMARY menu help messages, by "moves can be bought" (0) or not (1)
extern const u16 gBattleCastleOpponentAppMovesHelpMessages[2];
extern const u16 gBattleCastleOpponentAppRankUpHelpMessages[2];
extern const u16 gBattleCastleOpponentAppCancelHelpMessages[2];
extern const GraphicsModes gBattleCastleOpponentAppGraphicsModes;
extern const u16 gBattleCastleOpponentAppRankUpMessages[][3]; // "gained the ability to..." messages, by rank kind and rank
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Main1;
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Main0;
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Main2;
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Main3;
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Sub0;
extern const BgTemplate gBattleCastleOpponentAppBgTemplate_Sub1;
extern const GraphicsBanks gBattleCastleOpponentAppGraphicsBanks;
u32 Frontier_GetHpGaugeFrame(u16 hp, u16 maxHp);
void Frontier_BufferPartnerName(MessageFormat *msgFormat, int value);
int BattleCastle_GetPartySize(u8 frontierType, int kind);
u8 BattleCastle_GetOpponentMonCount(u8 frontierType, int kind); // number of opponent mons (defined with an int result)
void BattleCastle_SpendPoints(FrontierSave *frontierSave, int frontierType, int points);

enum {
    CASTLE_OPP_FADE_IN_START_SYNC,
    CASTLE_OPP_FADE_IN_WAIT_SYNC,
    CASTLE_OPP_FADE_IN_SEND_START,
    CASTLE_OPP_FADE_IN_WAIT_PARTNER,
    CASTLE_OPP_FADE_IN_WAIT_FADE,
};

enum {
    CASTLE_OPP_LINK_ACTION_SEND,
    CASTLE_OPP_LINK_ACTION_APPLY,
    CASTLE_OPP_LINK_ACTION_PLAY_EFFECT,
    CASTLE_OPP_LINK_ACTION_DELAY,
    CASTLE_OPP_LINK_ACTION_WAIT_SYNC,
    CASTLE_OPP_LINK_ACTION_FINISH,
};

enum {
    CASTLE_OPP_LINK_EXIT_SEND,
    CASTLE_OPP_LINK_EXIT_DELAY,
    CASTLE_OPP_LINK_EXIT_WAIT_SYNC,
};

enum {
    CASTLE_OPP_FADE_OUT_START,
    CASTLE_OPP_FADE_OUT_WAIT,
};

// Entry points and link receive callbacks
void BattleCastleOpponentApp_ReceiveRanks(int netId, void *unused, u16 *data, BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveAction(int netId, int unused, u16 *data, BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveCursor(int netId, int unused, u16 *data, BattleCastleOpponentApp *app);
void BattleCastleOpponentApp_ReceiveExit(int netId, int unused, u16 *data, BattleCastleOpponentApp *app);
static BOOL BattleCastleOpponentApp_FadeIn(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_DrawScreen(BattleCastleOpponentApp *app);
static BOOL BattleCastleOpponentApp_HandleMenu(BattleCastleOpponentApp *app);
static BOOL BattleCastleOpponentApp_LinkAction(BattleCastleOpponentApp *app);
static BOOL BattleCastleOpponentApp_LinkExit(BattleCastleOpponentApp *app);
static BOOL BattleCastleOpponentApp_FadeOut(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_FreeResources(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_DisableDisplay(void);
static void BattleCastleOpponentApp_InitGraphics(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_InitBgGraphics(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_InitSpriteGfx(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_FreeBgLayers(BgConfig *bgConfig);
static void BattleCastleOpponentApp_VBlankCallback(void *arg);
static void BattleCastleOpponentApp_SetGraphicsBanks(void);
static void BattleCastleOpponentApp_InitBgLayers(BgConfig *bgConfig);
static void BattleCastleOpponentApp_LoadPartyBg(BattleCastleOpponentApp *app, GFBgLayer bgId);
static void BattleCastleOpponentApp_LoadBgPalette(void);
static void BattleCastleOpponentApp_LoadSummaryBg(BattleCastleOpponentApp *app, GFBgLayer bgId);
static void BattleCastleOpponentApp_LoadSummaryBgPalette(void);
static void BattleCastleOpponentApp_LoadSubScreenBg(BattleCastleOpponentApp *app, GFBgLayer bgId);
static u8 BattleCastleOpponentApp_PrintMessage(BattleCastleOpponentApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId);
static u8 BattleCastleOpponentApp_AddMessage(BattleCastleOpponentApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId);
static int BattleCastleOpponentApp_ShowMessage(BattleCastleOpponentApp *app, int msgNo, u8 fontId);
static void BattleCastleOpponentApp_PrintPartyHp(BattleCastleOpponentApp *app, Window *window);
static void BattleCastleOpponentApp_PrintPartyLevels(BattleCastleOpponentApp *app, Window *window);
static void BattleCastleOpponentApp_ShowYesNoPrompt(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_OpenLevelMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CloseLevelMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_BufferNumber(BattleCastleOpponentApp *app, int field, int value, int digits, int mode);
static void BattleCastleOpponentApp_BufferSpeciesName(BattleCastleOpponentApp *app, int field, BoxPokemon *boxMon);
static void BattleCastleOpponentApp_BufferPlayerName(BattleCastleOpponentApp *app, int field);
static void BattleCastleOpponentApp_PrintPlayerName(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId);
static void BattleCastleOpponentApp_PrintPartnerName(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId);
static void BattleCastleOpponentApp_PrintGenderSymbol(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId, u8 gender);
static void BattleCastleOpponentApp_PrintChoosePrompt(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_HideChoosePrompt(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_OpenMainMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CloseMainMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_OpenSummaryMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CloseSummaryMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_SetMainState(BattleCastleOpponentApp *app, int *state, int newState);
static void BattleCastleOpponentApp_MoveCursor(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_DrawCursor(BattleCastleOpponentApp *app, u32 index, int isPartner);
static void BattleCastleOpponentApp_GetCursorPos(BattleCastleOpponentApp *app, u32 *x, u32 *y, u32 index);
static void BattleCastleOpponentApp_GetTrainerNamePositions(BattleCastleOpponentApp *app, u16 *x1, u16 *y1, u16 *x2, u16 *y2);
static int BattleCastleOpponentApp_PlayActionEffect(BattleCastleOpponentApp *app, int slot, u32 action);
static int BattleCastleOpponentApp_FinishLinkAction(BattleCastleOpponentApp *app, int slot, int action);
static int BattleCastleOpponentApp_GetLevelChangeCost(int levelChange);
static void BattleCastleOpponentApp_CloseAll(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CloseWindow(Window *window);
static BOOL BattleCastleOpponentApp_SendLinkCommand(BattleCastleOpponentApp *app, int command, int param);
static void BattleCastleOpponentApp_PrepareLinkRanks(BattleCastleOpponentApp *app, int command);
static void BattleCastleOpponentApp_PrepareLinkAction(BattleCastleOpponentApp *app, int command, int slot);
static void BattleCastleOpponentApp_PrepareLinkCursor(BattleCastleOpponentApp *app, int command);
static void BattleCastleOpponentApp_PrepareLinkExit(BattleCastleOpponentApp *app, int command);
static void BattleCastleOpponentApp_SetStatsBought(BattleCastleOpponentApp *app, int slot);
static void BattleCastleOpponentApp_BuyStats(BattleCastleOpponentApp *app, int slot);
static void BattleCastleOpponentApp_SetMovesBought(BattleCastleOpponentApp *app, int slot);
static void BattleCastleOpponentApp_BuyMoves(BattleCastleOpponentApp *app, int slot);
static void BattleCastleOpponentApp_CloseMenus(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_OpenMessageBox(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_PrintCastlePoints(BattleCastleOpponentApp *app, Window *window);
static void BattleCastleOpponentApp_ConfirmLevelChange(BattleCastleOpponentApp *app, u8 levelChange);
static void BattleCastleOpponentApp_PrintTrainerNames(BattleCastleOpponentApp *app, Window *window);
static void BattleCastleOpponentApp_ApplyAction(BattleCastleOpponentApp *app, u8 slot, u32 action);
static void BattleCastleOpponentApp_CheckMon(BattleCastleOpponentApp *app, u32 slot);
static void BattleCastleOpponentApp_CheckMonAndRedrawHp(BattleCastleOpponentApp *app, int slot);
static void BattleCastleOpponentApp_ChangeLevel(BattleCastleOpponentApp *app, int slot, int levelChange);
static void BattleCastleOpponentApp_HideSummaryBg(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_UpdateMonIcons(BattleCastleOpponentApp *app);
static int BattleCastleOpponentApp_TryBuy(BattleCastleOpponentApp *app, int cost, int failMsg);
static void BattleCastleOpponentApp_RankUp(BattleCastleOpponentApp *app, u8 slot, int action);
static void BattleCastleOpponentApp_ShowBoughtMarks(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_ShowMonBoughtMarks(BattleCastleOpponentApp *app, u8 slot);
static void BattleCastleOpponentApp_GetBoughtMarkPos(BattleCastleOpponentApp *app, u32 *x, u32 *y);
static void BattleCastleOpponentApp_PrintExpandedMessage(BattleCastleOpponentApp *app, Window *window, MsgData *msgData, int msgNo, int x, int y, int fontId, int color, int alignment);
static void BattleCastleOpponentApp_LoadMonSummary(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_PrintSummaryLabels(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_PrintMonSummary(BattleCastleOpponentApp *app, int copyMode);
static void BattleCastleOpponentApp_ShowMonSummary(BattleCastleOpponentApp *app, int copyMode);
static void BattleCastleOpponentApp_ShowSummaryPanel(BattleCastleOpponentApp *app, int show);
static void BattleCastleOpponentApp_LoadMonPic(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CreateListMenu(BattleCastleOpponentApp *app, u8 count, u8 x, u8 y, u8 width);
static void BattleCastleOpponentApp_CreateMainMenuList(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CreateSummaryMenuList(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_CreateLevelMenuList(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_DestroyListMenu(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_PrintMainMenuHelp(BattleCastleOpponentApp *app);
static void BattleCastleOpponentApp_PrintSummaryMenuHelp(BattleCastleOpponentApp *app);

// Palette flash of the party app's pressed button (app->pressedButton); FALSE when done
BOOL BattleCastlePartyApp_AnimatePressedButton(BattleCastlePartyApp *app) {
    const u8 *rect = gBattleCastlePartyAppButtonRects[app->pressedButton];

    switch (app->buttonAnimState) {
    case CASTLE_BUTTON_ANIM_HIGHLIGHT:
        BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, rect[0], rect[1], rect[2], rect[3], 6);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
        app->buttonAnimState++;
        break;
    case CASTLE_BUTTON_ANIM_RESTORE:
        app->buttonAnimTimer++;
        if (app->buttonAnimTimer == 4) {
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, rect[0], rect[1], rect[2], rect[3], 5);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_2);
            app->buttonAnimTimer = 0;
            app->buttonAnimState++;
        }
        break;
    case CASTLE_BUTTON_ANIM_WAIT:
        app->buttonAnimTimer++;
        if (app->buttonAnimTimer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// BattleCastleOpponentApp init
BOOL BattleCastleOpponentApp_Init(OverlayManager *manager, int *state) {
    BattleCastleOpponentApp *app;
    BattleCastleOpponentAppArgs *args;
    int i;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleCastleOpponentApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_107, 0x30000);
    app = OverlayManager_CreateAndGetData(manager, sizeof(BattleCastleOpponentApp), HEAP_ID_107);
    memset(app, 0, sizeof(BattleCastleOpponentApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_107);
    app->manager = manager;
    args = OverlayManager_GetArgs(manager);
    app->saveData = args->saveData;
    app->castleChallenge = Save_Frontier_GetCastleChallenge(app->saveData);
    app->castleFlags = Save_Frontier_GetCastleFlags(app->saveData);
    app->frontierType = args->frontierType;
    app->selectionOut = &args->selection;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->party = args->party;
    app->checked = args->checked;
    app->levelChanges = args->levelChanges;
    app->statsBought = args->statsBought;
    app->movesBought = args->movesBought;
    app->linkSlot = 0xFF;
    app->partnerPoints = args->partnerPoints;
    app->frontierSave = Save_Frontier_GetStatic(app->saveData);
    for (i = 0; i < 3; i++) {
        app->partnerRanks[i] = 1;
    }
    app->numMons = BattleCastle_IsMultiType(app->frontierType) == FALSE ? 3 : 4;
    app->numOwnSlots = 4;
    app->prevCursorPos = app->numOwnSlots - 1;
    app->partnerCursorPos = 0;
    BattleCastleOpponentApp_InitGraphics(app);
    if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
        FrontierLink_RegisterCommands(app);
    }
    return TRUE;
}

// BattleCastleOpponentApp main
BOOL BattleCastleOpponentApp_Main(OverlayManager *manager, int *state) {
    BattleCastleOpponentApp *app = OverlayManager_GetData(manager);

    if (app->partnerExited == TRUE) {
        if (*state == CASTLE_OPP_MAIN_MENU) {
            app->partnerExited = FALSE;
            BattleCastleOpponentApp_CloseAll(app);
            BattleCastleOpponentApp_HideSummaryBg(app);
            if (app->effectSprite != NULL) {
                BattleCastleSprite_Delete(app->effectSprite);
                app->flags.raw &= ~1;
            }
            BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
            Frontier_BufferPartnerName(app->msgFormat, 0);
            app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00007, 1);
            BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_LINK_EXIT);
        }
    } else if (app->linkSlot != 0xFF && (*state == CASTLE_OPP_MAIN_MENU || *state == CASTLE_OPP_MAIN_LINK_EXIT)) {
        app->partnerExited = FALSE;
        BattleCastleOpponentApp_CloseAll(app);
        BattleCastleOpponentApp_HideSummaryBg(app);
        BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_LINK_ACTION);
    }

    switch (*state) {
    case CASTLE_OPP_MAIN_FADE_IN:
        if (BattleCastleOpponentApp_FadeIn(app) == TRUE) {
            BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_MENU);
        }
        break;
    case CASTLE_OPP_MAIN_MENU:
        if (BattleCastleOpponentApp_HandleMenu(app) == TRUE) {
            if (app->linkActionPending == TRUE) {
                BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_LINK_ACTION);
            } else if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
                BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_LINK_EXIT);
            } else {
                BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_FADE_OUT);
            }
        }
        break;
    case CASTLE_OPP_MAIN_LINK_ACTION:
        if (BattleCastleOpponentApp_LinkAction(app) == TRUE) {
            BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_MENU);
        }
        break;
    case CASTLE_OPP_MAIN_LINK_EXIT:
        if (BattleCastleOpponentApp_LinkExit(app) == TRUE) {
            BattleCastleOpponentApp_SetMainState(app, state, CASTLE_OPP_MAIN_FADE_OUT);
        }
        break;
    case CASTLE_OPP_MAIN_FADE_OUT:
        if (BattleCastleOpponentApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    }

    BattleCastleOpponentApp_UpdateMonIcons(app);
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    return FALSE;
}

// BattleCastleOpponentApp exit
BOOL BattleCastleOpponentApp_Exit(OverlayManager *manager, int *state) {
    BattleCastleOpponentApp *app = OverlayManager_GetData(manager);

    *app->selectionOut = app->cursorPos;
    G2_BlendNone();
    GF_DestroyVramTransferManager();
    BattleCastleOpponentApp_FreeResources(app);
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_107);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

static BOOL BattleCastleOpponentApp_FadeIn(BattleCastleOpponentApp *app) {
    switch (app->subState) {
    case CASTLE_OPP_FADE_IN_START_SYNC:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(CASTLE_OPP_SYNC_START);
        }
        app->subState++;
        break;
    case CASTLE_OPP_FADE_IN_WAIT_SYNC:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            if (LinkSync_IsSynced(CASTLE_OPP_SYNC_START) == TRUE) {
                LinkSync_ClearPackets();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case CASTLE_OPP_FADE_IN_SEND_START:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            if (BattleCastleOpponentApp_SendLinkCommand(app, CASTLE_OPP_LINK_CMD_START, 0) == TRUE) {
                app->subState++;
            }
        } else {
            BattleCastleOpponentApp_DrawScreen(app);
            BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_107);
            app->subState++;
        }
        break;
    case CASTLE_OPP_FADE_IN_WAIT_PARTNER:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            if (app->linkRecvCount < 2) {
                break;
            }
            app->linkRecvCount = 0;
            BattleCastleOpponentApp_DrawScreen(app);
            BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_107);
            app->subState++;
        } else {
            app->subState++;
        }
        break;
    case CASTLE_OPP_FADE_IN_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleCastleOpponentApp_DrawScreen(BattleCastleOpponentApp *app) {
    Window *window = &app->windows[CASTLE_OPP_WIN_POINTS];

    FillWindowPixelBuffer(window, 0);
    BattleCastleOpponentApp_PrintTrainerNames(app, window);
    BattleCastleOpponentApp_PrintCastlePoints(app, window);
    BattleCastleOpponentApp_PrintPartyHp(app, &app->windows[CASTLE_OPP_WIN_HP]);
    BattleCastleOpponentApp_PrintPartyLevels(app, &app->windows[CASTLE_OPP_WIN_LEVELS]);
    BattleCastleOpponentApp_PrintChoosePrompt(app);
    GfGfx_BothDispOn();
}

static BOOL BattleCastleOpponentApp_HandleMenu(BattleCastleOpponentApp *app) {
    u32 input;
    u32 points;
    Pokemon *mon;

    switch (app->subState) {
    case CASTLE_OPP_STATE_RESUME:
        app->unk_00B = 0;
        app->subState = CASTLE_OPP_STATE_SELECT_MON;
        if (app->flags.bits.linkResume == CASTLE_OPP_RESUME_SELECT_MON) {
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_PrintChoosePrompt(app);
        } else if (app->flags.bits.linkResume == CASTLE_OPP_RESUME_SUMMARY_MENU) {
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            BattleCastleSprite_ShowAnim11WithSE(app->menuOpenSprite, 200, 105);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
        } else if (app->flags.bits.linkResume == CASTLE_OPP_RESUME_BOUGHT_PAGE) {
            app->subState = CASTLE_OPP_STATE_SHOW_BOUGHT_PAGE;
        }
        app->flags.bits.linkResume = CASTLE_OPP_RESUME_NONE;
        break;
    case CASTLE_OPP_STATE_SELECT_MON:
        switch ((u32)BattleCastle_HandlePartyGridInput(app->partyGrid)) {
        case 0:
        case 1:
        case 2:
        case 3:
            BattleCastleOpponentApp_HideChoosePrompt(app);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            break;
        case CASTLE_OPP_GRID_EXIT:
            PlaySE(SEQ_SE_DP_SELECT);
            return TRUE;
        case (u32)GRID_MENU_CANCEL:
            if (app->cursorPos != app->numOwnSlots) {
                BattleCastle_SetGridInputsToNext(app->partyGrid);
                BattleCastleOpponentApp_SetCursorPos(app, CASTLE_OPP_GRID_EXIT, app->cursorPos);
            }
            break;
        }
        break;
    case CASTLE_OPP_STATE_MAIN_MENU:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        BattleCastleOpponentApp_PrintMainMenuHelp(app);
        switch (input) {
        case CASTLE_OPP_ACTION_CHECK:
            app->action = input;
            if (app->checked[BattleCastle_GetMonIndex(app->numMons, app->cursorPos)] == FALSE) {
                BattleCastleOpponentApp_CloseMainMenu(app);
                BattleCastleOpponentApp_OpenMessageBox(app);
                BattleCastleOpponentApp_BufferNumber(app, 0, CASTLE_CP_CHECK, 4, 0);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00016, 1);
                BattleCastleOpponentApp_ShowYesNoPrompt(app);
                app->subState = CASTLE_OPP_STATE_CONFIRM_CHECK;
            } else {
                BattleCastleOpponentApp_CloseMainMenu(app);
                mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, app->cursorPos));
                BattleCastleOpponentApp_OpenMessageBox(app);
                BattleCastleOpponentApp_BufferSpeciesName(app, 0, Mon_GetBoxMon(mon));
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00020, 1);
                app->subState = CASTLE_OPP_STATE_MESSAGE;
            }
            break;
        case CASTLE_OPP_ACTION_LEVEL:
            app->action = input;
            BattleCastleOpponentApp_CloseMainMenu(app);
            BattleCastleOpponentApp_OpenMessageBox(app);
            BattleCastleOpponentApp_BufferSpeciesName(app, 0, app->summary.boxMon);
            app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00021, 1);
            BattleCastleOpponentApp_OpenLevelMenu(app);
            app->subState = CASTLE_OPP_STATE_LEVEL_MENU;
            break;
        case CASTLE_OPP_ACTION_SUMMARY:
            BattleCastleOpponentApp_CloseMainMenu(app);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
            break;
        case 6:
        case (u32)LIST_CANCEL:
            BattleCastleOpponentApp_CloseMainMenu(app);
            BattleCastleOpponentApp_PrintChoosePrompt(app);
            app->subState = CASTLE_OPP_STATE_RESUME;
            break;
        case CASTLE_OPP_ACTION_STATS:
        case CASTLE_OPP_ACTION_MOVES:
        case CASTLE_OPP_ACTION_RANK_UP:
        case (u32)LIST_NOTHING_CHOSEN:
            break;
        }
        break;
    case CASTLE_OPP_STATE_CONFIRM_CHECK:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt.prompt)) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            points = FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            if (points < CASTLE_CP_CHECK) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00028, 1);
                app->subState = CASTLE_OPP_STATE_MESSAGE;
            } else if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
                BattleCastle_SpendPoints(app->frontierSave, app->frontierType, CASTLE_CP_CHECK);
                BattleCastleOpponentApp_PrintCastlePoints(app, &app->windows[CASTLE_OPP_WIN_POINTS]);
                BattleCastleOpponentApp_CheckMon(app, app->cursorPos);
                BattleCastleOpponentApp_LoadMonPic(app);
                BattleCastleOpponentApp_PrintMonSummary(app, 0);
                app->subState = CASTLE_OPP_STATE_CHECK_DONE;
            } else {
                app->linkActionPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            break;
        }
        break;
    case CASTLE_OPP_STATE_LEVEL_MENU:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        switch (input) {
        case CASTLE_OPP_LEVEL_MENU_CANCEL:
        case (u32)LIST_CANCEL:
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_CloseLevelMenu(app);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            break;
        case CASTLE_OPP_LEVEL_MENU_UP:
            FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            BattleCastleOpponentApp_CloseLevelMenu(app);
            if (app->levelChanges[BattleCastle_GetMonIndex(app->numMons, app->cursorPos)] == CASTLE_LEVEL_UP) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00029, 1);
                app->subState = CASTLE_OPP_STATE_MESSAGE;
            } else {
                BattleCastleOpponentApp_ConfirmLevelChange(app, CASTLE_LEVEL_UP);
                app->subState = CASTLE_OPP_STATE_CONFIRM_LEVEL;
            }
            break;
        case CASTLE_OPP_LEVEL_MENU_DOWN:
            FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            BattleCastleOpponentApp_CloseLevelMenu(app);
            if (app->levelChanges[BattleCastle_GetMonIndex(app->numMons, app->cursorPos)] == CASTLE_LEVEL_DOWN) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00030, 1);
                app->subState = CASTLE_OPP_STATE_MESSAGE;
            } else {
                BattleCastleOpponentApp_ConfirmLevelChange(app, CASTLE_LEVEL_DOWN);
                app->subState = CASTLE_OPP_STATE_CONFIRM_LEVEL;
            }
            break;
        case (u32)LIST_NOTHING_CHOSEN:
            break;
        }
        break;
    case CASTLE_OPP_STATE_CONFIRM_LEVEL:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt.prompt)) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            points = FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            if (points < (u32)BattleCastleOpponentApp_GetLevelChangeCost(app->levelChange)) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00028, 1);
                app->subState = CASTLE_OPP_STATE_MESSAGE;
                return FALSE;
            }
            app->linkLevelChange = app->levelChange;
            if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
                BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
                BattleCastle_SpendPoints(app->frontierSave, app->frontierType, BattleCastleOpponentApp_GetLevelChangeCost(app->levelChange));
                BattleCastleOpponentApp_PrintCastlePoints(app, &app->windows[CASTLE_OPP_WIN_POINTS]);
                BattleCastleOpponentApp_ChangeLevel(app, app->cursorPos, app->levelChange);
                app->subState = CASTLE_OPP_STATE_ACTION_EFFECT;
            } else {
                app->linkActionPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            break;
        }
        break;
    case CASTLE_OPP_STATE_SUMMARY_MENU:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        BattleCastle_PlaySEOnInput(input, SEQ_SE_DP_SELECT);
        BattleCastleOpponentApp_PrintSummaryMenuHelp(app);
        switch (input) {
        case (u32)LIST_CANCEL:
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_CloseSummaryMenu(app);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            break;
        case CASTLE_OPP_ACTION_STATS:
            app->action = input;
            BattleCastleOpponentApp_CloseSummaryMenu(app);
            if (app->statsBought[BattleCastle_GetMonIndex(app->numMons, app->cursorPos)] == FALSE) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                BattleCastleOpponentApp_BufferNumber(app, 0, CASTLE_CP_STATS, 4, 0);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00043, 1);
                BattleCastleOpponentApp_ShowYesNoPrompt(app);
                app->subState = CASTLE_OPP_STATE_CONFIRM_STATS;
            } else {
                app->subState = CASTLE_OPP_STATE_STATS_PAGE;
            }
            break;
        case CASTLE_OPP_ACTION_MOVES:
            app->action = input;
            BattleCastleOpponentApp_CloseSummaryMenu(app);
            if (BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO) == 1) {
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00042, 1);
                app->subState = CASTLE_OPP_STATE_SUMMARY_MENU_MESSAGE;
                return FALSE;
            }
            if (app->movesBought[BattleCastle_GetMonIndex(app->numMons, app->cursorPos)] == FALSE) {
                BattleCastleOpponentApp_OpenMessageBox(app);
                BattleCastleOpponentApp_BufferNumber(app, 0, CASTLE_CP_MOVES, 4, 0);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00079, 1);
                BattleCastleOpponentApp_ShowYesNoPrompt(app);
                app->subState = CASTLE_OPP_STATE_CONFIRM_MOVES;
            } else {
                app->subState = CASTLE_OPP_STATE_MOVES_PAGE;
            }
            break;
        case CASTLE_OPP_ACTION_RANK_UP:
            if (BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO) == 2) {
                BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
                BattleCastleOpponentApp_CloseSummaryMenu(app);
                BattleCastleOpponentApp_OpenMainMenu(app);
                app->subState = CASTLE_OPP_STATE_MAIN_MENU;
            } else {
                app->action = input;
                BattleCastleOpponentApp_CloseSummaryMenu(app);
                FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
                BattleCastleOpponentApp_BufferNumber(app, 0, CASTLE_CP_RANK_UP, 4, 0);
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00091, 1);
                BattleCastleOpponentApp_ShowYesNoPrompt(app);
                app->subState = CASTLE_OPP_STATE_CONFIRM_RANK_UP;
            }
            break;
        case (u32)LIST_NOTHING_CHOSEN:
            break;
        }
        break;
    case CASTLE_OPP_STATE_CONFIRM_STATS:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt.prompt)) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            if (BattleCastleOpponentApp_TryBuy(app, CASTLE_CP_STATS, msg_0033_00046) == TRUE) {
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
            break;
        }
        break;
    case CASTLE_OPP_STATE_CONFIRM_MOVES:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt.prompt)) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            if (BattleCastleOpponentApp_TryBuy(app, CASTLE_CP_MOVES, msg_0033_00082) == TRUE) {
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
            break;
        }
        break;
    case CASTLE_OPP_STATE_CONFIRM_RANK_UP:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt.prompt)) {
        case YESNORESPONSE_YES:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            points = FrontierSave_GetStat(app->frontierSave, BattleCastle_GetPointsStat(app->frontierType), FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO);
            if (points < CASTLE_CP_RANK_UP) {
                BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
                app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00082, 1);
                app->subState = CASTLE_OPP_STATE_SUMMARY_MENU_MESSAGE;
            } else if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
                BattleCastleOpponentApp_RankUp(app, app->cursorPos, CASTLE_OPP_ACTION_RANK_UP);
                app->subState = CASTLE_OPP_STATE_RANK_UP;
            } else {
                app->linkActionPending = TRUE;
                return TRUE;
            }
            break;
        case YESNORESPONSE_NO:
            BattleCastlePrompt_Hide(&app->yesNoPrompt);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
            break;
        }
        break;
    case CASTLE_OPP_STATE_RANK_UP:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            BattleCastleOpponentApp_PrintMonSummary(app, 0);
            BattleCastleSprite_ShowAnim11WithSE(app->menuOpenSprite, 200, 105);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
        }
        break;
    case CASTLE_OPP_STATE_CHECK_DONE:
        BattleCastleOpponentApp_PrintPartyHp(app, &app->windows[CASTLE_OPP_WIN_HP]);
        app->subState = CASTLE_OPP_STATE_ACTION_EFFECT;
        // fallthrough
    case CASTLE_OPP_STATE_ACTION_EFFECT:
        if (BattleCastleOpponentApp_PlayActionEffect(app, app->cursorPos, app->action) == TRUE) {
            app->subState = CASTLE_OPP_STATE_MESSAGE;
        }
        break;
    case CASTLE_OPP_STATE_BUY_EFFECT:
        if (BattleCastleOpponentApp_PlayActionEffect(app, app->cursorPos, app->action) == TRUE) {
            app->subState = CASTLE_OPP_STATE_SHOW_BOUGHT_PAGE;
        }
        break;
    case CASTLE_OPP_STATE_SHOW_BOUGHT_PAGE:
        if (app->action == CASTLE_OPP_ACTION_STATS) {
            app->subState = CASTLE_OPP_STATE_STATS_PAGE;
        } else {
            app->subState = CASTLE_OPP_STATE_MOVES_PAGE;
        }
        break;
    case CASTLE_OPP_STATE_SUMMARY_MENU_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastleOpponentApp_OpenSummaryMenu(app);
            app->subState = CASTLE_OPP_STATE_SUMMARY_MENU;
        }
        break;
    case CASTLE_OPP_STATE_MESSAGE:
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            BattleCastleOpponentApp_PrintChoosePrompt(app);
            app->subState = CASTLE_OPP_STATE_RESUME;
        }
        break;
    case CASTLE_OPP_STATE_STATS_PAGE:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            break;
        }
        if (gSystem.newKeys & PAD_KEY_RIGHT) {
            break;
        }
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastleOpponentApp_HideSummaryBg(app);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
        }
        break;
    case CASTLE_OPP_STATE_MOVES_PAGE:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            break;
        }
        if (gSystem.newKeys & PAD_KEY_RIGHT) {
            break;
        }
        if (BattleCastle_IsAdvancePressed() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BattleCastleOpponentApp_HideSummaryBg(app);
            BattleCastleOpponentApp_OpenMainMenu(app);
            app->subState = CASTLE_OPP_STATE_MAIN_MENU;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleCastleOpponentApp_LinkAction(BattleCastleOpponentApp *app) {
    switch (app->subState) {
    case CASTLE_OPP_LINK_ACTION_SEND:
        app->flags.bits.linkResume = CASTLE_OPP_RESUME_SELECT_MON;
        if (BattleCastleOpponentApp_SendLinkCommand(app, CASTLE_OPP_LINK_CMD_ACTION, app->cursorPos) == TRUE) {
            app->linkActionPending = FALSE;
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_ACTION_APPLY:
        if (app->linkSlot != 0xFF) {
            u8 action;
            u8 slot;

            app->linkRecvCount = 0;
            action = app->action;
            slot = app->linkSlot;
            if (action == CASTLE_OPP_ACTION_RANK_UP) {
                BattleCastleOpponentApp_RankUp(app, slot, CASTLE_OPP_ACTION_RANK_UP);
            } else {
                BattleCastleOpponentApp_ApplyAction(app, slot, action);
            }
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_ACTION_PLAY_EFFECT: {
        int slot = BattleCastle_GetSideSlot(app->numOwnSlots, app->linkSlot);
        if (BattleCastleOpponentApp_PlayActionEffect(app, slot, app->action) == TRUE) {
            app->timer = 30;
            app->subState++;
        }
        break;
    }
    case CASTLE_OPP_LINK_ACTION_DELAY:
        app->timer--;
        if (app->timer == 0) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(CASTLE_OPP_SYNC_ACTION);
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_ACTION_WAIT_SYNC:
        if (LinkSync_IsSynced(CASTLE_OPP_SYNC_ACTION) == TRUE) {
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_ACTION_FINISH: {
        int slot = BattleCastle_GetSideSlot(app->numOwnSlots, app->linkSlot);
        if (BattleCastleOpponentApp_FinishLinkAction(app, slot, app->action) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_Init(HEAP_ID_107);
            app->linkSlot = 0xFF;
            app->partnerExited = FALSE;
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}

static BOOL BattleCastleOpponentApp_LinkExit(BattleCastleOpponentApp *app) {
    switch (app->subState) {
    case CASTLE_OPP_LINK_EXIT_SEND:
        if (BattleCastleOpponentApp_SendLinkCommand(app, CASTLE_OPP_LINK_CMD_EXIT, 0) == TRUE) {
            app->timer = 30;
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_EXIT_DELAY:
        if (app->timer != 0) {
            app->timer--;
        }
        if (app->timer == 0) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(CASTLE_OPP_SYNC_EXIT);
            app->subState++;
        }
        break;
    case CASTLE_OPP_LINK_EXIT_WAIT_SYNC:
        if (LinkSync_IsSynced(CASTLE_OPP_SYNC_EXIT) == TRUE) {
            LinkSync_ClearPackets();
            BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleCastleOpponentApp_FadeOut(BattleCastleOpponentApp *app) {
    switch (app->subState) {
    case CASTLE_OPP_FADE_OUT_START:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_107);
        app->subState++;
        break;
    case CASTLE_OPP_FADE_OUT_WAIT:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleCastleOpponentApp_FreeResources(BattleCastleOpponentApp *app) {
    int numMons;
    int i;
    int j;

    BattleCastlePrompt_Free(&app->yesNoPrompt);
    BattleCastle_DestroyListMenuSpawner(app->listMenuSpawner);
    BattleCastle_FreeGrid(app->partyGrid);
    BattleCastleSprite_Delete(app->cursor);
    BattleCastleSprite_Delete(app->partnerCursor);
    BattleCastleSprite_Delete(app->menuOpenSprite);
    BattleCastleSprite_Delete(app->monPic);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            BattleCastleSprite_Delete(app->boughtMarks[i][j]);
        }
    }
    numMons = BattleCastle_GetOpponentMonCount(app->frontierType, 1);
    for (i = 0; i < numMons; i++) {
        BattleCastleSprite_Delete(app->hiddenMonIcons[i]);
        BattleCastleSprite_Delete(app->monIcons[i]);
        BattleCastleSprite_Delete(app->hpBars[i]);
    }
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleCastle_FreeSpriteGfx(&app->spriteGfx);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    MessagePrinter_Delete(app->msgPrinter);
    FontID_Release(4);
    for (i = 0; i < 3; i++) {
        String_Delete(app->strings[i]);
    }
    BattleCastle_RemoveWindows(app->windows, 1);
    BattleCastleOpponentApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
}

static void BattleCastleOpponentApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void BattleCastleOpponentApp_InitGraphics(BattleCastleOpponentApp *app) {
    struct {
        int row;
        int numMons;
        int iconX;
        int markX;
        u8 *markRow;    // app, shifted by one row of app->boughtMarks per step
        u16 namePos[4]; // unused here
        u32 y;
        u32 x;
    } v;
    int i;
    int j;
    int columnX;

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_107);
    BattleCastleOpponentApp_InitBgGraphics(app);
    BattleCastleOpponentApp_InitSpriteGfx(app);
    FontID_Alloc(4, HEAP_ID_107);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0033_bin, HEAP_ID_107);
    app->msgFormat = MessageFormat_New(HEAP_ID_107);
    app->expandedString = String_New(600, HEAP_ID_107);
    app->rawString = String_New(600, HEAP_ID_107);
    for (i = 0; i < 3; i++) {
        app->strings[i] = String_New(32, HEAP_ID_107);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_107);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_107);
    app->msgPrinter = MessagePrinter_New(1, 2, 0, HEAP_ID_107);
    BattleCastle_AddWindows(app->bgConfig, app->windows, 1);
    BattleCastleOpponentApp_GetTrainerNamePositions(app, &v.namePos[3], &v.namePos[2], &v.namePos[1], &v.namePos[0]);
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
        int initial = 60;
        columnX = 64;
        v.iconX = initial;
    } else {
        int initial = 28;
        columnX = 32;
        v.iconX = initial;
    }
    v.numMons = BattleCastle_GetOpponentMonCount(app->frontierType, 1);
    for (i = 0; i < v.numMons; i++, columnX += 64) {
        app->hiddenMonIcons[i] = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 7, v.iconX, 62, 2, 0);
        app->hpBars[i] = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 15, columnX, 78, 3, 0);
        app->monIcons[i] = BattleCastleSprite_New(&app->spriteGfx, i + 10, 10, 5, 1, columnX, 58, 2, 0);
        BattleCastleSprite_SetMonIconPalette(app->monIcons[i], Party_GetMonByIndex(app->party, i));
        if (!app->checked[i]) {
            BattleCastleSprite_SetDrawFlag(app->hiddenMonIcons[i], TRUE);
            BattleCastleSprite_SetDrawFlag(app->monIcons[i], FALSE);
            BattleCastleSprite_SetDrawFlag(app->hpBars[i], FALSE);
        } else {
            BattleCastleSprite_SetDrawFlag(app->hiddenMonIcons[i], FALSE);
            BattleCastleSprite_SetDrawFlag(app->monIcons[i], TRUE);
            BattleCastleSprite_SetDrawFlag(app->hpBars[i], TRUE);
        }
        v.iconX += 64;
    }
    BattleCastleOpponentApp_GetCursorPos(app, &v.x, &v.y, 0);
    app->cursor = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 1, v.x, v.y, 2, 0);
    app->partnerCursor = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 2, v.x, v.y, 2, 0);
    if (!BattleCastle_IsMultiType(app->frontierType)) {
        BattleCastleSprite_SetDrawFlag(app->partnerCursor, FALSE);
    }
    v.row = 0;
    v.markX = 0;
    v.markRow = (u8 *)app;
    do {
        int dy;
        BattleCastleOpponentApp *rowApp;

        j = 0;
        rowApp = (BattleCastleOpponentApp *)v.markRow;
        dy = j;
        do {
            BattleCastleOpponentApp_GetBoughtMarkPos(app, &v.x, &v.y);
            rowApp->boughtMarks[0][j] = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 12, v.x + v.markX, v.y + dy, 2, 0);
            BattleCastleSprite_SetDrawFlag(rowApp->boughtMarks[0][j], FALSE);
            dy += 12;
            j++;
        } while (j < 2);
        v.markX += 64;
        v.markRow += sizeof(app->boughtMarks[0]);
        v.row++;
    } while (v.row < 4);
    BattleCastleOpponentApp_ShowBoughtMarks(app);
    app->menuOpenSprite = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, 11, 20, 20, 0, 0);
    BattleCastleSprite_SetDrawFlag(app->menuOpenSprite, FALSE);
    BattleCastleOpponentApp_LoadMonSummary(app);
    BattleCastleOpponentApp_PrintSummaryLabels(app);
    BattleCastleOpponentApp_PrintMonSummary(app, 1);
    app->monPic = BattleCastleSprite_NewSubScreen(&app->spriteGfx, 2, 2, 2, 0, 48, 40, 0, 0);
    BattleCastleOpponentApp_LoadMonPic(app);
    app->partyGrid = BattleCastleOpponentApp_CreatePartyGrid(app, 1, app->numMons);
    app->listMenuSpawner = BattleCastle_CreateListMenuSpawner(app->spriteGfx.spriteList, app->paletteData);
    BattleCastlePrompt_Init(&app->yesNoPrompt);
    if (Comm_IsActive()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 6, 10);
    Main_SetVBlankIntrCB(BattleCastleOpponentApp_VBlankCallback, app);
}

static void BattleCastleOpponentApp_InitBgGraphics(BattleCastleOpponentApp *app) {
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    BattleCastleOpponentApp_SetGraphicsBanks();
    BattleCastleOpponentApp_InitBgLayers(app->bgConfig);
    app->paletteData = PaletteData_Init(HEAP_ID_107);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_107);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_107);
    BattleCastleOpponentApp_LoadPartyBg(app, GF_BG_LYR_MAIN_3);
    BattleCastleOpponentApp_LoadBgPalette();
    BattleCastleOpponentApp_LoadSummaryBg(app, GF_BG_LYR_MAIN_2);
    BattleCastleOpponentApp_LoadSummaryBgPalette();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    BattleCastleOpponentApp_LoadSubScreenBg(app, GF_BG_LYR_SUB_0);
}

static void BattleCastleOpponentApp_InitSpriteGfx(BattleCastleOpponentApp *app) {
    BattleCastle_InitSpriteGfx(&app->spriteGfx, app->party, BattleCastle_IsMultiType(app->frontierType));
}

static void BattleCastleOpponentApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK; // GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB)
}

static void BattleCastleOpponentApp_VBlankCallback(void *arg) {
    // OS_SetIrqCheckFlag(OS_IE_V_BLANK) with the DTCM address written out
    vu32 *dtcm = (vu32 *)0x027E0000;
    BattleCastleOpponentApp *app = arg;

    if (app->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(app->paletteData);
    }
    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
}

static void BattleCastleOpponentApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = gBattleCastleOpponentAppGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void BattleCastleOpponentApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main1;
    BgTemplate main0;
    BgTemplate main2;
    BgTemplate main3;
    BgTemplate sub0;
    BgTemplate sub1;

    modes = gBattleCastleOpponentAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    main1 = gBattleCastleOpponentAppBgTemplate_Main1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main0 = gBattleCastleOpponentAppBgTemplate_Main0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main2 = gBattleCastleOpponentAppBgTemplate_Main2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    main3 = gBattleCastleOpponentAppBgTemplate_Main3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &main3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    sub0 = gBattleCastleOpponentAppBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = gBattleCastleOpponentAppBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
}

static void BattleCastleOpponentApp_LoadPartyBg(BattleCastleOpponentApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x30, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x2C, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    } else {
        GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x2D, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    }
}

static void BattleCastleOpponentApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9D, &plttData, HEAP_ID_107);

    DC_FlushRange(plttData->pRawData, 0x80);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x80);
    Heap_Free(file);
}

static void BattleCastleOpponentApp_LoadSummaryBg(BattleCastleOpponentApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x30, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x2E, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
}

static void BattleCastleOpponentApp_LoadSummaryBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9D, &plttData, HEAP_ID_107);

    DC_FlushRange(plttData->pRawData, 0x80);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x80);
    Heap_Free(file);
}

static void BattleCastleOpponentApp_LoadSubScreenBg(BattleCastleOpponentApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x28, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x93, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_107);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_107);
}

static u8 BattleCastleOpponentApp_PrintMessage(BattleCastleOpponentApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    u8 printerId;

    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgNo, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    printerId = AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, speed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    ScheduleWindowCopyToVram(window);
    return printerId;
}

static u8 BattleCastleOpponentApp_AddMessage(BattleCastleOpponentApp *app, Window *window, int msgNo, int x, int y, int speed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    u8 printerId;

    ReadMsgDataIntoString(app->msgData, msgNo, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    printerId = AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, speed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    ScheduleWindowCopyToVram(window);
    return printerId;
}

static int BattleCastleOpponentApp_ShowMessage(BattleCastleOpponentApp *app, int msgNo, u8 fontId) {
    int printerId = BattleCastleOpponentApp_PrintMessage(app, &app->windows[CASTLE_OPP_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, fontId);

    ScheduleWindowCopyToVram(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
    return printerId;
}

static void BattleCastleOpponentApp_PrintPartyHp(BattleCastleOpponentApp *app, Window *window) {
    int x;
    int i;
    u8 numMons;
    Pokemon *mon;

    FillWindowPixelBuffer(window, 0);
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
        x = 36;
    } else {
        x = 4;
    }
    numMons = BattleCastle_GetOpponentMonCount(app->frontierType, 1);
    for (i = 0; i < numMons; i++) {
        if (app->checked[i] == FALSE) {
            FillWindowPixelRect(window, 0, x + i * 64, 1, 64, 16);
        } else {
            mon = Party_GetMonByIndex(app->party, i);
            PrintUIntOnWindow(app->msgPrinter, GetMonData(mon, MON_DATA_HP, NULL), 3, PRINTING_MODE_RIGHT_ALIGN, window, x + i * 64, 1);
            MessagePrinter_PrintGlyph(app->msgPrinter, 0, window, x + 24 + i * 64, 1);
            PrintUIntOnWindow(app->msgPrinter, GetMonData(mon, MON_DATA_MAX_HP, NULL), 3, PRINTING_MODE_LEFT_ALIGN, window, x + 32 + i * 64, 1);
        }
    }
    ScheduleWindowCopyToVram(window);
}

static void BattleCastleOpponentApp_PrintPartyLevels(BattleCastleOpponentApp *app, Window *window) {
    int x;
    int genderX;
    int i;
    int numMons;
    Pokemon *mon;
    u32 value;

    FillWindowPixelBuffer(window, 0);
    if (!BattleCastle_IsMultiType(app->frontierType)) {
        x = 40;
        genderX = 80;
    } else {
        x = 8;
        genderX = 48;
    }
    numMons = BattleCastle_GetOpponentMonCount(app->frontierType, 1);
    for (i = 0; i < numMons; i++, x += 64, genderX += 64) {
        mon = Party_GetMonByIndex(app->party, i);
        value = GetMonData(mon, MON_DATA_LEVEL, NULL);
        MessagePrinter_PrintGlyphAndUInt(app->msgPrinter, 1, value, 3, PRINTING_MODE_LEFT_ALIGN, window, x, 1);
        value = GetMonData(mon, MON_DATA_GENDER, NULL);
        BattleCastleOpponentApp_PrintGenderSymbol(app, window, genderX, 1, 0, value);
    }
    ScheduleWindowCopyToVram(window);
}

static void BattleCastleOpponentApp_ShowYesNoPrompt(BattleCastleOpponentApp *app) {
    BattleCastlePrompt_Show(&app->yesNoPrompt, app->bgConfig);
}

static void BattleCastleOpponentApp_OpenLevelMenu(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_CreateLevelMenuList(app);
}

static void BattleCastleOpponentApp_CloseLevelMenu(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_DestroyListMenu(app);
}

static void BattleCastleOpponentApp_BufferNumber(BattleCastleOpponentApp *app, int field, int value, int digits, int mode) {
    BufferIntegerAsString(app->msgFormat, field, value, digits, (PrintingMode)mode, TRUE);
}

static void BattleCastleOpponentApp_BufferSpeciesName(BattleCastleOpponentApp *app, int field, BoxPokemon *boxMon) {
    BufferBoxMonSpeciesName(app->msgFormat, field, boxMon);
}

static void BattleCastleOpponentApp_BufferPlayerName(BattleCastleOpponentApp *app, int field) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    BufferPlayersName(app->msgFormat, field, profile);
}

static void BattleCastleOpponentApp_PrintPlayerName(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    String *name = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_107);
    u32 color;

    CopyU16ArrayToString(name, PlayerProfile_GetNamePtr(profile));
    color = PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE ? CASTLE_OPP_TEXT_COLOR_MALE_NAME : CASTLE_OPP_TEXT_COLOR_FEMALE;
    AddTextPrinterParameterizedWithColor(window, fontId, name, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(name);
}

static void BattleCastleOpponentApp_PrintPartnerName(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    u32 color = PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE ? CASTLE_OPP_TEXT_COLOR_MALE_NAME : CASTLE_OPP_TEXT_COLOR_FEMALE;

    BufferPlayersName(app->msgFormat, 0, profile);
    BattleCastleOpponentApp_AddMessage(app, window, msg_0033_00001, x, y, TEXT_SPEED_NOTRANSFER, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 0);
}

static void BattleCastleOpponentApp_PrintGenderSymbol(BattleCastleOpponentApp *app, Window *window, int x, int y, u8 fontId, u8 gender) {
    int msgNo;
    u8 fg;
    u8 shadow;
    u8 bg;

    if (gender == MON_MALE) {
        msgNo = msg_0033_00064; // "♂"
        fg = 7;
        shadow = 8;
        bg = 0;
    } else if (gender == MON_FEMALE) {
        msgNo = msg_0033_00065; // "♀"
        fg = 3;
        shadow = 4;
        bg = 0;
    } else {
        return;
    }
    BattleCastleOpponentApp_AddMessage(app, window, msgNo, x, y, TEXT_SPEED_NOTRANSFER, fg, shadow, bg, fontId);
}

static void BattleCastleOpponentApp_PrintChoosePrompt(BattleCastleOpponentApp *app) {
    app->printerId = BattleCastleOpponentApp_PrintMessage(app, &app->windows[CASTLE_OPP_WIN_TRAINER_NAMES], msg_0033_00005, 0, 5, TEXT_SPEED_NOTRANSFER, 1, 2, 0, 0); // "EXIT"
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_CHOOSE_PROMPT], Options_GetFrame(app->options));
    app->printerId = BattleCastleOpponentApp_PrintMessage(app, &app->windows[CASTLE_OPP_WIN_CHOOSE_PROMPT], msg_0033_00004, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
}

static void BattleCastleOpponentApp_HideChoosePrompt(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_CHOOSE_PROMPT]);
}

static void BattleCastleOpponentApp_OpenMainMenu(BattleCastleOpponentApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    app->listCursorPos = CASTLE_OPP_LIST_POS_NONE;
    BattleCastleOpponentApp_CreateMainMenuList(app);
    BattleCastleOpponentApp_PrintMainMenuHelp(app);
}

static void BattleCastleOpponentApp_CloseMainMenu(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
    BattleCastleOpponentApp_DestroyListMenu(app);
}

static void BattleCastleOpponentApp_OpenSummaryMenu(BattleCastleOpponentApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    app->listCursorPos = CASTLE_OPP_LIST_POS_NONE;
    BattleCastleOpponentApp_CreateSummaryMenuList(app);
    BattleCastleOpponentApp_PrintSummaryMenuHelp(app);
}

static void BattleCastleOpponentApp_CloseSummaryMenu(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_DestroyListMenu(app);
}

static void BattleCastleOpponentApp_SetMainState(BattleCastleOpponentApp *app, int *state, int newState) {
    app->subState = 0;
    *state = newState;
}

static void BattleCastleOpponentApp_MoveCursor(BattleCastleOpponentApp *app) {
    PlaySE(SEQ_SE_DP_SELECT);
    if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
        BattleCastleOpponentApp_SendLinkCommand(app, CASTLE_OPP_LINK_CMD_CURSOR, app->cursorPos);
    }
    BattleCastleOpponentApp_DrawCursor(app, app->cursorPos, FALSE);
}

static void BattleCastleOpponentApp_DrawCursor(BattleCastleOpponentApp *app, u32 index, int isPartner) {
    BattleCastleSprite *cursor;
    int seq;
    int exitSeq;
    int x;
    int y;

    if (isPartner == FALSE) {
        cursor = app->cursor;
        seq = 1;
        exitSeq = 0;
    } else {
        cursor = app->partnerCursor;
        seq = 2;
        exitSeq = 17;
    }
    if (index >= app->numOwnSlots) {
        BattleCastleSprite_SetAnim(cursor, exitSeq);
        BattleCastleSprite_SetPosition(cursor, 224, 160);
        if (isPartner == FALSE) {
            BattleCastleOpponentApp_ShowSummaryPanel(app, FALSE);
        }
    } else {
        BattleCastleSprite_SetAnim(cursor, seq);
        BattleCastleOpponentApp_GetCursorPos(app, (u32 *)&x, (u32 *)&y, index);
        BattleCastleSprite_SetPosition(cursor, (u16)x, (u16)y);
        if (isPartner == FALSE) {
            if (app->prevCursorPos >= app->numOwnSlots) {
                BattleCastleOpponentApp_ShowSummaryPanel(app, TRUE);
            }
            BattleCastleOpponentApp_ShowMonSummary(app, 0);
        }
    }
}

static void BattleCastleOpponentApp_GetCursorPos(BattleCastleOpponentApp *app, u32 *x, u32 *y, u32 index) {
    if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
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

static void BattleCastleOpponentApp_GetTrainerNamePositions(BattleCastleOpponentApp *app, u16 *x1, u16 *y1, u16 *x2, u16 *y2) {
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
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

// The slot's level-change effect (LEVEL) / summary redraw; TRUE when done
static int BattleCastleOpponentApp_PlayActionEffect(BattleCastleOpponentApp *app, int slot, u32 action) {
    int index = BattleCastle_GetMonIndex(app->numMons, slot);

    Party_GetMonByIndex(app->party, index);
    switch (action) {
    case CASTLE_OPP_ACTION_CHECK:
        app->flags.raw &= ~1;
        return TRUE;
    case CASTLE_OPP_ACTION_LEVEL:
        if (app->flags.bits.effectActive == FALSE) {
            app->flags.bits.effectActive = TRUE;
            int seq = app->linkLevelChange == CASTLE_LEVEL_UP ? 9 : 10;
            int baseX = BattleCastle_IsMultiType(app->frontierType) == FALSE ? 80 : 48;
            s16 x = baseX + index * 64;
            app->effectSprite = BattleCastleSprite_New(&app->spriteGfx, 0, 0, 0, seq, x, 50, 0, 0);
        }
        if (BattleCastleSprite_IsAnimated(app->effectSprite) == FALSE) {
            BattleCastleSprite_Delete(app->effectSprite);
            app->effectSprite = NULL;
            app->flags.raw &= ~1;
            return TRUE;
        }
        break;
    case CASTLE_OPP_ACTION_STATS:
        if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
            BattleCastleOpponentApp_PrintMonSummary(app, 0);
            app->flags.raw &= ~1;
            return TRUE;
        }
        app->flags.raw &= ~1;
        return TRUE;
    case CASTLE_OPP_ACTION_MOVES:
        if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
            BattleCastleOpponentApp_PrintMonSummary(app, 0);
            app->flags.raw &= ~1;
            return TRUE;
        }
        app->flags.raw &= ~1;
        return TRUE;
    case CASTLE_OPP_ACTION_RANK_UP:
        app->flags.raw &= ~1;
        return TRUE;
    }
    return FALSE;
}

// After a link action: which menu to reopen; TRUE when done
static int BattleCastleOpponentApp_FinishLinkAction(BattleCastleOpponentApp *app, int slot, int action) {
    u8 ownSlots = app->numOwnSlots;
    int index = BattleCastle_GetMonIndex(app->numMons, slot);

    Party_GetMonByIndex(app->party, index);
    switch (action) {
    case CASTLE_OPP_ACTION_CHECK:
    case CASTLE_OPP_ACTION_LEVEL:
        app->flags.raw &= ~1;
        return TRUE;
    case CASTLE_OPP_ACTION_RANK_UP:
        if (app->flags.bits.effectActive == FALSE) {
            BattleCastleOpponentApp_PrintMonSummary(app, 0);
        }
        app->flags.raw &= ~1;
        return TRUE;
    case CASTLE_OPP_ACTION_STATS:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            if (Comm_GetLocalNetId() == 0) {
                if (app->linkSlot >= ownSlots) {
                    app->flags.raw &= ~1;
                    return TRUE;
                }
            } else if (app->linkSlot < ownSlots) {
                app->flags.raw &= ~1;
                return TRUE;
            }
        }
        if (app->flags.bits.effectActive == FALSE) {
            app->flags.bits.linkResume = CASTLE_OPP_RESUME_BOUGHT_PAGE;
            app->flags.raw &= ~1;
            return TRUE;
        }
        break;
    case CASTLE_OPP_ACTION_MOVES:
        if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
            if (Comm_GetLocalNetId() == 0) {
                if (app->linkSlot >= ownSlots) {
                    app->flags.raw &= ~1;
                    return TRUE;
                }
            } else if (app->linkSlot < ownSlots) {
                app->flags.raw &= ~1;
                return TRUE;
            }
        }
        if (app->flags.bits.effectActive == FALSE) {
            app->flags.bits.linkResume = CASTLE_OPP_RESUME_BOUGHT_PAGE;
            app->flags.raw &= ~1;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static int BattleCastleOpponentApp_GetLevelChangeCost(int levelChange) {
    return levelChange == CASTLE_LEVEL_UP ? 1 : 15;
}

static void BattleCastleOpponentApp_CloseAll(BattleCastleOpponentApp *app) {
    BattleCastleOpponentApp_CloseMenus(app);
    BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
    BattleCastleSprite_SetDrawFlag(app->menuOpenSprite, FALSE);
}

static void BattleCastleOpponentApp_CloseWindow(Window *window) {
    ClearFrameAndWindow2(window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(window);
}

static BOOL BattleCastleOpponentApp_SendLinkCommand(BattleCastleOpponentApp *app, int command, int param) {
    int cmd;

    switch (command) {
    case CASTLE_OPP_LINK_CMD_START:
        cmd = 0x35;
        BattleCastleOpponentApp_PrepareLinkRanks(app, command);
        break;
    case CASTLE_OPP_LINK_CMD_ACTION:
        cmd = 0x36;
        BattleCastleOpponentApp_PrepareLinkAction(app, command, param);
        break;
    case CASTLE_OPP_LINK_CMD_CURSOR:
        cmd = 0x37;
        BattleCastleOpponentApp_PrepareLinkCursor(app, command);
        break;
    case CASTLE_OPP_LINK_CMD_EXIT:
        cmd = 0x38;
        BattleCastleOpponentApp_PrepareLinkExit(app, command);
        break;
    }
    if (Comm_SendCommand(cmd, app->linkSendBuf, sizeof(app->linkSendBuf)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void BattleCastleOpponentApp_PrepareLinkRanks(BattleCastleOpponentApp *app, int command) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    int i;
    int gender;
    u8 *dest;

    app->linkSendBuf[0] = command;
    gender = PlayerProfile_GetTrainerGender(profile);
    i = 0;
    app->linkSendBuf[1] = gender;
    dest = (u8 *)app + 2 * sizeof(u16);
    do {
        *(u16 *)(dest + offsetof(BattleCastleOpponentApp, linkSendBuf)) = BattleCastle_GetRank(app->saveData, app->frontierType, i);
        i++;
        dest += sizeof(u16);
    } while (i < 3);
}

// Link receive callback: the partner's info ranks
void BattleCastleOpponentApp_ReceiveRanks(int netId, void *unused, u16 *data, BattleCastleOpponentApp *app) {
    int i;
    u16 *rank;

    app->linkRecvCount++;
    if (netId != Comm_GetLocalNetId()) {
        i = 0;
        rank = data + 2;
        do {
            app->partnerRanks[i] = *rank;
            i++;
            rank++;
        } while (i < 3);
    }
}

static void BattleCastleOpponentApp_PrepareLinkAction(BattleCastleOpponentApp *app, int command, int slot) {
    app->linkSendBuf[0] = command;
    app->linkSendBuf[1] = slot;
    if (Comm_GetLocalNetId() == 0 && app->linkSlot == 0xFF) {
        app->linkSlot = slot;
    }
    app->linkSendBuf[2] = app->linkSlot;
    app->linkSendBuf[4] = app->linkLevelChange;
    app->linkSendBuf[5] = app->action;
}

// Link receive callback: the action the partner chose (the host decides when both act at once)
void BattleCastleOpponentApp_ReceiveAction(int netId, int unused, u16 *data, BattleCastleOpponentApp *app) {
    app->linkRecvCount++;
    if (netId != Comm_GetLocalNetId()) {
        app->partnerSlot = data[1];
        if (Comm_GetLocalNetId() == 0) {
            if (app->linkSlot != 0xFF) {
                app->partnerSlot = 0;
                return;
            }
            app->linkSlot = app->partnerSlot + app->numOwnSlots;
            app->linkLevelChange = data[4];
            app->action = data[5];
        } else {
            app->linkSlot = data[2];
            app->linkLevelChange = data[4];
            app->action = data[5];
        }
    }
}

static void BattleCastleOpponentApp_PrepareLinkCursor(BattleCastleOpponentApp *app, int command) {
    app->linkSendBuf[0] = command;
    app->linkSendBuf[1] = app->cursorPos;
}

// Link receive callback: the partner's cursor
void BattleCastleOpponentApp_ReceiveCursor(int netId, int unused, u16 *data, BattleCastleOpponentApp *app) {
    if (netId != Comm_GetLocalNetId()) {
        app->partnerCursorPos = data[1];
        BattleCastleOpponentApp_DrawCursor(app, app->partnerCursorPos, TRUE);
    }
}

static void BattleCastleOpponentApp_PrepareLinkExit(BattleCastleOpponentApp *app, int command) {
    app->linkSendBuf[0] = TRUE;
}

// Link receive callback: the partner left
void BattleCastleOpponentApp_ReceiveExit(int netId, int unused, u16 *data, BattleCastleOpponentApp *app) {
    if (netId != Comm_GetLocalNetId()) {
        app->partnerExited = data[0];
    }
}

static void BattleCastleOpponentApp_SetStatsBought(BattleCastleOpponentApp *app, int slot) {
    int index = BattleCastle_GetMonIndex(app->numMons, slot);
    app->statsBought[index] = TRUE;
}

static void BattleCastleOpponentApp_BuyStats(BattleCastleOpponentApp *app, int slot) {
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    BattleCastleOpponentApp_BufferSpeciesName(app, 0, Mon_GetBoxMon(Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot))));
    app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00047, 1);
    BattleCastleOpponentApp_SetStatsBought(app, slot);
    BattleCastleOpponentApp_ShowBoughtMarks(app);
    if (app->cursorPos == slot) {
        BattleCastleOpponentApp_LoadMonSummary(app);
        BattleCastleOpponentApp_PrintMonSummary(app, 0);
    }
    PlaySE(SEQ_SE_DP_UG_020);
}

static void BattleCastleOpponentApp_SetMovesBought(BattleCastleOpponentApp *app, int slot) {
    int index = BattleCastle_GetMonIndex(app->numMons, slot);
    app->movesBought[index] = TRUE;
}

static void BattleCastleOpponentApp_BuyMoves(BattleCastleOpponentApp *app, int slot) {
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    BattleCastleOpponentApp_BufferSpeciesName(app, 0, Mon_GetBoxMon(Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot))));
    app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00083, 1);
    BattleCastleOpponentApp_SetMovesBought(app, slot);
    BattleCastleOpponentApp_ShowBoughtMarks(app);
    if (app->cursorPos == slot) {
        BattleCastleOpponentApp_LoadMonSummary(app);
        BattleCastleOpponentApp_PrintMonSummary(app, 0);
    }
    PlaySE(SEQ_SE_DP_UG_020);
}

static void BattleCastleOpponentApp_CloseMenus(BattleCastleOpponentApp *app) {
    if (app->flags.bits.listMenuOpen == TRUE) {
        TouchscreenListMenu_DestroyButtons(app->listMenu);
        BattleCastleOpponentApp_DestroyListMenu(app);
    }
    BattleCastlePrompt_Hide(&app->yesNoPrompt);
}

static void BattleCastleOpponentApp_OpenMessageBox(BattleCastleOpponentApp *app) {
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
}

static void BattleCastleOpponentApp_PrintCastlePoints(BattleCastleOpponentApp *app, Window *window) {
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    int leftPoints;
    int rightPoints;
    int cpStat;

    BattleCastleOpponentApp_GetTrainerNamePositions(app, &x1, &y1, &x2, &y2);
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
        u16 x = x1 + 72;
        u16 y = y1 + 1;

        FillWindowPixelRect(window, 0, x, y, 48, 16);
        cpStat = BattleCastle_GetPointsStat(app->frontierType);
        leftPoints = FrontierSave_GetStat(app->frontierSave, cpStat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
        BattleCastleOpponentApp_BufferNumber(app, 0, leftPoints, 4, 0);
        app->printerId = BattleCastleOpponentApp_AddMessage(app, window, msg_0033_00002, x, y, TEXT_SPEED_NOTRANSFER, 1, 2, 0, 0);
    } else {
        FillWindowPixelRect(window, 0, 64, 0, 48, 16);
        FillWindowPixelRect(window, 0, 192, 0, 48, 16);
        if (Comm_GetLocalNetId() == 0) {
            cpStat = BattleCastle_GetPointsStat(app->frontierType);
            leftPoints = FrontierSave_GetStat(app->frontierSave, cpStat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
            rightPoints = app->partnerPoints;
        } else {
            leftPoints = app->partnerPoints;
            cpStat = BattleCastle_GetPointsStat(app->frontierType);
            rightPoints = FrontierSave_GetStat(app->frontierSave, cpStat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType)));
        }
        BattleCastleOpponentApp_BufferNumber(app, 0, leftPoints, 4, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, window, app->msgData, msg_0033_00002, 112, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastleOpponentApp_BufferNumber(app, 0, rightPoints, 4, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, window, app->msgData, msg_0033_00003, 240, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    }
    ScheduleWindowCopyToVram(window);
}

static void BattleCastleOpponentApp_ConfirmLevelChange(BattleCastleOpponentApp *app, u8 levelChange) {
    int cost = BattleCastleOpponentApp_GetLevelChangeCost(levelChange);

    BattleCastleOpponentApp_BufferNumber(app, 0, cost, 4, 0);
    app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00025, 1);
    BattleCastleOpponentApp_ShowYesNoPrompt(app);
    app->levelChange = levelChange;
}

static void BattleCastleOpponentApp_PrintTrainerNames(BattleCastleOpponentApp *app, Window *window) {
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;

    BattleCastleOpponentApp_GetTrainerNamePositions(app, &x1, &y1, &x2, &y2);
    if (!BattleCastle_IsMultiType(app->frontierType)) {
        BattleCastleOpponentApp_PrintPlayerName(app, window, (u16)(x1 + 4), (u16)(y1 + 1), 0);
    } else if (!Comm_GetLocalNetId()) {
        int y = y1;
        y = (u16)(y + 1);
        BattleCastleOpponentApp_PrintPlayerName(app, window, x1, y, 0);
        BattleCastleOpponentApp_PrintPartnerName(app, window, x2, (u16)(y2 + 1), 0);
    } else {
        int y = y1;
        y = (u16)(y + 1);
        BattleCastleOpponentApp_PrintPartnerName(app, window, x1, y, 0);
        BattleCastleOpponentApp_PrintPlayerName(app, window, x2, (u16)(y2 + 1), 0);
    }
    ScheduleWindowCopyToVram(window);
}

// Pay for and apply a link action (also the partner's); the partner pays with its own points
static void BattleCastleOpponentApp_ApplyAction(BattleCastleOpponentApp *app, u8 slot, u32 action) {
    u32 index;
    u32 ownSlots;
    int cost;

    PlaySE(SEQ_SE_DP_PIRORIRO2);
    ownSlots = app->numOwnSlots;
    index = BattleCastle_GetSideSlot(ownSlots, slot);
    switch (action) {
    case CASTLE_OPP_ACTION_CHECK:
        cost = CASTLE_CP_CHECK;
        break;
    case CASTLE_OPP_ACTION_LEVEL:
        cost = BattleCastleOpponentApp_GetLevelChangeCost(app->linkLevelChange);
        break;
    case CASTLE_OPP_ACTION_STATS:
        cost = CASTLE_CP_STATS;
        break;
    case CASTLE_OPP_ACTION_MOVES:
        cost = CASTLE_CP_MOVES;
        break;
    }
    if (!Comm_GetLocalNetId()) {
        if (slot < ownSlots) {
            BattleCastleOpponentApp_BufferPlayerName(app, 5);
            BattleCastle_SpendPoints(app->frontierSave, app->frontierType, cost);
        } else {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            app->partnerPoints -= cost;
        }
    } else {
        if (slot < ownSlots) {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            app->partnerPoints -= cost;
        } else {
            BattleCastleOpponentApp_BufferPlayerName(app, 5);
            BattleCastle_SpendPoints(app->frontierSave, app->frontierType, cost);
        }
    }
    BattleCastleOpponentApp_PrintCastlePoints(app, &app->windows[CASTLE_OPP_WIN_POINTS]);
    BattleCastleOpponentApp_CloseMenus(app);
    switch (action) {
    case CASTLE_OPP_ACTION_CHECK:
        BattleCastleOpponentApp_CheckMonAndRedrawHp(app, index);
        return;
    case CASTLE_OPP_ACTION_LEVEL:
        BattleCastleOpponentApp_ChangeLevel(app, index, app->linkLevelChange);
        return;
    case CASTLE_OPP_ACTION_STATS:
        BattleCastleOpponentApp_BuyStats(app, index);
        return;
    case CASTLE_OPP_ACTION_MOVES:
        BattleCastleOpponentApp_BuyMoves(app, index);
        break;
    }
}

static void BattleCastleOpponentApp_CheckMon(BattleCastleOpponentApp *app, u32 slot) {
    int index;
    Pokemon *mon;

    index = BattleCastle_GetMonIndex(app->numMons, slot);
    mon = Party_GetMonByIndex(app->party, index);
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    BattleCastleOpponentApp_BufferSpeciesName(app, 0, Mon_GetBoxMon(mon));
    app->printerId = BattleCastleOpponentApp_ShowMessage(app, msg_0033_00020, 1);

    index = BattleCastle_GetMonIndex(app->numMons, slot);
    app->checked[index] = TRUE;
    index = BattleCastle_GetMonIndex(app->numMons, slot);
    BattleCastleSprite_SetDrawFlag(app->hiddenMonIcons[index], FALSE);
    index = BattleCastle_GetMonIndex(app->numMons, slot);
    BattleCastleSprite_SetDrawFlag(app->monIcons[index], TRUE);

    BattleCastleOpponentApp_PrintPartyLevels(app, &app->windows[CASTLE_OPP_WIN_LEVELS]);
    if (app->cursorPos == slot) {
        BattleCastleOpponentApp_LoadMonSummary(app);
        BattleCastleOpponentApp_PrintMonSummary(app, 0);
        BattleCastleOpponentApp_LoadMonPic(app);
    }

    index = BattleCastle_GetMonIndex(app->numMons, slot);
    BattleCastleSprite_SetDrawFlag(app->hpBars[index], TRUE);
    PlaySE(SEQ_SE_DP_UG_020);
}

static void BattleCastleOpponentApp_CheckMonAndRedrawHp(BattleCastleOpponentApp *app, int slot) {
    BattleCastleOpponentApp_CheckMon(app, slot);
    BattleCastleOpponentApp_PrintPartyHp(app, &app->windows[CASTLE_OPP_WIN_HP]);
}

// LEVEL +5 / -5 toggles: a second purchase of the same change resets the mon to level 50
static void BattleCastleOpponentApp_ChangeLevel(BattleCastleOpponentApp *app, int slot, int levelChange) {
    Pokemon *mon;
    u32 exp;

    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    mon = Party_GetMonByIndex(app->party, BattleCastle_GetMonIndex(app->numMons, slot));
    BattleCastleOpponentApp_BufferSpeciesName(app, 0, Mon_GetBoxMon(mon));
    if (levelChange == CASTLE_LEVEL_UP) {
        app->printerId = (u8)BattleCastleOpponentApp_ShowMessage(app, msg_0033_00031, 1);
        PlaySE(SEQ_SE_DP_OPEN7);
    } else {
        app->printerId = (u8)BattleCastleOpponentApp_ShowMessage(app, msg_0033_00032, 1);
        PlaySE(SEQ_SE_DP_CLOSE7);
    }
    if (app->levelChanges[BattleCastle_GetMonIndex(app->numMons, slot)] == CASTLE_LEVEL_NORMAL) {
        app->levelChanges[BattleCastle_GetMonIndex(app->numMons, slot)] = (u8)levelChange;
    } else {
        app->levelChanges[BattleCastle_GetMonIndex(app->numMons, slot)] = CASTLE_LEVEL_NORMAL;
    }
    if (app->levelChanges[BattleCastle_GetMonIndex(app->numMons, slot)] == CASTLE_LEVEL_NORMAL) {
        exp = GetMonExpBySpeciesAndLevel(GetMonData(mon, MON_DATA_SPECIES, NULL), 50);
        SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
        CalcMonLevelAndStats(mon);
    } else if (app->levelChanges[BattleCastle_GetMonIndex(app->numMons, slot)] == CASTLE_LEVEL_UP) {
        exp = GetMonExpBySpeciesAndLevel(GetMonData(mon, MON_DATA_SPECIES, NULL), 55);
        SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
        CalcMonLevelAndStats(mon);
    } else {
        exp = GetMonExpBySpeciesAndLevel(GetMonData(mon, MON_DATA_SPECIES, NULL), 45);
        SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
        CalcMonLevelAndStats(mon);
    }
    BattleCastleOpponentApp_PrintPartyHp(app, &app->windows[CASTLE_OPP_WIN_HP]);
    BattleCastleOpponentApp_PrintPartyLevels(app, &app->windows[CASTLE_OPP_WIN_LEVELS]);
    if (app->cursorPos == slot) {
        BattleCastleOpponentApp_LoadMonSummary(app);
        BattleCastleOpponentApp_PrintMonSummary(app, 0);
    }
}

static void BattleCastleOpponentApp_HideSummaryBg(BattleCastleOpponentApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

static void BattleCastleOpponentApp_UpdateMonIcons(BattleCastleOpponentApp *app) {
    int count = BattleCastle_GetPartySize(app->frontierType, 1);
    int i;

    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(app->party, i);
        u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
        u32 maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        u32 hpState = Frontier_GetHpGaugeFrame(hp, maxHp);

        if (app->monIcons[i] != NULL) {
            BattleCastleSprite_ChangeAnim(app->monIcons[i], hpState);
            if (i != BattleCastle_GetMonIndex(app->numMons, app->cursorPos) || app->cursorPos >= app->numOwnSlots) {
                BattleCastleSprite_SetBounce(app->monIcons[i], 0);
            } else {
                BattleCastleSprite_SetBounce(app->monIcons[i], 1);
            }
        }
    }
}

// Buy the stats (cost CASTLE_CP_STATS) or moves of the selected mon; TRUE when the link exchange takes over
static int BattleCastleOpponentApp_TryBuy(BattleCastleOpponentApp *app, int cost, int failMsg) {
    int cpStat = BattleCastle_GetPointsStat(app->frontierType);
    int cpStatKind = FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(app->frontierType));

    if (FrontierSave_GetStat(app->frontierSave, cpStat, cpStatKind) < (u32)cost) {
        BattleCastleOpponentApp_OpenMessageBox(app);
        app->printerId = BattleCastleOpponentApp_ShowMessage(app, failMsg, 1);
        app->subState = CASTLE_OPP_STATE_MESSAGE;
        return FALSE;
    }
    if (BattleCastle_IsMultiType(app->frontierType) == FALSE) {
        BattleCastleOpponentApp_CloseWindow(&app->windows[CASTLE_OPP_WIN_MESSAGE]);
        BattleCastle_SpendPoints(app->frontierSave, app->frontierType, cost);
        BattleCastleOpponentApp_PrintCastlePoints(app, &app->windows[CASTLE_OPP_WIN_POINTS]);
        if (cost == CASTLE_CP_STATS) {
            BattleCastleOpponentApp_BuyStats(app, app->cursorPos);
        } else {
            BattleCastleOpponentApp_BuyMoves(app, app->cursorPos);
        }
        app->subState = CASTLE_OPP_STATE_BUY_EFFECT;
        return FALSE;
    }
    app->linkActionPending = TRUE;
    return TRUE;
}

// RANK UP (info rank): the player or the partner pays CASTLE_CP_RANK_UP
static void BattleCastleOpponentApp_RankUp(BattleCastleOpponentApp *app, u8 slot, int action) {
    u8 kind;
    u32 ownSlots;

    BattleCastle_GetPartySize(app->frontierType, 0);
    if (action == CASTLE_OPP_ACTION_RANK_UP) {
        kind = CASTLE_RANK_INFO;
    } else {
        GF_AssertFail();
    }
    ownSlots = app->numOwnSlots;
    BattleCastle_GetSideSlot(ownSlots, slot);
    // from here on, slot holds the rank before the purchase
    if (Comm_GetLocalNetId() == 0) {
        if (slot < ownSlots) {
            FrontierSave *frontierSave;
            int stat;

            BattleCastleOpponentApp_BufferPlayerName(app, 5);
            BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            BattleCastle_SpendPoints(app->frontierSave, app->frontierType, CASTLE_CP_RANK_UP);
            slot = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            frontierSave = Save_Frontier_GetStatic(app->saveData);
            stat = BattleCastle_GetRankStat(app->frontierType, kind);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(app->frontierType, kind)), (u16)(slot + 1));
            if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
                app->flags.bits.linkResume = CASTLE_OPP_RESUME_SUMMARY_MENU;
            }
        } else {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            slot = app->partnerRanks[kind];
            app->partnerPoints -= CASTLE_CP_RANK_UP;
            app->partnerRanks[kind]++;
        }
    } else {
        if (slot < ownSlots) {
            BattleCastle_BufferPartnerName(app->msgFormat, 5);
            slot = app->partnerRanks[kind];
            app->partnerPoints -= CASTLE_CP_RANK_UP;
            app->partnerRanks[kind]++;
        } else {
            FrontierSave *frontierSave;
            int stat;

            BattleCastleOpponentApp_BufferPlayerName(app, 5);
            BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            BattleCastle_SpendPoints(app->frontierSave, app->frontierType, CASTLE_CP_RANK_UP);
            slot = BattleCastle_GetRank(app->saveData, app->frontierType, kind);
            frontierSave = Save_Frontier_GetStatic(app->saveData);
            stat = BattleCastle_GetRankStat(app->frontierType, kind);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(app->frontierType, kind)), (u16)(slot + 1));
            if (BattleCastle_IsMultiType(app->frontierType) == TRUE) {
                app->flags.bits.linkResume = CASTLE_OPP_RESUME_SUMMARY_MENU;
            }
        }
    }
    BattleCastleOpponentApp_CloseMenus(app);
    BattleCastleOpponentApp_PrintCastlePoints(app, &app->windows[CASTLE_OPP_WIN_POINTS]);
    BattleCastle_DrawWindowFrame(&app->windows[CASTLE_OPP_WIN_MESSAGE], Options_GetFrame(app->options));
    app->printerId = BattleCastleOpponentApp_ShowMessage(app, gBattleCastleOpponentAppRankUpMessages[kind][slot], 1);
}

static void BattleCastleOpponentApp_ShowBoughtMarks(BattleCastleOpponentApp *app) {
    int i;
    int numMons = BattleCastle_GetOpponentMonCount(app->frontierType, 1);

    for (i = 0; i < numMons; i++) {
        BattleCastleOpponentApp_ShowMonBoughtMarks(app, (u8)i);
    }
}

static void BattleCastleOpponentApp_ShowMonBoughtMarks(BattleCastleOpponentApp *app, u8 slot) {
    if (app->statsBought[slot] == TRUE) {
        BattleCastleSprite_SetDrawFlag(app->boughtMarks[slot][0], TRUE);
    }
    if (app->movesBought[slot] == TRUE) {
        BattleCastleSprite_SetDrawFlag(app->boughtMarks[slot][1], TRUE);
    }
}

static void BattleCastleOpponentApp_GetBoughtMarkPos(BattleCastleOpponentApp *app, u32 *x, u32 *y) {
    *x = BattleCastle_IsMultiType(app->frontierType) == TRUE ? 64 : 96;
    *y = 60;
}

static void BattleCastleOpponentApp_PrintExpandedMessage(BattleCastleOpponentApp *app, Window *window, MsgData *msgData, int msgNo, int x, int y, int fontId, int color, int alignment) {
    String *string = NewString_ReadMsgData(msgData, msgNo);

    StringExpandPlaceholders(app->msgFormat, app->expandedString, string);
    BattleCastle_PrintString(window, app->expandedString, x, y, fontId, color, alignment);
    String_Delete(string);
}

static void BattleCastleOpponentApp_LoadMonSummary(BattleCastleOpponentApp *app) {
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
        app->summary.hideGender = 0;
    } else {
        app->summary.hideGender = 1;
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

static void BattleCastleOpponentApp_PrintSummaryLabels(BattleCastleOpponentApp *app) {
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_LEVEL_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ABILITY_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_NATURE_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ITEM_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_HP_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ATTACK_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SP_ATK_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_DEFENSE_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SP_DEF_LABEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SPEED_LABEL], 0);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_LEVEL_LABEL], app->msgData, msg_0033_00066, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ABILITY_LABEL], app->msgData, msg_0033_00052, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_NATURE_LABEL], app->msgData, msg_0033_00050, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ITEM_LABEL], app->msgData, msg_0033_00048, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_HP_LABEL], app->msgData, msg_0033_00067, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ATTACK_LABEL], app->msgData, msg_0033_00054, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SP_ATK_LABEL], app->msgData, msg_0033_00058, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_DEFENSE_LABEL], app->msgData, msg_0033_00056, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SP_DEF_LABEL], app->msgData, msg_0033_00060, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SPEED_LABEL], app->msgData, msg_0033_00062, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_LEVEL_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ABILITY_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_NATURE_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ITEM_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_HP_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ATTACK_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SP_ATK_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_DEFENSE_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SP_DEF_LABEL]);
    CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SPEED_LABEL]);
}

// Summary panel of the selected mon; what is shown depends on what was bought (CHECK / stats / moves)
static void BattleCastleOpponentApp_PrintMonSummary(BattleCastleOpponentApp *app, int copyMode) {
    int infoRank;
    int index;
    u8 i;
    String *string;
    int halfSlashWidth;
    int width;

    infoRank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO);
    index = BattleCastle_GetMonIndex(app->numMons, app->cursorPos);

    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SPECIES], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_GENDER], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_LEVEL], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ABILITY], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_NATURE], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ITEM], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SUMMARY_HP], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_ATTACK], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SP_ATK], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_DEFENSE], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SP_DEF], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_SPEED], 0);
    FillWindowPixelBuffer(&app->windows[CASTLE_OPP_WIN_MOVES], 0);

    if (!app->summary.hideGender) {
        if (app->summary.gender == MON_MALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_GENDER], app->msgData, msg_0033_00064, 0, 0, 0, CASTLE_OPP_TEXT_COLOR_MALE, CASTLE_ALIGN_LEFT);
        } else if (app->summary.gender == MON_FEMALE) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_GENDER], app->msgData, msg_0033_00065, 0, 0, 0, CASTLE_OPP_TEXT_COLOR_FEMALE, CASTLE_ALIGN_LEFT);
        }
    }
    BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.level, 3, 0);
    BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_LEVEL], app->msgData, msg_0033_00072, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);

    // CHECK: species and HP
    if (app->checked[index]) {
        BufferBoxMonSpeciesName(app->msgFormat, 0, Mon_GetBoxMon(app->summary.mon));
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_SPECIES], app->msgData, msg_0033_00069, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.hp, 3, 0);
        BattleCastleOpponentApp_BufferNumber(app, 1, app->summary.maxHp, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_SUMMARY_HP], app->msgData, msg_0033_00078, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SUMMARY_HP]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    } else {
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SPECIES], app->msgData, msg_0033_00075, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SUMMARY_HP], app->msgData, msg_0033_00077, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SUMMARY_HP]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    }

    // stats summary: ability, nature, item and stats
    if (app->statsBought[index]) {
        BufferAbilityName(app->msgFormat, 0, app->summary.ability);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_ABILITY], app->msgData, msg_0033_00053, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BufferNatureName(app->msgFormat, 0, app->summary.nature);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_NATURE], app->msgData, msg_0033_00051, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BufferItemName(app->msgFormat, 0, app->summary.heldItem);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_ITEM], app->msgData, msg_0033_00049, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.attack, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_ATTACK], app->msgData, msg_0033_00055, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_ATTACK]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.spAttack, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_SP_ATK], app->msgData, msg_0033_00059, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SP_ATK]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.defense, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_DEFENSE], app->msgData, msg_0033_00057, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_DEFENSE]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.spDefense, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_SP_DEF], app->msgData, msg_0033_00061, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SP_DEF]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.speed, 3, 0);
        BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_SPEED], app->msgData, msg_0033_00063, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SPEED]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    } else {
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ABILITY], app->msgData, msg_0033_00075, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_NATURE], app->msgData, msg_0033_00075, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ITEM], app->msgData, msg_0033_00075, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_ATTACK], app->msgData, msg_0033_00074, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_ATTACK]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SP_ATK], app->msgData, msg_0033_00074, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SP_ATK]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_DEFENSE], app->msgData, msg_0033_00074, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_DEFENSE]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SP_DEF], app->msgData, msg_0033_00074, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SP_DEF]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_SPEED], app->msgData, msg_0033_00074, GetWindowWidth(&app->windows[CASTLE_OPP_WIN_SPEED]) * 8, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_RIGHT);
    }

    // move list: one line per move, "pp / maxPp" centered on the slash at x = 120
    if (infoRank == 1) {
        BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00076, 0, 0, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
    } else if (app->movesBought[index] == FALSE) {
        for (i = 0; i < 4; i++) {
            BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00075, 0, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
            string = NewString_ReadMsgData(app->msgData, msg_0033_00068);
            halfSlashWidth = (u8)(FontID_String_GetWidth(0, string, 0) >> 1);
            BattleCastle_PrintString(&app->windows[CASTLE_OPP_WIN_MOVES], string, 120, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_CENTER);
            String_Delete(string);
            string = NewString_ReadMsgData(app->msgData, msg_0033_00073);
            width = FontID_String_GetWidth(0, string, 0);
            BattleCastle_PrintString(&app->windows[CASTLE_OPP_WIN_MOVES], string, 120 - halfSlashWidth - width, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
            String_Delete(string);
            BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00073, 120 + halfSlashWidth, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
        }
    } else {
        for (i = 0; i < 4; i++) {
            BufferMoveName(app->msgFormat, i, app->summary.moves[i]);
            BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00084 + i, 0, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
            if (app->summary.moves[i] == MOVE_NONE) {
                BattleCastle_PrintMsg(&app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00090, 120, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_CENTER);
            } else {
                string = NewString_ReadMsgData(app->msgData, msg_0033_00068);
                halfSlashWidth = (u8)(FontID_String_GetWidth(0, string, 0) >> 1);
                BattleCastle_PrintString(&app->windows[CASTLE_OPP_WIN_MOVES], string, 120, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_CENTER);
                String_Delete(string);
                BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.pp[i], 2, 0);
                string = NewString_ReadMsgData(app->msgData, msg_0033_00089);
                StringExpandPlaceholders(app->msgFormat, app->expandedString, string);
                width = FontID_String_GetWidth(0, app->expandedString, 0);
                BattleCastle_PrintString(&app->windows[CASTLE_OPP_WIN_MOVES], app->expandedString, 120 - halfSlashWidth - width, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
                String_Delete(string);
                BattleCastleOpponentApp_BufferNumber(app, 0, app->summary.maxPp[i], 2, 0);
                BattleCastleOpponentApp_PrintExpandedMessage(app, &app->windows[CASTLE_OPP_WIN_MOVES], app->msgData, msg_0033_00089, 120 + halfSlashWidth, i * 16, 0, CASTLE_OPP_TEXT_COLOR, CASTLE_ALIGN_LEFT);
            }
        }
    }

    if (copyMode == TRUE) {
        for (i = CASTLE_OPP_WIN_SUMMARY_FIRST; i <= CASTLE_OPP_WIN_SUMMARY_LAST; i++) {
            ScheduleWindowCopyToVram(&app->windows[i]);
        }
    } else {
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SPECIES]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_GENDER]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_LEVEL]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ABILITY]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_NATURE]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ITEM]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SUMMARY_HP]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_ATTACK]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SP_ATK]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_DEFENSE]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SP_DEF]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_SPEED]);
        CopyWindowPixelsToVram_TextMode(&app->windows[CASTLE_OPP_WIN_MOVES]);
    }
}

static void BattleCastleOpponentApp_ShowMonSummary(BattleCastleOpponentApp *app, int copyMode) {
    BattleCastleOpponentApp_LoadMonSummary(app);
    BattleCastleOpponentApp_PrintMonSummary(app, copyMode);
    BattleCastleOpponentApp_LoadMonPic(app);
}

static void BattleCastleOpponentApp_ShowSummaryPanel(BattleCastleOpponentApp *app, int show) {
    u32 i;

    if (show == TRUE) {
        for (i = CASTLE_OPP_WIN_SUMMARY_FIRST; i <= CASTLE_OPP_WIN_SUMMARY_LAST; i++) {
            ScheduleWindowCopyToVram(&app->windows[i]);
        }
        BattleCastleSprite_SetDrawFlag(app->monPic, TRUE);
    } else {
        for (i = CASTLE_OPP_WIN_SUMMARY_FIRST; i <= CASTLE_OPP_WIN_SUMMARY_LAST; i++) {
            ClearWindowTilemapAndScheduleTransfer(&app->windows[i]);
        }
        BattleCastleSprite_SetDrawFlag(app->monPic, FALSE);
    }
}

// Picture of the selected mon; a placeholder until it is checked
static void BattleCastleOpponentApp_LoadMonPic(BattleCastleOpponentApp *app) {
    int index = BattleCastle_GetMonIndex(app->numMons, app->cursorPos);

    if (app->checked[index] == FALSE) {
        BattleCastleSprite_LoadMonPic(app->monPic, app->summary.boxMon, SPECIES_NONE, app->summary.personality);
    } else {
        BattleCastleSprite_LoadMonPic(app->monPic, app->summary.boxMon, app->summary.species, app->summary.personality);
    }
}

void BattleCastleOpponentApp_SetCursorPos(BattleCastleOpponentApp *app, u32 cursorPos, u32 prevCursorPos) {
    app->cursorPos = cursorPos;
    app->prevCursorPos = prevCursorPos;
    BattleCastleOpponentApp_MoveCursor(app);
}

static void BattleCastleOpponentApp_CreateListMenu(BattleCastleOpponentApp *app, u8 count, u8 x, u8 y, u8 width) {
    TouchscreenListMenuTemplate template;
    TouchscreenListMenuHeader header;
    u16 tileStart;
    u16 tileCount;
    int numMons;

    BattleCastle_GetMenuTiles(&tileStart, &tileCount);
    template = gBattleCastleOpponentAppListMenuTemplate;
    template.baseTile = tileStart;
    template.charOffset = tileStart + tileCount - 27;
    template.unkA = tileCount;
    header.template = template;
    header.listMenuItems = app->listMenuItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = count;
    app->listMenu = BattleCastle_CreateListMenu(app->listMenuSpawner, &header, x, y, width);
    app->flags.bits.listMenuOpen = TRUE;
    numMons = BattleCastle_GetPartySize(app->frontierType, 1);
    BattleCastle_SetSpritesTranslucent(app->monIcons, numMons, 1);
    BattleCastle_SetSpritesTranslucent(app->hiddenMonIcons, numMons, 1);
}

// CHECK (until checked) / LEVEL / SUMMARY (until everything is bought) / CANCEL
static void BattleCastleOpponentApp_CreateMainMenuList(BattleCastleOpponentApp *app) {
    u16 count;
    u8 infoRank;
    int index;

    count = 0;
    app->listMenuItems = ListMenuItems_New(4, HEAP_ID_107);
    infoRank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO);
    index = BattleCastle_GetMonIndex(app->numMons, app->cursorPos);
    if (app->checked[index] == FALSE) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00008, CASTLE_OPP_ACTION_CHECK);
        count++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00009, CASTLE_OPP_ACTION_LEVEL);
    count++;
    if (app->statsBought[index] == FALSE || app->movesBought[index] == FALSE || infoRank != 2) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00010, CASTLE_OPP_ACTION_SUMMARY);
        count++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00011, LIST_CANCEL);
    count++;
    BattleCastleOpponentApp_CreateListMenu(app, count, 17, gBattleCastleOpponentAppListMenuY[count], 13);
}

// SUMMARY (until bought) / MOVES (until bought, needs info rank 2) / RANK UP (below rank 2) / CANCEL
static void BattleCastleOpponentApp_CreateSummaryMenuList(BattleCastleOpponentApp *app) {
    u16 count;
    u8 infoRank;
    int index;

    count = 0;
    app->listMenuItems = ListMenuItems_New(4, HEAP_ID_107);
    infoRank = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO);
    index = BattleCastle_GetMonIndex(app->numMons, app->cursorPos);
    if (app->statsBought[index] == FALSE) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00033, CASTLE_OPP_ACTION_STATS);
        count++;
    }
    if (infoRank != 1 && app->movesBought[index] == FALSE) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00034, CASTLE_OPP_ACTION_MOVES);
        count++;
    }
    if (infoRank != 2) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00035, CASTLE_OPP_ACTION_RANK_UP);
        count++;
    }
    ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00036, LIST_CANCEL);
    count++;
    BattleCastleOpponentApp_CreateListMenu(app, count, 17, gBattleCastleOpponentAppListMenuY[count], 13);
}

// LEVEL +5 / LEVEL -5 / CANCEL
static void BattleCastleOpponentApp_CreateLevelMenuList(BattleCastleOpponentApp *app) {
    u32 i;

    app->listMenuItems = ListMenuItems_New(3, HEAP_ID_107);
    for (i = 0; i < 3; i++) {
        ListMenuItems_AppendFromMsgData(app->listMenuItems, app->msgData, msg_0033_00022 + i, i);
    }
    BattleCastleOpponentApp_CreateListMenu(app, 3, 17, 8, 13);
}

static void BattleCastleOpponentApp_DestroyListMenu(BattleCastleOpponentApp *app) {
    int numMons = BattleCastle_GetPartySize(app->frontierType, 1);

    BattleCastle_SetSpritesTranslucent(app->monIcons, numMons, 0);
    BattleCastle_SetSpritesTranslucent(app->hiddenMonIcons, numMons, 0);
    BattleCastle_DestroyListMenu(app->listMenu);
    ListMenuItems_Delete(app->listMenuItems);
    app->flags.bits.listMenuOpen = FALSE;
}

static void BattleCastleOpponentApp_PrintMainMenuHelp(BattleCastleOpponentApp *app) {
    int msgNo;

    if (app->listCursorPos != app->listMenu->cursorPos) {
        switch ((u32)app->listMenuItems[app->listMenu->cursorPos].value) {
        case CASTLE_OPP_ACTION_CHECK:
            msgNo = msg_0033_00012;
            break;
        case CASTLE_OPP_ACTION_LEVEL:
            msgNo = msg_0033_00013;
            break;
        case CASTLE_OPP_ACTION_SUMMARY:
            msgNo = msg_0033_00014;
            break;
        case (u32)LIST_CANCEL:
            msgNo = msg_0033_00015;
            break;
        }
        BattleCastleOpponentApp_PrintMessage(app, &app->windows[CASTLE_OPP_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
        app->listCursorPos = app->listMenu->cursorPos;
    }
}

static void BattleCastleOpponentApp_PrintSummaryMenuHelp(BattleCastleOpponentApp *app) {
    int msgNo;
    int variant;

    if (app->listCursorPos != app->listMenu->cursorPos) {
        variant = BattleCastle_GetRank(app->saveData, app->frontierType, CASTLE_RANK_INFO) == 1 ? 0 : 1;
        switch ((u32)app->listMenuItems[app->listMenu->cursorPos].value) {
        case CASTLE_OPP_ACTION_STATS:
            msgNo = gBattleCastleOpponentAppStatsHelpMessages[variant];
            break;
        case CASTLE_OPP_ACTION_MOVES:
            msgNo = gBattleCastleOpponentAppMovesHelpMessages[variant];
            break;
        case CASTLE_OPP_ACTION_RANK_UP:
            msgNo = gBattleCastleOpponentAppRankUpHelpMessages[variant];
            break;
        case (u32)LIST_CANCEL:
            msgNo = gBattleCastleOpponentAppCancelHelpMessages[variant];
            break;
        }
        BattleCastleOpponentApp_PrintMessage(app, &app->windows[CASTLE_OPP_WIN_MESSAGE], msgNo, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
        app->listCursorPos = app->listMenu->cursorPos;
    }
}
