#ifndef POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_SCREENS_H

#include "global.h"

#include "wifi_setup/wifi_setup_ui.h"

// Access-point setup screens: the list screen and the text entry screen shared with the screens that
// follow them.

// One row of the list screen (0x2A bytes).
typedef struct WifiSetupListRow {
    u8 text[40];
    u8 kind;
    u8 unk_29;
} WifiSetupListRow;

// Work of the list screen (allocated by the screen, gWifiSetupListScreen.work).
typedef struct WifiSetupListWork {
    union {
        WifiSetupListRow *rows; // 0x00
        u8 *rowData;
    };

    void *bgScreenFile;           // 0x04
    void *paletteFile;           // 0x08
    void *buffer;           // 0x0C
    GXOamAttr *rowOams[5];  // 0x10
    GXOamAttr *iconOams[5]; // 0x24
    void *task;             // 0x38
    void *scrollBgTask;           // 0x3C
    u16 total;              // 0x40, scroll range
    u16 rowKindChars[3];          // 0x42, character numbers per row kind
    u16 iconChars[4];          // 0x48
    s8 selected;            // 0x50
    u8 count;               // 0x51, number of rows
    u8 index;               // 0x52
    u8 scrollBarRange;              // 0x53
    u8 confirmed;              // 0x54
    u8 delay;               // 0x55
    u8 bgScrollDirty;              // 0x56
    u8 draggingScrollBar;              // 0x57
    u8 scrollLimitSoundPlayed;              // 0x58
    u8 cursorLimitSoundPlayed;              // 0x59
} WifiSetupListWork;

typedef struct WifiSetupListScreen {
    u8 selected;
    u8 unk_01;
    u16 position; // scroll position
    WifiSetupListWork *work;
} WifiSetupListScreen;

// Text entry screen: a 32-character text and its length.
typedef struct WifiSetupTextEntry {
    WifiSetupCanvas *canvas;
    GXOamAttr *cursor;
    u8 text[33];
    u8 length;
    u8 flag;
} WifiSetupTextEntry;

extern WifiSetupListScreen gWifiSetupListScreen;
extern WifiSetupTextEntry *gWifiSetupTextEntry;

void WifiSetupClient_Start(void);
void WifiSetupClient_Stop(void);
int WifiSetupClient_GetResult(void);
void WifiSetupClient_StoreResult(void);
void WifiSetupScreens_StartQuit(void);
void WifiSetupScreens_Init(void);
void WifiSetupTopMenu_Init(void);
void WifiSetupAoss_Init(void);
void WifiSetupListScreen_Init(void);
void WifiSetupTextEntry_Init(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
