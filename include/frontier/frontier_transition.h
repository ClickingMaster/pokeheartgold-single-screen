#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_TRANSITION_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_TRANSITION_H

#include "bg_window.h"
#include "heap.h"
#include "scanline_effect.h"
#include "sys_task.h"

typedef struct FrontierWipeBox FrontierWipeBox;
typedef struct FrontierWipe FrontierWipe;

typedef struct FrontierScanline {
    ScanlineEffect *effect;
    SysTask *task;
    u32 counter;
} FrontierScanline;

// BG2 and BG3 affine registers of one scanline.
typedef struct FrontierScanlineRegs {
    u32 data[8];
} FrontierScanlineRegs;

typedef void (*FrontierHBlankDmaCallback)(void *arg);

typedef struct FrontierHBlankDma {
    SysTask *vblankTask;
    SysTask *vwaitTask;
    BOOL active;
    BOOL pending;
    FrontierHBlankDmaCallback onVWait;
    FrontierHBlankDmaCallback onVBlank;
    void *arg;
    DoubleBuffer *buffers;
    FrontierScanlineRegs lines[2][192];
    void *destination;
} FrontierHBlankDma;

// screen 1 (main) or 2 (sub) flashes to brightness and back count times while the other screen fades to
// otherBrightness; *done is set to TRUE at the end.
void FrontierTransition_StartFlash(u32 screen, s32 brightness, s32 otherBrightness, u32 *done, u32 count);
FrontierWipe *FrontierWipe_New(enum HeapID heapID);
void FrontierWipe_Free(FrontierWipe *wipe);
void FrontierWipe_Start(FrontierWipe *wipe, u8 duration, u8 delay, Window *window, u8 color);
BOOL FrontierWipe_UpdateHorizontal(FrontierWipe *wipe);
BOOL FrontierWipe_UpdateVertical(FrontierWipe *wipe);
void FrontierScanline_Init(FrontierScanline *scanline, enum HeapID heapID);
void FrontierScanline_StartWave(FrontierScanline *scanline, u8 first, u8 last, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority);
FrontierHBlankDma *FrontierHBlankDma_New(void *destination, const FrontierScanlineRegs *initial, enum HeapID heapID);
void FrontierHBlankDma_Delete(FrontierHBlankDma *dma);
void *FrontierHBlankDma_GetBuffer(FrontierHBlankDma *dma);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_TRANSITION_H
