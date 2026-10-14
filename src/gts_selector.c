#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "gts_internal.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pokedex.h"
#include "sound_playback.h"
#include "sprite.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

// The selector of SEEK POKEMON (screen 4) and of the wanted Pokemon of a deposit (screen 6): a panel that slides in
// on the main screen and returns the chosen species, gender, level range or country (GtsSelector_Start's mode).
// Species: a grid of letter groups (ABC .. YZ), then the letters of the group, then the list of the species starting
// with the letter that the player has seen. Countries: the same letter groups plus ANY, the letters, then the list.
// Gender and level: a list. Groups and letters without any species seen (any country) are greyed out.
// GtsSelector_Main returns -1 while the player chooses.

typedef enum GtsSelectorMode {
    GTS_SELECTOR_MODE_SPECIES,
    GTS_SELECTOR_MODE_GENDER,
    GTS_SELECTOR_MODE_LEVEL,
    GTS_SELECTOR_MODE_COUNTRY,
} GtsSelectorMode;

// The panel pages (GtsSelector_AddWindows, GtsSelector_GetTouchedButton). The letter pages of species and countries
// share the hitboxes (5 and 7).
typedef enum GtsSelectorPage {
    GTS_SELECTOR_PAGE_SPECIES_LIST,
    GTS_SELECTOR_PAGE_GENDERS,
    GTS_SELECTOR_PAGE_LEVELS,
    GTS_SELECTOR_PAGE_COUNTRY_LIST,
    GTS_SELECTOR_PAGE_SPECIES_GROUPS,
    GTS_SELECTOR_PAGE_LETTERS,
    GTS_SELECTOR_PAGE_COUNTRY_GROUPS,
    GTS_SELECTOR_PAGE_COUNTRY_LETTERS,
} GtsSelectorPage;

typedef enum GtsSelectorState {
    GTS_SELECTOR_STATE_IDLE,
    GTS_SELECTOR_STATE_WAIT_SHOW,
    GTS_SELECTOR_STATE_WAIT_MOVE,
    GTS_SELECTOR_STATE_WAIT_HIDE,
    GTS_SELECTOR_STATE_OPEN_SPECIES_GROUPS,
    GTS_SELECTOR_STATE_HANDLE_SPECIES_GROUPS,
    GTS_SELECTOR_STATE_CLOSE_SPECIES_GROUPS,
    GTS_SELECTOR_STATE_REOPEN_SPECIES_GROUPS,
    GTS_SELECTOR_STATE_OPEN_SPECIES_LETTERS,
    GTS_SELECTOR_STATE_HANDLE_SPECIES_LETTERS,
    GTS_SELECTOR_STATE_CLOSE_SPECIES_LETTERS,
    GTS_SELECTOR_STATE_BACK_TO_SPECIES_LETTERS,
    GTS_SELECTOR_STATE_OPEN_SPECIES_LIST,
    GTS_SELECTOR_STATE_HANDLE_SPECIES_LIST,
    GTS_SELECTOR_STATE_CANCEL_SPECIES_LIST,
    GTS_SELECTOR_STATE_FINISH_SPECIES,
    GTS_SELECTOR_STATE_OPEN_COUNTRY_GROUPS,
    GTS_SELECTOR_STATE_HANDLE_COUNTRY_GROUPS,
    GTS_SELECTOR_STATE_CLOSE_COUNTRY_GROUPS,
    GTS_SELECTOR_STATE_REOPEN_COUNTRY_GROUPS,
    GTS_SELECTOR_STATE_OPEN_COUNTRY_LETTERS,
    GTS_SELECTOR_STATE_HANDLE_COUNTRY_LETTERS,
    GTS_SELECTOR_STATE_CLOSE_COUNTRY_LETTERS,
    GTS_SELECTOR_STATE_BACK_TO_COUNTRY_LETTERS,
    GTS_SELECTOR_STATE_OPEN_COUNTRY_LIST,
    GTS_SELECTOR_STATE_HANDLE_COUNTRY_LIST,
    GTS_SELECTOR_STATE_CANCEL_COUNTRY_LIST,
    GTS_SELECTOR_STATE_FINISH_COUNTRY,
    GTS_SELECTOR_STATE_OPEN_GENDERS,
    GTS_SELECTOR_STATE_HANDLE_GENDERS,
    GTS_SELECTOR_STATE_FINISH_GENDER,
    GTS_SELECTOR_STATE_OPEN_LEVELS,
    GTS_SELECTOR_STATE_HANDLE_LEVELS,
    GTS_SELECTOR_STATE_FINISH_LEVEL,
} GtsSelectorState;

struct GtsSelector {
    BgConfig *bgConfig;         // 0x00
    Window *windows;            // 0x04, the screen's windows[16]
    Window *messageWindow;      // 0x08
    Sprite *cursorSprite;       // 0x0C
    Sprite *rightArrowSprite;   // 0x10
    Sprite *leftArrowSprite;    // 0x14
    Sprite *screenCursorSprite; // 0x18, the screen's own cursor (may be NULL), hidden under the panel
    BgCellManager *cellManager; // 0x1C, slides the panel
    Pokedex *pokedex;           // 0x20
    MsgData *gtsMsgData;        // 0x24
    MsgData *speciesNames;      // 0x28
    MsgData *countryNames;      // 0x2C
    void *johtoDexFlags;        // 0x30
    ListMenuItem *listItems;    // 0x34
    MessagePrinter *printer;    // 0x38, of the page numbers
    s16 group;                  // 0x3C, letter group 0..8; -1 none, -2 ANY (countries)
    s16 letter;                 // 0x3E, in the group, -1 none
    s16 species;                // 0x40, the result of GTS_SELECTOR_MODE_SPECIES
    s16 country;                // 0x42, the result of GTS_SELECTOR_MODE_COUNTRY
    s8 gender;                  // 0x44, the result of GTS_SELECTOR_MODE_GENDER
    s8 levelRange;              // 0x45, the result of GTS_SELECTOR_MODE_LEVEL
    u8 filler_46[2];            // 0x46
    s32 cursor;                 // 0x48, s32 (not int) for matching
    int state;                  // 0x4C, GtsSelectorState
    int nextState;              // 0x50
    u32 bgId;                   // 0x54, of the windows
    s16 mode;                   // 0x58, GtsSelectorMode
    s16 page;                   // 0x5A, of the list
    int count;                  // 0x5C, of listItems
    int levelRangeKind;         // 0x60, see Gts_CreateLevelRangeListItems
    u8 enabled[26];             // 0x64, of the groups or letters shown
    u8 savedCursor;             // 0x7E, the grid cursor while on a button below the grid
}; // size: 0x80

typedef struct GtsSelectorWindowPosition {
    u8 x;
    u8 y;
} GtsSelectorWindowPosition;

typedef struct GtsSelectorWindowPosition3 {
    u8 x;
    u8 y;
    u8 unused;
} GtsSelectorWindowPosition3;

// {first letter, letter count} of the letter groups ABC .. YZ.
typedef struct GtsSelectorLetterGroup {
    int first;
    int count;
} GtsSelectorLetterGroup;

// Not declared in a public header (bg_cell_manager.c).
void BgCellManager_LoadCellTilemap(BgCellManager *manager, int index, int narcId, int fileId, int compressed);
void BgCellManager_DrawCell(BgCellManager *manager, int index);
void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y);
void BgCellManager_PutWindowTiles(BgCellManager *manager, int index, Window *window);

extern int (*const sGtsSelectorStates[])(GtsSelector *selector);
extern const GtsSelectorWindowPosition sGtsSelectorGenderWindowPositions[];
extern const GtsSelectorWindowPosition sGtsSelectorLevelWindowPositions[];
extern const GtsSelectorWindowPosition sGtsSelectorCountryWindowPositions[];
extern const GtsSelectorWindowPosition3 sGtsSelectorSpeciesWindowPositions[];
extern const GtsSelectorWindowPosition sGtsSelectorGridWindowPositions[];
// The cursor positions are {x, y} pairs; the asm labels the y column separately. The list cursors are in pixels, the
// grid cursors in tiles of the panel.
extern const u8 sGtsSelectorGenderCursorX[];
extern const u8 sGtsSelectorGenderCursorY[];
extern const u8 sGtsSelectorSpeciesCursorX[];
extern const u8 sGtsSelectorSpeciesCursorY[];
extern const u8 sGtsSelectorLevelCursorX[];
extern const u8 sGtsSelectorLevelCursorY[];
extern const u8 sGtsSelectorCountryCursorX[];
extern const u8 sGtsSelectorCountryCursorY[];
extern const u8 sGtsSelectorCountryGroupCursorX[];
extern const u8 sGtsSelectorCountryGroupCursorY[];
extern const u8 sGtsSelectorGroupCursorX[];
extern const u8 sGtsSelectorGroupCursorY[];
// The value of each row of the gender list: EITHER MON_GENDERLESS, then MON_MALE, MON_FEMALE, BACK -2.
extern const int sGtsSelectorGenders[];
// The cursor's next button for up, down, left, right on each button; the asm labels each column of the letter and
// country group tables separately.
extern const u8 sGtsSelectorLetterNextUp[];
extern const u8 sGtsSelectorLetterNextDown[];
extern const u8 sGtsSelectorLetterNextLeft[];
extern const u8 sGtsSelectorLetterNextRight[];
extern const u8 sGtsSelectorGroupNext[];
extern const u8 sGtsSelectorCountryGroupNextUp[];
extern const u8 sGtsSelectorCountryGroupNextDown[];
extern const u8 sGtsSelectorCountryGroupNextLeft[];
extern const u8 sGtsSelectorCountryGroupNextRight[];
extern const TouchscreenHitbox sGtsSelectorGroupHitboxes[];
extern const TouchscreenHitbox sGtsSelectorCountryGroupHitboxes[];
extern const TouchscreenHitbox sGtsSelectorListHitboxes[];
extern const TouchscreenHitbox sGtsSelectorGenderHitboxes[];
extern const TouchscreenHitbox sGtsSelectorCountryListHitboxes[];
extern const TouchscreenHitbox sGtsSelectorLetterHitboxes[];
// The first letter of each letter group.
extern const int gGtsSelectorGroupStarts[];
extern const GtsSelectorLetterGroup gGtsSelectorLetterGroups[];
// The same table read from its second word: ov70_02245F5C[group][0] is the letter count of the group.
extern const int ov70_02245F5C[][2];
// The message of each letter, A .. Z.
extern const int sGtsSelectorLetterMsgs[];

