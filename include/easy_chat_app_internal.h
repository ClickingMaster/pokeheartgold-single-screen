#ifndef POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H
#define POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H

#include "heap.h"
#include "menu_input_state.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "touchscreen.h"

// Overlay 102 (easy chat): the app logic (EasyChatApp) drives a screen object (EasyChatAppView) by starting
// numbered view commands; the view reads the app state back through the getters below.
typedef struct EasyChatApp EasyChatApp;
typedef struct EasyChatAppView EasyChatAppView;
typedef struct ECMenu ECMenu;

// what the app edits (EasyChatArgs type, EasyChatApp_GetType)
enum {
    EASY_CHAT_TYPE_WORD,
    EASY_CHAT_TYPE_TWO_WORDS,
    EASY_CHAT_TYPE_MESSAGE,
};

// layout of the category list (EasyChatApp_GetListMode)
enum {
    EASY_CHAT_LIST_MODE_CATEGORIES,
    EASY_CHAT_LIST_MODE_INITIALS,
};

// commands of EasyChatAppView_StartCommand
enum {
    EASY_CHAT_VIEW_CMD_LOAD,
    EASY_CHAT_VIEW_CMD_UNLOAD,
    EASY_CHAT_VIEW_CMD_FADE_IN,
    EASY_CHAT_VIEW_CMD_REDRAW_SENTENCE,
    EASY_CHAT_VIEW_CMD_NOP,
    EASY_CHAT_VIEW_CMD_SHOW_CATEGORY_CURSOR,
    EASY_CHAT_VIEW_CMD_SHOW_WORD_LIST_CURSOR,
    EASY_CHAT_VIEW_CMD_FOCUS_MENU,
    EASY_CHAT_VIEW_CMD_FOCUS_SENTENCE,
    EASY_CHAT_VIEW_CMD_OPEN_CATEGORIES,
    EASY_CHAT_VIEW_CMD_CLOSE_CATEGORIES,
    EASY_CHAT_VIEW_CMD_OPEN_WORD_LIST,
    EASY_CHAT_VIEW_CMD_WORD_LIST_TO_CATEGORIES,
    EASY_CHAT_VIEW_CMD_CLOSE_WORD_LIST,
    EASY_CHAT_VIEW_CMD_WORD_LIST_TO_MENU,
    EASY_CHAT_VIEW_CMD_MOVE_MENU_CURSOR,
    EASY_CHAT_VIEW_CMD_SET_MENU_CURSOR,
    EASY_CHAT_VIEW_CMD_SET_CATEGORY_CURSOR,
    EASY_CHAT_VIEW_CMD_SET_WORD_LIST_CURSOR,
    EASY_CHAT_VIEW_CMD_SCROLL_WORD_LIST,
    EASY_CHAT_VIEW_CMD_HIDE_MESSAGE,
    EASY_CHAT_VIEW_CMD_CONFIRM_FINISH,
    EASY_CHAT_VIEW_CMD_CONFIRM_QUIT,
    EASY_CHAT_VIEW_CMD_SHOW_INCOMPLETE_MESSAGE,
    EASY_CHAT_VIEW_CMD_DRAW_YES_NO_CURSOR,
    EASY_CHAT_VIEW_CMD_CLOSE_YES_NO,
    EASY_CHAT_VIEW_CMD_SWITCH_MODE,
    EASY_CHAT_VIEW_CMD_SWITCH_LIST_MODE,
    EASY_CHAT_VIEW_CMD_PRESS_PAGE_UP,
    EASY_CHAT_VIEW_CMD_PRESS_PAGE_DOWN,
    EASY_CHAT_VIEW_CMD_RELEASE_PAGE_UP,
    EASY_CHAT_VIEW_CMD_RELEASE_PAGE_DOWN,
    EASY_CHAT_VIEW_CMD_COUNT,
};

EasyChatAppView *EasyChatAppView_Create(EasyChatApp *app, ECMenu *menu);
void EasyChatAppView_Delete(EasyChatAppView *view);
void EasyChatAppView_StartCommand(EasyChatAppView *view, u32 command);
BOOL EasyChatAppView_AreCommandsDone(EasyChatAppView *view);
BOOL EasyChatAppView_IsCommandDone(EasyChatAppView *view, u32 command);
Sprite *EasyChatAppView_GetSliderSprite(EasyChatAppView *view);
u32 EasyChatAppView_GetNumWordSlots(EasyChatAppView *view);
void EasyChatAppView_GetWordSlotHitbox(EasyChatAppView *view, TouchscreenHitbox *hitbox, u8 slot);
int EasyChatAppView_GetYesNoResult(EasyChatAppView *view);
u8 EasyChatInitialGrid_GetUp(u32 category);
u8 EasyChatInitialGrid_GetDown(u32 category);
u8 EasyChatInitialGrid_GetLeft(u32 category);
u8 EasyChatInitialGrid_GetRight(u32 category);
u8 EasyChatInitialGrid_GetBelowButton(u32 category);
u8 EasyChatInitialGrid_GetAboveButton(u32 category);
int EasyChatInitialGrid_FindCellAt(u32 x, u32 y);

MenuInputState *EasyChatApp_GetMenuInputStatePtr(EasyChatApp *app);
u32 EasyChatApp_GetType(EasyChatApp *app);
u8 EasyChatApp_GetPromptMsgNo(EasyChatApp *app);
u8 EasyChatApp_GetListMode(EasyChatApp *app);
u16 EasyChatApp_GetMsgBank(EasyChatApp *app);
u16 EasyChatApp_GetWord(EasyChatApp *app, int slot);
String *EasyChatApp_GetMessageString(EasyChatApp *app, enum HeapID heapID);
u8 EasyChatApp_GetMessageIndex(EasyChatApp *app);
u16 EasyChatApp_GetWordSlot(EasyChatApp *app);
u16 EasyChatApp_GetMenuCursor(EasyChatApp *app);
u16 EasyChatApp_GetCategory(EasyChatApp *app);
u32 EasyChatApp_GetCategoryWordCount(EasyChatApp *app);
void EasyChatApp_GetCategoryWord(EasyChatApp *app, u32 index, String *dest);
int EasyChatApp_GetWordListCursor(EasyChatApp *app);
s32 EasyChatApp_GetWordListScroll(EasyChatApp *app);
BOOL EasyChatApp_CanScrollWordListUp(EasyChatApp *app);
BOOL EasyChatApp_CanScrollWordListDown(EasyChatApp *app);
u32 EasyChatApp_GetFrame(EasyChatApp *app);
u8 EasyChatApp_GetYesNoCursor(EasyChatApp *app);

#endif // POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H
