#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void (*_02111848[2])(void);
void _ZSt10unexpectedv(void);

void _ZSt10unexpectedv(void) {
    _02111848[1]();
}