void GtsSelector_AddWindows(GtsSelector *selector, int page);
void GtsSelector_RemoveWindows(GtsSelector *selector, int page);
int GtsSelector_GetTouchedButton(GtsSelector *selector, u32 page);
int GtsSelector_HandleGroupInput(GtsSelector *selector, const u8 *enabled);
int GtsSelector_GetLetter(GtsSelector *selector, int button);
int GtsSelector_HandleLetterInput(GtsSelector *selector, const u8 *enabled);
int GtsSelector_CountPages(int count, int perPage);
int GtsSelector_SelectSpeciesButton(GtsSelector *selector, int button);
void GtsSelector_ChangeSpeciesPage(GtsSelector *selector, int direction);
int GtsSelector_HandleSpeciesListInput(GtsSelector *selector);
int GtsSelector_HandleGenderInput(GtsSelector *selector);
void GtsSelector_ChangeLevelPage(GtsSelector *selector, int direction);
int GtsSelector_SelectLevelButton(GtsSelector *selector, int button);
int GtsSelector_HandleLevelInput(GtsSelector *selector);
int GtsSelector_GetCountryGroup(GtsSelector *selector, int button);
int GtsSelector_HandleCountryGroupInput(GtsSelector *selector, const u8 *enabled);
int GtsSelector_SelectCountryButton(GtsSelector *selector, int button);
void GtsSelector_ChangeCountryPage(GtsSelector *selector, int direction);
int GtsSelector_HandleCountryListInput(GtsSelector *selector);
void GtsSelector_ShowGroups(GtsSelector *selector, int page, int y);
void GtsSelector_ShowGenders(GtsSelector *selector);
void GtsSelector_ShowLevels(GtsSelector *selector);
void GtsSelector_PrintString(BgCellManager *cellManager, Window *window, String *string, int x, u32 color);
int GtsSelector_Idle(GtsSelector *selector);
int GtsSelector_WaitShow(GtsSelector *selector);
int GtsSelector_WaitMove(GtsSelector *selector);
int GtsSelector_WaitHide(GtsSelector *selector);
int GtsSelector_OpenSpeciesGroups(GtsSelector *selector);
int GtsSelector_HandleSpeciesGroups(GtsSelector *selector);
int GtsSelector_CloseSpeciesGroups(GtsSelector *selector);
int GtsSelector_ReopenSpeciesGroups(GtsSelector *selector);
int GtsSelector_OpenSpeciesLetters(GtsSelector *selector);
int GtsSelector_HandleSpeciesLetters(GtsSelector *selector);
int GtsSelector_CloseSpeciesLetters(GtsSelector *selector);
int GtsSelector_BackToSpeciesLetters(GtsSelector *selector);
int GtsSelector_CountSeenSpecies(void *unused, Pokedex *pokedex, int count, const u16 *species);
int GtsSelector_CreateSpeciesListItems(ListMenuItem **items, MsgData *speciesNames, MsgData *unused2, void *unused3, int letter, Pokedex *pokedex);
void GtsSelector_PrintListPage(GtsSelector *selector, int page, int count);
void GtsSelector_PrintCountryPage(GtsSelector *selector, int page, int count);
int GtsSelector_OpenSpeciesList(GtsSelector *selector);
int GtsSelector_HandleSpeciesList(GtsSelector *selector);
int GtsSelector_CancelSpeciesList(GtsSelector *selector);
int GtsSelector_FinishSpecies(GtsSelector *selector);
int GtsSelector_OpenCountryGroups(GtsSelector *selector);
int GtsSelector_HandleCountryGroups(GtsSelector *selector);
int GtsSelector_CloseCountryGroups(GtsSelector *selector);
int GtsSelector_ReopenCountryGroups(GtsSelector *selector);
int GtsSelector_OpenCountryLetters(GtsSelector *selector);
int GtsSelector_HandleCountryLetters(GtsSelector *selector);
int GtsSelector_CloseCountryLetters(GtsSelector *selector);
int GtsSelector_BackToCountryLetters(GtsSelector *selector);
int GtsSelector_OpenCountryList(GtsSelector *selector);
int GtsSelector_HandleCountryList(GtsSelector *selector);
int GtsSelector_CancelCountryList(GtsSelector *selector);
int GtsSelector_FinishCountry(GtsSelector *selector);
int GtsSelector_OpenGenders(GtsSelector *selector);
int GtsSelector_HandleGenders(GtsSelector *selector);
int GtsSelector_FinishGender(GtsSelector *selector);
int GtsSelector_OpenLevels(GtsSelector *selector);
int GtsSelector_HandleLevels(GtsSelector *selector);
int GtsSelector_FinishLevel(GtsSelector *selector);
void GtsSelector_PrintMessage(BgCellManager *cellManager, MsgData *msgData, Window *window, int msgId, int x, u32 color);
void GtsSelector_PrintButtonLabel(BgCellManager *cellManager, MsgData *msgData, Window *window, int msgId);
void GtsSelector_PrintPageNumber(BgCellManager *cellManager, MessagePrinter *printer, Window *window, int page, int pageCount);
int GtsSelector_CountSeenSpeciesOfLetter(GtsSelector *selector, int letter);
BOOL GtsSelector_GroupHasSeenSpecies(GtsSelector *selector, int group);

void GtsSelector_AddWindows(GtsSelector *selector, int page) {
    int i;
    switch (page) {
    case GTS_SELECTOR_PAGE_SPECIES_GROUPS:
        for (i = 0; i < 9; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorGridWindowPositions[i].x, sGtsSelectorGridWindowPositions[i].y, 3, 2, 1, 0x30 + i * 6);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0x12F);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    case GTS_SELECTOR_PAGE_LETTERS:
        AddWindowParameterized(selector->bgConfig, &selector->windows[0], selector->bgId, 1, 2, 3, 2, 1, 0x30);
        FillWindowPixelBuffer(&selector->windows[0], 0x22);
        for (i = 1; i < 4; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorGridWindowPositions[i - 1].x, sGtsSelectorGridWindowPositions[i - 1].y, 2, 2, 1, 0x36 + (i - 1) * 4);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0x12F);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    case GTS_SELECTOR_PAGE_SPECIES_LIST:
        for (i = 0; i < 4; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorSpeciesWindowPositions[i].x, sGtsSelectorSpeciesWindowPositions[i].y, 8, 2, 1, 0x30 + i * 0x10);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[4], selector->bgId, 6, 14, 5, 1, 1, 0x70);
        FillWindowPixelBuffer(&selector->windows[4], 0x22);
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0x12F);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    case GTS_SELECTOR_PAGE_COUNTRY_GROUPS:
        for (i = 0; i < 9; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorGridWindowPositions[i].x, sGtsSelectorGridWindowPositions[i].y, 3, 2, 1, 0x30 + i * 6);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0x12F);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        AddWindowParameterized(selector->bgConfig, &selector->windows[15], selector->bgId, 2, 2, 9, 2, 1, 0x66);
        FillWindowPixelBuffer(&selector->windows[15], 0x22);
        break;
    case GTS_SELECTOR_PAGE_COUNTRY_LIST:
        for (i = 0; i < 5; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorCountryWindowPositions[i].x, sGtsSelectorCountryWindowPositions[i].y, 23, 2, 1, 0x30 + i * 0x2E);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[5], selector->bgId, 13, 16, 5, 1, 1, 0x116);
        FillWindowPixelBuffer(&selector->windows[5], 0x22);
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 24, 17, 6, 2, 1, 0x12F);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    case GTS_SELECTOR_PAGE_GENDERS:
        for (i = 0; i < 3; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorGenderWindowPositions[i].x, sGtsSelectorGenderWindowPositions[i].y, 8, 2, 1, 0x30 + i * 0x10);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0x60);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    case GTS_SELECTOR_PAGE_LEVELS:
        for (i = 0; i < 4; i++) {
            AddWindowParameterized(selector->bgConfig, &selector->windows[i], selector->bgId, sGtsSelectorLevelWindowPositions[i].x, sGtsSelectorLevelWindowPositions[i].y, 11, 2, 1, 0x30 + i * 0x16);
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
        }
        AddWindowParameterized(selector->bgConfig, &selector->windows[4], selector->bgId, 6, 14, 5, 1, 1, 0x88);
        FillWindowPixelBuffer(&selector->windows[4], 0x22);
        AddWindowParameterized(selector->bgConfig, &selector->windows[14], selector->bgId, 9, 17, 6, 2, 1, 0xA1);
        FillWindowPixelBuffer(&selector->windows[14], 0x22);
        break;
    }
}

