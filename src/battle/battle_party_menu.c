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
#include "move.h"
#include "msgdata.h"
#include "palette.h"
#include "party.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
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
#include "message_printer_number.h"
#include "unk_0208805C.h"
#include "unk_02088288.h"

// In-battle party screen (overlay 8). Pages: 0 party list, 1 mon menu (SHIFT / SUMMARY / CHECK MOVES), 2 summary,
// 3 move list, 4 move detail, 5 restore PP, 6 / 8 learn move (battle / contest view), 7 / 9 confirm the move to
// forget (battle / contest view).

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
    u8 hideGender : 1; // clear when GetMonData(MON_DATA_NO_PRINT_GENDER) returns 1
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
    u8 activeMon;    // party slot selected on entry (msg 78)
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
} BattlePartyMenu; // size: 0x2090

typedef struct BattlePartyMenuPos {
    int x;
    int y;
} BattlePartyMenuPos;

typedef struct BattlePartyMenuOrder {
    u8 slots[6];
} BattlePartyMenuOrder;

void BattlePartyMenu_Task(SysTask *task, void *data);
int BattlePartyMenu_Setup(BattlePartyMenu *menu);
int BattlePartyMenu_HandlePartyListInput(BattlePartyMenu *menu);
int BattlePartyMenu_TryUseItemOnMon(BattlePartyMenu *menu);
int BattlePartyMenu_HandleMonMenuInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleSummaryInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleCheckMovesInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleMoveDetailInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleLearnMoveInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleForgetMoveInput(BattlePartyMenu *menu);
int BattlePartyMenu_HandleRestorePPInput(BattlePartyMenu *menu);
int BattlePartyMenu_ShowPartyListPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowMonMenuPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowSummaryPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowCheckMovesPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowMoveDetailPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowLearnMovePage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowForgetMovePage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowRestorePPPage(BattlePartyMenu *menu);
int BattlePartyMenu_RefreshMonPage(BattlePartyMenu *menu);
int BattlePartyMenu_ShowPendingMessage(BattlePartyMenu *menu);
int BattlePartyMenu_CloseMessage(BattlePartyMenu *menu);
int BattlePartyMenu_WaitMessage(const BattlePartyMenu *menu);
int BattlePartyMenu_WaitMessageInput(const BattlePartyMenu *menu);
int BattlePartyMenu_WaitButtonAnim(const BattlePartyMenu *menu);
int BattlePartyMenu_AnimateItemEffect(BattlePartyMenu *menu);
int BattlePartyMenu_AnimateRestoreAllPP(BattlePartyMenu *menu);
int BattlePartyMenu_StartFadeOut(BattlePartyMenu *menu);
int BattlePartyMenu_Exit(SysTask *task, BattlePartyMenu *menu);
void BattlePartyMenu_InitBgs(BattlePartyMenu *menu);
void BattlePartyMenu_FreeBgs(BgConfig *bgConfig);
void BattlePartyMenu_LoadBgGraphics(BattlePartyMenu *menu);
void BattlePartyMenu_InitText(BattlePartyMenu *menu);
void BattlePartyMenu_FreeText(BattlePartyMenu *menu);
void BattlePartyMenu_LoadMonData(BattlePartyMenu *menu);
BOOL BattlePartyMenu_HandlePartyListSelection(BattlePartyMenu *menu);
u8 BattlePartyMenu_GetMonMenuInput(BattlePartyMenu *menu);
u8 BattlePartyMenu_GetSummaryInput(BattlePartyMenu *menu);
u8 BattlePartyMenu_GetCheckMovesInput(BattlePartyMenu *menu);
u8 BattlePartyMenu_GetMoveDetailInput(BattlePartyMenu *menu);
int BattlePartyMenu_CheckTouchHitbox(BattlePartyMenu *menu, const TouchscreenHitbox *hitboxes);
int BattlePartyMenu_GetSlotType(BattlePartyMenu *menu, int slot);
u8 BattlePartyMenu_FindNextMon(BattlePartyMenu *menu, int slot, int direction);
void BattlePartyMenu_DrawExpBar(BattlePartyMenu *menu, int page);
void BattlePartyMenu_SetExpBarTile(BattlePartyMenu *menu, u16 tile, u16 x, int y);
void ov08_0221D77C(BattlePartyMenu *menu, u16 tile, int pos);
void ov08_0221D81C(BattlePartyMenu *menu);
void BattlePartyMenu_ChangePage(BattlePartyMenu *menu, int page);
void BattlePartyMenu_LoadPageTilemaps(BattlePartyMenu *menu, int page);
int BattlePartyMenu_CanSwitchToSelectedMon(BattlePartyMenu *menu);
u8 BattlePartyMenu_IsSelectedMonEgg(BattlePartyMenu *menu);
BOOL BattlePartyMenu_IsDoubleBattle(BattlePartyMenu *menu);
BOOL BattlePartyMenu_IsMultiBattle(BattlePartyMenu *menu);
int BattlePartyMenu_IsPartnerMon(BattlePartyMenu *menu, u32 slot);
BOOL BattlePartyMenu_IsSelectedMoveHM(BattlePartyMenu *menu);
void BattlePartyMenu_ClearBattleMoveInfo(BattlePartyMenu *menu);
void BattlePartyMenu_ClearContestMoveInfo(BattlePartyMenu *menu);
void BattlePartyMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID);
void BattlePartyMenu_AddWindows(BattlePartyMenu *menu);
void BattlePartyMenu_AddPageWindows(BattlePartyMenu *menu, int page);
void BattlePartyMenu_RemovePageWindows(BattlePartyMenu *menu);
void BattlePartyMenu_RemoveWindows(BattlePartyMenu *menu);
void BattlePartyMenu_PrintPageText(BattlePartyMenu *menu, int page);
void BattlePartyMenu_PrintMonName(BattlePartyMenu *menu, int window, int fontId, u8 slot, u8 x, u8 y);
void BattlePartyMenu_PrintMonLevel(BattlePartyMenu *menu, int window, int slot, int x, u8 y);
void BattlePartyMenu_PrintMonHP(BattlePartyMenu *menu, int window, int slot, u32 x, u8 y);
void BattlePartyMenu_DrawHPBar(BattlePartyMenu *menu, int window, int slot, u16 x, volatile u8 y);
void BattlePartyMenu_PrintAbilityName(BattlePartyMenu *menu, int window, int slot);
void BattlePartyMenu_PrintHeldItemName(BattlePartyMenu *menu, int window, int slot);
void BattlePartyMenu_PrintMoveName(BattlePartyMenu *menu, int move, int window, int msgNo, u16 fontId, u16 y, u32 color);
void BattlePartyMenu_PrintPPLabel(BattlePartyMenu *menu, int window, u16 x, u32 y);
void BattlePartyMenu_PrintPrompt(BattlePartyMenu *menu, int msgNo);
void BattlePartyMenu_PrintButtonLabel(BattlePartyMenu *menu, int window, int msgNo);
void BattlePartyMenu_PrintLevelAndNextExp(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintAttack(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintDefense(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintSpeed(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintSpAttack(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintSpDefense(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintHPStat(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintAbilityDescription(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintAccuracyLabel(BattlePartyMenu *menu, int window);
void BattlePartyMenu_PrintMoveAccuracy(BattlePartyMenu *menu, int window, int accuracy);
void BattlePartyMenu_PrintPowerLabel(BattlePartyMenu *menu, int window);
void BattlePartyMenu_PrintMovePower(BattlePartyMenu *menu, int window, u32 power);
void BattlePartyMenu_PrintMoveDescription(BattlePartyMenu *menu, int window, int move);
void BattlePartyMenu_PrintCategoryLabel(BattlePartyMenu *menu, int window);
void BattlePartyMenu_PrintMoveCategory(BattlePartyMenu *menu, int window, int moveClass);
void BattlePartyMenu_PrintMovePP(BattlePartyMenu *menu, int window, int pp, int maxPP);
void BattlePartyMenu_PrintForgetButtonLabel(BattlePartyMenu *menu, int window);
void BattlePartyMenu_PrintHMCantBeForgotten(BattlePartyMenu *menu);
void BattlePartyMenu_PrintMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window);
void BattlePartyMenu_UpdateMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window);
void BattlePartyMenu_PrintPartyListPage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintMonHPAndBar(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintPartyMonLevel(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintMonMenuPage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintMonNameCentered(BattlePartyMenu *menu, int slot);
void BattlePartyMenu_PrintCheckMovesPage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintSummaryPage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintMoveDetailPage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintLearnMovePage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintForgetMovePage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintRestorePPPage(BattlePartyMenu *menu);
void BattlePartyMenu_UpdateMovePP(BattlePartyMenu *menu, int window, int moveSlot);
void BattlePartyMenu_PrintContestForgetMovePage(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMessage(BattlePartyMenu *menu);
void BattlePartyMenu_PrintMessage(BattlePartyMenu *menu);
void BattlePartyMenu_BufferItemResultMessage(BattlePartyMenu *menu);
void BattlePartyMenu_BufferEmbargoMessage(BattlePartyMenu *menu);
void BattlePartyMenu_InitSprites(BattlePartyMenu *menu);
void BattlePartyMenu_CreateSpriteManager(BattlePartyMenu *menu);
void BattlePartyMenu_LoadMonIconResources(BattlePartyMenu *menu);
void BattlePartyMenu_LoadStatusIconResources(BattlePartyMenu *menu);
void BattlePartyMenu_LoadTypeIconResources(BattlePartyMenu *menu);
void BattlePartyMenu_LoadHeldItemIconResources(BattlePartyMenu *menu);
void ov08_02220928(BattlePartyMenu *menu);
ManagedSprite *BattlePartyMenu_CreateSprite(BattlePartyMenu *menu, int index);
void BattlePartyMenu_CreateSprites(BattlePartyMenu *menu);
void BattlePartyMenu_FreeSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y);
void BattlePartyMenu_SetMonIconPalettes(BattlePartyMenu *menu);
void BattlePartyMenu_SetTypeIcon(BattlePartyMenu *menu, ManagedSprite *sprite, int resId, u32 type);
void BattlePartyMenu_SetMoveCategoryIcon(BattlePartyMenu *menu, ManagedSprite *sprite, u32 moveClass);
void BattlePartyMenu_ShowStatusIcon(int status, ManagedSprite *sprite, int x, int y);
void BattlePartyMenu_ShowMonTypeIcons(BattlePartyMenu *menu, BattlePartyMenuMon *mon, const BattlePartyMenuPos *pos);
void BattlePartyMenu_ShowHeldItemIcon(u16 item, ManagedSprite *sprite, int x, int y);
void BattlePartyMenu_ShowCapsuleIcon(BOOL hasCapsule, ManagedSprite *sprite, int x, int y);
void BattlePartyMenu_ShowPageSprites(BattlePartyMenu *menu, u32 page);
void BattlePartyMenu_ShowPartyListSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMonMenuSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowSummarySprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowCheckMovesSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMoveDetailSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowLearnMoveSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowForgetMoveSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowRestorePPSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowContestForgetMoveSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowContestLearnMoveSprites(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMoveTypeIcons(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMoveContestTypeIcons(BattlePartyMenu *menu);
void BattlePartyMenu_ShowMoveIcons(BattlePartyMenu *menu);
void BattlePartyMenu_SetSpriteAnim(ManagedSprite *sprite, u32 anim);
int BattlePartyMenu_GetMonIconAnim(BattlePartyMenuMon *mon);
void BattlePartyMenu_UpdateMonIcons(BattlePartyMenu *menu);
s16 BattlePartyMenu_CalcConditionMarkerCoord(u8 stat, int target, int base);
void BattlePartyMenu_ShowConditionMarkers(BattlePartyMenu *menu);
void BattlePartyMenu_CreateCursor(BattlePartyMenu *menu);
void BattlePartyMenu_FreeCursor(BattlePartyMenu *menu);
void BattlePartyMenu_SetLearnMoveCursorMask(BattlePartyMenu *menu);
void BattlePartyMenu_SetForgetMoveCursorMask(BattlePartyMenu *menu);
void BattlePartyMenu_SetPageCursor(BattlePartyMenu *menu, u32 page);
void BattlePartyMenu_DeactivateCursor(BattlePartyMenu *menu);
void BattlePartyMenu_StoreButtonTilemaps(BattlePartyMenu *menu, const u16 *screen);
void BattlePartyMenu_StoreLargeButtonTilemaps(BattlePartyMenu *menu, const u16 *screen);
void BattlePartyMenu_CopyTilemapRect(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height);
u16 *BattlePartyMenu_GetButtonTilemap(BattlePartyMenu *menu, int button, int frame, int partner);
void BattlePartyMenu_BuildButtonTilemap(BattlePartyMenu *menu, u16 *dst, int button, int frame, u8 palette);
void BattlePartyMenu_DrawButton(BattlePartyMenu *menu, u8 button, u8 frame, u8 palette);
void BattlePartyMenu_ScrollButtonWindows(BattlePartyMenu *menu, u32 button, int frame);
void BattlePartyMenu_OffsetButtonSprites(BattlePartyMenu *menu, u32 button, int frame);
void BattlePartyMenu_StartButtonAnim(BattlePartyMenu *menu, u32 button);
void BattlePartyMenu_UpdateButtonAnim(BattlePartyMenu *menu);
void BattlePartyMenu_DrawPageButtons(BattlePartyMenu *menu, u32 page);
void BattlePartyMenu_LoadPagePalette(BattlePartyMenu *menu, int page);
u8 BattlePartyMenu_HasMultipleNonEggMons(BattlePartyMenu *menu);
void BattlePartyMenu_RedrawSelectedMonButton(BattlePartyMenu *menu);

// type_icon_resources.c defines it without parameters; move_relearner_app.c declares (and calls) it with one, so
// the public header cannot carry a prototype yet.
NarcId MoveCategoryIcon_GetNarc(void);

extern const BattlePartyMenuOrder sBattlePartyMultiBattleOrder;
extern const GraphicsModes sBattlePartyGraphicsModes;
extern const TouchscreenHitbox sBattlePartyForgetMoveHitboxes[];
extern const TouchscreenHitbox sBattlePartyMonMenuHitboxes[];
extern const TouchscreenHitbox sBattlePartySummaryHitboxes[];
extern const TouchscreenHitbox sBattlePartyMoveDetailHitboxes[];
extern const TouchscreenHitbox sBattlePartyRestorePPHitboxes[];
extern const BgTemplate sBattlePartyBgTemplateSub3;
extern const BgTemplate sBattlePartyBgTemplateSub0;
extern const BgTemplate sBattlePartyBgTemplateSub2;
extern const BgTemplate sBattlePartyBgTemplateSub1;
extern const TouchscreenHitbox sBattlePartyPartyListHitboxes[];
extern const TouchscreenHitbox sBattlePartyLearnMoveHitboxes[];
extern const TouchscreenHitbox sBattlePartyCheckMovesHitboxes[];
extern const u32 sBattlePartyPageScreenFiles[][2];
extern const u32 sBattlePartyMoveNameMsgIds[];
extern const u32 sBattlePartyMonNameMsgIds[];
extern const WindowTemplate sBattlePartyMonMenuWindows[];
extern const WindowTemplate sBattlePartyRestorePPWindows[];
extern const WindowTemplate sBattlePartyLearnMoveWindows[];
extern const WindowTemplate sBattlePartyWindowTemplates[];
extern const WindowTemplate sBattlePartyContestForgetMoveWindows[];
extern const WindowTemplate sBattlePartyCheckMovesWindows[];
extern const WindowTemplate sBattlePartyForgetMoveWindows[];
extern const WindowTemplate sBattlePartyMoveDetailWindows[];
extern const WindowTemplate sBattlePartyPartyListWindows[];
extern const WindowTemplate sBattlePartySummaryWindows[];
extern const BattlePartyMenuPos sBattlePartyForgetMoveTypeIconPos[];
extern const BattlePartyMenuPos sBattlePartyLearnMoveTypeIconPos[];
extern const BattlePartyMenuPos sBattlePartyCheckMovesTypeIconPos[];
extern const BattlePartyMenuPos sBattlePartyMoveDetailTypeIconPos[];
extern const BattlePartyMenuPos sBattlePartySummaryTypeIconPos[];
extern const BattlePartyMenuPos sBattlePartyCheckMovesMoveIconPos[];
extern const BattlePartyMenuPos sBattlePartyLearnMoveMoveIconPos[];
extern const BattlePartyMenuPos sBattlePartyListIconPos[];
extern const BattlePartyMenuPos sBattlePartyListStatusIconPos[];
extern const SpriteResourceCountsListUnion sBattlePartySpriteCapacities;
// {char, palette, cell, anim, draw priority} per sprite; the interior labels collapse once the table moves to C
extern const int sBattlePartySpriteTemplates[][5];
extern const int sBattlePartySpriteTemplatesPltt[][5];
extern const int sBattlePartySpriteTemplatesCell[][5];
extern const int sBattlePartySpriteTemplatesAnim[][5];
extern const int sBattlePartySpriteTemplatesPriority[][5];
extern const u8 sBattlePartyButtonScrollAmounts[];
extern const u8 sBattlePartySlotButtonScrollAmounts[];
extern const s16 sBattlePartyButtonSpriteOffsets[];
extern const s16 sBattlePartySlotButtonSpriteOffsets[];
extern const u8 *const sBattlePartyButtonWindowLists[];
// {x, y, width, height} per button, read as bytes through labels on its members
extern const u8 sBattlePartyButtonRects[];
extern const u8 sBattlePartyButtonRectsY[];
extern const u8 sBattlePartyButtonRectsWidth[];
extern const u8 sBattlePartyButtonRectsHeight[];
extern const DpadMenuBox *const sBattlePartyPageCursorBoxes[];

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
    menu->battlerPosition = ov12_0223AB0C(context->battleSystem, context->battlerId);
}

void BattlePartyMenu_Task(SysTask *task, void *data) {
    BattlePartyMenu *menu = data;

    switch (menu->state) {
    case 0:
        menu->state = BattlePartyMenu_Setup(menu);
        break;
    case 1:
        menu->state = BattlePartyMenu_HandlePartyListInput(menu);
        break;
    case 2:
        menu->state = BattlePartyMenu_HandleMonMenuInput(menu);
        break;
    case 3:
        menu->state = BattlePartyMenu_HandleSummaryInput(menu);
        break;
    case 4:
        menu->state = BattlePartyMenu_HandleCheckMovesInput(menu);
        break;
    case 5:
        menu->state = BattlePartyMenu_HandleMoveDetailInput(menu);
        break;
    case 6:
        menu->state = BattlePartyMenu_ShowPartyListPage(menu);
        break;
    case 7:
        menu->state = BattlePartyMenu_ShowMonMenuPage(menu);
        break;
    case 8:
        menu->state = BattlePartyMenu_ShowSummaryPage(menu);
        break;
    case 9:
        menu->state = BattlePartyMenu_ShowCheckMovesPage(menu);
        break;
    case 10:
        menu->state = BattlePartyMenu_ShowMoveDetailPage(menu);
        break;
    case 11:
        menu->state = BattlePartyMenu_ShowLearnMovePage(menu);
        break;
    case 12:
        menu->state = BattlePartyMenu_ShowForgetMovePage(menu);
        break;
    case 13:
        menu->state = BattlePartyMenu_ShowRestorePPPage(menu);
        break;
    case 14:
        menu->state = BattlePartyMenu_RefreshMonPage(menu);
        break;
    case 15:
        menu->state = BattlePartyMenu_ShowPendingMessage(menu);
        break;
    case 16:
        menu->state = BattlePartyMenu_CloseMessage(menu);
        break;
    case 17:
        menu->state = BattlePartyMenu_WaitMessage(menu);
        break;
    case 18:
        menu->state = BattlePartyMenu_WaitMessageInput(menu);
        break;
    case 19:
        menu->state = BattlePartyMenu_HandleLearnMoveInput(menu);
        break;
    case 20:
        menu->state = BattlePartyMenu_HandleForgetMoveInput(menu);
        break;
    case 21:
        menu->state = BattlePartyMenu_HandleRestorePPInput(menu);
        break;
    case 22:
        menu->state = BattlePartyMenu_WaitButtonAnim(menu);
        break;
    case 23:
        menu->state = BattlePartyMenu_AnimateItemEffect(menu);
        break;
    case 24:
        menu->state = BattlePartyMenu_AnimateRestoreAllPP(menu);
        break;
    case 25:
        menu->state = BattlePartyMenu_StartFadeOut(menu);
        break;
    case 26:
        if (BattlePartyMenu_Exit(task, menu) == 1) {
            return;
        }
        break;
    }
    BattlePartyMenu_UpdateMonIcons(menu);
    SpriteSystem_DrawSprites(menu->spriteManager);
    BattlePartyMenu_UpdateButtonAnim(menu);
}

int BattlePartyMenu_Setup(BattlePartyMenu *menu) {
    int nextState;

    G2S_BlendNone();
    if (menu->context->mode == 3) {
        menu->page = 6;
        nextState = 19;
    } else {
        menu->page = 0;
        nextState = 1;
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
    if (menu->page == 0 && BattlePartyMenu_IsPartnerMon(menu, 0) == 1) {
        menu->context->selectedMon = 1;
    }
    BattlePartyMenu_SetPageCursor(menu, menu->page);
    BattlePartyMenu_DrawExpBar(menu, menu->page);
    PaletteData_BeginPaletteFade(menu->paletteData, 10, 0xFFFF, -8, 16, 0, 0);
    return nextState;
}

int BattlePartyMenu_HandlePartyListInput(BattlePartyMenu *menu) {
    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData) != 0) {
        return 1;
    }
    if (BattlePartyMenu_HandlePartyListSelection(menu) == 1) {
        if (menu->context->selectedMon == 6) {
            if (menu->context->mode != 1) {
                PlaySE(SEQ_SE_DP_DECIDE);
                BattlePartyMenu_StartButtonAnim(menu, 6);
                return 25;
            }
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            BattlePartyMenu_StartButtonAnim(menu, menu->context->selectedMon);
            if (menu->context->mode == 2) {
                return BattlePartyMenu_TryUseItemOnMon(menu);
            }
            menu->nextState = 7;
            return 22;
        }
    }
    return 1;
}

int BattlePartyMenu_TryUseItemOnMon(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;

    if ((context->selectedMon == 0 && context->embargo[0]) || (context->selectedMon == 1 && context->embargo[1])) {
        BattlePartyMenu_BufferEmbargoMessage(menu);
        BattlePartyMenu_ShowMessage(menu);
        menu->context->selectedMon = 6;
        menu->nextState = 25;
        return 17;
    }
    if (GetItemAttr(context->item, ITEMATTR_PP_RESTORE, context->heapID) && !GetItemAttr(context->item, ITEMATTR_PP_RESTORE_ALL, context->heapID) && !menu->mons[context->selectedMon].isEgg) {
        menu->nextState = 13;
        return 22;
    }
    if (BattleSystem_RecoverStatus(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon], 0, context->item) == TRUE) {
        if (GetItemAttr(context->item, ITEMATTR_PP_RESTORE_ALL, context->heapID)) {
            menu->nextState = 13;
        } else if (BattlePartyMenu_GetSlotType(menu, context->selectedMon) == 1 && !GetItemAttr(context->item, ITEMATTR_REVIVE, context->heapID)) {
            BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
            menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
            context->hpRestored = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_HP, NULL);
            context->hpRestored -= menu->mons[context->selectedMon].hp;
            menu->nextState = 25;
        } else {
            menu->nextState = 23;
        }
        menu->effectState = 0;
        return 22;
    }
    ReadMsgDataIntoString(menu->msgData, 81, menu->string);
    BattlePartyMenu_ShowMessage(menu);
    menu->context->selectedMon = 6;
    menu->nextState = 25;
    return 17;
}

int BattlePartyMenu_HandleMonMenuInput(BattlePartyMenu *menu) {
    switch (BattlePartyMenu_GetMonMenuInput(menu)) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 7);
        if (BattlePartyMenu_CanSwitchToSelectedMon(menu) == 1) {
            return 25;
        }
        menu->nextState = 15;
        return 22;
    case 1:
        if (BattlePartyMenu_IsSelectedMonEgg(menu) == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 8);
        menu->nextState = 8;
        return 22;
    case 2:
        if (BattlePartyMenu_IsSelectedMonEgg(menu) == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 10);
        menu->nextState = 9;
        return 22;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = 6;
        return 22;
    }
    return 2;
}

int BattlePartyMenu_HandleSummaryInput(BattlePartyMenu *menu) {
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
        menu->nextState = 14;
        return 22;
    case 1:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, 1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 13);
        menu->nextState = 14;
        return 22;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 11);
        menu->nextState = 9;
        return 22;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->monMenuPosition = 1;
        menu->nextState = 7;
        return 22;
    }
    return 3;
}

int BattlePartyMenu_HandleCheckMovesInput(BattlePartyMenu *menu) {
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
        menu->nextState = 10;
        return 22;
    case 4:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, -1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 12);
        menu->nextState = 14;
        return 22;
    case 5:
        slot = BattlePartyMenu_FindNextMon(menu, menu->context->selectedMon, 1);
        if (slot == 0xFF) {
            break;
        }
        menu->context->selectedMon = slot;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 13);
        menu->nextState = 14;
        return 22;
    case 6:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 9);
        menu->nextState = 8;
        return 22;
    case 7:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->monMenuPosition = 2;
        menu->nextState = 7;
        return 22;
    }
    return 4;
}

