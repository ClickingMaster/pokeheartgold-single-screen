#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_screens.h"

#include "assert.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "string_util.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// x of the first digit of each group: [0] the first group has 4 digits, [1] otherwise
typedef struct VsRecorderNumberPadStartX {
    int x[2][3];
} VsRecorderNumberPadStartX;

// keys 0-9 (centre and half size); the last two entries are replaced by back and OK
typedef struct VsRecorderNumberPadKey {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} VsRecorderNumberPadKey;

typedef struct VsRecorderNumberPadKeys {
    VsRecorderNumberPadKey keys[12];
} VsRecorderNumberPadKeys;

// asm/to91_w41503_battle_names_anchor.s
extern const VsRecorderNumberPadFunc sVsRecorderNumberPadStates[];
extern const VsRecorderNumberPadStartX sVsRecorderNumberPadStartX;
extern const VsRecorderNumberPadKeys sVsRecorderNumberPadKeys;
extern f32 sVsRecorderDigitScalesNewGroup[];
extern f32 sVsRecorderDigitScalesOldGroup[];

// VsRecorderNumberPad.hitboxes: digits 0-11, then the keys
#define NUMBER_PAD_KEY_0    12
#define NUMBER_PAD_KEY_9    21
#define NUMBER_PAD_KEY_BACK 22
#define NUMBER_PAD_KEY_OK   23

#define NUMBER_PAD_RES_DIGITS  1000
#define NUMBER_PAD_RES_CURSORS 1001

#pragma force_active on

