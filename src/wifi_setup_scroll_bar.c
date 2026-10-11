#pragma force_active on
#include "wifi_setup_scroll_bar.h"

#include "global.h"

#include "wifi_setup_access_points.h"
#include "wifi_setup_local_wireless.h"
#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Vertical scroll bar and the access-point scan work allocation (see include/wifi_setup_scroll_bar.h).

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

u8 WifiSetupScrollBar_GetValue(void) {
    return gWifiSetupScrollBar->value;
}

u8 WifiSetupScrollBar_GetEvent(void) {
    return gWifiSetupScrollBar->event;
}

void WifiSetupScrollBar_SetValue(int value) {
    WifiSetupScrollBar_SetThumbPos(value);
}

void WifiSetupScrollBar_Enable(void) {
    gWifiSetupScrollBar->disabled = 0;
}

void WifiSetupScrollBar_Disable(void) {
    gWifiSetupScrollBar->disabled = 1;
}

void WifiSetupScrollBar_Task(void *task, void *arg) {
    gWifiSetupScrollBar->event = 0;
    switch (gWifiSetupScrollBar->state) {
    case 0:
        if (gWifiSetupScrollBar->disabled) {
            return;
        }
        switch (WifiSetupScrollBar_GetTouchedPart()) {
        case 1:
            if (gWifiSetupScrollBar->kind == 0) {
                return;
            }
            WifiSetupSound_Play(0x16);
            WifiSetupSound_SetVolume(0);
            gWifiSetupScrollBar->event = 1;
            WifiSetupInput_GetTouchPos(&gWifiSetupScrollBar->touchStart);
            gWifiSetupScrollBar->dragStart = gWifiSetupScrollBar->value;
            gWifiSetupScrollBar->state = 1;
            return;
        case 2:
            WifiSetupScrollBar_PressArrow(2);
            return;
        case 3:
            WifiSetupScrollBar_PressArrow(3);
            return;
        case 4:
            WifiSetupScrollBar_JumpToTouch();
            return;
        }
        break;
    case 1:
        WifiSetupScrollBar_Drag();
        break;
    case 2:
        if (WifiSetupScrollBar_GetHeldArrow(2) != 2) {
            gWifiSetupScrollBar->event = 5;
            gWifiSetupScrollBar->state = 0;
            return;
        }
        if (WifiSetupScrollBar_GetTouchedPart() == 2) {
            WifiSetupScrollBar_PressArrow(2);
        }
        break;
    case 3:
        if (WifiSetupScrollBar_GetHeldArrow(3) != 3) {
            gWifiSetupScrollBar->event = 7;
            gWifiSetupScrollBar->state = 0;
            return;
        }
        if (WifiSetupScrollBar_GetTouchedPart() == 3) {
            WifiSetupScrollBar_PressArrow(3);
        }
        break;
    }
}

void WifiSetupScrollBar_Drag(void) {
    WifiSetupPoint p;
    WifiSetupScrollBar *s;
    int v;
    int max;

    if (WifiSetupInput_IsTouchHeld(&ov13_02245958)) {
        WifiSetupInput_GetTouchPos(&p);
        s = gWifiSetupScrollBar;
        if (p.x >= s->pos.x - 0x1e) {
            v = s->dragStart + (p.y - s->touchStart.y);
            if (v < 0) {
                v = 0;
            } else {
                max = s->length - ov13_02245794[s->kind];
                if (v >= max) {
                    v = max;
                }
            }
            WifiSetupScrollBar_PlayDragSound(v);
            WifiSetupScrollBar_SetThumbPos(v);
            gWifiSetupScrollBar->event = 2;
            return;
        }
    }
    WifiSetupSound_Stop();
    gWifiSetupScrollBar->state = 0;
    gWifiSetupScrollBar->event = 3;
}

