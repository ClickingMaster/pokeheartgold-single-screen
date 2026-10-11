#ifndef POKEHEARTGOLD_WIFI_SETUP_SCROLL_BAR_H
#define POKEHEARTGOLD_WIFI_SETUP_SCROLL_BAR_H

#include "global.h"

#include "wifi_setup_ui.h"

// Screen helpers of the access-point setup (ARM code in overlay 13): a sub-screen copy of the BG 2
// screen flushed by a task when dirty, palette uploads, the vertical scroll bar (thumb plus two
// arrows) and the access-point scan work allocation.

// scroll bar parts (WifiSetupScrollBar_GetPartRect)
#define WIFI_SETUP_SCROLL_BAR_THUMB 1
#define WIFI_SETUP_SCROLL_BAR_UP    2
#define WIFI_SETUP_SCROLL_BAR_DOWN  3
#define WIFI_SETUP_SCROLL_BAR_TRACK 4

// WifiSetupScrollBar.state
#define WIFI_SETUP_SCROLL_BAR_STATE_IDLE     0
#define WIFI_SETUP_SCROLL_BAR_STATE_DRAGGING 1
#define WIFI_SETUP_SCROLL_BAR_STATE_UP       2
#define WIFI_SETUP_SCROLL_BAR_STATE_DOWN     3

// BG 2 screen copy (0x608 bytes).
typedef struct WifiSetupBgScreen {
    void *task;
    u16 data[0x300];
    u8 dirty;
} WifiSetupBgScreen;

typedef struct WifiSetupBgScreenState {
    WifiSetupBgScreen *screen;
    void *paletteSrc;
    void *paletteDst;
} WifiSetupBgScreenState;

// Vertical scroll bar (0x20 bytes).
typedef struct WifiSetupScrollBar {
    WifiSetupOamBlock *thumb;
    int unk_04;
    int unk_08;
    void *task;
    WifiSetupPoint pos;        // top of the track
    WifiSetupPoint touchStart; // touch position when the drag started
    u8 dragStart;              // value when the drag started
    u8 length;                 // track length
    u8 value;                  // thumb offset from the top of the track
    u8 kind;
    u8 state; // WIFI_SETUP_SCROLL_BAR_STATE_*
    u8 event; // this frame: 1 grabbed, 2 dragged, 3 released/jumped, 4/6 arrow pressed, 5/7 arrow released
    u8 disabled;
} WifiSetupScrollBar;

extern WifiSetupBgScreenState gWifiSetupBgScreen;
extern WifiSetupScrollBar *gWifiSetupScrollBar;

void WifiSetupBgScreen_Create(void *src);
void WifiSetupBgScreen_Destroy(void);
void WifiSetupBgScreen_MarkDirty(void);
void WifiSetupBgScreen_CopyRect(u16 *src, int offset, int width, int rows);
void WifiSetupBgScreen_UpdateTask(void *task, void *arg);
void WifiSetupPalette_LoadRow(u8 *src, int index, int palette);
void WifiSetupPalette_Load(void *src);

void WifiSetupScrollBar_Create(int kind, int length, int x, int y, int value);
void WifiSetupScrollBar_Destroy(void);
u8 WifiSetupScrollBar_GetValue(void);
u8 WifiSetupScrollBar_GetEvent(void);
void WifiSetupScrollBar_SetValue(int value);
void WifiSetupScrollBar_Enable(void);
void WifiSetupScrollBar_Disable(void);

void WifiSetupApScan_AllocWork(void);
void WifiSetupApScan_FreeWork(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SCROLL_BAR_H
