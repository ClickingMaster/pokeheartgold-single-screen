#pragma force_active on
#include "wifi_setup_settings.h"

#include <nitro/fx/fx_cp.h>

#include "global.h"

#include "wifi_setup_app.h"
#include "wifi_setup_keyboard.h"
#include "wifi_setup_screens.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Connection settings screen (0x02232E04-0x022347E4). The item handlers, the scroll tasks and the
// drawing tasks follow in src/wifi_setup_auto_setup.c.

// Setup record of the access point being edited (WifiSetupConnections_GetEdit); only the members used here.
typedef struct WifiSetupSettingsRecord {
    u8 unk_00[0x40];
    u8 ssid[0xA6];    // 0x40
    u8 keyType : 2;   // 0xE6, 0 none, 1 WEP 40, 2 WEP 104, 3 WEP 128
    u8 keyFormat : 6; // 1 = text, half the length of the hex key
    u8 unk_E7[14];
    u8 autoAddress; // 0xF5
    u8 autoDns;     // 0xF6
} WifiSetupSettingsRecord;

typedef struct WifiSetupGraphicsPath21 {
    char text[21];
} WifiSetupGraphicsPath21;

typedef struct WifiSetupGraphicsPath22 {
    char text[22];
} WifiSetupGraphicsPath22;

typedef struct WifiSetupGraphicsPath23 {
    char text[23];
} WifiSetupGraphicsPath23;

extern const WifiSetupGraphicsPath21 gWifiSetupSettingsYbBgStep2NclPath;
extern const WifiSetupGraphicsPath22 gWifiSetupSettingsYbBgStep21NclPath;
extern const WifiSetupGraphicsPath23 gWifiSetupSettingsJb3ListBackNscPath;
extern u8 gWifiSetupSettingsButtonTemplates[][3]; // OAM template per button type and option
extern u16 gWifiSetupSettingsRowTileOffsets[];   // top screen tile offset per first visible item
extern WifiSetupRect gWifiSetupSettingsTouchRects[];
extern s8 gWifiSetupSettingsDigitPositions[];
extern u8 gWifiSetupSettingsRowItems[]; // item per row touched
extern u8 gWifiSetupSettingsItemRows[];
extern WifiSetupRect gWifiSetupInputPanelRect;
extern u8 gWifiSetupSettingsYbObjKbNclPath[];
extern u8 gWifiSetupSettingsJbBgStep2NcgPath[];
extern u8 gWifiSetupSettingsJbBgStep21NcgPath[];
extern u8 gWifiSetupSettingsJb3ListNscPath[];
extern u8 gWifiSetupSettingsYbObjMainNclPath[];
extern u16 gWifiSetupSettingsAddressFormat[]; // address format
extern u32 ov13_0224F554[]; // &gWifiSetupSettings.work

// src/wifi_setup_ui_parts.c; WifiSetupMessage_Hide is called with an argument
extern int WifiSetupMessage_Show();
extern int WifiSetupMessage_Hide();
extern void WifiSetupBanner_Show(int);
extern void WifiSetupBackground_LoadSlotListHeader(void);
extern void WifiSetup_LoadGraphics(const void *path, void *load);
extern void *WifiSetup_LocalizePath(const void *path);
extern void WifiSetupCursor_Hide(void);

extern u8 *WifiSetupConnections_GetEdit(); // also called with arguments
extern void WifiSetupBgScreen_Create(void *);
extern void WifiSetupBgScreen_MarkDirty(void);
extern void WifiSetupBgScreen_CopyRect(void *, int, int, int);
extern void WifiSetupScrollBar_Create(int, int, int, int, int);
extern void WifiSetupBgScreen_Destroy(void);
extern void WifiSetupScrollBar_Destroy(void);
extern int WifiSetupScrollBar_GetValue(void);
extern int WifiSetupScrollBar_GetEvent(void);
extern int WifiSetupScrollBar_Disable();
extern int WifiSetupConnections_SetAutoAddress();
extern int WifiSetupConnections_SetAutoDns();
extern void WifiSetupConnections_Edit(int);
extern int WifiSetupDialog_Open();
extern int DWCi_BACKUPlConvMaskCidr(void *);
extern int swprintf(u16 *, u32, const u16 *, ...);
extern s32 FX_DivS32(s32, s32);
extern s32 FX_ModS32(s32, s32);