void WifiSetupScrollBar_PlayDragSound(int value) {
    int d = gWifiSetupScrollBar->value - value;
    int a;
    if (d < 0) {
        d = -d;
    }
    if (d < 2) {
        a = 0;
    } else if (d >= 6) {
        a = 0x7f;
    } else {
        a = FX_DivS32(0x7f, 6 - d);
    }
    WifiSetupSound_SetVolume(a);
    if (d < 2) {
        a = -0x100;
    } else if (d >= 6) {
        a = 0x100;
    } else {
        a = FX_DivS32(0x200, 6 - d) - 0x100;
    }
    WifiSetupSound_SetTrackPitch(0xFFFF, a);
}

void WifiSetupScrollBar_PressArrow(int state) {
    gWifiSetupScrollBar->state = state;
    gWifiSetupScrollBar->event = (state == 2) ? 4 : 6;
}

void WifiSetupScrollBar_JumpToTouch(void) {
    WifiSetupPoint p;
    WifiSetupScrollBar *s;
    u32 w;
    int v;
    int max;

    WifiSetupInput_GetTouchPos(&p);
    s = gWifiSetupScrollBar;
    w = ov13_02245794[s->kind];
    v = p.y - s->pos.y - (w >> 1);
    if (v < 0) {
        v = 0;
    } else {
        max = s->length - w;
        if (v >= max) {
            v = max;
        }
    }
    WifiSetupScrollBar_SetThumbPos(v);
    gWifiSetupScrollBar->event = 3;
}

int WifiSetupScrollBar_GetTouchedPart(void) {
    WifiSetupRect r;
    int i;
    WifiSetupScrollBar_GetPartRect(1, &r);
    if (WifiSetupInput_IsTouchRepeated(&r)) {
        return 1;
    }
    for (i = 2; i <= 3; i++) {
        WifiSetupScrollBar_GetPartRect(i, &r);
        if (WifiSetupInput_IsTouchRepeated(&r)) {
            return i;
        }
    }
    WifiSetupScrollBar_GetPartRect(4, &r);
    if (WifiSetupInput_IsTouchPressed(&r)) {
        return 4;
    }
    return 0;
}

int WifiSetupScrollBar_GetHeldArrow(int part) {
    WifiSetupRect r;
    int i;
    for (i = 2; i <= 3; i++) {
        WifiSetupScrollBar_GetPartRect(i, &r);
        if (WifiSetupInput_IsTouchHeld(&r)) {
            return i;
        }
    }
    return 0;
}

void WifiSetupScrollBar_GetPartRect(int part, WifiSetupRect *r) {
    u16 x = gWifiSetupScrollBar->pos.x;
    u16 y;
    r->left = x;
    r->right = x + 12;
    switch (part) {
    case 1:
        r->top = gWifiSetupScrollBar->pos.y + gWifiSetupScrollBar->value;
        r->bottom = r->top + ov13_02245794[gWifiSetupScrollBar->kind];
        break;
    case 2:
        r->top = gWifiSetupScrollBar->pos.y - 13;
        r->bottom = gWifiSetupScrollBar->pos.y;
        break;
    case 3:
        r->top = gWifiSetupScrollBar->pos.y + gWifiSetupScrollBar->length;
        r->bottom = r->top + 13;
        break;
    case 4:
        y = gWifiSetupScrollBar->pos.y;
        r->top = y;
        r->bottom = y + gWifiSetupScrollBar->length;
        break;
    }
}

void WifiSetupScrollBar_SetThumbPos(int value) {
    WifiSetupOamBlock_SetPosition(gWifiSetupScrollBar->thumb, -1, gWifiSetupScrollBar->pos.x, value + gWifiSetupScrollBar->pos.y);
    gWifiSetupScrollBar->value = value;
}

void WifiSetupApScan_AllocWork(void) {
    if (gWifiSetupApScan != NULL) {
        return;
    }
    gWifiSetupApScan = WifiSetupHeap_AllocZeroed(0x1E60, 0x20) /* sizeof(WifiSetupApScan) */;
}
