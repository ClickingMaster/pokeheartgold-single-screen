#ifndef TO58_R2_OV07_MID0_PRIVATE_H
#define TO58_R2_OV07_MID0_PRIVATE_H
#include "global.h"

typedef struct R2Motion {
    s16 x, y;
    s32 duration;
    s32 value8;
    s32 valueC;
    s32 currentX;
    s32 currentY;
    s32 deltaX;
    s32 deltaY;
    s32 deltaTail;
} R2Motion;

typedef struct R2TaskWork {
    void *vblankTask;
    void *vwaitTask;
    s32 enabled;
    s32 state;
    void *arg10;
    void *arg14;
    void *arg18;
} R2TaskWork;

void GF_AssertFail(void);
int _s32_div_f(int,int);
int BattleAnimUtil_CalcStep(int,int,int);
void *Heap_Alloc(int,int);
void *SysTask_CreateOnVWaitQueue(void (*)(void *,void *),void *,int);
void *SysTask_CreateOnVBlankQueue(void (*)(void *,void *),void *,int);
void Pokepic_SetAttr(void *,int,int);
void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
void ManagedSprite_SetPositionXY(void *,s16,s16);

#endif
