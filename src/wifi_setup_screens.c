#pragma force_active on
#include "wifi_setup_screens.h"

#include "global.h"

#include "wifi_setup_app.h"
#include "wifi_setup_internal.h"
#include "wifi_setup_keyboard.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point setup screens (0x0222BF60-0x02231AD4): setup client start/stop and status, the top
// menu, the message screens that follow it, the list screen (gWifiSetupListScreen: rows, scroll
// bar, row icons) and the text entry screen (gWifiSetupTextEntry). Every screen is a chain of state functions run
// by WifiSetupApp_SetStateFunc: load graphics, fade in, wait for the button bar, handle input, fade
// out, hand over to the next screen.

// Status copied from the setup client's status callback.
typedef struct WifiSetupClientStatus {
    int state; // 0 idle .. 7, see WifiSetupClient_GetResult
    int unk_04;
    int unk_08;
} WifiSetupClientStatus;

// View of the setup client's result block (WifiSetupResult, 0xE8 bytes).
typedef struct WifiSetupClientResult {
    u8 unk_00[0x20];
    int unk_20;
    int unk_24;
    u8 unk_28[0xC0];
} WifiSetupClientResult;

// Copies of path strings (the path is localised in place).
typedef struct WifiSetupPath22 {
    char text[22];
} WifiSetupPath22;

typedef struct WifiSetupPath23 {
    char text[23];
} WifiSetupPath23;

typedef struct WifiSetupPath25 {
    char text[25];
} WifiSetupPath25;

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
    u8 unk_1;
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

typedef struct WifiSetupWiFiInfo {
    u64 id;
    u8 unk_08[0xC];
} WifiSetupWiFiInfo;

extern WifiSetupClientStatus *ov13_0224F4F4;
extern WifiSetupTopMenu ov13_0224F4FC;
extern u8 ov13_0224F504;
extern WifiSetupMenu3 ov13_0224F508;
extern u8 ov13_0224F514;
extern u8 ov13_0224F518;
extern u8 ov13_0224F51C;
extern u8 ov13_0224F520[];
extern u8 ov13_0224F524[];
extern u8 ov13_0224F528;
extern WifiSetupWaitScreen ov13_0224F52C;
extern u8 ov13_0224F534;
extern u8 ov13_0224F53C[];

extern const WifiSetupBytePair ov13_0224532C;
extern WifiSetupRect ov13_0224532E[];
extern u16 ov13_0224533E[];
extern u16 ov13_02245340[];
extern u16 ov13_02245342[];
extern u16 ov13_02245344[];
extern const WifiSetupPath22 ov13_0224534E;
extern u8 ov13_02245364[];
extern const WifiSetupPath22 ov13_0224536C;
extern const WifiSetupPath23 ov13_02245382;
extern WifiSetupRect ov13_0224539A[];
extern u16 ov13_022453B2[];
extern u16 ov13_022453B4[];
extern u16 ov13_022453B6[];
extern u16 ov13_022453B8[];
extern const WifiSetupPath22 ov13_022453CD;
extern const WifiSetupPath25 ov13_022453E3;
extern WifiSetupRect ov13_022453FC[];
extern u16 ov13_02245424[];
extern u16 ov13_02245426[];
extern u16 ov13_02245428[];
extern u32 ov13_02245454[];
extern char *ov13_02246018[];

// graphics and text bank paths
extern char ov13_02246358[];
extern u8 ov13_0224544C[];
extern u8 ov13_02245450[];
extern char ov13_02246300[];
extern char ov13_02246318[];
extern char ov13_02246330[];
extern char ov13_02246344[];
extern char ov13_022462D8[];
extern char ov13_022462EC[];
extern char ov13_0224629C[];
extern char ov13_022462B0[];
extern char ov13_022462C4[];
extern char ov13_02246034[];
extern char ov13_02246044[];
extern char ov13_02246058[];
extern char ov13_0224606C[];
extern char ov13_02246080[];
extern char ov13_02246094[];
extern char ov13_022460AC[];
extern char ov13_022460C4[];
extern char ov13_022460DC[];
extern char ov13_022460F4[];
extern char ov13_0224610C[];
extern char ov13_02246124[];
extern char ov13_02246138[];
extern char ov13_0224614C[];
extern char ov13_02246164[];
extern char ov13_0224617C[];
extern char ov13_02246190[];
extern char ov13_022461A4[];
extern char ov13_022461B8[];
extern char ov13_02246258[];
extern char ov13_02246270[];
extern char ov13_02246288[];
extern char ov13_022453CC[];
extern u8 ov13_0224546C[];
extern u8 ov13_02245470[];
extern char ov13_02246370[];
extern char ov13_02246384[];
extern char ov13_0224639C[];
extern char ov13_022463B4[];
extern char ov13_022463C8[];

extern u16 ov13_022461CC[];
extern u16 ov13_02246208[];
extern u16 ov13_02246230[];

extern void WifiSetupConnectionList_Init(void);
extern void WifiSetupMethodMenu_Init(void);
extern void WifiSetupTestPrompt_Init(void);
extern void WifiSetupBgScreen_Create(void *a);
extern void WifiSetupBgScreen_Destroy(void);
extern void WifiSetupBgScreen_MarkDirty(void);
extern void WifiSetupPalette_LoadRow(void *a, int b, int c);
extern void WifiSetupScrollBar_Create(int a, int b, int c, int d, int e);
extern void WifiSetupScrollBar_Destroy(void);
extern int WifiSetupScrollBar_GetValue(void);
extern int WifiSetupScrollBar_GetEvent(void);
extern void WifiSetupScrollBar_SetValue(int);
extern void WifiSetupScrollBar_Enable(void);
extern void WifiSetupScrollBar_Disable(void);
extern void WifiSetupApScan_FreeWork(void);
extern u8 WifiSetupApScan_GetEntries(void *p);
extern void WifiSetupConnections_Load(void);
extern void WifiSetupConnections_Free(void);
extern u8 *WifiSetupConnections_GetEdit(void);
extern void WifiSetupConnections_SetSsid(void *);
extern void WifiSetupConnections_GetSsid(void *);
extern void WifiSetupConnections_Edit(int);
extern void WifiSetupConnections_SetFromClientResult(WifiSetupClientResult *a);
extern void WifiSetupConnections_Reset(void);
extern void WifiSetupSound_Stop(void);
extern void WifiSetupBanner_LoadScreen(int a);
extern void WifiSetupDialog_Open(int a, int b, int c, int d, int e);
extern void WifiSetupDialog_Close(void);
extern int WifiSetupDialog_GetSelected(void);
extern int WifiSetupDialog_IsOpen(void);
extern void WifiSetupApScanScreen_Init(void);
extern void WifiSetupSettings_Init(void);
extern void WifiSetupText_FreeCanvas(void *);
extern void DWCi_BM_GetWiFiInfo(WifiSetupWiFiInfo *info);
extern void OS_Terminate(void);
extern s32 FX_DivS32(s32 a, s32 b);
extern s32 FX_ModS32(s32 a, s32 b);
extern int swprintf(u16 *buf, u32 n, const u16 *fmt, ...);

