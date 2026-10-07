#pragma force_active on
#include "global.h"
int ov112_021EDCB0(int x);

// @fn ov112_021EDCB0
int ov112_021EDCB0(int x) {
    switch (x) {
    case 4:
        return 0x22;
    case 5:
        return 0x24;
    case 7:
        return 0x25;
    case 6:
        return 0x2B;
    case 8:
    case 9:
        return 0x26;
    case 3:
    case 11:
    case 12:
        return 0x27;
    case 13:
        return 0x23;
    case 14:
        return 0x2A;
    case 0x21:
    case 0x2A:
        break;
    }
    return x;
}
