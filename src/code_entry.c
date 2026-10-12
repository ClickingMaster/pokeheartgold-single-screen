#include "code_entry.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0038.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "vram_transfer_manager.h"

// Keypad cells (sCodeEntryKeyGrid / sCodeEntryKeyRects): digits 0-9 in two rows of five, then BACK and OK.
#define CODE_ENTRY_KEY_BACK 10
#define CODE_ENTRY_KEY_OK   11

// Touch hitboxes: the 16 digit slots, the 10 digit keys, BACK, OK.
#define CODE_ENTRY_HITBOX_KEYS 16
#define CODE_ENTRY_HITBOX_BACK 26
#define CODE_ENTRY_HITBOX_OK   27
#define CODE_ENTRY_HITBOX_NUM  28

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

// A digit slot, a group separator, one of the three cursors or a BACK/OK button.
typedef struct CodeEntryCell {
    int value;  // digit + 1 (0: empty); separator: index of the digit before it; cursor: current anim
    int group;  // 1-based group of a digit
    int active; // digit is in the selected group (drawn large)
    ManagedSprite *sprite;
    TouchscreenHitbox *hitbox;
    s16 velocityX; // key cursor: column
    s16 velocityY; // key cursor: row
    u8 moveFrames; // button: frames since it was pressed
    u8 animStep;
    u8 padding[2];
} CodeEntryCell;

typedef struct CodeEntryRange {
    u16 start;
    u16 end;
} CodeEntryRange;

typedef struct CodeEntryButtonLabelAlloc {
    UnkStruct_02021AC8 charAlloc;
} CodeEntryButtonLabelAlloc;

typedef struct CodeEntryApp {
    CodeEntryCell digits[16];
    CodeEntryCell separators[3];
    CodeEntryCell cursors[3]; // digit cursor (value: digit index), key cursor, key press flash
    CodeEntryCell buttons[2]; // BACK, OK (value: pressed state)
    s16 groupX[5];
    CodeEntryRange groupRanges[5];
    u8 padding2BE[2];
    int state;
    int unk2C4;
    int subState;
    int frame;
    int digitCount;
    int selectedGroup;
    int previousGroup;
    int selectedStart;
    int selectedEnd;
    int previousStart;
    int previousEnd;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    TouchHitboxController *touchController;
    TouchscreenHitbox hitboxes[CODE_ENTRY_HITBOX_NUM];
    int touchActive;
    FontSystem *fontSystem;
    TextOBJ *buttonLabels[2];
    CodeEntryButtonLabelAlloc buttonLabelAllocs[2];
    Window promptWindow;
    int pendingAction;
    int pendingArg;
    int pendingToLast;
    CodeEntryArgs args;
    int separatorCount;
    int firstEditable;
} CodeEntryApp; // size: 0x3F4

typedef struct CodeEntryKeyGrid {
    int cells[3][5];
} CodeEntryKeyGrid;

typedef struct CodeEntryKeyRect {
    s16 x;
    s16 y;
    s16 halfWidth;
    s16 halfHeight;
} CodeEntryKeyRect;

typedef BOOL (*CodeEntryStateFunc)(CodeEntryApp *app);

typedef struct CodeEntryMainBgTemplates {
    BgTemplate templates[3];
} CodeEntryMainBgTemplates;

typedef struct CodeEntrySubBgTemplates {
    BgTemplate templates[4];
} CodeEntrySubBgTemplates;

// data in asm/code_entry_data.s
extern const GraphicsBanks sCodeEntryGraphicsBanks;
extern const CodeEntryMainBgTemplates sCodeEntryMainBgTemplates;
extern const CodeEntrySubBgTemplates sCodeEntrySubBgTemplates;
extern const CodeEntryStateFunc sCodeEntryStates[4];
extern const CodeEntryKeyGrid sCodeEntryKeyGrid;
extern const CodeEntryKeyRect sCodeEntryKeyRects[12];
extern const OamCharTransferParam sCodeEntryOamCharTransferParam;
extern const SpriteResourceCountsListUnion sCodeEntrySpriteResourceCounts;
extern const OamManagerParam sCodeEntryOamManagerParam;
extern f32 sCodeEntryGrowScales[];
extern f32 sCodeEntryShrinkScales[];