extern int WifiSetupSettings_GetItemRow();
extern int WifiSetupSettings_GetRowItem();
extern void WifiSetupSettings_ScrollUpTask(void);
extern void WifiSetupSettings_ScrollDownTask(void);
extern void WifiSetupSettings_ScrollTask(void);
extern void WifiSetupSettings_ShowCursor(void);
extern void WifiSetupSettings_UpdateCursorItem(void);
extern int WifiSetupSettings_SetCursorItem();
extern void WifiSetupSettings_MoveCursor(int);
extern void WifiSetupSettings_ButtonTask(void);
extern int WifiSetupSettings_HighlightButton();
extern int WifiSetupSettings_PressButton();
extern int WifiSetupSettings_IsComplete();
extern int WifiSetupSettings_WaitIncompleteDialog();
extern void WifiSetupSaveScreen_Init(void);
extern void WifiSetupConnectionList_Init(void);
extern void WifiSetupMethodMenu_Init(void);
extern void WifiSetupConnectionTest_Init(void);
extern void WifiSetupAddressEntry_Init(void);

void WifiSetupSettings_Init(void) {
    u8 *record = WifiSetupConnections_GetEdit();
    int i;
    gWifiSetupSettings.work = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupSettingsWork), 4);
    gWifiSetupSettings.work->autoDns = record[0xF6];
    gWifiSetupSettings.work->lastButton = 12;
    WifiSetupSettings_ResetPosition();
    WifiSetupSettings_LoadGraphics();
    if (!gWifiSetupSettings.fromMenu) {
        WifiSetupMessage_Show(48, gWifiSetupSettingsDigitPositions[WifiSetupApp_GetLanguage()], record[0xF4] + 1);
    } else {
        WifiSetupMessage_Show(69, -1, 0);
    }
    WifiSetupBanner_Show(1);
    WifiSetupBackground_LoadSlotListHeader();
    WifiSetupScrollBar_Create(2, 85, 241, 65, (gWifiSetupSettings.scroll * 55) / WIFI_SETUP_SETTINGS_MAX_SCROLL);
    gWifiSetupSettings.work->canvas = WifiSetupText_InitBgCanvas(0, 1);
    for (i = 0; i < 7; i++) { // rows and the two switches
        gWifiSetupSettings.work->rows[i] = WifiSetupOamTemplate_AllocOam(0, 41);
    }
    switch (record[0xE7]) {
    case 1:
        gWifiSetupSettings.work->ssidIcon = WifiSetupOamTemplate_AllocOam(0, 80);
        gWifiSetupSettings.work->ssidIcon->attr2 = (gWifiSetupSettings.work->ssidIcon->attr2 & ~0xc00) | 0xc00;
        break;
    case 2:
        gWifiSetupSettings.work->ssidIcon = WifiSetupOamTemplate_AllocOam(0, 81);
        gWifiSetupSettings.work->ssidIcon->attr2 = (gWifiSetupSettings.work->ssidIcon->attr2 & ~0xc00) | 0xc00;
        break;
    }
    gWifiSetupSettings.work->task2 = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupSettings_ScrollTask, 0, 110);
    gWifiSetupSettings.work->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ButtonTask, 0, 120);
    WifiSetupSettings_DrawRows();
    WifiSetupSettings_ShowCursor();
    WifiSetupApp_SetStateFunc(WifiSetupSettings_FadeIn);
}

#define REG16(a)    (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)

