#include "battle/battle_party_menu.h"

#include <stddef.h>
#include <string.h>

#include "global.h"

#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "battle/battle_cursor.h"
#include "battle/battle_dpad_cursor.h"
#include "battle/battle_main.h"
#include "battle/battle_system.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0006.h"
#include "msgdata/msg/msg_0722.h"
#include "msgdata/msg/msg_0749.h"

#include "bag.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "message_printer_number.h"
#include "move.h"
#include "msgdata.h"
#include "palette.h"
#include "party.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_summary.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "type_icon_resources.h"
#include "ui_util.h"

// In-battle party screen. Pages: 0 party list, 1 mon menu (SHIFT / SUMMARY / CHECK MOVES), 2 summary,
// 3 move list, 4 move detail, 5 restore PP, 6 / 8 learn move (battle / contest view), 7 / 9 confirm the move to
// forget (battle / contest view).

enum BattlePartyPage {
    BATTLE_PARTY_PAGE_PARTY_LIST,
    BATTLE_PARTY_PAGE_MON_MENU,
    BATTLE_PARTY_PAGE_SUMMARY,
    BATTLE_PARTY_PAGE_CHECK_MOVES,
    BATTLE_PARTY_PAGE_MOVE_DETAIL,
    BATTLE_PARTY_PAGE_RESTORE_PP,
    BATTLE_PARTY_PAGE_LEARN_MOVE,
    BATTLE_PARTY_PAGE_FORGET_MOVE,
    BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST,
    BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST,
};

enum BattlePartyViewMode {
    BATTLE_PARTY_VIEW_BATTLE,
    BATTLE_PARTY_VIEW_CONTEST,
};

typedef struct BattlePartyMenuMove {
    u16 move;
    u8 pp;
    u8 maxPP;
    u8 type;
    u8 moveClass;
    u8 accuracy;
    u8 power;
} BattlePartyMenuMove;

typedef struct BattlePartyMenuMon {
    Pokemon *mon;
    u16 species;
    u16 attack;
    u16 defense;
    u16 speed;
    u16 spAttack;
    u16 spDefense;
    u16 hp;
    u16 maxHp;
    u8 type1;
    u8 type2;
    u8 level : 7;
    u8 hideGender : 1;
    u8 gender : 3;
    u8 status : 4; // status icon id, 7 for none
    u8 isEgg : 1;
    u16 ability;
    u16 heldItem;
    u32 exp;
    u32 levelExp;
    u32 nextLevelExp;
    u8 contestStats[5];
    u8 ballCapsule;
    u8 form;
    BattlePartyMenuMove moves[4];
} BattlePartyMenuMon;

typedef struct BattlePartyMenu {
    BattlePartyContext *context;
    BattlePartyMenuMon mons[6];
    BgConfig *bgConfig;
    PaletteData *paletteData;
    // Button tilemaps cut out of the screen files, one entry per press animation frame
    u16 monButtonTiles[4][16 * 6];
    u16 partnerMonButtonTiles[4][16 * 6];
    u16 menuButtonTiles[4][13 * 5]; // SUMMARY, CHECK MOVES
    u16 prevMonButtonTiles[4][5 * 5];
    u16 nextMonButtonTiles[4][5 * 5];
    u16 cancelButtonTiles[4][5 * 5];
    u16 shiftButtonTiles[3][30 * 17];
    u16 moveButtonTiles[4][16 * 6];
    u16 forgetButtonTiles[3][26 * 5];
    u16 viewToggleButtonTiles[3][9 * 4]; // battle / contest move view
    u16 moveTabButtonTiles[3][5 * 2];
    u16 pagePalettes[2][16]; // background palette 6: default, move list page
    u8 buttonAnimState;
    u8 buttonAnimTimer;
    u8 buttonAnimButton;
    u8 unk_1FA3_0 : 4;
    u8 buttonAnimPalette : 3;
    u8 buttonAnimActive : 1;
    MessagePrinter *msgPrinter;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    SpriteManager *spriteManager;
    // 0-5 held items, 6 summary held item, 7-12 mon icons, 13-18 status icons, 19-20 mon types, 21-24 move types,
    // 25 new move type, 26 move category, 27-31 condition markers, 32-37 ball capsule marks
    ManagedSprite *sprites[38];
    Window windows[2]; // prompt, message
    Window *pageWindows;
    u8 pageWindowCount;
    u8 windowSet;    // the summary, move list and move detail pages alternate between two sets of windows
    u8 activeMon;    // party slot selected on entry
    u8 viewMode : 4; // 0 battle, 1 contest move view
    u8 canToggleView : 4;
    u8 state;
    u8 nextState;
    u8 page;
    u8 textPrinterId;
    u8 effectState;
    u16 hpTarget;
    u16 ppTargets[4];
    BattleDpadCursor *cursor;
    u8 monMenuPosition;
    u8 learnMovePosition;
    u8 forgetMovePosition;
    u8 battlerPosition;
} BattlePartyMenu;

typedef struct BattlePartyMenuPos {
    int x;
    int y;
} BattlePartyMenuPos;

typedef struct BattlePartyMenuOrder {
    u8 slots[6];
} BattlePartyMenuOrder;

