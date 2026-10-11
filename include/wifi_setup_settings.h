#ifndef POKEHEARTGOLD_WIFI_SETUP_SETTINGS_H
#define POKEHEARTGOLD_WIFI_SETUP_SETTINGS_H

#include "global.h"

#include "wifi_setup_ui.h"

// Connection settings screen of the access-point setup (ARM code in overlay 13): a scrolling list
// of 12 items (SSID, key, the "obtain address automatically" and "obtain DNS automatically"
// switches, IP address, subnet mask, gateway, DNS servers, ...) with a scroll bar on the top
// screen. Items are 29 pixels apart; five are visible at a time.

#define WIFI_SETUP_SETTINGS_NUM_ITEMS  12
#define WIFI_SETUP_SETTINGS_ROW_HEIGHT 29
#define WIFI_SETUP_SETTINGS_MAX_SCROLL 145 // 5 rows
#define WIFI_SETUP_SETTINGS_NUM_ROWS   5   // rows drawn at a time

// item codes returned by WifiSetupSettings_GetTouchedItem besides the list items
#define WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON  2
#define WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF 3
#define WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON      7
#define WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF     8
#define WIFI_SETUP_SETTINGS_ITEM_BACK             13
#define WIFI_SETUP_SETTINGS_ITEM_NONE             14

typedef struct WifiSetupSettingsWork {
    void *task;                  // 0x00, queue 0
    volatile u8 buttonStates[4]; // 0x04
    u16 *screenData;             // 0x08, top screen BG tiles of all items
    void *paletteData;           // 0x0C
    void *charData;              // 0x10
    WifiSetupCanvas *canvas;     // 0x14, text of the visible rows
    GXOamAttr *rows[5];          // 0x18, one object per visible row
    GXOamAttr *addressSwitch;    // 0x2C, buttons of the "obtain address automatically" item
    GXOamAttr *dnsSwitch;        // 0x30, buttons of the "obtain DNS automatically" item
    GXOamAttr *ssidIcon;         // 0x34, next to the first item when record[0xE7] is 1 or 2, else NULL
    void *scrollTask;            // 0x38, row scroll in progress
    void *task2;                 // 0x3C, queue 1
    u8 selection;                // 0x40, item chosen to leave the screen with
    u8 scrollBarTimer;           // 0x41
    u8 numItems;                 // 0x42
    u8 autoDns;                  // 0x43, DNS switch kept while the address is set by hand
    u8 objectsDirty;             // 0x44
    u8 scrollBarHeld;            // 0x45
    u8 scrollBarAtEnd;           // 0x46
    u8 keyAtEnd;                 // 0x47
} WifiSetupSettingsWork;

typedef struct WifiSetupSettings {
    u8 cursorItem; // 0x00
    u8 fromMenu;   // 0x01
    u8 unk_02;
    u8 unk_03;
    u16 scroll; // 0x04, 0 - WIFI_SETUP_SETTINGS_MAX_SCROLL
    u16 unk_06;
    WifiSetupSettingsWork *work; // 0x08
} WifiSetupSettings;

extern WifiSetupSettings gWifiSetupSettings;

void WifiSetupSettings_Init(void);
void WifiSetupSettings_LoadGraphics(void);
void WifiSetupSettings_ResetPosition(void);
void WifiSetupSettings_FadeIn(void);
void WifiSetupSettings_WaitFadeIn(void);
void WifiSetupSettings_Main(void);
void WifiSetupSettings_HandleKeys(void);
int WifiSetupSettings_HandleTouch(void);
void WifiSetupSettings_SelectItem(int item);
void WifiSetupSettings_SetAutoAddress(u32 on);
void WifiSetupSettings_SetAutoDns(int on);
void WifiSetupSettings_HandleScrollBar(void);
void WifiSetupSettings_StartClose(void);
void WifiSetupSettings_FadeOut(void);
void WifiSetupSettings_Exit(void);
void WifiSetupSettings_DrawRows(void);
void WifiSetupSettings_DrawValue(u32 item, void *row);
void WifiSetupSettings_DrawButtons(u32 item, void *row);
int WifiSetupSettings_IsItemEnabled(int item);
void WifiSetupSettings_SetButtonOam(unsigned type, int option, int row);
void WifiSetupSettings_DrawText(u8 *text, int row);
void WifiSetupSettings_DrawAddress(u8 *address, int row);
void WifiSetupSettings_UpdateObjects(void);
int WifiSetupSettings_GetTouchedItem(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SETTINGS_H