void GtsSelector_RemoveWindows(GtsSelector *selector, int page) {
    // {y, x}: the order of the stack slots.
    s8 position[2];

    BgCellManager_GetCellPosition(selector->cellManager, 0, &position[1], &position[0]);
    switch (page) {
    case GTS_SELECTOR_PAGE_SPECIES_GROUPS: {
        int i;
        for (i = 0; i < 9; i++) {
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            RemoveWindow(&selector->windows[i]);
        }
        if (position[1] == 16) {
            FillBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 5, 17, 1, 15, 15, 16);
            BgCommitTilemapBufferToVram(selector->bgConfig, GF_BG_LYR_MAIN_2);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    case GTS_SELECTOR_PAGE_LETTERS: {
        int i;
        for (i = 0; i < 4; i++) {
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            RemoveWindow(&selector->windows[i]);
        }
        if (position[1] == 16) {
            FillBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 5, 17, 1, 15, 15, 16);
            BgCommitTilemapBufferToVram(selector->bgConfig, GF_BG_LYR_MAIN_2);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    case GTS_SELECTOR_PAGE_SPECIES_LIST: {
        int i;
        for (i = 0; i < 5; i++) {
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            RemoveWindow(&selector->windows[i]);
        }
        if (position[1] == 16) {
            FillBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 5, 17, 1, 15, 15, 16);
            BgCommitTilemapBufferToVram(selector->bgConfig, GF_BG_LYR_MAIN_2);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    case GTS_SELECTOR_PAGE_COUNTRY_GROUPS: {
        int i;
        for (i = 0; i < 9; i++) {
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            RemoveWindow(&selector->windows[i]);
        }
        if (position[1] == 16) {
            FillBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 5, 17, 1, 15, 15, 16);
            BgCommitTilemapBufferToVram(selector->bgConfig, GF_BG_LYR_MAIN_2);
        }
        RemoveWindow(&selector->windows[14]);
        RemoveWindow(&selector->windows[15]);
        break;
    }
    case GTS_SELECTOR_PAGE_COUNTRY_LIST: {
        int i;
        for (i = 0; i < 6; i++) {
            FillWindowPixelBuffer(&selector->windows[i], 0x22);
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            RemoveWindow(&selector->windows[i]);
        }
        if (position[1] == 16 || position[1] == 17) {
            FillBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 5, 18, 1, 14, 15, 16);
            BgCommitTilemapBufferToVram(selector->bgConfig, GF_BG_LYR_MAIN_2);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    case GTS_SELECTOR_PAGE_GENDERS: {
        int i;
        for (i = 0; i < 3; i++) {
            RemoveWindow(&selector->windows[i]);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    case GTS_SELECTOR_PAGE_LEVELS: {
        int i;
        for (i = 0; i < 5; i++) {
            RemoveWindow(&selector->windows[i]);
        }
        RemoveWindow(&selector->windows[14]);
        break;
    }
    }
}

// bgId: the layer of the windows; levelRangeKind: the level ranges of the level list.
GtsSelector *GtsSelector_New(GtsSelectorArgs *args, int bgId, int levelRangeKind) {
    GtsSelector *selector = Heap_Alloc(HEAP_ID_61, sizeof(GtsSelector));
    selector->bgConfig = args->bgConfig;
    selector->windows = args->windows;
    selector->messageWindow = args->messageWindow;
    selector->cursorSprite = args->sprites[0];
    selector->rightArrowSprite = args->sprites[1];
    selector->leftArrowSprite = args->sprites[2];
    selector->screenCursorSprite = args->sprites[3];
    selector->gtsMsgData = args->gtsMsgData;
    selector->speciesNames = args->speciesNames;
    selector->countryNames = args->countryNames;
    selector->pokedex = args->pokedex;
    selector->johtoDexFlags = args->johtoDexFlags;
    selector->bgId = bgId;
    selector->levelRangeKind = levelRangeKind;
    selector->group = 0;
    selector->letter = 0;
    selector->species = 0;
    selector->country = 0;
    selector->gender = 0;
    selector->levelRange = 0;
    selector->cursor = 0;
    selector->savedCursor = 0;
    selector->state = GTS_SELECTOR_STATE_IDLE;
    selector->cellManager = BgCellManager_New(selector->bgConfig, 2, 1, HEAP_ID_61);
    BgCellManager_InitCell(selector->cellManager, 0, 2, 32, 20);
    Gts_SetSpritePosition(selector->cursorSprite, 136, 40);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 47);
    selector->printer = MessagePrinter_New(15, 14, 2, HEAP_ID_61);
    return selector;
}

void GtsSelector_Start(GtsSelector *selector, int mode) {
    selector->mode = mode;
    switch (mode) {
    case GTS_SELECTOR_MODE_SPECIES:
        selector->cursor = 0;
        selector->savedCursor = 0;
        selector->group = 0;
        selector->letter = 0;
        selector->state = GTS_SELECTOR_STATE_OPEN_SPECIES_GROUPS;
        break;
    case GTS_SELECTOR_MODE_COUNTRY:
        selector->cursor = 0;
        selector->savedCursor = 0;
        selector->group = 0;
        selector->letter = 0;
        selector->state = GTS_SELECTOR_STATE_OPEN_COUNTRY_GROUPS;
        break;
    case GTS_SELECTOR_MODE_GENDER:
        selector->cursor = 0;
        selector->savedCursor = 0;
        selector->state = GTS_SELECTOR_STATE_OPEN_GENDERS;
        break;
    case GTS_SELECTOR_MODE_LEVEL:
        selector->cursor = 0;
        selector->savedCursor = 0;
        selector->state = GTS_SELECTOR_STATE_OPEN_LEVELS;
        break;
    }
}

void GtsSelector_Delete(GtsSelector *selector) {
    MessagePrinter_Delete(selector->printer);
    BgCellManager_Delete(selector->cellManager);
    Heap_Free(selector);
}

// Returns -1 while the player chooses, -2 for BACK, else the choice: the species, the gender (MON_MALE, MON_FEMALE,
// MON_GENDERLESS for EITHER), the index of the level range, or 1 + the country's index in the country list (0: ANY).
int GtsSelector_Main(GtsSelector *selector) {
    int result = sGtsSelectorStates[selector->state](selector);
    BgCellManager_Update(selector->cellManager);
    return result;
}

#pragma force_active on

// The button touched on the page, -1 for none. Pages above 7 return the selector (never used).
int GtsSelector_GetTouchedButton(GtsSelector *selector, u32 page) {
    int button = (int)selector;
    switch (page) {
    case GTS_SELECTOR_PAGE_SPECIES_GROUPS:
        return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorGroupHitboxes);
    case GTS_SELECTOR_PAGE_COUNTRY_GROUPS:
        return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorCountryGroupHitboxes);
    case GTS_SELECTOR_PAGE_SPECIES_LIST:
    case GTS_SELECTOR_PAGE_LEVELS:
        return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorListHitboxes);
    case GTS_SELECTOR_PAGE_GENDERS:
        return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorGenderHitboxes);
    case GTS_SELECTOR_PAGE_COUNTRY_LIST:
        return TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorCountryListHitboxes);
    case GTS_SELECTOR_PAGE_LETTERS:
    case GTS_SELECTOR_PAGE_COUNTRY_LETTERS:
        button = TouchscreenHitbox_FindRectAtTouchNew(sGtsSelectorLetterHitboxes);
    }
    return button;
}

#pragma force_active reset