static void BattlePartyMenu_Task(SysTask *task, void *data);
static int BattlePartyMenu_Setup(BattlePartyMenu *menu);
static int BattlePartyMenu_HandlePartyListInput(BattlePartyMenu *menu);
static int BattlePartyMenu_TryUseItemOnMon(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleMonMenuInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleSummaryInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleCheckMovesInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleMoveDetailInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleLearnMoveInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleForgetMoveInput(BattlePartyMenu *menu);
static int BattlePartyMenu_HandleRestorePPInput(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowPartyListPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowMonMenuPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowSummaryPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowCheckMovesPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowMoveDetailPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowLearnMovePage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowForgetMovePage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowRestorePPPage(BattlePartyMenu *menu);
static int BattlePartyMenu_RefreshMonPage(BattlePartyMenu *menu);
static int BattlePartyMenu_ShowPendingMessage(BattlePartyMenu *menu);
static int BattlePartyMenu_CloseMessage(BattlePartyMenu *menu);
static int BattlePartyMenu_WaitMessage(const BattlePartyMenu *menu);
static int BattlePartyMenu_WaitMessageInput(const BattlePartyMenu *menu);
static int BattlePartyMenu_WaitButtonAnim(const BattlePartyMenu *menu);
static int BattlePartyMenu_AnimateItemEffect(BattlePartyMenu *menu);
static int BattlePartyMenu_AnimateRestoreAllPP(BattlePartyMenu *menu);
static int BattlePartyMenu_StartFadeOut(BattlePartyMenu *menu);
static int BattlePartyMenu_Exit(SysTask *task, BattlePartyMenu *menu);
static void BattlePartyMenu_InitBgs(BattlePartyMenu *menu);
static void BattlePartyMenu_FreeBgs(BgConfig *bgConfig);
static void BattlePartyMenu_LoadBgGraphics(BattlePartyMenu *menu);
static void BattlePartyMenu_InitText(BattlePartyMenu *menu);
static void BattlePartyMenu_FreeText(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadMonData(BattlePartyMenu *menu);
static BOOL BattlePartyMenu_HandlePartyListSelection(BattlePartyMenu *menu);
static u8 BattlePartyMenu_GetMonMenuInput(BattlePartyMenu *menu);
static u8 BattlePartyMenu_GetSummaryInput(BattlePartyMenu *menu);
static u8 BattlePartyMenu_GetCheckMovesInput(BattlePartyMenu *menu);
static u8 BattlePartyMenu_GetMoveDetailInput(BattlePartyMenu *menu);
static int BattlePartyMenu_CheckTouchHitbox(BattlePartyMenu *menu, const TouchscreenHitbox *hitboxes);
static int BattlePartyMenu_GetSlotType(BattlePartyMenu *menu, int slot);
static u8 BattlePartyMenu_FindNextMon(BattlePartyMenu *menu, int slot, int direction);
static void BattlePartyMenu_DrawExpBar(BattlePartyMenu *menu, int page);
static void BattlePartyMenu_SetExpBarTile(BattlePartyMenu *menu, u16 tile, u16 x, int y);
static void BattlePartyMenu_SetContestInfoTile(BattlePartyMenu *menu, u16 tile, int pos);
static void BattlePartyMenu_ClearContestInfoTiles(BattlePartyMenu *menu);
static void BattlePartyMenu_ChangePage(BattlePartyMenu *menu, int page);
static void BattlePartyMenu_LoadPageTilemaps(BattlePartyMenu *menu, int page);
static int BattlePartyMenu_CanSwitchToSelectedMon(BattlePartyMenu *menu);
static u8 BattlePartyMenu_IsSelectedMonEgg(BattlePartyMenu *menu);
static BOOL BattlePartyMenu_IsDoubleBattle(BattlePartyMenu *menu);
static BOOL BattlePartyMenu_IsMultiBattle(BattlePartyMenu *menu);
static int BattlePartyMenu_IsPartnerMon(BattlePartyMenu *menu, u32 slot);
static BOOL BattlePartyMenu_IsSelectedMoveHM(BattlePartyMenu *menu);
static void BattlePartyMenu_ClearBattleMoveInfo(BattlePartyMenu *menu);
static void BattlePartyMenu_ClearContestMoveInfo(BattlePartyMenu *menu);
static void BattlePartyMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID);
static void BattlePartyMenu_AddWindows(BattlePartyMenu *menu);
static void BattlePartyMenu_AddPageWindows(BattlePartyMenu *menu, int page);
static void BattlePartyMenu_RemovePageWindows(BattlePartyMenu *menu);
static void BattlePartyMenu_RemoveWindows(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintPageText(BattlePartyMenu *menu, int page);
static void BattlePartyMenu_PrintMonName(BattlePartyMenu *menu, int window, int fontId, u8 slot, u8 x, u8 y);
static void BattlePartyMenu_PrintMonLevel(BattlePartyMenu *menu, int window, int slot, int x, u8 y);
static void BattlePartyMenu_PrintMonHP(BattlePartyMenu *menu, int window, int slot, u32 x, u8 y);
static void BattlePartyMenu_DrawHPBar(BattlePartyMenu *menu, int window, int slot, u16 x, u8 y);
static void BattlePartyMenu_PrintAbilityName(BattlePartyMenu *menu, int window, int slot);
static void BattlePartyMenu_PrintHeldItemName(BattlePartyMenu *menu, int window, int slot);
static void BattlePartyMenu_PrintMoveName(BattlePartyMenu *menu, int move, int window, int msgNo, u16 fontId, u16 y, u32 color);
static void BattlePartyMenu_PrintPPLabel(BattlePartyMenu *menu, int window, u16 x, u32 y);
static void BattlePartyMenu_PrintPrompt(BattlePartyMenu *menu, int msgNo);
static void BattlePartyMenu_PrintButtonLabel(BattlePartyMenu *menu, int window, int msgNo);
static void BattlePartyMenu_PrintLevelAndNextExp(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintAttack(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintDefense(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintSpeed(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintSpAttack(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintSpDefense(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintHPStat(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintAbilityDescription(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintAccuracyLabel(BattlePartyMenu *menu, int window);
static void BattlePartyMenu_PrintMoveAccuracy(BattlePartyMenu *menu, int window, int accuracy);
static void BattlePartyMenu_PrintPowerLabel(BattlePartyMenu *menu, int window);
static void BattlePartyMenu_PrintMovePower(BattlePartyMenu *menu, int window, u32 power);
static void BattlePartyMenu_PrintMoveDescription(BattlePartyMenu *menu, int window, int move);
static void BattlePartyMenu_PrintCategoryLabel(BattlePartyMenu *menu, int window);
static void BattlePartyMenu_PrintMoveCategory(BattlePartyMenu *menu, int window, int moveClass);
static void BattlePartyMenu_PrintMovePP(BattlePartyMenu *menu, int window, int pp, int maxPP);
static void BattlePartyMenu_PrintForgetButtonLabel(BattlePartyMenu *menu, int window);
static void BattlePartyMenu_PrintHMCantBeForgotten(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window);
static void BattlePartyMenu_UpdateMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window);
static void BattlePartyMenu_PrintPartyListPage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintMonHPAndBar(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintPartyMonLevel(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintMonMenuPage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintMonNameCentered(BattlePartyMenu *menu, int slot);
static void BattlePartyMenu_PrintCheckMovesPage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintSummaryPage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintMoveDetailPage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintLearnMovePage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintForgetMovePage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintRestorePPPage(BattlePartyMenu *menu);
static void BattlePartyMenu_UpdateMovePP(BattlePartyMenu *menu, int window, int moveSlot);
static void BattlePartyMenu_PrintContestForgetMovePage(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMessage(BattlePartyMenu *menu);
static void BattlePartyMenu_PrintMessage(BattlePartyMenu *menu);
static void BattlePartyMenu_BufferItemResultMessage(BattlePartyMenu *menu);
static void BattlePartyMenu_BufferEmbargoMessage(BattlePartyMenu *menu);
static void BattlePartyMenu_InitSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_CreateSpriteManager(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadMonIconResources(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadStatusIconResources(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadTypeIconResources(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadHeldItemIconResources(BattlePartyMenu *menu);
static void BattlePartyMenu_LoadSpriteResB00B(BattlePartyMenu *menu);
static ManagedSprite *BattlePartyMenu_CreateSprite(BattlePartyMenu *menu, int index);
static void BattlePartyMenu_CreateSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_FreeSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y);
static void BattlePartyMenu_SetMonIconPalettes(BattlePartyMenu *menu);
static void BattlePartyMenu_SetTypeIcon(BattlePartyMenu *menu, ManagedSprite *sprite, int resId, u32 type);
static void BattlePartyMenu_SetMoveCategoryIcon(BattlePartyMenu *menu, ManagedSprite *sprite, u32 moveClass);
static void BattlePartyMenu_ShowStatusIcon(int status, ManagedSprite *sprite, int x, int y);
static void BattlePartyMenu_ShowMonTypeIcons(BattlePartyMenu *menu, BattlePartyMenuMon *mon, const BattlePartyMenuPos *pos);
static void BattlePartyMenu_ShowHeldItemIcon(u16 item, ManagedSprite *sprite, int x, int y);
static void BattlePartyMenu_ShowCapsuleIcon(BOOL hasCapsule, ManagedSprite *sprite, int x, int y);
static void BattlePartyMenu_ShowPageSprites(BattlePartyMenu *menu, u32 page);
static void BattlePartyMenu_ShowPartyListSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMonMenuSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowSummarySprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowCheckMovesSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMoveDetailSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowLearnMoveSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowForgetMoveSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowRestorePPSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowContestForgetMoveSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowContestLearnMoveSprites(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMoveTypeIcons(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMoveContestTypeIcons(BattlePartyMenu *menu);
static void BattlePartyMenu_ShowMoveIcons(BattlePartyMenu *menu);
static void BattlePartyMenu_SetSpriteAnim(ManagedSprite *sprite, u32 anim);
static int BattlePartyMenu_GetMonIconAnim(BattlePartyMenuMon *mon);
static void BattlePartyMenu_UpdateMonIcons(BattlePartyMenu *menu);
static s16 BattlePartyMenu_CalcConditionMarkerCoord(u8 stat, int target, int base);
static void BattlePartyMenu_ShowConditionMarkers(BattlePartyMenu *menu);
static void BattlePartyMenu_CreateCursor(BattlePartyMenu *menu);
static void BattlePartyMenu_FreeCursor(BattlePartyMenu *menu);
static void BattlePartyMenu_SetLearnMoveCursorMask(BattlePartyMenu *menu);
static void BattlePartyMenu_SetForgetMoveCursorMask(BattlePartyMenu *menu);
static void BattlePartyMenu_SetPageCursor(BattlePartyMenu *menu, u32 page);
static void BattlePartyMenu_DeactivateCursor(BattlePartyMenu *menu);
static void BattlePartyMenu_StoreButtonTilemaps(BattlePartyMenu *menu, const u16 *screen);
static void BattlePartyMenu_StoreLargeButtonTilemaps(BattlePartyMenu *menu, const u16 *screen);
static void BattlePartyMenu_CopyTilemapRect(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height);
static u16 *BattlePartyMenu_GetButtonTilemap(BattlePartyMenu *menu, int button, int frame, int partner);
static void BattlePartyMenu_BuildButtonTilemap(BattlePartyMenu *menu, u16 *dst, int button, int frame, u8 palette);
static void BattlePartyMenu_DrawButton(BattlePartyMenu *menu, u8 button, u8 frame, u8 palette);
static void BattlePartyMenu_ScrollButtonWindows(BattlePartyMenu *menu, u32 button, int frame);
static void BattlePartyMenu_OffsetButtonSprites(BattlePartyMenu *menu, u32 button, int frame);
static void BattlePartyMenu_StartButtonAnim(BattlePartyMenu *menu, u32 button);
static void BattlePartyMenu_UpdateButtonAnim(BattlePartyMenu *menu);
static void BattlePartyMenu_DrawPageButtons(BattlePartyMenu *menu, u32 page);
static void BattlePartyMenu_LoadPagePalette(BattlePartyMenu *menu, int page);
static u8 BattlePartyMenu_HasMultipleNonEggMons(BattlePartyMenu *menu);
static void BattlePartyMenu_RedrawSelectedMonButton(BattlePartyMenu *menu);

NarcId MoveCategoryIcon_GetNarc(void);

extern const u32 gBattlePartyMoveNameMsgIds[];
extern const u32 gBattlePartyMonNameMsgIds[];
extern const WindowTemplate gBattlePartyMonMenuWindows[];
extern const WindowTemplate gBattlePartyRestorePPWindows[];
extern const WindowTemplate gBattlePartyLearnMoveWindows[];
extern const WindowTemplate gBattlePartyWindowTemplates[];
extern const WindowTemplate gBattlePartyContestForgetMoveWindows[];
extern const WindowTemplate gBattlePartyCheckMovesWindows[];
extern const WindowTemplate gBattlePartyForgetMoveWindows[];
extern const WindowTemplate gBattlePartyMoveDetailWindows[];
extern const WindowTemplate gBattlePartyPartyListWindows[];
extern const WindowTemplate gBattlePartySummaryWindows[];
extern const BattlePartyMenuPos gBattlePartyForgetMoveTypeIconPos[];
extern const BattlePartyMenuPos gBattlePartyLearnMoveTypeIconPos[];
extern const BattlePartyMenuPos gBattlePartyCheckMovesTypeIconPos[];
extern const BattlePartyMenuPos gBattlePartyMoveDetailTypeIconPos[];
extern const BattlePartyMenuPos gBattlePartySummaryTypeIconPos[];
extern const BattlePartyMenuPos gBattlePartyCheckMovesMoveIconPos[];
extern const BattlePartyMenuPos gBattlePartyLearnMoveMoveIconPos[];
extern const BattlePartyMenuPos gBattlePartyListIconPos[];
extern const BattlePartyMenuPos gBattlePartyListStatusIconPos[];
extern const SpriteResourceCountsListUnion gBattlePartySpriteCapacities;
// {char, palette, cell, anim, draw priority} per sprite
extern const int gBattlePartySpriteTemplates[][5];
extern const u8 gBattlePartyButtonScrollAmounts[];
extern const u8 gBattlePartySlotButtonScrollAmounts[];
extern const s16 gBattlePartyButtonSpriteOffsets[];
extern const s16 gBattlePartySlotButtonSpriteOffsets[];
extern const u8 *const gBattlePartyButtonWindowLists[];
// {x, y, width, height} per button
// {x, y, width, height} of each button on the bottom screen tilemap
extern const u8 gBattlePartyButtonRects[][4];
extern const DpadMenuBox *const gBattlePartyPageCursorBoxes[];

static const BattlePartyMenuOrder sBattlePartyMultiBattleOrder = {
    { 0, 2, 4, 1, 3, 5 }
};

static const GraphicsModes sBattlePartyGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const TouchscreenHitbox sBattlePartyForgetMoveHitboxes[] = {
    { .rect = { 152, 191, 0, 207 } },
    { .rect = { 0, 39, 184, 255 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartyMonMenuHitboxes[] = {
    { .rect = { 8, 143, 8, 247 } },
    { .rect = { 152, 191, 0, 103 } },
    { .rect = { 152, 191, 104, 207 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartySummaryHitboxes[] = {
    { .rect = { 152, 191, 0, 39 } },
    { .rect = { 152, 191, 40, 79 } },
    { .rect = { 152, 191, 96, 199 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartyMoveDetailHitboxes[] = {
    { .rect = { 152, 167, 88, 127 } },
    { .rect = { 152, 167, 128, 167 } },
    { .rect = { 168, 183, 88, 127 } },
    { .rect = { 168, 183, 128, 167 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartyRestorePPHitboxes[] = {
    { .rect = { 48, 95, 0, 127 } },
    { .rect = { 48, 95, 128, 255 } },
    { .rect = { 96, 143, 0, 127 } },
    { .rect = { 96, 143, 128, 255 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const BgTemplate sBattlePartyBgTemplateSub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x1000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattlePartyBgTemplateSub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattlePartyBgTemplateSub0 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x1800,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sBattlePartyBgTemplateSub2 = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const TouchscreenHitbox sBattlePartyLearnMoveHitboxes[] = {
    { .rect = { 48, 95, 0, 127 } },
    { .rect = { 48, 95, 128, 255 } },
    { .rect = { 96, 143, 0, 127 } },
    { .rect = { 96, 143, 128, 255 } },
    { .rect = { 144, 191, 64, 191 } },
    { .rect = { 0, 39, 184, 255 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartyPartyListHitboxes[] = {
    { .rect = { 0, 47, 0, 127 } },
    { .rect = { 8, 55, 128, 255 } },
    { .rect = { 48, 95, 0, 127 } },
    { .rect = { 56, 103, 128, 255 } },
    { .rect = { 96, 143, 0, 127 } },
    { .rect = { 104, 151, 128, 255 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattlePartyCheckMovesHitboxes[] = {
    { .rect = { 48, 95, 0, 127 } },
    { .rect = { 48, 95, 128, 255 } },
    { .rect = { 96, 143, 0, 127 } },
    { .rect = { 96, 143, 128, 255 } },
    { .rect = { 152, 191, 0, 39 } },
    { .rect = { 152, 191, 40, 79 } },
    { .rect = { 152, 191, 96, 199 } },
    { .rect = { 152, 191, 216, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const u32 sBattlePartyPageScreenFiles[][2] = {
    { 1,  0  },
    { 19, 18 },
    { 5,  4  },
    { 3,  2  },
    { 7,  6  },
    { 9,  8  },
    { 11, 10 },
    { 15, 14 },
    { 13, 12 },
    { 17, 16 }
};

#pragma force_active on

void BattlePartyMenu_Create(BattlePartyContext *context) {
    SysTask *task;
    BattlePartyMenu *menu;

    if (context->selectedMon > 5) {
        context->selectedMon = 0;
    }
    task = CreateSysTaskAndEnvironment(BattlePartyMenu_Task, sizeof(BattlePartyMenu), 0, context->heapID);
    menu = SysTask_GetData(task);
    memset(menu, 0, sizeof(BattlePartyMenu));
    menu->context = context;
    menu->bgConfig = BattleSystem_GetBgConfig(context->battleSystem);
    menu->paletteData = BattleSystem_GetPaletteData(context->battleSystem);
    menu->state = 0;
    menu->activeMon = context->selectedMon;
    menu->canToggleView = 0;
    menu->battlerPosition = BattleSystem_GetBattlerType(context->battleSystem, context->battlerId);
}

#pragma force_active reset

enum {
    BATTLE_PARTY_STATE_SETUP,
    BATTLE_PARTY_STATE_PARTY_LIST,
    BATTLE_PARTY_STATE_MON_MENU,
    BATTLE_PARTY_STATE_SUMMARY,
    BATTLE_PARTY_STATE_CHECK_MOVES,
    BATTLE_PARTY_STATE_MOVE_DETAIL,
    BATTLE_PARTY_STATE_SHOW_PARTY_LIST,
    BATTLE_PARTY_STATE_SHOW_MON_MENU,
    BATTLE_PARTY_STATE_SHOW_SUMMARY,
    BATTLE_PARTY_STATE_SHOW_CHECK_MOVES,
    BATTLE_PARTY_STATE_SHOW_MOVE_DETAIL,
    BATTLE_PARTY_STATE_SHOW_LEARN_MOVE,
    BATTLE_PARTY_STATE_SHOW_FORGET_MOVE,
    BATTLE_PARTY_STATE_SHOW_RESTORE_PP,
    BATTLE_PARTY_STATE_REFRESH_MON_PAGE,
    BATTLE_PARTY_STATE_SHOW_PENDING_MESSAGE,
    BATTLE_PARTY_STATE_CLOSE_MESSAGE,
    BATTLE_PARTY_STATE_WAIT_MESSAGE,
    BATTLE_PARTY_STATE_WAIT_MESSAGE_INPUT,
    BATTLE_PARTY_STATE_LEARN_MOVE,
    BATTLE_PARTY_STATE_FORGET_MOVE,
    BATTLE_PARTY_STATE_RESTORE_PP,
    BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM,
    BATTLE_PARTY_STATE_ITEM_EFFECT,
    BATTLE_PARTY_STATE_RESTORE_ALL_PP,
    BATTLE_PARTY_STATE_FADE_OUT,
    BATTLE_PARTY_STATE_EXIT,
};

static void BattlePartyMenu_Task(SysTask *task, void *data) {
    BattlePartyMenu *menu = data;

    switch (menu->state) {
    case BATTLE_PARTY_STATE_SETUP:
        menu->state = BattlePartyMenu_Setup(menu);
        break;
    case BATTLE_PARTY_STATE_PARTY_LIST:
        menu->state = BattlePartyMenu_HandlePartyListInput(menu);
        break;
    case BATTLE_PARTY_STATE_MON_MENU:
        menu->state = BattlePartyMenu_HandleMonMenuInput(menu);
        break;
    case BATTLE_PARTY_STATE_SUMMARY:
        menu->state = BattlePartyMenu_HandleSummaryInput(menu);
        break;
    case BATTLE_PARTY_STATE_CHECK_MOVES:
        menu->state = BattlePartyMenu_HandleCheckMovesInput(menu);
        break;
    case BATTLE_PARTY_STATE_MOVE_DETAIL:
        menu->state = BattlePartyMenu_HandleMoveDetailInput(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_PARTY_LIST:
        menu->state = BattlePartyMenu_ShowPartyListPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_MON_MENU:
        menu->state = BattlePartyMenu_ShowMonMenuPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_SUMMARY:
        menu->state = BattlePartyMenu_ShowSummaryPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_CHECK_MOVES:
        menu->state = BattlePartyMenu_ShowCheckMovesPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_MOVE_DETAIL:
        menu->state = BattlePartyMenu_ShowMoveDetailPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_LEARN_MOVE:
        menu->state = BattlePartyMenu_ShowLearnMovePage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_FORGET_MOVE:
        menu->state = BattlePartyMenu_ShowForgetMovePage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_RESTORE_PP:
        menu->state = BattlePartyMenu_ShowRestorePPPage(menu);
        break;
    case BATTLE_PARTY_STATE_REFRESH_MON_PAGE:
        menu->state = BattlePartyMenu_RefreshMonPage(menu);
        break;
    case BATTLE_PARTY_STATE_SHOW_PENDING_MESSAGE:
        menu->state = BattlePartyMenu_ShowPendingMessage(menu);
        break;
    case BATTLE_PARTY_STATE_CLOSE_MESSAGE:
        menu->state = BattlePartyMenu_CloseMessage(menu);
        break;
    case BATTLE_PARTY_STATE_WAIT_MESSAGE:
        menu->state = BattlePartyMenu_WaitMessage(menu);
        break;
    case BATTLE_PARTY_STATE_WAIT_MESSAGE_INPUT:
        menu->state = BattlePartyMenu_WaitMessageInput(menu);
        break;
    case BATTLE_PARTY_STATE_LEARN_MOVE:
        menu->state = BattlePartyMenu_HandleLearnMoveInput(menu);
        break;
    case BATTLE_PARTY_STATE_FORGET_MOVE:
        menu->state = BattlePartyMenu_HandleForgetMoveInput(menu);
        break;
    case BATTLE_PARTY_STATE_RESTORE_PP:
        menu->state = BattlePartyMenu_HandleRestorePPInput(menu);
        break;
    case BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM:
        menu->state = BattlePartyMenu_WaitButtonAnim(menu);
        break;
    case BATTLE_PARTY_STATE_ITEM_EFFECT:
        menu->state = BattlePartyMenu_AnimateItemEffect(menu);
        break;
    case BATTLE_PARTY_STATE_RESTORE_ALL_PP:
        menu->state = BattlePartyMenu_AnimateRestoreAllPP(menu);
        break;
    case BATTLE_PARTY_STATE_FADE_OUT:
        menu->state = BattlePartyMenu_StartFadeOut(menu);
        break;
    case BATTLE_PARTY_STATE_EXIT:
        if (BattlePartyMenu_Exit(task, menu) == 1) {
            return;
        }
        break;
    }
    BattlePartyMenu_UpdateMonIcons(menu);
    SpriteSystem_DrawSprites(menu->spriteManager);
    BattlePartyMenu_UpdateButtonAnim(menu);
}

static int BattlePartyMenu_Setup(BattlePartyMenu *menu) {
    int nextState;

    G2S_BlendNone();
    if (menu->context->mode == BATTLE_PARTY_MODE_LEARN_MOVE) {
        menu->page = BATTLE_PARTY_PAGE_LEARN_MOVE;
        nextState = BATTLE_PARTY_STATE_LEARN_MOVE;
    } else {
        menu->page = BATTLE_PARTY_PAGE_PARTY_LIST;
        nextState = BATTLE_PARTY_STATE_PARTY_LIST;
    }
    menu->cursor = BattleDpadCursor_New(menu->context->heapID);
    BattlePartyMenu_LoadMonData(menu);
    BattlePartyMenu_InitBgs(menu);
    BattlePartyMenu_LoadBgGraphics(menu);
    BattlePartyMenu_InitText(menu);
    FontID_Alloc(4, menu->context->heapID);
    BattlePartyMenu_LoadPageTilemaps(menu, menu->page);
    BattlePartyMenu_DrawPageButtons(menu, menu->page);
    BattlePartyMenu_LoadPagePalette(menu, menu->page);
    BattlePartyMenu_InitSprites(menu);
    BattlePartyMenu_ShowPageSprites(menu, menu->page);
    BattlePartyMenu_AddWindows(menu);
    BattlePartyMenu_PrintPageText(menu, menu->page);
    if (menu->context->cursorActive) {
        BattleDpadCursor_SetActive(menu->cursor, 1);
    }
    if (menu->page == BATTLE_PARTY_PAGE_PARTY_LIST && BattlePartyMenu_IsPartnerMon(menu, 0) == 1) {
        menu->context->selectedMon = 1;
    }
    BattlePartyMenu_SetPageCursor(menu, menu->page);
    BattlePartyMenu_DrawExpBar(menu, menu->page);
    PaletteData_BeginPaletteFade(menu->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -8, 16, 0, 0);
    return nextState;
}

static int BattlePartyMenu_HandlePartyListInput(BattlePartyMenu *menu) {
    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData) != 0) {
        return BATTLE_PARTY_STATE_PARTY_LIST;
    }
    if (BattlePartyMenu_HandlePartyListSelection(menu) == 1) {
        if (menu->context->selectedMon == 6) {
            if (menu->context->mode != BATTLE_PARTY_MODE_SWITCH_NO_CANCEL) {
                PlaySE(SEQ_SE_DP_DECIDE);
                BattlePartyMenu_StartButtonAnim(menu, 6);
                return BATTLE_PARTY_STATE_FADE_OUT;
            }
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            BattlePartyMenu_StartButtonAnim(menu, menu->context->selectedMon);
            if (menu->context->mode == BATTLE_PARTY_MODE_USE_ITEM) {
                return BattlePartyMenu_TryUseItemOnMon(menu);
            }
            menu->nextState = BATTLE_PARTY_STATE_SHOW_MON_MENU;
            return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
        }
    }
    return BATTLE_PARTY_STATE_PARTY_LIST;
}

static int BattlePartyMenu_TryUseItemOnMon(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;

    if ((context->selectedMon == 0 && context->embargo[0]) || (context->selectedMon == 1 && context->embargo[1])) {
        BattlePartyMenu_BufferEmbargoMessage(menu);
        BattlePartyMenu_ShowMessage(menu);
        menu->context->selectedMon = 6;
        menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        return BATTLE_PARTY_STATE_WAIT_MESSAGE;
    }
    if (GetItemAttr(context->item, ITEMATTR_PP_RESTORE, context->heapID) && !GetItemAttr(context->item, ITEMATTR_PP_RESTORE_ALL, context->heapID) && !menu->mons[context->selectedMon].isEgg) {
        menu->nextState = BATTLE_PARTY_STATE_SHOW_RESTORE_PP;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    if (BattleSystem_RecoverStatus(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon], 0, context->item) == TRUE) {
        if (GetItemAttr(context->item, ITEMATTR_PP_RESTORE_ALL, context->heapID)) {
            menu->nextState = BATTLE_PARTY_STATE_SHOW_RESTORE_PP;
        } else if (BattlePartyMenu_GetSlotType(menu, context->selectedMon) == 1 && !GetItemAttr(context->item, ITEMATTR_REVIVE, context->heapID)) {
            BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
            menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
            context->hpRestored = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_HP, NULL);
            context->hpRestored -= menu->mons[context->selectedMon].hp;
            menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        } else {
            menu->nextState = BATTLE_PARTY_STATE_ITEM_EFFECT;
        }
        menu->effectState = 0;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    ReadMsgDataIntoString(menu->msgData, msg_0006_00081, menu->string);
    BattlePartyMenu_ShowMessage(menu);
    menu->context->selectedMon = 6;
    menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
    return BATTLE_PARTY_STATE_WAIT_MESSAGE;
}

static int BattlePartyMenu_HandleMonMenuInput(BattlePartyMenu *menu) {
    switch (BattlePartyMenu_GetMonMenuInput(menu)) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 7);
        if (BattlePartyMenu_CanSwitchToSelectedMon(menu) == 1) {
            return BATTLE_PARTY_STATE_FADE_OUT;
        }
        menu->nextState = BATTLE_PARTY_STATE_SHOW_PENDING_MESSAGE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 1:
        if (BattlePartyMenu_IsSelectedMonEgg(menu) == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 8);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_SUMMARY;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 2:
        if (BattlePartyMenu_IsSelectedMonEgg(menu) == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 10);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_CHECK_MOVES;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_PARTY_LIST;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_MON_MENU;
}

static int BattlePartyMenu_HandleSummaryInput(BattlePartyMenu *menu) {
    int slot;

    switch (BattlePartyMenu_GetSummaryInput(menu)) {
    case 0:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, -1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 12);
        menu->nextState = BATTLE_PARTY_STATE_REFRESH_MON_PAGE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 1:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, 1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 13);
        menu->nextState = BATTLE_PARTY_STATE_REFRESH_MON_PAGE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 11);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_CHECK_MOVES;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->monMenuPosition = 1;
        menu->nextState = BATTLE_PARTY_STATE_SHOW_MON_MENU;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_SUMMARY;
}

static int BattlePartyMenu_HandleCheckMovesInput(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_GetCheckMovesInput(menu);
    int slot;

    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (menu->mons[menu->context->selectedMon].moves[input].move == MOVE_NONE) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, (u8)(input + 14));
        menu->context->selectedMove = input;
        menu->nextState = BATTLE_PARTY_STATE_SHOW_MOVE_DETAIL;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 4:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, -1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 12);
        menu->nextState = BATTLE_PARTY_STATE_REFRESH_MON_PAGE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 5:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, 1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 13);
        menu->nextState = BATTLE_PARTY_STATE_REFRESH_MON_PAGE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 6:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 9);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_SUMMARY;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 7:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->monMenuPosition = 2;
        menu->nextState = BATTLE_PARTY_STATE_SHOW_MON_MENU;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_CHECK_MOVES;
}

static int BattlePartyMenu_HandleMoveDetailInput(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_GetMoveDetailInput(menu);

    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (menu->context->selectedMove != input) {
            if (menu->mons[menu->context->selectedMon].moves[input].move == MOVE_NONE) {
                break;
            }
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->context->selectedMove = input;
        return BATTLE_PARTY_STATE_SHOW_MOVE_DETAIL;
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_CHECK_MOVES;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_MOVE_DETAIL;
}

static int BattlePartyMenu_HandleLearnMoveInput(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_CheckTouchHitbox(menu, sBattlePartyLearnMoveHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 6;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        menu->context->selectedMove = input;
        menu->learnMovePosition = input;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, (u8)(input + 23));
        menu->nextState = BATTLE_PARTY_STATE_SHOW_FORGET_MOVE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 5:
        if (menu->canToggleView == 0) {
            break;
        }
        menu->viewMode ^= 1;
        menu->learnMovePosition = input;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 18);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_LEARN_MOVE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 6:
        menu->context->selectedMove = 4;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_LEARN_MOVE;
}

static int BattlePartyMenu_HandleForgetMoveInput(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_CheckTouchHitbox(menu, sBattlePartyForgetMoveHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 2;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (menu->viewMode == BATTLE_PARTY_VIEW_BATTLE) {
            BattlePartyMenu_StartButtonAnim(menu, 28);
        } else {
            BattlePartyMenu_StartButtonAnim(menu, 29);
        }
        if (BattlePartyMenu_IsSelectedMoveHM(menu) == 1) {
            BattlePartyMenu_PrintHMCantBeForgotten(menu);
            if (menu->viewMode == BATTLE_PARTY_VIEW_BATTLE) {
                BattlePartyMenu_ClearBattleMoveInfo(menu);
            } else {
                BattlePartyMenu_ClearContestMoveInfo(menu);
            }
            menu->nextState = BATTLE_PARTY_STATE_FORGET_MOVE;
        } else {
            menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        }
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 1:
        if (menu->canToggleView == 0) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 18);
        menu->viewMode ^= 1;
        menu->forgetMovePosition = input;
        menu->nextState = BATTLE_PARTY_STATE_SHOW_FORGET_MOVE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->forgetMovePosition = 0;
        menu->nextState = BATTLE_PARTY_STATE_SHOW_LEARN_MOVE;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_FORGET_MOVE;
}

static int BattlePartyMenu_HandleRestorePPInput(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    int input = BattlePartyMenu_CheckTouchHitbox(menu, sBattlePartyRestorePPHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 4;
        }
    } else {
        BattlePartyMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (menu->mons[context->selectedMon].moves[input].move == MOVE_NONE) {
            break;
        }
        menu->context->selectedMove = input;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, (u8)(input + 19));
        if (BattleSystem_RecoverStatus(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon], input, context->item) == TRUE) {
            menu->effectState = 0;
            menu->nextState = BATTLE_PARTY_STATE_ITEM_EFFECT;
            return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
        }
        ReadMsgDataIntoString(menu->msgData, msg_0006_00081, menu->string);
        BattlePartyMenu_ShowMessage(menu);
        menu->context->selectedMon = 6;
        menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        return BATTLE_PARTY_STATE_WAIT_MESSAGE;
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = BATTLE_PARTY_STATE_SHOW_PARTY_LIST;
        return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
    }
    return BATTLE_PARTY_STATE_RESTORE_PP;
}

static int BattlePartyMenu_ShowPartyListPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_PARTY_LIST);
    return BATTLE_PARTY_STATE_PARTY_LIST;
}

static int BattlePartyMenu_ShowMonMenuPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_MON_MENU);
    return BATTLE_PARTY_STATE_MON_MENU;
}

static int BattlePartyMenu_ShowSummaryPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_SUMMARY);
    return BATTLE_PARTY_STATE_SUMMARY;
}

static int BattlePartyMenu_ShowCheckMovesPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_CHECK_MOVES);
    return BATTLE_PARTY_STATE_CHECK_MOVES;
}

static int BattlePartyMenu_ShowMoveDetailPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_MOVE_DETAIL);
    return BATTLE_PARTY_STATE_MOVE_DETAIL;
}

static int BattlePartyMenu_ShowLearnMovePage(BattlePartyMenu *menu) {
    if (menu->viewMode == BATTLE_PARTY_VIEW_BATTLE) {
        BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_LEARN_MOVE);
    } else {
        BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST);
    }
    return BATTLE_PARTY_STATE_LEARN_MOVE;
}

static int BattlePartyMenu_ShowForgetMovePage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowMoveIcons(menu);
    if (menu->viewMode == BATTLE_PARTY_VIEW_BATTLE) {
        BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_FORGET_MOVE);
    } else {
        BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST);
    }
    return BATTLE_PARTY_STATE_FORGET_MOVE;
}

static int BattlePartyMenu_ShowRestorePPPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, BATTLE_PARTY_PAGE_RESTORE_PP);
    if (GetItemAttr(menu->context->item, ITEMATTR_PP_RESTORE_ALL, menu->context->heapID) != 0) {
        return BATTLE_PARTY_STATE_RESTORE_ALL_PP;
    }
    return BATTLE_PARTY_STATE_RESTORE_PP;
}

static int BattlePartyMenu_RefreshMonPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowPageSprites(menu, menu->page);
    BattlePartyMenu_PrintPageText(menu, menu->page);
    BattlePartyMenu_DrawPageButtons(menu, menu->page);
    BattlePartyMenu_DrawExpBar(menu, menu->page);
    if (menu->page == BATTLE_PARTY_PAGE_SUMMARY) {
        return BATTLE_PARTY_STATE_SUMMARY;
    }
    return BATTLE_PARTY_STATE_CHECK_MOVES;
}

static int BattlePartyMenu_ShowPendingMessage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowMessage(menu);
    menu->nextState = BATTLE_PARTY_STATE_CLOSE_MESSAGE;
    return BATTLE_PARTY_STATE_WAIT_MESSAGE;
}

static int BattlePartyMenu_CloseMessage(BattlePartyMenu *menu) {
    ClearFrameAndWindow2(&menu->windows[1], FALSE);
    return BATTLE_PARTY_STATE_MON_MENU;
}

static int BattlePartyMenu_WaitMessage(const BattlePartyMenu *menu) {
    if (TextPrinterCheckActive(menu->textPrinterId) == 0) {
        return BATTLE_PARTY_STATE_WAIT_MESSAGE_INPUT;
    }
    return BATTLE_PARTY_STATE_WAIT_MESSAGE;
}

static int BattlePartyMenu_WaitMessageInput(const BattlePartyMenu *menu) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == 1) {
        return menu->nextState;
    }
    return BATTLE_PARTY_STATE_WAIT_MESSAGE_INPUT;
}

static int BattlePartyMenu_WaitButtonAnim(const BattlePartyMenu *menu) {
    if (menu->buttonAnimActive == 0) {
        return menu->nextState;
    }
    return BATTLE_PARTY_STATE_WAIT_BUTTON_ANIM;
}

enum {
    ITEM_EFFECT_STATE_START,
    ITEM_EFFECT_STATE_RESTORE_HP,
    ITEM_EFFECT_STATE_RESTORE_PP,
    ITEM_EFFECT_STATE_FINISH,
    ITEM_EFFECT_STATE_START_HP,
};

static int BattlePartyMenu_AnimateItemEffect(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;

    switch (menu->effectState) {
    case ITEM_EFFECT_STATE_START:
        menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
        BattlePartyMenu_BufferItemResultMessage(menu);
        if (menu->page == BATTLE_PARTY_PAGE_RESTORE_PP) {
            menu->ppTargets[0] = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_MOVE1_PP + context->selectedMove, NULL);
            menu->effectState = ITEM_EFFECT_STATE_RESTORE_PP;
        } else {
            int status = Pokemon_GetStatusIconId(menu->mons[context->selectedMon].mon);
            menu->mons[context->selectedMon].status = status;
            if (menu->mons[context->selectedMon].status == PARTY_MON_STATUS_ICON_OK) {
                ManagedSprite_SetDrawFlag(menu->sprites[context->selectedMon + 13], FALSE);
                BattlePartyMenu_PrintPartyMonLevel(menu, context->selectedMon);
            }
            menu->hpTarget = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_HP, NULL);
            menu->effectState = ITEM_EFFECT_STATE_START_HP;
        }
        PlaySE(SEQ_SE_DP_KAIFUKU);
        break;
    case ITEM_EFFECT_STATE_RESTORE_HP:
        if (menu->hpTarget != menu->mons[context->selectedMon].hp) {
            menu->mons[context->selectedMon].hp++;
            BattlePartyMenu_PrintMonHPAndBar(menu, context->selectedMon);
        } else {
            menu->effectState = ITEM_EFFECT_STATE_FINISH;
        }
        break;
    case ITEM_EFFECT_STATE_RESTORE_PP:
        if (menu->ppTargets[0] != menu->mons[context->selectedMon].moves[context->selectedMove].pp) {
            menu->mons[context->selectedMon].moves[context->selectedMove].pp++;
            BattlePartyMenu_UpdateMovePP(menu, (u16)(context->selectedMove + 1), context->selectedMove);
        } else {
            menu->effectState = ITEM_EFFECT_STATE_FINISH;
        }
        break;
    case ITEM_EFFECT_STATE_FINISH:
        BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
        BattlePartyMenu_ShowMessage(menu);
        menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        return BATTLE_PARTY_STATE_WAIT_MESSAGE;
    case ITEM_EFFECT_STATE_START_HP:
        if (menu->hpTarget != menu->mons[context->selectedMon].hp) {
            menu->mons[context->selectedMon].hp++;
            BattlePartyMenu_PrintMonHPAndBar(menu, context->selectedMon);
            BattlePartyMenu_RedrawSelectedMonButton(menu);
        }
        menu->effectState = ITEM_EFFECT_STATE_RESTORE_HP;
        break;
    }
    return BATTLE_PARTY_STATE_ITEM_EFFECT;
}

enum {
    RESTORE_ALL_PP_STATE_START,
    RESTORE_ALL_PP_STATE_RESTORE,
    RESTORE_ALL_PP_STATE_FINISH,
};

static int BattlePartyMenu_AnimateRestoreAllPP(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    u32 i;
    u32 count;

    switch (menu->effectState) {
    case RESTORE_ALL_PP_STATE_START:
        menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
        for (i = 0; i < 4; i++) {
            if (menu->mons[context->selectedMon].moves[i].move != MOVE_NONE) {
                menu->ppTargets[i] = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_MOVE1_PP + i, NULL);
            }
        }
        BattlePartyMenu_BufferItemResultMessage(menu);
        PlaySE(SEQ_SE_DP_KAIFUKU);
        menu->effectState = RESTORE_ALL_PP_STATE_RESTORE;
        break;
    case RESTORE_ALL_PP_STATE_RESTORE:
        count = 0;
        for (i = 0; i < 4; i++) {
            if (menu->mons[context->selectedMon].moves[i].move == MOVE_NONE) {
                count++;
            } else if (menu->ppTargets[i] != menu->mons[context->selectedMon].moves[i].pp) {
                menu->mons[context->selectedMon].moves[i].pp++;
                BattlePartyMenu_UpdateMovePP(menu, (u16)(i + 1), (u16)i);
            } else {
                count++;
            }
        }
        if (count == 4) {
            menu->effectState = RESTORE_ALL_PP_STATE_FINISH;
        }
        break;
    case RESTORE_ALL_PP_STATE_FINISH:
        BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
        BattlePartyMenu_ShowMessage(menu);
        menu->nextState = BATTLE_PARTY_STATE_FADE_OUT;
        return BATTLE_PARTY_STATE_WAIT_MESSAGE;
    }
    return BATTLE_PARTY_STATE_RESTORE_ALL_PP;
}

static int BattlePartyMenu_StartFadeOut(BattlePartyMenu *menu) {
    PaletteData_BeginPaletteFade(menu->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -8, 0, 16, 0);
    return BATTLE_PARTY_STATE_EXIT;
}

static int BattlePartyMenu_Exit(SysTask *task, BattlePartyMenu *menu) {
    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData) != 0) {
        return 0;
    }
    BattlePartyMenu_FreeText(menu);
    BattlePartyMenu_FreeSprites(menu);
    BattlePartyMenu_RemoveWindows(menu);
    BattlePartyMenu_FreeBgs(menu->bgConfig);
    menu->context->cursorActive = BattleDpadCursor_IsActive(menu->cursor);
    BattleDpadCursor_Free(menu->cursor);
    FontID_Release(4);
    menu->context->done = TRUE;
    DestroySysTaskAndEnvironment(task);
    return 1;
}

static void BattlePartyMenu_InitBgs(BattlePartyMenu *menu) {
    GraphicsModes graphicsModes;
    BgTemplate template3;
    BgTemplate template2;
    BgTemplate template1;
    BgTemplate template0;

    graphicsModes = sBattlePartyGraphicsModes;
    SetScreenModeAndDisable(&graphicsModes, SCREEN_SUB);
    template3 = sBattlePartyBgTemplateSub3;
    InitBgFromTemplate(menu->bgConfig, GF_BG_LYR_SUB_3, &template3, GF_BG_TYPE_TEXT);
    template2 = sBattlePartyBgTemplateSub2;
    InitBgFromTemplate(menu->bgConfig, GF_BG_LYR_SUB_2, &template2, GF_BG_TYPE_TEXT);
    template1 = sBattlePartyBgTemplateSub1;
    InitBgFromTemplate(menu->bgConfig, GF_BG_LYR_SUB_1, &template1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(menu->bgConfig, GF_BG_LYR_SUB_1);
    template0 = sBattlePartyBgTemplateSub0;
    InitBgFromTemplate(menu->bgConfig, GF_BG_LYR_SUB_0, &template0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(menu->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, menu->context->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, menu->context->heapID);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, GF_BG_LYR_SUB_0);
}

static void BattlePartyMenu_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
}

static void BattlePartyMenu_LoadBgGraphics(BattlePartyMenu *menu) {
    NARC *narc;
    void *screenFile;
    NNSG2dScreenData *screenData;
    u16 *palette;
    u16 *buffer;
    u32 frame;

    narc = NARC_New(NARC_a_0_7_1, menu->context->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 22, menu->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, menu->context->heapID);
    screenFile = NARC_AllocAndReadWholeMember(narc, 20, menu->context->heapID);
    NNS_G2dGetUnpackedScreenData(screenFile, &screenData);
    BattlePartyMenu_StoreButtonTilemaps(menu, (const u16 *)screenData->rawData);
    Heap_Free(screenFile);
    screenFile = NARC_AllocAndReadWholeMember(narc, 21, menu->context->heapID);
    NNS_G2dGetUnpackedScreenData(screenFile, &screenData);
    BattlePartyMenu_StoreLargeButtonTilemaps(menu, (const u16 *)screenData->rawData);
    Heap_Free(screenFile);
    PaletteData_LoadNarc(menu->paletteData, NARC_a_0_7_1, 23, menu->context->heapID, PLTTBUF_SUB_BG, 0x200, 0);
    NARC_Delete(narc);
    palette = PaletteData_GetUnfadedBuf(menu->paletteData, PLTTBUF_SUB_BG);
    memcpy(menu->pagePalettes, &palette[0xC0], sizeof(menu->pagePalettes));
    PaletteData_LoadNarc(menu->paletteData, NARC_graphic_font, 7, menu->context->heapID, PLTTBUF_SUB_BG, 0x20, 0xD0);
    PaletteData_LoadNarc(menu->paletteData, NARC_graphic_font, 8, menu->context->heapID, PLTTBUF_SUB_BG, 0x20, 0xF0);
    frame = BattleSystem_GetFrame(menu->context->battleSystem);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, GetUserFrameGfx2TilesMember(frame), menu->bgConfig, GF_BG_LYR_SUB_0, 1, 0, FALSE, menu->context->heapID);
    PaletteData_LoadNarc(menu->paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), menu->context->heapID, PLTTBUF_SUB_BG, 0x20, 0xE0);
    // background palette 13: the window frame palette with two colours from palette 9
    palette = PaletteData_GetUnfadedBuf(menu->paletteData, PLTTBUF_SUB_BG);
    buffer = Heap_Alloc(menu->context->heapID, 0x20);
    memcpy(buffer, &palette[0xD0], 0x20);
    memcpy(&buffer[7], &palette[0x9A], 4);
    memcpy(&buffer[3], &palette[0x9C], 4);
    PaletteData_LoadPalette(menu->paletteData, buffer, PLTTBUF_SUB_BG, 0xD0, 0x20);
    Heap_Free(buffer);
}

static void BattlePartyMenu_InitText(BattlePartyMenu *menu) {
    menu->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0006_bin, menu->context->heapID);
    menu->msgPrinter = MessagePrinter_New(15, 14, 0, menu->context->heapID);
    menu->msgFormat = MessageFormat_New(menu->context->heapID);
    menu->string = String_New(0x200, menu->context->heapID);
}

static void BattlePartyMenu_FreeText(BattlePartyMenu *menu) {
    DestroyMsgData(menu->msgData);
    MessagePrinter_Delete(menu->msgPrinter);
    MessageFormat_Delete(menu->msgFormat);
    String_Delete(menu->string);
}

static void BattlePartyMenu_LoadMonData(BattlePartyMenu *menu) {
    u16 i, j;

    for (i = 0; i < Party_GetCount(menu->context->party); i++) {
        menu->mons[i].mon = Party_GetMonByIndex(menu->context->party, i);
        menu->mons[i].species = GetMonData(menu->mons[i].mon, MON_DATA_SPECIES, NULL);
        if (menu->mons[i].species == SPECIES_NONE) {
            continue;
        }
        menu->mons[i].attack = GetMonData(menu->mons[i].mon, MON_DATA_ATK, NULL);
        menu->mons[i].defense = GetMonData(menu->mons[i].mon, MON_DATA_DEF, NULL);
        menu->mons[i].speed = GetMonData(menu->mons[i].mon, MON_DATA_SPEED, NULL);
        menu->mons[i].spAttack = GetMonData(menu->mons[i].mon, MON_DATA_SP_ATK, NULL);
        menu->mons[i].spDefense = GetMonData(menu->mons[i].mon, MON_DATA_SP_DEF, NULL);
        menu->mons[i].hp = GetMonData(menu->mons[i].mon, MON_DATA_HP, NULL);
        menu->mons[i].maxHp = GetMonData(menu->mons[i].mon, MON_DATA_MAX_HP, NULL);
        menu->mons[i].type1 = GetMonData(menu->mons[i].mon, MON_DATA_TYPE_1, NULL);
        menu->mons[i].type2 = GetMonData(menu->mons[i].mon, MON_DATA_TYPE_2, NULL);
        menu->mons[i].level = GetMonData(menu->mons[i].mon, MON_DATA_LEVEL, NULL);
        if (GetMonData(menu->mons[i].mon, MON_DATA_NO_PRINT_GENDER, NULL) == 1) {
            menu->mons[i].hideGender = FALSE;
        } else {
            menu->mons[i].hideGender = TRUE;
        }
        menu->mons[i].gender = GetMonGender(menu->mons[i].mon);
        menu->mons[i].status = Pokemon_GetStatusIconId(menu->mons[i].mon);
        menu->mons[i].isEgg = GetMonData(menu->mons[i].mon, MON_DATA_IS_EGG, NULL);
        menu->mons[i].ability = GetMonData(menu->mons[i].mon, MON_DATA_ABILITY, NULL);
        menu->mons[i].heldItem = GetMonData(menu->mons[i].mon, MON_DATA_HELD_ITEM, NULL);
        menu->mons[i].exp = GetMonData(menu->mons[i].mon, MON_DATA_EXPERIENCE, NULL);
        menu->mons[i].levelExp = GetMonExpBySpeciesAndLevel(menu->mons[i].species, menu->mons[i].level);
        if (menu->mons[i].level == 100) {
            menu->mons[i].nextLevelExp = menu->mons[i].levelExp;
        } else {
            menu->mons[i].nextLevelExp = GetMonExpBySpeciesAndLevel(menu->mons[i].species, menu->mons[i].level + 1);
        }
        menu->mons[i].contestStats[0] = GetMonData(menu->mons[i].mon, MON_DATA_COOL, NULL);
        menu->mons[i].contestStats[1] = GetMonData(menu->mons[i].mon, MON_DATA_BEAUTY, NULL);
        menu->mons[i].contestStats[2] = GetMonData(menu->mons[i].mon, MON_DATA_CUTE, NULL);
        menu->mons[i].contestStats[3] = GetMonData(menu->mons[i].mon, MON_DATA_SMART, NULL);
        menu->mons[i].contestStats[4] = GetMonData(menu->mons[i].mon, MON_DATA_TOUGH, NULL);
        menu->mons[i].ballCapsule = GetMonData(menu->mons[i].mon, MON_DATA_BALL_CAPSULE_ID, NULL);
        menu->mons[i].form = GetMonData(menu->mons[i].mon, MON_DATA_FORM, NULL);
        for (j = 0; j < 4; j++) {
            BattlePartyMenuMove *move = &menu->mons[i].moves[j];
            move->move = GetMonData(menu->mons[i].mon, MON_DATA_MOVE1 + j, NULL);
            if (move->move == MOVE_NONE) {
                continue;
            }
            move->pp = GetMonData(menu->mons[i].mon, MON_DATA_MOVE1_PP + j, NULL);
            move->maxPP = GetMonData(menu->mons[i].mon, MON_DATA_MOVE1_PP_UPS + j, NULL);
            move->maxPP = GetMoveMaxPP(move->move, move->maxPP);
            move->type = GetMoveAttr(move->move, MOVEATTR_TYPE);
            move->moveClass = GetMoveAttr(move->move, MOVEATTR_CLASS);
            move->accuracy = GetMoveAttr(move->move, MOVEATTR_ACCURACY);
            move->power = GetMoveAttr(move->move, MOVEATTR_POWER);
        }
    }
}

static BOOL BattlePartyMenu_HandlePartyListSelection(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_CheckTouchHitbox(menu, sBattlePartyPartyListHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 6;
        } else if (input == -1) {
            return FALSE;
        }
        if (input == 6 || BattlePartyMenu_GetSlotType(menu, input)) {
            menu->context->selectedMon = input;
            return TRUE;
        }
    } else if (input == 6 || BattlePartyMenu_GetSlotType(menu, input)) {
        menu->context->selectedMon = input;
        BattlePartyMenu_DeactivateCursor(menu);
        return TRUE;
    }
    return FALSE;
}

// Touch first, then the d-pad cursor; B picks the given button. 0xFF when nothing was pressed.
#define DEFINE_GET_PAGE_INPUT(name, hitboxes, cancelButton)           \
    static u8 name(BattlePartyMenu *menu) {                           \
        int input = BattlePartyMenu_CheckTouchHitbox(menu, hitboxes); \
        if (input == -1) {                                            \
            input = BattleDpadCursor_HandleInput(menu->cursor);       \
            if (input == -2) {                                        \
                input = cancelButton;                                 \
            } else if (input == -1) {                                 \
                return 0xFF;                                          \
            }                                                         \
        } else {                                                      \
            BattlePartyMenu_DeactivateCursor(menu);                   \
        }                                                             \
        return input;                                                 \
    }

DEFINE_GET_PAGE_INPUT(BattlePartyMenu_GetMonMenuInput, sBattlePartyMonMenuHitboxes, 3)
DEFINE_GET_PAGE_INPUT(BattlePartyMenu_GetSummaryInput, sBattlePartySummaryHitboxes, 3)
DEFINE_GET_PAGE_INPUT(BattlePartyMenu_GetCheckMovesInput, sBattlePartyCheckMovesHitboxes, 7)
DEFINE_GET_PAGE_INPUT(BattlePartyMenu_GetMoveDetailInput, sBattlePartyMoveDetailHitboxes, 4)

static int BattlePartyMenu_CheckTouchHitbox(BattlePartyMenu *menu, const TouchscreenHitbox *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}

// 0: empty slot, 1: a mon the player can use, 2: a mon of the partner trainer (multi battle) or a second
// battler slot
static int BattlePartyMenu_GetSlotType(BattlePartyMenu *menu, int slot) {
    if (menu->mons[slot].species == SPECIES_NONE) {
        return 0;
    }
    if (slot == 0 || ((BattlePartyMenu_IsDoubleBattle(menu) || BattlePartyMenu_IsMultiBattle(menu)) && slot == 1)) {
        return 1;
    }
    return 2;
}

static u8 BattlePartyMenu_FindNextMon(BattlePartyMenu *menu, int slot, int direction) {
    int start = slot;

    if (BattlePartyMenu_IsMultiBattle(menu) == TRUE) {
        BattlePartyMenuOrder order = sBattlePartyMultiBattleOrder;
        for (slot = 0; slot < 6; slot++) {
            if (start == order.slots[slot]) {
                break;
            }
        }
        while (TRUE) {
            slot += direction;
            if (slot < 0) {
                slot = 5;
            } else if (slot >= 6) {
                slot = 0;
            }
            if (start == order.slots[slot]) {
                break;
            }
            if (BattlePartyMenu_GetSlotType(menu, order.slots[slot]) && !menu->mons[order.slots[slot]].isEgg) {
                return order.slots[slot];
            }
        }
    } else {
        while (TRUE) {
            slot += direction;
            if (slot < 0) {
                slot = 5;
            } else if (slot >= 6) {
                slot = 0;
            }
            if (start == slot) {
                break;
            }
            if (BattlePartyMenu_GetSlotType(menu, slot) && !menu->mons[slot].isEgg) {
                return slot;
            }
        }
    }
    return 0xFF;
}

static void BattlePartyMenu_DrawExpBar(BattlePartyMenu *menu, int page) {
    u8 pixels;
    u8 i;
    BattlePartyMenuMon *mon;
    u8 slot;
    u32 exp;
    u32 expToNextLevel;

    if (page != BATTLE_PARTY_PAGE_SUMMARY) {
        return;
    }
    slot = menu->context->selectedMon;
    mon = &menu->mons[slot];
    if (mon->level < 100) {
        u32 levelExp = mon->levelExp;
        expToNextLevel = mon->nextLevelExp - levelExp;
        exp = mon->exp - levelExp;
    } else {
        expToNextLevel = 0;
        exp = 0;
    }
    pixels = CalculateHpBarPixelsLength(exp, expToNextLevel, 64);
    for (i = 0; i < 8; i++) {
        u16 tile;
        if (pixels >= 8) {
            tile = 0x1E;
        } else {
            tile = pixels + 0x16;
        }
        BattlePartyMenu_SetExpBarTile(menu, tile, i + 10, 8);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels -= 8;
        }
    }
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, GF_BG_LYR_SUB_3);
}