#define REG16(a)    (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)

void WifiSetupClient_Start(void) {
    ov13_0224F4F4 = WifiSetupHeap_AllocZeroed(0xc, -4);
    if (ov13_02226D74(0xf, 0x40, WifiSetupClient_OnStatus, WifiSetupClient_Alloc, WifiSetupClient_Free, 0x800) != 1) {
        OS_Terminate();
    }
    OS_Sleep(10);
}

void WifiSetupClient_Stop(void) {
    if (ov13_02226E5C() != 1) {
        OS_Terminate();
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F4F4);
}

int WifiSetupClient_GetResult(void) {
    WifiSetupClientResult buf;
    switch (ov13_0224F4F4->state) {
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
        if (ov13_02226F28(&buf) != 1) {
            OS_Terminate();
        }
        if (buf.unk_20 >= 0 && buf.unk_20 <= 3 && buf.unk_24 == 1) {
            return 3;
        }
        return 5;
    case 7:
        return 4;
    }
}

void WifiSetupClient_StoreResult(void) {
    WifiSetupClientResult buf;
    if (ov13_02226F28(&buf) != 1) {
        OS_Terminate();
    }
    WifiSetupConnections_SetFromClientResult(&buf);
}

void WifiSetupClient_OnStatus(s32 *status) {
    *ov13_0224F4F4 = *(WifiSetupClientStatus *)status;
}

void *WifiSetupClient_Alloc(u32 size) {
    return WifiSetupHeap_AllocZeroed(size, 0x20);
}

void WifiSetupClient_Free(void *p) {
    WifiSetupHeap_Free(p);
}

void WifiSetupScreens_StartQuit(void) {
    WifiSetupFade_Start(3, 1, 0x3f, 0x14);
    WifiSetupFade_Start(3, 0, 0x3f, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_Quit);
}

void WifiSetupScreens_Quit(void) {
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
    WifiSetupText_UnloadBank(ov13_0224F4F8);
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
        ov13_0224F4F8 = WifiSetupText_LoadBank(ov13_02246034);
    } else {
        ov13_0224F4F8 = WifiSetupText_LoadBank(ov13_02246018[WifiSetupApp_GetLanguage()]);
    }
    WifiSetupOamTemplate_Load(1, WifiSetup_LocalizePath(ov13_02246044));
    WifiSetupOamTemplate_Load(0, WifiSetup_LocalizePath(ov13_02246058));
    WifiSetup_LoadGraphics(ov13_0224606C, GXS_LoadBG1Char);
    WifiSetup_LoadGraphics(ov13_02246080, GXS_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246094, GXS_LoadOBJ);
    WifiSetup_LoadGraphics(ov13_022460AC, GXS_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_022460C4, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_022460DC, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022460F4, GX_LoadOBJ);
    WifiSetup_LoadGraphics(ov13_0224610C, GX_LoadOBJPltt);
    switch (WifiSetupApp_GetKind()) {
    case 0:
        WifiSetup_LoadGraphics(ov13_02246124, GXS_LoadBG1Scr);
        break;
    case 1:
        WifiSetup_LoadGraphics(ov13_02246138, GXS_LoadBG1Scr);
        break;
    }
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000A);
    WifiSetupDisplay_ShowPlanes(1, 2);
    WifiSetupDisplay_ShowPlanes(0, 2);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_FadeIn);
}

void WifiSetupScreens_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 2, 0x14);
    WifiSetupFade_Start(2, 0, 2, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupScreens_WaitFadeIn);
}

void WifiSetupScreens_WaitFadeIn(void) {
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
    i = ov13_0224F4FC.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_0224533E[i], ov13_02245342[i], ov13_02245340[i], ov13_02245344[i]);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_FadeIn);
}

void WifiSetupTopMenu_LoadGraphics(void) {
    WifiSetupPath22 name;
    name = ov13_0224534E;
    WifiSetup_LoadGraphics(ov13_0224614C, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246164, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_0224617C, GX_LoadBG2Scr);
    ov13_0224F4FC.graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(name.text), NULL, 4);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTopMenu_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitFadeIn);
}

void WifiSetupTopMenu_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitButtonBar);
}

void WifiSetupTopMenu_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Main);
}

void WifiSetupTopMenu_Main(void) {
    WifiSetupTopMenu_HandleInput();
    WifiSetupTopMenu_Update();
    WifiSetupTopMenu_CheckButtons();
}

void WifiSetupTopMenu_HandleInput(void) {
    u32 i;
    WifiSetupRect *entry = ov13_0224532E;
    for (i = 0; i < 2; i++, entry++) {
        if (WifiSetupInput_IsTouchPressed(entry) != 0) {
            int idx;
            WifiSetupButtonBar_SelectIfNone(1);
            ov13_0224F4FC.selected = i;
            idx = ov13_0224F4FC.selected * 4;
            WifiSetupCursor_ShowCorners(ov13_0224533E[idx], ov13_02245342[idx], ov13_02245340[idx], ov13_02245344[idx]);
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x40) != 0) {
        WifiSetupTopMenu_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80) != 0) {
        WifiSetupTopMenu_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x20) != 0) {
        WifiSetupTopMenu_MoveCursor(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x10) != 0) {
        WifiSetupTopMenu_MoveCursor(2);
    }
}

void WifiSetupTopMenu_Update(void) {
}

void WifiSetupTopMenu_CheckButtons(void) {
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

void WifiSetupTopMenu_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_WaitFadeOut);
}

void WifiSetupTopMenu_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x16, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTopMenu_End);
}

void WifiSetupTopMenu_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(ov13_0224F4FC.graphics);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    switch (ov13_0224F4FC.selected) {
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

void WifiSetupTopMenu_MoveCursor(int dir) {
    int idx;
    if (dir == 1 || dir == 3) {
        return;
    }
    ov13_0224F4FC.selected ^= 1;
    WifiSetupSound_Play(8);
    idx = ov13_0224F4FC.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_0224533E[idx], ov13_02245342[idx], ov13_02245340[idx], ov13_02245344[idx]);
}

