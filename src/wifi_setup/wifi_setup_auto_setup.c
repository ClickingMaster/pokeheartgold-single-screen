#include "global.h"

#include "dwc/dwc_bm_internal.h"
#include "wifi_setup/wifi_setup_access_points.h"
#include "wifi_setup/wifi_setup_app.h"
#include "wifi_setup/wifi_setup_connection_screens.h"
#include "wifi_setup/wifi_setup_local_wireless.h"
#include "wifi_setup/wifi_setup_screens.h"
#include "wifi_setup/wifi_setup_scroll_bar.h"
#include "wifi_setup/wifi_setup_settings.h"
#include "wifi_setup/wifi_setup_ui.h"
#include "wifi_setup/wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point setup: the rest of the connection settings screen (cursor,
// scrolling, switch buttons, completeness check), the "settings saved"
// screen, the four screens of the automatic setup with the setup client (WifiSetupClient_*):
// search -> connect -> receive the settings -> done, and the access-point scan screen.
// Ends with the start of the connection slot screen.

// rect index in gWifiSetupSettingsCursorPairRects per cursor item
typedef struct WifiSetupSettingsCursorRects {
    u8 rects[14];
} WifiSetupSettingsCursorRects;

extern u8 gWifiSetupSettingsButtonPaletteRows[]; // palette row per button item 11-13
extern u8 gWifiSetupSettingsRowItems[];          // item per list row
extern u8 gWifiSetupSettingsScrollOffsets[];     // BG 2 offset per first visible item
extern u8 gWifiSetupSettingsItemRows[];          // list row per item
extern WifiSetupRect gWifiSetupSettingsCursorPairRects[];
extern WifiSetupSettingsCursorRects gWifiSetupSettingsCursorRects;
extern WifiSetupPath22 gWifiSetupConnectionListYbBgStep11NclPath;
extern char gWifiSetupSaveScreenJbBgStep3NcgPath[];
extern char gWifiSetupSaveScreenYbBgStep3NclPath[];
extern char gWifiSetupSaveScreenXb4NoneNscPath[];
extern char gWifiSetupAutoConnectXb4MultiNscPath[];
extern char gWifiSetupAutoReceiveXb4MultiNscPath[];
extern char gWifiSetupAutoDoneXb4MultiNscPath[];
extern char gWifiSetupAutoSearchJbBgStep3NcgPath[];
extern char gWifiSetupAutoSearchYbBgStep3NclPath[];
extern char gWifiSetupAutoSearchXb4MultiNscPath[];
extern char gWifiSetupApScanScreenJbBgStep3NcgPath[];
extern char gWifiSetupApScanScreenYbBgStep3NclPath[];
extern char gWifiSetupApScanScreenXb4MultiNscPath[];
extern char gWifiSetupConnectionListYbObjWayNclPath[];
extern char gWifiSetupConnectionListJbBgStep1NcgPath[];
extern char gWifiSetupConnectionListJbBgStep1NclPath[];
extern char gWifiSetupConnectionListJb2ApNscPath[];

extern s32 FX_DivS32(s32 n, s32 d);

