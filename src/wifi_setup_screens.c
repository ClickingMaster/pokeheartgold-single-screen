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

extern void ov13_022368D8(void);
extern void ov13_02237618(void);
extern void ov13_02238440(void);
extern void ov13_02239228(void *a);
extern void ov13_02239288(void);
extern void ov13_022392B4(void);
extern void ov13_02239378(void *a, int b, int c);
extern void ov13_0223943C(int a, int b, int c, int d, int e);
extern void ov13_02239514(void);
extern int ov13_02239550(void);
extern int ov13_02239564(void);
extern void ov13_02239578(int);
extern void ov13_02239584(void);
extern void ov13_0223959C(void);
extern void ov13_02239B34(void);
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
extern void ov13_022363F4(void);
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

void ov13_0222C10C(void) {
    WifiSetupFade_Start(3, 1, 0x3f, 0x14);
    WifiSetupFade_Start(3, 0, 0x3f, 0x14);
    WifiSetupApp_SetStateFunc(ov13_0222C148);
}

void ov13_0222C148(void) {
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

void ov13_0222C1BC(void) {
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
    WifiSetupApp_SetStateFunc(ov13_0222C39C);
}

void ov13_0222C39C(void) {
    WifiSetupFade_Start(2, 1, 2, 0x14);
    WifiSetupFade_Start(2, 0, 2, 0x14);
    WifiSetupApp_SetStateFunc(ov13_0222C3D8);
}

void ov13_0222C3D8(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    switch (WifiSetupApp_GetKind()) {
    case 0:
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222C44C);
        break;
    case 1:
        ov13_0222B8F8(1, 1);
        WifiSetupApp_SetStateFunc(ov13_022368D8);
        break;
    }
}

void ov13_0222C44C(void) {
    int i;
    ov13_0222C4C8();
    WifiSetupBackground_LoadHeader(0);
    WifiSetupMessage_Show(0x2e, -1, 0);
    WifiSetupBanner_LoadScreen(4);
    i = ov13_0224F4FC.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_0224533E[i], ov13_02245342[i], ov13_02245340[i], ov13_02245344[i]);
    WifiSetupApp_SetStateFunc(ov13_0222C5B4);
}

void ov13_0222C4C8(void) {
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

void ov13_0222C5B4(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(ov13_0222C608);
}

void ov13_0222C608(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    WifiSetupApp_SetStateFunc(ov13_0222C644);
}

void ov13_0222C644(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222C66C);
}

void ov13_0222C66C(void) {
    ov13_0222C680();
    ov13_0222C7AC();
    ov13_0222C7B0();
}

void ov13_0222C680(void) {
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
        ov13_0222C914(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80) != 0) {
        ov13_0222C914(3);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x20) != 0) {
        ov13_0222C914(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x10) != 0) {
        ov13_0222C914(2);
    }
}

void ov13_0222C7AC(void) {
}

void ov13_0222C7B0(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(ov13_0222C10C);
        break;
    case 1:
        WifiSetupSound_Play(6);
        ov13_0222C984();
        WifiSetupApp_SetStateFunc(ov13_0222C800);
        break;
    }
}

void ov13_0222C800(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222C820);
}

void ov13_0222C820(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x16, 8);
    WifiSetupApp_SetStateFunc(ov13_0222C86C);
}

void ov13_0222C86C(void) {
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
        ov13_0222B8F8(1, 0);
        WifiSetupApp_SetStateFunc(ov13_022368D8);
        break;
    case 1:
        ov13_0222B8F8(0, 0);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
        break;
    }
}

void ov13_0222C914(int dir) {
    int idx;
    if (dir == 1 || dir == 3) {
        return;
    }
    ov13_0224F4FC.selected ^= 1;
    WifiSetupSound_Play(8);
    idx = ov13_0224F4FC.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_0224533E[idx], ov13_02245342[idx], ov13_02245340[idx], ov13_02245344[idx]);
}

void ov13_0222C984(void) {
    WifiSetupBytePair tbl = ov13_0224532C;
    ov13_02239378(ov13_0224F4FC.graphics, tbl.values[ov13_0224F4FC.selected], tbl.values[ov13_0224F4FC.selected]);
}

void ov13_0222C9C4(void) {
    ov13_0224F504 = 0;
    ov13_0222CA0C();
    WifiSetupBackground_LoadHeader(0x12);
    WifiSetupMessage_Show(0x3b, -1, 0);
    WifiSetup_PrintMessage(0x17);
    WifiSetupApp_SetStateFunc(ov13_0222CA8C);
}

void ov13_0222CA0C(void) {
    WifiSetup_LoadGraphics(ov13_02246190, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222CA8C(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222CAE0);
}

void ov13_0222CAE0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    WifiSetupApp_SetStateFunc(ov13_0222CB1C);
}

void ov13_0222CB1C(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222CB44);
}

void ov13_0222CB44(void) {
    ov13_0222CB58();
    ov13_0222CB90();
    ov13_0222CB94();
}

void ov13_0222CB58(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222CB90(void) {
}

void ov13_0222CB94(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(ov13_0222CBFC);
        break;
    case 1:
        WifiSetupSound_Play(6);
        WifiSetupDialog_Open(0x18, 0, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        WifiSetupApp_SetStateFunc(ov13_0222CD38);
        break;
    }
}

void ov13_0222CBFC(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222CC1C);
}

