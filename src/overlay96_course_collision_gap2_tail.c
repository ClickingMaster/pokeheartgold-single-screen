#include "overlay96_course_collision_gap2_private.h"

u8 ov96_02207300(u8 *work, int excluded, int radius, const VecFx32 *position, const void *arg4, VecFx32 *out) {
    u8 count;
    u8 result;
    int i;
    VecFx32 localOut;
    void *candidates[20];
    int points[24];

    ov96_02206F1C(work, position->x, position->y, candidates, &count);
    i = 0;
    result = 0;
    for (; i < count; i++) {
        int code;
        ov96_02206FA4(radius, candidates[i], points);
        code = ov96_0220703C(position, arg4, radius, (const Ov96PointW17 *)points, &localOut);
        if (code != 0) {
            if (code >= 9) {
                out->x = localOut.x;
                out->y = localOut.y;
                out->z = 0;
            } else if (code != excluded) {
                out->x = localOut.x;
                out->y = localOut.y;
                out->z = 0;
            }
            result = (u8)code;
            break;
        }
    }
    return result;
}
