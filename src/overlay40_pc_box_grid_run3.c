#include "overlay40_pc_box_grid_private.h"

int ov40_022313F0(void *p) {
    s16 x;
    s16 y;
    int r = ov40_0223142C(p);
    VsRecorder_GetSpritePos(PTR(p, 0x5FC), &x, &y);
    OrbitCursor_SetTarget(PTR(p, 0x6F0), x + 0x10, y);
    return r;
}