static void WifiSetupSettings_WaitIncompleteDialogClosed(void);
static void WifiSetupSaveScreen_LoadGraphics(void);
static void WifiSetupSaveScreen_FadeIn(void);
static void WifiSetupSaveScreen_WaitFadeIn(void);
static void WifiSetupSaveScreen_Main(void);
static void WifiSetupSaveScreen_HandleInput(void);
static void WifiSetupSaveScreen_Update(void);
static void WifiSetupSaveScreen_FadeOut(void);
static void WifiSetupSaveScreen_End(void);
static int WifiSetupAutoConnect_IsLidClosed(void);
static void WifiSetupAutoConnect_Init(void);
static void WifiSetupAutoConnect_LoadGraphics(void);
static void WifiSetupAutoConnect_FadeIn(void);
static void WifiSetupAutoConnect_WaitFadeIn(void);
static void WifiSetupAutoConnect_WaitButtonBar(void);
static void WifiSetupAutoConnect_Main(void);
static void WifiSetupAutoConnect_HandleInput(void);
static void WifiSetupAutoConnect_Update(void);
static void WifiSetupAutoConnect_CheckButtons(void);
static void WifiSetupAutoConnect_Exit(void);
static void WifiSetupAutoConnect_WaitFadeOut(void);
static void WifiSetupAutoConnect_End(void);
static void WifiSetupAutoConnect_CheckClient(void);
static void WifiSetupAutoConnect_WaitErrorDialog(void);
static void WifiSetupAutoConnect_WaitErrorDialogClosed(void);
static int WifiSetupAutoReceive_IsLidClosed(void);
static void WifiSetupAutoReceive_Init(void);
static void WifiSetupAutoReceive_LoadGraphics(void);
static void WifiSetupAutoReceive_FadeIn(void);
static void WifiSetupAutoReceive_WaitFadeIn(void);
static void WifiSetupAutoReceive_WaitButtonBar(void);
static void WifiSetupAutoReceive_Main(void);
static void WifiSetupAutoReceive_HandleInput(void);
static void WifiSetupAutoReceive_Update(void);
static void WifiSetupAutoReceive_CheckButtons(void);
static void WifiSetupAutoReceive_Exit(void);
static void WifiSetupAutoReceive_WaitFadeOut(void);
static void WifiSetupAutoReceive_End(void);
static void WifiSetupAutoReceive_CheckClient(void);
static void WifiSetupAutoReceive_WaitErrorDialog(void);
static void WifiSetupAutoReceive_WaitErrorDialogClosed(void);
static void WifiSetupAutoDone_Init(void);
static void WifiSetupAutoDone_LoadGraphics(void);
static void WifiSetupAutoDone_FadeIn(void);
static void WifiSetupAutoDone_WaitFadeIn(void);
static void WifiSetupAutoDone_Main(void);
static void WifiSetupAutoDone_Update(void);
static void WifiSetupAutoDone_FadeOut(void);
static void WifiSetupAutoDone_End(void);
static void WifiSetupAutoDone_UpdateTimer(void);
static int WifiSetupAutoSearch_IsLidClosed(void);
static void WifiSetupAutoSearch_LoadGraphics(void);
static void WifiSetupAutoSearch_FadeIn(void);
static void WifiSetupAutoSearch_WaitFadeIn(void);
static void WifiSetupAutoSearch_WaitButtonBar(void);
static void WifiSetupAutoSearch_Main(void);
static void WifiSetupAutoSearch_HandleInput(void);
static void WifiSetupAutoSearch_Update(void);
static void WifiSetupAutoSearch_CheckButtons(void);
static void WifiSetupAutoSearch_Exit(void);
static void WifiSetupAutoSearch_WaitFadeOut(void);
static void WifiSetupAutoSearch_End(void);
static void WifiSetupAutoSearch_CheckClient(void);
static void WifiSetupAutoSearch_WaitErrorDialog(void);
static void WifiSetupAutoSearch_WaitErrorDialogClosed(void);
static void WifiSetupApScanScreen_LoadGraphics(void);
static void WifiSetupApScanScreen_FadeIn(void);
static void WifiSetupApScanScreen_WaitFadeIn(void);
static void WifiSetupApScanScreen_WaitButtonBar(void);
static void WifiSetupApScanScreen_Main(void);
static void WifiSetupApScanScreen_UpdateScan(void);
static void WifiSetupApScanScreen_HandleInput(void);
static void WifiSetupApScanScreen_Update(void);
static void WifiSetupApScanScreen_CheckButtons(void);
static void WifiSetupApScanScreen_Exit(void);
static void WifiSetupApScanScreen_WaitFadeOut(void);
static void WifiSetupApScanScreen_End(void);
static void WifiSetupApScanScreen_WaitDialog(void);
static void WifiSetupApScanScreen_WaitDialogClosed(void);
static void WifiSetupConnectionList_LoadGraphics(void);

int WifiSetupSettings_GetItemRow(int item) {
    int d = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    int i;
    for (i = 0; i < 4; i++, d++) {
        if (d == item) {
            return i;
        }
    }
    return -1;
}

int WifiSetupSettings_GetRowItem(int row) {
    int d = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    return d + row;
}

