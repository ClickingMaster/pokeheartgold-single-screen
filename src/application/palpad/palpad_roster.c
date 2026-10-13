#include "global.h"

#include "constants/sndseq.h"

#include "application/palpad/palpad_internal.h"

#include "brightness.h"
#include "gf_g2d_renderer.h"
#include "list_menu_items.h"
#include "options.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "frontier_save_access.h"

// input given to PalPadRoster_HandleAction
enum PalPadRosterInput {
    ROSTER_INPUT_BACK,
    ROSTER_INPUT_PAGE_LEFT,
    ROSTER_INPUT_PAGE_RIGHT,
    ROSTER_INPUT_SELECT = 5,
};

// result of the roster input handlers
enum PalPadRosterAction {
    ROSTER_ACTION_NONE,
    ROSTER_ACTION_PAGE_LEFT,
    ROSTER_ACTION_PAGE_RIGHT,
    ROSTER_ACTION_EXIT,
    ROSTER_ACTION_OPEN_MENU,
};

#define ROSTER_CURSOR_RETURN 8
#define ROSTER_CURSOR_HIDDEN 10

typedef struct PalPadSlotRect {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
    u8 bgId;
    u8 palette;
    u8 selectedPalette;
} PalPadSlotRect;

typedef struct PalPadListMenuEntry {
    int msgId;
    int value;
} PalPadListMenuEntry;

typedef void (*PalPadRosterScrollCallback)(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage);

extern const u16 sPalPadRosterPageBaseTiles[3];
extern const PalPadPanelLayout sPalPadPanelLayouts[3];
extern const PalPadListMenuEntry sPalPadRosterFriendMenus[2][4];
extern const SpriteTemplate sPalPadDeleteEffectSpriteTemplate;
extern const TouchscreenHitbox sPalPadRosterHitboxes[];
extern const PalPadSlotRect sPalPadRosterSlotRects[9];

// second label on the table, at sPalPadRosterSlotRects[0].y: some functions read the y coordinates
// through it
typedef struct PalPadSlotRectFromY {
    s16 y;
    u8 unk_02[8];
} PalPadSlotRectFromY;

extern const PalPadSlotRectFromY sPalPadRosterSlotY[9];
extern const u32 sPalPadRosterNameColors[3];

// the template follows the last entry of sPalPadRosterNameColors
extern const struct {
    u32 nameColor;
    TouchscreenListMenuTemplate template;
} sPalPadRosterFriendMenuTemplate;

extern const NNSG2dViewRect sPalPadRenderSurfaceRect;
extern const PalPadRosterScrollCallback sPalPadRosterScrollCallbacks[4];

