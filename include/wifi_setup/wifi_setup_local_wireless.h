#ifndef POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H
#define POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H

#include <nitro/wm/wm_internal.h>

#include "global.h"

#include "wifi_setup/wifi_setup_ui.h"

// Local-wireless part of the access-point setup: the title banner, the dialog window, the scan for a Wi-Fi USB
// connector (access points whose SSID starts with "NWCUSBAP") and the DS Download Play parent.

#define WIFI_SETUP_BANNER_NUM_LETTERS   5
#define WIFI_SETUP_USB_SCAN_NUM_ENTRIES 16

// Title banner, allocated by WifiSetupBanner_Open.
typedef struct WifiSetupBanner {
    WifiSetupOamBlock *letters[WIFI_SETUP_BANNER_NUM_LETTERS];
    void *task; // slide task, NULL when the banner is still
    u8 closing; // set by WifiSetupBanner_Close
} WifiSetupBanner;

typedef struct WifiSetupBannerState {
    void *screen; // BG screen data being loaded
    WifiSetupBanner *banner;
} WifiSetupBannerState;

// One USB connector seen by the first scan.
typedef struct WifiSetupUsbScanEntry {
    u8 bssid[6];
    u8 flag; // bit 0 of the byte after the SSID tag
} WifiSetupUsbScanEntry;

// Scan work (32-byte aligned; the WM work buffer comes first).
typedef struct WifiSetupUsbScan {
    u8 wmBuf[0xF00];
    u8 scanBuf[0x400];
    WifiSetupUsbScanEntry entries[WIFI_SETUP_USB_SCAN_NUM_ENTRIES];
    void (*callback)(int result);
    WmScanExParam param;
    WmStatus status;
    OSTick startTick;
    void *task;
    vu8 endState; // 1 ending, 2 ended
    u8 found;
    u8 rescanning; // set once the first scan period is over
    u8 unk_1B97[9];
} WifiSetupUsbScan;

// Dialog window: a frame, a text line and up to two buttons sliding up from the bottom of the screen.
typedef struct WifiSetupDialog {
    WifiSetupOamBlock *frame;
    WifiSetupOamBlock *textBlock;
    WifiSetupOamBlock *buttons[2];
    WifiSetupCanvas *canvas;
    void *task;
    u16 timer;     // kind 5: frames shown
    s8 brightness; // fade
    s8 selected;   // button index, -1 none, -2 not yet shown
    u8 kind;
    u8 closeDelay;
    u8 fade; // fade the screen in and out
} WifiSetupDialog;

// State of the DS Download Play parent: bit masks of child AIDs per stage.
typedef struct WifiSetupMbParentState {
    u16 state;
    u16 connectedMask;
    u16 requestedMask;
    u16 waitingMask;
    u16 sendingMask;
    u16 sentMask;
    u16 bootableMask;
} WifiSetupMbParentState;

typedef struct WifiSetupMbParentChild {
    u8 user[0x16];
    u8 macAddr[6];
    u16 aid;
} WifiSetupMbParentChild;

typedef struct WifiSetupMbParentUser {
    u8 favoriteColor : 4;
    u8 playerNo : 4;
    u8 nameLength;
    u16 name[10];
} WifiSetupMbParentUser;

typedef struct WifiSetupMbParentFile {
    const char *path;
} WifiSetupMbParentFile;

typedef struct WifiSetupMbParentConnectArg {
    u8 unk_00[0xA];
    u8 macAddr[6];
} WifiSetupMbParentConnectArg;

// Download Play parent work, inside the buffer given to WifiSetupMbParent_Init.
typedef struct WifiSetupMbParent {
    WifiSetupMbParentState head;
    WifiSetupMbParentChild children[1]; // indexed by aid - 1
    u8 segment[0x10040 - 0x2C];
    u8 mbWork[0x1B140 - 0x10040];
    void *mbWorkPtr;
    void *segmentPtr;
    u8 padding_1B148[0x18];
    u8 whWork[0x1D780 - 0x1B160]; // WifiSetupWhWork
} WifiSetupMbParent;

extern WifiSetupBannerState gWifiSetupBanner;
extern WifiSetupUsbScan *gWifiSetupUsbScan;
extern WifiSetupDialog *gWifiSetupDialog;
extern WifiSetupMbParent *gWifiSetupMbParent;

void WifiSetupSound_Play(int seq);
void WifiSetupSound_SetVolume(int volume);
void WifiSetupSound_SetTrackPitch(int trackMask, int pitch);
void WifiSetupSound_Stop(void);
void WifiSetupSound_UpdateTask(void *task, void *arg);

void WifiSetupBanner_Open(int kind);
void WifiSetupBanner_Close(void);
void WifiSetupBanner_SetKind(int kind);
BOOL WifiSetupBanner_IsMoving(void);
BOOL WifiSetupBanner_IsNotClosing(void);
void WifiSetupBanner_LoadScreen(int kind);

BOOL WifiSetupUsbScan_Start(void (*callback)(int));
BOOL WifiSetupUsbScan_End(void);
void WifiSetupUsbScan_SetCallback(void (*callback)(int));

void WifiSetupDialog_Open(int message, int kind, u8 fade, int digitPos, int digit);
void WifiSetupDialog_Close(void);
int WifiSetupDialog_GetSelected(void);
BOOL WifiSetupDialog_IsOpen(void);

void WifiSetupMbParent_Init(void *buf);
void WifiSetupMbParent_Setup(u32 ggid, u32 tgid);
void WifiSetupMbParent_Start(WifiSetupMbParentFile *file, u32 channel);
void WifiSetupMbParent_StartDownload(void);
BOOL WifiSetupMbParent_AreAllBootable(void);
void WifiSetupMbParent_BootChildren(void);
void WifiSetupMbParent_End(void);
u16 WifiSetupMbParent_GetState(void);
u16 WifiSetupMbParent_GetMask(int index);

#endif // POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H
