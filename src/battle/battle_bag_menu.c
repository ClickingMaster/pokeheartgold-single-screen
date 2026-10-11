#include "battle/battle_bag_menu.h"

#include <string.h>

#include "global.h"

#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/sndseq.h"

#include "battle/battle_cursor.h"
#include "battle/battle_dpad_cursor.h"
#include "battle/battle_finger.h"
#include "battle/battle_main.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "party.h"
#include "pokemon_storage_system.h"
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

// In-battle bag screen (overlay 8): pocket menu (page 0), item list (page 1) and use-item page (page 2) on the
// touch screen, plus the auto-played tutorial bag.

typedef struct BattleBagButtonRect {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} BattleBagButtonRect;

typedef struct BattleBagMenu {
    BattleBagContext *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    MessagePrinter *msgPrinter;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    Window messageWindow;
    Window *pageWindows;
    u8 pageWindowCount;
    u8 itemListWindowBank; // the item list alternates between two sets of 12 windows
    u8 textPrinterId;
    BattleDpadCursor *cursor;
    BattleFinger *finger;
    ItemSlot pocketItems[5][36];
    SpriteManager *spriteManager;
    ManagedSprite *itemIcons[6];
    // Button tilemaps cut out of the screen file, one entry per press animation frame
    u16 pocketButtonTiles[3][16 * 9];
    u16 lastUsedButtonTiles[4][26 * 5];
    u16 cancelButtonTiles[3][5 * 5];
    u16 itemButtonTiles[4][16 * 6];
    u16 prevPageButtonTiles[4][5 * 5];
    u16 nextPageButtonTiles[4][5 * 5];
    u16 pocketIconTiles[4][3][4 * 4];
    u8 buttonAnimState;
    u8 buttonAnimTimer;
    u8 buttonAnimButton;
    u8 buttonAnimActive : 4;
    u8 unk_1141_4 : 4;
    u8 filler_1142[8];
    u8 state;
    u8 nextState;
    u8 page;
    u8 pocket;
    s8 pageDelta;
    u8 itemCounts[5];
    u8 lastPages[5];
    u8 tutorialState;
    u8 tutorialTimer;
} BattleBagMenu; // size: 0x115C

void BattleBagMenu_Task(SysTask *task, void *data);
int BattleBagMenu_Setup(BattleBagMenu *menu);
int BattleBagMenu_HandlePocketMenuInput(BattleBagMenu *menu);
int BattleBagMenu_HandleItemListInput(BattleBagMenu *menu);
int BattleBagMenu_ChangeItemPage(BattleBagMenu *menu);
int BattleBagMenu_HandleUseItemInput(BattleBagMenu *menu);
int BattleBagMenu_TryUseItem(BattleBagMenu *menu);
int BattleBagMenu_ShowPocketMenuPage(BattleBagMenu *menu);
int BattleBagMenu_ShowItemListPage(BattleBagMenu *menu);
int BattleBagMenu_ShowUseItemPage(BattleBagMenu *menu);
int BattleBagMenu_CloseMessage(BattleBagMenu *menu);
int BattleBagMenu_WaitMessage(const BattleBagMenu *menu);
int BattleBagMenu_WaitMessageInput(const BattleBagMenu *menu);
int BattleBagMenu_WaitButtonAnim(const BattleBagMenu *menu);
int BattleBagMenu_StartFadeOut(BattleBagMenu *menu);
int BattleBagMenu_Exit(SysTask *task, BattleBagMenu *menu);
int BattleBagMenu_RunTutorial(BattleBagMenu *menu);
void BattleBagMenu_InitBgs(BattleBagMenu *menu);
void BattleBagMenu_FreeBgs(BgConfig *bgConfig);
void BattleBagMenu_LoadBgGraphics(BattleBagMenu *menu);
void BattleBagMenu_InitText(BattleBagMenu *menu);
void BattleBagMenu_FreeText(BattleBagMenu *menu);
void BattleBagMenu_SetPageBgOffset(BattleBagMenu *menu, int page);
void BattleBagMenu_SetUseItemPagePalette(BattleBagMenu *menu, int page);
void BattleBagMenu_ChangePage(BattleBagMenu *menu, u8 page);
int BattleBagMenu_CheckTouchHitbox(BattleBagMenu *menu, const TouchscreenHitbox *hitboxes);
int BattleBagMenu_GetBattlerSelectedMonIndex(BattleBagMenu *menu);
void BattleBagMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID);
void BattleBagMenu_AddWindows(BattleBagMenu *menu);
void BattleBagMenu_AddPageWindows(BattleBagMenu *menu, int page);
void BattleBagMenu_RemovePageWindows(BattleBagMenu *menu);
void BattleBagMenu_RemoveWindows(BattleBagMenu *menu);
void BattleBagMenu_PrintPageText(BattleBagMenu *menu, int page);
void BattleBagMenu_PrintCenteredText(BattleBagMenu *menu, u32 window, u32 msgNo, FontID fontId, u32 y, u32 color);
void BattleBagMenu_PrintPocketMenuPage(BattleBagMenu *menu);
void BattleBagMenu_PrintItemName(BattleBagMenu *menu, int slot, int entry, int window, u32 fontId, u32 color);
void BattleBagMenu_PrintItemQuantity(BattleBagMenu *menu, int slot, int entry, int window, u32 fontId, int y, int color);
void BattleBagMenu_PrintItemListEntry(BattleBagMenu *menu, int entry);
void BattleBagMenu_PrintItemList(BattleBagMenu *menu);
void BattleBagMenu_PrintPageNumber(BattleBagMenu *menu);
void BattleBagMenu_PrintPocketName(BattleBagMenu *menu);
void BattleBagMenu_PrintItemListPage(BattleBagMenu *menu);
void BattleBagMenu_PrintUseItemName(BattleBagMenu *menu, int slot);
void BattleBagMenu_PrintItemDescription(BattleBagMenu *menu, int slot);
void BattleBagMenu_PrintUseItemPage(BattleBagMenu *menu);
void BattleBagMenu_ShowMessage(BattleBagMenu *menu);
void BattleBagMenu_PrintMessage(BattleBagMenu *menu);
BOOL BattleBagMenu_ValidateLastUsedItem(BattleBagMenu *menu);
void BattleBagMenu_SelectLastUsedItem(BattleBagMenu *menu);
void BattleBagMenu_SortItemsIntoPockets(BattleBagMenu *menu);
u16 BattleBagMenu_GetItemOnPage(BattleBagMenu *menu, u32 position);
void BattleBagMenu_InitSprites(BattleBagMenu *menu);
void BattleBagMenu_CreateSpriteManager(BattleBagMenu *menu);
void BattleBagMenu_LoadItemIconResources(BattleBagMenu *menu);
void BattleBagMenu_SetItemIconGraphics(BattleBagMenu *menu, u16 item, int resId);
void BattleBagMenu_SetItemIconPalette(BattleBagMenu *menu, u16 item, u32 slot, int resId);
ManagedSprite *BattleBagMenu_CreateSprite(BattleBagMenu *menu, int index);
void BattleBagMenu_CreateSprites(BattleBagMenu *menu);
void BattleBagMenu_FreeSprites(BattleBagMenu *menu);
void BattleBagMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y);
void BattleBagMenu_ShowPageSprites(BattleBagMenu *menu, u32 page);
void BattleBagMenu_ShowLastUsedItemIcon(BattleBagMenu *menu);
void BattleBagMenu_ShowItemListIcons(BattleBagMenu *menu);
void BattleBagMenu_ShowUseItemIcon(BattleBagMenu *menu);
void BattleBagMenu_CreateCursor(BattleBagMenu *menu);
void BattleBagMenu_FreeCursor(BattleBagMenu *menu);
void BattleBagMenu_SetPageCursor(BattleBagMenu *menu, int page);
void BattleBagMenu_DeactivateCursor(BattleBagMenu *menu);
void BattleBagMenu_CreateFinger(BattleBagMenu *menu);
void BattleBagMenu_FreeFinger(BattleBagMenu *menu);
void BattleBagMenu_UpdateTutorialFinger(BattleBagMenu *menu, int page);
void BattleBagMenu_StoreButtonTilemaps(BattleBagMenu *menu, const u16 *screen);
void BattleBagMenu_CopyTilemapRect(u16 *dst, const u16 *src, u16 x, u16 y, u8 width, u8 height);
u16 *BattleBagMenu_GetButtonTilemap(BattleBagMenu *menu, u32 button, u32 frame);
u16 BattleBagMenu_GetButtonPalette(BattleBagMenu *menu, u32 button, u32 frame, u32 page);
void BattleBagMenu_CopyPocketIconTiles(BattleBagMenu *menu, u16 *dst, u32 button, u32 frame);
void BattleBagMenu_BuildButtonTilemap(BattleBagMenu *menu, u16 *dst, u8 button, u8 frame, u8 page);
void BattleBagMenu_DrawButton(BattleBagMenu *menu, u8 button, u8 frame, u8 page);
void BattleBagMenu_ScrollButtonWindows(BattleBagMenu *menu, u8 button, int frame);
void BattleBagMenu_OffsetButtonSprite(BattleBagMenu *menu, u32 button, u32 frame);
void BattleBagMenu_StartButtonAnim(BattleBagMenu *menu, u8 button, u8 mode);
void BattleBagMenu_UpdateButtonAnim(BattleBagMenu *menu);
void BattleBagMenu_DrawPageButtons(BattleBagMenu *menu, u8 page);

