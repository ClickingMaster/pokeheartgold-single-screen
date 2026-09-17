#include "to50_field_movement_terrain_check_internal.h"

BOOL sub_0205DCFC(PlayerAvatar *avatar, MapObject *mapObject, int direction) {
    void *fieldSystem;
    int x;
    int z;
    int dx;
    int dz;
    u32 behavior;

    if (direction != -1 && PlayerAvatar_GetState(avatar) == 1) {
        fieldSystem = MapObject_GetFieldSystem(mapObject);
        x = MapObject_GetXCoord(mapObject);
        dx = GetDeltaXByFacingDirection(direction);
        z = MapObject_GetZCoord(mapObject);
        dz = GetDeltaYByFacingDirection(direction);
        behavior = GetMetatileBehavior(fieldSystem, x + dx, z + dz);
        if (sub_02060E54(mapObject, behavior)) {
            return TRUE;
        }
        if (sub_02060EBC(mapObject, behavior)) {
            return TRUE;
        }
        if (MetatileBehavior_IsVeryTallGrass(behavior)) {
            return TRUE;
        }
        if (MetatileBehavior_IsMud(behavior)) {
            return TRUE;
        }
    }
    return FALSE;
}