void WifiSetupTopMenu_HighlightSelection(void) {
    WifiSetupBytePair tbl = ov13_0224532C;
    WifiSetupPalette_LoadRow(ov13_0224F4FC.graphics, tbl.values[ov13_0224F4FC.selected], tbl.values[ov13_0224F4FC.selected]);
}

void WifiSetupEraseConfirm_Init(void) {
    ov13_0224F504 = 0;
    WifiSetupEraseConfirm_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x12);
    WifiSetupMessage_Show(0x3b, -1, 0);
    WifiSetup_PrintMessage(0x17);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_FadeIn);
}

void WifiSetupEraseConfirm_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246190, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupEraseConfirm_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitFadeIn);
}

void WifiSetupEraseConfirm_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitButtonBar);
}

void WifiSetupEraseConfirm_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Main);
}

void WifiSetupEraseConfirm_Main(void) {
    WifiSetupEraseConfirm_HandleInput();
    WifiSetupEraseConfirm_Update();
    WifiSetupEraseConfirm_CheckButtons();
}

void WifiSetupEraseConfirm_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupEraseConfirm_Update(void) {
}

void WifiSetupEraseConfirm_CheckButtons(void) {
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

void WifiSetupEraseConfirm_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitFadeOut);
}

void WifiSetupEraseConfirm_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F504 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_End);
}

void WifiSetupEraseConfirm_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F504 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F504 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F504 == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Init);
    }
}

void WifiSetupEraseConfirm_WaitDialog(void) {
    int r = WifiSetupDialog_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        WifiSetupSound_Play(0xe);
        ov13_0224F504 = 1;
    } else {
        WifiSetupSound_Play(7);
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_WaitDialogClosed);
}

void WifiSetupEraseConfirm_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupEraseConfirm_Exit);
}

void WifiSetupEraseDone_Init(void) {
    WifiSetupEraseDone_LoadGraphics();
    WifiSetup_PrintMessage(0x19);
    WifiSetupConnections_Reset();
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_FadeIn);
}

void WifiSetupEraseDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022461A4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupEraseDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitFadeIn);
}

void WifiSetupEraseDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(5);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitButtonBar);
}

void WifiSetupEraseDone_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Main);
}

void WifiSetupEraseDone_Main(void) {
    WifiSetupEraseDone_HandleInput();
    WifiSetupEraseDone_Update();
    WifiSetupEraseDone_CheckButtons();
}

void WifiSetupEraseDone_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupEraseDone_Update(void) {
}

void WifiSetupEraseDone_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_Exit);
}

void WifiSetupEraseDone_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_WaitFadeOut);
}

void WifiSetupEraseDone_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 0x3f, 0x40);
    WifiSetupFade_Start(3, 0, 0x3f, 0x40);
    WifiSetupApp_SetStateFunc(WifiSetupEraseDone_PowerOff);
}

void WifiSetupEraseDone_PowerOff(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

void WifiSetupSystemInfo_Init(void) {
    WifiSetupSystemInfo_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x11);
    WifiSetupMessage_Show(0x3a, -1, 0);
    WifiSetupSystemInfo_DrawInfo();
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_FadeIn);
}

void WifiSetupSystemInfo_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022461B8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupSystemInfo_DrawInfo(void) {
    WifiSetupWiFiInfo info;
    u32 parts[4];
    u16 buf[0x14];
    u8 mac[6];
    u64 id;
    int i;
    WifiSetupCanvas *win = WifiSetupText_InitBgCanvas(0, 0);
    OS_GetMacAddress(mac);
    swprintf(buf, 0x14, ov13_022461CC, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    WifiSetupText_DrawInRect(win, 8, 0x40, 0xf0, 0x1c, 2, 0x480, buf);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.id != 0) {
        id = info.id;
        parts[3] = (id % 10) * 1000;
        id /= 10;
        for (i = 0; i < 3; i++) {
            parts[2 - i] = id % 10000;
            id /= 10000;
        }
        swprintf(buf, 0x14, ov13_02246208, parts[0], parts[1], parts[2], parts[3]);
    } else {
        swprintf(buf, 0x14, ov13_02246230);
    }
    WifiSetupText_DrawInRect(win, 8, 0x78, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
}

void WifiSetupSystemInfo_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitFadeIn);
}

void WifiSetupSystemInfo_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(2);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitButtonBar);
}

void WifiSetupSystemInfo_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_Main);
}

void WifiSetupSystemInfo_Main(void) {
    WifiSetupSystemInfo_HandleInput();
    WifiSetupSystemInfo_Update();
    WifiSetupSystemInfo_CheckButtons();
}

void WifiSetupSystemInfo_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupSystemInfo_Update(void) {
}

void WifiSetupSystemInfo_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_Exit);
}

void WifiSetupSystemInfo_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_WaitFadeOut);
}

void WifiSetupSystemInfo_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupSystemInfo_End);
}

void WifiSetupSystemInfo_End(void) {
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
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
}

void WifiSetupOptions_Init(void) {
    int idx;
    ov13_0224F508.unk_1 = 0;
    WifiSetupOptions_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x10);
    WifiSetupBanner_LoadScreen(3);
    WifiSetupMessage_Show(0x39, -1, 0);
    ov13_0224F508.oam = WifiSetupOamTemplate_AllocOam(0, 0x5b);
    ov13_0224F508.oam->attr01 = (ov13_0224F508.oam->attr01 & 0xFE00FF00) | 0x84 | 0xE00000;
    ov13_0224F508.oam->attr2 = (ov13_0224F508.oam->attr2 & ~0xc00) | 0xc00;
    idx = ov13_0224F508.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_FadeIn);
}

void WifiSetupOptions_LoadGraphics(void) {
    WifiSetupPath22 a;
    WifiSetupPath23 b;
    WifiSetupWiFiInfo info;
    u8 *src;
    u8 *dst;
    int i;
    u8 *pal;
    a = ov13_0224536C;
    b = ov13_02245382;
    WifiSetup_LoadGraphics(ov13_02246258, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246270, GX_LoadBG2Scr);
    ov13_0224F508.graphics = WifiSetupFile_Load(WifiSetup_LocalizePath(b.text), NULL, 4);
    pal = WifiSetupFile_Load(WifiSetup_LocalizePath(a.text), NULL, 4);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.id == 0) {
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
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupOptions_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitFadeIn);
}

void WifiSetupOptions_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitButtonBar);
}

void WifiSetupOptions_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Main);
}

void WifiSetupOptions_Main(void) {
    WifiSetupOptions_HandleInput();
    WifiSetupOptions_Update();
    WifiSetupOptions_CheckButtons();
}