// the number of an uploaded video, "XX-XXXXX-XXXXX" (msg 0x127)
void VsRecorder_PrintVideoNumber(VsRecorderVideoSearch *search, VsRecorderWork *work) {
    MessageFormat *msgFmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    u64 value = search->videoNumber;
    u64 upper = value;
    String *low = String_New(255, HEAP_ID_109);
    String *mid = String_New(255, HEAP_ID_109);
    String *high = String_New(255, HEAP_ID_109);
    upper /= 100000;
    String *msg = NewString_ReadMsgData(work->msgData, 0x127);
    String *out = String_New(255, HEAP_ID_109);

    String16_FormatInteger(low, value % 100000, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    String16_FormatInteger(mid, upper % 100000, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    String16_FormatInteger(high, upper / 100000, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferString(msgFmt, 2, low, 0, 1, 2);
    BufferString(msgFmt, 1, mid, 0, 1, 2);
    BufferString(msgFmt, 0, high, 0, 1, 2);
    StringExpandPlaceholders(msgFmt, out, msg);
    FillWindowPixelBuffer(&work->messageWindow, 0xCC);
    AddTextPrinterParameterizedWithColor(&work->messageWindow, 0, out, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(&work->messageWindow);
    String_Delete(low);
    String_Delete(mid);
    String_Delete(high);
    String_Delete(msg);
    String_Delete(out);
    MessageFormat_ResetBuffers(msgFmt);
    MessageFormat_Delete(msgFmt);
}

#pragma force_active reset

VsRecorderNumberPadConfig *VsRecorder_NewNumberPadConfig(enum HeapID heapID, u32 length, const int *groupLengths, void *options) {
    VsRecorderNumberPadConfig *config;
    int i;
    const int *src;
    int *dst;

    config = Heap_Alloc(heapID, sizeof(VsRecorderNumberPadConfig));
    MI_CpuFill8(config, 0, sizeof(VsRecorderNumberPadConfig));
    config->length = length;
    config->text = String_New(length + 1, heapID);
    config->options = options;
    i = 0;
    src = groupLengths;
    dst = (int *)config;
    while (i < 3) {
        dst[1] = *src; // groupLengths[i]
        i++;
        src++;
        dst++;
    }
    config->groupLengths[i] = groupLengths[i - 1];
    return config;
}

void VsRecorder_FreeNumberPadConfig(VsRecorderNumberPadConfig *config) {
    GF_ASSERT(config->text != NULL);
    GF_ASSERT(config != NULL);
    String_Delete(config->text);
    Heap_Free(config);
}

void VsRecorder_NumberPadSetGroup(VsRecorderNumberPad *pad, int group) {
    int previous = pad->group;
    pad->prevGroup = previous;
    pad->group = group;
    pad->groupFirst = 0;
    pad->groupEnd = 0;
    pad->prevGroupFirst = 0;
    pad->prevGroupEnd = 0;

    if (pad->group != 0) {
        pad->groupFirst = pad->groupBounds[pad->group - 1][0];
        pad->groupEnd = pad->groupBounds[pad->group - 1][1];
    }
    if (pad->prevGroup != 0) {
        pad->prevGroupFirst = pad->groupBounds[pad->prevGroup - 1][0];
        pad->prevGroupEnd = pad->groupBounds[pad->prevGroup - 1][1];
    }
}

#pragma force_active on

void VsRecorder_NumberPadInitLayout(VsRecorderNumberPad *pad) {
    VsRecorderNumberPadStartX startX;
    int i;
    int sel;
    int sum;
    int group;
    int j;

    pad->touchInput = TRUE;
    pad->groupBounds[0][0] = 0;
    pad->groupBounds[0][1] = pad->config.groupLengths[0];
    pad->groupBounds[1][0] = pad->config.groupLengths[0];
    pad->groupBounds[1][1] = pad->config.groupLengths[0] + pad->config.groupLengths[1];
    pad->groupBounds[2][0] = pad->config.groupLengths[0] + pad->config.groupLengths[1];
    pad->groupBounds[2][1] = pad->config.groupLengths[2] + (pad->config.groupLengths[0] + pad->config.groupLengths[1]);
    VsRecorder_NumberPadSetGroup(pad, 1);
    for (i = 0; i < 3; i++) {
        pad->count += pad->config.groupLengths[i];
    }
    sel = 0;
    startX = sVsRecorderNumberPadStartX;
    if (pad->config.groupLengths[0] != 4) {
        sel = 1;
    }
    for (i = 0; i < 3; i++) {
        pad->startX[i] = startX.x[sel][i];
    }
    pad->startX[i] = startX.x[sel][i - 1];
    sum = 0;
    for (i = 0; i < 2; i++) {
        sum += pad->config.groupLengths[i];
        pad->separators[i].value = sum - 1;
    }
    i = 0;
    group = 0;
    do {
        for (j = 0; j < pad->config.groupLengths[group]; j++) {
            pad->digits[i].group = group + 1;
            i++;
        }
        group++;
    } while (i < pad->count);
}

#pragma force_active reset

void VsRecorder_NumberPadSetState(VsRecorderNumberPad *pad, int state) {
    pad->state = state;
    pad->unk_1F8 = 0;
    pad->step = 0;
    pad->timer = 0;
}

#pragma force_active on

BOOL VsRecorder_NumberPadInit(VsRecorderNumberPad *pad) {
    VsRecorder_NumberPadInitLayout(pad);
    VsRecorder_NumberPadInitHitboxes(pad);
    VsRecorder_NumberPadLoadGfx(pad);
    VsRecorder_NumberPadUpdateEnabled(pad);
    VsRecorder_NumberPadCreateDigitSprites(pad);
    VsRecorder_NumberPadLayoutDigits(pad, FALSE);
    VsRecorder_NumberPadShowSprites(pad);
    VsRecorder_NumberPadCreateCursors(pad);
    VsRecorder_NumberPadUpdateHitboxes(pad);
    VsRecorder_NumberPadSetState(pad, VS_RECORDER_NUMBER_PAD_INPUT);
    return FALSE;
}

#pragma force_active reset

BOOL VsRecorder_NumberPadFree(VsRecorderNumberPad *pad) {
    TouchHitboxController_Destroy(pad->hitboxController);
    VsRecorder_NumberPadFreeSprites(pad);
    return TRUE;
}

BOOL VsRecorder_NumberPadInput(VsRecorderNumberPad *pad) {
    if (pad->step == 0) {
        pad->step++;
    } else {
        VsRecorder_NumberPadRunAction(pad);
        TouchHitboxController_IsTriggered(pad->hitboxController);
    }
    return FALSE;
}

#pragma force_active on

// the digits of the new group grow, those of the old group shrink back
BOOL VsRecorder_NumberPadSwitchGroup(VsRecorderNumberPad *pad) {
    int i;
    VsRecorderNumberPadDigit *digit;

    switch (pad->step) {
    case 0:
        VsRecorder_NumberPadSetCursorDrawFlag(pad, 0, FALSE);
        for (i = 0; i < pad->count; i++) {
            digit = pad->digits + i;
            if (digit->frames) {
                ManagedSprite_OffsetPositionXY(digit->sprite, digit->dx, digit->dy);
                digit->frames--;
                if (i >= pad->groupFirst && i < pad->groupEnd) {
                    ManagedSprite_SetAffineScale(digit->sprite, sVsRecorderDigitScalesNewGroup[digit->scaleStep], sVsRecorderDigitScalesNewGroup[digit->scaleStep]);
                    digit->scaleStep++;
                    ManagedSprite_SetAffineOverwriteMode(digit->sprite, 2);
                }
                if (i >= pad->prevGroupFirst && i < pad->prevGroupEnd) {
                    ManagedSprite_SetAffineScale(digit->sprite, sVsRecorderDigitScalesOldGroup[digit->scaleStep], sVsRecorderDigitScalesOldGroup[digit->scaleStep]);
                    digit->scaleStep++;
                    ManagedSprite_SetAffineOverwriteMode(digit->sprite, 2);
                }
            }
        }
        for (i = 0; i < 2; i++) {
            if (pad->separators[i].frames) {
                ManagedSprite_OffsetPositionXY(pad->separators[i].sprite, pad->separators[i].dx, pad->separators[i].dy);
                pad->separators[i].frames--;
            }
        }
        if (!pad->digits[0].frames) {
            for (i = pad->groupFirst; i < pad->groupEnd; i++) {
                digit = pad->digits + i;
                ManagedSprite_SetAnim(digit->sprite, VsRecorder_GetDigitAnim(digit->value, digit->enabled));
                ManagedSprite_TickFrame(digit->sprite);
            }
            for (i = pad->prevGroupFirst; i < pad->prevGroupEnd; i++) {
                digit = pad->digits + i;
                ManagedSprite_SetAnim(digit->sprite, VsRecorder_GetDigitAnim(digit->value, digit->enabled));
                ManagedSprite_TickFrame(digit->sprite);
            }
            pad->step++;
        }
        pad->timer++;
        break;
    case 1:
        for (i = pad->groupFirst; i < pad->groupEnd; i++) {
            digit = pad->digits + i;
            if (digit->scaleStep == 6) {
                ManagedSprite_SetAffineOverwriteMode(digit->sprite, 1);
            } else {
                ManagedSprite_SetAffineScale(digit->sprite, sVsRecorderDigitScalesNewGroup[digit->scaleStep], sVsRecorderDigitScalesNewGroup[digit->scaleStep]);
                digit->scaleStep++;
            }
        }
        for (i = pad->prevGroupFirst; i < pad->prevGroupEnd; i++) {
            digit = pad->digits + i;
            if (digit->scaleStep == 6) {
                ManagedSprite_SetAffineOverwriteMode(digit->sprite, 1);
            } else {
                ManagedSprite_SetAffineScale(digit->sprite, sVsRecorderDigitScalesOldGroup[digit->scaleStep], sVsRecorderDigitScalesOldGroup[digit->scaleStep]);
                digit->scaleStep++;
            }
        }
        pad->timer++;
        if (pad->timer == 6) {
            pad->step++;
        }
        break;
    default:
        VsRecorder_NumberPadUpdateHitboxes(pad);
        if (pad->actionMode == 0) {
            VsRecorder_NumberPadMoveCursor(pad, VsRecorder_NumberPadFindGroupFirst(pad, pad->actionValue));
        } else {
            VsRecorder_NumberPadMoveCursor(pad, VsRecorder_NumberPadFindGroupLast(pad, pad->actionValue));
        }
        if (pad->group) {
            VsRecorder_NumberPadSetCursorDrawFlag(pad, 0, TRUE);
        }
        VsRecorder_NumberPadClearAction(pad);
        VsRecorder_NumberPadSetState(pad, VS_RECORDER_NUMBER_PAD_INPUT);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// one step of the number pad; TRUE when it closed
BOOL VsRecorder_NumberPadMain(VsRecorderNumberPad *pad) {
    BOOL result = sVsRecorderNumberPadStates[pad->state](pad);

    if (result == FALSE) {
        VsRecorder_NumberPadUpdateCursors(pad);
    }
    return result;
}

// OK: empty digits become 0, the number and its text are stored
void VsRecorder_NumberPadConfirm(VsRecorderNumberPad *pad) {
    u32 num;
    String *str;
    int i;
    VsRecorderNumberPadDigit *digit;

    str = String_New(100, HEAP_ID_109);
    pad->confirmed = TRUE;
    pad->number = 0;
    if (pad->count > 0) {
        i = 0;
        digit = pad->digits;
        do {
            if (digit->value == 0) {
                digit->value = 1;
                ManagedSprite_SetAnim(digit->sprite, VsRecorder_GetDigitAnim(digit->value, digit->enabled));
            }
            num = digit->value - 1;
            if (i != 0) {
                pad->number *= 10;
            }
            pad->number += num;
            String16_FormatInteger(str, num, 1, PRINTING_MODE_RIGHT_ALIGN, 1);
            String_Cat(pad->config.text, str);
            i++;
            digit++;
        } while (i < pad->count);
    }
    String_Delete(str);
    VsRecorder_NumberPadSetState(pad, VS_RECORDER_NUMBER_PAD_FREE);
}

#pragma force_active on

// back: clears the selected digit and moves to the previous one; on the first digit the pad closes (cancelled)
void VsRecorder_NumberPadBack(VsRecorderNumberPad *pad) {
    if (pad->group == 0) {
        pad->cursors[0].value = pad->count - 1;
        u32 group = pad->digits[pad->cursors[0].value].group;
        pad->action = VS_RECORDER_NUMBER_PAD_ACTION_GROUP;
        pad->actionValue = group;
        pad->actionMode = 1;
        return;
    }
    int index = pad->cursors[0].value;
    *(volatile int *)&pad->digits[index].value = 0;
    ManagedSprite_SetAnim(pad->digits[index].sprite, VsRecorder_GetDigitAnim(*(volatile int *)&pad->digits[index].value, pad->digits[index].enabled));
    u32 group = pad->digits[index].group;
    if (index > 0) {
        ManagedSprite_SetAnim(pad->digits[index - 1].sprite, VsRecorder_GetDigitAnim(*(volatile int *)&pad->digits[index - 1].value, pad->digits[index - 1].enabled));
        u32 prevGroup = pad->digits[index - 1].group;
        if (group != prevGroup) {
            pad->action = VS_RECORDER_NUMBER_PAD_ACTION_GROUP;
            pad->actionValue = prevGroup;
            pad->actionMode = 1;
        } else {
            pad->action = VS_RECORDER_NUMBER_PAD_ACTION_MOVE;
            pad->actionValue = index - 1;
        }
    } else {
        pad->confirmed = FALSE;
        VsRecorder_NumberPadSetState(pad, VS_RECORDER_NUMBER_PAD_FREE);
    }
}

void VsRecorder_NumberPadInitHitboxes(VsRecorderNumberPad *pad) {
    int i;
    VsRecorderNumberPadKeys keys;

    for (i = 0; i < 12; i++) {
        pad->digits[i].hitbox = &pad->hitboxes[i];
    }
    keys = sVsRecorderNumberPadKeys;
    for (; i < 24; i++) {
        pad->hitboxes[i].rect.top = keys.keys[i - 12].y - keys.keys[i - 12].halfHeight;
        pad->hitboxes[i].rect.left = keys.keys[i - 12].x - keys.keys[i - 12].halfWidth;
        pad->hitboxes[i].rect.bottom = keys.keys[i - 12].y + keys.keys[i - 12].halfHeight;
        pad->hitboxes[i].rect.right = keys.keys[i - 12].x + keys.keys[i - 12].halfWidth;
    }
    pad->hitboxes[NUMBER_PAD_KEY_BACK].rect.top = 152;
    pad->hitboxes[NUMBER_PAD_KEY_BACK].rect.left = 32;
    pad->hitboxes[NUMBER_PAD_KEY_BACK].rect.bottom = 184;
    pad->hitboxes[NUMBER_PAD_KEY_BACK].rect.right = 128;
    pad->hitboxes[NUMBER_PAD_KEY_OK].rect.top = 152;
    pad->hitboxes[NUMBER_PAD_KEY_OK].rect.left = 128;
    pad->hitboxes[NUMBER_PAD_KEY_OK].rect.bottom = 184;
    pad->hitboxes[NUMBER_PAD_KEY_OK].rect.right = 224;
    pad->hitboxController = TouchHitboxController_Create(pad->hitboxes, 24, VsRecorder_OnNumberPadTouch, pad, HEAP_ID_109);
}

// digits select a digit (or its group); keys 0-9 enter a digit and advance; back; OK
void VsRecorder_OnNumberPadTouch(u32 index, u32 event, void *arg) {
    VsRecorderNumberPad *pad = arg;
    VsRecorderNumberPadDigit *digit;
    int slot, group, next, nextGroup;

    if (pad->state == VS_RECORDER_NUMBER_PAD_INPUT) {
        if (pad->touchInput != TRUE) {
            pad->touchInput = TRUE;
        }
        if (event != 0) {
            return;
        }
        if (index <= 11) {
            digit = &pad->digits[index];
            if (digit->enabled == TRUE) {
                pad->action = VS_RECORDER_NUMBER_PAD_ACTION_MOVE;
                pad->actionValue = index;
            } else {
                pad->action = VS_RECORDER_NUMBER_PAD_ACTION_GROUP;
                pad->actionValue = digit->group;
            }
            PlaySE(SEQ_SE_PL_BREC57);
            return;
        }
        if (index == NUMBER_PAD_KEY_BACK) {
            pad->cursors[1].dx = 0;
            pad->cursors[1].dy = 2;
        } else if (index == NUMBER_PAD_KEY_OK) {
            pad->cursors[1].dx = 3;
            pad->cursors[1].dy = 2;
        } else {
            pad->cursors[1].dx = (index - NUMBER_PAD_KEY_0) % 5;
            pad->cursors[1].dy = (index - NUMBER_PAD_KEY_0) / 5;
        }
        if (index >= NUMBER_PAD_KEY_0 && index <= NUMBER_PAD_KEY_9) {
            if (pad->group == 0) {
                return;
            }
            PlaySE(SEQ_SE_PL_BREC57);
            slot = pad->cursors[0].value;
            pad->digits[slot].value = index - (NUMBER_PAD_KEY_0 - 1);
            ManagedSprite_SetAnim(pad->digits[slot].sprite, VsRecorder_GetDigitAnim(pad->digits[slot].value, pad->digits[slot].enabled));
            VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, TRUE);
            index -= NUMBER_PAD_KEY_0;
            VsRecorder_NumberPadPlaceKeyCursor(pad, index);
            VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, FALSE);
            VsRecorder_NumberPadSetCursorDrawFlag(pad, 2, TRUE);
            VsRecorder_NumberPadPlaceCursorOnKey(pad, index, 2);
            ManagedSprite_SetAnim(pad->cursors[2].sprite, 3);
            group = pad->digits[slot].group;
            next = slot + 1;
            if (next == pad->count) {
                pad->action = VS_RECORDER_NUMBER_PAD_ACTION_GROUP;
                pad->actionValue = 0;
                pad->actionMode = 0;
                return;
            }
            nextGroup = pad->digits[next].group;
            if (group != nextGroup) {
                pad->action = VS_RECORDER_NUMBER_PAD_ACTION_GROUP;
                pad->actionValue = nextGroup;
                pad->actionMode = 0;
                return;
            }
            pad->action = VS_RECORDER_NUMBER_PAD_ACTION_MOVE;
            pad->actionValue = next;
        } else if (index == NUMBER_PAD_KEY_BACK) {
            PlaySE(SEQ_SE_PL_BREC57);
            VsRecorder_NumberPadBack(pad);
        } else {
            PlaySE(SEQ_SE_PL_BREC57);
            VsRecorder_NumberPadConfirm(pad);
        }
    }
}

#pragma force_active reset

void VsRecorder_NumberPadRunAction(VsRecorderNumberPad *pad) {
    switch (pad->action) {
    case VS_RECORDER_NUMBER_PAD_ACTION_GROUP:
        VsRecorder_NumberPadSetGroup(pad, pad->actionValue);
        VsRecorder_NumberPadUpdateEnabled(pad);
        VsRecorder_NumberPadLayoutDigits(pad, TRUE);
        VsRecorder_NumberPadSetState(pad, VS_RECORDER_NUMBER_PAD_SWITCH);
        pad->action = VS_RECORDER_NUMBER_PAD_ACTION_DONE;
        break;
    case VS_RECORDER_NUMBER_PAD_ACTION_MOVE:
        VsRecorder_NumberPadMoveCursor(pad, pad->actionValue);
        VsRecorder_NumberPadClearAction(pad);
        break;
    case VS_RECORDER_NUMBER_PAD_ACTION_NONE:
    case VS_RECORDER_NUMBER_PAD_ACTION_DONE:
        break;
    }
}

void VsRecorder_NumberPadClearAction(VsRecorderNumberPad *pad) {
    pad->action = VS_RECORDER_NUMBER_PAD_ACTION_NONE;
    pad->actionValue = 0;
    pad->actionMode = 0;
}

int VsRecorder_NumberPadFindGroupFirst(VsRecorderNumberPad *pad, int group) {
    int i;
    int n = pad->count;
    VsRecorderNumberPadDigit *digit = pad->digits;
    for (i = 0; i < n; i++) {
        if (group == digit->group) {
            return i;
        }
        digit++;
    }
    return 0;
}

int VsRecorder_NumberPadFindGroupLast(VsRecorderNumberPad *pad, int group) {
    int i;
    int found;
    int n = pad->count;
    VsRecorderNumberPadDigit *digit = pad->digits;
    found = 0;
    for (i = 0; i < n; i++) {
        if (group == digit->group) {
            found = 1;
        } else if (found == 1) {
            return i - 1;
        }
        digit++;
    }
    return n - 1;
}

#pragma force_active on

void VsRecorder_NumberPadLoadGfx(VsRecorderNumberPad *pad) {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *plttData;

    spriteSystem = pad->spriteSystem;
    spriteManager = pad->spriteManager;
    plttData = pad->plttData;
    bgConfig = pad->bgConfig;
    narc = pad->narc;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 54, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 43, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 44, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, NUMBER_PAD_RES_DIGITS);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 40, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, NUMBER_PAD_RES_DIGITS);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 41, FALSE, NUMBER_PAD_RES_DIGITS);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 42, FALSE, NUMBER_PAD_RES_DIGITS);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 88, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, NUMBER_PAD_RES_CURSORS);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 87, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, NUMBER_PAD_RES_CURSORS);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 85, FALSE, NUMBER_PAD_RES_CURSORS);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 86, FALSE, NUMBER_PAD_RES_CURSORS);
}

