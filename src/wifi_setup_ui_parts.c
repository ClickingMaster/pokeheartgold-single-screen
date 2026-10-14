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

extern WifiSetupButtonBar *gWifiSetupButtonBar;
extern u8 gWifiSetupMessageShown;
extern char gWifiSetupLocalizedPath[];
extern WifiSetupOamBlock **gWifiSetupCursorBlocks;
extern void *gWifiSetupBackgroundScreen;
extern void *gWifiSetupTextBank;
extern u16 gWifiSetupButtonBarButtonSize[];
extern u16 gWifiSetupButtonBarButtonX[];
extern u16 gWifiSetupButtonBarButtonY[];
extern u8 gWifiSetupButtonBarTemplates[];
extern u8 gWifiSetupButtonBarPositions[];
extern u8 gWifiSetupButtonBarButtonCounts[];
extern u32 gWifiSetupTextFlagsByLanguage[];
extern u16 gWifiSetupMessageTextX[];
extern u16 gWifiSetupMessageTextY[];
extern u8 gWifiSetupLanguageLetters[];
extern u8 gWifiSetupCursorPairTemplates[];
extern u8 gWifiSetupSignalIconTemplates[][4];
extern u8 gWifiSetupKeyboardButtonTemplates[];
extern u8 gWifiSetupKeyboardExtraKeyTemplates[];
extern char sStr_CharJtNullNscL[];
extern char *gWifiSetupBackgroundScreenPaths[];
extern char sStr_CharJbBgHlNcgL[];
extern u16 *gWifiSetupKeyboardPageLabels[];

extern s32 FX_DivS32(s32, s32);
extern int WM_GetAllowedChannel(void);
extern int WM_GetLinkLevel(void);
extern int WifiSetupApp_GetLanguage(void);
extern int WifiSetupApp_GetKind(void);
extern void WifiSetupApp_GetTransition(int *, int *);
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
    palette = gWifiSetupBusyIconPalettes[gWifiSetupBusyIcon->style];
    oam = gWifiSetupBusyIcon->oam;
    oam->attr01 &= ~0xc00;
    oam->attr2 = (oam->attr2 & ~0xf000) | (palette << 12);
    oam = gWifiSetupBusyIcon->oam;
    oam->attr01 = (oam->attr01 & 0xfe00ff00) | 0xe6008b;
}

void WifiSetupButtonBar_Create(int type) {
    int count = gWifiSetupButtonBarButtonCounts[type], i;
    gWifiSetupButtonBar = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupButtonBar), 4);
    gWifiSetupButtonBar->selected = -2;
    gWifiSetupButtonBar->type = type;
    for (i = 0; i < count; i++) {
        gWifiSetupButtonBar->buttons[i] = WifiSetupOamTemplate_AllocBlock(0, gWifiSetupButtonBarTemplates[type * 2 + i], 1);
        WifiSetupOamBlock_SetPriority(gWifiSetupButtonBar->buttons[i], -1, 1);
    }
    gWifiSetupButtonBar->bar = WifiSetupOamTemplate_AllocBlock(0, 1, 1);
    WifiSetupOamBlock_SetPriority(gWifiSetupButtonBar->bar, -1, 1);
    WifiSetupButtonBar_SetY(192);
    gWifiSetupButtonBar->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupButtonBar_SlideInTask, NULL, 120);
}

void WifiSetupButtonBar_Close(void) {
    gWifiSetupButtonBar->closing = 1;
    WifiSetupTask_SetFunc(gWifiSetupButtonBar->task, (WifiSetupTaskFunc)WifiSetupButtonBar_SlideOutTask);
}

int WifiSetupButtonBar_GetSelected(void) {
    return gWifiSetupButtonBar->selected;
}

void WifiSetupButtonBar_SelectIfNone(int button) {
    if (gWifiSetupButtonBar->selected == -1) {
        gWifiSetupButtonBar->selected = button;
    }
}

void WifiSetupButtonBar_SetSelected(int button) {
    gWifiSetupButtonBar->selected = button;
}

int WifiSetupButtonBar_IsNotClosing(void) {
    if (!gWifiSetupButtonBar) {
        return 1;
    }
    if (gWifiSetupButtonBar->closing == 0) {
        return 1;
    }
    return 0;
}

void WifiSetupButtonBar_Unlock(void) {
    gWifiSetupButtonBar->locked = 0;
}

void WifiSetupButtonBar_Lock(void) {
    gWifiSetupButtonBar->locked = 1;
}

void WifiSetupButtonBar_SetY(int y) {
    int count = gWifiSetupButtonBarButtonCounts[gWifiSetupButtonBar->type], i;
    WifiSetupOamBlock_SetPosition(gWifiSetupButtonBar->bar, -1, 0, y);
    for (i = 0; i < count; i++) {
        WifiSetupOamBlock_SetPosition(gWifiSetupButtonBar->buttons[i], -1, gWifiSetupButtonBarButtonX[(gWifiSetupButtonBarPositions + gWifiSetupButtonBar->type * 2)[i] * 2], y + 4);
    }
}