void WifiSetupOptions_HandleInput(void) {
    u32 i;
    WifiSetupRect *entry = ov13_0224539A;
    for (i = 0; i < 3; i++, entry++) {
        if (WifiSetupInput_IsTouchPressed(entry) != 0) {
            int idx;
            WifiSetupButtonBar_SelectIfNone(1);
            ov13_0224F508.selected = i;
            idx = ov13_0224F508.selected * 4;
            WifiSetupCursor_ShowCorners(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
            return;
        }
    }
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x40) != 0) {
        WifiSetupOptions_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80) != 0) {
        WifiSetupOptions_MoveCursor(3);
    }
}

void WifiSetupOptions_Update(void) {
}

void WifiSetupOptions_CheckButtons(void) {
    WifiSetupWiFiInfo info;
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        DWCi_BM_GetWiFiInfo(&info);
        if (ov13_0224F508.selected != 0 && info.id == 0) {
            WifiSetupSound_Play(9);
            WifiSetupButtonBar_SetSelected(-1);
            return;
        }
        WifiSetupSound_Play(6);
        WifiSetupOptions_HighlightSelection();
        ov13_0224F508.unk_1 = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Exit);
}

void WifiSetupOptions_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_WaitFadeOut);
}

void WifiSetupOptions_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (ov13_0224F508.unk_1 != 0) {
        WifiSetupButtonBar_Close();
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_End);
}

void WifiSetupOptions_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupOam_Release(ov13_0224F508.oam);
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(ov13_0224F508.graphics);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    if (ov13_0224F508.unk_1 == 0) {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTopMenu_Init);
        return;
    }
    switch (ov13_0224F508.selected) {
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

void WifiSetupOptions_MoveCursor(int dir) {
    int idx;
    if (dir == 1) {
        ov13_0224F508.selected = FX_ModS32(ov13_0224F508.selected + 2, 3);
    } else {
        ov13_0224F508.selected = FX_ModS32(ov13_0224F508.selected + 1, 3);
    }
    WifiSetupSound_Play(8);
    idx = ov13_0224F508.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
}

void WifiSetupOptions_HighlightSelection(void) {
    WifiSetupPalette_LoadRow(ov13_0224F508.graphics, ov13_02245364[ov13_0224F508.selected], ov13_02245364[ov13_0224F508.selected]);
}

void WifiSetupTransferring_Init(void) {
    WifiSetupTransferring_LoadGraphics();
    WifiSetup_PrintMessage(0x1e);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSignalIcon_Create(1);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_FadeIn);
}

void WifiSetupTransferring_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246288, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferring_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_WaitFadeIn);
}

void WifiSetupTransferring_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupTransfer_SetCallback((WifiSetupSessionCallback)WifiSetupTransferring_OnTransferEvent);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_Main);
}

void WifiSetupTransferring_Main(void) {
    WifiSetupTransferring_HandleInput();
    WifiSetupTransferring_Update();
}

void WifiSetupTransferring_HandleInput(void) {
}

void WifiSetupTransferring_Update(void) {
}

void WifiSetupTransferring_FadeOut(void) {
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_End);
}

void WifiSetupTransferring_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupSignalIcon_Destroy();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(0, 1);
    if (ov13_0224F514 == 0) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else if (ov13_0224F514 == 2) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_Init);
    } else {
        WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Init);
    }
}

void WifiSetupTransferring_OnTransferEvent(int a) {
    if (a == 2) {
        WifiSetupConnections_Reset();
        ov13_0224F514 = 1;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x10);
    } else if (a == 3) {
        ov13_0224F514 = 2;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
    } else {
        ov13_0224F514 = 0;
        WifiSetupSound_Stop();
        WifiSetupSound_Play(0x12);
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupTransfer_Cancel();
    WifiSetupApp_SetStateFunc(WifiSetupTransferring_FadeOut);
}

void WifiSetupTransferDone_Init(void) {
    WifiSetupTransferDone_LoadGraphics();
    WifiSetup_PrintMessage(0x1f);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_FadeIn);
}

void WifiSetupTransferDone_LoadGraphics(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitFadeIn);
}

void WifiSetupTransferDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(5);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitButtonBar);
}

void WifiSetupTransferDone_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Main);
}

void WifiSetupTransferDone_Main(void) {
    WifiSetupTransferDone_HandleInput();
    WifiSetupTransferDone_Update();
    WifiSetupTransferDone_CheckButtons();
}

void WifiSetupTransferDone_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupTransferDone_Update(void) {
}

void WifiSetupTransferDone_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_Exit);
}

void WifiSetupTransferDone_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_WaitFadeOut);
}

void WifiSetupTransferDone_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 0x3f, 0x40);
    WifiSetupFade_Start(3, 0, 0x3f, 0x40);
    WifiSetupApp_SetStateFunc(WifiSetupTransferDone_PowerOff);
}

void WifiSetupTransferDone_PowerOff(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

void WifiSetupTransferInfo_Init(void) {
    WifiSetupTransferInfo_LoadGraphics();
    WifiSetup_PrintMessage(0x1a);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_FadeIn);
}

void WifiSetupTransferInfo_LoadGraphics(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferInfo_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitFadeIn);
}

void WifiSetupTransferInfo_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(4);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitButtonBar);
}

void WifiSetupTransferInfo_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Main);
}

void WifiSetupTransferInfo_Main(void) {
    WifiSetupTransferInfo_HandleInput();
    WifiSetupTransferInfo_Update();
    WifiSetupTransferInfo_CheckButtons();
}

void WifiSetupTransferInfo_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupTransferInfo_Update(void) {
}

void WifiSetupTransferInfo_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Exit);
}

void WifiSetupTransferInfo_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_WaitFadeOut);
}

void WifiSetupTransferInfo_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_End);
}

void WifiSetupTransferInfo_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Init);
}

void WifiSetupTransferIntro_Init(void) {
    ov13_0224F518 = 0;
    WifiSetupTransferIntro_LoadGraphics();
    WifiSetupBackground_LoadHeader(0x13);
    WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetup_PrintMessage(0x1b);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_FadeIn);
}

void WifiSetupTransferIntro_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_0224629C, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferIntro_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitFadeIn);
}

void WifiSetupTransferIntro_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitButtonBar);
}

void WifiSetupTransferIntro_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_Main);
}

void WifiSetupTransferIntro_Main(void) {
    WifiSetupTransferIntro_HandleInput();
    WifiSetupTransferIntro_Update();
    WifiSetupTransferIntro_CheckButtons();
}

void WifiSetupTransferIntro_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupTransferIntro_Update(void) {
}

