#include "frontier/frontier_transition.h"

#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "scanline_effect.h"
#include "sys_task_api.h"

// Screen transition effects of the frontier script system: master brightness flashes, window wipes made of 96
// growing boxes, a scanline wave and a double-buffered HBlank DMA of the BG2/BG3 affine registers.

// Not declared in the game headers (NitroSDK GX).
extern void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

typedef struct FrontierTransitionLerp {
    s32 value;
    s32 start;
    s32 delta;
    s32 step;
    s32 duration;
} FrontierTransitionLerp;

typedef struct FrontierTransitionFade {
    FrontierTransitionLerp lerp;
    s32 screen;
} FrontierTransitionFade;

typedef struct FrontierTransitionFlash {
    u32 state;
    u32 count;
    u32 iteration;
    u32 screen;
    s32 brightness;
    s32 otherBrightness;
    FrontierTransitionFade fade;
    FrontierTransitionFade otherFade;
    u32 *done;
} FrontierTransitionFlash;

struct FrontierWipeBox {
    FrontierTransitionLerp x;
    FrontierTransitionLerp y;
    Window *window;
    u8 width;
    u8 height;
    u8 active;
    u8 color;
};

struct FrontierWipe {
    Window *window;
    FrontierWipeBox *boxes[96];
    u8 started;
    u8 column;
    u8 row;
    u8 side;
    u8 finished;
    u8 duration;
    u8 delay;
    s8 timer;
    u8 color;
    u8 active;
};

extern const u8 gFrontierWipeRows[2][6];
extern const u8 gFrontierWipeColumns[2][16];

void FrontierTransition_FlashTask(SysTask *task, void *data);
void FrontierTransition_SetBrightness(u32 screen, s32 brightness);
void FrontierTransitionFade_Init(FrontierTransitionFade *fade, s32 start, s32 end, s32 screen, s32 duration);
BOOL FrontierTransitionFade_Step(FrontierTransitionFade *fade);
void FrontierTransitionFade_ApplyTask(SysTask *task, void *data);
void FrontierTransitionLerp_Init(FrontierTransitionLerp *lerp, s32 start, s32 end, s32 duration);
BOOL FrontierTransitionLerp_Step(FrontierTransitionLerp *lerp);
FrontierWipeBox *FrontierWipeBox_New(enum HeapID heapID);
void FrontierWipeBox_Free(FrontierWipeBox *box);
void FrontierWipeBox_Start(FrontierWipeBox *box, s32 x0, s32 x1, s32 y0, s32 y1, s32 duration, Window *window, u32 width, u32 height, u8 color);
BOOL FrontierWipeBox_Update(FrontierWipeBox *box);
void FrontierWipeBox_FillRect(Window *window, s32 top, s32 bottom, s32 left, s32 right, u8 color);
SysTask *FrontierScanline_CreateTask(FrontierScanline *scanline);
void FrontierScanline_VBlankTask(SysTask *task, void *data);
void FrontierHBlankDma_VWaitTask(SysTask *task, void *data);
void FrontierHBlankDma_VBlankTask(SysTask *task, void *data);
void FrontierHBlankDma_StartTasks(FrontierHBlankDma *dma, void *arg, FrontierHBlankDmaCallback onVWait, FrontierHBlankDmaCallback onVBlank);
void FrontierHBlankDma_StopTasks(FrontierHBlankDma *dma);
void FrontierHBlankDma_StartDma(FrontierHBlankDma *dma);
void FrontierHBlankDma_SwapAndRestart(FrontierHBlankDma *dma);
void FrontierHBlankDma_Restart(FrontierHBlankDma *dma);

void FrontierTransition_StartFlash(u32 screen, s32 brightness, s32 otherBrightness, u32 *done, u32 count) {
    FrontierTransitionFlash *flash = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierTransitionFlash));
    memset(flash, 0, sizeof(FrontierTransitionFlash));
    SysTask_CreateOnMainQueue(FrontierTransition_FlashTask, flash, 5);
    flash->done = done;
    if (done != NULL) {
        *done = FALSE;
    }
    flash->screen = screen;
    flash->brightness = brightness;
    flash->otherBrightness = otherBrightness;
    flash->count = count;
}

