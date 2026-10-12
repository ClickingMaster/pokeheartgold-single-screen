#ifndef POKEHEARTGOLD_SCREEN_WINDOW_WIPE_H
#define POKEHEARTGOLD_SCREEN_WINDOW_WIPE_H

#include "global.h"

#include "heap.h"
#include "sys_task_api.h"

// Clock wipe: a screen fade that sweeps a hardware window around a centre point, one angle step per few frames.
// The window's horizontal span is rebuilt for every scanline into the back half of a scanline buffer that the
// palette fade module copies in during VBlank and feeds to the window registers per HBlank.

typedef struct WindowRegisterQueue WindowRegisterQueue;

typedef struct ClockWipeConfig {
    s16 centerX;
    s16 centerY;
    u32 startAngle;
    u8 window; // 0 = WIN0, 1 = WIN1
    u8 insideMask;
    u8 outsideMask;
    u8 queued; // 0: write the window registers at once, otherwise queue them for VBlank
} ClockWipeConfig;

// Fields of the palette fade module's per-screen state used here
typedef struct ClockWipeFade {
    u32 unk00;
    int steps;
    int framesPerStep;
    int state;
    int screen;
    struct ClockWipe *wipe;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
} ClockWipeFade; // size: 0x24

typedef struct ScreenWipeScanlines {
    void *buffer;
    int count;
    int screen;
} ScreenWipeScanlines; // the palette fade module's scanline buffer

typedef struct ClockWipe {
    ScreenWipeScanlines scanlines;
    int centerX;
    int centerY;
    int unused14;
    int startAngle;
    int angle;
    int angleStep;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    enum HeapID heapId;
    int queued;
    int window;
    int screen;
    int maskSwapPending; // the window's inside/outside masks are swapped once the sweep passes half a turn
    WindowRegisterQueue *regQueue;
    void *blankPair;
} ClockWipe; // size: 0x4C

// Window register writes queued for VBlank, one slot per screen (and window)
typedef struct WindowVisibleArgs {
    u32 mask;
    int screen;
} WindowVisibleArgs;

typedef struct WindowInsideArgs {
    u32 planeMask;
    BOOL effect;
    int window;
    int screen;
} WindowInsideArgs;

typedef struct WindowOutsideArgs {
    u32 planeMask;
    BOOL effect;
    int screen;
} WindowOutsideArgs;

typedef struct WindowRectArgs {
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    int window;
    int screen;
} WindowRectArgs;

struct WindowRegisterQueue {
    WindowVisibleArgs visible[2];
    WindowInsideArgs inside[2][2]; // [screen][window]
    u8 unk50[0x18];
    WindowOutsideArgs outside[2];
    WindowRectArgs rect[2][2]; // [screen][window]
}; // size: 0xC0

void ClockWipe_Begin(ClockWipeFade *fade, const ClockWipeConfig *config);
BOOL ClockWipe_Update(ClockWipeFade *fade);
void ClockWipe_Init(ClockWipe *wipe, const ClockWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL ClockWipe_Step(ClockWipe *wipe);
void ClockWipe_GetScanlineSpan(int unused, int centerX, int centerY, int y, int startAngle, int endAngle, int *left, int *right);
void ClockWipe_BuildScanlines(ClockWipe *wipe);

void ScreenWindow_SetVisible(u32 mask, int screen);
void ScreenWindow_SetInside(u32 planeMask, BOOL effect, int window, int screen);
void ScreenWindow_SetOutside(u32 planeMask, BOOL effect, int screen);
s8 ScreenWindow_GetInside(int window, int screen);
s8 ScreenWindow_GetOutside(int screen);
void ScreenWindow_SetRect(int left, int top, int right, int bottom, int window, int screen);
void ScreenWindow_QueueSetVisible(WindowRegisterQueue *queue, u32 mask, int screen);
void ScreenWindow_QueueSetInside(WindowRegisterQueue *queue, u32 planeMask, BOOL effect, int window, int screen);
void ScreenWindow_QueueSetOutside(WindowRegisterQueue *queue, u32 planeMask, BOOL effect, int screen);
void ScreenWindow_QueueSetRect(WindowRegisterQueue *queue, int left, int top, int right, int bottom, int window, int screen);
void ScreenWindow_SetVisibleTask(SysTask *task, void *data);
void ScreenWindow_SetInsideTask(SysTask *task, void *data);
void ScreenWindow_SetOutsideTask(SysTask *task, void *data);
void ScreenWindow_SetRectTask(SysTask *task, void *data);

#endif // POKEHEARTGOLD_SCREEN_WINDOW_WIPE_H
