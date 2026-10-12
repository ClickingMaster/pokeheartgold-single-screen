#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_movement_utilities_internal.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "field_terrain.h"

// Defined elsewhere without a header yet.
int sub_020548EC(FieldSystem *fieldSystem, int x, int z); // defined as u8; the caller truncates the result itself
int FieldSystem_GetTerrainHeight(FieldSystem *fieldSystem, int y, int x, int z, u8 *floorType);
void MapObject_GetPreviousCoordsBehind(LocalMapObject *object, int *x, int *z);

BOOL sub_02060BFC(LocalMapObject *self, int x, int y, int z) {
    MapObjectManager *manager = MapObject_GetManager(self);
    LocalMapObject *object = MapObjectManager_GetObjects2(manager);
    int count = MapObjectManager_GetObjectCount(manager);

    do {
        if (object != self && MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_ACTIVE) != 0 && MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK18) == 0) {
            int objectX = MapObject_GetXCoord(object);
            int objectZ = MapObject_GetZCoord(object);
            if (objectX == x && objectZ == z) {
                int delta = MapObject_GetYCoord(object) - y;
                if (delta < 0) {
                    delta = -delta;
                }
                if (delta < 2) {
                    return TRUE;
                }
            }
            objectX = MapObject_GetPreviousXCoord(object);
            objectZ = MapObject_GetPreviousZCoord(object);
            if (objectX == x && objectZ == z) {
                int delta = MapObject_GetYCoord(object) - y;
                if (delta < 0) {
                    delta = -delta;
                }
                if (delta < 2) {
                    return TRUE;
                }
            }
        }
        MapObjectArray_NextObject(&object);
        count--;
    } while (count != 0);
    return FALSE;
}

BOOL sub_02060CA8(LocalMapObject *self, int x, int y, int z) {
    int objectX;
    int objectZ;
    MapObjectManager *manager = MapObject_GetManager(self);
    LocalMapObject *object = MapObjectManager_GetObjects2(manager);
    int count = MapObjectManager_GetObjectCount(manager);

    do {
        if (object != self && MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_ACTIVE) != 0) {
            objectX = MapObject_GetXCoord(object);
            objectZ = MapObject_GetZCoord(object);
            if (objectX == x && objectZ == z) {
                int delta = MapObject_GetYCoord(object) - y;
                if (delta < 0) {
                    delta = -delta;
                }
                if (delta < 2) {
                    return TRUE;
                }
            }
            objectX = MapObject_GetPreviousXCoord(object);
            objectZ = MapObject_GetPreviousZCoord(object);
            if (objectX == x && objectZ == z) {
                int delta = MapObject_GetYCoord(object) - y;
                if (delta < 0) {
                    delta = -delta;
                }
                if (delta < 2) {
                    return TRUE;
                }
            }
            if (MapObject_GetID(object) == 0xFD && FollowMonObject_IsLarge(object) != 0 && MapObject_CheckVisible(object) == FALSE) {
                MapObject_GetPreviousCoordsBehind(object, &objectX, &objectZ);
                if (objectX == x && objectZ == z) {
                    int delta = MapObject_GetYCoord(object) - y;
                    if (delta < 0) {
                        delta = -delta;
                    }
                    if (delta < 2) {
                        return TRUE;
                    }
                }
            }
        }
        MapObjectArray_NextObject(&object);
        count--;
    } while (count != 0);
    return FALSE;
}

