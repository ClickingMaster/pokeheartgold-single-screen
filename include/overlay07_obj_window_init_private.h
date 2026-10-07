#ifndef OVERLAY07_OBJ_WINDOW_INIT_PRIVATE_H
#define OVERLAY07_OBJ_WINDOW_INIT_PRIVATE_H
#include "global.h"
#include "sys_task.h"
typedef struct Ov07Init {
    int value0, value4, value8;
    s16 xVel, yVel, x, y;
    u16 alphaA, alphaB;
} Ov07Init;
void ov07_0223174C(void *, const Ov07Init *);
void ov07_022315C0(SysTask *, void *);
#endif
