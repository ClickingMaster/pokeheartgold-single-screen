#ifndef TO91_OV07_MOTION_OWNER_H
#define TO91_OV07_MOTION_OWNER_H
#include "global.h"
struct ManagedSprite;
struct S {s16 x,y;int steps;u32 a;int ra;u32 b;int rb,da,db;};
typedef struct Ov07MotionRow {struct S interpolation;unsigned char unusedRepresentation[4];} Ov07MotionRow;
typedef struct Ov07MotionWork {void *owner,*spriteSystem,*spriteManager;int state,timer,amplitude;struct ManagedSprite *sprites[6];Ov07MotionRow motions[6];u8 remaining[0x28];s16 x,y;} Ov07MotionWork;
#endif
