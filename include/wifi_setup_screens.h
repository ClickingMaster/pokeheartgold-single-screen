#ifndef POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_SCREENS_H

#include "global.h"

#include "wifi_setup_ui.h"

// Access-point setup screens (src/wifi_setup_screens.c): the list screen and the text entry screen
// shared with the screens that follow them.

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

    void *unk_04;           // 0x04
    void *unk_08;           // 0x08
    void *buffer;           // 0x0C
    GXOamAttr *rowOams[5];  // 0x10
    GXOamAttr *iconOams[5]; // 0x24
    void *task;             // 0x38
    void *unk_3C;           // 0x3C
    u16 total;              // 0x40, scroll range
    u16 unk_42[3];          // 0x42, character numbers per row kind
    u16 unk_48[4];          // 0x48
    s8 selected;            // 0x50
    u8 count;               // 0x51, number of rows
    u8 index;               // 0x52
    u8 unk_53;              // 0x53
    u8 unk_54;              // 0x54
    u8 delay;               // 0x55
    u8 unk_56;              // 0x56
    u8 unk_57;              // 0x57
    u8 unk_58;              // 0x58
    u8 unk_59;              // 0x59
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
void WifiSetupClient_OnStatus(s32 *status);
void *WifiSetupClient_Alloc(u32 size);
void WifiSetupClient_Free(void *p);
void WifiSetupScreens_StartQuit(void);
void WifiSetupScreens_Quit(void);
void WifiSetupScreens_Init(void);
void WifiSetupScreens_FadeIn(void);
void WifiSetupScreens_WaitFadeIn(void);
void WifiSetupTopMenu_Init(void);
void WifiSetupTopMenu_LoadGraphics(void);
void WifiSetupTopMenu_FadeIn(void);
void WifiSetupTopMenu_WaitFadeIn(void);
void WifiSetupTopMenu_WaitButtonBar(void);
void WifiSetupTopMenu_Main(void);
void WifiSetupTopMenu_HandleInput(void);
void WifiSetupTopMenu_Update(void);
void WifiSetupTopMenu_CheckButtons(void);
void WifiSetupTopMenu_Exit(void);
void WifiSetupTopMenu_WaitFadeOut(void);
void WifiSetupTopMenu_End(void);
void WifiSetupTopMenu_MoveCursor(int dir);
void WifiSetupTopMenu_HighlightSelection(void);
void WifiSetupEraseConfirm_Init(void);
void WifiSetupEraseConfirm_LoadGraphics(void);
void WifiSetupEraseConfirm_FadeIn(void);
void WifiSetupEraseConfirm_WaitFadeIn(void);
void WifiSetupEraseConfirm_WaitButtonBar(void);
void WifiSetupEraseConfirm_Main(void);
void WifiSetupEraseConfirm_HandleInput(void);
void WifiSetupEraseConfirm_Update(void);
void WifiSetupEraseConfirm_CheckButtons(void);
void WifiSetupEraseConfirm_Exit(void);
void WifiSetupEraseConfirm_WaitFadeOut(void);
void WifiSetupEraseConfirm_End(void);
void WifiSetupEraseConfirm_WaitDialog(void);
void WifiSetupEraseConfirm_WaitDialogClosed(void);
void WifiSetupEraseDone_Init(void);
void WifiSetupEraseDone_LoadGraphics(void);
void WifiSetupEraseDone_FadeIn(void);
void WifiSetupEraseDone_WaitFadeIn(void);
void WifiSetupEraseDone_WaitButtonBar(void);
void WifiSetupEraseDone_Main(void);
void WifiSetupEraseDone_HandleInput(void);
void WifiSetupEraseDone_Update(void);
void WifiSetupEraseDone_CheckButtons(void);
void WifiSetupEraseDone_Exit(void);
void WifiSetupEraseDone_WaitFadeOut(void);
void WifiSetupEraseDone_PowerOff(void);
void WifiSetupSystemInfo_Init(void);
void WifiSetupSystemInfo_LoadGraphics(void);
void WifiSetupSystemInfo_DrawInfo(void);
void WifiSetupSystemInfo_FadeIn(void);
void WifiSetupSystemInfo_WaitFadeIn(void);
void WifiSetupSystemInfo_WaitButtonBar(void);
void WifiSetupSystemInfo_Main(void);
void WifiSetupSystemInfo_HandleInput(void);
void WifiSetupSystemInfo_Update(void);
void WifiSetupSystemInfo_CheckButtons(void);
void WifiSetupSystemInfo_Exit(void);
void WifiSetupSystemInfo_WaitFadeOut(void);
void WifiSetupSystemInfo_End(void);
void WifiSetupOptions_Init(void);
void WifiSetupOptions_LoadGraphics(void);
void WifiSetupOptions_FadeIn(void);
void WifiSetupOptions_WaitFadeIn(void);
void WifiSetupOptions_WaitButtonBar(void);
void WifiSetupOptions_Main(void);
void WifiSetupOptions_HandleInput(void);
void WifiSetupOptions_Update(void);
void WifiSetupOptions_CheckButtons(void);
void WifiSetupOptions_Exit(void);
void WifiSetupOptions_WaitFadeOut(void);
void WifiSetupOptions_End(void);
void WifiSetupOptions_MoveCursor(int dir);
void WifiSetupOptions_HighlightSelection(void);
void WifiSetupTransferring_Init(void);
void WifiSetupTransferring_LoadGraphics(void);
void WifiSetupTransferring_FadeIn(void);
void WifiSetupTransferring_WaitFadeIn(void);
void WifiSetupTransferring_Main(void);
void WifiSetupTransferring_HandleInput(void);
void WifiSetupTransferring_Update(void);
void WifiSetupTransferring_FadeOut(void);
void WifiSetupTransferring_End(void);
void WifiSetupTransferring_OnTransferEvent(int a);
void WifiSetupTransferDone_Init(void);
void WifiSetupTransferDone_LoadGraphics(void);
void WifiSetupTransferDone_FadeIn(void);
void WifiSetupTransferDone_WaitFadeIn(void);
void WifiSetupTransferDone_WaitButtonBar(void);
void WifiSetupTransferDone_Main(void);
void WifiSetupTransferDone_HandleInput(void);
void WifiSetupTransferDone_Update(void);
void WifiSetupTransferDone_CheckButtons(void);
void WifiSetupTransferDone_Exit(void);
void WifiSetupTransferDone_WaitFadeOut(void);
void WifiSetupTransferDone_PowerOff(void);
void WifiSetupTransferInfo_Init(void);
void WifiSetupTransferInfo_LoadGraphics(void);
void WifiSetupTransferInfo_FadeIn(void);
void WifiSetupTransferInfo_WaitFadeIn(void);
void WifiSetupTransferInfo_WaitButtonBar(void);
void WifiSetupTransferInfo_Main(void);
void WifiSetupTransferInfo_HandleInput(void);
void WifiSetupTransferInfo_Update(void);
void WifiSetupTransferInfo_CheckButtons(void);
void WifiSetupTransferInfo_Exit(void);
void WifiSetupTransferInfo_WaitFadeOut(void);
void WifiSetupTransferInfo_End(void);
void WifiSetupTransferIntro_Init(void);
void WifiSetupTransferIntro_LoadGraphics(void);
void WifiSetupTransferIntro_FadeIn(void);
void WifiSetupTransferIntro_WaitFadeIn(void);
void WifiSetupTransferIntro_WaitButtonBar(void);
void WifiSetupTransferIntro_Main(void);
void WifiSetupTransferIntro_HandleInput(void);
void WifiSetupTransferIntro_Update(void);
void WifiSetupTransferIntro_CheckButtons(void);
void WifiSetupTransferIntro_Exit(void);
void WifiSetupTransferIntro_WaitFadeOut(void);
void WifiSetupTransferIntro_End(void);
void WifiSetupTransferCancelled_Init(void);
void WifiSetupTransferCancelled_LoadGraphics(void);
void WifiSetupTransferCancelled_FadeIn(void);
void WifiSetupTransferCancelled_WaitFadeIn(void);
void WifiSetupTransferCancelled_Main(void);
void WifiSetupTransferCancelled_Update(void);
void WifiSetupTransferCancelled_FadeOut(void);
void WifiSetupTransferCancelled_End(void);
void WifiSetupTransferCancelled_UpdateTimer(void);
void WifiSetupTransferFailed_Init(void);
void WifiSetupTransferFailed_LoadGraphics(void);
void WifiSetupTransferFailed_FadeIn(void);
void WifiSetupTransferFailed_WaitFadeIn(void);
void WifiSetupTransferFailed_WaitButtonBar(void);
void WifiSetupTransferFailed_Main(void);
void WifiSetupTransferFailed_HandleInput(void);
void WifiSetupTransferFailed_Update(void);
void WifiSetupTransferFailed_CheckButtons(void);
void WifiSetupTransferFailed_Exit(void);
void WifiSetupTransferFailed_WaitFadeOut(void);
void WifiSetupTransferFailed_End(void);
void WifiSetupTransferConfirm_Init(void);
void WifiSetupTransferConfirm_LoadGraphics(void);
void WifiSetupTransferConfirm_FadeIn(void);
void WifiSetupTransferConfirm_WaitFadeIn(void);
void WifiSetupTransferConfirm_WaitDialog(void);
void WifiSetupTransferConfirm_Main(void);
void WifiSetupTransferConfirm_HandleInput(void);
void WifiSetupTransferConfirm_Update(void);
void WifiSetupTransferConfirm_WaitDialogClosed(void);
void WifiSetupTransferConfirm_WaitFadeOut(void);
void WifiSetupTransferConfirm_End(void);
void WifiSetupTransferConfirm_OnTransferEvent(void);
int WifiSetupTransferWait_IsLidClosed(void);
void WifiSetupTransferWait_Init(void);
void WifiSetupTransferWait_LoadGraphics(void);
void WifiSetupTransferWait_FadeIn(void);
void WifiSetupTransferWait_WaitFadeIn(void);
void WifiSetupTransferWait_WaitButtonBar(void);
void WifiSetupTransferWait_Main(void);
void WifiSetupTransferWait_HandleInput(void);
void WifiSetupTransferWait_Update(void);
void WifiSetupTransferWait_CheckButtons(void);
void WifiSetupTransferWait_Exit(void);
void WifiSetupTransferWait_WaitFadeOut(void);
void WifiSetupTransferWait_End(void);
void WifiSetupTransferWait_OnTransferEvent(int a);
int WifiSetupAoss_IsLidClosed(void);
void WifiSetupAoss_Init(void);
void WifiSetupAoss_LoadGraphics(void);
void WifiSetupAoss_FadeIn(void);
void WifiSetupAoss_WaitFadeIn(void);
void WifiSetupAoss_WaitButtonBar(void);
void WifiSetupAoss_Main(void);
void WifiSetupAoss_HandleInput(void);
void WifiSetupAoss_Update(void);
void WifiSetupAoss_CheckButtons(void);
void WifiSetupAoss_Exit(void);
void WifiSetupAoss_WaitFadeOut(void);
void WifiSetupAoss_End(void);
void WifiSetupAoss_InputTask(void);
void WifiSetupAoss_WaitErrorDialog(void);
void WifiSetupAoss_WaitErrorDialogClosed(void);
void WifiSetupAoss_WaitFinish(void);
void WifiSetupAoss_Cancel(void);
void WifiSetupAossDone_Init(void);
void WifiSetupAossDone_LoadGraphics(void);
void WifiSetupAossDone_FadeIn(void);
void WifiSetupAossDone_WaitFadeIn(void);
void WifiSetupAossDone_Main(void);
void WifiSetupAossDone_Update(void);
void WifiSetupAossDone_FadeOut(void);
void WifiSetupAossDone_End(void);
void WifiSetupAossDone_UpdateTimer(void);
void WifiSetupListScreen_Init(void);
void WifiSetupListScreen_LoadGraphics(void);
void ov13_0222FC68(void);
void ov13_0222FD10(void);
void ov13_0222FE84(void);
void ov13_0222FED8(void);
void ov13_0222FF14(void);
void ov13_0222FF3C(void);
void ov13_0222FF50(void);
void ov13_022301A4(void);
void ov13_02230414(void);
void ov13_02230520(void);
void ov13_02230540(void);
void ov13_022305B0(void);
void ov13_022307AC(void);
void ov13_02230854(int index, int row);
void ov13_022309CC(int a, int b);
void ov13_02230A58(void);
void ov13_02230B14(void *task);
void ov13_02230BD0(void *task);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230D10(int input);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F54(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void WifiSetupTextEntry_Init(void);
void WifiSetupTextEntry_LoadGraphics(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_022312C8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022314D8(void);
void ov13_022315B0(void);
void ov13_022316B4(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_0223186C(void);
int ov13_0223191C(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