void PalPadRoster_Show(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_Hide(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
int PalPadRoster_HandleAction(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, int input);
int PalPadRoster_HandleKeys(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
int PalPadRoster_HandleTouch(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, BOOL *touched);
int PalPadRoster_HandleInput(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_StartPageScroll(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
BOOL PalPadRoster_UpdatePageScroll(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_OpenFriendMenu(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadRoster_HandleFriendMenu(PalPadRoster *roster, u32 *inputState, PalPadGfx *gfx);
BOOL PalPadRoster_ShowFriendCode(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_ConfirmDelete(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
BOOL PalPadRoster_StartDeletePrompt(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_DrawPage(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, int page, int cursor, int panel, enum HeapID heapId);
void PalPadRoster_DrawTabs(PalPadRoster *roster, PalPadGfx *gfx, int page);
void PalPadRoster_StartScroll(PalPadRoster *roster, PalPadShared *shared, u16 dir);
void PalPadRoster_PrintMessage(PalPadRoster *roster, PalPadGfx *gfx, int msgId, enum HeapID heapId);
BOOL PalPadRoster_IsMessageDone(PalPadRoster *roster, u32 *inputState);
void PalPadRoster_ClearMessage(PalPadRoster *roster);
int PalPadRoster_GetSelectedIndex(PalPadRoster *roster);
void PalPadRoster_MenuCallback(TouchscreenListMenu *menu, u8 index, void *data, int event);
void PalPadRoster_StartDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx);
BOOL PalPadRoster_UpdateDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx);
void PalPadRoster_EndDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx);
void PalPadRosterPage_Init(PalPadRosterPage *page, PalPadGfx *gfx, const PalPadPanelLayout *layout, int baseTile, enum HeapID heapId);
void PalPadRosterPage_Free(PalPadRosterPage *page, PalPadGfx *gfx);
void PalPadRosterPage_DrawFrame(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens);
void PalPadRoster_SetCursor(PalPadRosterPage *page, PalPadGfx *gfx, int cursor);
void PalPadRosterPage_HighlightSlot(PalPadRosterPage *page, PalPadGfx *gfx, int cursor, int highlight);
void PalPadRosterPage_Draw(PalPadRosterPage *page, PalPadShared *shared, PalPadGfx *gfx, CharSpriteManager *trainerIcons, PalPadScreens *screens, PalPadFriendList *friends, int pageNo, enum HeapID heapId);
void PalPadRosterPage_Clear(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens);
void PalPadRosterPage_DeleteIcons(PalPadRosterPage *page, PalPadGfx *gfx);
void PalPadRosterPage_DrawFriend(PalPadRosterPage *page, CharSpriteManager *trainerIcons, PalPadGfx *gfx, PalPadScreens *screens, int slot, FriendRoster *wifiList, int friendSlot, enum HeapID heapId);
void PalPadRosterPage_DrawEmptySlot(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens, int slot);

static inline int Abs16(s16 value) {
    return value < 0 ? -value : value;
}

static inline int AddTiles(int a, int b) {
    return a + b;
}

void PalPadRoster_Init(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    roster->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(shared->saveData));
    PalPadRoster_Setup(roster, shared, gfx, heapId);
}

#pragma force_active on

int PalPadRoster_Main(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int index;
    FriendRoster *wifiList;

    switch (shared->subState) {
    case 0:
        roster->page = 0;
        roster->prevPage = 0;
        roster->cursor = 0;
        roster->prevCursor = 0;
        // fallthrough
    case 1:
        PalPadShared_ReadFriendList(shared);
        PalPadRoster_Show(roster, shared, gfx, heapId);
        shared->subState = 2;
        break;
    case 2:
        switch (PalPadRoster_HandleInput(roster, shared, gfx)) {
        case ROSTER_ACTION_NONE:
            break;
        case ROSTER_ACTION_PAGE_LEFT:
            PalPadRoster_StartScroll(roster, shared, 2);
            break;
        case ROSTER_ACTION_PAGE_RIGHT:
            PalPadRoster_StartScroll(roster, shared, 3);
            break;
        case ROSTER_ACTION_EXIT:
            shared->subState = 23;
            break;
        case ROSTER_ACTION_OPEN_MENU:
            roster->timer = 4;
            shared->subState = 5;
            break;
        }
        break;
    case 3:
        PalPadRoster_StartPageScroll(roster, shared, gfx, heapId);
        shared->subState = 4;
        break;
    case 4:
        if (PalPadRoster_UpdatePageScroll(roster, shared, gfx, heapId) == TRUE) {
            shared->subState = 2;
        }
        break;
    case 5:
        if (roster->timer-- <= 0) {
            roster->timer = 0;
            PalPadShared_SetSelectedFriend(shared, PalPadRoster_GetSelectedIndex(roster));
            PalPadRoster_OpenFriendMenu(roster, shared, gfx, heapId);
            shared->subState = 6;
        }
        break;
    case 6: // returns the state of the chosen item
        shared->subState = PalPadRoster_HandleFriendMenu(roster, &shared->inputState, gfx);
        break;
    case 7: // details
        roster->timer = 4;
        shared->subState = 8;
        break;
    case 8:
        roster->timer--;
        if (roster->timer <= 0) {
            PalPadRoster_Hide(roster, shared, gfx);
            PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_DETAILS, 0);
            return 1;
        }
        break;
    case 9: // back from the details
        roster->page = shared->selectedFriend / PALPAD_ROSTER_PAGE_SLOT;
        roster->cursor = shared->selectedFriend % PALPAD_ROSTER_PAGE_SLOT;
        shared->subState = 1;
        break;
    case 10: // rewrite name
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)16, (enum FadeType)16, RGB_BLACK, 6, 1, heapId);
        index = PalPadRoster_GetSelectedIndex(roster);
        wifiList = Save_FriendRoster_Get(shared->saveData);
        PalPadShared_SetInitialName(shared, FriendRoster_GetPlayerName(wifiList, shared->friends.slots[index]));
        shared->subState = 11;
        break;
    case 11:
        if (IsPaletteFadeFinished()) {
            PalPadRoster_Hide(roster, shared, gfx);
            PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_NAMING, 4);
            return 1;
        }
        break;
    case 12: // back from the naming screen with a new name
        index = PalPadRoster_GetSelectedIndex(roster);
        wifiList = Save_FriendRoster_Get(shared->saveData);
        FriendRoster_SetPlayerName(wifiList, shared->friends.slots[index], shared->name);
        // fallthrough
    case 13:
        PalPadRoster_Show(roster, shared, gfx, heapId);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)17, (enum FadeType)17, RGB_BLACK, 6, 1, heapId);
        shared->subState = 14;
        break;
    case 14:
        if (IsPaletteFadeFinished()) {
            shared->subState = 2;
        }
        break;
    case 15: // delete
        PalPadRoster_ConfirmDelete(roster, shared, gfx, heapId);
        shared->subState = 16;
        break;
    case 16:
        if (PalPadRoster_StartDeletePrompt(roster, shared, gfx) == TRUE) {
            shared->subState = 17;
        }
        break;
    case 17:
        switch (PalPadGfx_HandleYesNo(&shared->inputState, gfx)) {
        case YESNORESPONSE_YES:
            PalPadRoster_ClearMessage(roster);
            shared->subState = 18;
            break;
        case YESNORESPONSE_NO:
            PalPadRoster_ClearMessage(roster);
            PalPadGfx_SetArrowsVisible(gfx, TRUE);
            PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, FALSE);
            shared->subState = 2;
            break;
        }
        break;
    case 18:
        PalPadRoster_StartDeleteEffect(roster, gfx);
        shared->subState = 19;
        break;
    case 19:
        if (PalPadRoster_UpdateDeleteEffect(roster, gfx) == TRUE) {
            wifiList = Save_FriendRoster_Get(shared->saveData);
            index = PalPadRoster_GetSelectedIndex(roster);
            FriendRoster_RemoveFriend(wifiList, shared->friends.slots[index]);
            FrontierSave_DeleteRow(Save_Frontier_GetStatic(shared->saveData), shared->friends.slots[index]);
            PalPadShared_ReadFriendList(shared);
            PalPadRoster_DrawPage(roster, shared, gfx, roster->page, roster->cursor, 0, heapId);
            PalPadGfx_SetArrowsVisible(gfx, TRUE);
            shared->subState = 20;
        }
        break;
    case 20:
        PalPadRoster_EndDeleteEffect(roster, gfx);
        shared->subState = 2;
        break;
    case 21: // friend code (friend whose code is not confirmed yet)
        if (PalPadRoster_ShowFriendCode(roster, shared, gfx, heapId) == TRUE) {
            shared->subState = 22;
        } else {
            PalPadGfx_SetArrowsVisible(gfx, TRUE);
            shared->subState = 2;
        }
        break;
    case 22:
        if (PalPadRoster_IsMessageDone(roster, &shared->inputState) && (gSystem.newKeys || System_GetTouchNew())) {
            PalPadGfx_SetArrowsVisible(gfx, TRUE);
            PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, FALSE);
            shared->subState = 2;
            PalPadRoster_ClearMessage(roster);
        }
        break;
    case 23:
        PalPadRoster_Hide(roster, shared, gfx);
        PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_MENU, 2);
        return 1;
    }
    return 0;
}