void ov13_0222CC1C(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F504 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222CC80);
}

void ov13_0222CC80(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F504 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F504 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F504 == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222CDA8);
    }
}

void ov13_0222CD38(void) {
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
    WifiSetupApp_SetStateFunc(ov13_0222CD88);
}

void ov13_0222CD88(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222CBFC);
}

void ov13_0222CDA8(void) {
    ov13_0222CDCC();
    WifiSetup_PrintMessage(0x19);
    WifiSetupConnections_Reset();
    WifiSetupApp_SetStateFunc(ov13_0222CE4C);
}

void ov13_0222CDCC(void) {
    WifiSetup_LoadGraphics(ov13_022461A4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222CE4C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222CE80);
}

void ov13_0222CE80(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(5);
    WifiSetupApp_SetStateFunc(ov13_0222CEAC);
}

void ov13_0222CEAC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222CED4);
}

void ov13_0222CED4(void) {
    ov13_0222CEE8();
    ov13_0222CF08();
    ov13_0222CF0C();
}

void ov13_0222CEE8(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222CF08(void) {
}

void ov13_0222CF0C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(ov13_0222CF34);
}

void ov13_0222CF34(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222CF54);
}

void ov13_0222CF54(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 0x3f, 0x40);
    WifiSetupFade_Start(3, 0, 0x3f, 0x40);
    WifiSetupApp_SetStateFunc(ov13_0222CFA0);
}

void ov13_0222CFA0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

void ov13_0222CFD4(void) {
    ov13_0222D008();
    WifiSetupBackground_LoadHeader(0x11);
    WifiSetupMessage_Show(0x3a, -1, 0);
    ov13_0222D088();
    WifiSetupApp_SetStateFunc(ov13_0222D25C);
}

void ov13_0222D008(void) {
    WifiSetup_LoadGraphics(ov13_022461B8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222D088(void) {
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

void ov13_0222D25C(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222D2B0);
}

void ov13_0222D2B0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(2);
    WifiSetupApp_SetStateFunc(ov13_0222D2EC);
}

void ov13_0222D2EC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222D314);
}

void ov13_0222D314(void) {
    ov13_0222D328();
    ov13_0222D348();
    ov13_0222D34C();
}

void ov13_0222D328(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222D348(void) {
}

void ov13_0222D34C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(ov13_0222D374);
}

void ov13_0222D374(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222D394);
}

void ov13_0222D394(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222D3E4);
}

void ov13_0222D3E4(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    WifiSetupApp_SetStateFunc(ov13_0222D454);
}

void ov13_0222D454(void) {
    int idx;
    ov13_0224F508.unk_1 = 0;
    ov13_0222D51C();
    WifiSetupBackground_LoadHeader(0x10);
    WifiSetupBanner_LoadScreen(3);
    WifiSetupMessage_Show(0x39, -1, 0);
    ov13_0224F508.oam = WifiSetupOamTemplate_AllocOam(0, 0x5b);
    ov13_0224F508.oam->attr01 = (ov13_0224F508.oam->attr01 & 0xFE00FF00) | 0x84 | 0xE00000;
    ov13_0224F508.oam->attr2 = (ov13_0224F508.oam->attr2 & ~0xc00) | 0xc00;
    idx = ov13_0224F508.selected * 4;
    WifiSetupCursor_ShowCorners(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
    WifiSetupApp_SetStateFunc(ov13_0222D6B8);
}

void ov13_0222D51C(void) {
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

void ov13_0222D6B8(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    WifiSetupApp_SetStateFunc(ov13_0222D70C);
}

void ov13_0222D70C(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    WifiSetupApp_SetStateFunc(ov13_0222D748);
}

void ov13_0222D748(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222D770);
}

void ov13_0222D770(void) {
    ov13_0222D784();
    ov13_0222D878();
    ov13_0222D87C();
}

void ov13_0222D784(void) {
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
        ov13_0222DAB0(1);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80) != 0) {
        ov13_0222DAB0(3);
    }
}

void ov13_0222D878(void) {
}

void ov13_0222D87C(void) {
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
        ov13_0222DB40();
        ov13_0224F508.unk_1 = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222D920);
}

void ov13_0222D920(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222D940);
}

void ov13_0222D940(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (ov13_0224F508.unk_1 != 0) {
        WifiSetupButtonBar_Close();
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    WifiSetupApp_SetStateFunc(ov13_0222D9A4);
}

void ov13_0222D9A4(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupOam_Release(ov13_0224F508.oam);
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    WifiSetupFile_Free(ov13_0224F508.graphics);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    if (ov13_0224F508.unk_1 == 0) {
        ov13_0222B8F8(0, 0);
        WifiSetupApp_SetStateFunc(ov13_0222C44C);
        return;
    }
    switch (ov13_0224F508.selected) {
    case 0:
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222CFD4);
        break;
    case 1:
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222C9C4);
        break;
    case 2:
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222E220);
        break;
    }
}

