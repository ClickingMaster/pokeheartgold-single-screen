#ifndef POKEHEARTGOLD_OVERLAY93_ARM_SLOP_R1_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY93_ARM_SLOP_R1_INTERNAL_H

// clang-format off
#include "global.h"
#include <nnsys/g3d/gecom.h>
// clang-format on

typedef struct Ov93SlopR1Vtx {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
    s32 g;
    s32 h;
} Ov93SlopR1Vtx;

typedef struct Ov93SlopR1VtxSet {
    u8 _p000[0x0c];
    Ov93SlopR1Vtx lo[8];
    Ov93SlopR1Vtx hi[8];
} Ov93SlopR1VtxSet;

typedef struct Ov93SlopR1Work Ov93SlopR1Work;

void ov93_0225EFAC(Ov93SlopR1VtxSet *ctx);
void ov93_0225F268(s32 a0, s32 a1, s16 *p2, s16 *p3);
s32 ov93_0225F8E4(Ov93SlopR1Work *work, s32 a1, s32 a2, s32 a3, s32 *p4, s32 *p5);

#endif