#pragma force_active reset

void VsRecorder_NumberPadShowSprites(VsRecorderNumberPad *pad) {
    int i = 0;
    VsRecorderNumberPadDigit *digit;
    if (pad->count > 0) {
        digit = pad->digits;
        do {
            if (digit->sprite != NULL) {
                ManagedSprite_SetDrawFlag(digit->sprite, TRUE);
            }
            i++;
            digit++;
        } while (i < pad->count);
    }
    for (i = 0; i < 2; i++) {
        if (pad->separators[i].sprite != NULL) {
            ManagedSprite_SetDrawFlag(pad->separators[i].sprite, TRUE);
        }
    }
}

void VsRecorder_NumberPadFreeSprites(VsRecorderNumberPad *pad) {
    int i = 0;
    VsRecorderNumberPadDigit *digit;
    if (pad->count > 0) {
        digit = pad->digits;
        do {
            Sprite_DeleteAndFreeResources(digit->sprite);
            i++;
            digit++;
        } while (i < pad->count);
    }
    for (i = 0; i < 2; i++) {
        Sprite_DeleteAndFreeResources(pad->separators[i].sprite);
    }
    for (i = 0; i < 3; i++) {
        Sprite_DeleteAndFreeResources(pad->cursors[i].sprite);
    }
    SpriteManager_UnloadCharObjById(pad->spriteManager, NUMBER_PAD_RES_DIGITS);
    SpriteManager_UnloadPlttObjById(pad->spriteManager, NUMBER_PAD_RES_DIGITS);
    SpriteManager_UnloadCellObjById(pad->spriteManager, NUMBER_PAD_RES_DIGITS);
    SpriteManager_UnloadAnimObjById(pad->spriteManager, NUMBER_PAD_RES_DIGITS);
    SpriteManager_UnloadCharObjById(pad->spriteManager, NUMBER_PAD_RES_CURSORS);
    SpriteManager_UnloadPlttObjById(pad->spriteManager, NUMBER_PAD_RES_CURSORS);
    SpriteManager_UnloadCellObjById(pad->spriteManager, NUMBER_PAD_RES_CURSORS);
    SpriteManager_UnloadAnimObjById(pad->spriteManager, NUMBER_PAD_RES_CURSORS);
}