void WifiSetupButtonBar_SlideInTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupButtonBar->bar, 0, &x, &y);
    y -= 4;
    WifiSetupButtonBar_SetY(y);
    if (y > 168) {
        return;
    }
    WifiSetupButtonBar_SetY(168);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupButtonBar_WaitTask);
}

void WifiSetupButtonBar_WaitTask(void *task) {
    gWifiSetupButtonBar->selected = -1;
    gWifiSetupButtonBar->timer++;
    if (gWifiSetupButtonBar->timer < 4) {
        return;
    }
    gWifiSetupButtonBar->timer = 0;
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupButtonBar_InputTask);
}

void WifiSetupButtonBar_InputTask(void) {
    WifiSetupRect rect;
    int count = gWifiSetupButtonBarButtonCounts[gWifiSetupButtonBar->type], i;
    if (gWifiSetupButtonBar->locked == 0) {
        if (gWifiSetupButtonBar->selected != -1) {
            return;
        }
        for (i = 0; i < count; i++) {
            WifiSetup_RectFromPosSize((WifiSetupPoint *)(gWifiSetupButtonBarButtonX + (gWifiSetupButtonBarPositions + gWifiSetupButtonBar->type * 2)[i] * 2), (WifiSetupPoint *)gWifiSetupButtonBarButtonSize, &rect);
            if (WifiSetupInput_IsTouchPressed(&rect) != 0) {
                int templ;
                GXOamAttr *oam;
                if (gWifiSetupButtonBar->pressTask) {
                    break;
                }
                templ = (gWifiSetupButtonBarTemplates + gWifiSetupButtonBar->type * 2)[i];
                oam = WifiSetupOamBlock_GetOam(gWifiSetupButtonBar->buttons[i], 0);
                WifiSetupOamTemplate_Copy(0, templ + 1, oam);
                templ = (gWifiSetupButtonBarPositions + gWifiSetupButtonBar->type * 2)[i] * 2;
                WifiSetupOamBlock_SetPosition(gWifiSetupButtonBar->buttons[i], -1, gWifiSetupButtonBarButtonX[templ], gWifiSetupButtonBarButtonY[templ]);
                WifiSetupOamBlock_SetPriority(gWifiSetupButtonBar->buttons[i], -1, 1);
                gWifiSetupButtonBar->pressTask = WifiSetupTask_Create(0, (WifiSetupTaskFunc)WifiSetupButtonBar_PressTask, NULL, 110);
                gWifiSetupButtonBar->selected = i;
                return;
            }
        }
    }
    gWifiSetupButtonBar->selected = -1;
}

void WifiSetupButtonBar_SlideOutTask(void *task) {
    int x, y;
    WifiSetupOamBlock_GetPosition(gWifiSetupButtonBar->bar, 0, &x, &y);
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
    if (gWifiSetupButtonBar->pressTask) {
        WifiSetupTask_DestroyLater(0, gWifiSetupButtonBar->pressTask);
    }
    for (i = 0; i < 2; i++) {
        if (gWifiSetupButtonBar->buttons[i]) {
            WifiSetupOamBlock_Free(gWifiSetupButtonBar->buttons[i]);
        }
    }
    WifiSetupOamBlock_Free(gWifiSetupButtonBar->bar);
    // &gWifiSetupButtonBar, formed from the next label (own literal-pool slot, as in the original)
    WifiSetupHeap_FreeAndClear(&gWifiSetupMessageShown - 4);
}

// Restores the pressed button's normal graphics after 16 frames.
void WifiSetupButtonBar_PressTask(void *task) {
    int count, i;
    gWifiSetupButtonBar->timer++;
    if (gWifiSetupButtonBar->timer < 16) {
        return;
    }
    count = gWifiSetupButtonBarButtonCounts[gWifiSetupButtonBar->type];
    for (i = 0; i < count; i++) {
        int templ = (gWifiSetupButtonBarTemplates + gWifiSetupButtonBar->type * 2)[i];
        GXOamAttr *oam = WifiSetupOamBlock_GetOam(gWifiSetupButtonBar->buttons[i], 0);
        WifiSetupOamTemplate_Copy(0, templ, oam);
        WifiSetupOamBlock_SetPriority(gWifiSetupButtonBar->buttons[i], -1, 1);
    }
    WifiSetupButtonBar_SetY(168);
    gWifiSetupButtonBar->timer = 0;
    gWifiSetupButtonBar->selected = -1;
    if (!gWifiSetupButtonBar->pressTask) {
        return;
    }
    gWifiSetupButtonBar->pressTask = NULL;
    WifiSetupTask_DestroyLater(0, task);
}