// The 3x3 grid of letter groups (buttons 0..8) and BACK (9). Returns the group, -2 for BACK, -1 for none; groups
// with enabled[group] FALSE cannot be chosen.
int GtsSelector_HandleGroupInput(GtsSelector *selector, const u8 *enabled) {
    int prevCursor = selector->cursor;
    u32 button;
    u32 keys;

    if (prevCursor < 9) {
        selector->savedCursor = prevCursor;
    }
    keys = gSystem.newAndRepeatedKeys;
    if (keys & PAD_KEY_UP) {
        selector->cursor = sGtsSelectorGroupNext[selector->cursor * 4];
    } else if (keys & PAD_KEY_DOWN) {
        selector->cursor = sGtsSelectorGroupNext[selector->cursor * 4 + 1];
    } else if (keys & PAD_KEY_LEFT) {
        selector->cursor = sGtsSelectorGroupNext[selector->cursor * 4 + 2];
    } else if (keys & PAD_KEY_RIGHT) {
        selector->cursor = sGtsSelectorGroupNext[selector->cursor * 4 + 3];
    }

    // Back into the grid from BACK: the column the cursor left, in the bottom (up) or top (down) row.
    if (prevCursor >= 9 && selector->cursor < 9) {
        keys = gSystem.newAndRepeatedKeys;
        if (keys & PAD_KEY_UP) {
            selector->cursor = selector->savedCursor;
            if (selector->cursor + 4 < 9) {
                do {
                    selector->cursor += 4;
                } while (selector->cursor + 4 < 9);
            }
        } else if (keys & PAD_KEY_DOWN) {
            selector->cursor = selector->savedCursor;
            if (selector->cursor - 4 >= 0) {
                do {
                    selector->cursor -= 4;
                } while (selector->cursor - 4 >= 0);
            }
        }
    }

    if (prevCursor != selector->cursor) {
        PlaySE(SEQ_SE_DP_SELECT);
        // The y is read from the x label (+1).
        Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorGroupCursorX[selector->cursor * 2] + 16) * 8, sGtsSelectorGroupCursorX[selector->cursor * 2 + 1] * 8);
        if (selector->cursor == 9) {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 61);
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_SPECIES_GROUPS);
    if (button != -1) {
        if (button == 9) {
            PlaySE(SEQ_SE_DP_SELECT);
            return -2;
        }
        GF_ASSERT(button < 9);
        if (enabled == NULL || enabled[button] != FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return button;
        }
        goto none;
    }

    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_A) {
        if (selector->cursor == 9) {
            return -2;
        }
        GF_ASSERT(selector->cursor < 9);
        if (enabled == NULL || enabled[selector->cursor] != FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return selector->cursor;
        }
    } else if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
none:
    return -1;
}

#pragma force_active on

// The letter of a button of the letter page: -1 past the group's letters, -2 for BACK (3).
int GtsSelector_GetLetter(GtsSelector *selector, int button) {
    int group;
    if (button != 3) {
        group = selector->group;
        if (group <= 8) {
            if (button >= ov70_02245F5C[group][0]) {
                button = -1;
            }
            return button;
        }
        return button;
    } else {
        return -2;
    }
}

// The letters of a group (buttons 0..2) and BACK (3). Returns the letter in the group, -2 for BACK, -1 for none;
// letters with enabled[first letter of the group + letter] FALSE cannot be chosen.
int GtsSelector_HandleLetterInput(GtsSelector *selector, const u8 *enabled) {
    int prevCursor = selector->cursor;
    int button;

    if (prevCursor < 3) {
        selector->savedCursor = prevCursor;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        selector->cursor = sGtsSelectorLetterNextUp[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        selector->cursor = sGtsSelectorLetterNextDown[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        selector->cursor = sGtsSelectorLetterNextLeft[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        selector->cursor = sGtsSelectorLetterNextRight[selector->cursor * 4];
    }
    if (prevCursor == 3 && selector->cursor < 3) {
        selector->cursor = selector->savedCursor;
    }
    if (prevCursor != selector->cursor) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor == 3) {
            Gts_SetSpritePosition(selector->cursorSprite, 192, 136);
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorGroupCursorX[selector->cursor * 2] + 16) * 8, sGtsSelectorGroupCursorY[selector->cursor * 2] * 8);
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 47);
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_LETTERS);
    if (button != -1) {
        button = GtsSelector_GetLetter(selector, button);
        if (button == -1) {
            return button;
        }
        // enabled[first letter + button], indexed the other way round.
        if (button != -2 && enabled && !gGtsSelectorGroupStarts[selector->group][enabled + button]) {
            goto none;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return button;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        button = GtsSelector_GetLetter(selector, selector->cursor);
        if (button == -1) {
            return button;
        }
        // enabled[first letter + button], indexed the other way round.
        if (button != -2 && enabled && !gGtsSelectorGroupStarts[selector->group][enabled + button]) {
            goto none;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return button;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
none:
    return -1;
}

#pragma force_active reset

// At least one page, also for an empty list.
int GtsSelector_CountPages(int count, int perPage) {
    int add;
    if (count == 0) {
        return 1;
    }
    add = perPage - 1;
    return (count + add) / perPage;
}

// Buttons 0..3 the rows, 4 BACK, 5 and 6 the page arrows.
int GtsSelector_SelectSpeciesButton(GtsSelector *selector, int button) {
    int index;
    switch (button) {
    case 4:
        return -2;
    case 5:
        GtsSelector_ChangeSpeciesPage(selector, -1);
        return -1;
    case 6:
        GtsSelector_ChangeSpeciesPage(selector, 1);
        return -1;
    }
    index = button + selector->page * 4;
    if (index < selector->count) {
        PlaySE(SEQ_SE_DP_SELECT);
        return selector->listItems[index].value;
    }
    return -1;
}

void GtsSelector_ChangeSpeciesPage(GtsSelector *selector, int direction) {
    int lastPage = GtsSelector_CountPages(selector->count, 4) - 1;
    int pageCount;
    if (lastPage == 0) {
        return;
    }
    if (direction < 0) {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->leftArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->leftArrowSprite, 39);
        if (selector->page != 0) {
            selector->page--;
        } else {
            selector->page = lastPage;
        }
    } else {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->rightArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->rightArrowSprite, 38);
        if (selector->page < lastPage) {
            selector->page++;
        } else {
            selector->page = 0;
        }
    }
    GtsSelector_PrintListPage(selector, selector->page, selector->count);
    pageCount = GtsSelector_CountPages(selector->count, 4);
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[4], selector->page, pageCount);
}

// Rows 0..3 and BACK (4); left and right turn the page.
int GtsSelector_HandleSpeciesListInput(GtsSelector *selector) {
    int prevCursor = selector->cursor;
    int button;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (selector->cursor != 0) {
            selector->cursor--;
        } else {
            selector->cursor = 4;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        if (selector->cursor != 4) {
            selector->cursor++;
        } else {
            selector->cursor = 0;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        GtsSelector_ChangeSpeciesPage(selector, -1);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        GtsSelector_ChangeSpeciesPage(selector, 1);
    }

    if (prevCursor != selector->cursor) {
        int cursor;
        PlaySE(SEQ_SE_DP_SELECT);
        cursor = selector->cursor;
        Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorSpeciesCursorX[cursor * 2], sGtsSelectorSpeciesCursorY[cursor * 2]);
        if (selector->cursor == 4) {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_SPECIES_LIST);
    if (button != -1) {
        return GtsSelector_SelectSpeciesButton(selector, button);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return GtsSelector_SelectSpeciesButton(selector, selector->cursor);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
    return -1;
}

// EITHER, male, female and BACK.
int GtsSelector_HandleGenderInput(GtsSelector *selector) {
    int prevCursor = selector->cursor;
    int button;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor != 0) {
            selector->cursor--;
        } else {
            selector->cursor = 3;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor != 3) {
            selector->cursor++;
        } else {
            selector->cursor = 0;
        }
    }

    if (prevCursor != selector->cursor) {
        int cursor = selector->cursor;
        Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorGenderCursorX[cursor * 2], sGtsSelectorGenderCursorY[cursor * 2]);
        if (selector->cursor == 3) {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_GENDERS);
    if (button != -1) {
        PlaySE(SEQ_SE_DP_SELECT);
        return sGtsSelectorGenders[button];
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_SELECT);
        return sGtsSelectorGenders[selector->cursor];
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
    return -1;
}

// The level lists have 3 pages.
void GtsSelector_ChangeLevelPage(GtsSelector *selector, int direction) {
    int pageCount;
    if (direction < 0) {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->leftArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->leftArrowSprite, 39);
        if (selector->page != 0) {
            selector->page--;
        } else {
            selector->page = 2;
        }
    } else {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->rightArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->rightArrowSprite, 38);
        if (selector->page < 2) {
            selector->page++;
        } else {
            selector->page = 0;
        }
    }
    GtsSelector_PrintListPage(selector, selector->page, selector->count);
    pageCount = GtsSelector_CountPages(selector->count, 4);
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[4], selector->page, pageCount);
}

// Buttons as GtsSelector_SelectSpeciesButton. Returns the value of the button's item of the first page (the row,
// as the items' values are their indexes); GtsSelector_HandleLevels adds the page.
int GtsSelector_SelectLevelButton(GtsSelector *selector, int button) {
    int index;
    switch (button) {
    case 4:
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    case 5:
        GtsSelector_ChangeLevelPage(selector, -1);
        return -1;
    case 6:
        GtsSelector_ChangeLevelPage(selector, 1);
        return -1;
    }
    index = button + selector->page * 4;
    if (index < selector->count) {
        PlaySE(SEQ_SE_DP_SELECT);
        return selector->listItems[button].value;
    }
    return -1;
}

#pragma force_active on

int GtsSelector_HandleLevelInput(GtsSelector *selector) {
    int prevCursor = selector->cursor;
    int button;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor) {
            selector->cursor--;
        } else {
            selector->cursor = 4;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor != 4) {
            selector->cursor++;
        } else {
            selector->cursor = 0;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        GtsSelector_ChangeLevelPage(selector, -1);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        GtsSelector_ChangeLevelPage(selector, 1);
    }
    if (prevCursor != selector->cursor) {
        Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorLevelCursorX[selector->cursor * 2], sGtsSelectorLevelCursorY[selector->cursor * 2]);
        if (selector->cursor == 4) {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
        }
    }
    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_LEVELS);
    if (button != -1) {
        return GtsSelector_SelectLevelButton(selector, button);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return GtsSelector_SelectLevelButton(selector, selector->cursor);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
    return -1;
}

