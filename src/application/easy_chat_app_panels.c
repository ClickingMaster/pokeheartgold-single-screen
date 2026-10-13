#include "global.h"

#include "bg_window.h"
#include "easy_chat.h"
#include "easy_chat_app_internal.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu_cursor.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_control_code.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "unk_02009D48.h"
#include "yes_no_prompt.h"

typedef struct EasyChatSentenceView EasyChatSentenceView;
typedef struct EasyChatMenuView EasyChatMenuView;
typedef struct EasyChatCategoryView EasyChatCategoryView;
typedef struct EasyChatWordListView EasyChatWordListView;
typedef struct EasyChatPageButtonsView EasyChatPageButtonsView;

// Scrolls a BG layer by a fixed amount over a number of frames (VBlank task).
typedef struct EasyChatBgScroll {
    BgConfig *bgConfig;    // 0x00
    int layer;             // 0x04
    enum BgPosAdjustOp op; // 0x08
    int position;          // 0x0C, fx32 while running
    int target;            // 0x10
    int step;              // 0x14
    u16 frames;            // 0x18
    u16 state;             // 0x1A
    SysTask *task;         // 0x1C
} EasyChatBgScroll;        // size: 0x20

// Fades the blend alpha or the brightness of BG planes over a number of frames (VBlank task).
typedef struct EasyChatBlendFade {
    u32 plane1;      // 0x00
    u32 plane2;      // 0x04
    s32 value;       // 0x08
    s32 target;      // 0x0C
    s32 step;        // 0x10
    u32 frames;      // 0x14
    u32 done;        // 0x18
    SysTask *task;   // 0x1C
} EasyChatBlendFade; // size: 0x20

typedef struct EasyChatWordSlotPos {
    s16 x;
    s16 y;
} EasyChatWordSlotPos;

// Splits the message string of the "message" mode into text runs, word slots and line breaks.
typedef struct EasyChatMessageParser {
    String *string;    // 0x00
    const u16 *cursor; // 0x04
    int state;         // 0x08
} EasyChatMessageParser;

enum {
    EC_PARSE_TEXT,
    EC_PARSE_WORD,
    EC_PARSE_NEWLINE,
    EC_PARSE_END,
};

// The sentence panel: the word slots (or the message with its slots), the cursor on them, the
// message window and the yes/no prompt.
struct EasyChatSentenceView {
    EasyChatAppView *view;                // 0x000
    EasyChatApp *app;                     // 0x004
    ECMenu *menu;                         // 0x008
    Window sentenceWindows[2];            // 0x00C, main and sub screen
    Window messageWindows[2];             // 0x02C
    Window yesNoCursorWindow;             // 0x04C
    Window *messageWindow;                // 0x05C
    Sprite *slotCursor;                   // 0x060
    Sprite *subSlotCursor;                // 0x064
    Sprite *leftArrow;                    // 0x068
    Sprite *rightArrow;                   // 0x06C
    Sprite *sliderBar;                    // 0x070
    Sprite *sliderThumb;                  // 0x074
    MsgData *msgData;                     // 0x078
    String *string;                       // 0x07C
    struct ListMenuCursor *listCursor;    // 0x080
    EasyChatWordSlotPos slotPositions[2]; // 0x084
    u32 numSlots;                         // 0x08C
    u32 iconTile;                         // 0x090
    EasyChatMessageParser parser;         // 0x094
    u16 palettes[10 * 16];                // 0x0A0, BG palette 0 for each message bank
    s16 mainScrollY;                      // 0x1E0
    s16 subScrollY;                       // 0x1E2
    u8 scrollFrames;                      // 0x1E4
    u8 scrollDirection;                   // 0x1E5
    YesNoPrompt *yesNoPrompt;             // 0x1E8
    u16 frameTile;                        // 0x1EC
    u16 yesNoTile;                        // 0x1EE
    MenuInputState *menuInputState;       // 0x1F0
}; // size: 0x1F4

// The menu buttons on the right (mode switch, finish, quit) and the mode switch animation.
struct EasyChatMenuView {
    EasyChatAppView *view;           // 0x00
    EasyChatApp *app;                // 0x04
    ECMenu *menu;                    // 0x08
    Sprite *cursor;                  // 0x0C
    BgConfig *bgConfig;              // 0x10
    MsgData *msgData;                // 0x14
    String *labels[3];               // 0x18
    Window windows[3];               // 0x24
    void *buttonsFile;               // 0x54
    void *modesFile;                 // 0x58
    NNSG2dScreenData *buttonsScreen; // 0x5C
    NNSG2dScreenData *modesScreen;   // 0x60
    SysTask *modeSwitchTask;         // 0x64
}; // size: 0x68

typedef struct EasyChatModeSwitch {
    EasyChatMenuView *menuView; // 0x00
    u32 state;                  // 0x04
    u16 timer;                  // 0x08
    u16 mode;                   // 0x0A
    u16 count;                  // 0x0C
} EasyChatModeSwitch;

// The category grid (or the initial letter grid) on BG 1.
struct EasyChatCategoryView {
    EasyChatAppView *view;          // 0x00
    EasyChatApp *app;               // 0x04
    ECMenu *menu;                   // 0x08
    BgConfig *bgConfig;             // 0x0C
    u32 unk10;                      // 0x10
    u32 state;                      // 0x14
    u32 unk18;                      // 0x18
    EasyChatBlendFade fade;         // 0x1C
    EasyChatBgScroll scroll;        // 0x3C
    Sprite *cursor;                 // 0x5C
    MenuInputState *menuInputState; // 0x60
}; // size: 0x64

// The word list of a category on BG 2.
struct EasyChatWordListView {
    EasyChatAppView *view;          // 0x00
    EasyChatApp *app;               // 0x04
    ECMenu *menu;                   // 0x08
    BgConfig *bgConfig;             // 0x0C
    Window listWindow;              // 0x10
    Window wordWindow;              // 0x20
    Sprite *cursor;                 // 0x30
    Sprite *upArrow;                // 0x34
    Sprite *downArrow;              // 0x38
    String *word;                   // 0x3C
    EasyChatBlendFade fade;         // 0x40
    EasyChatBgScroll scroll;        // 0x60
    u32 fadeDone;                   // 0x80
    u8 unk84[8];                    // 0x84
    int scrollY;                    // 0x8C
    int topRow;                     // 0x90
    MenuInputState *menuInputState; // 0x94
}; // size: 0x98

// The page up/down buttons of the word list on the sub screen.
struct EasyChatPageButtonsView {
    EasyChatAppView *view;  // 0x00
    EasyChatApp *app;       // 0x04
    ECMenu *menu;           // 0x08
    BgConfig *bgConfig;     // 0x0C
    SpriteList *spriteList; // 0x10
    u32 unk14;              // 0x14
    u32 unk18;              // 0x18
    u8 unk1C[0x10];         // 0x1C
    u32 unk2C;              // 0x2C
}; // size: 0x30

// One cell of the initial letter grid (data in .rodata).
typedef struct EasyChatInitialGridCell {
    u16 letter;
    u8 x;
    u8 y;
    u8 up;
    u8 down;
    u8 left;
    u8 right;
    u8 belowButton;
    u8 aboveButton;
} EasyChatInitialGridCell;

#define EC_INITIAL_GRID_CELLS 27

#define EC_BLEND_PLANEMASK_ALL (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD)

// The sentence panel passes unnarrowed ints to these u16/u8 parameters.
#define GetECWordIntoStringByIndexInt ((void (*)(int, String *))GetECWordIntoStringByIndex)
#define ListMenuUpdateCursorObjInt    ((void (*)(struct ListMenuCursor *, Window *, int, int))ListMenuUpdateCursorObj)

// render_window.c
void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);
// easy chat save data (category / initial unlocked)
u32 ECMenu_GetCategoryCount(ECMenu *menu, u32 category);
u32 ECMenu_GetInitialCount(ECMenu *menu, u32 group);

extern const WindowTemplate ov102_021EC808[3];
extern const EasyChatInitialGridCell ov102_021EC820[EC_INITIAL_GRID_CELLS];

BgConfig *EasyChatAppView_GetBgConfig(EasyChatAppView *view);
SpriteList *EasyChatAppView_GetSpriteList(EasyChatAppView *view);
SysTask *EasyChatAppView_CreateVBlankTask(SysTaskFunc func, void *data, u32 priority);
void EasyChatAppView_InitSpriteHeader(EasyChatAppView *view, SpriteResourcesHeader *header, u32 index, u8 priority);
Sprite *EasyChatAppView_CreateSprite(EasyChatAppView *view, SpriteResourcesHeader *header, s32 x, s32 y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen);