extern const GraphicsModes sBattleBagGraphicsModes;
extern const BgTemplate sBattleBagBgTemplateSub0;
extern const BgTemplate sBattleBagBgTemplateSub1;
extern const BgTemplate sBattleBagBgTemplateSub2;
extern const TouchscreenHitbox sBattleBagPocketMenuHitboxes[];
extern const TouchscreenHitbox sBattleBagItemListHitboxes[];
extern const TouchscreenHitbox sBattleBagUseItemHitboxes[];
extern const WindowTemplate sBattleBagMessageWindowTemplate;
extern const WindowTemplate sBattleBagPocketMenuWindows[];
extern const WindowTemplate sBattleBagItemListWindows[];
extern const WindowTemplate sBattleBagUseItemWindows[];
// The tables below are still assembly data; some members are addressed through their own labels.
extern const u32 sBattleBagItemEntryMsgIds[][2];
extern const u32 sBattleBagItemEntryQuantityMsgIds[][2];
extern const SpriteResourceCountsListUnion sBattleBagSpriteCapacities;
extern const int sBattleBagSpriteTemplates[][5];
extern const int sBattleBagSpriteTemplatesPltt[][5];
extern const int sBattleBagSpriteTemplatesCell[][5];
extern const int sBattleBagSpriteTemplatesAnim[][5];
extern const int sBattleBagSpriteTemplatesPriority[][5];
extern const u8 sBattleBagPocketOrder[];
extern const s32 sBattleBagItemListIconPos[][2];
extern const DpadMenuBox *const sBattleBagPageCursorBoxes[];
extern const s32 sBattleBagTutorialFingerPos[][2];
extern const s32 sBattleBagTutorialFingerPosY[][2];
extern const BattleBagButtonRect sBattleBagButtonRects[];
extern const u8 sBattleBagButtonRectsY[];
extern const u8 sBattleBagButtonRectsWidth[];
extern const u8 sBattleBagButtonRectsHeight[];
extern const u8 *const sBattleBagButtonWindowLists[];

void BattleBagMenu_Create(BattleBagContext *context) {
    BattleBagMenu *menu;
    BagCursor *bagCursor;
    u8 i;

    menu = SysTask_GetData(CreateSysTaskAndEnvironment(BattleBagMenu_Task, sizeof(BattleBagMenu), 100, context->heapID));
    memset(menu, 0, sizeof(BattleBagMenu));
    menu->context = context;
    menu->bgConfig = BattleSystem_GetBgConfig(context->battleSystem);
    menu->paletteData = BattleSystem_GetPaletteData(context->battleSystem);
    menu->state = 0;
    bagCursor = BattleSystem_GetBagCursor(context->battleSystem);
    for (i = 0; i < 5; i++) {
        BagCursor_Battle_PocketGetPosition(bagCursor, i, &menu->context->pocketPositions[i], &menu->context->pocketPages[i]);
    }
    menu->context->lastUsedItem = BagCursor_Battle_GetLastUsedItem(bagCursor);
    menu->context->lastUsedPocket = BagCursor_Battle_GetLastUsedPocket(bagCursor);
    BattleBagMenu_ValidateLastUsedItem(menu);
    if (BattleSystem_GetBattleType(menu->context->battleSystem) & BATTLE_TYPE_TUTORIAL) {
        menu->context->isTutorial = 1;
    }
}

void BattleBagMenu_Task(SysTask *task, void *data) {
    BattleBagMenu *menu = data;
    int state = menu->state;

    switch (state) {
    case 0:
        menu->state = BattleBagMenu_Setup(menu);
        break;
    case 1:
        menu->state = BattleBagMenu_HandlePocketMenuInput(menu);
        break;
    case 2:
        menu->state = BattleBagMenu_HandleItemListInput(menu);
        break;
    case 3:
        menu->state = BattleBagMenu_HandleUseItemInput(menu);
        break;
    case 4:
        menu->state = BattleBagMenu_ShowPocketMenuPage(menu);
        break;
    case 5:
        menu->state = BattleBagMenu_ShowItemListPage(menu);
        break;
    case 6:
        menu->state = BattleBagMenu_ShowUseItemPage(menu);
        break;
    case 7:
        menu->state = BattleBagMenu_ChangeItemPage(menu);
        break;
    case 8:
        menu->state = BattleBagMenu_CloseMessage(menu);
        break;
    case 9:
        menu->state = BattleBagMenu_WaitMessage(menu);
        break;
    case 10:
        menu->state = BattleBagMenu_WaitMessageInput(menu);
        break;
    case 11:
        menu->state = BattleBagMenu_WaitButtonAnim(menu);
        break;
    case 12:
        menu->state = BattleBagMenu_RunTutorial(menu);
        break;
    case 13:
        menu->state = BattleBagMenu_StartFadeOut(menu);
        break;
    case 14:
        if (BattleBagMenu_Exit(task, menu) == 1) {
            return;
        }
        break;
    }
    BattleBagMenu_UpdateButtonAnim(menu);
    SpriteSystem_DrawSprites(menu->spriteManager);
}

int BattleBagMenu_Setup(BattleBagMenu *menu) {
    G2S_BlendNone();
    menu->cursor = BattleDpadCursor_New(menu->context->heapID);
    BattleBagMenu_InitBgs(menu);
    BattleBagMenu_LoadBgGraphics(menu);
    BattleBagMenu_InitText(menu);
    FontID_Alloc(4, menu->context->heapID);
    menu->pocket = BagCursor_Battle_GetPocket(BattleSystem_GetBagCursor(menu->context->battleSystem));
    BattleBagMenu_SortItemsIntoPockets(menu);
    BattleBagMenu_DrawPageButtons(menu, menu->page);
    BattleBagMenu_AddWindows(menu);
    BattleBagMenu_PrintPageText(menu, menu->page);
    BattleBagMenu_InitSprites(menu);
    BattleBagMenu_ShowPageSprites(menu, menu->page);
    if (menu->context->cursorActive) {
        BattleDpadCursor_SetActive(menu->cursor, 1);
    }
    BattleBagMenu_SetPageCursor(menu, menu->page);
    BattleBagMenu_UpdateTutorialFinger(menu, menu->page);
    PaletteData_BeginPaletteFade(menu->paletteData, 10, 0xFFFF, -8, 16, 0, 0);
    if (menu->context->isTutorial == 1) {
        return 12;
    }
    return 1;
}

int BattleBagMenu_HandlePocketMenuInput(BattleBagMenu *menu) {
    int input;
    u8 pocket;

    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData)) {
        return 1;
    }
    input = BattleBagMenu_CheckTouchHitbox(menu, sBattleBagPocketMenuHitboxes);
    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 5;
        }
    } else {
        BattleBagMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        pocket = input;
        menu->pocket = pocket;
        menu->nextState = 5;
        BattleBagMenu_StartButtonAnim(menu, pocket, 0);
        return 11;
    case 4:
        if (!menu->context->lastUsedItem) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->pocket = menu->context->lastUsedPocket;
        menu->nextState = 6;
        BattleBagMenu_SelectLastUsedItem(menu);
        BattleBagMenu_StartButtonAnim(menu, 4, 0);
        return 11;
    case 5:
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->context->selectedItem = ITEM_NONE;
        menu->context->selectedPocket = 4;
        BattleBagMenu_StartButtonAnim(menu, 5, 0);
        return 13;
    }
    return 1;
}

#pragma force_active on

