#include "wifi_setup_ui_parts.h"

#include <string.h>

#include "global.h"

#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active on

typedef struct WifiSetupTextBox {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} WifiSetupTextBox;

extern WifiSetupButtonBar *ov13_0224F468;
extern u8 ov13_0224F46C;
extern char ov13_0224F470[];
extern WifiSetupOamBlock **ov13_0224F4B0;
extern void *ov13_0224F4B4;
extern void *ov13_0224F4F8;
extern u16 ov13_02244EB8[];
extern u16 ov13_02244EBC[];
extern u16 ov13_02244EBE[];
extern u8 ov13_02244EC4[];
extern u8 ov13_02244ED2[];
extern u8 ov13_02244EE0[];
extern u32 ov13_02244EF8[];
extern u16 ov13_02244F14[];
extern u16 ov13_02244F16[];
extern u8 ov13_02244F30[];
extern u8 ov13_02244F38[];
extern u8 ov13_02244F44[][4];
extern u8 ov13_02244F54[];
extern u8 ov13_02244F5C[];
extern char ov13_02245B20[];
extern char *ov13_02245D04[];
extern char ov13_02245D54[];
extern u16 *ov13_02245D74[];

extern s32 FX_DivS32(s32, s32);
extern int WM_GetAllowedChannel(void);
extern int WM_GetLinkLevel(void);
extern int WifiSetupApp_GetLanguage(void);
extern int WifiSetupApp_GetKind(void);
extern void ov13_0222B90C(int *, int *);
extern void *WifiSetupText_GetString(void *, int);
extern void *WifiSetupText_GetStringWithDigit(void *, int, int, int);
extern void WifiSetupBanner_Open(int);
extern void WifiSetupBanner_SetKind(int);
extern u8 *WifiSetupConnections_GetEdit(void);

void WifiSetupBusyIcon_Task(void *task) {
    GXOamAttr *oam;
    u32 palette;
    gWifiSetupBusyIcon->frameCounter = FX_ModS32(gWifiSetupBusyIcon->frameCounter + 1, 40);
    {
        int templ = FX_DivS32(gWifiSetupBusyIcon->frameCounter, 5) + 71;
        WifiSetupOamTemplate_Copy(0, templ, (*(WifiSetupBusyIcon *volatile *)&gWifiSetupBusyIcon)->oam);
    }
    oam = gWifiSetupBusyIcon->oam;
    oam->attr2 = (oam->attr2 & ~0xc00) | 0x400;
    palette = ov13_02244EB0[gWifiSetupBusyIcon->style];
    oam = gWifiSetupBusyIcon->oam;
    oam->attr01 &= ~0xc00;
    oam->attr2 = (oam->attr2 & ~0xf000) | (palette << 12);
    oam = gWifiSetupBusyIcon->oam;
    oam->attr01 = (oam->attr01 & 0xfe00ff00) | 0xe6008b;
}

void WifiSetupButtonBar_Create(int type) {
    int count = ov13_02244EE0[type], i;
    ov13_0224F468 = WifiSetupHeap_AllocZeroed(28, 4);
    ov13_0224F468->selected = -2;
    ov13_0224F468->type = type;
    for (i = 0; i < count; i++) {
        ov13_0224F468->buttons[i] = WifiSetupOamTemplate_AllocBlock(0, ov13_02244EC4[type * 2 + i], 1);
        WifiSetupOamBlock_SetPriority(ov13_0224F468->buttons[i], -1, 1);
    }
    ov13_0224F468->bar = WifiSetupOamTemplate_AllocBlock(0, 1, 1);
    WifiSetupOamBlock_SetPriority(ov13_0224F468->bar, -1, 1);
    WifiSetupButtonBar_SetY(192);
    ov13_0224F468->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupButtonBar_SlideInTask, NULL, 120);
}