int BattlePartyMenu_HandleMoveDetailInput(BattlePartyMenu *menu) {
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
        return 10;
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = 9;
        return 22;
    }
    return 5;
}

#pragma force_active reset

int BattlePartyMenu_HandleLearnMoveInput(BattlePartyMenu *menu) {
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
        menu->nextState = 12;
        return 22;
    case 5:
        if (menu->canToggleView == 0) {
            break;
        }
        menu->viewMode ^= 1;
        menu->learnMovePosition = input;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 18);
        menu->nextState = 11;
        return 22;
    case 6:
        menu->context->selectedMove = 4;
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = 25;
        return 22;
    }
    return 19;
}

int BattlePartyMenu_HandleForgetMoveInput(BattlePartyMenu *menu) {
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
        if (menu->viewMode == 0) {
            BattlePartyMenu_StartButtonAnim(menu, 28);
        } else {
            BattlePartyMenu_StartButtonAnim(menu, 29);
        }
        if (BattlePartyMenu_IsSelectedMoveHM(menu) == 1) {
            BattlePartyMenu_PrintHMCantBeForgotten(menu);
            if (menu->viewMode == 0) {
                BattlePartyMenu_ClearBattleMoveInfo(menu);
            } else {
                BattlePartyMenu_ClearContestMoveInfo(menu);
            }
            menu->nextState = 20;
        } else {
            menu->nextState = 25;
        }
        return 22;
    case 1:
        if (menu->canToggleView == 0) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 18);
        menu->viewMode ^= 1;
        menu->forgetMovePosition = input;
        menu->nextState = 12;
        return 22;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->forgetMovePosition = 0;
        menu->nextState = 11;
        return 22;
    }
    return 20;
}

