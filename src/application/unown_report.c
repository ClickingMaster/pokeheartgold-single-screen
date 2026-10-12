#include "unown_report.h"

#include "global.h"

#include "constants/species.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "sys_vars.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Unown Report: a book listing the Unown forms the player has seen (seven icons per page, two
// pages per spread) after a cover page, followed by research pages unlocked by the report level.

#define UNOWN_ICONS_PER_SPREAD 14

enum UnownReportWindow {
    UNOWN_REPORT_WIN_TITLE,
    UNOWN_REPORT_WIN_LEFT_PAGE,
    UNOWN_REPORT_WIN_RIGHT_PAGE,
    UNOWN_REPORT_WIN_FOOTER,
    UNOWN_REPORT_WIN_COUNT,
};

enum UnownReportSprite {
    UNOWN_REPORT_SPRITE_BOOK,
    UNOWN_REPORT_SPRITE_PREV_ARROW,
    UNOWN_REPORT_SPRITE_NEXT_ARROW,
    UNOWN_REPORT_SPRITE_COUNT,
};

enum UnownReportMainState {
    UNOWN_REPORT_MAIN_FADE_IN,
    UNOWN_REPORT_MAIN_FADE_OUT,
    UNOWN_REPORT_MAIN_INPUT,
    UNOWN_REPORT_MAIN_TURN_PAGE,
    UNOWN_REPORT_MAIN_EXIT,
};

typedef struct UnownReportScreen {
    void *file;
    NNSG2dScreenData *screenData;
    void *rawData;
    u8 width; // in tiles
    u8 height;
} UnownReportScreen;

typedef struct UnownReportApp {
    enum HeapID heapID;
    UnownReportArgs *args;
    u32 unk_08;
    Pokedex *pokedex;
    u16 subState;
    MenuInputState menuInputState;
    u8 textFrameDelay;
    u8 frame;
    u8 page;
    u8 numPages;
    u8 numFormSpreads;
    u8 numInfoPages;
    u8 unk_1E;
    u8 numSeenForms;
    u8 seenForms[28];
    u8 reportLevel;
    BgConfig *bgConfig;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *template;
    String *buffer;
    String *title;
    String *pageNumberTemplate;
    String *playerName;
    u8 unk_60[8];
    Window windows[UNOWN_REPORT_WIN_COUNT];
    YesNoPrompt *yesNoPrompt;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    SpriteManager *iconManager;
    Sprite *sprites[UNOWN_REPORT_SPRITE_COUNT];
    ManagedSprite *icons[UNOWN_ICONS_PER_SPREAD];
    NARC *iconNarc;
    UnownReportScreen mainScreens[3]; // cover, Unown forms, research pages
    UnownReportScreen subScreens[3];
} UnownReportApp;

// Research text of one info page: two blocks of message lines, each shown once the report
// level reaches its threshold.
typedef struct UnownReportTextBlock {
    u16 firstMsg : 6;
    u16 numMsgs : 2;
    u16 minLevel : 8;
} UnownReportTextBlock;

typedef struct UnownReportInfoPage {
    UnownReportTextBlock left;
    UnownReportTextBlock right;
} UnownReportInfoPage;

void ov113_021E59F8();
int UnownReport_GetSeenPunctuationForms(Pokedex *pokedex, u8 *forms);
void UnownReport_InitData(UnownReportApp *app);
void UnownReport_RestoreMenuInputState(UnownReportApp *app);
BOOL UnownReport_SetupGraphics(UnownReportApp *app);
BOOL UnownReport_Teardown(UnownReportApp *app);
int UnownReport_FadeIn(UnownReportApp *app);
int UnownReport_FadeOut(UnownReportApp *app);
int UnownReport_HandleInput(UnownReportApp *app);
int UnownReport_TurnPage(UnownReportApp *app);
void UnownReport_CreateGraphics(UnownReportApp *app);
void UnownReport_DestroyGraphics(UnownReportApp *app);
void UnownReport_DrawFirstPage(UnownReportApp *app);
int UnownReport_ProcessInput(UnownReportApp *app);
u8 UnownReport_GetNumInfoPages(UnownReportApp *app);
void UnownReport_DrawPage(UnownReportApp *app, int unused);
void UnownReport_DrawTitle(UnownReportApp *app, int bookAnimSeq);
BOOL UnownReport_IsBookAnimating(UnownReportApp *app);
int UnownReport_HandleButton(UnownReportApp *app, u32 button);
int UnownReport_HandleKeys(UnownReportApp *app);
int UnownReport_HandleTouch(UnownReportApp *app, BOOL *touched);
void UnownReport_DrawCoverPage(UnownReportApp *app);
void UnownReport_DrawFormSpread(UnownReportApp *app, u8 spread);
void UnownReport_DrawInfoPage(UnownReportApp *app, u8 infoPage);
#pragma force_active reset

