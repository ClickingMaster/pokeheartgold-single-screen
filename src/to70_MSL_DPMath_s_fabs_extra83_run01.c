#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
double fabs(double);
double fabs(double x) { ((u32 *)&x)[1]&=0x7fffffff;return x; }
