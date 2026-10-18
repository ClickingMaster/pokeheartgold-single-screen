#include "global.h"

#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "gts/gts_internal.h"
#include "msgdata/msg/msg_0775.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_storage_system.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// Choosing a Pokemon (screen 5): the party and the PC boxes are shown one at a time; with nextScreenArg
// GTS_SELECT_MON_MODE_DEPOSIT the chosen Pokemon is offered for deposit (then the wanted Pokemon is chosen, screen
// 6), with GTS_SELECT_MON_MODE_TRADE it is offered for the Pokemon of the selected search result (only Pokemon the
// depositor wants can be picked, the others are dimmed). Pokemon with a special Ribbon, in a special form, holding the
// Griseous Orb, and Eggs can't be offered; the Ball Capsule of a party Pokemon is detached first.

typedef enum GtsSelectMonMode {
    GTS_SELECT_MON_MODE_DEPOSIT = 5,
    GTS_SELECT_MON_MODE_TRADE,
} GtsSelectMonMode;

typedef enum GtsSelectMonState {
    GTS_SELECT_MON_STATE_START,
    GTS_SELECT_MON_STATE_HANDLE_INPUT,
    GTS_SELECT_MON_STATE_FADE_OUT,
    GTS_SELECT_MON_STATE_WAIT_TEXT,
    GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR,
    GTS_SELECT_MON_STATE_SHOW_EXIT_PROMPT,
    GTS_SELECT_MON_STATE_HANDLE_EXIT_PROMPT,
    GTS_SELECT_MON_STATE_OPEN_DEPOSIT_MENU,
    GTS_SELECT_MON_STATE_HANDLE_DEPOSIT_MENU,
    GTS_SELECT_MON_STATE_OPEN_TRADE_MENU,
    GTS_SELECT_MON_STATE_HANDLE_TRADE_MENU,
    GTS_SELECT_MON_STATE_ASK_REMOVE_CAPSULE_FOR_TRADE,
    GTS_SELECT_MON_STATE_SHOW_CAPSULE_PROMPT_FOR_TRADE,
    GTS_SELECT_MON_STATE_HANDLE_CAPSULE_PROMPT_FOR_TRADE,
    GTS_SELECT_MON_STATE_ASK_REMOVE_CAPSULE,
    GTS_SELECT_MON_STATE_SHOW_CAPSULE_PROMPT,
    GTS_SELECT_MON_STATE_HANDLE_CAPSULE_PROMPT,
} GtsSelectMonState;

#define GTS_SELECT_MON_SLOT_COUNT 30
// GtsApp.cursorSlot past the slots (the cursor's positions).
#define GTS_SELECT_MON_BACK     30
#define GTS_SELECT_MON_BOX_NAME 31
// What GtsSelectMon_GetTouchedButton returns past the slots.
#define GTS_SELECT_MON_TOUCH_BACK     30
#define GTS_SELECT_MON_TOUCH_NEXT_BOX 31
#define GTS_SELECT_MON_TOUCH_PREV_BOX 32
// A move in sGtsSelectMonCursorMoves to this value + 1 or - 1 shows the next or previous box.
#define GTS_SELECT_MON_MOVE_BOX 100

// What GtsSelectMon_GetSlotContent finds in a slot.
typedef enum GtsSlotContent {
    GTS_SLOT_EMPTY,
    GTS_SLOT_MON,
    GTS_SLOT_EGG,
} GtsSlotContent;

// What GtsSelectMon_GetFormRestriction says about a Pokemon.
#define GTS_FORM_ALLOWED     0
#define GTS_FORM_SPECIAL     1 // Rotom, Giratina or Shaymin in another form
#define GTS_FORM_SPIKY_PICHU 2

struct GtsMonIcon {
    u32 vramOffset;
    u32 palette; // dimmed: + 3
    Sprite *sprite;
    u8 pixels[0x200];
};

typedef struct GtsSelectMonPosition {
    u16 x;
    u16 y;
} GtsSelectMonPosition;

// The summary of a Pokemon, and the first fields of the wanted Pokemon, as they are copied into the record: in u16
// halves.
typedef struct GtsMonFields {
    u16 species;

    union {
        struct {
            u8 gender;
            u8 level;
            u8 maxLevel;
        };

        u16 halves[2];
    };
} GtsMonFields;

void WirelessStatusIcon_SetLevel(s32 level);

extern int (*const sGtsSelectMonStates[])(GtsApp *app);

static void GtsSelectMon_InitBgs(BgConfig *bgConfig);
static void GtsSelectMon_FreeBgs(BgConfig *bgConfig);
static void GtsSelectMon_LoadGraphics(GtsApp *app);
static void GtsSelectMon_CreateSprites(GtsApp *app);
static void GtsSelectMon_DeleteSprites(GtsApp *app);
static void GtsSelectMon_CreateWindows(GtsApp *app);
static void GtsSelectMon_RemoveWindows(GtsApp *app);
static void GtsSelectMon_CreateData(GtsApp *app);
static void GtsSelectMon_FreeData(GtsApp *app);
static void GtsSelectMon_SelectMonToDeposit(GtsApp *app);
static void GtsSelectMon_SelectMonToTrade(GtsApp *app);
static int GtsSelectMon_GetTouchedButton(void);
static void GtsSelectMon_Leave(GtsApp *app, int mode);
static void GtsSelectMon_MoveCursor(GtsApp *app);
static void GtsSelectMon_SetCursorPosition(Sprite *sprite, int position);
static int GtsSelectMon_StepIndex(int index, int count, int step);
static BOOL GtsSelectMon_StartTrade(GtsApp *app);
static void GtsSelectMon_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, int unused4, BOOL inPromptWindow);
static void *GtsSelectMon_LoadCharData(NARC *narc, int member, NNSG2dCharacterData **charData, enum HeapID heapID);
static void GtsSelectMon_LoadMonIcon(int species, int form, BOOL isEgg, int slot, Sprite *sprite, NARC *narc, GtsMonIcon *icon);
static void GtsSelectMon_SetSummaryLevel(BoxPokemon *boxMon, GtsMonSummary *summary);
static void GtsSelectMon_TransferMonIcons(GtsApp *app);
static void GtsSelectMon_ShowSlotMon(BoxPokemon *boxMon, Sprite *iconSprite, Sprite *itemSprite, u16 *species, int slot, NARC *narc, GtsMonSummary *summary, GtsMonIcon *icon);
static void GtsSelectMon_ShowBox(GtsApp *app, int box);
static BOOL GtsSelectMon_CanOffer(Party *party, PCStorage *pcStorage, int box, int slot);
static BOOL GtsSelectMon_HasSpecialRibbon(BoxPokemon *boxMon);
static int GtsSelectMon_GetFormRestriction(BoxPokemon *boxMon);
static BOOL GtsSelectMon_HoldsGriseousOrb(BoxPokemon *boxMon);
static int GtsSelectMon_GetSlotContent(Party *party, PCStorage *pcStorage, int box, int slot);
static BOOL GtsSelectMon_MatchesWanted(GtsMonSummary *summary, GtsWantedMon *wanted);
static BOOL GtsSelectMon_BoxMonMatchesWanted(BoxPokemon *boxMon, GtsWantedMon *wanted);
static void GtsSelectMon_InitTradeRecord(GtsRecord *record, GtsApp *app);
static void GtsSelectMon_DimUnwantedIcons(GtsMonSummary *summaries, Sprite **sprites, GtsWantedMon *wanted, GtsMonIcon *icons);

