#include "global.h"

#include "constants/sndseq.h"

#include "code_entry_internal.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

// Keypad cells (sCodeEntryKeyGrid / sCodeEntryKeyRects): digits 0-9 in two rows of five, then BACK and OK.
#define CODE_ENTRY_KEY_BACK 10
#define CODE_ENTRY_KEY_OK   11

enum CodeEntryState {
    CODE_ENTRY_STATE_SETUP,
    CODE_ENTRY_STATE_INPUT,
    CODE_ENTRY_STATE_SWITCH_GROUP,
    CODE_ENTRY_STATE_FADE_OUT,
};

enum CodeEntryAction {
    CODE_ENTRY_ACTION_NONE = 0,
    CODE_ENTRY_ACTION_SELECT_GROUP = 1, // arg: group (1-based)
    CODE_ENTRY_ACTION_MOVE_CURSOR = 2,  // arg: digit index
    CODE_ENTRY_ACTION_BUSY = 0xFF,
};

typedef struct CodeEntryKeyRect {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} CodeEntryKeyRect;

static void CodeEntry_SetState(CodeEntryApp *app, int state);
static BOOL CodeEntry_State_Setup(CodeEntryApp *app);
static BOOL CodeEntry_State_FadeOut(CodeEntryApp *app);
static BOOL CodeEntry_State_Input(CodeEntryApp *app);
static BOOL CodeEntry_State_SwitchGroup(CodeEntryApp *app);
static void CodeEntry_HandleKeys(CodeEntryApp *app);
static void CodeEntry_Confirm(CodeEntryApp *app);
static void CodeEntry_Backspace(CodeEntryApp *app);
static inline void CodeEntry_CopyKeyRects(u16 *dst, const u16 *src);
static void CodeEntry_OnTouch(u32 index, u32 event, void *arg);
static void CodeEntry_RunPendingAction(CodeEntryApp *app);
static void CodeEntry_ClearPendingAction(CodeEntryApp *app);
static int CodeEntry_GetGroupFirstDigit(CodeEntryApp *app, int group);
static int CodeEntry_GetGroupLastDigit(CodeEntryApp *app, int group);

static const CodeEntryStateFunc sCodeEntryStates[] = {
    [CODE_ENTRY_STATE_SETUP] = CodeEntry_State_Setup,
    [CODE_ENTRY_STATE_INPUT] = CodeEntry_State_Input,
    [CODE_ENTRY_STATE_SWITCH_GROUP] = CodeEntry_State_SwitchGroup,
    [CODE_ENTRY_STATE_FADE_OUT] = CodeEntry_State_FadeOut,
};

static const CodeEntryKeyGrid sCodeEntryKeyGrid = {
    .cells = {
              { 0, 1, 2, 3, 4 },
              { 5, 6, 7, 8, 9 },
              { CODE_ENTRY_KEY_BACK, CODE_ENTRY_KEY_BACK, CODE_ENTRY_KEY_BACK, CODE_ENTRY_KEY_OK, CODE_ENTRY_KEY_OK },
              },
};

static const CodeEntryKeyRect sCodeEntryKeyRects[] = {
    { 32,  80,  20, 20 },
    { 80,  80,  20, 20 },
    { 128, 80,  20, 20 },
    { 176, 80,  20, 20 },
    { 224, 80,  20, 20 },
    { 32,  128, 20, 20 },
    { 80,  128, 20, 20 },
    { 128, 128, 20, 20 },
    { 176, 128, 20, 20 },
    { 224, 128, 20, 20 },
    { 64,  176, 60, 12 },
    { 192, 176, 60, 12 },
};

static f32 sCodeEntryShrinkScales[] = { 0.8f, 0.6f, 0.4f, 0.2f, 0.8f, 1.0f, 1.0f };

static f32 sCodeEntryGrowScales[] = { 0.5f, 0.2f, 0.5f, 1.0f, 1.2f, 1.0f, 1.0f };