int BattlePartyMenu_HandleRestorePPInput(BattlePartyMenu *menu) {
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
            menu->nextState = 23;
            return 22;
        }
        ReadMsgDataIntoString(menu->msgData, 81, menu->string);
        BattlePartyMenu_ShowMessage(menu);
        menu->context->selectedMon = 6;
        menu->nextState = 25;
        return 17;
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        BattlePartyMenu_StartButtonAnim(menu, 6);
        menu->nextState = 6;
        return 22;
    }
    return 21;
}

#pragma force_active on

int BattlePartyMenu_ShowPartyListPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 0);
    return 1;
}

int BattlePartyMenu_ShowMonMenuPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 1);
    return 2;
}

int BattlePartyMenu_ShowSummaryPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 2);
    return 3;
}

int BattlePartyMenu_ShowCheckMovesPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 3);
    return 4;
}

int BattlePartyMenu_ShowMoveDetailPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 4);
    return 5;
}

int BattlePartyMenu_ShowLearnMovePage(BattlePartyMenu *menu) {
    if (menu->viewMode == 0) {
        BattlePartyMenu_ChangePage(menu, 6);
    } else {
        BattlePartyMenu_ChangePage(menu, 8);
    }
    return 19;
}

int BattlePartyMenu_ShowForgetMovePage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowMoveIcons(menu);
    if (menu->viewMode == 0) {
        BattlePartyMenu_ChangePage(menu, 7);
    } else {
        BattlePartyMenu_ChangePage(menu, 9);
    }
    return 20;
}

int BattlePartyMenu_ShowRestorePPPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ChangePage(menu, 5);
    if (GetItemAttr(menu->context->item, ITEMATTR_PP_RESTORE_ALL, menu->context->heapID) != 0) {
        return 24;
    }
    return 21;
}

int BattlePartyMenu_RefreshMonPage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowPageSprites(menu, menu->page);
    BattlePartyMenu_PrintPageText(menu, menu->page);
    BattlePartyMenu_DrawPageButtons(menu, menu->page);
    BattlePartyMenu_DrawExpBar(menu, menu->page);
    if (menu->page == 2) {
        return 3;
    }
    return 4;
}

int BattlePartyMenu_ShowPendingMessage(BattlePartyMenu *menu) {
    BattlePartyMenu_ShowMessage(menu);
    menu->nextState = 16;
    return 17;
}

int BattlePartyMenu_CloseMessage(BattlePartyMenu *menu) {
    ClearFrameAndWindow2(&menu->windows[1], FALSE);
    return 2;
}

int BattlePartyMenu_WaitMessage(const BattlePartyMenu *menu) {
    if (TextPrinterCheckActive(menu->textPrinterId) == 0) {
        return 18;
    }
    return 17;
}

int BattlePartyMenu_WaitMessageInput(const BattlePartyMenu *menu) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == 1) {
        return menu->nextState;
    }
    return 18;
}

int BattlePartyMenu_WaitButtonAnim(const BattlePartyMenu *menu) {
    if (menu->buttonAnimActive == 0) {
        return menu->nextState;
    }
    return 22;
}

int BattlePartyMenu_AnimateItemEffect(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;

    switch (menu->effectState) {
    case 0:
        menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
        BattlePartyMenu_BufferItemResultMessage(menu);
        if (menu->page == 5) {
            menu->ppTargets[0] = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_MOVE1_PP + context->selectedMove, NULL);
            menu->effectState = 2;
        } else {
            int status = Pokemon_GetStatusIconId(menu->mons[context->selectedMon].mon);
            menu->mons[context->selectedMon].status = status;
            if (menu->mons[context->selectedMon].status == 7) {
                ManagedSprite_SetDrawFlag(menu->sprites[context->selectedMon + 13], FALSE);
                BattlePartyMenu_PrintPartyMonLevel(menu, context->selectedMon);
            }
            menu->hpTarget = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_HP, NULL);
            menu->effectState = 4;
        }
        PlaySE(SEQ_SE_DP_KAIFUKU);
        break;
    case 1:
        if (menu->hpTarget != menu->mons[context->selectedMon].hp) {
            menu->mons[context->selectedMon].hp++;
            BattlePartyMenu_PrintMonHPAndBar(menu, context->selectedMon);
        } else {
            menu->effectState = 3;
        }
        break;
    case 2:
        if (menu->ppTargets[0] != menu->mons[context->selectedMon].moves[context->selectedMove].pp) {
            menu->mons[context->selectedMon].moves[context->selectedMove].pp++;
            BattlePartyMenu_UpdateMovePP(menu, (u16)(context->selectedMove + 1), context->selectedMove);
        } else {
            menu->effectState = 3;
        }
        break;
    case 3:
        BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
        BattlePartyMenu_ShowMessage(menu);
        menu->nextState = 25;
        return 17;
    case 4:
        if (menu->hpTarget != menu->mons[context->selectedMon].hp) {
            menu->mons[context->selectedMon].hp++;
            BattlePartyMenu_PrintMonHPAndBar(menu, context->selectedMon);
            BattlePartyMenu_RedrawSelectedMonButton(menu);
        }
        menu->effectState = 1;
        break;
    }
    return 23;
}

int BattlePartyMenu_AnimateRestoreAllPP(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    u32 i;
    u32 count;

    switch (menu->effectState) {
    case 0:
        menu->mons[context->selectedMon].mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, context->partyOrder[context->selectedMon]);
        for (i = 0; i < 4; i++) {
            if (menu->mons[context->selectedMon].moves[i].move != MOVE_NONE) {
                menu->ppTargets[i] = GetMonData(menu->mons[context->selectedMon].mon, MON_DATA_MOVE1_PP + i, NULL);
            }
        }
        BattlePartyMenu_BufferItemResultMessage(menu);
        PlaySE(SEQ_SE_DP_KAIFUKU);
        menu->effectState = 1;
        break;
    case 1:
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
            menu->effectState = 2;
        }
        break;
    case 2:
        BattlePartyMenu_ConsumeItem(context->battleSystem, context->item, context->pocket, context->heapID);
        BattlePartyMenu_ShowMessage(menu);
        menu->nextState = 25;
        return 17;
    }
    return 24;
}

int BattlePartyMenu_StartFadeOut(BattlePartyMenu *menu) {
    PaletteData_BeginPaletteFade(menu->paletteData, 10, 0xFFFF, -8, 0, 16, 0);
    return 26;
}

int BattlePartyMenu_Exit(SysTask *task, BattlePartyMenu *menu) {
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

void BattlePartyMenu_InitBgs(BattlePartyMenu *menu) {
    GraphicsModes graphicsModes;
    BgTemplate template3;
    BgTemplate template2;
    BgTemplate template1;
    BgTemplate template0;

    graphicsModes = sBattlePartyGraphicsModes;
    SetScreenModeAndDisable(&graphicsModes, SCREEN_SUB);
    template3 = sBattlePartyBgTemplateSub3;
    InitBgFromTemplate(menu->bgConfig, 7, &template3, 0);
    template2 = sBattlePartyBgTemplateSub2;
    InitBgFromTemplate(menu->bgConfig, 6, &template2, 0);
    template1 = sBattlePartyBgTemplateSub1;
    InitBgFromTemplate(menu->bgConfig, 5, &template1, 0);
    BgClearTilemapBufferAndCommit(menu->bgConfig, 5);
    template0 = sBattlePartyBgTemplateSub0;
    InitBgFromTemplate(menu->bgConfig, 4, &template0, 0);
    BgClearTilemapBufferAndCommit(menu->bgConfig, 4);
    BG_ClearCharDataRange(5, 0x20, 0, menu->context->heapID);
    BG_ClearCharDataRange(4, 0x20, 0, menu->context->heapID);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 5);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 4);
}

void BattlePartyMenu_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 6);
    FreeBgTilemapBuffer(bgConfig, 7);
}

void BattlePartyMenu_LoadBgGraphics(BattlePartyMenu *menu) {
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

void BattlePartyMenu_InitText(BattlePartyMenu *menu) {
    menu->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 6, menu->context->heapID);
    menu->msgPrinter = MessagePrinter_New(15, 14, 0, menu->context->heapID);
    menu->msgFormat = MessageFormat_New(menu->context->heapID);
    menu->string = String_New(0x200, menu->context->heapID);
}

void BattlePartyMenu_FreeText(BattlePartyMenu *menu) {
    DestroyMsgData(menu->msgData);
    MessagePrinter_Delete(menu->msgPrinter);
    MessageFormat_Delete(menu->msgFormat);
    String_Delete(menu->string);
}

void BattlePartyMenu_LoadMonData(BattlePartyMenu *menu) {
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

BOOL BattlePartyMenu_HandlePartyListSelection(BattlePartyMenu *menu) {
    int input = BattlePartyMenu_CheckTouchHitbox(menu, sBattlePartyPartyListHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 6;
        } else if (input == -1) {
            return FALSE;
        }
        if (input != 6 && !BattlePartyMenu_GetSlotType(menu, input)) {
            goto fail;
        }
        menu->context->selectedMon = input;
        return TRUE;
    }
    if (input != 6 && !BattlePartyMenu_GetSlotType(menu, input)) {
        goto fail;
    }
    menu->context->selectedMon = input;
    BattlePartyMenu_DeactivateCursor(menu);
    return TRUE;
fail:
    return FALSE;
}

