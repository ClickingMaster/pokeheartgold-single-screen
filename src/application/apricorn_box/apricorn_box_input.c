#include "global.h"

#include "constants/sndseq.h"

#include "application/apricorn_box/apricorn_box_screen_internal.h"

#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

static int ApricornBoxScreen_ViewSelect(ApricornBoxScreen *screen, u32 input);
static int ApricornBoxScreen_ViewKeys(ApricornBoxScreen *screen);
static int ApricornBoxScreen_ViewTouch(ApricornBoxScreen *screen, int *hit);
static int ApricornBoxScreen_BlenderSelect(ApricornBoxScreen *screen, u32 input);
static int ApricornBoxScreen_BlenderKeys(ApricornBoxScreen *screen);
static int ApricornBoxScreen_BlenderTouch(ApricornBoxScreen *screen, int *hit);
static int ApricornBoxScreen_KurtSelect(ApricornBoxScreen *screen, u32 input);
static int ApricornBoxScreen_KurtKeys(ApricornBoxScreen *screen);
static int ApricornBoxScreen_KurtTouch(ApricornBoxScreen *screen, int *hit);
static int ApricornBoxScreen_QuantitySelect(ApricornBoxScreen *screen, int input);
static int ApricornBoxScreen_StepQuantity(int value, int max, int delta);
static int ApricornBoxScreen_QuantityKeys(ApricornBoxScreen *screen);
static int ApricornBoxScreen_QuantityTouch(ApricornBoxScreen *screen, int *hit);
static BOOL ApricornBoxScreen_MoveCursor(ApricornBoxScreen *screen);
static void ApricornBoxScreen_SwitchColumn(ApricornBoxScreen *screen, int direction);

static const TouchscreenHitbox sApricornBoxQuantityHitboxes[] = {
    { .rect = { 104, 120, 136, 184 } },
    { .rect = { 128, 144, 136, 184 } },
    { .rect = { 88, 104, 94, 108 } },
    { .rect = { 128, 144, 94, 108 } },
    { .rect = { 88, 104, 74, 88 } },
    { .rect = { 128, 144, 74, 88 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sApricornBoxHitboxes[] = {
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 26, 48, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 74, 64, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 26, 80, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 74, 96, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 26, 112, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 74, 128, 24 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 26, 144, 24 } },
    { .rect = { 72, 160, 152, 232 } },
    { .rect = { 160, 192, 192, 0 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

u8 ApricornBoxScreen_GetCursorTarget(ApricornBoxScreen *screen) {
    switch (screen->cursorArea) {
    case 0:
        return screen->cursorRow * 2;
    case 1:
        return screen->cursorRow * 2 + 1;
    case 2:
        if (screen->mode == APRICORN_BOX_MODE_BLENDER) {
            return 7;
        }
        return 8;
    case 3:
        return 8;
    }
    return 0;
}

int ApricornBoxScreen_HandleViewInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_ViewTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_ViewKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleBlenderInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_BlenderTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_BlenderKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleDrag(ApricornBoxScreen *screen) {
    s16 x;
    s16 y;
    u8 hit;
    Sprite_GetPositionXY(screen->sprites[screen->selectedApricorn + 2], &x, &y);
    hit = TouchscreenHitbox_PointIsIn(&sApricornBoxHitboxes[7], x, y);
    if (System_GetTouchHeld()) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        Sprite_SetPositionXY(screen->sprites[screen->selectedApricorn + 2], x, y);
        Sprite_SetPositionXY(screen->sprites[1], x, y - 6);
        if (hit) {
            if (screen->overBlender == 0) {
                screen->overBlender = 1;
                PlaySE(SEQ_SE_GS_BON01);
                Sprite_SetAnimCtrlSeq(screen->sprites[10], 21);
            }
        } else if (screen->overBlender != 0) {
            screen->overBlender = 0;
            PlaySE(SEQ_SE_GS_BON01);
            Sprite_SetAnimCtrlSeq(screen->sprites[10], 20);
        }
        return -1;
    }
    screen->overBlender = 0;
    if (hit) {
        PlaySE(SEQ_SE_DP_BOX01);
        return 1;
    }
    PlaySE(SEQ_SE_DP_HYUN2);
    return 0;
}

int ApricornBoxScreen_HandleKurtInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_KurtTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_KurtKeys(screen);
    }
    return result;
}

int ApricornBoxScreen_HandleQuantityInput(ApricornBoxScreen *screen) {
    int hit = 0;
    int result = ApricornBoxScreen_QuantityTouch(screen, &hit);
    if (hit == 0) {
        result = ApricornBoxScreen_QuantityKeys(screen);
    }
    return result;
}

static int ApricornBoxScreen_ViewSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        screen->selectedApricorn = input;
        return 3;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    return 3;
}

static int ApricornBoxScreen_ViewKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_ViewSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_ViewSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 3;
    }
    return 3;
}