void WifiSetupSettings_LoadGraphics(void) {
    WifiSetupGraphicsPath23 scrName;
    WifiSetupGraphicsPath21 plttName;
    WifiSetupGraphicsPath22 charName;
    scrName = gWifiSetupSettingsJb3ListBackNscPath;
    plttName = gWifiSetupSettingsYbBgStep2NclPath;
    charName = gWifiSetupSettingsYbBgStep21NclPath;
    REG16(0x0400000C) = (REG16(0x0400000C) & 0x43) | 0xe18;
    WifiSetup_LoadGraphics(gWifiSetupSettingsYbObjKbNclPath, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(gWifiSetupSettingsJbBgStep2NcgPath, GX_LoadBG3Char);
    WifiSetup_LoadGraphics(gWifiSetupSettingsJbBgStep21NcgPath, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(&plttName, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(gWifiSetupSettingsJb3ListNscPath, GX_LoadBG3Scr);
    {
        void *path;
        void *data;
        path = WifiSetup_LocalizePath(&scrName);
        data = WifiSetupFile_Load(path, 0, 4);
        gWifiSetupSettings.work->screenData = data;
        WifiSetupBgScreen_Create(gWifiSetupSettings.work->screenData);
        WifiSetupBgScreen_MarkDirty();
        path = WifiSetup_LocalizePath(&plttName);
        data = WifiSetupFile_Load(path, 0, 4);
        gWifiSetupSettings.work->paletteData = data;
        path = WifiSetup_LocalizePath(&charName);
        data = WifiSetupFile_Load(path, 0, 4);
        gWifiSetupSettings.work->charData = data;
    }
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    REG16(0x0400000A) = (u16)((REG16(0x0400000A) & ~3) | 2);
    SET_PRI3(0x0400000C);
    REG16(0x0400000E) = (u16)((REG16(0x0400000E) & ~3) | 2);
}

void WifiSetupSettings_ResetPosition(void) {
    int returning;
    WifiSetupConnections_GetEdit();
    WifiSetupApp_GetScreenArgs(0, &returning);
    if (returning) {
        return;
    }
    gWifiSetupSettings.scroll = 0;
    gWifiSetupSettings.cursorItem = 0;
    gWifiSetupSettings.cursorRow = 0;
}

void WifiSetupSettings_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 29, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 29);
    WifiSetupApp_SetStateFunc(WifiSetupSettings_WaitFadeIn);
}

void WifiSetupSettings_WaitFadeIn(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupSettings_Main);
}

void WifiSetupSettings_Main(void) {
    WifiSetupSettings_HandleKeys();
    WifiSetupSettings_HandleScrollBar();
}

void WifiSetupSettings_HandleKeys(void) {
    if (gWifiSetupSettings.work->scrollTask || gWifiSetupSettings.work->scrollBarHeld) {
        return;
    }
    if (WifiSetupSettings_HandleTouch()) {
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_A)) {
        WifiSetupSettings_SelectItem(gWifiSetupSettings.cursorItem);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(PAD_BUTTON_B)) {
        WifiSetupSound_Play(7);
        gWifiSetupSettings.work->selection = WIFI_SETUP_SETTINGS_ITEM_BACK;
        WifiSetupApp_SetStateFunc(WifiSetupSettings_StartClose);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_L)) {
        if (gWifiSetupSettings.scroll == WIFI_SETUP_SETTINGS_MAX_SCROLL) {
            if (gWifiSetupSettings.work->keyAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->keyAtEnd = 1;
            return;
        }
        WifiSetupSound_Play(19);
        gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollDownTask, 0, 120);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_L)) {
        gWifiSetupSettings.work->keyAtEnd = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_BUTTON_R)) {
        if (gWifiSetupSettings.scroll == 0) {
            if (gWifiSetupSettings.work->keyAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->keyAtEnd = 1;
            return;
        }
        WifiSetupSound_Play(19);
        gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollUpTask, 0, 120);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_BUTTON_R)) {
        gWifiSetupSettings.work->keyAtEnd = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_UP)) {
        WifiSetupSettings_MoveCursor(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_KEY_UP)) {
        gWifiSetupSettings.work->keyAtEnd = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_DOWN)) {
        WifiSetupSettings_MoveCursor(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(PAD_KEY_DOWN)) {
        gWifiSetupSettings.work->keyAtEnd = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_LEFT)) {
        WifiSetupSettings_MoveCursor(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(PAD_KEY_RIGHT)) {
        WifiSetupSettings_MoveCursor(2);
    }
}

int WifiSetupSettings_HandleTouch(void) {
    int item;
    WifiSetupConnections_GetEdit();
    item = WifiSetupSettings_GetTouchedItem();
    if (item == WIFI_SETUP_SETTINGS_ITEM_NONE) {
        return 0;
    }
    if (!WifiSetupSettings_IsItemEnabled(item)) {
        WifiSetupSound_Play(9);
        return 1;
    }
    WifiSetupSettings_SetCursorItem(item);
    switch (item) {
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON:
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF:
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON:
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF:
        WifiSetupSettings_PressButton();
        break;
    }
    WifiSetupSettings_SelectItem(item);
    return 1;
}

