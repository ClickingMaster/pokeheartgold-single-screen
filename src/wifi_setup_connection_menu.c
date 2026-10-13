#pragma force_active on
#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_address_entry.h"
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

// Access-point setup (ARM code in overlay 13): the screen of the three connection slots (edit,
// erase, or the Wi-Fi USB connector), the menu of the four setup methods, the connection test
// (prompt, test, "connection succeeded"), and the screens before and after the Wi-Fi USB connector
// screen (see wifi_setup_usb_connector_screen.c).

// Copies of path strings (the path is localised in place).
typedef struct WifiSetupPath22 {
    char text[22];
} WifiSetupPath22;

typedef struct WifiSetupPaletteRows {
    u8 rows[4];
} WifiSetupPaletteRows;

// dialog message per connection test mode
typedef struct WifiSetupTestPromptMessages {
    u32 messages[3];
} WifiSetupTestPromptMessages;

// parameters of the connection test (DWC_AC_Create)
typedef struct WifiSetupConnectionTestParams {
    u8 unk_00[10];
    u8 slot; // 1-3, 4 for the Wi-Fi USB connector
    u8 unk_0B;
} WifiSetupConnectionTestParams;

extern u8 ov13_02245600[];                    // slot icon template per setup method (3 = empty)
extern WifiSetupPaletteRows ov13_02245604[2]; // palette row per slot, icon palette per setup method
extern WifiSetupPoint ov13_02245622[];        // slot icons 0-2, erase buttons 3-5
extern WifiSetupRect ov13_0224563A[];         // touch rects of the 7 selections
extern WifiSetupRect ov13_02245672[];         // cursor rects of the 7 selections
extern s8 ov13_022456AC[];                    // digit position of the erase dialog per language
extern WifiSetupPaletteRows ov13_022456B4;    // palette row per setup method
extern WifiSetupPath22 ov13_022456B8;
extern s8 ov13_022456CE[][4][4];         // next method per method and direction, -1 none, -2 last
extern WifiSetupRect ov13_022456EE[][4]; // touch rects of the methods
extern WifiSetupRect ov13_0224572E[][4]; // cursor rects of the methods
extern s8 ov13_02245770[];               // digit position of the method menu message per language
extern WifiSetupConnectionTestParams ov13_02245778;
extern WifiSetupTestPromptMessages ov13_02245784;
extern char ov13_02246664[];
extern char ov13_0224667C[];
extern char ov13_02246694[];
extern char ov13_022466AC[];
extern char ov13_022466C0[];
extern char ov13_022466D8[];
extern char ov13_022466F0[];
extern char ov13_02246704[];
extern char ov13_02246718[];
extern char ov13_0224672C[];
extern char ov13_02246740[];
extern char ov13_02246758[];
extern char ov13_02246770[];
extern char ov13_02246784[];
extern char ov13_0224679C[];
extern char ov13_022467B4[];

extern BOOL DWC_AC_Create(WifiSetupConnectionTestParams *params);
extern int DWC_AC_Process(void);
extern int DWC_AC_GetStatus(void);
extern BOOL DWC_AC_Destroy(void);
extern void DWCi_AC_SetUserProfile(int slot, WifiSetupConnection *connection);
extern u8 *DWCi_BACKUPlGetWifi(void);

void WifiSetupConnectionList_CreateIcons(void) {
    int k;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        k = WifiSetupConnections_GetSetupMethod(i);
        if (k == WIFI_SETUP_METHOD_ERASED) {
            k = 3;
        } else {
            gWifiSetupConnectionList.work->eraseButtons[i] = WifiSetupOamTemplate_AllocBlock(0, 0x11, 1);
            j = i + 3;
            WifiSetupOamBlock_SetPosition(gWifiSetupConnectionList.work->eraseButtons[i], -1, ov13_02245622[j].x, ov13_02245622[j].y);
            WifiSetupOamBlock_SetPriority(gWifiSetupConnectionList.work->eraseButtons[i], -1, 3);
        }
        gWifiSetupConnectionList.work->slotIcons[i] = WifiSetupOamTemplate_AllocBlock(0, ov13_02245600[k], 1);
        WifiSetupOamBlock_SetPosition(gWifiSetupConnectionList.work->slotIcons[i], -1, ov13_02245622[i].x, ov13_02245622[i].y);
        WifiSetupOamBlock_SetPriority(gWifiSetupConnectionList.work->slotIcons[i], -1, 3);
    }
}

