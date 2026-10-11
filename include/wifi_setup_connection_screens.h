#ifndef POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H

#include "global.h"

#include "wifi_setup_ui.h"

// Connection screens of the access-point setup (ARM code in overlay 13):
//  - src/wifi_setup_auto_setup.c: the rest of the connection settings screen (cursor, scrolling,
//    switch buttons, completeness check), the "settings saved" screen, the four steps of the
//    automatic setup (search, connect, receive the settings, done), the access-point scan screen;
//  - src/wifi_setup_connection_menu.c: the screen of the three connection slots, the setup method
//    menu, the connection test with its prompt and result, and the screens around the Wi-Fi USB
//    connector.
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
#define WIFI_SETUP_METHOD_MENU_CLIENT  1 // ov13_0222F2AC
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
void WifiSetupSettings_WaitIncompleteDialogClosed(void);
void WifiSetupSettings_SetFromMenu(u8 fromMenu);

void WifiSetupSaveScreen_Init(void);
void WifiSetupSaveScreen_LoadGraphics(void);
void WifiSetupSaveScreen_FadeIn(void);
void WifiSetupSaveScreen_WaitFadeIn(void);
void WifiSetupSaveScreen_Main(void);
void WifiSetupSaveScreen_HandleInput(void);
void WifiSetupSaveScreen_Update(void);
void WifiSetupSaveScreen_FadeOut(void);
void WifiSetupSaveScreen_End(void);

int WifiSetupAutoConnect_IsLidClosed(void);
void WifiSetupAutoConnect_Init(void);
void WifiSetupAutoConnect_LoadGraphics(void);
void WifiSetupAutoConnect_FadeIn(void);
void WifiSetupAutoConnect_WaitFadeIn(void);
void WifiSetupAutoConnect_WaitButtonBar(void);
void WifiSetupAutoConnect_Main(void);
void WifiSetupAutoConnect_HandleInput(void);
void WifiSetupAutoConnect_Update(void);
void WifiSetupAutoConnect_CheckButtons(void);
void WifiSetupAutoConnect_Exit(void);
void WifiSetupAutoConnect_WaitFadeOut(void);
void WifiSetupAutoConnect_End(void);
void WifiSetupAutoConnect_CheckClient(void);
void WifiSetupAutoConnect_WaitErrorDialog(void);
void WifiSetupAutoConnect_WaitErrorDialogClosed(void);

int WifiSetupAutoReceive_IsLidClosed(void);
void WifiSetupAutoReceive_Init(void);
void WifiSetupAutoReceive_LoadGraphics(void);
void WifiSetupAutoReceive_FadeIn(void);
void WifiSetupAutoReceive_WaitFadeIn(void);
void WifiSetupAutoReceive_WaitButtonBar(void);
void WifiSetupAutoReceive_Main(void);
void WifiSetupAutoReceive_HandleInput(void);
void WifiSetupAutoReceive_Update(void);
void WifiSetupAutoReceive_CheckButtons(void);
void WifiSetupAutoReceive_Exit(void);
void WifiSetupAutoReceive_WaitFadeOut(void);
void WifiSetupAutoReceive_End(void);
void WifiSetupAutoReceive_CheckClient(void);
void WifiSetupAutoReceive_WaitErrorDialog(void);
void WifiSetupAutoReceive_WaitErrorDialogClosed(void);

void WifiSetupAutoDone_Init(void);
void WifiSetupAutoDone_LoadGraphics(void);
void WifiSetupAutoDone_FadeIn(void);
void WifiSetupAutoDone_WaitFadeIn(void);
void WifiSetupAutoDone_Main(void);
void WifiSetupAutoDone_Update(void);
void WifiSetupAutoDone_FadeOut(void);
void WifiSetupAutoDone_End(void);
void WifiSetupAutoDone_UpdateTimer(void);

int WifiSetupAutoSearch_IsLidClosed(void);
void WifiSetupAutoSearch_Init(void);
void WifiSetupAutoSearch_LoadGraphics(void);
void WifiSetupAutoSearch_FadeIn(void);
void WifiSetupAutoSearch_WaitFadeIn(void);
void WifiSetupAutoSearch_WaitButtonBar(void);
void WifiSetupAutoSearch_Main(void);
void WifiSetupAutoSearch_HandleInput(void);
void WifiSetupAutoSearch_Update(void);
void WifiSetupAutoSearch_CheckButtons(void);
void WifiSetupAutoSearch_Exit(void);
void WifiSetupAutoSearch_WaitFadeOut(void);
void WifiSetupAutoSearch_End(void);
void WifiSetupAutoSearch_CheckClient(void);
void WifiSetupAutoSearch_WaitErrorDialog(void);
void WifiSetupAutoSearch_WaitErrorDialogClosed(void);

void WifiSetupApScanScreen_Init(void);
void WifiSetupApScanScreen_LoadGraphics(void);
void WifiSetupApScanScreen_FadeIn(void);
void WifiSetupApScanScreen_WaitFadeIn(void);
void WifiSetupApScanScreen_WaitButtonBar(void);
void WifiSetupApScanScreen_Main(void);
void WifiSetupApScanScreen_UpdateScan(void);
void WifiSetupApScanScreen_HandleInput(void);
void WifiSetupApScanScreen_Update(void);
void WifiSetupApScanScreen_CheckButtons(void);
void WifiSetupApScanScreen_Exit(void);
void WifiSetupApScanScreen_WaitFadeOut(void);
void WifiSetupApScanScreen_End(void);
void WifiSetupApScanScreen_WaitDialog(void);
void WifiSetupApScanScreen_WaitDialogClosed(void);

