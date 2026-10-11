#include "screen_window_wipe.h"

#include <nitro/fx/fx_trig.h>
#include <nitro/gx/g2.h>

// Palette fade module (not merged yet): declared with the types its callers here were matched against
extern void sub_02010E64(ClockWipeScanlines *scanlines, u8 window, int screen, enum HeapID heapId);
extern void sub_02010EC8(ClockWipeScanlines *scanlines);
extern s16 *sub_02010EE0(ClockWipeScanlines *scanlines, int index);
extern void sub_02010F00(SysTask *task, void *scanlines);
extern void sub_02010F34(int queued, WindowRegisterQueue *regQueue, int screen);
extern void sub_02010F84(WindowRegisterQueue *regQueue, u8 insideMask, u8 outsideMask, u8 window, int screen, int left, int top, int right, int bottom, int queued);
extern void sub_02010FEC(WindowRegisterQueue *regQueue, int window, int screen, int queued);
extern void sub_02011068(WindowRegisterQueue *regQueue, int a1, int screen, int queued);
extern void sub_02010C38(void);
extern void sub_0200FF88(void *blankPair, ClockWipe *wipe, void (*callback)(void), int screen, enum HeapID heapId);
extern void sub_0200FFB4(void *blankPair, int screen, enum HeapID heapId);