void WifiSetupConnectionList_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_WaitFadeIn);
}

void WifiSetupConnectionList_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_WaitButtonBar);
}

void WifiSetupConnectionList_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    if (WifiSetupBanner_IsMoving() == 1) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Main);
}

void WifiSetupConnectionList_Main(void) {
    WifiSetupConnectionList_HandleInput();
    WifiSetupConnectionList_Update();
    WifiSetupConnectionList_CheckButtons();
}

void WifiSetupConnectionList_HandleInput(void) {
    u32 i;
    for (i = 0; i < 7; i++) {
        if (WifiSetupInput_IsTouchPressed(&ov13_0224563A[i])) {
            WifiSetupButtonBar_SelectIfNone(1);
            gWifiSetupConnectionList.selected = i;
            WifiSetupConnectionList_ShowCursor();
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupConnectionList_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupConnectionList_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT)) {
        WifiSetupConnectionList_MoveCursor(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT)) {
        WifiSetupConnectionList_MoveCursor(2);
    }
}

void WifiSetupConnectionList_Update(void) {
}

void WifiSetupConnectionList_CheckButtons(void) {
    u8 sel;
    int idx;
    int k;
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        switch (WifiSetupApp_GetKind()) {
        case 0:
            WifiSetupSound_Play(7);
            gWifiSetupConnectionList.work->exit = WIFI_SETUP_CONNECTION_LIST_BACK;
            break;
        case 1:
            WifiSetupButtonBar_Lock();
            WifiSetupApp_SetStateFunc(WifiSetupScreens_StartQuit);
            return;
        }
        break;
    case 1:
        gWifiSetupConnectionList.work->exit = WIFI_SETUP_CONNECTION_LIST_NEXT;
        sel = gWifiSetupConnectionList.selected;
        if (sel >= WIFI_SETUP_CONNECTION_LIST_FIRST_ERASE) {
            idx = sel - WIFI_SETUP_CONNECTION_LIST_FIRST_ERASE;
            if (WifiSetupConnections_GetSetupMethod(idx) == WIFI_SETUP_METHOD_ERASED) {
                WifiSetupSound_Play(9);
                WifiSetupButtonBar_SetSelected(-1);
                return;
            }
            WifiSetupSound_Play(6);
            k = ov13_022456AC[WifiSetupApp_GetLanguage()];
            WifiSetupDialog_Open(0x46, 0, 1, k, idx + 1);
            WifiSetupConnectionList_PressEraseButton();
            WifiSetupButtonBar_Lock();
            WifiSetupApp_SetStateFunc(WifiSetupConnectionList_WaitEraseDialog);
            return;
        }
        if (sel <= 2) {
            WifiSetupConnections_Edit(sel);
        }
        WifiSetupSound_Play(6);
        WifiSetupConnectionList_HighlightSelection();
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Exit);
}

void WifiSetupConnectionList_Exit(void) {
    if (gWifiSetupConnectionList.work->exit == WIFI_SETUP_CONNECTION_LIST_BACK) {
        WifiSetupBanner_Close();
    }
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_WaitFadeOut);
}

void WifiSetupConnectionList_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupConnectionList.work->exit == WIFI_SETUP_CONNECTION_LIST_NEXT) {
        if (gWifiSetupConnectionList.selected == WIFI_SETUP_CONNECTION_LIST_USB || WifiSetupConnections_GetEdit()->setupMethod != 0xFF) {
            WifiSetupButtonBar_Close();
        }
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_End);
}