// {x, y} of the cursor on each slot, on BACK and on the box name.
// The next and previous box arrows.
// Where the cursor goes from each position: up, down, left, right.
// The Ribbons that can't be traded (MON_DATA_*).

int GtsSearch_PrintSearchFailed(GtsApp *app);
int GtsSearch_WaitText(GtsApp *app);
int GtsSearch_WaitTextAndPause(GtsApp *app);
int GtsSearch_ScrollOut(GtsApp *app);
int GtsSearch_ScrollIn(GtsApp *app);
int GtsSearch_HandleScrolledOut(GtsApp *app);
int GtsSelectMon_Init(GtsApp *app, int state);
int GtsSelectMon_Main(GtsApp *app, int state);
int GtsSelectMon_Exit(GtsApp *app, int state);
int GtsSelectMon_Start(GtsApp *app);
int GtsSelectMon_HandleInput(GtsApp *app);
int GtsSelectMon_OpenDepositMenu(GtsApp *app);
int GtsSelectMon_HandleDepositMenu(GtsApp *app);
int GtsSelectMon_OpenTradeMenu(GtsApp *app);
int GtsSelectMon_HandleTradeMenu(GtsApp *app);
int GtsSelectMon_FadeOut(GtsApp *app);
int GtsSelectMon_ShowExitPrompt(GtsApp *app);
int GtsSelectMon_HandleExitPrompt(GtsApp *app);
int GtsSelectMon_AskRemoveCapsuleForTrade(GtsApp *app);
int GtsSelectMon_ShowCapsulePromptForTrade(GtsApp *app);
int GtsSelectMon_HandleCapsulePromptForTrade(GtsApp *app);
int GtsSelectMon_AskRemoveCapsule(GtsApp *app);
int GtsSelectMon_ShowCapsulePrompt(GtsApp *app);
int GtsSelectMon_HandleCapsulePrompt(GtsApp *app);
int GtsSelectMon_WaitText(GtsApp *app);
int GtsSelectMon_WaitTextAndClear(GtsApp *app);

// The last states of SEEK POKEMON (screen 4).

static const GtsSelectMonPosition sGtsSelectMonArrowPositions[2] = {
    { 154, 12 },
    { 14,  12 },
};

static const GraphicsModes sGtsSelectMonGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const u16 sGtsSpecialRibbons[10] = {
    MON_DATA_MARINE_RIBBON,
    MON_DATA_LAND_RIBBON,
    MON_DATA_SKY_RIBBON,
    MON_DATA_RED_RIBBON,
    MON_DATA_GREEN_RIBBON,
    MON_DATA_BLUE_RIBBON,
    MON_DATA_FESTIVAL_RIBBON,
    MON_DATA_CARNIVAL_RIBBON,
    MON_DATA_CLASSIC_RIBBON,
    MON_DATA_PREMIER_RIBBON,
};