static void CodeEntry_SetState(CodeEntryApp *app, int state) {
    app->state = state;
    app->unk2C4 = 0;
    app->subState = 0;
    app->frame = 0;
}

static BOOL CodeEntry_State_Setup(CodeEntryApp *app) {
    void *file;
    NNSG2dPaletteData *palette;

    CodeEntry_LoadGraphics(app);
    CodeEntry_MarkSelectedDigits(app);
    CodeEntry_CreateDigitSprites(app);
    CodeEntry_LayoutDigits(app, FALSE);
    CodeEntry_CreateCursorSprites(app);
    CodeEntry_CreateButtonSprites(app);
    CodeEntry_UpdateDigitHitboxes(app);
    CodeEntry_InitFont(app);
    CodeEntry_CreateButtonLabels(app);
    CodeEntry_CreatePromptWindow(app->bgConfig, &app->promptWindow, GF_BG_LYR_SUB_0, 2, 0x15, 0x1B, 2, 0x64, app->args.promptMsg);
    if (app->args.showWirelessIcon) {
        file = WirelessStatusIcon_LoadPaletteFile(HEAP_ID_108);
        NNS_G2dGetUnpackedPaletteData(file, &palette);
        PaletteData_LoadPalette(app->paletteData, palette->pRawData, PLTTBUF_SUB_OBJ, 0xE0, 0x20);
        Heap_Free(file);
    }
    CodeEntry_SetState(app, CODE_ENTRY_STATE_INPUT);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_108);
    return FALSE;
}

static BOOL CodeEntry_State_FadeOut(CodeEntryApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_108);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

static BOOL CodeEntry_State_Input(CodeEntryApp *app) {
    if (app->subState == 0) {
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState++;
        }
    } else {
        CodeEntry_RunPendingAction(app);
        TouchHitboxController_IsTriggered(app->touchController);
        CodeEntry_HandleKeys(app);
    }
    return FALSE;
}

// The digits slide to their new places; those leaving the selected group shrink, those entering it grow.
static BOOL CodeEntry_State_SwitchGroup(CodeEntryApp *app) {
    int i;

    switch (app->subState) {
    case 0:
        CodeEntry_SetCursorVisible(app, 0, FALSE);
        for (i = 0; i < app->digitCount; i++) {
            if (app->digits[i].moveFrames) {
                ManagedSprite_OffsetPositionXY(app->digits[i].sprite, app->digits[i].velocityX, app->digits[i].velocityY);
                app->digits[i].moveFrames--;
                if (i >= app->selectedStart && i < app->selectedEnd) {
                    ManagedSprite_SetAffineScale(app->digits[i].sprite, sCodeEntryGrowScales[app->digits[i].animStep], sCodeEntryGrowScales[app->digits[i].animStep]);
                    app->digits[i].animStep++;
                }
                if (i >= app->previousStart && i < app->previousEnd) {
                    ManagedSprite_SetAffineScale(app->digits[i].sprite, sCodeEntryShrinkScales[app->digits[i].animStep], sCodeEntryShrinkScales[app->digits[i].animStep]);
                    app->digits[i].animStep++;
                }
            }
        }
        for (i = 0; i < app->separatorCount; i++) {
            if (app->separators[i].moveFrames) {
                ManagedSprite_OffsetPositionXY(app->separators[i].sprite, app->separators[i].velocityX, app->separators[i].velocityY);
                app->separators[i].moveFrames--;
            }
        }
        if (!app->digits[0].moveFrames) {
            for (i = app->selectedStart; i < app->selectedEnd; i++) {
                ManagedSprite_SetAnim(app->digits[i].sprite, CodeEntry_GetDigitAnim(app->digits[i].value, app->digits[i].active));
                ManagedSprite_TickFrame(app->digits[i].sprite);
            }
            for (i = app->previousStart; i < app->previousEnd; i++) {
                ManagedSprite_SetAnim(app->digits[i].sprite, CodeEntry_GetDigitAnim(app->digits[i].value, app->digits[i].active));
                ManagedSprite_TickFrame(app->digits[i].sprite);
            }
            app->subState++;
        }
        app->frame++;
        break;
    case 1:
        for (i = app->selectedStart; i < app->selectedEnd; i++) {
            if (app->digits[i].animStep != 6) {
                ManagedSprite_SetAffineScale(app->digits[i].sprite, sCodeEntryGrowScales[app->digits[i].animStep], sCodeEntryGrowScales[app->digits[i].animStep]);
                app->digits[i].animStep++;
            }
        }
        for (i = app->previousStart; i < app->previousEnd; i++) {
            if (app->digits[i].animStep != 6) {
                ManagedSprite_SetAffineScale(app->digits[i].sprite, sCodeEntryShrinkScales[app->digits[i].animStep], sCodeEntryShrinkScales[app->digits[i].animStep]);
                app->digits[i].animStep++;
            }
        }
        app->frame++;
        if (app->frame == 6) {
            app->subState++;
        }
        break;
    default:
        CodeEntry_UpdateDigitHitboxes(app);
        if (!app->pendingToLast) {
            CodeEntry_MoveDigitCursor(app, CodeEntry_GetGroupFirstDigit(app, app->pendingArg));
        } else {
            CodeEntry_MoveDigitCursor(app, CodeEntry_GetGroupLastDigit(app, app->pendingArg));
        }
        if (app->selectedGroup) {
            CodeEntry_SetCursorVisible(app, 0, TRUE);
        }
        CodeEntry_ClearPendingAction(app);
        CodeEntry_SetState(app, CODE_ENTRY_STATE_INPUT);
        break;
    }
    return FALSE;
}