void WifiSetupSettings_SelectItem(int item) {
    if (!WifiSetupSettings_IsItemEnabled(item)) {
        WifiSetupSound_Play(9);
        return;
    }
    switch (item) {
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_ON:
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_ADDRESS_OFF:
        WifiSetupSound_Play(6);
        WifiSetupSettings_SetAutoAddress(((item - 2) ^ 1) != 0);
        return;
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON:
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_OFF:
        WifiSetupSound_Play(6);
        WifiSetupSettings_SetAutoDns(((item - 7) ^ 1) != 0);
        return;
    }
    gWifiSetupSettings.work->selection = item;
    if ((u32)(item - 11) <= 1) {
        WifiSetupSettings_HighlightButton();
        if (!WifiSetupSettings_IsComplete()) {
            WifiSetupSound_Play(9);
            WifiSetupDialog_Open(6, 1, 1, -1, 0);
            WifiSetupApp_SetStateFunc((WifiSetupAppStateFunc)WifiSetupSettings_WaitIncompleteDialog);
            return;
        }
        if (item == 11) {
            WifiSetupSound_Play(6);
        } else {
            WifiSetupSound_Play(14);
        }
    } else if (item == WIFI_SETUP_SETTINGS_ITEM_BACK) {
        WifiSetupSettings_HighlightButton();
        WifiSetupSound_Play(7);
    } else {
        WifiSetupSound_Play(6);
        WifiSetupSettings_SetButtonOam(0, 1, WifiSetupSettings_GetItemRow(gWifiSetupSettingsItemRows[item]));
        WifiSetupSettings_UpdateObjects();
    }
    WifiSetupScrollBar_Disable();
    WifiSetupApp_SetStateFunc(WifiSetupSettings_StartClose);
}

void WifiSetupSettings_SetAutoAddress(u32 on) {
    int autoDns;
    u8 *record = WifiSetupConnections_GetEdit();
    if (record[0xF5] == on) {
        return;
    }
    if (on) {
        autoDns = gWifiSetupSettings.work->autoDns != 0;
    } else {
        gWifiSetupSettings.work->autoDns = record[0xF6];
        autoDns = 0;
    }
    WifiSetupConnections_SetAutoAddress(on);
    WifiSetupConnections_SetAutoDns(autoDns);
    WifiSetupSettings_DrawRows();
}

void WifiSetupSettings_SetAutoDns(int on) {
    u32 old = WifiSetupConnections_GetEdit()[0xF6];
    if (old == (u32)on) {
        return;
    }
    gWifiSetupSettings.work->autoDns = on;
    WifiSetupConnections_SetAutoDns(on);
    WifiSetupSettings_DrawRows();
}

void WifiSetupSettings_HandleScrollBar(void) {
    int rem;
    if (gWifiSetupSettings.work->scrollTask) {
        return;
    }
    if (gWifiSetupSettings.work->scrollBarTimer) {
        gWifiSetupSettings.work->scrollBarTimer--;
    }
    switch (WifiSetupScrollBar_GetEvent()) {
    case 0:
        goto end;
    case 1:
        gWifiSetupSettings.work->scrollBarHeld = 1;
        return;
    case 2:
        if (gWifiSetupSettings.work->scrollBarTimer) {
            return;
        }
        WifiSetupCursor_Hide();
        gWifiSetupSettings.scroll = (WIFI_SETUP_SETTINGS_MAX_SCROLL * WifiSetupScrollBar_GetValue()) / 55;
        WifiSetupSettings_DrawRows();
        gWifiSetupSettings.work->scrollBarTimer = 4;
        return;
    case 3:
        gWifiSetupSettings.work->scrollBarHeld = 0;
        gWifiSetupSettings.scroll = (WIFI_SETUP_SETTINGS_MAX_SCROLL * WifiSetupScrollBar_GetValue()) / 55;
        WifiSetupSound_Play(19);
        WifiSetupSettings_DrawRows();
        rem = FX_ModS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
        if (rem == 0) {
            WifiSetupSettings_UpdateCursorItem();
            return;
        }
        if (rem < 16) {
            gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollUpTask, 0, 120);
        } else {
            gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollDownTask, 0, 120);
            return;
        }
        return;
    case 4:
        if (gWifiSetupSettings.scroll == 0) {
            if (gWifiSetupSettings.work->scrollBarAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->scrollBarAtEnd = 1;
            return;
        }
        WifiSetupSound_Play(19);
        gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollUpTask, 0, 120);
        return;
    case 6:
        if (gWifiSetupSettings.scroll == WIFI_SETUP_SETTINGS_MAX_SCROLL) {
            if (gWifiSetupSettings.work->scrollBarAtEnd) {
                return;
            }
            WifiSetupSound_Play(9);
            gWifiSetupSettings.work->scrollBarAtEnd = 1;
            return;
        }
        WifiSetupSound_Play(19);
        gWifiSetupSettings.work->scrollTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSettings_ScrollDownTask, 0, 120);
        return;
    case 5:
    case 7:
        gWifiSetupSettings.work->scrollBarAtEnd = 0;
        return;
    default:
        goto end;
    }
