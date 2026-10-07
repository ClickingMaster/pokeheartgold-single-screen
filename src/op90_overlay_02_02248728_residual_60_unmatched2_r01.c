#pragma force_active on
#include "global.h"

typedef struct Mask5 {
    u8 bytes[5];
} Mask5;

extern const Mask5 ov02_02253A54;
BOOL ov02_02250738(int index, u8 mask);

// @fn ov02_02250738
BOOL ov02_02250738(int index, u8 mask) {
    Mask5 masks = ov02_02253A54;
    int i;
    for (i = 0; i < 5; i++) {
        if ((masks.bytes[index - 1] & mask) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
