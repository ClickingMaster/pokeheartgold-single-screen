#include "easy_chat_app.h"

#include "global.h"

#include "constants/easy_chat.h"
#include "constants/sndseq.h"

#include "easy_chat_app_internal.h"
#include "heap.h"
#include "mail_message.h"
#include "mail_misc.h"
#include "overlay_manager.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_system.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// The message cursor passes its signed bank/number without narrowing them to the u16 parameters.
#define SetMsgBankAndNum ((void (*)(MailMessage *, s32, s32))MailMsg_SetMsgBankAndNum)

typedef struct EasyChatMessageCursor {
    s16 bank;            // 0x00
    s8 number;           // 0x02
    s8 count;            // 0x03, messages per bank
    s8 total;            // 0x04
} EasyChatMessageCursor; // size: 0x6

typedef struct EasyChatWordPage {
    u16 topRow;      // 0x00
    u16 maxTopRow;   // 0x02
    u16 numWords;    // 0x04
    u8 column;       // 0x06
    u8 row;          // 0x07
    s32 scroll;      // 0x08, rows scrolled by the last move
    u16 onCancel;    // 0x0C, cursor on the cancel button below the list
    u16 touchedWord; // 0x0E
} EasyChatWordPage;  // size: 0x10

typedef int (*EasyChatAppHandler)(EasyChatApp *app, u32 *state);
typedef void (*EasyChatAppSubHandler)(EasyChatApp *app, u32 *state);

struct EasyChatApp {
    EasyChatArgs *args;                   // 0x00
    u32 type;                             // 0x04, EASY_CHAT_TYPE_*
    MailMessage message;                  // 0x08
    u16 words[2];                         // 0x10
    EasyChatAppView *view;                // 0x14
    ECMenu *menu;                         // 0x18
    u32 state;                            // 0x1C
    EasyChatAppHandler handler;           // 0x20
    EasyChatAppHandler nextHandler;       // 0x24
    u32 subState;                         // 0x28
    EasyChatAppSubHandler subHandler;     // 0x2C
    u16 newKeys;                          // 0x30
    u16 heldKeys;                         // 0x32
    u16 repeatedKeys;                     // 0x34
    u8 filler_36[2];                      // 0x36
    u32 touchButton;                      // 0x38
    u8 filler_3C[4];                      // 0x3C
    TouchHitboxController *touchHitboxes; // 0x40
    int unk_44;                           // 0x44
    int unk_48;                           // 0x48
    u16 menuCursor;                       // 0x4C
    u16 wordSlot;                         // 0x4E
    u16 category;                         // 0x50
    u16 prevCategory;                     // 0x52
    EasyChatWordPage wordPage;            // 0x54
    EasyChatMessageCursor messageCursor;  // 0x64
    u8 numSlots;                          // 0x6A
    u8 listMode;                          // 0x6B, EASY_CHAT_LIST_MODE_*
    u8 dragging;                          // 0x6C
    s16 dragX;                            // 0x6E
    u8 yesNo[2];                          // 0x70
    MenuInputState menuInputState;        // 0x74
    int unk_78;                           // 0x78
}; // size: 0x7C

// entries of the word and message menus (menuCursor) and the actions their input handlers take
enum {
    EASY_CHAT_MENU_EDIT_WORD,
    EASY_CHAT_MENU_FINISH,
    EASY_CHAT_MENU_QUIT,
    EASY_CHAT_MENU_MOVE_CURSOR,
    EASY_CHAT_MENU_CHANGE_MESSAGE,
};

// EasyChatApp_Handler_WordMenu
enum {
    EASY_CHAT_WORD_MENU_INPUT,
    EASY_CHAT_WORD_MENU_CONFIRM_FINISH,
    EASY_CHAT_WORD_MENU_CONFIRM_QUIT,
    EASY_CHAT_WORD_MENU_RESUME,
    EASY_CHAT_WORD_MENU_OPEN_CATEGORIES,
};

// EasyChatApp_Handler_MessageMenu
enum {
    EASY_CHAT_MESSAGE_MENU_INIT,
    EASY_CHAT_MESSAGE_MENU_INPUT,
    EASY_CHAT_MESSAGE_MENU_CHANGE_MESSAGE,
    EASY_CHAT_MESSAGE_MENU_CONFIRM_FINISH,
    EASY_CHAT_MESSAGE_MENU_CONFIRM_QUIT,
    EASY_CHAT_MESSAGE_MENU_RESUME,
    EASY_CHAT_MESSAGE_MENU_OPEN_CATEGORIES,
};

// EasyChatApp_Handler_CategoryMenu
enum {
    EASY_CHAT_CATEGORY_MENU_INPUT,
    EASY_CHAT_CATEGORY_MENU_EXIT,
    EASY_CHAT_CATEGORY_MENU_SWITCH_LIST_MODE,
};

// EasyChatApp_Handler_WordList
enum {
    EASY_CHAT_WORD_LIST_INPUT,
    EASY_CHAT_WORD_LIST_SCROLL,
    EASY_CHAT_WORD_LIST_SCROLL_PAGE,
    EASY_CHAT_WORD_LIST_EXIT,
};

#define EASY_CHAT_CATEGORY_BACK 0xFE // the back button below the category list

// Category reached by each direction key from a category of the grid (the 13th entry is the back button).
typedef struct EasyChatCategoryNeighbors {
    u8 up;
    u8 down;
    u8 left;
    u8 right;
} EasyChatCategoryNeighbors;