static void BattlePartyMenu_SetExpBarTile(BattlePartyMenu *menu, u16 tile, u16 x, int y) {
    FillBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_3, tile, (u8)x, (u8)y, 1, 1, TILEMAP_FILL_KEEP_PAL);
}

// writes the 2x2 tile block at row 14 of background 7 for one of six slots
static void BattlePartyMenu_SetContestInfoTile(BattlePartyMenu *menu, u16 tile, int pos) {
    FillBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_3, tile, pos * 2 + 2, 14, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_3, tile + 1, pos * 2 + 3, 14, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_3, tile + 32, pos * 2 + 2, 15, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_3, tile + 33, pos * 2 + 3, 15, 1, 1, TILEMAP_FILL_KEEP_PAL);
}

static void BattlePartyMenu_ClearContestInfoTiles(BattlePartyMenu *menu) {
    u16 i;

    for (i = 0; i < 6; i++) {
        BattlePartyMenu_SetContestInfoTile(menu, 0x125, (u8)i);
    }
}

static void BattlePartyMenu_ChangePage(BattlePartyMenu *menu, int page) {
    BattlePartyMenu_LoadPageTilemaps(menu, page);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, GF_BG_LYR_SUB_0, 0);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, GF_BG_LYR_SUB_1, 0);
    BattlePartyMenu_ShowPageSprites(menu, page);
    BattlePartyMenu_RemovePageWindows(menu);
    BattlePartyMenu_AddPageWindows(menu, page);
    BattlePartyMenu_PrintPageText(menu, page);
    BattlePartyMenu_DrawExpBar(menu, page);
    BattlePartyMenu_SetPageCursor(menu, page);
    BattlePartyMenu_DrawPageButtons(menu, page);
    BattlePartyMenu_LoadPagePalette(menu, page);
    menu->page = page;
}