// Touch first, then the d-pad cursor; B picks the given button. 0xFF when nothing was pressed.
#define DEFINE_GET_PAGE_INPUT(name, hitboxes, cancelButton)           \
    u8 name(BattlePartyMenu *menu) {                                  \
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

int BattlePartyMenu_CheckTouchHitbox(BattlePartyMenu *menu, const TouchscreenHitbox *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}

#pragma force_active reset

// 0: empty slot, 1: a mon the player can use, 2: a mon of the partner trainer (multi battle) or a second
// battler slot
int BattlePartyMenu_GetSlotType(BattlePartyMenu *menu, int slot) {
    if (menu->mons[slot].species == SPECIES_NONE) {
        return 0;
    }
    if (slot == 0) {
        goto return_one;
    }
    if (BattlePartyMenu_IsDoubleBattle(menu)) {
        goto check_one;
    }
    if (!BattlePartyMenu_IsMultiBattle(menu)) {
        goto return_two;
    }
check_one:
    if (slot != 1) {
        goto return_two;
    }
return_one:
    return 1;
return_two:
    return 2;
}

#pragma force_active on

u8 BattlePartyMenu_FindNextMon(BattlePartyMenu *menu, int slot, int direction) {
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

#pragma force_active reset

void BattlePartyMenu_DrawExpBar(BattlePartyMenu *menu, int page) {
    u8 pixels;
    u8 i;
    BattlePartyMenuMon *mon;
    u8 slot;
    u32 exp;
    u32 expToNextLevel;

    if (page != 2) {
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
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 7);
}

void BattlePartyMenu_SetExpBarTile(BattlePartyMenu *menu, u16 tile, u16 x, int y) {
    FillBgTilemapRect(menu->bgConfig, 7, tile, (u8)x, (u8)y, 1, 1, TILEMAP_FILL_KEEP_PAL);
}

#pragma force_active on

// writes the 2x2 tile block at row 14 of background 7 for one of six slots
void ov08_0221D77C(BattlePartyMenu *menu, u16 tile, int pos) {
    FillBgTilemapRect(menu->bgConfig, 7, tile, pos * 2 + 2, 14, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, 7, tile + 1, pos * 2 + 3, 14, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, 7, tile + 32, pos * 2 + 2, 15, 1, 1, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(menu->bgConfig, 7, tile + 33, pos * 2 + 3, 15, 1, 1, TILEMAP_FILL_KEEP_PAL);
}

#pragma force_active reset

void ov08_0221D81C(BattlePartyMenu *menu) {
    u16 i;

    for (i = 0; i < 6; i++) {
        ov08_0221D77C(menu, 0x125, (u8)i);
    }
}

void BattlePartyMenu_ChangePage(BattlePartyMenu *menu, int page) {
    BattlePartyMenu_LoadPageTilemaps(menu, page);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, 4, 0);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, 5, 0);
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

void BattlePartyMenu_LoadPageTilemaps(BattlePartyMenu *menu, int page) {
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

#pragma force_active on

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

// A slot of the mons array addressed as base plus byte offset (the trapped-mon message reads it this way).
typedef BattlePartyMenuMon BattlePartyMenuMons[6];

typedef struct BattlePartyMenuMonRef {
    u32 offset;
    BattlePartyMenuMons *mons;
} BattlePartyMenuMonRef;

#pragma opt_propagation off

// Buffers the reason the selected mon cannot be sent out; returns 1 if it can be.
int BattlePartyMenu_CanSwitchToSelectedMon(BattlePartyMenu *menu) {
    BattlePartyContext *context;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    if (BattlePartyMenu_IsPartnerMon(menu, menu->context->selectedMon) == 1) {
        String *template = NewString_ReadMsgData(menu->msgData, 80);
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
        ReadMsgDataIntoString(menu->msgData, 79, menu->string);
        return 0;
    }
    context = menu->context;
    if (context->reservedMon != 6) {
        u8 slot = context->selectedMon;
        if (context->reservedMon == context->partyOrder[slot]) {
            String *template = NewString_ReadMsgData(menu->msgData, 93);
            BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(menu->mons[slot].mon));
            StringExpandPlaceholders(menu->msgFormat, menu->string, template);
            String_Delete(template);
            return 0;
        }
    }
    if (context->move) {
        u8 slot = menu->activeMon;
        BattlePartyMenuMonRef active = { slot * sizeof(BattlePartyMenuMon), &menu->mons };
        String *template = NewString_ReadMsgData(menu->msgData, 78);
        BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(*(Pokemon **)((u8 *)active.mons + active.offset)));
        StringExpandPlaceholders(menu->msgFormat, menu->string, template);
        String_Delete(template);
        return 0;
    }
    return 1;
}

#pragma opt_propagation reset
#pragma force_active reset

u8 BattlePartyMenu_IsSelectedMonEgg(BattlePartyMenu *menu) {
    u8 slot = menu->context->selectedMon;

    if (menu->mons[slot].isEgg != 0) {
        return 1;
    }
    return 0;
}

// Multi battles alongside an AI partner are not treated as double or multi battles here.
#define BATTLE_TYPE_AI_MULTI (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_MULTI | BATTLE_TYPE_AI)

#pragma force_active on

BOOL BattlePartyMenu_IsDoubleBattle(BattlePartyMenu *menu) {
    u32 battleType = BattleSystem_GetBattleType(menu->context->battleSystem);

    if (battleType != BATTLE_TYPE_AI_MULTI && battleType != (BATTLE_TYPE_TRAINER | BATTLE_TYPE_AI_MULTI) && (battleType & (BATTLE_TYPE_DOUBLES | BATTLE_TYPE_TAG)) != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL BattlePartyMenu_IsMultiBattle(BattlePartyMenu *menu) {
    u32 battleType = BattleSystem_GetBattleType(menu->context->battleSystem);

    if (battleType != BATTLE_TYPE_AI_MULTI && battleType != (BATTLE_TYPE_TRAINER | BATTLE_TYPE_AI_MULTI) && (battleType & BATTLE_TYPE_MULTI) != 0) {
        return TRUE;
    }
    return FALSE;
}

// In a multi battle, the slots of the partner trainer's mons: the odd slots, or the even ones for battler
// position 2.
int BattlePartyMenu_IsPartnerMon(BattlePartyMenu *menu, u32 slot) {
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

BOOL BattlePartyMenu_IsSelectedMoveHM(BattlePartyMenu *menu) {
    u16 move;

    if (menu->context->selectedMove == 4) {
        move = menu->context->move;
    } else {
        move = menu->mons[menu->context->selectedMon].moves[menu->context->selectedMove].move;
    }
    return MoveIsHM(move);
}

#pragma force_active reset

void BattlePartyMenu_ClearBattleMoveInfo(BattlePartyMenu *menu) {
    ManagedSprite_SetDrawFlag(menu->sprites[26], FALSE);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[10]);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[6]);
    ClearWindowTilemapAndScheduleTransfer(&menu->pageWindows[7]);
}

#pragma force_active on

void BattlePartyMenu_ClearContestMoveInfo(BattlePartyMenu *menu) {
    ov08_0221D81C(menu);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 7);
}

#pragma force_active reset

void BattlePartyMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID) {
    if (item != ITEM_BLUE_FLUTE && item != ITEM_RED_FLUTE && item != ITEM_YELLOW_FLUTE) {
        Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, heapID);
    }
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}

void BattlePartyMenu_AddWindows(BattlePartyMenu *menu) {
    u32 i;

    for (i = 0; i < 2; i++) {
        AddWindow(menu->bgConfig, &menu->windows[i], &sBattlePartyWindowTemplates[i]);
    }
    BattlePartyMenu_AddPageWindows(menu, menu->page);
}

void BattlePartyMenu_AddPageWindows(BattlePartyMenu *menu, int page) {
    const WindowTemplate *templates;
    u8 i;

    switch (page) {
    case 0:
        templates = sBattlePartyPartyListWindows;
        menu->pageWindowCount = 6;
        break;
    case 1:
        templates = sBattlePartyMonMenuWindows;
        menu->pageWindowCount = 4;
        break;
    case 2:
        templates = sBattlePartySummaryWindows;
        menu->pageWindowCount = 35;
        break;
    case 3:
        templates = sBattlePartyCheckMovesWindows;
        menu->pageWindowCount = 11;
        break;
    case 4:
        templates = sBattlePartyMoveDetailWindows;
        menu->pageWindowCount = 17;
        break;
    case 5:
        templates = sBattlePartyRestorePPWindows;
        menu->pageWindowCount = 5;
        break;
    case 6:
    case 8:
        templates = sBattlePartyLearnMoveWindows;
        menu->pageWindowCount = 6;
        break;
    case 7:
        templates = sBattlePartyForgetMoveWindows;
        menu->pageWindowCount = 12;
        break;
    case 9:
        templates = sBattlePartyContestForgetMoveWindows;
        menu->pageWindowCount = 7;
        break;
    }
    menu->pageWindows = AllocWindows(menu->context->heapID, menu->pageWindowCount);
    for (i = 0; i < menu->pageWindowCount; i++) {
        AddWindow(menu->bgConfig, &menu->pageWindows[i], &templates[i]);
    }
}

#pragma force_active on

void BattlePartyMenu_RemovePageWindows(BattlePartyMenu *menu) {
    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
}

void BattlePartyMenu_RemoveWindows(BattlePartyMenu *menu) {
    u32 i;

    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
    for (i = 0; i < 2; i++) {
        RemoveWindow(&menu->windows[i]);
    }
}

void BattlePartyMenu_PrintPageText(BattlePartyMenu *menu, int page) {
    switch (page) {
    case 0:
        BattlePartyMenu_PrintPartyListPage(menu);
        break;
    case 1:
        BattlePartyMenu_PrintMonMenuPage(menu);
        break;
    case 2:
        BattlePartyMenu_PrintSummaryPage(menu);
        break;
    case 3:
        BattlePartyMenu_PrintCheckMovesPage(menu);
        break;
    case 4:
        BattlePartyMenu_PrintMoveDetailPage(menu);
        break;
    case 5:
        BattlePartyMenu_PrintRestorePPPage(menu);
        break;
    case 6:
    case 8:
        BattlePartyMenu_PrintLearnMovePage(menu);
        break;
    case 7:
        BattlePartyMenu_PrintForgetMovePage(menu);
        break;
    case 9:
        BattlePartyMenu_PrintContestForgetMovePage(menu);
        break;
    }
}

// The printers below pass a variable font id on as 32 bits, without the FontID narrowing of the text.h prototype.
typedef u8 (*AddTextPrinterWithColorFunc)(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

void BattlePartyMenu_PrintMonName(BattlePartyMenu *menu, int window, int fontId, u8 slot, u8 x, u8 y) {
    BattlePartyMenuMon *mon;
    String *name;
    String *template;
    Window *windows;
    Window *win;

    windows = menu->pageWindows;
    win = &windows[window];
    mon = &menu->mons[slot];
    name = String_New(12, menu->context->heapID);
    template = NewString_ReadMsgData(menu->msgData, sBattlePartyMonNameMsgIds[slot]);
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
            String *symbol = NewString_ReadMsgData(menu->msgData, 16);
            u32 width = GetWindowWidth(win);
            int symbolX = width * 8 - FontID_String_GetWidth(0, symbol, 0);
            if (fontId == 0) {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), NULL);
            } else {
                AddTextPrinterParameterizedWithColor(win, 0, symbol, symbolX, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(10, 11, 0), NULL);
            }
            String_Delete(symbol);
        } else if (mon->gender == MON_FEMALE) {
            String *symbol = NewString_ReadMsgData(menu->msgData, 17);
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

#pragma force_active reset

void BattlePartyMenu_PrintMonLevel(BattlePartyMenu *menu, int window, int slot, int x, u8 y) {
    int offset = window << 4;
    u32 level = menu->mons[slot].level;

    sub_0200CE7C(menu->msgPrinter, 1, level, 3, PRINTING_MODE_LEFT_ALIGN, (Window *)((u8 *)menu->pageWindows + offset), x + 8, y);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + offset));
}

#pragma force_active on

void BattlePartyMenu_PrintMonHP(BattlePartyMenu *menu, int window, int slot, u32 x, u8 y) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u32 offset = window * sizeof(Window);

    PrintUIntOnWindow(menu->msgPrinter, mon->hp, 3, PRINTING_MODE_RIGHT_ALIGN, (Window *)((u8 *)menu->pageWindows + offset), x, y);
    sub_0200CDAC(menu->msgPrinter, 0, (Window *)((u8 *)menu->pageWindows + offset), x + 24, y);
    x += 32;
    PrintUIntOnWindow(menu->msgPrinter, mon->maxHp, 3, PRINTING_MODE_LEFT_ALIGN, (Window *)((u8 *)menu->pageWindows + offset), x, y);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + offset));
}

// `y` is volatile: the bar's row offsets are computed from a reloaded copy.
void BattlePartyMenu_DrawHPBar(BattlePartyMenu *menu, int window, int slot, u16 x, volatile u8 y) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u8 color = 1;
    u8 width = CalculateHpBarPixelsLength(mon->hp, mon->maxHp, 48);
    int shadow;

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
    {
        u8 top = y;
        shadow = color + 1;
        FillWindowPixelRect(&menu->pageWindows[window], (u8)shadow, x, (u16)(top + 1), width, 1);
        FillWindowPixelRect(&menu->pageWindows[window], color, x, (u16)(top + 2), width, 2);
        FillWindowPixelRect(&menu->pageWindows[window], (u8)shadow, x, (u16)(top + 4), width, 1);
        ScheduleWindowCopyToVram(&menu->pageWindows[window]);
    }
}