int BattleBagMenu_HandleItemListInput(BattleBagMenu *menu) {
    u32 input = BattleBagMenu_CheckTouchHitbox(menu, sBattleBagItemListHitboxes);

    if (input == -1u) {
        int wasActive = BattleDpadCursor_IsActive(menu->cursor);
        int prevPosition = BattleDpadCursor_GetPosition(menu->cursor);
        input = BattleDpadCursor_HandleInput(menu->cursor);
        int position = BattleDpadCursor_GetPosition(menu->cursor);
        if (input == -2u) {
            input = 6;
        } else if (wasActive == 1 && prevPosition == position) {
            // left/right against the edge of the item grid turns the page
            u32 keys = gSystem.newAndRepeatedKeys;
            if ((keys & PAD_KEY_LEFT) && (position == 0 || position == 2 || position == 4)) {
                input = 7;
            }
            if ((keys & PAD_KEY_RIGHT) && (position == 1 || position == 3 || position == 5)) {
                input = 8;
            }
        }
    } else {
        BattleBagMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (BattleBagMenu_GetItemOnPage(menu, input)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->context->pocketPositions[menu->pocket] = input;
            menu->nextState = 6;
            BattleBagMenu_StartButtonAnim(menu, input + 6, 0);
            return 11;
        }
        break;
    case 6:
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->nextState = 4;
        BattleBagMenu_StartButtonAnim(menu, 14, 0);
        return 11;
    case 7:
        if (menu->lastPages[menu->pocket]) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->nextState = 7;
            menu->pageDelta = -1;
            BattleBagMenu_StartButtonAnim(menu, 12, 0);
            return 11;
        }
        break;
    case 8:
        if (menu->lastPages[menu->pocket]) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->nextState = 7;
            menu->pageDelta = 1;
            BattleBagMenu_StartButtonAnim(menu, 13, 0);
            return 11;
        }
        break;
    }
    return 2;
}

int BattleBagMenu_ChangeItemPage(BattleBagMenu *menu) {
    s8 page = menu->context->pocketPages[menu->pocket];

    menu->context->pocketPositions[menu->pocket] = 0;
    page += menu->pageDelta;
    if (page > menu->lastPages[menu->pocket]) {
        menu->context->pocketPages[menu->pocket] = 0;
    } else if (page < 0) {
        menu->context->pocketPages[menu->pocket] = menu->lastPages[menu->pocket];
    } else {
        menu->context->pocketPages[menu->pocket] = page;
    }
    BattleBagMenu_PrintItemList(menu);
    BattleBagMenu_PrintPageNumber(menu);
    BattleBagMenu_ShowPageSprites(menu, menu->page);
    BattleBagMenu_DrawPageButtons(menu, menu->page);
    return 2;
}

#pragma force_active reset

int BattleBagMenu_HandleUseItemInput(BattleBagMenu *menu) {
    int input = BattleBagMenu_CheckTouchHitbox(menu, sBattleBagUseItemHitboxes);

    if (input == -1) {
        input = BattleDpadCursor_HandleInput(menu->cursor);
        if (input == -2) {
            input = 1;
        }
    } else {
        BattleBagMenu_DeactivateCursor(menu);
    }
    switch (input) {
    case 0: {
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->context->selectedItem = BattleBagMenu_GetItemOnPage(menu, menu->context->pocketPositions[menu->pocket]);
        menu->context->selectedPocket = menu->pocket;
        BattleBagMenu_StartButtonAnim(menu, 15, 0);
        return BattleBagMenu_TryUseItem(menu);
    }
    case 1: {
        PlaySE(SEQ_SE_DP_DECIDE);
        menu->nextState = 5;
        BattleBagMenu_StartButtonAnim(menu, 16, 0);
        return 11;
    }
    default:
        return 3;
    }
}

#pragma force_active on

int BattleBagMenu_TryUseItem(BattleBagMenu *menu) {
    BattleBagContext *context = menu->context;
    int slot, battleUseFunc;
    Pokemon *mon;
    String *string;
    MsgData *msgData;
    Party *party;
    PCStorage *pcStorage;

    if (menu->pocket == 3) {
        slot = BattleBagMenu_GetBattlerSelectedMonIndex(menu);
        battleUseFunc = GetItemAttr(context->selectedItem, ITEMATTR_BATTLEUSEFUNC, context->heapID);
        if (context->embargo && context->selectedItem != ITEM_GUARD_SPEC_ && battleUseFunc != 3) {
            mon = BattleSystem_GetPartyMon(context->battleSystem, context->battlerId, slot);
            string = NewString_ReadMsgData(menu->msgData, 46);
            BufferBoxMonNickname(menu->msgFormat, 0, Mon_GetBoxMon(mon));
            BufferMoveName(menu->msgFormat, 1, MOVE_EMBARGO);
            StringExpandPlaceholders(menu->msgFormat, menu->string, string);
            String_Delete(string);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
        if (BattleSystem_RecoverStatus(context->battleSystem, context->battlerId, slot, 0, context->selectedItem) == TRUE) {
            BattleBagMenu_ConsumeItem(context->battleSystem, context->selectedItem, menu->pocket, context->heapID);
            return 13;
        }
        if (battleUseFunc == 3) {
            if (!(BattleSystem_GetBattleType(context->battleSystem) & BATTLE_TYPE_TRAINER)) {
                BattleBagMenu_ConsumeItem(context->battleSystem, context->selectedItem, menu->pocket, context->heapID);
                return 13;
            }
            msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 40, context->heapID);
            string = NewString_ReadMsgData(msgData, 37);
            BufferPlayersName(menu->msgFormat, 0, context->profile);
            StringExpandPlaceholders(menu->msgFormat, menu->string, string);
            String_Delete(string);
            DestroyMsgData(msgData);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
        ReadMsgDataIntoString(menu->msgData, 34, menu->string);
        BattleBagMenu_ShowMessage(menu);
        menu->nextState = 8;
        return 9;
    }
    if (menu->pocket == 2) {
        if (context->ballBlocked[0] == 1) {
            ReadMsgDataIntoString(menu->msgData, 44, menu->string);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
        if (context->ballBlocked[1] == 1) {
            ReadMsgDataIntoString(menu->msgData, 47, menu->string);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
        if (context->ballBlocked[2] == 1) {
            ReadMsgDataIntoString(menu->msgData, 48, menu->string);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
        party = BattleSystem_GetParty(context->battleSystem, context->battlerId);
        pcStorage = BattleSystem_GetPcStorage(context->battleSystem);
        if (Party_GetCount(party) == PARTY_SIZE && PCStorage_FindFirstBoxWithEmptySlot(pcStorage) == NUM_BOXES) {
            ReadMsgDataIntoString(menu->msgData, 45, menu->string);
            BattleBagMenu_ShowMessage(menu);
            menu->nextState = 8;
            return 9;
        }
    }
    return 13;
}

int BattleBagMenu_ShowPocketMenuPage(BattleBagMenu *menu) {
    BattleBagMenu_ChangePage(menu, 0);
    return 1;
}

int BattleBagMenu_ShowItemListPage(BattleBagMenu *menu) {
    BattleBagMenu_ChangePage(menu, 1);
    return 2;
}

int BattleBagMenu_ShowUseItemPage(BattleBagMenu *menu) {
    BattleBagMenu_ChangePage(menu, 2);
    return 3;
}

int BattleBagMenu_CloseMessage(BattleBagMenu *menu) {
    ClearFrameAndWindow2(&menu->messageWindow, FALSE);
    return 3;
}

int BattleBagMenu_WaitMessage(const BattleBagMenu *menu) {
    if (TextPrinterCheckActive(menu->textPrinterId) == 0) {
        return 10;
    }
    return 9;
}

int BattleBagMenu_WaitMessageInput(const BattleBagMenu *menu) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == TRUE) {
        return menu->nextState;
    }
    return 10;
}

int BattleBagMenu_WaitButtonAnim(const BattleBagMenu *menu) {
    if (menu->buttonAnimState == 2) {
        return menu->nextState;
    }
    return 11;
}

int BattleBagMenu_StartFadeOut(BattleBagMenu *menu) {
    PaletteData_BeginPaletteFade(menu->paletteData, 10, 0xFFFF, -8, 0, 16, 0);
    return 14;
}

int BattleBagMenu_Exit(SysTask *task, BattleBagMenu *menu) {
    BagCursor *bagCursor;
    u8 i;

    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData)) {
        return 0;
    }
    BattleBagMenu_FreeSprites(menu);
    BattleBagMenu_RemoveWindows(menu);
    BattleBagMenu_FreeText(menu);
    BattleBagMenu_FreeBgs(menu->bgConfig);
    menu->context->cursorActive = BattleDpadCursor_IsActive(menu->cursor);
    BattleDpadCursor_Free(menu->cursor);
    FontID_Release(4);
    if (menu->context->selectedItem) {
        bagCursor = BattleSystem_GetBagCursor(menu->context->battleSystem);
        for (i = 0; i < 5; i++) {
            BagCursor_Battle_PocketSetPosition(bagCursor, i, menu->context->pocketPositions[i], menu->context->pocketPages[i]);
        }
        BagCursor_Battle_SetPocket(bagCursor, menu->pocket);
    }
    menu->context->done = TRUE;
    DestroySysTaskAndEnvironment(task);
    return 1;
}

