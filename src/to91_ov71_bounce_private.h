#ifndef TO91_OV71_BOUNCE_PRIVATE_H
#define TO91_OV71_BOUNCE_PRIVATE_H
#include "global.h"
#include "sys_task_api.h"
#include "overlay71_opus_partial_internal.h"
typedef struct Ov71BounceJob {
    SysTask **out;
    s32 state;
    void *object;
    s32 timer;
    void *pic;
    void *sprite;
    s32 angle, delta, count;
    Ov71Vec3 pos;
    s32 velocity;
    Ov71S16Vec3 rotation;
    u16 scale;
    s16 step, target;
    s32 frames;
} Ov71BounceJob;
void ov71_0224BABC(SysTask *task, void *context);
void ov71_0224B9CC(void *work, SysTask **out);
#endif
