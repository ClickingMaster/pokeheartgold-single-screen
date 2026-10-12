#ifndef POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H
#define POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H

#include "global.h"

#include "heap.h"
#include "screen_window_wipe.h"
#include "sys_task_api.h"

// Window wipes of the screen fade. Each effect moves one or both hardware windows over a number of steps,
// framesPerStep frames each. The shaped effects rebuild the window's horizontal span for every scanline into
// the back half of the palette fade module's scanline buffer, which is copied to the front half in VBlank and
// fed to the window registers per HBlank.

// One window's scanline buffer
typedef struct WipeScanlineBuffer {
    s16 left[192];
    s16 right[192];
    s16 nextLeft[192];
    s16 nextRight[192];
    u32 window;
} WipeScanlineBuffer; // size: 0x604

// Fields of the palette fade module's per-screen state used by the wipes
typedef struct ScreenWipeFade {
    u32 unk00;
    int steps;
    int framesPerStep;
    int state;
    int screen;
    void *effect;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
    u16 color;
    int unk28;
} ScreenWipeFade;

typedef struct RectWipeConfig {
    u8 start[4]; // left, top, right, bottom
    u8 end[4];
    u8 window; // 0 = WIN0, 1 = WIN1
    u8 insideMask;
    u8 outsideMask;
    u8 queued; // 0: write the window registers at once, otherwise queue them for VBlank
} RectWipeConfig;

// A window rectangle moved linearly from one rectangle to another
typedef struct RectWipe {
    s32 rect[4]; // left, top, right, bottom, in 1/128 pixels
    s32 delta[4];
    s32 end[4]; // in pixels
    int screen;
    int window;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    int queued;
    WindowRegisterQueue *regQueue;
} RectWipe; // size: 0x4C

void RectWipe_Begin(ScreenWipeFade *fade, const RectWipeConfig *config);
BOOL RectWipe_Update(ScreenWipeFade *fade);
void DoubleRectWipe_Begin(ScreenWipeFade *fade, const RectWipeConfig *config0, const RectWipeConfig *config1);
BOOL DoubleRectWipe_Update(ScreenWipeFade *fade);
void RectWipe_Init(RectWipe *wipe, const RectWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue);
BOOL RectWipe_Step(RectWipe *wipe);

typedef struct CircleWipeConfig {
    s16 startRadius;
    s16 endRadius;
    s16 centerX;
    s16 centerY;
    u8 window;
    u8 insideMask;
    u8 outsideMask;
    u8 queued;
} CircleWipeConfig;

// A circle around a fixed centre whose radius grows or shrinks
typedef struct CircleWipe {
    ScreenWipeScanlines scanlines;
    int radius; // in 1/128 pixels
    int centerX;
    int centerY;
    int radiusStep;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    enum HeapID heapId;
    int queued;
    WindowRegisterQueue *regQueue;
    void *blankPair;
} CircleWipe; // size: 0x38

typedef struct AngleWipeConfig {
    u16 startAngle;
    u16 endAngle;
    u8 window;
    u8 insideMask;
    u8 outsideMask;
    u8 queued;
} AngleWipeConfig;

// A wedge opening down from the top centre of the screen: half its width on scanline y is tan(angle) * y
typedef struct WedgeWipe {
    ScreenWipeScanlines scanlines;
    int angle; // in 1/128 units
    int angleStep;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    int queued;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
} WedgeWipe; // size: 0x30

// Half span sin(angle) * amplitude in the middle of the screen, widening along a slope towards the top and
// bottom edges (the two halves are mirrored)
typedef struct SlopeWipe {
    ScreenWipeScanlines scanlines;
    fx32 amplitude;
    int angle;
    int angleStep;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    int queued;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
} SlopeWipe; // size: 0x34

typedef struct MultiRectWipeConfig {
    const u8 (*starts)[4]; // left, top, right, bottom
    const u8 (*ends)[4];
    u16 count;
    u16 window;
    u8 insideMask;
    u8 outsideMask;
    u16 queued;
} MultiRectWipeConfig;