#pragma force_active reset

void PalPadRoster_Exit(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadRoster_Teardown(roster, shared, gfx);
}

#pragma force_active on

void PalPadRoster_Setup(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    SpriteTemplate template;
    String *string;
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        PalPadRosterPage_Init(&roster->pages[i], gfx, &sPalPadPanelLayouts[i], sPalPadRosterPageBaseTiles[i], heapId);
    }
    roster->trainerIcons = CharSpriteManager_New(gfx->spriteList, NULL, 16, heapId);
    CharSpriteManager_LoadAvatarChars(roster->trainerIcons, 1, 2, heapId);
    PalPad_AddWindow(gfx->bgConfig, &roster->returnWindow, GF_BG_LYR_MAIN_1, 21, 21, 9, 2, 11, 0x14F, 0);
    string = NewString_ReadMsgData(gfx->msgData, 7);
    AddTextPrinterParameterizedWithColor(&roster->returnWindow, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 15, 0), NULL);
    String_Delete(string);
    for (i = 0; i < 2; i++) {
        roster->menuItems[i] = ListMenuItems_New(4, heapId);
        for (j = 0; j < 4; j++) {
            ListMenuItems_AppendFromMsgData(roster->menuItems[i], gfx->msgData, sPalPadRosterFriendMenus[i][j].msgId, sPalPadRosterFriendMenus[i][j].value);
        }
    }
    PalPad_AddWindow(gfx->bgConfig, &roster->msgWindow, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 11, 0x161, 15);
    roster->msgText = String_New(0x80, heapId);
    template = sPalPadDeleteEffectSpriteTemplate;
    template.spriteList = gfx->spriteList;
    template.header = &gfx->spriteHeader;
    template.heapID = heapId;
    roster->deleteEffect = Sprite_CreateAffine(&template);
    Sprite_SetDrawFlag(roster->deleteEffect, FALSE);
}

#pragma force_active reset

void PalPadRoster_Teardown(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    int i;
    PalPadRosterPage *page;

    Sprite_Delete(roster->deleteEffect);
    RemoveWindow(&roster->msgWindow);
    String_Delete(roster->msgText);
    for (i = 0; i < 2; i++) {
        ListMenuItems_Delete(roster->menuItems[i]);
    }
    RemoveWindow(&roster->returnWindow);
    CharSpriteManager_UnloadAvatarChars(roster->trainerIcons);
    CharSpriteManager_Free(roster->trainerIcons);
    for (i = 0, page = roster->pages; i < 3; i++, page++) {
        PalPadRosterPage_Free(page, gfx);
    }
}

void PalPadRoster_Show(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    NNSG2dScreenData *screen;

    PalPadGfx_PrintTitle(gfx, &gfx->titleWindow, 5);
    screen = gfx->screens.data[1];
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, 17, 20, 14, 4, screen->rawData, 3, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleWindowCopyToVram(&roster->returnWindow);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
    PalPadGfx_SetArrowsVisible(gfx, TRUE);
    PalPadRoster_DrawPage(roster, shared, gfx, roster->page, roster->cursor, 0, heapId);
    PalPadGfx_SetCursor(gfx, TRUE, 1);
    PalPadGfx_SetCursorPriority(gfx, 1, 16);
}

