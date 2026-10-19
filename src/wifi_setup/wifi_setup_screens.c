#include "wifi_setup/wifi_setup_screens.h"

#include "global.h"

#include "dwc/dwc_bm_internal.h"
#include "wifi_setup/wifi_setup_app.h"
#include "wifi_setup/wifi_setup_connection_screens.h"
#include "wifi_setup/wifi_setup_internal.h"
#include "wifi_setup/wifi_setup_keyboard.h"
#include "wifi_setup/wifi_setup_local_wireless.h"
#include "wifi_setup/wifi_setup_scroll_bar.h"
#include "wifi_setup/wifi_setup_settings.h"
#include "wifi_setup/wifi_setup_ui.h"
#include "wifi_setup/wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point setup screens: setup client start/stop and status, the top menu, the message screens that follow
// it, the list screen and the text entry screen.

// Status copied from the setup client's status callback.
typedef struct WifiSetupClientStatus {
    int state; // 0 idle .. 7, see WifiSetupClient_GetResult
    int timeLeft;
    int error;
} WifiSetupClientStatus;

typedef struct WifiSetupBytePair {
    u8 values[2];
} WifiSetupBytePair;

// Top menu: two choices, the screen's graphics file.
typedef struct WifiSetupTopMenu {
    u8 selected;
    void *graphics;
} WifiSetupTopMenu;

typedef struct WifiSetupMenu3 {
    u8 selected;
    u8 confirmed;
    void *graphics;
    GXOamAttr *oam;
} WifiSetupMenu3;

typedef struct WifiSetupWaitScreen {
    u8 flag;
    u16 counter;
    void *task;
} WifiSetupWaitScreen;

typedef struct WifiSetupBox {
    u16 x, y, width, height;
} WifiSetupBox;

extern WifiSetupClientStatus *gWifiSetupClientStatus;
extern WifiSetupTopMenu gWifiSetupTopMenu;
extern u8 gWifiSetupEraseConfirmed;
extern WifiSetupMenu3 gWifiSetupOptions;
extern u8 gWifiSetupTransferResult;
extern u8 gWifiSetupTransferIntroProceed;
extern u8 gWifiSetupTransferCancelledTimer;
extern u8 gWifiSetupTransferFailed[];
extern u8 gWifiSetupTransferConfirm[];
extern u8 gWifiSetupTransferWaitResult;
extern WifiSetupWaitScreen gWifiSetupAoss;
extern u8 gWifiSetupAossDoneTimer;
extern u8 ov13_0224F53C[];

extern const WifiSetupBytePair gWifiSetupTopMenuPaletteRows;
extern WifiSetupRect gWifiSetupTopMenuTouchRects[];
extern u16 gWifiSetupTopMenuCursorLeft[];
extern u16 gWifiSetupTopMenuCursorTop[];
extern u16 gWifiSetupTopMenuCursorRight[];
extern u16 gWifiSetupTopMenuCursorBottom[];
extern const WifiSetupPath22 gWifiSetupTopMenuYbBgStep11NclPath;
extern u8 gWifiSetupOptionsPaletteRows[];
extern const WifiSetupPath22 gWifiSetupOptionsYbBgOptionNclPath;
extern const WifiSetupPath23 gWifiSetupOptionsYbBgOption1NclPath;
extern WifiSetupRect gWifiSetupOptionsTouchRects[];
extern u16 gWifiSetupOptionsCursorLeft[];
extern u16 gWifiSetupOptionsCursorTop[];
extern u16 gWifiSetupOptionsCursorRight[];
extern u16 gWifiSetupOptionsCursorBottom[];
extern const WifiSetupPath22 gWifiSetupListScreenYbBgStep31NclPath;
extern const WifiSetupPath25 gWifiSetupListScreenXb4ApListBackNscPath;
extern WifiSetupRect gWifiSetupListScreenTouchRects[];
extern u16 gWifiSetupListScreenCursorLeft[];
extern u16 gWifiSetupListScreenCursorTop[];
extern u16 gWifiSetupListScreenCursorRight[];
extern u32 gWifiSetupTextEntryParams[];
extern char *gWifiSetupMessageBankPaths[];

// graphics and text bank paths
extern char gWifiSetupListScreenJb4ApListNscPath[];
extern u8 gWifiSetupListScreenRowTemplates[];
extern u8 gWifiSetupListScreenIconTemplates[];
extern char gWifiSetupAossJbBgStep3NcgPath[];
extern char gWifiSetupAossYbBgStep3NclPath[];
extern char gWifiSetupAossXb4MultiNscPath[];
extern char gWifiSetupAossDoneXb4MultiNscPath[];
extern char gWifiSetupTransferConfirmJb5MoveNscPath[];
extern char gWifiSetupTransferWaitYb5MultiNscPath[];
extern char gWifiSetupTransferIntroYb5MultiNscPath[];
extern char gWifiSetupTransferCancelledYb5MultiNscPath[];
extern char gWifiSetupTransferFailedYb5MultiNscPath[];
extern char sStr_MsgUsaBmgL[];
extern char sStr_CharJtMainNceL[];
extern char sStr_CharJbMainNceL[];
extern char sStr_CharJtBgMainNcgL[];
extern char sStr_CharJtBgMainNclL[];
extern char sStr_CharJtObjMainNcgL[];
extern char sStr_CharXtObjMainNclL[];
extern char sStr_CharJbBgStep1_NcgL[];
extern char sStr_CharJbBgStep1_NclL[];
extern char sStr_CharJbObjMainNcgL[];
extern char sStr_CharYbObjMainNclL[];
extern char sStr_CharJtTopNscL[];
extern char sStr_CharJtStep1_NscL[];
extern char sStr_CharJbBgStep1_NcgL_2[];
extern char sStr_CharJbBgStep1_NclL_2[];
extern char sStr_CharJb2MenuNscL[];
extern char sStr_CharYb5MultiNscL[];
extern char sStr_CharYb5MultiNscL_2[];
extern char sStr_CharJb5InfoNscL[];
extern char gWifiSetupOptionsJbBgOptionNcgPath[];
extern char gWifiSetupOptionsJb5OptMenuNscPath[];
extern char gWifiSetupTransferringYb5MultiNscPath[];
typedef struct WifiSetupPaletteRow {
    u8 row;
} WifiSetupPaletteRow;

extern const WifiSetupPaletteRow gWifiSetupListScreenRescanPaletteRow;
extern u8 gWifiSetupTextEntryCursorRowY[];
extern u8 gWifiSetupTextEntryColumnX[];
extern char gWifiSetupTextEntryYbObjKbNclPath[];
extern char gWifiSetupTextEntryJbBgStep3NcgPath[];
extern char gWifiSetupTextEntryYbBgStep3NclPath[];
extern char gWifiSetupTextEntryXb4EditNscPath[];
extern char gWifiSetupTextEntryYbObjMainNclPath[];

extern u16 gWifiSetupSystemInfoMacFormat[];
extern u16 gWifiSetupSystemInfoIdFormat[];
extern u16 gWifiSetupSystemInfoNoIdText[];

extern void WifiSetupBgScreen_Create(void *a);
extern void WifiSetupApScan_FreeWork(void);
extern u8 WifiSetupApScan_GetEntries(void *p);
extern void WifiSetupConnections_Load(void);
extern void WifiSetupConnections_Free(void);
extern u8 *WifiSetupConnections_GetEdit(void);
extern void WifiSetupConnections_SetSsid(void *);
extern void WifiSetupConnections_GetSsid(void *);
extern void WifiSetupConnections_Edit(int);
extern void WifiSetupConnections_Reset(void);
extern void WifiSetupConnections_SetFromClientResult(WifiSetupResult *result);
extern void WifiSetupText_FreeCanvas(void *);
extern s32 FX_DivS32(s32 a, s32 b);
extern int swprintf(u16 *buf, u32 n, const u16 *fmt, ...);


