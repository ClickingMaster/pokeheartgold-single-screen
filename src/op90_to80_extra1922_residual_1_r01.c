#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
u32 NNSi_G2dCalcRequiredOBJ(u32 w,u32 h);
#pragma force_active on
// @fn NNSi_G2dCalcRequiredOBJ
u32 NNSi_G2dCalcRequiredOBJ(u32 w,u32 h){const u32 wx=w/8;const u32 hy=h/8;u32 n=0;const u32 wa=((w&2)>>1)+(w&1);const u32 w4=(w&4)>>2;const u32 ha=((h&2)>>1)+(h&1);const u32 h4=(h&4)>>2;n+=wx*hy;n+=hy*(wa*2+w4);n+=wx*(ha*2+h4);n+=(wa+w4)*(ha+h4);return n;}