static void EasyChatApp_OnTouchButton(u32 index, u32 event, void *arg);
static EasyChatApp *EasyChatApp_Create(OverlayManager *man);
static void EasyChatMessageCursor_Init(EasyChatMessageCursor *cursor, MailMessage *message);
static void EasyChatMessageCursor_Next(EasyChatMessageCursor *cursor, MailMessage *message);
static void EasyChatMessageCursor_Prev(EasyChatMessageCursor *cursor, MailMessage *message);
static void EasyChatMessageCursor_SetIndex(EasyChatMessageCursor *cursor, MailMessage *message, u8 index);
static u8 EasyChatMessageCursor_GetIndex(EasyChatMessageCursor *cursor);
static void EasyChatApp_Delete(EasyChatApp *app, OverlayManager *man);
static void EasyChatApp_SetHandler(EasyChatApp *app, EasyChatAppHandler handler);
static BOOL EasyChatApp_UpdateMenuInputState(EasyChatApp *app, u32 *state);
static int EasyChatApp_Handler_Main(EasyChatApp *app, u32 *state);
static int EasyChatApp_WordMenuAction(EasyChatApp *app, int action);
static int EasyChatApp_WordMenuHandleKeys(EasyChatApp *app);
static int EasyChatApp_GetOneWordMenuTouch(EasyChatApp *app);
static int EasyChatApp_GetTwoWordMenuTouch(EasyChatApp *app);
static int EasyChatApp_WordMenuHandleTouch(EasyChatApp *app);
static int EasyChatApp_WordMenuRun(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_WordMenu(EasyChatApp *app, u32 *state);
static int EasyChatApp_MessageMenuAction(EasyChatApp *app, int action);
static int EasyChatApp_MessageMenuHandleKeys(EasyChatApp *app);
static BOOL EasyChatApp_IsTouchOnSlider(EasyChatApp *app, int x, int y);
static BOOL EasyChatApp_IsTouchInSliderRange(EasyChatApp *app, int x, int y);
static int EasyChatApp_GetMessageMenuTouch(EasyChatApp *app);
static int EasyChatApp_MessageMenuHandleTouch(EasyChatApp *app);
static int EasyChatApp_MessageMenuRun(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_MessageMenu(EasyChatApp *app, u32 *state);
static int EasyChatApp_Handler_CategoryMenu(EasyChatApp *app, u32 *state);
static void EasyChatApp_CategoryMenuHandleKeys(EasyChatApp *app, u32 *state);
static int EasyChatApp_GetListButtonTouch(EasyChatApp *app);
static int EasyChatApp_GetCategoryGridTouch(EasyChatApp *app);
static int EasyChatApp_GetInitialGridTouch(EasyChatApp *app);
static void EasyChatApp_CategoryMenuHandleTouch(EasyChatApp *app, u32 *state);
static void EasyChatApp_CategoryMenuHandleInput(EasyChatApp *app, u32 *state);
static BOOL EasyChatApp_CategoryHasWords(ECMenu *menu, u8 listMode, u16 category);
static BOOL EasyChatApp_MoveCategoryCursor(EasyChatApp *app);
BOOL EasyChatApp_MoveCategoryGridCursor(EasyChatApp *app);
BOOL EasyChatApp_MoveInitialGridCursor(EasyChatApp *app);
static void EasyChatWordPage_Init(EasyChatWordPage *page, EasyChatApp *app);
static int EasyChatWordPage_GetCursorPos(EasyChatWordPage *page);
static u32 EasyChatWordPage_GetWordIndex(EasyChatWordPage *page);
static s32 EasyChatWordPage_GetScroll(EasyChatWordPage *page);
static u16 EasyChatWordPage_GetTopRow(EasyChatWordPage *page);
static u16 EasyChatWordPage_GetMaxTopRow(EasyChatWordPage *page);
static int EasyChatApp_Handler_WordList(EasyChatApp *app, u32 *state);
static void EasyChatApp_WordListHandleKeys(EasyChatApp *app, u32 *state);
static int EasyChatApp_GetWordListTouch(EasyChatApp *app);
static void EasyChatApp_WordListHandleTouch(EasyChatApp *app, u32 *state);
static void EasyChatApp_WordListHandleInput(EasyChatApp *app, u32 *state);
static int EasyChatWordPage_HandleKeys(EasyChatWordPage *page, u32 keys);
static int EasyChatWordPage_PageUp(EasyChatWordPage *page);
static int EasyChatWordPage_PageDown(EasyChatWordPage *page);
static void EasyChatApp_SetSelectedWord(EasyChatApp *app);
static BOOL EasyChatApp_Handler_Exit(EasyChatApp *app, u32 *state);
static void EasyChatApp_SetSubHandler(EasyChatApp *app, EasyChatAppSubHandler subHandler);
static void EasyChatApp_EndSubHandler(EasyChatApp *app);
static void EasyChatApp_SubHandler_Start(EasyChatApp *app, u32 *state);
static void EasyChatApp_SubHandler_ConfirmFinish(EasyChatApp *app, u32 *state);
static void EasyChatApp_SubHandler_ConfirmQuit(EasyChatApp *app, u32 *state);
static BOOL EasyChatApp_HasChanged(EasyChatApp *app);
static BOOL EasyChatApp_IsComplete(EasyChatApp *app);
static void EasyChatYesNo_Set(u8 *result, u8 a, u8 b);
static u8 EasyChatYesNo_GetCursor(u8 *result);

static const EasyChatCategoryNeighbors sEasyChatCategoryGridNeighbors[13] = {
    { EASY_CHAT_CATEGORY_BACK, 3,                       2,                       1                       },
    { EASY_CHAT_CATEGORY_BACK, 4,                       0,                       2                       },
    { EASY_CHAT_CATEGORY_BACK, 5,                       1,                       0                       },
    { 0,                       6,                       5,                       4                       },
    { 1,                       7,                       3,                       5                       },
    { 2,                       8,                       4,                       3                       },
    { 3,                       9,                       8,                       7                       },
    { 4,                       10,                      6,                       8                       },
    { 5,                       11,                      7,                       6                       },
    { 6,                       EASY_CHAT_CATEGORY_BACK, 11,                      10                      },
    { 7,                       EASY_CHAT_CATEGORY_BACK, 9,                       11                      },
    { 8,                       EASY_CHAT_CATEGORY_BACK, 10,                      9                       },
    { 9,                       0,                       EASY_CHAT_CATEGORY_BACK, EASY_CHAT_CATEGORY_BACK },
};

static const TouchscreenHitbox sEasyChatInitialGridHitboxes[2] = {
    { .rect = { 64, 128, 20, 215 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static BOOL (*const sEasyChatCategoryCursorFuncs[2])(EasyChatApp *) = {
    EasyChatApp_MoveCategoryGridCursor,
    EasyChatApp_MoveInitialGridCursor,
};

static const TouchscreenHitbox sEasyChatWordListHitboxes[3] = {
    { .rect = { 48, 104, 228, 252 } },
    { .rect = { 104, 160, 228, 252 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const EasyChatAppHandler sEasyChatAppHandlers[3] = {
    EasyChatApp_Handler_WordMenu,
    EasyChatApp_Handler_WordMenu,
    EasyChatApp_Handler_MessageMenu,
};

static const TouchscreenHitbox sEasyChatListButtonHitboxes[4] = {
    { .rect = { 164, 186, 196, 252 } },
    { .rect = { 0, 42, 16, 104 } },
    { .rect = { 0, 42, 152, 240 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sEasyChatOneWordMenuHitboxes[4] = {
    { .rect = { 112, 128, 192, 0 } },
    { .rect = { 136, 152, 192, 0 } },
    { .rect = { 16, 32, 80, 176 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sEasyChatTwoWordMenuHitboxes[5] = {
    { .rect = { 112, 128, 192, 0 } },
    { .rect = { 136, 152, 192, 0 } },
    { .rect = { 16, 32, 24, 120 } },
    { .rect = { 16, 32, 136, 232 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sEasyChatMessageMenuHitboxes[5] = {
    { .rect = { 112, 128, 192, 0 } },
    { .rect = { 136, 152, 192, 0 } },
    { .rect = { 52, 76, 8, 40 } },
    { .rect = { 52, 76, 216, 248 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sEasyChatButtonHitboxes[5] = {
    { .rect = { 24, 80, 24, 112 } },
    { .rect = { 112, 168, 24, 112 } },
    { .rect = { 40, 72, 176, 208 } },
    { .rect = { 120, 152, 176, 208 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

BOOL EasyChat_Init(OverlayManager *man, int *state) {
    EasyChatApp *app;

    switch (*state) {
    case 0:
        Sound_SetSceneAndPlayBGM(62, 0, 0);
        Heap_Create(HEAP_ID_3, HEAP_ID_34, 0x8000);
        Heap_Create(HEAP_ID_3, HEAP_ID_35, 0x28000);
        app = EasyChatApp_Create(man);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_LOAD);
        (*state)++;
        break;
    case 1:
        app = OverlayManager_GetData(man);
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            app->numSlots = app->type == EASY_CHAT_TYPE_MESSAGE ? EasyChatAppView_GetNumWordSlots(app->view) : 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL EasyChat_Main(OverlayManager *man, int *state) {
    EasyChatApp *app = OverlayManager_GetData(man);

    app->newKeys = gSystem.newKeys;
    app->heldKeys = gSystem.heldKeys;
    app->repeatedKeys = gSystem.newAndRepeatedKeys;
    TouchHitboxController_IsTriggered(app->touchHitboxes);
    if (app->subHandler != NULL) {
        app->subHandler(app, &app->subState);
        return FALSE;
    }
    return app->handler(app, &app->state);
}

static void EasyChatApp_OnTouchButton(u32 index, u32 event, void *arg) {
    EasyChatApp *app = arg;

    switch (event) {
    case 0:
        app->touchButton = index;
        switch (index) {
        case 2:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_PRESS_PAGE_UP);
            break;
        case 3:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_PRESS_PAGE_DOWN);
            break;
        }
        return;
    case 1:
    case 3:
        switch (index) {
        case 2:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_RELEASE_PAGE_UP);
            break;
        case 3:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_RELEASE_PAGE_DOWN);
            break;
        }
        app->touchButton = 4;
        break;
    case 2:
        if (index - 2 <= 1) {
            app->touchButton = index;
        }
        break;
    default:
        app->touchButton = 4;
        break;
    }
}

BOOL EasyChat_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    EasyChatApp_Delete(OverlayManager_GetData(man), man);
    Heap_Destroy(HEAP_ID_34);
    Heap_Destroy(HEAP_ID_35);
    return TRUE;
}

static EasyChatApp *EasyChatApp_Create(OverlayManager *man) {
    EasyChatApp *app = OverlayManager_CreateAndGetData(man, sizeof(EasyChatApp), HEAP_ID_34);

    MI_CpuFill8(app, 0, sizeof(EasyChatApp));
    app->args = OverlayManager_GetArgs(man);
    app->type = EasyChatArgs_GetType(app->args);
    app->menuInputState = (MenuInputState)EasyChatArgs_GetMenuInputState(app->args);
    EasyChatArgs_CopyMessage(app->args, (MailMessage *)app->words, &app->message);
    if (app->type == EASY_CHAT_TYPE_MESSAGE) {
        EasyChatMessageCursor_Init(&app->messageCursor, &app->message);
    }
    app->menu = ECMenu_New(HEAP_ID_34, app->args);
    app->view = EasyChatAppView_Create(app, app->menu);
    app->touchHitboxes = TouchHitboxController_Create(sEasyChatButtonHitboxes, 5, EasyChatApp_OnTouchButton, app, HEAP_ID_34);
    app->unk_44 = 0;
    app->unk_48 = 0;
    app->listMode = EASY_CHAT_LIST_MODE_CATEGORIES;
    app->subHandler = NULL;
    app->subState = 0;
    app->wordSlot = 0;
    EasyChatApp_SetHandler(app, EasyChatApp_Handler_Main);
    EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_Start);
    return app;
}

static void EasyChatMessageCursor_Init(EasyChatMessageCursor *cursor, MailMessage *message) {
    int i;

    cursor->bank = MailMsg_GetMsgBank(message);
    cursor->count = 20;
    cursor->number = MailMsg_GetMsgNo(message);
    for (i = 0; i < 5; i++) {
        cursor->total += cursor->count;
    }
}

static void EasyChatMessageCursor_Next(EasyChatMessageCursor *cursor, MailMessage *message) {
    cursor->number++;
    if (cursor->number >= cursor->count) {
        cursor->number = 0;
        cursor->bank++;
        if (cursor->bank >= 5) {
            cursor->bank = 0;
        }
    }
    SetMsgBankAndNum(message, cursor->bank, cursor->number);
}

static void EasyChatMessageCursor_Prev(EasyChatMessageCursor *cursor, MailMessage *message) {
    cursor->number--;
    if (cursor->number < 0) {
        cursor->bank--;
        if (cursor->bank < 0) {
            cursor->bank = 4;
        }
        cursor->number = cursor->count - 1;
    }
    SetMsgBankAndNum(message, cursor->bank, cursor->number);
}

static void EasyChatMessageCursor_SetIndex(EasyChatMessageCursor *cursor, MailMessage *message, u8 index) {
    cursor->number = index % cursor->count;
    cursor->bank = index / cursor->count;
    SetMsgBankAndNum(message, cursor->bank, cursor->number);
}

static u8 EasyChatMessageCursor_GetIndex(EasyChatMessageCursor *cursor) {
    return cursor->number + cursor->bank * cursor->count;
}

static void EasyChatApp_Delete(EasyChatApp *app, OverlayManager *man) {
    EasyChatArgs_SetMenuInputState(app->args, app->menuInputState);
    TouchHitboxController_Destroy(app->touchHitboxes);
    EasyChatAppView_Delete(app->view);
    ECMenu_Delete(app->menu);
    OverlayManager_FreeData(man);
}

static void EasyChatApp_SetHandler(EasyChatApp *app, EasyChatAppHandler handler) {
    app->handler = handler;
    app->state = 0;
    app->unk_78 = 0;
}

static BOOL EasyChatApp_UpdateMenuInputState(EasyChatApp *app, u32 *state) {
#pragma unused(state)
    if (app->menuInputState == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys != 0) {
            app->touchButton = 4;
            app->menuInputState = MENU_INPUT_STATE_BUTTONS;
            return FALSE;
        }
    } else {
        if (gSystem.heldKeys != 0) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            app->menuInputState = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

static int EasyChatApp_Handler_Main(EasyChatApp *app, u32 *state) {
    return sEasyChatAppHandlers[app->type](app, state);
}

static int EasyChatApp_WordMenuAction(EasyChatApp *app, int action) {
    switch (action) {
    case EASY_CHAT_MENU_QUIT:
        PlaySE(SEQ_SE_DP_SELECT);
        return EASY_CHAT_WORD_MENU_CONFIRM_QUIT;
    case EASY_CHAT_MENU_FINISH:
        PlaySE(SEQ_SE_DP_SELECT);
        return EASY_CHAT_WORD_MENU_CONFIRM_FINISH;
    case EASY_CHAT_MENU_EDIT_WORD:
        PlaySE(SEQ_SE_DP_SELECT);
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_OPEN_CATEGORIES);
        return EASY_CHAT_WORD_MENU_OPEN_CATEGORIES;
    case EASY_CHAT_MENU_MOVE_CURSOR:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        break;
    }
    return EASY_CHAT_WORD_MENU_INPUT;
}

static int EasyChatApp_WordMenuHandleKeys(EasyChatApp *app) {
    u16 keys = app->newKeys;

    if (keys & PAD_BUTTON_B) {
        app->menuCursor = EASY_CHAT_MENU_QUIT;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_QUIT);
    }
    if (keys & PAD_BUTTON_A) {
        return EasyChatApp_WordMenuAction(app, app->menuCursor);
    }
    if (keys & PAD_BUTTON_START) {
        app->menuCursor = EASY_CHAT_MENU_FINISH;
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
    }
    if (keys & PAD_KEY_DOWN) {
        if (app->menuCursor == EASY_CHAT_MENU_QUIT) {
            return EASY_CHAT_WORD_MENU_INPUT;
        }
        app->menuCursor++;
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
    }
    if (keys & PAD_KEY_UP) {
        if (app->menuCursor == EASY_CHAT_MENU_EDIT_WORD) {
            return EASY_CHAT_WORD_MENU_INPUT;
        }
        app->menuCursor--;
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
    }
    if (app->type == EASY_CHAT_TYPE_WORD) {
        return EASY_CHAT_WORD_MENU_INPUT;
    }
    if (keys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        app->wordSlot ^= 1;
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
    }
    return EASY_CHAT_WORD_MENU_INPUT;
}

static int EasyChatApp_GetOneWordMenuTouch(EasyChatApp *app) {
#pragma unused(app)
    return TouchscreenHitbox_FindRectAtTouchNew(sEasyChatOneWordMenuHitboxes);
}

static int EasyChatApp_GetTwoWordMenuTouch(EasyChatApp *app) {
#pragma unused(app)
    return TouchscreenHitbox_FindRectAtTouchNew(sEasyChatTwoWordMenuHitboxes);
}

static int EasyChatApp_WordMenuHandleTouch(EasyChatApp *app) {
    int result;

    if (app->type == EASY_CHAT_TYPE_WORD) {
        result = EasyChatApp_GetOneWordMenuTouch(app);
    } else {
        result = EasyChatApp_GetTwoWordMenuTouch(app);
    }
    switch (result) {
    case 0:
        app->menuCursor = EASY_CHAT_MENU_FINISH;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_FINISH);
    case 1:
        app->menuCursor = EASY_CHAT_MENU_QUIT;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_QUIT);
    case 2:
    case 3:
        app->category = 0;
        app->wordSlot = result - 2;
        app->menuCursor = EASY_CHAT_MENU_EDIT_WORD;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_WordMenuAction(app, EASY_CHAT_MENU_EDIT_WORD);
    }
    return EASY_CHAT_WORD_MENU_INPUT;
}

static int EasyChatApp_WordMenuRun(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case EASY_CHAT_WORD_MENU_INPUT:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
                *state = EasyChatApp_WordMenuHandleKeys(app);
            } else {
                *state = EasyChatApp_WordMenuHandleTouch(app);
            }
        }
        break;
    case EASY_CHAT_WORD_MENU_CONFIRM_FINISH:
        *state = EASY_CHAT_WORD_MENU_RESUME;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmFinish);
        break;
    case EASY_CHAT_WORD_MENU_CONFIRM_QUIT:
        *state = EASY_CHAT_WORD_MENU_RESUME;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        break;
    case EASY_CHAT_WORD_MENU_RESUME:
        *state = EASY_CHAT_WORD_MENU_INPUT;
        break;
    case EASY_CHAT_WORD_MENU_OPEN_CATEGORIES:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, EasyChatApp_Handler_CategoryMenu);
        }
        break;
    }
    return 0;
}

int EasyChatApp_Handler_WordMenu(EasyChatApp *app, u32 *state) {
    if (*state == EASY_CHAT_WORD_MENU_INPUT && EasyChatApp_UpdateMenuInputState(app, state)) {
        return 0;
    }
    return EasyChatApp_WordMenuRun(app, state);
}

static int EasyChatApp_MessageMenuAction(EasyChatApp *app, int action) {
    switch (action) {
    case EASY_CHAT_MENU_QUIT:
        PlaySE(SEQ_SE_DP_SELECT);
        return EASY_CHAT_MESSAGE_MENU_CONFIRM_QUIT;
    case EASY_CHAT_MENU_FINISH:
        PlaySE(SEQ_SE_DP_SELECT);
        return EASY_CHAT_MESSAGE_MENU_CONFIRM_FINISH;
    case EASY_CHAT_MENU_EDIT_WORD:
        if (EasyChatAppView_GetNumWordSlots(app->view) == 0) {
            return EASY_CHAT_MESSAGE_MENU_INPUT;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_OPEN_CATEGORIES);
        return EASY_CHAT_MESSAGE_MENU_OPEN_CATEGORIES;
    case EASY_CHAT_MENU_MOVE_CURSOR:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        break;
    case EASY_CHAT_MENU_CHANGE_MESSAGE:
        PlaySE(SEQ_SE_DP_SELECT);
        app->wordSlot = 0;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_REDRAW_SENTENCE);
        return EASY_CHAT_MESSAGE_MENU_CHANGE_MESSAGE;
    }
    return EASY_CHAT_MESSAGE_MENU_INPUT;
}

static int EasyChatApp_MessageMenuHandleKeys(EasyChatApp *app) {
    if (app->newKeys & PAD_BUTTON_B) {
        app->menuCursor = EASY_CHAT_MENU_QUIT;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_QUIT);
    }
    if (app->newKeys & PAD_BUTTON_A) {
        return EasyChatApp_MessageMenuAction(app, app->menuCursor);
    }
    if (app->newKeys & PAD_KEY_UP) {
        if (app->menuCursor != EASY_CHAT_MENU_EDIT_WORD) {
            app->menuCursor--;
            return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
        }
        if (app->numSlots != 0 && app->wordSlot != 0) {
            app->wordSlot--;
            return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
        }
        return EASY_CHAT_MESSAGE_MENU_INPUT;
    }
    if (app->newKeys & PAD_KEY_DOWN) {
        if (app->menuCursor == EASY_CHAT_MENU_EDIT_WORD) {
            if (app->numSlots == 2 && app->wordSlot == 0) {
                app->wordSlot++;
                return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
            }
            app->menuCursor++;
            return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
        }
        if (app->menuCursor == EASY_CHAT_MENU_FINISH) {
            app->menuCursor++;
            return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_MOVE_CURSOR);
        }
        return EASY_CHAT_MESSAGE_MENU_INPUT;
    }
    if (app->repeatedKeys & PAD_KEY_LEFT) {
        EasyChatMessageCursor_Prev(&app->messageCursor, &app->message);
        return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_CHANGE_MESSAGE);
    }
    if (app->repeatedKeys & PAD_KEY_RIGHT) {
        EasyChatMessageCursor_Next(&app->messageCursor, &app->message);
        return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_CHANGE_MESSAGE);
    }
    return EASY_CHAT_MESSAGE_MENU_INPUT;
}

static BOOL EasyChatApp_IsTouchOnSlider(EasyChatApp *app, int x, int y) {
    int touchX;
    int touchY;

    if (!System_GetTouchHeld()) {
        return FALSE;
    }
    touchX = gSystem.touchX;
    if (x < touchX - 10 || x > touchX + 10) {
        return FALSE;
    }
    touchY = gSystem.touchY;
    if (y < touchY - 12 || y > touchY + 12) {
        return FALSE;
    }
    return TRUE;
}

static BOOL EasyChatApp_IsTouchInSliderRange(EasyChatApp *app, int x, int y) {
    int touchY;

    if (!System_GetTouchHeld()) {
        return FALSE;
    }
    if (x < 18 || x > 236) {
        return FALSE;
    }
    touchY = gSystem.touchY;
    if (y < touchY - 24 || y > touchY + 24) {
        return FALSE;
    }
    return TRUE;
}

static int EasyChatApp_GetMessageMenuTouch(EasyChatApp *app) {
    struct {
        s16 y;
        s16 x;
        TouchscreenHitbox hitbox;
    } local;
    Sprite *sprite = EasyChatAppView_GetSliderSprite(app->view);

    Sprite_GetPositionXY(sprite, &local.x, &local.y);
    if (app->dragging) {
        int distance;
        s16 x;

        if (!EasyChatApp_IsTouchInSliderRange(app, local.x, local.y)) {
            app->dragging = FALSE;
            return -1;
        }
        x = (s16)gSystem.touchX;
        if (x < 28) {
            x = 28;
        } else if (x > 226) {
            x = 226;
        }
        distance = x - app->dragX;
        if (distance < 0) {
            distance = 0 - distance;
        }
        if (distance < 2) {
            return -1;
        }
        Sprite_SetPositionXY(sprite, x, local.y);
        app->dragX = x;
        app->wordSlot = 0;
        EasyChatMessageCursor_SetIndex(&app->messageCursor, &app->message, (x - 28) / 2);
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_REDRAW_SENTENCE);
        return -1;
    } else {
        int result;
        int count;
        int i;

        if (!System_GetTouchNew()) {
            return -1;
        }
        if (EasyChatApp_IsTouchOnSlider(app, local.x, local.y)) {
            app->dragX = gSystem.touchX;
            app->dragging = TRUE;
            PlaySE(SEQ_SE_DP_SELECT);
            return 6;
        }
        result = TouchscreenHitbox_FindRectAtTouchNew(sEasyChatMessageMenuHitboxes);
        if (result == -1) {
            count = EasyChatAppView_GetNumWordSlots(app->view);
            for (i = 0; i < count; i++) {
                EasyChatAppView_GetWordSlotHitbox(app->view, &local.hitbox, i);
                if (TouchscreenHitbox_PointIsIn(&local.hitbox, gSystem.touchX, gSystem.touchY)) {
                    return i + 4;
                }
            }
            return -1;
        }
        return result;
    }
}

static int EasyChatApp_MessageMenuHandleTouch(EasyChatApp *app) {
    int choice = EasyChatApp_GetMessageMenuTouch(app);

    switch (choice) {
    case 0:
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        app->menuCursor = choice + 1;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_MessageMenuAction(app, app->menuCursor);
    case 2:
    case 3:
        app->menuCursor = EASY_CHAT_MENU_EDIT_WORD;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        if (choice == 2) {
            EasyChatMessageCursor_Prev(&app->messageCursor, &app->message);
        } else {
            EasyChatMessageCursor_Next(&app->messageCursor, &app->message);
        }
        return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_CHANGE_MESSAGE);
    case 4:
    case 5:
        app->menuCursor = EASY_CHAT_MENU_EDIT_WORD;
        app->wordSlot = choice - 4;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        return EasyChatApp_MessageMenuAction(app, EASY_CHAT_MENU_EDIT_WORD);
    case 6:
        app->menuCursor = EASY_CHAT_MENU_EDIT_WORD;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR);
        break;
    }
    return EASY_CHAT_MESSAGE_MENU_INPUT;
}

static int EasyChatApp_MessageMenuRun(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case EASY_CHAT_MESSAGE_MENU_INIT:
        app->numSlots = EasyChatAppView_GetNumWordSlots(app->view);
        *state = EASY_CHAT_MESSAGE_MENU_INPUT;
    case EASY_CHAT_MESSAGE_MENU_INPUT:
        if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
            *state = EasyChatApp_MessageMenuHandleKeys(app);
        } else {
            *state = EasyChatApp_MessageMenuHandleTouch(app);
        }
        break;
    case EASY_CHAT_MESSAGE_MENU_CHANGE_MESSAGE:
        if (EasyChatAppView_IsCommandDone(app->view, EASY_CHAT_VIEW_CMD_REDRAW_SENTENCE)) {
            *state = EASY_CHAT_MESSAGE_MENU_INIT;
        }
        break;
    case EASY_CHAT_MESSAGE_MENU_CONFIRM_FINISH:
        *state = EASY_CHAT_MESSAGE_MENU_RESUME;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmFinish);
        break;
    case EASY_CHAT_MESSAGE_MENU_CONFIRM_QUIT:
        *state = EASY_CHAT_MESSAGE_MENU_RESUME;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        break;
    case EASY_CHAT_MESSAGE_MENU_RESUME:
        *state = EASY_CHAT_MESSAGE_MENU_INPUT;
        break;
    case EASY_CHAT_MESSAGE_MENU_OPEN_CATEGORIES:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, EasyChatApp_Handler_CategoryMenu);
        }
        break;
    }
    return 0;
}

