#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void sys_exit(void);
void _ExitProcess(void);

void _ExitProcess(void) {
    sys_exit();
}