static void BattlePartyMenu_LoadPageTilemaps(BattlePartyMenu *menu, int page) {
    u32 i;
    const u32 *files = sBattlePartyPageScreenFiles[page];

    for (i = 0; i < 2; i++) {
        NNSG2dScreenData *screenData;
        void *screenFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_7_1, files[i], menu->context->heapID);
        NNS_G2dGetUnpackedScreenData(screenFile, &screenData);
        LoadRectToBgTilemapRect(menu->bgConfig, i + 6, screenData->rawData, 0, 0, 32, 24);
        ScheduleBgTilemapBufferTransfer(menu->bgConfig, i + 6);
        Heap_Free(screenFile);
    }
}

// Expands message msgNo with the mon's nickname into menu->string.
static inline void BattlePartyMenu_BufferSlotMonMessage(BattlePartyMenu *menu, BattlePartyMenuMon *slot, int msgNo) {
    String *template = NewString_ReadMsgData(menu->msgData, msgNo);
    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(slot->mon));
    StringExpandPlaceholders(menu->msgFormat, menu->string, template);
    String_Delete(template);
}

static inline void BattlePartyMenu_BufferMonMessage(BattlePartyMenu *menu, Pokemon *mon, int msgNo) {
    String *template = NewString_ReadMsgData(menu->msgData, msgNo);
    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(menu->msgFormat, menu->string, template);
    String_Delete(template);
}

// Buffers the reason the selected mon cannot be sent out; returns 1 if it can be.
static int BattlePartyMenu_CanSwitchToSelectedMon(BattlePartyMenu *menu) {
    BattlePartyContext *context;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    if (BattlePartyMenu_IsPartnerMon(menu, menu->context->selectedMon) == 1) {
        String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00080);
        int partner;
        context = menu->context;
        partner = BattleSystem_GetBattlerIdPartner(context->battleSystem, context->battlerId);
        Trainer *trainer = BattleSystem_GetTrainer(menu->context->battleSystem, partner);
        BufferTrainerNameFromDataStruct(menu->msgFormat, 0, trainer);
        StringExpandPlaceholders(menu->msgFormat, menu->string, template);
        String_Delete(template);
        return 0;
    }
    if (mon->hp == 0) {
        BattlePartyMenu_BufferSlotMonMessage(menu, mon, 77);
        return 0;
    }
    context = menu->context;
    {
        u8 index = context->partyOrder[context->selectedMon];
        if (context->battlingMons[0] == index || context->battlingMons[1] == index) {
            BattlePartyMenu_BufferSlotMonMessage(menu, mon, 76);
            return 0;
        }
    }
    if (BattlePartyMenu_IsSelectedMonEgg(menu) == 1) {
        ReadMsgDataIntoString(menu->msgData, msg_0006_00079, menu->string);
        return 0;
    }
    context = menu->context;
    if (context->reservedMon != 6) {
        u8 slot = context->selectedMon;
        if (context->reservedMon == context->partyOrder[slot]) {
            String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00093);
            BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(menu->mons[slot].mon));
            StringExpandPlaceholders(menu->msgFormat, menu->string, template);
            String_Delete(template);
            return 0;
        }
    }
    if (context->move) {
        BattlePartyMenuMon *active = &menu->mons[menu->activeMon];
        String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00078);
        BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(active->mon));
        StringExpandPlaceholders(menu->msgFormat, menu->string, template);
        String_Delete(template);
        return 0;
    }
    return 1;
}

static u8 BattlePartyMenu_IsSelectedMonEgg(BattlePartyMenu *menu) {
    u8 slot = menu->context->selectedMon;

    if (menu->mons[slot].isEgg != 0) {
        return 1;
    }
    return 0;
}

// Multi battles alongside an AI partner are not treated as double or multi battles here.
#define BATTLE_TYPE_AI_MULTI (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI)

static BOOL BattlePartyMenu_IsDoubleBattle(BattlePartyMenu *menu) {
    u32 battleType = BattleSystem_GetBattleType(menu->context->battleSystem);

    if (battleType != BATTLE_TYPE_AI_MULTI && battleType != (BATTLE_TYPE_TRAINER | BATTLE_TYPE_AI_MULTI) && (battleType & (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TAG)) != 0) {
        return TRUE;
    }
    return FALSE;
}

static BOOL BattlePartyMenu_IsMultiBattle(BattlePartyMenu *menu) {
    u32 battleType = BattleSystem_GetBattleType(menu->context->battleSystem);

    if (battleType != BATTLE_TYPE_AI_MULTI && battleType != (BATTLE_TYPE_TRAINER | BATTLE_TYPE_AI_MULTI) && (battleType & BATTLE_TYPE_MULTI) != 0) {
        return TRUE;
    }
    return FALSE;
}

// In a multi battle, the slots of the partner trainer's mons: the odd slots, or the even ones for battler
// position 2.
static int BattlePartyMenu_IsPartnerMon(BattlePartyMenu *menu, u32 slot) {
    if (BattlePartyMenu_IsMultiBattle(menu) == TRUE) {
        if (menu->battlerPosition == 2) {
            if (slot & 1) {
                return 1;
            }
        } else {
            if (!(slot & 1)) {
                return 1;
            }
        }
    }
    return 0;
}

static BOOL BattlePartyMenu_IsSelectedMoveHM(BattlePartyMenu *menu) {
    u16 move;

    if (menu->context->selectedMove == 4) {
        move = menu->context->move;
    } else {
        move = menu->mons[menu->context->selectedMon].moves[menu->context->selectedMove].move;
    }
    return MoveIsHM(move);
}

static void BattlePartyMenu_ClearBattleMoveInfo(BattlePartyMenu *menu) {
    ManagedSprite_SetDrawFlag(menu->sprites[26], FALSE);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[10]);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[6]);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[7]);
}

static void BattlePartyMenu_ClearContestMoveInfo(BattlePartyMenu *menu) {
    BattlePartyMenu_ClearContestInfoTiles(menu);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, GF_BG_LYR_SUB_3);
}

