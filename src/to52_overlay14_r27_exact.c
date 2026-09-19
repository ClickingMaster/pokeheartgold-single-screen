#include "to52_overlay14_r27_exact_private.h"

void ov14_021ED35C(void *p) {
    if (U8_AT(p, 0x29) == 1) {
        ov14_021F0C58(p);
        return;
    }
    ov14_021E7F4C(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021F0234(p, ov14_021E9518, 0x33);
}

void ov14_021ED38C(void *p) {
    u32 a;
    u32 b;
    u32 next;
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        a = 6;
        b = 0x22;
        next = 0x75;
    } else {
        a = 3;
        b = 0x22;
        next = 0xc;
        if (U8_AT(p, 0x2a) != 0) {
            b = U8_AT(p, 0x2b);
            ov14_021E7588(p, b);
        }
    }
    ov14_021F6AC0(p, a, b);
    ov14_021F5C84(p, 0);
    ov14_021F5E94(p, 0);
    ov14_021F5EB4(p, 0);
    ov14_021E8248(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E82A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021F0234(p, ov14_021E94BC, next);
}

u32 ov14_021ED414(void *p) {
    u32 input = GridInputHandler_HandleInput_AllowHold(PTR_AT(PTR_AT(p, 0x34), 0x2c));
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 0);
    case 1:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 1);
    case 2:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 2);
    case 3:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 3);
    case 4:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 4);
    case 5:
        PlaySE(0x5dd);
        return ov14_021F1170(p, 5);
    case 6:
        PlaySE(0x5dc);
        return ov14_021F11F8(p, -1);
    case 7:
        PlaySE(0x5dc);
        return ov14_021F11F8(p, 1);
    case 8:
        ov14_021F29E4(PTR_AT(p, 0x34), 9, 8);
        if (U8_AT(p, 0x1f) == U8_AT(p, 0x25)) {
            PlaySE(0x5f3);
            return ov14_021F2270(p, 0xc, 0x3d);
        }
        PlaySE(0x5dc);
        return ov14_021F2270(p, 0xc, 0xa2);
    case 9:
        ov14_021F29E4(PTR_AT(p, 0x34), 9, 8);
        PlaySE(0x5dd);
        return ov14_021F2270(p, 6, 0xa3);
    case 10:
        ov14_021F29E4(PTR_AT(p, 0x34), 9, 8);
        PlaySE(0x5dd);
        return ov14_021F2270(p, 7, 0xa4);
    case -3:
        PlaySE(0x5dc);
        if (U32_AT(PTR_AT(p, 0x34), 8) != 0) {
            PlaySE(0x5dc);
            return ov14_021F0244(p, 0x3e);
        }
        break;
    case -4:
        PlaySE(0x5dc);
        break;
    case 11:
        ov14_021F29E4(PTR_AT(p, 0x34), 9, 8);
        /* fall through */
    case -2:
        PlaySE(0x5dc);
        return ov14_021F2270(p, 0xa, 0x39);
    }
    return 0x3d;
}

void ov14_021ED5B0(void *p) {
    void *inner = PTR_AT(p, 0x34);
    if (U32_AT(PTR_AT(p, 0), 8) == 1) {
        if (ov14_021E834C(PTR_AT(inner, 0x2f0)) == 0) {
            ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
    } else {
        ov14_021E8234(PTR_AT(inner, 0x2f0));
        ov14_021E8294(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        ov14_021E8314(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    ov14_021F0234(p, ov14_021E94BC, 0x35);
}