void WifiSetupSettings_ScrollUpTask(void *task) {
    int m;
    WifiSetupScrollBar_Disable();
    WifiSetupCursor_Hide();
    if (gWifiSetupSettings.scroll > 6) {
        gWifiSetupSettings.scroll -= 6;
    } else {
        gWifiSetupSettings.scroll = 0;
    }
    m = FX_ModS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    if (m == 0x17) {
        WifiSetupSettings_DrawRows();
        return;
    }
    if (m > 0x17) {
        gWifiSetupSettings.scroll += WIFI_SETUP_SETTINGS_ROW_HEIGHT - m;
        m = 0;
    }
    WifiSetupSettings_UpdateObjects();
    if (m != 0) {
        return;
    }
    WifiSetupScrollBar_SetValue(gWifiSetupSettings.scroll * 0x37 / WIFI_SETUP_SETTINGS_MAX_SCROLL);
    WifiSetupScrollBar_Enable();
    WifiSetupSettings_UpdateCursorItem();
    gWifiSetupSettings.work->scrollTask = NULL;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupSettings_ScrollDownTask(void *task) {
    int m;
    WifiSetupScrollBar_Disable();
    WifiSetupCursor_Hide();
    gWifiSetupSettings.scroll += 6;
    m = FX_ModS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    if (m >= 6) {
        WifiSetupSettings_UpdateObjects();
        return;
    }
    gWifiSetupSettings.scroll -= m;
    WifiSetupSettings_DrawRows();
    WifiSetupScrollBar_SetValue(gWifiSetupSettings.scroll * 0x37 / WIFI_SETUP_SETTINGS_MAX_SCROLL);
    WifiSetupScrollBar_Enable();
    WifiSetupSettings_UpdateCursorItem();
    gWifiSetupSettings.work->scrollTask = NULL;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupSettings_ScrollTask(void) {
    int d;
    int m;
    if (gWifiSetupSettings.work->objectsDirty == 0) {
        return;
    }
    d = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    m = FX_ModS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT) - 0x33;
    d = gWifiSetupSettingsScrollOffsets[d];
    G2_SetBG0Offset(0, m);
    G2_SetBG2Offset(0, m + d);
    gWifiSetupSettings.work->objectsDirty = 0;
}

void WifiSetupSettings_ShowCursor(void) {
    WifiSetupRect rect;
    WifiSetupSettingsCursorRects rects = gWifiSetupSettingsCursorRects;
    int idx = rects.rects[gWifiSetupSettings.cursorItem];
    if (idx >= 3) {
        WifiSetupCursor_ShowPair(3, gWifiSetupSettingsCursorPairRects[idx].left, gWifiSetupSettingsCursorPairRects[idx].right, gWifiSetupSettingsCursorPairRects[idx].top);
        return;
    }
    rect = gWifiSetupSettingsCursorPairRects[idx];
    rect.top += gWifiSetupSettings.cursorRow * WIFI_SETUP_SETTINGS_ROW_HEIGHT;
    WifiSetupCursor_ShowPair(1, rect.left, rect.right, rect.top);
}

void WifiSetupSettings_UpdateCursorItem(void) {
    WifiSetupConnection *save;
    int n;
    u8 item = gWifiSetupSettings.cursorItem;
    if ((u8)(item + 245) <= 2) { // the buttons 11-13
        WifiSetupSettings_ShowCursor();
        return;
    }
    save = WifiSetupConnections_GetEdit();
    n = gWifiSetupSettings.cursorRow + FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    switch (n) {
    case 2:
        if (save->autoAddress) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON;
        } else {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF;
        }
        break;
    case 6:
        if (save->autoDns) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON;
        } else {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF;
        }
        break;
    default:
        gWifiSetupSettings.cursorItem = gWifiSetupSettingsRowItems[n];
        break;
    }
    WifiSetupSettings_ShowCursor();
}

void WifiSetupSettings_SetCursorItem(u8 item) {
    gWifiSetupSettings.cursorItem = item;
    gWifiSetupSettings.cursorRow = WifiSetupSettings_GetItemRow(gWifiSetupSettingsItemRows[item]);
    WifiSetupSettings_ShowCursor();
}