static int ApricornBoxScreen_ViewTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    if (input == -1 || input == 7) {
        return 3;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    screen->lastRowRight = 0;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = 0;
    if (input == 8) {
        screen->cursorArea = 2;
    } else {
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
    }
    return ApricornBoxScreen_ViewSelect(screen, input);
}

static int ApricornBoxScreen_BlenderSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        ApricornBoxScreen_UpdateCursor(screen);
        if (screen->apricornCounts[input] == 0) {
            return 4;
        }
        PlaySE(SEQ_SE_DP_BOX02);
        screen->selectedApricorn = input;
        return screen->menuInputState == MENU_INPUT_STATE_BUTTONS ? 7 : 8;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    if (input == 7) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        screen->selectedApricorn = input;
        if (screen->juiceServings == 0 && screen->numBlending == 0) {
            ApricornBoxScreen_PrintMessage(screen, 25, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        if (screen->juiceServings == 0 && screen->numBlending != 0) {
            ApricornBoxScreen_PrintMessage(screen, 27, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        if (screen->juiceServings != 0 && screen->numBlending != 0) {
            ApricornBoxScreen_PrintMessage(screen, 28, TEXT_SPEED_NOTRANSFER);
            ApricornBoxScreen_SetApricornsFocus(screen, 1);
            return 15;
        }
        return 11;
    }
    return 4;
}

static int ApricornBoxScreen_BlenderKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_BlenderSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_BlenderSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 4;
    }
    return 4;
}

static int ApricornBoxScreen_BlenderTouch(ApricornBoxScreen *screen, int *hit) {
    int input;
    int value;
    input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    value = 0;
    if (input == ~value) {
        return 4;
    }
    value = 1;
    *hit = value;
    screen->menuInputState = (MenuInputState)value;
    value = 0;
    screen->lastRowRight = value;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = value;
    switch (input) {
    case 7:
        screen->cursorArea = 2;
        break;
    case 8:
        screen->cursorArea = 3;
        break;
    default:
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
        break;
    }
    return ApricornBoxScreen_BlenderSelect(screen, input);
}

static int ApricornBoxScreen_KurtSelect(ApricornBoxScreen *screen, u32 input) {
    if (input <= 6) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        if (screen->apricornCounts[input] == 0) {
            return 5;
        }
        screen->selectedApricorn = input;
        screen->quantity = 1;
        screen->maxQuantity = screen->apricornCounts[screen->selectedApricorn];
        return 6;
    }
    if (input == 8) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawCloseButton(screen, 1);
        return 1;
    }
    return 5;
}

static int ApricornBoxScreen_KurtKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_KurtSelect(screen, 8);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_KurtSelect(screen, ApricornBoxScreen_GetCursorTarget(screen));
    }
    if (ApricornBoxScreen_MoveCursor(screen)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_UpdateCursor(screen);
        return 5;
    }
    return 5;
}

static int ApricornBoxScreen_KurtTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxHitboxes);
    if (input == -1 || input == 7) {
        return 5;
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    screen->lastRowRight = 0;
    screen->lastRowLeft = screen->lastRowRight;
    screen->cursorRow = 0;
    if (input == 8) {
        screen->cursorArea = 2;
    } else {
        screen->cursorArea = input % 2;
        screen->cursorRow = input / 2;
    }
    return ApricornBoxScreen_KurtSelect(screen, input);
}

static int ApricornBoxScreen_QuantitySelect(ApricornBoxScreen *screen, int input) {
    if (input == 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawConfirmButton(screen, 0, 1);
        return 0;
    }
    if (input == 1) {
        PlaySE(SEQ_SE_DP_SELECT);
        ApricornBoxScreen_DrawConfirmButton(screen, 1, 1);
        return 1;
    }
    switch (input) {
    case 2:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, 1);
        break;
    case 4:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, 10);
        break;
    case 3:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, -1);
        break;
    case 5:
        screen->quantity = ApricornBoxScreen_StepQuantity(screen->quantity, screen->maxQuantity, -10);
        break;
    default:
        return 6;
    }
    PlaySE(SEQ_SE_DP_SELECT);
    ApricornBoxScreen_PrintQuantity(screen, screen->quantity);
    return 6;
}

static int ApricornBoxScreen_StepQuantity(int value, int max, int delta) {
    if (delta > 0) {
        if (value == max) {
            return 1;
        }
        value += delta;
        if (value > max) {
            return max;
        }
    } else {
        if (value == 1) {
            return max;
        }
        value += delta;
        if (value <= 0) {
            return 1;
        }
    }
    return value;
}

