#include "easy_chat_app.h"

#include "global.h"

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

// Word menus of the easy chat word picker (ARM9, easy_chat_menu_*.c).
ECMenu *ECMenu_New(enum HeapID heapID, EasyChatArgs *args);
void ECMenu_Delete(ECMenu *menu);
u32 ECMenu_GetCategoryCount(ECMenu *menu, u32 category);
void ECMenu_GetCategoryWordString(ECMenu *menu, u32 category, u32 index, String *dest);
u16 ECMenu_GetCategoryWord(ECMenu *menu, u32 category, u32 index);
u32 ECMenu_GetInitialCount(ECMenu *menu, u32 group);
void ECMenu_GetInitialWordString(ECMenu *menu, u32 group, u32 index, String *dest);
u16 ECMenu_GetInitialWord(ECMenu *menu, u32 group, u32 index);

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
    u32 type;                             // 0x04, 0/1: one/two words, 2: mail message
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
    u8 listMode;                          // 0x6B, 0: categories, 1: initials
    u8 dragging;                          // 0x6C
    s16 dragX;                            // 0x6E
    u8 unk_70[2];                         // 0x70
    MenuInputState menuInputState;        // 0x74
    int unk_78;                           // 0x78
}; // size: 0x7C

extern const TouchscreenHitbox ov102_021EC5E0[];
extern const TouchscreenHitbox ov102_021EC5F4[];
extern const TouchscreenHitbox ov102_021EC600[];
extern const TouchscreenHitbox ov102_021EC610[];
extern const TouchscreenHitbox ov102_021EC620[];
extern const TouchscreenHitbox ov102_021EC634[];
extern const TouchscreenHitbox ov102_021EC648[];
// Category grid neighbours, 4 bytes per category (13th: the back button): up, down, left, right.
extern const u8 ov102_021EC65C[];
extern const u8 ov102_021EC65D[];
extern const u8 ov102_021EC65E[];
extern const u8 ov102_021EC65F[];
extern const EasyChatAppHandler ov102_021EC5E8[];
extern BOOL (*const _021EC5D8[])(EasyChatApp *app);

