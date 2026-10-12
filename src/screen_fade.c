#include "screen_fade.h"

#include "global.h"

#include "constants/gx.h"

#include "heap.h"
#include "screen_fade_effects.h"
#include "screen_window_wipe.h"
#include "sys_task_api.h"
#include "system.h"

// The palette fade: BeginNormalPaletteFade starts a fade of one or both screens (enum FadeMode) and
// HandleFadeUpdateFrame advances it every frame. Each screen runs one ScreenFade whose effect is the FadeFunc of its
// enum FadeType (screen_fade_effects.h). The window wipes share one window register queue and one pair of HBlank
// callbacks, one per screen.

typedef void (*FadeHBlankCallback)(void *data);

// One HBlank callback per screen, called from a single HBlank interrupt handler
typedef struct FadeBlankPair {
    void *data[2];
    FadeHBlankCallback callbacks[2];
    u32 active[2];
} FadeBlankPair;

typedef struct FadeBlankStartArgs {
    FadeBlankPair *blankPair;
    void *data;
    FadeHBlankCallback callback;
    int screen;
} FadeBlankStartArgs;

typedef struct FadeBlankStopArgs {
    FadeBlankPair *blankPair;
    int screen;
} FadeBlankStopArgs;

typedef struct PaletteFade {
    u32 order; // 0: both screens at once, 1: the main screen first, 2: the sub screen first
    u32 mainActive;
    u32 subActive;
    u32 mainEnabled; // the screens taking part in the fade
    u32 subEnabled;
    ScreenFade main;
    ScreenFade sub;
    FadeBlankPair blankPair;
    WindowRegisterQueue regQueue;
    u16 active;
    u8 unk14E;
    u8 unk14F;
    u16 color; // the color of the last fade, used when a fade passes 0xFFFF
} PaletteFade; // size: 0x154

typedef BOOL (*FadeFunc)(ScreenFade *fade);

void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness); // not declared by the SDK headers here

void SetBothScreensBrightnessByColor(u16 color);
void FillScreenWithColor(int screen, u16 color);
BOOL DoFadeUpdateFrame(PaletteFade *fade, ScreenFade *main, ScreenFade *sub);
void FadeWork_UpdateFrame(u32 *active, ScreenFade *fade);
BOOL CallFadeFunc(ScreenFade *fade);
void PaletteFade_SetMode(enum FadeMode mode, PaletteFade *fade);
void PaletteFade_SetScreenOrder(PaletteFade *fade, u32 order, u32 main, u32 sub);
void ScreenFade_Init(ScreenFade *fade, u32 type, int steps, int framesPerStep, int state, void *effect, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId, u16 color);
void FadeBlankPair_Init(FadeBlankPair *blankPair);
void FadeBlankPair_HBlankCallback(FadeBlankPair *blankPair);
void FadeBlankPair_Start(FadeBlankPair *blankPair, void *data, FadeHBlankCallback callback, int screen);
void FadeBlankPair_Stop(FadeBlankPair *blankPair, int screen);
void FadeBlankPair_StartTask(SysTask *task, void *data);
void FadeBlankPair_StopTask(SysTask *task, void *data);
void FadeBlankPair_DummyCallback(void *data);
u16 PaletteFade_ResolveColor(PaletteFade *fade, u16 color);
u16 PaletteFade_GetEndColor(PaletteFade *fade);
void ScreenFade_ResetBrightnessTask(SysTask *task, void *data);
void ScreenFade_QueueBrightnessReset(ScreenFade *fade);
void ScreenFade_HoldFadeColor(ScreenFade *fade);
void PaletteFade_Clear(PaletteFade *fade);
void HandleEndFade(PaletteFade *fade);

const FadeFunc sFadeFuncPtrs[] = {
    FadeFunc_00,
    FadeFunc_01,
    FadeFunc_02,
    FadeFunc_03,
    FadeFunc_04,
    FadeFunc_05,
    FadeFunc_06,
    FadeFunc_07,
    FadeFunc_08,
    FadeFunc_09,
    FadeFunc_10,
    FadeFunc_11,
    FadeFunc_12,
    FadeFunc_13,
    FadeFunc_14,
    FadeFunc_15,
    FadeFunc_16,
    FadeFunc_17,
    FadeFunc_18,
    FadeFunc_19,
    FadeFunc_20,
    FadeFunc_21,
    FadeFunc_22,
    FadeFunc_23,
    FadeFunc_24,
    FadeFunc_25,
    FadeFunc_26,
    FadeFunc_27,
    FadeFunc_28,
    FadeFunc_29,
    FadeFunc_30,
    FadeFunc_31,
    FadeFunc_32,
    FadeFunc_33,
    FadeFunc_34,
    FadeFunc_35,
    FadeFunc_36,
    FadeFunc_37,
    FadeFunc_38,
    FadeFunc_39,
    FadeFunc_40,
    FadeFunc_41,
    FadeFunc_42,
};

