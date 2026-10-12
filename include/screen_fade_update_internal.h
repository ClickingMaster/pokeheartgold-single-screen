#ifndef POKEHEARTGOLD_SCREEN_FADE_UPDATE_INTERNAL_H
#define POKEHEARTGOLD_SCREEN_FADE_UPDATE_INTERNAL_H
#include "global.h"

#include "heap.h"
#include "screen_fade.h"
#include "screen_fade_effects.h"
#include "sys_task_api.h"

typedef void (*FadeBlankFunc)(u32);

typedef struct FadeBlankPair {
    u32 values[2];
    FadeBlankFunc funcs[2];
    u32 active[2];
} FadeBlankPair;

void SetMasterBrightnessByColor(PMLCDTarget screen, u16 color);
void sub_0200FE78(u32 *dst, u32 a1, u32 a2, u32 a3);
void sub_0200FE84(ScreenFade *fade, u32 type, int steps, int framesPerStep, int state, void *effect, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId, u16 color);
void sub_0200FEB0(FadeBlankPair *data);
void sub_0200FECC(FadeBlankPair *data);
void sub_02010014(u32 value);
u16 sub_02010018(u8 *data, u16 color);
u16 sub_0201002C(u8 *data);
void sub_02010050(SysTask *task, void *arg);
void sub_02010064(ScreenFade *fade);
void sub_02010094(ScreenFade *fade);
void sub_020100C4(void *data);
void sub_0200FE14(int mode, u32 *dst);

#include "global.h"

typedef struct FadeUpdate {
    u32 mode;
    u32 mainActive;
    u32 subActive;
} FadeUpdate;

typedef BOOL (*FadeFunc)(void *);
extern FadeFunc sFadeFuncPtrs[];
BOOL DoFadeUpdateFrame(FadeUpdate *state, void *mainWork, void *subWork);
void FadeWork_UpdateFrame(u32 *active, void *work);
BOOL CallFadeFunc(void *work);

void HBlankInterruptDisable(void);
void sub_0200FF5C(FadeBlankPair *data, int index);
void sub_0200FF88(void *blankPair, void *data, void (*callback)(void *data), int screen, enum HeapID heapId);
void sub_0200FFD8(SysTask *task, void *arg);
void sub_0200FFF8(SysTask *task, void *arg);
void sub_0200FEE4(void *blankPair, void *data, void (*callback)(void *data), int screen);

typedef struct FadeSetTaskData {
    void *blankPair;
    void *data;
    void (*callback)(void *data);
    int screen;
} FadeSetTaskData;

typedef struct FadeClearTaskData {
    FadeBlankPair *data;
    int index;
} FadeClearTaskData;

void sub_0200FE14(int mode, u32 *dst);
void sub_0200FE78(u32 *dst, u32 a1, u32 a2, u32 a3);
void sub_0200FE84(ScreenFade *fade, u32 type, int steps, int framesPerStep, int state, void *effect, int screen, WindowRegisterQueue *regQueue, void *blankPair, enum HeapID heapId, u16 color);
void sub_0200FEB0(FadeBlankPair *data);
void sub_0200FECC(FadeBlankPair *data);
BOOL DoFadeUpdateFrame(FadeUpdate *state, void *mainWork, void *subWork);
void FadeWork_UpdateFrame(u32 *active, void *work);
BOOL CallFadeFunc(void *work);
void sub_0200FF5C(FadeBlankPair *data, int index);
void sub_0200FF88(void *blankPair, void *data, void (*callback)(void *data), int screen, enum HeapID heapId);
void sub_0200FFD8(SysTask *task, void *arg);
void sub_0200FFF8(SysTask *task, void *arg);
void sub_02010014(u32 value);
u16 sub_02010018(u8 *data, u16 color);
u16 sub_0201002C(u8 *data);
void sub_02010050(SysTask *task, void *arg);
void sub_02010064(ScreenFade *fade);
void sub_02010094(ScreenFade *fade);

#endif
