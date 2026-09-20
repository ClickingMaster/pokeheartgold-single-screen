#include "to54_atomic_x10_to54_overlay91_run_032_internal.h"
typedef struct {
    u8 active;
    u8 pad01[0xCB];
    s32 radius;
    s32 slope;
    u8 padD4[4];
    s32 centerX;
    s32 lowerY;
    s32 centerZ;
    s32 height;
} X10Shape;

extern const VecFx32 ov91_02261C4C;

void ov91_0225DD24(void *p, s16 angle) {
    S16(p, 4) = angle;
    MTX_Identity33_((MtxFx33 *)((u8 *)p + 0xC));
    MTX_RotX33_((MtxFx33 *)((u8 *)p + 0xC), FX_SinIdx(angle), FX_CosIdx(angle));
}

void ov91_0225DD50(void *p, s16 angle) {
    VecFx32 v;
    fx32 dot;
    S16(p, 2) = angle;
    MTX_Identity33_((MtxFx33 *)((u8 *)p + 0x54));
    MTX_Identity33_((MtxFx33 *)((u8 *)p + 0x30));
    MTX_RotY33_((MtxFx33 *)((u8 *)p + 0x30), FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_Concat33((MtxFx33 *)((u8 *)p + 0xC), (MtxFx33 *)((u8 *)p + 0x30), (MtxFx33 *)((u8 *)p + 0x54));
    MTX_MultVec33(&ov91_02261C4C, (MtxFx33 *)((u8 *)p + 0x54), (VecFx32 *)((u8 *)p + 0x78));
    v.x = 0;
    v.y = 0;
    v.z = -S32(p, 8);
    MTX_MultVec33(&v, (MtxFx33 *)((u8 *)p + 0x54), (VecFx32 *)((u8 *)p + 0x84));
    v.x = S32(p, 0xB8) + S32(p, 0x9C);
    v.y = S32(p, 0xBC);
    v.z = S32(p, 0xA4) + 0x50000;
    MTX_MultVec33(&v, (MtxFx33 *)((u8 *)p + 0x30), &v);
    VEC_Add(&v, (VecFx32 *)((u8 *)p + 0x84), (VecFx32 *)((u8 *)p + 0x84));
    dot = FX_Mul(S32(p, 0x78), S32(p, 0x84)) + FX_Mul(S32(p, 0x7C), S32(p, 0x88)) + FX_Mul(S32(p, 0x80), S32(p, 0x8C));
    S32(p, 0x90) = dot;
    MTX_MultVec33((VecFx32 *)((u8 *)p + 0x9C), (MtxFx33 *)((u8 *)p + 0x30), (VecFx32 *)((u8 *)p + 0xD8));
}

int ov91_0225DE8C(void *shape, const VecFx32 *target, void *arg2, VecFx32 *out) {
    VecFx32 ignored;
    VecFx32 delta;
    VecFx32 normalized;
    fx32 intersection;
    fx32 distance;
    fx32 factor;
    int status = 0;
    if (U8(shape, 0) == 0) {
        return status;
    }
    ov91_02261208((void *)target, arg2, (u8 *)shape + 0x78, (void *)S32(shape, 0x90), &ignored, &intersection);
    VEC_Subtract((VecFx32 *)((u8 *)shape + 0x84), target, &delta);
    distance = VEC_Mag(&delta);
    if (intersection >= 0 && intersection <= FX32_ONE) {
        if (distance <= S32(shape, 8)) {
            if (U8(shape, 1) == 1) {
                status = 1;
            } else {
                status = 2;
            }
        } else {
            return 2;
        }
    }
    if (intersection > 0 && distance < S32(shape, 0x94)) {
        factor = FX_Mul(S32(shape, 0x94) - distance, S32(shape, 0x98));
        VEC_Normalize(&delta, &normalized);
        out->x = FX_Mul(normalized.x, factor);
        out->y = FX_Mul(normalized.y, factor);
        out->z = FX_Mul(normalized.z, factor);
        if (distance - factor <= S32(shape, 8)) {
            if (U8(shape, 1) == 1) {
                status = 1;
            } else {
                status = 2;
            }
        } else {
            status = 3;
        }
    }
    return status;
}

BOOL ov91_0225DFB0(X10Shape *shape, const VecFx32 *pos) {
    VecFx32 delta;
    VecFx32 center;
    VecFx32 horizontal;
    fx32 distance;
    s32 scaled;
    s32 y;
    if (shape->active == 0) {
        return FALSE;
    }
    if (shape->lowerY < pos->y && shape->lowerY + shape->height > pos->y) {
        center.x = shape->centerX;
        center.z = shape->centerZ;
        center.y = 0;
        horizontal.x = pos->x;
        horizontal.z = pos->z;
        horizontal.y = 0;
        VEC_Subtract(&center, &horizontal, &delta);
        distance = VEC_Mag(&delta);
        y = (pos->y - shape->lowerY) >> FX32_SHIFT;
        scaled = _s32_div_f(y * shape->slope, shape->height >> FX32_SHIFT);
        if (distance < shape->radius - scaled) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 ov91_0225E044(void *p, s32 x) {
    s32 a = 0x5A;
    s32 t;
    a <<= 2;
    t = _s32_div_f(a * x, 0xFFFF);
    t = _s32_div_f((0x5A - t) * S32(p, 0xAC), 0x5A);
    t += S32(p, 0xB0);
    return t;
}

s32 ov91_0225E070(void *p, s32 x) {
    s32 a = 0x5A;
    s32 t;
    a <<= 2;
    t = _s32_div_f(a * x, 0xFFFF);
    t = _s32_div_f(t * S32(p, 0xC4), 0x5A);
    t += S32(p, 0xC0);
    return t;
}

void ov91_0225E09C(void *p, s32 x) {
    s32 retained;
    S32(p, 0xA8) = x;
    S32(p, 0xAC) = FX_Mul(x, 30 << FX32_SHIFT);
    S32(p, 0xB0) = FX_Mul(x, 50 << FX32_SHIFT);
    retained = FX_Mul(x, 80 << FX32_SHIFT);
    S32(p, 0xB4) = retained;
    S32(p, 0xB8) = 0;
    S32(p, 0xBC) = FX_Mul(x, 55 << FX32_SHIFT);
    S32(p, 0xC0) = FX_Mul(x, 150 << FX32_SHIFT);
    S32(p, 0xC4) = FX_Mul(x, 60 << FX32_SHIFT);
    S32(p, 0xC8) = retained;
    S32(p, 0xCC) = FX_Mul(x, 120 << FX32_SHIFT);
    S32(p, 0xD0) = S32(p, 0xCC) - S32(p, 0xC8);
    S32(p, 0xD4) = FX_Mul(x, 0x1800);
}