#pragma force_active reset

void BattlePartyMenu_PrintAbilityName(BattlePartyMenu *menu, int window, int slot) {
    String *string = String_New(16, menu->context->heapID);
    String *template = NewString_ReadMsgData(menu->msgData, 8);

    BufferAbilityName(menu->msgFormat, 0, menu->mons[slot].ability);
    StringExpandPlaceholders(menu->msgFormat, string, template);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(template);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

void BattlePartyMenu_PrintHeldItemName(BattlePartyMenu *menu, int window, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    String *string;

    if (mon->heldItem == ITEM_NONE) {
        string = NewString_ReadMsgData(menu->msgData, 20);
    } else {
        String *template;
        string = String_New(18, menu->context->heapID);
        template = NewString_ReadMsgData(menu->msgData, 9);
        BufferItemName(menu->msgFormat, 0, mon->heldItem);
        StringExpandPlaceholders(menu->msgFormat, string, template);
        String_Delete(template);
    }
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

void BattlePartyMenu_PrintMoveName(BattlePartyMenu *menu, int move, int window, int msgNo, u16 fontId, u16 y, u32 color) {
    Window *windows = menu->pageWindows;
    int offset = window << 4;
    Window *win = (Window *)((u8 *)windows + offset);
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

#pragma force_active on

void BattlePartyMenu_PrintPPLabel(BattlePartyMenu *menu, int window, u16 x, u32 y) {
    String *string = NewString_ReadMsgData(menu->msgData, 14);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[window], 0, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[window]);
}

void BattlePartyMenu_PrintPrompt(BattlePartyMenu *menu, int msgNo) {
    String *string;

    DrawFrameAndWindow2(&menu->windows[0], TRUE, 1, 14);
    FillWindowPixelBuffer(&menu->windows[0], 15);
    string = NewString_ReadMsgData(menu->msgData, msgNo);
    AddTextPrinterParameterizedWithColor(&menu->windows[0], 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->windows[0]);
}

void BattlePartyMenu_PrintButtonLabel(BattlePartyMenu *menu, int window, int msgNo) {
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
void BattlePartyMenu_PrintLevelAndNextExp(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u16 base = menu->windowSet * 22;
    String *string = NewString_ReadMsgData(menu->msgData, 23);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[19], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, 24);
    String *expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    int offset = (base + 11) << 4;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, expanded, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);

    string = NewString_ReadMsgData(menu->msgData, 25);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[20], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, 26);
    expanded = String_New(14, menu->context->heapID);
    if (mon->level < 100) {
        BufferIntegerAsString(menu->msgFormat, 0, mon->nextLevelExp - mon->exp, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    } else {
        BufferIntegerAsString(menu->msgFormat, 0, 0, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    }
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    offset = (base + 12) << 4;
    u32 width = GetWindowWidth((Window *)((u8 *)menu->pageWindows + offset));
    u16 x = width * 8 - FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, expanded, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(&menu->pageWindows[19]);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + ((base + 11) << 4)));
    ScheduleWindowCopyToVram(&menu->pageWindows[20]);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + offset));
}

#define DEFINE_PRINT_STAT(name, labelWindow, labelMsgNo, valueMsgNo, field, valueWindow)                                                                              \
    void name(BattlePartyMenu *menu, int slot) {                                                                                                                      \
        u16 base = menu->windowSet * 22;                                                                                                                              \
        String *string = NewString_ReadMsgData(menu->msgData, labelMsgNo);                                                                                            \
        AddTextPrinterParameterizedWithColor(&menu->pageWindows[labelWindow], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);              \
        String_Delete(string);                                                                                                                                        \
        String *template = NewString_ReadMsgData(menu->msgData, valueMsgNo);                                                                                          \
        String *expanded = String_New(8, menu->context->heapID);                                                                                                      \
        u16 value = menu->mons[slot].field;                                                                                                                           \
        BufferIntegerAsString(menu->msgFormat, 0, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);                                                                          \
        StringExpandPlaceholders(menu->msgFormat, expanded, template);                                                                                                \
        u8 width = FontID_String_GetWidth(0, expanded, 0);                                                                                                            \
        int offset = (base + valueWindow) << 4;                                                                                                                       \
        u8 x = GetWindowWidth((Window *)((u8 *)menu->pageWindows + offset)) * 8 - width;                                                                              \
        AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, expanded, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL); \
        String_Delete(template);                                                                                                                                      \
        String_Delete(expanded);                                                                                                                                      \
        ScheduleWindowCopyToVram(&menu->pageWindows[labelWindow]);                                                                                                    \
        ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + offset));                                                                                       \
    }

DEFINE_PRINT_STAT(BattlePartyMenu_PrintAttack, 14, 32, 33, attack, 5)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintDefense, 15, 34, 35, defense, 6)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpeed, 16, 40, 41, speed, 7)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpAttack, 17, 36, 37, spAttack, 8)
DEFINE_PRINT_STAT(BattlePartyMenu_PrintSpDefense, 18, 38, 39, spDefense, 9)

void BattlePartyMenu_PrintHPStat(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuMon *mon = &menu->mons[slot];
    u16 base = menu->windowSet * 22;
    String *string = NewString_ReadMsgData(menu->msgData, 28);

    AddTextPrinterParameterizedWithColor(&menu->pageWindows[13], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, 31);
    u32 slashWidth = FontID_String_GetWidth(0, string, 0);
    u32 windowWidth = GetWindowWidth(&menu->pageWindows[4]);
    u16 middle = (windowWidth * 8 - slashWidth) / 2;
    int offset = (base + 4) << 4;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, string, middle, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);

    string = NewString_ReadMsgData(menu->msgData, 29);
    String *expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->hp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    u32 width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, expanded, middle - width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);

    string = NewString_ReadMsgData(menu->msgData, 30);
    expanded = String_New(8, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, mon->maxHp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + offset), 0, expanded, middle + slashWidth, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(&menu->pageWindows[13]);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + offset));
}

void BattlePartyMenu_PrintAbilityDescription(BattlePartyMenu *menu, int slot) {
    int window = menu->windowSet * 22;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 722, menu->context->heapID);
    u16 msgNo = menu->mons[slot].ability;
    String *string = NewString_ReadMsgData(msgData, msgNo);

    window = (window + 2) << 4;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)menu->pageWindows + window), 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + window));
}

void BattlePartyMenu_PrintAccuracyLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, 51);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintMoveAccuracy(BattlePartyMenu *menu, int window, int accuracy) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];

    if (accuracy == 0) {
        String *string = NewString_ReadMsgData(menu->msgData, 50);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
    } else {
        String *template = NewString_ReadMsgData(menu->msgData, 52);
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

void BattlePartyMenu_PrintPowerLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, 48);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintMovePower(BattlePartyMenu *menu, int window, u32 power) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];

    if (power <= 1) {
        String *string = NewString_ReadMsgData(menu->msgData, 50);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(win) * 8 - width;
        AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
    } else {
        String *template = NewString_ReadMsgData(menu->msgData, 49);
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

void BattlePartyMenu_PrintMoveDescription(BattlePartyMenu *menu, int window, int move) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 749, menu->context->heapID);
    String *string = NewString_ReadMsgData(msgData, move);

    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintCategoryLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, 53);
    u16 width = FontID_String_GetWidth(0, string, 0);
    u16 x = (GetWindowWidth(win) * 8 - width) / 2;

    AddTextPrinterParameterizedWithColor(win, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintMoveCategory(BattlePartyMenu *menu, int window, int moveClass) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string;

    switch (moveClass) {
    case 0:
        string = NewString_ReadMsgData(menu->msgData, 54);
        break;
    case 1:
        string = NewString_ReadMsgData(menu->msgData, 56);
        break;
    case 2:
        string = NewString_ReadMsgData(menu->msgData, 55);
        break;
    }
    AddTextPrinterParameterizedWithColor(win, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintMovePP(BattlePartyMenu *menu, int window, int pp, int maxPP) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, 46);
    u32 slashWidth = FontID_String_GetWidth(0, string, 0);
    u32 middle = (GetWindowWidth(win) * 8 - slashWidth) / 2;

    AddTextPrinterParameterizedWithColor(win, 0, string, middle, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, 44);
    String *expanded = String_New(6, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, pp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    u32 width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, middle - width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    string = NewString_ReadMsgData(menu->msgData, 45);
    expanded = String_New(6, menu->context->heapID);
    BufferIntegerAsString(menu->msgFormat, 0, maxPP, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, middle + slashWidth, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintForgetButtonLabel(BattlePartyMenu *menu, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string;
    u32 width;

    if (menu->context->selectedMove == 4) {
        string = NewString_ReadMsgData(menu->msgData, 59);
    } else {
        string = NewString_ReadMsgData(menu->msgData, 58);
    }
    width = FontID_String_GetWidth(4, string, 0);
    AddTextPrinterParameterizedWithColor(win, 4, string, (96 - width) / 2, 5, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 9), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_PrintHMCantBeForgotten(BattlePartyMenu *menu) {
    Window *window;
    String *string;

    if (menu->page == 7) {
        window = &menu->pageWindows[8];
    } else {
        window = &menu->pageWindows[5];
    }
    FillWindowPixelBuffer(window, 0);
    string = NewString_ReadMsgData(menu->msgData, 60);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void BattlePartyMenu_PrintMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *expanded = String_New(6, menu->context->heapID);
    String *string = NewString_ReadMsgData(menu->msgData, 43);

    AddTextPrinterParameterizedWithColor(win, 0, string, 40, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, 46);
    u32 width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(win, 0, string, 80, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, 45);
    BufferIntegerAsString(menu->msgFormat, 0, move->maxPP, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width += 80;
    AddTextPrinterParameterizedWithColor(win, 0, expanded, width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(menu->msgData, 44);
    BufferIntegerAsString(menu->msgFormat, 0, move->pp, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, 80 - width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

void BattlePartyMenu_UpdateMoveListPP(BattlePartyMenu *menu, BattlePartyMenuMove *move, int window) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *expanded = String_New(6, menu->context->heapID);
    String *string = NewString_ReadMsgData(menu->msgData, 43);
    u32 width = FontID_String_GetWidth(0, string, 0);

    String_Delete(string);
    width += 40;
    FillWindowPixelRect(win, 0, (u16)width, 24, (u16)(80 - width), 16);
    string = NewString_ReadMsgData(menu->msgData, 44);
    BufferIntegerAsString(menu->msgFormat, 0, move->pp, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, expanded, string);
    width = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(win, 0, expanded, 80 - width, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(win);
}

// The party list passes the slot to BattlePartyMenu_PrintMonName as a u16, without the u8 narrowing.
typedef void (*PrintMonNameFunc)(BattlePartyMenu *menu, int window, int fontId, int slot, int x, int y);

void BattlePartyMenu_PrintPartyListPage(BattlePartyMenu *menu) {
    int i = 0;

    if (Party_GetCount(menu->context->party) > 0) {
        u32 windowOffset = 0;
        do {
            FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + windowOffset), 0);
            if (menu->mons[i].species != SPECIES_NONE) {
                ((PrintMonNameFunc)BattlePartyMenu_PrintMonName)(menu, i, 4, (u16)i, 32, 7);
                if (menu->mons[i].isEgg == FALSE) {
                    BattlePartyMenu_PrintMonHPAndBar(menu, (u8)i);
                }
                if (Pokemon_GetStatusIconId(menu->mons[i].mon) == 7) {
                    BattlePartyMenu_PrintPartyMonLevel(menu, (u8)i);
                }
            }
            windowOffset += sizeof(Window);
            i++;
        } while (i < Party_GetCount(menu->context->party));
    }
    if (menu->context->mode == 2) {
        BattlePartyMenu_PrintPrompt(menu, 7);
    } else {
        BattlePartyMenu_PrintPrompt(menu, 6);
    }
}

void BattlePartyMenu_PrintMonHPAndBar(BattlePartyMenu *menu, int slot) {
    FillWindowPixelRect(&menu->pageWindows[slot], 0, 56, 32, 24, 8);
    FillWindowPixelRect(&menu->pageWindows[slot], 0, 64, 24, 64, 8);
    BattlePartyMenu_PrintMonHP(menu, slot, slot, 56, 32);
    BattlePartyMenu_DrawHPBar(menu, slot, slot, 64, 24);
}

void BattlePartyMenu_PrintPartyMonLevel(BattlePartyMenu *menu, int slot) {
    if (menu->mons[slot].isEgg == FALSE) {
        BattlePartyMenu_PrintMonLevel(menu, slot, slot, 0, 32);
    }
}

#pragma force_active reset

void BattlePartyMenu_PrintMonMenuPage(BattlePartyMenu *menu) {
    FillWindowPixelBuffer(menu->pageWindows, 0);
    FillWindowPixelBuffer(menu->pageWindows + 1, 0);
    FillWindowPixelBuffer(menu->pageWindows + 2, 0);
    FillWindowPixelBuffer(menu->pageWindows + 3, 0);
    BattlePartyMenu_PrintMonNameCentered(menu, menu->context->selectedMon);
    BattlePartyMenu_PrintButtonLabel(menu, 1, 15);
    if (!menu->mons[menu->context->selectedMon].isEgg) {
        BattlePartyMenu_PrintButtonLabel(menu, 2, 18);
        BattlePartyMenu_PrintButtonLabel(menu, 3, 19);
        return;
    }
    ScheduleWindowCopyToVram(menu->pageWindows + 2);
    ScheduleWindowCopyToVram(menu->pageWindows + 3);
}

#pragma force_active on

// Locals kept in memory: the name is printed through reloaded copies.
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
                symbol = NewString_ReadMsgData(menu->msgData, 16);
            } else if (mon->gender == MON_FEMALE) {
                symbol = NewString_ReadMsgData(menu->msgData, 17);
            }
        }
    }
    return symbol;
}

void BattlePartyMenu_PrintMonNameCentered(BattlePartyMenu *menu, int slot) {
    BattlePartyMenuNameLocals locals;
    locals.window = menu->pageWindows;
    locals.mon = &menu->mons[slot];
    locals.name = String_New(12, menu->context->heapID);
    String *template = NewString_ReadMsgData(menu->msgData, sBattlePartyMonNameMsgIds[slot]);
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
void BattlePartyMenu_PrintCheckMovesPage(BattlePartyMenu *menu) {
    u16 base = menu->windowSet * 6, i;
    int window1, window2, window3, window4;

    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + base * 16), 0);
    window1 = (base + 1) * 16;
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + window1), 0);
    window2 = (base + 2) * 16;
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + window2), 0);
    window3 = (base + 3) * 16;
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + window3), 0);
    window4 = (base + 4) * 16;
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + window4), 0);
    FillWindowPixelBuffer(&menu->pageWindows[5], 0);
    BattlePartyMenu_PrintMonName(menu, base, 0, menu->context->selectedMon, 0, 0);
    for (i = 0; i < 4; i++) {
        BattlePartyMenuMove *move = &menu->mons[menu->context->selectedMon].moves[i];
        if (move->move) {
            BattlePartyMenu_PrintMoveName(menu, move->move, base + 1 + i, sBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, move, base + 1 + i);
        }
    }
    BattlePartyMenu_PrintButtonLabel(menu, 5, 18);
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + window1));
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + window2));
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + window3));
    ScheduleWindowCopyToVram((Window *)((u8 *)menu->pageWindows + window4));
    menu->windowSet ^= 1;
}

