#ifndef OVERLAY07_SLOT_MOTION_WORK_PRIVATE_H
#define OVERLAY07_SLOT_MOTION_WORK_PRIVATE_H
#include "global.h"
#include "heap.h"
#include "sys_task.h"
#include "overlay07_script_slot_private.h"
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
typedef struct W02SinMotion {
    s16 x, y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
} W02SinMotion;
union Ov07SinMotionBridge { R304SinMotion creator; W02SinMotion updater; };
typedef struct Ov07SlotMotionWork {
    Ov07T10Work prefix;
    Ov07T10Slot slot;
    union Ov07SinMotionBridge motion;
} Ov07SlotMotionWork;
typedef char Ov07SlotMotionWorkSize[(sizeof(Ov07SlotMotionWork)==84)?1:-1];
#endif
