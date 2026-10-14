#pragma force_active on
#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_app.h"
#include "wifi_setup_connection_screens.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_screens.h"
#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_settings.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point setup (ARM code in overlay 13): the rest of the connection settings screen (cursor,
// scrolling, switch buttons, completeness check, see wifi_setup_settings.c), the "settings saved"
// screen, the four screens of the automatic setup with the setup client (WifiSetupClient_*):
// search -> connect -> receive the settings -> done, and the access-point scan screen.
// Ends with the start of the connection slot screen (see wifi_setup_connection_menu.c).

// Copies of path strings (the path is localised in place).
typedef struct WifiSetupPath22 {
    char text[22];
} WifiSetupPath22;

// rect index in gWifiSetupSettingsCursorPairRects per cursor item
typedef struct WifiSetupSettingsCursorRects {
    u8 rects[14];
} WifiSetupSettingsCursorRects;

extern u8 gWifiSetupSettingsButtonPaletteRows[]; // palette row per button item 11-13
extern u8 gWifiSetupSettingsRowItems[]; // item per list row
extern u8 gWifiSetupSettingsScrollOffsets[]; // BG 2 offset per first visible item
extern u8 gWifiSetupSettingsItemRows[]; // list row per item
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
extern BOOL DWC_BACKUPlCheckAddress(const u8 *address);
extern BOOL DWC_BACKUPlCheckIp(const u8 *ip, const u8 *mask);

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
    if (item == WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF && save->autoAddress == 0 && (dir == 0 || dir == 2)) {
        return;
    }
    switch (item) {
    case 0:
        if (dir == 1) {
            gWifiSetupSettings.cursorItem = 11;
        } else if (dir == 3) {
            gWifiSetupSettings.cursorRow++;
        } else {
            ret = 2;
        }
        break;
    case 10:
        if (dir == 1) {
            gWifiSetupSettings.cursorRow--;
        } else if (dir == 3) {
            gWifiSetupSettings.cursorItem = gWifiSetupSettings.work->lastButton;
        } else {
            ret = 2;
        }
        break;
    case 11:
        if (dir == 1) {
            if (gWifiSetupSettings.work->keyAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->keyAtEnd = 1;
            return;
        } else if (dir == 3) {
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
        if (dir == 1) {
            gWifiSetupSettings.cursorItem = 10;
            gWifiSetupSettings.cursorRow = 3;
            gWifiSetupSettings.scroll = WIFI_SETUP_SETTINGS_MAX_SCROLL;
            WifiSetupSettings_DrawRows();
            WifiSetupScrollBar_SetValue(0x37);
        } else if (dir == 3) {
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
        if (dir == 1) {
            if (gWifiSetupSettings.cursorRow != 0) {
                gWifiSetupSettings.cursorRow--;
                break;
            }
            WifiSetupSound_Play(0x13);
            gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollUpTask, NULL, 120);
            return;
        }
        if (dir == 3) {
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
    WifiSetupSettingsSwitchItems switches = ov13_022454FC;
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

void WifiSetupSettings_WaitIncompleteDialogClosed(void) {
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

void WifiSetupSaveScreen_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupSaveScreenXb4NoneNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupSaveScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_WaitFadeIn);
}

void WifiSetupSaveScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupDialog_Open(0x44, 5, 1, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_Main);
}

void WifiSetupSaveScreen_Main(void) {
    WifiSetupSaveScreen_HandleInput();
    WifiSetupSaveScreen_Update();
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_FadeOut);
}

void WifiSetupSaveScreen_HandleInput(void) {
}

void WifiSetupSaveScreen_Update(void) {
}

void WifiSetupSaveScreen_FadeOut(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_End);
}

void WifiSetupSaveScreen_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    WifiSetupApp_SetTransition(2, 1);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
}

int WifiSetupAutoConnect_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void WifiSetupAutoConnect_Init(void) {
    gWifiSetupAutoConnectDone = 0;
    WifiSetupAutoConnect_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x25);
    WifiSetupBusyIcon_Create(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_FadeIn);
}

void WifiSetupAutoConnect_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoConnectXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupAutoConnect_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitFadeIn);
}

void WifiSetupAutoConnect_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitButtonBar);
}

void WifiSetupAutoConnect_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Main);
}

void WifiSetupAutoConnect_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoConnect_CheckClient();
    WifiSetupAutoConnect_HandleInput();
    WifiSetupAutoConnect_Update();
    WifiSetupAutoConnect_CheckButtons();
}

void WifiSetupAutoConnect_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoConnect_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupAutoConnect_Update(void) {
}

void WifiSetupAutoConnect_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Exit);
}

void WifiSetupAutoConnect_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitFadeOut);
}

void WifiSetupAutoConnect_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupButtonBar_Close();
    }
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_End);
}

void WifiSetupAutoConnect_End(void) {
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
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (gWifiSetupAutoConnectDone == 0) {
        WifiSetupClient_Stop();
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Init);
}

void WifiSetupAutoConnect_CheckClient(void) {
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

void WifiSetupAutoConnect_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_WaitErrorDialogClosed);
}

void WifiSetupAutoConnect_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Exit);
}