static void BattlePartyMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID) {
    if (item != ITEM_BLUE_FLUTE && item != ITEM_RED_FLUTE && item != ITEM_YELLOW_FLUTE) {
        Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, heapID);
    }
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}

static void BattlePartyMenu_AddWindows(BattlePartyMenu *menu) {
    u32 i;

    for (i = 0; i < 2; i++) {
        AddWindow(menu->bgConfig, &menu->windows[i], &gBattlePartyWindowTemplates[i]);
    }
    BattlePartyMenu_AddPageWindows(menu, menu->page);
}

static void BattlePartyMenu_AddPageWindows(BattlePartyMenu *menu, int page) {
    const WindowTemplate *templates;
    u8 i;

    switch (page) {
    case BATTLE_PARTY_PAGE_PARTY_LIST:
        templates = gBattlePartyPartyListWindows;
        menu->pageWindowCount = 6;
        break;
    case BATTLE_PARTY_PAGE_MON_MENU:
        templates = gBattlePartyMonMenuWindows;
        menu->pageWindowCount = 4;
        break;
    case BATTLE_PARTY_PAGE_SUMMARY:
        templates = gBattlePartySummaryWindows;
        menu->pageWindowCount = 35;
        break;
    case BATTLE_PARTY_PAGE_CHECK_MOVES:
        templates = gBattlePartyCheckMovesWindows;
        menu->pageWindowCount = 11;
        break;
    case BATTLE_PARTY_PAGE_MOVE_DETAIL:
        templates = gBattlePartyMoveDetailWindows;
        menu->pageWindowCount = 17;
        break;
    case BATTLE_PARTY_PAGE_RESTORE_PP:
        templates = gBattlePartyRestorePPWindows;
        menu->pageWindowCount = 5;
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE:
    case BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST:
        templates = gBattlePartyLearnMoveWindows;
        menu->pageWindowCount = 6;
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE:
        templates = gBattlePartyForgetMoveWindows;
        menu->pageWindowCount = 12;
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST:
        templates = gBattlePartyContestForgetMoveWindows;
        menu->pageWindowCount = 7;
        break;
    }
    menu->pageWindows = AllocWindows(menu->context->heapID, menu->pageWindowCount);
    for (i = 0; i < menu->pageWindowCount; i++) {
        AddWindow(menu->bgConfig, &menu->pageWindows[i], &templates[i]);
    }
}

static void BattlePartyMenu_RemovePageWindows(BattlePartyMenu *menu) {
    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
}

static void BattlePartyMenu_RemoveWindows(BattlePartyMenu *menu) {
    u32 i;

    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
    for (i = 0; i < 2; i++) {
        RemoveWindow(&menu->windows[i]);
    }
}

static void BattlePartyMenu_PrintPageText(BattlePartyMenu *menu, int page) {
    switch (page) {
    case BATTLE_PARTY_PAGE_PARTY_LIST:
        BattlePartyMenu_PrintPartyListPage(menu);
        break;
    case BATTLE_PARTY_PAGE_MON_MENU:
        BattlePartyMenu_PrintMonMenuPage(menu);
        break;
    case BATTLE_PARTY_PAGE_SUMMARY:
        BattlePartyMenu_PrintSummaryPage(menu);
        break;
    case BATTLE_PARTY_PAGE_CHECK_MOVES:
        BattlePartyMenu_PrintCheckMovesPage(menu);
        break;
    case BATTLE_PARTY_PAGE_MOVE_DETAIL:
        BattlePartyMenu_PrintMoveDetailPage(menu);
        break;
    case BATTLE_PARTY_PAGE_RESTORE_PP:
        BattlePartyMenu_PrintRestorePPPage(menu);
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE:
    case BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST:
        BattlePartyMenu_PrintLearnMovePage(menu);
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE:
        BattlePartyMenu_PrintForgetMovePage(menu);
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST:
        BattlePartyMenu_PrintContestForgetMovePage(menu);
        break;
    }
}

typedef u8 (*AddTextPrinterWithColorFunc)(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

static void BattlePartyMenu_PrintMonName(BattlePartyMenu *menu, int window, int fontId, u8 slot, u8 x, u8 y) {
    BattlePartyMenuMon *mon;
    String *name;
    String *template;
    Window *windows;
    Window *win;

    windows = menu->pageWindows;
    win = &windows[window];
    mon = &menu->mons[slot];
    name = String_New(12, menu->context->heapID);
    template = NewString_ReadMsgData(menu->msgData, gBattlePartyMonNameMsgIds[slot]);
    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon->mon));
    StringExpandPlaceholders(menu->msgFormat, name, template);
    if (fontId == 0) {
        ((AddTextPrinterWithColorFunc)AddTextPrinterParameterizedWithColor)(win, fontId, name, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    } else {
        ((AddTextPrinterWithColorFunc)AddTextPrinterParameterizedWithColor)(win, fontId, name, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 9), NULL);
    }
    String_Delete(template);
    String_Delete(name);
    if (mon->hideGender == FALSE && mon->isEgg == FALSE) {
        if (mon->gender == MON_MALE) {
            String *symbol = NewString_ReadMsgData(menu->msgData, msg_0006_00016);
            u32 width = GetWindowWidth(win);
            int symbolX = width * 8 - FontID_String_GetWidth(0, symbol, 0);
            if (fontId == 0) {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), NULL);
            } else {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(10, 11, 0), NULL);
            }
            String_Delete(symbol);
        } else if (mon->gender == MON_FEMALE) {
            String *symbol = NewString_ReadMsgData(menu->msgData, msg_0006_00017);
            u32 width = GetWindowWidth(win);
            int symbolX = width * 8 - FontID_String_GetWidth(0, symbol, 0);
            if (fontId == 0) {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
            } else {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(12, 13, 0), NULL);
            }
            String_Delete(symbol);
        }
    }
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMonLevel(BattlePartyMenu *menu, int window, int slot, int x, u8 y) {
    u32 level = menu->mons[slot].level;

    MessagePrinter_PrintGlyphAndUInt(menu->msgPrinter, 1, level, 3, PRINTING_MODE_LEFT_ALIGN, &menu->pageWindows[window], x + 8, y);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_PrintMonHP(BattlePartyMenu *menu, int window, int slot, u32 x, u8 y) {
    BattlePartyMenuMon *mon = &menu->mons[slot];

    PrintUIntOnWindow(menu->msgPrinter, mon->hp, 3, PRINTING_MODE_RIGHT_ALIGN, &menu->pageWindows[window], x, y);
    MessagePrinter_PrintGlyph(menu->msgPrinter, 0, &menu->pageWindows[window], x + 24, y);
    x += 32;
    PrintUIntOnWindow(menu->msgPrinter, mon->maxHp, 3, PRINTING_MODE_LEFT_ALIGN, &menu->pageWindows[window], x, y);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_DrawHPBar(BattlePartyMenu *menu, int window, int slot, u16 x, u8 y) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u8 color = 1;
    u8 width = CalculateHpBarPixelsLength(mon->hp, mon->maxHp, 48);

    switch (CalculateHpBarColor(mon->hp, mon->maxHp, 48)) {
    case 0:
        ScheduleWindowCopyToVram(&menu->pageWindows[window]);
        return;
    case 3:
    case 4:
        color = 1;
        break;
    case 2:
        color = 3;
        break;
    case 1:
        color = 5;
        break;
    }
    FillWindowPixelRect(&menu->pageWindows[window], color + 1, x, y + 1, width, 1);
    FillWindowPixelRect(&menu->pageWindows[window], color, x, y + 2, width, 2);
    FillWindowPixelRect(&menu->pageWindows[window], color + 1, x, y + 4, width, 1);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_PrintAbilityName(BattlePartyMenu *menu, int window, int slot) {
    String *string = String_New(16, menu->context->heapID);
    String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00008);

    BufferAbilityName(menu->msgFormat, 0, menu->mons[slot].ability);
    StringExpandPlaceholders(menu->msgFormat, string, template);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(template);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_PrintHeldItemName(BattlePartyMenu *menu, int window, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    String *string;

    if (mon->heldItem == ITEM_NONE) {
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00020);
    } else {
        String *template;
        string = String_New(18, menu->context->heapID);
        template = NewString_ReadMsgData(menu->msgData, msg_0006_00009);
        BufferItemName(menu->msgFormat, 0, mon->heldItem);
        StringExpandPlaceholders(menu->msgFormat, string, template);
        String_Delete(template);
    }
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_PrintMoveName(BattlePartyMenu *menu, int move, int window, int msgNo, u16 fontId, u16 y, u32 color) {
    Window *win = &menu->pageWindows[window];
    String *string = String_New(16, menu->context->heapID);
    String *template = NewString_ReadMsgData(menu->msgData, msgNo);
    int x;

    BufferMoveName(menu->msgFormat, 0, move);
    StringExpandPlaceholders(menu->msgFormat, string, template);
    if (fontId == 4) {
        x = ((GetWindowWidth(win) << 3) - FontID_String_GetWidth(fontId, string, 0)) >> 1;
    } else {
        x = 0;
    }
    ((AddTextPrinterWithColorFunc)AddTextPrinterParameterizedWithColor)(win, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(template);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintPPLabel(BattlePartyMenu *menu, int window, u16 x, u32 y) {
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00014);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

static void BattlePartyMenu_PrintPrompt(BattlePartyMenu *menu, int msgNo) {
    String *string;

    DrawFrameAndWindow2(&menu->windows[0], TRUE, 1, 14);
    FillWindowPixelBuffer(&menu->windows[0], 15);
    string = NewString_ReadMsgData(menu->msgData, msgNo);
    AddTextPrinterParameterizedWithColor(&menu->windows[0], 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->windows[0]);
}

static void BattlePartyMenu_PrintButtonLabel(BattlePartyMenu *menu, int window, int msgNo) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msgNo);
    u32 width = FontID_String_GetWidth(4, string, 0);
    u32 windowWidth = GetWindowWidth(win);

    AddTextPrinterParameterizedWithColor(win, 4, string, (windowWidth * 8 - width) / 2, 5, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 9), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

// The summary page has two sets of 22 windows (`windowSet`); the stat labels sit in shared windows 13-21.
static void BattlePartyMenu_PrintLevelAndNextExp(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u16 base = menu->windowSet * 22;
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00023);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[19], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00024);
    String *expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + 11], 0, expanded, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00025);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[20], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00026);
    expanded = String_New(14, menu->context->heapID);
    if (mon->level < 100) {
        BufferIntegerAsString(menu->msgFormat, 0, mon->nextLevelExp - mon->exp, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    } else {
        BufferIntegerAsString(menu->msgFormat, 0, 0, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    }
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    u32 width = GetWindowWidth(&menu->pageWindows[base + 12]);
    u16 x = width * 8 - FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + 12], 0, expanded, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(&menu->pageWindows[19]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 11]);
    ScheduleWindowCopyToVram(&menu->pageWindows[20]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 12]);
}

#define DEFINE_PRINT_STAT(name, labelWindow, labelMsgNo, valueMsgNo, field, valueWindow)                                                                        \
    static void name(BattlePartyMenu *menu, int slot) {                                                                                                         \
        u16 base = menu->windowSet * 22;                                                                                                                        \
        String *string = NewString_ReadMsgData(menu->msgData, labelMsgNo);                                                                                      \
        AddTextPrinterParameterizedWithColor(&menu->pageWindows[labelWindow], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);        \
        String_Delete(string);                                                                                                                                  \
        String *template = NewString_ReadMsgData(menu->msgData, valueMsgNo);                                                                                    \
        String *expanded = String_New(8, menu->context->heapID);                                                                                                \
        u16 value = menu->mons[slot].field;                                                                                                                     \
        BufferIntegerAsString(menu->msgFormat, 0, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);                                                                    \
        StringExpandPlaceholders(menu->msgFormat, expanded, template);                                                                                          \
        u8 width = FontID_String_GetWidth(0, expanded, 0);                                                                                                      \
        u8 x = GetWindowWidth(&menu->pageWindows[base + valueWindow]) * 8 - width;                                                                              \
        AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + valueWindow], 0, expanded, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL); \
        String_Delete(template);                                                                                                                                \
        String_Delete(expanded);                                                                                                                                \
        ScheduleWindowCopyToVram(&menu->pageWindows[labelWindow]);                                                                                              \
        ScheduleWindowCopyToVram(&menu->pageWindows[base + valueWindow]);                                                                                       \
    }

DEFINE_PRINT_STAT(BattlePartyMenu_PrintAttack, 14, 32, 33, attack, 5)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintDefense, 15, 34, 35, defense, 6)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpeed, 16, 40, 41, speed, 7)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpAttack, 17, 36, 37, spAttack, 8)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpDefense, 18, 38, 39, spDefense, 9)

static void BattlePartyMenu_PrintHPStat(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u16 base = menu->windowSet * 22;
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00028);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[13], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00031);
    u32 slashWidth = FontID_String_GetWidth(0, string, 0);
    u32 windowWidth = GetWindowWidth(&menu->pageWindows[4]);
    u16 middle = (windowWidth * 8 - slashWidth) / 2;
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + 4], 0, string, middle, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00029);
    String *expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->hp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    u32 width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + 4], 0, expanded, middle - width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);

    string = NewString_ReadMsgData(menu->msgData, msg_0006_00030);
    expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->maxHp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[base + 4], 0, expanded, middle + slashWidth, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(&menu->pageWindows[13]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 4]);
}

static void BattlePartyMenu_PrintAbilityDescription(BattlePartyMenu *menu, int slot) {
    int window = menu->windowSet * 22;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0722_bin, menu->context->heapID);
    u16 msgNo = menu->mons[slot].ability;
    String *string = NewString_ReadMsgData(msgData, msgNo);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window + 2], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(&menu->pageWindows[window + 2]);
}

static void BattlePartyMenu_PrintAccuracyLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00051);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMoveAccuracy(BattlePartyMenu *menu, int window, int accuracy) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];

    if (accuracy == 0) {
        String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00050);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
    } else {
        String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00052);
        String *string = String_New(8, menu->context->heapID);
        BufferIntegerAsString(menu->msgFormat, 0, accuracy, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(menu->msgFormat, string, template);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(template);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintPowerLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00048);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMovePower(BattlePartyMenu *menu, int window, u32 power) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];

    if (power <= 1) {
        String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00050);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
    } else {
        String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00049);
        String *string = String_New(8, menu->context->heapID);
        BufferIntegerAsString(menu->msgFormat, 0, power, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(menu->msgFormat, string, template);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(template);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMoveDescription(BattlePartyMenu *menu, int window, int move) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0749_bin, menu->context->heapID);
    String *string = NewString_ReadMsgData(msgData, move);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintCategoryLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00053);
    u16 width = FontID_String_GetWidth(0, string, 0);
    u16 x = (GetWindowWidth(win) * 8 - width) / 2;

    AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMoveCategory(BattlePartyMenu *menu, int window, int moveClass) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string;

    switch (moveClass) {
    case CATEGORY_PHYSICAL:
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00054);
        break;
    case CATEGORY_SPECIAL:
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00056);
        break;
    case CATEGORY_STATUS:
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00055);
        break;
    }
    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintMovePP(BattlePartyMenu *menu, int window, int pp, int maxPP) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00046);
    u32 slashWidth = FontID_String_GetWidth(0, string, 0);
    u32 middle = (GetWindowWidth(win) * 8 - slashWidth) / 2;

    AddTextPrinterParameterizedWithColor(win, 0, string, middle, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00044);
    String *expanded = String_New(6, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, pp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    u32 width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, middle - width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00045);
    expanded = String_New(6, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, maxPP, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, middle + slashWidth, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintForgetButtonLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string;
    u32 width;

    if (menu->context->selectedMove == 4) {
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00059);
    } else {
        string = NewString_ReadMsgData(menu->msgData, msg_0006_00058);
    }
    width = FontID_String_GetWidth(4, string, 0);
    AddTextPrinterParameterizedWithColor(win, 4, string, (96 - width) / 2, 5, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 9), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_PrintHMCantBeForgotten(BattlePartyMenu *menu) {
    Window *window;
    String *string;

    if (menu->page == BATTLE_PARTY_PAGE_FORGET_MOVE) {
        window = &menu->pageWindows[8];
    } else {
        window = &menu->pageWindows[5];
    }
    FillWindowPixelBuffer(window, 0);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00060);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

