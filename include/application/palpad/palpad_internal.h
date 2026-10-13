#ifndef POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
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
#include "touchscreen_list_menu.h"
#include "unk_02009D48.h"
#include "friend_roster.h"
#include "yes_no_prompt.h"

// Pal Pad (OVY_43): friend roster, friend code registration and friend records.
// PalPad_Main runs one screen at a time (PalPadWork.screen); a finished screen names the next one
// through PalPadShared_SetNextScreen.

enum PalPadScreen {
    PALPAD_SCREEN_MENU,
    PALPAD_SCREEN_ROSTER,
    PALPAD_SCREEN_NAMING,
    PALPAD_SCREEN_FRIEND_CODE,
    PALPAD_SCREEN_DETAILS,
    PALPAD_SCREEN_EXIT,
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
    BgConfig *bgConfig;                          // 0x000
    SpriteList *spriteList;                      // 0x004
    NNSG2dRenderSurface renderSurface;           // 0x008
    MessageFormat *msgFormat;                    // 0x050
    MsgData *msgData;                            // 0x054, msg_0779
    NARC *narc;                                  // 0x058, a/0/8/5
    TouchscreenListMenuSpawner *listMenuSpawner; // 0x05C
    YesNoPrompt *yesNoPrompt;                    // 0x060
    Window titleWindow;                          // 0x064
    String *titleText;                           // 0x074
    String *titleBuffer;                         // 0x078
    String *pageText;                            // 0x07C
    String *pageBuffer;                          // 0x080
    u32 titlePrinterId;                          // 0x084
    SpriteResourcesHeader spriteHeader;          // 0x088
    SpriteResource *spriteResources[4];          // 0x0AC
    G2dRenderer renderer;                        // 0x0BC
    GF_2DGfxResMan *resourceManagers[4];         // 0x1E4
    Sprite *arrows[2];                           // 0x1F4, left/up and right/down
    Sprite *cursor;                              // 0x1FC
    PalPadScreens screens;                       // 0x200
} PalPadGfx;

// Snapshot of the registered friends, taken whenever the roster is (re)drawn.
typedef struct PalPadFriendList {
    u32 count;                     // 0x00
    int numberedCount;             // 0x04
    u8 slots[PALPAD_FRIEND_MAX];   // 0x08, friend list slot of each listed friend
    u8 numbers[PALPAD_FRIEND_MAX]; // 0x28, 0xFF = friend code not confirmed yet
} PalPadFriendList;

// State shared by the screens (friend list snapshot, strings handed between screens).
typedef struct PalPadShared {
    u32 inputState;           // 0x00, MenuInputState: 1 = last input was a touch
    SaveData *saveData;       // 0x04
    u8 subState;              // 0x08, state of the running screen
    u8 nextScreen;            // 0x09
    u8 nextSubState;          // 0x0A
    s8 selectedFriend;        // 0x0B, index into friends.slots
    u8 textSpeed;             // 0x0C
    u8 frame;                 // 0x0D
    u8 lrPaging;              // 0x0E, L/R turn pages (button mode is not L=A)
    PalPadFriendList friends; // 0x10
    String *name;             // 0x58
    String *code;             // 0x5C
    const u16 *initialName;   // 0x60
    BOOL useInitialName;      // 0x64
} PalPadShared;

// Screen 0: top menu
typedef struct PalPadMenu {
    u32 selection;            // 0x00
    u32 timer;                // 0x04
    Window menuWindow;        // 0x08
    void *screenFile;         // 0x18
    NNSG2dScreenData *screen; // 0x1C
    Window msgWindow;         // 0x20
    u32 unused30;             // 0x30
    u32 printerId;            // 0x34
    u32 textSpeed;            // 0x38
    String *msgText;          // 0x3C
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
    PalPadPanelLayout layout;                                // 0x00
    Window window;                                           // 0x08
    CharSprite *icons[PALPAD_ROSTER_PAGE_SLOT]; // 0x18
} PalPadRosterPage;

typedef struct PalPadRoster {
    s16 page;                              // 0x00
    s16 prevPage;                          // 0x02
    s16 cursor;                            // 0x04, 0-7 = slot, 8 = return
    s16 prevCursor;                        // 0x06
    PalPadRosterPage pages[3];             // 0x08
    s16 scrollStep;                        // 0xB0
    u16 scrollDir;                         // 0xB2
    CharSpriteManager *trainerIcons; // 0xB4
    Window returnWindow;                   // 0xB8
    Window msgWindow;                      // 0xC8
    String *msgText;                       // 0xD8
    u32 printerId;                         // 0xDC
    u32 textSpeed;                         // 0xE0
    ListMenuItem *menuItems[2];            // 0xE4
    TouchscreenListMenu *menu;             // 0xEC
    u8 unk_F0[4];
    int timer;            // 0xF4
    Sprite *deleteEffect; // 0xF8
    u32 lastFrame;        // 0xFC
} PalPadRoster;

