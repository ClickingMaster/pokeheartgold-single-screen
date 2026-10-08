#pragma force_active on
#include "global.h"

#include "field_system.h"
#include "field_metatile_query_internal.h"

extern const u8 ov02_02253A4C[];

BOOL ov02_0224EF6C(u8 behavior, BOOL collision, u32 state);
BOOL ov02_0224EB48(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);

#define GRID_TO_FX32(n) ((fx32)(((n) * 16 + 8) > 0 ? (float)(((n) * 16 + 8) << 12) + 0.5f : (float)(((n) * 16 + 8) << 12) - 0.5f))

// @fn ov02_0224EB48
BOOL ov02_0224EB48(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state) {
    u8 query[3];
    u8 behavior;
    BOOL collision;
    int i;
    u8 count;
    s16 start;
    int height;
    s8 *delta;
    s8 *q;

    q = (s8 *)query;
    q[1] = ov02_02253A4C[0];
    q[2] = ov02_02253A4C[1];
    *outX = x;
    *outZ = z;
    switch (direction) {
    case 0:
    case 1:
        z = (s16)(z + q[direction == 0 ? 1 : 2]);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        collision = sub_020548C0(fieldSystem, x, z);
        height = sub_02054774(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(z), query);
        if (!ov02_0224EF6C(behavior, collision, state) || query[0] != 1 || height != y) {
            return FALSE;
        }
        for (delta = (s8 *)&query[1], i = 0; i < 2; delta++, i++) {
            s16 testX = x - *delta;
            behavior = GetMetatileBehavior(fieldSystem, testX, z);
            collision = sub_020548C0(fieldSystem, testX, z);
            height = sub_02054774(fieldSystem, y, GRID_TO_FX32(testX), GRID_TO_FX32(z), query);
            if (ov02_0224EF6C(behavior, collision, state) && query[0] == 1 && height == y) {
                *outX = x - i;
                *outZ = z;
                return TRUE;
            }
        }
        break;
    case 2:
    case 3:
        count = 0;
        if (direction == 2) {
            start = x - 2;
        } else {
            start = x + 1;
        }
        for (i = 0; i < 2; i++) {
            behavior = GetMetatileBehavior(fieldSystem, (s16)(start + i), z);
            collision = sub_020548C0(fieldSystem, (s16)(start + i), z);
            if (ov02_0224EF6C(behavior, collision, state)) {
                height = sub_02054774(fieldSystem, y, GRID_TO_FX32((s16)(start + i)), GRID_TO_FX32(z), query);
                if (query[0] == 1 && height == y) {
                    count++;
                }
            }
        }
        if (count == 2) {
            *outX = start;
            *outZ = z;
            return TRUE;
        }
        break;
    }
    return FALSE;
}