void WifiSetupButtonBar_Close(void) {
    ov13_0224F468->closing = 1;
    WifiSetupTask_SetFunc(ov13_0224F468->task, (WifiSetupTaskFunc)WifiSetupButtonBar_SlideOutTask);
}

int WifiSetupButtonBar_GetSelected(void) {
    return ov13_0224F468->selected;
}

void WifiSetupButtonBar_SelectIfNone(int button) {
    if (ov13_0224F468->selected == -1) {
        ov13_0224F468->selected = button;
    }
}

void WifiSetupButtonBar_SetSelected(int button) {
    ov13_0224F468->selected = button;
}

int ov13_022272A0(void) {
    if (!ov13_0224F468) {
        return 1;
    }
    if (ov13_0224F468->closing == 0) {
        return 1;
    }
    return 0;
}

void WifiSetupButtonBar_Unlock(void) {
    ov13_0224F468->locked = 0;
}

void WifiSetupButtonBar_Lock(void) {
    ov13_0224F468->locked = 1;
}

void WifiSetupButtonBar_SetY(int y) {
    int count = ov13_02244EE0[ov13_0224F468->type], i;
    WifiSetupOamBlock_SetPosition(ov13_0224F468->bar, -1, 0, y);
    for (i = 0; i < count; i++) {
        WifiSetupOamBlock_SetPosition(ov13_0224F468->buttons[i], -1, ov13_02244EBC[(ov13_02244ED2 + ov13_0224F468->type * 2)[i] * 2], y + 4);
    }
}

void WifiSetupButtonBar_SlideInTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(ov13_0224F468->bar, 0, &x, &y);
    y -= 4;
    WifiSetupButtonBar_SetY(y);
    if (y > 168) {
        return;
    }
    WifiSetupButtonBar_SetY(168);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupButtonBar_WaitTask);
}

void WifiSetupButtonBar_WaitTask(void *task) {
    ov13_0224F468->selected = -1;
    ov13_0224F468->timer++;
    if (ov13_0224F468->timer < 4) {
        return;
    }
    ov13_0224F468->timer = 0;
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupButtonBar_InputTask);
}

void WifiSetupButtonBar_InputTask(void) {
    WifiSetupRect rect;
    int count = ov13_02244EE0[ov13_0224F468->type], i;
    if (ov13_0224F468->locked == 0) {
        if (ov13_0224F468->selected != -1) {
            return;
        }
        for (i = 0; i < count; i++) {
            WifiSetup_RectFromPosSize((WifiSetupPoint *)(ov13_02244EBC + (ov13_02244ED2 + ov13_0224F468->type * 2)[i] * 2), (WifiSetupPoint *)ov13_02244EB8, &rect);
            if (WifiSetupInput_IsTouchPressed(&rect) != 0) {
                int templ;
                GXOamAttr *oam;
                if (ov13_0224F468->pressTask) {
                    break;
                }
                templ = (ov13_02244EC4 + ov13_0224F468->type * 2)[i];
                oam = WifiSetupOamBlock_GetOam(ov13_0224F468->buttons[i], 0);
                WifiSetupOamTemplate_Copy(0, templ + 1, oam);
                templ = (ov13_02244ED2 + ov13_0224F468->type * 2)[i] * 2;
                WifiSetupOamBlock_SetPosition(ov13_0224F468->buttons[i], -1, ov13_02244EBC[templ], ov13_02244EBE[templ]);
                WifiSetupOamBlock_SetPriority(ov13_0224F468->buttons[i], -1, 1);
                ov13_0224F468->pressTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupButtonBar_PressTask, NULL, 110);
                ov13_0224F468->selected = i;
                return;
            }
        }
    }
    ov13_0224F468->selected = -1;
}

void WifiSetupButtonBar_SlideOutTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(ov13_0224F468->bar, 0, &x, &y);
    y += 4;
    WifiSetupButtonBar_SetY(y);
    if (y < 192) {
        return;
    }
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupButtonBar_DestroyTask);
}