// Screen 2: naming screen + code entry run as nested applications
typedef struct PalPadNaming {
    OverlayManager *manager;
    NamingScreenArgs *namingArgs;
    void *codeEntryArgs;
} PalPadNaming;

// Screen 3: own friend code
typedef struct PalPadFriendCode {
    Window windows[3];        // 0x00
    void *screenFile;         // 0x30
    NNSG2dScreenData *screen; // 0x34
    int timer;                // 0x38
} PalPadFriendCode;

// Screen 4: friend details, 3 record pages (8 with the Battle Frontier pages)
typedef struct PalPadDetailsSlot {
    PalPadPanelLayout layout; // 0x00
    u32 baseTile;             // 0x08
    Window *windows[8];       // 0x0C, one window array per page
    u32 windowCounts[8];      // 0x2C
} PalPadDetailsSlot;

typedef struct PalPadDetailsScreen {
    void *file;
    NNSG2dScreenData *data;
} PalPadDetailsScreen;

typedef struct PalPadDetails {
    s16 page;                       // 0x000
    s16 prevPage;                   // 0x002
    int scrollStep;                 // 0x004
    int direction;                  // 0x008, page scroll 2: left, 3: right; friend change 0: previous, 1: next
    u16 friendChangeDelay;          // 0x00C
    u16 friendChangeHeld;           // 0x00E
    BOOL hasFrontierPages;          // 0x010
    PalPadDetailsSlot slots[3];     // 0x014
    PalPadDetailsScreen screens[4]; // 0x0F8
    Window profileWindows[8];       // 0x118
} PalPadDetails;

typedef struct PalPadWindowSpec {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} PalPadWindowSpec;

typedef struct PalPadWork {
    u32 screen;                  // 0x000, enum PalPadScreen
    PalPadGfx gfx;               // 0x004
    PalPadShared shared;         // 0x214
    PalPadArgs *args;            // 0x27C
    PalPadMenu menu;             // 0x280
    PalPadRoster roster;         // 0x2C0
    PalPadNaming naming;         // 0x3C0
    PalPadFriendCode friendCode; // 0x3CC
    PalPadDetails details;       // 0x408
    s32 screenChangeDelay;       // 0x5A0
} PalPadWork;

// No public header declares these (friend list save, DWC friend keys).
int FriendRoster_IsFriendValid(FriendRoster *wifiList, int slot);
void *FriendRoster_GetDwcFriendData(FriendRoster *wifiList, int slot);
void sub_0202C270(FriendRoster *wifiList, int slot, String *name);
void FriendRoster_RemoveFriend(FriendRoster *wifiList, int slot);
void FrontierSave_DeleteRow(void *frontier, int slot);
int Comm_FindFriendSlotByKey(SaveData *saveData, u64 friendKey, int *slot);
u64 DWC_CreateFriendKey(void *userData);
int DWC_CheckFriendKey(void *userData, u64 friendKey);
void DWC_CreateFriendKeyToken(void *friendData, u64 friendKey);
u64 DWC_GetFriendKey(void *friendData);

