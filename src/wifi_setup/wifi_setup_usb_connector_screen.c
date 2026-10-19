#include "global.h"

#include "wifi_setup/wifi_setup_app.h"
#include "wifi_setup/wifi_setup_connection_screens.h"
#include "wifi_setup/wifi_setup_local_wireless.h"
#include "wifi_setup/wifi_setup_scroll_bar.h"
#include "wifi_setup/wifi_setup_ui.h"
#include "wifi_setup/wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Wi-Fi USB connector screen: waits for the USB connector scan (WifiSetupUsbScan) and reacts to its
// result with dialogs; the exit code picks the next screen. Also the creation of the BG 2 screen copy.

// values of gWifiSetupUsbConnectorExit: the screen that follows
enum WifiSetupUsbConnectorExit {
    USB_CONNECTOR_RUNNING,
    USB_CONNECTOR_DONE,  // the connector is set up
    USB_CONNECTOR_BACK,  // back to the connection list
    USB_CONNECTOR_RETRY, // back to the connector intro
};

static void WifiSetupUsbConnector_WaitFadeIn(void);
static void WifiSetupUsbConnector_WaitButtonBar(void);
static void WifiSetupUsbConnector_Main(void);
static void WifiSetupUsbConnector_HandleInput(void);
static void WifiSetupUsbConnector_Update(void);
static void WifiSetupUsbConnector_CheckButtons(void);
static void WifiSetupUsbConnector_Exit(void);
static void WifiSetupUsbConnector_WaitFadeOut(void);
static void WifiSetupUsbConnector_End(void);
static void WifiSetupUsbConnector_WaitNotFound(void);
static void WifiSetupUsbConnector_WaitNotFoundClosed(void);
static void WifiSetupUsbConnector_WaitChoice(void);
static void WifiSetupUsbConnector_WaitChoiceClosed(void);


void WifiSetupUsbConnector_Start(void) {
    WifiSetupFade_Start(2, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupDisplay_ShowPlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitFadeIn);
}

static void WifiSetupUsbConnector_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(2);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitButtonBar);
}

static void WifiSetupUsbConnector_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Main);
}

static void WifiSetupUsbConnector_Main(void) {
    WifiSetupUsbConnector_HandleInput();
    WifiSetupUsbConnector_Update();
    WifiSetupUsbConnector_CheckButtons();
}

static void WifiSetupUsbConnector_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupUsbConnector_IsLidClosed()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

static void WifiSetupUsbConnector_Update(void) {
}

static void WifiSetupUsbConnector_CheckButtons(void) {
    if (gWifiSetupUsbConnectorExit != USB_CONNECTOR_RUNNING) {
        return;
    }
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    gWifiSetupUsbConnectorExit = USB_CONNECTOR_BACK;
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
}

static void WifiSetupUsbConnector_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitFadeOut);
}

static void WifiSetupUsbConnector_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_BACK || gWifiSetupUsbConnectorExit == USB_CONNECTOR_RETRY) {
        WifiSetupFade_Start(3, 1, GX_BLEND_PLANEMASK_BG0, 8);
    }
    WifiSetupFade_Start(3, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, 8);
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_End);
}

static void WifiSetupUsbConnector_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_BACK) {
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
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_BACK || gWifiSetupUsbConnectorExit == USB_CONNECTOR_RETRY) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, GX_PLANEMASK_BG0);
    }
    WifiSetupDisplay_HidePlanes(0, GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_BACK) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
    } else if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_RETRY) {
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
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_RETRY;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
        WifiSetupDialog_Open(0x10, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitNotFound);
        break;
    case 1:
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_DONE;
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
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_BACK;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(9);
        WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
        break;
    }
}

static void WifiSetupUsbConnector_WaitNotFound(void) {
    if (WifiSetupDialog_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitNotFoundClosed);
}

static void WifiSetupUsbConnector_WaitNotFoundClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_Exit);
}

static void WifiSetupUsbConnector_WaitChoice(void) {
    int r = WifiSetupDialog_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_RETRY;
        WifiSetupSound_Play(6);
    } else {
        gWifiSetupUsbConnectorExit = USB_CONNECTOR_DONE;
        WifiSetupSound_Play(7);
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupUsbConnector_WaitChoiceClosed);
}

static void WifiSetupUsbConnector_WaitChoiceClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    if (gWifiSetupUsbConnectorExit == USB_CONNECTOR_DONE) {
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
