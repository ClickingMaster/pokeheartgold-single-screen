#pragma force_active on
#include "global.h"
#include "field_system.h"
extern const u8 ov02_02253A4C[];
BOOL ov02_0224E828(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);
BOOL ov02_0224EB48(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);
BOOL ov02_0224EE4C(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outA, s16 *outB, u8 state);
// @fn ov02_0224EE4C
BOOL ov02_0224EE4C(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outA, s16 *outB, u8 state) {
    s8 offs[2];
    int i;
    offs[0] = ov02_02253A4C[4];
    offs[1] = ov02_02253A4C[5];
    *outA = x;
    *outB = z;
    switch (direction) {
    case 0:
    case 1:
        if (ov02_0224E828(fieldSystem, x, z, direction, y, outA, outB, state)) {
            for (i = 0; i < 2; i++) {
                if (ov02_0224E828(fieldSystem, (s16)(x + offs[i]), z, direction, y, outA, outB, state)) {
                    *outA = x - i;
                    if (direction == 0) {
                        *outB = z - 1;
                    } else {
                        *outB = z + 2;
                    }
                    return TRUE;
                }
            }
        }
        break;
    case 2:
    case 3:
        if (ov02_0224EB48(fieldSystem, x, z, direction, y, outA, outB, state)) {
            for (i = 0; i < 2; i++) {
                if (ov02_0224EB48(fieldSystem, x, (s16)(z - offs[i]), direction, y, outA, outB, state)) {
                    if (direction == 2) {
                        *outA = x - 2;
                    } else {
                        *outA = x + 1;
                    }
                    *outB = z + i;
                    return TRUE;
                }
            }
        }
        break;
    }
    return FALSE;
}