int EasyChatApp_Handler_MessageMenu(EasyChatApp *app, u32 *state) {
    if (*state <= EASY_CHAT_MESSAGE_MENU_INPUT && EasyChatApp_UpdateMenuInputState(app, state)) {
        return 0;
    }
    return EasyChatApp_MessageMenuRun(app, state);
}

static int EasyChatApp_Handler_CategoryMenu(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case EASY_CHAT_CATEGORY_MENU_INPUT:
        if (EasyChatAppView_IsCommandDone(app->view, EASY_CHAT_VIEW_CMD_SET_WORD_LIST_CURSOR)) {
            EasyChatApp_CategoryMenuHandleInput(app, state);
        }
        break;
    case EASY_CHAT_CATEGORY_MENU_EXIT:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, app->nextHandler);
        }
        break;
    case EASY_CHAT_CATEGORY_MENU_SWITCH_LIST_MODE:
        if (EasyChatAppView_IsCommandDone(app->view, EASY_CHAT_VIEW_CMD_SWITCH_LIST_MODE)) {
            *state = EASY_CHAT_CATEGORY_MENU_INPUT;
        }
        break;
    }
    return 0;
}

static void EasyChatApp_CategoryMenuHandleKeys(EasyChatApp *app, u32 *state) {
    u16 keys;
    u32 touchButton = app->touchButton;

    if ((touchButton == 0 && app->listMode != EASY_CHAT_LIST_MODE_CATEGORIES) || (touchButton == 1 && app->listMode != EASY_CHAT_LIST_MODE_INITIALS) || (app->newKeys & PAD_BUTTON_SELECT)) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->touchButton = 4;
        app->listMode ^= 1;
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SWITCH_LIST_MODE);
        *state = EASY_CHAT_CATEGORY_MENU_SWITCH_LIST_MODE;
        return;
    }
    keys = app->newKeys;
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_Main;
        *state = EASY_CHAT_CATEGORY_MENU_EXIT;
        return;
    }
    if (keys & PAD_BUTTON_A) {
        u16 category = app->category;

        if (category == EASY_CHAT_CATEGORY_BACK) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_CATEGORIES);
            app->nextHandler = EasyChatApp_Handler_Main;
            *state = EASY_CHAT_CATEGORY_MENU_EXIT;
            return;
        }
        if (EasyChatApp_CategoryHasWords(app->menu, app->listMode, category)) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatWordPage_Init(&app->wordPage, app);
            app->nextHandler = EasyChatApp_Handler_WordList;
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_OPEN_WORD_LIST);
            *state = EASY_CHAT_CATEGORY_MENU_EXIT;
            return;
        }
        PlaySE(SEQ_SE_DP_CUSTOM06);
        return;
    }
    if (EasyChatApp_MoveCategoryCursor(app)) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SET_CATEGORY_CURSOR);
    }
}

