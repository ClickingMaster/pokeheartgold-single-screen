#ifndef POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H
#define POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H

#include "global.h"

#include "heap.h"
#include "screen_fade_effects.h"
#include "screen_window_wipe.h"
#include "sys_task_api.h"

// Window wipes of the screen fade (screen_fade_effects.h). Each effect moves one or both hardware windows over a
// number of steps, framesPerStep frames each. The shaped effects rebuild the window's span for every scanline in
// a WipeScanlineBuffer, the line shutters the window's on/off flag per scanline in a LineMask.

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

void RectWipe_Begin(ScreenFade *fade, const RectWipeConfig *config);
BOOL RectWipe_Update(ScreenFade *fade);
void DoubleRectWipe_Begin(ScreenFade *fade, const RectWipeConfig *config0, const RectWipeConfig *config1);
BOOL DoubleRectWipe_Update(ScreenFade *fade);
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

void CircleWipe_Begin(ScreenFade *fade, const CircleWipeConfig *config);
BOOL CircleWipe_Update(ScreenFade *fade);
void CircleWipe_Init(CircleWipe *wipe, const CircleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL CircleWipe_Step(CircleWipe *wipe);
void CircleWipe_GetScanlineSpan(int radius, int centerX, int centerY, int y, int *left, int *right);
void CircleWipe_BuildScanlines(CircleWipe *wipe);
void WedgeWipe_Begin(ScreenFade *fade, const AngleWipeConfig *config);
BOOL WedgeWipe_Update(ScreenFade *fade);
void WedgeWipe_Init(WedgeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL WedgeWipe_Step(WedgeWipe *wipe);
void WedgeWipe_BuildScanlines(WedgeWipe *wipe);
void SlopeWipe_Begin(ScreenFade *fade, const AngleWipeConfig *config);
BOOL SlopeWipe_Update(ScreenFade *fade);
void SlopeWipe_Init(SlopeWipe *wipe, const AngleWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL SlopeWipe_Step(SlopeWipe *wipe);
void SlopeWipe_BuildScanlines(SlopeWipe *wipe);

void MultiRectWipe_Begin(ScreenFade *fade, const MultiRectWipeConfig *config);
BOOL MultiRectWipe_Update(ScreenFade *fade);
void MultiRectWipe_Init(MultiRectWipe *wipe, const MultiRectWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL MultiRectWipe_Step(MultiRectWipe *wipe);
void MultiRectWipe_FreeRects(MultiRectWipe *wipe);
void MultiRectWipe_BuildScanlines(MultiRectWipe *wipe);
void MultiRectWipe_DrawRect(MultiRectWipe *wipe, RectLerp *rect);
void MultiRectWipe_Advance(MultiRectWipe *wipe);
void SectorWipe_Begin(ScreenFade *fade, const SectorWipeConfig *config);
BOOL SectorWipe_Update(ScreenFade *fade);
void SectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL SectorWipe_Step(SectorWipe *wipe);
void SectorWipe_Finish(SectorWipe *wipe);
void SectorWipe_BuildScanlines(SectorWipe *wipe);
void SectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator);
void MirroredSectorWipe_Begin(ScreenFade *fade, const SectorWipeConfig *config);
BOOL MirroredSectorWipe_Update(ScreenFade *fade);
void MirroredSectorWipe_Init(SectorWipe *wipe, const SectorWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL MirroredSectorWipe_Step(SectorWipe *wipe);
void MirroredSectorWipe_Finish(SectorWipe *wipe);
void MirroredSectorWipe_BuildScanlines(SectorWipe *wipe);
void MirroredSectorWipe_Interpolate(WipeInterpolation *interp, int numerator, int denominator);
void LineShutterWipe_Begin(ScreenFade *fade, const LineShutterConfig *config);
BOOL LineShutterWipe_Update(ScreenFade *fade);
void LineShutterWipe_Init(LineShutterWipe *wipe, const LineShutterConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL LineShutterWipe_Step(LineShutterWipe *wipe);
void LineShutterWipe_Finish(LineShutterWipe *wipe);
void LineShutterWipe_BuildLines(LineShutterWipe *wipe);
void LineShutterWipe_FillRange(const LineShutterRange *range, u8 *lines, int step, int steps);
void RectShutterWipe_Begin(ScreenFade *fade, const RectShutterWipeConfig *config);
BOOL RectShutterWipe_Update(ScreenFade *fade);
void RectShutterWipe_InitRectFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL RectShutterWipe_StepRectFirst(RectShutterWipe *wipe, ScreenFade *fade);
void RectShutterWipe_InitShutterFirst(RectShutterWipe *wipe, const RectShutterWipeConfig *config, int steps, int framesPerStep, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId);
BOOL RectShutterWipe_StepShutterFirst(RectShutterWipe *wipe, ScreenFade *fade);

#endif // POKEHEARTGOLD_SCREEN_WIPE_EFFECTS_H