static void WifiSetupClient_OnStatus(s32 *status);
static void *WifiSetupClient_Alloc(u32 size);
static void WifiSetupClient_Free(void *p);
static void WifiSetupScreens_Quit(void);
static void WifiSetupScreens_FadeIn(void);
static void WifiSetupScreens_WaitFadeIn(void);
static void WifiSetupTopMenu_LoadGraphics(void);
static void WifiSetupTopMenu_FadeIn(void);
static void WifiSetupTopMenu_WaitFadeIn(void);
static void WifiSetupTopMenu_WaitButtonBar(void);
static void WifiSetupTopMenu_Main(void);
static void WifiSetupTopMenu_HandleInput(void);
static void WifiSetupTopMenu_Update(void);
static void WifiSetupTopMenu_CheckButtons(void);
static void WifiSetupTopMenu_Exit(void);
static void WifiSetupTopMenu_WaitFadeOut(void);
static void WifiSetupTopMenu_End(void);
static void WifiSetupTopMenu_MoveCursor(int dir);
static void WifiSetupTopMenu_HighlightSelection(void);
static void WifiSetupEraseConfirm_Init(void);
static void WifiSetupEraseConfirm_LoadGraphics(void);
static void WifiSetupEraseConfirm_FadeIn(void);
static void WifiSetupEraseConfirm_WaitFadeIn(void);
static void WifiSetupEraseConfirm_WaitButtonBar(void);
static void WifiSetupEraseConfirm_Main(void);
static void WifiSetupEraseConfirm_HandleInput(void);
static void WifiSetupEraseConfirm_Update(void);
static void WifiSetupEraseConfirm_CheckButtons(void);
static void WifiSetupEraseConfirm_Exit(void);
static void WifiSetupEraseConfirm_WaitFadeOut(void);
static void WifiSetupEraseConfirm_End(void);
static void WifiSetupEraseConfirm_WaitDialog(void);
static void WifiSetupEraseConfirm_WaitDialogClosed(void);
static void WifiSetupEraseDone_Init(void);
static void WifiSetupEraseDone_LoadGraphics(void);
static void WifiSetupEraseDone_FadeIn(void);
static void WifiSetupEraseDone_WaitFadeIn(void);
static void WifiSetupEraseDone_WaitButtonBar(void);
static void WifiSetupEraseDone_Main(void);
static void WifiSetupEraseDone_HandleInput(void);
static void WifiSetupEraseDone_Update(void);
static void WifiSetupEraseDone_CheckButtons(void);
static void WifiSetupEraseDone_Exit(void);
static void WifiSetupEraseDone_WaitFadeOut(void);
static void WifiSetupEraseDone_PowerOff(void);
static void WifiSetupSystemInfo_Init(void);
static void WifiSetupSystemInfo_LoadGraphics(void);
static void WifiSetupSystemInfo_DrawInfo(void);
static void WifiSetupSystemInfo_FadeIn(void);
static void WifiSetupSystemInfo_WaitFadeIn(void);
static void WifiSetupSystemInfo_WaitButtonBar(void);
static void WifiSetupSystemInfo_Main(void);
static void WifiSetupSystemInfo_HandleInput(void);
static void WifiSetupSystemInfo_Update(void);
static void WifiSetupSystemInfo_CheckButtons(void);
static void WifiSetupSystemInfo_Exit(void);
static void WifiSetupSystemInfo_WaitFadeOut(void);
static void WifiSetupSystemInfo_End(void);
static void WifiSetupOptions_Init(void);
static void WifiSetupOptions_LoadGraphics(void);
static void WifiSetupOptions_FadeIn(void);
static void WifiSetupOptions_WaitFadeIn(void);
static void WifiSetupOptions_WaitButtonBar(void);
static void WifiSetupOptions_Main(void);
static void WifiSetupOptions_HandleInput(void);
static void WifiSetupOptions_Update(void);
static void WifiSetupOptions_CheckButtons(void);
static void WifiSetupOptions_Exit(void);
static void WifiSetupOptions_WaitFadeOut(void);
static void WifiSetupOptions_End(void);
static void WifiSetupOptions_MoveCursor(int dir);
static void WifiSetupOptions_HighlightSelection(void);
static void WifiSetupTransferring_Init(void);
static void WifiSetupTransferring_LoadGraphics(void);
static void WifiSetupTransferring_FadeIn(void);
static void WifiSetupTransferring_WaitFadeIn(void);
static void WifiSetupTransferring_Main(void);
static void WifiSetupTransferring_HandleInput(void);
static void WifiSetupTransferring_Update(void);
static void WifiSetupTransferring_FadeOut(void);
static void WifiSetupTransferring_End(void);
static void WifiSetupTransferring_OnTransferEvent(int a);
static void WifiSetupTransferDone_Init(void);
static void WifiSetupTransferDone_LoadGraphics(void);
static void WifiSetupTransferDone_FadeIn(void);
static void WifiSetupTransferDone_WaitFadeIn(void);
static void WifiSetupTransferDone_WaitButtonBar(void);
static void WifiSetupTransferDone_Main(void);
static void WifiSetupTransferDone_HandleInput(void);
static void WifiSetupTransferDone_Update(void);
static void WifiSetupTransferDone_CheckButtons(void);
static void WifiSetupTransferDone_Exit(void);
static void WifiSetupTransferDone_WaitFadeOut(void);
static void WifiSetupTransferDone_PowerOff(void);
static void WifiSetupTransferInfo_Init(void);
static void WifiSetupTransferInfo_LoadGraphics(void);
static void WifiSetupTransferInfo_FadeIn(void);
static void WifiSetupTransferInfo_WaitFadeIn(void);
static void WifiSetupTransferInfo_WaitButtonBar(void);
static void WifiSetupTransferInfo_Main(void);
static void WifiSetupTransferInfo_HandleInput(void);
static void WifiSetupTransferInfo_Update(void);
static void WifiSetupTransferInfo_CheckButtons(void);
static void WifiSetupTransferInfo_Exit(void);
static void WifiSetupTransferInfo_WaitFadeOut(void);
static void WifiSetupTransferInfo_End(void);
static void WifiSetupTransferIntro_Init(void);
static void WifiSetupTransferIntro_LoadGraphics(void);
static void WifiSetupTransferIntro_FadeIn(void);
static void WifiSetupTransferIntro_WaitFadeIn(void);
static void WifiSetupTransferIntro_WaitButtonBar(void);
static void WifiSetupTransferIntro_Main(void);
static void WifiSetupTransferIntro_HandleInput(void);
static void WifiSetupTransferIntro_Update(void);
static void WifiSetupTransferIntro_CheckButtons(void);
static void WifiSetupTransferIntro_Exit(void);
static void WifiSetupTransferIntro_WaitFadeOut(void);
static void WifiSetupTransferIntro_End(void);
static void WifiSetupTransferCancelled_Init(void);
static void WifiSetupTransferCancelled_LoadGraphics(void);
static void WifiSetupTransferCancelled_FadeIn(void);
static void WifiSetupTransferCancelled_WaitFadeIn(void);
static void WifiSetupTransferCancelled_Main(void);
static void WifiSetupTransferCancelled_Update(void);
static void WifiSetupTransferCancelled_FadeOut(void);
static void WifiSetupTransferCancelled_End(void);
static void WifiSetupTransferCancelled_UpdateTimer(void);
static void WifiSetupTransferFailed_Init(void);
static void WifiSetupTransferFailed_LoadGraphics(void);
static void WifiSetupTransferFailed_FadeIn(void);
static void WifiSetupTransferFailed_WaitFadeIn(void);
static void WifiSetupTransferFailed_WaitButtonBar(void);
static void WifiSetupTransferFailed_Main(void);
static void WifiSetupTransferFailed_HandleInput(void);
static void WifiSetupTransferFailed_Update(void);
static void WifiSetupTransferFailed_CheckButtons(void);
static void WifiSetupTransferFailed_Exit(void);
static void WifiSetupTransferFailed_WaitFadeOut(void);
static void WifiSetupTransferFailed_End(void);
static void WifiSetupTransferConfirm_Init(void);
static void WifiSetupTransferConfirm_LoadGraphics(void);
static void WifiSetupTransferConfirm_FadeIn(void);
static void WifiSetupTransferConfirm_WaitFadeIn(void);
static void WifiSetupTransferConfirm_WaitDialog(void);
static void WifiSetupTransferConfirm_Main(void);
static void WifiSetupTransferConfirm_HandleInput(void);
static void WifiSetupTransferConfirm_Update(void);
static void WifiSetupTransferConfirm_WaitDialogClosed(void);
static void WifiSetupTransferConfirm_WaitFadeOut(void);
static void WifiSetupTransferConfirm_End(void);
static void WifiSetupTransferConfirm_OnTransferEvent(void);
static int WifiSetupTransferWait_IsLidClosed(void);
static void WifiSetupTransferWait_Init(void);
static void WifiSetupTransferWait_LoadGraphics(void);
static void WifiSetupTransferWait_FadeIn(void);
static void WifiSetupTransferWait_WaitFadeIn(void);
static void WifiSetupTransferWait_WaitButtonBar(void);
static void WifiSetupTransferWait_Main(void);
static void WifiSetupTransferWait_HandleInput(void);
static void WifiSetupTransferWait_Update(void);
static void WifiSetupTransferWait_CheckButtons(void);
static void WifiSetupTransferWait_Exit(void);
static void WifiSetupTransferWait_WaitFadeOut(void);
static void WifiSetupTransferWait_End(void);
static void WifiSetupTransferWait_OnTransferEvent(int a);
static int WifiSetupAoss_IsLidClosed(void);
static void WifiSetupAoss_LoadGraphics(void);
static void WifiSetupAoss_FadeIn(void);
static void WifiSetupAoss_WaitFadeIn(void);
static void WifiSetupAoss_WaitButtonBar(void);
static void WifiSetupAoss_Main(void);
static void WifiSetupAoss_HandleInput(void);
static void WifiSetupAoss_Update(void);
static void WifiSetupAoss_CheckButtons(void);
static void WifiSetupAoss_Exit(void);
static void WifiSetupAoss_WaitFadeOut(void);
static void WifiSetupAoss_End(void);
static void WifiSetupAoss_InputTask(void);
static void WifiSetupAoss_WaitErrorDialog(void);
static void WifiSetupAoss_WaitErrorDialogClosed(void);
static void WifiSetupAoss_WaitFinish(void);
static void WifiSetupAoss_Cancel(void);
static void WifiSetupAossDone_Init(void);
static void WifiSetupAossDone_LoadGraphics(void);
static void WifiSetupAossDone_FadeIn(void);
static void WifiSetupAossDone_WaitFadeIn(void);
static void WifiSetupAossDone_Main(void);
static void WifiSetupAossDone_Update(void);
static void WifiSetupAossDone_FadeOut(void);
static void WifiSetupAossDone_End(void);
static void WifiSetupAossDone_UpdateTimer(void);
static void WifiSetupListScreen_LoadGraphics(void);
static void WifiSetupListScreen_CreateScrollBar(void);
static void WifiSetupListScreen_CreateRowObjects(void);
static void WifiSetupListScreen_FadeIn(void);
static void WifiSetupListScreen_WaitFadeIn(void);
static void WifiSetupListScreen_WaitButtonBar(void);
static void WifiSetupListScreen_Main(void);
static void WifiSetupListScreen_HandleInput(void);
static void WifiSetupListScreen_HandleScrollBar(void);
static void WifiSetupListScreen_CheckButtons(void);
static void WifiSetupListScreen_Exit(void);
static void WifiSetupListScreen_WaitFadeOut(void);
static void WifiSetupListScreen_End(void);
static void WifiSetupListScreen_DrawRows(void);
static void WifiSetupListScreen_DrawRowText(int index, int row);
static void WifiSetupListScreen_SetRowIcons(int a, int b);
static void WifiSetupListScreen_PositionRows(void);
static void WifiSetupListScreen_ScrollUpTask(void *task);
static void WifiSetupListScreen_ScrollDownTask(void *task);
static void WifiSetupListScreen_ScrollBgTask(void);
static void WifiSetupListScreen_ShowCursor(void);
static void WifiSetupListScreen_MoveCursor(int input);
static void WifiSetupListScreen_StartScrollUp(void);
static void WifiSetupListScreen_StartScrollDown(void);
static void WifiSetupListScreen_HighlightRescanButton(void);
static void WifiSetupListScreen_WaitErrorDialog(void);
static void WifiSetupListScreen_WaitErrorDialogClosed(void);
static void WifiSetupTextEntry_LoadGraphics(void);
static void WifiSetupTextEntry_FadeIn(void);
static void WifiSetupTextEntry_WaitFadeIn(void);
static void WifiSetupTextEntry_WaitKeyboard(void);
static void WifiSetupTextEntry_Main(void);
static void WifiSetupTextEntry_HandleInput(void);
static void WifiSetupTextEntry_Update(void);
static void WifiSetupTextEntry_Exit(void);
static void WifiSetupTextEntry_CloseKeyboard(void);
static void WifiSetupTextEntry_WaitKeyboardClosed(void);
static void WifiSetupTextEntry_End(void);
static void WifiSetupTextEntry_DrawText(void);
static void WifiSetupTextEntry_UpdateCursor(void);
static void WifiSetupTextEntry_WaitConfirmDialog(void);
static void WifiSetupTextEntry_WaitConfirmDialogClosed(void);
static int WifiSetupTextEntry_IsTextValid(void);
static void WifiSetupTextEntry_WaitErrorDialog(void);
static void WifiSetupTextEntry_WaitErrorDialogClosed(void);