void WifiSetupButtonBar_DestroyTask(void *task) {
    int i;
    WifiSetupTask_DestroyLater(0, task);
    if (ov13_0224F468->pressTask) {
        WifiSetupTask_DestroyLater(0, ov13_0224F468->pressTask);
    }
    for (i = 0; i < 2; i++) {
        if (ov13_0224F468->buttons[i]) {
            WifiSetupOamBlock_Free(ov13_0224F468->buttons[i]);
        }
    }
    WifiSetupOamBlock_Free(ov13_0224F468->bar);
    // &ov13_0224F468, formed from the next label (own literal-pool slot, as in the original)
    WifiSetupHeap_FreeAndClear(&ov13_0224F46C - 4);
}

// Restores the pressed button's normal graphics after 16 frames.
void WifiSetupButtonBar_PressTask(void *task) {
    int count, i;
    ov13_0224F468->timer++;
    if (ov13_0224F468->timer < 16) {
        return;
    }
    count = ov13_02244EE0[ov13_0224F468->type];
    for (i = 0; i < count; i++) {
        int templ = (ov13_02244EC4 + ov13_0224F468->type * 2)[i];
        GXOamAttr *oam = WifiSetupOamBlock_GetOam(ov13_0224F468->buttons[i], 0);
        WifiSetupOamTemplate_Copy(0, templ, oam);
        WifiSetupOamBlock_SetPriority(ov13_0224F468->buttons[i], -1, 1);
    }
    WifiSetupButtonBar_SetY(168);
    ov13_0224F468->timer = 0;
    ov13_0224F468->selected = -1;
    if (!ov13_0224F468->pressTask) {
        return;
    }
    ov13_0224F468->pressTask = NULL;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupMessage_Reset(void) {
    ov13_0224F46C = 0;
}

// Shows message (a, b, c) of the message bank in the window at the top of the top screen.
int WifiSetupMessage_Show(int a, int b, int c) {
    void *canvas, *text;
    u32 flags;
    if (ov13_0224F46C) {
        return 0;
    }
    WifiSetup_LoadGraphics(ov13_02245B20, GXS_LoadBG0Scr);
    *(vu32 *)0x04001010 = 0x01920000;
    canvas = WifiSetupText_InitBgCanvas(1, 0);
    text = WifiSetupText_GetStringWithDigit(ov13_0224F4F8, a, b, c);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 20, 0, 216, 64, 2, flags, text);
    WifiSetupText_MarkDirty(canvas);
    ov13_0224F46C = 1;
    return 1;
}

int WifiSetupMessage_Hide(void) {
    if (!ov13_0224F46C) {
        return 0;
    }
    WifiSetupText_FreeBgCanvas(1);
    ov13_0224F46C = 0;
    return 1;
}

// Language variant of a file name: character 5 is replaced by the language letter, unless it is 'x'
// (shared) or 'y' (shared except for language 0).
char *WifiSetup_LocalizePath(char *path) {
    int language;
    strncpy(ov13_0224F470, path, 63);
    if (((u8 *)path)[5] == 'x') {
        return ov13_0224F470;
    }
    language = WifiSetupApp_GetLanguage();
    if (((u8 *)path)[5] == 'y' && language != 0) {
        return ov13_0224F470;
    }
    ov13_0224F470[5] = ov13_02244F30[language];
    return ov13_0224F470;
}

void WifiSetup_LoadGraphics(char *path, void (*load)(const void *, u32, u32)) {
    u32 size;
    void *data;
    path = WifiSetup_LocalizePath(path);
    data = WifiSetupFile_Load(path, &size, 4);
    DC_FlushRange(data, size);
    load(data, 0, size);
    WifiSetupFile_Free(data);
}

void ov13_02227964(int a) {
    int type;
    ov13_0222B90C(&type, 0);
    if (type == 1) {
        WifiSetupBanner_Open(a);
    } else if (type == 2) {
        WifiSetupBanner_SetKind(a);
    }
}