void WifiSetupTransferIntro_CheckButtons(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(6);
        ov13_0224F518 = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_Exit);
}

void WifiSetupTransferIntro_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_WaitFadeOut);
}

void WifiSetupTransferIntro_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F518 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferIntro_End);
}

void WifiSetupTransferIntro_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F518 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F518 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F518 == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferInfo_Init);
    }
}

void WifiSetupTransferCancelled_Init(void) {
    ov13_0224F51C = 0;
    WifiSetupTransferCancelled_LoadGraphics();
    WifiSetup_PrintMessage(0x21);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_FadeIn);
}

void WifiSetupTransferCancelled_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022462B0, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferCancelled_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_WaitFadeIn);
}

void WifiSetupTransferCancelled_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_Main);
}

void WifiSetupTransferCancelled_Main(void) {
    WifiSetupTransferCancelled_UpdateTimer();
    WifiSetupTransferCancelled_Update();
}

void WifiSetupTransferCancelled_Update(void) {
}

void WifiSetupTransferCancelled_FadeOut(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_End);
}

void WifiSetupTransferCancelled_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    WifiSetupApp_SetTransition(0, 1);
    WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
}

void WifiSetupTransferCancelled_UpdateTimer(void) {
    if (++ov13_0224F51C < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferCancelled_FadeOut);
}

void WifiSetupTransferFailed_Init(void) {
    WifiSetupTransferFailed_LoadGraphics();
    WifiSetup_PrintMessage(0x20);
    ov13_0224F520[0] = WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_FadeIn);
}

void WifiSetupTransferFailed_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022462C4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferFailed_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    if (ov13_0224F520[0] != 0) {
        WifiSetupFade_Start(2, 1, 1, 8);
        WifiSetupDisplay_ShowPlanes(1, 1);
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitFadeIn);
}

void WifiSetupTransferFailed_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(3);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitButtonBar);
}

void WifiSetupTransferFailed_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    if (WifiSetupTransfer_IsFinished() == 0) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Main);
}

void WifiSetupTransferFailed_Main(void) {
    WifiSetupTransferFailed_HandleInput();
    WifiSetupTransferFailed_Update();
    WifiSetupTransferFailed_CheckButtons();
}

void WifiSetupTransferFailed_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupTransferFailed_Update(void) {
}

void WifiSetupTransferFailed_CheckButtons(void) {
    int r = WifiSetupButtonBar_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        ov13_0224F520[1] = 1;
        WifiSetupSound_Play(6);
    } else {
        ov13_0224F520[1] = 0;
        WifiSetupSound_Play(7);
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Exit);
}

void WifiSetupTransferFailed_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_WaitFadeOut);
}

void WifiSetupTransferFailed_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F520[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_End);
}

void WifiSetupTransferFailed_End(void) {
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
    if (ov13_0224F520[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F520[1] == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Init);
    }
}

void WifiSetupTransferConfirm_Init(void) {
    u16 buf[11];
    u8 *src = WifiSetupTransfer_GetChildUser();
    WifiSetupCanvas *win = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F524[1] = 0;
    ov13_0224F524[0] = 0;
    WifiSetupTransferConfirm_LoadGraphics();
    MIi_CpuClear16(0, buf, sizeof(buf));
    MIi_CpuCopy16((u16 *)(src + 2), buf, src[1] * 2);
    WifiSetupText_DrawInRect(win, 8, 0x35, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
    WifiSetupTransfer_SetCallback((WifiSetupSessionCallback)WifiSetupTransferConfirm_OnTransferEvent);
    WifiSetupDialog_Open(0x1d, 4, 0, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_FadeIn);
}

void WifiSetupTransferConfirm_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022462D8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferConfirm_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitFadeIn);
}

void WifiSetupTransferConfirm_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitDialog);
}

void WifiSetupTransferConfirm_WaitDialog(void) {
    if (WifiSetupDialog_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_Main);
}

void WifiSetupTransferConfirm_Main(void) {
    WifiSetupTransferConfirm_HandleInput();
    WifiSetupTransferConfirm_Update();
    switch (WifiSetupDialog_GetSelected()) {
    case 0:
        ov13_0224F524[1] = 0;
        WifiSetupSound_Play(7);
        break;
    case 1:
        ov13_0224F524[1] = 1;
        WifiSetupSound_Play(0xe);
        break;
    default:
        return;
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitDialogClosed);
}

void WifiSetupTransferConfirm_HandleInput(void) {
}

void WifiSetupTransferConfirm_Update(void) {
}

void WifiSetupTransferConfirm_WaitDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    if (ov13_0224F524[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_WaitFadeOut);
}

void WifiSetupTransferConfirm_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F524[0] == 0 && ov13_0224F524[1] == 1) {
        WifiSetupTransfer_StartDownload();
    } else {
        WifiSetupTransfer_Cancel();
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_End);
}

void WifiSetupTransferConfirm_End(void) {
    if (ov13_0224F524[0] != 0 || ov13_0224F524[1] == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F524[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupApp_SetTransition(0, 1);
    if (ov13_0224F524[0] != 0) {
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else if (ov13_0224F524[1] == 0) {
        WifiSetupDisplay_HidePlanes(1, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else {
        WifiSetupApp_SetStateFunc(WifiSetupTransferring_Init);
    }
}

void WifiSetupTransferConfirm_OnTransferEvent(void) {
    ov13_0224F524[0] = 1;
}

int WifiSetupTransferWait_IsLidClosed(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void WifiSetupTransferWait_Init(void) {
    WifiSetupTransfer_Start((WifiSetupSessionCallback)WifiSetupTransferWait_OnTransferEvent);
    ov13_0224F528 = 0;
    WifiSetupTransferWait_LoadGraphics();
    WifiSetup_PrintMessage(0x1c);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_FadeIn);
}

void WifiSetupTransferWait_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022462EC, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupTransferWait_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitFadeIn);
}

void WifiSetupTransferWait_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitButtonBar);
}

void WifiSetupTransferWait_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Main);
}

void WifiSetupTransferWait_Main(void) {
    WifiSetupTransferWait_HandleInput();
    WifiSetupTransferWait_Update();
    WifiSetupTransferWait_CheckButtons();
}

void WifiSetupTransferWait_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupTransferWait_IsLidClosed() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupTransferWait_Update(void) {
}

void WifiSetupTransferWait_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Exit);
}

void WifiSetupTransferWait_Exit(void) {
    if (ov13_0224F528 == 0) {
        WifiSetupTransfer_Cancel();
    }
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_WaitFadeOut);
}

