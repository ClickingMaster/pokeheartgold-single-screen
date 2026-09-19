#ifndef W07_OV96_PRIVATE_H
#define W07_OV96_PRIVATE_H
#include "global.h"

#include "sprite.h"
typedef struct W07PositionObject {
    void *object;
    Sprite *sprite1;
    Sprite *sprite2;
    s32 offsetY;
    s32 offsetX;
    s32 halfHeight;
    s32 radius;
    u8 pad1C[4];
    s32 x;
    s32 y;
    s32 center;
    s32 bottom;
} W07PositionObject;
void *ov96_021E8BAC(void *);
BOOL ov96_021E92AC(void *);
void *ov96_021EAA20(void *);
void ov96_021EAD88(W07PositionObject *, s32, s32, BOOL);
void ov96_021EAE4C(W07PositionObject *, s32, s32);
void ov96_021EAE9C(void *, s32 *, s32 *);
#endif