static const BgTemplate sGtsSelectMonBg2Template = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sGtsSelectMonBg3Template = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sGtsSelectMonBg1Template = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sGtsSelectMonBg0Template = {
    .x = 0,
    .y = 0,
    .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GtsSelectMonPosition sGtsSelectMonCursorPositions[32] = {
    { 19,  36  },
    { 45,  36  },
    { 71,  36  },
    { 97,  36  },
    { 123, 36  },
    { 149, 36  },
    { 19,  59  },
    { 45,  59  },
    { 71,  59  },
    { 97,  59  },
    { 123, 59  },
    { 149, 59  },
    { 19,  82  },
    { 45,  82  },
    { 71,  82  },
    { 97,  82  },
    { 123, 82  },
    { 149, 82  },
    { 19,  105 },
    { 45,  105 },
    { 71,  105 },
    { 97,  105 },
    { 123, 105 },
    { 149, 105 },
    { 19,  128 },
    { 45,  128 },
    { 71,  128 },
    { 97,  128 },
    { 123, 128 },
    { 149, 128 },
    { 224, 135 },
    { 82,  16  },
};

static const u8 sGtsSelectMonCursorMoves[32][4] = {
    { 31, 6,  5,  1   },
    { 31, 7,  0,  2   },
    { 31, 8,  1,  3   },
    { 31, 9,  2,  4   },
    { 31, 10, 3,  5   },
    { 31, 11, 4,  0   },
    { 0,  12, 11, 7   },
    { 1,  13, 6,  8   },
    { 2,  14, 7,  9   },
    { 3,  15, 8,  10  },
    { 4,  16, 9,  11  },
    { 5,  17, 10, 6   },
    { 6,  18, 17, 13  },
    { 7,  19, 12, 14  },
    { 8,  20, 13, 15  },
    { 9,  21, 14, 16  },
    { 10, 22, 15, 17  },
    { 11, 23, 16, 12  },
    { 12, 24, 23, 19  },
    { 13, 25, 18, 20  },
    { 14, 26, 19, 21  },
    { 15, 27, 20, 22  },
    { 16, 28, 21, 23  },
    { 17, 29, 22, 18  },
    { 18, 31, 30, 25  },
    { 19, 31, 24, 26  },
    { 20, 31, 25, 27  },
    { 21, 31, 26, 28  },
    { 22, 31, 27, 29  },
    { 23, 31, 28, 30  },
    { 30, 30, 29, 24  },
    { 26, 2,  99, 101 },
};

static const TouchscreenHitbox sGtsSelectMonHitboxes[34] = {
    { .rect = { 23, 49, 8, 30 } },
    { .rect = { 23, 49, 34, 56 } },
    { .rect = { 23, 49, 60, 82 } },
    { .rect = { 23, 49, 86, 108 } },
    { .rect = { 23, 49, 112, 134 } },
    { .rect = { 23, 49, 138, 160 } },
    { .rect = { 46, 72, 8, 30 } },
    { .rect = { 46, 72, 34, 56 } },
    { .rect = { 46, 72, 60, 82 } },
    { .rect = { 46, 72, 86, 108 } },
    { .rect = { 46, 72, 112, 134 } },
    { .rect = { 46, 72, 138, 160 } },
    { .rect = { 69, 95, 8, 30 } },
    { .rect = { 69, 95, 34, 56 } },
    { .rect = { 69, 95, 60, 82 } },
    { .rect = { 69, 95, 86, 108 } },
    { .rect = { 69, 95, 112, 134 } },
    { .rect = { 69, 95, 138, 160 } },
    { .rect = { 92, 118, 8, 30 } },
    { .rect = { 92, 118, 34, 56 } },
    { .rect = { 92, 118, 60, 82 } },
    { .rect = { 92, 118, 86, 108 } },
    { .rect = { 92, 118, 112, 134 } },
    { .rect = { 92, 118, 138, 160 } },
    { .rect = { 115, 141, 8, 30 } },
    { .rect = { 115, 141, 34, 56 } },
    { .rect = { 115, 141, 60, 82 } },
    { .rect = { 115, 141, 86, 108 } },
    { .rect = { 115, 141, 112, 134 } },
    { .rect = { 115, 141, 138, 160 } },
    { .rect = { 123, 148, 186, 245 } },
    { .rect = { 4, 28, 146, 162 } },
    { .rect = { 4, 28, 2, 18 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

int GtsSearch_PrintSearchFailed(GtsApp *app) {
    GtsSearch_PrintMessage(app, msg_0775_00152, 1, 0, 0xF0F); // The search failed...
    Gts_SetState(app, 21, 1);                                 // GTS_SEARCH_STATE_WAIT_TEXT, GTS_SEARCH_STATE_HANDLE_INPUT
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSearch_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSearch_WaitTextAndPause(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->waitFrames++;
        if (app->waitFrames > 45) {
            app->waitFrames = 0;
            app->state = app->nextState;
        }
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSearch_ScrollOut(GtsApp *app) {
    app->scrollY++;
    if (IsPaletteFadeFinished()) {
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        app->scrollY = -16;
        if (app->hasSearchResults) {
            Sprite_SetAnimCtrlSeq(app->avatarSprites[1], 16);
            app->state = 2; // GTS_SEARCH_STATE_FADE_OUT
            Gts_SetNextScreen(app, 3, 16);
            app->selectedResult = 0;
        }
        Sprite_SetDrawFlag(app->panelSprite, FALSE);
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSearch_ScrollIn(GtsApp *app) {
    app->scrollY--;
    if (IsPaletteFadeFinished()) {
        app->scrollY = 0;
        app->state = 1; // GTS_SEARCH_STATE_HANDLE_INPUT
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSearch_HandleScrolledOut(GtsApp *app) {
    int index;

    if (gSystem.newKeys & PAD_BUTTON_A) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 16, 1, HEAP_ID_61);
    } else {
        index = Gts_GetTouchedAvatar(app->searchResultCount);
        if (app->hasSearchResults && index >= 0) {
            Sprite_SetAnimCtrlSeq(app->avatarSprites[index + 1], index * 4 + 16);
            app->state = 2; // GTS_SEARCH_STATE_FADE_OUT
            Gts_SetNextScreen(app, 3, 0);
            app->selectedResult = index;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

void GtsSearch_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4) {
    ReadMsgDataIntoString(app->gtsMsgData, msgId, app->msgBuffer);
    FillWindowPixelBuffer(&app->waitingIconWindow, 15);
    DrawFrameAndWindow2(&app->waitingIconWindow, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(&app->waitingIconWindow, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
}

void GtsSearch_DrawFieldLabels(Window *fieldWindows, Window *lineWindow, MsgData *msgData) {
    String *string;

    string = NewString_ReadMsgData(msgData, msg_0775_00061); // POKEMON
    Gts_PrintAlignedFont0(&fieldWindows[0], string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, msg_0775_00063); // GENDER
    Gts_PrintAlignedFont0(&fieldWindows[2], string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, msg_0775_00065); // LEVEL
    Gts_PrintAlignedFont0(&fieldWindows[4], string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, msg_0775_00171); // LOCATION
    Gts_PrintAlignedFont0(lineWindow, string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, msg_0775_00067); // SEARCH
    Gts_PrintAlignedFont4(&fieldWindows[6], string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, msg_0775_00238); // CANCEL
    Gts_PrintAlignedFont4(&fieldWindows[7], string, 0, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    String_Delete(string);
}

// The SELECT button (to the search results); greyed out when there are none.
void GtsSearch_DrawResultsButton(BgConfig *bgConfig, Window *window, MsgData *msgData, BOOL enabled) {
    u32 color = MAKE_TEXT_COLOR(1, 2, 0);
    u8 palette;
    String *string;

    if (enabled) {
        color = MAKE_TEXT_COLOR(15, 2, 0);
        palette = 0;
    } else {
        palette = 2;
    }
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_1, 18, 2, 14, 4, palette);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
    string = NewString_ReadMsgData(msgData, msg_0775_00069); // SELECT
    Gts_PrintAlignedFont4(window, string, 0, 0, 0, color);
    String_Delete(string);
}

BOOL GtsSearch_IsSameSearch(GtsWantedMon *criteria, GtsWantedMon *lastSearch, u32 country, u32 lastCountry) {
    if (criteria->species == lastSearch->species && criteria->gender == lastSearch->gender && criteria->minLevel == lastSearch->minLevel && criteria->maxLevel == lastSearch->maxLevel && country == lastCountry) {
        return TRUE;
    }
    return FALSE;
}

// VBlank: the main screen scrolls up by scrollY, the sub screen down.
void GtsSearch_ScrollBgs(GtsApp *app) {
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, -app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, -app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -app->scrollY);
    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, -app->scrollY);
}

// Darkens main BG 1 and 3 behind the selector.
void GtsSearch_SetBlendBrightness(int brightness) {
    if (brightness != 0) {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3, brightness);
    } else {
        reg_G2_BLDCNT = 0;
    }
}

// Choosing a Pokemon (screen 5).

int GtsSelectMon_Init(GtsApp *app, int state) {
    GtsSelectMon_CreateData(app);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    GtsSelectMon_InitBgs(app->bgConfig);
    GtsSelectMon_LoadGraphics(app);
    GtsSelectMon_CreateWindows(app);
    GtsSelectMon_CreateSprites(app);
    Gts_ResetAvatarPositions(app);
    if (GXS_GetMasterBrightness() == 0) {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    GtsSelectMon_ShowBox(app, app->selectedBox);
    Gts_CreateWirelessStatusIcon(app);
    app->state = GTS_SELECT_MON_STATE_START;
    return GTS_APP_STATE_WAIT_FADE_IN;
}

int GtsSelectMon_Main(GtsApp *app, int state) {
    WirelessStatusIcon_SetLevel(Gts_GetWifiIconLevel());
    return sGtsSelectMonStates[app->state](app);
}

int GtsSelectMon_Exit(GtsApp *app, int state) {
    WirelessStatusIcon_Destroy();
    if (GXS_GetMasterBrightness()) {
        Gts_LowerAvatars(app);
    }
    GtsSelectMon_DeleteSprites(app);
    GtsSelectMon_FreeData(app);
    GtsSelectMon_RemoveWindows(app);
    GtsSelectMon_FreeBgs(app->bgConfig);
    Sprite_SetDrawFlag(app->panelSprite, FALSE);
    Gts_SwitchScreen(app);
    return GTS_APP_STATE_INIT_SCREEN;
}

static void GtsSelectMon_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;

    modes = sGtsSelectMonGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    bg0 = sGtsSelectMonBg0Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    bg1 = sGtsSelectMonBg1Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    bg2 = sGtsSelectMonBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    bg3 = sGtsSelectMonBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    Gts_InitSubScreenBgs(bgConfig, 0, FALSE);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_61);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_61);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void GtsSelectMon_FreeBgs(BgConfig *bgConfig) {
    Gts_FreeSubScreenBgs(bgConfig);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
}

static void GtsSelectMon_LoadGraphics(GtsApp *app) {
    BgConfig *bgConfig = app->bgConfig;
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(app->args->options), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 13, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xA00, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 27, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 38, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_61);
    Gts_LoadSubScreenBg(app);
    Gts_ShowSubScreenPanel(app);
    NARC_Delete(narc);
}

// The cursor, each slot's icon, held item and (in the party) Ball Capsule, the box arrows.
static void GtsSelectMon_CreateSprites(GtsApp *app) {
    SpriteTemplate template;
    int i;

    Gts_InitSpriteTemplate(&template, app, &app->headers[0], NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = sGtsSelectMonCursorPositions[app->selectedSlot].x << FX32_SHIFT;
    template.position.y = sGtsSelectMonCursorPositions[app->selectedSlot].y << FX32_SHIFT;
    app->cursorSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(app->cursorSprite, TRUE);
    Sprite_SetAnimCtrlSeq(app->cursorSprite, 4);
    if (app->selectedSlot == GTS_SELECT_MON_BOX_NAME || app->selectedSlot <= 5) {
        Sprite_SetPriority(app->cursorSprite, 0);
    } else {
        Sprite_SetPriority(app->cursorSprite, 1);
    }
    for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
        template.position.x = sGtsSelectMonCursorPositions[i].x << FX32_SHIFT;
        template.position.y = sGtsSelectMonCursorPositions[i].y << FX32_SHIFT;
        template.drawPriority = 20;
        app->monIconSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(app->monIconSprites[i], i + 6);
        Sprite_SetPriority(app->monIconSprites[i], 1);
    }
    for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
        template.position.x = sGtsSelectMonCursorPositions[i].x << FX32_SHIFT;
        template.position.y = (sGtsSelectMonCursorPositions[i].y + 6) << FX32_SHIFT;
        template.drawPriority = 10;
        app->heldItemSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(app->heldItemSprites[i], 40);
        Sprite_SetPriority(app->heldItemSprites[i], 1);
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        template.position.x = (sGtsSelectMonCursorPositions[i].x + 8) << FX32_SHIFT;
        template.position.y = (sGtsSelectMonCursorPositions[i].y + 6) << FX32_SHIFT;
        template.drawPriority = 10;
        app->capsuleSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(app->capsuleSprites[i], 42);
        Sprite_SetPriority(app->capsuleSprites[i], 1);
    }
    for (i = 0; i < 2; i++) {
        template.position.x = sGtsSelectMonArrowPositions[i].x << FX32_SHIFT;
        template.position.y = sGtsSelectMonArrowPositions[i].y << FX32_SHIFT;
        app->arrowSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(app->arrowSprites[i], i + 38);
        Sprite_SetPriority(app->arrowSprites[i], 1);
    }
    Sprite_SetDrawFlag(app->panelSprite, TRUE);
    Gts_SetSpritePosition(app->panelSprite, 55, 424);
}

static void GtsSelectMon_DeleteSprites(GtsApp *app) {
    int i;

    for (i = 0; i < 2; i++) {
        Sprite_Delete(app->arrowSprites[i]);
    }
    Sprite_Delete(app->cursorSprite);
    for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
        Sprite_Delete(app->monIconSprites[i]);
        Sprite_Delete(app->heldItemSprites[i]);
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        Sprite_Delete(app->capsuleSprites[i]);
    }
}

// The box name, the message and prompt windows, the CANCEL button.
static void GtsSelectMon_CreateWindows(GtsApp *app) {
    AddWindowParameterized(app->bgConfig, &app->boxNameWindow, GF_BG_LYR_MAIN_3, 4, 0, 13, 3, 13, 0x60);
    FillWindowPixelBuffer(&app->boxNameWindow, 0);
    CopyWindowToVram(&app->boxNameWindow);
    AddWindowParameterized(app->bgConfig, &app->waitingIconWindow, GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 13, 0x109);
    FillWindowPixelBuffer(&app->waitingIconWindow, 0);
    AddWindowParameterized(app->bgConfig, &app->promptWindow, GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 13, 0x13F);
    FillWindowPixelBuffer(&app->promptWindow, 0);
    AddWindowParameterized(app->bgConfig, &app->windows[1], GF_BG_LYR_MAIN_1, 24, 16, 6, 2, 0, 0x165);
    FillWindowPixelBuffer(&app->windows[1], 6);
    Gts_PrintAlignedFont0(&app->windows[1], app->cancelText, 0, 1, 1, MAKE_TEXT_COLOR(1, 3, 6));
    if (app->nextScreenArg == GTS_SELECT_MON_MODE_DEPOSIT) {
        Gts_CreatePanelWindow(app, 3);
    } else {
        Gts_CreatePanelWindow(app, 1);
    }
}

static void GtsSelectMon_RemoveWindows(GtsApp *app) {
    RemoveWindow(&app->panelWindow);
    RemoveWindow(&app->promptWindow);
    RemoveWindow(&app->windows[1]);
    RemoveWindow(&app->waitingIconWindow);
    RemoveWindow(&app->boxNameWindow);
}

static void GtsSelectMon_CreateData(GtsApp *app) {
    app->boxName = String_New(18, HEAP_ID_61);
    app->msgBuffer = String_New(180, HEAP_ID_61);
    app->cancelText = NewString_ReadMsgData(app->gtsMsgData, msg_0775_00109); // CANCEL
    if (app->selectedSlot == GTS_SELECT_MON_BACK) {
        app->selectedSlot = 0;
    }
    app->boxSummaries = Heap_Alloc(HEAP_ID_61, sizeof(GtsMonSummary) * GTS_SELECT_MON_SLOT_COUNT);
}

static void GtsSelectMon_FreeData(GtsApp *app) {
    Heap_Free(app->boxSummaries);
    String_Delete(app->boxName);
    String_Delete(app->msgBuffer);
    String_Delete(app->cancelText);
}

int GtsSelectMon_Start(GtsApp *app) {
    int msgId;

    if (IsPaletteFadeFinished()) {
        if (app->nextScreenArg == GTS_SELECT_MON_MODE_DEPOSIT) {
            msgId = 21; // Choose the Pokemon you want to offer.
        } else if (app->nextScreenArg == GTS_SELECT_MON_MODE_TRADE) {
            msgId = 17; // Choose the Pokemon you are trading.
        }
        GtsSelectMon_PrintMessage(app, msgId, 1, 0, 0xF0F, FALSE);
        Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT, GTS_SELECT_MON_STATE_HANDLE_INPUT);
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

static void GtsSelectMon_SelectMonToDeposit(GtsApp *app) {
    BoxPokemon *boxMon;

    if (app->selectedSlot == GTS_SELECT_MON_BACK) {
        Gts_SetNextScreen(app, 1, 0);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        PlaySE(SEQ_SE_DP_SELECT);
        return;
    }
    if (app->selectedSlot == GTS_SELECT_MON_BOX_NAME) {
        return;
    }
    PlaySE(SEQ_SE_DP_SELECT);
    switch (GtsSelectMon_GetSlotContent(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot)) {
    case GTS_SLOT_MON:
        if (GtsSelectMon_CanOffer(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot)) {
            boxMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
            BufferBoxMonNickname(app->msgFormat, 0, boxMon);
            GtsSelectMon_PrintMessage(app, msg_0775_00022, 1, 0, 0xF0F, FALSE); // Offer {nickname}?
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT, GTS_SELECT_MON_STATE_OPEN_DEPOSIT_MENU);
        } else {
            GtsSelectMon_PrintMessage(app, msg_0775_00026, 1, 0, 0xF0F, TRUE); // That Pokemon may not be offered for trade!
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
        }
        break;
    case GTS_SLOT_EGG:
        GtsSelectMon_PrintMessage(app, msg_0775_00027, 1, 0, 0xF0F, TRUE); // An Egg may not be offered for trade!
        Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
        break;
    }
}

// Only the Pokemon the selected search result wants can be picked.
static void GtsSelectMon_SelectMonToTrade(GtsApp *app) {
    BoxPokemon *boxMon;

    if (app->selectedSlot == GTS_SELECT_MON_BACK) {
        GtsSelectMon_Leave(app, GTS_SELECT_MON_MODE_TRADE);
        return;
    }
    if (app->selectedSlot == GTS_SELECT_MON_BOX_NAME) {
        return;
    }
    switch (GtsSelectMon_GetSlotContent(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot)) {
    case GTS_SLOT_MON:
        boxMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
        if (GtsSelectMon_BoxMonMatchesWanted(boxMon, &app->searchResults[app->selectedResult].wanted)) {
            if (GtsSelectMon_CanOffer(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot)) {
                BufferBoxMonNickname(app->msgFormat, 0, boxMon);
                GtsSelectMon_PrintMessage(app, msg_0775_00018, 1, 0, 0xF0F, FALSE); // Do what with this {nickname}?
                Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT, GTS_SELECT_MON_STATE_OPEN_TRADE_MENU);
                PlaySE(SEQ_SE_DP_SELECT);
            } else {
                GtsSelectMon_PrintMessage(app, msg_0775_00026, 1, 0, 0xF0F, TRUE); // That Pokemon may not be offered for trade!
                Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
            }
        } else {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        break;
    case GTS_SLOT_EMPTY:
    case GTS_SLOT_EGG:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
}

static int GtsSelectMon_GetTouchedButton(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectMonHitboxes);
}

// BACK: to the main menu (deposit) or back to the search result (trade).
static void GtsSelectMon_Leave(GtsApp *app, int mode) {
    if (mode == GTS_SELECT_MON_MODE_DEPOSIT) {
        Gts_SetNextScreen(app, 1, 0);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (mode == GTS_SELECT_MON_MODE_TRADE) {
        Gts_SetNextScreen(app, 3, 17);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

int GtsSelectMon_HandleInput(GtsApp *app) {
    int button = GtsSelectMon_GetTouchedButton();

    if (button != -1) {
        switch (button) {
        case GTS_SELECT_MON_TOUCH_NEXT_BOX:
            Sprite_SetAnimActiveFlag(app->arrowSprites[0], TRUE);
            Sprite_SetAnimCtrlSeq(app->arrowSprites[0], 38);
            app->selectedBox = GtsSelectMon_StepIndex(app->selectedBox, GTS_BOX_PARTY + 1, 1);
            GtsSelectMon_ShowBox(app, app->selectedBox);
            PlaySE(SEQ_SE_DP_SELECT);
            break;
        case GTS_SELECT_MON_TOUCH_PREV_BOX:
            Sprite_SetAnimActiveFlag(app->arrowSprites[1], TRUE);
            Sprite_SetAnimCtrlSeq(app->arrowSprites[1], 39);
            app->selectedBox = GtsSelectMon_StepIndex(app->selectedBox, GTS_BOX_PARTY + 1, -1);
            GtsSelectMon_ShowBox(app, app->selectedBox);
            PlaySE(SEQ_SE_DP_SELECT);
            break;
        case GTS_SELECT_MON_TOUCH_BACK:
            GtsSelectMon_Leave(app, app->nextScreenArg);
            app->selectedSlot = button;
            GtsSelectMon_SetCursorPosition(app->cursorSprite, app->selectedSlot);
            break;
        default:
            app->selectedSlot = button;
            GtsSelectMon_SetCursorPosition(app->cursorSprite, app->selectedSlot);
            if (app->nextScreenArg == GTS_SELECT_MON_MODE_DEPOSIT) {
                GtsSelectMon_SelectMonToDeposit(app);
            } else if (app->nextScreenArg == GTS_SELECT_MON_MODE_TRADE) {
                GtsSelectMon_SelectMonToTrade(app);
            }
            break;
        }
    } else {
        GtsSelectMon_MoveCursor(app);
        if (app->nextScreenArg == GTS_SELECT_MON_MODE_DEPOSIT) {
            if (gSystem.newKeys & PAD_BUTTON_B) {
                GtsSelectMon_Leave(app, GTS_SELECT_MON_MODE_DEPOSIT);
            } else if (gSystem.newKeys & PAD_BUTTON_A) {
                GtsSelectMon_SelectMonToDeposit(app);
            }
        } else if (app->nextScreenArg == GTS_SELECT_MON_MODE_TRADE) {
            if (gSystem.newKeys & PAD_BUTTON_B) {
                GtsSelectMon_Leave(app, GTS_SELECT_MON_MODE_TRADE);
            } else if (gSystem.newKeys & PAD_BUTTON_A) {
                GtsSelectMon_SelectMonToTrade(app);
            }
        }
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

// The D-pad; left or right off the box name shows the previous or next box.
static void GtsSelectMon_MoveCursor(GtsApp *app) {
    int moved;
    int direction;
    u32 keys;
    u8 next;
    u8 arrow;

    moved = FALSE;
    direction = 0;
    keys = gSystem.newKeys;
    if (keys & PAD_KEY_UP) {
        direction = 1;
    } else if (keys & PAD_KEY_DOWN) {
        direction = 2;
    } else if (keys & PAD_KEY_LEFT) {
        direction = 3;
    } else if (keys & PAD_KEY_RIGHT) {
        direction = 4;
    }
    if (direction != 0) {
        next = sGtsSelectMonCursorMoves[app->selectedSlot][direction - 1];
        if (next != app->selectedSlot) {
            if (next == GTS_SELECT_MON_MOVE_BOX - 1 || next == GTS_SELECT_MON_MOVE_BOX + 1) {
                arrow = (u8)(next != GTS_SELECT_MON_MOVE_BOX + 1);
                Sprite_SetAnimActiveFlag(app->arrowSprites[arrow], TRUE);
                Sprite_SetAnimCtrlSeq(app->arrowSprites[arrow], arrow + 38);
                app->selectedBox = GtsSelectMon_StepIndex(app->selectedBox, GTS_BOX_PARTY + 1, next - GTS_SELECT_MON_MOVE_BOX);
                GtsSelectMon_ShowBox(app, app->selectedBox);
                PlaySE(SEQ_SE_DP_SELECT);
            } else {
                moved = TRUE;
                app->selectedSlot = next;
            }
        }
    }
    if (moved) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
    GtsSelectMon_SetCursorPosition(app->cursorSprite, app->selectedSlot);
}

static void GtsSelectMon_SetCursorPosition(Sprite *sprite, int position) {
    Gts_SetSpritePositionRounded(sprite, sGtsSelectMonCursorPositions[position].x, sGtsSelectMonCursorPositions[position].y);
    if (position == GTS_SELECT_MON_BOX_NAME || (position >= 0 && position <= 5)) {
        Sprite_SetPriority(sprite, 0);
    } else {
        Sprite_SetPriority(sprite, 1);
    }
}

// index + step, wrapping around in 0..count - 1.
static int GtsSelectMon_StepIndex(int index, int count, int step) {
    index += step;
    if (index < 0) {
        return count - 1;
    }
    if (index == count) {
        return 0;
    }
    return index;
}

int GtsSelectMon_OpenDepositMenu(GtsApp *app) {
    app->listItems = ListMenuItems_New(3, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00098, 1); // SUMMARY
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00099, 2); // OFFER
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00100, 3); // CANCEL
    app->menu = Gts_CreateListMenu(app, 3, 10);
    app->state = GTS_SELECT_MON_STATE_HANDLE_DEPOSIT_MENU;
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_HandleDepositMenu(GtsApp *app) {
    u32 input;
    BoxPokemon *boxMon;
    BOOL hasCapsule;

    input = TouchscreenListMenu_HandleInput(app->menu);
    switch (input) {
    case 1: // SUMMARY
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 8, 5);
        break;
    case 2: // OFFER
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        boxMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
        if (GtsSelectMon_HasSpecialRibbon(boxMon)) {
            GtsSelectMon_PrintMessage(app, msg_0775_00037, 1, 0, 0xF0F, TRUE); // It has a special Ribbon.
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
            break;
        }
        {
            int restriction = GtsSelectMon_GetFormRestriction(boxMon);
            if (restriction) {
                if (restriction == GTS_FORM_SPECIAL) {
                    GtsSelectMon_PrintMessage(app, msg_0775_00177, 1, 0, 0xF0F, TRUE); // This Pokemon cannot be traded in this Forme!
                } else {
                    GtsSelectMon_PrintMessage(app, msg_0775_00178, 1, 0, 0xF0F, TRUE); // Spiky-eared PICHU cannot be offered in trade.
                }
                Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
                break;
            }
        }
        if (GtsSelectMon_HoldsGriseousOrb(boxMon)) {
            GtsSelectMon_PrintMessage(app, msg_0775_00179, 1, 0, 0xF0F, TRUE); // This Pokemon cannot be traded while holding that item!
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
            break;
        }
        hasCapsule = FALSE;
        if (Gts_IsPartyBox(app->selectedBox)) {
            Pokemon *partyMon = Party_GetMonByIndex(app->args->party, app->selectedSlot);
            if (GetMonData(partyMon, MON_DATA_BALL_CAPSULE_ID, NULL)) {
                hasCapsule = TRUE;
                app->state = GTS_SELECT_MON_STATE_ASK_REMOVE_CAPSULE;
            }
        }
        if (!hasCapsule) {
            app->selectedMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
            app->state = GTS_SELECT_MON_STATE_FADE_OUT;
            Gts_SetNextScreen(app, 6, 0);
        }
        break;
    case 3: // CANCEL
    case -2:
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SELECT_MON_STATE_START;
        break;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_OpenTradeMenu(GtsApp *app) {
    app->listItems = ListMenuItems_New(3, HEAP_ID_61);
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00087, 1); // SUMMARY
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00088, 2); // TRADE
    ListMenuItems_AppendFromMsgData(app->listItems, app->gtsMsgData, msg_0775_00089, 3); // CANCEL
    app->menu = Gts_CreateListMenu(app, 3, 10);
    app->state = GTS_SELECT_MON_STATE_HANDLE_TRADE_MENU;
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_HandleTradeMenu(GtsApp *app) {
    u32 input;
    BoxPokemon *boxMon;
    BOOL hasCapsule;

    input = TouchscreenListMenu_HandleInput(app->menu);
    switch (input) {
    case 1: // SUMMARY
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 8, 6);
        break;
    case 2: // TRADE
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        boxMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
        if (GtsSelectMon_HasSpecialRibbon(boxMon)) {
            GtsSelectMon_PrintMessage(app, msg_0775_00037, 1, 0, 0xF0F, TRUE); // It has a special Ribbon.
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
            break;
        }
        {
            int restriction = GtsSelectMon_GetFormRestriction(boxMon);
            if (restriction) {
                if (restriction == GTS_FORM_SPECIAL) {
                    GtsSelectMon_PrintMessage(app, msg_0775_00177, 1, 0, 0xF0F, TRUE); // This Pokemon cannot be traded in this Forme!
                } else {
                    GtsSelectMon_PrintMessage(app, msg_0775_00178, 1, 0, 0xF0F, TRUE); // Spiky-eared PICHU cannot be offered in trade.
                }
                Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
                break;
            }
        }
        if (GtsSelectMon_HoldsGriseousOrb(boxMon)) {
            GtsSelectMon_PrintMessage(app, msg_0775_00179, 1, 0, 0xF0F, TRUE); // This Pokemon cannot be traded while holding that item!
            Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
            break;
        }
        hasCapsule = FALSE;
        if (Gts_IsPartyBox(app->selectedBox)) {
            Pokemon *partyMon = Party_GetMonByIndex(app->args->party, app->selectedSlot);
            if (GetMonData(partyMon, MON_DATA_BALL_CAPSULE_ID, NULL)) {
                hasCapsule = TRUE;
                app->state = GTS_SELECT_MON_STATE_ASK_REMOVE_CAPSULE_FOR_TRADE;
            }
        }
        if (!hasCapsule) {
            GtsSelectMon_StartTrade(app);
        }
        break;
    case 3: // CANCEL
    case -2:
        Gts_DestroyListMenu(app);
        ListMenuItems_Delete(app->listItems);
        ClearFrameAndWindow2(&app->waitingIconWindow, FALSE);
        app->state = GTS_SELECT_MON_STATE_START;
        break;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_FadeOut(GtsApp *app) {
    if (app->nextScreen == 0 || app->nextScreen == 8 || app->nextScreen == 3) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
        app->fadedBothScreens = 1;
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_61);
    }
    app->state = GTS_SELECT_MON_STATE_START;
    return GTS_APP_STATE_EXIT_SCREEN;
}

int GtsSelectMon_ShowExitPrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 12, 0x1AD, 8, TRUE);
    app->state = GTS_SELECT_MON_STATE_HANDLE_EXIT_PROMPT;
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_HandleExitPrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 0, 0);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->state = GTS_SELECT_MON_STATE_START;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_AskRemoveCapsuleForTrade(GtsApp *app) {
    GtsSelectMon_PrintMessage(app, msg_0775_00025, 1, 0, 0xF0F, TRUE); // The Ball Capsule will be detached. Is that OK?
    Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT, GTS_SELECT_MON_STATE_SHOW_CAPSULE_PROMPT_FOR_TRADE);
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_ShowCapsulePromptForTrade(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 10, 0x1AD, 8, TRUE);
    app->state = GTS_SELECT_MON_STATE_HANDLE_CAPSULE_PROMPT_FOR_TRADE;
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_HandleCapsulePromptForTrade(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        GtsSelectMon_StartTrade(app);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->promptWindow, FALSE);
        app->state = GTS_SELECT_MON_STATE_HANDLE_INPUT;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

// The search result's Pokemon comes into the party when it holds Mail (unless the offered Pokemon leaves the party).
static BOOL GtsSelectMon_StartTrade(GtsApp *app) {
    if (Gts_MonHoldsMail(&app->searchResults[app->selectedResult].mon) && app->selectedBox != GTS_BOX_PARTY && Party_GetCount(app->args->party) == PARTY_SIZE) {
        GtsSelectMon_PrintMessage(app, msg_0775_00028, 1, 0, 0xF0F, TRUE); // Your party is full. You can't receive a Pokemon!
        Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT_AND_CLEAR, GTS_SELECT_MON_STATE_HANDLE_INPUT);
        return FALSE;
    }
    app->selectedMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
    app->state = GTS_SELECT_MON_STATE_FADE_OUT;
    app->fadedBothScreens = 1;
    Gts_SetNextScreen(app, 7, 9);
    GtsSelectMon_InitTradeRecord(&app->record, app);
    app->searchResultCount = 0;
    return TRUE;
}

int GtsSelectMon_AskRemoveCapsule(GtsApp *app) {
    GtsSelectMon_PrintMessage(app, msg_0775_00025, 1, 0, 0xF0F, TRUE); // The Ball Capsule will be detached. Is that OK?
    Gts_SetState(app, GTS_SELECT_MON_STATE_WAIT_TEXT, GTS_SELECT_MON_STATE_SHOW_CAPSULE_PROMPT);
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_ShowCapsulePrompt(GtsApp *app) {
    app->yesNoPrompt = Gts_CreateYesNoPrompt(app->bgConfig, 10, 0x1AD, 8, TRUE);
    app->state = GTS_SELECT_MON_STATE_HANDLE_CAPSULE_PROMPT;
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_HandleCapsulePrompt(GtsApp *app) {
    int response = Gts_HandleYesNoPrompt(app);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        app->selectedMon = Gts_GetBoxMon(app->args->party, app->args->pcStorage, app->selectedBox, app->selectedSlot);
        app->state = GTS_SELECT_MON_STATE_FADE_OUT;
        Gts_SetNextScreen(app, 6, 0);
    } else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(app->yesNoPrompt);
        ClearFrameAndWindow2(&app->promptWindow, FALSE);
        app->state = GTS_SELECT_MON_STATE_HANDLE_INPUT;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_WaitText(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

int GtsSelectMon_WaitTextAndClear(GtsApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        ClearFrameAndWindow2(&app->promptWindow, FALSE);
        app->state = app->nextState;
    }
    return GTS_APP_STATE_RUN_SCREEN;
}

static void GtsSelectMon_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, int unused4, BOOL inPromptWindow) {
    String *string = NewString_ReadMsgData(app->gtsMsgData, msgId);
    Window *window;
    StringExpandPlaceholders(app->msgFormat, app->msgBuffer, string);
    window = inPromptWindow == FALSE ? &app->waitingIconWindow : &app->promptWindow;
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, FALSE, 1, 14);
    app->textPrinterId = AddTextPrinterParameterized(window, 1, app->msgBuffer, 0, 0, textSpeed, NULL);
    String_Delete(string);
}

static void *GtsSelectMon_LoadCharData(NARC *narc, int member, NNSG2dCharacterData **charData, enum HeapID heapID) {
    void *data = NARC_AllocAndReadWholeMember(narc, member, heapID);
    if (data != NULL && !NNS_G2dGetUnpackedBGCharacterData(data, charData)) {
        Heap_Free(data);
        return NULL;
    }
    return data;
}

// Reads the icon of a Pokemon for slot's sprite (its own cell, at tile slot * 16 + 12).
static void GtsSelectMon_LoadMonIcon(int species, int form, BOOL isEgg, int slot, Sprite *sprite, NARC *narc, GtsMonIcon *icon) {
    NNSG2dCharacterData *charData;
    void *data;
    int member = GetMonIconNaixEx(species, isEgg, form);
    data = GtsSelectMon_LoadCharData(narc, member, &charData, HEAP_ID_61);
    MIi_CpuCopyFast(charData->pRawData, (u32 *)icon->pixels, sizeof(icon->pixels));
    icon->vramOffset = (slot * 16 + 12) * 0x20;
    icon->sprite = sprite;
    icon->palette = GetMonIconPaletteEx(species, form, isEgg) + 3;
    Heap_Free(data);
}

static void GtsSelectMon_SetSummaryLevel(BoxPokemon *boxMon, GtsMonSummary *summary) {
    summary->level = CalcBoxMonLevel(boxMon);
}

// VBlank task: loads the icons of the shown box.
static void GtsSelectMon_TransferMonIcons(GtsApp *app) {
    int i;
    GtsMonIcon *icon = app->monIcons;
    for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
        if (icon->sprite != NULL) {
            DC_FlushRange(icon->pixels, sizeof(icon->pixels));
            GX_LoadOBJ(icon->pixels, icon->vramOffset, sizeof(icon->pixels));
            Sprite_SetPaletteOverride(icon->sprite, icon->palette);
        }
        icon++;
    }
    Heap_Free(app->monIcons);
}

// The icon and held item (or Mail) of a slot's Pokemon; an Egg's level is 0.
static void GtsSelectMon_ShowSlotMon(BoxPokemon *boxMon, Sprite *iconSprite, Sprite *itemSprite, u16 *species, int slot, NARC *narc, GtsMonSummary *summary, GtsMonIcon *icon) {
    u32 exists;
    u32 item;
    u32 isEgg;
    u32 form;

    AcquireBoxMonLock(boxMon);
    exists = GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL);
    *species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
    isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
    item = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
    summary->species = *species;
    summary->gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL) + 1;
    if (isEgg != FALSE) {
        summary->level = 0;
    }
    ReleaseBoxMonLock(boxMon, TRUE);
    if (exists != FALSE) {
        GtsSelectMon_LoadMonIcon(*species, form, isEgg, slot, iconSprite, narc, icon);
        Sprite_SetDrawFlag(iconSprite, TRUE);
        if (item != ITEM_NONE) {
            Sprite_SetDrawFlag(itemSprite, TRUE);
            if (ItemIdIsMail((u16)item)) {
                Sprite_SetAnimCtrlSeq(itemSprite, 41);
            } else {
                Sprite_SetAnimCtrlSeq(itemSprite, 40);
            }
        } else {
            Sprite_SetDrawFlag(itemSprite, FALSE);
        }
    } else {
        Sprite_SetDrawFlag(iconSprite, FALSE);
        Sprite_SetDrawFlag(itemSprite, FALSE);
        icon->sprite = NULL;
    }
}

// Shows a PC box, or the party (GTS_BOX_PARTY); the icons are loaded in the next VBlank. Trading, the Pokemon the
// selected search result doesn't want are dimmed.
static void GtsSelectMon_ShowBox(GtsApp *app, int box) {
    PCStorage *pcStorage = app->args->pcStorage;
    NARC *narc;
    GtsMonIcon *icons;
    u16 i;
    u16 species[GTS_SELECT_MON_SLOT_COUNT];

    icons = Heap_AllocAtEnd(HEAP_ID_3, sizeof(GtsMonIcon) * GTS_SELECT_MON_SLOT_COUNT);
    app->monIcons = icons;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_61);
    if (box >= 0 && box < GTS_BOX_PARTY) {
        for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
            GtsSelectMon_SetSummaryLevel(PCStorage_GetMonByIndexPair(pcStorage, box, i), &app->boxSummaries[i]);
        }
        for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
            app->boxSummaries[i].species = SPECIES_NONE;
            GtsSelectMon_ShowSlotMon(PCStorage_GetMonByIndexPair(pcStorage, box, i), app->monIconSprites[i], app->heldItemSprites[i], &species[i], i, narc, &app->boxSummaries[i], &icons[i]);
            if (i < PARTY_SIZE) {
                Sprite_SetDrawFlag(app->capsuleSprites[i], FALSE);
            }
        }
        PCStorage_GetBoxName(pcStorage, box, app->boxName);
    } else {
        int count = Party_GetCount(app->args->party);
        for (i = 0; i < count; i++) {
            Pokemon *mon = Party_GetMonByIndex(app->args->party, i);
            BoxPokemon *boxMon = Mon_GetBoxMon(mon);
            GtsSelectMon_SetSummaryLevel(boxMon, &app->boxSummaries[i]);
            GtsSelectMon_ShowSlotMon(boxMon, app->monIconSprites[i], app->heldItemSprites[i], &species[i], i, narc, &app->boxSummaries[i], &icons[i]);
            if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
                Sprite_SetDrawFlag(app->capsuleSprites[i], TRUE);
            } else {
                Sprite_SetDrawFlag(app->capsuleSprites[i], FALSE);
            }
        }
        for (; i < GTS_SELECT_MON_SLOT_COUNT; i++) {
            app->boxSummaries[i].species = SPECIES_NONE;
            Sprite_SetDrawFlag(app->monIconSprites[i], FALSE);
            Sprite_SetDrawFlag(app->heldItemSprites[i], FALSE);
            icons[i].sprite = NULL;
            if (i < PARTY_SIZE) {
                Sprite_SetDrawFlag(app->capsuleSprites[i], FALSE);
            }
        }
        ReadMsgDataIntoString(app->gtsMsgData, msg_0775_00092, app->boxName); // IN PARTY
    }
    NARC_Delete(narc);
    FillWindowPixelBuffer(&app->boxNameWindow, 0);
    Gts_PrintAlignedFont0(&app->boxNameWindow, app->boxName, 0, 5, 1, MAKE_TEXT_COLOR(1, 2, 0));
    if (app->nextScreenArg == GTS_SELECT_MON_MODE_TRADE) {
        GtsSelectMon_DimUnwantedIcons(app->boxSummaries, app->monIconSprites, &app->searchResults[app->selectedResult].wanted, icons);
    }
    app->vblankTask = GtsSelectMon_TransferMonIcons;
}