#pragma force_active reset

void BattlePartyMenu_PrintSummaryPage(BattlePartyMenu *menu) {
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
    BattlePartyMenu_PrintButtonLabel(menu, 21, 19);
    menu->windowSet ^= 1;
}

#pragma force_active on
#pragma opt_common_subs off

// The move detail page has two sets of 11 windows (`windowSet`); windows 6-10 are shared.
void BattlePartyMenu_PrintMoveDetailPage(BattlePartyMenu *menu) {
    int base = menu->windowSet * 11;
    BattlePartyMenuMove *move;
    int slot;

    FillWindowPixelBuffer(&menu->pageWindows[6], 0);
    FillWindowPixelBuffer(&menu->pageWindows[7], 0);
    FillWindowPixelBuffer(&menu->pageWindows[8], 0);
    FillWindowPixelBuffer(&menu->pageWindows[9], 0);
    FillWindowPixelBuffer(&menu->pageWindows[10], 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + (base + 1) * 16), 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + base * 16), 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + (base + 2) * 16), 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + (base + 3) * 16), 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + (base + 5) * 16), 0);
    FillWindowPixelBuffer((Window *)((u8 *)menu->pageWindows + (base + 4) * 16), 0);
    slot = menu->context->selectedMon;
    move = &menu->mons[slot].moves[menu->context->selectedMove];
    BattlePartyMenu_PrintMonName(menu, 6, 0, slot, 0, 0);
    BattlePartyMenu_PrintPPLabel(menu, 7, 0, 0);
    BattlePartyMenu_PrintMoveName(menu, move->move, base, sBattlePartyMoveNameMsgIds[menu->context->selectedMove], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
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

#pragma opt_common_subs reset

void BattlePartyMenu_PrintLearnMovePage(BattlePartyMenu *menu) {
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
            BattlePartyMenu_PrintMoveName(menu, moves[i].move, i + 1, sBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, &moves[i], i + 1);
        }
    }
    BattlePartyMenu_PrintMoveName(menu, menu->context->move, 5, 73, 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
    newMove.pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
    newMove.maxPP = newMove.pp;
    BattlePartyMenu_PrintMoveListPP(menu, &newMove, 5);
    ScheduleWindowCopyToVram(&menu->pageWindows[1]);
    ScheduleWindowCopyToVram(&menu->pageWindows[2]);
    ScheduleWindowCopyToVram(&menu->pageWindows[3]);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
    ScheduleWindowCopyToVram(&menu->pageWindows[5]);
}

void BattlePartyMenu_PrintForgetMovePage(BattlePartyMenu *menu) {
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
        BattlePartyMenu_PrintMoveName(menu, move->move, 1, sBattlePartyMoveNameMsgIds[moveSlot], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMoveAccuracy(menu, 6, move->accuracy);
        BattlePartyMenu_PrintMovePower(menu, 7, move->power);
        BattlePartyMenu_PrintMoveDescription(menu, 8, move->move);
        BattlePartyMenu_PrintMoveCategory(menu, 10, move->moveClass);
        BattlePartyMenu_PrintMovePP(menu, 3, move->pp, move->maxPP);
    } else {
        pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
        BattlePartyMenu_PrintMoveName(menu, menu->context->move, 1, 73, 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMoveDescription(menu, 8, menu->context->move);
        BattlePartyMenu_PrintMoveAccuracy(menu, 6, GetMoveAttr(menu->context->move, MOVEATTR_ACCURACY));
        BattlePartyMenu_PrintMovePower(menu, 7, GetMoveAttr(menu->context->move, MOVEATTR_POWER));
        BattlePartyMenu_PrintMoveCategory(menu, 10, GetMoveAttr(menu->context->move, MOVEATTR_CLASS));
        BattlePartyMenu_PrintMovePP(menu, 3, pp, pp);
    }
    BattlePartyMenu_PrintForgetButtonLabel(menu, 11);
}

void BattlePartyMenu_PrintRestorePPPage(BattlePartyMenu *menu) {
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
            BattlePartyMenu_PrintMoveName(menu, moves[i].move, i + 1, sBattlePartyMoveNameMsgIds[i], 4, 7, MAKE_TEXT_COLOR(7, 8, 9));
            BattlePartyMenu_PrintMoveListPP(menu, &moves[i], i + 1);
        }
    }
    if (GetItemAttr(menu->context->item, ITEMATTR_PP_RESTORE_ALL, menu->context->heapID) == 0) {
        BattlePartyMenu_PrintPrompt(menu, 94);
    }
    ScheduleWindowCopyToVram(&menu->pageWindows[1]);
    ScheduleWindowCopyToVram(&menu->pageWindows[2]);
    ScheduleWindowCopyToVram(&menu->pageWindows[3]);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
}

void BattlePartyMenu_UpdateMovePP(BattlePartyMenu *menu, int window, int moveSlot) {
    u8 slot = menu->context->selectedMon;
    BattlePartyMenu_UpdateMoveListPP(menu, &menu->mons[slot].moves[moveSlot], window);
}

void BattlePartyMenu_PrintContestForgetMovePage(BattlePartyMenu *menu) {
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
    string = NewString_ReadMsgData(menu->msgData, 57);
    AddTextPrinterParameterizedWithColor(&menu->pageWindows[4], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&menu->pageWindows[4]);
    if (menu->context->selectedMove < 4) {
        u32 moveSlot = menu->context->selectedMove;
        moves = menu->mons[menu->context->selectedMon].moves;
        move = &moves[moveSlot];
        BattlePartyMenu_PrintMoveName(menu, move->move, 1, sBattlePartyMoveNameMsgIds[moveSlot], 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMovePP(menu, 3, move->pp, move->maxPP);
    } else {
        pp = GetMoveAttr(menu->context->move, MOVEATTR_PP);
        BattlePartyMenu_PrintMoveName(menu, menu->context->move, 1, 73, 0, 0, MAKE_TEXT_COLOR(15, 14, 0));
        BattlePartyMenu_PrintMovePP(menu, 3, pp, pp);
    }
    BattlePartyMenu_PrintForgetButtonLabel(menu, 6);
}

void BattlePartyMenu_ShowMessage(BattlePartyMenu *menu) {
    DrawFrameAndWindow2(&menu->windows[1], TRUE, 1, 14);
    FillWindowPixelBuffer(&menu->windows[1], 15);
    BattlePartyMenu_PrintMessage(menu);
}

void BattlePartyMenu_PrintMessage(BattlePartyMenu *menu) {
    int delay;

    TextFlags_SetCanABSpeedUpPrint(TRUE);
    delay = BattleSystem_GetTextFrameDelay(menu->context->battleSystem);
    menu->textPrinterId = AddTextPrinterParameterized(&menu->windows[1], 1, menu->string, 0, 0, delay, NULL);
}

void BattlePartyMenu_BufferItemResultMessage(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    Pokemon *mon;
    void *itemData;
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
        BattlePartyMenu_BufferMonMessage(menu, mon, 88);
    } else if (hp != prevHp) {
        String *template = NewString_ReadMsgData(menu->msgData, 82);
        BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
        BufferIntegerAsString(menu->msgFormat, 1, hp - menu->mons[context->selectedMon].hp, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(menu->msgFormat, menu->string, template);
        String_Delete(template);
    } else if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PP_RESTORE) || GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PP_RESTORE_ALL)) {
        ReadMsgDataIntoString(menu->msgData, 87, menu->string);
    } else if (healed == 1) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 92);
    } else if (healed == 2) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 83);
    } else if (healed == 4) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 85);
    } else if (healed == 8) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 86);
    } else if (healed == 0x10) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 84);
    } else if (healed == 0x20) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 90);
    } else if (healed == 0x40) {
        BattlePartyMenu_BufferMonMessage(menu, mon, 91);
    } else {
        BattlePartyMenu_BufferMonMessage(menu, mon, 89);
    }
    Heap_Free(itemData);
}

void BattlePartyMenu_BufferEmbargoMessage(BattlePartyMenu *menu) {
    BattlePartyContext *context = menu->context;
    u8 *partyOrder = &context->partyOrder[context->selectedMon];
    Pokemon *mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, *partyOrder);
    String *template = NewString_ReadMsgData(menu->msgData, 95);

    BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
    BufferMoveName(menu->msgFormat, 1, MOVE_EMBARGO);
    StringExpandPlaceholders(menu->msgFormat, menu->string, template);
    String_Delete(template);
}

void BattlePartyMenu_InitSprites(BattlePartyMenu *menu) {
    BattlePartyMenu_CreateSpriteManager(menu);
    BattlePartyMenu_LoadMonIconResources(menu);
    BattlePartyMenu_LoadStatusIconResources(menu);
    BattlePartyMenu_LoadTypeIconResources(menu);
    BattlePartyMenu_LoadHeldItemIconResources(menu);
    ov08_02220928(menu);
    BattlePartyMenu_CreateSprites(menu);
    BattlePartyMenu_CreateCursor(menu);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void BattlePartyMenu_CreateSpriteManager(BattlePartyMenu *menu) {
    SpriteResourceCountsListUnion capacities = sBattlePartySpriteCapacities;
    SpriteSystem *spriteSystem;

    spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    menu->spriteManager = SpriteManager_New(spriteSystem);
    SpriteSystem_InitSprites(spriteSystem, menu->spriteManager, 43);
    SpriteSystem_InitManagerWithCapacities(spriteSystem, menu->spriteManager, &capacities);
}

void BattlePartyMenu_LoadMonIconResources(BattlePartyMenu *menu) {
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, menu->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    u32 i;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, sub_02074490(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_02074498(), FALSE, 0xB007);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_020744A4(), FALSE, 0xB007);
    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, Pokemon_GetIconNaix(menu->mons[i].mon), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007 + i);
        } else {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetMonIconNaixEx(SPECIES_NONE, FALSE, 0), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB007 + i);
        }
    }
    NARC_Delete(narc);
}

