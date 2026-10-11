#include "to50_map_object_collision_probe_internal.h"

int sub_02064468(LocalMapObject *object, int direction, int distance) {
    int x;
    int y;
    int z;
    int i;

    if (distance == 0) {
        return TRUE;
    }
    x = MapObject_GetXCoord(object);
    z = MapObject_GetZCoord(object);
    y = MapObject_GetYCoord(object);
    x += GetDeltaXByFacingDirection(direction);
    z += GetDeltaYByFacingDirection(direction);
    for (i = 0; i < distance - 1; i++) {
        if ((MapObject_CheckCollisionAt(object, x, y, z, direction) & ~1) != 0) {
            return TRUE;
        }
        x += GetDeltaXByFacingDirection(direction);
        z += GetDeltaYByFacingDirection(direction);
    }
    if ((MapObject_CheckCollisionAt(object, x, y, z, direction) & ~1) == 4) {
        return FALSE;
    }
    return TRUE;
}