BOOL Gts_IsPartyBox(int box) {
    return box == GTS_BOX_PARTY;
}

BoxPokemon *Gts_GetBoxMon(Party *party, PCStorage *pcStorage, int box, int slot) {
    if (Gts_IsPartyBox(box)) {
        if (slot > Party_GetCount(party) - 1) {
            return NULL;
        }
        return Mon_GetBoxMon(Party_GetMonByIndex(party, slot));
    }
    return PCStorage_GetMonByIndexPair(pcStorage, box, slot);
}

// The last Pokemon of the party can't be offered.
static BOOL GtsSelectMon_CanOffer(Party *party, PCStorage *pcStorage, int box, int slot) {
    if (Gts_IsPartyBox(box) && Party_GetCount(party) < 2) {
        return FALSE;
    }
    return TRUE;
}

static BOOL GtsSelectMon_HasSpecialRibbon(BoxPokemon *boxMon) {
    int sum = 0;
    int decrypted = AcquireBoxMonLock(boxMon);
    int i;
    for (i = 0; i < 10; i++) {
        sum += GetBoxMonData(boxMon, sGtsSpecialRibbons[i], NULL);
    }
    ReleaseBoxMonLock(boxMon, decrypted);
    return sum != 0;
}

static int GtsSelectMon_GetFormRestriction(BoxPokemon *boxMon) {
    int decrypted = AcquireBoxMonLock(boxMon);
    int species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    int form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
    ReleaseBoxMonLock(boxMon, decrypted);
    if (form > 0) {
        switch (species) {
        case SPECIES_ROTOM:
        case SPECIES_GIRATINA:
        case SPECIES_SHAYMIN:
            return GTS_FORM_SPECIAL;
        case SPECIES_PICHU:
            return GTS_FORM_SPIKY_PICHU;
        }
    }
    return GTS_FORM_ALLOWED;
}

