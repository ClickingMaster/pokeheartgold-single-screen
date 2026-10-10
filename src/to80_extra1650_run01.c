#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sprintf(char *, const char *, ...);
extern int ov45_022577F4;
extern char ov45_022577F0[];
extern char ov45_0225709C[];
char *ov45_022514F4(void);

char *ov45_022514F4(void) {
    sprintf(ov45_022577F0, ov45_0225709C, ov45_022577F4++);
    ov45_022577F4 %= 1000;
    return ov45_022577F0;
}
