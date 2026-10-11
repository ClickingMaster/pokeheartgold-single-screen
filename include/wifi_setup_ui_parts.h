#ifndef POKEHEARTGOLD_WIFI_SETUP_UI_PARTS_H
#define POKEHEARTGOLD_WIFI_SETUP_UI_PARTS_H

#include "wifi_setup_keyboard.h"
#include "wifi_setup_ui.h"

// Screen parts shared by the access-point setup screens (ARM code in overlay 13): busy icon, sliding
// button bar, message window, cursors, BG1 frame, signal-strength icon and the software keyboard.

// Animated busy icon in the bottom-right corner of the touch screen.
typedef struct WifiSetupBusyIcon {
    void *task;
    GXOamAttr *oam;
    u8 style; // index into the palette table
    u8 frameCounter;
} WifiSetupBusyIcon;

// Bar with up to two touch buttons that slides in from the bottom of the touch screen.
typedef struct WifiSetupButtonBar {
    WifiSetupOamBlock *buttons[2];
    WifiSetupOamBlock *bar;
    void *task;
    void *pressTask;
    u16 timer;
    s8 selected; // -1: none, -2: not yet shown
    u8 type;
    u8 locked;
    u8 closing;
} WifiSetupButtonBar;

// Wireless signal strength icon in the top-right corner of the top screen.
typedef struct WifiSetupSignalIcon {
    GXOamAttr *oam;
    void *task;
    s8 type;
} WifiSetupSignalIcon;

extern WifiSetupBusyIcon *gWifiSetupBusyIcon;
extern WifiSetupSignalIcon *gWifiSetupSignalIcon;
extern const u8 ov13_02244EB0[];

void WifiSetupBusyIcon_Create(u8 style);
void WifiSetupBusyIcon_Destroy(void);
void WifiSetupBusyIcon_Task(void *task);
void WifiSetupButtonBar_Create(int type);
void WifiSetupButtonBar_Close(void);
int WifiSetupButtonBar_GetSelected(void);
void WifiSetupButtonBar_SelectIfNone(int button);
void WifiSetupButtonBar_SetSelected(int button);
int WifiSetupButtonBar_IsNotClosing(void);
void WifiSetupButtonBar_Unlock(void);
void WifiSetupButtonBar_Lock(void);
void WifiSetupButtonBar_SetY(int y);
void WifiSetupButtonBar_SlideInTask(void *task);
void WifiSetupButtonBar_WaitTask(void *task);
void WifiSetupButtonBar_InputTask(void);
void WifiSetupButtonBar_SlideOutTask(void *task);
void WifiSetupButtonBar_DestroyTask(void *task);
void WifiSetupButtonBar_PressTask(void *task);
void WifiSetupMessage_Reset(void);
int WifiSetupMessage_Show(int a, int b, int c);
int WifiSetupMessage_Hide(void);
char *WifiSetup_LocalizePath(char *path);
void WifiSetup_LoadGraphics(char *path, void (*load)(const void *, u32, u32));
void WifiSetupBanner_Show(int a);
void WifiSetupButtonBar_Show(int type);
void WifiSetup_PrintMessage(int message);
void WifiSetup_PrintMessageWithText(const u16 *str, int message);
u32 WifiSetup_GetTextFlags(void);
void WifiSetupCursor_Init(void);
void WifiSetupCursor_Free(void);
void WifiSetupCursor_ShowPair(int kind, int x1, int x2, int y);
void WifiSetupCursor_ShowCorners(int left, int right, int top, int bottom);
void WifiSetupCursor_Hide();
void WifiSetupBackground_Init(void);
void WifiSetupBackground_Free(void);
void WifiSetupBackground_LoadHeader(int n);
void WifiSetupBackground_LoadSlotHeader(void);
void ov13_02227E58(void);
void WifiSetupBackground_TransferTask(void *task);
void WifiSetupSignalIcon_Create(int type);
void WifiSetupSignalIcon_Destroy(void);
void WifiSetupSignalIcon_Task(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_UI_PARTS_H