void WifiSetupTransferWait_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F528 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_End);
}

void WifiSetupTransferWait_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (WifiSetupTransfer_IsFinished() == 0) {
            return;
        }
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F528 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F528 == 0) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupOptions_Init);
    } else if (ov13_0224F528 == 2) {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferFailed_Init);
    } else {
        WifiSetupApp_SetTransition(0, 1);
        WifiSetupApp_SetStateFunc(WifiSetupTransferConfirm_Init);
    }
}

void WifiSetupTransferWait_OnTransferEvent(int a) {
    WifiSetupSound_Stop();
    if (a == 0) {
        ov13_0224F528 = 1;
        WifiSetupSound_Play(0x10);
    } else {
        ov13_0224F528 = 2;
        WifiSetupSound_Play(0x12);
    }
    WifiSetupTransfer_SetCallback(NULL);
    WifiSetupApp_SetStateFunc(WifiSetupTransferWait_Exit);
}

int WifiSetupAoss_IsLidClosed(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void WifiSetupAoss_Init(void) {
    ov13_0224F52C.counter = 0;
    WifiSetupAoss_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetupMessage_Show(0x36, -1, 0);
    WifiSetupBanner_Show(2);
    WifiSetup_PrintMessage(0x22);
    WifiSetupBusyIcon_Create(0);
    WifiSetupApp_StartClient();
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_FadeIn);
}

void WifiSetupAoss_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246300, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246318, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246330, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupAoss_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFadeIn);
}

void WifiSetupAoss_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(1);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitButtonBar);
}

void WifiSetupAoss_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    ov13_0224F52C.task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupAoss_InputTask, 0, 0x78);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Main);
}

void WifiSetupAoss_Main(void) {
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_Update();
    WifiSetupAoss_CheckButtons();
    switch (WifiSetupApp_PollClient()) {
    case 0:
        return;
    case 1:
        if (ov13_0224F52C.task == NULL) {
            return;
        }
        ov13_0224F52C.flag = 1;
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
        ov13_0224F52C.task = NULL;
        WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFinish);
        break;
    case 2:
        if (ov13_0224F52C.task == NULL) {
            return;
        }
        WifiSetupSound_Stop();
        WifiSetupDialog_Open(0xc, 1, 1, -1, 0);
        WifiSetupSound_Play(9);
        WifiSetupButtonBar_Lock();
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
        ov13_0224F52C.task = NULL;
        WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitErrorDialog);
        break;
    }
}

void WifiSetupAoss_HandleInput(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupAoss_IsLidClosed() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void WifiSetupAoss_Update(void) {
}

void WifiSetupAoss_CheckButtons(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    if (ov13_0224F52C.task == NULL) {
        return;
    }
    WifiSetupTask_DestroyLater(1, ov13_0224F52C.task);
    ov13_0224F52C.task = NULL;
    WifiSetupButtonBar_Lock();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Cancel);
}

void WifiSetupAoss_Exit(void) {
    WifiSetupButtonBar_Lock();
    if (ov13_0224F52C.task != NULL) {
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
    }
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitFadeOut);
}

void WifiSetupAoss_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F52C.flag == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAoss_End);
}

void WifiSetupAoss_End(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F52C.flag == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (WifiSetupButtonBar_IsNotClosing() == 0) {
        return;
    }
    WifiSetupApp_StopClient(ov13_0224F52C.flag != 0 ? 1 : 0);
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F52C.flag == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F52C.flag == 0) {
        WifiSetupApp_SetTransition(2, 1);
        WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
    } else {
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupAossDone_Init);
    }
}

void WifiSetupAoss_InputTask(void) {
    WifiSetupInput_Update();
    WifiSetupTask_RunQueue(0);
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_CheckButtons();
}

void WifiSetupAoss_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_WaitErrorDialogClosed);
}

void WifiSetupAoss_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

void WifiSetupAoss_WaitFinish(void) {
    WifiSetupAoss_HandleInput();
    WifiSetupAoss_Update();
    WifiSetupAoss_CheckButtons();
    ov13_0224F52C.counter++;
    if (ov13_0224F52C.counter < 0x438) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

void WifiSetupAoss_Cancel(void) {
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    ov13_0224F52C.flag = 0;
    WifiSetupApp_SetStateFunc(WifiSetupAoss_Exit);
}

void WifiSetupAossDone_Init(void) {
    ov13_0224F534 = 0;
    WifiSetupAossDone_LoadGraphics();
    WifiSetupBackground_LoadSlotHeader();
    WifiSetup_PrintMessage(0x23);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_FadeIn);
}

void WifiSetupAossDone_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246344, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void WifiSetupAossDone_FadeIn(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_WaitFadeIn);
}

void WifiSetupAossDone_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_Main);
}

void WifiSetupAossDone_Main(void) {
    WifiSetupAossDone_UpdateTimer();
    WifiSetupAossDone_Update();
}

void WifiSetupAossDone_Update(void) {
}

void WifiSetupAossDone_FadeOut(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_End);
}

void WifiSetupAossDone_End(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
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

void WifiSetupAossDone_UpdateTimer(void) {
    if (++ov13_0224F534 < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAossDone_FadeOut);
}

void WifiSetupListScreen_Init(void) {
    int result;
    gWifiSetupListScreen.work = WifiSetupHeap_AllocZeroed(0x5c, 4);
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
    gWifiSetupListScreen.work->unk_3C = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollBgTask, 0, 0x6e);
    WifiSetupListScreen_DrawRows();
    WifiSetupListScreen_ShowCursor();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_FadeIn);
}

void WifiSetupListScreen_LoadGraphics(void) {
    WifiSetupPath25 a;
    WifiSetupPath22 b;
    a = ov13_022453E3;
    b = ov13_022453CD;
    WifiSetup_LoadGraphics(ov13_02246358, GX_LoadBG3Scr);
    gWifiSetupListScreen.work->unk_04 = WifiSetupFile_Load(WifiSetup_LocalizePath(a.text), 0, 4);
    WifiSetupBgScreen_Create(gWifiSetupListScreen.work->unk_04);
    WifiSetupBgScreen_MarkDirty();
    gWifiSetupListScreen.work->unk_08 = WifiSetupFile_Load(WifiSetup_LocalizePath(b.text), 0, 4);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    REG16(0x0400000A) = (u16)((REG16(0x0400000A) & ~3) | 2);
    SET_PRI3(0x0400000C);
    REG16(0x0400000E) = (u16)((REG16(0x0400000E) & ~3) | 2);
}