void WifiSetupSettings_MoveCursor(int dir) {
    WifiSetupConnection *save = WifiSetupConnections_GetEdit();
    int ret = 0;
    u8 item = gWifiSetupSettings.cursorItem;
    if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF && save->autoAddress == 0 && (dir == WIFI_SETUP_CURSOR_LEFT || dir == WIFI_SETUP_CURSOR_RIGHT)) {
        return;
    }
    switch (item) {
    case 0:
        if (dir == WIFI_SETUP_CURSOR_UP) {
            gWifiSetupSettings.cursorItem = 11;
        } else if (dir == WIFI_SETUP_CURSOR_DOWN) {
            gWifiSetupSettings.cursorRow++;
        } else {
            ret = 2;
        }
        break;
    case 10:
        if (dir == WIFI_SETUP_CURSOR_UP) {
            gWifiSetupSettings.cursorRow--;
        } else if (dir == WIFI_SETUP_CURSOR_DOWN) {
            gWifiSetupSettings.cursorItem = gWifiSetupSettings.work->lastButton;
        } else {
            ret = 2;
        }
        break;
    case 11:
        if (dir == WIFI_SETUP_CURSOR_UP) {
            if (gWifiSetupSettings.work->keyAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->keyAtEnd = 1;
            return;
        } else if (dir == WIFI_SETUP_CURSOR_DOWN) {
            gWifiSetupSettings.cursorItem = 0;
            gWifiSetupSettings.cursorRow = 0;
            gWifiSetupSettings.scroll = 0;
            WifiSetupSettings_DrawRows();
            WifiSetupScrollBar_SetValue(0);
        } else {
            ret = 2;
        }
        break;
    case 12:
    case 13:
        gWifiSetupSettings.work->lastButton = item;
        if (dir == WIFI_SETUP_CURSOR_UP) {
            gWifiSetupSettings.cursorItem = 10;
            gWifiSetupSettings.cursorRow = 3;
            gWifiSetupSettings.scroll = WIFI_SETUP_SETTINGS_MAX_SCROLL;
            WifiSetupSettings_DrawRows();
            WifiSetupScrollBar_SetValue(0x37);
        } else if (dir == WIFI_SETUP_CURSOR_DOWN) {
            if (gWifiSetupSettings.work->keyAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->keyAtEnd = 1;
            return;
        } else if (item == 12) {
            gWifiSetupSettings.cursorItem = 13;
        } else {
            gWifiSetupSettings.cursorItem = 12;
        }
        break;
    default:
        if (dir == WIFI_SETUP_CURSOR_UP) {
            if (gWifiSetupSettings.cursorRow != 0) {
                gWifiSetupSettings.cursorRow--;
                break;
            }
            WifiSetupSound_Play(0x13);
            gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollUpTask, NULL, 120);
            return;
        }
        if (dir == WIFI_SETUP_CURSOR_DOWN) {
            if (gWifiSetupSettings.cursorRow < 3) {
                gWifiSetupSettings.cursorRow++;
                break;
            }
            WifiSetupSound_Play(0x13);
            gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollDownTask, NULL, 120);
            return;
        }
        ret = 2;
        if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF;
        } else if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON;
        } else if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF;
        } else if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF) {
            gWifiSetupSettings.cursorItem = WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON;
        } else {
            break;
        }
        WifiSetupSound_Play(8);
        WifiSetupSettings_ShowCursor();
        break;
    }
    if (ret == 2) {
        return;
    }
    WifiSetupSound_Play(8);
    if (ret != 0) {
        return;
    }
    WifiSetupSettings_UpdateCursorItem();
}

void WifiSetupSettings_HighlightButton(void) {
    WifiSetupPalette_LoadRow(gWifiSetupSettings.work->charData, gWifiSetupSettingsButtonPaletteRows[gWifiSetupSettings.cursorItem - 11], gWifiSetupSettingsButtonPaletteRows[gWifiSetupSettings.cursorItem - 11]);
}

void WifiSetupSettings_ButtonTask(void) {
    int i;
    for (i = 0; i < 4; i++) {
        if (gWifiSetupSettings.work->buttonStates[i] != 0) {
            gWifiSetupSettings.work->buttonStates[i]--;
            if (gWifiSetupSettings.work->buttonStates[i] == 0) {
                WifiSetupSettings_DrawRows();
            }
        }
    }
}

void WifiSetupSettings_PressButton(void) {
    WifiSetupSettingsSwitchItems switches = gWifiSetupSettingsSwitchItems;
    int i;
    for (i = 0; i < 4; i++) {
        if (gWifiSetupSettings.cursorItem == switches.items[i]) {
            gWifiSetupSettings.work->buttonStates[i] = 0x14;
            if (i & 1) {
                gWifiSetupSettings.work->buttonStates[i - 1] = 0;
                return;
            }
            gWifiSetupSettings.work->buttonStates[i + 1] = 0;
            return;
        }
    }
}

