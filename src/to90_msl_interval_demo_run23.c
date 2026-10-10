#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
void _ZdlPv(void *);
void *_ZNSt9bad_allocD0Ev(void *);

void *_ZNSt9bad_allocD0Ev(void *p) {
    _ZdlPv(p);
    return p;
}