void UnownReport_ShowScreen(UnownReportApp *app, UnownReportScreen *screen, u32 bgId);
void UnownReport_PrintText(Window *window, String *string, FontID fontId, int x, u16 y);
void UnownReport_PrintTextFont0(Window *window, String *string, int x, u16 y);
void UnownReport_VBlankCallback(void *data);
void UnownReport_SetVramBanks(UnownReportApp *app);
void UnownReport_InitBgs(UnownReportApp *app);
void UnownReport_FreeBgs(UnownReportApp *app);
void UnownReport_LoadScreen(UnownReportScreen *screen, NARC *narc, int member, enum HeapID heapID);
void UnownReport_LoadBgGraphics(UnownReportApp *app);
void UnownReport_FreeScreens(UnownReportApp *app);
void UnownReport_InitMessages(UnownReportApp *app);
void UnownReport_FreeMessages(UnownReportApp *app);
void UnownReport_InitWindows(UnownReportApp *app);
void UnownReport_FreeWindows(UnownReportApp *app);
void UnownReport_InitSpriteSystem(UnownReportApp *app);
void UnownReport_FreeSpriteSystem(UnownReportApp *app);
void UnownReport_CreateSprites(UnownReportApp *app);
void UnownReport_DestroySprites(UnownReportApp *app);
void UnownReport_CreateSpriteObjects(UnownReportApp *app);
void UnownReport_DeleteSpriteObjects(UnownReportApp *app);
void UnownReport_LoadIcon(UnownReportApp *app, int icon, int form);
void UnownReport_UpdateArrows(UnownReportApp *app);

extern const ResdatIdList ov113_021E6B74;
extern const TouchscreenHitbox ov113_021E6B82[];
extern const GraphicsModes ov113_021E6B94;
extern const OamCharTransferParam ov113_021E6BA4;
extern const SpriteResourceCountsListUnion ov113_021E6BB8;
extern const UnownReportInfoPage ov113_021E6BD0[];
extern const BgTemplate ov113_021E6BE8;
extern const BgTemplate ov113_021E6C04;
extern const BgTemplate ov113_021E6C20;
extern const BgTemplate ov113_021E6C3C;
extern const BgTemplate ov113_021E6C58;
extern const BgTemplate ov113_021E6C74;
extern const WindowTemplate ov113_021E6C90[];
extern const OamManagerParam ov113_021E6CB0;
extern const GraphicsBanks ov113_021E6CD0;
extern const UnmanagedSpriteTemplate ov113_021E6CF8[];