EasyChatSentenceView *EasyChatSentenceView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatSentenceView_Delete(EasyChatSentenceView *sentence);
void EasyChatSentenceView_Load(EasyChatSentenceView *sentence, NARC *narc);
void EasyChatSentenceView_HBlankCallback(EasyChatSentenceView *sentence);
void EasyChatSentenceView_ToggleScrollWindows(EasyChatSentenceView *sentence, BOOL enable);
void EasyChatSentenceView_StartScroll(EasyChatSentenceView *sentence, u8 direction);
BOOL EasyChatSentenceView_UpdateScroll(EasyChatSentenceView *sentence);
void EasyChatSentenceView_LoadPalettes(EasyChatSentenceView *sentence, NARC *narc);
void EasyChatSentenceView_ApplyPalette(EasyChatSentenceView *sentence);
void EasyChatSentenceView_InitSlotPositions(EasyChatSentenceView *sentence);
void EasyChatSentenceView_CreateSprites(EasyChatSentenceView *sentence);
void EasyChatSentenceView_DrawSentence(EasyChatSentenceView *sentence);
void EasyChatSentenceView_GetSlotHitbox(EasyChatSentenceView *sentence, TouchscreenHitbox *hitbox, u32 slot);
int EasyChatSentenceView_DrawMessage(EasyChatSentenceView *sentence, Window *window);
void EasyChatSentenceView_UpdateSliderThumb(EasyChatSentenceView *sentence);
void EasyChatMessageParser_Init(EasyChatMessageParser *parser, EasyChatSentenceView *sentence);
void EasyChatMessageParser_Free(EasyChatMessageParser *parser);
int EasyChatMessageParser_Next(EasyChatMessageParser *parser, String *dest);
void EasyChatSentenceView_GetSlotTextPos(const EasyChatWordSlotPos *slot, EasyChatWordSlotPos *textPos);
void EasyChatSentenceView_GetSlotCursorPos(const EasyChatWordSlotPos *slot, EasyChatWordSlotPos *cursorPos);
void EasyChatSentenceView_ClearSlot(Window *window, const EasyChatWordSlotPos *slot);
void EasyChatSentenceView_DrawWord(EasyChatSentenceView *sentence, Window *window, const EasyChatWordSlotPos *slot, int word);
u32 EasyChatSentenceView_GetNumSlots(EasyChatSentenceView *sentence);
void EasyChatSentenceView_SelectMessageWindow(EasyChatSentenceView *sentence, u32 index);
void EasyChatSentenceView_PrintMessage(EasyChatSentenceView *sentence, u32 message);
void EasyChatSentenceView_DeactivateCursor(EasyChatSentenceView *sentence);
void EasyChatSentenceView_ActivateCursor(EasyChatSentenceView *sentence);
void EasyChatSentenceView_SetCursorVisible(EasyChatSentenceView *sentence, BOOL draw);
void EasyChatSentenceView_HideSlider(EasyChatSentenceView *sentence);
void EasyChatSentenceView_ShowSlider(EasyChatSentenceView *sentence);
Sprite *EasyChatSentenceView_GetSliderThumb(EasyChatSentenceView *sentence);
void EasyChatSentenceView_MoveCursorToSlot(EasyChatSentenceView *sentence, u16 slot);
void EasyChatSentenceView_SetCursorActive(EasyChatSentenceView *sentence, BOOL active);
void EasyChatSentenceView_StartYesNo(EasyChatSentenceView *sentence, int initialCursorPos);
int EasyChatSentenceView_HandleYesNo(EasyChatSentenceView *sentence);
void EasyChatSentenceView_DrawYesNoCursor(EasyChatSentenceView *sentence, int index);
EasyChatMenuView *EasyChatMenuView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatMenuView_Delete(EasyChatMenuView *menuView);
void EasyChatMenuView_Load(EasyChatMenuView *menuView, NARC *narc);
void EasyChatMenuView_Unload(EasyChatMenuView *menuView);
void EasyChatMenuView_DrawMainButtons(EasyChatMenuView *menuView, BOOL draw, BOOL showCursor);
void EasyChatMenuView_DrawListButton(EasyChatMenuView *menuView, BOOL draw);
void EasyChatMenuView_AddWindows(EasyChatMenuView *menuView, BgConfig *bgConfig);
void EasyChatMenuView_CreateCursor(EasyChatMenuView *menuView);
void EasyChatMenuView_DeactivateCursor(EasyChatMenuView *menuView);
void EasyChatMenuView_ActivateCursor(EasyChatMenuView *menuView);
void EasyChatMenuView_SetCursorVisible(EasyChatMenuView *menuView, BOOL draw);
void EasyChatMenuView_MoveCursor(EasyChatMenuView *menuView, u16 cursor);
void EasyChatMenuView_DrawModeButton(EasyChatMenuView *menuView, int mode, int selected);
void EasyChatMenuView_ModeSwitchTask(SysTask *task, void *data);
void EasyChatMenuView_StartModeSwitch(EasyChatMenuView *menuView);
BOOL EasyChatMenuView_IsModeSwitchDone(EasyChatMenuView *menuView);
void EasyChatMenuView_ShowListButton(EasyChatMenuView *menuView);
void EasyChatMenuView_ShowMainButtons(EasyChatMenuView *menuView);
EasyChatCategoryView *EasyChatCategoryView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatCategoryView_Delete(EasyChatCategoryView *categories);
void EasyChatCategoryView_Load(EasyChatCategoryView *categories, NARC *narc);
u32 EasyChatCategoryView_DrawCategories(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile);
u32 EasyChatCategoryView_DrawInitials(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile);
u32 EasyChatCategoryView_LoadGfxDummy(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile);
void EasyChatCategoryView_CreateCursor(EasyChatCategoryView *categories);
void EasyChatCategoryView_SetCursorVisible(EasyChatCategoryView *categories, BOOL draw);
void EasyChatCategoryView_MoveCursor(EasyChatCategoryView *categories, u32 category);
void EasyChatCategoryView_StartOpen(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateOpen(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartClose(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateClose(EasyChatCategoryView *categories);
void EasyChatCategoryView_ResetScrollY(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartDim(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateDim(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartFadeOut(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateFadeOut(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartFadeIn(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateFadeIn(EasyChatCategoryView *categories);
void EasyChatCategoryView_ResetScrollX(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartModeScroll(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateModeScroll(EasyChatCategoryView *categories);
EasyChatWordListView *EasyChatWordListView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatWordListView_Delete(EasyChatWordListView *wordList);
void EasyChatWordListView_Load(EasyChatWordListView *wordList);
void EasyChatWordListView_CreateSprites(EasyChatWordListView *wordList);
void EasyChatWordListView_DrawWords(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_StartFadeIn(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_UpdateFadeIn(EasyChatWordListView *wordList);
void EasyChatWordListView_StartFadeOut(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_UpdateFadeOut(EasyChatWordListView *wordList);
void EasyChatWordListView_SetCursorVisible(EasyChatWordListView *wordList, BOOL draw);
void EasyChatWordListView_MoveCursor(EasyChatWordListView *wordList, u32 cursor);
void EasyChatWordListView_StartScroll(EasyChatWordListView *wordList, int rows);
BOOL EasyChatWordListView_UpdateScroll(EasyChatWordListView *wordList);
void EasyChatWordListView_ClearRows(EasyChatWordListView *wordList, int rows);
void EasyChatWordListView_ResetScroll(EasyChatWordListView *wordList);
void EasyChatWordListView_DrawWord(EasyChatWordListView *wordList, int index, u32 y);
EasyChatPageButtonsView *EasyChatPageButtonsView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatPageButtonsView_Delete(EasyChatPageButtonsView *pageButtons);
void EasyChatPageButtonsView_Load(EasyChatPageButtonsView *pageButtons, NARC *narc);
void EasyChatPageButtonsView_SetEnabled(void);
void EasyChatPageButtonsView_SetPressed(EasyChatPageButtonsView *pageButtons, int button, int pressed);
void EasyChatBgScroll_Start(EasyChatBgScroll *scroll, BgConfig *bgConfig, int layer, int axis, int delta, int frames);
BOOL EasyChatBgScroll_Update(EasyChatBgScroll *scroll);
void EasyChatBgScroll_Task(SysTask *task, void *data);
void EasyChatBlendFade_StartAlpha(EasyChatBlendFade *fade, u32 plane1, u32 plane2, s32 start, s32 target, s32 frames);
BOOL EasyChatBlendFade_IsAlphaDone(EasyChatBlendFade *fade);
void EasyChatBlendFade_AlphaTask(SysTask *task, void *data);
void EasyChatBlendFade_StartBrightness(EasyChatBlendFade *fade, u32 plane, s32 start, s32 target, s32 frames);
BOOL EasyChatBlendFade_IsBrightnessDone(EasyChatBlendFade *fade);
void EasyChatBlendFade_BrightnessTask(SysTask *task, void *data);
u32 EasyChatInitialGrid_GetCount(void);
void EasyChatInitialGrid_GetLetter(u32 cell, String *dest);
void EasyChatInitialGrid_GetPosition(u32 cell, u32 *x, u32 *y);

// Sentence panel

#pragma force_active on
EasyChatSentenceView *EasyChatSentenceView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu) {
    EasyChatSentenceView *sentence = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatSentenceView));

    sentence->view = view;
    sentence->app = app;
    sentence->menu = menu;
    sentence->menuInputState = EasyChatApp_GetMenuInputStatePtr(sentence->app);
    sentence->slotCursor = NULL;
    sentence->subSlotCursor = NULL;
    sentence->leftArrow = NULL;
    sentence->rightArrow = NULL;
    sentence->sliderBar = NULL;
    sentence->sliderThumb = NULL;
    sentence->string = String_New(128, HEAP_ID_35);
    sentence->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 283, HEAP_ID_35);
    sentence->listCursor = ListMenuCursorNew(HEAP_ID_35);
    sentence->mainScrollY = 0;
    sentence->subScrollY = 0;
    return sentence;
}
#pragma force_active reset

void EasyChatSentenceView_Delete(EasyChatSentenceView *sentence) {
    if (sentence->listCursor != NULL) {
        DestroyListMenuCursorObj(sentence->listCursor);
    }
    if (sentence->slotCursor != NULL) {
        Sprite_Delete(sentence->slotCursor);
    }
    if (sentence->subSlotCursor != NULL) {
        Sprite_Delete(sentence->subSlotCursor);
    }
    if (sentence->leftArrow != NULL) {
        Sprite_Delete(sentence->leftArrow);
    }
    if (sentence->rightArrow != NULL) {
        Sprite_Delete(sentence->rightArrow);
    }
    if (sentence->sliderBar != NULL) {
        Sprite_Delete(sentence->sliderBar);
    }
    if (sentence->sliderThumb != NULL) {
        Sprite_Delete(sentence->sliderThumb);
    }
    if (sentence->msgData != NULL) {
        DestroyMsgData(sentence->msgData);
    }
    if (sentence->string != NULL) {
        String_Delete(sentence->string);
    }
    RemoveWindow(&sentence->sentenceWindows[0]);
    RemoveWindow(&sentence->sentenceWindows[1]);
    RemoveWindow(&sentence->messageWindows[0]);
    RemoveWindow(&sentence->messageWindows[1]);
    RemoveWindow(&sentence->yesNoCursorWindow);
    YesNoPrompt_Destroy(sentence->yesNoPrompt);
    Heap_Free(sentence);
}

#pragma force_active on
void EasyChatSentenceView_Load(EasyChatSentenceView *sentence, NARC *narc) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(sentence->view);
    u32 baseTile;

    EasyChatSentenceView_LoadPalettes(sentence, narc);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, HEAP_ID_35);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_35);
    sentence->subScrollY = -192;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, sentence->subScrollY);
    baseTile = GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_35);
    baseTile >>= 5;
    AddWindowParameterized(bgConfig, &sentence->sentenceWindows[0], GF_BG_LYR_MAIN_0, 3, 1, 27, 4, 0, baseTile);
    AddWindowParameterized(bgConfig, &sentence->sentenceWindows[1], GF_BG_LYR_SUB_0, 3, 1, 27, 4, 0, baseTile);
    AddWindowParameterized(bgConfig, &sentence->messageWindows[0], GF_BG_LYR_MAIN_0, 2, 21, 27, 2, 11, baseTile + 108);
    AddWindowParameterized(bgConfig, &sentence->messageWindows[1], GF_BG_LYR_MAIN_0, 2, 21, 19, 2, 11, baseTile + 108);
    AddWindowParameterized(bgConfig, &sentence->yesNoCursorWindow, GF_BG_LYR_MAIN_0, 25, 12, 8, 4, 11, baseTile + 162);
    sentence->yesNoTile = baseTile + 162;
    sentence->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_35);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, 0, bgConfig, GF_BG_LYR_MAIN_0, baseTile + 222, 0, FALSE, HEAP_ID_35);
    sentence->iconTile = baseTile + 222;
    GfGfxLoader_GXLoadPal(NARC_a_0_3_8, 25, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 32, HEAP_ID_35);
    sentence->frameTile = baseTile + 231;
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, baseTile + 231, 15, EasyChatApp_GetFrame(sentence->app), HEAP_ID_35);
    sentence->messageWindow = &sentence->messageWindows[0];
    DrawFrame2AroundWindow(&sentence->messageWindows[0], baseTile + 231, 15);
    PutWindowTilemap(&sentence->sentenceWindows[0]);
    PutWindowTilemap(&sentence->sentenceWindows[1]);
    PutWindowTilemap(&sentence->messageWindows[0]);
    PutWindowTilemap(&sentence->messageWindows[1]);
    EasyChatSentenceView_InitSlotPositions(sentence);
    EasyChatSentenceView_DrawSentence(sentence);
    EasyChatSentenceView_PrintMessage(sentence, 0);
    EasyChatSentenceView_CreateSprites(sentence);
    if (EasyChatApp_GetType(sentence->app) == 2) {
        EasyChatSentenceView_UpdateSliderThumb(sentence);
    }
}
#pragma force_active reset

void EasyChatSentenceView_HBlankCallback(EasyChatSentenceView *sentence) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(sentence->view);

    if (GX_GetVCount() < 48) {
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, sentence->mainScrollY);
    } else {
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
    }
}