void EasyChatApp_OnTouchButton(u32 index, u32 event, void *arg);
EasyChatApp *EasyChatApp_Create(OverlayManager *man);
void EasyChatMessageCursor_Init(EasyChatMessageCursor *cursor, MailMessage *message);
void EasyChatMessageCursor_Next(EasyChatMessageCursor *cursor, MailMessage *message);
void EasyChatMessageCursor_Prev(EasyChatMessageCursor *cursor, MailMessage *message);
void EasyChatMessageCursor_SetIndex(EasyChatMessageCursor *cursor, MailMessage *message, u8 index);
u8 EasyChatMessageCursor_GetIndex(EasyChatMessageCursor *cursor);
void EasyChatApp_Delete(EasyChatApp *app, OverlayManager *man);
void EasyChatApp_SetHandler(EasyChatApp *app, EasyChatAppHandler handler);
BOOL EasyChatApp_UpdateMenuInputState(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_Main(EasyChatApp *app, u32 *state);
int EasyChatApp_WordMenuAction(EasyChatApp *app, int action);
int EasyChatApp_WordMenuHandleKeys(EasyChatApp *app);
int EasyChatApp_GetOneWordMenuTouch(EasyChatApp *app);
int EasyChatApp_GetTwoWordMenuTouch(EasyChatApp *app);
int EasyChatApp_WordMenuHandleTouch(EasyChatApp *app);
int EasyChatApp_WordMenuRun(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_WordMenu(EasyChatApp *app, u32 *state);
int EasyChatApp_MessageMenuAction(EasyChatApp *app, int action);
int EasyChatApp_MessageMenuHandleKeys(EasyChatApp *app);
BOOL EasyChatApp_IsTouchOnSlider(EasyChatApp *app, int x, int y);
BOOL EasyChatApp_IsTouchInSliderRange(EasyChatApp *app, int x, int y);
int EasyChatApp_GetMessageMenuTouch(EasyChatApp *app);
int EasyChatApp_MessageMenuHandleTouch(EasyChatApp *app);
int EasyChatApp_MessageMenuRun(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_MessageMenu(EasyChatApp *app, u32 *state);
int EasyChatApp_Handler_CategoryMenu(EasyChatApp *app, u32 *state);
void EasyChatApp_CategoryMenuHandleKeys(EasyChatApp *app, u32 *state);
int EasyChatApp_GetListButtonTouch(EasyChatApp *app);
int EasyChatApp_GetCategoryGridTouch(EasyChatApp *app);
int EasyChatApp_GetInitialGridTouch(EasyChatApp *app);
void EasyChatApp_CategoryMenuHandleTouch(EasyChatApp *app, u32 *state);
void EasyChatApp_CategoryMenuHandleInput(EasyChatApp *app, u32 *state);
BOOL EasyChatApp_CategoryHasWords(ECMenu *menu, u8 listMode, u16 category);
BOOL EasyChatApp_MoveCategoryCursor(EasyChatApp *app);
BOOL EasyChatApp_MoveCategoryGridCursor(EasyChatApp *app);
BOOL EasyChatApp_MoveInitialGridCursor(EasyChatApp *app);
void EasyChatWordPage_Init(EasyChatWordPage *page, EasyChatApp *app);
int EasyChatWordPage_GetCursorPos(EasyChatWordPage *page);
u32 EasyChatWordPage_GetWordIndex(EasyChatWordPage *page);
s32 EasyChatWordPage_GetScroll(EasyChatWordPage *page);
u16 EasyChatWordPage_GetTopRow(EasyChatWordPage *page);
u16 EasyChatWordPage_GetMaxTopRow(EasyChatWordPage *page);
int EasyChatApp_Handler_WordList(EasyChatApp *app, u32 *state);
void EasyChatApp_WordListHandleKeys(EasyChatApp *app, u32 *state);
int EasyChatApp_GetWordListTouch(EasyChatApp *app);
void EasyChatApp_WordListHandleTouch(EasyChatApp *app, u32 *state);
void EasyChatApp_WordListHandleInput(EasyChatApp *app, u32 *state);
int EasyChatWordPage_HandleKeys(EasyChatWordPage *page, u32 keys);
int EasyChatWordPage_PageUp(EasyChatWordPage *page);
int EasyChatWordPage_PageDown(EasyChatWordPage *page);
void EasyChatApp_SetSelectedWord(EasyChatApp *app);
BOOL EasyChatApp_Handler_Exit(EasyChatApp *app, u32 *state);
void EasyChatApp_SetSubHandler(EasyChatApp *app, EasyChatAppSubHandler subHandler);
void EasyChatApp_EndSubHandler(EasyChatApp *app);
void EasyChatApp_SubHandler_Start(EasyChatApp *app, u32 *state);
void EasyChatApp_SubHandler_ConfirmFinish(EasyChatApp *app, u32 *state);
void EasyChatApp_SubHandler_ConfirmQuit(EasyChatApp *app, u32 *state);
BOOL EasyChatApp_HasChanged(EasyChatApp *app);
BOOL EasyChatApp_IsComplete(EasyChatApp *app);
void ov102_021E8F5C(u8 *result, u8 a, u8 b);
u8 ov102_021E8F64(u8 *result);

#pragma force_active on

BOOL EasyChat_Init(OverlayManager *man, int *state) {
    EasyChatApp *app;

    switch (*state) {
    case 0:
        Sound_SetSceneAndPlayBGM(62, 0, 0);
        Heap_Create(HEAP_ID_3, HEAP_ID_34, 0x8000);
        Heap_Create(HEAP_ID_3, HEAP_ID_35, 0x28000);
        app = EasyChatApp_Create(man);
        EasyChatAppView_StartCommand(app->view, 0);
        (*state)++;
        break;
    case 1:
        app = OverlayManager_GetData(man);
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            app->numSlots = app->type == 2 ? EasyChatAppView_GetNumWordSlots(app->view) : 0;
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

void EasyChatApp_OnTouchButton(u32 index, u32 event, void *arg) {
    EasyChatApp *app = arg;

    switch (event) {
    case 0:
        app->touchButton = index;
        switch (index) {
        case 2:
            EasyChatAppView_StartCommand(app->view, 28);
            break;
        case 3:
            EasyChatAppView_StartCommand(app->view, 29);
            break;
        }
        return;
    case 1:
    case 3:
        switch (index) {
        case 2:
            EasyChatAppView_StartCommand(app->view, 30);
            break;
        case 3:
            EasyChatAppView_StartCommand(app->view, 31);
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

#pragma force_active reset

BOOL EasyChat_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    EasyChatApp_Delete(OverlayManager_GetData(man), man);
    Heap_Destroy(HEAP_ID_34);
    Heap_Destroy(HEAP_ID_35);
    return TRUE;
}

#pragma force_active on

EasyChatApp *EasyChatApp_Create(OverlayManager *man) {
    EasyChatApp *app = OverlayManager_CreateAndGetData(man, sizeof(EasyChatApp), HEAP_ID_34);

    MI_CpuFill8(app, 0, sizeof(EasyChatApp));
    app->args = OverlayManager_GetArgs(man);
    app->type = sub_02090D70(app->args);
    app->menuInputState = (MenuInputState)sub_02090E44(app->args);
    sub_02090D8C(app->args, (MailMessage *)app->words, &app->message);
    if (app->type == 2) {
        EasyChatMessageCursor_Init(&app->messageCursor, &app->message);
    }
    app->menu = ECMenu_New(HEAP_ID_34, app->args);
    app->view = EasyChatAppView_Create(app, app->menu);
    app->touchHitboxes = TouchHitboxController_Create(ov102_021EC648, 5, EasyChatApp_OnTouchButton, app, HEAP_ID_34);
    app->unk_44 = 0;
    app->unk_48 = 0;
    app->listMode = 0;
    app->subHandler = NULL;
    app->subState = 0;
    app->wordSlot = 0;
    EasyChatApp_SetHandler(app, EasyChatApp_Handler_Main);
    EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_Start);
    return app;
}

void EasyChatMessageCursor_Init(EasyChatMessageCursor *cursor, MailMessage *message) {
    int i;

    cursor->bank = MailMsg_GetMsgBank(message);
    cursor->count = 20;
    cursor->number = MailMsg_GetMsgNo(message);
    for (i = 0; i < 5; i++) {
        cursor->total += cursor->count;
    }
}

#pragma force_active reset

void EasyChatMessageCursor_Next(EasyChatMessageCursor *cursor, MailMessage *message) {
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

void EasyChatMessageCursor_Prev(EasyChatMessageCursor *cursor, MailMessage *message) {
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

void EasyChatMessageCursor_SetIndex(EasyChatMessageCursor *cursor, MailMessage *message, u8 index) {
    cursor->number = index % cursor->count;
    cursor->bank = index / cursor->count;
    SetMsgBankAndNum(message, cursor->bank, cursor->number);
}

u8 EasyChatMessageCursor_GetIndex(EasyChatMessageCursor *cursor) {
    return cursor->number + cursor->bank * cursor->count;
}

void EasyChatApp_Delete(EasyChatApp *app, OverlayManager *man) {
    sub_02090E5C(app->args, app->menuInputState);
    TouchHitboxController_Destroy(app->touchHitboxes);
    EasyChatAppView_Delete(app->view);
    ECMenu_Delete(app->menu);
    OverlayManager_FreeData(man);
}

void EasyChatApp_SetHandler(EasyChatApp *app, EasyChatAppHandler handler) {
    app->handler = handler;
    app->state = 0;
    app->unk_78 = 0;
}

BOOL EasyChatApp_UpdateMenuInputState(EasyChatApp *app, u32 *state) {
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

int EasyChatApp_Handler_Main(EasyChatApp *app, u32 *state) {
    return ov102_021EC5E8[app->type](app, state);
}

int EasyChatApp_WordMenuAction(EasyChatApp *app, int action) {
    switch (action) {
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        return 2;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        return 1;
    case 0:
        PlaySE(SEQ_SE_DP_SELECT);
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, 9);
        return 4;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 15);
        break;
    }
    return 0;
}

int EasyChatApp_WordMenuHandleKeys(EasyChatApp *app) {
    u16 keys = app->newKeys;

    if (keys & PAD_BUTTON_B) {
        app->menuCursor = 2;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_WordMenuAction(app, 2);
    }
    if (keys & PAD_BUTTON_A) {
        return EasyChatApp_WordMenuAction(app, app->menuCursor);
    }
    if (keys & PAD_BUTTON_START) {
        app->menuCursor = 1;
        return EasyChatApp_WordMenuAction(app, 3);
    }
    if (keys & PAD_KEY_DOWN) {
        if (app->menuCursor == 2) {
            return 0;
        }
        app->menuCursor++;
        return EasyChatApp_WordMenuAction(app, 3);
    }
    if (keys & PAD_KEY_UP) {
        if (app->menuCursor == 0) {
            return 0;
        }
        app->menuCursor--;
        return EasyChatApp_WordMenuAction(app, 3);
    }
    if (app->type == 0) {
        return 0;
    }
    if (keys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        app->wordSlot ^= 1;
        return EasyChatApp_WordMenuAction(app, 3);
    }
    return 0;
}

int EasyChatApp_GetOneWordMenuTouch(EasyChatApp *app) {
#pragma unused(app)
    return TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC610);
}

int EasyChatApp_GetTwoWordMenuTouch(EasyChatApp *app) {
#pragma unused(app)
    return TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC620);
}

int EasyChatApp_WordMenuHandleTouch(EasyChatApp *app) {
    int result;

    if (app->type == 0) {
        result = EasyChatApp_GetOneWordMenuTouch(app);
    } else {
        result = EasyChatApp_GetTwoWordMenuTouch(app);
    }
    switch (result) {
    case 0:
        app->menuCursor = 1;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_WordMenuAction(app, 1);
    case 1:
        app->menuCursor = 2;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_WordMenuAction(app, 2);
    case 2:
    case 3:
        app->category = 0;
        app->wordSlot = result - 2;
        app->menuCursor = 0;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_WordMenuAction(app, 0);
    }
    return 0;
}

int EasyChatApp_WordMenuRun(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
                *state = EasyChatApp_WordMenuHandleKeys(app);
            } else {
                *state = EasyChatApp_WordMenuHandleTouch(app);
            }
        }
        break;
    case 1:
        *state = 3;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmFinish);
        break;
    case 2:
        *state = 3;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        break;
    case 3:
        *state = 0;
        break;
    case 4:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, EasyChatApp_Handler_CategoryMenu);
        }
        break;
    }
    return 0;
}