void ov13_022279B4(int type) {
    int barType;
    ov13_0222B90C(0, &barType);
    if (barType == 1) {
        WifiSetupButtonBar_Create(type);
    }
}

void WifiSetup_PrintMessage(int message) {
    void *canvas, *text;
    u32 flags;
    canvas = WifiSetupText_InitBgCanvas(0, 0);
    text = WifiSetupText_GetString(ov13_0224F4F8, message);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 13, 40, 230, 112, 2, flags, text);
    WifiSetupText_MarkDirty(canvas);
}

void WifiSetup_PrintMessageWithText(const u16 *str, int message) {
    void *canvas, *text;
    u32 flags;
    int x, y;
    canvas = WifiSetupText_InitBgCanvas(0, 0);
    text = WifiSetupText_GetString(ov13_0224F4F8, message);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 13, 60, 230, 94, 2, flags, text);
    x = WifiSetupApp_GetLanguage();
    y = WifiSetupApp_GetLanguage();
    WifiSetupText_Draw(canvas, ov13_02244F14[x * 2], ov13_02244F16[y * 2], 2, 521, str);
    WifiSetupText_MarkDirty(canvas);
}

u32 WifiSetup_GetTextFlags(void) {
    return ov13_02244EF8[WifiSetupApp_GetLanguage()];
}

void WifiSetupCursor_Init(void) {
    ov13_0224F4B0 = WifiSetupHeap_AllocZeroed(16, 4);
}

void WifiSetupCursor_Free(void) {
    WifiSetupCursor_Hide();
    WifiSetupHeap_FreeAndClear(&ov13_0224F4B0);
}

// Two arrows (kind selects the template pair) at (x1, y) and (x2, y).
void WifiSetupCursor_ShowPair(int kind, int x1, int x2, int y) {
    int i;
    u8 *templ;
    WifiSetupCursor_Hide();
    templ = ov13_02244F38 + kind * 2;
    for (i = 0; i < 2; i++) {
        ov13_0224F4B0[i] = WifiSetupOamTemplate_AllocBlock(0, *templ++, 1);
        WifiSetupOamBlock_SetPriority(ov13_0224F4B0[i], -1, 1);
    }
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[0], -1, x1, y);
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[1], -1, x2, y);
}

// Four corner markers around the rectangle.
void WifiSetupCursor_ShowCorners(int left, int right, int top, int bottom) {
    int templ = 6, i;
    WifiSetupCursor_Hide(left);
    for (i = 0; i < 4; i++, templ++) {
        ov13_0224F4B0[i] = WifiSetupOamTemplate_AllocBlock(0, templ, 1);
        WifiSetupOamBlock_SetPriority(ov13_0224F4B0[i], -1, 1);
    }
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[0], -1, left, top);
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[1], -1, right, top);
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[2], -1, left, bottom);
    WifiSetupOamBlock_SetPosition(ov13_0224F4B0[3], -1, right, bottom);
}

void WifiSetupCursor_Hide() {
    int i;
    for (i = 0; i < 4; i++) {
        if (ov13_0224F4B0[i]) {
            WifiSetupOamBlock_Free(ov13_0224F4B0[i]);
            ov13_0224F4B0[i] = NULL;
        }
    }
}

void WifiSetupBackground_Init(void) {
    ov13_0224F4B4 = WifiSetupHeap_AllocZeroed(192, 4);
    WifiSetup_LoadGraphics(ov13_02245D54, GX_LoadBG1Char);
    switch (WifiSetupApp_GetKind()) {
    case 0:
        WifiSetup_LoadGraphics(ov13_02245D04[0], GX_LoadBG1Scr);
        break;
    case 1:
        WifiSetup_LoadGraphics(ov13_02245D04[1], GX_LoadBG1Scr);
        break;
    }
}