void WifiSetupConnectionList_End(void) {
    u32 i;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!WifiSetupBanner_IsNotClosing()) {
        return;
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (gWifiSetupConnectionList.work->slotIcons[i] != NULL) {
            WifiSetupOamBlock_Free(gWifiSetupConnectionList.work->slotIcons[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (gWifiSetupConnectionList.work->eraseButtons[i] != NULL) {
            WifiSetupOamBlock_Free(gWifiSetupConnectionList.work->eraseButtons[i]);
        }
    }
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(gWifiSetupConnectionList.work->graphics);
    WifiSetup_LoadGraphics(ov13_02246664, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    if (gWifiSetupConnectionList.work->exit == WIFI_SETUP_CONNECTION_LIST_BACK) {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Init);
    } else {
        switch (gWifiSetupConnectionList.selected) {
        case 0:
        case 1:
        case 2:
            WifiSetupApp_SetTransition(2, 0);
            if (WifiSetupConnections_GetEdit()->setupMethod == WIFI_SETUP_METHOD_ERASED) {
                WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
            } else {
                WifiSetupApp_SetScreenArgs(0, 0);
                WifiSetupSettings_SetFromMenu(0);
                WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
            }
            break;
        case 3:
            WifiSetupApp_SetTransition(2, 1);
            WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_Init);
            break;
        }
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F570);
}

void WifiSetupConnectionList_ShowCursor(void) {
    u8 sel = gWifiSetupConnectionList.selected;
    if (sel < 4) {
        WifiSetupCursor_ShowCorners(ov13_02245672[sel].left, ov13_02245672[sel].right, ov13_02245672[sel].top, ov13_02245672[sel].bottom);
    } else {
        WifiSetupCursor_ShowPair(0, ov13_02245672[sel].left, ov13_02245672[sel].right, ov13_02245672[sel].top);
    }
}

void WifiSetupConnectionList_MoveCursor(int dir) {
    BOOL changed = TRUE;
    u8 c;
    int c4;
    switch (gWifiSetupConnectionList.selected) {
    case 0:
        gWifiSetupConnectionList.work->column = 0;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 2;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 1;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 3;
        } else {
            gWifiSetupConnectionList.selected = 4;
        }
        break;
    case 1:
        gWifiSetupConnectionList.work->column = 1;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 0;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 2;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 3;
        } else {
            gWifiSetupConnectionList.selected = 5;
        }
        break;
    case 2:
        gWifiSetupConnectionList.work->column = 2;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 1;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 0;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 3;
        } else {
            gWifiSetupConnectionList.selected = 6;
        }
        break;
    case 3:
        c = gWifiSetupConnectionList.work->column;
        c4 = c + 4;
        if (dir == 1) {
            gWifiSetupConnectionList.selected = c4;
        } else if (dir == 3) {
            gWifiSetupConnectionList.selected = c;
        } else {
            changed = FALSE;
        }
        break;
    case 4:
        gWifiSetupConnectionList.work->column = 0;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 6;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 5;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 0;
        } else {
            gWifiSetupConnectionList.selected = 3;
        }
        break;
    case 5:
        gWifiSetupConnectionList.work->column = 1;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 4;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 6;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 1;
        } else {
            gWifiSetupConnectionList.selected = 3;
        }
        break;
    case 6:
        gWifiSetupConnectionList.work->column = 2;
        if (dir == 0) {
            gWifiSetupConnectionList.selected = 5;
        } else if (dir == 2) {
            gWifiSetupConnectionList.selected = 4;
        } else if (dir == 1) {
            gWifiSetupConnectionList.selected = 2;
        } else {
            gWifiSetupConnectionList.selected = 3;
        }
        break;
    }
    if (changed) {
        WifiSetupSound_Play(8);
        WifiSetupConnectionList_ShowCursor();
    }
}

void WifiSetupConnectionList_PressEraseButton(void) {
    int idx = gWifiSetupConnectionList.selected - WIFI_SETUP_CONNECTION_LIST_FIRST_ERASE;
    WifiSetupOamTemplate_Copy(0, 0x32, WifiSetupOamBlock_GetOam(gWifiSetupConnectionList.work->eraseButtons[idx], 0));
    WifiSetupOamBlock_SetPosition(gWifiSetupConnectionList.work->eraseButtons[idx], -1, ov13_02245622[idx + 3].x, ov13_02245622[idx + 3].y);
    WifiSetupOamBlock_SetPriority(gWifiSetupConnectionList.work->eraseButtons[idx], -1, 3);
}