int EasyChatApp_Handler_WordMenu(EasyChatApp *app, u32 *state) {
    if (*state == 0 && EasyChatApp_UpdateMenuInputState(app, state)) {
        return 0;
    }
    return EasyChatApp_WordMenuRun(app, state);
}

int EasyChatApp_MessageMenuAction(EasyChatApp *app, int action) {
    switch (action) {
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        return 3;
    case 0:
        if (EasyChatAppView_GetNumWordSlots(app->view) == 0) {
            return 1;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, 9);
        return 6;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 15);
        break;
    case 4:
        PlaySE(SEQ_SE_DP_SELECT);
        app->wordSlot = 0;
        EasyChatAppView_StartCommand(app->view, 3);
        return 2;
    }
    return 1;
}

#pragma force_active on

int EasyChatApp_MessageMenuHandleKeys(EasyChatApp *app) {
    if (app->newKeys & PAD_BUTTON_B) {
        app->menuCursor = 2;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_MessageMenuAction(app, 2);
    }
    if (app->newKeys & PAD_BUTTON_A) {
        return EasyChatApp_MessageMenuAction(app, app->menuCursor);
    }
    if (app->newKeys & PAD_KEY_UP) {
        if (app->menuCursor != 0) {
            app->menuCursor--;
            return EasyChatApp_MessageMenuAction(app, 3);
        }
        if (app->numSlots != 0 && app->wordSlot != 0) {
            app->wordSlot--;
            return EasyChatApp_MessageMenuAction(app, 3);
        }
        return 1;
    }
    if (app->newKeys & PAD_KEY_DOWN) {
        if (app->menuCursor == 0) {
            if (app->numSlots == 2 && app->wordSlot == 0) {
                app->wordSlot++;
                return EasyChatApp_MessageMenuAction(app, 3);
            }
            app->menuCursor++;
            return EasyChatApp_MessageMenuAction(app, 3);
        }
        if (app->menuCursor == 1) {
            app->menuCursor++;
            return EasyChatApp_MessageMenuAction(app, 3);
        }
        return 1;
    }
    if (app->repeatedKeys & PAD_KEY_LEFT) {
        EasyChatMessageCursor_Prev(&app->messageCursor, &app->message);
        return EasyChatApp_MessageMenuAction(app, 4);
    }
    if (app->repeatedKeys & PAD_KEY_RIGHT) {
        EasyChatMessageCursor_Next(&app->messageCursor, &app->message);
        return EasyChatApp_MessageMenuAction(app, 4);
    }
    return 1;
}

