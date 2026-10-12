#include "screen_wipe_effects.h"

#include <nitro/fx/fx_trig.h>
#include <string.h>

#include "assert.h"

u32 FX_Sqrt(u32 value);

void RectWipe_Begin(ScreenFade *fade, const RectWipeConfig *config) {
    RectWipe *wipe;

    fade->effect = Heap_Alloc(fade->heapId, sizeof(RectWipe));
    wipe = fade->effect;
    RectWipe_Init(wipe, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue);
    if (config->window == 0) {
        ScreenFade_SetWindowVisible(fade->regQueue, 1, wipe->screen, wipe->queued);
    } else {
        ScreenFade_SetWindowVisible(fade->regQueue, 2, wipe->screen, wipe->queued);
    }
    fade->state++;
}

BOOL RectWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    RectWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (RectWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, fade->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        Heap_Free(wipe);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

void DoubleRectWipe_Begin(ScreenFade *fade, const RectWipeConfig *config0, const RectWipeConfig *config1) {
    RectWipe *wipes;

    fade->effect = Heap_Alloc(fade->heapId, sizeof(RectWipe) * 2);
    wipes = fade->effect;
    RectWipe_Init(&wipes[0], config0, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue);
    RectWipe_Init(&wipes[1], config1, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue);
    ScreenFade_SetWindowVisible(fade->regQueue, 3, fade->screen, wipes[0].queued);
    fade->state++;
}

BOOL DoubleRectWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    RectWipe *wipes = fade->effect;

    switch (fade->state) {
    case 1: {
        int finished = RectWipe_Step(&wipes[0]);
        finished += RectWipe_Step(&wipes[1]);
        if (finished == 2) {
            ScreenFade_SetEndWindows(wipes[0].queued, fade->regQueue, fade->screen);
            fade->state++;
        }
        break;
    }
    case 2:
        Heap_Free(wipes);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

void RectWipe_Init(RectWipe *wipe, const RectWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue) {
    ScreenFade_InitRectLerp(wipe->rect, wipe->end, wipe->delta, config->start, config->end, steps);
    wipe->screen = screen;
    wipe->window = config->window;
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->queued = config->queued;
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, config->window, screen, config->start[0], config->start[1], config->start[2], config->start[3], wipe->queued);
}

BOOL RectWipe_Step(RectWipe *wipe) {
    int stepsLeft;

    if (++wipe->frameCounter >= wipe->framesPerStep) {
        wipe->frameCounter = 0;
        stepsLeft = wipe->stepsLeft - 1;
        if (stepsLeft > 0) {
            wipe->stepsLeft = stepsLeft;
            ScreenFade_AddRect(wipe->rect, wipe->delta);
        } else {
            ScreenWindow_QueueSetRect(wipe->regQueue, wipe->end[0], wipe->end[1], wipe->end[2], wipe->end[3], wipe->window, wipe->screen);
            return TRUE;
        }
        ScreenWindow_QueueSetRect(wipe->regQueue, wipe->rect[0] / 128, wipe->rect[1] / 128, wipe->rect[2] / 128, wipe->rect[3] / 128, wipe->window, wipe->screen);
    }
    return FALSE;
}

void CircleWipe_Begin(ScreenFade *fade, const CircleWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(CircleWipe));
    CircleWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL CircleWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    CircleWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (CircleWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    default:
        GF_AssertFail();
    }
    return done;
}