#pragma force_active on
BOOL UnownReport_Init(OverlayManager *man, int *state) {
    UnownReportApp *app;

    switch (*state) {
    case 0:
        ov113_021E59F8(man);
        Heap_Create(HEAP_ID_3, HEAP_ID_152, 0x20000);
        app = OverlayManager_CreateAndGetData(man, sizeof(UnownReportApp), HEAP_ID_152);
        MI_CpuFill8(app, 0, sizeof(UnownReportApp));
        app->heapID = HEAP_ID_152;
        app->args = OverlayManager_GetArgs(man);
        UnownReport_InitData(app);
        (*state)++;
        break;
    case 1:
        if (UnownReport_SetupGraphics(OverlayManager_GetData(man))) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL UnownReport_Main(OverlayManager *man, int *state) {
    UnownReportApp *app = OverlayManager_GetData(man);

    switch (*state) {
    case UNOWN_REPORT_MAIN_FADE_IN:
        *state = UnownReport_FadeIn(app);
        break;
    case UNOWN_REPORT_MAIN_FADE_OUT:
        *state = UnownReport_FadeOut(app);
        break;
    case UNOWN_REPORT_MAIN_INPUT:
        *state = UnownReport_HandleInput(app);
        break;
    case UNOWN_REPORT_MAIN_TURN_PAGE:
        *state = UnownReport_TurnPage(app);
        break;
    case UNOWN_REPORT_MAIN_EXIT:
        return TRUE;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    SpriteSystem_DrawSprites(app->iconManager);
    return FALSE;
}
#pragma force_active reset

BOOL UnownReport_Exit(OverlayManager *man, int *state) {
    UnownReportApp *app = OverlayManager_GetData(man);

    if (!UnownReport_Teardown(app)) {
        return FALSE;
    }
    ov113_021E59F8();
    UnownReport_RestoreMenuInputState(app);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_152);
    return TRUE;
}

// Called with the overlay manager from UnownReport_Init and without arguments from UnownReport_Exit
void ov113_021E59F8(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

// The "!" and "?" forms (26 and 27)
int UnownReport_GetSeenPunctuationForms(Pokedex *pokedex, u8 *forms) {
    int count = 0;
    int i;

    for (i = 0; i < 28; i++) {
        u8 form = Pokedex_GetSeenFormByIdx_Unown(pokedex, i, TRUE);
        if ((u8)(form + 230) <= 1) { // form - 26 <= 1
            forms[count++] = form;
        }
    }
    return count;
}

#pragma force_active on
void UnownReport_InitData(UnownReportApp *app) {
    UnownReportArgs *args = app->args;
    Options *options;
    int i;

    app->menuInputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->textFrameDelay = Options_GetTextFrameDelay(options);
    app->frame = Options_GetFrame(options);
    app->pokedex = Save_Pokedex_Get(app->args->saveData);
    app->reportLevel = Save_VarsFlags_GetUnownReportLevel(Save_VarsFlags_Get(app->args->saveData));
    app->numInfoPages = UnownReport_GetNumInfoPages(app);
    app->numSeenForms = Pokedex_GetSeenFormNum_Unown(app->pokedex, TRUE);
    if (app->reportLevel >= 4) {
        u8 punctuation[4];
        u8 count;

        for (i = 0; i <= 25; i++) {
            app->seenForms[i] = i;
        }
        count = (u8)UnownReport_GetSeenPunctuationForms(app->pokedex, punctuation);
        for (i = 0; i < count; i++) {
            app->seenForms[26 + i] = punctuation[i];
        }
    } else {
        for (i = 0; i < app->numSeenForms; i++) {
            app->seenForms[i] = Pokedex_GetSeenFormByIdx_Unown(app->pokedex, i, TRUE);
        }
    }
    app->numFormSpreads = (int)app->numSeenForms / UNOWN_ICONS_PER_SPREAD;
    if (app->numSeenForms && (int)app->numSeenForms % UNOWN_ICONS_PER_SPREAD) {
        app->numFormSpreads++;
    }
    app->numPages = app->numFormSpreads + app->numInfoPages + 1;
}
#pragma force_active reset

void UnownReport_RestoreMenuInputState(UnownReportApp *app) {
    UnownReportArgs *args = app->args;
    MenuInputStateMgr_SetState(args->menuInputStatePtr, app->menuInputState);
}

BOOL UnownReport_SetupGraphics(UnownReportApp *app) {
    switch (app->subState) {
    case 0:
        UnownReport_CreateGraphics(app);
        app->subState++;
        break;
    case 1:
        UnownReport_DrawFirstPage(app);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        Sound_SetSceneAndPlayBGM(69, 0, 0);
        app->subState = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL UnownReport_Teardown(UnownReportApp *app) {
    GF_TouchpadStopSampling();
    UnownReport_DestroyGraphics(app);
    return TRUE;
}

#pragma force_active on
int UnownReport_FadeIn(UnownReportApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->subState = 0;
            return UNOWN_REPORT_MAIN_INPUT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_FADE_IN;
}

int UnownReport_FadeOut(UnownReportApp *app) {
    switch (app->subState) {
    case 0:
        if (UnownReport_IsBookAnimating(app) == FALSE) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, app->heapID);
            app->subState++;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            app->subState = 0;
            return UNOWN_REPORT_MAIN_EXIT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_FADE_OUT;
}
#pragma force_active reset

int UnownReport_HandleInput(UnownReportApp *app) {
    return UnownReport_ProcessInput(app);
}

#pragma force_active on
int UnownReport_TurnPage(UnownReportApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UPWARD_OUT, FADE_TYPE_DOWNWARD_OUT, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            UnownReport_DrawPage(app, app->page);
            app->subState++;
        }
        break;
    case 2:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_UNK_5, RGB_BLACK, 6, 1, app->heapID);
        app->subState++;
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            app->subState = 0;
            return UNOWN_REPORT_MAIN_INPUT;
        }
        break;
    }
    return UNOWN_REPORT_MAIN_TURN_PAGE;
}
#pragma force_active reset

void UnownReport_CreateGraphics(UnownReportApp *app) {
    UnownReport_InitBgs(app);
    UnownReport_LoadBgGraphics(app);
    UnownReport_InitMessages(app);
    UnownReport_InitWindows(app);
    UnownReport_CreateSprites(app);
    Main_SetVBlankIntrCB(UnownReport_VBlankCallback, app);
}

void UnownReport_DestroyGraphics(UnownReportApp *app) {
    UnownReport_DestroySprites(app);
    UnownReport_FreeWindows(app);
    UnownReport_FreeMessages(app);
    UnownReport_FreeScreens(app);
    UnownReport_FreeBgs(app);
}

void UnownReport_DrawFirstPage(UnownReportApp *app) {
    UnownReport_DrawTitle(app, 0);
    UnownReport_DrawPage(app, app->page);
}

#pragma force_active on
int UnownReport_ProcessInput(UnownReportApp *app) {
    BOOL touched = FALSE;
    int nextState = UnownReport_HandleTouch(app, &touched);

    if (!touched) {
        nextState = UnownReport_HandleKeys(app);
    }
    return nextState;
}
#pragma force_active reset

u8 UnownReport_GetNumInfoPages(UnownReportApp *app) {
    u8 level = app->reportLevel;
    int i;

    for (i = 0; i < 6; i++) {
        if ((s32)(((u32)((const u16(*)[2])ov113_021E6BD0)[i][0] << 16) >> 24) > (s32)level) { // left.minLevel
            break;
        }
    }
    return i;
}

void UnownReport_DrawPage(UnownReportApp *app, int unused) {
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], 0);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], 0);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_FOOTER], 0);
    if (app->page == 0) {
        UnownReport_DrawCoverPage(app);
    } else if (app->page - 1 < app->numFormSpreads) {
        UnownReport_DrawFormSpread(app, app->page - 1);
    } else {
        UnownReport_DrawInfoPage(app, app->page - (app->numFormSpreads + 1));
    }
    BufferIntegerAsString(app->msgFormat, 0, app->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(app->msgFormat, 1, app->numPages, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->buffer, app->pageNumberTemplate);
    AddTextPrinterParameterizedWithColor(&app->windows[UNOWN_REPORT_WIN_FOOTER], 0, app->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE]);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE]);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_FOOTER]);
    UnownReport_UpdateArrows(app);
}

