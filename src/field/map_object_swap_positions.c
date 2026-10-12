#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/movements.h"

#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "overlay_02.h"
#include "task.h"

// Two map objects next to each other step into each other's place (the player and the following Pokemon).
typedef struct SwapPositionsEnv {
    u32 state;
    LocalMapObject *object1;
    LocalMapObject *object2;
    s32 object1X;
    s32 object1Z;
    s32 object1Direction;
    s32 object2X;
    s32 object2Z;
} SwapPositionsEnv;

SwapPositionsEnv *SwapPositions_CreateTaskEnv(LocalMapObject *object1, LocalMapObject *object2);
BOOL Task_SwapMapObjectPositions(TaskManager *taskManager);
u32 SwapPositions_GetStepTowards(const s32 *object1Position, const s32 *object2Position);
u32 SwapPositions_GetFaceMovement(u8 direction);
u32 SwapPositions_GetTurnAroundMovement(u8 direction);
u32 SwapPositions_GetOppositeStep(u32 movement);

void MapObjects_SwapPositions(LocalMapObject *object1, LocalMapObject *object2, TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_SwapMapObjectPositions, SwapPositions_CreateTaskEnv(object1, object2));
}

SwapPositionsEnv *SwapPositions_CreateTaskEnv(LocalMapObject *object1, LocalMapObject *object2) {
    SwapPositionsEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(SwapPositionsEnv));
    env->state = 0;
    env->object1 = object1;
    env->object2 = object2;
    return env;
}

BOOL Task_SwapMapObjectPositions(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    SwapPositionsEnv *env = TaskManager_GetEnvironment(taskManager);
    BOOL done = FALSE;

    switch (env->state) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        env->state++;
    case 1:
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object1)) {
            break;
        }
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object2)) {
            break;
        }
        MapObject_PauseMovement(FollowMon_GetMapObject(fieldSystem));
        env->state++;
        break;
    case 2: {
        u32 movement;
        env->object1X = MapObject_GetXCoord(env->object1);
        env->object1Z = MapObject_GetZCoord(env->object1);
        env->object1Direction = MapObject_GetFacingDirection(env->object1);
        env->object2X = MapObject_GetXCoord(env->object2);
        env->object2Z = MapObject_GetZCoord(env->object2);
        movement = SwapPositions_GetStepTowards(&env->object1X, &env->object2X);
        MapObject_SetHeldMovement(env->object1, movement);
        MapObject_SetHeldMovement(env->object2, SwapPositions_GetOppositeStep(movement));
        env->state++;
        break;
    }
    case 3:
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object1)) {
            break;
        }
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object2)) {
            break;
        }
        env->state++;
        break;
    case 4:
        MapObject_SetHeldMovement(env->object1, SwapPositions_GetTurnAroundMovement(MapObject_GetFacingDirection(env->object1)));
        MapObject_SetHeldMovement(env->object2, SwapPositions_GetFaceMovement(env->object1Direction));
        env->state++;
        break;
    case 5:
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object1)) {
            break;
        }
        if (!MapObject_AreBitsSetForMovementScriptInit(env->object2)) {
            break;
        }
        MapObject_ClearHeldMovementIfActive(env->object1);
        MapObject_ClearHeldMovementIfActive(env->object2);
        done = TRUE;
        Heap_Free(env);
        break;
    }

    return done;
}

// Positions are {x, z}; the objects must be next to each other in a straight line.
u32 SwapPositions_GetStepTowards(const s32 *object1Position, const s32 *object2Position) {
    u32 movement = MOVEMENT_STEP_DOWN;

    if (object1Position[0] == object2Position[0]) {
        if (object1Position[1] > object2Position[1]) {
            movement = MOVEMENT_STEP_UP;
        } else if (object1Position[1] >= object2Position[1]) {
            GF_AssertFail();
        }
    } else if (object1Position[1] == object2Position[1]) {
        if (object1Position[0] > object2Position[0]) {
            movement = MOVEMENT_STEP_LEFT;
        } else if (object1Position[0] < object2Position[0]) {
            movement = MOVEMENT_STEP_RIGHT;
        } else {
            GF_AssertFail();
        }
    } else {
        GF_AssertFail();
    }
    return movement;
}

u32 SwapPositions_GetFaceMovement(u8 direction) {
    switch (direction) {
    case DIR_NORTH:
        return MOVEMENT_FACE_UP;
    case DIR_SOUTH:
        return MOVEMENT_FACE_DOWN;
    case DIR_WEST:
        return MOVEMENT_FACE_LEFT;
    case DIR_EAST:
        return MOVEMENT_FACE_RIGHT;
    default:
        GF_AssertFail();
        return MOVEMENT_FACE_UP;
    }
}

u32 SwapPositions_GetTurnAroundMovement(u8 direction) {
    switch (direction) {
    case DIR_NORTH:
        return MOVEMENT_FACE_DOWN;
    case DIR_SOUTH:
        return MOVEMENT_FACE_UP;
    case DIR_WEST:
        return MOVEMENT_FACE_RIGHT;
    case DIR_EAST:
        return MOVEMENT_FACE_LEFT;
    default:
        GF_AssertFail();
        return MOVEMENT_FACE_UP;
    }
}

u32 SwapPositions_GetOppositeStep(u32 movement) {
    switch (movement) {
    case MOVEMENT_STEP_UP:
        return MOVEMENT_STEP_DOWN;
    case MOVEMENT_STEP_DOWN:
        return MOVEMENT_STEP_UP;
    case MOVEMENT_STEP_LEFT:
        return MOVEMENT_STEP_RIGHT;
    case MOVEMENT_STEP_RIGHT:
        return MOVEMENT_STEP_LEFT;
    default:
        GF_AssertFail();
        return MOVEMENT_FACE_UP;
    }
}
