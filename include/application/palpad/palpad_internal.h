#ifndef POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "friend_roster.h"
#include "message_format.h"
#include "msgdata.h"
#include "naming_screen.h"
#include "oam_manager.h"
#include "overlay_42.h"
#include "overlay_43.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

// Pal Pad: friend roster, friend code registration and friend records.

enum PalPadScreen {
    PALPAD_SCREEN_MENU,
    PALPAD_SCREEN_ROSTER,
    PALPAD_SCREEN_NAMING,
    PALPAD_SCREEN_FRIEND_CODE,
    PALPAD_SCREEN_DETAILS,
    PALPAD_SCREEN_EXIT,
};

enum PalPadMenuState {
    PALPAD_MENU_STATE_FADE_IN,
    PALPAD_MENU_STATE_WAIT_FADE_IN,
    PALPAD_MENU_STATE_SHOW,
    PALPAD_MENU_STATE_INPUT,
    PALPAD_MENU_STATE_SELECT_DELAY,
    PALPAD_MENU_STATE_FADE_OUT,
    PALPAD_MENU_STATE_WAIT_FADE_OUT,
    PALPAD_MENU_STATE_RETURN_FROM_NAMING,
    PALPAD_MENU_STATE_CHECK_FRIEND_CODE,
    PALPAD_MENU_STATE_START_REGISTER_YES_NO,
    PALPAD_MENU_STATE_REGISTER_YES_NO,
    PALPAD_MENU_STATE_WAIT_MESSAGE,
};

enum PalPadRosterState {
    PALPAD_ROSTER_STATE_INIT,
    PALPAD_ROSTER_STATE_SHOW,
    PALPAD_ROSTER_STATE_INPUT,
    PALPAD_ROSTER_STATE_START_SCROLL,
    PALPAD_ROSTER_STATE_SCROLL,
    PALPAD_ROSTER_STATE_MENU_DELAY,
    PALPAD_ROSTER_STATE_FRIEND_MENU,
    PALPAD_ROSTER_STATE_OPEN_DETAILS,
    PALPAD_ROSTER_STATE_WAIT_OPEN_DETAILS,
    PALPAD_ROSTER_STATE_RETURN_FROM_DETAILS,
    PALPAD_ROSTER_STATE_RENAME,
    PALPAD_ROSTER_STATE_WAIT_RENAME_FADE,
    PALPAD_ROSTER_STATE_SET_NEW_NAME,
    PALPAD_ROSTER_STATE_RETURN_FROM_NAMING,
    PALPAD_ROSTER_STATE_WAIT_FADE_IN,
    PALPAD_ROSTER_STATE_DELETE,
    PALPAD_ROSTER_STATE_START_DELETE_YES_NO,
    PALPAD_ROSTER_STATE_DELETE_YES_NO,
    PALPAD_ROSTER_STATE_START_DELETE_EFFECT,
    PALPAD_ROSTER_STATE_DELETE_EFFECT,
    PALPAD_ROSTER_STATE_END_DELETE_EFFECT,
    PALPAD_ROSTER_STATE_SHOW_FRIEND_CODE,
    PALPAD_ROSTER_STATE_WAIT_FRIEND_CODE,
    PALPAD_ROSTER_STATE_EXIT,
};

enum PalPadNamingState {
    PALPAD_NAMING_STATE_REGISTER,
    PALPAD_NAMING_STATE_ENTER_NAME,
    PALPAD_NAMING_STATE_ENTER_CODE,
    PALPAD_NAMING_STATE_EXIT_REGISTER,
    PALPAD_NAMING_STATE_RENAME,
    PALPAD_NAMING_STATE_ENTER_NEW_NAME,
    PALPAD_NAMING_STATE_EXIT_RENAME,
};

enum PalPadFriendCodeState {
    PALPAD_FRIEND_CODE_STATE_SHOW,
    PALPAD_FRIEND_CODE_STATE_INPUT,
    PALPAD_FRIEND_CODE_STATE_EXIT_DELAY,
};

enum PalPadDetailsState {
    PALPAD_DETAILS_STATE_INIT,
    PALPAD_DETAILS_STATE_SHOW,
    PALPAD_DETAILS_STATE_INPUT,
    PALPAD_DETAILS_STATE_START_SCROLL,
    PALPAD_DETAILS_STATE_SCROLL,
    PALPAD_DETAILS_STATE_CHANGE_FRIEND,
    PALPAD_DETAILS_STATE_EXIT,
};

#define PALPAD_FRIEND_MAX       32
#define PALPAD_ROSTER_PAGE_MAX  4
#define PALPAD_ROSTER_PAGE_SLOT 8

// any key that switches the input state back to buttons
#define PALPAD_KEYS_ANY (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)