end:
    return;
}

void WifiSetupSettings_StartClose(void) {
    WifiSetupScrollBar_Disable();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupSettings_FadeOut);
}

void WifiSetupSettings_FadeOut(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 29, 8);
    WifiSetupApp_SetStateFunc(WifiSetupSettings_Exit);
}

void WifiSetupSettings_Exit(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupTask_Destroy(0, gWifiSetupSettings.work->task);
    WifiSetupTask_Destroy(1, gWifiSetupSettings.work->task2);
    int i;
    for (i = 0; i < 7; i++) { // rows and the two switches
        WifiSetupOam_Release(gWifiSetupSettings.work->rows[i]);
    }
    if (gWifiSetupSettings.work->ssidIcon) {
        WifiSetupOam_Release(gWifiSetupSettings.work->ssidIcon);
    }
    WifiSetupScrollBar_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupCursor_Hide();
    if (gWifiSetupSettings.work->selection != 12) {
        WifiSetupMessage_Hide(gWifiSetupSettings.work->selection);
    }
    WifiSetupBgScreen_Destroy();
    WifiSetupFile_Free(gWifiSetupSettings.work->screenData);
    for (i = 0; i < 2; i++) {
        WifiSetupFile_Free((&gWifiSetupSettings.work->paletteData)[i]);
    }
    WifiSetup_LoadGraphics(gWifiSetupSettingsYbObjMainNclPath, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 29);
    *(vu32 *)0x04000010 = 0;
    *(vu32 *)0x04000018 = 0;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0xe10;
    int selection = gWifiSetupSettings.work->selection;
    switch (selection) {
    case 0:
    case 1:
        WifiSetupApp_SetScreenArgs(selection, 0);
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Init);
        break;
    case 4:
    case 5:
    case 6:
    case 9:
    case 10: {
        int kind = selection - 4;
        if ((u32)selection >= 9) {
            kind -= 2;
        }
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetScreenArgs(kind, 0);
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_Init);
        break;
    }
    case 11: {
        u8 *record = WifiSetupConnections_GetEdit();
        record[0xd0] = DWCi_BACKUPlConvMaskCidr(record + 0xf0);
        if (record[0xf5]) {
            MI_CpuFill8(record + 0xc0, 0, 4);
            MI_CpuFill8(record + 0xc4, 0, 4);
            MI_CpuFill8(record + 0xf0, 0, 4);
            record[0xd0] = 0;
        }
        if (record[0xf6]) {
            MI_CpuFill8(record + 0xc8, 0, 8);
        }
        WifiSetupApp_SetTransition(2, 0);
        WifiSetupApp_SetScreenArgs(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupConnectionTest_Init);
        break;
    }
    case 12:
        WifiSetupApp_SetTransition(0, 0);
        WifiSetupApp_SetStateFunc(WifiSetupSaveScreen_Init);
        break;
    case WIFI_SETUP_SETTINGS_ITEM_BACK:
        if (gWifiSetupSettings.fromMenu == 0) {
            WifiSetupApp_SetTransition(2, 1);
            WifiSetupApp_SetStateFunc(WifiSetupConnectionList_Init);
        } else {
            WifiSetupConnections_Edit(WifiSetupConnections_GetEdit()[0xf4]);
            WifiSetupApp_SetTransition(0, 1);
            WifiSetupApp_SetStateFunc(WifiSetupMethodMenu_Init);
        }
        break;
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F554);
}

