#ifndef POKEHEARTGOLD_SCREEN_FADE_EFFECTS_H
#define POKEHEARTGOLD_SCREEN_FADE_EFFECTS_H

#include "global.h"

#include "heap.h"
#include "sys_task_api.h"

// The effects of the palette fade (screen_fade.h): one FadeFunc per enum FadeType, called every frame for each
// screen until it returns TRUE. FadeFunc_00/01 fade the master brightness; the others fill the backdrop with the
// fade color and move a hardware window over the screen (screen_wipe_effects.h, screen_window_wipe.h), covering
// the screen (fade out) or uncovering it (fade in). The shaped wipes rebuild their window's span for every
// scanline into a WipeScanlineBuffer, or the window's on/off flag per scanline into a LineMask; an HBlank
// callback feeds those to the window registers line by line.

typedef struct WindowRegisterQueue WindowRegisterQueue;

// One screen's fade
typedef struct ScreenFade {
    u32 type; // enum FadeType
    int steps;
    int framesPerStep;
    int state;
    int screen;
    void *effect;
    WindowRegisterQueue *regQueue;
    void *blankPair;
    enum HeapID heapId;
    u16 color;
    BOOL fadeOut; // the screen ends covered by the fade color
    BOOL isBrightnessFade;
} ScreenFade; // size: 0x30

// One window's horizontal span per scanline. The wipes write the next frame's spans to nextLeft/nextRight,
// which a VBlank task copies to left/right for the HBlank callback.
typedef struct WipeScanlineBuffer {
    s16 left[192];
    s16 right[192];
    s16 nextLeft[192];
    s16 nextRight[192];
    u32 window;       // 0 = WIN0, 1 = WIN1
} WipeScanlineBuffer; // size: 0x604

typedef struct ScreenWipeScanlines {
    WipeScanlineBuffer *buffer; // count buffers
    int count;
    int screen;
} ScreenWipeScanlines;

// One window's on/off flag per scanline, copied to latched in VBlank and applied per HBlank
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

BOOL FadeFunc_00(ScreenFade *fade);
BOOL FadeFunc_01(ScreenFade *fade);
BOOL FadeFunc_02(ScreenFade *fade);
BOOL FadeFunc_03(ScreenFade *fade);
BOOL FadeFunc_04(ScreenFade *fade);
BOOL FadeFunc_05(ScreenFade *fade);
BOOL FadeFunc_06(ScreenFade *fade);
BOOL FadeFunc_07(ScreenFade *fade);
BOOL FadeFunc_08(ScreenFade *fade);
BOOL FadeFunc_09(ScreenFade *fade);
BOOL FadeFunc_10(ScreenFade *fade);
BOOL FadeFunc_11(ScreenFade *fade);
BOOL FadeFunc_12(ScreenFade *fade);
BOOL FadeFunc_13(ScreenFade *fade);
BOOL FadeFunc_14(ScreenFade *fade);
BOOL FadeFunc_15(ScreenFade *fade);
BOOL FadeFunc_16(ScreenFade *fade);
BOOL FadeFunc_17(ScreenFade *fade);
BOOL FadeFunc_18(ScreenFade *fade);
BOOL FadeFunc_19(ScreenFade *fade);
BOOL FadeFunc_20(ScreenFade *fade);
BOOL FadeFunc_21(ScreenFade *fade);
BOOL FadeFunc_22(ScreenFade *fade);
BOOL FadeFunc_23(ScreenFade *fade);
BOOL FadeFunc_24(ScreenFade *fade);
BOOL FadeFunc_25(ScreenFade *fade);
BOOL FadeFunc_26(ScreenFade *fade);
BOOL FadeFunc_27(ScreenFade *fade);
BOOL FadeFunc_28(ScreenFade *fade);
BOOL FadeFunc_29(ScreenFade *fade);
BOOL FadeFunc_30(ScreenFade *fade);
BOOL FadeFunc_31(ScreenFade *fade);
BOOL FadeFunc_32(ScreenFade *fade);
BOOL FadeFunc_33(ScreenFade *fade);
BOOL FadeFunc_34(ScreenFade *fade);
BOOL FadeFunc_35(ScreenFade *fade);
BOOL FadeFunc_36(ScreenFade *fade);
BOOL FadeFunc_37(ScreenFade *fade);
BOOL FadeFunc_38(ScreenFade *fade);
BOOL FadeFunc_39(ScreenFade *fade);
BOOL FadeFunc_40(ScreenFade *fade);
BOOL FadeFunc_41(ScreenFade *fade);
BOOL FadeFunc_42(ScreenFade *fade);

fx32 ScreenFade_Tan(int angle);
int ScreenFade_MulTan(int angle, int value);
void ScreenFade_FillMulTan(int angle, s32 *dest, int end, int start);
fx32 ScreenFade_HalfDivTan(int angle, int value);
int ScreenFade_StepDelta(int start, int end, int steps);
int ScreenFade_AddClamped(int value, int increment);
void ScreenFade_AddRect(s32 *values, const s32 *deltas);
void ScreenFade_InitRectLerp(s32 *values, s32 *ends, s32 *deltas, const u8 *start, const u8 *end, int steps);

void BrightnessTransition_Start(ScreenFade *fade, BOOL fadeOut);
BOOL BrightnessTransition_Run(ScreenFade *fade);

void ScreenWipeScanlines_HBlankCallback(void *data);
void ScreenWipeScanlines_Init(ScreenWipeScanlines *scanlines, int window, int screen, enum HeapID heapId);
void ScreenWipeScanlines_Free(ScreenWipeScanlines *scanlines);
void ScreenWipeScanlines_FreeBuffer(ScreenWipeScanlines *scanlines);
WipeScanlineBuffer *ScreenWipeScanlines_GetBuffer(ScreenWipeScanlines *scanlines, int index);
void ScreenWipeScanlines_CommitTask(SysTask *task, void *data);

void ScreenFade_SetEndWindows(BOOL cover, WindowRegisterQueue *regQueue, int screen);
void ScreenFade_SetWindow(WindowRegisterQueue *regQueue, int insideMask, int outsideMask, int window, int screen, int left, int top, int right, int bottom, BOOL queued);
void ScreenFade_SwapWindowMasks(WindowRegisterQueue *regQueue, int window, int screen, BOOL queued);
void ScreenFade_SetWindowVisible(WindowRegisterQueue *regQueue, u32 mask, int screen, BOOL queued);

void LineMask_Init(LineMask *mask, int screen, int count, int window0, int window1);
SysTask *LineMask_StartCommitTask(LineMask *mask);
void LineMask_StartHBlank(void *blankPair, LineMask *mask, enum HeapID heapId);
void LineMask_StopHBlank(void *blankPair, LineMask *mask, enum HeapID heapId);
void LineMask_CommitTask(SysTask *task, void *data);
void LineMask_HBlankCallback(void *data);

// Palette fade state
void SetBothBackdropColors(u16 color);
void SetMasterBrightness(int screen, int brightness);
void FadeBlankPair_StartOnVBlank(void *blankPair, void *data, void (*callback)(void *data), int screen, enum HeapID heapId);
void FadeBlankPair_StopOnVBlank(void *blankPair, int screen, enum HeapID heapId);

#endif // POKEHEARTGOLD_SCREEN_FADE_EFFECTS_H