#pragma force_active reset

// Buttons of the country groups: 0..8 the groups, 9 BACK (-2), 10 ANY.
int GtsSelector_GetCountryGroup(GtsSelector *selector, int button) {
    switch (button) {
    case 9:
        return -2;
    case 10:
        return 10;
    default:
        return button;
    }
}

// The 3x3 grid of letter groups, BACK (9) and ANY (10). Returns the group, 10 for ANY, -2 for BACK, -1 for none.
int GtsSelector_HandleCountryGroupInput(GtsSelector *selector, const u8 *enabled) {
    int prevCursor = selector->cursor;
    int button;

    if (prevCursor < 9) {
        selector->savedCursor = prevCursor;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        selector->cursor = sGtsSelectorCountryGroupNextUp[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        selector->cursor = sGtsSelectorCountryGroupNextDown[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        selector->cursor = sGtsSelectorCountryGroupNextLeft[selector->cursor * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        selector->cursor = sGtsSelectorCountryGroupNextRight[selector->cursor * 4];
    }

    if (prevCursor >= 9 && selector->cursor < 9) {
        if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
            selector->cursor = selector->savedCursor;
            if (selector->cursor + 4 < 9) {
                do {
                    selector->cursor += 4;
                } while (selector->cursor + 4 < 9);
            }
        } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
            selector->cursor = selector->savedCursor;
            if (selector->cursor - 4 >= 0) {
                do {
                    selector->cursor -= 4;
                } while (selector->cursor - 4 >= 0);
            }
        }
    }

    if (prevCursor != selector->cursor) {
        int cursor;
        PlaySE(SEQ_SE_DP_SELECT);
        cursor = selector->cursor;
        Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorCountryGroupCursorX[cursor * 2] + 16) << 3, sGtsSelectorCountryGroupCursorY[cursor * 2] << 3);
        switch (selector->cursor) {
        case 9:
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
            break;
        case 10:
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
            break;
        default:
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 61);
            break;
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_COUNTRY_GROUPS);
    if (button != -1) {
        button = GtsSelector_GetCountryGroup(selector, button);
        if (button == -2 || button == 11 || enabled == NULL || enabled[button] != FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return button;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        button = GtsSelector_GetCountryGroup(selector, selector->cursor);
        if (button == -2 || button == 11 || enabled == NULL || enabled[button] != FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return button;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
    return -1;
}

// Buttons 0..4 the rows, 5 BACK, 6 and 7 the page arrows.
int GtsSelector_SelectCountryButton(GtsSelector *selector, int button) {
    int index;
    switch (button) {
    case 5:
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    case 6:
        GtsSelector_ChangeCountryPage(selector, -1);
        return -1;
    case 7:
        GtsSelector_ChangeCountryPage(selector, 1);
        return -1;
    }
    index = button + selector->page * 5;
    if (index < selector->count) {
        PlaySE(SEQ_SE_DP_SELECT);
        return selector->listItems[index].value;
    }
    return -1;
}

void GtsSelector_ChangeCountryPage(GtsSelector *selector, int direction) {
    int lastPage = GtsSelector_CountPages(selector->count, 5) - 1;
    int pageCount;
    if (lastPage == 0) {
        return;
    }
    if (direction < 0) {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->leftArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->leftArrowSprite, 39);
        if (selector->page != 0) {
            selector->page--;
        } else {
            selector->page = lastPage;
        }
    } else {
        PlaySE(SEQ_SE_DP_SELECT78);
        Sprite_SetAnimActiveFlag(selector->rightArrowSprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->rightArrowSprite, 38);
        if (selector->page < lastPage) {
            selector->page++;
        } else {
            selector->page = 0;
        }
    }
    GtsSelector_PrintCountryPage(selector, selector->page, selector->count);
    pageCount = GtsSelector_CountPages(selector->count, 5);
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[5], selector->page, pageCount);
}

// Rows 0..4 and BACK (5); left and right turn the page.
int GtsSelector_HandleCountryListInput(GtsSelector *selector) {
    int prevCursor = selector->cursor;
    int button;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor != 0) {
            selector->cursor--;
        } else {
            selector->cursor = 5;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (selector->cursor != 5) {
            selector->cursor++;
        } else {
            selector->cursor = 0;
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        GtsSelector_ChangeCountryPage(selector, -1);
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        GtsSelector_ChangeCountryPage(selector, 1);
    }

    if (prevCursor != selector->cursor) {
        int cursor = selector->cursor;
        Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorCountryCursorX[cursor * 2], sGtsSelectorCountryCursorY[cursor * 2]);
        if (selector->cursor == 5) {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 48);
        } else {
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 50);
        }
    }

    button = GtsSelector_GetTouchedButton(selector, GTS_SELECTOR_PAGE_COUNTRY_LIST);
    if (button != -1) {
        return GtsSelector_SelectCountryButton(selector, button);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return GtsSelector_SelectCountryButton(selector, selector->cursor);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return -2;
    }
    return -1;
}

#pragma force_active on

// The letter groups of species (GTS_SELECTOR_PAGE_SPECIES_GROUPS) or countries (GTS_SELECTOR_PAGE_COUNTRY_GROUPS,
// with ANY), the panel at row y.
void GtsSelector_ShowGroups(GtsSelector *selector, int page, int y) {
    int fileId;
    if (page == GTS_SELECTOR_PAGE_COUNTRY_GROUPS) {
        fileId = 33;
    } else {
        fileId = 34;
    }
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, fileId, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, page);
    MI_CpuFill8(selector->enabled, TRUE, 26);
    int i;
    for (i = 0; i < 9; i++) {
        String *string = NewString_ReadMsgData(selector->gtsMsgData, i + 110); // ABC .. YZ
        int color;
        if (page == GTS_SELECTOR_PAGE_SPECIES_GROUPS) {
            if (GtsSelector_GroupHasSeenSpecies(selector, i) == TRUE) {
                color = MAKE_TEXT_COLOR(15, 14, 2);
                selector->enabled[i] = TRUE;
            } else {
                color = MAKE_TEXT_COLOR(8, 9, 2);
                selector->enabled[i] = FALSE;
            }
        } else {
            if (GtsSelector_HasCountriesInGroup(selector, i) == TRUE) {
                color = MAKE_TEXT_COLOR(15, 14, 2);
                selector->enabled[i] = TRUE;
            } else {
                color = MAKE_TEXT_COLOR(8, 9, 2);
                selector->enabled[i] = FALSE;
            }
        }
        GtsSelector_PrintString(selector->cellManager, &selector->windows[i], string, 2, color);
        String_Delete(string);
    }
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68); // BACK
    if (page == GTS_SELECTOR_PAGE_COUNTRY_GROUPS) {
        String *string = NewString_ReadMsgData(selector->gtsMsgData, 174); // ANY
        GtsSelector_PrintString(selector->cellManager, &selector->windows[15], string, 2, MAKE_TEXT_COLOR(15, 14, 2));
        String_Delete(string);
    }
    BgCellManager_DrawCellAt(selector->cellManager, 0, (s8)y, 0);
    BgCellManager_DrawCell(selector->cellManager, 0);
}

#pragma force_active reset

// EITHER, male, female.
void GtsSelector_ShowGenders(GtsSelector *selector) {
    int i;
    int offset;
    String *string;
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 31, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_GENDERS);
    // A byte offset into windows next to the index.
    i = 0;
    offset = i;
    do {
        string = NewString_ReadMsgData(selector->gtsMsgData, i + 124);
        GtsSelector_PrintString(selector->cellManager, (Window *)((u8 *)selector->windows + offset), string, 2, MAKE_TEXT_COLOR(15, 14, 2));
        String_Delete(string);
        i++;
        offset += sizeof(Window);
    } while (i < 3);
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 32, 0);
    BgCellManager_DrawCell(selector->cellManager, 0);
    BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 4);
}

void GtsSelector_ShowLevels(GtsSelector *selector) {
    int cursor;
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 35, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_LEVELS);
    if (selector->levelRangeKind == 1) {
        selector->count = Gts_CreateLevelRangeListItems(&selector->listItems, selector->gtsMsgData, 1);
    } else if (selector->levelRangeKind == 0) {
        selector->count = Gts_CreateLevelRangeListItems(&selector->listItems, selector->gtsMsgData, 0);
    }
    GtsSelector_PrintListPage(selector, 0, selector->count);
    selector->page = 0;
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[4], selector->page, GtsSelector_CountPages(selector->count, 4));
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 32, 0);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
    selector->cursor = 0;
    selector->savedCursor = 0;
    // The cursor is read back (volatile) instead of reusing the 0 just stored.
    cursor = *(volatile int *)&selector->cursor;
    Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorSpeciesCursorX[cursor * 2], sGtsSelectorSpeciesCursorY[cursor * 2]);
    BgCellManager_DrawCell(selector->cellManager, 0);
    BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 4);
    selector->state = GTS_SELECTOR_STATE_HANDLE_LEVELS;
}