void WifiSetupClient_Start(void) {
    gWifiSetupClientStatus = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupClientStatus), -4);
    if (WifiSetup_StartSimpleStart(0xf, 0x40, WifiSetupClient_OnStatus, WifiSetupClient_Alloc, WifiSetupClient_Free, 0x800) != 1) {
        OS_Terminate();
    }
    OS_Sleep(10);
}

void WifiSetupClient_Stop(void) {
    if (WifiSetup_StopSimpleStart() != 1) {
        OS_Terminate();
    }
    WifiSetupHeap_FreeAndClear(&gWifiSetupClientStatus);
}

int WifiSetupClient_GetResult(void) {
    WifiSetupResult buf;
    switch (gWifiSetupClientStatus->state) {
    case 0:
    case 1:
    case 3:
    case 5:
        return 0;
    case 2:
        return 1;
    case 4:
        return 2;
    case 6:
        if (WifiSetup_GetSimpleStartResult(&buf) != 1) {
            OS_Terminate();
        }
        if (buf.keyType >= 0 && buf.keyType <= 3 && buf.unk_24 == 1) {
            return 3;
        }
        return 5;
    case 7:
        return 4;
    }
}

void WifiSetupClient_StoreResult(void) {
    WifiSetupResult buf;
    if (WifiSetup_GetSimpleStartResult(&buf) != 1) {
        OS_Terminate();
    }
    WifiSetupConnections_SetFromClientResult(&buf);
}

static void WifiSetupClient_OnStatus(s32 *status) {
    *gWifiSetupClientStatus = *(WifiSetupClientStatus *)status;
}

static void *WifiSetupClient_Alloc(u32 size) {
    return WifiSetupHeap_AllocZeroed(size, 0x20);
}

static void WifiSetupClient_Free(void *p) {
    WifiSetupHeap_Free(p);
}

void WifiSetupScreens_StartQuit(void) {
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x14);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_Quit);
}

static void WifiSetupScreens_Quit(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupTask_SetQueueEnabled(0, 0);
    WifiSetupTask_SetQueueEnabled(1, 0);
    WifiSetupOamTemplate_Free(1);
    WifiSetupOamTemplate_Free(0);
    WifiSetupBackground_Free();
    WifiSetupCursor_Free();
    WifiSetupText_UnloadBank(gWifiSetupTextBank);
    WifiSetupText_FreeBanks();
    WifiSetupConnections_Free();
    WifiSetupApp_Exit();
}

void WifiSetupScreens_Init(void) {
    WifiSetupConnections_Load();
    WifiSetupText_InitBanks();
    WifiSetupCursor_Init();
    WifiSetupBackground_Init();
    WifiSetupMessage_Reset();
    if (WifiSetupApp_GetLanguage() == 1 && WifiSetupApp_HasOption(2) != 0) {
        gWifiSetupTextBank = WifiSetupText_LoadBank(sStr_MsgUsaBmgL);
    } else {
        gWifiSetupTextBank = WifiSetupText_LoadBank(gWifiSetupMessageBankPaths[WifiSetupApp_GetLanguage()]);
    }
    WifiSetupOamTemplate_Load(1, WifiSetup_LocalizePath(sStr_CharJtMainNceL));
    WifiSetupOamTemplate_Load(0, WifiSetup_LocalizePath(sStr_CharJbMainNceL));
    WifiSetup_LoadGraphics(sStr_CharJtBgMainNcgL, GXS_LoadBG1Char);
    WifiSetup_LoadGraphics(sStr_CharJtBgMainNclL, GXS_LoadBGPltt);
    WifiSetup_LoadGraphics(sStr_CharJtObjMainNcgL, GXS_LoadOBJ);
    WifiSetup_LoadGraphics(sStr_CharXtObjMainNclL, GXS_LoadOBJPltt);
    WifiSetup_LoadGraphics(sStr_CharJbBgStep1_NcgL, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(sStr_CharJbBgStep1_NclL, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(sStr_CharJbObjMainNcgL, GX_LoadOBJ);
    WifiSetup_LoadGraphics(sStr_CharYbObjMainNclL, GX_LoadOBJPltt);
    switch (WifiSetupApp_GetKind()) {
    case 0:
        WifiSetup_LoadGraphics(sStr_CharJtTopNscL, GXS_LoadBG1Scr);
        break;
    case 1:
        WifiSetup_LoadGraphics(sStr_CharJtStep1_NscL, GXS_LoadBG1Scr);
        break;
    }
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG1);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG1);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_FadeIn);
}

static void WifiSetupScreens_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG1, 0x14);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG1, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_WaitFadeIn);
}

static void WifiSetupScreens_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    switch (WifiSetupApp_GetKind()) {
    case 0:
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Init);
        break;
    case 1:
        WifiSetupApp_SetTransition(1, 1);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
        break;
    }
}

void WifiSetupTopMenu_Init(void) {
    int i;
    WifiSetupTopMenu_LoadGraphics();
    WifiSetupBackground_LoadHeader(0);
    WifiSetupMessage_Show(0x2e, -1, 0);
    WifiSetupBanner_LoadScreen(4);
    i = gWifiSetupTopMenu.selected * 4;
    WifiSetupCursor_ShowCorners(gWifiSetupTopMenuCursorLeft[i], gWifiSetupTopMenuCursorRight[i], gWifiSetupTopMenuCursorTop[i], gWifiSetupTopMenuCursorBottom[i]);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_FadeIn);
}

static void WifiSetupTopMenu_LoadGraphics(void) {
    WifiSetupPath22 name;
    name = gWifiSetupTopMenuYbBgStep11NclPath;
    WifiSetup_LoadGraphics(sStr_CharJbBgStep1_NcgL_2, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(sStr_CharJbBgStep1_NclL_2, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(sStr_CharJb2MenuNscL, GX_LoadBG2Scr);
    gWifiSetupTopMenu.graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(name.text), NULL, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTopMenu_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitFadeIn);
}

static void WifiSetupTopMenu_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitButtonBar);
}

static void WifiSetupTopMenu_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Main);
}

static void WifiSetupTopMenu_Main(void) {
    WifiSetupTopMenu_HandleInput();
    WifiSetupTopMenu_Update();
    WifiSetupTopMenu_CheckButtons();
}

static void WifiSetupTopMenu_HandleInput(void) {
    u32 i;
    WifiSetupRect *entry = gWifiSetupTopMenuTouchRects;
    for (i = 0; i < 2; i++, entry++) {
        if (WifiSetupInput_IsTouchPressed(entry) != 0) {
            int idx;
            WifiSetupButtonBar_SelectIfNone(1);
            gWifiSetupTopMenu.selected = i;
            idx = gWifiSetupTopMenu.selected * 4;
            WifiSetupCursor_ShowCorners(gWifiSetupTopMenuCursorLeft[idx], gWifiSetupTopMenuCursorRight[idx], gWifiSetupTopMenuCursorTop[idx], gWifiSetupTopMenuCursorBottom[idx]);
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP) != 0) {
        WifiSetupTopMenu_MoveCursor(WIFI_SETUP_CURSOR_UP);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN) != 0) {
        WifiSetupTopMenu_MoveCursor(WIFI_SETUP_CURSOR_DOWN);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT) != 0) {
        WifiSetupTopMenu_MoveCursor(WIFI_SETUP_CURSOR_LEFT);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT) != 0) {
        WifiSetupTopMenu_MoveCursor(WIFI_SETUP_CURSOR_RIGHT);
    }
}

static void WifiSetupTopMenu_Update(void) {
}

static void WifiSetupTopMenu_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(WifiSetupScreens_StartQuit);
        break;
    case 1:
        WifiSetupSound_Play(6);
        WifiSetupTopMenu_HighlightSelection();
        WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Exit);
        break;
    }
}

static void WifiSetupTopMenu_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitFadeOut);
}

static void WifiSetupTopMenu_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_End);
}

static void WifiSetupTopMenu_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(gWifiSetupTopMenu.graphics);
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    switch (gWifiSetupTopMenu.selected) {
    case 0:
        WifiSetupApp_SetTransition(1, 0);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
        break;
    case 1:
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
        break;
    }
}

static void WifiSetupTopMenu_MoveCursor(int dir) {
    int idx;
    if (dir == WIFI_SETUP_CURSOR_UP || dir == WIFI_SETUP_CURSOR_DOWN) {
        return;
    }
    gWifiSetupTopMenu.selected ^= 1;
    WifiSetupSound_Play(8);
    idx = gWifiSetupTopMenu.selected * 4;
    WifiSetupCursor_ShowCorners(gWifiSetupTopMenuCursorLeft[idx], gWifiSetupTopMenuCursorRight[idx], gWifiSetupTopMenuCursorTop[idx], gWifiSetupTopMenuCursorBottom[idx]);
}

static void WifiSetupTopMenu_HighlightSelection(void) {
    WifiSetupBytePair tbl = gWifiSetupTopMenuPaletteRows;
    WifiSetupPalette_LoadRow(gWifiSetupTopMenu.graphics, tbl.values[gWifiSetupTopMenu.selected], tbl.values[gWifiSetupTopMenu.selected]);
}

static void WifiSetupEraseConfirm_Init(void) {
    gWifiSetupEraseConfirmed = 0;
    WifiSetupEraseConfirm_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x12);
    WifiSetupMessage_Show(0x3b, -1, 0);
    WifiSetup_PrintMessage(0x17);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_FadeIn);
}

static void WifiSetupEraseConfirm_LoadGraphics(void) {
    WifiSetup_LoadGraphics(sStr_CharYb5MultiNscL, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupEraseConfirm_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitFadeIn);
}

static void WifiSetupEraseConfirm_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitButtonBar);
}

static void WifiSetupEraseConfirm_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Main);
}

static void WifiSetupEraseConfirm_Main(void) {
    WifiSetupEraseConfirm_HandleInput();
    WifiSetupEraseConfirm_Update();
    WifiSetupEraseConfirm_CheckButtons();
}

static void WifiSetupEraseConfirm_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupEraseConfirm_Update(void) {
}

static void WifiSetupEraseConfirm_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Exit);
        break;
    case 1:
        WifiSetupSound_Play(6);
        WifiSetupDialog_Open(0x18, 0, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitDialog);
        break;
    }
}

static void WifiSetupEraseConfirm_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitFadeOut);
}

static void WifiSetupEraseConfirm_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupEraseConfirmed == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_End);
}

static void WifiSetupEraseConfirm_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupEraseConfirmed == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupEraseConfirmed == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupEraseConfirmed == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Init);
    }
}

static void WifiSetupEraseConfirm_WaitDialog(void) {
    int r = WifiSetupDialog_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        WifiSetupSound_Play(0xe);
        gWifiSetupEraseConfirmed = 1;
    } else {
        WifiSetupSound_Play(7);
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitDialogClosed);
}

static void WifiSetupEraseConfirm_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Exit);
}