static int EasyChatApp_GetListButtonTouch(EasyChatApp *app) {
    int result = TouchscreenHitbox_FindRectAtTouchNew(sEasyChatListButtonHitboxes);

    if (result == -1) {
        return 0;
    }
    switch (result) {
    case 0:
        return 1;
    case 1:
        if (app->listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
            return 0;
        }
        break;
    case 2:
        if (app->listMode == EASY_CHAT_LIST_MODE_INITIALS) {
            return 0;
        }
        break;
    }
    return 2;
}

static int EasyChatApp_GetCategoryGridTouch(EasyChatApp *app) {
    TouchscreenHitbox hitbox;
    int row;
    int col;
    int x;
    int y;

    if (!System_GetTouchNew()) {
        return -1;
    }
    row = 0;
    y = 56;
    do {
        hitbox.rect.top = y;
        hitbox.rect.bottom = hitbox.rect.top + 16;
        col = 0;
        x = 12;
        do {
            hitbox.rect.left = x;
            hitbox.rect.right = hitbox.rect.left + 72;
            if (TouchscreenHitbox_PointIsIn(&hitbox, gSystem.touchX, gSystem.touchY)) {
                return row * 3 + col;
            }
            col++;
            x += 80;
        } while (col < 3);
        y += 24;
        row++;
    } while (row < 4);
    return -1;
}

