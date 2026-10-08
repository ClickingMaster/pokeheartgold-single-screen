#pragma force_active on
#include "global.h"
#include "scanline_effect_internal.h"
void ov01_021F1310(Scanline **ctx, u32 period);
// @fn ov01_021F1310
void ov01_021F1310(Scanline **ctx, u32 period) { int i; s16 v; u32 *p; p = sub_02014BF8(*ctx); for (i = 0; i < 192; i++) { v = *p; if (((i / period) & 1) == 0) { *p = (u16)v; } else { *p = (u16)-v; } p++; } }