// Bottom/top screen graphics, text and sprites shared by every screen.
typedef struct PalPadScreens {
    void *files[2];
    NNSG2dScreenData *data[2]; // [1]: roster frame and tabs
} PalPadScreens;

typedef struct PalPadGfx {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    NNSG2dRenderSurface renderSurface;
    MessageFormat *msgFormat;
    MsgData *msgData; // msg_0779
    NARC *narc;       // a/0/8/5
    TouchscreenListMenuSpawner *listMenuSpawner;
    YesNoPrompt *yesNoPrompt;
    Window titleWindow;
    String *titleText;
    String *titleBuffer;
    String *pageText;
    String *pageBuffer;
    u32 titlePrinterId;
    SpriteResourcesHeader spriteHeader;
    SpriteResource *spriteResources[4];
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[4];
    Sprite *arrows[2]; // left/up and right/down
    Sprite *cursor;
    PalPadScreens screens;
} PalPadGfx;

// Snapshot of the registered friends, taken whenever the roster is (re)drawn.
typedef struct PalPadFriendList {
    u32 count;
    int numberedCount;
    u8 slots[PALPAD_FRIEND_MAX];   // friend list slot of each listed friend
    u8 numbers[PALPAD_FRIEND_MAX]; // 0xFF = friend code not confirmed yet
} PalPadFriendList;

// State shared by the screens (friend list snapshot, strings handed between screens).
typedef struct PalPadShared {
    u32 inputState; // MenuInputState: 1 = last input was a touch
    SaveData *saveData;
    u8 subState; // state of the running screen
    u8 nextScreen;
    u8 nextSubState;
    s8 selectedFriend; // index into friends.slots
    u8 textSpeed;
    u8 frame;
    u8 lrPaging; // L/R turn pages (button mode is not L=A)
    PalPadFriendList friends;
    String *name;
    String *code;
    const u16 *initialName;
    BOOL useInitialName;
} PalPadShared;

// Screen 0: top menu
typedef struct PalPadMenu {
    u32 selection;
    u32 timer;
    Window menuWindow;
    void *screenFile;
    NNSG2dScreenData *screen;
    Window msgWindow;
    u32 unused30;
    u32 printerId;
    u32 textSpeed;
    String *msgText;
} PalPadMenu;

// Screen 1: friend roster, 4 pages of 8 slots; three page panels (current page and the two scrolling in)
// Position (in tiles, scrolled pages are off screen) and size of a roster page / details panel
typedef struct PalPadPanelLayout {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
} PalPadPanelLayout;

typedef struct PalPadRosterPage {
    PalPadPanelLayout layout;
    Window window;
    CharSprite *icons[PALPAD_ROSTER_PAGE_SLOT];
} PalPadRosterPage;

typedef struct PalPadRoster {
    s16 page;
    s16 prevPage;
    s16 cursor; // 0-7 = slot, 8 = return
    s16 prevCursor;
    PalPadRosterPage pages[3];
    s16 scrollStep;
    u16 scrollDir;
    CharSpriteManager *trainerIcons;
    Window returnWindow;
    Window msgWindow;
    String *msgText;
    u32 printerId;
    u32 textSpeed;
    ListMenuItem *menuItems[2];
    TouchscreenListMenu *menu;
    u8 unk_F0[4];
    int timer;
    Sprite *deleteEffect;
    u32 lastFrame;
} PalPadRoster;

// Screen 2: naming screen + code entry run as nested applications
typedef struct PalPadNaming {
    OverlayManager *manager;
    NamingScreenArgs *namingArgs;
    void *codeEntryArgs;
} PalPadNaming;

// Screen 3: own friend code
typedef struct PalPadFriendCode {
    Window windows[3];
    void *screenFile;
    NNSG2dScreenData *screen;
    int timer;
} PalPadFriendCode;

// Screen 4: friend details, 3 record pages (8 with the Battle Frontier pages)
typedef struct PalPadDetailsSlot {
    PalPadPanelLayout layout;
    u32 baseTile;
    Window *windows[8]; // one window array per page
    u32 windowCounts[8];
} PalPadDetailsSlot;

typedef struct PalPadDetailsScreen {
    void *file;
    NNSG2dScreenData *data;
} PalPadDetailsScreen;

typedef struct PalPadDetails {
    s16 page;
    s16 prevPage;
    int scrollStep;
    int direction; // page scroll 2: left, 3: right; friend change 0: previous, 1: next
    u16 friendChangeDelay;
    u16 friendChangeHeld;
    BOOL hasFrontierPages;
    PalPadDetailsSlot slots[3];
    PalPadDetailsScreen screens[4];
    Window profileWindows[8];
} PalPadDetails;

typedef struct PalPadWindowSpec {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} PalPadWindowSpec;