void GtsSelector_PrintString(BgCellManager *cellManager, Window *window, String *string, int x, u32 color) {
    AddTextPrinterParameterizedWithColor(window, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(cellManager, 0, window);
}

// The selector's states. They return -1 until a choice is made.

int GtsSelector_Idle(GtsSelector *selector) {
    return -1;
}

#pragma force_active on

// The panel slid in: show the cursor (and the page arrows of the level list).
int GtsSelector_WaitShow(GtsSelector *selector) {
    if (BgCellManager_IsCellMoving(selector->cellManager, 0) == FALSE) {
        Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
        if (selector->mode == GTS_SELECTOR_MODE_LEVEL) {
            Gts_SetSpritePosition(selector->rightArrowSprite, 228, 120);
            Gts_SetSpritePosition(selector->leftArrowSprite, 154, 120);
            Sprite_SetDrawFlag(selector->rightArrowSprite, TRUE);
            Sprite_SetDrawFlag(selector->leftArrowSprite, TRUE);
        }
        selector->state = selector->nextState;
    }
    return -1;
}

#pragma force_active reset

int GtsSelector_WaitMove(GtsSelector *selector) {
    if (!BgCellManager_IsCellMoving(selector->cellManager, 0)) {
        selector->state = selector->nextState;
    }
    return -1;
}

int GtsSelector_WaitHide(GtsSelector *selector) {
    if (!BgCellManager_IsCellMoving(selector->cellManager, 0)) {
        BgCellManager_EraseCell(selector->cellManager, 0);
        selector->state = selector->nextState;
    }
    return -1;
}

int GtsSelector_OpenSpeciesGroups(GtsSelector *selector) {
    GtsSelector_ShowGroups(selector, GTS_SELECTOR_PAGE_SPECIES_GROUPS, 32);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 61);
    Gts_SetSpritePosition(selector->cursorSprite, 136, 40);
    BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 4);
    selector->state = GTS_SELECTOR_STATE_WAIT_SHOW;
    selector->nextState = GTS_SELECTOR_STATE_HANDLE_SPECIES_GROUPS;
    selector->group = -1;
    return -1;
}

// The gotos keep the block order (structured code compiles differently).
int GtsSelector_HandleSpeciesGroups(GtsSelector *selector) {
    u32 group = GtsSelector_HandleGroupInput(selector, selector->enabled);
    if (group == (u32)-2) {
        goto cancel;
    }
    if (group > 8) {
        goto done;
    }
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_CLOSE_SPECIES_GROUPS;
    selector->group = group;
    goto done;
cancel:
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
    selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
    selector->nextState = GTS_SELECTOR_STATE_CLOSE_SPECIES_GROUPS;
    selector->group = -1;
done:
    return -1;
}

// The gotos keep the block order (structured code compiles differently).
int GtsSelector_CloseSpeciesGroups(GtsSelector *selector) {
    int state;
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_SPECIES_GROUPS);
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    if (selector->group < 0) {
        goto cancel;
    }
    GF_ASSERT(selector->group <= 8);
    // A group of one letter skips the letters.
    if (ov70_02245F5C[selector->group][0] == 1) {
        selector->letter = 0;
        state = GTS_SELECTOR_STATE_OPEN_SPECIES_LIST;
    } else {
        state = GTS_SELECTOR_STATE_OPEN_SPECIES_LETTERS;
    }
    goto setState;
cancel:
    return -2;
setState:
    selector->state = state;
    return -1;
}

#pragma force_active on

#pragma opt_strength_reduction off

int GtsSelector_ReopenSpeciesGroups(GtsSelector *selector) {
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 34, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_SPECIES_GROUPS);
    MI_CpuFill8(selector->enabled, TRUE, 26);
    // A byte offset into windows next to the index.
    int i = 0;
    int offset = i;
    for (; i < 9; i++, offset += sizeof(Window)) {
        String *string = NewString_ReadMsgData(selector->gtsMsgData, i + 110);
        u32 color;
        int enabled;
        u8 *dest;
        // dest is &enabled[i], computed from the selector moved by i bytes.
        if (GtsSelector_GroupHasSeenSpecies(selector, i) == TRUE) {
            color = MAKE_TEXT_COLOR(15, 14, 2);
            dest = ((GtsSelector *)((u8 *)selector + i))->enabled;
            enabled = TRUE;
        } else {
            color = MAKE_TEXT_COLOR(8, 9, 2);
            dest = ((GtsSelector *)((u8 *)selector + i))->enabled;
            enabled = FALSE;
        }
        *dest = enabled;
        GtsSelector_PrintString(selector->cellManager, (Window *)((u8 *)selector->windows + offset), string, 2, color);
        String_Delete(string);
    }
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 16, 0);
    BgCellManager_DrawCell(selector->cellManager, 0);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 61);
    Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorGroupCursorX[selector->group * 2] + 16) * 8, sGtsSelectorGroupCursorY[selector->group * 2] * 8);
    selector->cursor = selector->group;
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    selector->state = GTS_SELECTOR_STATE_HANDLE_SPECIES_GROUPS;
    return -1;
}

#pragma opt_strength_reduction reset

// The group's letters (A, B, C ...), greyed out without any species seen.
int GtsSelector_OpenSpeciesLetters(GtsSelector *selector) {
    int i;
    int color;
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 32, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_LETTERS);
    MI_CpuFill8(selector->enabled, TRUE, 26);
    GtsSelector_PrintMessage(selector->cellManager, selector->gtsMsgData, selector->windows, selector->group + 110, 2, MAKE_TEXT_COLOR(15, 14, 2));
    i = 1;
    if (i <= ov70_02245F5C[selector->group][0]) {
        do {
            if (GtsSelector_CountSeenSpeciesOfLetter(selector, gGtsSelectorGroupStarts[selector->group] + i - 1) > 0) {
                color = MAKE_TEXT_COLOR(15, 14, 2);
                selector->enabled[gGtsSelectorGroupStarts[selector->group] + i - 1] = TRUE;
            } else {
                color = MAKE_TEXT_COLOR(8, 9, 2);
                selector->enabled[gGtsSelectorGroupStarts[selector->group] + i - 1] = FALSE;
            }
            GtsSelector_PrintMessage(selector->cellManager, selector->gtsMsgData, &selector->windows[i], sGtsSelectorLetterMsgs[gGtsSelectorLetterGroups[selector->group].first + i - 1], 5, color);
            i++;
        } while (i <= gGtsSelectorLetterGroups[selector->group].count);
    }
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 16, 0);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 47);
    if (selector->letter < 0) {
        selector->cursor = 0;
    } else {
        selector->cursor = selector->letter;
    }
    Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorGroupCursorX[selector->cursor * 2] + 16) * 8, sGtsSelectorGroupCursorY[selector->cursor * 2] * 8);
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    selector->state = GTS_SELECTOR_STATE_HANDLE_SPECIES_LETTERS;
    return -1;
}

#pragma force_active reset

// The gotos keep the block order (structured code compiles differently).
int GtsSelector_HandleSpeciesLetters(GtsSelector *selector) {
    int letter = GtsSelector_HandleLetterInput(selector, selector->enabled);
    switch (letter) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        goto accepted;
    default:
        break;
    }
    if (letter == -2) {
        goto cancel;
    }
    goto done;
accepted:
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_CLOSE_SPECIES_LETTERS;
    selector->letter = letter;
    goto done;
cancel:
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_CLOSE_SPECIES_LETTERS;
    selector->letter = -1;
done:
    return -1;
}

int GtsSelector_CloseSpeciesLetters(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_LETTERS);
    if (selector->letter >= 0) {
        selector->state = GTS_SELECTOR_STATE_OPEN_SPECIES_LIST;
    } else {
        selector->state = GTS_SELECTOR_STATE_REOPEN_SPECIES_GROUPS;
    }
    return -1;
}

int GtsSelector_BackToSpeciesLetters(GtsSelector *selector) {
    selector->state = GTS_SELECTOR_STATE_OPEN_SPECIES_LETTERS;
    return -1;
}

#pragma force_active on

int GtsSelector_CountSeenSpecies(void *unused, Pokedex *pokedex, int count, const u16 *species) {
    int i = 0;
    int seen = 0;
    if (count > 0) {
        do {
            if (Pokedex_CheckMonSeenFlag(pokedex, *species)) {
                seen++;
            }
            species++;
            i++;
        } while (i < count);
    }
    return seen;
}

#pragma force_active reset

// The seen species starting with the letter; returns their count.
int GtsSelector_CreateSpeciesListItems(ListMenuItem **items, MsgData *speciesNames, MsgData *unused2, void *unused3, int letter, Pokedex *pokedex) {
    int speciesCount;
    int i, count;
    u16 *species = Gts_LoadSpeciesByLetter(HEAP_ID_61, letter, (u32 *)&speciesCount); // compared signed below
    u16 *entry;
    count = GtsSelector_CountSeenSpecies(unused3, pokedex, speciesCount, species);
    *items = ListMenuItems_New(count + 1, HEAP_ID_61);
    i = 0;
    if (speciesCount > 0) {
        entry = species;
        do {
            if (Pokedex_CheckMonSeenFlag(pokedex, *entry)) {
                ListMenuItems_AppendFromMsgData(*items, speciesNames, *entry, *entry);
            }
            i++;
            entry++;
        } while (i < (int)speciesCount);
    }
    Heap_Free(species);
    return count;
}