BOOL WifiSetupSettings_IsComplete(void) {
    WifiSetupConnection *save = WifiSetupConnections_GetEdit();
    if (save->ssid[0] == 0) {
        return FALSE;
    }
    if (save->autoDns == 0 && !DWC_BACKUPlCheckAddress(save->dns1) && !DWC_BACKUPlCheckAddress(save->dns2)) {
        return FALSE;
    }
    if (save->autoAddress == 0) {
        if (!DWC_BACKUPlCheckAddress(save->ip)) {
            return FALSE;
        }
        if (!DWC_BACKUPlCheckAddress(save->gateway)) {
            return FALSE;
        }
        if (!DWC_BACKUPlCheckIp(save->ip, save->mask)) {
            return FALSE;
        }
    }
    return TRUE;
}

void WifiSetupSettings_WaitIncompleteDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupPalette_Load(gWifiSetupSettings.work->paletteData);
    WifiSetupApp_SetStateFunc(WifiSetupSettings_WaitIncompleteDialogClosed);
}

static void WifiSetupSettings_WaitIncompleteDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupSettings_Main);
}

void WifiSetupSettings_SetFromMenu(u8 fromMenu) {
    gWifiSetupSettings.fromMenu = fromMenu;
}

void WifiSetupSaveScreen_Init(void) {
    WifiSetupSaveScreen_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupConnections_SaveEdit();
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_FadeIn);
}

static void WifiSetupSaveScreen_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenXb4NoneNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupSaveScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_WaitFadeIn);
}

static void WifiSetupSaveScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupDialog_Open(0x44, 5, 1, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_Main);
}

static void WifiSetupSaveScreen_Main(void) {
    WifiSetupSaveScreen_HandleInput();
    WifiSetupSaveScreen_Update();
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_FadeOut);
}

static void WifiSetupSaveScreen_HandleInput(void) {
}

static void WifiSetupSaveScreen_Update(void) {
}

static void WifiSetupSaveScreen_FadeOut(void) {
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_End);
}

static void WifiSetupSaveScreen_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetTransition(2, 1);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
}

static int WifiSetupAutoConnect_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

static void WifiSetupAutoConnect_Init(void) {
    gWifiSetupAutoConnectDone = 0;
    WifiSetupAutoConnect_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x25);
    WifiSetupBusyIcon_Create(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_FadeIn);
}

static void WifiSetupAutoConnect_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoConnectXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAutoConnect_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitFadeIn);
}

static void WifiSetupAutoConnect_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitButtonBar);
}

static void WifiSetupAutoConnect_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Main);
}

static void WifiSetupAutoConnect_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoConnect_CheckClient();
    WifiSetupAutoConnect_HandleInput();
    WifiSetupAutoConnect_Update();
    WifiSetupAutoConnect_CheckButtons();
}

static void WifiSetupAutoConnect_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoConnect_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupAutoConnect_Update(void) {
}

static void WifiSetupAutoConnect_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Exit);
}

static void WifiSetupAutoConnect_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitFadeOut);
}

static void WifiSetupAutoConnect_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupButtonBar_Close();
    }
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_End);
}

static void WifiSetupAutoConnect_End(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (gWifiSetupAutoConnectDone == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupClient_Stop();
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Init);
}

static void WifiSetupAutoConnect_CheckClient(void) {
    switch (WifiSetupClient_GetResult()) {
    case 2:
        gWifiSetupAutoConnectDone = 1;
        WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Exit);
        break;
    case 4:
        gWifiSetupAutoConnectDone = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(9);
        WifiSetupDialog_Open(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitErrorDialog);
        break;
    }
}

static void WifiSetupAutoConnect_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitErrorDialogClosed);
}

static void WifiSetupAutoConnect_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Exit);
}

static int WifiSetupAutoReceive_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

static void WifiSetupAutoReceive_Init(void) {
    gWifiSetupAutoReceiveDone = 0;
    WifiSetupAutoReceive_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x2A);
    WifiSetupBusyIcon_Create(2);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_FadeIn);
}

static void WifiSetupAutoReceive_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoReceiveXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAutoReceive_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitFadeIn);
}

