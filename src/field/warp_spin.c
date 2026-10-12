#include "field/warp_spin.h"

#include "global.h"

#include "constants/sndseq.h"

#include "field/map_object_models.h"

#include "heap.h"
#include "map_object.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sys_task_api.h"

#define WARP_SPIN_FRAMES 20

// The height of the spin after n frames.
#define WARP_SPIN_HEIGHT(n) ((fx32)((double)(n) * ((double)((n) << 11) + 9011.2)))

// A quarter turn counterclockwise.
void WarpSpin_RotatePlayer(WarpSpinTask *task) {
    switch (task->facing) {
    case DIR_NORTH:
        task->facing = DIR_WEST;
        break;
    case DIR_WEST:
        task->facing = DIR_SOUTH;
        break;
    case DIR_SOUTH:
        task->facing = DIR_EAST;
        break;
    case DIR_EAST:
        task->facing = DIR_NORTH;
        break;
    }
    PlayerAvatar_SetFacingDirection(task->fieldSystem->playerAvatar, task->facing);
}

void Task_WarpSpinDeparture(SysTask *sysTask, WarpSpinTask *task) {
    VecFx32 pos;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(task->fieldSystem->playerAvatar);

    switch (task->state) {
    case 0:
        task->state = 1;
        PlaySE(SEQ_SE_DP_TELE2);
    case 1:
        break;
    default:
        return;
    }
    if (task->counter % 2 != 0) {
        WarpSpin_RotatePlayer(task);
    }
    sub_0205F990(mapObject, &pos);
    pos.y = WARP_SPIN_HEIGHT(task->counter);
    sub_0205F9A0(mapObject, &pos);
    task->counter++;
    if (task->counter == WARP_SPIN_FRAMES) {
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
        return;
    }
    if (task->counter > WARP_SPIN_FRAMES && IsPaletteFadeFinished()) {
        *task->done = 1;
        Heap_Free(task);
        SysTask_Destroy(sysTask);
    }
}

void Task_WarpSpinArrival(SysTask *sysTask, WarpSpinTask *task) {
    VecFx32 pos;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(task->fieldSystem->playerAvatar);
    int n;

    switch (task->state) {
    case 0:
        MapObject_UnpauseMovement(mapObject);
        sub_0205F990(mapObject, &pos);
        n = WARP_SPIN_FRAMES - task->counter;
        pos.y = WARP_SPIN_HEIGHT(n);
        sub_0205F9A0(mapObject, &pos);
        MapObject_TryUpdateDraw(mapObject);
        PlaySE(SEQ_SE_DP_TELE2);
        task->state = 1;
    case 1:
        if (task->counter % 2 != 0) {
            WarpSpin_RotatePlayer(task);
        }
        sub_0205F990(mapObject, &pos);
        n = WARP_SPIN_FRAMES - task->counter;
        pos.y = WARP_SPIN_HEIGHT(n);
        sub_0205F9A0(mapObject, &pos);
        task->counter++;
        if (task->counter == 2) {
            BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
        }
        if (task->counter > WARP_SPIN_FRAMES) {
            task->state = 2;
            return;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PlayerAvatar_SetFacingDirection(task->fieldSystem->playerAvatar, DIR_SOUTH);
            *task->done = 1;
            Heap_Free(task);
            SysTask_Destroy(sysTask);
        }
        break;
    default:
        return;
    }
}

void FieldSystem_StartWarpSpin(FieldSystem *fieldSystem, BOOL departing, int *done) {
    WarpSpinTask *task = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(WarpSpinTask));

    MI_CpuFill8(task, 0, sizeof(WarpSpinTask));
    task->fieldSystem = fieldSystem;
    task->done = done;
    task->facing = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    if (departing) {
        SysTask_CreateOnMainQueue((SysTaskFunc)Task_WarpSpinDeparture, task, 100);
    } else {
        SysTask_CreateOnMainQueue((SysTaskFunc)Task_WarpSpinArrival, task, 100);
    }
}