void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapID);
void WirelessStatusIcon_ShowOnBothScreens(int a0, enum HeapID heapID);
void WirelessStatusIcon_SetLevel(int level);
void WirelessStatusIcon_Create(void);
void WirelessStatusIcon_Destroy(void);
int ov00_021EC9D4(void);
u32 GetUserFrameGfx2PaletteMember(u32 frame); // render_window.c

CodeEntryArgs *CodeEntryArgs_Alloc(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon);
void CodeEntry_SetupBgs(BgConfig *bgConfig);
void CodeEntry_VBlank(CodeEntryApp *app);
void CodeEntry_SelectGroup(CodeEntryApp *app, int group);
void CodeEntry_InitLayout(CodeEntryApp *app);
void CodeEntry_SetState(CodeEntryApp *app, int state);
BOOL CodeEntry_State_Setup(CodeEntryApp *app);
BOOL CodeEntry_State_FadeOut(CodeEntryApp *app);
BOOL CodeEntry_State_Input(CodeEntryApp *app);
BOOL CodeEntry_State_SwitchGroup(CodeEntryApp *app);
BOOL CodeEntry_RunState(CodeEntryApp *app);
void CodeEntry_HandleKeys(CodeEntryApp *app);
void CodeEntry_Confirm(CodeEntryApp *app);
void CodeEntry_Backspace(CodeEntryApp *app);
void CodeEntry_InitTouch(CodeEntryApp *app);
void CodeEntry_OnTouch(u32 index, u32 event, void *arg);
void CodeEntry_RunPendingAction(CodeEntryApp *app);
void CodeEntry_ClearPendingAction(CodeEntryApp *app);
int CodeEntry_GetGroupFirstDigit(CodeEntryApp *app, int group);
int CodeEntry_GetGroupLastDigit(CodeEntryApp *app, int group);
void CodeEntry_InitSpriteSystem(CodeEntryApp *app);
void CodeEntry_LoadGraphics(CodeEntryApp *app);
void CodeEntry_FreeGraphics(CodeEntryApp *app);
void CodeEntry_CreateDigitSprites(CodeEntryApp *app);
void CodeEntry_CreateCursorSprites(CodeEntryApp *app);
void CodeEntry_CreateButtonSprites(CodeEntryApp *app);
void CodeEntry_SetCursorVisible(CodeEntryApp *app, int cursor, int visible);
void CodeEntry_MoveDigitCursor(CodeEntryApp *app, int digit);
void CodeEntry_MoveKeyCursor(CodeEntryApp *app, int key);
void CodeEntry_MoveCursorToKey(CodeEntryApp *app, int key, int cursor);
void CodeEntry_UpdateCursors(CodeEntryApp *app);
void CodeEntry_SetButtonLabelPosition(TextOBJ *label, int x, int y);
void CodeEntry_UpdateButtons(CodeEntryApp *app);
int CodeEntry_GetDigitAnim(int value, int active);
void CodeEntry_MarkSelectedDigits(CodeEntryApp *app);
void CodeEntry_LayoutDigits(CodeEntryApp *app, int animate);
void CodeEntry_UpdateDigitHitboxes(CodeEntryApp *app);
void CodeEntry_InitFont(CodeEntryApp *app);
void CodeEntry_FreeButtonLabels(CodeEntryApp *app);
void CodeEntry_LoadButtonLabelPalette(CodeEntryApp *app);
void CodeEntry_CreateButtonLabels(CodeEntryApp *app);
void CodeEntry_CreateButtonLabel(CodeEntryApp *app, int button, int x, int y, int palette);
void CodeEntry_CreatePromptWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int msgNo);
void CodeEntry_PrintPrompt(Window *window, int msgNo);

BOOL CodeEntry_Init(OverlayManager *manager, int *state) {
    CodeEntryApp *app;

    Heap_Create(HEAP_ID_3, HEAP_ID_108, 0x40000);
    app = OverlayManager_CreateAndGetData(manager, sizeof(CodeEntryApp), HEAP_ID_108);
    memset(app, 0, sizeof(CodeEntryApp));
    app->args = *(CodeEntryArgs *)OverlayManager_GetArgs(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(vu32 *)0x04000000 &= 0xFFFFE0FF;
    *(vu32 *)0x04001000 &= 0xFFFFE0FF;
    app->narc = NARC_New(NARC_a_1_9_0, HEAP_ID_108);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_108);
    app->paletteData = PaletteData_Init(HEAP_ID_108);
    PaletteData_SetAutoTransparent(app->paletteData, TRUE);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_108);
    CodeEntry_SetupBgs(app->bgConfig);
    CodeEntry_InitSpriteSystem(app);
    CodeEntry_InitLayout(app);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    CodeEntry_InitTouch(app);
    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_Create();
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_108);
    }
    G2x_SetBlendAlpha_(0x04000050, 0, 6, 0xF, 7);
    G2x_SetBlendAlpha_(0x04001050, 0, 0xE, 7, 8);
    Main_SetVBlankIntrCB((void (*)(void *))CodeEntry_VBlank, app);
    return TRUE;
}