#pragma force_active on

// 4 rows of the list (species or level ranges).
void GtsSelector_PrintListPage(GtsSelector *selector, int page, int count) {
    int i;
    for (i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&selector->windows[i], 0x22);
        if (page * 4 + i < count) {
            GtsSelector_PrintString(selector->cellManager, &selector->windows[i], selector->listItems[page * 4 + i].text, 0, MAKE_TEXT_COLOR(15, 14, 2));
        } else {
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            BgCellManager_PutWindowTiles(selector->cellManager, 0, &selector->windows[i]);
        }
    }
}

// 5 rows of the country list.
void GtsSelector_PrintCountryPage(GtsSelector *selector, int page, int count) {
    int i;
    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&selector->windows[i], 0x22);
        if (page * 5 + i < count) {
            GtsSelector_PrintString(selector->cellManager, &selector->windows[i], selector->listItems[page * 5 + i].text, 0, MAKE_TEXT_COLOR(15, 14, 2));
        } else {
            CopyWindowPixelsToVram_TextMode(&selector->windows[i]);
            BgCellManager_PutWindowTiles(selector->cellManager, 0, &selector->windows[i]);
        }
    }
}

#pragma force_active reset

int GtsSelector_OpenSpeciesList(GtsSelector *selector) {
    int value;
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 35, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_SPECIES_LIST);
    value = selector->letter + gGtsSelectorGroupStarts[selector->group];
    selector->count = GtsSelector_CreateSpeciesListItems(&selector->listItems, selector->speciesNames, selector->gtsMsgData, selector->johtoDexFlags, value, selector->pokedex);
    selector->page = 0;
    GtsSelector_PrintListPage(selector, 0, selector->count);
    value = GtsSelector_CountPages(selector->count, 4);
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[4], selector->page, value);
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 16, 0);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
    selector->cursor = 0;
    Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorSpeciesCursorX[selector->cursor * 2], sGtsSelectorSpeciesCursorY[selector->cursor * 2]);
    Gts_SetSpritePosition(selector->rightArrowSprite, 228, 120);
    Gts_SetSpritePosition(selector->leftArrowSprite, 154, 120);
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    Sprite_SetDrawFlag(selector->rightArrowSprite, TRUE);
    Sprite_SetDrawFlag(selector->leftArrowSprite, TRUE);
    selector->state = GTS_SELECTOR_STATE_HANDLE_SPECIES_LIST;
    return -1;
}

int GtsSelector_HandleSpeciesList(GtsSelector *selector) {
    int species = GtsSelector_HandleSpeciesListInput(selector);
    switch (species) {
    case -2:
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        PlaySE(SEQ_SE_DP_SELECT);
        selector->state = GTS_SELECTOR_STATE_CANCEL_SPECIES_LIST;
        selector->species = -1;
        break;
    case -1:
        break;
    default:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_SPECIES;
        selector->species = species;
        break;
    }
    return -1;
}

int GtsSelector_CancelSpeciesList(GtsSelector *selector) {
    Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
    Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_SPECIES_LIST);
    ListMenuItems_Delete(selector->listItems);
    GF_ASSERT(selector->group <= 8);
    selector->state = ov70_02245F5C[selector->group][0] == 1 ? GTS_SELECTOR_STATE_REOPEN_SPECIES_GROUPS : GTS_SELECTOR_STATE_BACK_TO_SPECIES_LETTERS;
    return -1;
}

int GtsSelector_FinishSpecies(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_SPECIES_LIST);
    ListMenuItems_Delete(selector->listItems);
    return selector->species;
}

int GtsSelector_OpenCountryGroups(GtsSelector *selector) {
    int cursor;
    GtsSelector_ShowGroups(selector, GTS_SELECTOR_PAGE_COUNTRY_GROUPS, 32);
    BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 4);
    selector->state = GTS_SELECTOR_STATE_WAIT_SHOW;
    selector->nextState = GTS_SELECTOR_STATE_HANDLE_COUNTRY_GROUPS;
    selector->cursor = 10; // ANY
    selector->group = -1;
    cursor = selector->cursor;
    Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorCountryGroupCursorX[cursor * 2] + 16) * 8, sGtsSelectorCountryGroupCursorY[cursor * 2] * 8);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
    return -1;
}

// The gotos keep the block order (structured code compiles differently).
int GtsSelector_HandleCountryGroups(GtsSelector *selector) {
    u32 group = GtsSelector_HandleCountryGroupInput(selector, selector->enabled);
    if (group == 10) {
        goto any;
    }
    if (group == (u32)-2) {
        goto cancel;
    }
    if (group == (u32)-1) {
        goto done;
    }
    goto chosen;
cancel:
    BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
    selector->nextState = GTS_SELECTOR_STATE_CLOSE_COUNTRY_GROUPS;
    selector->group = -1;
    goto done;
any:
    BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
    selector->nextState = GTS_SELECTOR_STATE_CLOSE_COUNTRY_GROUPS;
    selector->group = -2;
    goto done;
chosen:
    GF_ASSERT(group <= 8);
    selector->group = group;
    selector->state = GTS_SELECTOR_STATE_CLOSE_COUNTRY_GROUPS;
done:
    return -1;
}

// Returns -2 for BACK and 0 for ANY.
int GtsSelector_CloseCountryGroups(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_COUNTRY_GROUPS);
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    if (selector->group < 0) {
        if (selector->group == -1) {
            return -2;
        }
        if (selector->group == -2) {
            return 0;
        }
    } else {
        GF_ASSERT(selector->group <= 8);
        if (ov70_02245F5C[selector->group][0] == 1) {
            // A group of one letter skips the letters.
            selector->letter = 0;
            BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 3);
            if (selector->screenCursorSprite) {
                Sprite_SetDrawFlag(selector->screenCursorSprite, FALSE);
            }
            Sprite_SetAnimCtrlSeq(selector->cursorSprite, 50);
            selector->state = GTS_SELECTOR_STATE_WAIT_MOVE;
            selector->nextState = GTS_SELECTOR_STATE_OPEN_COUNTRY_LIST;
        } else {
            selector->state = GTS_SELECTOR_STATE_OPEN_COUNTRY_LETTERS;
        }
    }
    return -1;
}

int GtsSelector_ReopenCountryGroups(GtsSelector *selector) {
    int cursor;
    u8 x, y;
    GtsSelector_ShowGroups(selector, GTS_SELECTOR_PAGE_COUNTRY_GROUPS, 16);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 61);
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    cursor = selector->group;
    x = sGtsSelectorCountryGroupCursorX[cursor * 2];
    y = sGtsSelectorCountryGroupCursorY[cursor * 2];
    selector->cursor = cursor;
    Gts_SetSpritePosition(selector->cursorSprite, (x + 16) * 8, y * 8);
    if (selector->screenCursorSprite) {
        Sprite_SetDrawFlag(selector->screenCursorSprite, TRUE);
    }
    selector->state = GTS_SELECTOR_STATE_HANDLE_COUNTRY_GROUPS;
    return -1;
}

// The group's letters, greyed out without any country.
int GtsSelector_OpenCountryLetters(GtsSelector *selector) {
    int i;
    int offset;
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 32, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_LETTERS);
    MI_CpuFill8(selector->enabled, TRUE, 26);
    GtsSelector_PrintMessage(selector->cellManager, selector->gtsMsgData, selector->windows, selector->group + 110, 2, MAKE_TEXT_COLOR(15, 14, 2));
    // A byte offset into windows next to the index.
    i = 1;
    offset = sizeof(Window);
    do {
        int color;
        if (GtsSelector_CountCountriesOfLetter(selector, gGtsSelectorGroupStarts[selector->group] + i - 1) > 0) {
            color = MAKE_TEXT_COLOR(15, 14, 2);
            selector->enabled[gGtsSelectorGroupStarts[selector->group] + i - 1] = TRUE;
        } else {
            color = MAKE_TEXT_COLOR(8, 9, 2);
            selector->enabled[gGtsSelectorGroupStarts[selector->group] + i - 1] = FALSE;
        }
        GtsSelector_PrintMessage(selector->cellManager, selector->gtsMsgData, (Window *)((u8 *)selector->windows + offset), sGtsSelectorLetterMsgs[gGtsSelectorLetterGroups[selector->group].first + i - 1], 5, color);
        i++;
        offset += sizeof(Window);
    } while (i < 4);
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 16, 0);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 47);
    if (selector->letter < 0) {
        selector->cursor = 0;
    } else {
        selector->cursor = selector->letter;
    }
    i = selector->cursor;
    Gts_SetSpritePosition(selector->cursorSprite, (sGtsSelectorGroupCursorX[i * 2] + 16) * 8, sGtsSelectorGroupCursorY[i * 2] * 8);
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    if (selector->screenCursorSprite) {
        Sprite_SetDrawFlag(selector->screenCursorSprite, TRUE);
    }
    selector->state = GTS_SELECTOR_STATE_HANDLE_COUNTRY_LETTERS;
    return -1;
}

