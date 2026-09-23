#include "global.h"

#pragma require_prototypes off
u16 ov45_0222AA5C(void *);
extern const u16 ov45_02254A84[][2];

int ov45_0222A9A4(void *p) {
    u16 v = ov45_0222AA5C(p);
    int i;
    for (i = 0; i < 16; i++) {
        if (v == ov45_02254A84[i][0]) {
            return i;
        }
    }
    return 0x18;
}
