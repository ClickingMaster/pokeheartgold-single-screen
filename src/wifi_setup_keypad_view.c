#pragma force_active on
#include "global.h"

#include "wifi_setup_keyboard.h"
#include "wifi_setup_keypad.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Numeric keypad, second part: key highlight, cursor, the slide-out animation and teardown.

typedef struct RowPosition {
    int y;
    int x;
} RowPosition;

static inline void SetKeyPalette(GXOamAttr *oam, u32 palette) {
    oam->attr01 &= ~0xc00u;
    oam->attr2 = (oam->attr2 & ~0xf000u) | (palette << 12);
}

void WifiSetupKeypad_SetKeyHighlight(int key, int highlighted) {
    if (key < 0) {
        return;
    }
    if (key < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS) {
        WifiSetupKeypad *keypad = gWifiSetupKeypad;
        SetKeyPalette(keypad->keys[key], ov13_0224521C[highlighted]);
    } else if (key - WIFI_SETUP_KEYPAD_KEY_DELETE < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS) {
        WifiSetupKeypad *keypad = gWifiSetupKeypad;
        SetKeyPalette(keypad->extraKeys[key - WIFI_SETUP_KEYPAD_KEY_DELETE], ov13_0224521C[highlighted]);
    } else {
        WifiSetupKeypad *keypad = gWifiSetupKeypad;
        WifiSetupOamBlock_SetModeAndPalette(keypad->buttons[key - WIFI_SETUP_KEYPAD_KEY_BUTTON_0], -1, 0, ov13_02245210[highlighted]);
    }
}

// Shapes the cursor for the key it is on (the buttons get a wider frame) and moves it there.
void WifiSetupKeypad_UpdateCursor(void) {
    int templ = gWifiSetupKeypad->cursorKey <= WIFI_SETUP_KEYPAD_KEY_NEXT ? 0x44 : 0x45;
    GXOamAttr *oam = WifiSetupOamBlock_GetOam(gWifiSetupKeypad->cursor, 0);
    WifiSetupOamTemplate_Copy(0, templ, oam);
    WifiSetupOamBlock_SetPriority(gWifiSetupKeypad->cursor, -1, 2);
    WifiSetupOamBlock_SetPosition(gWifiSetupKeypad->cursor, -1, ov13_022452A2[gWifiSetupKeypad->cursorKey * 2], ov13_022452A4[gWifiSetupKeypad->cursorKey * 2]);
}

// direction: 0 left, 1 up, 2 right, 3 down. Negative entries of the move table pick the target
// from the key the cursor came from, so that moving through the wide button returns to the same
// column.
void WifiSetupKeypad_MoveCursor(int direction) {
    s8 old = gWifiSetupKeypad->cursorKey;
    gWifiSetupKeypad->cursorKey = ov13_022452DA[old][direction];
    if (gWifiSetupKeypad->cursorKey == 13 && (direction == 1 || direction == 3)) {
        gWifiSetupKeypad->returnKey = old;
    } else if (gWifiSetupKeypad->cursorKey == -1) {
        if (gWifiSetupKeypad->returnKey == 1 || gWifiSetupKeypad->returnKey == 10) {
            gWifiSetupKeypad->cursorKey = 10;
        } else {
            gWifiSetupKeypad->cursorKey = 11;
        }
    } else if (gWifiSetupKeypad->cursorKey == -2) {
        if (gWifiSetupKeypad->returnKey == 1 || gWifiSetupKeypad->returnKey == 10) {
            gWifiSetupKeypad->cursorKey = 1;
        } else {
            gWifiSetupKeypad->cursorKey = 2;
        }
    }
    WifiSetupKeypad_UpdateCursor();
    WifiSetupSound_Play(8);
}

// Each slide-out task lowers one row by 12 pixels per frame until it has left the screen, then
// starts the row above it; the buttons go first.

void WifiSetupKeypad_SlideOutButtonsTask(void *task) {
    RowPosition pos;
    u32 attr = WifiSetupOamBlock_GetOam(gWifiSetupKeypad->buttons[0], 0)->attr01;
    pos.x = (attr & 0x1ff0000) >> 16;
    pos.y = (attr & 0xff) + 12;
    WifiSetupKeypad_SetRowY(4, pos.y, pos.x);
    if (*(int *)&pos < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideOutRow3Task);
}

void WifiSetupKeypad_SlideOutRow3Task(void *task) {
    RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[9]->attr01;
    pos.x = (attr & 0x1ff0000) >> 16;
    pos.y = (attr & 0xff) + 12;
    WifiSetupKeypad_SetRowY(3, pos.y, pos.x);
    if (*(int *)&pos < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideOutRow2Task);
}

void WifiSetupKeypad_SlideOutRow2Task(void *task) {
    RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[6]->attr01;
    pos.x = (attr & 0x1ff0000) >> 16;
    pos.y = (attr & 0xff) + 12;
    WifiSetupKeypad_SetRowY(2, pos.y, pos.x);
    if (*(int *)&pos < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideOutRow1Task);
}

void WifiSetupKeypad_SlideOutRow1Task(void *task) {
    RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[3]->attr01;
    pos.x = (attr & 0x1ff0000) >> 16;
    pos.y = (attr & 0xff) + 12;
    WifiSetupKeypad_SetRowY(1, pos.y, pos.x);
    if (*(int *)&pos < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideOutRow0Task);
}

void WifiSetupKeypad_SlideOutRow0Task(void *task) {
    RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[0]->attr01;
    pos.x = (attr & 0x1ff0000) >> 16;
    pos.y = (attr & 0xff) + 12;
    WifiSetupKeypad_SetRowY(0, pos.y, pos.x);
    if (*(int *)&pos < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_DestroyTask);
}

WifiSetupKeypad *gWifiSetupKeypad;

void WifiSetupKeypad_DestroyTask(void *task) {
    int i;
    WifiSetupTask_DestroyLater(0, task);
    for (i = 0; i < 4; i++) {
        WifiSetupOamBlock_Free(gWifiSetupKeypad->rowBlocks[i]);
        WifiSetupText_FreeObjCanvas(gWifiSetupKeypad->rows[i]);
    }
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_BUTTONS; i++) {
        WifiSetupOamBlock_Free(gWifiSetupKeypad->buttons[i]);
    }
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS; i++) {
        WifiSetupOam_Release(gWifiSetupKeypad->extraKeys[i]);
    }
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS; i++) {
        WifiSetupOam_Release(gWifiSetupKeypad->keys[i]);
    }
    WifiSetupHeap_FreeAndClear(&gWifiSetupKeypad);
}
