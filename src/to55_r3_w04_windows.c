#include "to55_r3_w04_private.h"

void ov112_021E9C10(Ov112Windows *p, int which) {
    if (ov112_021FEC80[which] == 0) {
        u16 i = 0;
        while (i < p->lowerCount) {
            RemoveWindow(&p->lower[i]);
            i++;
        }
        p->lowerState = 0xff;
    } else {
        u16 i = 0;
        while (i < p->upperCount) {
            RemoveWindow(&p->upper[i]);
            i++;
        }
        p->upperState = 0xff;
    }
}
