#ifndef TO55_W03_OVERLAY112_PRIVATE_H
#define TO55_W03_OVERLAY112_PRIVATE_H

#include "global.h"

u32 GetBoxMonData(void *boxmon, int attr, void *dest);
int _s32_div_f(int numerator, int denominator);

void ov112_021E8C5C(const u8 *values, void *boxmon, u16 *stats);

#endif