void WifiSetupMessage_Reset(void) {
    gWifiSetupMessageShown = 0;
}

// Shows message (a, b, c) of the message bank in the window at the top of the top screen.
int WifiSetupMessage_Show(int a, int b, int c) {
    void *canvas, *text;
    u32 flags;
    if (gWifiSetupMessageShown) {
        return 0;
    }
    WifiSetup_LoadGraphics(sStr_CharJtNullNscL, GXS_LoadBG0Scr);
    *(vu32 *)0x04001010 = 0x01920000;
    canvas = WifiSetupText_InitBgCanvas(1, 0);
    text = WifiSetupText_GetStringWithDigit(gWifiSetupTextBank, a, b, c);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 20, 0, 216, 64, 2, flags, text);
    WifiSetupText_MarkDirty(canvas);
    gWifiSetupMessageShown = 1;
    return 1;
}

int WifiSetupMessage_Hide(void) {
    if (!gWifiSetupMessageShown) {
        return 0;
    }
    WifiSetupText_FreeBgCanvas(1);
    gWifiSetupMessageShown = 0;
    return 1;
}

// Language variant of a file name: character 5 is replaced by the language letter, unless it is 'x'
// (shared) or 'y' (shared except for language 0).
char *WifiSetup_LocalizePath(char *path) {
    int language;
    strncpy(gWifiSetupLocalizedPath, path, 63);
    if (((u8 *)path)[5] == 'x') {
        return gWifiSetupLocalizedPath;
    }
    language = WifiSetupApp_GetLanguage();
    if (((u8 *)path)[5] == 'y' && language != 0) {
        return gWifiSetupLocalizedPath;
    }
    gWifiSetupLocalizedPath[5] = gWifiSetupLanguageLetters[language];
    return gWifiSetupLocalizedPath;
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

void WifiSetupBanner_Show(int a) {
    int type;
    WifiSetupApp_GetTransition(&type, 0);
    if (type == 1) {
        WifiSetupBanner_Open(a);
    } else if (type == 2) {
        WifiSetupBanner_SetKind(a);
    }
}

void WifiSetupButtonBar_Show(int type) {
    int barType;
    WifiSetupApp_GetTransition(0, &barType);
    if (barType == 1) {
        WifiSetupButtonBar_Create(type);
    }
}

void WifiSetup_PrintMessage(int message) {
    void *canvas, *text;
    u32 flags;
    canvas = WifiSetupText_InitBgCanvas(0, 0);
    text = WifiSetupText_GetString(gWifiSetupTextBank, message);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 13, 40, 230, 112, 2, flags, text);
    WifiSetupText_MarkDirty(canvas);
}

void WifiSetup_PrintMessageWithText(const u16 *str, int message) {
    void *canvas, *text;
    u32 flags;
    int x, y;
    canvas = WifiSetupText_InitBgCanvas(0, 0);
    text = WifiSetupText_GetString(gWifiSetupTextBank, message);
    flags = WifiSetup_GetTextFlags();
    WifiSetupText_DrawInRect(canvas, 13, 60, 230, 94, 2, flags, text);
    x = WifiSetupApp_GetLanguage();
    y = WifiSetupApp_GetLanguage();
    WifiSetupText_Draw(canvas, gWifiSetupMessageTextX[x * 2], gWifiSetupMessageTextY[y * 2], 2, 521, str);
    WifiSetupText_MarkDirty(canvas);
}

u32 WifiSetup_GetTextFlags(void) {
    return gWifiSetupTextFlagsByLanguage[WifiSetupApp_GetLanguage()];
}

void WifiSetupCursor_Init(void) {
    gWifiSetupCursorBlocks = WifiSetupHeap_AllocZeroed(16, 4);
}

void WifiSetupCursor_Free(void) {
    WifiSetupCursor_Hide();
    WifiSetupHeap_FreeAndClear(&gWifiSetupCursorBlocks);
}

// Two arrows (kind selects the template pair) at (x1, y) and (x2, y).
void WifiSetupCursor_ShowPair(int kind, int x1, int x2, int y) {
    int i;
    u8 *templ;
    WifiSetupCursor_Hide();
    templ = gWifiSetupCursorPairTemplates + kind * 2;
    for (i = 0; i < 2; i++) {
        gWifiSetupCursorBlocks[i] = WifiSetupOamTemplate_AllocBlock(0, *templ++, 1);
        WifiSetupOamBlock_SetPriority(gWifiSetupCursorBlocks[i], -1, 1);
    }
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[0], -1, x1, y);
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[1], -1, x2, y);
}