void WifiSetupBackground_Free(void) {
    WifiSetupHeap_FreeAndClear(&ov13_0224F4B4);
}

// Replaces the first 192 bytes (3 rows) of the BG1 screen at the next vblank.
void WifiSetupBackground_LoadHeader(int n) {
    void *data;
    data = WifiSetupFile_Load(WifiSetup_LocalizePath(ov13_02245D04[n]), NULL, 4);
    MIi_CpuCopyFast(data, ov13_0224F4B4, 192);
    WifiSetupFile_Free(data);
    WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupBackground_TransferTask, NULL, 120);
}

void ov13_02227E40(void) {
    WifiSetupBackground_LoadHeader(WifiSetupConnections_GetEdit()[244] + 5);
}

void ov13_02227E58(void) {
    WifiSetupBackground_LoadHeader(WifiSetupConnections_GetEdit()[244] + 2);
}

void WifiSetupBackground_TransferTask(void *task) {
    DC_FlushRange(ov13_0224F4B4, 192);
    GX_LoadBG1Scr(ov13_0224F4B4, 0, 192);
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupSignalIcon_Create(int type) {
    int templ;
    if (gWifiSetupSignalIcon) {
        return;
    }
    {
        WifiSetupSignalIcon *icon = WifiSetupHeap_AllocZeroed(12, 4);
        templ = *(volatile u8 *)&ov13_02244F44[type][0];
        gWifiSetupSignalIcon = icon;
        icon->type = type;
    }
    gWifiSetupSignalIcon->oam = WifiSetupOamTemplate_AllocOam(0, templ);
    gWifiSetupSignalIcon->oam->attr01 = (gWifiSetupSignalIcon->oam->attr01 & 0xfe00ff00) | 0x26 | 0xe50000;
    gWifiSetupSignalIcon->oam->attr2 = (gWifiSetupSignalIcon->oam->attr2 & ~0xc00) | 0x800;
    gWifiSetupSignalIcon->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupSignalIcon_Task, NULL, 120);
}

WifiSetupSignalIcon *gWifiSetupSignalIcon;

void WifiSetupSignalIcon_Destroy(void) {
    if (gWifiSetupSignalIcon == NULL) {
        return;
    }
    WifiSetupTask_Destroy(0, gWifiSetupSignalIcon->task);
    WifiSetupOam_Release(gWifiSetupSignalIcon->oam);
    WifiSetupHeap_FreeAndClear(&gWifiSetupSignalIcon);
}

// Shows the current link level (0 when no channel is allowed) at (229, 38).
void WifiSetupSignalIcon_Task(void) {
    int level = 0;
    u16 ime = *(vu16 *)0x4000208;
    GXOamAttr *oam;
    *(vu16 *)0x4000208 = 0;
    if (WM_GetAllowedChannel() != 0x8000) {
        level = WM_GetLinkLevel();
    }
    (void)*(vu16 *)0x4000208;
    *(vu16 *)0x4000208 = ime;
    WifiSetupOamTemplate_Copy(0, ((u8 *)ov13_02244F44 + gWifiSetupSignalIcon->type * 4)[level], gWifiSetupSignalIcon->oam);
    oam = gWifiSetupSignalIcon->oam;
    oam->attr01 = (oam->attr01 & 0xfe00ff00) | 0xe50026;
    oam = gWifiSetupSignalIcon->oam;
    oam->attr2 = (oam->attr2 & ~0xc00) | 0x800;
}

