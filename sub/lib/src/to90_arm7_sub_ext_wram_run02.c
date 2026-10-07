#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
u8 WMSP_GetRssi8_06001B5C(int x);
// @fn WMSP_GetRssi8_06001B5C
u8 WMSP_GetRssi8_06001B5C(int x){ if(x&2) return x>>2; else return (x>>2)+25; }