void FrontierTransition_FlashTask(SysTask *task, void *data) {
    FrontierTransitionFlash *flash = data;
    switch (flash->state) {
    case 0:
        if (flash->screen == 1) {
            FrontierTransitionFade_Init(&flash->otherFade, 0, flash->otherBrightness, 2, 8);
        } else if (flash->screen == 2) {
            FrontierTransitionFade_Init(&flash->otherFade, 0, flash->otherBrightness, 1, 8);
        }
        flash->state++;
        break;
    case 1:
        FrontierTransitionFade_Init(&flash->fade, 0, flash->brightness, flash->screen, 3);
        flash->state++;
        break;
    case 2:
        if (FrontierTransitionFade_Step(&flash->fade)) {
            flash->state++;
        }
        break;
    case 3:
        FrontierTransitionFade_Init(&flash->fade, flash->brightness, 0, flash->screen, 3);
        flash->state++;
        break;
    case 4:
        if (FrontierTransitionFade_Step(&flash->fade)) {
            flash->iteration++;
            if (flash->iteration == flash->count) {
                flash->state = 5;
            } else {
                flash->state = 1;
            }
        }
        break;
    case 5:
        flash->state = 0;
        flash->iteration = 0;
        if (flash->done != NULL) {
            *flash->done = TRUE;
        }
        SysTask_Destroy(task);
        Heap_Free(flash);
        return;
    }
    FrontierTransitionFade_Step(&flash->otherFade);
}

void FrontierTransition_SetBrightness(u32 screen, s32 brightness) {
    if (screen == 1) {
        GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, brightness);
    } else {
        GXx_SetMasterBrightness_(&reg_GXS_DB_MASTER_BRIGHT, brightness);
    }
}

void FrontierTransitionFade_Init(FrontierTransitionFade *fade, s32 start, s32 end, s32 screen, s32 duration) {
    fade->screen = screen;
    FrontierTransitionLerp_Init(&fade->lerp, start, end, duration);
}

BOOL FrontierTransitionFade_Step(FrontierTransitionFade *fade) {
    BOOL finished = FrontierTransitionLerp_Step(&fade->lerp);
    SysTask_CreateOnVWaitQueue(FrontierTransitionFade_ApplyTask, fade, 10);
    return finished;
}

void FrontierTransitionFade_ApplyTask(SysTask *task, void *data) {
    FrontierTransitionFade *fade = data;
    FrontierTransition_SetBrightness(fade->screen, fade->lerp.value);
    SysTask_Destroy(task);
}

void FrontierTransitionLerp_Init(FrontierTransitionLerp *lerp, s32 start, s32 end, s32 duration) {
    lerp->value = start;
    lerp->start = start;
    lerp->delta = end - start;
    lerp->duration = duration;
    lerp->step = 0;
}

BOOL FrontierTransitionLerp_Step(FrontierTransitionLerp *lerp) {
    s32 value = lerp->delta * lerp->step / lerp->duration;
    value += lerp->start;
    lerp->value = value;
    if (lerp->step + 1 <= lerp->duration) {
        lerp->step++;
        return FALSE;
    }
    lerp->step = lerp->duration;
    return TRUE;
}

FrontierWipeBox *FrontierWipeBox_New(enum HeapID heapID) {
    FrontierWipeBox *box = Heap_Alloc(heapID, sizeof(FrontierWipeBox));
    memset(box, 0, sizeof(FrontierWipeBox));
    return box;
}

void FrontierWipeBox_Free(FrontierWipeBox *box) {
    Heap_Free(box);
}

