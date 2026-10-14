#pragma force_active on
#include "global.h"

#include "wifi_setup_keyboard.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Software keyboard, third part: page switching, key highlight, cursor, the slide-out animation
// and teardown.

typedef struct RowPosition {
    int y;
    int x;
} RowPosition;

static inline void SetKeyPalette(GXOamAttr *oam, u32 palette) {
    oam->attr01 &= ~0xc00u;
    oam->attr2 = (oam->attr2 & ~0xf000u) | (palette << 12);
}

#pragma opt_strength_reduction off

void WifiSetupKeyboard_SetPage(int page) {
    int i;
    int altPage = FALSE, shift = FALSE;
    u32 key;
    gWifiSetupKeyboard->page = page;
    for (i = 0, key = 0; i < 4; i++, key += 12) {
        WifiSetupKeyboard_SetRowY(page, i, gWifiSetupKeyboardKeyPositions[key].y);
    }
    if (page == 2) {
        altPage = TRUE;
    } else if (page == 1) {
        shift = TRUE;
    }
    WifiSetupKeyboard_SetKeyHighlight(WIFI_SETUP_KEY_ALT_PAGE, altPage);
    WifiSetupKeyboard_SetKeyHighlight(WIFI_SETUP_KEY_SHIFT, shift);
}

#pragma opt_strength_reduction reset

void WifiSetupKeyboard_SetKeyHighlight(int key, int highlighted) {
    if (key < 0) {
        return;
    }
    if (key < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS) {
        WifiSetupKeyboard *keyboard = gWifiSetupKeyboard;
        SetKeyPalette(keyboard->keys[key], gWifiSetupKeyboardKeyPalettes[highlighted]);
    } else if (key - WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS < WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS) {
        WifiSetupKeyboard *keyboard = gWifiSetupKeyboard;
        SetKeyPalette(keyboard->extraKeys[key - WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS], gWifiSetupKeyboardKeyPalettes[highlighted]);
    } else {
        WifiSetupKeyboard *keyboard = gWifiSetupKeyboard;
        WifiSetupOamBlock_SetModeAndPalette(keyboard->buttons[key - WIFI_SETUP_KEY_BUTTON_0], -1, 0, gWifiSetupKeyboardButtonPalettes[highlighted]);
    }
}

// Shapes the cursor for the key it is on (wide keys get a wider frame) and moves it there.
void WifiSetupKeyboard_UpdateCursor(void) {
    int templ;
    GXOamAttr *oam;
    switch (gWifiSetupKeyboard->cursorKey) {
    case 47:
        templ = 0x42;
        break;
    case 48:
        templ = 0x41;
        break;
    case 49:
        templ = 0x43;
        break;
    case 50:
        templ = 0x41;
        break;
    case 51:
    case 52:
        templ = 0x45;
        break;
    default:
        templ = 0x40;
        break;
    }
    oam = WifiSetupOamBlock_GetOam(gWifiSetupKeyboard->cursor, 0);
    WifiSetupOamTemplate_Copy(0, templ, oam);
    WifiSetupOamBlock_SetPriority(gWifiSetupKeyboard->cursor, -1, 2);
    WifiSetupOamBlock_SetPosition(gWifiSetupKeyboard->cursor, -1, gWifiSetupKeyboardCursorX[gWifiSetupKeyboard->cursorKey * 2], gWifiSetupKeyboardCursorY[gWifiSetupKeyboard->cursorKey * 2]);
}