static void WifiSetupEraseDone_Init(void) {
    WifiSetupEraseDone_LoadGraphics();
    WifiSetup_PrintMessage(0x19);
    WifiSetupConnections_Reset();
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_FadeIn);
}

static void WifiSetupEraseDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(sStr_CharYb5MultiNscL_2, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupEraseDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitFadeIn);
}

static void WifiSetupEraseDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(5);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitButtonBar);
}

static void WifiSetupEraseDone_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Main);
}

static void WifiSetupEraseDone_Main(void) {
    WifiSetupEraseDone_HandleInput();
    WifiSetupEraseDone_Update();
    WifiSetupEraseDone_CheckButtons();
}

static void WifiSetupEraseDone_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupEraseDone_Update(void) {
}

static void WifiSetupEraseDone_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Exit);
}

static void WifiSetupEraseDone_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitFadeOut);
}

static void WifiSetupEraseDone_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x40);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x40);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_PowerOff);
}

static void WifiSetupEraseDone_PowerOff(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

static void WifiSetupSystemInfo_Init(void) {
    WifiSetupSystemInfo_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x11);
    WifiSetupMessage_Show(0x3a, -1, 0);
    WifiSetupSystemInfo_DrawInfo();
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_FadeIn);
}

static void WifiSetupSystemInfo_LoadGraphics(void) {
    WifiSetup_LoadGraphics(sStr_CharJb5InfoNscL, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupSystemInfo_DrawInfo(void) {
    DwcWiFiInfo info;
    u32 parts[4];
    u16 buf[20];
    u8 mac[6];
    u64 id;
    int i;
    WifiSetupCanvas *win = WifiSetupText_InitBgCanvas(0, 0);
    OS_GetMacAddress(mac);
    swprintf(buf, NELEMS(buf), gWifiSetupSystemInfoMacFormat, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    WifiSetupText_DrawInRect(win, 8, 0x40, 0xf0, 0x1c, 2, 0x480, buf);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.userId != 0) {
        id = info.userId;
        parts[3] = (id % 10) * 1000;
        id /= 10;
        for (i = 0; i < 3; i++) {
            parts[2 - i] = id % 10000;
            id /= 10000;
        }
        swprintf(buf, NELEMS(buf), gWifiSetupSystemInfoIdFormat, parts[0], parts[1], parts[2], parts[3]);
    } else {
        swprintf(buf, NELEMS(buf), gWifiSetupSystemInfoNoIdText);
    }
    WifiSetupText_DrawInRect(win, 8, 0x78, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
}

static void WifiSetupSystemInfo_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitFadeIn);
}

static void WifiSetupSystemInfo_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(2);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitButtonBar);
}

static void WifiSetupSystemInfo_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_Main);
}

static void WifiSetupSystemInfo_Main(void) {
    WifiSetupSystemInfo_HandleInput();
    WifiSetupSystemInfo_Update();
    WifiSetupSystemInfo_CheckButtons();
}

static void WifiSetupSystemInfo_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupSystemInfo_Update(void) {
}

static void WifiSetupSystemInfo_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_Exit);
}

static void WifiSetupSystemInfo_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitFadeOut);
}

static void WifiSetupSystemInfo_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_End);
}

static void WifiSetupSystemInfo_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
}

static void WifiSetupOptions_Init(void) {
    int idx;
    gWifiSetupOptions.confirmed = 0;
    WifiSetupOptions_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x10);
    WifiSetupBanner_LoadScreen(3);
    WifiSetupMessage_Show(0x39, -1, 0);
    gWifiSetupOptions.oam = WifiSetupOamTemplate_AllocOam(0, 0x5b);
    gWifiSetupOptions.oam->attr01 = (gWifiSetupOptions.oam->attr01 & 0xFE00FF00) | 0x84 | 0xE00000;
    gWifiSetupOptions.oam->attr2 = (gWifiSetupOptions.oam->attr2 & ~0xc00) | 0xc00;
    idx = gWifiSetupOptions.selected * 4;
    WifiSetupCursor_ShowCorners(gWifiSetupOptionsCursorLeft[idx], gWifiSetupOptionsCursorRight[idx], gWifiSetupOptionsCursorTop[idx], gWifiSetupOptionsCursorBottom[idx]);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_FadeIn);
}

static void WifiSetupOptions_LoadGraphics(void) {
    WifiSetupPath22 a;
    WifiSetupPath23 b;
    DwcWiFiInfo info;
    u8 *src;
    u8 *dst;
    int i;
    u8 *pal;
    a = gWifiSetupOptionsYbBgOptionNclPath;
    b = gWifiSetupOptionsYbBgOption1NclPath;
    WifiSetup_LoadGraphics(gWifiSetupOptionsJbBgOptionNcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupOptionsJb5OptMenuNscPath, GX_LoadBG2Scr);
    gWifiSetupOptions.graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(b.text), NULL, 4);
    pal = WifiSetupFile_Load(WifiSetup_LocalizePath(a.text), NULL, 4);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.userId == 0) {
        src = pal + 0xc0;
        dst = pal + 0x40;
        for (i = 0; i < 2; i++) {
            MI_CpuCopy8(src, dst, 0x20);
            src += 0x20;
            dst += 0x20;
        }
    }
    DC_FlushRange(pal, 0x200);
    GX_LoadBGPltt(pal, 0, 0x200);
    WifiSetupFile_Free(pal);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupOptions_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitFadeIn);
}

static void WifiSetupOptions_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitButtonBar);
}

static void WifiSetupOptions_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Main);
}

static void WifiSetupOptions_Main(void) {
    WifiSetupOptions_HandleInput();
    WifiSetupOptions_Update();
    WifiSetupOptions_CheckButtons();
}

static void WifiSetupOptions_HandleInput(void) {
    u32 i;
    WifiSetupRect *entry = gWifiSetupOptionsTouchRects;
    for (i = 0; i < 3; i++, entry++) {
        if (WifiSetupInput_IsTouchPressed(entry) != 0) {
            int idx;
            WifiSetupButtonBar_SelectIfNone(1);
            gWifiSetupOptions.selected = i;
            idx = gWifiSetupOptions.selected * 4;
            WifiSetupCursor_ShowCorners(gWifiSetupOptionsCursorLeft[idx], gWifiSetupOptionsCursorRight[idx], gWifiSetupOptionsCursorTop[idx], gWifiSetupOptionsCursorBottom[idx]);
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP) != 0) {
        WifiSetupOptions_MoveCursor(WIFI_SETUP_CURSOR_UP);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN) != 0) {
        WifiSetupOptions_MoveCursor(WIFI_SETUP_CURSOR_DOWN);
    }
}

static void WifiSetupOptions_Update(void) {
}

static void WifiSetupOptions_CheckButtons(void) {
    DwcWiFiInfo info;
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        DWCi_BM_GetWiFiInfo(&info);
        if (gWifiSetupOptions.selected != 0 && info.userId == 0) {
            WifiSetupSound_Play(9);
            WifiSetupButtonBar_SetSelected(-1);
            return;
        }
        WifiSetupSound_Play(6);
        WifiSetupOptions_HighlightSelection();
        gWifiSetupOptions.confirmed = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Exit);
}

static void WifiSetupOptions_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitFadeOut);
}

static void WifiSetupOptions_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (gWifiSetupOptions.confirmed != 0) {
        WifiSetupButtonBar_Close();
    }
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_End);
}

static void WifiSetupOptions_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupOam_Release(gWifiSetupOptions.oam);
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(gWifiSetupOptions.graphics);
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupOptions.confirmed == 0) {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Init);
        return;
    }
    switch (gWifiSetupOptions.selected) {
    case 0:
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_Init);
        break;
    case 1:
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Init);
        break;
    case 2:
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_Init);
        break;
    }
}

static void WifiSetupOptions_MoveCursor(int dir) {
    int idx;
    if (dir == WIFI_SETUP_CURSOR_UP) {
        gWifiSetupOptions.selected = FX_ModS32(gWifiSetupOptions.selected + 2, 3);
    } else {
        gWifiSetupOptions.selected = FX_ModS32(gWifiSetupOptions.selected + 1, 3);
    }
    WifiSetupSound_Play(8);
    idx = gWifiSetupOptions.selected * 4;
    WifiSetupCursor_ShowCorners(gWifiSetupOptionsCursorLeft[idx], gWifiSetupOptionsCursorRight[idx], gWifiSetupOptionsCursorTop[idx], gWifiSetupOptionsCursorBottom[idx]);
}

static void WifiSetupOptions_HighlightSelection(void) {
    WifiSetupPalette_LoadRow(gWifiSetupOptions.graphics, gWifiSetupOptionsPaletteRows[gWifiSetupOptions.selected], gWifiSetupOptionsPaletteRows[gWifiSetupOptions.selected]);
}

static void WifiSetupTransferring_Init(void) {
    WifiSetupTransferring_LoadGraphics();
    WifiSetup_PrintMessage(0x1e);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSignalIcon_Create(1);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_FadeIn);
}

static void WifiSetupTransferring_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferringYb5MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferring_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_WaitFadeIn);
}

static void WifiSetupTransferring_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupTransfer_SetCallback((WifiSetupSessionCallback)WifiSetupTransferring_OnTransferEvent);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_Main);
}

static void WifiSetupTransferring_Main(void) {
    WifiSetupTransferring_HandleInput();
    WifiSetupTransferring_Update();
}

static void WifiSetupTransferring_HandleInput(void) {
}

static void WifiSetupTransferring_Update(void) {
}

static void WifiSetupTransferring_FadeOut(void) {
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_End);
}

static void WifiSetupTransferring_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupSignalIcon_Destroy();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(0, 1);
    if (gWifiSetupTransferResult == 0) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else if (gWifiSetupTransferResult == 2) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_Init);
    } else {
        WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Init);
    }
}

static void WifiSetupTransferring_OnTransferEvent(int a) {
    if (a == 2) {
        WifiSetupConnections_Reset();
        gWifiSetupTransferResult = 1;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x10);
    } else if (a == 3) {
        gWifiSetupTransferResult = 2;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
    } else {
        gWifiSetupTransferResult = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupTransfer_Cancel();
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_FadeOut);
}

static void WifiSetupTransferDone_Init(void) {
    WifiSetupTransferDone_LoadGraphics();
    WifiSetup_PrintMessage(0x1f);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_FadeIn);
}

static void WifiSetupTransferDone_LoadGraphics(void) {
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitFadeIn);
}

static void WifiSetupTransferDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(5);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitButtonBar);
}

static void WifiSetupTransferDone_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Main);
}

static void WifiSetupTransferDone_Main(void) {
    WifiSetupTransferDone_HandleInput();
    WifiSetupTransferDone_Update();
    WifiSetupTransferDone_CheckButtons();
}

