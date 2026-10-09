#pragma force_active on
#include "global.h"

#include "field_system.h"
#include "field_metatile_query_internal.h"

extern const u8 ov02_02253A4C[];

BOOL ov02_0224EF6C(u8 behavior, BOOL collision, u32 state);
BOOL ov02_0224E828(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);

#define GRID_TO_FX32(n) ((fx32)(((n) * 16 + 8) > 0 ? (float)(((n) * 16 + 8) << 12) + 0.5f : (float)(((n) * 16 + 8) << 12) - 0.5f))

// @fn ov02_0224E828
BOOL ov02_0224E828(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state) {
    u8 query[3];
    u8 behavior;
    BOOL collision;
    unsigned int i;
    u8 count;
    u8 idx;
    s8 offset;
    int height;
    s8 *delta;
    s8 *q;

    q = (s8 *)query;
    q[1] = ov02_02253A4C[2];
    q[2] = ov02_02253A4C[3];
    *outX = x;
    *outZ = z;
    switch (direction) {
    case 0:
    case 1:
        count = 0;
        if (direction != 0) {
            idx = 1;
        } else {
            idx = 0;
        }
        offset = ((s8 *)&query[1])[idx];
        for (i = 0; (int)i < 2; i++) {
            s16 testZ = z + offset * (i + 1);
            behavior = GetMetatileBehavior(fieldSystem, x, testZ);
            collision = sub_020548C0(fieldSystem, x, testZ);
            if (ov02_0224EF6C(behavior, collision, state)) {
                height = sub_02054774(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(testZ), query);
                if (query[0] == 1 && height == y) {
                    count++;
                }
            }
        }
        if (count == 2) {
            *outX = x;
            if (direction == 0) {
                *outZ = z - 1;
            } else {
                *outZ = z + 2;
            }
            return TRUE;
        }
        break;
    case 2:
    case 3:
        x = (s16)(x + q[direction == 2 ? 1 : 2]);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        collision = sub_020548C0(fieldSystem, x, z);
        height = sub_02054774(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(z), query);
        if (!ov02_0224EF6C(behavior, collision, state) || query[0] != 1 || height != y) {
            return FALSE;
        }
        for (delta = (s8 *)&query[1], i = 0; (int)i < 2; delta++, i++) {
            s16 testZ2 = z + *delta;
            behavior = GetMetatileBehavior(fieldSystem, x, testZ2);
            collision = sub_020548C0(fieldSystem, x, testZ2);
            height = sub_02054774(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(testZ2), query);
            if (ov02_0224EF6C(behavior, collision, state) && query[0] == 1 && height == y) {
                *outX = x;
                *outZ = z + i;
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}