static BOOL GtsSelectMon_HoldsGriseousOrb(BoxPokemon *boxMon) {
    int decrypted = AcquireBoxMonLock(boxMon);
    int item = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
    ReleaseBoxMonLock(boxMon, decrypted);
    return item == ITEM_GRISEOUS_ORB;
}

static int GtsSelectMon_GetSlotContent(Party *party, PCStorage *pcStorage, int box, int slot) {
    BoxPokemon *boxMon = Gts_GetBoxMon(party, pcStorage, box, slot);
    if (boxMon == NULL) {
        return GTS_SLOT_EMPTY;
    }
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL) == FALSE) {
        return GTS_SLOT_EMPTY;
    }
    if (GetBoxMonData(boxMon, MON_DATA_SANITY_IS_EGG, NULL) != FALSE) {
        return GTS_SLOT_EGG;
    }
    return GTS_SLOT_MON;
}

// gender 3 (either) and levels 0 match any.
static BOOL GtsSelectMon_MatchesWanted(GtsMonSummary *summary, GtsWantedMon *wanted) {
    if (summary->species != wanted->species) {
        return FALSE;
    }
    if (wanted->gender != 3 && wanted->gender != summary->gender) {
        return FALSE;
    }
    if (summary->level == 0) {
        return FALSE;
    }
    if (wanted->minLevel != 0 && wanted->minLevel > summary->level) {
        return FALSE;
    }
    if (wanted->maxLevel != 0 && wanted->maxLevel < summary->level) {
        return FALSE;
    }
    return TRUE;
}

