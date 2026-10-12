#ifndef POKEHEARTGOLD_BRIGHTNESS_TRANSITION_INTERNAL_H
#define POKEHEARTGOLD_BRIGHTNESS_TRANSITION_INTERNAL_H
#include "global.h"

typedef struct BrightnessTransition {
    s32 steps;
    s32 delay;
    s32 counter;
    s32 current;
    s32 target;
    s32 delta;
    s32 screen;
} BrightnessTransition;

typedef struct BrightnessController {
    u8 pad_00[4];
    s32 duration;
    s32 target;
    s32 state;
    s32 screen;
    BrightnessTransition *transition;
    u8 pad_18[8];
    u32 heapId;
    u16 setting;
} BrightnessController;

// Per-scanline horizontal bounds of one window (0x604 bytes).
typedef struct ScanlineWindowBuffer {
    s16 left[192];
    s16 right[192];
    u8 pad300[0x300];
    u32 window; // 0x600: 0 or 1
} ScanlineWindowBuffer;

typedef struct ScanlineBufferController {
    void *buffer;
    s32 count;
    s32 screen;
} ScanlineBufferController;

void *Heap_Alloc(u32, u32);
void Heap_Free(void *);
void GF_AssertFail(void);
void SetMasterBrightness(int, int);
s32 sub_02010A6C(s32, s32, s32);

void sub_02010AB0(s32 *, s32 *, s32 *, const u8 *, const u8 *, s32);
void BrightnessTransition_Start(BrightnessController *, BOOL);
BOOL BrightnessTransition_Run(BrightnessController *);
BOOL BrightnessTransition_Step(BrightnessTransition *);
void ScanlineWindow_HBlankCallback(ScanlineBufferController *);
ScanlineWindowBuffer *sub_02010EE0(ScanlineBufferController *, int);
void ScanlineBufferController_Init(ScanlineBufferController *, int, int, u32);
#endif