void WifiSetupConnectionList_Init(void);
void WifiSetupConnectionList_LoadGraphics(void);
void WifiSetupConnectionList_CreateIcons(void);
void WifiSetupConnectionList_FadeIn(void);
void WifiSetupConnectionList_WaitFadeIn(void);
void WifiSetupConnectionList_WaitButtonBar(void);
void WifiSetupConnectionList_Main(void);
void WifiSetupConnectionList_HandleInput(void);
void WifiSetupConnectionList_Update(void);
void WifiSetupConnectionList_CheckButtons(void);
void WifiSetupConnectionList_Exit(void);
void WifiSetupConnectionList_WaitFadeOut(void);
void WifiSetupConnectionList_End(void);
void WifiSetupConnectionList_ShowCursor(void);
void WifiSetupConnectionList_MoveCursor(int dir);
void WifiSetupConnectionList_PressEraseButton(void);
void WifiSetupConnectionList_WaitEraseDialog(void);
void WifiSetupConnectionList_WaitEraseDialogClosed(void);
void WifiSetupConnectionList_HighlightSelection(void);

void WifiSetupMethodMenu_Init(void);
void WifiSetupMethodMenu_LoadGraphics(void);
void WifiSetupMethodMenu_FadeIn(void);
void WifiSetupMethodMenu_WaitFadeIn(void);
void WifiSetupMethodMenu_WaitButtonBar(void);
void WifiSetupMethodMenu_Main(void);
void WifiSetupMethodMenu_HandleInput(void);
void WifiSetupMethodMenu_Update(void);
void WifiSetupMethodMenu_CheckButtons(void);
void WifiSetupMethodMenu_Exit(void);
void WifiSetupMethodMenu_WaitFadeOut(void);
void WifiSetupMethodMenu_End(void);
void WifiSetupMethodMenu_MoveCursor(int dir);
void WifiSetupMethodMenu_HighlightSelection(void);

void WifiSetupConnectionTest_Init(void);
void WifiSetupConnectionTest_LoadGraphics(void);
void WifiSetupConnectionTest_Start(void);
void WifiSetupConnectionTest_FadeIn(void);
void WifiSetupConnectionTest_WaitFadeIn(void);
void WifiSetupConnectionTest_Main(void);
void WifiSetupConnectionTest_HandleInput(void);
void WifiSetupConnectionTest_Update(void);
void WifiSetupConnectionTest_CheckButtons(void);
void WifiSetupConnectionTest_FadeOut(void);
void WifiSetupConnectionTest_WaitFadeOut(void);
void WifiSetupConnectionTest_End(void);
void WifiSetupConnectionTest_PollTask(void *task);
void *WifiSetupConnectionTest_Alloc(void *arg, u32 size);
void WifiSetupConnectionTest_Free(void *arg, void *ptr);

void WifiSetupConnectionOk_Init(void);
void WifiSetupConnectionOk_LoadGraphics(void);
void WifiSetupConnectionOk_FadeIn(void);
void WifiSetupConnectionOk_WaitFadeIn(void);
void WifiSetupConnectionOk_Main(void);
void WifiSetupConnectionOk_Update(void);
void WifiSetupConnectionOk_FadeOut(void);
void WifiSetupConnectionOk_End(void);
void WifiSetupConnectionOk_UpdateTimer(void);

void WifiSetupTestPrompt_Init(void);
void WifiSetupTestPrompt_LoadGraphics(void);
void WifiSetupTestPrompt_FadeIn(void);
void WifiSetupTestPrompt_WaitFadeIn(void);
void WifiSetupTestPrompt_Main(void);
void WifiSetupTestPrompt_HandleInput(void);
void WifiSetupTestPrompt_Update(void);
void WifiSetupTestPrompt_WaitDialogClosed(void);
void WifiSetupTestPrompt_End(void);

void WifiSetupUsbConnectorDone_Init(void);
void WifiSetupUsbConnectorDone_LoadGraphics(void);
void WifiSetupUsbConnectorDone_FadeIn(void);
void WifiSetupUsbConnectorDone_WaitFadeIn(void);
void WifiSetupUsbConnectorDone_Main(void);
void WifiSetupUsbConnectorDone_Update(void);
void WifiSetupUsbConnectorDone_FadeOut(void);
void WifiSetupUsbConnectorDone_End(void);
void WifiSetupUsbConnectorDone_UpdateTimer(void);

void WifiSetupUsbConnectorIntro_Init(void);
void WifiSetupUsbConnectorIntro_LoadGraphics(void);
void WifiSetupUsbConnectorIntro_FadeIn(void);
void WifiSetupUsbConnectorIntro_WaitFadeIn(void);
void WifiSetupUsbConnectorIntro_WaitButtonBar(void);
void WifiSetupUsbConnectorIntro_Main(void);
void WifiSetupUsbConnectorIntro_HandleInput(void);
void WifiSetupUsbConnectorIntro_Update(void);
void WifiSetupUsbConnectorIntro_CheckButtons(void);
void WifiSetupUsbConnectorIntro_Exit(void);
void WifiSetupUsbConnectorIntro_WaitFadeOut(void);
void WifiSetupUsbConnectorIntro_End(void);

// src/wifi_setup_usb_connector_screen.c
void WifiSetupUsbConnector_Start(void);
void WifiSetupUsbConnector_ScanCallback(int result);
BOOL WifiSetupUsbConnector_IsLidClosed(void);
void WifiSetupUsbConnector_Init(void);
void WifiSetupUsbConnector_LoadGraphics(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_CONNECTION_SCREENS_H