// direction: 0 left, 1 up, 2 right, 3 down. Negative entries of the move table pick the target
// from the key the cursor came from, so that moving through a wide key returns to the same column.
void WifiSetupKeyboard_MoveCursor(int direction) {
    int old = gWifiSetupKeyboard->cursorKey;
    gWifiSetupKeyboard->cursorKey = gWifiSetupKeyboardCursorMoves[old][direction];
    if (gWifiSetupKeyboard->cursorKey == 46 && direction == 3) {
        gWifiSetupKeyboard->returnKey = old;
    } else if (gWifiSetupKeyboard->cursorKey == 51 && (direction == 1 || direction == 3)) {
        gWifiSetupKeyboard->returnKey = old;
    } else if (gWifiSetupKeyboard->cursorKey == 52 && (direction == 1 || direction == 3)) {
        if (old != 46) {
            gWifiSetupKeyboard->returnKey = old;
        }
    } else if (gWifiSetupKeyboard->cursorKey == -1) {
        if (gWifiSetupKeyboard->returnKey == 35 || gWifiSetupKeyboard->returnKey == WIFI_SETUP_KEY_DELETE) {
            gWifiSetupKeyboard->cursorKey = 35;
        } else {
            gWifiSetupKeyboard->cursorKey = 34;
        }
    } else if (gWifiSetupKeyboard->cursorKey == -2) {
        switch (gWifiSetupKeyboard->returnKey) {
        case 0:
        case 49:
            gWifiSetupKeyboard->cursorKey = 49;
            break;
        case 1:
        case 36:
            gWifiSetupKeyboard->cursorKey = 36;
            break;
        case 3:
        case 38:
            gWifiSetupKeyboard->cursorKey = 38;
            break;
        case 4:
        case 39:
            gWifiSetupKeyboard->cursorKey = 39;
            break;
        case 5:
        case 40:
            gWifiSetupKeyboard->cursorKey = 40;
            break;
        default:
            gWifiSetupKeyboard->cursorKey = 37;
            break;
        }
    } else if (gWifiSetupKeyboard->cursorKey == -3) {
        switch (gWifiSetupKeyboard->returnKey) {
        case 6:
        case 41:
            gWifiSetupKeyboard->cursorKey = 41;
            break;
        case 7:
        case 42:
            gWifiSetupKeyboard->cursorKey = 42;
            break;
        case 9:
        case 44:
            gWifiSetupKeyboard->cursorKey = 44;
            break;
        case 10:
        case 45:
            gWifiSetupKeyboard->cursorKey = 45;
            break;
        case 11:
        case 34:
        case 35:
        case WIFI_SETUP_KEY_DELETE:
            gWifiSetupKeyboard->cursorKey = 46;
            break;
        default:
            gWifiSetupKeyboard->cursorKey = 43;
            break;
        }
    } else if (gWifiSetupKeyboard->cursorKey == -4) {
        switch (gWifiSetupKeyboard->returnKey) {
        case 0:
        case 49:
            gWifiSetupKeyboard->cursorKey = 0;
            break;
        case 1:
        case 36:
            gWifiSetupKeyboard->cursorKey = 1;
            break;
        case 3:
        case 38:
            gWifiSetupKeyboard->cursorKey = 3;
            break;
        case 4:
        case 39:
            gWifiSetupKeyboard->cursorKey = 4;
            break;
        case 5:
        case 40:
            gWifiSetupKeyboard->cursorKey = 5;
            break;
        default:
            gWifiSetupKeyboard->cursorKey = 2;
            break;
        }
    } else if (gWifiSetupKeyboard->cursorKey == -5) {
        switch (gWifiSetupKeyboard->returnKey) {
        case 6:
        case 41:
            gWifiSetupKeyboard->cursorKey = 6;
            break;
        case 7:
        case 42:
            gWifiSetupKeyboard->cursorKey = 7;
            break;
        case 9:
        case 44:
            gWifiSetupKeyboard->cursorKey = 9;
            break;
        case 10:
        case 45:
            gWifiSetupKeyboard->cursorKey = 10;
            break;
        case 11:
        case 34:
            gWifiSetupKeyboard->cursorKey = 11;
            break;
        case 35:
        case WIFI_SETUP_KEY_DELETE:
            gWifiSetupKeyboard->cursorKey = 50;
            break;
        default:
            gWifiSetupKeyboard->cursorKey = 8;
            break;
        }
    }
    WifiSetupKeyboard_UpdateCursor();
    WifiSetupSound_Play(8);
}

// Slide-out: each task moves one row down by 12 pixels per frame until it is off screen, then
// starts the row above it. The row's x position is passed to WifiSetupKeyboard_SetRowY as an
// extra, unused argument.

void WifiSetupKeyboard_SlideOutButtonsTask(void *task) {
    RowPosition pos;
    u32 word = WifiSetupOamBlock_GetOam(gWifiSetupKeyboard->buttons[0], 0)->attr01;
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 4, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) { // pos.y, read through memory
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideOutRow3Task);
}

void WifiSetupKeyboard_SlideOutRow3Task(void *task) {
    RowPosition pos;
    u32 word = gWifiSetupKeyboard->keys[36]->attr01;
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 3, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) { // pos.y, read through memory
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideOutRow2Task);
}

void WifiSetupKeyboard_SlideOutRow2Task(void *task) {
    RowPosition pos;
    u32 word = gWifiSetupKeyboard->keys[24]->attr01;
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 2, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) { // pos.y, read through memory
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideOutRow1Task);
}

void WifiSetupKeyboard_SlideOutRow1Task(void *task) {
    RowPosition pos;
    u32 word = gWifiSetupKeyboard->keys[12]->attr01;
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 1, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) { // pos.y, read through memory
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideOutRow0Task);
}

void WifiSetupKeyboard_SlideOutRow0Task(void *task) {
    RowPosition pos;
    u32 word = gWifiSetupKeyboard->keys[0]->attr01;
    pos.x = (word & 0x1ff0000) >> 16;
    pos.y = (word & 0xff) + 12;
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 0, pos.y, pos.x);
    if (*(int *)&pos < 0xc0) { // pos.y, read through memory
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_DestroyTask);
}

WifiSetupKeyboard *gWifiSetupKeyboard;

void WifiSetupKeyboard_DestroyTask(void *task) {
    int page, row;
    WifiSetupTask_DestroyLater(0, task);
    for (page = 0; page < 3; page++) {
        for (row = 0; row < 4; row++) {
            WifiSetupText_FreeObjCanvas(gWifiSetupKeyboard->rows[page][row]);
            if (page == 0) {
                WifiSetupOamBlock_Free(gWifiSetupKeyboard->rowBlocks[row]);
            }
        }
    }
    for (row = 0; row < WIFI_SETUP_KEYBOARD_NUM_BUTTONS; row++) {
        WifiSetupOamBlock_Free(gWifiSetupKeyboard->buttons[row]);
    }
    for (row = 0; row < WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS; row++) {
        WifiSetupOam_Release(gWifiSetupKeyboard->extraKeys[row]);
    }
    for (row = 0; row < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS; row++) {
        WifiSetupOam_Release(gWifiSetupKeyboard->keys[row]);
    }
    WifiSetupHeap_FreeAndClear(&gWifiSetupKeyboard);
}
