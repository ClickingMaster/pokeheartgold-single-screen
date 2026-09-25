#include "global.h"
extern void ov91_0225EEBC(void *, void *, void *);
extern void ov91_0225F008(void *, void *);
extern s32 FX_Div(s32, s32);
void ov91_0225D3C4(u8 *, const u8 *);
typedef struct Curve91 {
    s32 value;
    s32 start;
    s32 slope;
    s32 curvature;
    s32 step;
    s32 duration;
} Curve91;
void ov91_0225D40C(Curve91 *, s32, s32, s32, s32);
s32 ov91_0225D46C(Curve91 *);
void ov91_0225D3C4(u8 *p, const u8 *v) {
    p[0x8784] = v[0];
    p[0x8785] = v[1];
    p[0x8786] = v[2];
    p[0x8787] = v[3];
    *(u32 *)(p + 0x8788) = 1;
    ov91_0225EEBC(p + 0x8730, *(void **)(p + 0x877c), p + 0x8784);
    ov91_0225F008(p, p + 0x8730);
}
void ov91_0225D40C(Curve91 *curve, s32 start, s32 end, s32 slope, s32 duration) {
    s32 delta = end - start - (s32)(((s64)slope * (duration << 12) + 0x800) >> 12);
    s32 curvature = FX_Div((s32)((((s64)delta << 13) + 0x800) >> 12), (duration * duration) << 12);
    curve->value = start;
    curve->start = start;
    curve->slope = slope;
    curve->curvature = curvature;
    curve->step = 0;
    curve->duration = duration;
}

s32 ov91_0225D46C(Curve91 *curve) {
    s32 step = curve->step;
    s32 slopePart = (s32)(((s64)curve->slope * (step << 12) + 0x800) >> 12);
    s32 curvePart = (s32)(((s64)curve->curvature * ((step * step) << 12) + 0x800) >> 12);
    curve->value = curve->start + (slopePart + FX_Div(curvePart, 0x2000));
    s32 stepValue = *(volatile s32 *)&curve->step;
    s32 duration = curve->duration;
    s32 next = stepValue + 1;
    if (next <= duration) {
        curve->step = next;
        return 0;
    }
    curve->step = duration;
    return 1;
}