static void WifiSetupTransferDone_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupTransferDone_Update(void) {
}

static void WifiSetupTransferDone_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Exit);
}

static void WifiSetupTransferDone_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitFadeOut);
}

static void WifiSetupTransferDone_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x40);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0x40);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_PowerOff);
}

static void WifiSetupTransferDone_PowerOff(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

static void WifiSetupTransferInfo_Init(void) {
    WifiSetupTransferInfo_LoadGraphics();
    WifiSetup_PrintMessage(0x1a);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_FadeIn);
}

static void WifiSetupTransferInfo_LoadGraphics(void) {
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferInfo_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitFadeIn);
}

static void WifiSetupTransferInfo_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(4);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitButtonBar);
}

static void WifiSetupTransferInfo_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Main);
}

static void WifiSetupTransferInfo_Main(void) {
    WifiSetupTransferInfo_HandleInput();
    WifiSetupTransferInfo_Update();
    WifiSetupTransferInfo_CheckButtons();
}

static void WifiSetupTransferInfo_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupTransferInfo_Update(void) {
}

static void WifiSetupTransferInfo_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Exit);
}

static void WifiSetupTransferInfo_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitFadeOut);
}

static void WifiSetupTransferInfo_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_End);
}

static void WifiSetupTransferInfo_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Init);
}

static void WifiSetupTransferIntro_Init(void) {
    gWifiSetupTransferIntroProceed = 0;
    WifiSetupTransferIntro_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x13);
    WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetup_PrintMessage(0x1b);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_FadeIn);
}

static void WifiSetupTransferIntro_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferIntroYb5MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferIntro_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitFadeIn);
}

static void WifiSetupTransferIntro_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitButtonBar);
}

static void WifiSetupTransferIntro_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_Main);
}

static void WifiSetupTransferIntro_Main(void) {
    WifiSetupTransferIntro_HandleInput();
    WifiSetupTransferIntro_Update();
    WifiSetupTransferIntro_CheckButtons();
}

static void WifiSetupTransferIntro_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupTransferIntro_Update(void) {
}

static void WifiSetupTransferIntro_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(6);
        gWifiSetupTransferIntroProceed = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_Exit);
}

static void WifiSetupTransferIntro_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitFadeOut);
}

static void WifiSetupTransferIntro_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupTransferIntroProceed == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_End);
}

static void WifiSetupTransferIntro_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupTransferIntroProceed == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupTransferIntroProceed == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupTransferIntroProceed == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Init);
    }
}

static void WifiSetupTransferCancelled_Init(void) {
    gWifiSetupTransferCancelledTimer = 0;
    WifiSetupTransferCancelled_LoadGraphics();
    WifiSetup_PrintMessage(0x21);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_FadeIn);
}

static void WifiSetupTransferCancelled_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferCancelledYb5MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferCancelled_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_WaitFadeIn);
}

static void WifiSetupTransferCancelled_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_Main);
}

static void WifiSetupTransferCancelled_Main(void) {
    WifiSetupTransferCancelled_UpdateTimer();
    WifiSetupTransferCancelled_Update();
}

static void WifiSetupTransferCancelled_Update(void) {
}

static void WifiSetupTransferCancelled_FadeOut(void) {
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_End);
}

static void WifiSetupTransferCancelled_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
}

static void WifiSetupTransferCancelled_UpdateTimer(void) {
    if (++gWifiSetupTransferCancelledTimer < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_FadeOut);
}

static void WifiSetupTransferFailed_Init(void) {
    WifiSetupTransferFailed_LoadGraphics();
    WifiSetup_PrintMessage(0x20);
    gWifiSetupTransferFailed[0] = WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_FadeIn);
}

static void WifiSetupTransferFailed_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferFailedYb5MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferFailed_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupTransferFailed[0] != 0) {
        WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
        WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitFadeIn);
}

static void WifiSetupTransferFailed_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitButtonBar);
}

static void WifiSetupTransferFailed_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Main);
}

static void WifiSetupTransferFailed_Main(void) {
    WifiSetupTransferFailed_HandleInput();
    WifiSetupTransferFailed_Update();
    WifiSetupTransferFailed_CheckButtons();
}

static void WifiSetupTransferFailed_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupTransferFailed_Update(void) {
}

static void WifiSetupTransferFailed_CheckButtons(void) {
    int r = WifiSetupButtonBar_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        gWifiSetupTransferFailed[1] = 1;
        WifiSetupSound_Play(6);
    } else {
        gWifiSetupTransferFailed[1] = 0;
        WifiSetupSound_Play(7);
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Exit);
}

static void WifiSetupTransferFailed_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitFadeOut);
}

static void WifiSetupTransferFailed_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupTransferFailed[1] == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_End);
}

static void WifiSetupTransferFailed_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupTransferFailed[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupTransferFailed[1] == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Init);
    }
}

static void WifiSetupTransferConfirm_Init(void) {
    u16 buf[11];
    u8 *src = WifiSetupTransfer_GetChildUser();
    WifiSetupCanvas *win = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupTransferConfirm[1] = 0;
    gWifiSetupTransferConfirm[0] = 0;
    WifiSetupTransferConfirm_LoadGraphics();
    MIi_CpuClear16(0, buf, sizeof(buf));
    MIi_CpuCopy16((u16 *)(src + 2), buf, src[1] * 2);
    WifiSetupText_DrawInRect(win, 8, 0x35, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
    WifiSetupTransfer_SetCallback((WifiSetupSessionCallback)WifiSetupTransferConfirm_OnTransferEvent);
    WifiSetupDialog_Open(0x1d, 4, 0, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_FadeIn);
}

static void WifiSetupTransferConfirm_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferConfirmJb5MoveNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferConfirm_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitFadeIn);
}

static void WifiSetupTransferConfirm_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitDialog);
}

static void WifiSetupTransferConfirm_WaitDialog(void) {
    if (WifiSetupDialog_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_Main);
}

static void WifiSetupTransferConfirm_Main(void) {
    WifiSetupTransferConfirm_HandleInput();
    WifiSetupTransferConfirm_Update();
    switch (WifiSetupDialog_GetSelected()) {
    case 0:
        gWifiSetupTransferConfirm[1] = 0;
        WifiSetupSound_Play(7);
        break;
    case 1:
        gWifiSetupTransferConfirm[1] = 1;
        WifiSetupSound_Play(0xe);
        break;
    default:
        return;
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitDialogClosed);
}

static void WifiSetupTransferConfirm_HandleInput(void) {
}

static void WifiSetupTransferConfirm_Update(void) {
}

static void WifiSetupTransferConfirm_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    if (gWifiSetupTransferConfirm[1] == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitFadeOut);
}

static void WifiSetupTransferConfirm_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupTransferConfirm[0] == 0 && gWifiSetupTransferConfirm[1] == 1) {
        WifiSetupTransfer_StartDownload();
    } else {
        WifiSetupTransfer_Cancel();
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_End);
}

static void WifiSetupTransferConfirm_End(void) {
    if (gWifiSetupTransferConfirm[0] != 0 || gWifiSetupTransferConfirm[1] == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupTransferConfirm[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupApp_SetTransition(0, 1);
    if (gWifiSetupTransferConfirm[0] != 0) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else if (gWifiSetupTransferConfirm[1] == 0) {
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetStateFunc(WifiSetupTransferring_Init);
    }
}

static void WifiSetupTransferConfirm_OnTransferEvent(void) {
    gWifiSetupTransferConfirm[0] = 1;
}

static int WifiSetupTransferWait_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

static void WifiSetupTransferWait_Init(void) {
    WifiSetupTransfer_Start((WifiSetupSessionCallback)WifiSetupTransferWait_OnTransferEvent);
    gWifiSetupTransferWaitResult = 0;
    WifiSetupTransferWait_LoadGraphics();
    WifiSetup_PrintMessage(0x1c);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_FadeIn);
}

static void WifiSetupTransferWait_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTransferWaitYb5MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTransferWait_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitFadeIn);
}

static void WifiSetupTransferWait_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitButtonBar);
}

static void WifiSetupTransferWait_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Main);
}

static void WifiSetupTransferWait_Main(void) {
    WifiSetupTransferWait_HandleInput();
    WifiSetupTransferWait_Update();
    WifiSetupTransferWait_CheckButtons();
}

static void WifiSetupTransferWait_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupTransferWait_IsLidClosed() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupTransferWait_Update(void) {
}

static void WifiSetupTransferWait_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Exit);
}

static void WifiSetupTransferWait_Exit(void) {
    if (gWifiSetupTransferWaitResult == 0) {
        WifiSetupTransfer_Cancel();
    }
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitFadeOut);
}

static void WifiSetupTransferWait_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (gWifiSetupTransferWaitResult == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupTransferWaitResult == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_End);
}

static void WifiSetupTransferWait_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupTransferWaitResult == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    if (gWifiSetupTransferWaitResult == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupTransferWaitResult == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupTransferWaitResult == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else if (gWifiSetupTransferWaitResult == 2) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_Init);
    }
}

static void WifiSetupTransferWait_OnTransferEvent(int a) {
    WifiSetupSound_Stop();
    if (a == 0) {
        gWifiSetupTransferWaitResult = 1;
        WifiSetupSound_Play(0x10);
    } else {
        gWifiSetupTransferWaitResult = 2;
        WifiSetupSound_Play(0x12);
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Exit);
}

static int WifiSetupAoss_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void WifiSetupAoss_Init(void) {
    gWifiSetupAoss.counter = 0;
    WifiSetupAoss_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupMessage_Show(0x36, -1, 0);
    WifiSetupBanner_Show(2);
    WifiSetup_PrintMessage(0x22);
    WifiSetupBusyIcon_Create(0);
    WifiSetupApp_StartAossClient();
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_FadeIn);
}

static void WifiSetupAoss_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAossJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupAossYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupAossXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAoss_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFadeIn);
}

static void WifiSetupAoss_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitButtonBar);
}

static void WifiSetupAoss_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    gWifiSetupAoss.task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupAoss_InputTask, 0, 0x78);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Main);
}

static void WifiSetupAoss_Main(void) {
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_Update();
    WifiSetupAoss_CheckButtons();
    switch (WifiSetupApp_PollAossClient()) {
    case WIFI_SETUP_CLIENT_BUSY:
        return;
    case WIFI_SETUP_CLIENT_DONE:
        if (gWifiSetupAoss.task == NULL) {
            return;
        }
        gWifiSetupAoss.flag = 1;
        WifiSetupTask_Destroy(1, gWifiSetupAoss.task);
        gWifiSetupAoss.task = NULL;
        WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFinish);
        break;
    case WIFI_SETUP_CLIENT_FAILED:
        if (gWifiSetupAoss.task == NULL) {
            return;
        }
        WifiSetupSound_Stop();
        WifiSetupDialog_Open(0xc, 1, 1, -1, 0);
        WifiSetupSound_Play(9);
        WifiSetupButtonBar_Lock();
        WifiSetupTask_Destroy(1, gWifiSetupAoss.task);
        gWifiSetupAoss.task = NULL;
        WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitErrorDialog);
        break;
    }
}