void WifiSetupSettings_DrawRows(void) {
    int first = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    int item, row;
    WifiSetupText_Clear(gWifiSetupSettings.work->canvas, 0);
    item = first;
    row = 0;
    for (; row < 5; row++, item++) {
        WifiSetupSettings_DrawValue(item, (void *)row);
    }
    WifiSetupBgScreen_CopyRect(gWifiSetupSettings.work->screenData + gWifiSetupSettingsRowTileOffsets[first], 0, 30, 19);
    for (item = 0; item < 5; item++, first++) {
        WifiSetupSettings_DrawButtons(first, (void *)item);
    }
    WifiSetupBgScreen_MarkDirty();
    WifiSetupText_MarkDirty(gWifiSetupSettings.work->canvas);
    WifiSetupSettings_UpdateObjects();
}

void WifiSetupSettings_DrawValue(u32 item, void *row) {
    WifiSetupSettingsRecord *record = (WifiSetupSettingsRecord *)WifiSetupConnections_GetEdit(item, row);
    switch (item) {
    case 0:
        WifiSetupSettings_DrawText((u8 *)record + 0x40, (int)row);
        return;
    case 1: {
        int len;
        u8 buf[33];
        switch (record->keyType) {
        case 0:
            return;
        case 1:
            len = 10;
            break;
        case 2:
            len = 26;
            break;
        case 3:
            len = 32;
            break;
        }
        if (record->keyFormat == 1) {
            len /= 2;
        }
        MI_CpuFill8(buf, 0, 33);
        memset(buf, '*', len);
        WifiSetupSettings_DrawText(buf, (int)row);
        return;
    }
    case 3:
        if (record->autoAddress) {
            return;
        }
        WifiSetupSettings_DrawAddress((u8 *)record + 0xc0, (int)row);
        return;
    case 4:
        if (record->autoAddress) {
            return;
        }
        WifiSetupSettings_DrawAddress((u8 *)record + 0xf0, (int)row);
        return;
    case 5:
        if (record->autoAddress) {
            return;
        }
        WifiSetupSettings_DrawAddress((u8 *)record + 0xc4, (int)row);
        return;
    case 7:
        if (record->autoDns) {
            return;
        }
        WifiSetupSettings_DrawAddress((u8 *)record + 0xc8, (int)row);
        return;
    case 8:
        if (record->autoDns) {
            return;
        }
        WifiSetupSettings_DrawAddress((u8 *)record + 0xcc, (int)row);
    }
}

#pragma opt_propagation off

void WifiSetupSettings_DrawButtons(u32 item, void *row) {
    WifiSetupSettingsRecord *record = (WifiSetupSettingsRecord *)WifiSetupConnections_GetEdit(item, row);
    int type, option;
    switch (item) {
    case 0:
    case 1:
        type = 0;
        option = 0;
        if (!WifiSetupSettings_IsItemEnabled(0)) {
            option = 2;
        }
        break;
    case 2: {
        int on = record->autoAddress;
        int pressed = 0, otherType;
        WifiSetupSettingsWork *work;
        int held;
        if (on) {
            type = 1;
            otherType = 4;
        } else {
            type = 2;
            otherType = 3;
        }
        work = gWifiSetupSettings.work;
        held = work->buttonStates[0];
        option = pressed;
        if (held) {
            option = 1;
        }
        if (work->buttonStates[1]) {
            pressed = 1;
        }
        WifiSetupSettings_SetButtonOam(otherType, pressed, (int)row);
        break;
    }
    case 3:
    case 4:
    case 5:
        type = 0;
        option = record->autoAddress ? 2 : 0;
        break;
    case 6: {
        int otherType, pressed = 0;
        WifiSetupSettingsWork *work;
        option = 0;
        if (record->autoDns) {
            type = 1;
            otherType = 4;
        } else {
            if (!record->autoAddress) {
                option = 2;
            }
            type = 2;
            otherType = 3;
        }
        work = gWifiSetupSettings.work;
        if (work->buttonStates[2]) {
            option = 1;
        }
        if (work->buttonStates[3]) {
            pressed = 1;
        }
        WifiSetupSettings_SetButtonOam(otherType, pressed, (int)row);
        break;
    }
    case 7:
    case 8:
        type = 0;
        option = record->autoDns ? 2 : 0;
        break;
    default:
        type = 0;
        option = 2;
    }
    WifiSetupSettings_SetButtonOam(type, option, (int)row);
}