#pragma force_active reset

BOOL EasyChatApp_IsTouchOnSlider(EasyChatApp *app, int x, int y) {
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

BOOL EasyChatApp_IsTouchInSliderRange(EasyChatApp *app, int x, int y) {
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

#pragma force_active on

int EasyChatApp_GetMessageMenuTouch(EasyChatApp *app) {
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
        EasyChatAppView_StartCommand(app->view, 3);
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
        result = TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC634);
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

#pragma force_active reset

int EasyChatApp_MessageMenuHandleTouch(EasyChatApp *app) {
    int choice = EasyChatApp_GetMessageMenuTouch(app);

    switch (choice) {
    case 0:
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        app->menuCursor = choice + 1;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_MessageMenuAction(app, app->menuCursor);
    case 2:
    case 3:
        app->menuCursor = 0;
        EasyChatAppView_StartCommand(app->view, 15);
        if (choice == 2) {
            EasyChatMessageCursor_Prev(&app->messageCursor, &app->message);
        } else {
            EasyChatMessageCursor_Next(&app->messageCursor, &app->message);
        }
        return EasyChatApp_MessageMenuAction(app, 4);
    case 4:
    case 5:
        app->menuCursor = 0;
        app->wordSlot = choice - 4;
        EasyChatAppView_StartCommand(app->view, 15);
        return EasyChatApp_MessageMenuAction(app, 0);
    case 6:
        app->menuCursor = 0;
        EasyChatAppView_StartCommand(app->view, 15);
        break;
    }
    return 1;
}

int EasyChatApp_MessageMenuRun(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        app->numSlots = EasyChatAppView_GetNumWordSlots(app->view);
        *state = 1;
    case 1:
        if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
            *state = EasyChatApp_MessageMenuHandleKeys(app);
        } else {
            *state = EasyChatApp_MessageMenuHandleTouch(app);
        }
        break;
    case 2:
        if (EasyChatAppView_IsCommandDone(app->view, 3)) {
            *state = 0;
        }
        break;
    case 3:
        *state = 5;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmFinish);
        break;
    case 4:
        *state = 5;
        EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        break;
    case 5:
        *state = 1;
        break;
    case 6:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, EasyChatApp_Handler_CategoryMenu);
        }
        break;
    }
    return 0;
}