static int EasyChatApp_GetInitialGridTouch(EasyChatApp *app) {
    int result;
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sEasyChatInitialGridHitboxes);

    result = -1;
    if (hit != result) {
        result = EasyChatInitialGrid_FindCellAt((s16)(gSystem.touchX - 20), (s16)(gSystem.touchY - 64));
    }
    return result;
}

static void EasyChatApp_CategoryMenuHandleTouch(EasyChatApp *app, u32 *state) {
    int input = EasyChatApp_GetListButtonTouch(app);

    switch (input) {
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_Main;
        *state = EASY_CHAT_CATEGORY_MENU_EXIT;
        return;
    case 2:
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->listMode ^= 1;
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SWITCH_LIST_MODE);
        *state = EASY_CHAT_CATEGORY_MENU_SWITCH_LIST_MODE;
        return;
    }
    if (app->listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
        input = EasyChatApp_GetCategoryGridTouch(app);
        if (input < 0) {
            return;
        }
        if (!EasyChatApp_CategoryHasWords(app->menu, app->listMode, input)) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            return;
        }
        app->category = input;
    } else {
        input = EasyChatApp_GetInitialGridTouch(app);
        if (input < 0) {
            return;
        }
        if (!EasyChatApp_CategoryHasWords(app->menu, app->listMode, input)) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            return;
        }
        app->category = input;
    }
    PlaySE(SEQ_SE_DP_SELECT);
    EasyChatWordPage_Init(&app->wordPage, app);
    app->nextHandler = EasyChatApp_Handler_WordList;
    EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_OPEN_WORD_LIST);
    *state = EASY_CHAT_CATEGORY_MENU_EXIT;
}