typedef struct RectLerp {
    s32 rect[4]; // left, top, right, bottom, in 1/128 pixels
    s32 delta[4];
    s32 end[4];
} RectLerp; // size: 0x30

// Several rectangles moved linearly, drawn into one window's scanlines
typedef struct MultiRectWipe {
    ScreenWipeScanlines scanlines;
    RectLerp *rects;
    int count;
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    int queued;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
} MultiRectWipe; // size: 0x30

typedef struct SectorWipeConfig {
    u16 startPhase;
    u16 endPhase;
    u8 insideMask;
    u8 outsideMask;
    u16 queued;
} SectorWipeConfig;

typedef struct WipeInterpolation {
    int value;
    int start;
    int delta;
} WipeInterpolation;

// Both windows bounded by lines through the screen centre whose angle follows the phase
typedef struct SectorWipe {
    ScreenWipeScanlines scanlines;
    WipeInterpolation phase;
    int steps;
    int step;
    int framesPerStep;
    int frameCounter;
    int queued;
    enum HeapID heapId;
    WindowRegisterQueue *regQueue;
    void *blankPair;
} SectorWipe; // size: 0x38

// Per-scanline window on/off flags, swapped into the window registers by an HBlank callback
typedef struct LineMaskWindow {
    u8 lines[192];
    u8 latched[192];
    int window;
} LineMaskWindow;

typedef struct LineMask {
    LineMaskWindow windows[2];
    u8 count;
    u8 screen;
} LineMask; // size: 0x30C

// Scanlines start..end flip from `initial` to its opposite, the boundary moving from start to end
typedef struct LineShutterRange {
    u8 start;
    u8 end;
    u16 initial;
} LineShutterRange;

typedef struct LineShutterConfig {
    const LineShutterRange *ranges;
    u16 rangeCount;
    u16 queued;
} LineShutterConfig;

typedef struct LineShutterWipe {
    LineMask mask;
    const LineShutterRange *ranges;
    int rangeCount;
    int steps;
    int step;
    int framesPerStep;
    int frameCounter;
    int queued;
    enum HeapID heapId;
    WindowRegisterQueue *regQueue;
    void *blankPair;
} LineShutterWipe; // size: 0x334

typedef struct RectShutterWipeConfig {
    RectWipeConfig rect; // rect.queued also selects the order: 0 = rectangle first
    LineShutterConfig shutter;
    fx32 rectShare; // part of the steps given to the rectangle
} RectShutterWipeConfig;

// A rectangle wipe and a line shutter, one after the other
typedef struct RectShutterWipe {
    RectWipe rect;
    LineShutterWipe shutter;
    const RectShutterWipeConfig *config;
    u8 laterSteps;
    u8 phase;
    u8 shutterFirst;
} RectShutterWipe; // size: 0x388