// The gotos keep the block order (structured code compiles differently).
int GtsSelector_HandleCountryLetters(GtsSelector *selector) {
    int letter = GtsSelector_HandleLetterInput(selector, selector->enabled);
    switch (letter) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        goto accepted;
    default:
        break;
    }
    if (letter == -2) {
        goto cancel;
    }
    goto done;
accepted:
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_CLOSE_COUNTRY_LETTERS;
    selector->letter = letter;
    goto done;
cancel:
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    selector->state = GTS_SELECTOR_STATE_CLOSE_COUNTRY_LETTERS;
    selector->letter = -1;
done:
    return -1;
}

int GtsSelector_CloseCountryLetters(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_LETTERS);
    Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
    if (selector->letter < 0) {
        selector->state = GTS_SELECTOR_STATE_REOPEN_COUNTRY_GROUPS;
    } else {
        BgCellManager_StartCellMove(selector->cellManager, 0, -4, 0, 3);
        if (selector->screenCursorSprite) {
            Sprite_SetDrawFlag(selector->screenCursorSprite, FALSE);
        }
        Sprite_SetAnimCtrlSeq(selector->cursorSprite, 50);
        selector->state = GTS_SELECTOR_STATE_WAIT_MOVE;
        selector->nextState = GTS_SELECTOR_STATE_OPEN_COUNTRY_LIST;
    }
    return -1;
}

int GtsSelector_BackToCountryLetters(GtsSelector *selector) {
    selector->state = GTS_SELECTOR_STATE_OPEN_COUNTRY_LETTERS;
    selector->cursor = selector->letter;
    return -1;
}

int GtsSelector_OpenCountryList(GtsSelector *selector) {
    BgCellManager_LoadCellTilemap(selector->cellManager, 0, NARC_a_1_0_0, 30, 1);
    BgCellManager_GetCellTilemap(selector->cellManager, 0);
    GtsSelector_AddWindows(selector, GTS_SELECTOR_PAGE_COUNTRY_LIST);
    selector->count = Gts_CreateCountryListItems(&selector->listItems, selector->countryNames, selector->letter + gGtsSelectorGroupStarts[selector->group]);
    GtsSelector_PrintCountryPage(selector, 0, selector->count);
    selector->page = 0;
    GtsSelector_PrintPageNumber(selector->cellManager, selector->printer, &selector->windows[5], selector->page, GtsSelector_CountPages(selector->count, 5));
    GtsSelector_PrintButtonLabel(selector->cellManager, selector->gtsMsgData, &selector->windows[14], 68);
    BgCellManager_DrawCellAt(selector->cellManager, 0, 1, 0);
    selector->cursor = 0;
    {
        u32 i = selector->cursor * 2;
        Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorCountryCursorX[i], sGtsSelectorCountryCursorY[i]);
    }
    Gts_SetSpritePosition(selector->rightArrowSprite, 176, 136);
    Gts_SetSpritePosition(selector->leftArrowSprite, 88, 136);
    Sprite_SetDrawFlag(selector->cursorSprite, TRUE);
    Sprite_SetDrawFlag(selector->rightArrowSprite, TRUE);
    Sprite_SetDrawFlag(selector->leftArrowSprite, TRUE);
    selector->state = GTS_SELECTOR_STATE_HANDLE_COUNTRY_LIST;
    return -1;
}

int GtsSelector_HandleCountryList(GtsSelector *selector) {
    int country = GtsSelector_HandleCountryListInput(selector);
    switch (country) {
    case -2:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_MOVE;
        selector->nextState = GTS_SELECTOR_STATE_CANCEL_COUNTRY_LIST;
        break;
    case -1:
        break;
    default:
        BgCellManager_StartCellMove(selector->cellManager, 0, 6, 0, 6);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_COUNTRY;
        selector->country = country;
        break;
    }
    return -1;
}

int GtsSelector_CancelCountryList(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_COUNTRY_LIST);
    ListMenuItems_Delete(selector->listItems);
    GF_ASSERT(selector->group <= 8);
    selector->state = ov70_02245F5C[selector->group][0] == 1 ? GTS_SELECTOR_STATE_REOPEN_COUNTRY_GROUPS : GTS_SELECTOR_STATE_BACK_TO_COUNTRY_LETTERS;
    return -1;
}

int GtsSelector_FinishCountry(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_COUNTRY_LIST);
    if (selector->screenCursorSprite) {
        Sprite_SetDrawFlag(selector->screenCursorSprite, TRUE);
    }
    ListMenuItems_Delete(selector->listItems);
    return selector->country;
}

int GtsSelector_OpenGenders(GtsSelector *selector) {
    selector->cursor = 0;
    GtsSelector_ShowGenders(selector);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
    u32 i = selector->cursor * 2;
    Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorGenderCursorX[i], sGtsSelectorGenderCursorY[i]);
    selector->state = GTS_SELECTOR_STATE_WAIT_SHOW;
    selector->nextState = GTS_SELECTOR_STATE_HANDLE_GENDERS;
    return -1;
}

int GtsSelector_HandleGenders(GtsSelector *selector) {
    int gender = GtsSelector_HandleGenderInput(selector);
    switch (gender) {
    case -2:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_GENDER;
        selector->gender = -2;
        break;
    case -1:
        break;
    default:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_GENDER;
        selector->gender = gender;
        break;
    }
    return -1;
}

int GtsSelector_FinishGender(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_GENDERS);
    return selector->gender;
}

int GtsSelector_OpenLevels(GtsSelector *selector) {
    selector->cursor = 0;
    GtsSelector_ShowLevels(selector);
    Sprite_SetAnimCtrlSeq(selector->cursorSprite, 49);
    u32 i = selector->cursor * 2;
    Gts_SetSpritePosition(selector->cursorSprite, sGtsSelectorLevelCursorX[i], sGtsSelectorLevelCursorY[i]);
    selector->state = GTS_SELECTOR_STATE_WAIT_SHOW;
    selector->nextState = GTS_SELECTOR_STATE_HANDLE_LEVELS;
    return -1;
}

#pragma force_active on

int GtsSelector_HandleLevels(GtsSelector *selector) {
    int button = GtsSelector_HandleLevelInput(selector);
    switch (button) {
    case -2:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_LEVEL;
        selector->levelRange = -2;
        break;
    case -1:
        break;
    default:
        BgCellManager_StartCellMove(selector->cellManager, 0, 4, 0, 4);
        Sprite_SetDrawFlag(selector->cursorSprite, FALSE);
        Sprite_SetDrawFlag(selector->rightArrowSprite, FALSE);
        Sprite_SetDrawFlag(selector->leftArrowSprite, FALSE);
        selector->state = GTS_SELECTOR_STATE_WAIT_HIDE;
        selector->nextState = GTS_SELECTOR_STATE_FINISH_LEVEL;
        selector->levelRange = selector->listItems[selector->page * 4 + button].value;
        break;
    }
    return -1;
}

#pragma force_active reset

int GtsSelector_FinishLevel(GtsSelector *selector) {
    GtsSelector_RemoveWindows(selector, GTS_SELECTOR_PAGE_LEVELS);
    ListMenuItems_Delete(selector->listItems);
    return selector->levelRange;
}

void GtsSelector_PrintMessage(BgCellManager *cellManager, MsgData *msgData, Window *window, int msgId, int x, u32 color) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    GtsSelector_PrintString(cellManager, window, string, x, color);
    String_Delete(string);
}

void GtsSelector_PrintButtonLabel(BgCellManager *cellManager, MsgData *msgData, Window *window, int msgId) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    AddTextPrinterParameterizedWithColor(window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 2), NULL);
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(cellManager, 0, window);
    String_Delete(string);
}

// "page/pageCount", page 0-based.
void GtsSelector_PrintPageNumber(BgCellManager *cellManager, MessagePrinter *printer, Window *window, int page, int pageCount) {
    PrintUIntOnWindow(printer, page + 1, 2, PRINTING_MODE_RIGHT_ALIGN, window, 0, 0);
    MessagePrinter_PrintGlyph(printer, 0, window, 16, 0);
    PrintUIntOnWindow(printer, pageCount, 2, PRINTING_MODE_LEFT_ALIGN, window, 24, 0);
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(cellManager, 0, window);
}

int GtsSelector_CountSeenSpeciesOfLetter(GtsSelector *selector, int letter) {
    u32 count;
    u16 *species = Gts_LoadSpeciesByLetter(HEAP_ID_61, letter, &count);
    int seen = GtsSelector_CountSeenSpecies(selector->johtoDexFlags, selector->pokedex, count, species);
    Heap_Free(species);
    return seen;
}

#pragma force_active on

BOOL GtsSelector_GroupHasSeenSpecies(GtsSelector *selector, int group) {
    int first = gGtsSelectorGroupStarts[group];
    int count = ov70_02245F5C[group][0];
    if (count > 0) {
        int i;
        for (i = 0; i < count; i++) {
            if (GtsSelector_CountSeenSpeciesOfLetter(selector, first + i) > 0) {
                return TRUE;
            }
            count = gGtsSelectorLetterGroups[group].count;
        }
    } else if (GtsSelector_CountSeenSpeciesOfLetter(selector, first) > 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

int GtsSelector_CountCountriesOfLetter(void *unused, u32 letter) {
    int start;
    return Gts_GetCountryGroupRange(letter, &start);
}