static void BattlePartyMenu_PrintMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *expanded = String_New(6, menu->context->heapID);
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00043);

    AddTextPrinterParameterizedWithColor(win, 0, string, 40, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00046);
    u32 width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(win, 0, string, 80, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00045);
    BufferIntegerAsString(menu->msgFormat, 0, move->maxPP, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width += 80;
    AddTextPrinterParameterizedWithColor(win, 0, expanded, width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00044);
    BufferIntegerAsString(menu->msgFormat, 0, move->pp, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, 80 - width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

static void BattlePartyMenu_UpdateMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *expanded = String_New(6, menu->context->heapID);
    String *string = NewString_ReadMsgData(menu->msgData, msg_0006_00043);
    u32 width = FontID_String_GetWidth(0, string, 0);

    String_Delete(string);
    width += 40;
    FillWindowPixelRect(win, 0, (u16)width, 24, (u16)(80 - width), 16);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00044);
    BufferIntegerAsString(menu->msgFormat, 0, move->pp, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, 80 - width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

typedef void (*PrintMonNameFunc)(BattlePartyMenu *menu, int window, int fontId, int slot, int x, int y);

static void BattlePartyMenu_PrintPartyListPage(BattlePartyMenu *menu) {
    int i;

    for (i = 0; i < Party_GetCount(menu->context->party); i++) {
        FillWindowPixelBuffer(&menu->pageWindows[i], 0);
        if (menu->mons[i].species != SPECIES_NONE) {
            ((PrintMonNameFunc)BattlePartyMenu_PrintMonName)(menu, i, 4, (u16)i, 32, 7);
            if (menu->mons[i].isEgg == FALSE) {
                BattlePartyMenu_PrintMonHPAndBar(menu, (u8)i);
            }
            if (Pokemon_GetStatusIconId(menu->mons[i].mon) == 7) {
                BattlePartyMenu_PrintPartyMonLevel(menu, (u8)i);
            }
        }
    }
    if (menu->context->mode == BATTLE_PARTY_MODE_USE_ITEM) {
        BattlePartyMenu_PrintPrompt(menu, msg_0006_00007);
    } else {
        BattlePartyMenu_PrintPrompt(menu, msg_0006_00006);
    }
}

static void BattlePartyMenu_PrintMonHPAndBar(BattlePartyMenu *menu, int slot) {
    FillWindowPixelRect(&menu->pageWindows[slot], 0, 56, 32, 24, 8);
    FillWindowPixelRect(&menu->pageWindows[slot], 0, 64, 24, 64, 8);
    BattlePartyMenu_PrintMonHP(menu, slot, slot, 56, 32);
    BattlePartyMenu_DrawHPBar(menu, slot, slot, 64, 24);
}

static void BattlePartyMenu_PrintPartyMonLevel(BattlePartyMenu *menu, int slot) {
    if (menu->mons[slot].isEgg == FALSE) {
        BattlePartyMenu_PrintMonLevel(menu, slot, slot, 0, 32);
    }
}

static void BattlePartyMenu_PrintMonMenuPage(BattlePartyMenu *menu) {
    FillWindowPixelBuffer(menu->pageWindows, 0);
    FillWindowPixelBuffer(menu->pageWindows + 1, 0);
    FillWindowPixelBuffer(menu->pageWindows + 2, 0);
    FillWindowPixelBuffer(menu->pageWindows + 3, 0);
    BattlePartyMenu_PrintMonNameCentered(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintButtonLabel(menu, 1, msg_0006_00015);
    if (!menu->mons[menu->context->selectedMon].isEgg) {
        BattlePartyMenu_PrintButtonLabel(menu, 2, msg_0006_00018);
        BattlePartyMenu_PrintButtonLabel(menu, 3, msg_0006_00019);
        return;
    }
    ScheduleWindowCopyToVram(menu->pageWindows + 2);
    ScheduleWindowCopyToVram(menu->pageWindows + 3);
}

typedef struct BattlePartyMenuNameLocals {
    String *volatile name;
    BattlePartyMenuMon *volatile mon;
    Window *volatile window;
} BattlePartyMenuNameLocals;

static inline String *BattlePartyMenu_ReadGenderSymbol(BattlePartyMenu *menu, BattlePartyMenuNameLocals *locals) {
    String *symbol = NULL;
    if (!locals->mon->hideGender) {
        BattlePartyMenuMon *mon = locals->mon;
        if (!mon->isEgg) {
            if (mon->gender == MON_MALE) {
                symbol = NewString_ReadMsgData(menu->msgData, msg_0006_00016);
            } else if (mon->gender == MON_FEMALE) {
                symbol = NewString_ReadMsgData(menu->msgData, msg_0006_00017);
            }
        }
    }
    return symbol;
}

static void BattlePartyMenu_PrintMonNameCentered(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuNameLocals locals;
    locals.window = menu->pageWindows;
    locals.mon = &menu->mons[slot];
    locals.name = String_New(12, menu->context->heapID);
    String *template = NewString_ReadMsgData(menu->msgData, gBattlePartyMonNameMsgIds[slot]);
    String *symbol, *name;
    u8 width, symbolWidth, x;
    int gap;

    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(menu->mons[slot].mon));
    StringExpandPlaceholders(menu->msgFormat, locals.name, template);
    String_Delete(template);
    symbol = BattlePartyMenu_ReadGenderSymbol(menu, &locals);
    width = FontID_String_GetWidth(4, locals.name, 0);
    if (symbol == NULL) {
        symbolWidth = 0;
        gap = 0;
    } else {
        symbolWidth = FontID_String_GetWidth(0, symbol, 0);
        gap = 8;
    }
    x = (GetWindowWidth(locals.window) * 8 - width - symbolWidth - gap) / 2;
    name = locals.name;
    AddTextPrinterParameterizedWithColor(locals.window, 4, name, x, 7, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 9), NULL);
    String_Delete(locals.name);
    if (symbol) {
        if (locals.mon->gender == MON_MALE) {
            AddTextPrinterParameterizedWithColor(locals.window, 0, symbol, gap + (x + width), 8, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(10, 11, 0), NULL);
        } else {
            AddTextPrinterParameterizedWithColor(locals.window, 0, symbol, gap + (x + width), 8, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(12, 13, 0), NULL);
        }
        String_Delete(symbol);
    }
    ScheduleWindowCopyToVram(locals.window);
}

// The move list page has two sets of 6 windows (`windowSet`); window 5 is shared.
static void BattlePartyMenu_PrintCheckMovesPage(BattlePartyMenu *menu) {
    u16 base = menu->windowSet * 6, i;

    FillWindowPixelBuffer(&menu->pageWindows[base], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 4], 0);
    FillWindowPixelBuffer(&menu->pageWindows[5], 0);
    BattlePartyMenu_PrintMonName(menu, base, 0, menu->context->selectedMon, 0, 0);
    for (i = 0; i < 4; i++) {
        BattlePartyMenuMove *move = &menu->mons[menu->context->selectedMon].moves[i];
        if (move->move) {
            BattlePartyMenu_PrintMoveName(menu, move->move, base + 1 + i, gBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, move, base + 1 + i);
        }
    }
    BattlePartyMenu_PrintButtonLabel(menu, 5, msg_0006_00018);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 1]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 2]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 3]);
    ScheduleWindowCopyToVram(&menu->pageWindows[base + 4]);
    menu->windowSet ^= 1;
}

static void BattlePartyMenu_PrintSummaryPage(BattlePartyMenu *menu) {
    u32 base = (u32)menu->windowSet * 22;

    FillWindowPixelBuffer(&menu->pageWindows[19], 0);
    FillWindowPixelBuffer(&menu->pageWindows[20], 0);
    FillWindowPixelBuffer(&menu->pageWindows[14], 0);
    FillWindowPixelBuffer(&menu->pageWindows[15], 0);
    FillWindowPixelBuffer(&menu->pageWindows[16], 0);
    FillWindowPixelBuffer(&menu->pageWindows[17], 0);
    FillWindowPixelBuffer(&menu->pageWindows[18], 0);
    FillWindowPixelBuffer(&menu->pageWindows[13], 0);
    FillWindowPixelBuffer(&menu->pageWindows[21], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 10], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 11], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 12], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 5], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 6], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 7], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 8], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 9], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 4], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 3], 0);
    BattlePartyMenu_PrintMonName(menu, base, 0, menu->context->selectedMon, 0, 0);
    BattlePartyMenu_PrintHPStat(menu, menu->context->selectedMon);
    BattlePartyMenu_DrawHPBar(menu, base + 10, menu->context->selectedMon, 0, 0);
    BattlePartyMenu_PrintLevelAndNextExp(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintAttack(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintDefense(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintSpeed(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintSpAttack(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintSpDefense(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintAbilityName(menu, base + 1, menu->context->selectedMon);
    BattlePartyMenu_PrintHeldItemName(menu, base + 3, menu->context->selectedMon);
    BattlePartyMenu_PrintAbilityDescription(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintButtonLabel(menu, 21, msg_0006_00019);
    menu->windowSet ^= 1;
}

// The move detail page has two sets of 11 windows (`windowSet`); windows 6-10 are shared.
static void BattlePartyMenu_PrintMoveDetailPage(BattlePartyMenu *menu) {
    int base = menu->windowSet * 11;
    BattlePartyMenuMove *move;
    int slot;

    FillWindowPixelBuffer(&menu->pageWindows[6], 0);
    FillWindowPixelBuffer(&menu->pageWindows[7], 0);
    FillWindowPixelBuffer(&menu->pageWindows[8], 0);
    FillWindowPixelBuffer(&menu->pageWindows[9], 0);
    FillWindowPixelBuffer(&menu->pageWindows[10], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 5], 0);
    FillWindowPixelBuffer(&menu->pageWindows[base + 4], 0);
    slot = menu->context->selectedMon;
    move = &menu->mons[slot].moves[menu->context->selectedMove];
    BattlePartyMenu_PrintMonName(menu, 6, 0, slot, 0, 0);
    BattlePartyMenu_PrintPPLabel(menu, 7, 0, 0);
    BattlePartyMenu_PrintMoveName(menu, move->move, base, gBattlePartyMoveNameMsgIds[menu->context->selectedMove], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
    BattlePartyMenu_PrintAccuracyLabel(menu, 8);
    BattlePartyMenu_PrintMoveAccuracy(menu, base + 2, move->accuracy);
    BattlePartyMenu_PrintPowerLabel(menu, 9);
    BattlePartyMenu_PrintMovePower(menu, base + 3, move->power);
    BattlePartyMenu_PrintMoveDescription(menu, base + 4, move->move);
    BattlePartyMenu_PrintCategoryLabel(menu, 10);
    BattlePartyMenu_PrintMoveCategory(menu, base + 5, move->moveClass);
    BattlePartyMenu_PrintMovePP(menu, base + 1, move->pp, move->maxPP);
    menu->windowSet ^= 1;
}

static void BattlePartyMenu_PrintLearnMovePage(BattlePartyMenu *menu) {
    u32 i;
    BattlePartyMenuMove newMove;

    FillWindowPixelBuffer(&menu->pageWindows[0], 0);
    FillWindowPixelBuffer(&menu->pageWindows[1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[4], 0);
    FillWindowPixelBuffer(&menu->pageWindows[5], 0);
    BattlePartyMenu_PrintMonName(menu, 0, 0, menu->context->selectedMon, 0, 0);
    for (i = 0; i < 4; i++) {
        BattlePartyMenuMove *moves = menu->mons[menu->context->selectedMon].moves;
        if (moves[i].move) {
            BattlePartyMenu_PrintMoveName(menu, moves[i].move, i + 1, gBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, &moves[i], i + 1);
        }
    }
    BattlePartyMenu_PrintMoveName(menu, menu->context->move, 5, msg_0006_00073, 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
    newMove.pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
    newMove.maxPP = newMove.pp;
    BattlePartyMenu_PrintMoveListPP(menu, &newMove, 5);
    ScheduleWindowCopyToVram(&menu->pageWindows[1]);
    ScheduleWindowCopyToVram(&menu->pageWindows[2]);
    ScheduleWindowCopyToVram(&menu->pageWindows[3]);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
    ScheduleWindowCopyToVram(&menu->pageWindows[5]);
}

static void BattlePartyMenu_PrintForgetMovePage(BattlePartyMenu *menu) {
    BattlePartyMenuMove *moves, *move;
    u32 moveSlot;
    int pp;

    FillWindowPixelBuffer(&menu->pageWindows[0], 0);
    FillWindowPixelBuffer(&menu->pageWindows[2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[4], 0);
    FillWindowPixelBuffer(&menu->pageWindows[6], 0);
    FillWindowPixelBuffer(&menu->pageWindows[5], 0);
    FillWindowPixelBuffer(&menu->pageWindows[7], 0);
    FillWindowPixelBuffer(&menu->pageWindows[9], 0);
    FillWindowPixelBuffer(&menu->pageWindows[10], 0);
    FillWindowPixelBuffer(&menu->pageWindows[8], 0);
    FillWindowPixelBuffer(&menu->pageWindows[11], 0);
    BattlePartyMenu_PrintMonName(menu, 0, 0, menu->context->selectedMon, 0, 0);
    BattlePartyMenu_PrintPPLabel(menu, 2, 0, 0);
    BattlePartyMenu_PrintAccuracyLabel(menu, 4);
    BattlePartyMenu_PrintPowerLabel(menu, 5);
    BattlePartyMenu_PrintCategoryLabel(menu, 9);
    moveSlot = menu->context->selectedMove;
    if (moveSlot < 4) {
        moves = menu->mons[menu->context->selectedMon].moves;
        move = &moves[moveSlot];
        BattlePartyMenu_PrintMoveName(menu, move->move, 1, gBattlePartyMoveNameMsgIds[moveSlot], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMoveAccuracy(menu, 6, move->accuracy);
        BattlePartyMenu_PrintMovePower(menu, 7, move->power);
        BattlePartyMenu_PrintMoveDescription(menu, 8, move->move);
        BattlePartyMenu_PrintMoveCategory(menu, 10, move->moveClass);
        BattlePartyMenu_PrintMovePP(menu, 3, move->pp, move->maxPP);
    } else {
        pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
        BattlePartyMenu_PrintMoveName(menu, menu->context->move, 1, msg_0006_00073, 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMoveDescription(menu, 8, menu->context->move);
        BattlePartyMenu_PrintMoveAccuracy(menu, 6, GetMoveAttr(menu->context->move, MOVEATTR_ACCURACY));
        BattlePartyMenu_PrintMovePower(menu, 7, GetMoveAttr(menu->context->move, MOVEATTR_POWER));
        BattlePartyMenu_PrintMoveCategory(menu, 10, GetMoveAttr(menu->context->move, MOVEATTR_CLASS));
        BattlePartyMenu_PrintMovePP(menu, 3, pp, pp);
    }
    BattlePartyMenu_PrintForgetButtonLabel(menu, 11);
}

static void BattlePartyMenu_PrintRestorePPPage(BattlePartyMenu *menu) {
    u32 i;
    BattlePartyMenuMove *moves;

    FillWindowPixelBuffer(&menu->pageWindows[0], 0);
    FillWindowPixelBuffer(&menu->pageWindows[1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[4], 0);
    BattlePartyMenu_PrintMonName(menu, 0, 0, menu->context->selectedMon, 0, 0);
    for (i = 0; i < 4; i++) {
        moves = menu->mons[menu->context->selectedMon].moves;
        if (moves[i].move) {
            BattlePartyMenu_PrintMoveName(menu, moves[i].move, i + 1, gBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, &moves[i], i + 1);
        }
    }
    if (GetItemAttr(menu->context->item, ITEMATTR_PP_RESTORE_ALL, menu->context->heapID) == 0) {
        BattlePartyMenu_PrintPrompt(menu, msg_0006_00094);
    }
    ScheduleWindowCopyToVram(&menu->pageWindows[1]);
    ScheduleWindowCopyToVram(&menu->pageWindows[2]);
    ScheduleWindowCopyToVram(&menu->pageWindows[3]);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
}

static void BattlePartyMenu_UpdateMovePP(BattlePartyMenu *menu, int window, int moveSlot) {
    u8 slot = menu->context->selectedMon;
    BattlePartyMenu_UpdateMoveListPP(menu, &menu->mons[slot].moves[moveSlot], window);
}

static void BattlePartyMenu_PrintContestForgetMovePage(BattlePartyMenu *menu) {
    BattlePartyMenuMove *moves;
    BattlePartyMenuMove *move;
    String *string;
    int pp;

    FillWindowPixelBuffer(&menu->pageWindows[0], 0);
    FillWindowPixelBuffer(&menu->pageWindows[2], 0);
    FillWindowPixelBuffer(&menu->pageWindows[3], 0);
    FillWindowPixelBuffer(&menu->pageWindows[1], 0);
    FillWindowPixelBuffer(&menu->pageWindows[4], 0);
    FillWindowPixelBuffer(&menu->pageWindows[5], 0);
    FillWindowPixelBuffer(&menu->pageWindows[6], 0);
    BattlePartyMenu_PrintMonName(menu, 0, 0, menu->context->selectedMon, 0, 0);
    BattlePartyMenu_PrintPPLabel(menu, 2, 0, 0);
    string = NewString_ReadMsgData(menu->msgData, msg_0006_00057);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[4], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
    if (menu->context->selectedMove < 4) {
        u32 moveSlot = menu->context->selectedMove;
        moves = menu->mons[menu->context->selectedMon].moves;
        move = &moves[moveSlot];
        BattlePartyMenu_PrintMoveName(menu, move->move, 1, gBattlePartyMoveNameMsgIds[moveSlot], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMovePP(menu, 3, move->pp, move->maxPP);
    } else {
        pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
        BattlePartyMenu_PrintMoveName(menu, menu->context->move, 1, msg_0006_00073, 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMovePP(menu, 3, pp, pp);
    }
    BattlePartyMenu_PrintForgetButtonLabel(menu, 6);
}

static void BattlePartyMenu_ShowMessage(BattlePartyMenu *menu) {
    DrawFrameAndWindow2(&menu->windows[1], TRUE, 1, 14);
    FillWindowPixelBuffer(&menu->windows[1], 15);
    BattlePartyMenu_PrintMessage(menu);
}

static void BattlePartyMenu_PrintMessage(BattlePartyMenu *menu) {
    int delay;

    TextFlags_SetCanABSpeedUpPrint(TRUE);
    delay = BattleSystem_GetTextFrameDelay(menu->context->battleSystem);
    menu->textPrinterId = AddTextPrinterParameterized(&menu->windows[1], 1, menu->string, 0, 0, delay, NULL);
}

static void BattlePartyMenu_BufferItemResultMessage(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    Pokemon *mon;
    ItemData *itemData;
    u16 hp;
    u8 healed;
    u16 prevHp;

    itemData = LoadItemDataOrGfx(context->item, ITEMNARC_PARAM, context->heapID);
    mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
    hp = GetMonData(mon, MON_DATA_HP, NULL);
    healed = 0;
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_SLP_HEAL)) {
        healed |= 1;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PSN_HEAL)) {
        healed |= 2;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_BRN_HEAL)) {
        healed |= 4;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_FRZ_HEAL)) {
        healed |= 8;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PRZ_HEAL)) {
        healed |= 0x10;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_CFS_HEAL)) {
        healed |= 0x20;
    }
    if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_INF_HEAL)) {
        healed |= 0x40;
    }
    prevHp = menu->mons[context->selectedMon].hp;
    if (prevHp == 0 && hp != 0) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00088);
    } else if (hp != prevHp) {
        String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00082);
        BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
        BufferIntegerAsString(menu->msgFormat, 1, hp - menu->mons[context->selectedMon].hp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(menu->msgFormat, menu->string, template);
        String_Delete(template);
    } else if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PP_RESTORE) || GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PP_RESTORE_ALL)) {
        ReadMsgDataIntoString(menu->msgData, msg_0006_00087, menu->string);
    } else if (healed == 1) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00092);
    } else if (healed == 2) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00083);
    } else if (healed == 4) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00085);
    } else if (healed == 8) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00086);
    } else if (healed == 0x10) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00084);
    } else if (healed == 0x20) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00090);
    } else if (healed == 0x40) {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00091);
    } else {
        BattlePartyMenu_BufferMonMessage(menu, mon, msg_0006_00089);
    }
    Heap_Free(itemData);
}