void ov13_0222DAB0(int dir) {
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

void ov13_0222DB40(void) {
    ov13_02239378(ov13_0224F508.graphics, ov13_02245364[ov13_0224F508.selected], ov13_02245364[ov13_0224F508.selected]);
}

void ov13_0222DB6C(void) {
    ov13_0222DBA4();
    WifiSetup_PrintMessage(0x1e);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSignalIcon_Create(1);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(ov13_0222DC24);
}

void ov13_0222DBA4(void) {
    WifiSetup_LoadGraphics(ov13_02246288, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222DC24(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222DC58);
}

void ov13_0222DC58(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_0222BC84((WifiSetupSessionCallback)ov13_0222DD5C);
    WifiSetupApp_SetStateFunc(ov13_0222DC88);
}

void ov13_0222DC88(void) {
    ov13_0222DC98();
    ov13_0222DC9C();
}

void ov13_0222DC98(void) {
}

void ov13_0222DC9C(void) {
}

void ov13_0222DCA0(void) {
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222DCC8);
}

void ov13_0222DCC8(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupSignalIcon_Destroy();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    if (ov13_0224F514 == 0) {
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else if (ov13_0224F514 == 2) {
        WifiSetupApp_SetStateFunc(ov13_0222E56C);
    } else {
        WifiSetupApp_SetStateFunc(ov13_0222DDE4);
    }
}

void ov13_0222DD5C(int a) {
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
    ov13_0222BC84(NULL);
    ov13_0222BC0C();
    WifiSetupApp_SetStateFunc(ov13_0222DCA0);
}

void ov13_0222DDE4(void) {
    ov13_0222DE04();
    WifiSetup_PrintMessage(0x1f);
    WifiSetupApp_SetStateFunc(ov13_0222DE6C);
}

void ov13_0222DE04(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222DE6C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222DEA0);
}

void ov13_0222DEA0(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(5);
    WifiSetupApp_SetStateFunc(ov13_0222DECC);
}

void ov13_0222DECC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222DEF4);
}

void ov13_0222DEF4(void) {
    ov13_0222DF08();
    ov13_0222DF28();
    ov13_0222DF2C();
}

void ov13_0222DF08(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222DF28(void) {
}

void ov13_0222DF2C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(ov13_0222DF54);
}

void ov13_0222DF54(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222DF74);
}

void ov13_0222DF74(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 0x3f, 0x40);
    WifiSetupFade_Start(3, 0, 0x3f, 0x40);
    WifiSetupApp_SetStateFunc(ov13_0222DFC0);
}

void ov13_0222DFC0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

void ov13_0222DFF4(void) {
    ov13_0222E014();
    WifiSetup_PrintMessage(0x1a);
    WifiSetupApp_SetStateFunc(ov13_0222E07C);
}

void ov13_0222E014(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E07C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E0B0);
}

void ov13_0222E0B0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(4);
    WifiSetupApp_SetStateFunc(ov13_0222E0EC);
}

void ov13_0222E0EC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E114);
}

void ov13_0222E114(void) {
    ov13_0222E128();
    ov13_0222E148();
    ov13_0222E14C();
}

void ov13_0222E128(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E148(void) {
}

void ov13_0222E14C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(ov13_0222E174);
}

void ov13_0222E174(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E194);
}

void ov13_0222E194(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E1D0);
}

void ov13_0222E1D0(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    WifiSetupApp_SetStateFunc(ov13_0222EEB4);
}

void ov13_0222E220(void) {
    ov13_0224F518 = 0;
    ov13_0222E268();
    WifiSetupBackground_LoadHeader(0x13);
    WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetup_PrintMessage(0x1b);
    WifiSetupApp_SetStateFunc(ov13_0222E2E8);
}

void ov13_0222E268(void) {
    WifiSetup_LoadGraphics(ov13_0224629C, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E2E8(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E33C);
}

void ov13_0222E33C(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    WifiSetupApp_SetStateFunc(ov13_0222E368);
}

void ov13_0222E368(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E390);
}

void ov13_0222E390(void) {
    ov13_0222E3A4();
    ov13_0222E3DC();
    ov13_0222E3E0();
}

void ov13_0222E3A4(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E3DC(void) {
}

void ov13_0222E3E0(void) {
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
    WifiSetupApp_SetStateFunc(ov13_0222E430);
}

void ov13_0222E430(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E450);
}

void ov13_0222E450(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F518 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E4B4);
}

void ov13_0222E4B4(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F518 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F518 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F518 == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222DFF4);
    }
}

void ov13_0222E56C(void) {
    ov13_0224F51C = 0;
    ov13_0222E59C();
    WifiSetup_PrintMessage(0x21);
    WifiSetupApp_SetStateFunc(ov13_0222E608);
}

void ov13_0222E59C(void) {
    WifiSetup_LoadGraphics(ov13_022462B0, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E608(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E63C);
}

void ov13_0222E63C(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222E66C);
}

void ov13_0222E66C(void) {
    ov13_0222E720();
    ov13_0222E67C();
}

void ov13_0222E67C(void) {
}

void ov13_0222E680(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E6BC);
}

void ov13_0222E6BC(void) {
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
    ov13_0222B8F8(0, 1);
    WifiSetupApp_SetStateFunc(ov13_0222D454);
}

void ov13_0222E720(void) {
    if (++ov13_0224F51C < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222E680);
}