static void WifiSetupAoss_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAoss_IsLidClosed() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupAoss_Update(void) {
}

static void WifiSetupAoss_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    if (gWifiSetupAoss.task == NULL) {
        return;
    }
    WifiSetupTask_DestroyLater(1, gWifiSetupAoss.task);
    gWifiSetupAoss.task = NULL;
    WifiSetupButtonBar_Lock();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Cancel);
}

static void WifiSetupAoss_Exit(void) {
    WifiSetupButtonBar_Lock();
    if (gWifiSetupAoss.task != NULL) {
        WifiSetupTask_Destroy(1, gWifiSetupAoss.task);
    }
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFadeOut);
}

static void WifiSetupAoss_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupAoss.flag == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_End);
}

static void WifiSetupAoss_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupAoss.flag == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupApp_StopAossClient(gWifiSetupAoss.flag != 0 ? 1 : 0);
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupAoss.flag == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupAoss.flag == 0) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
    } else {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupAossDone_Init);
    }
}

static void WifiSetupAoss_InputTask(void) {
    WifiSetupInput_Update();
    WifiSetupTask_RunQueue(0);
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_CheckButtons();
}

static void WifiSetupAoss_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitErrorDialogClosed);
}

static void WifiSetupAoss_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

static void WifiSetupAoss_WaitFinish(void) {
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_Update();
    WifiSetupAoss_CheckButtons();
    gWifiSetupAoss.counter++;
    if (gWifiSetupAoss.counter < 0x438) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

static void WifiSetupAoss_Cancel(void) {
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    gWifiSetupAoss.flag = 0;
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

static void WifiSetupAossDone_Init(void) {
    gWifiSetupAossDoneTimer = 0;
    WifiSetupAossDone_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x23);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_FadeIn);
}

static void WifiSetupAossDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAossDoneXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAossDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_WaitFadeIn);
}

static void WifiSetupAossDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_Main);
}

static void WifiSetupAossDone_Main(void) {
    WifiSetupAossDone_UpdateTimer();
    WifiSetupAossDone_Update();
}

static void WifiSetupAossDone_Update(void) {
}

static void WifiSetupAossDone_FadeOut(void) {
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_End);
}

static void WifiSetupAossDone_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetScreenArgs(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Init);
}

static void WifiSetupAossDone_UpdateTimer(void) {
    if (++gWifiSetupAossDoneTimer < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_FadeOut);
}

void WifiSetupListScreen_Init(void) {
    int result;
    gWifiSetupListScreen.work = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupListWork), 4);
    WifiSetupApp_GetScreenArgs(&result, 0);
    if (result == 0) {
        gWifiSetupListScreen.selected = 0;
        gWifiSetupListScreen.position = 0;
    }
    gWifiSetupListScreen.work->count = WifiSetupApScan_GetEntries(gWifiSetupListScreen.work);
    WifiSetupListScreen_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupMessage_Show(0x34, -1, 0);
    WifiSetupBanner_Show(2);
    WifiSetupListScreen_CreateScrollBar();
    WifiSetupListScreen_CreateRowObjects();
    gWifiSetupListScreen.work->buffer = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupListScreen.work->scrollBgTask = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollBgTask, 0, 0x6e);
    WifiSetupListScreen_DrawRows();
    WifiSetupListScreen_ShowCursor();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_FadeIn);
}

static void WifiSetupListScreen_LoadGraphics(void) {
    WifiSetupPath25 a;
    WifiSetupPath22 b;
    a = gWifiSetupListScreenXb4ApListBackNscPath;
    b = gWifiSetupListScreenYbBgStep31NclPath;
    WifiSetup_LoadGraphics(gWifiSetupListScreenJb4ApListNscPath, GX_LoadBG3Scr);
    gWifiSetupListScreen.work->bgScreenFile = WifiSetupFile_Load(WifiSetup_LocalizePath(a.text), 0, 4);
    WifiSetupBgScreen_Create(gWifiSetupListScreen.work->bgScreenFile);
    WifiSetupBgScreen_MarkDirty();
    gWifiSetupListScreen.work->paletteFile = WifiSetupFile_Load(WifiSetup_LocalizePath(b.text), 0, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(2);
}

static void WifiSetupListScreen_CreateScrollBar(void) {
    int mode;
    int val;
    WifiSetupListWork *big = gWifiSetupListScreen.work;
    big->total = (big->count - 4) * 0x1c;
    big = gWifiSetupListScreen.work;
    val = 0;
    if (big->count <= 4) {
        mode = 0;
        big->scrollBarRange = 0;
    } else if (big->count <= 8) {
        big->scrollBarRange = 0x1f;
        mode = 1;
    } else {
        big->scrollBarRange = 0x37;
        mode = 2;
    }
    if (mode != 0) {
        val = FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->scrollBarRange, gWifiSetupListScreen.work->total);
    }
    WifiSetupScrollBar_Create(mode, 0x55, 0xec, 0x3f, val);
}

static void WifiSetupListScreen_CreateRowObjects(void) {
    int n = gWifiSetupListScreen.work->count;
    if (n > 5) {
        n = 5;
    }
    int i = 0;
    for (; i < n; i++) {
        gWifiSetupListScreen.work->rowOams[i] = WifiSetupOamTemplate_AllocOam(0, 46);
        gWifiSetupListScreen.work->iconOams[i] = WifiSetupOamTemplate_AllocOam(0, 24);
    }
    for (i = 0; (u32)i < 3; i++) {
        WifiSetupOamTemplate_Copy(0, gWifiSetupListScreenRowTemplates[i], gWifiSetupListScreen.work->rowOams[0]);
        gWifiSetupListScreen.work->rowKindChars[i] = gWifiSetupListScreen.work->rowOams[0]->attr2 & 1023;
    }
    for (i = 0; (u32)i < 4; i++) {
        WifiSetupOamTemplate_Copy(0, gWifiSetupListScreenIconTemplates[i], gWifiSetupListScreen.work->iconOams[0]);
        gWifiSetupListScreen.work->iconChars[i] = gWifiSetupListScreen.work->iconOams[0]->attr2 & 1023;
    }
    for (i = 0; i < n; i++) {
        gWifiSetupListScreen.work->rowOams[i]->attr2 = (gWifiSetupListScreen.work->rowOams[i]->attr2 & ~0xC00) | 0xC00;   // priority 3
        gWifiSetupListScreen.work->iconOams[i]->attr2 = (gWifiSetupListScreen.work->iconOams[i]->attr2 & ~0xC00) | 0xC00; // priority 3
    }
}

static void WifiSetupListScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitFadeIn);
}

static void WifiSetupListScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitButtonBar);
}

static void WifiSetupListScreen_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Main);
}

static void WifiSetupListScreen_Main(void) {
    WifiSetupListScreen_HandleInput();
    WifiSetupListScreen_HandleScrollBar();
    WifiSetupListScreen_CheckButtons();
}

static void WifiSetupListScreen_HandleInput(void) {
    int i;
    WifiSetupRect *box;
    if (gWifiSetupListScreen.work->task || gWifiSetupListScreen.work->draggingScrollBar) {
        return;
    }
    if (WifiSetupInput_IsTouchPressed(&gWifiSetupInputPanelRect)) {
        gWifiSetupListScreen.work->selected = -1;
        for (i = 0, box = gWifiSetupListScreenTouchRects; (u32)i < 5; i++, box++) {
            if (WifiSetupInput_IsTouchPressed(box)) {
                if (i < 4) {
                    gWifiSetupListScreen.work->selected = i;
                    break;
                }
                WifiSetupButtonBar_SelectIfNone(1);
                gWifiSetupListScreen.selected = i;
                WifiSetupListScreen_ShowCursor();
                return;
            }
        }
    }
    if (WifiSetupInput_IsTouchReleased(&gWifiSetupInputPanelRect)) {
        WifiSetupRect *pos;
        int j;
        for (j = 0, pos = gWifiSetupListScreenTouchRects; j < 4; j++, pos++) {
            if (WifiSetupInput_IsTouchReleased(pos)) {
                if (gWifiSetupListScreen.work->selected != j) {
                    break;
                }
                if (j >= gWifiSetupListScreen.work->count) {
                    WifiSetupSound_Play(9);
                    break;
                }
                WifiSetupButtonBar_SelectIfNone(1);
                gWifiSetupListScreen.selected = j;
                WifiSetupListScreen_ShowCursor();
                return;
            }
        }
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        WifiSetupButtonBar_SelectIfNone(1);
        WifiSetupScrollBar_Disable();
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_L)) {
        WifiSetupListScreen_StartScrollDown();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_L)) {
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_R)) {
        WifiSetupListScreen_StartScrollUp();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_R)) {
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupListScreen_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_KEY_UP)) {
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupListScreen_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_KEY_DOWN)) {
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 0;
    }
}

static void WifiSetupListScreen_HandleScrollBar(void) {
    if (gWifiSetupListScreen.work->task) {
        return;
    }
    if (gWifiSetupListScreen.work->delay) {
        gWifiSetupListScreen.work->delay--;
    }
    switch (WifiSetupScrollBar_GetEvent()) {
    case 0:
        break;
    case 1:
        gWifiSetupListScreen.work->draggingScrollBar = 1;
        WifiSetupButtonBar_Lock();
        break;
    case 2:
        if (gWifiSetupListScreen.work->delay) {
            break;
        }
        WifiSetupCursor_Hide();
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * WifiSetupScrollBar_GetValue(), gWifiSetupListScreen.work->scrollBarRange);
        WifiSetupListScreen_DrawRows();
        gWifiSetupListScreen.work->delay = 4;
        break;
    case 3: {
        int mod;
        gWifiSetupListScreen.work->draggingScrollBar = 0;
        WifiSetupButtonBar_Unlock();
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * WifiSetupScrollBar_GetValue(), gWifiSetupListScreen.work->scrollBarRange);
        WifiSetupSound_Play(19);
        WifiSetupListScreen_DrawRows();
        mod = FX_ModS32(gWifiSetupListScreen.position, 28);
        if (mod == 0) {
            WifiSetupListScreen_ShowCursor();
        } else if (mod < 14) {
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
        } else {
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollDownTask, 0, 120);
        }
        break;
    }
    case 4:
        if (gWifiSetupListScreen.position == 0) {
            if (gWifiSetupListScreen.work->scrollLimitSoundPlayed == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupListScreen.work->scrollLimitSoundPlayed = 1;
            }
        } else {
            WifiSetupSound_Play(19);
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
        }
        break;
    case 6:
        if (gWifiSetupListScreen.work->count <= 4 || gWifiSetupListScreen.position == gWifiSetupListScreen.work->total) {
            if (gWifiSetupListScreen.work->scrollLimitSoundPlayed == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupListScreen.work->scrollLimitSoundPlayed = 1;
            }
        } else {
            WifiSetupSound_Play(19);
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollDownTask, 0, 120);
        }
        break;
    case 5:
    case 7:
        gWifiSetupListScreen.work->scrollLimitSoundPlayed = 0;
        break;
    }
}

