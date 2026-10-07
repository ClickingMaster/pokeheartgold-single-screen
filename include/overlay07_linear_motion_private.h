#ifndef OVERLAY07_LINEAR_MOTION_PRIVATE_H
#define OVERLAY07_LINEAR_MOTION_PRIVATE_H
#include "global.h"
typedef struct W02LinearMotion {
    s16 x, y;
    int duration;
    int dx, dy;
    int curX, curY;
} W02LinearMotion;
#endif