PaletteFade gPaletteFade;

#pragma opt_common_subs off

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType mainType, enum FadeType subType, u16 color, int steps, int framesPerStep, enum HeapID heapId) {
    if (steps == 0) {
        GF_AssertFail();
    }
    if (framesPerStep == 0) {
        GF_AssertFail();
    }
    if (gPaletteFade.active != 0) {
        GF_AssertFail();
    }
    {
        PaletteFade *fade = &gPaletteFade;
        PaletteFade_Clear(&gPaletteFade);
        PaletteFade_SetMode(mode, &gPaletteFade);
        FadeBlankPair_Init(&gPaletteFade.blankPair);
        color = PaletteFade_ResolveColor(&gPaletteFade, color);
        ScreenFade_Init(&gPaletteFade.main, mainType, steps, framesPerStep, 0, NULL, 0, &gPaletteFade.regQueue, &gPaletteFade.blankPair, heapId, color);
        ScreenFade_Init(&gPaletteFade.sub, subType, steps, framesPerStep, 0, NULL, 1, &gPaletteFade.regQueue, &gPaletteFade.blankPair, heapId, color);
        gPaletteFade.active = 1;
        FadeWork_UpdateFrame(&gPaletteFade.mainActive, &gPaletteFade.main);
        FadeWork_UpdateFrame(&gPaletteFade.subActive, &gPaletteFade.sub);
        if (gPaletteFade.mainEnabled != 0) {
            ScreenFade_QueueBrightnessReset(&fade->main);
            fade->unk14E = 1;
        }
        if (fade->subEnabled != 0) {
            ScreenFade_QueueBrightnessReset(&fade->sub);
            fade->unk14F = 1;
        }
    }
}

#pragma opt_common_subs reset

void HandleFadeUpdateFrame(void) {
    PaletteFade *fade = &gPaletteFade;
    if (gPaletteFade.active != 0) {
        if (DoFadeUpdateFrame(fade, &fade->main, &fade->sub) == TRUE) {
            HandleEndFade(fade);
        }
    }
}

BOOL IsPaletteFadeFinished(void) {
    return gPaletteFade.active == 0;
}

// Ends the fade at once: both screens skip to their effect's clean-up.
void StopPaletteFade(void) {
    FadeBlankPair_Stop(&gPaletteFade.blankPair, 0);
    FadeBlankPair_Stop(&gPaletteFade.blankPair, 1);
    if (gPaletteFade.mainActive != 0) {
        gPaletteFade.main.state = 2;
    }
    if (gPaletteFade.subActive != 0) {
        gPaletteFade.sub.state = 2;
    }
    FadeWork_UpdateFrame(&gPaletteFade.mainActive, &gPaletteFade.main);
    FadeWork_UpdateFrame(&gPaletteFade.subActive, &gPaletteFade.sub);
    gPaletteFade.active = 0;
    gPaletteFade.unk14E = 0;
    gPaletteFade.unk14F = 0;
    PaletteFade_Clear(&gPaletteFade);
}

void ResetVisibleHardwareWindows(PMLCDTarget screen) {
    ScreenWindow_SetVisible(0, screen);
}

void SetMasterBrightnessNeutral(PMLCDTarget screen) {
    SetMasterBrightness(screen, 0);
}

void SetMasterBrightnessByColor(PMLCDTarget screen, u16 color) {
    if (color == 0xFFFF) {
        color = gPaletteFade.color;
    }
    SetMasterBrightness(screen, color == RGB_WHITE ? 16 : -16);
}

void SetBothScreensBrightnessByColor(u16 color) {
    int brightness;
    if (color == 0xFFFF) {
        color = gPaletteFade.color;
    }
    brightness = color == RGB_WHITE ? 16 : -16;
    SetMasterBrightness(0, brightness);
    SetMasterBrightness(1, brightness);
    gPaletteFade.color = color;
}

// Covers the screen with the backdrop, set to the color, through a window that shows no layer.
void FillScreenWithColor(int screen, u16 color) {
    if (color == 0xFFFF) {
        color = gPaletteFade.color;
    }
    if (screen == 0) {
        GX_LoadBGPltt(&color, 0, sizeof(u16));
    } else {
        GXS_LoadBGPltt(&color, 0, sizeof(u16));
    }
    ScreenWindow_QueueSetVisible(&gPaletteFade.regQueue, 1, screen);
    ScreenWindow_QueueSetInside(&gPaletteFade.regQueue, 0x3F, 0, 0, screen);
    ScreenWindow_QueueSetRect(&gPaletteFade.regQueue, 0, 0, 0, 0, 0, screen);
    ScreenWindow_QueueSetOutside(&gPaletteFade.regQueue, 0x20, 0, screen);
}

