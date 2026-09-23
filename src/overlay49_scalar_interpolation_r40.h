#ifndef POKEHEARTGOLD_OVERLAY49_SCALAR_INTERPOLATION_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY49_SCALAR_INTERPOLATION_PRIVATE_H
#include "global.h"
typedef struct Ov49ScalarInterpolation {
    s32 value;
    s32 start;
    s32 delta;
    u32 duration;
} Ov49ScalarInterpolation;
extern s64 _ll_mul(s64, s64);
extern s32 FX_Div(s32, s32);
#endif
