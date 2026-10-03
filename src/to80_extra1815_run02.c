#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 mode:2;u32 rest:30;} Bits;

void*ov96_0220B758(u8*w);
void*ov96_0220B758(u8*w){GF_ASSERT(w);return w+52+((Bits*)(w+64))->mode*4;}