static void WifiSetupAutoReceive_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitButtonBar);
}

static void WifiSetupAutoReceive_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Main);
}

static void WifiSetupAutoReceive_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoReceive_CheckClient();
    WifiSetupAutoReceive_HandleInput();
    WifiSetupAutoReceive_Update();
    WifiSetupAutoReceive_CheckButtons();
}

static void WifiSetupAutoReceive_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoReceive_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupAutoReceive_Update(void) {
}

static void WifiSetupAutoReceive_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Exit);
}

static void WifiSetupAutoReceive_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitFadeOut);
}

static void WifiSetupAutoReceive_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupAutoReceiveDone == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_End);
}

static void WifiSetupAutoReceive_End(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (gWifiSetupAutoReceiveDone == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupAutoReceiveDone == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupClient_Stop();
    if (gWifiSetupAutoReceiveDone == 0) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_Init);
}

static void WifiSetupAutoReceive_CheckClient(void) {
    switch (WifiSetupClient_GetResult()) {
    case 3:
        gWifiSetupAutoReceiveDone = 1;
        WifiSetupSound_Stop();
        WifiSetupClient_StoreResult();
        WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Exit);
        break;
    case 4:
        gWifiSetupAutoReceiveDone = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(9);
        WifiSetupDialog_Open(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitErrorDialog);
        break;
    case 5:
        gWifiSetupAutoReceiveDone = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
        WifiSetupDialog_Open(0xE, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitErrorDialog);
        break;
    }
}

static void WifiSetupAutoReceive_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitErrorDialogClosed);
}

static void WifiSetupAutoReceive_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Exit);
}

static void WifiSetupAutoDone_Init(void) {
    gWifiSetupAutoDoneTimer = 0;
    WifiSetupAutoDone_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x26);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_FadeIn);
}

static void WifiSetupAutoDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoDoneXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAutoDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_WaitFadeIn);
}

static void WifiSetupAutoDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_Main);
}

static void WifiSetupAutoDone_Main(void) {
    WifiSetupAutoDone_UpdateTimer();
    WifiSetupAutoDone_Update();
}

static void WifiSetupAutoDone_Update(void) {
}

static void WifiSetupAutoDone_FadeOut(void) {
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_End);
}

static void WifiSetupAutoDone_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
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

static void WifiSetupAutoDone_UpdateTimer(void) {
    if (++gWifiSetupAutoDoneTimer < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_FadeOut);
}

static int WifiSetupAutoSearch_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void WifiSetupAutoSearch_Init(void) {
    gWifiSetupAutoSearchDone = 0;
    WifiSetupAutoSearch_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupMessage_Show(0x37, -1, 0);
    WifiSetupBanner_Show(2);
    WifiSetup_PrintMessage(0x24);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSound_Play(0xB);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_FadeIn);
}

static void WifiSetupAutoSearch_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupAutoSearch_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitFadeIn);
}

static void WifiSetupAutoSearch_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupClient_Start();
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitButtonBar);
}

static void WifiSetupAutoSearch_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Main);
}

static void WifiSetupAutoSearch_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoSearch_CheckClient();
    WifiSetupAutoSearch_HandleInput();
    WifiSetupAutoSearch_Update();
    WifiSetupAutoSearch_CheckButtons();
}

static void WifiSetupAutoSearch_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoSearch_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupAutoSearch_Update(void) {
}

static void WifiSetupAutoSearch_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Exit);
}

static void WifiSetupAutoSearch_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitFadeOut);
}

static void WifiSetupAutoSearch_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupButtonBar_Close();
    }
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_End);
}

static void WifiSetupAutoSearch_End(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (gWifiSetupAutoSearchDone == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupClient_Stop();
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Init);
}

static void WifiSetupAutoSearch_CheckClient(void) {
    switch (WifiSetupClient_GetResult()) {
    case 1:
        gWifiSetupAutoSearchDone = 1;
        WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Exit);
        break;
    case 4:
        gWifiSetupAutoSearchDone = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(9);
        WifiSetupDialog_Open(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitErrorDialog);
        break;
    }
}

static void WifiSetupAutoSearch_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitErrorDialogClosed);
}

static void WifiSetupAutoSearch_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Exit);
}