static void EasyChatApp_CategoryMenuHandleInput(EasyChatApp *app, u32 *state) {
    if (EasyChatApp_UpdateMenuInputState(app, state)) {
        return;
    }
    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        EasyChatApp_CategoryMenuHandleKeys(app, state);
    } else {
        EasyChatApp_CategoryMenuHandleTouch(app, state);
    }
}

static BOOL EasyChatApp_CategoryHasWords(ECMenu *menu, u8 listMode, u16 category) {
    if (category == EASY_CHAT_CATEGORY_BACK || category == 0xFF) {
        return FALSE;
    }
    if (listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
        if (ECMenu_GetCategoryCount(menu, category)) {
            return TRUE;
        }
        return FALSE;
    }
    if (ECMenu_GetInitialCount(menu, category)) {
        return TRUE;
    }
    return FALSE;
}

static BOOL EasyChatApp_MoveCategoryCursor(EasyChatApp *app) {
    return sEasyChatCategoryCursorFuncs[app->listMode](app);
}

BOOL EasyChatApp_MoveCategoryGridCursor(EasyChatApp *app) {
    u16 category = app->category;
    u16 index = category;

    if (category == EASY_CHAT_CATEGORY_BACK) {
        index = 12;
    }
    if (category != EASY_CHAT_CATEGORY_BACK) {
        app->prevCategory = category;
        if (app->repeatedKeys & PAD_KEY_UP) {
            app->category = sEasyChatCategoryGridNeighbors[index].up;
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_DOWN) {
            app->category = sEasyChatCategoryGridNeighbors[index].down;
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_LEFT) {
            app->category = sEasyChatCategoryGridNeighbors[index].left;
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_RIGHT) {
            app->category = sEasyChatCategoryGridNeighbors[index].right;
            return TRUE;
        }
    } else {
        if (app->repeatedKeys & PAD_KEY_UP) {
            app->category = app->prevCategory % 3 + 9;
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_DOWN) {
            app->category = app->prevCategory % 3;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL EasyChatApp_MoveInitialGridCursor(EasyChatApp *app) {
    u16 category = app->category;

    if (category != EASY_CHAT_CATEGORY_BACK) {
        u16 next = 0xFF;

        if (app->repeatedKeys & PAD_KEY_UP) {
            next = EasyChatInitialGrid_GetUp(category);
        } else if (app->repeatedKeys & PAD_KEY_DOWN) {
            next = EasyChatInitialGrid_GetDown(category);
        } else if (app->repeatedKeys & PAD_KEY_RIGHT) {
            next = EasyChatInitialGrid_GetRight(category);
        } else if (app->repeatedKeys & PAD_KEY_LEFT) {
            next = EasyChatInitialGrid_GetLeft(category);
        }
        if (next != 0xFF) {
            app->prevCategory = app->category;
            app->category = next;
            return TRUE;
        }
    } else {
        if (app->repeatedKeys & PAD_KEY_UP) {
            app->category = EasyChatInitialGrid_GetAboveButton(app->prevCategory);
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_DOWN) {
            app->category = EasyChatInitialGrid_GetBelowButton(app->prevCategory);
            return TRUE;
        }
    }
    return FALSE;
}

static void EasyChatWordPage_Init(EasyChatWordPage *page, EasyChatApp *app) {
    page->topRow = 0;
    page->column = 0;
    page->row = 0;
    page->numWords = EasyChatApp_GetCategoryWordCount(app);
    page->scroll = 0;
    page->onCancel = FALSE;
    page->touchedWord = 0xFFFF;
    if (page->numWords > 10) {
        page->maxTopRow = (page->numWords - 10) / 2 + (page->numWords & 1);
    } else {
        page->maxTopRow = 0;
    }
}

static int EasyChatWordPage_GetCursorPos(EasyChatWordPage *page) {
    return page->column + page->row * 2;
}

static u32 EasyChatWordPage_GetWordIndex(EasyChatWordPage *page) {
    return page->topRow * 2 + EasyChatWordPage_GetCursorPos(page);
}

static s32 EasyChatWordPage_GetScroll(EasyChatWordPage *page) {
    return page->scroll;
}

static u16 EasyChatWordPage_GetTopRow(EasyChatWordPage *page) {
    return page->topRow;
}

static u16 EasyChatWordPage_GetMaxTopRow(EasyChatWordPage *page) {
    return page->maxTopRow;
}

static int EasyChatApp_Handler_WordList(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case EASY_CHAT_WORD_LIST_INPUT:
        EasyChatApp_WordListHandleInput(app, state);
        break;
    case EASY_CHAT_WORD_LIST_SCROLL:
        if (EasyChatAppView_IsCommandDone(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST)) {
            *state = EASY_CHAT_WORD_LIST_INPUT;
        }
        break;
    case EASY_CHAT_WORD_LIST_SCROLL_PAGE:
        if (EasyChatAppView_IsCommandDone(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST)) {
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SET_WORD_LIST_CURSOR);
            *state = EASY_CHAT_WORD_LIST_INPUT;
        }
        break;
    case EASY_CHAT_WORD_LIST_EXIT:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, app->nextHandler);
            *state = EASY_CHAT_WORD_LIST_INPUT;
        }
        break;
    }
    return 0;
}