BOOL CodeEntry_Main(OverlayManager *manager, int *state) {
    CodeEntryApp *app = OverlayManager_GetData(manager);
    BOOL done = CodeEntry_RunState(app);

    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_SetLevel(3 - ov00_021EC9D4());
    }
    if (done) {
        return TRUE;
    }
    return FALSE;
}

BOOL CodeEntry_Exit(OverlayManager *manager, int *state) {
    CodeEntryApp *app = OverlayManager_GetData(manager);

    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_Destroy();
    }
    CodeEntry_FreeGraphics(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(app->bgConfig);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(app->paletteData);
    NARC_Delete(app->narc);
    GF_TouchpadStopSampling();
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
    TouchHitboxController_Destroy(app->touchController);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_108);
    return TRUE;
}

CodeEntryArgs *CodeEntryArgs_Alloc(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon) {
    CodeEntryArgs *args = Heap_Alloc(heapID, sizeof(CodeEntryArgs));
    int i;

    args->length = length;
    args->text = String_New(length + 1, heapID);
    args->options = options;
    for (i = 0; i < 4; i++) {
        args->groupLengths[i] = groupLengths[i];
    }
    args->groupLengths[i] = groupLengths[i - 1];
    args->promptMsg = promptMsg;
    args->showWirelessIcon = showWirelessIcon;
    return args;
}

CodeEntryArgs *CodeEntryArgs_New(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon) {
    CodeEntryArgs *args = CodeEntryArgs_Alloc(heapID, length, groupLengths, options, promptMsg, showWirelessIcon);
    args->fixedGroups = 0;
    args->fixedValue = 0;
    return args;
}

CodeEntryArgs *CodeEntryArgs_NewWithFixedDigits(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon, int fixedGroups, u32 fixedValue) {
    CodeEntryArgs *args = CodeEntryArgs_Alloc(heapID, length, groupLengths, options, promptMsg, showWirelessIcon);
    args->fixedGroups = fixedGroups;
    args->fixedValue = fixedValue;
    return args;
}

void CodeEntryArgs_Delete(CodeEntryArgs *args) {
    if (args->text == NULL) {
        GF_AssertFail();
    }
    if (args == NULL) {
        GF_AssertFail();
    }
    String_Delete(args->text);
    Heap_Free(args);
}

