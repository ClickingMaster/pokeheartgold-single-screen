#include "to56_overlay13_r17_run05_private.h"

int ov13_02224938(u8 *data, u32 expectedType, void *dst, int transform) {
    u32 type, length;
    u8 *payload = ov13_02224624(data, &type, &length);
    if (payload == NULL) {
        return 0;
    }
    if (type != expectedType) {
        return 0;
    }
    if (transform) {
        ov13_02225510(dst, payload, length, transform, 16);
        length -= 8;
    } else {
        memcpy(dst, payload, length);
    }
    return length;
}