void PalPadRoster_Hide(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    int i;
    PalPadRosterPage *page;

    PalPadGfx_ClearTitle(gfx);
    ClearWindowTilemapAndScheduleTransfer(&roster->returnWindow);
    for (i = 0, page = roster->pages; i < 3; i++, page++) {
        PalPadRosterPage_Clear(page, gfx, &gfx->screens);
    }
    PalPadGfx_SetCursor(gfx, FALSE, 0xFF);
    PalPadGfx_SetCursorPriority(gfx, 1, 9);
    PalPadGfx_SetArrowsVisible(gfx, FALSE);
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, 17, 20, 14, 4, gfx->screens.data[1]->rawData, 17, 0, (u8)(gfx->screens.data[1]->screenWidth >> 3), (u8)(gfx->screens.data[1]->screenHeight >> 3));
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, 16, 0, 16, 4, gfx->screens.data[1]->rawData, 16, 4, (u8)(gfx->screens.data[1]->screenWidth >> 3), (u8)(gfx->screens.data[1]->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
}

int PalPadRoster_HandleAction(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, int input) {
    int cursor;
    int page;
    u32 count;

    if (input == ROSTER_INPUT_BACK) {
        PlaySE(SEQ_SE_DP_DECIDE);
        PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, TRUE);
        return ROSTER_ACTION_EXIT;
    }
    if (input == ROSTER_INPUT_SELECT) {
        cursor = roster->cursor;
        if (cursor == ROSTER_CURSOR_RETURN) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, TRUE);
            return ROSTER_ACTION_EXIT;
        }
        page = roster->page;
        count = shared->friends.count;
        if (count <= (u32)(cursor + page * PALPAD_ROSTER_PAGE_SLOT)) {
            return ROSTER_ACTION_NONE;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, TRUE);
        return ROSTER_ACTION_OPEN_MENU;
    } else if (input == ROSTER_INPUT_PAGE_LEFT) {
        PalPadGfx_AnimateArrow(gfx, 0, 1);
        return ROSTER_ACTION_PAGE_LEFT;
    } else if (input == ROSTER_INPUT_PAGE_RIGHT) {
        PalPadGfx_AnimateArrow(gfx, 1, 1);
        return ROSTER_ACTION_PAGE_RIGHT;
    }
    return ROSTER_ACTION_NONE;
}

#pragma force_active on

// Slots 0-3 are the left column, 4-7 the right one, 8 the return button.
int PalPadRoster_HandleKeys(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    u16 prevCursor;
    BOOL moved, turnPage, right;

    if (gSystem.newKeys & PALPAD_KEYS_ANY) {
        shared->inputState = 0;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_SELECT);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        roster->cursor = ROSTER_CURSOR_RETURN;
        PalPadRoster_SetCursor(&roster->pages[0], gfx, roster->cursor);
        return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_BACK);
    }
    prevCursor = roster->cursor;
    moved = FALSE;
    turnPage = FALSE;
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (roster->cursor == ROSTER_CURSOR_RETURN) {
            roster->cursor = roster->prevCursor;
            moved = TRUE;
            PalPadGfx_SetArrowsVisible(gfx, TRUE);
        } else if (roster->cursor < 4) {
            if (roster->cursor > 0) {
                roster->cursor--;
                moved = TRUE;
            }
        } else if (roster->cursor > 4) {
            roster->cursor--;
            moved = TRUE;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (roster->cursor == 3 || roster->cursor == 7) {
            roster->cursor = ROSTER_CURSOR_RETURN;
            moved = TRUE;
        } else if (roster->cursor < 4) {
            if (roster->cursor < 3) {
                roster->cursor++;
                moved = TRUE;
            }
        } else if (roster->cursor < 7) {
            roster->cursor++;
            moved = TRUE;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (roster->cursor != ROSTER_CURSOR_RETURN) {
            if (roster->cursor < 4) {
                roster->cursor += 4;
                moved = TRUE;
                turnPage = TRUE;
                right = FALSE;
            } else {
                roster->cursor -= 4;
                moved = TRUE;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (roster->cursor != ROSTER_CURSOR_RETURN) {
            if (roster->cursor < 4) {
                roster->cursor += 4;
                moved = TRUE;
            } else {
                roster->cursor -= 4;
                moved = TRUE;
                turnPage = TRUE;
                right = TRUE;
            }
        }
    } else if (shared->lrPaging) {
        if (shared->lrPaging && (gSystem.newAndRepeatedKeys & PAD_BUTTON_L)) {
            turnPage = TRUE;
            right = FALSE;
        } else if (shared->lrPaging && (gSystem.newAndRepeatedKeys & PAD_BUTTON_R)) {
            turnPage = TRUE;
            right = TRUE;
        }
    }
    if (turnPage == TRUE) {
        if (right) {
            roster->prevPage = roster->page;
            roster->page = (roster->page + 1) % PALPAD_ROSTER_PAGE_MAX;
            return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_PAGE_RIGHT);
        } else {
            roster->prevPage = roster->page;
            roster->page--;
            if (roster->page < 0) {
                roster->page += PALPAD_ROSTER_PAGE_MAX;
            }
            return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_PAGE_LEFT);
        }
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_BUTTON3);
        PalPadRoster_SetCursor(&roster->pages[0], gfx, roster->cursor);
        roster->prevCursor = prevCursor;
    }
    return ROSTER_ACTION_NONE;
}