int BattleBagMenu_RunTutorial(BattleBagMenu *menu) {
    if (PaletteData_GetSelectedBuffersBitmask(menu->paletteData)) {
        return 12;
    }
    switch (menu->tutorialState) {
    case 0:
        if (ov12_0226BD50(menu->finger) == TRUE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->pocket = 2;
            menu->nextState = 12;
            BattleBagMenu_StartButtonAnim(menu, 2, 0);
            menu->tutorialTimer = 0;
            menu->tutorialState++;
            return 11;
        }
        menu->tutorialTimer++;
        break;
    case 1:
        BattleBagMenu_ShowItemListPage(menu);
        menu->tutorialState++;
        break;
    case 2:
        if (ov12_0226BD50(menu->finger) == TRUE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->context->pocketPositions[menu->pocket] = 0;
            menu->nextState = 12;
            BattleBagMenu_StartButtonAnim(menu, 6, 0);
            menu->tutorialTimer = 0;
            menu->tutorialState++;
            return 11;
        }
        menu->tutorialTimer++;
        break;
    case 3:
        BattleBagMenu_ShowUseItemPage(menu);
        menu->tutorialState++;
        break;
    case 4:
        if (ov12_0226BD50(menu->finger) == TRUE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            menu->context->selectedItem = BattleBagMenu_GetItemOnPage(menu, menu->context->pocketPositions[menu->pocket]);
            menu->context->selectedPocket = menu->pocket;
            BattleBagMenu_StartButtonAnim(menu, 15, 0);
            return BattleBagMenu_TryUseItem(menu);
        }
        menu->tutorialTimer++;
        break;
    }
    return 12;
}

void BattleBagMenu_InitBgs(BattleBagMenu *menu) {
    GraphicsModes graphicsModes = sBattleBagGraphicsModes;
    SetScreenModeAndDisable(&graphicsModes, SCREEN_SUB);
    {
        BgTemplate template = sBattleBagBgTemplateSub2;
        InitBgFromTemplate(menu->bgConfig, 6, &template, 0);
    }
    {
        BgTemplate template = sBattleBagBgTemplateSub1;
        InitBgFromTemplate(menu->bgConfig, 5, &template, 0);
    }
    BgClearTilemapBufferAndCommit(menu->bgConfig, 5);
    {
        BgTemplate template = sBattleBagBgTemplateSub0;
        InitBgFromTemplate(menu->bgConfig, 4, &template, 0);
    }
    BgClearTilemapBufferAndCommit(menu->bgConfig, 4);
    BG_ClearCharDataRange(5, 0x20, 0, menu->context->heapID);
    BG_ClearCharDataRange(4, 0x20, 0, menu->context->heapID);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 5);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 4);
}

void BattleBagMenu_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 6);
}

void BattleBagMenu_LoadBgGraphics(BattleBagMenu *menu) {
    NARC *narc = NARC_New(NARC_a_0_7_7, menu->context->heapID);
    void *screenFile;
    NNSG2dScreenData *screenData;
    u32 frame;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, menu->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, menu->context->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, menu->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, menu->context->heapID);
    screenFile = NARC_AllocAndReadWholeMember(narc, 1, menu->context->heapID);
    NNS_G2dGetUnpackedScreenData(screenFile, &screenData);
    BattleBagMenu_StoreButtonTilemaps(menu, (const u16 *)screenData->rawData);
    Heap_Free(screenFile);
    NARC_Delete(narc);
    PaletteData_LoadNarc(menu->paletteData, NARC_a_0_7_7, 3, menu->context->heapID, PLTTBUF_SUB_BG, 0x180, 0);
    PaletteData_LoadNarc(menu->paletteData, NARC_graphic_font, 8, menu->context->heapID, PLTTBUF_SUB_BG, 0x20, 0xF0);
    frame = BattleSystem_GetFrame(menu->context->battleSystem);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, GetUserFrameGfx2TilesMember(frame), menu->bgConfig, GF_BG_LYR_SUB_0, 994, 0, FALSE, menu->context->heapID);
    PaletteData_LoadNarc(menu->paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), menu->context->heapID, PLTTBUF_SUB_BG, 0x20, 0xE0);
}

void BattleBagMenu_InitText(BattleBagMenu *menu) {
    menu->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 5, menu->context->heapID);
    menu->msgPrinter = MessagePrinter_New(15, 14, 0, menu->context->heapID);
    menu->msgFormat = MessageFormat_New(menu->context->heapID);
    menu->string = String_New(0x200, menu->context->heapID);
}

void BattleBagMenu_FreeText(BattleBagMenu *menu) {
    DestroyMsgData(menu->msgData);
    MessagePrinter_Delete(menu->msgPrinter);
    MessageFormat_Delete(menu->msgFormat);
    String_Delete(menu->string);
}

#pragma force_active reset

void BattleBagMenu_SetPageBgOffset(BattleBagMenu *menu, int page) {
    switch (page) {
    case 0:
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 1:
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_X, 0x100);
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        break;
    case 2:
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(menu->bgConfig, 6, BG_POS_OP_SET_Y, 0x100);
        break;
    }
}

void BattleBagMenu_SetUseItemPagePalette(BattleBagMenu *menu, int page) {
    if (page == 2) {
        BgTilemapRectChangePalette(menu->bgConfig, 6, 2, 0x23, 0x1C, 4, menu->pocket + 8);
        BgTilemapRectChangePalette(menu->bgConfig, 6, 2, 0x28, 0x1C, 8, menu->pocket + 8);
    }
}

void BattleBagMenu_ChangePage(BattleBagMenu *menu, u8 page) {
    BattleBagMenu_SetUseItemPagePalette(menu, page);
    BattleBagMenu_SetPageBgOffset(menu, page);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, 4, 0);
    BgFillTilemapBufferAndSchedule(menu->bgConfig, 5, 0);
    BattleBagMenu_RemovePageWindows(menu);
    BattleBagMenu_AddPageWindows(menu, page);
    BattleBagMenu_PrintPageText(menu, page);
    BattleBagMenu_DrawPageButtons(menu, page);
    BattleBagMenu_SetPageCursor(menu, page);
    BattleBagMenu_UpdateTutorialFinger(menu, page);
    menu->page = page;
    BattleBagMenu_ShowPageSprites(menu, menu->page);
}

#pragma force_active on

int BattleBagMenu_CheckTouchHitbox(BattleBagMenu *menu, const TouchscreenHitbox *hitboxes) {
    return TouchscreenHitbox_FindRectAtTouchNew(hitboxes);
}

#pragma force_active reset

int BattleBagMenu_GetBattlerSelectedMonIndex(BattleBagMenu *menu) {
    BattleBagContext *context = menu->context;
    BattleContext *battleContext = BattleSystem_GetBattleContext(context->battleSystem);
    return ov12_022581D4(menu->context->battleSystem, battleContext, 2, context->battlerId);
}

void BattleBagMenu_ConsumeItem(BattleSystem *battleSystem, u16 item, u8 pocket, enum HeapID heapID) {
    Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, heapID);
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}

void BattleBagMenu_AddWindows(BattleBagMenu *menu) {
    AddWindow(menu->bgConfig, &menu->messageWindow, &sBattleBagMessageWindowTemplate);
    BattleBagMenu_AddPageWindows(menu, menu->page);
}

void BattleBagMenu_AddPageWindows(BattleBagMenu *menu, int page) {
    const WindowTemplate *templates;
    u8 i;

    switch (page) {
    case 0:
        templates = sBattleBagPocketMenuWindows;
        menu->pageWindowCount = 5;
        break;
    case 1:
        templates = sBattleBagItemListWindows;
        menu->pageWindowCount = 26;
        break;
    case 2:
        templates = sBattleBagUseItemWindows;
        menu->pageWindowCount = 4;
        break;
    }
    menu->pageWindows = AllocWindows(menu->context->heapID, menu->pageWindowCount);
    for (i = 0; i < menu->pageWindowCount; i++) {
        AddWindow(menu->bgConfig, &menu->pageWindows[i], &templates[i]);
    }
}

