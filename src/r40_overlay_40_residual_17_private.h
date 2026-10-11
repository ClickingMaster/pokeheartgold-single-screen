#ifndef OVERLAY_40_RESIDUAL_17_PRIVATE_H
#define OVERLAY_40_RESIDUAL_17_PRIVATE_H

#include "global.h"

#define PTRAT(p, n) (*(void **)((u8 *)(p) + (n)))
#define U32AT(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define ADD(p, n)   ((void *)((u8 *)(p) + (n)))

void ov40_0222FB40(void *p);
void ov40_0222FB74(void *task, void *data);
void *ov40_0222FB90(void *p, void *value);

extern void BattleRecord_Create(void *, u32, void *);
extern void BattleRecord_Import(void *, void *, void *, u32, void *);
extern void VsRecorder_SetScreensFlipped(void *);
extern void sub_0203A86C(void);
extern void SysTask_Destroy(void *);
extern void *SysTask_CreateOnVBlankQueue(void (*)(void *, void *), void *, u32);

#endif
