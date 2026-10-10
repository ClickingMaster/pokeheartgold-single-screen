#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char *ov00_0221B9D0;
char *ov00_02212F60(char *, int);

char *ov00_02212F60(char *input, int delim) {
    int ch;
    char *p, *start;
    if (input) {
        ov00_0221B9D0 = input;
    }
    start = ov00_0221B9D0;
    p = start;
    while ((ch = *p) != 0 && ch != delim) {
        p++;
        ov00_0221B9D0 = p;
    }
    if (p == start) {
        start = 0;
    }
    if (ch == 0) {
        return start;
    }
    {
        char *next = ov00_0221B9D0;
        *p = 0;
        ov00_0221B9D0 = next + 1;
    }
    return start;
}
