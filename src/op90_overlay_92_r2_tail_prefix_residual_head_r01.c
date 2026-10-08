#pragma force_active on
#include "global.h"
typedef struct Ov92Hitbox Ov92Hitbox;
typedef struct Ov92TouchWork {
    u8 pad0[0x114];
    u8 motionA[0x30C - 0x114];
    int x30C;
    int x310;
    int x314;
    u8 pad318[8];
    u8 motionB[0x4D0 - 0x320];
    MtxFx43 mtx;
    u8 pad500[0x1FC8 - 0x500];
    VecFx32 pos;
    u8 pad1FD4[0];
    int lastX;
    int lastY;
    u8 pad1FDC[0x2B84 - 0x1FDC];
    int touched;
    u8 pad2B88[0xC];
    int everTouched;
    u8 pad2B98[0x2BB4 - 0x2B98];
    int altHitboxes;
} Ov92TouchWork;
extern const VecFx32 ov92_02263884;
extern const VecFx32 ov92_02263878;
BOOL System_GetTouchHeldCoords(u32 *x, u32 *y);
int TouchscreenHitbox_FindHitboxAtPoint(const Ov92Hitbox *, u32, u32);
void MTX_MultVec43(const VecFx32 *, const MtxFx43 *, VecFx32 *);
BOOL IsSEPlaying(u16);
BOOL PlaySE(u16);
void ov92_02260628(void *, f32);
void ov92_02260428(void *, s32, s32, s32, s32, f32, s32);
void ov92_02260A38(Ov92TouchWork *, s16, s16);
void ov92_0225CB2C(Ov92TouchWork *work, int x, int y);
// @fn ov92_0225CB2C
void ov92_0225CB2C(Ov92TouchWork *work, int x, int y) {
    VecFx32 vec1;
    VecFx32 vec2;
    f32 a;
    int v;
    if (x == 0 && y == 0) {
        work->lastX = 0;
        work->lastY = 0;
        if (work->x30C) {
            ov92_02260628(work->motionA, 1.0f);
            vec1 = ov92_02263884;
            MTX_MultVec43(&vec1, &work->mtx, &work->pos);
            return;
        }
        work->x310 = 0;
        work->x314 = 0;
        ov92_02260428(work->motionA, work->lastX, work->lastY, work->lastX, work->lastY, 1.88f, 0);
        ov92_02260428(work->motionB, work->lastX, work->lastY, work->lastX, work->lastY, 0.22f, 0);
        return;
    }
    v = work->pos.y >> 12;
    a = (100 - v) / 40;
    if (v >= 95) {
        a = 0.1f;
    }
    if (work->lastX == 0) {
        work->lastX = x;
        work->lastY = y;
    }
    ov92_02260428(work->motionA, x, y, work->lastX, work->lastY, 2.0f, 1);
    vec2 = ov92_02263878;
    MTX_MultVec43(&vec2, &work->mtx, &work->pos);
    if (!IsSEPlaying(0x58A)) {
        PlaySE(0x58A);
    }
    work->lastX = x;
    work->lastY = y;
}
