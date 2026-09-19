#include "overlay96_course_collision_gap2_private.h"

void ov96_02206FA4(int radius, const u16 *center, int *raw) {
    Ov96PointW17 *out = (Ov96PointW17 *)raw;
    int x = center[0];
    int y = center[1];

    out[0].x = x - 16;
    out[0].y = y - 16;
    out[1].x = x + 16;
    out[1].y = y - 16;
    out[2].x = x + 16;
    out[2].y = y + 16;
    out[3].x = x - 16;
    out[3].y = y + 16;
    out[4] = out[0];
    out[5] = out[1];
    out[6] = out[1];
    out[7] = out[2];
    out[8] = out[2];
    out[9] = out[3];
    out[10] = out[3];
    out[11] = out[0];
    out[4].y -= radius;
    out[5].y -= radius;
    out[6].x += radius;
    out[7].x += radius;
    out[8].y += radius;
    out[9].y += radius;
    out[10].x -= radius;
    out[11].x -= radius;
}
