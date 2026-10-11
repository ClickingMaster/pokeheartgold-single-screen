#pragma force_active on
#include "wifi_setup_keypad.h"

#include "global.h"

#include "wifi_setup_keyboard.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Numeric keypad, first part: creation, the accessors used by the screens, the slide-in animation
// and the per-frame input handling (touch press, release and hold, D-pad and buttons).

typedef struct RowPosition {
    int y;
    int x;
} RowPosition;

void WifiSetupKeypad_Create(void) {
    u16 box[4];
    u32 numObjs;
    u16 label[2];
    int firstLabel;
    int labelIndex;
    int i;
    // x and y of the label box are taken from the row-count table of wifi_setup_keypad_layout.c,
    // which follows the key sizes there: { 0, 1 }
    box[0] = ((const u16 *)&ov13_02245228)[16];
    box[1] = ((const u16 *)&ov13_02245228)[17];
    box[2] = 28;
    box[3] = 20;
    gWifiSetupKeypad = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupKeypad), 4);
    gWifiSetupKeypad->input = 31;
    gWifiSetupKeypad->cursorKey = 0;
    gWifiSetupKeypad->canDelete = 1;
    gWifiSetupKeypad->canType = 1;
    gWifiSetupKeypad->canAdvance = 1;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS; i++) {
        gWifiSetupKeypad->keys[i] = WifiSetupOamTemplate_AllocOam(0, 54);
        gWifiSetupKeypad->keys[i]->attr01 = (gWifiSetupKeypad->keys[i]->attr01 & 0xc1fffcff) | 0x200;
        gWifiSetupKeypad->keys[i]->attr2 = (gWifiSetupKeypad->keys[i]->attr2 & ~0xc00) | 0xc00;
    }
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS; i++) {
        gWifiSetupKeypad->extraKeys[i] = WifiSetupOamTemplate_AllocOam(0, ov13_02245214[i]);
        gWifiSetupKeypad->extraKeys[i]->attr01 = (gWifiSetupKeypad->extraKeys[i]->attr01 & 0xc1fffcff) | 0x200;
        gWifiSetupKeypad->extraKeys[i]->attr2 = (gWifiSetupKeypad->extraKeys[i]->attr2 & ~0xc00) | 0xc00;
    }
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_BUTTONS; i++) {
        gWifiSetupKeypad->buttons[i] = WifiSetupOamTemplate_AllocBlock(0, ov13_02245218[i], 1);
        WifiSetupOamBlock_SetAffineMode(gWifiSetupKeypad->buttons[i], -1, 512, 0);
        WifiSetupOamBlock_SetPriority(gWifiSetupKeypad->buttons[i], -1, 3);
    }
    i = 0;
    firstLabel = i;
    label[1] = (u16)i;
    for (; i < 4; i++) {
        int j;
        gWifiSetupKeypad->rows[i] = WifiSetupText_AllocObjCanvas(0, 12, 4, 0, &numObjs, 0);
        labelIndex = firstLabel;
        box[0] = 0;
        for (j = 0; j < 3; j++) {
            label[0] = ov13_02245260[labelIndex];
            WifiSetupText_DrawInRect(gWifiSetupKeypad->rows[i], *(volatile u16 *)&box[0], *(volatile u16 *)&box[1], *(volatile u16 *)&box[2], *(volatile u16 *)&box[3], 2, 1152, label);
            box[0] += 32;
            labelIndex++;
        }
        gWifiSetupKeypad->rowBlocks[i] = WifiSetupOamBlock_Alloc(0, numObjs, 0);
        firstLabel += 3;
    }
    gWifiSetupKeypad->cursor = WifiSetupOamTemplate_AllocBlock(0, 68, 1);
    WifiSetupOamBlock_SetAffineMode(gWifiSetupKeypad->cursor, -1, 512, 0);
    WifiSetupOamBlock_SetPriority(gWifiSetupKeypad->cursor, -1, 2);
    gWifiSetupKeypad->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupKeypad_SlideInRow0Task, NULL, 120);
    WifiSetupKeypad_SetRowY(0, 192);
}

void WifiSetupKeypad_Close(void) {
    WifiSetupOamBlock_Free(gWifiSetupKeypad->cursor);
    WifiSetupTask_SetFunc(gWifiSetupKeypad->task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideOutButtonsTask);
}

int WifiSetupKeypad_GetInput(void) {
    return gWifiSetupKeypad->input;
}

void WifiSetupKeypad_SetCanDelete(int canDelete) {
    gWifiSetupKeypad->canDelete = canDelete;
}

void WifiSetupKeypad_SetCanType(int canType) {
    gWifiSetupKeypad->canType = canType;
}

void WifiSetupKeypad_SetCanAdvance(int canAdvance) {
    gWifiSetupKeypad->canAdvance = canAdvance;
}

BOOL WifiSetupKeypad_Exists(void) {
    return gWifiSetupKeypad != NULL;
}

// Each slide-in task raises one row by 12 pixels per frame until it reaches its place, then starts
// the next row from the bottom of the screen. The row's x position is read but unused.

#pragma opt_propagation off

