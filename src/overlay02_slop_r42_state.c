#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D3B4(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj) {
    switch (*(int *)((u8 *)obj + 0xec)) {
    case 0:
        if (ov02_0224D178(obj) == 1) {
            Field3dObject_SetActiveFlag(obj, 0);
            (*(int *)((u8 *)obj + 0xec))++;
        }
        break;
    case 1:
        break;
    }
}