void FrontierWipeBox_Start(FrontierWipeBox *box, s32 x0, s32 x1, s32 y0, s32 y1, s32 duration, Window *window, u32 width, u32 height, u8 color) {
    GF_ASSERT(box->active == FALSE);
    FrontierTransitionLerp_Init(&box->x, x0, x1, duration);
    FrontierTransitionLerp_Init(&box->y, y0, y1, duration);
    box->window = window;
    box->width = width;
    box->height = height;
    box->color = color;
    box->active = TRUE;
}

BOOL FrontierWipeBox_Update(FrontierWipeBox *box) {
    BOOL finished;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    u32 width;
    u32 height;

    if (box->active == FALSE) {
        return TRUE;
    }
    finished = FrontierTransitionLerp_Step(&box->x);
    FrontierTransitionLerp_Step(&box->y);
    width = box->width;
    left = (s16)(box->x.value - (width >> 1));
    height = box->height;
    right = (s16)(left + width);
    top = (s16)(box->y.value - (height >> 1));
    bottom = (s16)(top + height);
    FrontierWipeBox_FillRect(box->window, top, bottom, left, right, box->color);
    return finished;
}

void FrontierWipeBox_FillRect(Window *window, s32 top, s32 bottom, s32 left, s32 right, u8 color) {
    if (right > 0 && bottom > 0 && left != right && top != bottom) {
        if (left < 0) {
            left = 0;
        }
        if (right > 256) {
            right = 256;
        }
        if (top < 0) {
            top = 0;
        }
        if (bottom > 256) {
            bottom = 256;
        }
        FillWindowPixelRect(window, color, (u16)left, (u16)top, (u16)(right - left), (u16)(bottom - top));
    }
}

FrontierWipe *FrontierWipe_New(enum HeapID heapID) {
    FrontierWipe *wipe = Heap_Alloc(heapID, sizeof(FrontierWipe));
    int i;
    memset(wipe, 0, sizeof(FrontierWipe));
    for (i = 0; i < 96; i++) {
        wipe->boxes[i] = FrontierWipeBox_New(heapID);
    }
    return wipe;
}

void FrontierWipe_Free(FrontierWipe *wipe) {
    int i;
    for (i = 0; i < 96; i++) {
        FrontierWipeBox_Free(wipe->boxes[i]);
    }
    Heap_Free(wipe);
}

void FrontierWipe_Start(FrontierWipe *wipe, u8 duration, u8 delay, Window *window, u8 color) {
    wipe->window = window;
    wipe->color = color;
    wipe->duration = duration;
    wipe->delay = delay;
    wipe->started = 0;
    wipe->column = 0;
    wipe->row = 0;
    wipe->side = 0;
    wipe->finished = 0;
    wipe->timer = 0;
    wipe->active = TRUE;
}

// 12 rows of 32x16 boxes sweeping sideways; rows are started in pairs from the top and bottom edges of both halves.
#pragma force_active on
BOOL FrontierWipe_UpdateHorizontal(FrontierWipe *wipe) {
    int i;
    BOOL finished;
    int x;
    int x1;
    int y;

    if (wipe->active == FALSE) {
        return TRUE;
    }
    if (wipe->started < 96) {
        wipe->timer--;
        if (wipe->timer <= 0) {
            wipe->timer = wipe->delay;
            x = gFrontierWipeColumns[wipe->side][wipe->column] * 32;
            x1 = x + 32;
            y = wipe->row * 16 + 8;
            FrontierWipeBox_Start(wipe->boxes[wipe->started], x, x1, y, y, wipe->duration, wipe->window, 32, 16, wipe->color);
            x = gFrontierWipeColumns[wipe->side ^ 1][wipe->column] * 32;
            x1 = x + 32;
            y = (5 - wipe->row) * 16 + 8;
            FrontierWipeBox_Start(wipe->boxes[wipe->started + 1], x, x1, y, y, wipe->duration, wipe->window, 32, 16, wipe->color);
            x = gFrontierWipeColumns[wipe->side][wipe->column] * 32;
            x1 = x + 32;
            y = (wipe->row + 6) * 16 + 8;
            FrontierWipeBox_Start(wipe->boxes[wipe->started + 2], x, x1, y, y, wipe->duration, wipe->window, 32, 16, wipe->color);
            x = gFrontierWipeColumns[wipe->side ^ 1][wipe->column] * 32;
            x1 = x + 32;
            y = (11 - wipe->row) * 16 + 8;
            FrontierWipeBox_Start(wipe->boxes[wipe->started + 3], x, x1, y, y, wipe->duration, wipe->window, 32, 16, wipe->color);
            wipe->started += 4;
            wipe->column++;
            if (wipe->column % 8 == 0) {
                wipe->side ^= 1;
                wipe->row++;
                wipe->column = 0;
            }
        }
    }
    for (i = wipe->finished; i < wipe->started; i++) {
        finished = FrontierWipeBox_Update(wipe->boxes[i]);
        if (finished == TRUE) {
            wipe->finished++;
        }
    }
    if (wipe->finished >= 96 && finished == TRUE) {
        wipe->active = FALSE;
        return TRUE;
    }
    return FALSE;
}

