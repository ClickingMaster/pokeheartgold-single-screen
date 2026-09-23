#include "overlay49_scalar_interpolation_r40.h"

BOOL ov49_0225932C(Ov49ScalarInterpolation *, u32);

BOOL ov49_0225932C(Ov49ScalarInterpolation *work, u32 current) {
    float currentFx;
    float durationFx;
    s32 scaled;
    s32 delta = work->delta;

    if (current != 0) {
        currentFx = (float)(current << 12) + 0.5f;
    } else {
        currentFx = (float)(current << 12) - 0.5f;
    }
    scaled = (s32)((_ll_mul((s64)delta, (s64)(s32)currentFx) + 0x800) >> 12);
    if (work->duration != 0) {
        durationFx = (float)(work->duration << 12) + 0.5f;
    } else {
        durationFx = (float)(work->duration << 12) - 0.5f;
    }
    scaled = FX_Div(scaled, (s32)durationFx);
    scaled += work->start;
    work->value = scaled;
    return current >= work->duration;
}