void CodeEntry_SetupBgs(BgConfig *bgConfig) {
    GraphicsBanks banks;
    CodeEntryMainBgTemplates mainBgs;
    CodeEntrySubBgTemplates subBgs;

    GfGfx_DisableEngineAPlanes();
    banks = sCodeEntryGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (u32 *)HW_BG_VRAM, 0x80000);
    MIi_CpuClear32(0, (u32 *)HW_DB_BG_VRAM, 0x20000);
    MIi_CpuClear32(0, (u32 *)HW_OBJ_VRAM, 0x40000);
    MIi_CpuClear32(0, (u32 *)HW_DB_OBJ_VRAM, 0x20000);
    mainBgs = sCodeEntryMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainBgs.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainBgs.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainBgs.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    subBgs = sCodeEntrySubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBgs.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subBgs.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subBgs.templates[2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &subBgs.templates[3], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void CodeEntry_VBlank(CodeEntryApp *app) {
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(app->paletteData);
    DoScheduledBgGpuUpdates(app->bgConfig);
    *(u32 *)((u8 *)OS_IRQTable + 0x3FF8) |= OS_IE_V_BLANK;
}

void CodeEntry_SelectGroup(CodeEntryApp *app, int group) {
    app->previousGroup = app->selectedGroup;
    app->selectedGroup = group;
    app->selectedStart = 0;
    app->selectedEnd = 0;
    app->previousStart = 0;
    app->previousEnd = 0;
    if (app->selectedGroup != 0) {
        int index = app->selectedGroup - 1;
        app->selectedStart = app->groupRanges[index].start;
        index = app->selectedGroup - 1;
        app->selectedEnd = app->groupRanges[index].end;
    }
    if (app->previousGroup != 0) {
        int index = app->previousGroup - 1;
        app->previousStart = app->groupRanges[index].start;
        index = app->previousGroup - 1;
        app->previousEnd = app->groupRanges[index].end;
    }
}

#pragma force_active on

void CodeEntry_InitLayout(CodeEntryApp *app) {
    int i, group, j, end;
    u16 start;

    app->touchActive = TRUE;
    start = 0;
    for (i = 0; i < 5; i++) {
        app->groupRanges[i].start = start;
        start += app->args.groupLengths[i];
        app->groupRanges[i].end = start;
    }
    CodeEntry_SelectGroup(app, app->args.fixedGroups + 1);
    for (i = 0; i < 4; i++) {
        if (app->args.groupLengths[i] == 0) {
            break;
        }
        app->digitCount += app->args.groupLengths[i];
        app->separatorCount++;
    }
    app->separatorCount--;
    app->groupX[0] = 0x70 - ((app->digitCount + app->separatorCount) * 8) / 2;
    for (i = 0; i < 4; i++) {
        app->groupX[i + 1] = 0x70 - (app->separatorCount * 8 + ((app->digitCount - app->args.groupLengths[i]) * 8 + app->args.groupLengths[i] * 32)) / 2;
    }
    app->groupX[1] += 12;
    end = 0;
    for (i = 0; i < app->separatorCount; i++) {
        end += app->args.groupLengths[i];
        app->separators[i].value = end - 1;
    }
    i = 0;
    group = 0;
    do {
        for (j = 0; j < app->args.groupLengths[group]; j++) {
            app->digits[i].group = group + 1;
            i++;
        }
        group++;
    } while (i < app->digitCount);
    for (i = 0; i < app->args.fixedGroups; i++) {
        app->firstEditable += app->args.groupLengths[i];
    }
}

#pragma force_active off

void CodeEntry_SetState(CodeEntryApp *app, int state) {
    app->state = state;
    app->unk2C4 = 0;
    app->subState = 0;
    app->frame = 0;
}

BOOL CodeEntry_State_Setup(CodeEntryApp *app) {
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

BOOL CodeEntry_State_FadeOut(CodeEntryApp *app) {
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

BOOL CodeEntry_State_Input(CodeEntryApp *app) {
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

#pragma force_active on

// The digits slide to their new places; those leaving the selected group shrink, those entering it grow.
BOOL CodeEntry_State_SwitchGroup(CodeEntryApp *app) {
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

#pragma force_active off

BOOL CodeEntry_RunState(CodeEntryApp *app) {
    BOOL result = sCodeEntryStates[app->state](app);
    CodeEntry_UpdateCursors(app);
    CodeEntry_UpdateButtons(app);
    SpriteSystem_DrawSprites(app->spriteManager);
    return result;
}

#pragma force_active on

void CodeEntry_HandleKeys(CodeEntryApp *app) {
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

#pragma force_active off

// OK: empty digits become 0, all digits are appended to args.text.
void CodeEntry_Confirm(CodeEntryApp *app) {
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
void CodeEntry_Backspace(CodeEntryApp *app) {
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

#pragma force_active on

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

void CodeEntry_OnTouch(u32 index, u32 event, void *arg) {
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

#pragma force_active off

void CodeEntry_RunPendingAction(CodeEntryApp *app) {
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

void CodeEntry_ClearPendingAction(CodeEntryApp *app) {
    app->pendingAction = CODE_ENTRY_ACTION_NONE;
    app->pendingArg = 0;
    app->pendingToLast = FALSE;
}

#pragma force_active on

int CodeEntry_GetGroupFirstDigit(CodeEntryApp *app, int group) {
    int i = 0, count = app->digitCount;
    for (; i < count; i++) {
        if (app->digits[i].group == group) {
            return i;
        }
    }
    return 0;
}

int CodeEntry_GetGroupLastDigit(CodeEntryApp *app, int group) {
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

#pragma force_active off

void CodeEntry_InitSpriteSystem(CodeEntryApp *app) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;

    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_108);
    oamManagerParam = sCodeEntryOamManagerParam;
    oamCharTransferParam = sCodeEntryOamCharTransferParam;
    SpriteSystem_Init(app->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    {
        SpriteResourceCountsListUnion counts = sCodeEntrySpriteResourceCounts;
        app->spriteManager = SpriteManager_New(app->spriteSystem);
        if (!SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 0x80)) {
            GF_AssertFail();
        }
        if (!SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &counts)) {
            GF_AssertFail();
        }
    }
}

#pragma force_active on

void CodeEntry_LoadGraphics(CodeEntryApp *app) {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *paletteData;

    spriteSystem = app->spriteSystem;
    spriteManager = app->spriteManager;
    paletteData = app->paletteData;
    bgConfig = app->bgConfig;
    narc = app->narc;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 14, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_1_9_0, 13, HEAP_ID_108, PLTTBUF_MAIN_BG, 0x20, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 15, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_108);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_1_9_0, 16, HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 1, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 2, FALSE, 1000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 3, FALSE, 1000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 5, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 4, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 6, FALSE, 1001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 7, FALSE, 1001);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 9, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 8, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1002);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 10, FALSE, 1002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 11, FALSE, 1002);
    int frame = Options_GetFrame(app->args.options);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_0, 1, 10, (u8)frame, HEAP_ID_108);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0xB0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_108, PLTTBUF_SUB_BG, 0x20, 0xC0);
}

#pragma force_active off

void CodeEntry_FreeGraphics(CodeEntryApp *app) {
    int i;

    for (i = 0; i < app->digitCount; i++) {
        Sprite_DeleteAndFreeResources(app->digits[i].sprite);
    }
    for (i = 0; i < app->separatorCount; i++) {
        Sprite_DeleteAndFreeResources(app->separators[i].sprite);
    }
    for (i = 0; i < 2; i++) {
        Sprite_DeleteAndFreeResources(app->buttons[i].sprite);
    }
    for (i = 0; i < 3; i++) {
        Sprite_DeleteAndFreeResources(app->cursors[i].sprite);
    }
    CodeEntry_FreeButtonLabels(app);
    RemoveWindow(&app->promptWindow);
}

#pragma force_active on

void CodeEntry_CreateDigitSprites(CodeEntryApp *app) {
    int placed = 0;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;
    int i, x;
    CodeEntryCell *separator, *digit;
    u32 value;
    ManagedSpriteTemplate template;

    template.x = placed;
    template.y = placed;
    template.z = placed;
    template.animation = placed;
    template.drawPriority = 10;
    template.bgPriority = placed;
    template.vramTransfer = placed;
    template.pal = placed;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1000;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

    // the fixed digits show fixedValue, last digit first
    value = app->args.fixedValue;
    i = app->firstEditable - 1;
    if (i >= 0) {
        CodeEntryCell *fixed = &app->digits[i];
        do {
            fixed->value = value % 10 + 1;
            value /= 10;
            fixed--;
            i--;
        } while (i >= 0);
    }

    i = 0;
    if (app->digitCount + app->separatorCount > 0) {
        x = 76;
        separator = app->digits;
        digit = app->digits;
        do {
            // separators are counted from the start of digits[] (they follow it), so separator[16] is separators[0]
            if (app->separatorCount && i == placed + separator[16].value + 1) {
                separator[16].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetPositionXY(separator[16].sprite, x, 24);
                ManagedSprite_SetAnim(separator[16].sprite, 22);
                ManagedSprite_TickFrame(separator[16].sprite);
                separator++;
                placed++;
            } else {
                digit->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
                ManagedSprite_SetPositionXY(digit->sprite, x, 24);
                ManagedSprite_SetAnim(digit->sprite, CodeEntry_GetDigitAnim(digit->value, digit->active));
                ManagedSprite_SetAffineOverwriteMode(digit->sprite, 2);
                ManagedSprite_TickFrame(digit->sprite);
                digit++;
            }
            i++;
            x += 8;
        } while (i < app->digitCount + app->separatorCount);
    }
}

#pragma force_active off

void CodeEntry_CreateCursorSprites(CodeEntryApp *app) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1001;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1001;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    app->cursors[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    app->cursors[1].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    app->cursors[2].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    CodeEntry_MoveDigitCursor(app, app->firstEditable);
    ManagedSprite_SetAnim(app->cursors[0].sprite, 0);
    ManagedSprite_TickFrame(app->cursors[0].sprite);
    app->cursors[1].velocityX = 0;
    app->cursors[1].velocityY = 0;
    app->cursors[1].value = 1;
    CodeEntry_MoveKeyCursor(app, 0);
    ManagedSprite_SetAnim(app->cursors[1].sprite, app->cursors[1].value);
    ManagedSprite_TickFrame(app->cursors[1].sprite);
    ManagedSprite_SetOamMode(app->cursors[1].sprite, GX_OAM_MODE_XLU);
    app->cursors[2].velocityX = 0;
    app->cursors[2].velocityY = 0;
    app->cursors[2].value = 1;
    CodeEntry_MoveKeyCursor(app, 0);
    ManagedSprite_SetAnim(app->cursors[2].sprite, app->cursors[2].value);
    ManagedSprite_TickFrame(app->cursors[2].sprite);
    ManagedSprite_SetOamMode(app->cursors[2].sprite, GX_OAM_MODE_XLU);
    CodeEntry_SetCursorVisible(app, 1, FALSE);
    CodeEntry_SetCursorVisible(app, 2, FALSE);
}

typedef struct CodeEntryButtonSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    volatile int pal; // volatile: stored twice, see below
    NNS_G2D_VRAM_TYPE vram;
    int resIdList[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    int vramTransfer;
} CodeEntryButtonSpriteTemplate;

void CodeEntry_CreateButtonSprites(CodeEntryApp *app) {
    CodeEntryButtonSpriteTemplate template;
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    int noResource = (template.vram = NNS_G2D_VRAM_TYPE_2DMAIN) - 2;
    template.pal = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 1002;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = noResource;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = noResource;
    app->buttons[0].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, (ManagedSpriteTemplate *)&template);
    template.pal = 1;
    app->buttons[1].sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, (ManagedSpriteTemplate *)&template);
    ManagedSprite_SetPositionXY(app->buttons[0].sprite, (app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.right) / 2, (app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_BACK].rect.bottom) / 2);
    ManagedSprite_SetAnim(app->buttons[0].sprite, 0);
    ManagedSprite_TickFrame(app->buttons[0].sprite);
    ManagedSprite_SetPositionXY(app->buttons[1].sprite, (app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.right) / 2, (app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_OK].rect.bottom) / 2);
    ManagedSprite_SetAnim(app->buttons[1].sprite, 0);
    ManagedSprite_TickFrame(app->buttons[1].sprite);
}