void EasyChatSentenceView_ToggleScrollWindows(EasyChatSentenceView *sentence, BOOL enable) {
#pragma unused(sentence)
    volatile u16 *winIn = (volatile u16 *)0x04000048;
    volatile u16 *winOut = (volatile u16 *)0x0400004A;

    *winOut = (*winOut & ~0x3F) | 0x1F;
    if (enable) {
        *winIn = (*winIn & ~0x3F) | 0x1A;
        *winIn = (*winIn & ~0x3F00) | 0x1B00;
        *(volatile u16 *)0x04000040 = 0xFF;
        *(volatile u16 *)0x04000044 = 0x30;
        *(volatile u16 *)0x04000042 = 0xFF;
        *(volatile u16 *)0x04000046 = 0xA0C0;
        *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & 0xFFFF1FFF) | 0x6000;
    } else {
        *winIn = (*winIn & ~0x3F) | 0x1F;
        *winIn = (*winIn & ~0x3F00) | 0x1F00;
        *(volatile u16 *)0x04000040 = 0;
        *(volatile u16 *)0x04000044 = 0;
        *(volatile u16 *)0x04000042 = 0;
        *(volatile u16 *)0x04000046 = 0;
        *(volatile u32 *)0x04000000 &= 0xFFFF1FFF;
    }
}

void EasyChatSentenceView_StartScroll(EasyChatSentenceView *sentence, u8 direction) {
    sentence->scrollFrames = 0;
    sentence->scrollDirection = direction;
    if (sentence->scrollDirection == 1) {
        volatile u16 *winIn = (volatile u16 *)0x04000048;
        *winIn = (*winIn & ~0x3F) | 0x1B;
    }
    Main_SetHBlankIntrCB((GFIntrCB)EasyChatSentenceView_HBlankCallback, sentence);
}

#pragma force_active on
BOOL EasyChatSentenceView_UpdateScroll(EasyChatSentenceView *sentence) {
    BgConfig *bgConfig;
    int delta;

    if (sentence->scrollFrames > 5) {
        if (sentence->scrollDirection == 0) {
            EasyChatSentenceView_ToggleScrollWindows(sentence, TRUE);
        } else {
            EasyChatSentenceView_ToggleScrollWindows(sentence, FALSE);
        }
        return TRUE;
    }
    bgConfig = EasyChatAppView_GetBgConfig(sentence->view);
    if (sentence->scrollDirection) {
        delta = -8;
    } else {
        delta = 8;
    }
    sentence->subScrollY += delta;
    sentence->mainScrollY += delta;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, sentence->mainScrollY);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, sentence->subScrollY);
    Sprite_OffsetPositionXY(sentence->slotCursor, 0, -delta);
    Sprite_OffsetPositionXY(sentence->subSlotCursor, 0, -delta);
    sentence->scrollFrames++;
    if (sentence->scrollDirection && sentence->scrollFrames == 6) {
        Main_SetHBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
    }
    return FALSE;
}
#pragma force_active reset

void EasyChatSentenceView_LoadPalettes(EasyChatSentenceView *sentence, NARC *narc) {
    NNSG2dPaletteData *paletteData;
    void *file;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x1C0, HEAP_ID_35);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 13, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_35);
    file = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 16, &paletteData, HEAP_ID_35);
    MIi_CpuCopy16(paletteData->pRawData, sentence->palettes, sizeof(sentence->palettes));
    DC_FlushRange(sentence->palettes, sizeof(sentence->palettes));
    Heap_Free(file);
}

void EasyChatSentenceView_ApplyPalette(EasyChatSentenceView *sentence) {
    if (EasyChatApp_GetType(sentence->app) == 2) {
        u32 palette = EasyChatApp_GetMsgBank(sentence->app) + 1;
        GX_LoadBGPltt(&sentence->palettes[palette * 16], 0, 32);
        GXS_LoadBGPltt(&sentence->palettes[palette * 16], 0, 32);
    } else {
        GX_LoadBGPltt(sentence->palettes, 0, 32);
        GXS_LoadBGPltt(sentence->palettes, 0, 32);
    }
}

void EasyChatSentenceView_InitSlotPositions(EasyChatSentenceView *sentence) {
    switch (EasyChatApp_GetType(sentence->app)) {
    case 0:
        sentence->slotPositions[0].x = 104;
        sentence->slotPositions[0].y = 16;
        sentence->numSlots = 1;
        break;
    case 1:
        sentence->slotPositions[0].x = 48;
        sentence->slotPositions[0].y = 16;
        sentence->slotPositions[1].x = 160;
        sentence->slotPositions[1].y = 16;
        sentence->numSlots = 2;
        break;
    case 2:
        sentence->numSlots = 0;
        break;
    }
}