void WifiSetupListScreen_CreateScrollBar(void) {
    int mode;
    int val;
    WifiSetupListWork *big = gWifiSetupListScreen.work;
    big->total = (big->count - 4) * 0x1c;
    big = gWifiSetupListScreen.work;
    val = 0;
    if (big->count <= 4) {
        mode = 0;
        big->unk_53 = 0;
    } else if (big->count <= 8) {
        big->unk_53 = 0x1f;
        mode = 1;
    } else {
        big->unk_53 = 0x37;
        mode = 2;
    }
    if (mode != 0) {
        val = FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->unk_53, gWifiSetupListScreen.work->total);
    }
    WifiSetupScrollBar_Create(mode, 0x55, 0xec, 0x3f, val);
}

void WifiSetupListScreen_CreateRowObjects(void) {
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
        WifiSetupOamTemplate_Copy(0, ov13_0224544C[i], gWifiSetupListScreen.work->rowOams[0]);
        gWifiSetupListScreen.work->unk_42[i] = gWifiSetupListScreen.work->rowOams[0]->attr2 & 1023;
    }
    for (i = 0; (u32)i < 4; i++) {
        WifiSetupOamTemplate_Copy(0, ov13_02245450[i], gWifiSetupListScreen.work->iconOams[0]);
        gWifiSetupListScreen.work->unk_48[i] = gWifiSetupListScreen.work->iconOams[0]->attr2 & 1023;
    }
    for (i = 0; i < n; i++) {
        gWifiSetupListScreen.work->rowOams[i]->attr2 = (gWifiSetupListScreen.work->rowOams[i]->attr2 & ~0xC00) | 0xC00;   // priority 3
        gWifiSetupListScreen.work->iconOams[i]->attr2 = (gWifiSetupListScreen.work->iconOams[i]->attr2 & ~0xC00) | 0xC00; // priority 3
    }
}

void WifiSetupListScreen_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x1d, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x1d);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitFadeIn);
}

void WifiSetupListScreen_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupButtonBar_Show(0);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitButtonBar);
}

void WifiSetupListScreen_WaitButtonBar(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Main);
}

void WifiSetupListScreen_Main(void) {
    WifiSetupListScreen_HandleInput();
    WifiSetupListScreen_HandleScrollBar();
    WifiSetupListScreen_CheckButtons();
}

void WifiSetupListScreen_HandleInput(void) {
    int i;
    WifiSetupRect *box;
    if (gWifiSetupListScreen.work->task || gWifiSetupListScreen.work->unk_57) {
        return;
    }
    if (WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        gWifiSetupListScreen.work->selected = -1;
        for (i = 0, box = ov13_022453FC; (u32)i < 5; i++, box++) {
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
    if (WifiSetupInput_IsTouchReleased(&ov13_02245958)) {
        WifiSetupRect *pos;
        int j;
        for (j = 0, pos = ov13_022453FC; j < 4; j++, pos++) {
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
    if (WifiSetupInput_IsKeyPressed(1)) {
        WifiSetupButtonBar_SelectIfNone(1);
        WifiSetupScrollBar_Disable();
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x200)) {
        WifiSetupListScreen_StartScrollDown();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x200)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x100)) {
        WifiSetupListScreen_StartScrollUp();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x100)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x40)) {
        WifiSetupListScreen_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x40)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80)) {
        WifiSetupListScreen_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x80)) {
        gWifiSetupListScreen.work->unk_59 = 0;
    }
}

void WifiSetupListScreen_HandleScrollBar(void) {
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
        gWifiSetupListScreen.work->unk_57 = 1;
        WifiSetupButtonBar_Lock();
        break;
    case 2:
        if (gWifiSetupListScreen.work->delay) {
            break;
        }
        WifiSetupCursor_Hide();
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * WifiSetupScrollBar_GetValue(), gWifiSetupListScreen.work->unk_53);
        WifiSetupListScreen_DrawRows();
        gWifiSetupListScreen.work->delay = 4;
        break;
    case 3: {
        int mod;
        gWifiSetupListScreen.work->unk_57 = 0;
        WifiSetupButtonBar_Unlock();
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * WifiSetupScrollBar_GetValue(), gWifiSetupListScreen.work->unk_53);
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
            if (gWifiSetupListScreen.work->unk_58 == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupListScreen.work->unk_58 = 1;
            }
        } else {
            WifiSetupSound_Play(19);
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
        }
        break;
    case 6:
        if (gWifiSetupListScreen.work->count <= 4 || gWifiSetupListScreen.position == gWifiSetupListScreen.work->total) {
            if (gWifiSetupListScreen.work->unk_58 == 0) {
                WifiSetupSound_Play(9);
                gWifiSetupListScreen.work->unk_58 = 1;
            }
        } else {
            WifiSetupSound_Play(19);
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollDownTask, 0, 120);
        }
        break;
    case 5:
    case 7:
        gWifiSetupListScreen.work->unk_58 = 0;
        break;
    }
}

void WifiSetupListScreen_CheckButtons(void) {
    int result, index;
    WifiSetupListWork *s;
    if (gWifiSetupListScreen.work->task || gWifiSetupListScreen.work->unk_57) {
        return;
    }
    result = WifiSetupButtonBar_GetSelected();
    switch (result) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        if (gWifiSetupListScreen.selected == 4) {
            gWifiSetupListScreen.work->unk_54 = 1;
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
            s->unk_54 = 1;
            gWifiSetupListScreen.work->index = index;
            WifiSetupSound_Play(6);
        }
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Exit);
}

void WifiSetupListScreen_Exit(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitFadeOut);
}

void WifiSetupListScreen_WaitFadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (gWifiSetupListScreen.work->unk_54) {
        WifiSetupButtonBar_Close();
    } else {
        WifiSetupButtonBar_Lock();
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 29, 8);
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_End);
}