void UnownReport_DrawTitle(UnownReportApp *app, int bookAnimSeq) {
    u32 x;
    u32 width;

    Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_BOOK], bookAnimSeq);
    Sprite_ResetAnimCtrlState(app->sprites[UNOWN_REPORT_SPRITE_BOOK]);
    FillWindowPixelBuffer(&app->windows[UNOWN_REPORT_WIN_TITLE], 0);
    width = FontID_String_GetWidth(4, app->title, 0);
    x = (48 - width) / 2;
    AddTextPrinterParameterizedWithColor(&app->windows[UNOWN_REPORT_WIN_TITLE], 4, app->title, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 15, 0), NULL);
    ScheduleWindowCopyToVram(&app->windows[UNOWN_REPORT_WIN_TITLE]);
}

BOOL UnownReport_IsBookAnimating(UnownReportApp *app) {
    return Sprite_IsAnimated(app->sprites[UNOWN_REPORT_SPRITE_BOOK]);
}

// button: 0 close, 1 previous page, 2 next page (also the touch hitbox index)
int UnownReport_HandleButton(UnownReportApp *app, u32 button) {
    switch (button) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        UnownReport_DrawTitle(app, 1);
        return UNOWN_REPORT_MAIN_FADE_OUT;
    case 1:
        if (app->page == 0) {
            return UNOWN_REPORT_MAIN_INPUT;
        }
        app->page--;
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 3);
        PlaySE(SEQ_SE_DP_SELECT);
        return UNOWN_REPORT_MAIN_TURN_PAGE;
    case 2:
        if (app->page >= app->numPages - 1) {
            return UNOWN_REPORT_MAIN_INPUT;
        }
        app->page++;
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 6);
        PlaySE(SEQ_SE_DP_SELECT);
        return UNOWN_REPORT_MAIN_TURN_PAGE;
    default:
        return UNOWN_REPORT_MAIN_INPUT;
    }
}

