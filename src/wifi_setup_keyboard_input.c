#pragma force_active on
#include "global.h"

#include "wifi_setup_keyboard.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Software keyboard, second part: end of the slide-in animation and the per-frame input handling
// (touch press, release and hold, D-pad and buttons).

#pragma opt_propagation off

void WifiSetupKeyboard_SlideInRow3Task(void *task) {
    u32 word = gWifiSetupKeyboard->keys[36]->attr01;
    u32 flag = word & 0x1ff0000;
    int size;

    volatile struct {
        int size;
        u32 flag;
    } info;

    word &= 255;
    size = word - 12;
    word = flag >> 16;
    info.flag = word;
    info.size = size;
    if (size > 147) {
        WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 3, size);
        return;
    }
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 3, 147);
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 4, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideInButtonsTask);
}

#pragma opt_propagation reset

void WifiSetupKeyboard_SlideInButtonsTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupKeyboard->buttons[0], 0, &x, &y);
    y -= 12;
    if (y > 170) {
        WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 4, y);
        return;
    }
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 4, 170);
    WifiSetupKeyboard_UpdateCursor();
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_InputTask);
}

void WifiSetupKeyboard_InputTask(void) {
    WifiSetupKeyboard_HandleTouchPress();
    WifiSetupKeyboard_HandleTouchRelease();
    WifiSetupKeyboard_HandleTouchHold();
    WifiSetupKeyboard_HandleButtons();
}

void WifiSetupKeyboard_HandleTouchPress(void) {
    WifiSetupRect rect;
    int i;
    if (!WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        return;
    }
    gWifiSetupKeyboard->pressedKey = -1;
    for (i = 0; i < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS; i++) {
        WifiSetup_RectFromPosSize(ov13_02244FA4 + i, &ov13_02244F68, &rect);
        if (WifiSetupInput_IsTouchPressed(&rect)) {
            if (!gWifiSetupKeyboard->canType) {
                WifiSetupSound_Play(9);
                return;
            }
            WifiSetupSound_Play(0);
            gWifiSetupKeyboard->pressedKey = i;
            return;
        }
    }
    for (i = 0; i < WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS; i++) {
        WifiSetup_RectFromPosSize(ov13_02244F84 + i, ov13_02244F94 + i, &rect);
        if (WifiSetupInput_IsTouchPressed(&rect)) {
            if ((i == 3 && !gWifiSetupKeyboard->canDelete) || (i == 2 && !gWifiSetupKeyboard->canType)) {
                WifiSetupSound_Play(9);
                return;
            }
            WifiSetupSound_Play(ov13_02245D80[i]);
            gWifiSetupKeyboard->pressedKey = i + WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS;
            return;
        }
    }
    {
        const WifiSetupPoint *size, *pos;
        int j;
        pos = ov13_02244F7C;
        size = &ov13_02244F64;
        for (j = 0; j < WIFI_SETUP_KEYBOARD_NUM_BUTTONS; j++, pos++) {
            WifiSetup_RectFromPosSize(pos, size, &rect);
            if (WifiSetupInput_IsTouchPressed(&rect)) {
                WifiSetupSound_Play(0);
                gWifiSetupKeyboard->pressedKey = j + WIFI_SETUP_KEY_BUTTON_0;
                return;
            }
        }
    }
}