static void BattlePartyMenu_BufferEmbargoMessage(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    u8 *partyOrder = &context->partyOrder[context->selectedMon];
    Pokemon *mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, *partyOrder);
    String *template = NewString_ReadMsgData(menu->msgData, msg_0006_00095);

    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
    BufferMoveName(menu->msgFormat, 1, MOVE_EMBARGO);
    StringExpandPlaceholders(menu->msgFormat, menu->string, template);
    String_Delete(template);
}

static void BattlePartyMenu_InitSprites(BattlePartyMenu *menu) {
    BattlePartyMenu_CreateSpriteManager(menu);
    BattlePartyMenu_LoadMonIconResources(menu);
    BattlePartyMenu_LoadStatusIconResources(menu);
    BattlePartyMenu_LoadTypeIconResources(menu);
    BattlePartyMenu_LoadHeldItemIconResources(menu);
    BattlePartyMenu_LoadSpriteResB00B(menu);
    BattlePartyMenu_CreateSprites(menu);
    BattlePartyMenu_CreateCursor(menu);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void BattlePartyMenu_CreateSpriteManager(BattlePartyMenu *menu) {
    SpriteResourceCountsListUnion capacities = gBattlePartySpriteCapacities;
    SpriteSystem *spriteSystem;

    spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    menu->spriteManager = SpriteManager_New(spriteSystem);
    SpriteSystem_InitSprites(spriteSystem, menu->spriteManager, 43);
    SpriteSystem_InitManagerWithCapacities(spriteSystem, menu->spriteManager, &capacities);
}

static void BattlePartyMenu_LoadMonIconResources(BattlePartyMenu *menu) {
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, menu->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    u32 i;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetMonIconCell32KNaix(), FALSE, 0xB007);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetMonIconAnim32KNaix(), FALSE, 0xB007);
    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, Pokemon_GetIconNaix(menu->mons[i].mon), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007 + i);
        } else {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetMonIconNaixEx(SPECIES_NONE, FALSE, 0), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007 + i);
        }
    }
    NARC_Delete(narc);
}

static void BattlePartyMenu_LoadStatusIconResources(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NARC *narc = NARC_New(NARC_a_0_3_9, menu->context->heapID);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, StatusIcon_GetPlttNaix(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB008);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, StatusIcon_GetCellNaix(), FALSE, 0xB008);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, StatusIcon_GetAnimNaix(), FALSE, 0xB008);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, StatusIcon_GetCharNaix(), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00D);
    NARC_Delete(narc);
}

static void BattlePartyMenu_LoadTypeIconResources(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    u32 resId;
    u32 last;

    TypeIcon_LoadPaletteBuffer(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0xB009);
    TypeIcon_LoadCellAnim(spriteSystem, menu->spriteManager, 0xB009, 0xB009);
    // char resources 0xB00E-0xB014: the two mon types, four moves and the new move
    last = 0xB00E;
    resId = last;
    last += 6;
    do {
        TypeIcon_LoadChar(spriteSystem, menu->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0, resId);
        resId++;
    } while (resId <= last);
    MoveCategoryIcon_LoadChar(spriteSystem, menu->spriteManager, NNS_G2D_VRAM_TYPE_2DSUB, 0, 0xB015);
}

static void BattlePartyMenu_LoadHeldItemIconResources(BattlePartyMenu *menu) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, menu->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, PartyMenu_GetHeldItemIconNCLR(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00A);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, PartyMenu_GetHeldItemIconNCER(), FALSE, 0xB00A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, PartyMenu_GetHeldItemIconNANR(), FALSE, 0xB00A);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, PartyMenu_GetHeldItemIconNCGR(), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB016);
    NARC_Delete(narc);
}

// resources 0xB00B / 0xB017 from NARC a/0/7/1
static void BattlePartyMenu_LoadSpriteResB00B(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    SpriteSystem_LoadPaletteBuffer(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, NARC_a_0_7_1, 27, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00B);
    SpriteSystem_LoadCellResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 25, FALSE, 0xB00B);
    SpriteSystem_LoadAnimResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 24, FALSE, 0xB00B);
    SpriteSystem_LoadCharResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 26, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB017);
}

static ManagedSprite *BattlePartyMenu_CreateSprite(BattlePartyMenu *menu, int index) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = gBattlePartySpriteTemplates[index][4];
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = gBattlePartySpriteTemplates[index][0];
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = gBattlePartySpriteTemplates[index][1];
    template.resIdList[GF_GFX_RES_TYPE_CELL] = gBattlePartySpriteTemplates[index][2];
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = gBattlePartySpriteTemplates[index][3];
    template.bgPriority = 1;
    template.vramTransfer = FALSE;
    return SpriteSystem_NewSprite(spriteSystem, menu->spriteManager, &template);
}

static void BattlePartyMenu_CreateSprites(BattlePartyMenu *menu) {
    u32 i;

    for (i = 0; i < 38; i++) {
        menu->sprites[i] = BattlePartyMenu_CreateSprite(menu, i);
    }
    BattlePartyMenu_SetMonIconPalettes(menu);
}

static void BattlePartyMenu_FreeSprites(BattlePartyMenu *menu) {
    u32 i;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    for (i = 0; i < 38; i++) {
        Sprite_DeleteAndFreeResources(menu->sprites[i]);
    }
    BattlePartyMenu_FreeCursor(menu);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, menu->spriteManager);
}

static void BattlePartyMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, x, y);
}

static void BattlePartyMenu_SetMonIconPalettes(BattlePartyMenu *menu) {
    int i;

    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            ManagedSprite_SetPaletteOverride(menu->sprites[7 + i], GetMonIconPaletteEx(menu->mons[i].species, menu->mons[i].form, menu->mons[i].isEgg));
        }
    }
}

static void BattlePartyMenu_SetTypeIcon(BattlePartyMenu *menu, ManagedSprite *sprite, int resId, u32 type) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetCharFile(type);

    SpriteSystem_ReplaceCharResObj(spriteSystem, menu->spriteManager, narc, file, TRUE, resId);
    ManagedSprite_SetPaletteOverride(sprite, TypeIcon_GetPalette(type) + 4);
}

static void BattlePartyMenu_SetMoveCategoryIcon(BattlePartyMenu *menu, ManagedSprite *sprite, u32 moveClass) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NarcId narc = MoveCategoryIcon_GetNarc();
    int file = MoveCategoryIcon_GetCharFile(moveClass);

    SpriteSystem_ReplaceCharResObj(spriteSystem, menu->spriteManager, narc, file, TRUE, 0xB015);
    ManagedSprite_SetPaletteOverride(sprite, MoveCategoryIcon_GetPalette(moveClass) + 4);
}

static void BattlePartyMenu_ShowStatusIcon(int status, ManagedSprite *sprite, int x, int y) {
    if (status != PARTY_MON_STATUS_ICON_OK) {
        ManagedSprite_SetAnim(sprite, status);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

static void BattlePartyMenu_ShowMonTypeIcons(BattlePartyMenu *menu, BattlePartyMenuMon *mon, const BattlePartyMenuPos *pos) {
    BattlePartyMenu_SetTypeIcon(menu, menu->sprites[19], 0xB00E, mon->type1);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[19], pos[0].x, pos[0].y);
    if (mon->type1 != mon->type2) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[20], 0xB00F, mon->type2);
        BattlePartyMenu_ShowSpriteAt(menu->sprites[20], pos[1].x, pos[1].y);
    }
}

static void BattlePartyMenu_ShowHeldItemIcon(u16 item, ManagedSprite *sprite, int x, int y) {
    if (item != ITEM_NONE) {
        if (ItemIdIsMail(item) == TRUE) {
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            ManagedSprite_SetAnim(sprite, 0);
        }
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

static void BattlePartyMenu_ShowCapsuleIcon(BOOL hasCapsule, ManagedSprite *sprite, int x, int y) {
    if (hasCapsule != FALSE) {
        ManagedSprite_SetAnim(sprite, 2);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

static void BattlePartyMenu_ShowPageSprites(BattlePartyMenu *menu, u32 page) {
    u32 i;

    for (i = 0; i < 38; i++) {
        ManagedSprite_SetDrawFlag(menu->sprites[i], FALSE);
    }
    switch (page) {
    case BATTLE_PARTY_PAGE_PARTY_LIST:
        BattlePartyMenu_ShowPartyListSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_MON_MENU:
        BattlePartyMenu_ShowMonMenuSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_SUMMARY:
        BattlePartyMenu_ShowSummarySprites(menu);
        break;
    case BATTLE_PARTY_PAGE_CHECK_MOVES:
        BattlePartyMenu_ShowCheckMovesSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_MOVE_DETAIL:
        BattlePartyMenu_ShowMoveDetailSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_RESTORE_PP:
        BattlePartyMenu_ShowRestorePPSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE:
        BattlePartyMenu_ShowLearnMoveSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE:
        BattlePartyMenu_ShowForgetMoveSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST:
        BattlePartyMenu_ShowContestLearnMoveSprites(menu);
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST:
        BattlePartyMenu_ShowContestForgetMoveSprites(menu);
        break;
    }
}

static void BattlePartyMenu_ShowPartyListSprites(BattlePartyMenu *menu) {
    int i;

    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + i], gBattlePartyListIconPos[i].x, gBattlePartyListIconPos[i].y);
            BattlePartyMenu_ShowStatusIcon(menu->mons[i].status, menu->sprites[13 + i], gBattlePartyListStatusIconPos[i].x, gBattlePartyListStatusIconPos[i].y);
            BattlePartyMenu_ShowHeldItemIcon(menu->mons[i].heldItem, menu->sprites[i], gBattlePartyListIconPos[i].x + 8, gBattlePartyListIconPos[i].y + 8);
            BattlePartyMenu_ShowCapsuleIcon(menu->mons[i].ballCapsule, menu->sprites[32 + i], gBattlePartyListIconPos[i].x + 16, gBattlePartyListIconPos[i].y + 8);
        }
    }
}

static void BattlePartyMenu_ShowMonMenuSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 128, 72);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 136, 80);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 144, 80);
}

static void BattlePartyMenu_ShowSummarySprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartySummaryTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[6], 20, 132);
}

static void BattlePartyMenu_ShowCheckMovesSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];
    u32 i;

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartyCheckMovesTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, mon->moves[i].type);
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], gBattlePartyCheckMovesMoveIconPos[i].x, gBattlePartyCheckMovesMoveIconPos[i].y);
        }
    }
}

static void BattlePartyMenu_ShowMoveDetailSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartyMoveDetailTypeIconPos);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + menu->context->selectedMove], 136, 48);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_SetMoveCategoryIcon(menu, menu->sprites[26], mon->moves[menu->context->selectedMove].moveClass);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[26], 24, 88);
}

static void BattlePartyMenu_ShowLearnMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
}

static void BattlePartyMenu_ShowForgetMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartyForgetMoveTypeIconPos);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + menu->context->selectedMove], 136, 48);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    if (menu->context->selectedMove < 4) {
        BattlePartyMenu_SetMoveCategoryIcon(menu, menu->sprites[26], mon->moves[menu->context->selectedMove].moveClass);
    } else {
        BattlePartyMenu_SetMoveCategoryIcon(menu, menu->sprites[26], GetMoveAttr(menu->context->move, MOVEATTR_CLASS));
    }
    BattlePartyMenu_ShowSpriteAt(menu->sprites[26], 24, 88);
}

static void BattlePartyMenu_ShowRestorePPSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, gBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
}

static void BattlePartyMenu_ShowContestForgetMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + menu->context->selectedMove], 136, 72);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowConditionMarkers(menu);
}

static void BattlePartyMenu_ShowContestLearnMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
    BattlePartyMenu_ShowConditionMarkers(menu);
}

static void BattlePartyMenu_ShowMoveTypeIcons(BattlePartyMenu *menu) {
    u16 i;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, mon->moves[i].type);
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], gBattlePartyLearnMoveMoveIconPos[i].x, gBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (menu->context->move != MOVE_NONE) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[25], 0xB014, (u8)GetMoveAttr(menu->context->move, MOVEATTR_TYPE));
        BattlePartyMenu_ShowSpriteAt(menu->sprites[25], 88, 176);
    }
}

// Contest type icons follow the 18 battle types in the type icon resources.
static void BattlePartyMenu_ShowMoveContestTypeIcons(BattlePartyMenu *menu) {
    u16 i;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, (u8)(GetMoveAttr(mon->moves[i].move, MOVEATTR_CONTEST_TYPE) + 18));
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], gBattlePartyLearnMoveMoveIconPos[i].x, gBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (menu->context->move != MOVE_NONE) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[25], 0xB014, (u8)(GetMoveAttr(menu->context->move, MOVEATTR_CONTEST_TYPE) + 18));
        BattlePartyMenu_ShowSpriteAt(menu->sprites[25], 88, 176);
    }
}

static void BattlePartyMenu_ShowMoveIcons(BattlePartyMenu *menu) {
    if (menu->viewMode == BATTLE_PARTY_VIEW_BATTLE) {
        BattlePartyMenu_ShowMoveTypeIcons(menu);
    } else {
        BattlePartyMenu_ShowMoveContestTypeIcons(menu);
    }
}

static void BattlePartyMenu_SetSpriteAnim(ManagedSprite *sprite, u32 anim) {
    if (anim != ManagedSprite_GetActiveAnim(sprite)) {
        ManagedSprite_SetAnimationFrame(sprite, 0);
        ManagedSprite_SetAnim(sprite, anim);
    }
}

static int BattlePartyMenu_GetMonIconAnim(BattlePartyMenuMon *mon) {
    if (mon->hp == 0) {
        return 0;
    }
    if (mon->status != PARTY_MON_STATUS_ICON_OK && mon->status != PARTY_MON_STATUS_ICON_FNT) {
        return 5;
    }
    switch (CalculateHpBarColor(mon->hp, mon->maxHp, 48)) {
    case 4:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    case 1:
        return 4;
    default:
        return 0;
    }
}

static void BattlePartyMenu_UpdateMonIcons(BattlePartyMenu *menu) {
    u16 i;

    for (i = 0; i < Party_GetCount(menu->context->party); i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            int anim = BattlePartyMenu_GetMonIconAnim(&menu->mons[i]);
            BattlePartyMenu_SetSpriteAnim(menu->sprites[7 + i], (u8)anim);
            ManagedSprite_TickNFrames(menu->sprites[7 + i], FX32_ONE);
        }
    }
}

static s16 BattlePartyMenu_CalcConditionMarkerCoord(u8 stat, int target, int base) {
    u32 n;

    if (base > target) {
        n = ((u32)((base - target) * (stat + 44)) << 16) / 300;
        return base - (s16)(n >> 16);
    }
    n = ((u32)((target - base) * (stat + 44)) << 16) / 300;
    return base + (s16)(n >> 16);
}

static void BattlePartyMenu_ShowConditionMarkers(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];
    int x, y;

    BattlePartyMenu_SetSpriteAnim(menu->sprites[27], 0);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[28], 1);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[29], 3);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[30], 4);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[31], 2);
    x = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[0], 144, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[0], 2, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[27], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[1], 164, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[1], 16, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[28], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[2], 156, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[2], 41, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[29], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[3], 131, 143);
    y = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[3], 41, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[30], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[4], 123, 143);
    y = BattlePartyMenu_CalcConditionMarkerCoord(mon->contestStats[4], 16, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[31], x, y);
}

static void BattlePartyMenu_CreateCursor(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    BattleCursor *cursor;

    BattleCursor_LoadResources(spriteSystem, menu->spriteManager, menu->paletteData, menu->context->heapID, 0xB018, 0xB00C, 0xB00C, 0xB00C);
    cursor = BattleCursor_New(spriteSystem, menu->spriteManager, menu->context->heapID, 0xB018, 0xB00C, 0xB00C, 0xB00C, 0, 1);
    BattleDpadCursor_SetBattleCursor(menu->cursor, cursor);
}

static void BattlePartyMenu_FreeCursor(BattlePartyMenu *menu) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(menu->cursor));
    BattleCursor_FreeResources(menu->spriteManager, 0xB018, 0xB00C, 0xB00C, 0xB00C);
}

// Without the view toggle button (box 6 resp. 2), the d-pad cursor skips it.
static void BattlePartyMenu_SetLearnMoveCursorMask(BattlePartyMenu *menu) {
    if (menu->canToggleView == 0) {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 0x5F);
    } else {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 0x7F);
    }
}

