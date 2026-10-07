#ifndef TO57_NEW28_PRIVATE_H
#define TO57_NEW28_PRIVATE_H
#include "global.h"
typedef struct SysTask SysTask;
void SysTask_Destroy(SysTask *);
void DoAllScreenBrightnessTransitionStep(void);
BOOL IsBrightnessTransitionActive(u32 screen);
void StartBrightnessTransition(u8 screen, s8 start, s8 end, s16 duration, u32 planeMask);
void ScreenBrightnessData_InitAll(void);
SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *, void *), void *, u32);
void *Heap_Alloc(u32, u32);
void Heap_Free(void *);
void MI_CpuFill8(void *, u8, u32);
void ov07_0221C69C(void);
int ov07_0223197C(void *, u16);
int ov07_0221FA04(void *, int);
int Pokepic_GetAttr(void *, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void BgSetPosTextAndCommit(void *, int, int, int);
struct BrightnessTaskData;
struct MotionData;
void ov07_0221D2F4(struct BrightnessTaskData *, int);
void ov07_0221D330(SysTask *, struct BrightnessTaskData *);
void ov07_0221D374(void *);
int ov07_0221D3CC(void *, u32);
void ov07_0221D4B0(SysTask *, void *);
void ov07_0221D4FC(SysTask *, struct MotionData *);
void ov07_0221D55C(void *, u32);
#endif