int UnownReport_HandleKeys(UnownReportApp *app) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN | PAD_BUTTON_X | PAD_BUTTON_Y)) {
        app->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        return UnownReport_HandleButton(app, 0);
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        return UnownReport_HandleButton(app, 1);
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        return UnownReport_HandleButton(app, 2);
    }
    return UNOWN_REPORT_MAIN_INPUT;
}

int UnownReport_HandleTouch(UnownReportApp *app, BOOL *touched) {
    int hitbox = TouchscreenHitbox_FindHitboxAtTouchNew(ov113_021E6B82);

    if (hitbox == -1) {
        return UNOWN_REPORT_MAIN_INPUT;
    }
    *touched = TRUE;
    app->menuInputState = MENU_INPUT_STATE_TOUCH;
    return UnownReport_HandleButton(app, hitbox);
}

void UnownReport_DrawCoverPage(UnownReportApp *app) {
    s32 i;
    u32 width;

    UnownReport_ShowScreen(app, &app->subScreens[0], GF_BG_LYR_SUB_2);
    UnownReport_ShowScreen(app, &app->mainScreens[0], GF_BG_LYR_MAIN_2);
    for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
        ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
    }
    ReadMsgDataIntoString(app->msgData, 1, app->buffer);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 0, 56);
    width = FontID_String_GetWidth(0, app->playerName, 0);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->playerName, (u16)(176 - width), 72);
    ReadMsgDataIntoString(app->msgData, 3, app->buffer);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 60);
    ReadMsgDataIntoString(app->msgData, 4, app->template);
    BufferIntegerAsString(app->msgFormat, 0, app->numSeenForms, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->buffer, app->template);
    UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 76);
}

#pragma force_active on
void UnownReport_DrawFormSpread(UnownReportApp *app, u8 spread) {
    Window *rightWindow;
    Window *leftWindow;
    int first;
    UnownReportApp *slot; // moves by one icon pointer per iteration, so slot->icons[0] is app->icons[i]
    int i;
    int y;

    UnownReport_ShowScreen(app, &app->subScreens[1], GF_BG_LYR_SUB_2);
    UnownReport_ShowScreen(app, &app->mainScreens[1], GF_BG_LYR_MAIN_2);
    first = UNOWN_ICONS_PER_SPREAD * spread;
    rightWindow = &app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE];
    i = 0;
    leftWindow = (Window *)app;
    slot = app;
    y = 0;
    leftWindow = (Window *)((u8 *)leftWindow + offsetof(UnownReportApp, windows[UNOWN_REPORT_WIN_LEFT_PAGE]));
    for (; i < UNOWN_ICONS_PER_SPREAD; i++, slot = (UnownReportApp *)((u8 *)slot + sizeof(ManagedSprite *)), y += 24) {
        int index = first + i;

        if (index >= app->numSeenForms) {
            ManagedSprite_SetDrawFlag(slot->icons[0], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(slot->icons[0], TRUE);
            UnownReport_LoadIcon(app, i, app->seenForms[index]);
            ReadMsgDataIntoString(app->msgData, app->seenForms[index] + 20, app->buffer);
            if (i < 7) {
                UnownReport_PrintTextFont0(leftWindow, app->buffer, 48, (u16)y);
            } else {
                UnownReport_PrintTextFont0(rightWindow, app->buffer, 48, (u16)(y - 168));
            }
        }
    }
}