#pragma force_active on
void EasyChatSentenceView_CreateSprites(EasyChatSentenceView *sentence) {
    SpriteResourcesHeader mainHeader, subHeader;
    EasyChatWordSlotPos pos;

    if (sentence->numSlots) {
        EasyChatSentenceView_GetSlotCursorPos(&sentence->slotPositions[0], &pos);
    } else {
        pos.x = 128;
        pos.y = 24;
    }
    EasyChatAppView_InitSpriteHeader(sentence->view, &mainHeader, 0, 0);
    EasyChatAppView_InitSpriteHeader(sentence->view, &subHeader, 1, 0);
    sentence->slotCursor = EasyChatAppView_CreateSprite(sentence->view, &mainHeader, pos.x, pos.y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    sentence->subSlotCursor = EasyChatAppView_CreateSprite(sentence->view, &subHeader, pos.x, pos.y - sentence->subScrollY, 0, NNS_G2D_VRAM_TYPE_2DSUB);
    Sprite_SetAnimCtrlSeq(sentence->subSlotCursor, 1);
    EasyChatSentenceView_SetCursorActive(sentence, TRUE);
    sentence->leftArrow = EasyChatAppView_CreateSprite(sentence->view, &mainHeader, 8, 64, 1, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(sentence->leftArrow, 19);
    sentence->rightArrow = EasyChatAppView_CreateSprite(sentence->view, &mainHeader, 248, 64, 1, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(sentence->rightArrow, 21);
    sentence->sliderThumb = EasyChatAppView_CreateSprite(sentence->view, &mainHeader, 28, 64, 1, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(sentence->sliderThumb, 18);
    sentence->sliderBar = EasyChatAppView_CreateSprite(sentence->view, &mainHeader, 128, 64, 1, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(sentence->sliderBar, 23);
    if (EasyChatApp_GetType(sentence->app) != 2) {
        Sprite_SetDrawFlag(sentence->rightArrow, FALSE);
        Sprite_SetDrawFlag(sentence->leftArrow, FALSE);
        Sprite_SetDrawFlag(sentence->sliderBar, FALSE);
        Sprite_SetDrawFlag(sentence->sliderThumb, FALSE);
    }
}

void EasyChatSentenceView_DrawSentence(EasyChatSentenceView *sentence) {
    int i;
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(sentence->view);
    Window *window;
    EasyChatWordSlotPos *slot0, *slot1;

    EasyChatSentenceView_ApplyPalette(sentence);
    for (i = 0, window = sentence->sentenceWindows, slot0 = &sentence->slotPositions[0], slot1 = &sentence->slotPositions[1]; i < 2; i++, window++) {
        FillWindowPixelBuffer(window, 13);
        switch (EasyChatApp_GetType(sentence->app)) {
        case 0:
            EasyChatSentenceView_ClearSlot(window, slot0);
            EasyChatSentenceView_DrawWord(sentence, window, slot0, EasyChatApp_GetWord(sentence->app, 0));
            break;
        case 1:
            EasyChatSentenceView_ClearSlot(window, slot0);
            EasyChatSentenceView_ClearSlot(window, slot1);
            EasyChatSentenceView_DrawWord(sentence, window, slot0, EasyChatApp_GetWord(sentence->app, 0));
            EasyChatSentenceView_DrawWord(sentence, window, slot1, EasyChatApp_GetWord(sentence->app, 1));
            break;
        case 2:
            sentence->numSlots = EasyChatSentenceView_DrawMessage(sentence, window);
            EasyChatSentenceView_UpdateSliderThumb(sentence);
            break;
        }
        CopyWindowPixelsToVram_TextMode(window);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_0);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_0);
}
#pragma force_active reset

void EasyChatSentenceView_GetSlotHitbox(EasyChatSentenceView *sentence, TouchscreenHitbox *hitbox, u32 slot) {
    hitbox->rect.left = sentence->slotPositions[slot].x - 24;
    hitbox->rect.right = hitbox->rect.left + 96;
    hitbox->rect.top = sentence->slotPositions[slot].y;
    hitbox->rect.bottom = hitbox->rect.top + 16;
}

#pragma force_active on
int EasyChatSentenceView_DrawMessage(EasyChatSentenceView *sentence, Window *window) {
    String *string;
    int x, y;
    int count, running;
    EasyChatSentenceView *cur;
    EasyChatWordSlotPos *pos;
    u32 word;

    string = EasyChatApp_GetMessageString(sentence->app, HEAP_ID_35);
    count = 0;
    y = 0;
    running = TRUE;
    x = 0;
    EasyChatMessageParser_Init(&sentence->parser, sentence);
    // `cur` advances by one slot position per word (the original walks the panel pointer itself)
    cur = sentence;
    pos = sentence->slotPositions;
    do {
        switch (EasyChatMessageParser_Next(&sentence->parser, string)) {
        case EC_PARSE_TEXT:
            AddTextPrinterParameterizedWithColor(window, 1, string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 13), NULL);
            x += FontID_String_GetWidth(1, string, 0);
            break;
        case EC_PARSE_WORD:
            cur->slotPositions[0].x = x + 50;
            cur->slotPositions[0].y = y + 8;
            EasyChatSentenceView_ClearSlot(window, pos);
            word = EasyChatApp_GetWord(sentence->app, count);
            if (word != EC_WORD_NULL) {
                EasyChatSentenceView_DrawWord(sentence, window, pos, word);
            }
            cur = (EasyChatSentenceView *)((u8 *)cur + sizeof(EasyChatWordSlotPos));
            pos++;
            count++;
            x += 100;
            break;
        case EC_PARSE_NEWLINE:
            y += 16;
            x = 0;
            break;
        case EC_PARSE_END:
            running = FALSE;
            break;
        }
    } while (running);
    EasyChatMessageParser_Free(&sentence->parser);
    String_Delete(string);
    return count;
}
#pragma force_active reset

void EasyChatSentenceView_UpdateSliderThumb(EasyChatSentenceView *sentence) {
    u8 index = EasyChatApp_GetMessageIndex(sentence->app);

    if (sentence->sliderThumb != NULL) {
        Sprite_SetPositionXY(sentence->sliderThumb, index * 2 + 28, 64);
    }
}

#pragma force_active on
void EasyChatMessageParser_Init(EasyChatMessageParser *parser, EasyChatSentenceView *sentence) {
    parser->string = EasyChatApp_GetMessageString(sentence->app, HEAP_ID_35);
    parser->cursor = String_cstr(parser->string);
    if (*parser->cursor == 0xFFFE) {
        parser->state = EC_PARSE_WORD;
    } else {
        parser->state = EC_PARSE_TEXT;
    }
}
#pragma force_active reset

void EasyChatMessageParser_Free(EasyChatMessageParser *parser) {
    String_Delete(parser->string);
}

#pragma force_active on
int EasyChatMessageParser_Next(EasyChatMessageParser *parser, String *dest) {
    const u16 *start = parser->cursor;
    int result;

    switch (parser->state) {
    case EC_PARSE_TEXT:
        while (parser->state == EC_PARSE_TEXT) {
            switch (*parser->cursor) {
            case 0xE000:
                parser->state = EC_PARSE_NEWLINE;
                break;
            case EOS:
                parser->state = EC_PARSE_END;
                break;
            case 0xFFFE:
                parser->state = EC_PARSE_WORD;
                break;
            default:
                parser->cursor++;
                break;
            }
        }
        CopyU16ArrayToStringN(dest, start, parser->cursor - start + 1);
        return EC_PARSE_TEXT;
    case EC_PARSE_WORD:
        parser->cursor = MsgArray_SkipControlCode(start);
        result = EC_PARSE_WORD;
        break;
    case EC_PARSE_NEWLINE:
        parser->cursor = start + 1;
        result = EC_PARSE_NEWLINE;
        break;
    case EC_PARSE_END:
    default:
        return EC_PARSE_END;
    }
    switch (*parser->cursor) {
    case 0xE000:
        parser->state = EC_PARSE_NEWLINE;
        break;
    case EOS:
        parser->state = EC_PARSE_END;
        break;
    case 0xFFFE:
        parser->state = EC_PARSE_WORD;
        break;
    default:
        parser->state = EC_PARSE_TEXT;
        break;
    }
    return result;
}
#pragma force_active reset

void EasyChatSentenceView_GetSlotTextPos(const EasyChatWordSlotPos *slot, EasyChatWordSlotPos *textPos) {
    textPos->x = slot->x - 48;
    textPos->y = slot->y - 8;
}

void EasyChatSentenceView_GetSlotCursorPos(const EasyChatWordSlotPos *slot, EasyChatWordSlotPos *cursorPos) {
    cursorPos->x = slot->x + 24;
    cursorPos->y = slot->y + 8;
}

void EasyChatSentenceView_ClearSlot(Window *window, const EasyChatWordSlotPos *slot) {
    EasyChatWordSlotPos pos;

    EasyChatSentenceView_GetSlotTextPos(slot, &pos);
    FillWindowPixelRect(window, 14, pos.x, pos.y, 96, 16);
}

#pragma force_active on
void EasyChatSentenceView_DrawWord(EasyChatSentenceView *sentence, Window *window, const EasyChatWordSlotPos *slot, int word) {
    EasyChatWordSlotPos pos;

    if (word != EC_WORD_NULL) {
        EasyChatSentenceView_GetSlotTextPos(slot, &pos);
        GetECWordIntoStringByIndexInt(word, sentence->string);
        FontID_String_GetWidth(1, sentence->string, 0);
        AddTextPrinterParameterizedWithColor(window, 1, sentence->string, pos.x, pos.y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 14), NULL);
    }
}
#pragma force_active reset

u32 EasyChatSentenceView_GetNumSlots(EasyChatSentenceView *sentence) {
    return sentence->numSlots;
}

void EasyChatSentenceView_SelectMessageWindow(EasyChatSentenceView *sentence, u32 index) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(sentence->view);
    Window *window;

    FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 20, 24, 4, TILEMAP_FILL_OVWT_PAL);
    ClearWindowTilemapAndCopyToVram(sentence->messageWindow);
    window = &sentence->messageWindows[index];
    sentence->messageWindow = window;
    DrawFrame2AroundWindow(window, sentence->frameTile, 15);
}

void EasyChatSentenceView_PrintMessage(EasyChatSentenceView *sentence, u32 message) {
    MessageFormat *msgFormat;
    String *string;

    FillWindowPixelBuffer(sentence->messageWindow, 9);
    switch (message) {
    case 0:
        msgFormat = MessageFormat_New_Custom(2, 60, HEAP_ID_DEFAULT);
        string = String_New(300, HEAP_ID_DEFAULT);
        BufferPocketName(msgFormat, 0, 0);
        ReadMsgDataIntoString(sentence->msgData, ov102_021E8F70(sentence->app), sentence->string);
        StringExpandPlaceholders(msgFormat, string, sentence->string);
        AddTextPrinterParameterizedWithColor(sentence->messageWindow, 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 9), NULL);
        String_Delete(string);
        MessageFormat_Delete(msgFormat);
        break;
    case 2:
        ReadMsgDataIntoString(sentence->msgData, 6, sentence->string);
        AddTextPrinterParameterizedWithColor(sentence->messageWindow, 1, sentence->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 9), NULL);
        break;
    case 1:
        ReadMsgDataIntoString(sentence->msgData, 7, sentence->string);
        AddTextPrinterParameterizedWithColor(sentence->messageWindow, 1, sentence->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 9), NULL);
        break;
    case 3:
        ReadMsgDataIntoString(sentence->msgData, 8, sentence->string);
        AddTextPrinterParameterizedWithColor(sentence->messageWindow, 1, sentence->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 9), NULL);
        break;
    }
    CopyWindowToVram(sentence->messageWindow);
}

void EasyChatSentenceView_DeactivateCursor(EasyChatSentenceView *sentence) {
    EasyChatSentenceView_SetCursorActive(sentence, FALSE);
}

void EasyChatSentenceView_ActivateCursor(EasyChatSentenceView *sentence) {
    EasyChatSentenceView_SetCursorActive(sentence, TRUE);
}

void EasyChatSentenceView_SetCursorVisible(EasyChatSentenceView *sentence, BOOL draw) {
    Sprite_SetDrawFlag(sentence->slotCursor, draw);
    Sprite_SetDrawFlag(sentence->subSlotCursor, draw);
    EasyChatSentenceView_SetCursorActive(sentence, TRUE);
}

