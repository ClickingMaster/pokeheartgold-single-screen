#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *window; u32 *flags; u8 text[12]; u8 count, state; u8 tail[2]; } Numeric24;
typedef struct { u8 b[4]; } Bytes4;
extern Numeric24 *ov13_0224F544;
extern const Bytes4 ov13_02245480;
extern void ov13_0223AA58(const char *, u8 *);
extern void ov13_0222B948(int *, int);
extern int DWC_BACKUPlCheckAddress(void *);
int ov13_0223261C(void);
// @fn ov13_0223261C
int ov13_0223261C(void) {
    Bytes4 limit = ov13_02245480;
    int i;
    int mode;
    u8 address[4];
    u8 *p;
    int j;
    int zero;
    for (i = 0; i < 4; i++) {
        p = &ov13_0224F544->text[i * 3];
        if (p[0] == ' ') {
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (p[j] > limit.b[j]) {
                return 0;
            }
            if (p[j] < limit.b[j]) {
                break;
            }
        }
    }
    ov13_0223AA58((const char *)ov13_0224F544->text, address);
    ov13_0222B948(&mode, 0);
    if (mode == 1) {
        zero = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 8; j++) {
                if (zero) {
                    if (address[i] & (1 << (7 - j))) {
                        return 0;
                    }
                } else if (!(address[i] & (1 << (7 - j)))) {
                    zero = 1;
                }
            }
        }
        return 1;
    }
    return DWC_BACKUPlCheckAddress(address) != 0;
}
