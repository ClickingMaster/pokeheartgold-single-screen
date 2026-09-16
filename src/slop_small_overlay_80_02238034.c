#include "global.h"

int ov80_02238344(int type);

int ov80_02238344(int type) {
    switch (type) {
    case 0:
        return 0x81;
    case 1:
        return 0x83;
    case 2:
        return 0x8F;
    case 3:
        return 0x8F;
    default:
        return 0x81;
    }
}