void EasyChatSentenceView_HideSlider(EasyChatSentenceView *sentence) {
    if (EasyChatApp_GetType(sentence->app) == 2) {
        Sprite_SetDrawFlag(sentence->leftArrow, FALSE);
        Sprite_SetDrawFlag(sentence->rightArrow, FALSE);
        Sprite_SetDrawFlag(sentence->sliderThumb, FALSE);
        Sprite_SetDrawFlag(sentence->sliderBar, FALSE);
    }
}

#pragma force_active on
void EasyChatSentenceView_ShowSlider(EasyChatSentenceView *sentence) {
    if (EasyChatApp_GetType(sentence->app) == 2) {
        Sprite_SetDrawFlag(sentence->leftArrow, TRUE);
        Sprite_SetDrawFlag(sentence->rightArrow, TRUE);
        Sprite_SetDrawFlag(sentence->sliderThumb, TRUE);
        Sprite_SetDrawFlag(sentence->sliderBar, TRUE);
        Sprite_SetAnimCtrlSeq(sentence->leftArrow, 19);
        Sprite_SetAnimCtrlSeq(sentence->rightArrow, 21);
    }
}
#pragma force_active reset

Sprite *EasyChatSentenceView_GetSliderThumb(EasyChatSentenceView *sentence) {
    return sentence->sliderThumb;
}

void EasyChatSentenceView_MoveCursorToSlot(EasyChatSentenceView *sentence, u16 slot) {
    VecFx32 position;
    EasyChatWordSlotPos pos;
    int offsetY;
    f32 offset;

    if (sentence->numSlots != 0) {
        EasyChatSentenceView_GetSlotCursorPos(&sentence->slotPositions[slot], &pos);
        position.x = pos.x << FX32_SHIFT;
        position.y = pos.y << FX32_SHIFT;
        position.z = 0;
    } else {
        position.x = 128 << FX32_SHIFT;
        position.y = 24 << FX32_SHIFT;
        position.z = 0;
    }
    Sprite_SetMatrix(sentence->slotCursor, &position);
    offsetY = 192 - sentence->subScrollY;
    if (offsetY > 0) {
        offset = (f32)(offsetY << FX32_SHIFT) + 0.5f;
    } else {
        offset = (f32)(offsetY << FX32_SHIFT) - 0.5f;
    }
    position.y += (fx32)offset;
    Sprite_SetMatrix(sentence->subSlotCursor, &position);
    EasyChatSentenceView_SetCursorActive(sentence, TRUE);
}

void EasyChatSentenceView_SetCursorActive(EasyChatSentenceView *sentence, BOOL active) {
    if (sentence->numSlots != 0) {
        if (active) {
            Sprite_SetAnimCtrlSeq(sentence->slotCursor, 0);
        } else {
            Sprite_SetAnimCtrlSeq(sentence->slotCursor, 1);
        }
    } else {
        if (active) {
            Sprite_SetAnimCtrlSeq(sentence->slotCursor, 14);
        } else {
            Sprite_SetAnimCtrlSeq(sentence->slotCursor, 15);
        }
    }
}

void EasyChatSentenceView_StartYesNo(EasyChatSentenceView *sentence, int initialCursorPos) {
    YesNoPromptTemplate template;
    BgConfig *bgConfig;
    u8 ignoreTouch;

    bgConfig = EasyChatAppView_GetBgConfig(sentence->view);
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgId = GF_BG_LYR_MAIN_0;
    template.bgConfig = bgConfig;
    template.tileStart = sentence->yesNoTile;
    template.plttSlot = 3;
    template.x = 25;
    template.y = 12;
    ignoreTouch = *sentence->menuInputState;
    template.ignoreTouchFlag = ignoreTouch;
    template.initialCursorPos = initialCursorPos;
    YesNoPrompt_InitFromTemplate(sentence->yesNoPrompt, &template);
}

int EasyChatSentenceView_HandleYesNo(EasyChatSentenceView *sentence) {
    int result;

    switch (YesNoPrompt_HandleInput(sentence->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        result = TRUE;
        break;
    case YESNORESPONSE_NO:
        result = FALSE;
        break;
    default:
        return -1;
    }
    YesNoPrompt_IsInTouchMode(sentence->yesNoPrompt);
    YesNoPrompt_Reset(sentence->yesNoPrompt);
    return result;
}

void EasyChatSentenceView_DrawYesNoCursor(EasyChatSentenceView *sentence, int index) {
    FillWindowPixelRect(&sentence->yesNoCursorWindow, 9, 0, 0, 14, 32);
    ListMenuUpdateCursorObjInt(sentence->listCursor, &sentence->yesNoCursorWindow, 0, index * 16);
}

// Menu buttons

EasyChatMenuView *EasyChatMenuView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu) {
    EasyChatMenuView *menuView = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatMenuView));

    menuView->view = view;
    menuView->app = app;
    menuView->menu = menu;
    menuView->cursor = NULL;
    menuView->modeSwitchTask = NULL;
    menuView->bgConfig = EasyChatAppView_GetBgConfig(menuView->view);
    menuView->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 284, HEAP_ID_35);
    return menuView;
}

void EasyChatMenuView_Delete(EasyChatMenuView *menuView) {
    EasyChatMenuView_Unload(menuView);
    DestroyMsgData(menuView->msgData);
    Heap_Free(menuView);
}

void EasyChatMenuView_Load(EasyChatMenuView *menuView, NARC *narc) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(menuView->view);

    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_35);
    menuView->buttonsFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 14, FALSE, &menuView->buttonsScreen, HEAP_ID_35);
    menuView->modesFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 15, FALSE, &menuView->modesScreen, HEAP_ID_35);
    EasyChatMenuView_AddWindows(menuView, bgConfig);
    EasyChatMenuView_CreateCursor(menuView);
    EasyChatMenuView_DrawMainButtons(menuView, TRUE, FALSE);
    EasyChatMenuView_DrawModeButton(menuView, 0, 1);
    EasyChatMenuView_DrawModeButton(menuView, 1, 0);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_3);
}

void EasyChatMenuView_Unload(EasyChatMenuView *menuView) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(menuView->view);
    Window *window;
    int i;

    if (menuView->cursor != NULL) {
        Sprite_Delete(menuView->cursor);
    }
    for (i = 0, window = menuView->windows; i < 3; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        String_Delete(menuView->labels[i]);
        window++;
    }
    Heap_Free(menuView->modesFile);
    Heap_Free(menuView->buttonsFile);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_3);
}

void EasyChatMenuView_DrawMainButtons(EasyChatMenuView *menuView, BOOL draw, BOOL showCursor) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(menuView->view);
    Window *window;
    int i;

    FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 0, 23, 13, 9, 7, TILEMAP_FILL_OVWT_PAL);
    EasyChatMenuView_SetCursorVisible(menuView, showCursor);
    if (!draw) {
        BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_0);
        return;
    }
    CopyToBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 23, 13, 9, 7, menuView->buttonsScreen->rawData, 0, 0, menuView->buttonsScreen->screenWidth / 8, menuView->buttonsScreen->screenHeight / 8);
    for (i = 0, window = menuView->windows; i < 2; i++) {
        int x;
        FillWindowPixelBuffer(window, 14);
        x = 56 - FontID_String_GetWidth(4, menuView->labels[i], 0);
        AddTextPrinterParameterizedWithColor(window, 4, menuView->labels[i], x / 2, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(3, 1, 14), NULL);
        window++;
    }
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_0);
}

void EasyChatMenuView_DrawListButton(EasyChatMenuView *menuView, BOOL draw) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(menuView->view);
    int x;

    if (!draw) {
        FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 0, 24, 20, 8, 4, TILEMAP_FILL_OVWT_PAL);
        ClearWindowTilemapAndCopyToVram(&menuView->windows[2]);
        ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_0);
        return;
    }
    CopyToBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 24, 20, 8, 4, menuView->buttonsScreen->rawData, 0, 7, menuView->buttonsScreen->screenWidth / 8, menuView->buttonsScreen->screenHeight / 8);
    FillWindowPixelBuffer(&menuView->windows[2], 14);
    x = 48 - FontID_String_GetWidth(4, menuView->labels[2], 0);
    AddTextPrinterParameterizedWithColor(&menuView->windows[2], 4, menuView->labels[2], x / 2, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(3, 1, 14), NULL);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_0);
}

void EasyChatMenuView_AddWindows(EasyChatMenuView *menuView, BgConfig *bgConfig) {
    u32 i;
    const WindowTemplate *template;
    Window *window;

    template = ov102_021EC808;
    for (i = 0, window = menuView->windows; i < 3; i++) {
        AddWindow(bgConfig, window, template);
        menuView->labels[i] = NewString_ReadMsgData(menuView->msgData, i);
        template ++;
        window++;
    }
    PutWindowTilemap(&menuView->windows[0]);
    FillWindowPixelBuffer(&menuView->windows[0], 14);
    PutWindowTilemap(&menuView->windows[1]);
    FillWindowPixelBuffer(&menuView->windows[1], 14);
}

