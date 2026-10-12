#include "global.h"

#include "field/field_follow_mon.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_follow_internal.h"
#include "metatile_behavior.h"
#include "unk_02054648.h"

// 3 for the step movements, 2 for the run movements, 1 for the fast run movements.
int GetMovementSpeed(u32 movement) {
    MovementList step = gStepMovements;
    MovementList run = gRunMovements;
    MovementList fastRun = gFastRunMovements;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (movement == step.movements[i]) {
            return 3;
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == run.movements[i]) {
            return 2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == fastRun.movements[i]) {
            return 1;
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

void FollowMon_SpawnGrassEffects(LocalMapObject *object) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    u32 behavior = GetMetatileBehavior(fieldSystem, x, z);

    if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
        TallGrass_Spawn(object, 0);
    } else if (MetatileBehavior_IsVeryTallGrass(behavior) == TRUE) {
        VeryTallGrass_Spawn(object, 0);
    }

    if (FollowMonObject_IsLarge(object) != FALSE) {
        u8 direction = MapObject_GetFacingDirection(object);
        if (direction == DIR_WEST || direction == DIR_EAST) {
            MapObject_GetCoordsBehind(object, &x, &z);
            behavior = GetMetatileBehavior(fieldSystem, x, z);
            if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
                TallGrass_SpawnAt(object, 1, x, z, 1);
                return;
            }
            if (MetatileBehavior_IsVeryTallGrass(behavior) == TRUE) {
                VeryTallGrass_SpawnAt(object, 1, x, z, 1);
            }
        }
    }
}
