#ifndef OVERLAY96_POSITION_HELPERS_R40_PRIVATE_H
#define OVERLAY96_POSITION_HELPERS_R40_PRIVATE_H
#include "global.h"
typedef struct Ov96PositionObject {
    void *object;
    void *sprite1;
    void *sprite2;
    u8 pad0C[4];
    s32 offsetY;
    s32 offsetX;
    s32 halfHeight;
    s32 radius;
    s32 x;
    s32 y;
    s32 center;
    s32 bottom;
} Ov96PositionObject;
void *PokeathlonCourseMonGfx_GetSprite(void *);
BOOL PokeathlonCourseMonGfx_IsShiftedUp(void *);
void PokeathlonCourseMonObject_SetPosition(Ov96PositionObject *, s32, s32, u32);
void Sprite_SetMatrix(void *, VecFx32 *);
void PokeathlonCourseObject_SetGroundPosition(Ov96PositionObject *, s32, s32);
void PokeathlonCourseObject_SetCenterPosition(Ov96PositionObject *, s32, s32, u32);
void PokeathlonCourseObject_GetCollisionCenter(Ov96PositionObject *, s32, s32, s32 *, s32 *);
void PokeathlonCourseObject_CenterToGround(Ov96PositionObject *, s32, s32, s32 *, s32 *);
void PokeathlonCourseObject_GroundToCenter(Ov96PositionObject *, s32, s32, s32 *, s32 *);
BOOL PokeathlonCourseObject_IsInTouchRange(Ov96PositionObject *, fx32, fx32, s32, s32);
#endif