void EasyChatMenuView_CreateCursor(EasyChatMenuView *menuView) {
    SpriteResourcesHeader header;

    EasyChatAppView_InitSpriteHeader(menuView->view, &header, 0, 0);
    menuView->cursor = EasyChatAppView_CreateSprite(menuView->view, &header, 220, 120, 5, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(menuView->cursor, 2);
    Sprite_SetDrawFlag(menuView->cursor, FALSE);
}

void EasyChatMenuView_DeactivateCursor(EasyChatMenuView *menuView) {
    Sprite_SetAnimCtrlSeq(menuView->cursor, 3);
}

void EasyChatMenuView_ActivateCursor(EasyChatMenuView *menuView) {
    Sprite_SetAnimCtrlSeq(menuView->cursor, 2);
}

void EasyChatMenuView_SetCursorVisible(EasyChatMenuView *menuView, BOOL draw) {
    Sprite_SetDrawFlag(menuView->cursor, draw);
}

void EasyChatMenuView_MoveCursor(EasyChatMenuView *menuView, u16 cursor) {
    VecFx32 position;

    position.z = 0;
    if (cursor == 1 || cursor != 2) {
        position.x = 55 << 14;
        position.y = 30 << 14;
    } else {
        position.x = 55 << 14;
        position.y = 9 << 16;
    }
    Sprite_SetMatrix(menuView->cursor, &position);
    Sprite_SetAnimCtrlSeq(menuView->cursor, 2);
}

#pragma force_active on
void EasyChatMenuView_DrawModeButton(EasyChatMenuView *menuView, int mode, int selected) {
    NNSG2dScreenData *screen = menuView->modesScreen;

    CopyToBgTilemapRect(menuView->bgConfig, GF_BG_LYR_MAIN_3, mode * 17 + 2, 0, 11, 6, screen->rawData, 0, selected * 6, screen->screenWidth >> 3, screen->screenHeight >> 3);
}
#pragma force_active reset

void EasyChatMenuView_ModeSwitchTask(SysTask *task, void *data) {
    EasyChatModeSwitch *modeSwitch = data;

    switch (modeSwitch->state) {
    case 0:
        EasyChatMenuView_DrawModeButton(modeSwitch->menuView, (u8)modeSwitch->mode, 1);
        EasyChatMenuView_DrawModeButton(modeSwitch->menuView, (u8)(modeSwitch->mode ^ 1), 0);
        ScheduleBgTilemapBufferTransfer(modeSwitch->menuView->bgConfig, GF_BG_LYR_MAIN_3);
        modeSwitch->count++;
        modeSwitch->state++;
        break;
    case 1:
        modeSwitch->timer++;
        if (modeSwitch->timer >= 4) {
            modeSwitch->menuView->modeSwitchTask = NULL;
            Heap_Free(modeSwitch);
            SysTask_Destroy(task);
        }
        break;
    }
}

void EasyChatMenuView_StartModeSwitch(EasyChatMenuView *menuView) {
    u8 mode = EasyChatApp_GetListMode(menuView->app);
    EasyChatModeSwitch *modeSwitch = Heap_Alloc(HEAP_ID_35, 0x1C);

    modeSwitch->menuView = menuView;
    modeSwitch->state = 0;
    modeSwitch->timer = 0;
    modeSwitch->mode = mode;
    menuView->modeSwitchTask = SysTask_CreateOnMainQueue(EasyChatMenuView_ModeSwitchTask, modeSwitch, 1);
}

BOOL EasyChatMenuView_IsModeSwitchDone(EasyChatMenuView *menuView) {
    if (menuView->modeSwitchTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

void EasyChatMenuView_ShowListButton(EasyChatMenuView *menuView) {
    EasyChatMenuView_DrawListButton(menuView, TRUE);
    EasyChatMenuView_DrawMainButtons(menuView, FALSE, FALSE);
}

void EasyChatMenuView_ShowMainButtons(EasyChatMenuView *menuView) {
    EasyChatMenuView_DrawListButton(menuView, FALSE);
    EasyChatMenuView_DrawMainButtons(menuView, TRUE, FALSE);
}

// Category grid

EasyChatCategoryView *EasyChatCategoryView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu) {
    EasyChatCategoryView *categories = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatCategoryView));

    categories->view = view;
    categories->app = app;
    categories->menu = menu;
    categories->bgConfig = EasyChatAppView_GetBgConfig(view);
    categories->unk10 = 0;
    categories->unk18 = 0;
    categories->cursor = NULL;
    categories->menuInputState = EasyChatApp_GetMenuInputStatePtr(categories->app);
    return categories;
}

void EasyChatCategoryView_Delete(EasyChatCategoryView *categories) {
    if (categories->cursor != NULL) {
        Sprite_Delete(categories->cursor);
    }
    Heap_Free(categories);
}

void EasyChatCategoryView_Load(EasyChatCategoryView *categories, NARC *narc) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(categories->view);
    u32 size;
    u32 tile;
    int zero = 0;

    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_35);
    size = GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_35);
    tile = EasyChatCategoryView_DrawCategories(categories, bgConfig, size >> 5);
    tile = EasyChatCategoryView_DrawInitials(categories, bgConfig, tile);
    EasyChatCategoryView_LoadGfxDummy(categories, bgConfig, tile);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, (enum BgPosAdjustOp)zero, zero - 4);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 16);
    EasyChatCategoryView_CreateCursor(categories);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1, -10);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_1);
}

#pragma force_active on
u32 EasyChatCategoryView_DrawCategories(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile) {
    Window window;
    int y;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 282, HEAP_ID_35);
    int x = 1, i;

    y = 9;
    for (i = 0; i < 12; i++, tile += 18, x += 10) {
        String *string;
        u32 color;
        u32 center;
        if (i != 0 && i % 3 == 0) {
            y += 3;
            x = 1;
        }
        if (ECMenu_GetCategoryCount(categories->menu, i)) {
            string = NewString_ReadMsgData(msgData, i);
            color = MAKE_TEXT_COLOR(1, 2, 15);
        } else {
            string = NewString_ReadMsgData(msgData, 12);
            color = MAKE_TEXT_COLOR(3, 4, 15);
        }
        GF_ASSERT(tile < 1006);
        AddWindowParameterized(bgConfig, &window, GF_BG_LYR_MAIN_1, x, y, 9, 2, 11, tile);
        FillWindowPixelBuffer(&window, 15);
        center = (72 - FontID_String_GetWidth(0, string, 0)) / 2;
        AddTextPrinterParameterizedWithColor(&window, 0, string, center, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
        PutWindowTilemap(&window);
        CopyWindowPixelsToVram_TextMode(&window);
        String_Delete(string);
        RemoveWindow(&window);
    }
    DestroyMsgData(msgData);
    return tile;
}
#pragma force_active reset

u32 EasyChatCategoryView_DrawInitials(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile) {
    Window window;
    String *string;
    u32 count;
    u32 i;

    GF_ASSERT(tile < 0x338);
    AddWindowParameterized(bgConfig, &window, GF_BG_LYR_MAIN_1, 34, 10, 25, 8, 11, tile);
    string = String_New(4, HEAP_ID_35);
    FillWindowPixelBuffer(&window, 15);
    count = EasyChatInitialGrid_GetCount();
    for (i = 0; i < count; i++) {
        u32 x;
        u32 y;
        u32 color;
        EasyChatInitialGrid_GetLetter(i, string);
        EasyChatInitialGrid_GetPosition(i, &x, &y);
        color = ECMenu_GetInitialCount(categories->menu, i) != 0 ? MAKE_TEXT_COLOR(1, 2, 15) : MAKE_TEXT_COLOR(3, 4, 15);
        AddTextPrinterParameterizedWithColor(&window, 0, string, x + 3, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    }
    CopyWindowPixelsToVram_TextMode(&window);
    PutWindowTilemap(&window);
    String_Delete(string);
    RemoveWindow(&window);
    return tile + 200;
}

u32 EasyChatCategoryView_LoadGfxDummy(EasyChatCategoryView *categories, BgConfig *bgConfig, u32 tile) {
#pragma unused(categories, bgConfig)
    return tile;
}

void EasyChatCategoryView_CreateCursor(EasyChatCategoryView *categories) {
    SpriteResourcesHeader header;

    EasyChatAppView_InitSpriteHeader(categories->view, &header, 0, 0);
    categories->cursor = EasyChatAppView_CreateSprite(categories->view, &header, 48, 64, 2, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(categories->cursor, 4);
    Sprite_SetDrawFlag(categories->cursor, FALSE);
}

void EasyChatCategoryView_SetCursorVisible(EasyChatCategoryView *categories, BOOL draw) {
    if (draw) {
        Sprite_SetDrawFlag(categories->cursor, TRUE);
        if (EasyChatApp_GetListMode(categories->app) == 0) {
            Sprite_SetAnimCtrlSeq(categories->cursor, 4);
        } else {
            Sprite_SetAnimCtrlSeq(categories->cursor, 6);
        }
    } else {
        Sprite_SetDrawFlag(categories->cursor, FALSE);
    }
}

void EasyChatCategoryView_MoveCursor(EasyChatCategoryView *categories, u32 category) {
    EasyChatCategoryView *self;
    u32 anim;
    u32 x;
    u32 y;
    VecFx32 position;
    u32 mode;

    self = categories;
    mode = EasyChatApp_GetListMode(self->app);
    anim = category;
    position.z = 0;
    if (anim == 0xFE) {
        position.x = 224;
        position.y = 176;
        anim = 16;
    } else if (mode == 0) {
        position.x = (anim % 3) * 80 + 48;
        position.y = (anim / 3) * 24 + 64;
        anim = 4;
    } else {
        EasyChatInitialGrid_GetPosition(anim, &x, &y);
        position.x = x + 26;
        position.y = y + 72;
        anim = 6;
    }
    position.x <<= FX32_SHIFT;
    position.y <<= FX32_SHIFT;
    Sprite_SetMatrix(self->cursor, &position);
    Sprite_SetAnimCtrlSeq(self->cursor, anim);
}

void EasyChatCategoryView_StartOpen(EasyChatCategoryView *categories) {
    categories->state = 0;
    EasyChatBlendFade_StartBrightness(&categories->fade, GX_BLEND_PLANEMASK_BG1, -10, 0, 16);
}

#pragma force_active on
BOOL EasyChatCategoryView_UpdateOpen(EasyChatCategoryView *categories) {
    switch (categories->state) {
    case 0:
        if (EasyChatBlendFade_IsBrightnessDone(&categories->fade)) {
            EasyChatBgScroll_Start(&categories->scroll, categories->bgConfig, GF_BG_LYR_MAIN_1, 1, 0, 6);
            categories->state++;
        }
        break;
    case 1:
        return EasyChatBgScroll_Update(&categories->scroll);
    }
    return FALSE;
}
#pragma force_active reset

void EasyChatCategoryView_StartClose(EasyChatCategoryView *categories) {
    categories->state = 0;
    EasyChatBgScroll_Start(&categories->scroll, categories->bgConfig, GF_BG_LYR_MAIN_1, 1, 0, 6);
}

#pragma force_active on
BOOL EasyChatCategoryView_UpdateClose(EasyChatCategoryView *categories) {
    switch (categories->state) {
    case 0:
        if (EasyChatBgScroll_Update(&categories->scroll)) {
            EasyChatBlendFade_StartBrightness(&categories->fade, GX_BLEND_PLANEMASK_BG1, 0, -10, 16);
            categories->state++;
        }
        break;
    case 1:
        return EasyChatBlendFade_IsBrightnessDone(&categories->fade);
    }
    return FALSE;
}
#pragma force_active reset

void EasyChatCategoryView_ResetScrollY(EasyChatCategoryView *categories) {
    BgSetPosTextAndCommit(categories->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 16);
}

void EasyChatCategoryView_StartDim(EasyChatCategoryView *categories) {
    EasyChatBlendFade_StartBrightness(&categories->fade, GX_BLEND_PLANEMASK_BG1, 0, -10, 16);
}

BOOL EasyChatCategoryView_UpdateDim(EasyChatCategoryView *categories) {
    return EasyChatBlendFade_IsBrightnessDone(&categories->fade);
}

void EasyChatCategoryView_StartFadeOut(EasyChatCategoryView *categories) {
    EasyChatBlendFade_StartAlpha(&categories->fade, GX_BLEND_PLANEMASK_BG1, EC_BLEND_PLANEMASK_ALL, 124, 0, 12);
}

BOOL EasyChatCategoryView_UpdateFadeOut(EasyChatCategoryView *categories) {
    if (EasyChatBlendFade_IsAlphaDone(&categories->fade)) {
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
void EasyChatCategoryView_StartFadeIn(EasyChatCategoryView *categories) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, EC_BLEND_PLANEMASK_ALL, 0, 16);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
    EasyChatBlendFade_StartAlpha(&categories->fade, GX_BLEND_PLANEMASK_BG1, EC_BLEND_PLANEMASK_ALL, 0, 124, 12);
}
#pragma force_active reset

BOOL EasyChatCategoryView_UpdateFadeIn(EasyChatCategoryView *categories) {
    return EasyChatBlendFade_IsAlphaDone(&categories->fade);
}

void EasyChatCategoryView_ResetScrollX(EasyChatCategoryView *categories) {
    u32 x;

    if (EasyChatApp_GetListMode(categories->app) == 0) {
        x = -4;
    } else {
        x = 252;
    }
    BgSetPosTextAndCommit(categories->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, x);
}

#pragma force_active on
void EasyChatCategoryView_StartModeScroll(EasyChatCategoryView *categories) {
    int delta;

    if (EasyChatApp_GetListMode(categories->app) == 0) {
        delta = -256;
    } else {
        delta = 256;
    }
    EasyChatBgScroll_Start(&categories->scroll, categories->bgConfig, GF_BG_LYR_MAIN_1, 0, delta, 16);
}
#pragma force_active reset

BOOL EasyChatCategoryView_UpdateModeScroll(EasyChatCategoryView *categories) {
    return EasyChatBgScroll_Update(&categories->scroll);
}

// Word list

EasyChatWordListView *EasyChatWordListView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu) {
    EasyChatWordListView *wordList = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatWordListView));

    wordList->view = view;
    wordList->app = app;
    wordList->menu = menu;
    wordList->bgConfig = EasyChatAppView_GetBgConfig(view);
    wordList->word = String_New(32, HEAP_ID_35);
    AddWindowParameterized(wordList->bgConfig, &wordList->listWindow, GF_BG_LYR_MAIN_2, 4, 0, 26, 32, 12, 1);
    AddWindowParameterized(wordList->bgConfig, &wordList->wordWindow, GF_BG_LYR_MAIN_2, 0, 0, 12, 4, 12, 1);
    wordList->cursor = NULL;
    wordList->upArrow = NULL;
    wordList->downArrow = NULL;
    wordList->menuInputState = EasyChatApp_GetMenuInputStatePtr(wordList->app);
    return wordList;
}