#pragma force_active on

void BattleBagMenu_RemovePageWindows(BattleBagMenu *menu) {
    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
}

void BattleBagMenu_RemoveWindows(BattleBagMenu *menu) {
    WindowArray_Delete(menu->pageWindows, menu->pageWindowCount);
    RemoveWindow(&menu->messageWindow);
}

void BattleBagMenu_PrintPageText(BattleBagMenu *menu, int page) {
    switch (page) {
    case 0:
        BattleBagMenu_PrintPocketMenuPage(menu);
        break;
    case 1:
        BattleBagMenu_PrintItemListPage(menu);
        break;
    case 2:
        BattleBagMenu_PrintUseItemPage(menu);
        break;
    }
}

void BattleBagMenu_PrintCenteredText(BattleBagMenu *menu, u32 window, u32 msgNo, FontID fontId, u32 y, u32 color) {
    Window *windows = menu->pageWindows;
    Window *win = &windows[window];
    String *string = NewString_ReadMsgData(menu->msgData, msgNo);
    u32 width = FontID_String_GetWidth(fontId, string, 0);
    u32 x = (GetWindowWidth(win) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(win, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}

void BattleBagMenu_PrintPocketMenuPage(BattleBagMenu *menu) {
    u32 i;

    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&menu->pageWindows[i], 0);
    }
    BattleBagMenu_PrintCenteredText(menu, 0, 0, 4, 7, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintCenteredText(menu, 0, 1, 4, 0x17, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintCenteredText(menu, 1, 2, 4, 7, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintCenteredText(menu, 1, 3, 4, 0x17, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintCenteredText(menu, 2, 7, 4, 7, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintCenteredText(menu, 3, 6, 4, 7, MAKE_TEXT_COLOR(3, 2, 1));
    if (menu->context->lastUsedItem != ITEM_NONE) {
        String *string = NewString_ReadMsgData(menu->msgData, 8);
        AddTextPrinterParameterizedWithColor(&menu->pageWindows[4], 4, string, 0, 5, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 2, 1), NULL);
        String_Delete(string);
        ScheduleWindowCopyToVram(&menu->pageWindows[4]);
    }
}

// The item list printers pass their font id on as 32 bits, without the FontID narrowing of the text.h prototype.
typedef u8 (*AddTextPrinterWithColorFunc)(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

void BattleBagMenu_PrintItemName(BattleBagMenu *menu, int slot, int entry, int window, u32 fontId, u32 color) {
    Window *windows = menu->pageWindows;
    String *string;
    u32 width;
    u32 x;

    FillWindowPixelBuffer(&windows[window], 0);
    if (menu->pocketItems[menu->pocket][slot].id != ITEM_NONE) {
        string = NewString_ReadMsgData(menu->msgData, sBattleBagItemEntryMsgIds[entry][0]);
        BufferItemName(menu->msgFormat, 0, menu->pocketItems[menu->pocket][slot].id);
        StringExpandPlaceholders(menu->msgFormat, menu->string, string);
        width = FontID_String_GetWidth(fontId, menu->string, 0);
        x = (GetWindowWidth(&windows[window]) * 8 - width) / 2;
        ((AddTextPrinterWithColorFunc)AddTextPrinterParameterizedWithColor)(&windows[window], fontId, menu->string, x, 7, TEXT_SPEED_NOTRANSFER, color, NULL);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(&windows[window]);
}

void BattleBagMenu_PrintItemQuantity(BattleBagMenu *menu, int slot, int entry, int window, u32 fontId, int y, int color) {
    Window *windows = menu->pageWindows;
    String *string;

    FillWindowPixelBuffer(&windows[window], 0);
    if (menu->pocketItems[menu->pocket][slot].quantity) {
        string = NewString_ReadMsgData(menu->msgData, sBattleBagItemEntryQuantityMsgIds[entry][0]);
        BufferIntegerAsString(menu->msgFormat, 0, menu->pocketItems[menu->pocket][slot].quantity, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(menu->msgFormat, menu->string, string);
        ((AddTextPrinterWithColorFunc)AddTextPrinterParameterizedWithColor)(&windows[window], fontId, menu->string, 0, y, TEXT_SPEED_NOTRANSFER, color, NULL);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(&windows[window]);
}

void BattleBagMenu_PrintItemListEntry(BattleBagMenu *menu, int entry) {
    int firstSlot = menu->context->pocketPages[menu->pocket] * 6;
    int firstWindow;

    if (menu->itemListWindowBank == 0) {
        firstWindow = 0;
    } else {
        firstWindow = 12;
    }
    BattleBagMenu_PrintItemName(menu, entry + firstSlot, entry, firstWindow + entry * 2, 4, MAKE_TEXT_COLOR(3, 2, 1));
    BattleBagMenu_PrintItemQuantity(menu, entry + firstSlot, entry, firstWindow + 1 + entry * 2, 0, 4, MAKE_TEXT_COLOR(1, 2, 0));
}

void BattleBagMenu_PrintItemList(BattleBagMenu *menu) {
    u16 i;

    FillBgTilemapRect(menu->bgConfig, 5, 0, 0, 0, 0x20, 0x13, 0x11);
    for (i = 0; i < 6; i++) {
        BattleBagMenu_PrintItemListEntry(menu, i);
    }
    menu->itemListWindowBank ^= 1;
}

#define PAGE_NUMBER_WINDOW(windows) ((Window *)((u32)(windows) + 25 * sizeof(Window)))

void BattleBagMenu_PrintPageNumber(BattleBagMenu *menu) {
    Window *windows;
    String *slash, *lastPage, *currentPage;
    u32 width, x;

    FillWindowPixelBuffer(&menu->pageWindows[25], 0);
    windows = menu->pageWindows;
    slash = NewString_ReadMsgData(menu->msgData, 28);
    width = FontID_String_GetWidth(0, slash, 0);
    // the window is addressed through an integer offset (pointer indexing changes the code)
    x = (GetWindowWidth(PAGE_NUMBER_WINDOW(windows)) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(PAGE_NUMBER_WINDOW(windows), 0, slash, x, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(slash);
    lastPage = NewString_ReadMsgData(menu->msgData, 29);
    BufferIntegerAsString(menu->msgFormat, 0, menu->lastPages[menu->pocket] + 1, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, menu->string, lastPage);
    AddTextPrinterParameterizedWithColor(PAGE_NUMBER_WINDOW(windows), 0, menu->string, x + width, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(lastPage);
    currentPage = NewString_ReadMsgData(menu->msgData, 30);
    BufferIntegerAsString(menu->msgFormat, 0, menu->context->pocketPages[menu->pocket] + 1, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(menu->msgFormat, menu->string, currentPage);
    AddTextPrinterParameterizedWithColor(PAGE_NUMBER_WINDOW(windows), 0, menu->string, x - FontID_String_GetWidth(0, menu->string, 0), 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(currentPage);
    ScheduleWindowCopyToVram(PAGE_NUMBER_WINDOW(windows));
}

void BattleBagMenu_PrintPocketName(BattleBagMenu *menu) {
    FillWindowPixelBuffer(&menu->pageWindows[24], 0);
    switch (menu->pocket) {
    case 0:
        BattleBagMenu_PrintCenteredText(menu, 24, 22, 0, 4, MAKE_TEXT_COLOR(1, 2, 0));
        BattleBagMenu_PrintCenteredText(menu, 24, 23, 0, 0x14, MAKE_TEXT_COLOR(1, 2, 0));
        break;
    case 1:
        BattleBagMenu_PrintCenteredText(menu, 24, 24, 0, 4, MAKE_TEXT_COLOR(1, 2, 0));
        BattleBagMenu_PrintCenteredText(menu, 24, 25, 0, 0x14, MAKE_TEXT_COLOR(1, 2, 0));
        break;
    case 2:
        BattleBagMenu_PrintCenteredText(menu, 24, 26, 0, 0xC, MAKE_TEXT_COLOR(1, 2, 0));
        break;
    case 3:
        BattleBagMenu_PrintCenteredText(menu, 24, 27, 0, 0xC, MAKE_TEXT_COLOR(1, 2, 0));
        break;
    }
}

void BattleBagMenu_PrintItemListPage(BattleBagMenu *menu) {
    BattleBagMenu_PrintItemList(menu);
    BattleBagMenu_PrintPocketName(menu);
    BattleBagMenu_PrintPageNumber(menu);
}

void BattleBagMenu_PrintUseItemName(BattleBagMenu *menu, int slot) {
    Window *window = menu->pageWindows;
    String *string = NewString_ReadMsgData(menu->msgData, 9);
    ItemSlot *items = menu->pocketItems[menu->pocket];
    BufferItemName(menu->msgFormat, 0, items[slot].id);
    StringExpandPlaceholders(menu->msgFormat, menu->string, string);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, menu->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void BattleBagMenu_PrintItemDescription(BattleBagMenu *menu, int slot) {
    Window *window = &menu->pageWindows[2];
    String *string = String_New(130, menu->context->heapID);
    // the heap id is passed truncated to 16 bits
    GetItemDescIntoString(string, menu->pocketItems[menu->pocket][slot].id, (enum HeapID)(u16)menu->context->heapID);
    AddTextPrinterParameterizedWithColor(window, 0, string, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void BattleBagMenu_PrintUseItemPage(BattleBagMenu *menu) {
    u32 i;
    u8 position, page;

    for (i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&menu->pageWindows[i], 0);
    }
    position = menu->context->pocketPositions[menu->pocket];
    page = menu->context->pocketPages[menu->pocket];
    BattleBagMenu_PrintUseItemName(menu, position + page * 6);
    BattleBagMenu_PrintItemQuantity(menu, position + page * 6, 0, 1, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    BattleBagMenu_PrintItemDescription(menu, position + page * 6);
    BattleBagMenu_PrintCenteredText(menu, 3, 31, 4, 5, MAKE_TEXT_COLOR(3, 2, 1));
}

void BattleBagMenu_ShowMessage(BattleBagMenu *menu) {
    DrawFrameAndWindow2(&menu->messageWindow, TRUE, 0x3E2, 14);
    FillWindowPixelBuffer(&menu->messageWindow, 15);
    BattleBagMenu_PrintMessage(menu);
}

void BattleBagMenu_PrintMessage(BattleBagMenu *menu) {
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    menu->textPrinterId = AddTextPrinterParameterized(&menu->messageWindow, 1, menu->string, 0, 0, BattleSystem_GetTextFrameDelay(menu->context->battleSystem), NULL);
}

BOOL BattleBagMenu_ValidateLastUsedItem(BattleBagMenu *menu) {
    if (menu->context->lastUsedItem == ITEM_NONE) {
        return FALSE;
    }
    if (!Bag_HasItem(menu->context->bag, menu->context->lastUsedItem, 1, menu->context->heapID)) {
        menu->context->lastUsedItem = ITEM_NONE;
        menu->context->lastUsedPocket = 0;
        return FALSE;
    }
    return TRUE;
}

void BattleBagMenu_SelectLastUsedItem(BattleBagMenu *menu) {
    u8 pocket = menu->pocket;
    BattleBagContext *context = menu->context;
    u32 i = 0;
    // Steps a view of the menu shifted to the pocket's row by one slot per item, so that
    // shifted->pocketItems[0][0] is menu->pocketItems[pocket][i] (indexing directly allocates registers differently).
    BattleBagMenu *shifted = (BattleBagMenu *)((u8 *)menu + pocket * sizeof(menu->pocketItems[0]));
    u16 item = context->lastUsedItem;

    for (; i < 36; i++, shifted = (BattleBagMenu *)((u8 *)shifted + sizeof(ItemSlot))) {
        if (item == shifted->pocketItems[0][0].id) {
            context->pocketPositions[pocket] = i % 6;
            menu->context->pocketPages[menu->pocket] = i / 6;
            return;
        }
    }
}

static inline void AddItemToBattlePockets(BattleBagMenu *menu, ItemSlot *item) {
    u32 i;
    u32 battlePockets = GetItemAttr(item->id, ITEMATTR_BATTLE_POCKET, menu->context->heapID);

    for (i = 0; i < 5; i++) {
        if (battlePockets & (1 << i)) {
            menu->pocketItems[sBattleBagPocketOrder[i]][menu->itemCounts[sBattleBagPocketOrder[i]]] = *item;
            menu->itemCounts[sBattleBagPocketOrder[i]]++;
        }
    }
}

// This function was built against a Bag_GetPocketSlotN prototype taking 16-bit pocket and slot numbers.
typedef ItemSlot *(*BagGetPocketSlotFunc)(Bag *bag, u16 pocket, u16 slot);

void BattleBagMenu_SortItemsIntoPockets(BattleBagMenu *menu) {
    u32 pocket, slot, i;
    ItemSlot *item;

    for (pocket = 0; pocket < POCKETS_COUNT; pocket++) {
        for (slot = 0;; slot++) {
            item = ((BagGetPocketSlotFunc)Bag_GetPocketSlotN)(menu->context->bag, pocket, slot);
            if (item == NULL) {
                break;
            }
            if (item->id != ITEM_NONE && item->quantity != 0) {
                AddItemToBattlePockets(menu, item);
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (menu->itemCounts[i] == 0) {
            menu->lastPages[i] = 0;
        } else {
            menu->lastPages[i] = (menu->itemCounts[i] - 1) / 6;
        }
        if (menu->lastPages[i] < menu->context->pocketPages[i]) {
            menu->context->pocketPages[i] = menu->lastPages[i];
        }
    }
}

u16 BattleBagMenu_GetItemOnPage(BattleBagMenu *menu, u32 position) {
    u32 pocket = menu->pocket;
    u32 slot = position + 6 * menu->context->pocketPages[pocket];
    u16 item = menu->pocketItems[pocket][slot].id;

    if (item == ITEM_NONE || menu->pocketItems[pocket][slot].quantity == 0) {
        item = ITEM_NONE;
    }
    return item;
}

void BattleBagMenu_InitSprites(BattleBagMenu *menu) {
    BattleBagMenu_CreateSpriteManager(menu);
    BattleBagMenu_LoadItemIconResources(menu);
    BattleBagMenu_CreateSprites(menu);
    BattleBagMenu_CreateCursor(menu);
    BattleBagMenu_CreateFinger(menu);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void BattleBagMenu_CreateSpriteManager(BattleBagMenu *menu) {
    SpriteResourceCountsListUnion capacities = sBattleBagSpriteCapacities;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    menu->spriteManager = SpriteManager_New(spriteSystem);
    SpriteSystem_InitSprites(spriteSystem, menu->spriteManager, 12);
    SpriteSystem_InitManagerWithCapacities(spriteSystem, menu->spriteManager, &capacities);
}

void BattleBagMenu_LoadItemIconResources(BattleBagMenu *menu) {
    NARC *narc = NARC_New(NARC_itemtool_itemdata_item_icon, menu->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    u32 i;
    int resId;

    for (i = 0; i < 6; i++) {
        resId = 0xB4B7 + i;
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetItemIndexMapping(ITEM_MASTER_BALL, ITEMNARC_NCGR), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, resId);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(menu->paletteData, PLTTBUF_SUB_OBJ, spriteSystem, menu->spriteManager, narc, GetItemIndexMapping(ITEM_MASTER_BALL, ITEMNARC_NCLR), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, resId);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetItemIconCell(), FALSE, 0xB4B7);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, menu->spriteManager, narc, GetItemIconAnim(), FALSE, 0xB4B7);
    NARC_Delete(narc);
}

void BattleBagMenu_SetItemIconGraphics(BattleBagMenu *menu, u16 item, int resId) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    SpriteSystem_ReplaceCharResObj(spriteSystem, menu->spriteManager, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(item, ITEMNARC_NCGR), FALSE, resId);
}

void BattleBagMenu_SetItemIconPalette(BattleBagMenu *menu, u16 item, u32 slot, int resId) {
    PaletteData_LoadNarc(menu->paletteData, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(item, ITEMNARC_NCLR), menu->context->heapID, PLTTBUF_SUB_OBJ, 0x20, slot << 4);
}

ManagedSprite *BattleBagMenu_CreateSprite(BattleBagMenu *menu, int index) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = sBattleBagSpriteTemplatesPriority[index][0];
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = sBattleBagSpriteTemplates[index][0];
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = sBattleBagSpriteTemplatesPltt[index][0];
    template.resIdList[GF_GFX_RES_TYPE_CELL] = sBattleBagSpriteTemplatesCell[index][0];
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = sBattleBagSpriteTemplatesAnim[index][0];
    template.bgPriority = 1;
    template.vramTransfer = FALSE;
    return SpriteSystem_NewSprite(spriteSystem, menu->spriteManager, &template);
}

void BattleBagMenu_CreateSprites(BattleBagMenu *menu) {
    u32 i;

    for (i = 0; i < 6; i++) {
        menu->itemIcons[i] = BattleBagMenu_CreateSprite(menu, i);
    }
}

void BattleBagMenu_FreeSprites(BattleBagMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    u32 i;

    for (i = 0; i < 6; i++) {
        Sprite_DeleteAndFreeResources(menu->itemIcons[i]);
    }
    BattleBagMenu_FreeCursor(menu);
    BattleBagMenu_FreeFinger(menu);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, menu->spriteManager);
}

void BattleBagMenu_ShowSpriteAt(ManagedSprite *sprite, int x, int y) {
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetPositionXY(sprite, x, y);
}

void BattleBagMenu_ShowPageSprites(BattleBagMenu *menu, u32 page) {
    u32 i;

    for (i = 0; i < 6; i++) {
        ManagedSprite_SetDrawFlag(menu->itemIcons[i], FALSE);
    }
    switch (page) {
    case 0:
        BattleBagMenu_ShowLastUsedItemIcon(menu);
        break;
    case 1:
        BattleBagMenu_ShowItemListIcons(menu);
        break;
    case 2:
        BattleBagMenu_ShowUseItemIcon(menu);
        break;
    }
}

void BattleBagMenu_ShowLastUsedItemIcon(BattleBagMenu *menu) {
    u16 item = menu->context->lastUsedItem;

    if (item != ITEM_NONE) {
        BattleBagMenu_SetItemIconGraphics(menu, item, 0xB4B7);
        BattleBagMenu_SetItemIconPalette(menu, menu->context->lastUsedItem, 0, 0xB4B7);
        BattleBagMenu_ShowSpriteAt(menu->itemIcons[0], 24, 178);
    }
}

void BattleBagMenu_ShowItemListIcons(BattleBagMenu *menu) {
    u32 i;

    for (i = 0; i < 6; i++) {
        u16 item = BattleBagMenu_GetItemOnPage(menu, i);
        if (item != ITEM_NONE) {
            BattleBagMenu_SetItemIconGraphics(menu, item, 0xB4B7 + i);
            BattleBagMenu_SetItemIconPalette(menu, item, (u16)i, 0xB4B7 + i);
            BattleBagMenu_ShowSpriteAt(menu->itemIcons[i], sBattleBagItemListIconPos[i][0], sBattleBagItemListIconPos[i][1]);
        }
    }
}

void BattleBagMenu_ShowUseItemIcon(BattleBagMenu *menu) {
    u16 item = BattleBagMenu_GetItemOnPage(menu, menu->context->pocketPositions[menu->pocket]);

    BattleBagMenu_SetItemIconGraphics(menu, item, 0xB4B7);
    BattleBagMenu_SetItemIconPalette(menu, item, 0, 0xB4B7);
    BattleBagMenu_ShowSpriteAt(menu->itemIcons[0], 40, 44);
}

void BattleBagMenu_CreateCursor(BattleBagMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    BattleCursor_LoadResources(spriteSystem, menu->spriteManager, menu->paletteData, menu->context->heapID, 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
    BattleDpadCursor_SetBattleCursor(menu->cursor, BattleCursor_New(spriteSystem, menu->spriteManager, menu->context->heapID, 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9, 0, 1));
}

void BattleBagMenu_FreeCursor(BattleBagMenu *menu) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(menu->cursor));
    BattleCursor_FreeResources(menu->spriteManager, 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
}

void BattleBagMenu_SetPageCursor(BattleBagMenu *menu, int page) {
    BattleDpadCursor_SetBoxes(menu->cursor, sBattleBagPageCursorBoxes[page]);
    switch (page) {
    case 0:
        BattleDpadCursor_SetPosition(menu->cursor, menu->pocket);
        break;
    case 1:
        BattleDpadCursor_SetPosition(menu->cursor, menu->context->pocketPositions[menu->pocket]);
        break;
    case 2:
        break;
    }
}

void BattleBagMenu_DeactivateCursor(BattleBagMenu *menu) {
    BattleDpadCursor *cursor = menu->cursor;
    BattleDpadCursor_SetActive(cursor, 0);
    BattleDpadCursor_ResetPosition(menu->cursor);
    BattleCursor_Disable(BattleDpadCursor_GetBattleCursor(menu->cursor));
}

#pragma force_active reset

void BattleBagMenu_CreateFinger(BattleBagMenu *menu) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(menu->context->battleSystem);
    BattleFinger_LoadResources(spriteSystem, menu->spriteManager, menu->context->heapID, menu->paletteData, 0xB4BD, 0xB4BD, 0xB4BD - 5, 0xB4BD - 5);
    {
        u32 resId = 0xB4BD;
        menu->finger = BattleFinger_New(spriteSystem, menu->spriteManager, menu->context->heapID, resId, resId, resId - 5, resId - 5, 0, 0);
    }
}

void BattleBagMenu_FreeFinger(BattleBagMenu *menu) {
    BattleFinger_Delete(menu->finger);
    BattleFinger_FreeResources(menu->spriteManager, 0xB4BD, 0xB4BD, 0xB4B8, 0xB4B8);
}

void BattleBagMenu_UpdateTutorialFinger(BattleBagMenu *menu, int page) {
    if (menu->context->isTutorial == 1) {
        ov12_0226BD2C(menu->finger, sBattleBagTutorialFingerPos[page][0], sBattleBagTutorialFingerPosY[page][0]);
        ov12_0226BD4C(menu->finger, 60);
    } else {
        BattleFinger_Disable(menu->finger);
    }
}

#pragma force_active on

void BattleBagMenu_StoreButtonTilemaps(BattleBagMenu *menu, const u16 *screen) {
    BattleBagMenu_CopyTilemapRect(menu->pocketButtonTiles[0], screen, 0, 0, 16, 9);
    BattleBagMenu_CopyTilemapRect(menu->pocketButtonTiles[1], screen, 0, 9, 16, 9);
    BattleBagMenu_CopyTilemapRect(menu->pocketButtonTiles[2], screen, 0, 18, 16, 9);
    BattleBagMenu_CopyTilemapRect(menu->lastUsedButtonTiles[0], screen, 0, 27, 26, 5);
    BattleBagMenu_CopyTilemapRect(menu->lastUsedButtonTiles[1], screen, 0, 32, 26, 5);
    BattleBagMenu_CopyTilemapRect(menu->lastUsedButtonTiles[2], screen, 0, 37, 26, 5);
    BattleBagMenu_CopyTilemapRect(menu->lastUsedButtonTiles[3], screen, 0, 42, 26, 5);
    BattleBagMenu_CopyTilemapRect(menu->cancelButtonTiles[0], screen, 0, 57, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->cancelButtonTiles[1], screen, 5, 57, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->cancelButtonTiles[2], screen, 10, 57, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->itemButtonTiles[0], screen, 16, 0, 16, 6);
    BattleBagMenu_CopyTilemapRect(menu->itemButtonTiles[1], screen, 16, 6, 16, 6);
    BattleBagMenu_CopyTilemapRect(menu->itemButtonTiles[2], screen, 16, 12, 16, 6);
    BattleBagMenu_CopyTilemapRect(menu->itemButtonTiles[3], screen, 16, 18, 16, 6);
    BattleBagMenu_CopyTilemapRect(menu->prevPageButtonTiles[0], screen, 0, 47, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->prevPageButtonTiles[1], screen, 5, 47, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->prevPageButtonTiles[2], screen, 10, 47, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->prevPageButtonTiles[3], screen, 15, 47, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->nextPageButtonTiles[0], screen, 0, 52, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->nextPageButtonTiles[1], screen, 5, 52, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->nextPageButtonTiles[2], screen, 10, 52, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->nextPageButtonTiles[3], screen, 15, 52, 5, 5);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[0][0], screen, 20, 47, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[0][1], screen, 24, 47, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[0][2], screen, 28, 47, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[1][0], screen, 20, 51, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[1][1], screen, 24, 51, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[1][2], screen, 28, 51, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[2][0], screen, 20, 55, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[2][1], screen, 24, 55, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[2][2], screen, 28, 55, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[3][0], screen, 20, 59, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[3][1], screen, 24, 59, 4, 4);
    BattleBagMenu_CopyTilemapRect(menu->pocketIconTiles[3][2], screen, 28, 59, 4, 4);
}

#pragma force_active reset

void BattleBagMenu_CopyTilemapRect(u16 *dst, const u16 *src, u16 x, u16 y, u8 width, u8 height) {
    u16 i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            dst[i * width + j] = src[(y + i) * 32 + j + x];
        }
    }
}

u16 *BattleBagMenu_GetButtonTilemap(BattleBagMenu *menu, u32 button, u32 frame) {
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
        return menu->pocketButtonTiles[frame];
    case 4:
        return menu->lastUsedButtonTiles[frame];
    case 5:
    case 14:
    case 16:
        return menu->cancelButtonTiles[frame];
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return menu->itemButtonTiles[frame];
    case 12:
        return menu->prevPageButtonTiles[frame];
    case 13:
        return menu->nextPageButtonTiles[frame];
    case 15:
        return menu->lastUsedButtonTiles[frame];
    default:
        return NULL;
    }
}

u16 BattleBagMenu_GetButtonPalette(BattleBagMenu *menu, u32 button, u32 frame, u32 page) {
    if (frame == 3) {
        return 5;
    }
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 0;
    case 4:
        return 3;
    case 5:
    case 12:
    case 13:
    case 14:
    case 16:
        return 2;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return (u16)(menu->pocket + 8);
    case 15:
        if (page == 2) {
            return (u16)(menu->pocket + 8);
        }
        return 1;
    default:
        return 0;
    }
}

void BattleBagMenu_CopyPocketIconTiles(BattleBagMenu *menu, u16 *dst, u32 button, u32 frame) {
    u16 *src;
    u16(*rows)[16] = (u16(*)[16])dst;
    u16 i, j;

    if (button == 0) {
        src = menu->pocketIconTiles[0][frame];
    } else if (button == 1) {
        src = menu->pocketIconTiles[1][frame];
    } else if (button == 2) {
        src = menu->pocketIconTiles[2][frame];
    } else if (button == 3) {
        src = menu->pocketIconTiles[3][frame];
    } else {
        return;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            rows[i][j + 6] = src[i * 4 + j];
        }
    }
}

#pragma force_active on

void BattleBagMenu_BuildButtonTilemap(BattleBagMenu *menu, u16 *dst, u8 button, u8 frame, u8 page) {
    u16 *src = BattleBagMenu_GetButtonTilemap(menu, button, frame);
    u16 palette = BattleBagMenu_GetButtonPalette(menu, button, frame, page) << 12;
    u16 i;

    for (i = 0; i < sBattleBagButtonRects[button].width * sBattleBagButtonRects[button].height; i++) {
        dst[i] = (src[i] & 0xFFF) | palette;
    }
    BattleBagMenu_CopyPocketIconTiles(menu, dst, button, frame);
}

void BattleBagMenu_DrawButton(BattleBagMenu *menu, u8 button, u8 frame, u8 page) {
    // the table is read here as bytes, through labels on its members
    int index = button * 4;
    int height = sBattleBagButtonRectsHeight[index];
    int width = sBattleBagButtonRectsWidth[index];
    u16 *buffer = Heap_Alloc(menu->context->heapID, width * height * 2);
    BattleBagMenu_BuildButtonTilemap(menu, buffer, button, frame, page);
    LoadRectToBgTilemapRect(menu->bgConfig, 6, buffer, ((const u8 *)sBattleBagButtonRects)[index], sBattleBagButtonRectsY[index], width, height);
    ScheduleBgTilemapBufferTransfer(menu->bgConfig, 6);
    Heap_Free(buffer);
}

void BattleBagMenu_ScrollButtonWindows(BattleBagMenu *menu, u8 button, int frame) {
    const u8 *windowList;
    u8 direction, pixels;
    u16 i;

    if (button >= 6 && button <= 11 && menu->itemListWindowBank == 0) {
        windowList = sBattleBagButtonWindowLists[button + 11];
    } else {
        windowList = sBattleBagButtonWindowLists[button];
    }
    if (!windowList) {
        return;
    }
    switch (frame) {
    case 0:
    case 2:
        direction = 1;
        pixels = 2;
        break;
    case 1:
        direction = 0;
        pixels = 4;
        break;
    }
    for (i = 0; i < 8; i++) {
        if (windowList[i] == 0xFF) {
            break;
        }
        ScrollWindow(&menu->pageWindows[windowList[i]], direction, pixels, 0);
        ScheduleWindowCopyToVram(&menu->pageWindows[windowList[i]]);
    }
}

void BattleBagMenu_OffsetButtonSprite(BattleBagMenu *menu, u32 button, u32 frame) {
    ManagedSprite *sprite;
    u8 i;

    if (button >= 6 && button <= 11) {
        sprite = menu->itemIcons[(int)(button - 6)];
    } else if (button == 4) {
        for (i = 0; i < 6; i++) {
            sprite = menu->itemIcons[i];
            if (ManagedSprite_GetDrawFlag(sprite)) {
                break;
            }
        }
    } else {
        return;
    }
    switch (frame) {
    case 0:
    case 2:
        ManagedSprite_OffsetPositionXY(sprite, 0, 2);
        break;
    case 1:
        ManagedSprite_OffsetPositionXY(sprite, 0, -4);
        break;
    }
}

#pragma force_active reset

void BattleBagMenu_StartButtonAnim(BattleBagMenu *menu, u8 button, u8 mode) {
    menu->buttonAnimState = 0;
    menu->buttonAnimTimer = 0;
    menu->buttonAnimButton = button;
    menu->unk_1141_4 = mode;
    menu->buttonAnimActive = TRUE;
}

#pragma force_active on

void BattleBagMenu_UpdateButtonAnim(BattleBagMenu *menu) {
    if (menu->buttonAnimActive) {
        switch (menu->buttonAnimState) {
        case 0:
            BattleBagMenu_DrawButton(menu, menu->buttonAnimButton, 1, menu->page);
            BattleBagMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 1);
            BattleBagMenu_OffsetButtonSprite(menu, menu->buttonAnimButton, 1);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = 1;
            break;
        case 1:
            BattleBagMenu_DrawButton(menu, menu->buttonAnimButton, 2, menu->page);
            BattleBagMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 2);
            BattleBagMenu_OffsetButtonSprite(menu, menu->buttonAnimButton, 2);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = 2;
            break;
        case 2:
            BattleBagMenu_DrawButton(menu, menu->buttonAnimButton, 0, menu->page);
            BattleBagMenu_ScrollButtonWindows(menu, menu->buttonAnimButton, 0);
            BattleBagMenu_OffsetButtonSprite(menu, menu->buttonAnimButton, 0);
            menu->buttonAnimTimer = 0;
            menu->buttonAnimState = 0;
            menu->buttonAnimActive = FALSE;
            break;
        }
    }
}

void BattleBagMenu_DrawPageButtons(BattleBagMenu *menu, u8 page) {
    u32 i;

    switch (page) {
    case 0:
        BattleBagMenu_DrawButton(menu, 0, 0, page);
        BattleBagMenu_DrawButton(menu, 1, 0, page);
        BattleBagMenu_DrawButton(menu, 2, 0, page);
        BattleBagMenu_DrawButton(menu, 3, 0, page);
        if (menu->context->lastUsedItem == ITEM_NONE) {
            BattleBagMenu_DrawButton(menu, 4, 3, page);
        } else {
            BattleBagMenu_DrawButton(menu, 4, 0, page);
        }
        BattleBagMenu_DrawButton(menu, 5, 0, page);
        break;
    case 1:
        for (i = 0; i < 6; i++) {
            if (BattleBagMenu_GetItemOnPage(menu, i) == ITEM_NONE) {
                BattleBagMenu_DrawButton(menu, i + 6, 3, page);
            } else {
                BattleBagMenu_DrawButton(menu, i + 6, 0, page);
            }
        }
        if (menu->lastPages[menu->pocket] == 0) {
            BattleBagMenu_DrawButton(menu, 12, 3, page);
            BattleBagMenu_DrawButton(menu, 13, 3, page);
        } else {
            BattleBagMenu_DrawButton(menu, 12, 0, page);
            BattleBagMenu_DrawButton(menu, 13, 0, page);
        }
        BattleBagMenu_DrawButton(menu, 14, 0, page);
        break;
    case 2:
        BattleBagMenu_DrawButton(menu, 15, 0, page);
        BattleBagMenu_DrawButton(menu, 16, 0, page);
        break;
    }
}
