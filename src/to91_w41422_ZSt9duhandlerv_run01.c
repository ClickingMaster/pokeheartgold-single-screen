#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void _ZSt9terminatev(void);
void _ZSt9duhandlerv(void);

void _ZSt9duhandlerv(void) {
    _ZSt9terminatev();
}