void WifiSetupKeyboard_Create(void) {
    WifiSetupTextBox box = { 0, 0, 0, 0 };
    u32 numObjs;

    struct {
        int offset;
        u16 code;
        volatile u16 unused;
    } pre;

    u8 *templ;
    int i, j, k;
    box.width = 17;
    box.height = 18;
    gWifiSetupKeyboard = WifiSetupHeap_AllocZeroed(296, 4);
    gWifiSetupKeyboard->input = 255;
    gWifiSetupKeyboard->cursorKey = 0;
    gWifiSetupKeyboard->canDelete = 1;
    gWifiSetupKeyboard->canType = 1;
    for (i = 0; i < 47; i++) {
        gWifiSetupKeyboard->keys[i] = WifiSetupOamTemplate_AllocOam(0, 52);
        gWifiSetupKeyboard->keys[i]->attr01 = (gWifiSetupKeyboard->keys[i]->attr01 & 0xc1fffcff) | 512;
        gWifiSetupKeyboard->keys[i]->attr2 = (gWifiSetupKeyboard->keys[i]->attr2 & ~0xc00) | 0xc00;
    }
    templ = ov13_02244F5C;
    for (i = 0; i < 4; i++) {
        gWifiSetupKeyboard->extraKeys[i] = WifiSetupOamTemplate_AllocOam(0, *templ++);
        gWifiSetupKeyboard->extraKeys[i]->attr01 = (gWifiSetupKeyboard->extraKeys[i]->attr01 & 0xc1fffcff) | 512;
        gWifiSetupKeyboard->extraKeys[i]->attr2 = (gWifiSetupKeyboard->extraKeys[i]->attr2 & ~0xc00) | 0xc00;
    }
    {
        u8 *buttonTempl = ov13_02244F54;
        for (i = 0; i < 2; i++) {
            gWifiSetupKeyboard->buttons[i] = WifiSetupOamTemplate_AllocBlock(0, *buttonTempl++, 1);
            WifiSetupOamBlock_SetAffineMode(gWifiSetupKeyboard->buttons[i], -1, 512, 0);
            WifiSetupOamBlock_SetPriority(gWifiSetupKeyboard->buttons[i], -1, 3);
        }
    }
    i = 0;
    pre.unused = i;
    for (; i < 3; i++) {
        pre.offset = 0;
        for (j = 0; j < 4; j++) {
            gWifiSetupKeyboard->rows[i][j] = WifiSetupText_AllocObjCanvas(0, 28, 2, 0, &numObjs, 0);
            box.x = 0;
            {
                int index = 0;
                for (k = pre.offset; index < 12;) {
                    pre.code = ov13_02245D74[i][k];
                    WifiSetupText_DrawInRect(gWifiSetupKeyboard->rows[i][j], box.x, box.y, box.width, box.height, 2, 1152, &pre.code);
                    index++;
                    box.x += 18;
                    k++;
                }
            }
            if (i == 0) {
                gWifiSetupKeyboard->rowBlocks[j] = WifiSetupOamBlock_Alloc(0, numObjs, 1);
            }
            pre.offset += 12;
        }
    }
    gWifiSetupKeyboard->cursor = WifiSetupOamTemplate_AllocBlock(0, 64, 1);
    WifiSetupOamBlock_SetAffineMode(gWifiSetupKeyboard->cursor, -1, 512, 0);
    WifiSetupOamBlock_SetPriority(gWifiSetupKeyboard->cursor, -1, 2);
    gWifiSetupKeyboard->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideInRow0Task, NULL, 120);
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 0, 192);
}

void WifiSetupKeyboard_Close(void) {
    WifiSetupOamBlock_Free(gWifiSetupKeyboard->cursor);
    WifiSetupTask_SetFunc(gWifiSetupKeyboard->task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideOutButtonsTask);
}

int WifiSetupKeyboard_GetInput(void) {
    return gWifiSetupKeyboard->input;
}

void WifiSetupKeyboard_SetCanDelete(int canDelete) {
    gWifiSetupKeyboard->canDelete = canDelete;
}

void WifiSetupKeyboard_SetCanType(int canType) {
    gWifiSetupKeyboard->canType = canType;
}

BOOL WifiSetupKeyboard_Exists(void) {
    return gWifiSetupKeyboard != NULL;
}