static void BattlePartyMenu_SetForgetMoveCursorMask(BattlePartyMenu *menu) {
    if (menu->canToggleView == 0) {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 5);
    } else {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 7);
    }
}

static void BattlePartyMenu_SetPageCursor(BattlePartyMenu *menu, u32 page) {
    BattleDpadCursor_SetBoxes(menu->cursor, gBattlePartyPageCursorBoxes[page]);
    switch (page) {
    case BATTLE_PARTY_PAGE_PARTY_LIST:
        BattleDpadCursor_SetPosition(menu->cursor, menu->context->selectedMon);
        menu->monMenuPosition = 0;
        menu->context->selectedMove = 0;
        break;
    case BATTLE_PARTY_PAGE_MON_MENU:
        BattleDpadCursor_SetPosition(menu->cursor, menu->monMenuPosition);
        menu->context->selectedMove = 0;
        break;
    case BATTLE_PARTY_PAGE_CHECK_MOVES:
    case BATTLE_PARTY_PAGE_MOVE_DETAIL:
        BattleDpadCursor_SetPosition(menu->cursor, menu->context->selectedMove);
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE:
    case BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST:
        BattlePartyMenu_SetLearnMoveCursorMask(menu);
        BattleDpadCursor_SetPosition(menu->cursor, menu->learnMovePosition);
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE:
    case BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST:
        BattlePartyMenu_SetForgetMoveCursorMask(menu);
        BattleDpadCursor_SetPosition(menu->cursor, menu->forgetMovePosition);
        break;
    }
}

static void BattlePartyMenu_DeactivateCursor(BattlePartyMenu *menu) {
    BattleDpadCursor_SetActive(menu->cursor, 0);
    BattleDpadCursor_ResetPosition(menu->cursor);
    BattleCursor_Disable(BattleDpadCursor_GetBattleCursor(menu->cursor));
}

static void BattlePartyMenu_StoreButtonTilemaps(BattlePartyMenu *menu, const u16 *screen) {
    BattlePartyMenu_CopyTilemapRect(menu->monButtonTiles[0], screen, 0, 0, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->monButtonTiles[1], screen, 0, 6, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->monButtonTiles[2], screen, 0, 12, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->monButtonTiles[3], screen, 0, 18, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->partnerMonButtonTiles[0], screen, 16, 0, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->partnerMonButtonTiles[1], screen, 16, 6, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->partnerMonButtonTiles[2], screen, 16, 12, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->partnerMonButtonTiles[3], screen, 16, 18, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->menuButtonTiles[0], screen, 0, 39, 13, 5);
    BattlePartyMenu_CopyTilemapRect(menu->menuButtonTiles[1], screen, 0, 44, 13, 5);
    BattlePartyMenu_CopyTilemapRect(menu->menuButtonTiles[2], screen, 13, 39, 13, 5);
    BattlePartyMenu_CopyTilemapRect(menu->menuButtonTiles[3], screen, 13, 44, 13, 5);
    BattlePartyMenu_CopyTilemapRect(menu->prevMonButtonTiles[0], screen, 0, 49, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->prevMonButtonTiles[1], screen, 5, 49, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->prevMonButtonTiles[2], screen, 10, 49, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->prevMonButtonTiles[3], screen, 15, 49, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->nextMonButtonTiles[0], screen, 0, 54, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->nextMonButtonTiles[1], screen, 5, 54, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->nextMonButtonTiles[2], screen, 10, 54, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->nextMonButtonTiles[3], screen, 15, 54, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->cancelButtonTiles[0], screen, 26, 24, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->cancelButtonTiles[1], screen, 26, 29, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->cancelButtonTiles[2], screen, 26, 34, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->cancelButtonTiles[3], screen, 26, 39, 5, 5);
    BattlePartyMenu_CopyTilemapRect(menu->forgetButtonTiles[0], screen, 0, 24, 26, 5);
    BattlePartyMenu_CopyTilemapRect(menu->forgetButtonTiles[1], screen, 0, 29, 26, 5);
    BattlePartyMenu_CopyTilemapRect(menu->forgetButtonTiles[2], screen, 0, 34, 26, 5);
    BattlePartyMenu_CopyTilemapRect(menu->viewToggleButtonTiles[0], screen, 20, 49, 9, 4);
    BattlePartyMenu_CopyTilemapRect(menu->viewToggleButtonTiles[1], screen, 20, 53, 9, 4);
    BattlePartyMenu_CopyTilemapRect(menu->viewToggleButtonTiles[2], screen, 20, 57, 9, 4);
    BattlePartyMenu_CopyTilemapRect(menu->moveTabButtonTiles[0], screen, 0, 59, 5, 2);
    BattlePartyMenu_CopyTilemapRect(menu->moveTabButtonTiles[1], screen, 5, 59, 5, 2);
    BattlePartyMenu_CopyTilemapRect(menu->moveTabButtonTiles[2], screen, 10, 59, 5, 2);
}

static void BattlePartyMenu_StoreLargeButtonTilemaps(BattlePartyMenu *menu, const u16 *screen) {
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[0], screen, 0, 0, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[1], screen, 0, 17, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[2], screen, 0, 34, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[0], screen, 0, 51, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[1], screen, 16, 51, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[2], screen, 0, 57, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[3], screen, 16, 57, 16, 6);
}

static void BattlePartyMenu_CopyTilemapRect(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height) {
    u16 row;
    u16 col;

    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            dst[row * width + col] = src[(y + row) * 32 + x + col];
        }
    }
}

static u16 *BattlePartyMenu_GetButtonTilemap(BattlePartyMenu *menu, int button, int frame, int partner) {
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (partner == 0) {
            return menu->monButtonTiles[frame];
        }
        return menu->partnerMonButtonTiles[frame];
    case 6:
        return menu->cancelButtonTiles[frame];
    case 7:
        return menu->shiftButtonTiles[frame];
    case 8:
    case 9:
    case 10:
    case 11:
        return menu->menuButtonTiles[frame];
    case 12:
        return menu->prevMonButtonTiles[frame];
    case 13:
        return menu->nextMonButtonTiles[frame];
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        return menu->moveButtonTiles[frame];
    case 28:
    case 29:
        return menu->forgetButtonTiles[frame];
    case 18:
        return menu->viewToggleButtonTiles[frame];
    case 30:
    case 31:
    case 32:
    case 33:
        return menu->moveTabButtonTiles[frame];
    default:
        return NULL;
    }
}

// Frame 3 is the disabled look. Mon buttons are greyed for an Egg's HP bar, shown red for a fainted mon and
// blue for a partner's mon; button 27 (the move to learn) uses palette 10.
static void BattlePartyMenu_BuildButtonTilemap(BattlePartyMenu *menu, u16 *dst, int button, int frame, u8 palette) {
    const u16 *src = BattlePartyMenu_GetButtonTilemap(menu, button, frame, palette);
    u8 width = gBattlePartyButtonRects[button][2];
    u8 height = gBattlePartyButtonRects[button][3];
    int size = width * height;
    u16 tiles[2];
    u8 i, j;

    memcpy(dst, src, size * 2);
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (menu->mons[button].species == SPECIES_NONE) {
            break;
        }
        if (menu->mons[button].isEgg) {
            tiles[0] = dst[width * 2 + 5];
            tiles[1] = dst[width + width * 2 + 5];
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 9; j++) {
                    dst[(i + 2) * width + 6 + j] = tiles[i];
                }
            }
        } else if (menu->mons[button].hp == 0) {
            for (i = 0; i < size; i++) {
                dst[i] = (dst[i] & 0xFFF) | 0x2000;
            }
        } else if (BattlePartyMenu_IsPartnerMon(menu, (u8)button) == 1) {
            for (i = 0; i < size; i++) {
                dst[i] = (dst[i] & 0xFFF) | 0x1000;
            }
        }
        break;
    case 27:
        for (i = 0; i < size; i++) {
            dst[i] = (dst[i] & 0xFFF) | 0xA000;
        }
        break;
    }
}

static void BattlePartyMenu_DrawButton(BattlePartyMenu *menu, u8 button, u8 frame, u8 palette) {
    int height = gBattlePartyButtonRects[button][3];
    int width = gBattlePartyButtonRects[button][2];
    u16 *buffer = Heap_Alloc(menu->context->heapID, width * height * 2);

    BattlePartyMenu_BuildButtonTilemap(menu, buffer, button, frame, palette);
    LoadRectToBgTilemapRect(menu->bgConfig, GF_BG_LYR_SUB_2, buffer, gBattlePartyButtonRects[button][0], gBattlePartyButtonRects[button][1], width, height);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(buffer);
}

typedef void (*ScrollWindowFunc)(Window *window, int direction, int pixels, int fillValue);
typedef void (*OffsetSpriteFunc)(ManagedSprite *sprite, int dx, int dy);

static void BattlePartyMenu_ScrollButtonWindows(BattlePartyMenu *menu, u32 button, int frame) {
    const u8 *windowList = gBattlePartyButtonWindowLists[button];
    u16 i;
    int pixels;
    int direction;

    if (windowList != NULL) {
        if (button <= 5) {
            pixels = gBattlePartySlotButtonScrollAmounts[frame];
        } else {
            pixels = gBattlePartyButtonScrollAmounts[frame];
        }
        switch (frame) {
        case 0:
            direction = 1;
            break;
        case 1:
            direction = 0;
            break;
        case 2:
            direction = 1;
            break;
        }
        // the move list page's move buttons sit in the current window set
        if (button >= 14 && button <= 17) {
            ((ScrollWindowFunc)ScrollWindow)((Window *)((u32)menu->pageWindows + (windowList[menu->windowSet] << 4)), direction, pixels, 0);
            ScheduleWindowCopyToVram((Window *)((u32)menu->pageWindows + (windowList[menu->windowSet] << 4)));
            return;
        }
        for (i = 0; i < 8; i++) {
            if (windowList[i] == 0xFF) {
                break;
            }
            ((ScrollWindowFunc)ScrollWindow)((Window *)((u32)menu->pageWindows + (windowList[i] << 4)), direction, pixels, 0);
            ScheduleWindowCopyToVram((Window *)((u32)menu->pageWindows + (windowList[i] << 4)));
        }
    }
}

static void BattlePartyMenu_OffsetButtonSprites(BattlePartyMenu *menu, u32 button, int frame) {
    int dy;

    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        dy = gBattlePartySlotButtonSpriteOffsets[frame];
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 13)], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[button], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 7)], 0, dy);
        break;
    case 7:
        dy = gBattlePartyButtonSpriteOffsets[frame];
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[menu->context->selectedMon], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[menu->context->selectedMon + 7], 0, dy);
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 7)], 0, gBattlePartyButtonSpriteOffsets[frame]);
        break;
    case 19:
    case 20:
    case 21:
    case 22:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 2)], 0, gBattlePartyButtonSpriteOffsets[frame]);
        break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button - 2)], 0, gBattlePartyButtonSpriteOffsets[frame]);
        break;
    }
}

static void BattlePartyMenu_StartButtonAnim(BattlePartyMenu *menu, u32 button) {
    menu->buttonAnimPalette = 0;
    if (button <= 5 && BattlePartyMenu_GetSlotType(menu, button) == 2) {
        menu->buttonAnimPalette = 1;
    }
    menu->buttonAnimState = 0;
    menu->buttonAnimTimer = 0;
    menu->buttonAnimButton = button;
    menu->buttonAnimActive = TRUE;
}

enum {
    BUTTON_ANIM_STATE_PRESS,
    BUTTON_ANIM_STATE_HOLD,
    BUTTON_ANIM_STATE_RELEASE,
};

static void BattlePartyMenu_UpdateButtonAnim(BattlePartyMenu *menu) {
    if (menu->buttonAnimActive) {
        switch (menu->buttonAnimState) {
        case BUTTON_ANIM_STATE_PRESS:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 1, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 1);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 1);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = BUTTON_ANIM_STATE_HOLD;
            break;
        case BUTTON_ANIM_STATE_HOLD:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 2, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 2);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 2);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = BUTTON_ANIM_STATE_RELEASE;
            break;
        case BUTTON_ANIM_STATE_RELEASE:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 0, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 0);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 0);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimActive = FALSE;
            break;
        }
    }
}

static void BattlePartyMenu_DrawPageButtons(BattlePartyMenu *menu, u32 page) {
    u16 i;

    switch (page) {
    case BATTLE_PARTY_PAGE_PARTY_LIST:
        for (i = 0; i < 6; i++) {
            int slotType = BattlePartyMenu_GetSlotType(menu, i);
            if (slotType == 0) {
                BattlePartyMenu_DrawButton(menu, (u8)i, 3, 1);
            } else if (slotType == 1) {
                BattlePartyMenu_DrawButton(menu, (u8)i, 0, 0);
            } else if (slotType == 2) {
                BattlePartyMenu_DrawButton(menu, (u8)i, 0, 1);
            }
        }
        if (menu->context->mode == BATTLE_PARTY_MODE_SWITCH_NO_CANCEL) {
            BattlePartyMenu_DrawButton(menu, 6, 3, 0);
        } else {
            BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        }
        break;
    case BATTLE_PARTY_PAGE_MON_MENU:
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        BattlePartyMenu_DrawButton(menu, 7, 0, 0);
        if (menu->mons[menu->context->selectedMon].isEgg) {
            BattlePartyMenu_DrawButton(menu, 8, 3, 0);
            BattlePartyMenu_DrawButton(menu, 10, 3, 0);
        } else {
            BattlePartyMenu_DrawButton(menu, 8, 0, 0);
            BattlePartyMenu_DrawButton(menu, 10, 0, 0);
        }
        break;
    case BATTLE_PARTY_PAGE_SUMMARY:
        if (BattlePartyMenu_HasMultipleNonEggMons(menu) == 1) {
            BattlePartyMenu_DrawButton(menu, 12, 0, 0);
            BattlePartyMenu_DrawButton(menu, 13, 0, 0);
        } else {
            BattlePartyMenu_DrawButton(menu, 12, 3, 0);
            BattlePartyMenu_DrawButton(menu, 13, 3, 0);
        }
        BattlePartyMenu_DrawButton(menu, 11, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        break;
    case BATTLE_PARTY_PAGE_CHECK_MOVES:
        if (BattlePartyMenu_HasMultipleNonEggMons(menu) == 1) {
            BattlePartyMenu_DrawButton(menu, 12, 0, 0);
            BattlePartyMenu_DrawButton(menu, 13, 0, 0);
        } else {
            BattlePartyMenu_DrawButton(menu, 12, 3, 0);
            BattlePartyMenu_DrawButton(menu, 13, 3, 0);
        }
        for (i = 0; i < 4; i++) {
            if (menu->mons[menu->context->selectedMon].moves[i].move != MOVE_NONE) {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 14), 0, 0);
            } else {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 14), 3, 0);
            }
        }
        BattlePartyMenu_DrawButton(menu, 9, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        break;
    case BATTLE_PARTY_PAGE_MOVE_DETAIL:
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        for (i = 0; i < 4; i++) {
            if (menu->context->selectedMove == i) {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 30), 2, 0);
            } else {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 30), 0, 0);
            }
        }
        break;
    case BATTLE_PARTY_PAGE_RESTORE_PP:
        for (i = 0; i < 4; i++) {
            if (menu->mons[menu->context->selectedMon].moves[i].move != MOVE_NONE) {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 19), 0, 0);
            } else {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 19), 3, 0);
            }
        }
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        break;
    case BATTLE_PARTY_PAGE_LEARN_MOVE:
    case BATTLE_PARTY_PAGE_LEARN_MOVE_CONTEST:
        BattlePartyMenu_DrawButton(menu, 23, 0, 0);
        BattlePartyMenu_DrawButton(menu, 24, 0, 0);
        BattlePartyMenu_DrawButton(menu, 25, 0, 0);
        BattlePartyMenu_DrawButton(menu, 26, 0, 0);
        BattlePartyMenu_DrawButton(menu, 27, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        if (menu->canToggleView == 1) {
            BattlePartyMenu_DrawButton(menu, 18, 0, 0);
        }
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE:
        BattlePartyMenu_DrawButton(menu, 28, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        if (menu->canToggleView == 1) {
            BattlePartyMenu_DrawButton(menu, 18, 0, 0);
        }
        break;
    case BATTLE_PARTY_PAGE_FORGET_MOVE_CONTEST:
        BattlePartyMenu_DrawButton(menu, 29, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        if (menu->canToggleView == 1) {
            BattlePartyMenu_DrawButton(menu, 18, 0, 0);
        }
        break;
    }
}

static void BattlePartyMenu_LoadPagePalette(BattlePartyMenu *menu, int page) {
    if (page == BATTLE_PARTY_PAGE_CHECK_MOVES) {
        PaletteData_LoadPalette(menu->paletteData, menu->pagePalettes[1], PLTTBUF_SUB_BG, 0xC0, 0x20);
    } else {
        PaletteData_LoadPalette(menu->paletteData, menu->pagePalettes[0], PLTTBUF_SUB_BG, 0xC0, 0x20);
    }
}

static u8 BattlePartyMenu_HasMultipleNonEggMons(BattlePartyMenu *menu) {
    u16 i;
    u16 count = 0;

    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE && !menu->mons[i].isEgg) {
            count++;
        }
    }
    if (count >= 2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void BattlePartyMenu_RedrawSelectedMonButton(BattlePartyMenu *menu) {
    if (BattlePartyMenu_GetSlotType(menu, menu->context->selectedMon) == 2) {
        BattlePartyMenu_DrawButton(menu, menu->context->selectedMon, 0, 1);
    } else {
        BattlePartyMenu_DrawButton(menu, menu->context->selectedMon, 0, 0);
    }
}