void WifiSetupKeyboard_HandleTouchRelease(void) {
    WifiSetupRect rect;
    int i;
    const WifiSetupPoint *pos, *size;
    gWifiSetupKeyboard->input = 0;
    if (!WifiSetupInput_IsTouchReleased(&ov13_02245958)) {
        return;
    }
    for (i = 0, pos = ov13_02244FA4; i < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02244F68, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i) {
                return;
            }
            gWifiSetupKeyboard->input = ov13_02245D68[gWifiSetupKeyboard->page][i];
            if (gWifiSetupKeyboard->page == 1) {
                WifiSetupKeyboard_SetPage(0);
            }
            gWifiSetupKeyboard->cursorKey = i;
            WifiSetupKeyboard_UpdateCursor();
            return;
        }
    }
    for (i = 2, size = ov13_02244F9C, pos = ov13_02244F8C; i < 4; i++, size++, pos++) {
        WifiSetup_RectFromPosSize(pos, size, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i + WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS) {
                return;
            }
            gWifiSetupKeyboard->input = ov13_02245208[i];
            if (gWifiSetupKeyboard->page == 1) {
                WifiSetupKeyboard_SetPage(0);
            }
            gWifiSetupKeyboard->cursorKey = i + WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS;
            WifiSetupKeyboard_UpdateCursor();
            return;
        }
    }
    for (i = 0, pos = ov13_02244F7C; i < WIFI_SETUP_KEYBOARD_NUM_BUTTONS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02244F64, &rect);
        if (WifiSetupInput_IsTouchReleased(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i + WIFI_SETUP_KEY_BUTTON_0) {
                return;
            }
            gWifiSetupKeyboard->input = ov13_02244F58[i];
            gWifiSetupKeyboard->cursorKey = i + WIFI_SETUP_KEY_BUTTON_0;
            WifiSetupKeyboard_UpdateCursor();
            return;
        }
    }
    WifiSetup_RectFromPosSize(ov13_02244F84, ov13_02244F94, &rect);
    if (WifiSetupInput_IsTouchReleased(&rect)) {
        if (gWifiSetupKeyboard->pressedKey != WIFI_SETUP_KEY_ALT_PAGE) {
            return;
        }
        WifiSetupKeyboard_ToggleAltPage();
        gWifiSetupKeyboard->cursorKey = WIFI_SETUP_KEY_ALT_PAGE;
        WifiSetupKeyboard_UpdateCursor();
        return;
    }
    WifiSetup_RectFromPosSize(ov13_02244F88, ov13_02244F98, &rect);
    if (WifiSetupInput_IsTouchReleased(&rect)) {
        if (gWifiSetupKeyboard->pressedKey != WIFI_SETUP_KEY_SHIFT) {
            return;
        }
        WifiSetupKeyboard_ToggleShift();
        gWifiSetupKeyboard->cursorKey = WIFI_SETUP_KEY_SHIFT;
        WifiSetupKeyboard_UpdateCursor();
    }
}

void WifiSetupKeyboard_ToggleAltPage(void) {
    WifiSetupKeyboard_SetPage(gWifiSetupKeyboard->page == 2 ? 0 : 2);
    WifiSetupSound_Play(1);
}

void WifiSetupKeyboard_ToggleShift(void) {
    WifiSetupKeyboard_SetPage(gWifiSetupKeyboard->page != 1 ? 1 : 0);
    WifiSetupSound_Play(1);
}

// Highlights the key under the stylus while it stays on the key it went down on; holding the
// delete key repeats it after 40 frames, then every 7 frames.
void WifiSetupKeyboard_HandleTouchHold(void) {
    WifiSetupRect rect;
    int i;
    const WifiSetupPoint *pos, *size;
    if (!WifiSetupInput_IsTouchHeld(&ov13_02245958)) {
        goto none;
    }
    pos = ov13_02244FA4;
    for (i = 0; i < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02244F68, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i) {
                goto none;
            }
            WifiSetupKeyboard_SetHoveredKey(i);
            goto reset;
        }
    }
    size = ov13_02244F9C;
    pos = ov13_02244F8C;
    for (i = 2; i < 4; i++, size++, pos++) {
        WifiSetup_RectFromPosSize(pos, size, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i + WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS) {
                goto none;
            }
            WifiSetupKeyboard_SetHoveredKey(i + WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS);
            if (i != 3) {
                return;
            }
            gWifiSetupKeyboard->repeatTimer++;
            if (gWifiSetupKeyboard->repeatTimer < 40) {
                return;
            }
            if (gWifiSetupKeyboard->canDelete == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupKeyboard->pressedKey = -1;
                return;
            }
            gWifiSetupKeyboard->input = WIFI_SETUP_KEYBOARD_INPUT_DELETE;
            gWifiSetupKeyboard->repeatTimer -= 7;
            return;
        }
    }
    pos = ov13_02244F7C;
    for (i = 0; i < WIFI_SETUP_KEYBOARD_NUM_BUTTONS; i++, pos++) {
        WifiSetup_RectFromPosSize(pos, &ov13_02244F64, &rect);
        if (WifiSetupInput_IsTouchHeld(&rect)) {
            if (gWifiSetupKeyboard->pressedKey != i + WIFI_SETUP_KEY_BUTTON_0) {
                goto none;
            }
            WifiSetupKeyboard_SetHoveredKey(i + WIFI_SETUP_KEY_BUTTON_0);
            goto reset;
        }
    }