#pragma force_active reset

// hitboxes: 0-7 slots, 8 return, 9/10 arrows, 11-14 page tabs
int PalPadRoster_HandleTouch(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, BOOL *touched) {
    u32 hit;
    int delta;

    hit = TouchscreenHitbox_FindHitboxAtTouchNew(sPalPadRosterHitboxes);
    if (hit == (u32)-1) {
        return ROSTER_ACTION_NONE;
    }
    if (hit <= ROSTER_CURSOR_RETURN) {
        *touched = TRUE;
        roster->cursor = hit;
        PlaySE(SEQ_SE_DP_BUTTON3);
        PalPadRoster_SetCursor(&roster->pages[0], gfx, roster->cursor);
        return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_SELECT);
    }
    if (hit == 9) {
        delta = -1;
    } else if (hit == 10) {
        delta = 1;
    } else {
        delta = (s8)((hit - 11) - roster->page);
    }
    if (delta == 0) {
        return ROSTER_ACTION_NONE;
    }
    *touched = TRUE;
    roster->prevPage = roster->page;
    roster->page += delta;
    if (delta >= 0) {
        roster->page %= PALPAD_ROSTER_PAGE_MAX;
        return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_PAGE_RIGHT);
    }
    if (roster->page < 0) {
        roster->page += PALPAD_ROSTER_PAGE_MAX;
    }
    return PalPadRoster_HandleAction(roster, shared, gfx, ROSTER_INPUT_PAGE_LEFT);
}

#pragma force_active on

int PalPadRoster_HandleInput(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    BOOL touched = FALSE;
    int result = PalPadRoster_HandleTouch(roster, shared, gfx, &touched);

    if (touched) {
        shared->inputState = TRUE;
        return result;
    }
    return PalPadRoster_HandleKeys(roster, shared, gfx);
}

#pragma force_active reset

void PalPadRoster_StartPageScroll(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int panel;

    if (roster->scrollDir == 2) {
        panel = 2;
    } else {
        panel = 1;
    }
    PalPadRoster_DrawPage(roster, shared, gfx, roster->page, roster->cursor, panel, heapId);
    PalPadRoster_SetCursor(&roster->pages[0], gfx, ROSTER_CURSOR_HIDDEN);
    PalPadGfx_SetArrowsAnimActive(gfx, FALSE);
    PlaySE(SEQ_SE_DP_SELECT5);
}

#pragma force_active on

BOOL PalPadRoster_UpdatePageScroll(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    NNSG2dViewRect rect;
    int offset;

    if (roster->scrollStep >= 8) {
        ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        rect = sPalPadRenderSurfaceRect;
        GF_SetG2dRendererSurface(&gfx->renderSurface, &rect);
        PalPadRosterPage_Clear(&roster->pages[roster->scrollDir == 2 ? 2 : 1], gfx, &gfx->screens);
        PalPadRoster_DrawPage(roster, shared, gfx, roster->page, roster->cursor, 0, heapId);
        PalPadGfx_SetArrowsAnimActive(gfx, TRUE);
        return TRUE;
    }
    offset = (roster->scrollStep * 256) / 8;
    if (roster->scrollDir == 2) {
        offset = -offset;
    }
    ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, offset);
    ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, offset);
    rect = sPalPadRenderSurfaceRect;
    rect.posTopLeft.x += (u32)offset << FX32_SHIFT;
    GF_SetG2dRendererSurface(&gfx->renderSurface, &rect);
    if (roster->scrollStep % 2) {
        PalPadRosterScrollCallback callback = sPalPadRosterScrollCallbacks[roster->scrollStep / 2];
        if (callback) {
            callback(roster, gfx, roster->page, roster->prevPage);
        }
    }
    roster->scrollStep++;
    return FALSE;
}

void PalPadRoster_OpenFriendMenu(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FriendRoster *wifiList;
    int index, codeState;
    TouchscreenListMenuHeader header;
    u8 *slots;

    wifiList = Save_FriendRoster_Get(shared->saveData);
    index = PalPadRoster_GetSelectedIndex(roster);
    slots = shared->friends.slots;
    codeState = FriendRoster_GetEntryField(wifiList, slots[index], 8);
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sPalPadRosterFriendMenuTemplate.template;
    if (codeState == 2) {
        header.listMenuItems = roster->menuItems[1];
    } else {
        header.listMenuItems = roster->menuItems[0];
    }
    header.bgConfig = gfx->bgConfig;
    header.numWindows = 4;
    roster->menu = TouchscreenListMenu_CreateWithCallback(gfx->listMenuSpawner, &header, (u8)shared->inputState, 13, 5, 17, 0, PalPadRoster_MenuCallback, NULL, TRUE);
    PalPadGfx_BufferFriendName(gfx, shared->saveData, slots[index], heapId);
    PalPadRoster_PrintMessage(roster, gfx, 12, heapId);
    PalPadGfx_SetArrowsVisible(gfx, FALSE);
    StartBrightnessTransition(4, 8, 0, (GXBlendPlaneMask)61, SCREEN_MASK_MAIN);
}

