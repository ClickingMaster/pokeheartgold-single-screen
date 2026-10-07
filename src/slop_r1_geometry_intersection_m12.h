#ifndef POKEHEARTGOLD_SLOP_R1_GEOMETRY_INTERSECTION_M12_H
#define POKEHEARTGOLD_SLOP_R1_GEOMETRY_INTERSECTION_M12_H

#include "global.h"

fx32 FX_Div(fx32 numerator, fx32 denominator);
fx32 FX_Modf(fx32 value, fx32 *integerPart);
BOOL sub_02020EB0(const void *a, const void *b, const void *c, const void *d);
BOOL sub_02020F4C(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *out);

#endif
