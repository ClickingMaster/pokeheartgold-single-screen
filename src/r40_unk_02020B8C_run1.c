#include "r40_unk_02020B8C_private.h"

void sub_02020E10(s32 angle, fx32 first, fx32 second, fx32 *outSecond, fx32 *outFirst) {
    fx32 ratio = FX_Div(FX_SinIdx(angle), FX_CosIdx(angle));
    fx32 value = FX_Mul(first, ratio);

    value = FX_Mul(value, 2 * FX32_ONE);
    *outFirst = value;
    *outSecond = FX_Mul(*outFirst, second);
}

static inline s32 GeometryXY_Read(const void *point, u32 offset) {
    return *(const s32 *)((const u8 *)point + offset);
}

BOOL sub_02020E80(const void *first, const void *second, const void *third) {
    s32 thirdTerm = GeometryXY_Read(second, 0) * (GeometryXY_Read(third, 4) - GeometryXY_Read(first, 4));
    s32 secondTerm = GeometryXY_Read(first, 0) * (GeometryXY_Read(second, 4) - GeometryXY_Read(third, 4));
    s32 firstTerm = GeometryXY_Read(third, 0) * (GeometryXY_Read(first, 4) - GeometryXY_Read(second, 4));

    return firstTerm + secondTerm + thirdTerm >= 0;
}