void CodeEntry_SetCursorVisible(CodeEntryApp *app, int cursor, int visible) {
    if (visible == TRUE) {
        ManagedSprite_SetDrawFlag(app->cursors[cursor].sprite, TRUE);
    } else {
        ManagedSprite_SetDrawFlag(app->cursors[cursor].sprite, FALSE);
    }
}

void CodeEntry_MoveDigitCursor(CodeEntryApp *app, int digit) {
    s16 x, y;
    if (digit >= app->firstEditable) {
        ManagedSprite *sprite = app->digits[digit].sprite;
        app->cursors[0].value = digit;
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        ManagedSprite_SetPositionXY(app->cursors[0].sprite, x, y + 0x10);
    }
}

#pragma force_active on

void CodeEntry_MoveKeyCursor(CodeEntryApp *app, int key) {
    ManagedSprite_SetPositionXY(app->cursors[1].sprite, (app->hitboxes[key + 16].rect.left + app->hitboxes[key + 16].rect.right) / 2, (app->hitboxes[key + 16].rect.top + app->hitboxes[key + 16].rect.bottom) / 2);
}

void CodeEntry_MoveCursorToKey(CodeEntryApp *app, int key, int cursor) {
    ManagedSprite_SetPositionXY(app->cursors[cursor].sprite, (app->hitboxes[key + 16].rect.left + app->hitboxes[key + 16].rect.right) / 2, (app->hitboxes[key + 16].rect.top + app->hitboxes[key + 16].rect.bottom) / 2);
}

