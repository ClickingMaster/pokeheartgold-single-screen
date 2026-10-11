#pragma force_active on
#include "global.h"

#include "wifi_setup_app.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Wi-Fi USB connector screen: waits for the USB connector scan (WifiSetupUsbScan) and reacts to its
// result with dialogs; the exit code picks the next screen. Followed by the creation of the BG 2
// screen copy (see wifi_setup_scroll_bar.c).

// gWifiSetupUsbConnectorExit
#define USB_CONNECTOR_RUNNING 0
#define USB_CONNECTOR_BACK    1

extern u8 gWifiSetupUsbConnectorExit;

extern BOOL WifiSetupUsbConnector_IsLidClosed(void);
extern void WifiSetupConnectionList_Init(void);
extern void WifiSetupUsbConnectorIntro_Init(void);
extern void WifiSetupUsbConnectorDone_Init(void);

void WifiSetupUsbConnector_Start(void);
void WifiSetupUsbConnector_WaitFadeIn(void);
void WifiSetupUsbConnector_WaitButtonBar(void);
void WifiSetupUsbConnector_Main(void);
void WifiSetupUsbConnector_HandleInput(void);
void WifiSetupUsbConnector_Update(void);
void WifiSetupUsbConnector_CheckButtons(void);
void WifiSetupUsbConnector_Exit(void);
void WifiSetupUsbConnector_WaitFadeOut(void);
void WifiSetupUsbConnector_End(void);
void WifiSetupUsbConnector_ScanCallback(int result);
void WifiSetupUsbConnector_WaitNotFound(void);
void WifiSetupUsbConnector_WaitNotFoundClosed(void);
void WifiSetupUsbConnector_WaitChoice(void);
void WifiSetupUsbConnector_WaitChoiceClosed(void);

void WifiSetupUsbConnector_Start(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitFadeIn);
}

void WifiSetupUsbConnector_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(2);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitButtonBar);
}

void WifiSetupUsbConnector_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Main);
}

void WifiSetupUsbConnector_Main(void) {
    WifiSetupUsbConnector_HandleInput();
    WifiSetupUsbConnector_Update();
    WifiSetupUsbConnector_CheckButtons();
}

void WifiSetupUsbConnector_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupUsbConnector_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupUsbConnector_Update(void) {
}

void WifiSetupUsbConnector_CheckButtons(void) {
    if (gWifiSetupUsbConnectorExit != USB_CONNECTOR_RUNNING) {
        return;
    }
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    gWifiSetupUsbConnectorExit = 2;
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
}

void WifiSetupUsbConnector_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitFadeOut);
}

void WifiSetupUsbConnector_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupUsbConnectorExit == 2 || gWifiSetupUsbConnectorExit == 3) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_End);
}

void WifiSetupUsbConnector_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupUsbConnectorExit == 2) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupUsbScan_End();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (gWifiSetupUsbConnectorExit == 2 || gWifiSetupUsbConnectorExit == 3) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (gWifiSetupUsbConnectorExit == 2) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
    } else if (gWifiSetupUsbConnectorExit == 3) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorIntro_Init);
    } else {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnectorDone_Init);
    }
}

void WifiSetupUsbConnector_ScanCallback(int result) {
    if (gWifiSetupUsbConnectorExit != USB_CONNECTOR_RUNNING) {
        return;
    }
    switch (result) {
    case 0:
        gWifiSetupUsbConnectorExit = 3;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
        WifiSetupDialog_Open(0x10, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitNotFound);
        break;
    case 1:
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_BACK;
        WifiSetupSound_Stop();
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
        break;
    case 2:
        WifiSetupSound_Stop();
        WifiSetupUsbScan_SetCallback(NULL);
        WifiSetupDialog_Open(0x11, 0, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitChoice);
        break;
    case 3:
        gWifiSetupUsbConnectorExit = 2;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(9);
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
        break;
    }
}

void WifiSetupUsbConnector_WaitNotFound(void) {
    if (WifiSetupDialog_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitNotFoundClosed);
}

void WifiSetupUsbConnector_WaitNotFoundClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
}

void WifiSetupUsbConnector_WaitChoice(void) {
    int r = WifiSetupDialog_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        gWifiSetupUsbConnectorExit = 3;
        WifiSetupSound_Play(6);
    } else {
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_BACK;
        WifiSetupSound_Play(7);
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitChoiceClosed);
}

void WifiSetupUsbConnector_WaitChoiceClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_BACK) {
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
        return;
    }
    WifiSetupSound_Play(0xb);
    WifiSetupButtonBar_Unlock();
    gWifiSetupUsbConnectorExit = USB_CONNECTOR_RUNNING;
    WifiSetupUsbScan_SetCallback(WifiSetupUsbConnector_ScanCallback);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Main);
}

void WifiSetupBgScreen_Create(void *src) {
    WifiSetupBgScreen *screen = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupBgScreen), 4);
    gWifiSetupBgScreen.screen = screen;
    MIi_CpuCopyFast(src, (u32 *)screen->data, 0x600);
    gWifiSetupBgScreen.screen->task = WifiSetupTask_Create(1, WifiSetupBgScreen_UpdateTask, NULL, 0x78);
}