static void EasyChatApp_WordListHandleKeys(EasyChatApp *app, u32 *state) {
    int input;

    if ((app->touchButton == 0 && app->listMode != EASY_CHAT_LIST_MODE_CATEGORIES) || (app->touchButton == 1 && app->listMode != EASY_CHAT_LIST_MODE_INITIALS) || (app->newKeys & PAD_BUTTON_SELECT)) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->touchButton = 4;
        app->category = 0;
        app->listMode ^= 1;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = EASY_CHAT_WORD_LIST_EXIT;
        return;
    }
    if (app->touchButton == 2) {
        input = EasyChatWordPage_PageUp(&app->wordPage);
    } else if (app->touchButton == 3) {
        input = EasyChatWordPage_PageDown(&app->wordPage);
    } else {
        input = EasyChatWordPage_HandleKeys(&app->wordPage, app->repeatedKeys);
    }
    switch (input) {
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SET_WORD_LIST_CURSOR);
        return;
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_SCROLL;
        return;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_SCROLL_PAGE;
        return;
    }
    if (app->newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = EASY_CHAT_WORD_LIST_EXIT;
        return;
    }
    if (app->newKeys & PAD_BUTTON_A) {
        if (app->wordPage.onCancel) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES);
            app->nextHandler = EasyChatApp_Handler_CategoryMenu;
            *state = EASY_CHAT_WORD_LIST_EXIT;
            return;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->wordPage.touchedWord = 0xFFFF;
        EasyChatApp_SetSelectedWord(app);
        app->nextHandler = EasyChatApp_Handler_Main;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_EXIT;
    }
}

static int EasyChatApp_GetWordListTouch(EasyChatApp *app) {
    int hit;
    u16 index;
    int row;
    int done;
    int top;
    TouchscreenHitbox hitbox;

    if (!System_GetTouchNew()) {
        return 0;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(sEasyChatWordListHitboxes);
    switch (hit) {
    case 0:
        return EasyChatWordPage_PageUp(&app->wordPage);
    case 1:
        return EasyChatWordPage_PageDown(&app->wordPage);
    }
    index = app->wordPage.topRow * 2;
    done = FALSE;
    row = 0;
    top = 48;
    while (row < 5) {
        int col;
        int left;

        hitbox.rect.top = top;
        hitbox.rect.bottom = hitbox.rect.top + 16;
        col = 0;
        left = 32;
        while (col < 2) {
            hitbox.rect.left = left;
            hitbox.rect.right = hitbox.rect.left + 72;
            if (TouchscreenHitbox_PointIsIn(&hitbox, gSystem.touchX, gSystem.touchY)) {
                app->wordPage.touchedWord = index;
                return 5;
            }
            index++;
            if (index >= app->wordPage.numWords) {
                done = TRUE;
                break;
            }
            col++;
            left += 104;
        }
        if (done) {
            break;
        }
        top += 24;
        row++;
    }
    return 0;
}

static void EasyChatApp_WordListHandleTouch(EasyChatApp *app, u32 *state) {
    int input = EasyChatApp_GetListButtonTouch(app);

    switch (input) {
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = EASY_CHAT_WORD_LIST_EXIT;
        return;
    case 2:
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->category = 0;
        app->listMode ^= 1;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = EASY_CHAT_WORD_LIST_EXIT;
        return;
    }
    input = EasyChatApp_GetWordListTouch(app);
    switch (input) {
    case 5:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatApp_SetSelectedWord(app);
        app->nextHandler = EasyChatApp_Handler_Main;
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_EXIT;
        return;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SET_WORD_LIST_CURSOR);
        return;
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_SCROLL;
        return;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST);
        *state = EASY_CHAT_WORD_LIST_SCROLL_PAGE;
        return;
    }
}

static void EasyChatApp_WordListHandleInput(EasyChatApp *app, u32 *state) {
    if (EasyChatApp_UpdateMenuInputState(app, state)) {
        return;
    }
    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        EasyChatApp_WordListHandleKeys(app, state);
    } else {
        EasyChatApp_WordListHandleTouch(app, state);
    }
}

static int EasyChatWordPage_HandleKeys(EasyChatWordPage *page, u32 keys) {
    u32 index;

    if (keys & PAD_KEY_DOWN) {
        if (page->onCancel) {
            return 4;
        }
        if (page->row < 4) {
            page->row++;
            index = EasyChatWordPage_GetWordIndex(page);
            if (index < page->numWords) {
                return 1;
            }
            if (index == page->numWords && (index & 1)) {
                page->column = 0;
                return 1;
            }
            page->row--;
            page->onCancel = TRUE;
            return 1;
        } else if (page->topRow < page->maxTopRow) {
            page->scroll = 1;
            page->topRow++;
            index = EasyChatWordPage_GetWordIndex(page);
            if (index < page->numWords) {
                return 2;
            }
            page->column = 0;
            return 3;
        }
        page->onCancel = TRUE;
        return 1;
    } else if (keys & PAD_KEY_UP) {
        if (page->onCancel) {
            page->onCancel = FALSE;
            return 1;
        }
        if (page->row) {
            page->row--;
            return 1;
        }
        if (page->topRow) {
            page->scroll = -1;
            page->topRow--;
            return 2;
        }
        return 4;
    } else if (keys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        page->column ^= 1;
        index = EasyChatWordPage_GetWordIndex(page);
        if (index < page->numWords) {
            return 1;
        }
        page->column ^= 1;
        return 4;
    } else if (keys & PAD_BUTTON_L) {
        return EasyChatWordPage_PageUp(page);
    } else if (keys & PAD_BUTTON_R) {
        return EasyChatWordPage_PageDown(page);
    }
    return 0;
}

static int EasyChatWordPage_PageUp(EasyChatWordPage *page) {
    u16 topRow = page->topRow;

    if (topRow != 0) {
        if (topRow >= 5) {
            page->topRow = topRow - 5;
            page->scroll = -5;
        } else {
            page->scroll = -topRow;
            page->topRow = 0;
        }
        return 2;
    }
    return 4;
}

static int EasyChatWordPage_PageDown(EasyChatWordPage *page) {
    if (page->topRow < page->maxTopRow) {
        if (page->topRow + 5 <= page->maxTopRow) {
            page->scroll = 5;
            page->topRow += 5;
        } else {
            page->scroll = page->maxTopRow - page->topRow;
            page->topRow = page->maxTopRow;
        }
        if (EasyChatWordPage_GetWordIndex(page) < page->numWords) {
            return 2;
        }
        page->column = 0;
        return 3;
    }
    return 4;
}

static void EasyChatApp_SetSelectedWord(EasyChatApp *app) {
    u32 index = app->wordPage.touchedWord;
    u16 word;

    if (index == 0xFFFF) {
        index = EasyChatWordPage_GetWordIndex(&app->wordPage);
    }
    if (app->listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
        word = ECMenu_GetCategoryWord(app->menu, app->category, index);
    } else {
        word = ECMenu_GetInitialWord(app->menu, app->category, index);
    }
    switch (app->type) {
    case EASY_CHAT_TYPE_WORD:
        app->words[0] = word;
        break;
    case EASY_CHAT_TYPE_TWO_WORDS:
        app->words[app->wordSlot] = word;
        break;
    case EASY_CHAT_TYPE_MESSAGE:
        MailMsg_SetFieldI(&app->message, app->wordSlot, word);
        break;
    }
    EasyChatApp_IsComplete(app);
}