BOOL MapObject_IsOutsideMovementRange(LocalMapObject *object, int x, int y, int z) {
    int initialX = MapObject_GetInitialX(object);
    int rangeX = MapObject_GetXRange(object);
    if (rangeX != -1) {
        int maxX = initialX + rangeX;
        int minX = initialX - rangeX;
        if (minX > x || maxX < x) {
            return TRUE;
        }
    }
    {
        int initialZ = MapObject_GetInitialZ(object);
        int rangeZ = MapObject_GetYRange(object);
        if (rangeZ != -1) {
            int maxZ = initialZ + rangeZ;
            int minZ = initialZ - rangeZ;
            if (minZ > z || maxZ < z) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL MapObject_IsMoveBlockedByBehavior(LocalMapObject *object, int x, int z, int direction) {
    if (sub_0205F8D0(object) == FALSE) {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
        u8 objectBehavior = sub_0205F504(object);
        u8 behavior = GetMetatileBehavior(fieldSystem, x, z);
        if (behavior == GetMetatileBehavior_None()) {
            return TRUE;
        }
        if (gMetatileBlocksExitTests[direction](objectBehavior) == TRUE || gMetatileBlocksEntryTests[direction](behavior) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_02060E54(LocalMapObject *object, int behavior) {
    if (sub_0205BA54((u8)behavior)) {
        if (!MapObject_CheckFlag28(object)) {
            return 1;
        }
    } else if (MetatileBehavior_IsSurfableWater((u8)behavior)) {
        return 1;
    } else if (sub_0205B78C((u8)behavior)) {
        u32 sprite = MapObject_GetSpriteID(object) - 0xB2;
        if (sprite <= 1) {
            return 1;
        }
    }
    return 0;
}

BOOL sub_02060EA4(LocalMapObject *object, int behavior) {
    if (MetatileBehavior_IsSand(behavior)) {
        return 1;
    }
    return 0;
}

BOOL sub_02060EBC(LocalMapObject *object, int behavior) {
    if (MetatileBehavior_IsSnow(behavior)) {
        return 1;
    }
    return 0;
}

BOOL sub_02060ED4(LocalMapObject *object, int behavior) {
    if (sub_0205B8C4(behavior)) {
        return 1;
    }
    return 0;
}

BOOL sub_02060EEC(LocalMapObject *object, int behavior) {
    if (MapObject_CheckFlag28(object) == 1 && sub_0205BA30((u8)behavior) == 1) {
        return 1;
    }
    return 0;
}

int GetDeltaXByFacingDirection(int direction) {
    return gDirectionDeltaX[direction];
}

int GetDeltaYByFacingDirection(int direction) {
    return gDirectionDeltaY[direction];
}

void MapObject_MoveCoordsInDirection(LocalMapObject *object, int direction) {
    MapObject_SetPreviousX(object, MapObject_GetXCoord(object));
    MapObject_SetPreviousY(object, MapObject_GetYCoord(object));
    MapObject_SetPreviousZ(object, MapObject_GetZCoord(object));
    MapObject_AddCurrentX(object, GetDeltaXByFacingDirection(direction));
    MapObject_AddCurrentY(object, 0);
    MapObject_AddCurrentZ(object, GetDeltaYByFacingDirection(direction));
}

void MapObject_SetPreviousCoordsToCurrent(LocalMapObject *object) {
    MapObject_SetPreviousX(object, MapObject_GetXCoord(object));
    MapObject_SetPreviousY(object, MapObject_GetYCoord(object));
    MapObject_SetPreviousZ(object, MapObject_GetZCoord(object));
}

int MapObject_GetMetatileBehaviorInDirection(LocalMapObject *object, int direction) {
    int x = MapObject_GetXCoord(object) + GetDeltaXByFacingDirection(direction);
    int z = MapObject_GetZCoord(object) + GetDeltaYByFacingDirection(direction);
    return GetMetatileBehavior(MapObject_GetFieldSystem(object), x, z);
}

u8 sub_02060FE0(LocalMapObject *object, int direction) {
    int x = MapObject_GetXCoord(object) + GetDeltaXByFacingDirection(direction);
    int z = MapObject_GetZCoord(object) + GetDeltaYByFacingDirection(direction);
    return sub_020548EC(MapObject_GetFieldSystem(object), x, z);
}

void MapObject_OffsetPositionInDirection(LocalMapObject *object, int direction, int distance) {
    VecFx32 position;
    MapObject_CopyPositionVector(object, &position);
    switch (direction) {
    case 0:
        position.z -= distance;
        break;
    case 1:
        position.z += distance;
        break;
    case 2:
        position.x -= distance;
        break;
    case 3:
        position.x += distance;
        break;
    }
    MapObject_SetPositionVector(object, &position);
}

BOOL MapObject_UpdateHeight(LocalMapObject *object) {
    VecFx32 position;
    VecFx32 adjusted;
    int result;

    MapObject_CopyPositionVector(object, &position);
    adjusted = position;
    if (MapObject_CheckIgnoreHeights(object) == TRUE) {
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK12);
        return FALSE;
    }
    result = MapObject_CheckFlag29(object);
    result = sub_02061248(MapObject_GetFieldSystem(object), &adjusted, result);
    if (result == TRUE) {
        position.y = adjusted.y;
        MapObject_SetPositionVector(object, &position);
        MapObject_SetPreviousY(object, MapObject_GetYCoord(object));
        MapObject_SetCurrentY(object, (position.y >> 3) / FX32_ONE);
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK12);
    } else {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK12);
    }
    return result;
}

BOOL MapObject_UpdateMetatileBehaviors(LocalMapObject *object) {
    u8 previous = GetMetatileBehavior_None();
    u8 current = previous;
    if (!sub_0205F8D0(object)) {
        FieldSystem *fieldSystem;
        int x;
        int z;
        x = MapObject_GetPreviousXCoord(object);
        z = MapObject_GetPreviousZCoord(object);
        fieldSystem = MapObject_GetFieldSystem(object);
        previous = GetMetatileBehavior(fieldSystem, x, z);
        x = MapObject_GetXCoord(object);
        z = MapObject_GetZCoord(object);
        current = GetMetatileBehavior(fieldSystem, x, z);
    }
    sub_0205F50C(object, previous);
    sub_0205F4FC(object, current);
    if (MetatileBehavior_IsNone(current) == 1) {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK11);
        return 0;
    }
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK11);
    return 1;
}

void OffsetPositionInDirection(int direction, VecFx32 *position, int distance) {
    switch (direction) {
    case 0:
        position->z -= distance;
        break;
    case 1:
        position->z += distance;
        break;
    case 2:
        position->x -= distance;
        break;
    case 3:
        position->x += distance;
        break;
    }
}

void SetPositionToTileCenter(int x, int z, VecFx32 *position) {
    position->x = (x << 16) + 0x8000;
    position->z = (z << 16) + 0x8000;
}

void sub_020611DC(LocalMapObject *object) {
    u32 movement = MapObject_GetMovement(object) - 0x33;
    if (movement <= 3) {
        sub_0205F430(object);
    }
}

u32 GetOppositeDirection(u32 direction) {
    return gOppositeDirections[direction];
}

int GetDirectionTowards(int x, int z, int targetX, int targetZ) {
    if (x > targetX) {
        return 2;
    }
    if (x < targetX) {
        return 3;
    }
    if (z > targetZ) {
        return 0;
    }
    return 1;
}

BOOL sub_0206121C(FieldSystem *fieldSystem, VecFx32 *position) {
    u8 behavior;
    int y = FieldSystem_GetTerrainHeight(fieldSystem, position->y, position->x, position->z, &behavior);
    if (behavior == 0) {
        return 0;
    }
    position->y = y;
    return 1;
}

BOOL sub_02061248(FieldSystem *fieldSystem, VecFx32 *position, BOOL allowType2) {
    u8 behavior;
    int y = FieldSystem_GetTerrainHeight(fieldSystem, position->y, position->x, position->z, &behavior);
    if (behavior == 0) {
        return 0;
    }
    if (behavior == 2 && allowType2 == 0) {
        return 0;
    }
    position->y = y;
    return 1;
}