void WifiSetupApScanScreen_Init(void) {
    gWifiSetupApScanScreen.timer = 0;
    gWifiSetupApScanScreen.result = WIFI_SETUP_AP_SCAN_RUNNING;
    WifiSetupApScanScreen_LoadGraphics();
    WifiSetupMessage_Show(0x33, -1, 0);
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupBanner_Show(2);
    WifiSetup_PrintMessage(0x33);
    WifiSetupBusyIcon_Create(0);
    WifiSetupApScan_AllocWork();
    WifiSetupApScan_Start();
    WifiSetupSound_Play(0xA);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_FadeIn);
}

static void WifiSetupApScanScreen_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

static void WifiSetupApScanScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitFadeIn);
}

static void WifiSetupApScanScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitButtonBar);
}

static void WifiSetupApScanScreen_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Main);
}

static void WifiSetupApScanScreen_Main(void) {
    WifiSetupApScanScreen_HandleInput();
    WifiSetupApScanScreen_Update();
    WifiSetupApScanScreen_CheckButtons();
    WifiSetupApScanScreen_UpdateScan();
}

static void WifiSetupApScanScreen_UpdateScan(void) {
    WifiSetupScanEntry *list;
    int n;
    int i;

    gWifiSetupApScanScreen.timer++;
    if (gWifiSetupApScanScreen.timer < 300) {
        return;
    }
    WifiSetupSound_Stop();
    n = WifiSetupApScan_GetEntries(&list);
    if (n == 0) {
        gWifiSetupApScanScreen.result = WIFI_SETUP_AP_SCAN_NONE;
        WifiSetupDialog_Open(0xF, 1, 1, -1, 0);
        WifiSetupSound_Play(0x12);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitDialog);
        return;
    }
    for (i = 0; i < n; i++) {
        if (list[i].security != 2) {
            break;
        }
    }
    if (i == n) {
        gWifiSetupApScanScreen.result = WIFI_SETUP_AP_SCAN_ONLY_WPA;
        WifiSetupDialog_Open(0xE, 1, 1, -1, 0);
        WifiSetupSound_Play(0x12);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitDialog);
        return;
    }
    gWifiSetupApScanScreen.result = WIFI_SETUP_AP_SCAN_FOUND;
    WifiSetupSound_Play(0xF);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Exit);
}

static void WifiSetupApScanScreen_HandleInput(void) {
    if (!WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupButtonBar_SelectIfNone(0);
}

static void WifiSetupApScanScreen_Update(void) {
}

static void WifiSetupApScanScreen_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Exit);
}

static void WifiSetupApScanScreen_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitFadeOut);
}

static void WifiSetupApScanScreen_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_End);
}

static void WifiSetupApScanScreen_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    while (!WifiSetupApScan_End()) {
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupApScanScreen.result != WIFI_SETUP_AP_SCAN_FOUND) {
        WifiSetupApScan_FreeWork();
        WifiSetupConnections_Edit(WifiSetupConnections_GetEdit()->index);
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetScreenArgs(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Init);
}

static void WifiSetupApScanScreen_WaitDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitDialogClosed);
}

static void WifiSetupApScanScreen_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Exit);
}

void WifiSetupConnectionList_Init(void) {
    WifiSetupConnectionListWork *work = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupConnectionListWork), 4);
    gWifiSetupConnectionList.work = work;
    work->exit = WIFI_SETUP_CONNECTION_LIST_STAY;
    WifiSetupConnectionList_LoadGraphics();
    WifiSetupBackground_LoadHeader(1);
    WifiSetupMessage_Show(0x2F, -1, 0);
    WifiSetupBanner_Show(0);
    WifiSetupConnectionList_CreateIcons();
    WifiSetupConnectionList_ShowCursor();
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_FadeIn);
}

static void WifiSetupConnectionList_LoadGraphics(void) {
    WifiSetupPath22 path = gWifiSetupConnectionListYbBgStep11NclPath;
    WifiSetup_LoadGraphics(gWifiSetupConnectionListYbObjWayNclPath, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJbBgStep1NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJbBgStep1NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJb2ApNscPath, GX_LoadBG2Scr);
    gWifiSetupConnectionList.work->graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(path.text), NULL, 4);
    WifiSetupDisplay_ShowPlanes(1, GX_PLANEMASK_OBJ);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}