#pragma force_active off

void CodeEntry_UpdateCursors(CodeEntryApp *app) {
    int i;

    ManagedSprite_TickFrame(app->cursors[0].sprite);
    ManagedSprite_TickFrame(app->cursors[1].sprite);
    ManagedSprite_TickFrame(app->cursors[2].sprite);
    for (i = 1; i < 3; i++) {
        int anim = ManagedSprite_GetActiveAnim(app->cursors[i].sprite);
        if (anim == 3) {
            if (!ManagedSprite_IsAnimated(app->cursors[i].sprite)) {
                ManagedSprite_SetAnim(app->cursors[i].sprite, app->cursors[i].value);
                if (app->touchActive == TRUE) {
                    CodeEntry_SetCursorVisible(app, 1, FALSE);
                } else {
                    CodeEntry_SetCursorVisible(app, 1, TRUE);
                }
                CodeEntry_SetCursorVisible(app, 2, FALSE);
            }
        } else {
            if (anim != app->cursors[i].value) {
                ManagedSprite_SetAnim(app->cursors[i].sprite, app->cursors[i].value);
            }
            if (ManagedSprite_GetActiveAnim(app->cursors[2].sprite) != 3) {
                if (app->touchActive == TRUE) {
                    CodeEntry_SetCursorVisible(app, 1, FALSE);
                } else {
                    CodeEntry_SetCursorVisible(app, 1, TRUE);
                }
            }
        }
    }
}