void ov13_0222E754(void) {
    ov13_0222E790();
    WifiSetup_PrintMessage(0x20);
    ov13_0224F520[0] = WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetupApp_SetStateFunc(ov13_0222E810);
}

void ov13_0222E790(void) {
    WifiSetup_LoadGraphics(ov13_022462C4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E810(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    if (ov13_0224F520[0] != 0) {
        WifiSetupFade_Start(2, 1, 1, 8);
        WifiSetupDisplay_ShowPlanes(1, 1);
    }
    WifiSetupApp_SetStateFunc(ov13_0222E878);
}

void ov13_0222E878(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    WifiSetupApp_SetStateFunc(ov13_0222E8B4);
}

void ov13_0222E8B4(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E8E8);
}

void ov13_0222E8E8(void) {
    ov13_0222E8FC();
    ov13_0222E934();
    ov13_0222E938();
}

void ov13_0222E8FC(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E934(void) {
}

void ov13_0222E938(void) {
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
    WifiSetupApp_SetStateFunc(ov13_0222E990);
}

void ov13_0222E990(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E9B0);
}

void ov13_0222E9B0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F520[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222EA14);
}

void ov13_0222EA14(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F520[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F520[1] == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222EEB4);
    }
}

void ov13_0222EABC(void) {
    u16 buf[11];
    u8 *src = ov13_0222BCA8();
    WifiSetupCanvas *win = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F524[1] = 0;
    ov13_0224F524[0] = 0;
    ov13_0222EB8C();
    MIi_CpuClear16(0, buf, sizeof(buf));
    MIi_CpuCopy16((u16 *)(src + 2), buf, src[1] * 2);
    WifiSetupText_DrawInRect(win, 8, 0x35, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
    ov13_0222BC84((WifiSetupSessionCallback)ov13_0222EE80);
    WifiSetupDialog_Open(0x1d, 4, 0, -1, 0);
    WifiSetupApp_SetStateFunc(ov13_0222EC0C);
}

void ov13_0222EB8C(void) {
    WifiSetup_LoadGraphics(ov13_022462D8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222EC0C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222EC40);
}

void ov13_0222EC40(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222EC64);
}

void ov13_0222EC64(void) {
    if (WifiSetupDialog_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222EC88);
}

void ov13_0222EC88(void) {
    ov13_0222ECF8();
    ov13_0222ECFC();
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
    ov13_0222BC84(NULL);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(ov13_0222ED00);
}

void ov13_0222ECF8(void) {
}

void ov13_0222ECFC(void) {
}

void ov13_0222ED00(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    if (ov13_0224F524[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222ED5C);
}

void ov13_0222ED5C(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F524[0] == 0 && ov13_0224F524[1] == 1) {
        ov13_0222BC9C();
    } else {
        ov13_0222BC0C();
    }
    WifiSetupApp_SetStateFunc(ov13_0222EDB8);
}

void ov13_0222EDB8(void) {
    if (ov13_0224F524[0] != 0 || ov13_0224F524[1] == 0) {
        if (ov13_0222BC68() == 0) {
            return;
        }
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F524[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    ov13_0222B8F8(0, 1);
    if (ov13_0224F524[0] != 0) {
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else if (ov13_0224F524[1] == 0) {
        WifiSetupDisplay_HidePlanes(1, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        WifiSetupApp_SetStateFunc(ov13_0222DB6C);
    }
}

void ov13_0222EE80(void) {
    ov13_0224F524[0] = 1;
}

int ov13_0222EE94(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void ov13_0222EEB4(void) {
    ov13_0222BB20((WifiSetupSessionCallback)ov13_0222F230);
    ov13_0224F528 = 0;
    ov13_0222EF00();
    WifiSetup_PrintMessage(0x1c);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(ov13_0222EF80);
}

void ov13_0222EF00(void) {
    WifiSetup_LoadGraphics(ov13_022462EC, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222EF80(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222EFB4);
}

void ov13_0222EFB4(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(1);
    WifiSetupApp_SetStateFunc(ov13_0222EFE0);
}

void ov13_0222EFE0(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F004);
}

void ov13_0222F004(void) {
    ov13_0222F018();
    ov13_0222F050();
    ov13_0222F054();
}

void ov13_0222F018(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_0222EE94() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222F050(void) {
}

void ov13_0222F054(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(ov13_0222F080);
}

void ov13_0222F080(void) {
    if (ov13_0224F528 == 0) {
        ov13_0222BC0C();
    }
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222F0B8);
}

void ov13_0222F0B8(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (ov13_0222BC68() == 0) {
            return;
        }
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F528 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F138);
}

void ov13_0222F138(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (ov13_0222BC68() == 0) {
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
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else if (ov13_0224F528 == 2) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222EABC);
    }
}

void ov13_0222F230(int a) {
    WifiSetupSound_Stop();
    if (a == 0) {
        ov13_0224F528 = 1;
        WifiSetupSound_Play(0x10);
    } else {
        ov13_0224F528 = 2;
        WifiSetupSound_Play(0x12);
    }
    ov13_0222BC84(NULL);
    WifiSetupApp_SetStateFunc(ov13_0222F080);
}

int ov13_0222F28C(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void ov13_0222F2AC(void) {
    ov13_0224F52C.counter = 0;
    ov13_0222F30C();
    ov13_02227E40();
    WifiSetupMessage_Show(0x36, -1, 0);
    ov13_02227964(2);
    WifiSetup_PrintMessage(0x22);
    WifiSetupBusyIcon_Create(0);
    WifiSetupApp_StartClient();
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(ov13_0222F3B4);
}

void ov13_0222F30C(void) {
    WifiSetup_LoadGraphics(ov13_02246300, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246318, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246330, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222F3B4(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222F408);
}

void ov13_0222F408(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(1);
    WifiSetupApp_SetStateFunc(ov13_0222F444);
}

void ov13_0222F444(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    ov13_0224F52C.task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)ov13_0222F764, 0, 0x78);
    WifiSetupApp_SetStateFunc(ov13_0222F48C);
}

void ov13_0222F48C(void) {
    ov13_0222F560();
    ov13_0222F598();
    ov13_0222F59C();
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
        WifiSetupApp_SetStateFunc(ov13_0222F7CC);
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
        WifiSetupApp_SetStateFunc(ov13_0222F780);
        break;
    }
}

void ov13_0222F560(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_0222F28C() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222F598(void) {
}

void ov13_0222F59C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    if (ov13_0224F52C.task == NULL) {
        return;
    }
    WifiSetupTask_DestroyLater(1, ov13_0224F52C.task);
    ov13_0224F52C.task = NULL;
    WifiSetupButtonBar_Lock();
    WifiSetupApp_SetStateFunc(ov13_0222F818);
}

void ov13_0222F5E8(void) {
    WifiSetupButtonBar_Lock();
    if (ov13_0224F52C.task != NULL) {
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
    }
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222F624);
}

void ov13_0222F624(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F52C.flag == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F688);
}