// palpad.c
void PalPad_VBlankCallback(void *data);
void PalPad_SetupScreens(PalPadWork *work, enum HeapID heapId);
void PalPad_TeardownScreens(PalPadWork *work);
void PalPad_AddWindow(BgConfig *bgConfig, Window *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile, u8 fillValue);
void PalPadShared_Init(PalPadShared *shared, PalPadArgs *args, enum HeapID heapId);
void PalPadShared_Free(PalPadShared *shared, PalPadArgs *args);
void PalPadShared_SetNextScreen(PalPadShared *shared, u8 screen, u8 subState);
void PalPadShared_ResetInputStateOnKey(u32 *inputState);
void PalPadShared_SetNameAndCode(PalPadShared *shared, const String *name, const String *code);
int PalPadShared_RegisterFriendCode(PalPadShared *shared, String *code, String *name);
void PalPadShared_SetInitialName(PalPadShared *shared, const u16 *name);
void PalPadShared_SetSelectedFriend(PalPadShared *shared, int index);
void PalPadShared_ReadFriendList(PalPadShared *shared);
void PalPadGfx_Init(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId);
void PalPadGfx_RenderSprites(PalPadGfx *gfx);
void PalPadGfx_VBlank(PalPadGfx *gfx);
void PalPadGfx_Free(PalPadGfx *gfx);
void PalPadGfx_SetVramBanks(void);
void PalPadGfx_InitBgs(PalPadGfx *gfx, enum HeapID heapId);
void PalPadGfx_FreeBgs(PalPadGfx *gfx);
void PalPadGfx_InitSpriteSystem(PalPadGfx *gfx, enum HeapID heapId);
void PalPadGfx_FreeSpriteSystem(PalPadGfx *gfx);
void PalPadGfx_InitMsg(PalPadGfx *gfx, enum HeapID heapId);
void PalPadGfx_FreeMsg(PalPadGfx *gfx);
void PalPadGfx_InitWindows(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId);
void PalPadGfx_FreeWindows(PalPadGfx *gfx);
void PalPadGfx_LoadScreens(PalPadGfx *gfx, enum HeapID heapId);
void PalPadGfx_FreeScreens(PalPadGfx *gfx);
void PalPadGfx_PrintTitle(PalPadGfx *gfx, Window *window, int msgId);
void PalPadGfx_ClearTitle(PalPadGfx *gfx);
void PalPadGfx_BufferFriendCode(PalPadGfx *gfx, u64 friendCode);
void PalPadGfx_BufferFriendName(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId);
void ov43_0222AB5C(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId);
BOOL PalPadGfx_BufferLastDate(PalPadGfx *gfx, SaveData *saveData, int slot);
void PalPadGfx_BufferNumber(PalPadGfx *gfx, int value);
void PalPadGfx_BufferActivityName(PalPadGfx *gfx, int activity);
void PalPadGfx_CreateSprites(PalPadGfx *gfx, enum HeapID heapId);
void PalPadGfx_DeleteSprites(PalPadGfx *gfx);
void PalPadGfx_SetArrowsVisible(PalPadGfx *gfx, BOOL visible);
void PalPadGfx_SetArrowsAnimActive(PalPadGfx *gfx, BOOL active);
void PalPadGfx_AnimateArrow(PalPadGfx *gfx, u8 arrow, int pressed);
void PalPadGfx_SetCursor(PalPadGfx *gfx, BOOL visible, int animSeq);
void PalPadGfx_SetCursorPriority(PalPadGfx *gfx, u8 priority, int drawPriority);
void PalPadGfx_StartYesNo(u32 *inputState, PalPadGfx *gfx, int initialCursorPos);
int PalPadGfx_HandleYesNo(u32 *inputState, PalPadGfx *gfx);

// palpad_menu.c
void PalPadMenu_Init(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadMenu_Main(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadMenu_Exit(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);
void PalPadMenu_Setup(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadMenu_Teardown(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx);

// palpad_roster.c (+ PalPadNaming_Init)
void PalPadRoster_Init(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadRoster_Main(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_Exit(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_Setup(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadRoster_Teardown(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void PalPadRoster_DeselectTab(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage);
void PalPadRoster_DrawTabsOnScroll(PalPadRoster *roster, PalPadGfx *gfx, s16 page, s16 prevPage);

void PalPadNaming_Init(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

// palpad_friend_code.c
int PalPadNaming_Main(PalPadNaming *naming, PalPadWork *work, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadNaming_Exit(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx);
void PalPadNaming_Setup(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadNaming_Teardown(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx);

void PalPadFriendCode_Init(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadFriendCode_Main(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void PalPadFriendCode_Exit(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void PalPadFriendCode_Setup(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadFriendCode_Teardown(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);

// palpad_details.c
void PalPadDetails_Init(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
int PalPadDetails_Main(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Exit(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_Setup(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Teardown(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetailsSlot_CopyWindowsToVram(PalPadDetailsSlot *slot, u32 page);
void PalPadDetailsSlot_ClearWindows(PalPadDetailsSlot *slot, u32 page);
void PalPadDetails_PrintUnionRoomPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_PrintBattleTowerPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_PrintBattleFactoryPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleCastlePage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleHallPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintBattleArcadePage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintWiFiPlazaPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_PrintReturnPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx);

// palpad_details_text.c
void PalPadDetailsSlot_ClearBg(PalPadGfx *gfx, PalPadDetailsSlot *slot);
void PalPadDetailsSlot_Print(PalPadDetailsSlot *slot, u32 page, u32 window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color, String *text, String *buffer, int align);
void PalPadDetails_PrintProfileText(Window *window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color);
void PalPadDetails_MovePage(PalPadDetails *details, int delta);

#endif // POKEHEARTGOLD_APPLICATION_PALPAD_PALPAD_INTERNAL_H
