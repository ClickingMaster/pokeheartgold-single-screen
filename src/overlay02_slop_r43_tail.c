#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    u8 *p = data;
    for (i = 0; i < 2; i++) {
        ov02_0224D1DC((Field3dObject *)p);
        p += 0xdc;
    }
}
