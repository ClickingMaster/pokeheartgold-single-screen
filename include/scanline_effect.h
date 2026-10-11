#ifndef POKEHEARTGOLD_SCANLINE_EFFECT_H
#define POKEHEARTGOLD_SCANLINE_EFFECT_H

#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

typedef struct DoubleBuffer {
    void *buffers[2];
    u8 current; // the buffer being written; the other one is shown
    u8 enabled; // DoubleBuffer_Swap does nothing while 0
    u8 pad[2];
} DoubleBuffer;

// One u32 per scanline (low half: x offset, high half: y offset), written to the destination register by HBlank DMA.
typedef struct ScanlineEffect {
    u32 buffers[2][192];
    DoubleBuffer *doubleBuffer;
    SysTask *task;
    u32 unk608;
    s16 wave[192];
    u8 first;
    u8 last;
    u16 pad78e;
    void *destination;
    u32 fill;
    s16 phase; // wave position * 100
    s16 speed;
} ScanlineEffect;

DoubleBuffer *DoubleBuffer_New(enum HeapID heap, void *first, void *second);
void DoubleBuffer_Delete(DoubleBuffer *doubleBuffer);
void *DoubleBuffer_GetCurrent(DoubleBuffer *doubleBuffer);
void *DoubleBuffer_GetOther(DoubleBuffer *doubleBuffer);
void DoubleBuffer_SetEnabled(DoubleBuffer *doubleBuffer, u8 enabled);
void DoubleBuffer_Swap(DoubleBuffer *doubleBuffer);
void HBlankDma_Stop(void);
void HBlankDma_Start(void *src, void *dst, u32 size, int wide);
ScanlineEffect *ScanlineEffect_New(enum HeapID heap);
void ScanlineEffect_StartWave(ScanlineEffect *effect, u8 first, u8 last, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority);
void ScanlineEffect_Stop(ScanlineEffect *effect);
void ScanlineEffect_Delete(ScanlineEffect *effect);
void *ScanlineEffect_GetBuffer(ScanlineEffect *effect);
void ScanlineEffect_SwapBuffers(ScanlineEffect *effect);
void ScanlineEffect_RestartDma(ScanlineEffect *effect);
void ScanlineEffect_BuildWaveTable(s16 *out, u32 count, u32 step, fx32 amplitude);
void ScanlineEffect_WaveTask(SysTask *task, void *data);
void ScanlineEffect_StopDma(void);
void ScanlineEffect_StartDma(ScanlineEffect *effect);

#endif // POKEHEARTGOLD_SCANLINE_EFFECT_H