int WifiSetupAutoReceive_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void WifiSetupAutoReceive_Init(void) {
    gWifiSetupAutoReceiveDone = 0;
    WifiSetupAutoReceive_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x2A);
    WifiSetupBusyIcon_Create(2);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_FadeIn);
}

void WifiSetupAutoReceive_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoReceiveXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupAutoReceive_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitFadeIn);
}

void WifiSetupAutoReceive_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitButtonBar);
}

void WifiSetupAutoReceive_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Main);
}

void WifiSetupAutoReceive_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoReceive_CheckClient();
    WifiSetupAutoReceive_HandleInput();
    WifiSetupAutoReceive_Update();
    WifiSetupAutoReceive_CheckButtons();
}

void WifiSetupAutoReceive_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoReceive_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupAutoReceive_Update(void) {
}

void WifiSetupAutoReceive_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Exit);
}

void WifiSetupAutoReceive_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitFadeOut);
}

void WifiSetupAutoReceive_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupAutoReceiveDone == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_End);
}

void WifiSetupAutoReceive_End(void) {
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
        WifiSetupDisplay_HidePlanes(1, 1);
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

void WifiSetupAutoReceive_CheckClient(void) {
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

void WifiSetupAutoReceive_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_WaitErrorDialogClosed);
}

void WifiSetupAutoReceive_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoReceive_Exit);
}

void WifiSetupAutoDone_Init(void) {
    gWifiSetupAutoDoneTimer = 0;
    WifiSetupAutoDone_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x26);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_FadeIn);
}

void WifiSetupAutoDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoDoneXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupAutoDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_WaitFadeIn);
}

void WifiSetupAutoDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_Main);
}

void WifiSetupAutoDone_Main(void) {
    WifiSetupAutoDone_UpdateTimer();
    WifiSetupAutoDone_Update();
}

void WifiSetupAutoDone_Update(void) {
}

void WifiSetupAutoDone_FadeOut(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_End);
}

void WifiSetupAutoDone_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetScreenArgs(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Init);
}

void WifiSetupAutoDone_UpdateTimer(void) {
    if (++gWifiSetupAutoDoneTimer < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAutoDone_FadeOut);
}

int WifiSetupAutoSearch_IsLidClosed(void) {
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

void WifiSetupAutoSearch_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupAutoSearchXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupAutoSearch_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitFadeIn);
}

void WifiSetupAutoSearch_WaitFadeIn(void) {
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

void WifiSetupAutoSearch_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Main);
}

void WifiSetupAutoSearch_Main(void) {
    OS_Sleep(10);
    WifiSetupAutoSearch_CheckClient();
    WifiSetupAutoSearch_HandleInput();
    WifiSetupAutoSearch_Update();
    WifiSetupAutoSearch_CheckButtons();
}

void WifiSetupAutoSearch_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAutoSearch_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupAutoSearch_Update(void) {
}

void WifiSetupAutoSearch_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Exit);
}

void WifiSetupAutoSearch_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitFadeOut);
}

void WifiSetupAutoSearch_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupButtonBar_Close();
    }
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_End);
}

void WifiSetupAutoSearch_End(void) {
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
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (gWifiSetupAutoSearchDone == 0) {
        WifiSetupClient_Stop();
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        return;
    }
    WifiSetupApp_SetTransition(0, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAutoConnect_Init);
}

void WifiSetupAutoSearch_CheckClient(void) {
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

void WifiSetupAutoSearch_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_WaitErrorDialogClosed);
}

void WifiSetupAutoSearch_WaitErrorDialogClosed(void) {
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

void WifiSetupApScanScreen_LoadGraphics(void) {
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenJbBgStep3NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenYbBgStep3NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupApScanScreenXb4MultiNscPath, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupApScanScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitFadeIn);
}

void WifiSetupApScanScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitButtonBar);
}

void WifiSetupApScanScreen_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Main);
}

void WifiSetupApScanScreen_Main(void) {
    WifiSetupApScanScreen_HandleInput();
    WifiSetupApScanScreen_Update();
    WifiSetupApScanScreen_CheckButtons();
    WifiSetupApScanScreen_UpdateScan();
}

void WifiSetupApScanScreen_UpdateScan(void) {
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

void WifiSetupApScanScreen_HandleInput(void) {
    if (!WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupButtonBar_SelectIfNone(0);
}

void WifiSetupApScanScreen_Update(void) {
}

void WifiSetupApScanScreen_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Exit);
}

void WifiSetupApScanScreen_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitFadeOut);
}

void WifiSetupApScanScreen_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_End);
}

void WifiSetupApScanScreen_End(void) {
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
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
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

void WifiSetupApScanScreen_WaitDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_WaitDialogClosed);
}

void WifiSetupApScanScreen_WaitDialogClosed(void) {
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

void WifiSetupConnectionList_LoadGraphics(void) {
    WifiSetupPath22 path = gWifiSetupConnectionListYbBgStep11NclPath;
    WifiSetup_LoadGraphics(gWifiSetupConnectionListYbObjWayNclPath, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJbBgStep1NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJbBgStep1NclPath, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupConnectionListJb2ApNscPath, GX_LoadBG2Scr);
    gWifiSetupConnectionList.work->graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(path.text), NULL, 4);
    WifiSetupDisplay_ShowPlanes(1, 0x10);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}