void ClockWipe_Begin(ClockWipeFade *fade, const ClockWipeConfig *config) {
    fade->wipe = Heap_Alloc(fade->heapId, sizeof(ClockWipe));
    ClockWipe_Init(fade->wipe, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL ClockWipe_Update(ClockWipeFade *fade) {
    BOOL done = FALSE;
    ClockWipe *wipe = fade->wipe;
    switch (fade->state) {
    case 1:
        if (ClockWipe_Step(wipe) == TRUE) {
            sub_02010F34(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        sub_02010EC8(&wipe->scanlines);
        Heap_Free(fade->wipe);
        fade->wipe = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return done;
}

void ClockWipe_Init(ClockWipe *wipe, const ClockWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    s16 *buffer;

    sub_02010E64(&wipe->scanlines, config->window, screen, heapId);
    wipe->unused14 = 0x7FFF80;
    wipe->centerX = config->centerX;
    wipe->centerY = config->centerY;
    wipe->startAngle = config->startAngle;
    wipe->angle = config->startAngle;
    wipe->angleStep = 0xFFFF / steps;
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    wipe->window = config->window;
    wipe->screen = screen;
    wipe->maskSwapPending = TRUE;
    ClockWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(sub_02010F00, wipe, 0x3FF);
    buffer = sub_02010EE0(&wipe->scanlines, 0);
    // first scanline's span
    sub_02010F84(regQueue, config->insideMask, config->outsideMask, config->window, screen, buffer[0x180], 0, buffer[0x240], 192, wipe->queued);
    if (config->window == 0) {
        sub_02011068(regQueue, 1, screen, wipe->queued);
    } else {
        sub_02011068(regQueue, 2, screen, wipe->queued);
    }
    sub_0200FF88(wipe->blankPair, wipe, sub_02010C38, screen, heapId);
}

BOOL ClockWipe_Step(ClockWipe *wipe) {
    wipe->frameCounter++;
    if (wipe->frameCounter >= wipe->framesPerStep) {
        wipe->frameCounter = 0;
        if (wipe->stepsLeft - 1 > 0) {
            wipe->angle += wipe->angleStep;
            wipe->stepsLeft--;
            while (wipe->angle >= 0xFFFF) {
                wipe->angle -= 0xFFFF;
            }
            while (wipe->angle < 0) {
                wipe->angle += 0xFFFF;
            }
            if (wipe->angle >= 0x7FFF && wipe->angle < wipe->startAngle && wipe->maskSwapPending == TRUE) {
                sub_02010FEC(wipe->regQueue, wipe->window, wipe->screen, wipe->queued);
                wipe->maskSwapPending = FALSE;
            }
            ClockWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(sub_02010F00, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

// Horizontal span [*left, *right] of the wedge between startAngle and endAngle (radius 0xFFFF around
// (centerX, centerY)) on scanline y, clamped to 0-255.
void ClockWipe_GetScanlineSpan(int unused, int centerX, int centerY, int y, int startAngle, int endAngle, int *left, int *right) {
    int *dest1 = left;
    int *dest2 = right;
    int sample = y;
    int originX = centerX;

    struct {
        int y2;
        int x2;
        int x1;
    } values;

    int angle1Saved = startAngle;
    int y1;
    int low;
    int high;
#pragma unused(unused)
    values.x1 = centerX + (0xFFFF * FX_SinCosTable_[((startAngle >> 4) << 1) + 1] >> 0xc);
    y1 = centerY + (0xFFFF * FX_SinCosTable_[(startAngle >> 4) << 1] >> 0xc);
    values.x2 = centerX + (0xFFFF * FX_SinCosTable_[((endAngle >> 4) << 1) + 1] >> 0xc);
    values.y2 = centerY + (0xFFFF * FX_SinCosTable_[(endAngle >> 4) << 1] >> 0xc);
    if (endAngle - angle1Saved == (0x0000FFFF >> 1)) {
        if (sample >= 0 && sample < centerY) {
            *dest1 = 0;
            *dest2 = 0xff;
            return;
        }
        return;
    }
    if (endAngle >= 0 && endAngle < (0x0000FFFF >> 1)) {
        low = y1 < centerY ? y1 : centerY;
        high = y1 > centerY ? y1 : centerY;
        if (low <= sample && sample <= high) {
            *dest1 = values.x1 + (values.x1 - originX) * (sample - y1) / (y1 - centerY);
        } else {
            *dest1 = values.x2 + (values.x2 - originX) * (sample - values.y2) / (values.y2 - centerY);
        }
        {
            int value = *dest1;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest1 = value;
        }
        *dest2 = 0xff;
        return;
    }
    low = y1 < centerY ? y1 : centerY;
    high = y1 > centerY ? y1 : centerY;
    if (low <= sample && sample <= high) {
        *dest1 = values.x1 + (values.x1 - originX) * (sample - y1) / (y1 - centerY);
        {
            int value = *dest1;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest1 = value;
        }
    } else {
        *dest1 = 0;
    }
    low = values.y2 < centerY ? values.y2 : centerY;
    high = values.y2 > centerY ? values.y2 : centerY;
    if (low <= sample && sample <= high) {
        *dest2 = values.x2 + (values.x2 - originX) * (sample - values.y2) / (values.y2 - centerY);
        {
            int value = *dest2;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest2 = value;
        }
    } else {
        *dest2 = *dest1;
    }
    if (*dest1 > *dest2) {
        int temp = *dest1;
        *dest1 = *dest2;
        *dest2 = temp;
    }
}

// Writes the spans of all 192 scanlines to the back half of the scanline buffer (lefts at 0x300, rights at 0x480)
void ClockWipe_BuildScanlines(ClockWipe *wipe) {
    s16 *dst = sub_02010EE0(&wipe->scanlines, 0);
    s32 i;
    s32 left;
    s32 right;

    for (i = 0; i < 192; i++) {
        ClockWipe_GetScanlineSpan(wipe->unused14, wipe->centerX, wipe->centerY, i, wipe->startAngle, wipe->angle, &left, &right);
        dst[0x180] = left;
        dst[0x240] = right;
        dst++;
    }
}

// DISPCNT bits 13-15: WIN0, WIN1, OBJ window
void ScreenWindow_SetVisible(u32 mask, int screen) {
    if (screen == 0) {
        reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | (mask << 13);
    } else {
        reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xFFFF1FFF) | (mask << 13);
    }
}

void ScreenWindow_SetInside(u32 planeMask, BOOL effect, int window, int screen) {
    if (window == 0) {
        if (screen == 0) {
            u32 value = (reg_G2_WININ & ~0x3F) | planeMask;
            if (effect) {
                value |= 0x20;
            }
            reg_G2_WININ = value;
        } else {
            u32 value = (reg_G2S_DB_WININ & ~0x3F) | planeMask;
            if (effect) {
                value |= 0x20;
            }
            reg_G2S_DB_WININ = value;
        }
    } else {
        if (screen == 0) {
            u32 value = (reg_G2_WININ & ~0x3F00) | (planeMask << 8);
            if (effect) {
                value |= 0x2000;
            }
            reg_G2_WININ = value;
        } else {
            u32 value = (reg_G2S_DB_WININ & ~0x3F00) | (planeMask << 8);
            if (effect) {
                value |= 0x2000;
            }
            reg_G2S_DB_WININ = value;
        }
    }
}

void ScreenWindow_SetOutside(u32 planeMask, BOOL effect, int screen) {
    if (screen == 0) {
        u32 value = (reg_G2_WINOUT & ~0x3F) | planeMask;
        if (effect) {
            value |= 0x20;
        }
        reg_G2_WINOUT = value;
    } else {
        u32 value = (reg_G2S_DB_WINOUT & ~0x3F) | planeMask;
        if (effect) {
            value |= 0x20;
        }
        reg_G2S_DB_WINOUT = value;
    }
}

s8 ScreenWindow_GetInside(int window, int screen) {
    volatile s8 result;
    if (window == 0) {
        if (screen == 0) {
            result = *(volatile u8 *)&reg_G2_WININ;
        } else {
            result = *(volatile u8 *)&reg_G2S_DB_WININ;
        }
    } else {
        if (screen == 0) {
            result = *((volatile u8 *)&reg_G2_WININ + 1);
        } else {
            result = *((volatile u8 *)&reg_G2S_DB_WININ + 1);
        }
    }
    return result;
}

#pragma force_active on

static inline GXWndPlane GetWindowPlane(volatile GXWndPlane *reg) {
    return *reg;
}

s8 ScreenWindow_GetOutside(int screen) {
    GXWndPlane plane;
    if (screen == 0) {
        plane = GetWindowPlane((volatile GXWndPlane *)&reg_G2_WINOUT);
    } else {
        plane = GetWindowPlane((volatile GXWndPlane *)&reg_G2S_DB_WINOUT);
    }
    return *(s8 *)&plane;
}

#pragma force_active off

void ScreenWindow_SetRect(int left, int top, int right, int bottom, int window, int screen) {
    if (window == 0) {
        if (screen == 0) {
            u32 h = (left << 8 & 0xFF00) | ((u32)right << 24 >> 24);
            u32 v = (top << 8 & 0xFF00) | ((u32)bottom << 24 >> 24);
            reg_G2_WIN0H = h;
            reg_G2_WIN0V = v;
        } else {
            u32 h = (left << 8 & 0xFF00) | ((u32)right << 24 >> 24);
            u32 v = (top << 8 & 0xFF00) | ((u32)bottom << 24 >> 24);
            reg_G2S_DB_WIN0H = h;
            reg_G2S_DB_WIN0V = v;
        }
    } else {
        if (screen == 0) {
            u32 h = (left << 8 & 0xFF00) | ((u32)right << 24 >> 24);
            u32 v = (top << 8 & 0xFF00) | ((u32)bottom << 24 >> 24);
            reg_G2_WIN1H = h;
            reg_G2_WIN1V = v;
        } else {
            u32 h = (left << 8 & 0xFF00) | ((u32)right << 24 >> 24);
            u32 v = (top << 8 & 0xFF00) | ((u32)bottom << 24 >> 24);
            reg_G2S_DB_WIN1H = h;
            reg_G2S_DB_WIN1V = v;
        }
    }
}

void ScreenWindow_QueueSetVisible(WindowRegisterQueue *queue, u32 mask, int screen) {
    WindowVisibleArgs *args = &queue->visible[screen];
    args->mask = mask;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(ScreenWindow_SetVisibleTask, args, 1);
}

void ScreenWindow_QueueSetInside(WindowRegisterQueue *queue, u32 planeMask, BOOL effect, int window, int screen) {
    WindowInsideArgs *args = &queue->inside[screen][window];
    args->planeMask = planeMask;
    args->effect = effect;
    args->window = window;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(ScreenWindow_SetInsideTask, args, 1);
}

void ScreenWindow_QueueSetOutside(WindowRegisterQueue *queue, u32 planeMask, BOOL effect, int screen) {
    WindowOutsideArgs *args = &queue->outside[screen];
    args->planeMask = planeMask;
    args->effect = effect;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(ScreenWindow_SetOutsideTask, args, 1);
}

void ScreenWindow_QueueSetRect(WindowRegisterQueue *queue, int left, int top, int right, int bottom, int window, int screen) {
    WindowRectArgs *args = &queue->rect[screen][window];
    args->left = left;
    args->top = top;
    args->right = right;
    args->bottom = bottom;
    args->window = window;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(ScreenWindow_SetRectTask, args, 1);
}

void ScreenWindow_SetVisibleTask(SysTask *task, void *data) {
    WindowVisibleArgs *args = data;
    ScreenWindow_SetVisible(args->mask, args->screen);
    SysTask_Destroy(task);
}

void ScreenWindow_SetInsideTask(SysTask *task, void *data) {
    WindowInsideArgs *args = data;
    ScreenWindow_SetInside(args->planeMask, args->effect, args->window, args->screen);
    SysTask_Destroy(task);
}

void ScreenWindow_SetOutsideTask(SysTask *task, void *data) {
    WindowOutsideArgs *args = data;
    ScreenWindow_SetOutside(args->planeMask, args->effect, args->screen);
    SysTask_Destroy(task);
}

void ScreenWindow_SetRectTask(SysTask *task, void *data) {
    WindowRectArgs *args = data;
    ScreenWindow_SetRect(args->left, args->top, args->right, args->bottom, args->window, args->screen);
    SysTask_Destroy(task);
}