void WifiSetupListScreen_End(void) {
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
    WifiSetupTask_Destroy(1, gWifiSetupListScreen.work->unk_3C);
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
    WifiSetupFile_Free(gWifiSetupListScreen.work->unk_04);
    WifiSetupFile_Free(gWifiSetupListScreen.work->unk_08);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 29);
    *(volatile u32 *)0x04000010 = 0;
    *(volatile u32 *)0x04000018 = 0;
    if (!gWifiSetupListScreen.work->unk_54) {
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

void WifiSetupListScreen_DrawRows(void) {
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

void WifiSetupListScreen_DrawRowText(int index, int row) {
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

void WifiSetupListScreen_SetRowIcons(int a, int b) {
    int off;
    GXOamAttr *p;
    if (a >= gWifiSetupListScreen.work->count) {
        return;
    }
    off = a * 42;
    p = gWifiSetupListScreen.work->rowOams[b];
    p->attr2 = (p->attr2 & ~0x3ff) | gWifiSetupListScreen.work->unk_42[(gWifiSetupListScreen.work->rowData + off)[40]];
    p = gWifiSetupListScreen.work->iconOams[b];
    p->attr2 = (p->attr2 & ~0x3ff) | gWifiSetupListScreen.work->unk_48[*(u16 *)(gWifiSetupListScreen.work->rowData + off + 38)];
}

void WifiSetupListScreen_PositionRows(void) {
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
    gWifiSetupListScreen.work->unk_56 = 1;
}

void WifiSetupListScreen_ScrollUpTask(void *task) {
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
    WifiSetupScrollBar_SetValue(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->unk_53, gWifiSetupListScreen.work->total));
    WifiSetupScrollBar_Enable();
    WifiSetupListScreen_ShowCursor();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupListScreen_ScrollDownTask(void *task) {
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
    WifiSetupScrollBar_SetValue(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->unk_53, gWifiSetupListScreen.work->total));
    WifiSetupScrollBar_Enable();
    WifiSetupListScreen_ShowCursor();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupListScreen_ScrollBgTask(void) {
    int x;
    if (!gWifiSetupListScreen.work->unk_56) {
        return;
    }
    x = ((FX_ModS32(gWifiSetupListScreen.position, 28) - 50) << 16) & 0x01ff0000;
    *(vu32 *)0x04000010 = x;
    *(vu32 *)0x04000018 = x;
    gWifiSetupListScreen.work->unk_56 = 0;
}

void WifiSetupListScreen_ShowCursor(void) {
    unsigned x = gWifiSetupListScreen.selected;
    WifiSetupCursor_ShowPair(x < 4 ? 2 : 3, ov13_02245424[x * 4], ov13_02245428[x * 4], ov13_02245426[x * 4]);
}

void WifiSetupListScreen_MoveCursor(int input) {
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
        if (gWifiSetupListScreen.work->unk_59) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->unk_59 = 1;
        return;
    }
    WifiSetupSound_Play(8);
    WifiSetupListScreen_ShowCursor();
}

void WifiSetupListScreen_StartScrollUp(void) {
    if (!gWifiSetupListScreen.position) {
        if (gWifiSetupListScreen.work->unk_59) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->unk_59 = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollUpTask, 0, 120);
}

void WifiSetupListScreen_StartScrollDown(void) {
    if (gWifiSetupListScreen.position == gWifiSetupListScreen.work->total || gWifiSetupListScreen.work->count <= 4) {
        if (gWifiSetupListScreen.work->unk_59) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->unk_59 = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupListScreen_ScrollDownTask, 0, 120);
}

void WifiSetupListScreen_HighlightRescanButton(void) {
    volatile u8 local;
    u8 x = ov13_022453CC[0];
    local = x;
    WifiSetupPalette_LoadRow(gWifiSetupListScreen.work->unk_08, x, x);
}

void WifiSetupListScreen_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_WaitErrorDialogClosed);
}

void WifiSetupListScreen_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupScrollBar_Enable();
    WifiSetupApp_SetStateFunc(WifiSetupListScreen_Main);
}

void WifiSetupTextEntry_Init(void) {
    WifiSetupBytePair p = *(const WifiSetupBytePair *)ov13_02245454;
    int a, b;
    gWifiSetupTextEntry = WifiSetupHeap_AllocZeroed(44, 4);
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

void WifiSetupTextEntry_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_02246370, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_02246384, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_0224639C, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022463B4, GX_LoadBG2Scr);
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 3;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 3;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 2;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 3;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 3;
}

void WifiSetupTextEntry_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 21, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 21);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitFadeIn);
}

void WifiSetupTextEntry_WaitFadeIn(void) {
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

void WifiSetupTextEntry_WaitKeyboard(void) {
    if (WifiSetupKeyboard_GetInput() == 255) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Main);
}

void WifiSetupTextEntry_Main(void) {
    WifiSetupTextEntry_HandleInput();
    WifiSetupTextEntry_Update();
}

void WifiSetupTextEntry_HandleInput(void) {
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

void WifiSetupTextEntry_Update(void) {
}

void WifiSetupTextEntry_Exit(void) {
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_CloseKeyboard);
}

void WifiSetupTextEntry_CloseKeyboard(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupKeyboard_Close();
    WifiSetupSound_Play(21);
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitKeyboardClosed);
}

void WifiSetupTextEntry_WaitKeyboardClosed(void) {
    u32 opts[2];
    int x;
    opts[0] = ov13_02245454[2];
    opts[1] = ov13_02245454[3];
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

void WifiSetupTextEntry_End(void) {
    int a, b;
    WifiSetupMessage_Hide();
    WifiSetupOam_Release(gWifiSetupTextEntry->cursor);
    WifiSetupText_FreeBgCanvas(0);
    WifiSetup_LoadGraphics(ov13_022463C8, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 21);
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

void WifiSetupTextEntry_DrawText(void) {
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
        rect.x = ov13_02245470[j];
        WifiSetupText_DrawInRect(gWifiSetupTextEntry->canvas, rect.x, rect.y, rect.width, rect.height, 2, 0x480, tile);
    }
    WifiSetupText_MarkDirty(gWifiSetupTextEntry->canvas);
}

void WifiSetupTextEntry_UpdateCursor(void) {
    unsigned x = gWifiSetupTextEntry->length;
    GXOamAttr *p = gWifiSetupTextEntry->cursor;
    int col = x & 15, row = (int)x >> 4;
    if (x >= 32) {
        row = 1;
        col = 15;
    }
    p->attr01 = (p->attr01 & 0xfe00ff00) | ov13_0224546C[row] | ((u32)ov13_02245470[col] << 23 >> 7);
}

void WifiSetupTextEntry_WaitConfirmDialog(void) {
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

void WifiSetupTextEntry_WaitConfirmDialogClosed(void) {
    void (*opts[2])(u8 *);
    int x;
    opts[0] = (void (*)(u8 *))ov13_02245454[4];
    opts[1] = (void (*)(u8 *))ov13_02245454[5];
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

int WifiSetupTextEntry_IsTextValid(void) {
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

void WifiSetupTextEntry_WaitErrorDialog(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitErrorDialogClosed);
}

void WifiSetupTextEntry_WaitErrorDialogClosed(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    gWifiSetupTextEntry->cursor->attr01 &= 0xc1fffcff;
    WifiSetupApp_SetStateFunc(WifiSetupTextEntry_WaitFadeIn);
}
