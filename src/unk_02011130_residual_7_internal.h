#ifndef PRIVATE_H
#define PRIVATE_H

#include "global.h"
#include "sys_task_api.h"

typedef struct FadeConfig11DEC {
    u16 start;
    u16 end;
    u8 direction;
    u8 mode1;
    u8 mode2;
    u8 value;
} FadeConfig11DEC;

typedef struct FadeWork11DEC {
    u8 padding00[0xC];
    s32 phase;
    s32 current;
    s32 step;
    s32 steps;
    s32 interval;
    s32 counter;
    u32 value;
    u32 arg5;
    u32 arg6;
    u32 arg7;
} FadeWork11DEC;

void ScanlineBufferController_Init(void *work, u32 direction, u32 arg4, u32 arg7);
void sub_02011F10(void *work);
void sub_02010F00(SysTask *task, void *work);
s16 *sub_02010EE0(void *work, int screen);
void sub_02010F84(u32 arg5, u32 mode1, u32 mode2, u32 direction, u32 arg4,
                  s32 first, s32 zero, s32 second, s32 amount, u32 value);
void sub_02011068(u32 arg5, int screen, u32 arg4, u32 value);
void ScanlineWindow_HBlankCallback(void);
void sub_0200FF88(u32 arg6, u32 work, u32 callback, u32 arg4, u32 arg7);
void sub_02011DEC(FadeWork11DEC *work, const FadeConfig11DEC *config, s32 steps, s32 interval,
                  u32 arg4, u32 arg5, u32 arg6, u32 arg7);

#endif