#pragma opt_propagation reset

int WifiSetupSettings_IsItemEnabled(int item) {
    u8 *record = WifiSetupConnections_GetEdit();
    int enabled = 1;
    switch (item) {
    case WIFI_SETUP_SETTINGS_ITEM_AUTO_DNS_ON:
        if (!record[0xF5]) {
            enabled = 0;
        }
        break;
    case 0:
    case 1:
        if ((u8)(record[0xE7] + 255) <= 1) {
            enabled = 0;
        }
        break;
    case 4:
    case 5:
    case 6:
        if (record[0xF5]) {
            enabled = 0;
        }
        break;
    case 9:
    case 10:
        if (record[0xF6]) {
            enabled = 0;
        }
        break;
    }
    return enabled;
}

void WifiSetupSettings_SetButtonOam(unsigned type, int option, int row) {
    u16 x[5];
    GXOamAttr **rows = gWifiSetupSettings.work->rows;
    GXOamAttr **oam;
    x[0] = 204;
    x[1] = 192;
    x[2] = 192;
    x[3] = 143;
    x[4] = 143;
    oam = rows + row;
    if ((unsigned)(type - 1) <= 1) {
        if (WifiSetupSettings_GetRowItem(row) == 2) {
            oam = &gWifiSetupSettings.work->addressSwitch;
        } else {
            oam = &gWifiSetupSettings.work->dnsSwitch;
        }
    }
    if (gWifiSetupSettingsButtonTemplates[type][option]) {
        WifiSetupOamTemplate_Copy(0, gWifiSetupSettingsButtonTemplates[type][option], *oam);
        {
            u32 value = *(volatile u16 *)&x[type];
            GXOamAttr *obj = *(GXOamAttr *volatile *)oam;
            obj->attr01 = ((value << 23) >> 7) | (obj->attr01 & 0xfe00ff00);
        }
        (*oam)->attr2 = ((*oam)->attr2 & ~0xc00) | 0xc00;
    } else {
        (*oam)->attr01 = ((*oam)->attr01 & 0xfe00ff00) | 0x1000000; // hidden
    }
}

void WifiSetupSettings_DrawText(u8 *text, int row) {
    u16 buf[17];
    int n, i, m;
    MI_CpuFill8(buf, 0, 34);
    n = WifiSetup_StrNLen(text, 32);
    m = n <= 16 ? n : 16;
    for (i = 0; i < m; i++) {
        int c = text[i];
        if (c == ' ') {
            buf[i] = 0xe01d;
        } else {
            buf[i] = c;
        }
    }
    row = row * WIFI_SETUP_SETTINGS_ROW_HEIGHT + 2;
    if (n <= 16) {
        row += 5;
    }
    WifiSetupText_DrawMonospace(gWifiSetupSettings.work->canvas, 72, row, 2, 8, buf, 1);
    if (n <= 16) {
        return;
    }
    // second line
    MI_CpuFill8(buf, 0, 34);
    n -= 16;
    for (i = 0; i < n; i++) {
        int c = text[i + 16];
        if (c == ' ') {
            buf[i] = 0xe01d;
        } else {
            buf[i] = c;
        }
    }
    WifiSetupText_DrawMonospace(gWifiSetupSettings.work->canvas, 72, row + 12, 2, 8, buf, 1);
}

void WifiSetupSettings_DrawAddress(u8 *address, int row) {
    u16 buf[16];
    swprintf(buf, 16, gWifiSetupSettingsAddressFormat, address[0], address[1], address[2], address[3]);
    WifiSetupText_DrawMonospace(gWifiSetupSettings.work->canvas, 95, row * WIFI_SETUP_SETTINGS_ROW_HEIGHT + 8, 2, 7, buf, 1);
}

static inline void GetObjPos(const GXOamAttr *oam, u32 *x, u32 *y) {
    *x = (oam->attr01 & 0x01FF0000) >> 16;
    *y = oam->attr01 & 0xff;
}

