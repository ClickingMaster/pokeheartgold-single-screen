#include "field/spin_slide.h"

#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_04.h"
#include "sound_playback.h"

// player avatar movement helpers, not declared by a header yet
BOOL sub_0205DFC8(PlayerAvatar *avatar);
void PlayerAvatar_SetMovement(PlayerAvatar *avatar, int movement);
int GetMovementForDirection(int direction, int movement);
BOOL PlayerAvatar_CheckCollision(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);

BOOL PlayerAvatar_TryStartSpinSlide(FieldSystem *fieldSystem, PlayerAvatar *avatar, int facingDirection) {
    u8 behavior = sub_0205F504(PlayerAvatar_GetMapObject(avatar));
    int direction;

    if (MetatileBehavior_IsSlideEast(behavior) == TRUE) {
        direction = DIR_EAST;
    } else if (MetatileBehavior_IsSlideWest(behavior) == TRUE) {
        direction = DIR_WEST;
    } else if (MetatileBehavior_IsSlideNorth(behavior) == TRUE) {
        direction = DIR_NORTH;
    } else if (MetatileBehavior_IsSlideSouth(behavior) == TRUE) {
        direction = DIR_SOUTH;
    } else {
        return FALSE;
    }
    SpinSlide_Start(fieldSystem, avatar, direction);
    return TRUE;
}

void SpinSlide_Start(FieldSystem *fieldSystem, PlayerAvatar *avatar, int direction) {
    SpinSlideTask *task = SpinSlide_AllocZeroed(sizeof(SpinSlideTask));

    task->fieldSystem = fieldSystem;
    task->avatar = avatar;
    task->direction = direction;
    PlaySE(SEQ_SE_DP_F209);
    FieldSystem_CreateTask(fieldSystem, Task_SpinSlide, task);
}

// A quarter turn counterclockwise.
int SpinSlide_RotateDirection(int direction) {
    switch (direction) {
    case DIR_NORTH:
        return DIR_WEST;
    case DIR_WEST:
        return DIR_SOUTH;
    case DIR_SOUTH:
        return DIR_EAST;
    case DIR_EAST:
        return DIR_NORTH;
    default:
        return DIR_NORTH;
    }
}

BOOL Task_SpinSlide(TaskManager *taskManager) {
    SpinSlideTask *task = TaskManager_GetEnvironment(taskManager);
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(task->avatar);
    u8 behavior = sub_0205F504(mapObject);

    switch (task->state) {
    case 0:
        MapObject_SetFlagsBits(mapObject, MAPOBJECTFLAG_UNK8);
        task->state++;
        break;
    case 1:
        if (sub_0205DFC8(task->avatar) == FALSE) {
            break;
        }
        PlayerAvatar_SetMovement(task->avatar, GetMovementForDirection(task->direction, 12));
        PlayerAvatar_SetFacingDirection(task->avatar, task->direction);
        task->state++;
        task->countdown = 7;
        break;
    case 2:
        if (task->countdown == 2 || task->countdown == 4 || task->countdown == 6) {
            task->direction = SpinSlide_RotateDirection(task->direction);
            PlayerAvatar_SetFacingDirection(task->avatar, task->direction);
        }
        task->countdown--;
        if (task->countdown != 0) {
            break;
        }
        if (MetatileBehavior_IsSlideEast(behavior) == TRUE) {
            task->direction = DIR_EAST;
            ViridianGymmick_HandleTileAction(task->fieldSystem, behavior);
        } else if (MetatileBehavior_IsSlideWest(behavior) == TRUE) {
            task->direction = DIR_WEST;
            ViridianGymmick_HandleTileAction(task->fieldSystem, behavior);
        } else if (MetatileBehavior_IsSlideNorth(behavior) == TRUE) {
            task->direction = DIR_NORTH;
            ViridianGymmick_HandleTileAction(task->fieldSystem, behavior);
        } else if (MetatileBehavior_IsSlideSouth(behavior) == TRUE) {
            task->direction = DIR_SOUTH;
            ViridianGymmick_HandleTileAction(task->fieldSystem, behavior);
        } else if (MetatileBehavior_StopSliding(behavior) == TRUE) {
            task->direction = SpinSlide_RotateDirection(task->direction);
            MapObject_ClearFlagsBits(mapObject, MAPOBJECTFLAG_UNK7);
            MapObject_ClearFlagsBits(mapObject, MAPOBJECTFLAG_UNK8);
            PlayerAvatar_SetFacingDirection(task->avatar, task->direction);
            SpinSlide_Free(task);
            StopSE(SEQ_SE_DP_F209, 0);
            return TRUE;
        } else {
            task->direction = SpinSlide_RotateDirection(task->direction);
        }
        if (PlayerAvatar_CheckCollision(task->avatar, mapObject, task->direction) == FALSE) {
            task->state = 1;
            break;
        }
        MapObject_ClearFlagsBits(mapObject, MAPOBJECTFLAG_UNK7);
        MapObject_ClearFlagsBits(mapObject, MAPOBJECTFLAG_UNK8);
        PlayerAvatar_SetFacingDirection(task->avatar, task->direction);
        SpinSlide_Free(task);
        StopSE(SEQ_SE_DP_F209, 0);
        return TRUE;
    }
    return FALSE;
}

void *SpinSlide_AllocZeroed(u32 size) {
    void *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);

    GF_ASSERT(ptr != NULL);
    memset(ptr, 0, size);
    return ptr;
}

void SpinSlide_Free(void *ptr) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, ptr);
}