void BattlePartyMenu_LoadStatusIconResources(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NARC *narc = NARC_New(NARC_a_0_3_9, menu->context->heapID);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, sub_0208AD58(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB008);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0208AD5C(), FALSE, 0xB008);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0208AD60(), FALSE, 0xB008);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0208AD54(), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00D);
    NARC_Delete(narc);
}

void BattlePartyMenu_LoadTypeIconResources(BattlePartyMenu *menu) {
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

void BattlePartyMenu_LoadHeldItemIconResources(BattlePartyMenu *menu) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, menu->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, sub_0207CAA0(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00A);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0207CAA4(), FALSE, 0xB00A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0207CAA8(), FALSE, 0xB00A);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, sub_0207CA9C(), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB016);
    NARC_Delete(narc);
}

// resources 0xB00B / 0xB017 from NARC a/0/7/1 (probably the contest condition markers)
void ov08_02220928(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    SpriteSystem_LoadPaletteBuffer(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, NARC_a_0_7_1, 27, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xB00B);
    SpriteSystem_LoadCellResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 25, FALSE, 0xB00B);
    SpriteSystem_LoadAnimResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 24, FALSE, 0xB00B);
    SpriteSystem_LoadCharResObj(spriteSystem, menu->spriteManager, NARC_a_0_7_1, 26, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xB017);
}

ManagedSprite *BattlePartyMenu_CreateSprite(BattlePartyMenu *menu, int index) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = sBattlePartySpriteTemplatesPriority[index][0];
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = sBattlePartySpriteTemplates[index][0];
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = sBattlePartySpriteTemplatesPltt[index][0];
    template.resIdList[GF_GFX_RES_TYPE_CELL] = sBattlePartySpriteTemplatesCell[index][0];
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = sBattlePartySpriteTemplatesAnim[index][0];
    template.bgPriority = 1;
    template.vramTransfer = FALSE;
    return SpriteSystem_NewSprite(spriteSystem, menu->spriteManager, &template);
}

void BattlePartyMenu_CreateSprites(BattlePartyMenu *menu) {
    u32 i;

    for (i = 0; i < 38; i++) {
        menu->sprites[i] = BattlePartyMenu_CreateSprite(menu, i);
    }
    BattlePartyMenu_SetMonIconPalettes(menu);
}

void BattlePartyMenu_FreeSprites(BattlePartyMenu *menu) {
    u32 i;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    for (i = 0; i < 38; i++) {
        Sprite_DeleteAndFreeResources(menu->sprites[i]);
    }
    BattlePartyMenu_FreeCursor(menu);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, menu->spriteManager);
}

void BattlePartyMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, x, y);
}

#pragma force_active reset

void BattlePartyMenu_SetMonIconPalettes(BattlePartyMenu *menu) {
    int i;

    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            ManagedSprite_SetPaletteOverride(menu->sprites[7 + i], GetMonIconPaletteEx(menu->mons[i].species, menu->mons[i].form, menu->mons[i].isEgg));
        }
    }
}

void BattlePartyMenu_SetTypeIcon(BattlePartyMenu *menu, ManagedSprite *sprite, int resId, u32 type) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NarcId narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetCharFile(type);

    SpriteSystem_ReplaceCharResObj(spriteSystem, menu->spriteManager, narc, file, TRUE, resId);
    ManagedSprite_SetPaletteOverride(sprite, TypeIcon_GetPalette(type) + 4);
}

void BattlePartyMenu_SetMoveCategoryIcon(BattlePartyMenu *menu, ManagedSprite *sprite, u32 moveClass) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    NarcId narc = MoveCategoryIcon_GetNarc();
    int file = MoveCategoryIcon_GetCharFile(moveClass);

    SpriteSystem_ReplaceCharResObj(spriteSystem, menu->spriteManager, narc, file, TRUE, 0xB015);
    ManagedSprite_SetPaletteOverride(sprite, MoveCategoryIcon_GetPalette(moveClass) + 4);
}

void BattlePartyMenu_ShowStatusIcon(int status, ManagedSprite *sprite, int x, int y) {
    if (status != 7) {
        ManagedSprite_SetAnim(sprite, status);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

void BattlePartyMenu_ShowMonTypeIcons(BattlePartyMenu *menu, BattlePartyMenuMon *mon, const BattlePartyMenuPos *pos) {
    BattlePartyMenu_SetTypeIcon(menu, menu->sprites[19], 0xB00E, mon->type1);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[19], pos[0].x, pos[0].y);
    if (mon->type1 != mon->type2) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[20], 0xB00F, mon->type2);
        BattlePartyMenu_ShowSpriteAt(menu->sprites[20], pos[1].x, pos[1].y);
    }
}

void BattlePartyMenu_ShowHeldItemIcon(u16 item, ManagedSprite *sprite, int x, int y) {
    if (item != ITEM_NONE) {
        if (ItemIdIsMail(item) == TRUE) {
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            ManagedSprite_SetAnim(sprite, 0);
        }
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

#pragma force_active on

void BattlePartyMenu_ShowCapsuleIcon(BOOL hasCapsule, ManagedSprite *sprite, int x, int y) {
    if (hasCapsule != FALSE) {
        ManagedSprite_SetAnim(sprite, 2);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

void BattlePartyMenu_ShowPageSprites(BattlePartyMenu *menu, u32 page) {
    u32 i;

    for (i = 0; i < 38; i++) {
        ManagedSprite_SetDrawFlag(menu->sprites[i], FALSE);
    }
    switch (page) {
    case 0:
        BattlePartyMenu_ShowPartyListSprites(menu);
        break;
    case 1:
        BattlePartyMenu_ShowMonMenuSprites(menu);
        break;
    case 2:
        BattlePartyMenu_ShowSummarySprites(menu);
        break;
    case 3:
        BattlePartyMenu_ShowCheckMovesSprites(menu);
        break;
    case 4:
        BattlePartyMenu_ShowMoveDetailSprites(menu);
        break;
    case 5:
        BattlePartyMenu_ShowRestorePPSprites(menu);
        break;
    case 6:
        BattlePartyMenu_ShowLearnMoveSprites(menu);
        break;
    case 7:
        BattlePartyMenu_ShowForgetMoveSprites(menu);
        break;
    case 8:
        BattlePartyMenu_ShowContestLearnMoveSprites(menu);
        break;
    case 9:
        BattlePartyMenu_ShowContestForgetMoveSprites(menu);
        break;
    }
}

void BattlePartyMenu_ShowPartyListSprites(BattlePartyMenu *menu) {
    int i;

    for (i = 0; i < 6; i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + i], sBattlePartyListIconPos[i].x, sBattlePartyListIconPos[i].y);
            BattlePartyMenu_ShowStatusIcon(menu->mons[i].status, menu->sprites[13 + i], sBattlePartyListStatusIconPos[i].x, sBattlePartyListStatusIconPos[i].y);
            BattlePartyMenu_ShowHeldItemIcon(menu->mons[i].heldItem, menu->sprites[i], sBattlePartyListIconPos[i].x + 8, sBattlePartyListIconPos[i].y + 8);
            BattlePartyMenu_ShowCapsuleIcon(menu->mons[i].ballCapsule, menu->sprites[32 + i], sBattlePartyListIconPos[i].x + 16, sBattlePartyListIconPos[i].y + 8);
        }
    }
}

void BattlePartyMenu_ShowMonMenuSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 128, 72);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 136, 80);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 144, 80);
}

void BattlePartyMenu_ShowSummarySprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartySummaryTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[6], 20, 132);
}

void BattlePartyMenu_ShowCheckMovesSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];
    u32 i;

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartyCheckMovesTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, mon->moves[i].type);
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], sBattlePartyCheckMovesMoveIconPos[i].x, sBattlePartyCheckMovesMoveIconPos[i].y);
        }
    }
}

void BattlePartyMenu_ShowMoveDetailSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowStatusIcon(mon->status, menu->sprites[13 + menu->context->selectedMon], 198, 20);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartyMoveDetailTypeIconPos);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + menu->context->selectedMove], 136, 48);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_SetMoveCategoryIcon(menu, menu->sprites[26], mon->moves[menu->context->selectedMove].moveClass);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[26], 24, 88);
}

void BattlePartyMenu_ShowLearnMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
}

void BattlePartyMenu_ShowForgetMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartyForgetMoveTypeIconPos);
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

void BattlePartyMenu_ShowRestorePPSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowMonTypeIcons(menu, mon, sBattlePartyLearnMoveTypeIconPos);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
}

void BattlePartyMenu_ShowContestForgetMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + menu->context->selectedMove], 136, 72);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowConditionMarkers(menu);
}

void BattlePartyMenu_ShowContestLearnMoveSprites(BattlePartyMenu *menu) {
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    BattlePartyMenu_ShowSpriteAt(menu->sprites[7 + menu->context->selectedMon], 24, 12);
    BattlePartyMenu_ShowHeldItemIcon(mon->heldItem, menu->sprites[menu->context->selectedMon], 32, 20);
    BattlePartyMenu_ShowCapsuleIcon(mon->ballCapsule, menu->sprites[32 + menu->context->selectedMon], 40, 20);
    BattlePartyMenu_ShowMoveIcons(menu);
    BattlePartyMenu_ShowConditionMarkers(menu);
}

void BattlePartyMenu_ShowMoveTypeIcons(BattlePartyMenu *menu) {
    u16 i;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, mon->moves[i].type);
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], sBattlePartyLearnMoveMoveIconPos[i].x, sBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (menu->context->move != MOVE_NONE) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[25], 0xB014, (u8)GetMoveAttr(menu->context->move, MOVEATTR_TYPE));
        BattlePartyMenu_ShowSpriteAt(menu->sprites[25], 88, 176);
    }
}

// Contest type icons follow the 18 battle types in the type icon resources.
void BattlePartyMenu_ShowMoveContestTypeIcons(BattlePartyMenu *menu) {
    u16 i;
    BattlePartyMenuMon *mon = &menu->mons[menu->context->selectedMon];

    for (i = 0; i < 4; i++) {
        if (mon->moves[i].move != MOVE_NONE) {
            BattlePartyMenu_SetTypeIcon(menu, menu->sprites[21 + i], 0xB010 + i, (u8)(GetMoveAttr(mon->moves[i].move, MOVEATTR_CONTEST_TYPE) + 18));
            BattlePartyMenu_ShowSpriteAt(menu->sprites[21 + i], sBattlePartyLearnMoveMoveIconPos[i].x, sBattlePartyLearnMoveMoveIconPos[i].y);
        }
    }
    if (menu->context->move != MOVE_NONE) {
        BattlePartyMenu_SetTypeIcon(menu, menu->sprites[25], 0xB014, (u8)(GetMoveAttr(menu->context->move, MOVEATTR_CONTEST_TYPE) + 18));
        BattlePartyMenu_ShowSpriteAt(menu->sprites[25], 88, 176);
    }
}

void BattlePartyMenu_ShowMoveIcons(BattlePartyMenu *menu) {
    if (menu->viewMode == 0) {
        BattlePartyMenu_ShowMoveTypeIcons(menu);
    } else {
        BattlePartyMenu_ShowMoveContestTypeIcons(menu);
    }
}

void BattlePartyMenu_SetSpriteAnim(ManagedSprite *sprite, u32 anim) {
    if (anim != ManagedSprite_GetActiveAnim(sprite)) {
        ManagedSprite_SetAnimationFrame(sprite, 0);
        ManagedSprite_SetAnim(sprite, anim);
    }
}

#pragma force_active reset