void SetBothBackdropColors(u16 color) {
    GX_LoadBGPltt(&color, 0, sizeof(u16));
    GXS_LoadBGPltt(&color, 0, sizeof(u16));
}

void SetMasterBrightness(int screen, int brightness) {
    if (screen == 0) {
        GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, brightness);
    } else {
        GXx_SetMasterBrightness_(&reg_GXS_DB_MASTER_BRIGHT, brightness);
    }
}

void HandleEndFade(PaletteFade *fade) {
    fade->active = 0;
    fade->color = PaletteFade_GetEndColor(fade);
    if (fade->mainEnabled != 0) {
        ScreenFade_HoldFadeColor(&fade->main);
        if (fade->main.fadeOut == FALSE) {
            gPaletteFade.unk14E = 0;
        }
    }
    if (fade->subEnabled != 0) {
        ScreenFade_HoldFadeColor(&fade->sub);
        if (fade->main.fadeOut == FALSE) { // the main screen's, even for the sub screen
            gPaletteFade.unk14F = 0;
        }
    }
    PaletteFade_Clear(fade);
}

BOOL DoFadeUpdateFrame(PaletteFade *fade, ScreenFade *main, ScreenFade *sub) {
    switch (fade->order) {
    case 0:
        FadeWork_UpdateFrame(&fade->mainActive, main);
        FadeWork_UpdateFrame(&fade->subActive, sub);
        break;
    case 1:
        if (fade->mainActive != 0) {
            FadeWork_UpdateFrame(&fade->mainActive, main);
        } else {
            FadeWork_UpdateFrame(&fade->subActive, sub);
        }
        break;
    case 2:
        if (fade->subActive != 0) {
            FadeWork_UpdateFrame(&fade->subActive, sub);
        } else {
            FadeWork_UpdateFrame(&fade->mainActive, main);
        }
        break;
    }
    if (fade->mainActive == 0 && fade->subActive == 0) {
        return TRUE;
    }
    return FALSE;
}

void FadeWork_UpdateFrame(u32 *active, ScreenFade *fade) {
    if (*active != 0 && CallFadeFunc(fade) == TRUE) {
        *active = 0;
    }
}

BOOL CallFadeFunc(ScreenFade *fade) {
    return sFadeFuncPtrs[fade->type](fade);
}

void PaletteFade_SetMode(enum FadeMode mode, PaletteFade *fade) {
    switch (mode) {
    case FADE_BOTH_SCREENS:
        PaletteFade_SetScreenOrder(fade, 0, 1, 1);
        break;
    case FADE_MAIN_THEN_SUB:
        PaletteFade_SetScreenOrder(fade, 1, 1, 1);
        break;
    case FADE_SUB_THEN_MAIN:
        PaletteFade_SetScreenOrder(fade, 2, 1, 1);
        break;
    case FADE_MAIN_ONLY:
        PaletteFade_SetScreenOrder(fade, 1, 1, 0);
        break;
    case FADE_SUB_ONLY:
        PaletteFade_SetScreenOrder(fade, 2, 0, 1);
        break;
    }
}

void PaletteFade_SetScreenOrder(PaletteFade *fade, u32 order, u32 main, u32 sub) {
    fade->order = order;
    fade->mainActive = main;
    fade->subActive = sub;
    fade->mainEnabled = main;
    fade->subEnabled = sub;
}

void ScreenFade_Init(ScreenFade *fade, u32 type, int steps, int framesPerStep, int state, void *effect, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId, u16 color) {
    fade->type = type;
    fade->steps = steps;
    fade->framesPerStep = framesPerStep;
    fade->state = state;
    fade->effect = effect;
    fade->screen = screen;
    fade->regQueue = regQueue;
    fade->blankPair = blankPair;
    fade->heapId = heapId;
    fade->color = color;
}

void FadeBlankPair_Init(FadeBlankPair *blankPair) {
    int i;
    for (i = 0; i < 2; i++) {
        blankPair->data[i] = NULL;
        blankPair->callbacks[i] = FadeBlankPair_DummyCallback;
        blankPair->active[i] = 0;
    }
}

void FadeBlankPair_HBlankCallback(FadeBlankPair *blankPair) {
    int i;
    for (i = 0; i < 2; i++) {
        blankPair->callbacks[i](blankPair->data[i]);
    }
}