void WifiSetupKeypad_SlideInRow0Task(void *task) {
    volatile RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[0]->attr01;
    int x;
    int y;
    u32 xBits = attr & 0x1ff0000;
    y = attr & 0xff;
    y -= 12;
    x = xBits >> 16;
    pos.x = x;
    pos.y = y;
    if (y > 76) {
        WifiSetupKeypad_SetRowY(0, y);
        return;
    }
    WifiSetupKeypad_SetRowY(0, 76);
    WifiSetupKeypad_SetRowY(1, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideInRow1Task);
}

void WifiSetupKeypad_SlideInRow1Task(void *task) {
    volatile RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[3]->attr01;
    int x;
    int y;
    u32 xBits = attr & 0x1ff0000;
    y = attr & 0xff;
    y -= 12;
    x = xBits >> 16;
    pos.x = x;
    pos.y = y;
    if (y > 99) {
        WifiSetupKeypad_SetRowY(1, y);
        return;
    }
    WifiSetupKeypad_SetRowY(1, 99);
    WifiSetupKeypad_SetRowY(2, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideInRow2Task);
}

void WifiSetupKeypad_SlideInRow2Task(void *task) {
    volatile RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[6]->attr01;
    int x;
    int y;
    u32 xBits = attr & 0x1ff0000;
    y = attr & 0xff;
    y -= 12;
    x = xBits >> 16;
    pos.x = x;
    pos.y = y;
    if (y > 122) {
        WifiSetupKeypad_SetRowY(2, y);
        return;
    }
    WifiSetupKeypad_SetRowY(2, 122);
    WifiSetupKeypad_SetRowY(3, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideInRow3Task);
}

void WifiSetupKeypad_SlideInRow3Task(void *task) {
    volatile RowPosition pos;
    u32 attr = gWifiSetupKeypad->keys[9]->attr01;
    int x;
    int y;
    u32 xBits = attr & 0x1ff0000;
    y = attr & 0xff;
    y -= 12;
    x = xBits >> 16;
    pos.x = x;
    pos.y = y;
    if (y > 145) {
        WifiSetupKeypad_SetRowY(3, y);
        return;
    }
    WifiSetupKeypad_SetRowY(3, 145);
    WifiSetupKeypad_SetRowY(4, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_SlideInButtonsTask);
}

#pragma opt_propagation reset

void WifiSetupKeypad_SlideInButtonsTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupKeypad->buttons[0], 0, &x, &y);
    y -= 12;
    if (y > 170) {
        WifiSetupKeypad_SetRowY(4, y);
        return;
    }
    WifiSetupKeypad_SetRowY(4, 170);
    WifiSetupKeypad_UpdateCursor();
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeypad_InputTask);
}

void WifiSetupKeypad_InputTask(void *task) {
    WifiSetupKeypad_HandleTouchPress();
    WifiSetupKeypad_HandleTouchRelease();
    WifiSetupKeypad_HandleTouchHold();
    WifiSetupKeypad_HandleButtons();
}

void WifiSetupKeypad_HandleTouchPress(void) {
    WifiSetupRect rect;
    int i;
    const WifiSetupPoint *pos;
    if (!WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        return;
    }
    gWifiSetupKeypad->pressedKey = -1;
    pos = ov13_0224527A;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245228, &rect);
        if (WifiSetupInput_IsTouchPressed(&rect)) {
            if (!gWifiSetupKeypad->canType) {
                WifiSetupSound_Play(9);
                return;
            }
            WifiSetupSound_Play(0);
            gWifiSetupKeypad->pressedKey = i;
            return;
        }
    }
    pos = ov13_02245250;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245230, &rect);
        if (WifiSetupInput_IsTouchPressed(&rect)) {
            if ((i == 0 && !gWifiSetupKeypad->canDelete) || (i == 1 && !gWifiSetupKeypad->canAdvance)) {
                WifiSetupSound_Play(9);
                return;
            }
            WifiSetupSound_Play(0);
            gWifiSetupKeypad->pressedKey = i + WIFI_SETUP_KEYPAD_KEY_DELETE;
            return;
        }
    }
    pos = ov13_02245258;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_BUTTONS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245234, &rect);
        if (WifiSetupInput_IsTouchPressed(&rect)) {
            WifiSetupSound_Play(0);
            gWifiSetupKeypad->pressedKey = i + WIFI_SETUP_KEYPAD_KEY_BUTTON_0;
            return;
        }
    }
}