void UnownReport_DrawInfoPage(UnownReportApp *app, u8 infoPage) {
    const UnownReportInfoPage *page;
    int i;

    if (infoPage == 0) {
        UnownReport_ShowScreen(app, &app->subScreens[2], GF_BG_LYR_SUB_2);
        UnownReport_ShowScreen(app, &app->mainScreens[2], GF_BG_LYR_MAIN_2);
        for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
            ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
        }
    }
    page = &ov113_021E6BD0[infoPage];
    if (page->left.minLevel <= app->reportLevel) {
        for (i = 0; i < page->left.numMsgs; i++) {
            ReadMsgDataIntoString(app->msgData, page->left.firstMsg + i, app->buffer);
            if (page->left.firstMsg + i == 15) {
                UnownReport_PrintText(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 3, 4, 0);
            } else {
                UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_LEFT_PAGE], app->buffer, 4, 24);
            }
        }
    }
    if (page->right.minLevel <= app->reportLevel) {
        for (i = 0; i < page->right.numMsgs; i++) {
            ReadMsgDataIntoString(app->msgData, page->right.firstMsg + i, app->buffer);
            UnownReport_PrintTextFont0(&app->windows[UNOWN_REPORT_WIN_RIGHT_PAGE], app->buffer, 4, 24);
        }
    }
}

#pragma force_active reset

void UnownReport_ShowScreen(UnownReportApp *app, UnownReportScreen *screen, u32 bgId) {
    CopyToBgTilemapRect(app->bgConfig, (u8)bgId, 0, 0, 32, 24, screen->rawData, 0, 0, screen->width, screen->height);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, (u8)bgId);
}

void UnownReport_PrintText(Window *window, String *string, FontID fontId, int x, u16 y) {
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

void UnownReport_PrintTextFont0(Window *window, String *string, int x, u16 y) {
    UnownReport_PrintText(window, string, 0, x, y);
}

#pragma force_active on
void UnownReport_VBlankCallback(void *data) {
    vu32 *dtcm = (vu32 *)0x027E0000;
    UnownReportApp *app = data;

    if (app->spriteSystem) {
        SpriteSystem_TransferOam();
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(app->bgConfig);
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}
#pragma force_active reset

void UnownReport_SetVramBanks(UnownReportApp *app) {
    GraphicsBanks banks = ov113_021E6CD0;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void UnownReport_InitBgs(UnownReportApp *app) {
    GraphicsModes modes;
    BgTemplate subBg1;
    BgTemplate subBg2;
    BgTemplate subBg3;
    BgTemplate mainBg1;
    BgTemplate mainBg2;
    BgTemplate mainBg3;

    UnownReport_SetVramBanks(app);
    app->bgConfig = BgConfig_Alloc(app->heapID);
    modes = ov113_021E6B94;
    SetBothScreensModesAndDisable(&modes);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    subBg1 = ov113_021E6C3C;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_1, &subBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    subBg2 = ov113_021E6C74;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &subBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);
    subBg3 = ov113_021E6BE8;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &subBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
    mainBg1 = ov113_021E6C04;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &mainBg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    mainBg2 = ov113_021E6C20;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &mainBg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    mainBg3 = ov113_021E6C58;
    InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &mainBg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, app->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, app->heapID);
}

void UnownReport_FreeBgs(UnownReportApp *app) {
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(app->bgConfig);
    {
        u32 powcnt = reg_GX_POWCNT;
        powcnt |= 0x8000;
        reg_GX_POWCNT = powcnt;
    }
}
#pragma force_active reset

void UnownReport_LoadScreen(UnownReportScreen *screen, NARC *narc, int member, enum HeapID heapID) {
    screen->file = GfGfxLoader_GetScrnDataFromOpenNarc(narc, member, TRUE, &screen->screenData, heapID);
    screen->width = (u32)screen->screenData->screenWidth >> 3;
    screen->height = (u32)screen->screenData->screenHeight >> 3;
    screen->rawData = screen->screenData->rawData;
}

#pragma force_active on
void UnownReport_LoadBgGraphics(UnownReportApp *app) {
    NARC *narc = NARC_New(NARC_a_2_3_9, app->heapID);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, app->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, app->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, app->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, app->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[0], narc, 10, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[1], narc, 12, app->heapID);
    UnownReport_LoadScreen(&app->subScreens[2], narc, 14, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[0], narc, 11, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[1], narc, 13, app->heapID);
    UnownReport_LoadScreen(&app->mainScreens[2], narc, 15, app->heapID);
    NARC_Delete(narc);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_1, 1, 13, app->frame, app->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, app->heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, app->heapID);
}
#pragma force_active reset