// digit and separator sprites in one row (VsRecorder_NumberPadLayoutDigits spaces them out)
void VsRecorder_NumberPadCreateDigitSprites(VsRecorderNumberPad *pad) {
    int separators = 0;
    SpriteSystem *spriteSystem = pad->spriteSystem;
    SpriteManager *spriteManager = pad->spriteManager;
    ManagedSpriteTemplate template;
    int i;
    VsRecorderNumberPad *separator; // walks from the pad base: separator->separators[0] is separators[separators]
    VsRecorderNumberPadDigit *digit;
    int x;

    template.x = 0x130;
    template.y = 0xE0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = NUMBER_PAD_RES_DIGITS;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = NUMBER_PAD_RES_DIGITS;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = NUMBER_PAD_RES_DIGITS;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = NUMBER_PAD_RES_DIGITS;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

    i = 0;
    if (pad->count + 2 > 0) {
        separator = pad;
        x = 0x4C;
        digit = pad->digits;
        do {
            if (i == separators + separator->separators[0].value + 1) {
                separator->separators[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetDrawFlag(separator->separators[0].sprite, FALSE);
                ManagedSprite_SetPositionXY(separator->separators[0].sprite, x, 0x18);
                ManagedSprite_SetAnim(separator->separators[0].sprite, 0x16);
                ManagedSprite_TickFrame(separator->separators[0].sprite);
                separator = (VsRecorderNumberPad *)((u8 *)separator + sizeof(VsRecorderNumberPadDigit));
                separators++;
            } else {
                digit->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetDrawFlag(digit->sprite, FALSE);
                ManagedSprite_SetPositionXY(digit->sprite, x, 0x18);
                ManagedSprite_SetAnim(digit->sprite, VsRecorder_GetDigitAnim(digit->value, digit->enabled));
                ManagedSprite_SetAffineOverwriteMode(digit->sprite, 0);
                ManagedSprite_TickFrame(digit->sprite);
                digit++;
            }
            i++;
            x += 8;
        } while (i < pad->count + 2);
    }
}

#pragma force_active on

static inline void VsRecorder_InitKeyCursor(VsRecorderNumberPad *pad, VsRecorderNumberPadDigit *cursor) {
    cursor->dx = 0;
    cursor->dy = 0;
    cursor->value = 1;
    VsRecorder_NumberPadPlaceKeyCursor(pad, 0);
    ManagedSprite_SetAnim(cursor->sprite, cursor->value);
    ManagedSprite_TickFrame(cursor->sprite);
    ManagedSprite_SetOamMode(cursor->sprite, GX_OAM_MODE_XLU);
}

void VsRecorder_NumberPadCreateCursors(VsRecorderNumberPad *pad) {
    SpriteSystem *spriteSystem = pad->spriteSystem;
    SpriteManager *spriteManager = pad->spriteManager;
    ManagedSpriteTemplate template;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = NUMBER_PAD_RES_CURSORS;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = NUMBER_PAD_RES_CURSORS;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = NUMBER_PAD_RES_CURSORS;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = NUMBER_PAD_RES_CURSORS;
    template.drawPriority = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    pad->cursors[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    pad->cursors[1].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    pad->cursors[2].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    VsRecorder_NumberPadMoveCursor(pad, 0);
    ManagedSprite_SetAnim(pad->cursors[0].sprite, 0);
    ManagedSprite_TickFrame(pad->cursors[0].sprite);
    VsRecorder_InitKeyCursor(pad, &pad->cursors[1]);
    VsRecorder_InitKeyCursor(pad, &pad->cursors[2]);
    VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, FALSE);
    VsRecorder_NumberPadSetCursorDrawFlag(pad, 2, FALSE);
}

#pragma force_active reset

void VsRecorder_NumberPadSetCursorDrawFlag(VsRecorderNumberPad *pad, int cursor, int draw) {
    if (draw == TRUE) {
        ManagedSprite_SetDrawFlag(pad->cursors[cursor].sprite, TRUE);
    } else {
        ManagedSprite_SetDrawFlag(pad->cursors[cursor].sprite, FALSE);
    }
}

void VsRecorder_NumberPadMoveCursor(VsRecorderNumberPad *pad, int digit) {
    s16 x;
    s16 y;
    ManagedSprite *sprite = pad->digits[digit].sprite;
    pad->cursors[0].value = digit;
    ManagedSprite_GetPositionXY(sprite, &x, &y);
    ManagedSprite_SetPositionXY(pad->cursors[0].sprite, x, y + 16);
}

#pragma force_active on

void VsRecorder_NumberPadPlaceKeyCursor(VsRecorderNumberPad *pad, int key) {
    int x = (s16)((pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.left + pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.right) / 2);
    int y = (s16)((pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.top + pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.bottom) / 2);
    ManagedSprite_SetPositionXY(pad->cursors[1].sprite, x, y - 8);
}

void VsRecorder_NumberPadPlaceCursorOnKey(VsRecorderNumberPad *pad, int key, int cursor) {
    int x = (s16)((pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.left + pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.right) / 2);
    int y = (s16)((pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.top + pad->hitboxes[key + NUMBER_PAD_KEY_0].rect.bottom) / 2);
    ManagedSprite_SetPositionXY(pad->cursors[cursor].sprite, x, y - 8);
}

#pragma force_active reset

// the key press (anim 3) hides itself when it ends; the key cursor shows unless the input was a touch
void VsRecorder_NumberPadUpdateCursors(VsRecorderNumberPad *pad) {
    VsRecorderNumberPad *entry; // entry->cursors[0] is cursors[i]
    int i;

    ManagedSprite_TickFrame(pad->cursors[0].sprite);
    ManagedSprite_TickFrame(pad->cursors[1].sprite);
    ManagedSprite_TickFrame(pad->cursors[2].sprite);

    for (i = 1, entry = (VsRecorderNumberPad *)((u8 *)pad + sizeof(VsRecorderNumberPadDigit)); i < 3; i++, entry = (VsRecorderNumberPad *)((u8 *)entry + sizeof(VsRecorderNumberPadDigit))) {
        u32 anim = ManagedSprite_GetActiveAnim(entry->cursors[0].sprite);
        if (anim == 3) {
            if (ManagedSprite_IsAnimated(entry->cursors[0].sprite) == FALSE) {
                ManagedSprite_SetAnim(entry->cursors[0].sprite, entry->cursors[0].value);
                if (pad->touchInput == TRUE) {
                    VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, FALSE);
                } else {
                    VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, TRUE);
                }
                VsRecorder_NumberPadSetCursorDrawFlag(pad, 2, FALSE);
            }
        } else {
            if (anim != entry->cursors[0].value) {
                ManagedSprite_SetAnim(entry->cursors[0].sprite, entry->cursors[0].value);
            }
            if (ManagedSprite_GetActiveAnim(pad->cursors[2].sprite) != 3) {
                if (pad->touchInput == TRUE) {
                    VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, FALSE);
                } else {
                    VsRecorder_NumberPadSetCursorDrawFlag(pad, 1, TRUE);
                }
            }
        }
    }
}