int EasyChatApp_Handler_MessageMenu(EasyChatApp *app, u32 *state) {
    if (*state <= 1 && EasyChatApp_UpdateMenuInputState(app, state)) {
        return 0;
    }
    return EasyChatApp_MessageMenuRun(app, state);
}

int EasyChatApp_Handler_CategoryMenu(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        if (EasyChatAppView_IsCommandDone(app->view, 18)) {
            EasyChatApp_CategoryMenuHandleInput(app, state);
        }
        break;
    case 1:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, app->nextHandler);
        }
        break;
    case 2:
        if (EasyChatAppView_IsCommandDone(app->view, 27)) {
            *state = 0;
        }
        break;
    }
    return 0;
}

void EasyChatApp_CategoryMenuHandleKeys(EasyChatApp *app, u32 *state) {
    u16 keys;
    u32 touchButton = app->touchButton;

    if ((touchButton == 0 && app->listMode != 0) || (touchButton == 1 && app->listMode != 1) || (app->newKeys & PAD_BUTTON_SELECT)) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->touchButton = 4;
        app->listMode ^= 1;
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, 27);
        *state = 2;
        return;
    }
    keys = app->newKeys;
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 10);
        app->nextHandler = EasyChatApp_Handler_Main;
        *state = 1;
        return;
    }
    if (keys & PAD_BUTTON_A) {
        u16 category = app->category;

        if (category == 0xFE) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatAppView_StartCommand(app->view, 10);
            app->nextHandler = EasyChatApp_Handler_Main;
            *state = 1;
            return;
        }
        if (EasyChatApp_CategoryHasWords(app->menu, app->listMode, category)) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatWordPage_Init(&app->wordPage, app);
            app->nextHandler = EasyChatApp_Handler_WordList;
            EasyChatAppView_StartCommand(app->view, 11);
            *state = 1;
            return;
        }
        PlaySE(SEQ_SE_DP_CUSTOM06);
        return;
    }
    if (EasyChatApp_MoveCategoryCursor(app)) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 17);
    }
}