void ov13_0222F688(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F52C.flag == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
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
        ov13_0222B8F8(2, 1);
        WifiSetupApp_SetStateFunc(ov13_02237618);
    } else {
        ov13_0222B8F8(0, 0);
        WifiSetupApp_SetStateFunc(ov13_0222F848);
    }
}

void ov13_0222F764(void) {
    WifiSetupInput_Update();
    WifiSetupTask_RunQueue(0);
    ov13_0222F560();
    ov13_0222F59C();
}

void ov13_0222F780(void) {
    if (WifiSetupDialog_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(ov13_0222F7AC);
}

void ov13_0222F7AC(void) {
    if (WifiSetupDialog_IsOpen() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F7CC(void) {
    ov13_0222F560();
    ov13_0222F598();
    ov13_0222F59C();
    ov13_0224F52C.counter++;
    if (ov13_0224F52C.counter < 0x438) {
        return;
    }
    WifiSetupSound_Stop();
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F818(void) {
    WifiSetupSound_Stop();
    WifiSetupSound_Play(7);
    ov13_0224F52C.flag = 0;
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F848(void) {
    ov13_0224F534 = 0;
    ov13_0222F884();
    ov13_02227E40();
    WifiSetup_PrintMessage(0x23);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(ov13_0222F8F0);
}

void ov13_0222F884(void) {
    WifiSetup_LoadGraphics(ov13_02246344, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222F8F0(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222F924);
}

void ov13_0222F924(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F948);
}

void ov13_0222F948(void) {
    ov13_0222FA08();
    ov13_0222F958();
}

void ov13_0222F958(void) {
}

void ov13_0222F95C(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F998);
}

void ov13_0222F998(void) {
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
    ov13_0222B8F8(0, 0);
    ov13_0222B934(0, 1);
    WifiSetupApp_SetStateFunc(ov13_02238440);
}

void ov13_0222FA08(void) {
    if (++ov13_0224F534 < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F95C);
}

void WifiSetupListScreen_Init(void) {
    int result;
    gWifiSetupListScreen.work = WifiSetupHeap_AllocZeroed(0x5c, 4);
    ov13_0222B948(&result, 0);
    if (result == 0) {
        gWifiSetupListScreen.selected = 0;
        gWifiSetupListScreen.position = 0;
    }
    gWifiSetupListScreen.work->count = WifiSetupApScan_GetEntries(gWifiSetupListScreen.work);
    WifiSetupListScreen_LoadGraphics();
    ov13_02227E40();
    WifiSetupMessage_Show(0x34, -1, 0);
    ov13_02227964(2);
    ov13_0222FC68();
    ov13_0222FD10();
    gWifiSetupListScreen.work->buffer = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupListScreen.work->unk_3C = WifiSetupTask_Create(1, (WifiSetupTaskFunc)ov13_02230C68, 0, 0x6e);
    ov13_022307AC();
    ov13_02230CC4();
    WifiSetupApp_SetStateFunc(ov13_0222FE84);
}

void WifiSetupListScreen_LoadGraphics(void) {
    WifiSetupPath25 a;
    WifiSetupPath22 b;
    a = ov13_022453E3;
    b = ov13_022453CD;
    WifiSetup_LoadGraphics(ov13_02246358, GX_LoadBG3Scr);
    gWifiSetupListScreen.work->unk_04 = WifiSetupFile_Load(WifiSetup_LocalizePath(a.text), 0, 4);
    ov13_02239228(gWifiSetupListScreen.work->unk_04);
    ov13_022392B4();
    gWifiSetupListScreen.work->unk_08 = WifiSetupFile_Load(WifiSetup_LocalizePath(b.text), 0, 4);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    REG16(0x0400000A) = (u16)((REG16(0x0400000A) & ~3) | 2);
    SET_PRI3(0x0400000C);
    REG16(0x0400000E) = (u16)((REG16(0x0400000E) & ~3) | 2);
}

void ov13_0222FC68(void) {
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
    ov13_0223943C(mode, 0x55, 0xec, 0x3f, val);
}

void ov13_0222FD10(void) {
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

void ov13_0222FE84(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x1d, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x1d);
    WifiSetupApp_SetStateFunc(ov13_0222FED8);
}

void ov13_0222FED8(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    WifiSetupApp_SetStateFunc(ov13_0222FF14);
}

void ov13_0222FF14(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222FF3C);
}

void ov13_0222FF3C(void) {
    ov13_0222FF50();
    ov13_022301A4();
    ov13_02230414();
}

void ov13_0222FF50(void) {
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
                ov13_02230CC4();
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
                ov13_02230CC4();
                return;
            }
        }
    }
    if (WifiSetupInput_IsKeyPressed(1)) {
        WifiSetupButtonBar_SelectIfNone(1);
        ov13_0223959C();
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x200)) {
        ov13_02230ED0();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x200)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x100)) {
        ov13_02230E5C();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x100)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x40)) {
        ov13_02230D10(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x40)) {
        gWifiSetupListScreen.work->unk_59 = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80)) {
        ov13_02230D10(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x80)) {
        gWifiSetupListScreen.work->unk_59 = 0;
    }
}

