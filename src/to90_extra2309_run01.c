#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern float __float_nan;
double nan(const char*);
double nan(const char*tag){return (double)__float_nan;}