static void WifiSetupListScreen_CheckButtons(void) {
    int result, index;
    WifiSetupListWork *s;
    if (gWifiSetupListScreen.work->task || gWifiSetupListScreen.work->draggingScrollBar) {
        return;
    }
    result = WifiSetupButtonBar_GetSelected();
    switch (result) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        if (gWifiSetupListScreen.selected == 4) {
            gWifiSetupListScreen.work->confirmed = 1;
            WifiSetupSound_Play(6);
            WifiSetupListScreen_HighlightRescanButton();
        } else {
            index = FX_DivS32(gWifiSetupListScreen.position, 28) + gWifiSetupListScreen.selected;
            s = gWifiSetupListScreen.work;
            if (s->rows[index].kind == 2) {
                WifiSetupSound_Play(9);
                WifiSetupScrollBar_Disable();
                WifiSetupButtonBar_Lock();
                WifiSetupDialog_Open(14, 1, 1, -1, 0);
                WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitErrorDialog);
                return;
            }
            s->confirmed = 1;
            gWifiSetupListScreen.work->index = index;
            WifiSetupSound_Play(6);
        }
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Exit);
}

static void WifiSetupListScreen_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitFadeOut);
}

static void WifiSetupListScreen_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupListScreen.work->confirmed) {
        WifiSetupButtonBar_Close();
    } else {
        WifiSetupButtonBar_Lock();
    }
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_End);
}

static void WifiSetupListScreen_End(void) {
    int i;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupTask_Destroy(1, gWifiSetupListScreen.work->scrollBgTask);
    for (i = 0; i < 5; i++) {
        if (gWifiSetupListScreen.work->rowOams[i]) {
            WifiSetupOam_Release(gWifiSetupListScreen.work->rowOams[i]);
        }
        if (gWifiSetupListScreen.work->iconOams[i]) {
            WifiSetupOam_Release(gWifiSetupListScreen.work->iconOams[i]);
        }
    }
    WifiSetupText_FreeCanvas(gWifiSetupListScreen.work->buffer);
    WifiSetupScrollBar_Destroy();
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupBgScreen_Destroy();
    WifiSetupFile_Free(gWifiSetupListScreen.work->bgScreenFile);
    WifiSetupFile_Free(gWifiSetupListScreen.work->paletteFile);
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ);
    G2_SetBG0Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    if (!gWifiSetupListScreen.work->confirmed) {
        WifiSetupApScan_FreeWork();
        WifiSetupConnections_Edit(WifiSetupConnections_GetEdit()[244]);
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
    } else if (gWifiSetupListScreen.selected == 4) {
        WifiSetupApScan_FreeWork();
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Init);
    } else {
        WifiSetupConnections_SetSsid(&gWifiSetupListScreen.work->rows[gWifiSetupListScreen.work->index]);
        WifiSetupApp_SetTransition(0, 0);
        if (gWifiSetupListScreen.work->rows[gWifiSetupListScreen.work->index].text[40]) {
            WifiSetupApp_SetTransition(0, 1);
            WifiSetupApp_SetScreenArgs(1, 1);
            WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Init);
        } else {
            WifiSetupApp_SetTransition(0, 1);
            WifiSetupApp_SetScreenArgs(0, 1);
            WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Init);
        }
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F53C);
}

static void WifiSetupListScreen_DrawRows(void) {
    int start = FX_DivS32(gWifiSetupListScreen.position, 28), count = gWifiSetupListScreen.work->count, row, i, second;
    WifiSetupText_Clear(gWifiSetupListScreen.work->buffer, 0);
    if (count > 5) {
        count = 5;
    }
    row = start;
    i = 0;
    if (count > 0) {
        do {
            WifiSetupListScreen_DrawRowText(row, i);
            ++i;
            ++row;
        } while (i < count);
    }
    second = start;
    row = 0;
    if (count > 0) {
        do {
            WifiSetupListScreen_SetRowIcons(second, row);
            ++row;
            ++second;
        } while (row < count);
    }
    WifiSetupText_MarkDirty(gWifiSetupListScreen.work->buffer);
    WifiSetupListScreen_PositionRows();
}

static void WifiSetupListScreen_DrawRowText(int index, int row) {
    int length = WifiSetup_StrNLen(gWifiSetupListScreen.work->rows[index].text, 32), y = row * 28, i, n;
    u16 text[17];
    if (index >= gWifiSetupListScreen.work->count) {
        return;
    }
    if (length <= 16) {
        y += 6;
    }
    MI_CpuFill8(text, 0, 34);
    n = length <= 16 ? length : 16;
    for (i = 0; i < n; i++) {
        text[i] = gWifiSetupListScreen.work->rows[index].text[i];
    }
    WifiSetupText_DrawMonospace(gWifiSetupListScreen.work->buffer, 10, y, 2, 10, text, 1);
    if (length <= 16) {
        return;
    }
    MI_CpuFill8(text, 0, 34);
    n = length - 16;
    for (i = 0; i < n; i++) {
        text[i] = gWifiSetupListScreen.work->rows[index].text[i + 16];
    }
    WifiSetupText_DrawMonospace(gWifiSetupListScreen.work->buffer, 10, y + 12, 2, 10, text, 1);
}

static void WifiSetupListScreen_SetRowIcons(int a, int b) {
    int off;
    GXOamAttr *p;
    if (a >= gWifiSetupListScreen.work->count) {
        return;
    }
    off = a * 42;
    p = gWifiSetupListScreen.work->rowOams[b];
    p->attr2 = (p->attr2 & ~0x3ff) | gWifiSetupListScreen.work->rowKindChars[(gWifiSetupListScreen.work->rowData + off)[40]];
    p = gWifiSetupListScreen.work->iconOams[b];
    p->attr2 = (p->attr2 & ~0x3ff) | gWifiSetupListScreen.work->iconChars[*(u16 *)(gWifiSetupListScreen.work->rowData + off + 38)];
}

static void WifiSetupListScreen_PositionRows(void) {
    int y = 54 - FX_ModS32(gWifiSetupListScreen.position, 28), n = gWifiSetupListScreen.work->count, i;
    if (n > 5) {
        n = 5;
    }
    for (i = 0; i < n; i++, y += 28) {
        GXOamAttr *p = gWifiSetupListScreen.work->rowOams[i];
        p->attr01 = (p->attr01 & 0xfe00ff00) | ((y - 2) & 255) | 0xb30000;
        p = gWifiSetupListScreen.work->iconOams[i];
        p->attr01 = (p->attr01 & 0xfe00ff00) | ((y + 1) & 255) | 0xd20000;
    }
    gWifiSetupListScreen.work->bgScrollDirty = 1;
}

static void WifiSetupListScreen_ScrollUpTask(void *task) {
    int rem;
    WifiSetupScrollBar_Disable();
    WifiSetupCursor_Hide();
    if (gWifiSetupListScreen.position > 4) {
        gWifiSetupListScreen.position -= 4;
    } else {
        gWifiSetupListScreen.position = 0;
    }
    rem = FX_ModS32(gWifiSetupListScreen.position, 28);
    if (rem == 24) {
        WifiSetupListScreen_DrawRows();
        return;
    }
    if (rem > 24) {
        gWifiSetupListScreen.position += 28 - rem;
        rem = 0;
    }
    WifiSetupListScreen_PositionRows();
    if (rem) {
        return;
    }
    WifiSetupScrollBar_SetValue(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->scrollBarRange, gWifiSetupListScreen.work->total));
    WifiSetupScrollBar_Enable();
    WifiSetupListScreen_ShowCursor();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

static void WifiSetupListScreen_ScrollDownTask(void *task) {
    int rem;
    WifiSetupScrollBar_Disable();
    WifiSetupCursor_Hide();
    gWifiSetupListScreen.position += 4;
    rem = FX_ModS32(gWifiSetupListScreen.position, 28);
    if (rem >= 4) {
        WifiSetupListScreen_PositionRows();
        return;
    }
    gWifiSetupListScreen.position -= rem;
    WifiSetupListScreen_DrawRows();
    WifiSetupScrollBar_SetValue(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->scrollBarRange, gWifiSetupListScreen.work->total));
    WifiSetupScrollBar_Enable();
    WifiSetupListScreen_ShowCursor();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

static void WifiSetupListScreen_ScrollBgTask(void) {
    int x;
    if (!gWifiSetupListScreen.work->bgScrollDirty) {
        return;
    }
    x = FX_ModS32(gWifiSetupListScreen.position, 28) - 50;
    G2_SetBG0Offset(0, x);
    G2_SetBG2Offset(0, x);
    gWifiSetupListScreen.work->bgScrollDirty = 0;
}

static void WifiSetupListScreen_ShowCursor(void) {
    unsigned x = gWifiSetupListScreen.selected;
    WifiSetupCursor_ShowPair(x < 4 ? 2 : 3, gWifiSetupListScreenCursorLeft[x * 4], gWifiSetupListScreenCursorRight[x * 4], gWifiSetupListScreenCursorTop[x * 4]);
}

static void WifiSetupListScreen_MoveCursor(int input) {
    int changed = 1;
    int state = gWifiSetupListScreen.selected;
    switch (state) {
    case 0:
        if (input == 1) {
            if (gWifiSetupListScreen.position == 0) {
                gWifiSetupListScreen.selected = 4;
            } else {
                WifiSetupSound_Play(19);
                gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
                return;
            }
        } else {
            if (gWifiSetupListScreen.work->count > 1) {
                gWifiSetupListScreen.selected = state + 1;
            } else {
                changed = 0;
            }
        }
        break;
    case 1:
    case 2:
        if (input == 1) {
            gWifiSetupListScreen.selected = state - 1;
        } else {
            if (gWifiSetupListScreen.work->count > state + 1) {
                gWifiSetupListScreen.selected = state + 1;
            } else {
                changed = 0;
            }
        }
        break;
    case 3:
        if (input == 1) {
            gWifiSetupListScreen.selected = state - 1;
        } else {
            WifiSetupListScreen_StartScrollDown();
            return;
        }
        break;
    case 4:
        if (input == 1) {
            changed = 0;
        } else {
            gWifiSetupListScreen.position = 0;
            gWifiSetupListScreen.selected = 0;
            WifiSetupListScreen_DrawRows();
            WifiSetupScrollBar_SetValue(0);
        }
        break;
    }
    if (!changed) {
        if (gWifiSetupListScreen.work->cursorLimitSoundPlayed) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 1;
        return;
    }
    WifiSetupSound_Play(8);
    WifiSetupListScreen_ShowCursor();
}

