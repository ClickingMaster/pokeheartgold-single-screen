#include "battle/battle_anim_util.h"

#include "global.h"

#include "geometry_math.h"
#include "heap.h"
#include "palette.h"
#include "pokepic.h"
#include "scanline_effect.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"

extern const BattleAnimFunc sBattleAnimFuncTable[];
extern const BattleAnimSpriteFunc sBattleAnimSpriteFuncTable[];

#pragma force_active on

void BattleAnimUtil_InitSinMotionFromStep(BattleAnimSinMotion *motion, u32 startX, u32 endX, u32 startY, u16 endY, fx32 ampX, fx32 ampY, u16 step) {
    s16 signedStep;

    GF_ASSERT(motion != NULL);
    if (startX > endX) {
        step = -step;
    }
    signedStep = (s16)step;
    motion->duration = ov07_0222204C(startX << FX32_SHIFT, endX << FX32_SHIFT, signedStep << FX32_SHIFT);
    motion->angleX = startX;
    motion->ampX = ampX;
    motion->angleY = startY;
    motion->ampY = ampY;
    motion->stepX = signedStep;
    motion->stepY = ((int)endY - (int)startY) / motion->duration;
}

BOOL BattleAnimUtil_UpdateSinMotion(BattleAnimSinMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->angleX += motion->stepX;
        motion->angleY += motion->stepY;
        motion->angleX = (u16)motion->angleX;
        motion->angleY = (u16)motion->angleY;
        motion->duration--;
        motion->x = FX_Mul(FX_SinIdx(motion->angleX), motion->ampX) >> FX32_SHIFT;
        motion->y = FX_Mul(FX_CosIdx(motion->angleY), motion->ampY) >> FX32_SHIFT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateSinMotionSprite(BattleAnimSinMotion *motion, s16 x, s16 y, ManagedSprite *sprite) {
    if (BattleAnimUtil_UpdateSinMotion(motion)) {
        ov07_0222207C(motion, sprite, x, y);
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateSinMotionPokepic(BattleAnimSinMotion *motion, int x, int y, Pokepic *pokepic) {
    if (BattleAnimUtil_UpdateSinMotion(motion)) {
        ov07_022220B8(motion, pokepic, x, y);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void BattleAnimUtil_InitLinearMotion(BattleAnimLinearMotion *motion, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration) {
    GF_ASSERT(motion != NULL);
    motion->x = startX;
    motion->y = startY;
    motion->duration = duration;
    motion->stepX = ov07_02222040(startX << FX32_SHIFT, endX << FX32_SHIFT, duration);
    motion->stepY = ov07_02222040(startY << FX32_SHIFT, endY << FX32_SHIFT, duration);
    motion->curX = startX << FX32_SHIFT;
    motion->curY = startY << FX32_SHIFT;
}

BOOL BattleAnimUtil_UpdateLinearMotion(BattleAnimLinearMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->curX += motion->stepX;
        motion->curY += motion->stepY;
        motion->x = motion->curX >> FX32_SHIFT;
        motion->y = motion->curY >> FX32_SHIFT;
        motion->duration--;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateLinearMotionSprite(BattleAnimLinearMotion *motion, ManagedSprite *sprite) {
    if (BattleAnimUtil_UpdateLinearMotion(motion)) {
        ov07_0222207C(motion, sprite, 0, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateLinearMotionPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic) {
    if (BattleAnimUtil_UpdateLinearMotion(motion)) {
        ov07_022220B8(motion, pokepic, 0, 0);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Linear motion plus a vertical arc (half a cosine period) of the given amplitude
void BattleAnimUtil_InitLinearSinMotion(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration, fx32 amplitude) {
    BattleAnimUtil_InitLinearMotion(linear, startX, endX, startY, endY, duration);
    sin->x = 0;
    sin->y = 0;
    ov07_022220FC(sin, 0, 0, 0x3FFF, 0xBFFF, 0, amplitude, duration);
}

#pragma force_active reset

BOOL BattleAnimUtil_UpdateLinearSinMotion(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin) {
    BOOL linearActive;
    BOOL sinActive;

    GF_ASSERT(linear != NULL);
    GF_ASSERT(sin != NULL);
    linearActive = BattleAnimUtil_UpdateLinearMotion(linear);
    sinActive = BattleAnimUtil_UpdateSinMotion(sin);
    linear->x += sin->x;
    linear->y += sin->y;
    if (linearActive == sinActive) {
        if (linearActive == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, ManagedSprite *sprite) {
    if (BattleAnimUtil_UpdateLinearSinMotion(linear, sin)) {
        ov07_0222207C(linear, sprite, 0, 0);
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_InitValueMotion(BattleAnimValueMotion *motion, int start, int end, int duration) {
    GF_ASSERT(motion != NULL);
    motion->value = start;
    motion->duration = duration;
    motion->step = ov07_02222040(start << FX32_SHIFT, end << FX32_SHIFT, duration) >> FX32_SHIFT;
}

void BattleAnimUtil_InitValueMotionFx(BattleAnimValueMotion *motion, int start, int end, int duration) {
    GF_ASSERT(motion != NULL);
    motion->value = start;
    motion->duration = duration;
    motion->step = ov07_02222040(start << FX32_SHIFT, end << FX32_SHIFT, duration);
    motion->current = start << FX32_SHIFT;
}

BOOL BattleAnimUtil_UpdateValueMotion(BattleAnimValueMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->value += motion->step;
        motion->duration--;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateValueMotionFx(BattleAnimValueMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->current += motion->step;
        motion->value = motion->current >> FX32_SHIFT;
        motion->duration--;
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_InitCosMotion(BattleAnimCosMotion *motion, u32 startAngle, u32 endAngle, u32 amplitude, u32 duration) {
    motion->duration = duration;
    motion->angle = startAngle;
    motion->amplitude = amplitude;
    motion->angleStep = (endAngle - startAngle) / duration;
}

#pragma force_active on

BOOL BattleAnimUtil_UpdateCosMotion(BattleAnimCosMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->angle = (u16)(motion->angle + motion->angleStep);
        motion->duration--;
        motion->value = FX_Mul(FX_CosIdx(motion->angle), motion->amplitude) >> FX32_SHIFT;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// Uniform scale from start / divisor to end / divisor (x and y in 1/256 units)
void BattleAnimUtil_InitScaleMotion(BattleAnimLinearMotion *motion, int start, int divisor, int end, int duration) {
    GF_ASSERT(motion != NULL);
    motion->duration = duration;
    int startScale = (start << 8) / divisor;
    int endScale = (end << 8) / divisor;
    motion->stepX = ov07_02222040(startScale << FX32_SHIFT, endScale << FX32_SHIFT, duration);
    motion->x = startScale;
    motion->y = startScale;
    motion->curX = motion->x << FX32_SHIFT;
    motion->curY = motion->y << FX32_SHIFT;
}

BOOL BattleAnimUtil_UpdateScaleMotion(BattleAnimLinearMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->duration--;
        motion->curX += motion->stepX;
        motion->curY += motion->stepX;
        motion->x = motion->curX >> FX32_SHIFT;
        motion->y = motion->curY >> FX32_SHIFT;
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_InitScaleMotionXY(BattleAnimLinearMotion *motion, int startX, int endX, int startY, s16 endY, s16 divisor, int duration) {
    GF_ASSERT(motion != NULL);
    motion->duration = duration;
    int startScaleX = (startX << 8) / divisor;
    int endScaleX = (endX << 8) / divisor;
    motion->stepX = ov07_02222040(startScaleX << FX32_SHIFT, endScaleX << FX32_SHIFT, duration);
    int startScaleY = (startY << 8) / divisor;
    int endScaleY = (endY << 8) / divisor;
    motion->stepY = ov07_02222040(startScaleY << FX32_SHIFT, endScaleY << FX32_SHIFT, duration);
    motion->x = startScaleX;
    motion->y = startScaleY;
    motion->curX = motion->x << FX32_SHIFT;
    motion->curY = motion->y << FX32_SHIFT;
}

BOOL BattleAnimUtil_UpdateScaleMotionXY(BattleAnimLinearMotion *motion) {
    GF_ASSERT(motion != NULL);
    if (motion->duration != 0) {
        motion->duration--;
        motion->curX += motion->stepX;
        motion->curY += motion->stepY;
        motion->x = motion->curX >> FX32_SHIFT;
        motion->y = motion->curY >> FX32_SHIFT;
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_GetScaleFloat(BattleAnimLinearMotion *motion, f32 *x, f32 *y) {
    *x = (f32)motion->x / 256.0f;
    *y = (f32)motion->y / 256.0f;
}

// Half the height an 80 pixel tall picture (less 2 * inset) loses at the given scale (1/256 units)
s16 BattleAnimUtil_CalcScaleYOffset(int y, int inset, fx32 scale) {
    fx32 whole;
    fx32 height = (80 - (inset << 1)) << FX32_SHIFT;
    fx32 scaled = FX_Mul(height, scale);
    fx32 offset = height - FX_Div(scaled, 256 << FX32_SHIFT);
    fx32 frac = FX_Modf(offset, &whole);
    if (frac != 0) {
        frac += FX32_ONE / 2;
    }
    int pixels = (whole + frac) >> FX32_SHIFT;
    return pixels / 2;
}

#pragma force_active on

void BattleAnimUtil_AlignPokepicToScale(Pokepic *pokepic, int y, int inset, fx32 scale, int mode) {
    int offset = BattleAnimUtil_CalcScaleYOffset(y, inset, scale);
    if (mode == 1) {
        offset = (s16)(offset * ~0u); // negated by a multiply
        y = (s16)(y - inset);
    }
    Pokepic_SetAttr(pokepic, POKEPIC_Y, y + offset);
}

void BattleAnimUtil_AlignSpriteToScale(ManagedSprite *sprite, int y, int inset, fx32 scale, int mode) {
    s16 spriteX;
    s16 spriteY;
    int offset = BattleAnimUtil_CalcScaleYOffset(y, inset, scale);
    if (mode == 1) {
        offset = (s16)(offset * ~0u); // negated by a multiply
        y = (s16)(y - inset);
    }
    ManagedSprite_GetPositionXY(sprite, &spriteX, &spriteY);
    ManagedSprite_SetPositionXY(sprite, spriteX, (s16)(y + offset));
}

#pragma force_active reset

BOOL BattleAnimUtil_UpdateScaleMotionSprite(BattleAnimLinearMotion *motion, ManagedSprite *sprite) {
    if (BattleAnimUtil_UpdateScaleMotion(motion)) {
        ov07_0222209C(motion, sprite);
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateScaleMotionPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic) {
    if (BattleAnimUtil_UpdateScaleMotion(motion)) {
        ov07_022220DC(motion, pokepic);
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateScaleMotionXYPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic) {
    if (BattleAnimUtil_UpdateScaleMotionXY(motion)) {
        ov07_022220DC(motion, pokepic);
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_InitShake(BattleAnimShake *shake, int ampX, int ampY, int interval, int count) {
    GF_ASSERT(shake != NULL);
    shake->timer = interval;
    shake->interval = interval;
    shake->count = count;
    shake->phase = 0;
    shake->ampX = ampX;
    shake->ampY = ampY;
    shake->prevX = -ampX;
    shake->prevY = -ampY;
    shake->x = 0;
    shake->y = 0;
}

BOOL BattleAnimUtil_UpdateShake(BattleAnimShake *shake) {
    GF_ASSERT(shake != NULL);
    if (shake->count != 0) {
        shake->timer++;
        if (shake->timer >= shake->interval) {
            shake->timer = 0;
            int prevX = shake->prevX;
            shake->prevX = shake->x;
            if (prevX == 0) {
                shake->x = 0;
            } else {
                shake->x = -prevX;
            }
            int prevY = shake->prevY;
            shake->prevY = shake->y;
            if (prevY == 0) {
                shake->y = 0;
            } else {
                shake->y = -prevY;
            }
            shake->phase++;
            if (shake->phase >= 4) {
                shake->phase = 0;
                shake->count--;
            }
        }
        return TRUE;
    }
    return FALSE;
}

BOOL BattleAnimUtil_UpdateShakePokepic(BattleAnimShake *shake, int x, int y, Pokepic *pokepic) {
    if (BattleAnimUtil_UpdateShake(shake)) {
        ov07_022220B8(shake, pokepic, x, y);
        return TRUE;
    }
    return FALSE;
}

void BattleAnimUtil_InitSpriteSequence(BattleAnimSpriteSequence *sequence, const BattleAnimSinMotion *motion, BattleAnimSinMotionFunc update, s16 x, s16 y, u16 interval, u8 count, u8 scale, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3) {
    int i;

    sequence->x = x;
    sequence->y = y;
    sequence->update = update;
    sequence->interval = interval;
    sequence->timer = 0;
    sequence->count = count;
    sequence->started = 0;
    sequence->scale = scale;
    for (i = 0; i < count; i++) {
        sequence->motions[i] = *motion;
    }
    sequence->sprites[0] = sprite0;
    sequence->sprites[1] = sprite1;
    sequence->sprites[2] = sprite2;
    sequence->sprites[3] = sprite3;
    for (i = 0; i < sequence->count; i++) {
        ManagedSprite_SetDrawFlag(sequence->sprites[i], FALSE);
    }
}

BOOL BattleAnimUtil_UpdateSpriteSequence(BattleAnimSpriteSequence *sequence) {
    BOOL active[4];
    int i;

    for (i = 0; i < 4; i++) {
        active[i] = TRUE;
    }

    if (sequence->interval <= sequence->timer) {
        ManagedSprite_SetDrawFlag(sequence->sprites[sequence->started], TRUE);
        sequence->started++;
        sequence->timer = 0;
    }
    if (sequence->started < sequence->count) {
        sequence->timer++;
    }
    for (i = 0; i < sequence->started; i++) {
        active[i] = sequence->update(&sequence->motions[i]);
        if (active[i]) {
            if (sequence->scale == FALSE) {
                ManagedSprite_SetPositionXY(sequence->sprites[i], sequence->x + sequence->motions[i].x, sequence->y + sequence->motions[i].y);
            } else {
                f32 x;
                f32 y;
                BattleAnimUtil_GetScaleFloat((BattleAnimLinearMotion *)&sequence->motions[i], &x, &y);
                ManagedSprite_SetAffineScale(sequence->sprites[i], x, y);
            }
        } else {
            ManagedSprite_SetDrawFlag(sequence->sprites[i], FALSE);
        }
    }
    for (i = 0; i < sequence->count; i++) {
        if (active[i] == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

void ov07_02222A44(BattleAnimSinMotion *motion, int multiplier, int duration) {
    ov07_022220FC(motion, 0, 0xFFFF, 0, 0xFFFF, 2 << 16, -0x8000, duration);
    motion->duration *= multiplier;
}

void BattleAnimTask_BlendAlphaMotion(SysTask *task, void *data) {
    BattleAnimBlendAlphaMotion *blend = data;
    BattleAnimLinearMotion *motion = &blend->motion;

    if (!BattleAnimUtil_UpdateLinearMotion(motion)) {
        blend->done = TRUE;
        SysTask_Destroy(task);
        return;
    }
    if (motion->x < 0) {
        motion->x = 0;
    }
    if (motion->y < 0) {
        motion->y = 0;
    }
    reg_G2_BLDALPHA = motion->x | (motion->y << 8);
}

void BattleAnimUtil_StartBlendAlphaMotion(BattleAnimBlendAlphaMotion *blend, s16 startEva, s16 endEva, s16 startEvb, s16 endEvb, int duration) {
    BattleAnimUtil_InitLinearMotion(&blend->motion, startEva, endEva, startEvb, endEvb, (u16)duration);
    blend->done = FALSE;
    SysTask_CreateOnMainQueue(BattleAnimTask_BlendAlphaMotion, blend, 0);
}

BOOL BattleAnimUtil_IsBlendAlphaMotionDone(BattleAnimBlendAlphaMotion *blend) {
    return blend->done;
}

void BattleAnimScanline_VWaitTask(SysTask *task, void *data) {
    BattleAnimScanlineTasks *tasks = data;

    if (tasks->active) {
        tasks->pending = TRUE;
        tasks->vwaitCallback(tasks->callbackArg);
    } else {
        HBlankDma_Stop();
    }
}

void BattleAnimScanline_VBlankTask(SysTask *task, void *data) {
    BattleAnimScanlineTasks *tasks = data;

    if (tasks->active && tasks->pending) {
        tasks->vblankCallback(tasks->callbackArg);
        tasks->pending = FALSE;
    }
}

void BattleAnimScanline_StartTasks(BattleAnimScanlineTasks *tasks, void *arg, BattleAnimScanlineCallback vwaitCallback, BattleAnimScanlineCallback vblankCallback) {
    GF_ASSERT(tasks != NULL);
    tasks->active = TRUE;
    tasks->pending = FALSE;
    tasks->callbackArg = arg;
    tasks->vwaitCallback = vwaitCallback;
    tasks->vblankCallback = vblankCallback;
    tasks->vwaitTask = SysTask_CreateOnVWaitQueue(BattleAnimScanline_VWaitTask, tasks, 0);
    tasks->vblankTask = SysTask_CreateOnVBlankQueue(BattleAnimScanline_VBlankTask, tasks, 0);
}

void BattleAnimScanline_StopTasks(BattleAnimScanlineTasks *tasks) {
    GF_ASSERT(tasks != NULL);
    if (tasks->vwaitTask != NULL) {
        SysTask_Destroy(tasks->vwaitTask);
    }
    if (tasks->vblankTask != NULL) {
        SysTask_Destroy(tasks->vblankTask);
    }
    HBlankDma_Stop();
}

void BattleAnimScanline_Deactivate(BattleAnimScanlineTasks *tasks) {
    GF_ASSERT(tasks != NULL);
    tasks->active = FALSE;
}

void BattleAnimScanline_StartHBlankDma(BattleAnimScanline *scanline) {
    void *buffer = DoubleBuffer_GetOther(scanline->doubleBuffer);
    HBlankDma_Stop();
    HBlankDma_Start(buffer, scanline->destination, 4, 1);
}

void BattleAnimScanline_SwapAndStartDma(BattleAnimScanline *scanline) {
    DoubleBuffer_Swap(scanline->doubleBuffer);
    BattleAnimScanline_StartHBlankDma(scanline);
}

void BattleAnimScanline_VBlankCallback(BattleAnimScanline *scanline) {
    BattleAnimScanline_StartHBlankDma(scanline);
}

BattleAnimScanline *BattleAnimScanline_New(void *destination, u32 fill, enum HeapID heapId) {
    BattleAnimScanline *scanline = Heap_Alloc(heapId, sizeof(BattleAnimScanline));
    memset(scanline, 0, sizeof(BattleAnimScanline));
    GF_ASSERT(scanline != NULL);
    scanline->doubleBuffer = DoubleBuffer_New(heapId, scanline->buffers[0], scanline->buffers[1]);
    GF_ASSERT(scanline->doubleBuffer != NULL);
    scanline->destination = destination;
    MI_CpuFill32(scanline->buffers[0], fill, sizeof(scanline->buffers[0]));
    MI_CpuFill32(scanline->buffers[1], fill, sizeof(scanline->buffers[1]));
    BattleAnimScanline_StartTasks(&scanline->tasks, scanline, (BattleAnimScanlineCallback)BattleAnimScanline_SwapAndStartDma, (BattleAnimScanlineCallback)BattleAnimScanline_VBlankCallback);
    return scanline;
}

void BattleAnimScanline_Free(BattleAnimScanline *scanline) {
    GF_ASSERT(scanline != NULL);
    BattleAnimScanline_StopTasks(&scanline->tasks);
    if (scanline->doubleBuffer != NULL) {
        DoubleBuffer_Delete(scanline->doubleBuffer);
    }
    Heap_Free(scanline);
}

u32 *BattleAnimScanline_GetWriteBuffer(BattleAnimScanline *scanline) {
    GF_ASSERT(scanline != NULL);
    return DoubleBuffer_GetCurrent(scanline->doubleBuffer);
}

void BattleAnimScanline_Disable(BattleAnimScanline *scanline) {
    GF_ASSERT(scanline != NULL);
    BattleAnimScanline_Deactivate(&scanline->tasks);
}

void BattleAnimScanline_WaveVWaitCallback(BattleAnimScanlineWave *wave) {
    ScanlineEffect_SwapBuffers(wave->effect);
    ScanlineEffect_RestartDma(wave->effect);
}

void BattleAnimScanline_WaveVBlankCallback(BattleAnimScanlineWave *wave) {
    ScanlineEffect_RestartDma(wave->effect);
}

#pragma force_active on

BattleAnimScanlineWave *BattleAnimScanline_NewWave(u8 firstLine, u8 lastLine, u8 step, fx32 amplitude, s16 speed, int bgLayer, u32 priority, u32 fill, enum HeapID heapId) {
    BattleAnimScanlineWave *wave = Heap_Alloc(heapId, sizeof(BattleAnimScanlineWave));
    void *destination;

    GF_ASSERT(wave != NULL);
    memset(wave, 0, sizeof(BattleAnimScanlineWave));
    destination = BattleAnimUtil_GetBgOffsetRegister(bgLayer);
    wave->effect = ScanlineEffect_New(heapId);
    ScanlineEffect_StartWave(wave->effect, firstLine, lastLine, step, amplitude, speed, destination, fill, priority);
    BattleAnimScanline_StartTasks(&wave->tasks, wave, (BattleAnimScanlineCallback)BattleAnimScanline_WaveVWaitCallback, (BattleAnimScanlineCallback)BattleAnimScanline_WaveVBlankCallback);
    return wave;
}

#pragma force_active reset

void BattleAnimScanline_FreeWave(BattleAnimScanlineWave *wave) {
    GF_ASSERT(wave != NULL);
    BattleAnimScanline_StopTasks(&wave->tasks);
    if (wave->effect != NULL) {
        ScanlineEffect_Delete(wave->effect);
    }
    Heap_Free(wave);
}

void *BattleAnimScanline_GetWaveBuffer(BattleAnimScanlineWave *wave) {
    GF_ASSERT(wave != NULL);
    return ScanlineEffect_GetBuffer(wave->effect);
}

void BattleAnimScanline_DisableWave(BattleAnimScanlineWave *wave) {
    GF_ASSERT(wave != NULL);
    BattleAnimScanline_Deactivate(&wave->tasks);
}

// Two 16-bit values in one scanline buffer word (BGnHOFS, BGnVOFS)
u32 BattleAnimUtil_PackXY(u16 x, u16 y) {
    return x | (y << 16);
}

void *BattleAnimUtil_GetBgOffsetRegister(int bgLayer) {
    switch (bgLayer) {
    case 0:
        return (void *)REG_BG0HOFS_ADDR;
    case 1:
        return (void *)REG_BG1HOFS_ADDR;
    case 2:
        return (void *)REG_BG2HOFS_ADDR;
    case 3:
        return (void *)REG_BG3HOFS_ADDR;
    }
    return (void *)bgLayer;
}

void BattleAnimUtil_CalcMidpoint(int x0, int y0, int x1, int y1, s16 *x, s16 *y) {
    *x = (x0 + x1) / 2;
    *y = (y0 + y1) / 2;
}

#pragma force_active on

void BattleAnimUtil_CalcDistance(s16 x, s16 y, s16 targetX, s16 targetY, fx32 *distance) {
    s16 dy = -(y - targetY);
    s16 dx = x - targetX;
    *distance = FX_Sqrt((dy * dy + dx * dx) << FX32_SHIFT);
}

// Angle (0..0xFFFF) of the direction from (x, y) to (targetX, targetY), y up
void BattleAnimUtil_CalcAngle(s16 x, s16 y, s16 targetX, s16 targetY, u16 *angle) {
    s16 dy = -(y - targetY);
    s16 dx = x - targetX;
    *angle = FX_Atan2Idx(dy << FX32_SHIFT, dx << FX32_SHIFT);
    if (*angle && dy < 0) {
        *angle = (*angle - 0x7FFF) * 0xFFFF;
    }
}

#pragma force_active reset

// Moves *value by step; TRUE (value = target) once it reaches or passes target
BOOL BattleAnimUtil_StepTowards(int *value, int target, int step) {
    if (step < 0) {
        int next = *value + step;
        if (next > target) {
            *value = next;
            return FALSE;
        }
        *value = target;
        return TRUE;
    } else {
        int next = *value + step;
        if (next < target) {
            *value = next;
            return FALSE;
        }
        *value = target;
        return TRUE;
    }
}

#pragma force_active on

void BattleAnimTask_PaletteBlend(SysTask *task, void *data) {
    BattleAnimPaletteBlend *blend = data;

    if (blend->active) {
        blend->timer++;
        if (blend->timer >= blend->delay) {
            int next;
            s8 nextS8;
            blend->timer = 0;
            PaletteData_BlendPalette(blend->paletteData, blend->bufferId, blend->offset, blend->size, blend->current, blend->color);
            if (blend->current == blend->end) {
                blend->active = FALSE;
                return;
            }
            next = blend->current + blend->step;
            nextS8 = next;
            if (blend->step > 0) {
                if (nextS8 > blend->end) {
                    blend->current = blend->end;
                } else {
                    blend->current = next;
                }
            } else {
                if (nextS8 < blend->end) {
                    blend->current = blend->end;
                } else {
                    blend->current = next;
                }
            }
        }
    }
}

#pragma force_active reset

BOOL BattleAnimUtil_IsPaletteBlendActive(BattleAnimPaletteBlend *blend) {
    GF_ASSERT(blend != NULL);
    return blend->active;
}

void BattleAnimUtil_FreePaletteBlend(BattleAnimPaletteBlend *blend) {
    GF_ASSERT(blend != NULL);
    SysTask_Destroy(blend->task);
    Heap_Free(blend);
}

#pragma force_active on

BattleAnimPaletteBlend *BattleAnimUtil_StartPaletteBlend(PaletteData *paletteData, enum HeapID heapId, PaletteBufferId bufferId, u16 offset, u16 size, s8 delay, s8 step, u8 start, u8 end, u16 color, u32 priority) {
    BattleAnimPaletteBlend *blend = Heap_Alloc(heapId, sizeof(BattleAnimPaletteBlend));
    GF_ASSERT(blend != NULL);
    blend->paletteData = paletteData;
    blend->bufferId = bufferId;
    blend->offset = offset;
    blend->size = size;
    blend->color = color;
    blend->start = start;
    blend->end = end;
    blend->current = start;
    blend->delay = delay;
    blend->timer = delay;
    if (blend->start < blend->end) {
        blend->step = step;
    } else {
        blend->step = -step;
    }
    blend->active = TRUE;
    blend->task = SysTask_CreateOnMainQueue(BattleAnimTask_PaletteBlend, blend, priority);
    return blend;
}

void BattleAnimUtil_PaletteToGrayscale(u16 *palette, int count) {
    int i;
    for (i = 0; i < count; i++) {
        int color = *palette;
        int b = (color >> 10) & 0x1F;
        int r = color & 0x1F;
        int g = (color >> 5) & 0x1F;
        int level = (b * 29 + (r * 76 + g * 151)) >> 8;
        *palette = (level << 10) | (level << 5) | level;
        palette++;
    }
}

void BattleAnimSystem_ApplyGrayscale(BattleAnimSystem *system) {
    u16 *buffer = PaletteData_GetFadedBuf(BattleAnimSystem_GetPaletteData(system), PLTTBUF_MAIN_BG);
    if (ov07_0221BFC0(system) == 1) {
        BattleAnimUtil_PaletteToGrayscale(buffer, 48);
    } else {
        BattleAnimUtil_PaletteToGrayscale(buffer, 128);
    }
}

#pragma force_active reset

void BattleAnimSystem_RestoreFromGrayscale(BattleAnimSystem *system) {
    PaletteData *paletteData = BattleAnimSystem_GetPaletteData(system);
    if (ov07_0221BFC0(system) == 1) {
        PaletteData_CopyPalette(paletteData, PLTTBUF_MAIN_BG, 0, PLTTBUF_MAIN_BG, 0, 48 * sizeof(u16));
        return;
    }
    PaletteData_CopyPalette(paletteData, PLTTBUF_MAIN_BG, 0, PLTTBUF_MAIN_BG, 0, 128 * sizeof(u16));
}

BattleAnimFunc BattleAnimSystem_GetFunc(u32 id) {
    if (id >= 0x54) {
        return NULL;
    }
    return sBattleAnimFuncTable[id];
}

BattleAnimSpriteFunc BattleAnimSystem_GetSpriteFunc(u32 id) {
    if (id >= 0x21) {
        return NULL;
    }
    return sBattleAnimSpriteFuncTable[id];
}

void BattleAnimFunc_None(BattleAnimSystem *system) {
}
