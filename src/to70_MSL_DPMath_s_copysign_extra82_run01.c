#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
double copysign(double,double);
double copysign(double x,double y) { ((u32 *)&x)[1]=(((u32 *)&x)[1]&0x7fffffff)|(((u32 *)&y)[1]&0x80000000);return x; }