static void WifiSetupListScreen_StartScrollUp(void) {
    if (!gWifiSetupListScreen.position) {
        if (gWifiSetupListScreen.work->cursorLimitSoundPlayed) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
}

static void WifiSetupListScreen_StartScrollDown(void) {
    if (gWifiSetupListScreen.position == gWifiSetupListScreen.work->total || gWifiSetupListScreen.work->count <= 4) {
        if (gWifiSetupListScreen.work->cursorLimitSoundPlayed) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->cursorLimitSoundPlayed = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollDownTask, 0, 120);
}

static void WifiSetupListScreen_HighlightRescanButton(void) {
    WifiSetupPaletteRow palette = gWifiSetupListScreenRescanPaletteRow;
    WifiSetupPalette_LoadRow(gWifiSetupListScreen.work->paletteFile, palette.row, palette.row);
}

static void WifiSetupListScreen_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitErrorDialogClosed);
}

static void WifiSetupListScreen_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupScrollBar_Enable();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Main);
}

void WifiSetupTextEntry_Init(void) {
    WifiSetupBytePair p = *(const WifiSetupBytePair *)gWifiSetupTextEntryParams;
    int a, b;
    gWifiSetupTextEntry = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupTextEntry), 4);
    WifiSetupApp_GetScreenArgs(&a, &b);
    if (a == 0) {
        WifiSetupConnections_GetSsid(gWifiSetupTextEntry->text);
        gWifiSetupTextEntry->length = WifiSetup_StrNLen(gWifiSetupTextEntry->text, 32);
    }
    WifiSetupTextEntry_LoadGraphics();
    WifiSetupBackground_LoadHeader(a + 9);
    if (b == 1) {
        WifiSetupMessage_Show(53, -1, 0);
    } else {
        WifiSetupMessage_Show(p.values[a], -1, 0);
    }
    WifiSetupBanner_Show(2);
    gWifiSetupTextEntry->canvas = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupTextEntry->cursor = WifiSetupOamTemplate_AllocOam(0, 62);
    {
        GXOamAttr *q = gWifiSetupTextEntry->cursor;
        q->attr2 = (q->attr2 & ~0xc00) | 0xc00;
    }
    WifiSetupTextEntry_UpdateCursor();
    WifiSetupTextEntry_DrawText();
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_FadeIn);
}

static void WifiSetupTextEntry_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupTextEntryYbObjKbNclPath, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(gWifiSetupTextEntryJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupTextEntryYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupTextEntryXb4EditNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupTextEntry_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitFadeIn);
}

static void WifiSetupTextEntry_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupKeyboard_Create();
    WifiSetupSound_Play(20);
    if (gWifiSetupTextEntry->length == 0) {
        WifiSetupKeyboard_SetCanDelete(0);
    }
    if (gWifiSetupTextEntry->length == 32) {
        WifiSetupKeyboard_SetCanType(0);
    }
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitKeyboard);
}

static void WifiSetupTextEntry_WaitKeyboard(void) {
    if (WifiSetupKeyboard_GetInput() == 255) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Main);
}

static void WifiSetupTextEntry_Main(void) {
    WifiSetupTextEntry_HandleInput();
    WifiSetupTextEntry_Update();
}

static void WifiSetupTextEntry_HandleInput(void) {
    int x = WifiSetupKeyboard_GetInput();
    switch (x) {
    case 0:
        break;
    case 128:
        if (gWifiSetupTextEntry->length == 0) {
            break;
        }
        WifiSetupSound_Play(3);
        gWifiSetupTextEntry->length--;
        gWifiSetupTextEntry->text[gWifiSetupTextEntry->length] = 0;
        if (gWifiSetupTextEntry->length == 0) {
            WifiSetupKeyboard_SetCanDelete(0);
        }
        WifiSetupKeyboard_SetCanType(1);
        break;
    case 130:
        WifiSetupSound_Play(7);
        gWifiSetupTextEntry->flag = 0;
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Exit);
        return;
    case 131:
        if (WifiSetupTextEntry_IsTextValid()) {
            WifiSetupSound_Play(6);
            gWifiSetupTextEntry->flag = 1;
        } else {
            gWifiSetupTextEntry->flag = 2;
            WifiSetupSound_Play(9);
        }
        gWifiSetupTextEntry->cursor->attr01 = (gWifiSetupTextEntry->cursor->attr01 & 0xc1fffcff) | 0x200;
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Exit);
        return;
    case 57373:
    default:
        if (gWifiSetupTextEntry->length == 32) {
            break;
        }
        WifiSetupSound_Play(1);
        gWifiSetupTextEntry->text[gWifiSetupTextEntry->length] = x;
        gWifiSetupTextEntry->length++;
        WifiSetupKeyboard_SetCanDelete(1);
        if (gWifiSetupTextEntry->length == 32) {
            WifiSetupKeyboard_SetCanType(0);
        }
        break;
    }
    WifiSetupTextEntry_DrawText();
    WifiSetupTextEntry_UpdateCursor();
}

static void WifiSetupTextEntry_Update(void) {
}

static void WifiSetupTextEntry_Exit(void) {
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_CloseKeyboard);
}

static void WifiSetupTextEntry_CloseKeyboard(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupKeyboard_Close();
    WifiSetupSound_Play(21);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitKeyboardClosed);
}

static void WifiSetupTextEntry_WaitKeyboardClosed(void) {
    u32 opts[2];
    int x;
    opts[0] = gWifiSetupTextEntryParams[2];
    opts[1] = gWifiSetupTextEntryParams[3];
    if (WifiSetupKeyboard_Exists()) {
        return;
    }
    if (gWifiSetupTextEntry->flag == 0) {
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_End);
        return;
    }
    if (gWifiSetupTextEntry->flag == 2) {
        WifiSetupDialog_Open(6, 3, 1, -1, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitErrorDialog);
        return;
    }
    WifiSetupApp_GetScreenArgs(0, &x);
    WifiSetupDialog_Open(opts[x], 2, 1, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitConfirmDialog);
}

static void WifiSetupTextEntry_End(void) {
    int a, b;
    WifiSetupMessage_Hide();
    WifiSetupOam_Release(gWifiSetupTextEntry->cursor);
    WifiSetupText_FreeBgCanvas(0);
    WifiSetup_LoadGraphics(gWifiSetupTextEntryYbObjMainNclPath, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_GetScreenArgs(&a, &b);
    if (b == 0) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetScreenArgs(0, a);
        WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
    } else if (gWifiSetupTextEntry->flag == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetScreenArgs(1, 0);
        WifiSetupApp_SetStateFunc(WifiSetupListScreen_Init);
    } else {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetScreenArgs(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Init);
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F53C + 4);
}

static void WifiSetupTextEntry_DrawText(void) {
    WifiSetupBox rect = { 0 };
    u16 tile[2];
    int j, i;
    rect.y = 32;
    rect.width = 14;
    rect.height = 16;
    WifiSetupText_Clear(gWifiSetupTextEntry->canvas, 0);
    tile[1] = 0;
    i = 0;
    j = 0;
    for (; i < 32; i++, j++) {
        u8 c;
        if (i == 16) {
            j = 0;
            rect.y = 49;
        }
        c = gWifiSetupTextEntry->text[i];
        if (c == 32) {
            tile[0] = 0xe01d;
        } else {
            tile[0] = c;
        }
        rect.x = gWifiSetupTextEntryColumnX[j];
        WifiSetupText_DrawInRect(gWifiSetupTextEntry->canvas, rect.x, rect.y, rect.width, rect.height, 2, 0x480, tile);
    }
    WifiSetupText_MarkDirty(gWifiSetupTextEntry->canvas);
}

static void WifiSetupTextEntry_UpdateCursor(void) {
    unsigned x = gWifiSetupTextEntry->length;
    GXOamAttr *p = gWifiSetupTextEntry->cursor;
    int col = x & 15, row = (int)x >> 4;
    if (x >= 32) {
        row = 1;
        col = 15;
    }
    p->attr01 = (p->attr01 & 0xfe00ff00) | gWifiSetupTextEntryCursorRowY[row] | ((u32)gWifiSetupTextEntryColumnX[col] << 23 >> 7);
}

static void WifiSetupTextEntry_WaitConfirmDialog(void) {
    gWifiSetupTextEntry->flag = WifiSetupDialog_GetSelected();
    switch (gWifiSetupTextEntry->flag) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(14);
        break;
    default:
        return;
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitConfirmDialogClosed);
}

static void WifiSetupTextEntry_WaitConfirmDialogClosed(void) {
    void (*opts[2])(u8 *);
    int x;
    opts[0] = (void (*)(u8 *))gWifiSetupTextEntryParams[4];
    opts[1] = (void (*)(u8 *))gWifiSetupTextEntryParams[5];
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    if (gWifiSetupTextEntry->flag == 0) {
        gWifiSetupTextEntry->cursor->attr01 &= 0xc1fffcff;
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitFadeIn);
        return;
    }
    WifiSetupApp_GetScreenArgs(&x, 0);
    opts[x](gWifiSetupTextEntry->text);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_End);
}

static int WifiSetupTextEntry_IsTextValid(void) {
    int a, b, len;
    WifiSetupApp_GetScreenArgs(&a, &b);
    if (b == 1) {
        WifiSetupMessage_Show(0x35, -1, 0);
    }
    WifiSetupApp_GetScreenArgs(&a, &b);
    if (a == 0) {
        return gWifiSetupTextEntry->text[0] != 0;
    }
    if (b == 1 && gWifiSetupTextEntry->text[0] == 0) {
        return 0;
    }
    len = WifiSetup_StrNLen(gWifiSetupTextEntry->text, 32);
    switch (len) {
    case 0:
    case 5:
    case 13:
    case 16:
        return 1;
    case 10:
    case 26:
    case 32: {
        int i;
        for (i = 0; i < len; i++) {
            unsigned int c = gWifiSetupTextEntry->text[i];
            if (c >= 48 && c <= 57) {
                continue;
            }
            if (c >= 65 && c <= 70) {
                continue;
            }
            if (c >= 97 && c <= 102) {
                continue;
            }
            return 0;
        }
    }
        return 1;
    default:
        return 0;
    }
}

static void WifiSetupTextEntry_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitErrorDialogClosed);
}

static void WifiSetupTextEntry_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    gWifiSetupTextEntry->cursor->attr01 &= 0xc1fffcff;
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitFadeIn);
}