#pragma force_active reset

int PalPadRoster_HandleFriendMenu(PalPadRoster *roster, u32 *inputState, PalPadGfx *gfx) {
    int result;

    if (!PalPadRoster_IsMessageDone(roster, inputState)) {
        return 6;
    }
    result = TouchscreenListMenu_HandleInput(roster->menu);
    if (result == 3) {
        goto cancel;
    }
    if (result == LIST_CANCEL) {
        goto cancel;
    }
    if (result == LIST_NOTHING_CHOSEN) {
        return 6;
    }
    goto close;
cancel:
    PalPadGfx_SetArrowsVisible(gfx, TRUE);
    result = 2;
close:
    PalPadRoster_ClearMessage(roster);
    *inputState = TouchscreenListMenu_WasLastInputTouch(roster->menu);
    TouchscreenListMenu_Destroy(roster->menu);
    if (result == 2) {
        PalPadGfx_SetArrowsVisible(gfx, TRUE);
        PalPadRosterPage_HighlightSlot(&roster->pages[0], gfx, roster->cursor, FALSE);
    }
    SetBlendBrightness(0, (GXBlendPlaneMask)61, SCREEN_MASK_MAIN);
    return result;
}

BOOL PalPadRoster_ShowFriendCode(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int index;
    u8 *slots;
    FriendRoster *wifiList;
    u64 friendCode;
    int msgId;

    wifiList = Save_FriendRoster_Get(shared->saveData);
    index = PalPadRoster_GetSelectedIndex(roster);
    slots = shared->friends.slots;
    friendCode = DWC_GetFriendKey(FriendRoster_GetDwcFriendData(wifiList, slots[index]));
    msgId = friendCode != 0 ? 60 : 66;
    PlaySE(SEQ_SE_DP_DECIDE);
    PalPadGfx_BufferFriendCode(gfx, friendCode);
    PalPadGfx_BufferFriendName(gfx, shared->saveData, slots[index], heapId);
    PalPadRoster_PrintMessage(roster, gfx, msgId, heapId);
    return TRUE;
}

void PalPadRoster_ConfirmDelete(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int index = PalPadRoster_GetSelectedIndex(roster);

    PalPadGfx_BufferFriendName(gfx, shared->saveData, shared->friends.slots[index], heapId);
    PalPadRoster_PrintMessage(roster, gfx, 57, heapId);
}

BOOL PalPadRoster_StartDeletePrompt(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    if (!PalPadRoster_IsMessageDone(roster, &shared->inputState)) {
        return FALSE;
    }
    PalPadGfx_StartYesNo(&shared->inputState, gfx, 1);
    return TRUE;
}

// panel 0 is the visible page; 1 / 2 are drawn off screen for a scroll to the right / left
void PalPadRoster_DrawPage(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, int page, int cursor, int panel, enum HeapID heapId) {
    PalPadRosterPage *rosterPage;

    if (panel == 0) {
        roster->page = page;
        roster->cursor = cursor;
        PalPadRoster_DrawTabs(roster, gfx, page);
    }
    rosterPage = &roster->pages[panel];
    PalPadRosterPage_DrawFrame(rosterPage, gfx, &gfx->screens);
    PalPadRosterPage_Draw(rosterPage, shared, gfx, roster->trainerIcons, &gfx->screens, &shared->friends, page, heapId);
    PalPadRoster_SetCursor(rosterPage, gfx, cursor);
}

void PalPadRoster_DrawTabs(PalPadRoster *roster, PalPadGfx *gfx, int page) {
    NNSG2dScreenData *screen = gfx->screens.data[1];

    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, 16, 0, 16, 4, screen->rawData, 0, 4, screen->screenWidth >> 3, screen->screenHeight >> 3);
    screen = gfx->screens.data[1];
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, page * 4 + 16, 0, 4, 4, screen->rawData, page * 4, 8, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
}

void PalPadRoster_DeselectTab(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage) {
    NNSG2dScreenData *screen = gfx->screens.data[1];

    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, prevPage * 4 + 16, 0, 4, 4, screen->rawData, prevPage * 4 + 16, 8, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
}

void PalPadRoster_DrawTabsOnScroll(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage) {
    NNSG2dScreenData *screen = gfx->screens.data[1];

    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, 16, 0, 16, 4, screen->rawData, 0, 4, screen->screenWidth >> 3, screen->screenHeight >> 3);
    screen = gfx->screens.data[1];
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, page * 4 + 16, 0, 4, 4, screen->rawData, page * 4, 8, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
}

void PalPadRoster_StartScroll(PalPadRoster *roster, PalPadShared *shared, u16 dir) {
    shared->subState = 3;
    roster->scrollDir = dir;
    roster->scrollStep = 0;
}

#pragma force_active on

