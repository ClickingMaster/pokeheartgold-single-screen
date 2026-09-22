#ifndef TO57_R3_04_PRIVATE_H
#define TO57_R3_04_PRIVATE_H
#include "global.h"
#pragma require_prototypes off

typedef struct R304SinMotion {
    s16 x;
    s16 y;
    s32 duration;
    s32 angleX;
    s32 ampX;
    s32 angleY;
    s32 ampY;
    s32 stepX;
    s32 stepY;
    s32 unk20;
} R304SinMotion;

typedef struct R304Transition {
    R304SinMotion motion;
    u32 done;
} R304Transition;

typedef struct R304Work {
    s16 x;
    s16 y;
    R304SinMotion motions[4];
    BOOL (*callback)(R304SinMotion *);
    void *sprites[4];
    u16 duration;
    u16 counter;
    u8 count;
    u8 active;
    u8 affine;
    u8 padAF;
} R304Work;

void ManagedSprite_SetDrawFlag(void *, BOOL);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_SetAffineScale(void *, u32, u32);
void ov07_02222644(R304SinMotion *, u32 *, u32 *);
void ov07_022220FC(R304SinMotion *, int, int, int, int, int, int, int);
void ov07_02222268(void *, s16, s16, s16, s16, u16);
BOOL ov07_022222B4(void *);
typedef struct SysTask SysTask;
SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *, void *), void *, u32);
void SysTask_Destroy(SysTask *);
#endif
