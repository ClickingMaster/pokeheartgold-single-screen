#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void _ZdlPv(void *);void _ZdaPv(void *);
#pragma force_active on
void _ZdaPv(void *p){_ZdlPv(p);}