none:
    WifiSetupKeyboard_SetHoveredKey(-1);
reset:
    gWifiSetupKeyboard->repeatTimer = 0;
}

void WifiSetupKeyboard_SetHoveredKey(int key) {
    if (key == gWifiSetupKeyboard->hoveredKey) {
        return;
    }
    WifiSetupKeyboard_SetKeyHighlight(key, 1);
    WifiSetupKeyboard_SetKeyHighlight(gWifiSetupKeyboard->hoveredKey, 0);
    gWifiSetupKeyboard->hoveredKey = key;
}

// D-pad moves the cursor, A types the key under it, B deletes, X and Y switch pages.
void WifiSetupKeyboard_HandleButtons(void) {
    int key;
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT)) {
        WifiSetupKeyboard_MoveCursor(0);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupKeyboard_MoveCursor(1);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT)) {
        WifiSetupKeyboard_MoveCursor(2);
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupKeyboard_MoveCursor(3);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        key = gWifiSetupKeyboard->cursorKey;
        if (key < WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS) {
            if (!gWifiSetupKeyboard->canType) {
                WifiSetupSound_Play(9);
                return;
            }
            gWifiSetupKeyboard->input = ov13_02245D68[gWifiSetupKeyboard->page][key];
            if (gWifiSetupKeyboard->page == 1) {
                WifiSetupKeyboard_SetPage(0);
            }
            return;
        }
        if (key - WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS < WIFI_SETUP_KEYBOARD_NUM_EXTRA_KEYS) {
            switch (key - WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS) {
            case 0:
                WifiSetupKeyboard_ToggleAltPage();
                return;
            case 1:
                WifiSetupKeyboard_ToggleShift();
                return;
            case 2:
                if (!gWifiSetupKeyboard->canType) {
                    WifiSetupSound_Play(9);
                    return;
                }
                break;
            case 3:
                if (!gWifiSetupKeyboard->canDelete) {
                    WifiSetupSound_Play(9);
                    return;
                }
                break;
            default:
                break;
            }
            if (gWifiSetupKeyboard->page == 1) {
                WifiSetupKeyboard_SetPage(0);
            }
            gWifiSetupKeyboard->input = ov13_02245208[key - WIFI_SETUP_KEYBOARD_NUM_CHAR_KEYS];
            return;
        }
        gWifiSetupKeyboard->input = ov13_02244F58[key - WIFI_SETUP_KEY_BUTTON_0];
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_B)) {
        if (!gWifiSetupKeyboard->canDelete) {
            if (gWifiSetupKeyboard->deleteErrorPlayed) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupKeyboard->deleteErrorPlayed = 1;
            return;
        }
        gWifiSetupKeyboard->input = WIFI_SETUP_KEYBOARD_INPUT_DELETE;
    } else if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_B)) {
        gWifiSetupKeyboard->deleteErrorPlayed = 0;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_X)) {
        WifiSetupKeyboard_ToggleAltPage();
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_Y)) {
        WifiSetupKeyboard_ToggleShift();
    }
}
