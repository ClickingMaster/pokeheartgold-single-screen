#ifndef POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_SCREENS_H

#include "global.h"

#include "wifi_setup/wifi_setup_ui.h"

// Access-point setup screens: the list screen and the text entry screen shared with the screens that
// follow them.

// Copies of path strings (the path is localised in place).
typedef struct WifiSetupPath22 {
    char text[22];
} WifiSetupPath22;

typedef struct WifiSetupPath23 {
    char text[23];
} WifiSetupPath23;

typedef struct WifiSetupPath25 {
    char text[25];
} WifiSetupPath25;

// One row of the list screen.
typedef struct WifiSetupListRow {
    u8 text[40];
    u8 kind;
    u8 unk_29;
} WifiSetupListRow;

// Work of the list screen (allocated by the screen, gWifiSetupListScreen.work).
typedef struct WifiSetupListWork {
    union {
        WifiSetupListRow *rows;
        u8 *rowData;
    };

    void *bgScreenFile;
    void *paletteFile;
    void *buffer;
    GXOamAttr *rowOams[5];
    GXOamAttr *iconOams[5];
    void *task;
    void *scrollBgTask;
    u16 total;           // scroll range
    u16 rowKindChars[3]; // character numbers per row kind
    u16 iconChars[4];
    s8 selected;
    u8 count;
    u8 index;
    u8 scrollBarRange;
    u8 confirmed;
    u8 delay;
    u8 bgScrollDirty;
    u8 draggingScrollBar;
    u8 scrollLimitSoundPlayed;
    u8 cursorLimitSoundPlayed;
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