static BOOL GtsSelectMon_BoxMonMatchesWanted(BoxPokemon *boxMon, GtsWantedMon *wanted) {
    GtsMonSummary summary;
    summary.species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    summary.gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL) + 1;
    summary.level = CalcBoxMonLevel(boxMon);
    return GtsSelectMon_MatchesWanted(&summary, wanted);
}

// The record sent with the trade: the offered Pokemon, its summary, and as the wanted Pokemon the species and gender
// of the search result's Pokemon.
static void GtsSelectMon_InitTradeRecord(GtsRecord *record, GtsApp *app) {
    GtsMonFields offered;
    GtsMonFields wanted;
    BoxPokemon *boxMon;

    offered.species = GetBoxMonData(app->selectedMon, MON_DATA_SPECIES, NULL);
    offered.gender = GetBoxMonData(app->selectedMon, MON_DATA_GENDER, NULL) + 1;
    offered.level = CalcBoxMonLevel(app->selectedMon);
    *(u16 *)&record->summary.species = offered.species;
    *(u16 *)&record->summary.gender = offered.halves[0]; // gender, level
    Gts_InitRecord(record, app);
    boxMon = Mon_GetBoxMon(&app->searchResults[app->selectedResult].mon);
    wanted.species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    wanted.gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL) + 1;
    wanted.level = 0;
    wanted.maxLevel = 0;
    *(u16 *)&record->wanted.species = wanted.species;
    *(u16 *)&record->wanted.gender = wanted.halves[0];   // gender, minLevel
    *(u16 *)&record->wanted.maxLevel = wanted.halves[1]; // maxLevel, unk_5
}

static void GtsSelectMon_DimUnwantedIcons(GtsMonSummary *summaries, Sprite **sprites, GtsWantedMon *wanted, GtsMonIcon *icons) {
    int i;
    for (i = 0; i < GTS_SELECT_MON_SLOT_COUNT; i++, summaries++, icons++) {
        if (summaries->species != SPECIES_NONE && GtsSelectMon_MatchesWanted(summaries, wanted) == FALSE) {
            icons->palette += 3;
        }
    }
}

BOOL Gts_MonHoldsMail(Pokemon *mon) {
    u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    if (ItemIdIsMail(item)) {
        return TRUE;
    }
    return FALSE;
}
