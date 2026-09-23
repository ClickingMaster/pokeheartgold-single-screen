#include "to58_overlay112_mid_private.h"

int ov112_021ECEF8(Ov112State *p) {
    switch (ov112_021E9750(p)) {
    case 0:
        ov112_021EA570(p);
        if (ov112_021E9338(p) == 1) {
            W(p,12) = 0x14;
            return 0x12;
        }
        W(p,12) = 0x10;
        return 0x12;
    case 1:
        ov112_021EA570(p);
        return 7;
    default:
        return 0xf;
    }
}