static inline void SetObjPos(GXOamAttr *oam, u32 x, u32 y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | (y & 0xff) | (((u32)x << 23) >> 7);
}

void WifiSetupSettings_UpdateObjects(void) {
    int x;
    int y;
    int first = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    int top = 52 - FX_ModS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    int pos;
    int i;
    if (gWifiSetupSettings.work->ssidIcon != NULL) {
        if (first == 0) {
            x = 0x26;
        } else {
            x = 0x100;
        }
        SetObjPos(gWifiSetupSettings.work->ssidIcon, x, top);
    }
    pos = top;
    for (i = 0; i < 5; i++) {
        GXOamAttr *oam = gWifiSetupSettings.work->rows[i];
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, pos);
        pos += WIFI_SETUP_SETTINGS_ROW_HEIGHT;
    }
    if (first <= 2) {
        GXOamAttr *oam = gWifiSetupSettings.work->addressSwitch;
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, (2 - first) * WIFI_SETUP_SETTINGS_ROW_HEIGHT + top);
    } else {
        SetObjPos(gWifiSetupSettings.work->addressSwitch, 256, 0);
    }
    if (first >= 2 && first <= 6) {
        GXOamAttr *oam = gWifiSetupSettings.work->dnsSwitch;
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, (6 - first) * WIFI_SETUP_SETTINGS_ROW_HEIGHT + top);
    } else {
        SetObjPos(gWifiSetupSettings.work->dnsSwitch, 256, 0);
    }
    gWifiSetupSettings.work->objectsDirty = 1;
}

const WifiSetupSettingsSwitchItems ov13_022454FC = {
    { 2, 3, 7, 8 }
};
static const WifiSetupRect sRowButtonBox = { 0xCC, 0x34, 0x1C, 0x18 };
static const WifiSetupRect sSwitchOffBox = { 0x8F, 0x34, 0x2C, 0x18 };
static const WifiSetupRect sSwitchOnBox = { 0xC0, 0x34, 0x2C, 0x18 };

int WifiSetupSettings_GetTouchedItem(void) {
    WifiSetupRect r;
    int item, i;
    if (!WifiSetupInput_IsTouchPressed(&gWifiSetupInputPanelRect)) {
        return WIFI_SETUP_SETTINGS_ITEM_NONE;
    }
    item = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    r = sRowButtonBox;
    for (i = 0; i < 4; i++) {
        if (item != 2 && item != 6) {
            if (WifiSetupInput_IsTouchPressedInBox(&r)) {
                return gWifiSetupSettingsRowItems[item];
            }
        }
        r.top += WIFI_SETUP_SETTINGS_ROW_HEIGHT;
        item++;
    }
    item = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    for (i = 0; i < 4; i++) {
        if (item == 2) {
            r = sSwitchOnBox;
            r.top += i * WIFI_SETUP_SETTINGS_ROW_HEIGHT;
            if (WifiSetupInput_IsTouchPressedInBox(&r)) {
                return ov13_022454FC.items[0];
            }
            r = sSwitchOffBox;
            r.top += i * WIFI_SETUP_SETTINGS_ROW_HEIGHT;
            if (WifiSetupInput_IsTouchPressedInBox(&r)) {
                return ov13_022454FC.items[1];
            }
            break;
        }
        item++;
    }
    item = FX_DivS32(gWifiSetupSettings.scroll, WIFI_SETUP_SETTINGS_ROW_HEIGHT);
    for (i = 0; i < 4; i++) {
        if (item == 6) {
            r = sSwitchOnBox;
            r.top += i * WIFI_SETUP_SETTINGS_ROW_HEIGHT;
            if (WifiSetupInput_IsTouchPressedInBox(&r)) {
                return ov13_022454FC.items[2];
            }
            r = sSwitchOffBox;
            r.top += i * WIFI_SETUP_SETTINGS_ROW_HEIGHT;
            if (WifiSetupInput_IsTouchPressedInBox(&r)) {
                return ov13_022454FC.items[3];
            }
            break;
        }
        item++;
    }
    for (i = 0; i < 3; i++) {
        if (WifiSetupInput_IsTouchPressed(&gWifiSetupSettingsTouchRects[i])) {
            return i + 11;
        }
    }
    return WIFI_SETUP_SETTINGS_ITEM_NONE;
}