void PalPadRoster_PrintMessage(PalPadRoster *roster, PalPadGfx *gfx, int msgId, enum HeapID heapId) {
    String *buffer;

    FillWindowPixelBuffer(&roster->msgWindow, 15);
    buffer = String_New(0x80, heapId);
    ReadMsgDataIntoString(gfx->msgData, msgId, buffer);
    StringExpandPlaceholders(gfx->msgFormat, roster->msgText, buffer);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    roster->printerId = AddTextPrinterParameterizedWithColor(&roster->msgWindow, 1, roster->msgText, 0, 0, roster->textSpeed, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    DrawFrameAndWindow2(&roster->msgWindow, TRUE, 70, 12);
    ScheduleWindowCopyToVram(&roster->msgWindow);
    String_Delete(buffer);
}

BOOL PalPadRoster_IsMessageDone(PalPadRoster *roster, u32 *inputState) {
    if (TextPrinterCheckActive(roster->printerId) == FALSE) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        *inputState = TextFlags_GetIsTouchSpeedingUpPrint();
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void PalPadRoster_ClearMessage(PalPadRoster *roster) {
    ClearFrameAndWindow2(&roster->msgWindow, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&roster->msgWindow);
    ScheduleWindowCopyToVram(&roster->returnWindow);
}

int PalPadRoster_GetSelectedIndex(PalPadRoster *roster) {
    int index = roster->page << 3;
    return index + roster->cursor;
}

void PalPadRoster_MenuCallback(TouchscreenListMenu *menu, u8 index, void *data, int event) {
    if (event == 1) {
        PlaySE(SEQ_SE_DP_SELECT);
        return;
    }
    if (event == 2) {
        PlaySE(SEQ_SE_DP_SELECT);
        return;
    }
    if (event == 3) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

// The deleted friend's slot is wiped by an animated sprite inside an OBJ window.
void PalPadRoster_StartDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx) {
    VecFx32 pos;
    int cursor;
    u32 reg;

    Sprite_SetDrawFlag(roster->deleteEffect, TRUE);
    PalPadGfx_SetCursor(gfx, FALSE, 0xFF);
    Sprite_SetAnimCtrlSeq(roster->deleteEffect, 4);
    roster->lastFrame = 0;
    Sprite_SetAnimActiveFlag(roster->deleteEffect, TRUE);
    cursor = roster->cursor;
    pos.x = sPalPadRosterSlotRects[cursor].x * 8;
    pos.y = sPalPadRosterSlotY[cursor].y * 8 + 252;
    pos.x <<= FX32_SHIFT;
    pos.y <<= FX32_SHIFT;
    Sprite_SetMatrix(roster->deleteEffect, &pos);
    reg = reg_G2_WINOUT;
    reg = (reg & 0xFFFFC0FF) | 0x700;
    reg_G2_WINOUT = reg;
    reg = reg_G2_WINOUT;
    reg = (reg & ~0x3F) | 0x1F;
    reg |= 0x20;
    reg_G2_WINOUT = reg;
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | 0x8000;
    PlaySE(SEQ_SE_DP_UG_025);
}

BOOL PalPadRoster_UpdateDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx) {
    int frame;

    if (!Sprite_IsAnimated(roster->deleteEffect)) {
        return TRUE;
    }
    frame = Sprite_GetAnimationFrame(roster->deleteEffect);
    if ((u32)(frame - roster->lastFrame) >= 2) {
        roster->lastFrame = frame;
        PlaySE(SEQ_SE_DP_UG_025);
    }
    return FALSE;
}

#pragma force_active on

void PalPadRoster_EndDeleteEffect(PalPadRoster *roster, PalPadGfx *gfx) {
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    Sprite_SetDrawFlag(roster->deleteEffect, FALSE);
    Sprite_SetAnimActiveFlag(roster->deleteEffect, FALSE);
    PalPadGfx_SetCursor(gfx, TRUE, 0xFF);
}

void PalPadRosterPage_Init(PalPadRosterPage *page, PalPadGfx *gfx, const PalPadPanelLayout *layout, int baseTile, enum HeapID heapId) {
    int y, x;

    page->layout = *layout;
    InitWindow(&page->window);
    y = layout->y;
    if (y < 0) {
        y = -y;
    }
    x = layout->x;
    if (x < 0) {
        x = -x;
    }
    AddWindowParameterized(gfx->bgConfig, &page->window, GF_BG_LYR_MAIN_3, x + 4, y + 4, 26, 17, 11, baseTile);
}

#pragma force_active reset

void PalPadRosterPage_Free(PalPadRosterPage *page, PalPadGfx *gfx) {
    RemoveWindow(&page->window);
}

void PalPadRosterPage_DrawFrame(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens) {
    int y = page->layout.y;
    if (y < 0) {
        y = -y;
    }
    int x = page->layout.x;
    if (x < 0) {
        x = -x;
    }
    NNSG2dScreenData *screen = screens->data[0];
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, x, y, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void PalPadRoster_SetCursor(PalPadRosterPage *page, PalPadGfx *gfx, int cursor) {
    if (cursor == ROSTER_CURSOR_HIDDEN) {
        PalPadGfx_SetCursor(gfx, FALSE, 0xFF);
        return;
    }
    const PalPadSlotRect *rect = &sPalPadRosterSlotRects[cursor];
    Sprite_SetPositionXY(gfx->cursor, (rect->x + 7) * 8, (rect->y + 2) * 8);
    PalPadGfx_SetCursor(gfx, TRUE, 0xFF);
}

void PalPadRosterPage_HighlightSlot(PalPadRosterPage *page, PalPadGfx *gfx, int cursor, int highlight) {
    int i;

    for (i = 0; i < 9; i++) {
        const PalPadSlotRect *rect = &sPalPadRosterSlotRects[i];
        int palette = (highlight == TRUE && cursor == i) ? rect->selectedPalette : rect->palette;
        BgTilemapRectChangePalette(gfx->bgConfig, rect->bgId, rect->x + Abs16(page->layout.x), rect->y + Abs16(page->layout.y), rect->width, rect->height, palette);
    }
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
}

void PalPadRosterPage_Draw(PalPadRosterPage *page, PalPadShared *shared, PalPadGfx *gfx, CharSpriteManager *trainerIcons, PalPadScreens *screens, PalPadFriendList *friends, int pageNo, enum HeapID heapId) {
    int first, i;
    FriendRoster *wifiList;

    wifiList = Save_FriendRoster_Get(shared->saveData);
    FillWindowPixelBuffer(&page->window, 0);
    PalPadRosterPage_DeleteIcons(page, gfx);
    first = pageNo * PALPAD_ROSTER_PAGE_SLOT;
    i = 0;
    do {
        if (friends->count > (u32)(first + i)) {
            PalPadRosterPage_DrawFriend(page, trainerIcons, gfx, screens, i, wifiList, friends->slots[first + i], heapId);
        } else {
            PalPadRosterPage_DrawEmptySlot(page, gfx, screens, i);
        }
        i++;
    } while (i < PALPAD_ROSTER_PAGE_SLOT);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleWindowCopyToVram(&page->window);
}

void PalPadRosterPage_Clear(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens) {
    PalPadRosterPage_DeleteIcons(page, gfx);
    ClearWindowTilemapAndScheduleTransfer(&page->window);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, Abs16(page->layout.x), Abs16(page->layout.y), page->layout.width, page->layout.height, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_3, 0, Abs16(page->layout.x), Abs16(page->layout.y), page->layout.width, page->layout.height, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
}

void PalPadRosterPage_DeleteIcons(PalPadRosterPage *page, PalPadGfx *gfx) {
    int i;

    for (i = 0; i < PALPAD_ROSTER_PAGE_SLOT; i++) {
        if (page->icons[i]) {
            CharSprite_Delete(page->icons[i]);
            page->icons[i] = NULL;
        }
    }
}

#pragma force_active on

void PalPadRosterPage_DrawFriend(PalPadRosterPage *page, CharSpriteManager *trainerIcons, PalPadGfx *gfx, PalPadScreens *screens, int slot, FriendRoster *wifiList, int friendSlot, enum HeapID heapId) {
    int avatar, gender;
    s16 position[4];
    String *string;
    u8 *owner = (u8 *)page; // the original addresses the icons and the window through a byte pointer

    avatar = FriendRoster_GetEntryField(wifiList, friendSlot, 7);
    gender = FriendRoster_GetEntryField(wifiList, friendSlot, 8);
    if (gender == 2) {
        PalPadRosterPage_DrawEmptySlot(page, gfx, screens, slot);
    } else {
        position[0] = (page->layout.x + sPalPadRosterSlotRects[slot].x) * 8 + 8;
        position[1] = (page->layout.y + sPalPadRosterSlotY[slot].y) * 8 + 262;
        position[2] = 8;
        position[3] = 1;
        *(CharSprite **)(owner + offsetof(PalPadRosterPage, icons) + slot * sizeof(CharSprite *)) = CharSpriteManager_CreateSprite(trainerIcons, position, avatar, heapId);
    }
    string = String_New(0x80, heapId);
    CopyU16ArrayToString(string, FriendRoster_GetPlayerName(wifiList, friendSlot));
    AddTextPrinterParameterizedWithColor((Window *)(owner + offsetof(PalPadRosterPage, window)), 4, string, sPalPadRosterSlotRects[slot].x * 8, sPalPadRosterSlotY[slot].y * 8 - 24, TEXT_SPEED_NOTRANSFER, sPalPadRosterNameColors[gender], NULL);
    String_Delete(string);
}

#pragma force_active reset

void PalPadRosterPage_DrawEmptySlot(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens, int slot) {
    int y = page->layout.y;
    int x;
    int offset = slot; // byte offset into the slot table, scaled below (kept for the original's order)
    NNSG2dScreenData *screen;

    if (y < 0) {
        y = -y;
    }
    x = page->layout.x;
    if (x < 0) {
        x = -x;
    }
    screen = screens->data[1];
    offset *= 10; // sizeof(PalPadSlotRect)
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, (u8)AddTiles(*(s16 *)((u8 *)sPalPadRosterSlotRects + offset), x), (u8)AddTiles(*(s16 *)((u8 *)sPalPadRosterSlotY + offset), y), 3, 4, screen->rawData, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void PalPadNaming_Init(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    PalPadNaming_Setup(naming, shared, gfx, heapId);
}
