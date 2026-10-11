#pragma force_active on
#include "global.h"

#include "wifi_setup_address_entry.h"
#include "wifi_setup_app.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// The end of the address entry (the "invalid address" message) and the error screen
// (0x02232770-0x02232E04): shows gWifiSetupErrorCode with the message for its range and waits for
// the button.

extern const u8 ov13_022454CC[];  // font per language
extern const u16 ov13_022454DC[]; // x, y of the code per language
extern const u16 ov13_022454DE[];
extern char ov13_02246458[];
extern const u16 ov13_0224646C[]; // error code format

extern int WifiSetupDialog_GetSelected(void);
extern void WifiSetupDialog_Close(void);
extern int WifiSetupDialog_IsOpen(void);
extern void WifiSetupSettings_Init(void);
extern void ov13_022368D8(void);
extern void ov13_022352C0(int);
extern int swprintf(u16 *, int, const u16 *, ...);

void WifiSetupAddressEntry_WaitInvalidMessage(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_FinishInvalidMessage);
}

void WifiSetupAddressEntry_FinishInvalidMessage(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    gWifiSetupAddressEntry->cursor->attr01 &= 0xc1fffcff; // show again
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_OpenKeypad);
}

void WifiSetupErrorScreen_Init(void) {
    WifiSetupErrorScreen_LoadGraphics();
    WifiSetupErrorScreen_DrawMessage();
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_FadeIn);
}

void WifiSetupErrorScreen_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246458, GX_LoadBG2Scr);
    reg_G2S_DB_BG0CNT = (reg_G2S_DB_BG0CNT & ~3) | 3;
    reg_G2S_DB_BG1CNT = (reg_G2S_DB_BG1CNT & ~3) | 3;
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 3;
    reg_G2_BG1CNT = (reg_G2_BG1CNT & ~3) | 3;
    reg_G2_BG2CNT = (reg_G2_BG2CNT & ~3) | 3;
}

void WifiSetupErrorScreen_DrawMessage(void) {
    int out, code;
    void *canvas, *msg;
    u16 text[8];
    int error;
    ov13_0222B948(0, &out);
    error = gWifiSetupErrorCode;
    // message index per error code range
    if (error >= -20099) {
        code = 0;
    } else if (error >= -20100) {
        code = 2;
    } else if (error >= -20101) {
        code = 19;
    } else if (error >= -20107) {
        code = 2;
    } else if (error >= -20108) {
        code = 18;
    } else if (error >= -20109) {
        code = 2;
    } else if (error >= -20110) {
        code = 3;
    } else if (error >= -20999) {
        code = 2;
    } else if (error >= -22999) {
        code = 0;
    } else if (error >= -23999) {
        code = 19;
    } else if (error >= -49999) {
        code = 0;
    } else if (error >= -50002) {
        code = 7;
    } else if (error >= -50003) {
        code = 5;
    } else if (error >= -50098) {
        code = 0;
    } else if (error >= -50099) {
        code = out == 2 ? 5 : 7;
    } else if (error >= -51098) {
        code = 0;
    } else if (error >= -51099) {
        code = out == 2 ? 5 : 8;
    } else if (error >= -51102) {
        code = 9;
    } else if (error >= -51103) {
        code = 5;
    } else if (error >= -51199) {
        code = 0;
    } else if (error >= -51299) {
        code = 20;
    } else if (error >= -51302) {
        code = 21;
    } else if (error >= -51303) {
        code = 4;
    } else if (error >= -51999) {
        code = 0;
    } else if (error >= -52002) {
        code = 10;
    } else if (error >= -52003) {
        code = 22;
    } else if (error >= -52099) {
        code = 0;
    } else if (error >= -52103) {
        code = 11;
    } else if (error >= -52199) {
        code = 0;
    } else if (error >= -52203) {
        code = 11;
    } else if (error >= -52299) {
        code = 0;
    } else if (error >= -52399) {
        code = 2;
    } else if (error >= -52999) {
        code = 0;
    } else if (error >= -53299) {
        code = 2;
    } else {
        code = 0;
    }
    canvas = WifiSetupText_InitBgCanvas(0, ov13_022454CC[WifiSetupApp_GetLanguage()]);
    msg = WifiSetupText_GetString(ov13_0224F4F8, code);
    swprintf(text, 8, ov13_0224646C, -gWifiSetupErrorCode);
    {
        int y = ov13_022454DE[WifiSetupApp_GetLanguage() * 2];
        {
            int x = ov13_022454DC[WifiSetupApp_GetLanguage() * 2];
            WifiSetupText_DrawMonospace(canvas, x, y, 2, 10, text, 0);
        }
    }
    WifiSetupText_DrawInRect(canvas, 13, 60, 230, 94, 2, WifiSetup_GetTextFlags(), msg);
    WifiSetupText_MarkDirty(canvas);
}

void WifiSetupErrorScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 21, 8);
    WifiSetupDisplay_ShowPlanes(0, 21);
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_WaitFadeIn);
}

void WifiSetupErrorScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_022279B4(4);
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_WaitButtonBar);
}

void WifiSetupErrorScreen_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_Main);
}

void WifiSetupErrorScreen_Main(void) {
    WifiSetupErrorScreen_HandleKeys();
    ov13_02232CA4();
    WifiSetupErrorScreen_HandleButton();
}

void WifiSetupErrorScreen_HandleKeys(void) {
    if (!WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        return;
    }
    WifiSetupButtonBar_SelectIfNone(0);
}

void ov13_02232CA4(void) {
}

void WifiSetupErrorScreen_HandleButton(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_StartClose);
}

void WifiSetupErrorScreen_StartClose(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_CloseButtonBar);
}

void WifiSetupErrorScreen_CloseButtonBar(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 21, 8);
    WifiSetupApp_SetStateFunc(WifiSetupErrorScreen_Exit);
}

void WifiSetupErrorScreen_Exit(void) {
    int x;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 21);
    ov13_0222B948(0, &x);
    if (x) {
        ov13_0222B8F8(2, 1);
        WifiSetupApp_SetStateFunc(ov13_022368D8);
    } else {
        ov13_0222B8F8(2, 0);
        ov13_0222B934(0, 0);
        ov13_022352C0(0);
        WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
    }
}

void WifiSetupErrorScreen_SetCode(int code) {
    gWifiSetupErrorCode = code;
}
