#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
void free(void *);
void _ZdlPv(void *);

void _ZdlPv(void *p) {
    if (p) {
        free(p);
    }
}