void FadeBlankPair_Start(FadeBlankPair *blankPair, void *data, FadeHBlankCallback callback, int screen) {
    u8 installed = TRUE;
    u32 *active = &blankPair->active[screen];

    if (*active != 0) {
        GF_AssertFail();
    }
    if (blankPair->callbacks[screen] == NULL) {
        GF_AssertFail();
    }
    if (blankPair->active[0] == 0 && blankPair->active[1] == 0) {
        installed = Main_SetHBlankIntrCB((GFIntrCB)FadeBlankPair_HBlankCallback, blankPair);
    }
    if (installed != TRUE) {
        GF_AssertFail();
    }
    blankPair->data[screen] = data;
    if (callback != NULL) {
        blankPair->callbacks[screen] = callback;
    } else {
        blankPair->callbacks[screen] = FadeBlankPair_DummyCallback;
    }
    *active = TRUE;
}

void FadeBlankPair_Stop(FadeBlankPair *blankPair, int screen) {
    blankPair->active[screen] = 0;
    if (blankPair->active[0] == 0 && blankPair->active[1] == 0) {
        HBlankInterruptDisable();
    }
    blankPair->callbacks[screen] = FadeBlankPair_DummyCallback;
    blankPair->data[screen] = NULL;
}

void FadeBlankPair_StartOnVBlank(void *blankPair, void *data, void (*callback)(void *data), int screen, enum HeapID heapId) {
    FadeBlankStartArgs *args = Heap_AllocAtEnd(heapId, sizeof(FadeBlankStartArgs));
    args->blankPair = blankPair;
    args->data = data;
    args->callback = callback;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(FadeBlankPair_StartTask, args, 0x400);
}

void FadeBlankPair_StopOnVBlank(void *blankPair, int screen, enum HeapID heapId) {
    FadeBlankStopArgs *args = Heap_AllocAtEnd(heapId, sizeof(FadeBlankStopArgs));
    args->blankPair = blankPair;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(FadeBlankPair_StopTask, args, 0x400);
}

void FadeBlankPair_StartTask(SysTask *task, void *data) {
    FadeBlankStartArgs *args = data;
    FadeBlankPair_Start(args->blankPair, args->data, args->callback, args->screen);
    SysTask_Destroy(task);
    Heap_Free(args);
}

void FadeBlankPair_StopTask(SysTask *task, void *data) {
    FadeBlankStopArgs *args = data;
    FadeBlankPair_Stop(args->blankPair, args->screen);
    SysTask_Destroy(task);
    Heap_Free(args);
}

void FadeBlankPair_DummyCallback(void *data) {
}

u16 PaletteFade_ResolveColor(PaletteFade *fade, u16 color) {
    if (color == 0xFFFF) {
        color = fade->color;
    }
    return color;
}

// The color the screens are left in: the main screen's fade color if it faded out, else the previous color.
u16 PaletteFade_GetEndColor(PaletteFade *fade) {
    ScreenFade *screenFade;
    if (fade->mainEnabled == TRUE) {
        screenFade = &fade->main;
    } else {
        screenFade = &fade->sub;
    }
    if (screenFade->fadeOut == TRUE) {
        return screenFade->color;
    }
    return fade->color;
}

void ScreenFade_ResetBrightnessTask(SysTask *task, void *data) {
    ScreenFade *fade = data;
    SetMasterBrightness(fade->screen, 0);
    SysTask_Destroy(task);
}

// A fade in from black or white done by a window wipe: the master brightness goes back to neutral on the next VBlank.
void ScreenFade_QueueBrightnessReset(ScreenFade *fade) {
    if (fade->fadeOut == FALSE && (fade->color == RGB_WHITE || fade->color == RGB_BLACK) && !fade->isBrightnessFade) {
        SysTask_CreateOnVWaitQueue(ScreenFade_ResetBrightnessTask, fade, 0x400);
    }
}

// A fade out to black or white done by a window wipe: the screen is left at full brightness of that color with
// the windows off.
void ScreenFade_HoldFadeColor(ScreenFade *fade) {
    if (fade->fadeOut == TRUE && (fade->color == RGB_WHITE || fade->color == RGB_BLACK) && !fade->isBrightnessFade) {
        SetMasterBrightnessByColor((PMLCDTarget)fade->screen, fade->color);
        ResetVisibleHardwareWindows((PMLCDTarget)fade->screen);
    }
}

void PaletteFade_Clear(PaletteFade *fade) {
    memset(fade, 0, offsetof(PaletteFade, main));
    memset(&fade->main, 0, sizeof(ScreenFade));
    memset(&fade->sub, 0, sizeof(ScreenFade));
    memset(&fade->blankPair, 0, sizeof(FadeBlankPair));
    memset(&fade->regQueue, 0, sizeof(WindowRegisterQueue));
}
