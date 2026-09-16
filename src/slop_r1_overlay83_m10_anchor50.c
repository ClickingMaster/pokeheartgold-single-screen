#include "slop_r1_overlay83_m10_private.h"

TouchscreenListMenu *ov83_02247CCC(TouchscreenListMenuSpawner *spawner, TouchscreenListMenuHeader *header, u8 x, u8 y, u8 width) {
    return TouchscreenListMenu_Create(spawner, header, 0, x, y, width, 0);
}
