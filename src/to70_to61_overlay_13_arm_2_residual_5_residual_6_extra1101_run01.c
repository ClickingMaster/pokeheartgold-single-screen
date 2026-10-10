#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[8];
    u8 text[32];
} Work;

extern Work *ov13_0224F540;
extern void ov13_0222B948(int *, int *);
extern void ov13_022277B8(int, int, int);
extern int ov13_02242074(const void *, int);
int ov13_0223191C(void);

int ov13_0223191C(void) {
    int a, b, len;
    ov13_0222B948(&a, &b);
    if (b == 1) {
        ov13_022277B8(0x35, -1, 0);
    }
    ov13_0222B948(&a, &b);
    if (a == 0) {
        return ov13_0224F540->text[0] != 0;
    }
    if (b == 1 && ov13_0224F540->text[0] == 0) {
        return 0;
    }
    len = ov13_02242074(ov13_0224F540->text, 32);
    switch (len) {
    case 0:
    case 5:
    case 13:
    case 16:
        return 1;
    case 10:
    case 26:
    case 32: {
        int i;
        for (i = 0; i < len; i++) {
            unsigned int c = ov13_0224F540->text[i];
            if (c >= 48 && c <= 57) {
                continue;
            }
            if (c >= 65 && c <= 70) {
                continue;
            }
            if (c >= 97 && c <= 102) {
                continue;
            }
            return 0;
        }
    }
        return 1;
    default:
        return 0;
    }
}
