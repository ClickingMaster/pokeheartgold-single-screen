#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void _ZdlPv(void *);
void _ZdaPv(void *);
#pragma force_active on

void _ZdaPv(void *p) {
    _ZdlPv(p);
}