void CodeEntry_SetButtonLabelPosition(TextOBJ *label, int x, int y) {
    if (label != NULL) {
        TextOBJ_SetPosition(label, x, y);
    }
}

// A pressed button shows its pressed frames (the label moves up by one pixel), then springs back.
void CodeEntry_UpdateButtons(CodeEntryApp *app) {
    int i;

    for (i = 0; i < 2; i++) {
        s16 x = (s16)((app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.left + app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.right) / 2) - 40;
        s16 y = (s16)((app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.top + app->hitboxes[CODE_ENTRY_HITBOX_BACK + i].rect.bottom) / 2) - 7;
        switch (app->buttons[i].value) {
        case 0:
            app->buttons[i].moveFrames = 0;
            break;
        case 1:
            app->buttons[i].moveFrames++;
            if (app->buttons[i].moveFrames == 1) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 1);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
            } else if (app->buttons[i].moveFrames == 2) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 2);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y - 1);
            } else if (app->buttons[i].moveFrames == 10) {
                ManagedSprite_SetAnim(app->buttons[i].sprite, 0);
                CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
                app->buttons[i].value++;
            }
            break;
        default:
            ManagedSprite_SetAnim(app->buttons[i].sprite, 0);
            CodeEntry_SetButtonLabelPosition(app->buttonLabels[i], x, y);
            app->buttons[i].value = 0;
            break;
        }
    }
}

int CodeEntry_GetDigitAnim(int value, int active) {
    int add = 0;
    if (active == FALSE) {
        add = 11;
    }
    return add + value;
}

void CodeEntry_MarkSelectedDigits(CodeEntryApp *app) {
    int i = 0;
    if (app->digitCount > 0) {
        CodeEntryCell *digit = app->digits;
        do {
            if (i >= app->selectedStart && i < app->selectedEnd) {
                digit->active = TRUE;
            } else {
                digit->active = FALSE;
            }
            i++;
            digit++;
        } while (i < app->digitCount);
    }
}

#pragma force_active on

// Places the digits and separators: the selected group is spread out (32 px per digit), the others are packed
// (8 px). With animate, sets the sprites moving there over 2 frames instead.
void CodeEntry_LayoutDigits(CodeEntryApp *app, int animate) {
    s16 x, y;
    int i;
    int separator;
    s16 pos;

    pos = app->groupX[app->selectedGroup];
    separator = 0;
    for (i = 0; i < app->digitCount; i++) {
        if (i >= app->selectedStart && i < app->selectedEnd) {
            if (i == app->selectedStart) {
                pos += 20;
            } else {
                pos += 32;
            }
        } else {
            if (i == 0) {
                pos += 20;
            } else {
                pos += 8;
            }
        }
        ManagedSprite_GetPositionXY(app->digits[i].sprite, &x, &y);
        if (!animate) {
            ManagedSprite_SetPositionXY(app->digits[i].sprite, pos, y);
        } else {
            app->digits[i].velocityX = (pos - x) / 2;
            app->digits[i].velocityY = 0;
            app->digits[i].moveFrames = 2;
            app->digits[i].animStep = 0;
        }
        if (i == app->separators[separator].value && separator != app->separatorCount) {
            ManagedSprite_GetPositionXY(app->separators[separator].sprite, &x, &y);
            if (app->selectedStart == app->selectedEnd) {
                pos += 8;
            } else if (i > app->selectedStart && i < app->selectedEnd) {
                pos += 20;
            } else {
                pos += 8;
            }
            if (!animate) {
                ManagedSprite_SetPositionXY(app->separators[separator].sprite, pos, y);
            } else {
                app->separators[separator].velocityX = (pos - x) / 2;
                app->separators[separator].velocityY = 0;
                app->separators[separator].moveFrames = 2;
            }
            separator++;
        }
    }
}