static int ApricornBoxScreen_QuantityKeys(ApricornBoxScreen *screen) {
    u32 keys = gSystem.newKeys;
    int down;
    if (keys & 0xCF3) {
        screen->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_B) {
        return ApricornBoxScreen_QuantitySelect(screen, 1);
    }
    if (keys & PAD_BUTTON_A) {
        return ApricornBoxScreen_QuantitySelect(screen, 0);
    }
    if (keys & PAD_KEY_UP) {
        return ApricornBoxScreen_QuantitySelect(screen, 2);
    }
    down = keys & PAD_KEY_DOWN;
    if (down) {
        return ApricornBoxScreen_QuantitySelect(screen, 3);
    }
    if (keys & PAD_KEY_RIGHT) {
        return ApricornBoxScreen_QuantitySelect(screen, 4);
    }
    if (keys & PAD_KEY_LEFT) {
        return ApricornBoxScreen_QuantitySelect(screen, 5);
    }
    if (down != 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        return 6;
    }
    return 6;
}

static int ApricornBoxScreen_QuantityTouch(ApricornBoxScreen *screen, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(sApricornBoxQuantityHitboxes);
    if (input == -1) {
        return 6;
    }
    if ((u32)(input - 4) <= 1 && screen->maxQuantity < 10) {
        return 6;
    }
    if (input >= 2) {
        Sprite **sprites = screen->sprites;
        u32 offset = (input + 9) * sizeof(Sprite *);
        Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)sprites + offset), TRUE);
        Sprite_ResetAnimCtrlState(*(Sprite **)((u8 *)sprites + offset));
    }
    *hit = 1;
    screen->menuInputState = MENU_INPUT_STATE_TOUCH;
    return ApricornBoxScreen_QuantitySelect(screen, input);
}

static BOOL ApricornBoxScreen_MoveCursor(ApricornBoxScreen *screen) {
    u32 keys;
    switch (screen->cursorArea) {
    case 0:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_RIGHT) {
            ApricornBoxScreen_SwitchColumn(screen, 0);
            return TRUE;
        }
        if ((keys & PAD_KEY_UP) && screen->cursorRow != 0) {
            screen->lastRowRight = 0;
            screen->cursorRow--;
            return TRUE;
        }
        if ((keys & PAD_KEY_DOWN) && screen->cursorRow < 3) {
            screen->lastRowRight = 0;
            screen->cursorRow++;
            return TRUE;
        }
        break;
    case 1:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_LEFT) {
            ApricornBoxScreen_SwitchColumn(screen, 1);
            return TRUE;
        }
        if (keys & PAD_KEY_RIGHT) {
            screen->lastRowRight = screen->cursorRow;
            screen->cursorRow = 0;
            screen->cursorArea++;
            return TRUE;
        }
        if ((keys & PAD_KEY_UP) && screen->cursorRow != 0) {
            screen->lastRowLeft = 0;
            screen->cursorRow--;
            return TRUE;
        }
        if ((keys & PAD_KEY_DOWN) && screen->cursorRow < 2) {
            screen->lastRowLeft = 0;
            screen->cursorRow++;
            return TRUE;
        }
        break;
    case 2:
        keys = gSystem.newKeys;
        if (keys & PAD_KEY_LEFT) {
            screen->cursorRow = screen->lastRowRight;
            screen->cursorArea--;
            return TRUE;
        }
        if (screen->mode == APRICORN_BOX_MODE_KURT) {
            return FALSE;
        }
        if ((keys & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) && screen->mode != APRICORN_BOX_MODE_VIEW) {
            screen->cursorArea++;
            return TRUE;
        }
        break;
    case 3:
        keys = gSystem.newKeys;
        if (keys & (PAD_KEY_LEFT | PAD_KEY_UP)) {
            screen->cursorArea--;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void ApricornBoxScreen_SwitchColumn(ApricornBoxScreen *screen, int direction) {
    u8 row;
    if (direction == 0) {
        screen->lastRowLeft = screen->cursorRow;
        screen->cursorArea = 1;
        row = screen->cursorRow;
        switch (row) {
        case 1:
        case 2:
            if (row != screen->lastRowRight && row - 1 != screen->lastRowRight) {
                screen->cursorRow = 1;
            } else {
                screen->cursorRow = screen->lastRowRight;
            }
            break;
        case 3:
            screen->cursorRow = 2;
            break;
        case 0:
        default:
            screen->cursorRow = 0;
            break;
        }
    } else {
        screen->lastRowRight = screen->cursorRow;
        screen->cursorArea = 0;
        switch (screen->cursorRow) {
        case 0:
            row = screen->lastRowLeft;
            if (row > 1) {
                screen->cursorRow = 1;
            } else {
                screen->cursorRow = row;
            }
            break;
        case 1:
            row = screen->lastRowLeft;
            if (row == 0) {
                screen->cursorRow = 1;
            } else if (row == 3) {
                screen->cursorRow = 2;
            } else {
                screen->cursorRow = row;
            }
            break;
        case 2:
            row = screen->lastRowLeft;
            if (row < 2) {
                screen->cursorRow = 2;
            } else {
                screen->cursorRow = row;
            }
            break;
        default:
            screen->cursorRow = 0;
            break;
        }
    }
}
