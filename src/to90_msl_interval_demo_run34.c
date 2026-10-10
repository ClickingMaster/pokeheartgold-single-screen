#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void abort(void);
void _ZSt9dthandlerv(void);
#pragma force_active on

void _ZSt9dthandlerv(void) {
    abort();
}