#pragma force_active off

void CodeEntry_UpdateDigitHitboxes(CodeEntryApp *app) {
    int i = 0;
    if (app->digitCount > 0) {
        CodeEntryCell *digit = app->digits;
        do {
            int halfWidth;
            int halfHeight;
            s16 y, x;
            if (i >= app->selectedStart && i < app->selectedEnd) {
                halfWidth = 16;
                halfHeight = 16;
            } else {
                halfWidth = 4;
                halfHeight = 8;
            }
            ManagedSprite_GetPositionXY(digit->sprite, &x, &y);
            digit->hitbox->rect.top = y - halfHeight;
            digit->hitbox->rect.left = x - halfWidth;
            digit->hitbox->rect.bottom = y + halfHeight;
            digit->hitbox->rect.right = x + halfWidth;
            i++;
            digit++;
        } while (i < app->digitCount);
    }
}

void CodeEntry_InitFont(CodeEntryApp *app) {
    app->fontSystem = FontSystem_NewInit(2, HEAP_ID_108);
    FontID_Alloc(2, HEAP_ID_108);
}

void CodeEntry_FreeButtonLabels(CodeEntryApp *app) {
    FontID_Release(2);
    FontOAM_Delete(app->buttonLabels[0]);
    sub_02021B5C(&app->buttonLabelAllocs[0].charAlloc);
    FontOAM_Delete(app->buttonLabels[1]);
    sub_02021B5C(&app->buttonLabelAllocs[1].charAlloc);
    FontSystem_Delete(app->fontSystem);
}

void CodeEntry_LoadButtonLabelPalette(CodeEntryApp *app) {
    SpriteSystem_LoadPaletteBuffer(app->paletteData, PLTTBUF_MAIN_OBJ, app->spriteSystem, app->spriteManager, NARC_graphic_font, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1003);
}

void CodeEntry_CreateButtonLabels(CodeEntryApp *app) {
    CodeEntry_LoadButtonLabelPalette(app);
    CodeEntry_CreateButtonLabel(app, 0, 0x4E, 0xA5, 0);
    CodeEntry_CreateButtonLabel(app, 1, 0xAC, 0xA5, 0);
}

#pragma force_active on

void CodeEntry_CreateButtonLabel(CodeEntryApp *app, int button, int x, int y, int palette) {
    TextOBJTemplate template;
    Window window;
    MsgData *msgData;
    String *string;
    s16 centerX, centerY;
    int size;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0038_bin, HEAP_ID_108);
    string = NewString_ReadMsgData(msgData, button + msg_0038_00002);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(app->bgConfig, &window, 10, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 2, string, FontID_String_GetCenterAlignmentX(2, string, 0, 80), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 2), NULL);
    size = FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_108);
    sub_02021AC8(size, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &app->buttonLabelAllocs[button].charAlloc);
    centerX = (app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.left + app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.right) / 2;
    centerY = (app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.top + app->hitboxes[button + CODE_ENTRY_HITBOX_BACK].rect.bottom) / 2;
    template.fontSystem = app->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(app->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(app->spriteManager, 1003);
    template.sprite = NULL;
    template.offset = app->buttonLabelAllocs[button].charAlloc.offset;
    template.x = (s16)(centerX - 40);
    template.y = (s16)(centerY - 7);
    template.unk_20 = 0;
    template.unk_24 = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_108;
    app->buttonLabels[button] = FontOAM_Create(&template);
    TextOBJ_SetPalOffsetRespectVramOffset(app->buttonLabels[button], palette);
    String_Delete(string);
    DestroyMsgData(msgData);
    RemoveWindow(&window);
}

#pragma force_active off

void CodeEntry_CreatePromptWindow(BgConfig *bgConfig, Window *window, int bgId, int x, int y, int width, int height, int baseTile, int msgNo) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, (u8)bgId, (u8)x, (u8)y, (u8)width, height, 12, (u16)baseTile);
    DrawFrameAndWindow2(window, TRUE, 1, 11);
    FillWindowPixelBuffer(window, 15);
    CopyWindowToVram(window);
    CodeEntry_PrintPrompt(window, msgNo);
}

void CodeEntry_PrintPrompt(Window *window, int msgNo) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0038_bin, HEAP_ID_108);
    string = NewString_ReadMsgData(msgData, msgNo);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 1, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    CopyWindowToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
}