void EasyChatWordListView_Delete(EasyChatWordListView *wordList) {
    if (wordList->cursor != NULL) {
        Sprite_Delete(wordList->cursor);
    }
    if (wordList->upArrow != NULL) {
        Sprite_Delete(wordList->upArrow);
    }
    if (wordList->downArrow != NULL) {
        Sprite_Delete(wordList->downArrow);
    }
    if (wordList->word != NULL) {
        String_Delete(wordList->word);
    }
    RemoveWindow(&wordList->wordWindow);
    RemoveWindow(&wordList->listWindow);
    Heap_Free(wordList);
}

void EasyChatWordListView_Load(EasyChatWordListView *wordList) {
    BgConfig *bgConfig = EasyChatAppView_GetBgConfig(wordList->view);

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_35);
    FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 32, 12);
    FillWindowPixelBuffer(&wordList->listWindow, 0);
    PutWindowTilemap(&wordList->listWindow);
    CopyWindowPixelsToVram_TextMode(&wordList->listWindow);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_2);
    EasyChatWordListView_CreateSprites(wordList);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
}

void EasyChatWordListView_CreateSprites(EasyChatWordListView *wordList) {
    SpriteResourcesHeader header;

    EasyChatAppView_InitSpriteHeader(wordList->view, &header, 0, 0);
    wordList->cursor = EasyChatAppView_CreateSprite(wordList->view, &header, 72, 56, 3, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(wordList->cursor, 8);
    Sprite_SetDrawFlag(wordList->cursor, FALSE);
    wordList->upArrow = EasyChatAppView_CreateSprite(wordList->view, &header, 240, 80, 4, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(wordList->upArrow, 24);
    Sprite_SetDrawFlag(wordList->upArrow, FALSE);
    wordList->downArrow = EasyChatAppView_CreateSprite(wordList->view, &header, 240, 128, 4, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetAnimCtrlSeq(wordList->downArrow, 26);
    Sprite_SetDrawFlag(wordList->downArrow, FALSE);
}

void EasyChatWordListView_DrawWords(EasyChatWordListView *wordList) {
    u32 count;
    u32 y;
    u32 i;

    FillWindowPixelBuffer(&wordList->listWindow, 0);
    EasyChatWordListView_ResetScroll(wordList);
    count = EasyChatApp_GetCategoryWordCount(wordList->app);
    if (count > 10) {
        count = 10;
    }
    y = wordList->scrollY;
    for (i = 0; i < count; i++) {
        EasyChatWordListView_DrawWord(wordList, i, y);
        if (i & 1) {
            y += 24;
        }
    }
    CopyWindowPixelsToVram_TextMode(&wordList->listWindow);
}

BOOL EasyChatWordListView_StartFadeIn(EasyChatWordListView *wordList) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, EC_BLEND_PLANEMASK_ALL, 0, 16);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
    wordList->fadeDone = 0;
    EasyChatBlendFade_StartAlpha(&wordList->fade, GX_BLEND_PLANEMASK_BG2, EC_BLEND_PLANEMASK_ALL, 0, 124, 12);
}

BOOL EasyChatWordListView_UpdateFadeIn(EasyChatWordListView *wordList) {
    if (wordList->fadeDone == 0) {
        if (EasyChatBlendFade_IsAlphaDone(&wordList->fade)) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void EasyChatWordListView_StartFadeOut(EasyChatWordListView *wordList) {
    wordList->fadeDone = 0;
    EasyChatBlendFade_StartAlpha(&wordList->fade, GX_BLEND_PLANEMASK_BG2, EC_BLEND_PLANEMASK_ALL, 124, 0, 12);
}

BOOL EasyChatWordListView_UpdateFadeOut(EasyChatWordListView *wordList) {
    if (wordList->fadeDone == 0) {
        if (!EasyChatBlendFade_IsAlphaDone(&wordList->fade)) {
            goto notDone;
        }
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        return TRUE;
    }
    return TRUE;
notDone:
    return FALSE;
}

void EasyChatWordListView_SetCursorVisible(EasyChatWordListView *wordList, BOOL draw) {
    if (draw) {
        Sprite_SetDrawFlag(wordList->cursor, TRUE);
        Sprite_SetDrawFlag(wordList->upArrow, EasyChatApp_CanScrollWordListUp(wordList->app));
        Sprite_SetDrawFlag(wordList->downArrow, EasyChatApp_CanScrollWordListDown(wordList->app));
    } else {
        Sprite_SetDrawFlag(wordList->cursor, draw);
        Sprite_SetDrawFlag(wordList->upArrow, FALSE);
        Sprite_SetDrawFlag(wordList->downArrow, FALSE);
    }
}

void EasyChatWordListView_MoveCursor(EasyChatWordListView *wordList, u32 cursor) {
    VecFx32 position;

    if (cursor == (u32)-1) {
        position.x = 14 << 16;
        position.y = 11 << 16;
        position.z = 0;
        Sprite_SetMatrix(wordList->cursor, &position);
        Sprite_SetAnimCtrlSeq(wordList->cursor, 16);
    } else {
        position.x = ((cursor & 1) * 112 + 72) << FX32_SHIFT;
        position.y = ((cursor >> 1) * 24 + 56) << FX32_SHIFT;
        position.z = 0;
        Sprite_SetMatrix(wordList->cursor, &position);
        Sprite_SetAnimCtrlSeq(wordList->cursor, 8);
    }
}

#pragma force_active on
void EasyChatWordListView_StartScroll(EasyChatWordListView *wordList, int rows) {
    int i;
    u8 y;
    u32 index;
    u8 nextY;
    u32 newY;
    u32 nextTopRow;
    int count, pixels, words;

    EasyChatWordListView_ClearRows(wordList, rows);
    words = rows * 2;
    index = wordList->topRow;
    nextTopRow = index + words;
    pixels = rows * 24;
    nextY = wordList->scrollY + pixels;
    newY = nextY;
    if (rows < 0) {
        y = nextY;
        index = nextTopRow;
        count = -words;
        words = -words;
    } else {
        y = wordList->scrollY + 120;
        index += 10;
        count = words;
        if (words + index > EasyChatApp_GetCategoryWordCount(wordList->app)) {
            count--;
        }
    }
    for (i = 0; i < count; i++) {
        EasyChatWordListView_DrawWord(wordList, index + i, y);
        if (i & 1) {
            y += 24;
        }
    }
    wordList->scrollY = newY;
    wordList->topRow = nextTopRow;
    CopyWindowPixelsToVram_TextMode(&wordList->listWindow);
    EasyChatBgScroll_Start(&wordList->scroll, wordList->bgConfig, GF_BG_LYR_MAIN_2, 1, pixels, words);
}
#pragma force_active reset

BOOL EasyChatWordListView_UpdateScroll(EasyChatWordListView *wordList) {
    if (EasyChatBgScroll_Update(&wordList->scroll)) {
        Sprite_SetDrawFlag(wordList->upArrow, EasyChatApp_CanScrollWordListUp(wordList->app));
        Sprite_SetDrawFlag(wordList->downArrow, EasyChatApp_CanScrollWordListDown(wordList->app));
        return TRUE;
    } else {
        return FALSE;
    }
}

void EasyChatWordListView_ClearRows(EasyChatWordListView *wordList, int rows) {
    int start;
    int end;

    if (rows > 0) {
        start = (u8)(wordList->scrollY + 120);
        end = (u8)(start + rows * 24);
    } else {
        end = wordList->scrollY;
        start = (u8)(end + rows * 24);
    }
    if (start < end) {
        FillWindowPixelRect(&wordList->listWindow, 0, 0, start, 208, end - start);
    } else {
        FillWindowPixelRect(&wordList->listWindow, 0, 0, start, 208, 256 - start);
        FillWindowPixelRect(&wordList->listWindow, 0, 0, 0, 208, end);
    }
}

void EasyChatWordListView_ResetScroll(EasyChatWordListView *wordList) {
    wordList->scrollY = 48;
    wordList->topRow = 0;
    BgSetPosTextAndCommit(wordList->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
}

#pragma force_active on
void EasyChatWordListView_DrawWord(EasyChatWordListView *wordList, int index, u32 y) {
    u32 x, height;

    EasyChatApp_GetCategoryWord(wordList->app, index, wordList->word);
    if (y <= 240) {
        AddTextPrinterParameterizedWithColor(&wordList->listWindow, 0, wordList->word, (index & 1) * 112, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        return;
    }
    // the word wraps around the bottom of the 256 pixel high window: print it apart and copy both halves
    height = 256 - y;
    FillWindowPixelBuffer(&wordList->wordWindow, 0);
    AddTextPrinterParameterizedWithColor(&wordList->wordWindow, 0, wordList->word, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    x = (index & 1) * 112;
    BlitBitmapRectToWindow(&wordList->listWindow, wordList->wordWindow.pixelBuffer, 0, 0, 96, 16, x, y, 96, height);
    BlitBitmapRectToWindow(&wordList->listWindow, wordList->wordWindow.pixelBuffer, 0, height, 96, 16, x, 0, 96, 16 - height);
}
#pragma force_active reset

// Page buttons

EasyChatPageButtonsView *EasyChatPageButtonsView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu) {
    EasyChatPageButtonsView *pageButtons = Heap_Alloc(HEAP_ID_35, sizeof(EasyChatPageButtonsView));

    pageButtons->view = view;
    pageButtons->app = app;
    pageButtons->menu = menu;
    pageButtons->bgConfig = EasyChatAppView_GetBgConfig(view);
    pageButtons->spriteList = EasyChatAppView_GetSpriteList(view);
    pageButtons->unk14 = 0;
    pageButtons->unk18 = 0;
    pageButtons->unk2C = 0;
    return pageButtons;
}

void EasyChatPageButtonsView_Delete(EasyChatPageButtonsView *pageButtons) {
    Heap_Free(pageButtons);
}

#pragma force_active on
void EasyChatPageButtonsView_Load(EasyChatPageButtonsView *pageButtons, NARC *narc) {
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 11, pageButtons->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 12, pageButtons->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_35);
    BgCommitTilemapBufferToVram(pageButtons->bgConfig, GF_BG_LYR_SUB_1);
}
#pragma force_active reset

void EasyChatPageButtonsView_SetEnabled(void) {
}

void EasyChatPageButtonsView_SetPressed(EasyChatPageButtonsView *pageButtons, int button, int pressed) {
#pragma unused(pageButtons, button, pressed)
}

// BG scroll and blend fade tasks

#pragma force_active on
void EasyChatBgScroll_Start(EasyChatBgScroll *scroll, BgConfig *bgConfig, int layer, int axis, int delta, int frames) {
    scroll->bgConfig = bgConfig;
    scroll->layer = layer;
    if (axis == 0) {
        scroll->position = Bg_GetXpos(bgConfig, (GFBgLayer)layer);
        scroll->op = BG_POS_OP_SET_X;
    } else {
        scroll->position = Bg_GetYpos(bgConfig, (GFBgLayer)layer);
        scroll->op = BG_POS_OP_SET_Y;
    }
    scroll->target = (int)(((u32)scroll->position + (u32)delta) & 511u);
    delta = (s32)((u32)delta << FX32_SHIFT);
    scroll->position = (s32)((u32)scroll->position << FX32_SHIFT);
    scroll->step = delta / frames;
    scroll->frames = frames;
    scroll->state = 0;
}
#pragma force_active reset

BOOL EasyChatBgScroll_Update(EasyChatBgScroll *scroll) {
    switch (scroll->state) {
    case 0:
        scroll->task = EasyChatAppView_CreateVBlankTask(EasyChatBgScroll_Task, scroll, 0);
        scroll->state++;
        break;
    case 1:
        if (scroll->frames == 0) {
            SysTask_Destroy(scroll->task);
            scroll->state++;
            return TRUE;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

void EasyChatBgScroll_Task(SysTask *task, void *data) {
    EasyChatBgScroll *scroll = data;

    if (scroll->frames != 0) {
        scroll->position += scroll->step;
        BgSetPosTextAndCommit(scroll->bgConfig, scroll->layer, (enum BgPosAdjustOp)(u8)scroll->op, scroll->position >> FX32_SHIFT);
        scroll->frames--;
    } else {
        BgSetPosTextAndCommit(scroll->bgConfig, scroll->layer, (enum BgPosAdjustOp)(u8)scroll->op, scroll->target);
    }
}

void EasyChatBlendFade_StartAlpha(EasyChatBlendFade *fade, u32 plane1, u32 plane2, s32 start, s32 target, s32 frames) {
    fade->plane1 = plane1;
    fade->plane2 = plane2;
    fade->frames = frames;
    fade->value = start;
    fade->step = (target - start) / frames;
    fade->target = target;
    fade->done = 0;
    fade->task = EasyChatAppView_CreateVBlankTask(EasyChatBlendFade_AlphaTask, fade, 0);
}

BOOL EasyChatBlendFade_IsAlphaDone(EasyChatBlendFade *fade) {
    if (fade->done == 0) {
        if (fade->frames == 0) {
            SysTask_Destroy(fade->task);
            fade->done++;
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

void EasyChatBlendFade_AlphaTask(SysTask *task, void *data) {
    EasyChatBlendFade *fade = data;
    s32 value;

    if (fade->frames != 0) {
        fade->value += fade->step;
        value = fade->value >> 3;
        fade->frames--;
    } else {
        value = fade->target >> 3;
    }
    if (value > 16) {
        value = 16;
    }
    G2_SetBlendAlpha(fade->plane1, fade->plane2, value, 16 - value);
}

void EasyChatBlendFade_StartBrightness(EasyChatBlendFade *fade, u32 plane, s32 start, s32 target, s32 frames) {
    G2_SetBlendBrightness(plane, start);
    fade->plane1 = plane;
    fade->frames = frames;
    fade->value = start << FX32_SHIFT;
    fade->target = target << FX32_SHIFT;
    fade->step = (fade->target - fade->value) / frames;
    fade->done = 0;
    fade->task = EasyChatAppView_CreateVBlankTask(EasyChatBlendFade_BrightnessTask, fade, 0);
}

BOOL EasyChatBlendFade_IsBrightnessDone(EasyChatBlendFade *fade) {
    if (fade->done == 0) {
        if (fade->frames == 0) {
            SysTask_Destroy(fade->task);
            fade->done++;
            return TRUE;
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

void EasyChatBlendFade_BrightnessTask(SysTask *task, void *data) {
    EasyChatBlendFade *fade = data;
    s32 value;

    if (fade->frames != 0) {
        fade->value += fade->step;
        value = fade->value >> FX32_SHIFT;
        fade->frames--;
    } else {
        value = fade->target >> FX32_SHIFT;
    }
    G2_SetBlendBrightness(fade->plane1, value);
}

// Initial letter grid

u32 EasyChatInitialGrid_GetCount(void) {
    return EC_INITIAL_GRID_CELLS;
}

void EasyChatInitialGrid_GetLetter(u32 cell, String *dest) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    CopyU16ArrayToStringN(dest, &ov102_021EC820[cell].letter, 2);
}

void EasyChatInitialGrid_GetPosition(u32 cell, u32 *x, u32 *y) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    *x = ov102_021EC820[cell].x;
    *y = ov102_021EC820[cell].y;
}

u8 EasyChatInitialGrid_GetUp(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].up;
}

u8 EasyChatInitialGrid_GetDown(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].down;
}

u8 EasyChatInitialGrid_GetLeft(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].left;
}

u8 EasyChatInitialGrid_GetRight(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].right;
}

u8 EasyChatInitialGrid_GetBelowButton(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].belowButton;
}

u8 EasyChatInitialGrid_GetAboveButton(u32 cell) {
    GF_ASSERT(cell < EC_INITIAL_GRID_CELLS);
    return ov102_021EC820[cell].aboveButton;
}

#pragma force_active on
int EasyChatInitialGrid_FindCellAt(u32 x, u32 y) {
    u32 i;
    const EasyChatInitialGridCell *cell;

    for (i = 0, cell = ov102_021EC820; i < EC_INITIAL_GRID_CELLS; i++, cell++) {
        if (cell->x <= x && x < cell->x + 16 && cell->y <= y && y < cell->y + 16) {
            return i;
        }
    }
    return -1;
}
#pragma force_active reset
