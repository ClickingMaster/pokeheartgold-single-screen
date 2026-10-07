#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern double __ieee754_pow(double,double);double pow(double,double);
#pragma force_active on
double pow(double a,double b){return __ieee754_pow(a,b);}