static BOOL EasyChatApp_Handler_Exit(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_UNLOAD);
        (*state)++;
        break;
    case 1:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void EasyChatApp_SetSubHandler(EasyChatApp *app, EasyChatAppSubHandler subHandler) {
    app->subHandler = subHandler;
    app->subState = 0;
}

static void EasyChatApp_EndSubHandler(EasyChatApp *app) {
    app->subHandler = NULL;
}

static void EasyChatApp_SubHandler_Start(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_FADE_IN);
        (*state)++;
        break;
    case 1:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_EndSubHandler(app);
        }
        break;
    }
}

static void EasyChatApp_SubHandler_ConfirmFinish(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        if (EasyChatApp_HasChanged(app) || EasyChatArgs_GetAlwaysConfirm(app->args)) {
            if (EasyChatApp_IsComplete(app)) {
                EasyChatYesNo_Set(app->yesNo, 1, 0);
                EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CONFIRM_FINISH);
                *state = 1;
            } else {
                EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_SHOW_INCOMPLETE_MESSAGE);
                *state = 2;
            }
        } else {
            EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        }
        break;
    case 1:
        switch (EasyChatAppView_GetYesNoResult(app->view)) {
        case 0:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_YES_NO);
            *state = 3;
            break;
        case 1:
            if (app->type == EASY_CHAT_TYPE_MESSAGE) {
                MailMsg_SetTrailingFieldsEmpty(&app->message);
            }
            EasyChatArgs_SetResult(app->args, (MailMessage *)app->words, &app->message);
            EasyChatApp_SetHandler(app, (EasyChatAppHandler)EasyChatApp_Handler_Exit);
            *state = 3;
            break;
        }
        break;
    case 2:
        if ((app->newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) || System_GetTouchNew()) {
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_HIDE_MESSAGE);
            *state = 3;
        }
        break;
    case 3:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_EndSubHandler(app);
        }
        break;
    }
}

static void EasyChatApp_SubHandler_ConfirmQuit(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatYesNo_Set(app->yesNo, 1, 1);
        EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CONFIRM_QUIT);
        *state = 1;
        break;
    case 1:
        switch (EasyChatAppView_GetYesNoResult(app->view)) {
        case 0:
            EasyChatAppView_StartCommand(app->view, EASY_CHAT_VIEW_CMD_CLOSE_YES_NO);
            *state = 2;
            break;
        case 1:
            EasyChatApp_SetHandler(app, (EasyChatAppHandler)EasyChatApp_Handler_Exit);
            *state = 2;
            break;
        }
        break;
    case 2:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_EndSubHandler(app);
        }
        break;
    }
}

static BOOL EasyChatApp_HasChanged(EasyChatApp *app) {
    if (EasyChatArgs_MessageEquals(app->args, (MailMessage *)app->words, &app->message)) {
        return FALSE;
    }
    return TRUE;
}

static BOOL EasyChatApp_IsComplete(EasyChatApp *app) {
    switch (app->type) {
    case EASY_CHAT_TYPE_WORD:
        if (app->words[0] != EC_WORD_NULL) {
            return TRUE;
        }
        return FALSE;
    case EASY_CHAT_TYPE_TWO_WORDS:
        if (app->words[0] != EC_WORD_NULL && app->words[1] != EC_WORD_NULL) {
            return TRUE;
        }
        return FALSE;
    case EASY_CHAT_TYPE_MESSAGE:
        return MailMsg_AllFieldsAreInit(&app->message);
    default:
        return FALSE;
    }
}

static void EasyChatYesNo_Set(u8 *result, u8 a, u8 b) {
    result[0] = b;
    result[1] = a;
}

static u8 EasyChatYesNo_GetCursor(u8 *result) {
    return result[0];
}

MenuInputState *EasyChatApp_GetMenuInputStatePtr(EasyChatApp *app) {
    return &app->menuInputState;
}

u32 EasyChatApp_GetType(EasyChatApp *app) {
    return app->type;
}

u8 EasyChatApp_GetPromptMsgNo(EasyChatApp *app) {
    return EasyChatArgs_GetPromptMsgNo(app->args);
}

u8 EasyChatApp_GetListMode(EasyChatApp *app) {
    return app->listMode;
}

u16 EasyChatApp_GetMsgBank(EasyChatApp *app) {
    return MailMsg_GetMsgBank(&app->message);
}

u16 EasyChatApp_GetWord(EasyChatApp *app, int slot) {
    if (app->type == EASY_CHAT_TYPE_MESSAGE) {
        return MailMsg_GetFieldI(&app->message, slot);
    }
    return app->words[slot];
}

String *EasyChatApp_GetMessageString(EasyChatApp *app, enum HeapID heapID) {
    return MailMsg_GetRawString(&app->message, heapID);
}

u8 EasyChatApp_GetMessageIndex(EasyChatApp *app) {
    return EasyChatMessageCursor_GetIndex(&app->messageCursor);
}

u16 EasyChatApp_GetWordSlot(EasyChatApp *app) {
    return app->wordSlot;
}

u16 EasyChatApp_GetMenuCursor(EasyChatApp *app) {
    return app->menuCursor;
}

u16 EasyChatApp_GetCategory(EasyChatApp *app) {
    return app->category;
}

u32 EasyChatApp_GetCategoryWordCount(EasyChatApp *app) {
    if (app->listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
        return ECMenu_GetCategoryCount(app->menu, app->category);
    } else {
        return ECMenu_GetInitialCount(app->menu, app->category);
    }
}

void EasyChatApp_GetCategoryWord(EasyChatApp *app, u32 index, String *dest) {
    if (app->listMode == EASY_CHAT_LIST_MODE_CATEGORIES) {
        ECMenu_GetCategoryWordString(app->menu, app->category, index, dest);
    } else {
        ECMenu_GetInitialWordString(app->menu, app->category, index, dest);
    }
}

int EasyChatApp_GetWordListCursor(EasyChatApp *app) {
    if (app->wordPage.onCancel) {
        return -1;
    }
    return EasyChatWordPage_GetCursorPos(&app->wordPage);
}

s32 EasyChatApp_GetWordListScroll(EasyChatApp *app) {
    return EasyChatWordPage_GetScroll(&app->wordPage);
}

BOOL EasyChatApp_CanScrollWordListUp(EasyChatApp *app) {
    if (EasyChatWordPage_GetTopRow(&app->wordPage) != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL EasyChatApp_CanScrollWordListDown(EasyChatApp *app) {
    int topRow = EasyChatWordPage_GetTopRow(&app->wordPage);

    if (topRow < (int)EasyChatWordPage_GetMaxTopRow(&app->wordPage)) {
        return TRUE;
    }
    return FALSE;
}

u32 EasyChatApp_GetFrame(EasyChatApp *app) {
    return EasyChatArgs_GetFrame(app->args);
}

u8 EasyChatApp_GetYesNoCursor(EasyChatApp *app) {
    return EasyChatYesNo_GetCursor(app->yesNo);
}