int EasyChatApp_GetListButtonTouch(EasyChatApp *app) {
    int result = TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC600);

    if (result == -1) {
        return 0;
    }
    switch (result) {
    case 0:
        return 1;
    case 1:
        if (app->listMode == 0) {
            return 0;
        }
        break;
    case 2:
        if (app->listMode == 1) {
            return 0;
        }
        break;
    }
    return 2;
}

#pragma force_active on

int EasyChatApp_GetCategoryGridTouch(EasyChatApp *app) {
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

#pragma force_active reset

int EasyChatApp_GetInitialGridTouch(EasyChatApp *app) {
    int result;
    int hit = TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC5E0);

    result = -1;
    if (hit != result) {
        result = EasyChatInitialGrid_FindCellAt((s16)(gSystem.touchX - 20), (s16)(gSystem.touchY - 64));
    }
    return result;
}

void EasyChatApp_CategoryMenuHandleTouch(EasyChatApp *app, u32 *state) {
    int input = EasyChatApp_GetListButtonTouch(app);

    switch (input) {
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 10);
        app->nextHandler = EasyChatApp_Handler_Main;
        *state = 1;
        return;
    case 2:
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->listMode ^= 1;
        app->category = 0;
        EasyChatAppView_StartCommand(app->view, 27);
        *state = 2;
        return;
    }
    if (app->listMode == 0) {
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
    EasyChatAppView_StartCommand(app->view, 11);
    *state = 1;
}

void EasyChatApp_CategoryMenuHandleInput(EasyChatApp *app, u32 *state) {
    if (EasyChatApp_UpdateMenuInputState(app, state)) {
        return;
    }
    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        EasyChatApp_CategoryMenuHandleKeys(app, state);
    } else {
        EasyChatApp_CategoryMenuHandleTouch(app, state);
    }
}

#pragma force_active on

