#pragma force_active on
#include "unk02012204_sol_partial_internal.h"
// @fn sub_02012238
void sub_02012238(VectorListWork *work, VectorRecord *rec) { s16 *buf = sub_02010EE0(work, 0); s32 i; s32 x0 = rec->x0 / 128, y0 = rec->y0 / 128, x1 = rec->x1 / 128, y1 = rec->y1 / 128; for (i = x1; i < y1; i++) { buf[0x180 + i] = x0; buf[0x240 + i] = y0; } }