void CircleWipe_Begin(ScreenWipeFade *fade, const CircleWipeConfig *config);
BOOL CircleWipe_Update(ScreenWipeFade *fade);
void CircleWipe_Init(CircleWipe *wipe, const CircleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL CircleWipe_Step(CircleWipe *wipe);
void CircleWipe_GetScanlineSpan(int radius, int centerX, int centerY, int y, int *left, int *right);
void CircleWipe_BuildScanlines(CircleWipe *wipe);
void WedgeWipe_Begin(ScreenWipeFade *fade, const AngleWipeConfig *config);
BOOL WedgeWipe_Update(ScreenWipeFade *fade);
void WedgeWipe_Init(WedgeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL WedgeWipe_Step(WedgeWipe *wipe);
void WedgeWipe_BuildScanlines(WedgeWipe *wipe);
void SlopeWipe_Begin(ScreenWipeFade *fade, const AngleWipeConfig *config);
BOOL SlopeWipe_Update(ScreenWipeFade *fade);
void SlopeWipe_Init(SlopeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL SlopeWipe_Step(SlopeWipe *wipe);
void SlopeWipe_BuildScanlines(SlopeWipe *wipe);

void MultiRectWipe_Begin(ScreenWipeFade *fade, const MultiRectWipeConfig *config);
BOOL MultiRectWipe_Update(ScreenWipeFade *fade);
void MultiRectWipe_Init(MultiRectWipe *wipe, const MultiRectWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL MultiRectWipe_Step(MultiRectWipe *wipe);
void MultiRectWipe_FreeRects(MultiRectWipe *wipe);
void MultiRectWipe_BuildScanlines(MultiRectWipe *wipe);
void MultiRectWipe_DrawRect(MultiRectWipe *wipe, RectLerp *rect);
void MultiRectWipe_Advance(MultiRectWipe *wipe);
void SectorWipe_Begin(ScreenWipeFade *fade, const SectorWipeConfig *config);
BOOL SectorWipe_Update(ScreenWipeFade *fade);
void SectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL SectorWipe_Step(SectorWipe *wipe);
void SectorWipe_Finish(SectorWipe *wipe);
void SectorWipe_BuildScanlines(SectorWipe *wipe);
void SectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator);
void MirroredSectorWipe_Begin(ScreenWipeFade *fade, const SectorWipeConfig *config);
BOOL MirroredSectorWipe_Update(ScreenWipeFade *fade);
void MirroredSectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL MirroredSectorWipe_Step(SectorWipe *wipe);
void MirroredSectorWipe_Finish(SectorWipe *wipe);
void MirroredSectorWipe_BuildScanlines(SectorWipe *wipe);
void MirroredSectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator);
void LineShutterWipe_Begin(ScreenWipeFade *fade, const LineShutterConfig *config);
BOOL LineShutterWipe_Update(ScreenWipeFade *fade);
void LineShutterWipe_Init(LineShutterWipe *wipe, const LineShutterConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL LineShutterWipe_Step(LineShutterWipe *wipe);
void LineShutterWipe_Finish(LineShutterWipe *wipe);
void LineShutterWipe_BuildLines(LineShutterWipe *wipe);
void LineShutterWipe_FillRange(const LineShutterRange *range, u8 *lines, int step, int steps);
void RectShutterWipe_Begin(ScreenWipeFade *fade, const RectShutterWipeConfig *config);
BOOL RectShutterWipe_Update(ScreenWipeFade *fade);
void RectShutterWipe_InitRectFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL RectShutterWipe_StepRectFirst(RectShutterWipe *wipe, ScreenWipeFade *fade);
void RectShutterWipe_InitShutterFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL RectShutterWipe_StepShutterFirst(RectShutterWipe *wipe, ScreenWipeFade *fade);

// Palette fade module (not merged yet)
int sub_020109D8(int angle, int value);
void sub_02010A00(int angle, s32 *dest, int end, int start);
int sub_02010A54(int angle, int value);
int sub_02010A6C(int start, int end, int steps);
int sub_02010A7C(int value, int increment);
void sub_02010A8C(s32 *values, const s32 *deltas);
void sub_02010AB0(s32 *starts, s32 *ends, s32 *deltas, const u8 *startBytes, const u8 *endBytes, s32 steps);
void ScanlineWindow_HBlankCallback(void);
void ScanlineBufferController_Init(ScreenWipeScanlines *scanlines, int window, int screen, enum HeapID heapId);
void sub_02010EC8(ScreenWipeScanlines *scanlines);
WipeScanlineBuffer *sub_02010EE0(ScreenWipeScanlines *scanlines, int index);
void sub_02010F00(SysTask *task, void *scanlines);
void sub_02010F34(int queued, WindowRegisterQueue *regQueue, int screen);
void sub_02010F84(WindowRegisterQueue *regQueue, int insideMask, int outsideMask, int window, int screen, int left, int top, int right, int bottom, int queued);
void sub_02011068(WindowRegisterQueue *regQueue, int windowMask, int screen, int queued);
void sub_02011080(void *lineMask, int screen, int kind, int unk3, int unk4);
SysTask *sub_020110C4(void *lineMask);
void sub_020110DC(void *blankPair, u8 *lineMask, enum HeapID heapId);
void sub_020110F4(void *blankPair, u8 *lineMask, enum HeapID heapId);
void sub_0200FF88(void *blankPair, void *effect, void (*callback)(void), int screen, enum HeapID heapId);
void sub_0200FFB4(void *blankPair, int screen, enum HeapID heapId);

#endif // POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H
