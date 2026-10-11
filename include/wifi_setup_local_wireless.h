#ifndef POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H
#define POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H

#include "global.h"

#include "wifi_setup_ui.h"

// Local-wireless part of the access-point setup (ARM code in overlay 13): the sound player
// controls, the title banner (five OAM letters sliding in over a BG screen), the dialog window
// with up to two buttons, the scan for a Wi-Fi USB connector (access points whose SSID starts
// with "NWCUSBAP") and the DS Download Play parent.

#define WIFI_SETUP_BANNER_NUM_LETTERS   5
#define WIFI_SETUP_USB_SCAN_NUM_ENTRIES 16

// Title banner, allocated by WifiSetupBanner_Open (0x1C bytes).
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

// Parameters of the scan (copied from a template); the SSID carries the tag, a flag byte and the
// owner's nickname.
typedef struct WifiSetupUsbScanParam {
    void *buf;
    u8 unk_04[4];
    u16 period;
    u8 unk_0A[10];
    u8 ssidTag[8]; // 0x14
    u8 unk_1C;
    u8 ssidFlag; // 0x1D
    u8 unk_1E[2];
    u8 nickname[0x24]; // 0x20
} WifiSetupUsbScanParam;

typedef struct WifiSetupUsbScanStatus {
    u16 state;
    u8 unk_002[0x7CE];
} WifiSetupUsbScanStatus;

// Scan work (0x1BA0 bytes, 32-byte aligned; the WM work buffer comes first).
typedef struct WifiSetupUsbScan {
    u8 wmBuf[0xF00];
    u8 scanBuf[0x400];                                              // 0xF00
    WifiSetupUsbScanEntry entries[WIFI_SETUP_USB_SCAN_NUM_ENTRIES]; // 0x1300
    void (*callback)(int result);                                   // 0x1370
    WifiSetupUsbScanParam param;                                    // 0x1374
    WifiSetupUsbScanStatus status;                                  // 0x13B8
    OSTick startTick;                                               // 0x1B88
    void *task;                                                     // 0x1B90
    vu8 endState;                                                   // 0x1B94, 1 ending, 2 ended
    u8 found;                                                       // 0x1B95
    u8 rescanning;                                                  // 0x1B96, set once the first scan period is over
    u8 unk_1B97[9];
} WifiSetupUsbScan;

// Access point as reported by a scan callback.
typedef struct WifiSetupUsbScanBss {
    u8 unk_00[4];
    u8 bssid[6];
    u8 unk_0A[2];
    u8 ssidTag[8]; // 0x0C
    u8 unk_14;
    u8 ssidFlag; // 0x15
} WifiSetupUsbScanBss;

typedef struct WifiSetupUsbScanCallbackArg {
    u16 apiid;
    u16 errcode;
    u8 unk_04[4];
    u16 state;
    u8 unk_0A[4];
    u16 count;
    WifiSetupUsbScanBss *bss[1];
} WifiSetupUsbScanCallbackArg;

// Dialog window (0x20 bytes): a frame, a text line and up to two buttons sliding up from the
// bottom of the screen.
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
    u8 mbWork[0x1B140 - 0x10040]; // 0x10040
    void *mbWorkPtr;              // 0x1B140
    void *segmentPtr;             // 0x1B144
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
BOOL WifiSetupMbParent_RegisterFile(WifiSetupMbParentFile *file);
void WifiSetupMbParent_AcceptChild(u16 aid);
void WifiSetupMbParent_KickChild(u16 aid);
void WifiSetupMbParent_StartSending(u16 aid);
void WifiSetupMbParent_StartDownload(void);
BOOL WifiSetupMbParent_AreAllBootable(void);
void WifiSetupMbParent_BootChildren(void);
void WifiSetupMbParent_End(void);
void WifiSetupMbParent_EndIfAllBooted(void);
void WifiSetupMbParent_StateCallback(u16 aid, u32 status, void *arg);
void WifiSetupMbParent_SetState(int state);
u16 WifiSetupMbParent_GetState(void);
u16 WifiSetupMbParent_GetMask(int index);

#endif // POKEHEARTGOLD_WIFI_SETUP_LOCAL_WIRELESS_H