void WifiSetupConnectionList_WaitEraseDialog(void) {
    int idx = gWifiSetupConnectionList.selected - WIFI_SETUP_CONNECTION_LIST_FIRST_ERASE;
    switch (WifiSetupDialog_GetSelected()) {
    case 1:
        WifiSetupSound_Play(0xE);
        WifiSetupConnections_Erase(idx);
        WifiSetupOamTemplate_Copy(0, 0x56, WifiSetupOamBlock_GetOam(gWifiSetupConnectionList.work->slotIcons[idx], 0));
        WifiSetupOamBlock_SetPosition(gWifiSetupConnectionList.work->slotIcons[idx], -1, ov13_02245622[idx].x, ov13_02245622[idx].y);
        WifiSetupOamBlock_SetPriority(gWifiSetupConnectionList.work->slotIcons[idx], -1, 3);
        WifiSetupOamBlock_Free(gWifiSetupConnectionList.work->eraseButtons[idx]);
        gWifiSetupConnectionList.work->eraseButtons[idx] = NULL;
        break;
    case 0:
        WifiSetupSound_Play(7);
        WifiSetupOamTemplate_Copy(0, 0x11, WifiSetupOamBlock_GetOam(gWifiSetupConnectionList.work->eraseButtons[idx], 0));
        WifiSetupOamBlock_SetPosition(gWifiSetupConnectionList.work->eraseButtons[idx], -1, ov13_02245622[idx + 3].x, ov13_02245622[idx + 3].y);
        WifiSetupOamBlock_SetPriority(gWifiSetupConnectionList.work->eraseButtons[idx], -1, 3);
        break;
    default:
        return;
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_WaitEraseDialogClosed);
}

void WifiSetupConnectionList_WaitEraseDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Main);
}

void WifiSetupConnectionList_HighlightSelection(void) {
    WifiSetupPaletteRows rows = ov13_02245604[0];
    WifiSetupPaletteRows palettes = ov13_02245604[1];
    int k;
    if (gWifiSetupConnectionList.selected > 3) {
        return;
    }
    WifiSetupPalette_LoadRow(gWifiSetupConnectionList.work->graphics, rows.rows[gWifiSetupConnectionList.selected], rows.rows[gWifiSetupConnectionList.selected]);
    if (gWifiSetupConnectionList.selected == 3) {
        return;
    }
    k = WifiSetupConnections_GetSetupMethod(gWifiSetupConnectionList.selected);
    if (k > 2) {
        k = 3;
    }
    WifiSetupOamBlock_SetModeAndPalette(gWifiSetupConnectionList.work->slotIcons[gWifiSetupConnectionList.selected], -1, 0, palettes.rows[k]);
}

void WifiSetupMethodMenu_Init(void) {
    WifiSetupConnection *connection = WifiSetupConnections_GetEdit();
    int k;
    gWifiSetupMethodMenu.confirmed = 0;
    if (gWifiSetupMethodMenu.lastOption == 0) {
        gWifiSetupMethodMenu.lastOption = 1;
    }
    if (WifiSetupApp_GetLanguage()) {
        if (gWifiSetupMethodMenu.selected == 2) {
            gWifiSetupMethodMenu.selected = 0;
        }
        if (gWifiSetupMethodMenu.lastOption == 2) {
            gWifiSetupMethodMenu.lastOption = 1;
        }
    }
    WifiSetupMethodMenu_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    k = WifiSetupApp_GetLanguage();
    WifiSetupMessage_Show(0x32, ov13_02245770[k], connection->index + 1);
    WifiSetupBanner_Show(1);
    WifiSetupCursor_ShowCorners(ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].left, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].right, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].top, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].bottom);
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_FadeIn);
}

void WifiSetupMethodMenu_LoadGraphics(void) {
    WifiSetupPath22 path = ov13_022456B8;
    WifiSetup_LoadGraphics(ov13_0224667C, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246694, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022466AC, GX_LoadBG2Scr);
    gWifiSetupMethodMenu.graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(path.text), NULL, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupMethodMenu_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_WaitFadeIn);
}

void WifiSetupMethodMenu_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_WaitButtonBar);
}

void WifiSetupMethodMenu_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Main);
}

void WifiSetupMethodMenu_Main(void) {
    WifiSetupMethodMenu_HandleInput();
    WifiSetupMethodMenu_Update();
    WifiSetupMethodMenu_CheckButtons();
}

void WifiSetupMethodMenu_HandleInput(void) {
    u32 i;
    for (i = 0; i < 4; i++) {
        if (WifiSetupInput_IsTouchPressed(&ov13_022456EE[WifiSetupApp_HasOption(1)][i])) {
            WifiSetupButtonBar_SelectIfNone(1);
            gWifiSetupMethodMenu.selected = i;
            WifiSetupCursor_ShowCorners(ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].left, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].right, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].top, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].bottom);
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupMethodMenu_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupMethodMenu_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT)) {
        WifiSetupMethodMenu_MoveCursor(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT)) {
        WifiSetupMethodMenu_MoveCursor(2);
    }
}