// digits outside the current group use the small frames (anims from 11)
int VsRecorder_GetDigitAnim(int value, BOOL enabled) {
    int anim = 0;
    if (enabled == FALSE) {
        anim = 11;
    }
    return anim + value;
}

void VsRecorder_NumberPadUpdateEnabled(VsRecorderNumberPad *pad) {
    int i;
    VsRecorderNumberPadDigit *digit;
    i = 0;
    if (pad->count > 0) {
        digit = pad->digits;
        do {
            if (i >= pad->groupFirst && i < pad->groupEnd) {
                digit->enabled = TRUE;
            } else {
                digit->enabled = FALSE;
            }
            i++;
            digit++;
        } while (i < pad->count);
    }
}

#pragma force_active on

// the digits of the current group are 32 pixels apart, the others 8; animate: move there in 2 frames
void VsRecorder_NumberPadLayoutDigits(VsRecorderNumberPad *pad, BOOL animate) {
    int i = 0, separators = 0;
    s16 x = pad->startX[pad->group];
    s16 position[2];
    if (pad->count > 0) {
        VsRecorderNumberPadDigit *digit = pad->digits;
        VsRecorderNumberPad *separator = pad; // separator->separators[0] is separators[separators]
        do {
            if (i >= pad->groupFirst && i < pad->groupEnd) {
                if (i == pad->groupFirst) {
                    x += 20;
                } else {
                    x += 32;
                }
            } else {
                if (i == 0) {
                    x += 20;
                } else {
                    x += 8;
                }
            }
            ManagedSprite_GetPositionXY(digit->sprite, &position[1], &position[0]);
            if (!animate) {
                ManagedSprite_SetPositionXY(digit->sprite, x, position[0]);
            } else {
                digit->dx = (x - position[1]) / 2;
                digit->dy = 0;
                digit->frames = 2;
                digit->scaleStep = 0;
            }
            if (i == separator->separators[0].value && separators != 2) {
                ManagedSprite_GetPositionXY(separator->separators[0].sprite, &position[1], &position[0]);
                if (pad->groupFirst == pad->groupEnd) {
                    x += 8;
                } else if (i > pad->groupFirst && i < pad->groupEnd) {
                    x += 20;
                } else {
                    x += 8;
                }
                if (!animate) {
                    ManagedSprite_SetPositionXY(separator->separators[0].sprite, x, position[0]);
                } else {
                    separator->separators[0].dx = (x - position[1]) / 2;
                    separator->separators[0].dy = 0;
                    separator->separators[0].frames = 2;
                }
                separator = (VsRecorderNumberPad *)((u8 *)separator + sizeof(VsRecorderNumberPadDigit));
                separators++;
            }
            digit++;
            i++;
        } while (i < pad->count);
    }
}

// each digit's touch box follows its sprite (16x16 around the current group's digits, 4x8 around the others)
void VsRecorder_NumberPadUpdateHitboxes(VsRecorderNumberPad *pad) {
    int i, dx, dy;
    VsRecorderNumberPadDigit *digit;
    s16 y, x;
    i = 0;
    if (pad->count > 0) {
        digit = pad->digits;
        do {
            if (i >= pad->groupFirst && i < pad->groupEnd) {
                dx = 16;
                dy = 16;
            } else {
                dx = 4;
                dy = 8;
            }
            ManagedSprite_GetPositionXY(digit->sprite, &x, &y);
            digit->hitbox->rect.top = y - dy;
            digit->hitbox->rect.left = x - dx;
            digit->hitbox->rect.bottom = y + dy;
            digit->hitbox->rect.right = x + dx;
            if (i == 0) {
                digit->hitbox->rect.left -= 4;
            }
            i++;
            digit++;
        } while (i < pad->count);
    }
}

#pragma force_active reset