int BattlePartyMenu_GetMonIconAnim(BattlePartyMenuMon *mon) {
    if (mon->hp == 0) {
        return 0;
    }
    if (mon->status != 7 && mon->status != 6) {
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

void BattlePartyMenu_UpdateMonIcons(BattlePartyMenu *menu) {
    u16 i;

    for (i = 0; i < Party_GetCount(menu->context->party); i++) {
        if (menu->mons[i].species != SPECIES_NONE) {
            int anim = BattlePartyMenu_GetMonIconAnim(&menu->mons[i]);
            // addressed as menu + i * 4 + the offset of the first icon
            ManagedSprite **sprite = (ManagedSprite **)((u8 *)menu + i * sizeof(ManagedSprite *) + offsetof(BattlePartyMenu, sprites[7]));
            BattlePartyMenu_SetSpriteAnim(*sprite, (u8)anim);
            ManagedSprite_TickNFrames(*sprite, FX32_ONE);
        }
    }
}

s16 BattlePartyMenu_CalcConditionMarkerCoord(u8 stat, int target, int base) {
    u32 n;

    if (base > target) {
        n = ((u32)((base - target) * (stat + 44)) << 16) / 300;
        return base - (s16)(n >> 16);
    }
    n = ((u32)((target - base) * (stat + 44)) << 16) / 300;
    return base + (s16)(n >> 16);
}

#pragma force_active on
#pragma opt_propagation off

void BattlePartyMenu_ShowConditionMarkers(BattlePartyMenu *menu) {
    register int offset = sizeof(BattlePartyMenuMon) * menu->context->selectedMon;
    register BattlePartyMenuMon *mons = menu->mons;
    int x, y;

    BattlePartyMenu_SetSpriteAnim(menu->sprites[27], 0);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[28], 1);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[29], 3);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[30], 4);
    BattlePartyMenu_SetSpriteAnim(menu->sprites[31], 2);
    x = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[0], 144, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[0], 2, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[27], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[1], 164, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[1], 16, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[28], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[2], 156, 144);
    y = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[2], 41, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[29], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[3], 131, 143);
    y = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[3], 41, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[30], x, y);
    x = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[4], 123, 143);
    y = BattlePartyMenu_CalcConditionMarkerCoord(((BattlePartyMenuMon *)((u8 *)mons + offset))->contestStats[4], 16, 24);
    BattlePartyMenu_ShowSpriteAt(menu->sprites[31], x, y);
}

#pragma opt_propagation reset
#pragma force_active reset

void BattlePartyMenu_CreateCursor(BattlePartyMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    BattleCursor *cursor;

    BattleCursor_LoadResources(spriteSystem, menu->spriteManager, menu->paletteData, menu->context->heapID, 0xB018, 0xB00C, 0xB00C, 0xB00C);
    cursor = BattleCursor_New(spriteSystem, menu->spriteManager, menu->context->heapID, 0xB018, 0xB00C, 0xB00C, 0xB00C, 0, 1);
    BattleDpadCursor_SetBattleCursor(menu->cursor, cursor);
}

void BattlePartyMenu_FreeCursor(BattlePartyMenu *menu) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(menu->cursor));
    BattleCursor_FreeResources(menu->spriteManager, 0xB018, 0xB00C, 0xB00C, 0xB00C);
}

// Without the view toggle button (box 6 resp. 2), the d-pad cursor skips it.
void BattlePartyMenu_SetLearnMoveCursorMask(BattlePartyMenu *menu) {
    if (menu->canToggleView == 0) {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 0x5F);
    } else {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 0x7F);
    }
}

void BattlePartyMenu_SetForgetMoveCursorMask(BattlePartyMenu *menu) {
    if (menu->canToggleView == 0) {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 5);
    } else {
        BattleDpadCursor_SetEnabledMask(menu->cursor, 7);
    }
}

#pragma force_active on

void BattlePartyMenu_SetPageCursor(BattlePartyMenu *menu, u32 page) {
    BattleDpadCursor_SetBoxes(menu->cursor, sBattlePartyPageCursorBoxes[page]);
    switch (page) {
    case 0:
        BattleDpadCursor_SetPosition(menu->cursor, menu->context->selectedMon);
        menu->monMenuPosition = 0;
        menu->context->selectedMove = 0;
        break;
    case 1:
        BattleDpadCursor_SetPosition(menu->cursor, menu->monMenuPosition);
        menu->context->selectedMove = 0;
        break;
    case 3:
    case 4:
        BattleDpadCursor_SetPosition(menu->cursor, menu->context->selectedMove);
        break;
    case 6:
    case 8:
        BattlePartyMenu_SetLearnMoveCursorMask(menu);
        BattleDpadCursor_SetPosition(menu->cursor, menu->learnMovePosition);
        break;
    case 7:
    case 9:
        BattlePartyMenu_SetForgetMoveCursorMask(menu);
        BattleDpadCursor_SetPosition(menu->cursor, menu->forgetMovePosition);
        break;
    }
}

#pragma force_active reset

void BattlePartyMenu_DeactivateCursor(BattlePartyMenu *menu) {
    BattleDpadCursor_SetActive(menu->cursor, 0);
    BattleDpadCursor_ResetPosition(menu->cursor);
    BattleCursor_Disable(BattleDpadCursor_GetBattleCursor(menu->cursor));
}

#pragma force_active on

void BattlePartyMenu_StoreButtonTilemaps(BattlePartyMenu *menu, const u16 *screen) {
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

#pragma force_active reset

void BattlePartyMenu_StoreLargeButtonTilemaps(BattlePartyMenu *menu, const u16 *screen) {
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[0], screen, 0, 0, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[1], screen, 0, 17, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->shiftButtonTiles[2], screen, 0, 34, 30, 17);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[0], screen, 0, 51, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[1], screen, 16, 51, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[2], screen, 0, 57, 16, 6);
    BattlePartyMenu_CopyTilemapRect(menu->moveButtonTiles[3], screen, 16, 57, 16, 6);
}

void BattlePartyMenu_CopyTilemapRect(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height) {
    u16 row;
    u16 col;

    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            dst[row * width + col] = src[(y + row) * 32 + x + col];
        }
    }
}

u16 *BattlePartyMenu_GetButtonTilemap(BattlePartyMenu *menu, int button, int frame, int partner) {
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

#pragma force_active on

// Frame 3 is the disabled look. Mon buttons are greyed for an Egg's HP bar, shown red for a fainted mon and
// blue for a partner's mon; button 27 (the move to learn) uses palette 10.
void BattlePartyMenu_BuildButtonTilemap(BattlePartyMenu *menu, u16 *dst, int button, int frame, u8 palette) {
    const u16 *src = BattlePartyMenu_GetButtonTilemap(menu, button, frame, palette);
    u8 width = sBattlePartyButtonRectsWidth[button * 4];
    u8 height = sBattlePartyButtonRectsHeight[button * 4];
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

void BattlePartyMenu_DrawButton(BattlePartyMenu *menu, u8 button, u8 frame, u8 palette) {
    // the table is read here as bytes, through labels on its members
    int index = button * 4;
    int height = sBattlePartyButtonRectsHeight[index];
    int width = sBattlePartyButtonRectsWidth[index];
    u16 *buffer = Heap_Alloc(menu->context->heapID, width * height * 2);

    BattlePartyMenu_BuildButtonTilemap(menu, buffer, button, frame, palette);
    LoadRectToBgTilemapRect(menu->bgConfig, 6, buffer, sBattlePartyButtonRects[index], sBattlePartyButtonRectsY[index], width, height);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 6);
    Heap_Free(buffer);
}

#pragma force_active reset

// The button animation passes its scroll and offset amounts on as ints, without the narrowing of the bg_window.h and
// sprite_system.h prototypes.
typedef void (*ScrollWindowFunc)(Window *window, int direction, int pixels, int fillValue);
typedef void (*OffsetSpriteFunc)(ManagedSprite *sprite, int dx, int dy);

void BattlePartyMenu_ScrollButtonWindows(BattlePartyMenu *menu, u32 button, int frame) {
    const u8 *windowList = sBattlePartyButtonWindowLists[button];
    u16 i;
    int pixels;
    int direction;

    if (windowList != NULL) {
        if (button <= 5) {
            pixels = sBattlePartySlotButtonScrollAmounts[frame];
        } else {
            pixels = sBattlePartyButtonScrollAmounts[frame];
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

// (the (int) index casts keep the sprite offsets from being folded)
void BattlePartyMenu_OffsetButtonSprites(BattlePartyMenu *menu, u32 button, int frame) {
    int dy;

    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        dy = sBattlePartySlotButtonSpriteOffsets[frame];
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 13)], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[button], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 7)], 0, dy);
        break;
    case 7:
        dy = sBattlePartyButtonSpriteOffsets[frame];
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[menu->context->selectedMon], 0, dy);
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[menu->context->selectedMon + 7], 0, dy);
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 7)], 0, sBattlePartyButtonSpriteOffsets[frame]);
        break;
    case 19:
    case 20:
    case 21:
    case 22:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button + 2)], 0, sBattlePartyButtonSpriteOffsets[frame]);
        break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        ((OffsetSpriteFunc)ManagedSprite_OffsetPositionXY)(menu->sprites[(int)(button - 2)], 0, sBattlePartyButtonSpriteOffsets[frame]);
        break;
    }
}

void BattlePartyMenu_StartButtonAnim(BattlePartyMenu *menu, u32 button) {
    menu->buttonAnimPalette = 0;
    if (button <= 5 && BattlePartyMenu_GetSlotType(menu, button) == 2) {
        menu->buttonAnimPalette = 1;
    }
    menu->buttonAnimState = 0;
    menu->buttonAnimTimer = 0;
    menu->buttonAnimButton = button;
    menu->buttonAnimActive = TRUE;
}

void BattlePartyMenu_UpdateButtonAnim(BattlePartyMenu *menu) {
    if (menu->buttonAnimActive) {
        switch (menu->buttonAnimState) {
        case 0:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 1, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 1);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 1);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = 1;
            break;
        case 1:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 2, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 2);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 2);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = 2;
            break;
        case 2:
            BattlePartyMenu_DrawButton(menu, menu->buttonAnimButton, 0, menu->buttonAnimPalette);
            BattlePartyMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 0);
            BattlePartyMenu_OffsetButtonSprites(menu, menu->buttonAnimButton, 0);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimActive = FALSE;
            break;
        }
    }
}

void BattlePartyMenu_DrawPageButtons(BattlePartyMenu *menu, u32 page) {
    u16 i;

    switch (page) {
    case 0:
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
        if (menu->context->mode == 1) {
            BattlePartyMenu_DrawButton(menu, 6, 3, 0);
        } else {
            BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        }
        break;
    case 1:
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
    case 2:
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
    case 3:
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
    case 4:
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        for (i = 0; i < 4; i++) {
            if (menu->context->selectedMove == i) {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 30), 2, 0);
            } else {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 30), 0, 0);
            }
        }
        break;
    case 5:
        for (i = 0; i < 4; i++) {
            if (menu->mons[menu->context->selectedMon].moves[i].move != MOVE_NONE) {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 19), 0, 0);
            } else {
                BattlePartyMenu_DrawButton(menu, (u8)(i + 19), 3, 0);
            }
        }
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        break;
    case 6:
    case 8:
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
    case 7:
        BattlePartyMenu_DrawButton(menu, 28, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        if (menu->canToggleView == 1) {
            BattlePartyMenu_DrawButton(menu, 18, 0, 0);
        }
        break;
    case 9:
        BattlePartyMenu_DrawButton(menu, 29, 0, 0);
        BattlePartyMenu_DrawButton(menu, 6, 0, 0);
        if (menu->canToggleView == 1) {
            BattlePartyMenu_DrawButton(menu, 18, 0, 0);
        }
        break;
    }
}

void BattlePartyMenu_LoadPagePalette(BattlePartyMenu *menu, int page) {
    if (page == 3) {
        PaletteData_LoadPalette(menu->paletteData, menu->pagePalettes[1], PLTTBUF_SUB_BG, 0xC0, 0x20);
    } else {
        PaletteData_LoadPalette(menu->paletteData, menu->pagePalettes[0], PLTTBUF_SUB_BG, 0xC0, 0x20);
    }
}

u8 BattlePartyMenu_HasMultipleNonEggMons(BattlePartyMenu *menu) {
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

void BattlePartyMenu_RedrawSelectedMonButton(BattlePartyMenu *menu) {
    if (BattlePartyMenu_GetSlotType(menu, menu->context->selectedMon) == 2) {
        BattlePartyMenu_DrawButton(menu, menu->context->selectedMon, 0, 1);
    } else {
        BattlePartyMenu_DrawButton(menu, menu->context->selectedMon, 0, 0);
    }
}