// Four corner markers around the rectangle.
void WifiSetupCursor_ShowCorners(int left, int right, int top, int bottom) {
    int templ = 6, i;
    WifiSetupCursor_Hide(left);
    for (i = 0; i < 4; i++, templ++) {
        gWifiSetupCursorBlocks[i] = WifiSetupOamTemplate_AllocBlock(0, templ, 1);
        WifiSetupOamBlock_SetPriority(gWifiSetupCursorBlocks[i], -1, 1);
    }
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[0], -1, left, top);
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[1], -1, right, top);
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[2], -1, left, bottom);
    WifiSetupOamBlock_SetPosition(gWifiSetupCursorBlocks[3], -1, right, bottom);
}

void WifiSetupCursor_Hide() {
    int i;
    for (i = 0; i < 4; i++) {
        if (gWifiSetupCursorBlocks[i]) {
            WifiSetupOamBlock_Free(gWifiSetupCursorBlocks[i]);
            gWifiSetupCursorBlocks[i] = NULL;
        }
    }
}

void WifiSetupBackground_Init(void) {
    gWifiSetupBackgroundScreen = WifiSetupHeap_AllocZeroed(192, 4);
    WifiSetup_LoadGraphics(sStr_CharJbBgHlNcgL, GX_LoadBG1Char);
    switch (WifiSetupApp_GetKind()) {
    case 0:
        WifiSetup_LoadGraphics(gWifiSetupBackgroundScreenPaths[0], GX_LoadBG1Scr);
        break;
    case 1:
        WifiSetup_LoadGraphics(gWifiSetupBackgroundScreenPaths[1], GX_LoadBG1Scr);
        break;
    }
}

void WifiSetupBackground_Free(void) {
    WifiSetupHeap_FreeAndClear(&gWifiSetupBackgroundScreen);
}

// Replaces the first 192 bytes (3 rows) of the BG1 screen at the next vblank.
void WifiSetupBackground_LoadHeader(int n) {
    void *data;
    data = WifiSetupFile_Load(WifiSetup_LocalizePath(gWifiSetupBackgroundScreenPaths[n]), NULL, 4);
    MIi_CpuCopyFast(data, gWifiSetupBackgroundScreen, 192);
    WifiSetupFile_Free(data);
    WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupBackground_TransferTask, NULL, 120);
}

void WifiSetupBackground_LoadSlotHeader(void) {
    WifiSetupBackground_LoadHeader(WifiSetupConnections_GetEdit()[244] + 5);
}

void WifiSetupBackground_LoadSlotListHeader(void) {
    WifiSetupBackground_LoadHeader(WifiSetupConnections_GetEdit()[244] + 2);
}

void WifiSetupBackground_TransferTask(void *task) {
    DC_FlushRange(gWifiSetupBackgroundScreen, 192);
    GX_LoadBG1Scr(gWifiSetupBackgroundScreen, 0, 192);
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupSignalIcon_Create(int type) {
    int templ;
    if (gWifiSetupSignalIcon) {
        return;
    }
    {
        WifiSetupSignalIcon *icon = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupSignalIcon), 4);
        templ = *(volatile u8 *)&gWifiSetupSignalIconTemplates[type][0];
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
    WifiSetupOamTemplate_Copy(0, ((u8 *)gWifiSetupSignalIconTemplates + gWifiSetupSignalIcon->type * 4)[level], gWifiSetupSignalIcon->oam);
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
    gWifiSetupKeyboard = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupKeyboard), 4);
    gWifiSetupKeyboard->input = 255;
    gWifiSetupKeyboard->cursorKey = 0;
    gWifiSetupKeyboard->canDelete = 1;
    gWifiSetupKeyboard->canType = 1;
    for (i = 0; i < 47; i++) {
        gWifiSetupKeyboard->keys[i] = WifiSetupOamTemplate_AllocOam(0, 52);
        gWifiSetupKeyboard->keys[i]->attr01 = (gWifiSetupKeyboard->keys[i]->attr01 & 0xc1fffcff) | 512;
        gWifiSetupKeyboard->keys[i]->attr2 = (gWifiSetupKeyboard->keys[i]->attr2 & ~0xc00) | 0xc00;
    }
    templ = gWifiSetupKeyboardExtraKeyTemplates;
    for (i = 0; i < 4; i++) {
        gWifiSetupKeyboard->extraKeys[i] = WifiSetupOamTemplate_AllocOam(0, *templ++);
        gWifiSetupKeyboard->extraKeys[i]->attr01 = (gWifiSetupKeyboard->extraKeys[i]->attr01 & 0xc1fffcff) | 512;
        gWifiSetupKeyboard->extraKeys[i]->attr2 = (gWifiSetupKeyboard->extraKeys[i]->attr2 & ~0xc00) | 0xc00;
    }
    {
        u8 *buttonTempl = gWifiSetupKeyboardButtonTemplates;
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
                    pre.code = gWifiSetupKeyboardPageLabels[i][k];
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