void ov13_022301A4(void) {
    if (gWifiSetupListScreen.work->task) {
        return;
    }
    if (gWifiSetupListScreen.work->delay) {
        gWifiSetupListScreen.work->delay--;
    }
    switch (ov13_02239564()) {
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
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * ov13_02239550(), gWifiSetupListScreen.work->unk_53);
        ov13_022307AC();
        gWifiSetupListScreen.work->delay = 4;
        break;
    case 3: {
        int mod;
        gWifiSetupListScreen.work->unk_57 = 0;
        WifiSetupButtonBar_Unlock();
        gWifiSetupListScreen.position = FX_DivS32(gWifiSetupListScreen.work->total * ov13_02239550(), gWifiSetupListScreen.work->unk_53);
        WifiSetupSound_Play(19);
        ov13_022307AC();
        mod = FX_ModS32(gWifiSetupListScreen.position, 28);
        if (mod == 0) {
            ov13_02230CC4();
        } else if (mod < 14) {
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230B14, 0, 120);
        } else {
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230BD0, 0, 120);
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
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230B14, 0, 120);
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
            gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230BD0, 0, 120);
        }
        break;
    case 5:
    case 7:
        gWifiSetupListScreen.work->unk_58 = 0;
        break;
    }
}

void ov13_02230414(void) {
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
            ov13_02230F54();
        } else {
            index = FX_DivS32(gWifiSetupListScreen.position, 28) + gWifiSetupListScreen.selected;
            s = gWifiSetupListScreen.work;
            if (s->rows[index].kind == 2) {
                WifiSetupSound_Play(9);
                ov13_0223959C();
                WifiSetupButtonBar_Lock();
                WifiSetupDialog_Open(14, 1, 1, -1, 0);
                WifiSetupApp_SetStateFunc(ov13_02230F84);
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
    WifiSetupApp_SetStateFunc(ov13_02230520);
}

void ov13_02230520(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_02230540);
}

void ov13_02230540(void) {
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
    WifiSetupApp_SetStateFunc(ov13_022305B0);
}

void ov13_022305B0(void) {
    int i;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!ov13_022272A0()) {
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
    ov13_02239514();
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    ov13_02239288();
    WifiSetupFile_Free(gWifiSetupListScreen.work->unk_04);
    WifiSetupFile_Free(gWifiSetupListScreen.work->unk_08);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 29);
    *(volatile u32 *)0x04000010 = 0;
    *(volatile u32 *)0x04000018 = 0;
    if (!gWifiSetupListScreen.work->unk_54) {
        ov13_02239B34();
        WifiSetupConnections_Edit(WifiSetupConnections_GetEdit()[244]);
        ov13_0222B8F8(2, 0);
        WifiSetupApp_SetStateFunc(ov13_02237618);
    } else if (gWifiSetupListScreen.selected == 4) {
        ov13_02239B34();
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_022363F4);
    } else {
        WifiSetupConnections_SetSsid(&gWifiSetupListScreen.work->rows[gWifiSetupListScreen.work->index]);
        ov13_0222B8F8(0, 0);
        if (gWifiSetupListScreen.work->rows[gWifiSetupListScreen.work->index].text[40]) {
            ov13_0222B8F8(0, 1);
            ov13_0222B934(1, 1);
            WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Init);
        } else {
            ov13_0222B8F8(0, 1);
            ov13_0222B934(0, 1);
            WifiSetupApp_SetStateFunc(ov13_02238440);
        }
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F53C);
}