void WifiSetupMethodMenu_Update(void) {
}

void WifiSetupMethodMenu_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(6);
        WifiSetupMethodMenu_HighlightSelection();
        gWifiSetupMethodMenu.confirmed = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Exit);
}

void WifiSetupMethodMenu_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_WaitFadeOut);
}

void WifiSetupMethodMenu_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupMethodMenu.confirmed) {
        WifiSetupButtonBar_Close();
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_End);
}

void WifiSetupMethodMenu_End(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(gWifiSetupMethodMenu.graphics);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    if (gWifiSetupMethodMenu.confirmed == 0) {
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
        return;
    }
    switch (gWifiSetupMethodMenu.selected) {
    case 0:
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupApScanScreen_Init);
        break;
    case 1:
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupAoss_Init);
        break;
    case 2:
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupAutoSearch_Init);
        break;
    case 3:
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetScreenArgs(0, 0);
        WifiSetupSettings_SetFromMenu(1);
        WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
        break;
    }
}

void WifiSetupMethodMenu_MoveCursor(int dir) {
    s8 cur = gWifiSetupMethodMenu.selected;
    s8 next = ov13_022456CE[WifiSetupApp_HasOption(1)][cur][dir];
    if (next == -1) {
        return;
    }
    if (next == 0) {
        gWifiSetupMethodMenu.lastOption = cur;
    }
    if (next == -2) {
        gWifiSetupMethodMenu.selected = gWifiSetupMethodMenu.lastOption;
    } else {
        gWifiSetupMethodMenu.selected = next;
    }
    WifiSetupSound_Play(8);
    WifiSetupCursor_ShowCorners(ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].left, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].right, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].top, ov13_0224572E[WifiSetupApp_HasOption(1)][gWifiSetupMethodMenu.selected].bottom);
}

void WifiSetupMethodMenu_HighlightSelection(void) {
    WifiSetupPaletteRows rows = ov13_022456B4;
    WifiSetupPalette_LoadRow(gWifiSetupMethodMenu.graphics, rows.rows[gWifiSetupMethodMenu.selected], rows.rows[gWifiSetupMethodMenu.selected]);
}

void WifiSetupConnectionTest_Init(void) {
    int mode;
    gWifiSetupConnectionTestPassed = 0;
    WifiSetupConnectionTest_LoadGraphics();
    WifiSetupApp_GetScreenArgs(0, &mode);
    if (mode == 0) {
        WifiSetupMessage_Show(0x31, -1, 0);
    }
    WifiSetupBanner_Show(2);
    if (mode == 0) {
        WifiSetupBackground_LoadSlotHeader();
    }
    WifiSetup_PrintMessage(0x2C);
    WifiSetupBusyIcon_Create(0);
    WifiSetupConnectionTest_Start();
    WifiSetupSignalIcon_Create(0);
    WifiSetupSound_Play(0xC);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_FadeIn);
}

void WifiSetupConnectionTest_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022466C0, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_022466D8, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022466F0, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupConnectionTest_Start(void) {
    int mode;
    WifiSetupConnectionTestParams param;
    WifiSetupConnection *connection = WifiSetupConnections_GetEdit();
    MIi_CpuCopy32((const void *)&ov13_02245778, (void *)&param, sizeof(param));
    WifiSetupApp_GetScreenArgs(0, &mode);
    if (mode == 2) {
        param.slot = 4;
    } else {
        param.slot = connection->index + 1;
    }
    if (!DWC_AC_Create(&param)) {
        OS_Terminate();
    }
    if (mode == 0) {
        DWCi_AC_SetUserProfile(connection->index, connection);
    }
    WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupConnectionTest_PollTask, NULL, 120);
}

void WifiSetupConnectionTest_FadeIn(void) {
    int mode;
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_GetScreenArgs(0, &mode);
    if (mode == 0) {
        WifiSetupFade_Start(2, 1, 1, 8);
        WifiSetupDisplay_ShowPlanes(1, 1);
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_WaitFadeIn);
}

void WifiSetupConnectionTest_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_Main);
}

void WifiSetupConnectionTest_Main(void) {
    WifiSetupConnectionTest_HandleInput();
    WifiSetupConnectionTest_Update();
    WifiSetupConnectionTest_CheckButtons();
}

void WifiSetupConnectionTest_HandleInput(void) {
}

