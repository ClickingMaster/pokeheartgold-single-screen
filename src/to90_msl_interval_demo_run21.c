#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void free(void*);void _ZdlPv(void*);
void _ZdlPv(void*p){if(p)free(p);}