typedef struct PalPadWork {
    u32 screen; // enum PalPadScreen
    PalPadGfx gfx;
    PalPadShared shared;
    PalPadArgs *args;
    PalPadMenu menu;
    PalPadRoster roster;
    PalPadNaming naming;
    PalPadFriendCode friendCode;
    PalPadDetails details;
    s32 screenChangeDelay;
} PalPadWork;

int FriendRoster_IsFriendValid(FriendRoster *wifiList, int slot);
void *FriendRoster_GetDwcFriendData(FriendRoster *wifiList, int slot);
void FriendRoster_SetPlayerName(FriendRoster *wifiList, int slot, String *name);
void FriendRoster_RemoveFriend(FriendRoster *wifiList, int slot);
int Comm_FindFriendSlotByKey(SaveData *saveData, u64 friendKey, int *slot);
u64 DWC_CreateFriendKey(void *userData);
int DWC_CheckFriendKey(void *userData, u64 friendKey);
void DWC_CreateFriendKeyToken(void *friendData, u64 friendKey);
u64 DWC_GetFriendKey(void *friendData);

void PalPad_SetupScreens(PalPadWork *work, enum HeapID heapId);
void PalPad_TeardownScreens(PalPadWork *work);
void PalPad_AddWindow(BgConfig *bgConfig, Window *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile, u8 fillValue);
void PalPadShared_SetNextScreen(PalPadShared *shared, u8 screen, u8 subState);
void PalPadShared_ResetInputStateOnKey(u32 *inputState);
void PalPadShared_SetNameAndCode(PalPadShared *shared, const String *name, const String *code);
int PalPadShared_RegisterFriendCode(PalPadShared *shared, String *code, String *name);
void PalPadShared_SetInitialName(PalPadShared *shared, const u16 *name);
void PalPadShared_SetSelectedFriend(PalPadShared *shared, int index);
void PalPadShared_ReadFriendList(PalPadShared *shared);
void PalPadGfx_PrintTitle(PalPadGfx *gfx, Window *window, int msgId);
void PalPadGfx_ClearTitle(PalPadGfx *gfx);
void PalPadGfx_BufferFriendCode(PalPadGfx *gfx, u64 friendCode);
void PalPadGfx_BufferFriendName(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId);
void PalPadGfx_BufferFriendGroupName(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId);
BOOL PalPadGfx_BufferLastDate(PalPadGfx *gfx, SaveData *saveData, int slot);
void PalPadGfx_BufferNumber(PalPadGfx *gfx, int value);
void PalPadGfx_BufferActivityName(PalPadGfx *gfx, int activity);
void PalPadGfx_SetArrowsVisible(PalPadGfx *gfx, BOOL visible);
void PalPadGfx_SetArrowsAnimActive(PalPadGfx *gfx, BOOL active);
void PalPadGfx_AnimateArrow(PalPadGfx *gfx, u8 arrow, int pressed);
void PalPadGfx_SetCursor(PalPadGfx *gfx, BOOL visible, int animSeq);
void PalPadGfx_SetCursorPriority(PalPadGfx *gfx, u8 priority, int drawPriority);
void PalPadGfx_StartYesNo(u32 *inputState, PalPadGfx *gfx, int initialCursorPos);
int PalPadGfx_HandleYesNo(u32 *inputState, PalPadGfx *gfx);

void PalPadMenu_Init(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadMenu_Main(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadMenu_Exit(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);
void PalPadMenu_Setup(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadMenu_Teardown(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);

void PalPadRoster_Init(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadRoster_Main(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_Exit(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_Setup(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_Teardown(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_DeselectTab(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage);
void PalPadRoster_DrawTabsOnScroll(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage);

void PalPadNaming_Init(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

int PalPadNaming_Main(PalPadNaming *naming, PalPadWork *work, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadNaming_Exit(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx);
void PalPadNaming_Setup(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadNaming_Teardown(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx);

void PalPadFriendCode_Init(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadFriendCode_Main(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void PalPadFriendCode_Exit(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void PalPadFriendCode_Setup(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadFriendCode_Teardown(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);

void PalPadDetails_Init(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadDetails_Main(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Exit(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_Setup(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Teardown(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_PrintUnionRoomPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_PrintBattleTowerPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_PrintBattleFactoryPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleCastlePage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleHallPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleArcadePage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintWiFiPlazaPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintReturnPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);

void PalPadDetailsSlot_ClearBg(PalPadGfx *gfx, PalPadDetailsSlot *slot);
void PalPadDetailsSlot_Print(PalPadDetailsSlot *slot, u32 page, u32 window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color, String *text, String *buffer, int align);
void PalPadDetails_PrintProfileText(Window *window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color);
void PalPadDetails_MovePage(PalPadDetails *details, int delta);

#endif // POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H