BOOL EasyChatApp_CategoryHasWords(ECMenu *menu, u8 listMode, u16 category) {
    if (category == 0xFE || category == 0xFF) {
        return FALSE;
    }
    if (listMode == 0) {
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

#pragma force_active reset

BOOL EasyChatApp_MoveCategoryCursor(EasyChatApp *app) {
    return _021EC5D8[app->listMode](app);
}

BOOL EasyChatApp_MoveCategoryGridCursor(EasyChatApp *app) {
    u16 category = app->category;
    u16 index = category;

    if (category == 0xFE) {
        index = 12;
    }
    if (category != 0xFE) {
        app->prevCategory = category;
        if (app->repeatedKeys & PAD_KEY_UP) {
            app->category = ov102_021EC65C[index * 4];
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_DOWN) {
            app->category = ov102_021EC65D[index * 4];
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_LEFT) {
            app->category = ov102_021EC65E[index * 4];
            return TRUE;
        }
        if (app->repeatedKeys & PAD_KEY_RIGHT) {
            app->category = ov102_021EC65F[index * 4];
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

    if (category != 0xFE) {
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

void EasyChatWordPage_Init(EasyChatWordPage *page, EasyChatApp *app) {
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

int EasyChatWordPage_GetCursorPos(EasyChatWordPage *page) {
    return page->column + page->row * 2;
}

u32 EasyChatWordPage_GetWordIndex(EasyChatWordPage *page) {
    return page->topRow * 2 + EasyChatWordPage_GetCursorPos(page);
}

s32 EasyChatWordPage_GetScroll(EasyChatWordPage *page) {
    return page->scroll;
}

u16 EasyChatWordPage_GetTopRow(EasyChatWordPage *page) {
    return page->topRow;
}

u16 EasyChatWordPage_GetMaxTopRow(EasyChatWordPage *page) {
    return page->maxTopRow;
}

int EasyChatApp_Handler_WordList(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatApp_WordListHandleInput(app, state);
        break;
    case 1:
        if (EasyChatAppView_IsCommandDone(app->view, 19)) {
            *state = 0;
        }
        break;
    case 2:
        if (EasyChatAppView_IsCommandDone(app->view, 19)) {
            EasyChatAppView_StartCommand(app->view, 18);
            *state = 0;
        }
        break;
    case 3:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_SetHandler(app, app->nextHandler);
            *state = 0;
        }
        break;
    }
    return 0;
}

void EasyChatApp_WordListHandleKeys(EasyChatApp *app, u32 *state) {
    int input;

    if ((app->touchButton == 0 && app->listMode != 0) || (app->touchButton == 1 && app->listMode != 1) || (app->newKeys & PAD_BUTTON_SELECT)) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->touchButton = 4;
        app->category = 0;
        app->listMode ^= 1;
        EasyChatAppView_StartCommand(app->view, 12);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = 3;
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
        EasyChatAppView_StartCommand(app->view, 18);
        return;
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 19);
        *state = 1;
        return;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 19);
        *state = 2;
        return;
    }
    if (app->newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 12);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = 3;
        return;
    }
    if (app->newKeys & PAD_BUTTON_A) {
        if (app->wordPage.onCancel) {
            PlaySE(SEQ_SE_DP_SELECT);
            EasyChatAppView_StartCommand(app->view, 12);
            app->nextHandler = EasyChatApp_Handler_CategoryMenu;
            *state = 3;
            return;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->wordPage.touchedWord = 0xFFFF;
        EasyChatApp_SetSelectedWord(app);
        app->nextHandler = EasyChatApp_Handler_Main;
        EasyChatAppView_StartCommand(app->view, 13);
        *state = 3;
    }
}

int EasyChatApp_GetWordListTouch(EasyChatApp *app) {
    int hit;
    u16 index;
    int row;
    int done;
    int top;
    TouchscreenHitbox hitbox;

    if (!System_GetTouchNew()) {
        return 0;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC5F4);
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

void EasyChatApp_WordListHandleTouch(EasyChatApp *app, u32 *state) {
    int input = EasyChatApp_GetListButtonTouch(app);

    switch (input) {
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 12);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = 3;
        return;
    case 2:
        PlaySE(SEQ_SE_DP_BUTTON9);
        app->category = 0;
        app->listMode ^= 1;
        EasyChatAppView_StartCommand(app->view, 12);
        app->nextHandler = EasyChatApp_Handler_CategoryMenu;
        *state = 3;
        return;
    }
    input = EasyChatApp_GetWordListTouch(app);
    switch (input) {
    case 5:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatApp_SetSelectedWord(app);
        app->nextHandler = EasyChatApp_Handler_Main;
        EasyChatAppView_StartCommand(app->view, 13);
        *state = 3;
        return;
    case 1:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 18);
        return;
    case 2:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 19);
        *state = 1;
        return;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        EasyChatAppView_StartCommand(app->view, 19);
        *state = 2;
        return;
    }
}

void EasyChatApp_WordListHandleInput(EasyChatApp *app, u32 *state) {
    if (EasyChatApp_UpdateMenuInputState(app, state)) {
        return;
    }
    if (app->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        EasyChatApp_WordListHandleKeys(app, state);
    } else {
        EasyChatApp_WordListHandleTouch(app, state);
    }
}

#pragma force_active on