void CircleWipe_Init(CircleWipe *wipe, const CircleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    WipeScanlineBuffer *buffer;
    int radiusStep;

    radiusStep = ScreenFade_StepDelta(config->startRadius, config->endRadius, steps);
    ScreenWipeScanlines_Init(&wipe->scanlines, config->window, screen, heapId);
    wipe->radius = config->startRadius << 7;
    wipe->centerX = config->centerX;
    wipe->centerY = config->centerY;
    wipe->radiusStep = radiusStep;
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    CircleWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
    buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, config->window, screen, buffer->nextLeft[0], 0, buffer->nextRight[0], 192, wipe->queued);
    if (config->window == 0) {
        ScreenFade_SetWindowVisible(regQueue, 1, screen, wipe->queued);
    } else {
        ScreenFade_SetWindowVisible(regQueue, 2, screen, wipe->queued);
    }
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL CircleWipe_Step(CircleWipe *wipe) {
    if (++wipe->frameCounter >= wipe->framesPerStep) {
        int stepsLeft;
        wipe->frameCounter = 0;
        stepsLeft = wipe->stepsLeft - 1;
        if (stepsLeft > 0) {
            wipe->stepsLeft = stepsLeft;
            wipe->radius += wipe->radiusStep;
            CircleWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

void CircleWipe_GetScanlineSpan(int radius, int centerX, int centerY, int y, int *left, int *right) {
    int pixelRadius = radius / 128;
    int distance = y - centerY;
    fx32 radiusFx;
    fx32 distanceFx;
    fx32 halfWidth;

    if (distance < 0) {
        distance = -distance;
    }
    if (distance >= pixelRadius) {
        *left = 0;
        *right = 0;
        return;
    }

    distanceFx = distance << FX32_SHIFT;
    radiusFx = pixelRadius << FX32_SHIFT;
    halfWidth = FX_Sqrt((u32)((((s64)radiusFx * radiusFx + 0x800) >> FX32_SHIFT) - (((s64)distanceFx * distanceFx + 0x800) >> FX32_SHIFT)));
    halfWidth >>= FX32_SHIFT;
    *left = centerX - halfWidth;
    if (*left < 0) {
        *left = 0;
    }
    *right = *left + halfWidth * 2;
    if (*right > 255) {
        *right = 255;
    }
}

void CircleWipe_BuildScanlines(CircleWipe *wipe) {
    WipeScanlineBuffer *buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    int y;

    for (y = 0; y < 192; y++) {
        int left;
        int right;

        if (y <= wipe->centerY) {
            CircleWipe_GetScanlineSpan(wipe->radius, wipe->centerX, wipe->centerY, y, &left, &right);
        } else if (y <= wipe->centerY * 2) {
            // mirror of the scanline above the centre
            left = buffer->nextLeft[wipe->centerY * 2 - y];
            right = buffer->nextRight[wipe->centerY * 2 - y];
        } else {
            CircleWipe_GetScanlineSpan(wipe->radius, wipe->centerX, wipe->centerY, y, &left, &right);
        }
        buffer->nextLeft[y] = left;
        buffer->nextRight[y] = right;
    }
}

void WedgeWipe_Begin(ScreenFade *fade, const AngleWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(WedgeWipe));
    WedgeWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL WedgeWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    WedgeWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (WedgeWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    default:
        GF_AssertFail();
    }
    return done;
}

void WedgeWipe_Init(WedgeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    WipeScanlineBuffer *buffer;

    wipe->angleStep = ScreenFade_StepDelta(config->startAngle, config->endAngle, steps);
    ScreenWipeScanlines_Init(&wipe->scanlines, config->window, screen, heapId);
    wipe->angle = config->startAngle << 7;
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    WedgeWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
    buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, config->window, screen, buffer->nextLeft[0], 0, buffer->nextRight[0], 192, wipe->queued);
    if (config->window == 0) {
        ScreenFade_SetWindowVisible(regQueue, 1, screen, wipe->queued);
    } else {
        ScreenFade_SetWindowVisible(regQueue, 2, screen, wipe->queued);
    }
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL WedgeWipe_Step(WedgeWipe *wipe) {
    if (++wipe->frameCounter >= wipe->framesPerStep) {
        int stepsLeft;
        wipe->frameCounter = 0;
        stepsLeft = wipe->stepsLeft - 1;
        if (stepsLeft > 0) {
            wipe->stepsLeft = stepsLeft;
            wipe->angle += wipe->angleStep;
            WedgeWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on

void WedgeWipe_BuildScanlines(WedgeWipe *wipe) {
    s32 halfWidths[192];
    int y;
    WipeScanlineBuffer *buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);

    ScreenFade_FillMulTan(wipe->angle / 128, halfWidths, 192, 0);
    for (y = 0; y < 192; y++) {
        buffer->nextLeft[y] = ScreenFade_AddClamped(128, -halfWidths[y]);
        buffer->nextRight[y] = ScreenFade_AddClamped(128, halfWidths[y]);
    }
}

#pragma force_active off

void SlopeWipe_Begin(ScreenFade *fade, const AngleWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(SlopeWipe));
    SlopeWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL SlopeWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    SlopeWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (SlopeWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

void SlopeWipe_Init(SlopeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    WipeScanlineBuffer *buffer;
    s32 angleStep = (config->endAngle - config->startAngle) / steps;

    ScreenWipeScanlines_Init(&wipe->scanlines, config->window, screen, heapId);
    wipe->amplitude = FX32_CONST(128);
    wipe->angle = config->startAngle;
    wipe->angleStep = angleStep;
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    SlopeWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
    buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, config->window, screen, buffer->nextLeft[96], 0, buffer->nextRight[96], 192, wipe->queued);
    if (config->window == 0) {
        ScreenFade_SetWindowVisible(regQueue, 1, screen, wipe->queued);
    } else {
        ScreenFade_SetWindowVisible(regQueue, 2, screen, wipe->queued);
    }
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL SlopeWipe_Step(SlopeWipe *wipe) {
    if (++wipe->frameCounter >= wipe->framesPerStep) {
        int stepsLeft;
        wipe->frameCounter = 0;
        stepsLeft = wipe->stepsLeft - 1;
        if (stepsLeft > 0) {
            wipe->stepsLeft = stepsLeft;
            wipe->angle += wipe->angleStep;
            SlopeWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on

void SlopeWipe_BuildScanlines(SlopeWipe *wipe) {
    s32 slope[192];
    s32 halfWidth;
    int count;
    WipeScanlineBuffer *buffer;

    buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    {
        s32 sine = FX_SinCosTable_[(wipe->angle >> 4) * 2];
        s64 product = (s64)sine * wipe->amplitude;
        halfWidth = ((s32)((product + 0x800) >> FX32_SHIFT)) >> FX32_SHIFT;
    }
    int angle = 65535 * (180 - ((halfWidth * 2 / 21) + 1) * 2) / 360 / 2;
    count = ScreenFade_HalfDivTan(angle, 256) >> FX32_SHIFT;
    int y;
    if (count >= 192) {
        GF_AssertFail();
    }
    ScreenFade_FillMulTan(angle, slope, count, 0);
    for (y = 0; y < 96; y++) {
        int pos = count - (y + 1);
        int width = halfWidth;
        int left;
        int right;
        if (pos > 0 && slope[pos] > halfWidth) {
            width = slope[pos];
        }
        left = ScreenFade_AddClamped(128, -width);
        right = ScreenFade_AddClamped(128, width);
        buffer->nextLeft[y] = left;
        buffer->nextRight[y] = right;
        buffer->nextLeft[191 - y] = left;
        buffer->nextRight[191 - y] = right;
    }
}

#pragma force_active off

void MultiRectWipe_Begin(ScreenFade *fade, const MultiRectWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(MultiRectWipe));
    MultiRectWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL MultiRectWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    MultiRectWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (MultiRectWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        MultiRectWipe_FreeRects(wipe);
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
        fade->state++;
        done = TRUE;
        break;
    case 3:
        done = TRUE;
        break;
    default:
        GF_AssertFail();
    }
    return done;
}

void MultiRectWipe_Init(MultiRectWipe *wipe, const MultiRectWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    int i;
    WipeScanlineBuffer *buffer;

    wipe->rects = Heap_Alloc(heapId, config->count * sizeof(RectLerp));
    if (wipe->rects == NULL) {
        GF_AssertFail();
    }
    wipe->count = config->count;
    for (i = 0; i < config->count; i++) {
        ScreenFade_InitRectLerp(wipe->rects[i].rect, wipe->rects[i].end, wipe->rects[i].delta, config->starts[i], config->ends[i], steps);
    }
    ScreenWipeScanlines_Init(&wipe->scanlines, config->window, screen, heapId);
    wipe->stepsLeft = steps;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    MultiRectWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
    buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, config->window, screen, buffer->nextLeft[0], 0, buffer->nextRight[0], 192, wipe->queued);
    if (config->window == 0) {
        ScreenFade_SetWindowVisible(wipe->regQueue, 1, screen, wipe->queued);
    } else {
        ScreenFade_SetWindowVisible(wipe->regQueue, 2, screen, wipe->queued);
    }
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL MultiRectWipe_Step(MultiRectWipe *wipe) {
    if (++wipe->frameCounter >= wipe->framesPerStep) {
        int stepsLeft;
        wipe->frameCounter = 0;
        stepsLeft = wipe->stepsLeft - 1;
        if (stepsLeft > 0) {
            wipe->stepsLeft = stepsLeft;
            MultiRectWipe_Advance(wipe);
            MultiRectWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

void MultiRectWipe_FreeRects(MultiRectWipe *wipe) {
    Heap_Free(wipe->rects);
    wipe->rects = NULL;
}

void MultiRectWipe_BuildScanlines(MultiRectWipe *wipe) {
    s32 i;

    // clears nextLeft and nextRight
    memset(ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0)->nextLeft, 0, sizeof(s16) * 192 * 2);
    for (i = wipe->count - 1; i >= 0; i--) {
        MultiRectWipe_DrawRect(wipe, &wipe->rects[i]);
    }
}

#pragma force_active on

void MultiRectWipe_DrawRect(MultiRectWipe *wipe, RectLerp *rect) {
    WipeScanlineBuffer *buffer = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    s32 y;
    s32 left = rect->rect[0] / 128, right = rect->rect[2] / 128, top = rect->rect[1] / 128, bottom = rect->rect[3] / 128;

    for (y = top; y < bottom; y++) {
        buffer->nextLeft[y] = left;
        buffer->nextRight[y] = right;
    }
}

#pragma force_active off

void MultiRectWipe_Advance(MultiRectWipe *wipe) {
    int i;

    for (i = 0; i < wipe->count; i++) {
        ScreenFade_AddRect(wipe->rects[i].rect, wipe->rects[i].delta);
    }
}

void SectorWipe_Begin(ScreenFade *fade, const SectorWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(SectorWipe));
    memset(fade->effect, 0, sizeof(SectorWipe));
    SectorWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL SectorWipe_Update(ScreenFade *fade) {
    SectorWipe *wipe = fade->effect;
    BOOL done = FALSE;

    switch (fade->state) {
    case 1:
        if (SectorWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        SectorWipe_Finish(wipe);
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
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

void SectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    WipeScanlineBuffer *buffer0;
    WipeScanlineBuffer *buffer1;

    wipe->phase.value = 0;
    wipe->phase.start = config->startPhase;
    wipe->phase.delta = config->endPhase - config->startPhase;
    ScreenWipeScanlines_Init(&wipe->scanlines, 2, screen, heapId);
    wipe->steps = steps;
    wipe->step = 0;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    SectorWipe_Interpolate(&wipe->phase, wipe->step, wipe->steps);
    SectorWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);

    buffer0 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    buffer1 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 1);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, 0, screen, buffer0->nextLeft[0], 0, buffer0->nextRight[0], 192, wipe->queued);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, 1, screen, buffer1->nextLeft[0], 0, buffer1->nextRight[0], 192, wipe->queued);
    ScreenFade_SetWindowVisible(regQueue, 3, screen, wipe->queued);
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL SectorWipe_Step(SectorWipe *wipe) {
    s32 next;

    wipe->frameCounter++;
    if (wipe->frameCounter >= wipe->framesPerStep) {
        wipe->frameCounter = 0;
        next = wipe->step + 1;
        if (next <= wipe->steps) {
            wipe->step = next;
            SectorWipe_Interpolate(&wipe->phase, wipe->step, wipe->steps);
            SectorWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

void SectorWipe_Finish(SectorWipe *wipe) {
}

#pragma force_active on

void SectorWipe_BuildScanlines(SectorWipe *wipe) {
    u32 phase = (u16)(wipe->phase.value % 0x3FFF);
    WipeScanlineBuffer *buffer0 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    WipeScanlineBuffer *buffer1 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 1);
    int y, x;
    u32 inv;

    // upper half: window 1 right of the centre line, window 0 mirrored below
    for (y = 0; y < 96; y++) {
        if (wipe->phase.value < 0x3FFF) {
            x = ScreenFade_MulTan(phase, 96 - y);
            if (x > 127) {
                x = 127;
            }
            buffer0->nextLeft[191 - y] = 128 - x;
            buffer0->nextRight[191 - y] = 128;
            buffer1->nextLeft[y] = 128;
            buffer1->nextRight[y] = x + 128;
        } else {
            buffer0->nextLeft[191 - y] = 0;
            buffer0->nextRight[191 - y] = 128;
            buffer1->nextLeft[y] = 128;
            buffer1->nextRight[y] = 255;
        }
    }
    inv = 0x3FFF - phase;

    for (y = 96; y < 192; y++) {
        if (wipe->phase.value < 0x3FFF) {
            buffer0->nextLeft[191 - y] = 128;
            buffer0->nextRight[191 - y] = 128;
            buffer1->nextLeft[y] = 128;
            buffer1->nextRight[y] = 128;
        } else {
            x = ScreenFade_MulTan(inv, y - 96);
            if (x > 127) {
                x = 127;
            }
            buffer0->nextLeft[191 - y] = 0;
            buffer0->nextRight[191 - y] = 128 - x;
            buffer1->nextLeft[y] = x + 128;
            buffer1->nextRight[y] = 255;
        }
    }
}

#pragma force_active off

void SectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator) {
    int value = interp->delta * numerator / denominator;
    interp->value = value + interp->start;
}

void MirroredSectorWipe_Begin(ScreenFade *fade, const SectorWipeConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(SectorWipe));
    memset(fade->effect, 0, sizeof(SectorWipe));
    MirroredSectorWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL MirroredSectorWipe_Update(ScreenFade *fade) {
    SectorWipe *wipe = fade->effect;
    BOOL done = FALSE;

    switch (fade->state) {
    case 1:
        if (MirroredSectorWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        MirroredSectorWipe_Finish(wipe);
        ScreenWipeScanlines_Free(&wipe->scanlines);
        Heap_Free(fade->effect);
        fade->effect = NULL;
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

void MirroredSectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    wipe->phase.value = config->startPhase;
    wipe->phase.start = config->startPhase;
    wipe->phase.delta = config->endPhase - config->startPhase;
    ScreenWipeScanlines_Init(&wipe->scanlines, 2, screen, heapId);
    wipe->steps = steps;
    wipe->step = 0;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    wipe->heapId = heapId;
    wipe->queued = config->queued;
    MirroredSectorWipe_BuildScanlines(wipe);
    SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
    ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 1);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, 0, screen, 0, 0, 255, 192, wipe->queued);
    ScreenFade_SetWindow(regQueue, config->insideMask, config->outsideMask, 1, screen, 0, 0, 255, 192, wipe->queued);
    ScreenFade_SetWindowVisible(regQueue, 3, screen, wipe->queued);
    sub_0200FF88(wipe->blankPair, wipe, ScreenWipeScanlines_HBlankCallback, screen, heapId);
}

BOOL MirroredSectorWipe_Step(SectorWipe *wipe) {
    wipe->frameCounter++;
    if (wipe->frameCounter >= wipe->framesPerStep) {
        wipe->frameCounter = 0;
        int next = wipe->step + 1;
        if (next <= wipe->steps) {
            wipe->step = next;
            MirroredSectorWipe_Interpolate(&wipe->phase, wipe->step, wipe->steps);
            MirroredSectorWipe_BuildScanlines(wipe);
            SysTask_CreateOnVWaitQueue(ScreenWipeScanlines_CommitTask, wipe, 0x3FF);
        } else {
            sub_0200FFB4(wipe->blankPair, wipe->scanlines.screen, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

void MirroredSectorWipe_Finish(SectorWipe *wipe) {
}

#pragma force_active on

void MirroredSectorWipe_BuildScanlines(SectorWipe *wipe) {
    int y;
    WipeScanlineBuffer *buffer0;
    WipeScanlineBuffer *buffer1;
    u32 inv;
    u32 phase = (u16)wipe->phase.value;

    buffer0 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 0);
    buffer1 = ScreenWipeScanlines_GetBuffer(&wipe->scanlines, 1);
    inv = 0x3FFF - phase;
    // both halves mirrored around the centre scanline
    for (y = 0; y < 96; y++) {
        u32 distance = 96 - y;
        int x0 = ScreenFade_MulTan(phase, distance);
        int x1 = ScreenFade_MulTan(inv, distance);
        s16 left0, right0, left1, right1;
        if (x0 > 127) {
            x0 = 127;
        }
        if (x1 > 127) {
            x1 = 127;
        }
        left0 = 128 - x1;
        buffer0->nextLeft[y] = left0;
        right0 = 128 - x0;
        buffer0->nextRight[y] = right0;
        buffer0->nextLeft[191 - y] = left0;
        buffer0->nextRight[191 - y] = right0;
        left1 = x0 + 128;
        buffer1->nextLeft[y] = left1;
        right1 = x1 + 128;
        buffer1->nextRight[y] = right1;
        buffer1->nextLeft[191 - y] = left1;
        buffer1->nextRight[191 - y] = right1;
    }
}

#pragma force_active off

void MirroredSectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator) {
    int value = interp->delta * numerator / denominator;
    value += interp->start;
    interp->value = value;
}

void LineShutterWipe_Begin(ScreenFade *fade, const LineShutterConfig *config) {
    fade->effect = Heap_Alloc(fade->heapId, sizeof(LineShutterWipe));
    memset(fade->effect, 0, sizeof(LineShutterWipe));
    LineShutterWipe_Init(fade->effect, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    fade->state++;
}

BOOL LineShutterWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    LineShutterWipe *wipe = fade->effect;

    switch (fade->state) {
    case 1:
        if (LineShutterWipe_Step(wipe) == TRUE) {
            ScreenFade_SetEndWindows(wipe->queued, wipe->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        LineShutterWipe_Finish(wipe);
        Heap_Free(fade->effect);
        fade->effect = NULL;
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

void LineShutterWipe_Init(LineShutterWipe *wipe, const LineShutterConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    LineMask_Init(&wipe->mask, screen, 1, 0, 0);
    if (config->queued == 0) {
        memset(wipe->mask.windows[0].lines, 1, 192);
        memset(wipe->mask.windows[0].latched, 1, 192);
    } else {
        memset(wipe->mask.windows[0].lines, 0, 192);
        memset(wipe->mask.windows[0].latched, 0, 192);
    }
    wipe->ranges = config->ranges;
    wipe->rangeCount = config->rangeCount;
    wipe->queued = config->queued;
    wipe->heapId = heapId;
    wipe->steps = steps;
    wipe->step = 0;
    wipe->framesPerStep = framesPerStep;
    wipe->frameCounter = 0;
    wipe->regQueue = regQueue;
    wipe->blankPair = blankPair;
    LineMask_StartHBlank(blankPair, &wipe->mask, heapId);
    if (config->queued == 1) {
        ScreenFade_SetWindow(regQueue, 0x20, 0x3F, 0, screen, 0, 0, 0, 0, config->queued);
    } else {
        ScreenFade_SetWindow(regQueue, 0x3F, 0x20, 0, screen, 0, 0, 0, 0, config->queued);
    }
    ScreenFade_SetWindowVisible(regQueue, 1, screen, wipe->queued);
}

BOOL LineShutterWipe_Step(LineShutterWipe *wipe) {
    wipe->frameCounter++;
    if (wipe->frameCounter >= wipe->framesPerStep) {
        wipe->frameCounter = 0;
        if (wipe->step + 1 <= wipe->steps) {
            wipe->step++;
            LineShutterWipe_BuildLines(wipe);
            LineMask_StartCommitTask(&wipe->mask);
        } else {
            LineMask_StopHBlank(wipe->blankPair, &wipe->mask, wipe->heapId);
            return TRUE;
        }
    }
    return FALSE;
}

void LineShutterWipe_Finish(LineShutterWipe *wipe) {
}

void LineShutterWipe_BuildLines(LineShutterWipe *wipe) {
    s32 i;

    for (i = 0; i < wipe->rangeCount; i++) {
        LineShutterWipe_FillRange(&wipe->ranges[i], wipe->mask.windows[0].lines, wipe->step, wipe->steps);
    }
}

void LineShutterWipe_FillRange(const LineShutterRange *range, u8 *lines, int step, int steps) {
    u32 start = range->start;
    u32 end = range->end;
    s32 boundary;
    s32 from;
    s32 to;
    u32 value;
    s32 y;

    boundary = (s32)(end - start) * step / steps;
    boundary += start;

    if (start <= end) {
        from = start;
        to = end;
        value = range->initial;
    } else {
        from = end;
        to = start;
        value = !range->initial;
    }

    for (y = from; y < to; y++) {
        if (y == boundary) {
            value = !value;
        }
        lines[y] = value;
    }
}

void RectShutterWipe_Begin(ScreenFade *fade, const RectShutterWipeConfig *config) {
    RectShutterWipe *wipe;

    fade->effect = Heap_Alloc(fade->heapId, sizeof(RectShutterWipe));
    memset(fade->effect, 0, sizeof(RectShutterWipe));

    wipe = fade->effect;

    if (config->rect.queued == 0) {
        RectShutterWipe_InitRectFirst(wipe, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    } else {
        RectShutterWipe_InitShutterFirst(wipe, config, fade->steps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
    }

    fade->state++;
}

BOOL RectShutterWipe_Update(ScreenFade *fade) {
    BOOL done = FALSE;
    RectShutterWipe *wipe = fade->effect;
    BOOL finished;

    switch (fade->state) {
    case 1:
        if (wipe->shutterFirst == 0) {
            finished = RectShutterWipe_StepRectFirst(wipe, fade);
        } else {
            finished = RectShutterWipe_StepShutterFirst(wipe, fade);
        }

        if (finished == TRUE) {
            ScreenFade_SetEndWindows(fade->fadeOut, fade->regQueue, fade->screen);
            fade->state++;
        }
        break;
    case 2:
        Heap_Free(wipe);
        fade->effect = NULL;
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

void RectShutterWipe_InitRectFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    s32 rectSteps = FX_Mul(steps << FX32_SHIFT, config->rectShare) >> FX32_SHIFT;

    wipe->laterSteps = steps - rectSteps;
    wipe->config = config;
    wipe->shutterFirst = config->rect.queued;

    RectWipe_Init(&wipe->rect, &config->rect, rectSteps, framesPerStep, screen, regQueue);

    if (config->rect.window == 0) {
        ScreenFade_SetWindowVisible(regQueue, 1, screen, config->rect.queued);
    } else {
        ScreenFade_SetWindowVisible(regQueue, 2, screen, config->rect.queued);
    }

    wipe->phase = 0;
}

BOOL RectShutterWipe_StepRectFirst(RectShutterWipe *wipe, ScreenFade *fade) {
    BOOL done = FALSE;

    switch (wipe->phase) {
    case 0:
        if (RectWipe_Step(&wipe->rect) == TRUE) {
            wipe->phase++;
            LineShutterWipe_Init(&wipe->shutter, &wipe->config->shutter, wipe->laterSteps, fade->framesPerStep, fade->screen, fade->regQueue, fade->blankPair, fade->heapId);
        }
        break;
    case 1:
        if (LineShutterWipe_Step(&wipe->shutter) == TRUE) {
            done = TRUE;
            wipe->phase++;
        }
        break;
    case 2:
        done = TRUE;
        break;
    }

    return done;
}

void RectShutterWipe_InitShutterFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId) {
    s32 rectSteps = FX_Mul(steps << FX32_SHIFT, config->rectShare) >> FX32_SHIFT;
    s32 shutterSteps;

    wipe->laterSteps = rectSteps;
    shutterSteps = steps - wipe->laterSteps;
    wipe->config = config;
    wipe->shutterFirst = config->rect.queued;

    LineShutterWipe_Init(&wipe->shutter, &wipe->config->shutter, shutterSteps, framesPerStep, screen, regQueue, blankPair, heapId);

    wipe->phase = 0;
}

BOOL RectShutterWipe_StepShutterFirst(RectShutterWipe *wipe, ScreenFade *fade) {
    BOOL done = FALSE;

    switch (wipe->phase) {
    case 0:
        if (LineShutterWipe_Step(&wipe->shutter) == TRUE) {
            wipe->phase++;
            RectWipe_Init(&wipe->rect, &wipe->config->rect, wipe->laterSteps, fade->framesPerStep, fade->screen, fade->regQueue);

            if (wipe->config->rect.window == 0) {
                ScreenFade_SetWindowVisible(fade->regQueue, 1, fade->screen, wipe->config->rect.queued);
            } else {
                ScreenFade_SetWindowVisible(fade->regQueue, 2, fade->screen, wipe->config->rect.queued);
            }
        }
        break;
    case 1:
        if (RectWipe_Step(&wipe->rect) == TRUE) {
            done = TRUE;
            wipe->phase++;
        }
        break;
    case 2:
        done = TRUE;
        break;
    }

    return done;
}
