#include "to53_overlay48_mode_helpers_private.h"

void ov48_02258F0C(void *p) {
    if (S32AT(p, 0) >= 0) {
        S32AT(p, 0) = S32AT(p, 0) % 0xFFFF;
    } else {
        S32AT(p, 0) = S32AT(p, 0) + ((S32AT(p, 0) >= 0 ? S32AT(p, 0) : -S32AT(p, 0)) / 0xFFFF + 1) * 0xFFFF;
    }
    if (S32AT(p, 4) >= 0) {
        S32AT(p, 4) = S32AT(p, 4) % 0xFFFF;
    } else {
        S32AT(p, 4) = S32AT(p, 4) + ((S32AT(p, 4) >= 0 ? S32AT(p, 4) : -S32AT(p, 4)) / 0xFFFF + 1) * 0xFFFF;
    }
}

u32 ov48_02258F64(void *p) {
    u32 r;

    switch (U16AT(p, 0)) {
    case 0:
        switch (ov48_0225909C(p)) {
        case 0:
            break;
        case 1:
            ov48_02259C4C(ADD(p, 0xC3CC));
            U16AT(p, 0) = 1;
            break;
        case 2:
            U16AT(p, 0) = 2;
            break;
        }
        break;
    case 1:
        if (ov48_02259C78(ADD(p, 0xC3CC), ADD(p, 0x224)) == 1) {
            U16AT(p, 0) = 0;
        }
        break;
    case 2:
        ov48_02259F48(AT_C700(p));
        ov48_0225A1D0(ADD(p, 0xC40C));
        ov48_02259E78(ADD(p, 0xC3E0));
        U16AT(p, 0)
        ++;
        break;
    case 3:
        r = ov48_02259F8C(AT_C700(p));
        if (r != 0) {
            if (r == 1) {
                goto return_one;
            }
            if (r != 2) {
                goto return_one;
            }
            ov48_0225A1EC(ADD(p, 0xC40C));
            ov48_02259FEC(ADD(p, 0xC6FC + 4));
            ov48_02259E90(ADD(p, 0xC3E0));
            U16AT(p, 0) = 0;
        }
        break;
return_one:
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