int EasyChatWordPage_HandleKeys(EasyChatWordPage *page, u32 keys) {
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

#pragma force_active reset

int EasyChatWordPage_PageUp(EasyChatWordPage *page) {
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

#pragma force_active on

int EasyChatWordPage_PageDown(EasyChatWordPage *page) {
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

void EasyChatApp_SetSelectedWord(EasyChatApp *app) {
    u32 index = app->wordPage.touchedWord;
    u16 word;

    if (index == 0xFFFF) {
        index = EasyChatWordPage_GetWordIndex(&app->wordPage);
    }
    if (app->listMode == 0) {
        word = ECMenu_GetCategoryWord(app->menu, app->category, index);
    } else {
        word = ECMenu_GetInitialWord(app->menu, app->category, index);
    }
    switch (app->type) {
    case 0:
        app->words[0] = word;
        break;
    case 1:
        app->words[app->wordSlot] = word;
        break;
    case 2:
        MailMsg_SetFieldI(&app->message, app->wordSlot, word);
        break;
    }
    EasyChatApp_IsComplete(app);
}

#pragma force_active reset

BOOL EasyChatApp_Handler_Exit(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatAppView_StartCommand(app->view, 1);
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

void EasyChatApp_SetSubHandler(EasyChatApp *app, EasyChatAppSubHandler subHandler) {
    app->subHandler = subHandler;
    app->subState = 0;
}

void EasyChatApp_EndSubHandler(EasyChatApp *app) {
    app->subHandler = NULL;
}

void EasyChatApp_SubHandler_Start(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        EasyChatAppView_StartCommand(app->view, 2);
        (*state)++;
        break;
    case 1:
        if (EasyChatAppView_AreCommandsDone(app->view)) {
            EasyChatApp_EndSubHandler(app);
        }
        break;
    }
}

#pragma force_active on

void EasyChatApp_SubHandler_ConfirmFinish(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        if (EasyChatApp_HasChanged(app) || sub_02090D88(app->args)) {
            if (EasyChatApp_IsComplete(app)) {
                ov102_021E8F5C(app->unk_70, 1, 0);
                EasyChatAppView_StartCommand(app->view, 21);
                *state = 1;
            } else {
                EasyChatAppView_StartCommand(app->view, 23);
                *state = 2;
            }
        } else {
            EasyChatApp_SetSubHandler(app, EasyChatApp_SubHandler_ConfirmQuit);
        }
        break;
    case 1:
        switch (EasyChatAppView_GetYesNoResult(app->view)) {
        case 0:
            EasyChatAppView_StartCommand(app->view, 25);
            *state = 3;
            break;
        case 1:
            if (app->type == 2) {
                MailMsg_SetTrailingFieldsEmpty(&app->message);
            }
            sub_02090E04(app->args, (MailMessage *)app->words, &app->message);
            EasyChatApp_SetHandler(app, (EasyChatAppHandler)EasyChatApp_Handler_Exit);
            *state = 3;
            break;
        }
        break;
    case 2:
        if ((app->newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) || System_GetTouchNew()) {
            EasyChatAppView_StartCommand(app->view, 20);
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

#pragma force_active reset

void EasyChatApp_SubHandler_ConfirmQuit(EasyChatApp *app, u32 *state) {
    switch (*state) {
    case 0:
        ov102_021E8F5C(app->unk_70, 1, 1);
        EasyChatAppView_StartCommand(app->view, 22);
        *state = 1;
        break;
    case 1:
        switch (EasyChatAppView_GetYesNoResult(app->view)) {
        case 0:
            EasyChatAppView_StartCommand(app->view, 25);
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

BOOL EasyChatApp_HasChanged(EasyChatApp *app) {
    if (sub_02090DC0(app->args, (MailMessage *)app->words, &app->message)) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

BOOL EasyChatApp_IsComplete(EasyChatApp *app) {
    switch (app->type) {
    case 0:
        if (app->words[0] != 0xFFFF) {
            return TRUE;
        }
        return FALSE;
    case 1:
        if (app->words[0] != 0xFFFF && app->words[1] != 0xFFFF) {
            return TRUE;
        }
        return FALSE;
    case 2:
        return MailMsg_AllFieldsAreInit(&app->message);
    default:
        return FALSE;
    }
}

#pragma force_active reset

void ov102_021E8F5C(u8 *result, u8 a, u8 b) {
    result[0] = b;
    result[1] = a;
}

u8 ov102_021E8F64(u8 *result) {
    return result[0];
}

MenuInputState *EasyChatApp_GetMenuInputStatePtr(EasyChatApp *app) {
    return &app->menuInputState;
}

u32 EasyChatApp_GetType(EasyChatApp *app) {
    return app->type;
}

u8 ov102_021E8F70(EasyChatApp *app) {
    return sub_02090D74(app->args);
}

u8 EasyChatApp_GetListMode(EasyChatApp *app) {
    return app->listMode;
}

u16 EasyChatApp_GetMsgBank(EasyChatApp *app) {
    return MailMsg_GetMsgBank(&app->message);
}

u16 EasyChatApp_GetWord(EasyChatApp *app, int slot) {
    if (app->type == 2) {
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
    if (app->listMode == 0) {
        return ECMenu_GetCategoryCount(app->menu, app->category);
    } else {
        return ECMenu_GetInitialCount(app->menu, app->category);
    }
}

void EasyChatApp_GetCategoryWord(EasyChatApp *app, u32 index, String *dest) {
    if (app->listMode == 0) {
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

u32 ov102_021E9084(EasyChatApp *app) {
    return sub_02090D78(app->args);
}

u8 ov102_021E9090(EasyChatApp *app) {
    return ov102_021E8F64(app->unk_70);
}