void ov13_022307AC(void) {
    int start = FX_DivS32(gWifiSetupListScreen.position, 28), count = gWifiSetupListScreen.work->count, row, i, second;
    WifiSetupText_Clear(gWifiSetupListScreen.work->buffer, 0);
    if (count > 5) {
        count = 5;
    }
    row = start;
    i = 0;
    if (count > 0) {
        do {
            ov13_02230854(row, i);
            ++i;
            ++row;
        } while (i < count);
    }
    second = start;
    row = 0;
    if (count > 0) {
        do {
            ov13_022309CC(second, row);
            ++row;
            ++second;
        } while (row < count);
    }
    WifiSetupText_MarkDirty(gWifiSetupListScreen.work->buffer);
    ov13_02230A58();
}

void ov13_02230854(int index, int row) {
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

void ov13_022309CC(int a, int b) {
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

void ov13_02230A58(void) {
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

void ov13_02230B14(void *task) {
    int rem;
    ov13_0223959C();
    WifiSetupCursor_Hide();
    if (gWifiSetupListScreen.position > 4) {
        gWifiSetupListScreen.position -= 4;
    } else {
        gWifiSetupListScreen.position = 0;
    }
    rem = FX_ModS32(gWifiSetupListScreen.position, 28);
    if (rem == 24) {
        ov13_022307AC();
        return;
    }
    if (rem > 24) {
        gWifiSetupListScreen.position += 28 - rem;
        rem = 0;
    }
    ov13_02230A58();
    if (rem) {
        return;
    }
    ov13_02239578(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->unk_53, gWifiSetupListScreen.work->total));
    ov13_02239584();
    ov13_02230CC4();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

void ov13_02230BD0(void *task) {
    int rem;
    ov13_0223959C();
    WifiSetupCursor_Hide();
    gWifiSetupListScreen.position += 4;
    rem = FX_ModS32(gWifiSetupListScreen.position, 28);
    if (rem >= 4) {
        ov13_02230A58();
        return;
    }
    gWifiSetupListScreen.position -= rem;
    ov13_022307AC();
    ov13_02239578(FX_DivS32(gWifiSetupListScreen.position * gWifiSetupListScreen.work->unk_53, gWifiSetupListScreen.work->total));
    ov13_02239584();
    ov13_02230CC4();
    gWifiSetupListScreen.work->task = 0;
    WifiSetupTask_DestroyLater(0, task);
}

void ov13_02230C68(void) {
    int x;
    if (!gWifiSetupListScreen.work->unk_56) {
        return;
    }
    x = ((FX_ModS32(gWifiSetupListScreen.position, 28) - 50) << 16) & 0x01ff0000;
    *(vu32 *)0x04000010 = x;
    *(vu32 *)0x04000018 = x;
    gWifiSetupListScreen.work->unk_56 = 0;
}

void ov13_02230CC4(void) {
    unsigned x = gWifiSetupListScreen.selected;
    WifiSetupCursor_ShowPair(x < 4 ? 2 : 3, ov13_02245424[x * 4], ov13_02245428[x * 4], ov13_02245426[x * 4]);
}

void ov13_02230D10(int input) {
    int changed = 1;
    int state = gWifiSetupListScreen.selected;
    switch (state) {
    case 0:
        if (input == 1) {
            if (gWifiSetupListScreen.position == 0) {
                gWifiSetupListScreen.selected = 4;
            } else {
                WifiSetupSound_Play(19);
                gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230B14, 0, 120);
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
            ov13_02230ED0();
            return;
        }
        break;
    case 4:
        if (input == 1) {
            changed = 0;
        } else {
            gWifiSetupListScreen.position = 0;
            gWifiSetupListScreen.selected = 0;
            ov13_022307AC();
            ov13_02239578(0);
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
    ov13_02230CC4();
}

void ov13_02230E5C(void) {
    if (!gWifiSetupListScreen.position) {
        if (gWifiSetupListScreen.work->unk_59) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->unk_59 = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230B14, 0, 120);
}

void ov13_02230ED0(void) {
    if (gWifiSetupListScreen.position == gWifiSetupListScreen.work->total || gWifiSetupListScreen.work->count <= 4) {
        if (gWifiSetupListScreen.work->unk_59) {
            return;
        }
        WifiSetupSound_Play(9);
        gWifiSetupListScreen.work->unk_59 = 1;
        return;
    }
    WifiSetupSound_Play(19);
    gWifiSetupListScreen.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_02230BD0, 0, 120);
}

void ov13_02230F54(void) {
    volatile u8 local;
    u8 x = ov13_022453CC[0];
    local = x;
    ov13_02239378(gWifiSetupListScreen.work->unk_08, x, x);
}

void ov13_02230F84(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(ov13_02230FB0);
}

void ov13_02230FB0(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_02239584();
    WifiSetupApp_SetStateFunc(ov13_0222FF3C);
}

void WifiSetupTextEntry_Init(void) {
    WifiSetupBytePair p = *(const WifiSetupBytePair *)ov13_02245454;
    int a, b;
    gWifiSetupTextEntry = WifiSetupHeap_AllocZeroed(44, 4);
    ov13_0222B948(&a, &b);
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
    ov13_02227964(2);
    gWifiSetupTextEntry->canvas = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupTextEntry->cursor = WifiSetupOamTemplate_AllocOam(0, 62);
    {
        GXOamAttr *q = gWifiSetupTextEntry->cursor;
        q->attr2 = (q->attr2 & ~0xc00) | 0xc00;
    }
    ov13_022317A8();
    ov13_022316B4();
    WifiSetupApp_SetStateFunc(ov13_022311C8);
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

void ov13_022311C8(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 21, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 21);
    WifiSetupApp_SetStateFunc(ov13_0223121C);
}

void ov13_0223121C(void) {
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
    WifiSetupApp_SetStateFunc(ov13_02231298);
}

void ov13_02231298(void) {
    if (WifiSetupKeyboard_GetInput() == 255) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_022312B8);
}

void ov13_022312B8(void) {
    ov13_022312C8();
    ov13_02231488();
}

void ov13_022312C8(void) {
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
        WifiSetupApp_SetStateFunc(ov13_0223148C);
        return;
    case 131:
        if (ov13_0223191C()) {
            WifiSetupSound_Play(6);
            gWifiSetupTextEntry->flag = 1;
        } else {
            gWifiSetupTextEntry->flag = 2;
            WifiSetupSound_Play(9);
        }
        gWifiSetupTextEntry->cursor->attr01 = (gWifiSetupTextEntry->cursor->attr01 & 0xc1fffcff) | 0x200;
        WifiSetupApp_SetStateFunc(ov13_0223148C);
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
    ov13_022316B4();
    ov13_022317A8();
}

void ov13_02231488(void) {
}

void ov13_0223148C(void) {
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_022314A8);
}

void ov13_022314A8(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupKeyboard_Close();
    WifiSetupSound_Play(21);
    WifiSetupApp_SetStateFunc(ov13_022314D8);
}

void ov13_022314D8(void) {
    u32 opts[2];
    int x;
    opts[0] = ov13_02245454[2];
    opts[1] = ov13_02245454[3];
    if (WifiSetupKeyboard_Exists()) {
        return;
    }
    if (gWifiSetupTextEntry->flag == 0) {
        WifiSetupApp_SetStateFunc(ov13_022315B0);
        return;
    }
    if (gWifiSetupTextEntry->flag == 2) {
        WifiSetupDialog_Open(6, 3, 1, -1, 0);
        WifiSetupApp_SetStateFunc(ov13_02231AA8);
        return;
    }
    ov13_0222B948(0, &x);
    WifiSetupDialog_Open(opts[x], 2, 1, -1, 0);
    WifiSetupApp_SetStateFunc(ov13_02231810);
}

void ov13_022315B0(void) {
    int a, b;
    WifiSetupMessage_Hide();
    WifiSetupOam_Release(gWifiSetupTextEntry->cursor);
    WifiSetupText_FreeBgCanvas(0);
    WifiSetup_LoadGraphics(ov13_022463C8, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 21);
    ov13_0222B948(&a, &b);
    if (b == 0) {
        ov13_0222B8F8(2, 1);
        ov13_0222B934(0, a);
        WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
    } else if (gWifiSetupTextEntry->flag == 0) {
        ov13_0222B8F8(0, 1);
        ov13_0222B934(1, 0);
        WifiSetupApp_SetStateFunc(WifiSetupListScreen_Init);
    } else {
        ov13_0222B8F8(0, 0);
        ov13_0222B934(0, 1);
        WifiSetupApp_SetStateFunc(ov13_02238440);
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F53C + 4);
}

void ov13_022316B4(void) {
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

void ov13_022317A8(void) {
    unsigned x = gWifiSetupTextEntry->length;
    GXOamAttr *p = gWifiSetupTextEntry->cursor;
    int col = x & 15, row = (int)x >> 4;
    if (x >= 32) {
        row = 1;
        col = 15;
    }
    p->attr01 = (p->attr01 & 0xfe00ff00) | ov13_0224546C[row] | ((u32)ov13_02245470[col] << 23 >> 7);
}

void ov13_02231810(void) {
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
    WifiSetupApp_SetStateFunc(ov13_0223186C);
}

void ov13_0223186C(void) {
    void (*opts[2])(u8 *);
    int x;
    opts[0] = (void (*)(u8 *))ov13_02245454[4];
    opts[1] = (void (*)(u8 *))ov13_02245454[5];
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    if (gWifiSetupTextEntry->flag == 0) {
        gWifiSetupTextEntry->cursor->attr01 &= 0xc1fffcff;
        WifiSetupApp_SetStateFunc(ov13_0223121C);
        return;
    }
    ov13_0222B948(&x, 0);
    opts[x](gWifiSetupTextEntry->text);
    WifiSetupApp_SetStateFunc(ov13_022315B0);
}

int ov13_0223191C(void) {
    int a, b, len;
    ov13_0222B948(&a, &b);
    if (b == 1) {
        WifiSetupMessage_Show(0x35, -1, 0);
    }
    ov13_0222B948(&a, &b);
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

void ov13_02231AA8(void) {
    if (WifiSetupDialog_GetSelected()) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(ov13_02231AD4);
}

void ov13_02231AD4(void) {
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    gWifiSetupTextEntry->cursor->attr01 &= 0xc1fffcff;
    WifiSetupApp_SetStateFunc(ov13_0223121C);
}