BOOL CodeEntry_RunState(CodeEntryApp *app) {
    BOOL result = sCodeEntryStates[app->state](app);
    CodeEntry_UpdateCursors(app);
    CodeEntry_UpdateButtons(app);
    SpriteSystem_DrawSprites(app->spriteManager);
    return result;
}

static void CodeEntry_HandleKeys(CodeEntryApp *app) {
    CodeEntryKeyGrid grid = sCodeEntryKeyGrid;
    BOOL moved = FALSE;
    s16 column = app->cursors[1].velocityX;
    s16 row = app->cursors[1].velocityY;
    int key = grid.cells[row][column];
    int cursor, group, nextGroup;

    if (app->state != CODE_ENTRY_STATE_INPUT || app->pendingAction == CODE_ENTRY_ACTION_SELECT_GROUP) {
        return;
    }
    if (app->touchActive == TRUE) {
        // the first key press after touch input only brings the key cursor back
        if (gSystem.newKeys == 0 || System_GetTouchHeld()) {
            return;
        }
        app->touchActive = FALSE;
        CodeEntry_MoveKeyCursor(app, key);
        if (key == CODE_ENTRY_KEY_BACK || key == CODE_ENTRY_KEY_OK) {
            if (app->cursors[1].value != 2) {
                app->cursors[1].value = 2;
                return;
            }
        } else {
            if (app->cursors[1].value != 1) {
                app->cursors[1].value = 1;
                return;
            }
        }
        return;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (row > 0) {
            app->cursors[1].velocityY--;
        } else {
            app->cursors[1].velocityY = 2;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        app->cursors[1].velocityY++;
        app->cursors[1].velocityY %= 3;
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (key == CODE_ENTRY_KEY_BACK) {
            app->cursors[1].velocityX = 3;
        } else if (key == CODE_ENTRY_KEY_OK) {
            app->cursors[1].velocityX = 0;
        } else {
            app->cursors[1].velocityX = column + 1;
            app->cursors[1].velocityX %= 5;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (key == CODE_ENTRY_KEY_BACK) {
            app->cursors[1].velocityX = 3;
        } else if (key == CODE_ENTRY_KEY_OK) {
            app->cursors[1].velocityX = 0;
        } else if (column > 0) {
            app->cursors[1].velocityX = column - 1;
        } else {
            app->cursors[1].velocityX = 4;
        }
        moved = TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (key == CODE_ENTRY_KEY_BACK) {
            CodeEntry_Backspace(app);
            PlaySE(SEQ_SE_DP_BUTTON3);
        } else if (key == CODE_ENTRY_KEY_OK) {
            CodeEntry_Confirm(app);
            PlaySE(SEQ_SE_DP_PIRORIRO);
        } else {
            if (app->selectedGroup == 0) {
                return;
            }
            cursor = app->cursors[0].value;
            app->digits[cursor].value = key + 1;
            CodeEntry_SetCursorVisible(app, 1, FALSE);
            CodeEntry_SetCursorVisible(app, 2, TRUE);
            CodeEntry_MoveCursorToKey(app, key, 2);
            ManagedSprite_SetAnim(app->digits[cursor].sprite, CodeEntry_GetDigitAnim(app->digits[cursor].value, app->digits[cursor].active));
            ManagedSprite_SetAnim(app->cursors[2].sprite, 3);
            group = app->digits[cursor].group;
            if (cursor + 1 == app->digitCount) {
                moved = TRUE;
                app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
                app->pendingArg = 0;
                app->cursors[1].velocityX = 3;
                app->cursors[1].velocityY = 2;
            } else {
                nextGroup = app->digits[cursor + 1].group;
                if (group != nextGroup) {
                    app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
                    app->pendingArg = nextGroup;
                } else {
                    app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
                    app->pendingArg = cursor + 1;
                }
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        CodeEntry_Backspace(app);
        PlaySE(SEQ_SE_DP_BUTTON3);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        if (app->cursors[0].value == app->firstEditable) {
            app->cursors[0].value = app->digitCount - 1;
        } else {
            app->cursors[0].value--;
        }
        cursor = app->cursors[0].value;
        if (app->digits[cursor].active == TRUE) {
            app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
            app->pendingArg = cursor;
        } else {
            app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
            app->pendingArg = app->digits[cursor].group;
            app->pendingToLast = TRUE;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        if (app->cursors[0].value == app->digitCount - 1) {
            app->cursors[0].value = app->firstEditable;
        } else {
            app->cursors[0].value++;
        }
        cursor = app->cursors[0].value;
        if (app->digits[cursor].active == TRUE) {
            app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
            app->pendingArg = cursor;
        } else {
            app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
            app->pendingArg = app->digits[cursor].group;
        }
        PlaySE(SEQ_SE_DP_SELECT78);
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_SELECT78);
        key = grid.cells[app->cursors[1].velocityY][app->cursors[1].velocityX];
        CodeEntry_MoveKeyCursor(app, key);
        if (key == CODE_ENTRY_KEY_BACK || key == CODE_ENTRY_KEY_OK) {
            if (app->cursors[1].value != 2) {
                app->cursors[1].value = 2;
                return;
            }
        } else {
            if (app->cursors[1].value != 1) {
                app->cursors[1].value = 1;
            }
        }
    }
}

// OK: empty digits become 0, all digits are appended to args.text.
static void CodeEntry_Confirm(CodeEntryApp *app) {
    String *digitString = String_New(100, HEAP_ID_108);
    int i;
    CodeEntryCell *digit;

    app->buttons[1].value = 1;
    app->buttons[1].moveFrames = 0;
    i = 0;
    if (app->digitCount > 0) {
        digit = app->digits;
        do {
            if (digit->value == 0) {
                digit->value = 1;
                ManagedSprite_SetAnim(digit->sprite, CodeEntry_GetDigitAnim(1, digit->active));
            }
            String16_FormatInteger(digitString, digit->value - 1, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            String_Cat(app->args.text, digitString);
            i++;
            digit++;
        } while (i < app->digitCount);
    }
    String_Delete(digitString);
    CodeEntry_SetState(app, CODE_ENTRY_STATE_FADE_OUT);
}

// BACK: clears the digit under the cursor and moves the cursor back by one.
static void CodeEntry_Backspace(CodeEntryApp *app) {
    s32 index;
    u32 value;
    u32 group;

    app->buttons[0].value = 1;
    app->buttons[0].moveFrames = 0;
    if (app->selectedGroup == 0) {
        app->cursors[0].value = app->digitCount - 1;
        value = app->digits[app->cursors[0].value].group;
        app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
        app->pendingArg = value;
        app->pendingToLast = TRUE;
        return;
    }
    index = app->cursors[0].value;
    app->digits[index].value = 0;
    value = CodeEntry_GetDigitAnim(app->digits[index].value, app->digits[index].active);
    ManagedSprite_SetAnim(app->digits[index].sprite, value);
    group = app->digits[index].group;
    if (index > app->firstEditable) {
        index--;
        value = CodeEntry_GetDigitAnim(app->digits[index].value, app->digits[index].active);
        ManagedSprite_SetAnim(app->digits[index].sprite, value);
        value = app->digits[index].group;
        if (group != value) {
            app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
            app->pendingArg = value;
            app->pendingToLast = TRUE;
            return;
        }
        app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
        app->pendingArg = index;
    }
}

static inline void CodeEntry_CopyKeyRects(u16 *dst, const u16 *src) {
    const u16 *source = src;
    u16 *target = dst;
    u32 count = sizeof(sCodeEntryKeyRects) / sizeof(u16);
    do {
        *target = *source;
        source++;
        target++;
    } while (--count);
}

void CodeEntry_InitTouch(CodeEntryApp *app) {
    int i;
    CodeEntryKeyRect rects[12];

    for (i = 0; i < CODE_ENTRY_HITBOX_KEYS; i++) {
        app->digits[i].hitbox = &app->hitboxes[i];
    }
    CodeEntry_CopyKeyRects((u16 *)rects, (const u16 *)sCodeEntryKeyRects);
    for (; i < CODE_ENTRY_HITBOX_NUM; i++) {
        app->hitboxes[i].rect.top = rects[i - 16].y - rects[i - 16].halfHeight;
        app->hitboxes[i].rect.left = rects[i - 16].x - rects[i - 16].halfWidth;
        app->hitboxes[i].rect.bottom = rects[i - 16].y + rects[i - 16].halfHeight;
        app->hitboxes[i].rect.right = rects[i - 16].x + rects[i - 16].halfWidth;
    }
    app->touchController = TouchHitboxController_Create(app->hitboxes, CODE_ENTRY_HITBOX_NUM, CodeEntry_OnTouch, app, HEAP_ID_108);
}

static void CodeEntry_OnTouch(u32 index, u32 event, void *arg) {
    CodeEntryApp *app = arg;
    int cursor;
    int group, nextGroup;

    if (app->state != CODE_ENTRY_STATE_INPUT) {
        return;
    }
    if (app->touchActive != TRUE) {
        app->touchActive = TRUE;
    }
    if (event == 0) {
        if (index < CODE_ENTRY_HITBOX_KEYS) {
            if (index >= app->firstEditable) {
                if (app->digits[index].active == TRUE) {
                    app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
                    app->pendingArg = index;
                } else {
                    app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
                    app->pendingArg = app->digits[index].group;
                }
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
            return;
        }
        if (index == CODE_ENTRY_HITBOX_BACK) {
            app->cursors[1].velocityX = 0;
            app->cursors[1].velocityY = 2;
            PlaySE(SEQ_SE_DP_BUTTON3);
        } else if (index == CODE_ENTRY_HITBOX_OK) {
            app->cursors[1].velocityX = 3;
            app->cursors[1].velocityY = 2;
            PlaySE(SEQ_SE_DP_PIRORIRO);
        } else {
            app->cursors[1].velocityX = (index - CODE_ENTRY_HITBOX_KEYS) % 5;
            app->cursors[1].velocityY = (index - CODE_ENTRY_HITBOX_KEYS) / 5;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
        if (index >= CODE_ENTRY_HITBOX_KEYS && index <= CODE_ENTRY_HITBOX_KEYS + 9) {
            if (app->selectedGroup == 0) {
                return;
            }
            cursor = app->cursors[0].value;
            app->digits[cursor].value = index - (CODE_ENTRY_HITBOX_KEYS - 1);
            ManagedSprite_SetAnim(app->digits[cursor].sprite, CodeEntry_GetDigitAnim(app->digits[cursor].value, app->digits[cursor].active));
            CodeEntry_SetCursorVisible(app, 1, TRUE);
            CodeEntry_MoveKeyCursor(app, index - CODE_ENTRY_HITBOX_KEYS);
            CodeEntry_SetCursorVisible(app, 1, FALSE);
            CodeEntry_SetCursorVisible(app, 2, TRUE);
            CodeEntry_MoveCursorToKey(app, index - CODE_ENTRY_HITBOX_KEYS, 2);
            ManagedSprite_SetAnim(app->cursors[2].sprite, 3);
            group = app->digits[cursor].group;
            if (cursor + 1 == app->digitCount) {
                app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
                app->pendingArg = 0;
                app->pendingToLast = FALSE;
                return;
            }
            nextGroup = app->digits[cursor + 1].group;
            if (group != nextGroup) {
                app->pendingAction = CODE_ENTRY_ACTION_SELECT_GROUP;
                app->pendingArg = nextGroup;
                app->pendingToLast = FALSE;
            } else {
                app->pendingAction = CODE_ENTRY_ACTION_MOVE_CURSOR;
                app->pendingArg = cursor + 1;
            }
        } else if (index == CODE_ENTRY_HITBOX_BACK) {
            CodeEntry_Backspace(app);
        } else {
            CodeEntry_Confirm(app);
        }
    }
}

static void CodeEntry_RunPendingAction(CodeEntryApp *app) {
    switch (app->pendingAction) {
    case CODE_ENTRY_ACTION_NONE:
        break;
    case CODE_ENTRY_ACTION_SELECT_GROUP:
        CodeEntry_SelectGroup(app, app->pendingArg);
        CodeEntry_MarkSelectedDigits(app);
        CodeEntry_LayoutDigits(app, TRUE);
        CodeEntry_SetState(app, CODE_ENTRY_STATE_SWITCH_GROUP);
        app->pendingAction = CODE_ENTRY_ACTION_BUSY;
        break;
    case CODE_ENTRY_ACTION_MOVE_CURSOR:
        CodeEntry_MoveDigitCursor(app, app->pendingArg);
        CodeEntry_ClearPendingAction(app);
        break;
    case CODE_ENTRY_ACTION_BUSY:
        break;
    }
}

static void CodeEntry_ClearPendingAction(CodeEntryApp *app) {
    app->pendingAction = CODE_ENTRY_ACTION_NONE;
    app->pendingArg = 0;
    app->pendingToLast = FALSE;
}

static int CodeEntry_GetGroupFirstDigit(CodeEntryApp *app, int group) {
    int i = 0, count = app->digitCount;
    for (; i < count; i++) {
        if (app->digits[i].group == group) {
            return i;
        }
    }
    return 0;
}

static int CodeEntry_GetGroupLastDigit(CodeEntryApp *app, int group) {
    int i;
    int found = FALSE;
    int count = app->digitCount;
    for (i = 0; i < count; i++) {
        if (app->digits[i].group == group) {
            found = TRUE;
        } else if (found == TRUE) {
            return i - 1;
        }
    }
    return count - 1;
}