void WifiSetupConnectionTest_Update(void) {
}

void WifiSetupConnectionTest_CheckButtons(void) {
}

void WifiSetupConnectionTest_FadeOut(void) {
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_WaitFadeOut);
}

void WifiSetupConnectionTest_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_End);
}

void WifiSetupConnectionTest_End(void) {
    WifiSetupConnections *connections = WifiSetupConnections_Get();
    if (!DWC_AC_Destroy()) {
        return;
    }
    MI_CpuCopy8(DWCi_BACKUPlGetWifi(), connections->pages[0].tail.page.info, 0xE);
    MI_CpuCopy8(DWCi_BACKUPlGetWifi(), connections->pages[1].tail.page.info, 0xE);
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (gWifiSetupConnectionTestPassed == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_Init);
    }
}

void WifiSetupConnectionTest_PollTask(void *task) {
    int r = DWC_AC_Process();
    if (r == 0) {
        return;
    }
    WifiSetupSignalIcon_Destroy();
    WifiSetupSound_Stop();
    if (r > 0) {
        gWifiSetupConnectionTestPassed = 1;
        WifiSetupSound_Play(0x11);
    } else {
        WifiSetupErrorScreen_SetCode(DWC_AC_GetStatus());
        WifiSetupSound_Play(0x12);
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_FadeOut);
    WifiSetupTask_DestroyLater(0, task);
}

void *WifiSetupConnectionTest_Alloc(void *arg, u32 size) {
    return WifiSetupHeap_AllocZeroed(size, 0x20);
}

void WifiSetupConnectionTest_Free(void *arg, void *ptr) {
    WifiSetupHeap_Free(ptr);
}

void WifiSetupConnectionOk_Init(void) {
    gWifiSetupConnectionOkTimer = 0;
    WifiSetupConnectionOk_LoadGraphics();
    WifiSetup_PrintMessage(0x2D);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_FadeIn);
}

void WifiSetupConnectionOk_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246704, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupConnectionOk_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_WaitFadeIn);
}

void WifiSetupConnectionOk_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_Main);
}

void WifiSetupConnectionOk_Main(void) {
    WifiSetupConnectionOk_UpdateTimer();
    WifiSetupConnectionOk_Update();
}

void WifiSetupConnectionOk_Update(void) {
}

void WifiSetupConnectionOk_FadeOut(void) {
    int mode;
    WifiSetupApp_GetScreenArgs(0, &mode);
    if (mode != 0) {
        WifiSetupBanner_Close();
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_End);
}

void WifiSetupConnectionOk_End(void) {
    int mode;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!WifiSetupBanner_IsNotClosing()) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(2, 1);
    WifiSetupApp_GetScreenArgs(0, &mode);
    if (mode == 0) {
        WifiSetupApp_SetScreenArgs(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Init);
}

void WifiSetupConnectionOk_UpdateTimer(void) {
    int mode;
    if (++gWifiSetupConnectionOkTimer < 0xB4) {
        return;
    }
    WifiSetupApp_GetScreenArgs(0, &mode);
    switch (WifiSetupApp_GetKind()) {
    case 0:
        break;
    case 1:
        if (mode != 0) {
            WifiSetupApp_SetStateFunc(WifiSetupScreens_StartQuit);
            return;
        }
        break;
    }
    WifiSetupApp_SetStateFunc(WifiSetupConnectionOk_FadeOut);
}

void WifiSetupTestPrompt_Init(void) {
    int mode;
    WifiSetupApScan_FreeWork();
    WifiSetupApp_GetScreenArgs(0, &mode);
    WifiSetupTestPrompt_LoadGraphics();
    WifiSetupMessage_Show(0x31, -1, 0);
    if (mode != 2) {
        WifiSetupBackground_LoadSlotHeader();
    }
    if (mode == 1) {
        WifiSetupConnections_SaveEdit();
    }
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_FadeIn);
}

void WifiSetupTestPrompt_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246718, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupTestPrompt_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_WaitFadeIn);
}

void WifiSetupTestPrompt_WaitFadeIn(void) {
    WifiSetupTestPromptMessages prompts = ov13_02245784;
    int mode;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_GetScreenArgs(0, &mode);
    WifiSetupDialog_Open(prompts.messages[mode], 1, 1, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Main);
}

