#ifndef POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H

#include "global.h"

#include "wifi_setup/wifi_setup_ui.h"

// Connection screens of the access-point setup (ARM code in overlay 13):
//  - the rest of the connection settings screen (cursor, scrolling, switch buttons, completeness check),
//    the "settings saved" screen, the four steps of the automatic setup (search, connect, receive the
//    settings, done), the access-point scan screen;
//  - the screen of the three connection slots, the setup method menu, the connection test with its
//    prompt and result, and the screens around the Wi-Fi USB connector.
// Every screen is a chain of state functions set with WifiSetupApp_SetStateFunc:
// Init -> FadeIn -> WaitFadeIn -> (WaitButtonBar) -> Main (HandleInput, Update, CheckButtons each
// frame) -> Exit -> WaitFadeOut -> End, which picks the next screen.

// gWifiSetupApScanScreen.result
#define WIFI_SETUP_AP_SCAN_RUNNING  0
#define WIFI_SETUP_AP_SCAN_FOUND    1
#define WIFI_SETUP_AP_SCAN_NONE     2
#define WIFI_SETUP_AP_SCAN_ONLY_WPA 3 // every access point found uses WPA

// WifiSetupConnectionListWork.exit
#define WIFI_SETUP_CONNECTION_LIST_STAY 0
#define WIFI_SETUP_CONNECTION_LIST_NEXT 1 // A on a slot
#define WIFI_SETUP_CONNECTION_LIST_BACK 2

// gWifiSetupConnectionList.selected: 0-2 the connection slots, 3 the Wi-Fi USB connector,
// 4-6 the erase buttons of slots 0-2
#define WIFI_SETUP_CONNECTION_LIST_USB         3
#define WIFI_SETUP_CONNECTION_LIST_FIRST_ERASE 4

// gWifiSetupMethodMenu.selected
#define WIFI_SETUP_METHOD_MENU_AP_SCAN 0
#define WIFI_SETUP_METHOD_MENU_CLIENT  1 // WifiSetupAoss_Init
#define WIFI_SETUP_METHOD_MENU_AUTO    2 // WifiSetupAutoSearch_Init
#define WIFI_SETUP_METHOD_MENU_MANUAL  3

typedef struct WifiSetupApScanScreen {
    u8 result; // WIFI_SETUP_AP_SCAN_*
    u16 timer;
} WifiSetupApScanScreen;

typedef struct WifiSetupConnectionListWork {
    void *graphics;                     // 0x00, palette rows of the slots
    WifiSetupOamBlock *slotIcons[3];    // 0x04, one per setup method, or the empty slot icon
    WifiSetupOamBlock *eraseButtons[3]; // 0x10, NULL for an empty slot
    u8 column;                          // 0x1C, slot the cursor left from, for the erase row
    u8 exit;                            // 0x1D, WIFI_SETUP_CONNECTION_LIST_*
} WifiSetupConnectionListWork;

typedef struct WifiSetupConnectionList {
    u8 selected;
    WifiSetupConnectionListWork *work;
} WifiSetupConnectionList;

typedef struct WifiSetupMethodMenu {
    s8 lastOption; // option the cursor returns to (move table value -2)
    s8 selected;   // WIFI_SETUP_METHOD_MENU_*
    u8 confirmed;
    void *graphics; // palette rows of the options
} WifiSetupMethodMenu;

extern u8 gWifiSetupAutoConnectDone;
extern u8 gWifiSetupAutoReceiveDone;
extern u8 gWifiSetupAutoDoneTimer;
extern u8 gWifiSetupAutoSearchDone;
extern WifiSetupApScanScreen gWifiSetupApScanScreen;
extern WifiSetupConnectionList gWifiSetupConnectionList;
extern void *ov13_0224F570; // = gWifiSetupConnectionList.work
extern WifiSetupMethodMenu gWifiSetupMethodMenu;
extern u8 gWifiSetupConnectionTestPassed;
extern u8 gWifiSetupConnectionOkTimer;
extern u8 gWifiSetupUsbConnectorDoneTimer;
extern u8 gWifiSetupUsbConnectorIntroNext;
extern u8 gWifiSetupUsbConnectorExit;

int WifiSetupSettings_GetItemRow(int item);
int WifiSetupSettings_GetRowItem(int row);
void WifiSetupSettings_ScrollUpTask(void *task);
void WifiSetupSettings_ScrollDownTask(void *task);
void WifiSetupSettings_ScrollTask(void);
void WifiSetupSettings_ShowCursor(void);
void WifiSetupSettings_UpdateCursorItem(void);
void WifiSetupSettings_SetCursorItem(u8 item);
void WifiSetupSettings_MoveCursor(int dir);
void WifiSetupSettings_HighlightButton(void);
void WifiSetupSettings_ButtonTask(void);
void WifiSetupSettings_PressButton(void);
BOOL WifiSetupSettings_IsComplete(void);
void WifiSetupSettings_WaitIncompleteDialog(void);
void WifiSetupSettings_SetFromMenu(u8 fromMenu);

void WifiSetupSaveScreen_Init(void);

void WifiSetupAutoSearch_Init(void);

void WifiSetupApScanScreen_Init(void);

void WifiSetupConnectionList_Init(void);
void WifiSetupConnectionList_CreateIcons(void);
void WifiSetupConnectionList_FadeIn(void);
void WifiSetupConnectionList_ShowCursor(void);

void WifiSetupMethodMenu_Init(void);

void WifiSetupConnectionTest_Init(void);
void *WifiSetupConnectionTest_Alloc(void *arg, u32 size);
void WifiSetupConnectionTest_Free(void *arg, void *ptr);

void WifiSetupTestPrompt_Init(void);

void WifiSetupUsbConnectorDone_Init(void);

void WifiSetupUsbConnectorIntro_Init(void);

// Wi-Fi USB connector screen
void WifiSetupUsbConnector_Start(void);
void WifiSetupUsbConnector_ScanCallback(int result);
BOOL WifiSetupUsbConnector_IsLidClosed(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H