// 16 columns of 16x32 boxes sweeping vertically, started in pairs from the left and right edges of both halves.
BOOL FrontierWipe_UpdateVertical(FrontierWipe *wipe) {
    int i;
    BOOL finished;

    if (!wipe->active) {
        return TRUE;
    }
    if (wipe->started < 96 && --wipe->timer <= 0) {
        s32 x;
        s32 y;
        wipe->timer = wipe->delay;
        x = wipe->column * 16 + 8;
        y = gFrontierWipeRows[wipe->side][wipe->row] * 32;
        FrontierWipeBox_Start(wipe->boxes[wipe->started], x, x, y, y + 32, wipe->duration, wipe->window, 16, 32, wipe->color);
        x = (7 - wipe->column) * 16 + 8;
        y = gFrontierWipeRows[wipe->side ^ 1][wipe->row] * 32;
        FrontierWipeBox_Start(wipe->boxes[wipe->started + 1], x, x, y, y + 32, wipe->duration, wipe->window, 16, 32, wipe->color);
        x = (wipe->column + 8) * 16 + 8;
        y = gFrontierWipeRows[wipe->side][wipe->row] * 32;
        FrontierWipeBox_Start(wipe->boxes[wipe->started + 2], x, x, y, y + 32, wipe->duration, wipe->window, 16, 32, wipe->color);
        x = (15 - wipe->column) * 16 + 8;
        y = gFrontierWipeRows[wipe->side ^ 1][wipe->row] * 32;
        FrontierWipeBox_Start(wipe->boxes[wipe->started + 3], x, x, y, y + 32, wipe->duration, wipe->window, 16, 32, wipe->color);
        wipe->started += 4;
        wipe->row++;
        if (wipe->row % 6 == 0) {
            wipe->side ^= 1;
            wipe->column++;
            wipe->row = 0;
        }
    }
    for (i = wipe->finished; i < wipe->started; i++) {
        finished = FrontierWipeBox_Update(wipe->boxes[i]);
        if (finished == TRUE) {
            wipe->finished++;
        }
    }
    if (wipe->finished >= 96 && finished == TRUE) {
        wipe->active = FALSE;
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

SysTask *FrontierScanline_CreateTask(FrontierScanline *scanline) {
    return SysTask_CreateOnVBlankQueue(FrontierScanline_VBlankTask, scanline, 1024);
}

void FrontierScanline_VBlankTask(SysTask *task, void *data) {
    FrontierScanline *scanline = data;
    if (scanline->counter >= 2) {
        ScanlineEffect_SwapBuffers(scanline->effect);
        scanline->counter = 0;
    }
    ScanlineEffect_RestartDma(scanline->effect);
    scanline->counter++;
}

void FrontierScanline_Init(FrontierScanline *scanline, enum HeapID heapID) {
    scanline->effect = ScanlineEffect_New(heapID);
    scanline->counter = 0;
    scanline->task = FrontierScanline_CreateTask(scanline);
}

void FrontierScanline_StartWave(FrontierScanline *scanline, u8 first, u8 last, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority) {
    ScanlineEffect_StartWave(scanline->effect, first, last, step, amplitude, speed, destination, fill, priority);
}

void FrontierHBlankDma_VWaitTask(SysTask *task, void *data) {
    FrontierHBlankDma *dma = data;
    if (dma->active) {
        dma->pending = TRUE;
        dma->onVWait(dma->arg);
    } else {
        HBlankDma_Stop();
    }
}

void FrontierHBlankDma_VBlankTask(SysTask *task, void *data) {
    FrontierHBlankDma *dma = data;
    if (dma->active && dma->pending) {
        dma->onVBlank(dma->arg);
        dma->pending = FALSE;
    }
}

void FrontierHBlankDma_StartTasks(FrontierHBlankDma *dma, void *arg, FrontierHBlankDmaCallback onVWait, FrontierHBlankDmaCallback onVBlank) {
    GF_ASSERT(dma != NULL);
    dma->active = TRUE;
    dma->pending = FALSE;
    dma->arg = arg;
    dma->onVWait = onVWait;
    dma->onVBlank = onVBlank;
    dma->vwaitTask = SysTask_CreateOnVWaitQueue(FrontierHBlankDma_VWaitTask, dma, 0);
    dma->vblankTask = SysTask_CreateOnVBlankQueue(FrontierHBlankDma_VBlankTask, dma, 0);
}

void FrontierHBlankDma_StopTasks(FrontierHBlankDma *dma) {
    GF_ASSERT(dma != NULL);
    if (dma->vwaitTask != NULL) {
        SysTask_Destroy(dma->vwaitTask);
    }
    if (dma->vblankTask != NULL) {
        SysTask_Destroy(dma->vblankTask);
    }
    HBlankDma_Stop();
}

void FrontierHBlankDma_StartDma(FrontierHBlankDma *dma) {
    void *buffer = DoubleBuffer_GetOther(dma->buffers);
    HBlankDma_Stop();
    HBlankDma_Start(buffer, dma->destination, sizeof(FrontierScanlineRegs), TRUE);
}

void FrontierHBlankDma_SwapAndRestart(FrontierHBlankDma *dma) {
    DoubleBuffer_Swap(dma->buffers);
    FrontierHBlankDma_StartDma(dma);
}

void FrontierHBlankDma_Restart(FrontierHBlankDma *dma) {
    FrontierHBlankDma_StartDma(dma);
}

FrontierHBlankDma *FrontierHBlankDma_New(void *destination, const FrontierScanlineRegs *initial, enum HeapID heapID) {
    FrontierHBlankDma *dma = Heap_Alloc(heapID, sizeof(FrontierHBlankDma));
    int i;
    memset(dma, 0, sizeof(FrontierHBlankDma));
    GF_ASSERT(dma != NULL);
    dma->buffers = DoubleBuffer_New(heapID, dma->lines[0], dma->lines[1]);
    GF_ASSERT(dma->buffers != NULL);
    dma->destination = destination;
    for (i = 0; i < 192; i++) {
        dma->lines[0][i] = *initial;
        dma->lines[1][i] = *initial;
    }
    FrontierHBlankDma_StartTasks(dma, dma, (FrontierHBlankDmaCallback)FrontierHBlankDma_SwapAndRestart, (FrontierHBlankDmaCallback)FrontierHBlankDma_Restart);
    return dma;
}

void FrontierHBlankDma_Delete(FrontierHBlankDma *dma) {
    GF_ASSERT(dma != NULL);
    FrontierHBlankDma_StopTasks(dma);
    if (dma->buffers != NULL) {
        DoubleBuffer_Delete(dma->buffers);
    }
    Heap_Free(dma);
}

void *FrontierHBlankDma_GetBuffer(FrontierHBlankDma *dma) {
    GF_ASSERT(dma != NULL);
    return DoubleBuffer_GetCurrent(dma->buffers);
}