void WifiSetupTestPrompt_Main(void) {
    WifiSetupTestPrompt_HandleInput();
    WifiSetupTestPrompt_Update();
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_WaitDialogClosed);
}

void WifiSetupTestPrompt_HandleInput(void) {
}

void WifiSetupTestPrompt_Update(void) {
}

void WifiSetupTestPrompt_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_End);
}

void WifiSetupTestPrompt_End(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupDisplay_HidePlanes(0, 0x14);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_Init);
}

void WifiSetupUsbConnectorDone_Init(void) {
    gWifiSetupUsbConnectorDoneTimer = 0;
    WifiSetupUsbConnectorDone_LoadGraphics();
    WifiSetupBackground_LoadHeader(8);
    WifiSetup_PrintMessage(0x29);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_FadeIn);
}

void WifiSetupUsbConnectorDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_0224672C, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupUsbConnectorDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_WaitFadeIn);
}

void WifiSetupUsbConnectorDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_Main);
}

void WifiSetupUsbConnectorDone_Main(void) {
    WifiSetupUsbConnectorDone_UpdateTimer();
    WifiSetupUsbConnectorDone_Update();
}

void WifiSetupUsbConnectorDone_Update(void) {
}

void WifiSetupUsbConnectorDone_FadeOut(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_End);
}

void WifiSetupUsbConnectorDone_End(void) {
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
    WifiSetupApp_SetScreenArgs(0, 2);
    WifiSetupApp_SetStateFunc(WifiSetupTestPrompt_Init);
}

void WifiSetupUsbConnectorDone_UpdateTimer(void) {
    if (++gWifiSetupUsbConnectorDoneTimer < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_FadeOut);
}

void WifiSetupUsbConnectorIntro_Init(void) {
    gWifiSetupUsbConnectorIntroNext = 0;
    WifiSetupUsbConnectorIntro_LoadGraphics();
    WifiSetupBackground_LoadHeader(8);
    WifiSetupMessage_Show(0x38, -1, 0);
    WifiSetupBanner_Show(1);
    WifiSetup_PrintMessage(0x27);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_FadeIn);
}

void WifiSetupUsbConnectorIntro_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246740, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246758, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246770, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void WifiSetupUsbConnectorIntro_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_WaitFadeIn);
}

void WifiSetupUsbConnectorIntro_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupButtonBar_Show(6);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_WaitButtonBar);
}

void WifiSetupUsbConnectorIntro_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_Main);
}

void WifiSetupUsbConnectorIntro_Main(void) {
    WifiSetupUsbConnectorIntro_HandleInput();
    WifiSetupUsbConnectorIntro_Update();
    WifiSetupUsbConnectorIntro_CheckButtons();
}

void WifiSetupUsbConnectorIntro_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupUsbConnectorIntro_Update(void) {
}

void WifiSetupUsbConnectorIntro_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(6);
        gWifiSetupUsbConnectorIntroNext = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_Exit);
}

void WifiSetupUsbConnectorIntro_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_WaitFadeOut);
}

void WifiSetupUsbConnectorIntro_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupUsbConnectorIntroNext == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_End);
}

void WifiSetupUsbConnectorIntro_End(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (gWifiSetupUsbConnectorIntroNext == 0 && WifiSetupFade_IsActive(1)) {
        return;
    }
    if (!WifiSetupButtonBar_IsNotClosing()) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupUsbConnectorIntroNext == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (gWifiSetupUsbConnectorIntroNext == 0) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
    } else {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Init);
    }
}

BOOL WifiSetupUsbConnector_IsLidClosed(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void WifiSetupUsbConnector_Init(void) {
    u16 name[11];
    OSOwnerInfo info;
    gWifiSetupUsbConnectorExit = 0;
    WifiSetupUsbConnector_LoadGraphics();
    WifiSetupBackground_LoadHeader(8);
    WifiSetupBanner_Show(2);
    OS_GetOwnerInfo(&info);
    MIi_CpuClear16(0, name, sizeof(name));
    MIi_CpuCopy16(info.nickName, name, info.nickNameLength * 2);
    WifiSetup_PrintMessageWithText(name, 0x28);
    WifiSetupBusyIcon_Create(0);
    WifiSetupUsbScan_Start(WifiSetupUsbConnector_ScanCallback);
    WifiSetupSound_Play(0xB);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Start);
}

void WifiSetupUsbConnector_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246784, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_0224679C, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022467B4, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}
