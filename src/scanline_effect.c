#include "scanline_effect.h"

// not declared in lib/include/nitro/mi/dma.h
void MI_HBlankDmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size);
void MI_HBlankDmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size);

DoubleBuffer *DoubleBuffer_New(enum HeapID heap, void *first, void *second) {
    DoubleBuffer *doubleBuffer = Heap_Alloc(heap, sizeof(DoubleBuffer));
    memset(doubleBuffer, 0, sizeof(DoubleBuffer));
    doubleBuffer->enabled = 1;
    doubleBuffer->buffers[0] = first;
    doubleBuffer->buffers[1] = second;
    return doubleBuffer;
}

void DoubleBuffer_Delete(DoubleBuffer *doubleBuffer) {
    GF_ASSERT(doubleBuffer);
    Heap_Free(doubleBuffer);
}

void *DoubleBuffer_GetCurrent(DoubleBuffer *doubleBuffer) {
    GF_ASSERT(doubleBuffer);
    return doubleBuffer->buffers[doubleBuffer->current];
}

void *DoubleBuffer_GetOther(DoubleBuffer *doubleBuffer) {
    GF_ASSERT(doubleBuffer);
    return doubleBuffer->buffers[1 ^ doubleBuffer->current];
}

void DoubleBuffer_SetEnabled(DoubleBuffer *doubleBuffer, u8 enabled) {
    GF_ASSERT(doubleBuffer);
    doubleBuffer->enabled = enabled;
}

void DoubleBuffer_Swap(DoubleBuffer *doubleBuffer) {
    if (doubleBuffer && doubleBuffer->enabled) {
        doubleBuffer->current ^= 1;
    }
}

void HBlankDma_Stop(void) {
    MI_StopDma(0);
    MI_WaitDma(0);
}

void HBlankDma_Start(void *src, void *dst, u32 size, int wide) {
    if (wide == 1) {
        MI_HBlankDmaCopy32(0, src, dst, size);
    } else {
        MI_HBlankDmaCopy16(0, src, dst, size);
    }
}

ScanlineEffect *ScanlineEffect_New(enum HeapID heap) {
    ScanlineEffect *effect = Heap_Alloc(heap, sizeof(ScanlineEffect));
    memset(effect, 0, sizeof(ScanlineEffect));
    effect->doubleBuffer = DoubleBuffer_New(heap, effect->buffers[0], effect->buffers[1]);
    return effect;
}

void ScanlineEffect_StartWave(ScanlineEffect *effect, u8 first, u8 last, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority) {
    GF_ASSERT(effect);
    GF_ASSERT(effect->task == NULL);
    effect->first = first;
    effect->last = last;
    effect->phase = 0;
    effect->speed = speed;
    effect->destination = destination;
    effect->fill = fill;
    ScanlineEffect_BuildWaveTable(effect->wave, 192, step, amplitude);
    effect->task = SysTask_CreateOnMainQueue(ScanlineEffect_WaveTask, effect, priority);
    MIi_CpuClear32(effect->fill, effect->buffers[0], sizeof(effect->buffers[0]));
    MIi_CpuClear32(effect->fill, effect->buffers[1], sizeof(effect->buffers[1]));
}

void ScanlineEffect_Stop(ScanlineEffect *effect) {
    GF_ASSERT(effect);
    if (effect->task) {
        SysTask_Destroy(effect->task);
        effect->task = NULL;
        memset(DoubleBuffer_GetCurrent(effect->doubleBuffer), effect->fill, sizeof(effect->buffers[0]));
    }
}

void ScanlineEffect_Delete(ScanlineEffect *effect) {
    ScanlineEffect_Stop(effect);
    ScanlineEffect_StopDma();
    DoubleBuffer_Delete(effect->doubleBuffer);
    Heap_Free(effect);
}

void *ScanlineEffect_GetBuffer(ScanlineEffect *effect) {
    return DoubleBuffer_GetCurrent(effect->doubleBuffer);
}

void ScanlineEffect_SwapBuffers(ScanlineEffect *effect) {
    if (effect && effect->task) {
        void *buffer;
        DoubleBuffer_Swap(effect->doubleBuffer);
        buffer = DoubleBuffer_GetCurrent(effect->doubleBuffer);
        MIi_CpuClear32(effect->fill, buffer, sizeof(effect->buffers[0]));
    }
}

void ScanlineEffect_RestartDma(ScanlineEffect *effect) {
    if (effect && effect->task) {
        ScanlineEffect_StopDma();
        ScanlineEffect_StartDma(effect);
    }
}

void ScanlineEffect_BuildWaveTable(s16 *out, u32 count, u32 step, fx32 amplitude) {
    u16 angle = 0;
    u32 i;
    for (i = 0; i < count; i++) {
        *out = FX_Mul(FX_SinIdx(angle), amplitude) >> 12;
        angle += step;
        out++;
    }
}

void ScanlineEffect_WaveTask(SysTask *task, void *data) {
    ScanlineEffect *effect = data;
    u32 *buffer = DoubleBuffer_GetCurrent(effect->doubleBuffer);
    u8 pos = effect->phase / 100;
    int i;
    for (i = effect->first; i <= effect->last; i++) {
        s16 x = (s16)buffer[i];
        u16 y = (u16)(buffer[i] >> 16);
        x += effect->wave[pos];
        buffer[i] = (y << 16) | (u16)x;
        pos = (pos + 1) % 192;
    }
    effect->phase += effect->speed;
    if (effect->phase >= 19200) {
        effect->phase %= 19200;
    } else if (effect->phase < 0) {
        effect->phase += 19200;
    }
}

void ScanlineEffect_StopDma(void) {
    HBlankDma_Stop();
}

void ScanlineEffect_StartDma(ScanlineEffect *effect) {
    void *buffer;
    GF_ASSERT(effect);
    buffer = DoubleBuffer_GetOther(effect->doubleBuffer);
    DC_FlushRange(buffer, sizeof(effect->buffers[0]));
    HBlankDma_Start(buffer, effect->destination, 4, 1);
}
