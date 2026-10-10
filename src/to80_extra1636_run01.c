#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern signed char ov38_02224F87[];
extern int ov38_02224DC8(void *, void *, void *, int);
extern int ov38_02221CA4(u8);
int ov38_02221CD8(char *, void *, void *);

int ov38_02221CD8(char *p, void *a, void *b) {
    int value;
    signed char *key;
    char *position;
    int index;
    if (ov38_02224DC8(a, b, p + 0x46, 18) < 0) {
        return 1;
    }
    key = ov38_02224F87;
    for (index = 19; index >= 2; index--, key--) {
        position = p + index;
        value = (signed char)position[0x44];
        for (;;) {
            value = ov38_02221CA4((u8)value);
            position[0x44] = value;
            if (value == *key) {
                break;
            }
            if (ov38_02224DC8(a, b, p + 0x46, 18) < 0) {
                return 1;
            }
        }
    }
    return 0;
}