void UnownReport_FreeScreens(UnownReportApp *app) {
    Heap_Free(app->mainScreens[2].file);
    Heap_Free(app->mainScreens[1].file);
    Heap_Free(app->mainScreens[0].file);
    Heap_Free(app->subScreens[2].file);
    Heap_Free(app->subScreens[1].file);
    Heap_Free(app->subScreens[0].file);
}

#pragma force_active on
void UnownReport_InitMessages(UnownReportApp *app) {
    FontID_Alloc(4, app->heapID);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0001_bin, app->heapID);
    app->msgFormat = MessageFormat_New_Custom(16, 16, app->heapID);
    app->template = String_New(512, app->heapID);
    app->buffer = String_New(512, app->heapID);
    app->title = NewString_ReadMsgData(app->msgData, 0);
    app->pageNumberTemplate = NewString_ReadMsgData(app->msgData, 5);
    app->playerName = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(app->args->saveData), app->heapID);
}
#pragma force_active reset

void UnownReport_FreeMessages(UnownReportApp *app) {
    String_Delete(app->playerName);
    String_Delete(app->pageNumberTemplate);
    String_Delete(app->title);
    String_Delete(app->buffer);
    String_Delete(app->template);
    MessageFormat_Delete(app->msgFormat);
    DestroyMsgData(app->msgData);
    FontID_Release(4);
}

#pragma force_active on
void UnownReport_InitWindows(UnownReportApp *app) {
    const WindowTemplate *template = ov113_021E6C90;
    Window *window;
    int i;

    i = 0;
    window = app->windows;
    for (; i < UNOWN_REPORT_WIN_COUNT; i++, template ++, window++) {
        AddWindow(app->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
    }
    app->yesNoPrompt = YesNoPrompt_Create(app->heapID);
}
#pragma force_active reset

void UnownReport_FreeWindows(UnownReportApp *app) {
    int i;
    Window *window;

    i = 0;
    window = app->windows;
    for (; i < UNOWN_REPORT_WIN_COUNT; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    YesNoPrompt_Destroy(app->yesNoPrompt);
}

#pragma force_active on
void UnownReport_InitSpriteSystem(UnownReportApp *app) {
    SpriteResourceCountsListUnion capacities;
    int i;

    GF_CreateVramTransferManager(32, app->heapID);
    app->spriteSystem = SpriteSystem_Alloc(app->heapID);
    SpriteSystem_Init(app->spriteSystem, &ov113_021E6CB0, &ov113_021E6BA4, 2);
    thunk_ClearMainOAM(app->heapID);
    thunk_ClearSubOAM(app->heapID);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 17);
    sub_0200D2A4(app->spriteSystem, app->spriteManager, &ov113_021E6B74, 0, 0);
    capacities = ov113_021E6BB8;
    app->iconManager = SpriteManager_New(app->spriteSystem);
    SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->iconManager, &capacities);
    SpriteManager_SetSpriteList(app->iconManager, SpriteManager_GetSpriteList(app->spriteManager));
    app->iconNarc = NARC_New(NARC_poketool_icongra_poke_icon, app->heapID);
    SpriteSystem_LoadPlttResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconPlttNaix(), FALSE, 1, 3, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconStaticCellNaix(), FALSE, 1);
    SpriteSystem_LoadAnimResObj(app->spriteSystem, app->iconManager, NARC_poketool_icongra_poke_icon, GetMonIconStaticAnimNaix(), FALSE, 1);
    for (i = 0; i < 7; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconNaixEx(SPECIES_UNOWN, FALSE, i), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, i + 1);
    }
    for (i = 7; i < UNOWN_ICONS_PER_SPREAD; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->iconManager, app->iconNarc, GetMonIconNaixEx(SPECIES_UNOWN, FALSE, i), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i + 1);
    }
}
#pragma force_active reset

