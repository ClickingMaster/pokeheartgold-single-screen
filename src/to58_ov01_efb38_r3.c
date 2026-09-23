#include "field_overlay_effects_internal.h"
typedef struct QuadraticInterp {
    s32 value;
    s32 start;
    s32 linear;
    s32 quadratic;
    s32 frame;
    s32 duration;
} QuadraticInterp;
void ov01_021EFEC8(QuadraticInterp *, s32, s32, s32, s32);
BOOL ov01_021EFF28(QuadraticInterp *);
void ov01_021EFEC8(QuadraticInterp *p, s32 start, s32 end, s32 linear, s32 duration) {
    s32 linearTotal = (s32)(((s64)linear * (duration << 12) + 0x800) >> 12);
    s32 remaining = end - start - linearTotal;
    s32 numerator = (s32)((((s64)remaining << 13) + 0x800) >> 12);
    s32 denominator = (duration * duration) << 12;
    s32 coeff = FX_Div(numerator, denominator);
    p->value = start;
    p->start = start;
    p->linear = linear;
    p->quadratic = coeff;
    p->frame = 0;
    p->duration = duration;
}
BOOL ov01_021EFF28(QuadraticInterp *p) {
    s32 frame = p->frame;
    s32 linear = (s32)(((s64)p->linear * (frame << 12) + 0x800) >> 12);
    s32 square = (frame * frame) << 12;
    s32 quad = (s32)(((s64)p->quadratic * square + 0x800) >> 12);
    p->value = linear + FX_Div(quad, 0x2000) + p->start;
    frame = *(volatile s32 *)&p->frame;
    if (frame + 1 <= p->duration) {
        p->frame = frame + 1;
        return FALSE;
    }
    p->frame = p->duration;
    return TRUE;
}
