#include "global.h"

#include "application/mailbox_app_internal.h"

#include "grid_input_handler.h"
#include "system.h"
#include "touchscreen.h"

static void MailboxApp_GridOnButton(void *data, int newTarget, int prevTarget);
static void MailboxApp_GridOnSwitchToTouchMode(void *data, int newTarget, int prevTarget);
static void MailboxApp_GridOnKeyMove(void *data, int newTarget, int prevTarget);
static void MailboxApp_GridOnTouch(void *data, int newTarget, int prevTarget);

static const TouchscreenHitbox sMailboxCloseMailHitboxes[] = {
    { .rect = { 160, 191, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sMailboxPageArrowHitboxes[] = {
    { .rect = { 160, 191, 8, 39 } },
    { .rect = { 160, 191, 40, 71 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const GridCallbacks sMailboxGridCallbacks = {
    .onButton = MailboxApp_GridOnButton,
    .onSwitchToTouchMode = MailboxApp_GridOnSwitchToTouchMode,
    .onKeyMove = MailboxApp_GridOnKeyMove,
    .onTouch = MailboxApp_GridOnTouch,
};

// The ten name plates of a page, then the CLOSE button
static const TouchscreenHitbox sMailboxGridHitboxes[] = {
    { .rect = { 0, 31, 8, 119 } },
    { .rect = { 0, 31, 136, 244 } },
    { .rect = { 32, 63, 8, 119 } },
    { .rect = { 32, 63, 136, 244 } },
    { .rect = { 64, 95, 8, 119 } },
    { .rect = { 64, 95, 136, 244 } },
    { .rect = { 96, 127, 8, 119 } },
    { .rect = { 96, 127, 136, 244 } },
    { .rect = { 128, 159, 8, 119 } },
    { .rect = { 128, 159, 136, 244 } },
    { .rect = { 160, 191, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sMailboxGridBoxes[] = {
    { 64,  16,  0, 0, 0,    2,  0,  1  },
    { 192, 16,  0, 0, 1,    3,  0,  1  },
    { 64,  48,  0, 0, 0,    4,  2,  3  },
    { 192, 48,  0, 0, 1,    5,  2,  3  },
    { 64,  80,  0, 0, 2,    6,  4,  5  },
    { 192, 80,  0, 0, 3,    7,  4,  5  },
    { 64,  112, 0, 0, 4,    8,  6,  7  },
    { 192, 112, 0, 0, 5,    9,  6,  7  },
    { 64,  144, 0, 0, 6,    10, 8,  9  },
    { 192, 144, 0, 0, 7,    10, 8,  9  },
    { 224, 176, 0, 0, 0x89, 10, 10, 10 },
};

// 0 / 1: the page arrows were touched.
int MailboxApp_GetTouchedPageArrow(MailboxApp *app) {
    if (app->data->lastPage == 0) {
        return -1;
    }
    return TouchscreenHitbox_FindRectAtTouchNew(sMailboxPageArrowHitboxes);
}

void MailboxApp_CreateGridInput(MailboxApp *app) {
    u32 cursorPos;

    if (app->page > app->data->lastPage) {
        app->page = app->data->lastPage;
        cursorPos = 0;
    } else {
        cursorPos = app->cursorPos;
    }
    app->data->gridInput = GridInputHandler_Create(sMailboxGridHitboxes, sMailboxGridBoxes, &sMailboxGridCallbacks, app, TRUE, (u8)cursorPos, HEAP_ID_157);
    MailboxApp_MoveCursor(app, cursorPos);
}

void MailboxApp_FreeGridInput(MailboxApp *app) {
    GridInputHandler_Free(app->data->gridInput);
}

static void MailboxApp_GridOnButton(void *data, int newTarget, int prevTarget) {
}

static void MailboxApp_GridOnSwitchToTouchMode(void *data, int newTarget, int prevTarget) {
}

static void MailboxApp_GridOnKeyMove(void *data, int newTarget, int prevTarget) {
    MailboxApp_MoveCursor(data, newTarget);
}

static void MailboxApp_GridOnTouch(void *data, int newTarget, int prevTarget) {
    MailboxApp_MoveCursor(data, newTarget);
}

void MailboxApp_MoveCursor(MailboxApp *app, u32 input) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(app->data->gridInput, input);

    MailboxAppData_SetSpritePosition(app->data, 2, box->left, box->top);
    if (input == 10) {
        MailboxAppData_SetSpriteAnim(app->data, 2, 4);
    } else {
        MailboxAppData_SetSpriteAnim(app->data, 2, 5);
    }
}

// The mail being read is closed by touching the CLOSE button or pressing A or B.
BOOL MailboxApp_IsCloseMailInput(void) {
    if (TouchscreenHitbox_FindRectAtTouchNew(sMailboxCloseMailHitboxes) != -1) {
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}