void UnownReport_FreeSpriteSystem(UnownReportApp *app) {
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->iconManager);
    SpriteSystem_DestroySpriteManager(app->spriteSystem, app->spriteManager);
    app->iconManager = NULL;
    app->spriteManager = NULL;
    NARC_Delete(app->iconNarc);
    SpriteSystem_Free(app->spriteSystem);
    app->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(app->heapID);
}

void UnownReport_CreateSprites(UnownReportApp *app) {
    UnownReport_InitSpriteSystem(app);
    UnownReport_CreateSpriteObjects(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void UnownReport_DestroySprites(UnownReportApp *app) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    UnownReport_DeleteSpriteObjects(app);
    UnownReport_FreeSpriteSystem(app);
}

void UnownReport_CreateSpriteObjects(UnownReportApp *app) {
    ManagedSpriteTemplate template;
    int i;
    int y;
    int upperY;

    for (i = 0; i < 3; i++) {
        app->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(app->spriteSystem, app->spriteManager, &ov113_021E6CF8[i]);
        Sprite_SetDrawFlag(app->sprites[i], TRUE);
        Sprite_SetAnimActiveFlag(app->sprites[i], TRUE);
        Sprite_SetPriority(app->sprites[i], 2);
    }
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgPriority = 1;
    {
        int pal = GetMonIconPaletteEx(SPECIES_UNOWN, 0, FALSE);
        i = 0;
        template.pal = pal;
    }
    template.vramTransfer = i;
    template.x = 40;
    template.resIdList[1] = 1;
    template.resIdList[2] = 1;
    template.resIdList[3] = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    upperY = 29;
    y = i;
    for (; i < UNOWN_ICONS_PER_SPREAD; i++) {
        if (i < 7) {
            template.y = upperY;
            template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        } else {
            template.y = y - 155;
            template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        }
        template.resIdList[0] = i + 1;
        app->icons[i] = SpriteSystem_NewSprite(app->spriteSystem, app->iconManager, &template);
        ManagedSprite_SetDrawFlag(app->icons[i], FALSE);
        upperY += 24;
        y += 24;
    }
}

void UnownReport_DeleteSpriteObjects(UnownReportApp *app) {
    int i;

    for (i = 0; i < UNOWN_ICONS_PER_SPREAD; i++) {
        if (app->icons[i] != NULL) {
            Sprite_DeleteAndFreeResources(app->icons[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (app->sprites[i] != NULL) {
            Sprite_Delete(app->sprites[i]);
        }
    }
}

// The icon graphics of slot `icon` are replaced with those of `form`
void UnownReport_LoadIcon(UnownReportApp *app, int icon, int form) {
    u32 fileId = GetMonIconNaixEx(SPECIES_UNOWN, FALSE, form);
    sub_0200E2B8(app->spriteSystem, app->iconManager, app->iconNarc, fileId, FALSE, icon + 1);
}

void UnownReport_UpdateArrows(UnownReportApp *app) {
    if (app->numPages == 1 || app->page == 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 4);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_PREV_ARROW], 2);
    }
    if (app->numPages == 1 || app->page == app->numPages - 1) {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 7);
    } else {
        Sprite_SetAnimCtrlSeq(app->sprites[UNOWN_REPORT_SPRITE_NEXT_ARROW], 5);
    }
}
