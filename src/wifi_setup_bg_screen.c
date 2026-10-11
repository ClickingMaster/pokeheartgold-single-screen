#pragma force_active on
#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_scroll_bar.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// BG 2 screen copy and palette uploads, scroll bar creation (see include/wifi_setup_scroll_bar.h).

extern u8 ov13_02245790[]; // thumb template per kind
extern u8 ov13_02245794[]; // thumb length per kind
extern WifiSetupRect ov13_02245958;
extern s32 FX_DivS32(s32, s32);
typedef struct WifiSetupApScan WifiSetupApScan; // wifi_setup_access_points.c
extern WifiSetupApScan *gWifiSetupApScan;

void WifiSetupPalette_LoadRowTask(void *task, void *arg);
void WifiSetupPalette_LoadTask(void *task, void *arg);
void WifiSetupScrollBar_Task(void *task, void *arg);
void WifiSetupScrollBar_Drag(void);
void WifiSetupScrollBar_PlayDragSound(int value);
void WifiSetupScrollBar_PressArrow(int state);
void WifiSetupScrollBar_JumpToTouch(void);
int WifiSetupScrollBar_GetTouchedPart(void);
int WifiSetupScrollBar_GetHeldArrow(int part);
void WifiSetupScrollBar_GetPartRect(int part, WifiSetupRect *rect);
void WifiSetupScrollBar_SetThumbPos(int value);

void WifiSetupBgScreen_MarkDirty(void) {
    gWifiSetupBgScreen.screen->dirty = 1;
}

void WifiSetupBgScreen_CopyRect(u16 *src, int offset, int width, int rows) {
    u16 *dst = gWifiSetupBgScreen.screen->data + offset;
    int i;
    for (i = 0; i < rows; i++) {
        MIi_CpuCopy16(src, dst, width * 2);
        src += 0x20;
        dst += 0x20;
    }
}

void WifiSetupBgScreen_UpdateTask(void *task, void *arg) {
    if (gWifiSetupBgScreen.screen->dirty == 0) {
        return;
    }
    DC_FlushRange(gWifiSetupBgScreen.screen->data, 0x600);
    GX_LoadBG2Scr(gWifiSetupBgScreen.screen->data, 0, 0x600);
    gWifiSetupBgScreen.screen->dirty = 0;
}

void WifiSetupPalette_LoadRow(u8 *src, int index, int palette) {
    gWifiSetupBgScreen.paletteSrc = src + index * 32;
    gWifiSetupBgScreen.paletteDst = (void *)(palette * 32 + 0x5000000);
    WifiSetupTask_Create(1, WifiSetupPalette_LoadRowTask, NULL, 0x78);
}

void WifiSetupPalette_LoadRowTask(void *task, void *arg) {
    MIi_CpuCopy16(gWifiSetupBgScreen.paletteSrc, gWifiSetupBgScreen.paletteDst, 0x20);
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupPalette_Load(void *src) {
    gWifiSetupBgScreen.paletteSrc = src;
    WifiSetupTask_Create(1, WifiSetupPalette_LoadTask, NULL, 0x78);
}

void WifiSetupPalette_LoadTask(void *task, void *arg) {
    MIi_CpuCopy16(gWifiSetupBgScreen.paletteSrc, (void *)0x5000000, 0x200);
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupScrollBar_Create(int kind, int length, int x, int y, int value) {
    gWifiSetupScrollBar = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupScrollBar), 4);
    gWifiSetupScrollBar->kind = kind;
    gWifiSetupScrollBar->length = length;
    gWifiSetupScrollBar->value = value;
    // called without the u16 conversions of the prototype
    ((void (*)(int, int, WifiSetupPoint *))WifiSetup_SetPoint)(x, y, &gWifiSetupScrollBar->pos);
    gWifiSetupScrollBar->thumb = WifiSetupOamTemplate_AllocBlock(0, ov13_02245790[kind], 1);
    WifiSetupOamBlock_SetPosition(gWifiSetupScrollBar->thumb, -1, x, y + value);
    WifiSetupOamBlock_SetPriority(gWifiSetupScrollBar->thumb, -1, 1);
    gWifiSetupScrollBar->task = WifiSetupTask_Create(0, WifiSetupScrollBar_Task, NULL, 0x80);
}