// A key is typed when the stylus is lifted on the key it went down on.
void WifiSetupKeypad_HandleTouchRelease(void) {
    WifiSetupRect rect;
    int i;
    const WifiSetupPoint *pos;
    gWifiSetupKeypad->input = WIFI_SETUP_KEYPAD_INPUT_NONE;
    if (!WifiSetupInput_IsTouchReleased(&ov13_02245958)) {
        return;
    }
    pos = ov13_0224527A;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245228, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i) {
                return;
            }
            gWifiSetupKeypad->input = ov13_02245314[i];
            gWifiSetupKeypad->cursorKey = i;
            WifiSetupKeypad_UpdateCursor();
            return;
        }
    }
    pos = ov13_02245250;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245230, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i + WIFI_SETUP_KEYPAD_KEY_DELETE) {
                return;
            }
            gWifiSetupKeypad->input = ov13_02245224[i];
            gWifiSetupKeypad->cursorKey = i + WIFI_SETUP_KEYPAD_KEY_DELETE;
            WifiSetupKeypad_UpdateCursor();
            return;
        }
    }
    pos = ov13_02245258;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_BUTTONS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245234, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i + WIFI_SETUP_KEYPAD_KEY_BUTTON_0) {
                return;
            }
            gWifiSetupKeypad->input = ov13_02245220[i];
            gWifiSetupKeypad->cursorKey = i + WIFI_SETUP_KEYPAD_KEY_BUTTON_0;
            WifiSetupKeypad_UpdateCursor();
            return;
        }
    }
}

// Highlights the key under the stylus while it stays on the key it went down on. Holding the
// delete key repeats it: after 40 frames, then every 7 frames.
void WifiSetupKeypad_HandleTouchHold(void) {
    WifiSetupRect rect;
    int i;
    const WifiSetupPoint *pos;
    if (!WifiSetupInput_IsTouchHeld(&ov13_02245958)) {
        goto cancel;
    }
    pos = ov13_0224527A;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245228, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i) {
                goto cancel;
            }
            WifiSetupKeypad_SetHoveredKey(i);
            goto done;
        }
    }
    pos = ov13_02245250;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245230, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i + WIFI_SETUP_KEYPAD_KEY_DELETE) {
                goto cancel;
            }
            WifiSetupKeypad_SetHoveredKey(i + WIFI_SETUP_KEYPAD_KEY_DELETE);
            if (i != 0) {
                goto done;
            }
            gWifiSetupKeypad->repeatTimer++;
            if (gWifiSetupKeypad->repeatTimer < 40) {
                return;
            }
            if (gWifiSetupKeypad->canDelete == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupKeypad->pressedKey = -1;
                return;
            }
            gWifiSetupKeypad->input = WIFI_SETUP_KEYPAD_INPUT_DELETE;
            gWifiSetupKeypad->repeatTimer -= 7;
            return;
        }
    }
    pos = ov13_02245258;
    for (i = 0; i < WIFI_SETUP_KEYPAD_NUM_BUTTONS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02245234, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeypad->pressedKey != i + WIFI_SETUP_KEYPAD_KEY_BUTTON_0) {
                goto cancel;
            }
            WifiSetupKeypad_SetHoveredKey(i + WIFI_SETUP_KEYPAD_KEY_BUTTON_0);
            goto done;
        }
    }
cancel:
    WifiSetupKeypad_SetHoveredKey(-1);
done:
    gWifiSetupKeypad->repeatTimer = 0;
}

void WifiSetupKeypad_SetHoveredKey(int key) {
    if (key == gWifiSetupKeypad->hoveredKey) {
        return;
    }
    WifiSetupKeypad_SetKeyHighlight(key, TRUE);
    WifiSetupKeypad_SetKeyHighlight(gWifiSetupKeypad->hoveredKey, FALSE);
    gWifiSetupKeypad->hoveredKey = key;
}

// D-pad moves the cursor, A types the key under it, B deletes.
void WifiSetupKeypad_HandleButtons(void) {
    WifiSetupKeypad *keypad;
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT)) {
        WifiSetupKeypad_MoveCursor(0);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupKeypad_MoveCursor(1);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT)) {
        WifiSetupKeypad_MoveCursor(2);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupKeypad_MoveCursor(3);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        keypad = gWifiSetupKeypad;
        int key = keypad->cursorKey;
        if (key < WIFI_SETUP_KEYPAD_NUM_DIGIT_KEYS) {
            if (keypad->canType) {
                keypad->input = ov13_02245314[key];
            } else {
                WifiSetupSound_Play(9);
            }
            return;
        } else if (key - WIFI_SETUP_KEYPAD_KEY_DELETE < WIFI_SETUP_KEYPAD_NUM_EXTRA_KEYS) {
            if ((key - WIFI_SETUP_KEYPAD_KEY_DELETE == 0 && keypad->canDelete == 0) || (key - WIFI_SETUP_KEYPAD_KEY_DELETE == 1 && keypad->canAdvance == 0)) {
                WifiSetupSound_Play(9);
                return;
            }
            keypad->input = ov13_02245224[key - WIFI_SETUP_KEYPAD_KEY_DELETE];
            return;
        } else {
            keypad->input = ov13_02245220[key - WIFI_SETUP_KEYPAD_KEY_BUTTON_0];
        }
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_B)) {
        keypad = gWifiSetupKeypad;
        if (keypad->canDelete == 0) {
            if (keypad->deleteErrorPlayed) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupKeypad->deleteErrorPlayed = TRUE;
            return;
        }
        keypad->input = WIFI_SETUP_KEYPAD_INPUT_DELETE;
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_B)) {
        gWifiSetupKeypad->deleteErrorPlayed = FALSE;
    }
}
