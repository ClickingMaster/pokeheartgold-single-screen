#include "global.h"

#include "constants/std_script.h"

#include "field/field_effect_helpers.h"

#include "field_effects.h"
#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_behavior_internal.h"
#include "map_object_movement_commands.h"
#include "map_object_movement_commands_internal.h"
#include "map_object_movement_utilities.h"
#include "map_object_movement_utilities_internal.h"
#include "overlay_01.h"
#include "field/map_object_render.h"
#include "overlay_01_022001E4.h"
#include "player_avatar.h"
#include "script_manager.h"
#include "sys_task_api.h"
#include "unk_02055418.h"

void MapObject_InitTypeBehavior(LocalMapObject *object) {
    MapObject_CallTypeBehaviorInit(object);
}

BOOL MapObject_UpdateTypeBehavior(LocalMapObject *object) {
    if (!MapObject_CallTypeBehaviorCheck(object)) {
        return FALSE;
    }
    if (MapObject_CallTypeBehaviorUpdate(object)) {
        return TRUE;
    }
    return FALSE;
}

void MapObject_CallTypeBehaviorInit(LocalMapObject *object) {
    gMapObjectTypeBehaviorInits[MapObject_GetType(object)](object);
}

BOOL MapObject_CallTypeBehaviorCheck(LocalMapObject *object) {
    return gMapObjectTypeBehaviorChecks[MapObject_GetType(object)](object);
}

BOOL MapObject_CallTypeBehaviorUpdate(LocalMapObject *object) {
    return gMapObjectTypeBehaviorUpdates[MapObject_GetType(object)](object);
}

BOOL MapObject_IsOffPreviousTile(LocalMapObject *object) {
    int x = MapObject_GetXCoord(object);
    if (x != MapObject_GetPreviousXCoord(object)) {
        return TRUE;
    }
    x = MapObject_GetZCoord(object);
    if (x != MapObject_GetPreviousZCoord(object)) {
        return TRUE;
    }
    return FALSE;
}

BOOL MapObject_IsOnPreviousTile(LocalMapObject *object) {
    int x = MapObject_GetXCoord(object);
    if (x != MapObject_GetPreviousXCoord(object)) {
        return FALSE;
    }
    x = MapObject_GetZCoord(object);
    if (x == MapObject_GetPreviousZCoord(object)) {
        return TRUE;
    }
    return FALSE;
}

void TypeBehavior_NoOpInit(LocalMapObject *object) {
}

int TypeBehavior_NoOpCheck(LocalMapObject *object) {
    return 0;
}

int TypeBehavior_NoOpUpdate(LocalMapObject *object) {
    return 0;
}

void TypeBehavior_LookSidewaysInit(LocalMapObject *object) {
    TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_InitBehaviorData(object, 9);
    state->limit = MapObject_GetParam(object, 1);
}

BOOL TypeBehavior_LookSidewaysCheck(LocalMapObject *object) {
    TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_GetBehaviorData(object);
    switch (state->state) {
    case 0:
        if (MapObject_IsOffPreviousTile(object) == TRUE) {
            state->state++;
        }
        break;
    case 1:
        if (MapObject_IsOnPreviousTile(object) == 0) {
            break;
        }
        state->count++;
        if (state->count < state->limit) {
            state->state = 0;
            break;
        }
        state->state++;
    case 2:
        if (MapObject_CheckSingleMovement(object) == TRUE) {
            break;
        }
        state->state++;
        state->count = 0;
        state->turnState = 0;
    case 3:
        return TRUE;
    }
    return FALSE;
}

BOOL TypeBehavior_LookSidewaysUpdate(LocalMapObject *object) {
    TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_GetBehaviorData(object);
    switch (state->turnState) {
    case 0: {
        FourValues dirTable = gLookSidewaysAxisByFacing;
        int dir = MapObject_GetFacingDirection(object);
        state->direction = dir;
        state->sequence = dirTable.values[dir];
        state->turnState++;
    }
    case 1: {
        TwoByTwoValues moveTable = gLookSidewaysDirections;
        MapObject_ForceSetHeldMovement(object, GetMovementForDirection(moveTable.values[state->sequence][state->index], 0));
        state->turnState++;
    }
    case 2:
        if (MapObject_RunForcedMovement(object) == 0) {
            return TRUE;
        }
        state->turnState++;
    case 3:
        state->timer++;
        if (state->timer < 8) {
            return TRUE;
        }
        state->timer = 0;
        state->turns++;
        if (state->turns < 4) {
            state->index = (state->index + 1) & 1;
            state->turnState = 1;
            return TRUE;
        }
        MapObject_SetFacingDirection(object, state->direction);
        state->turnState++;
        state->turns = 0;
        state->state = 0;
    }
    return FALSE;
}

void TypeBehavior_SpinInit(LocalMapObject *object) {
    TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_InitBehaviorData(object, 9);
    state->limit = MapObject_GetParam(object, 1);
    state->sequence = (MapObject_GetType(object) != 5);
}

BOOL TypeBehavior_SpinCheck(LocalMapObject *object) {
    TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_GetBehaviorData(object);
    switch (state->state) {
    case 0:
        if (MapObject_IsOffPreviousTile(object) == TRUE) {
            state->state++;
        }
        break;
    case 1:
        if (MapObject_IsOnPreviousTile(object) == 0) {
            break;
        }
        state->count++;
        if (state->count < state->limit) {
            state->state = 0;
            break;
        }
        state->state++;
    case 2:
        if (MapObject_CheckSingleMovement(object) == TRUE) {
            break;
        }
        state->state++;
        state->count = 0;
        state->turnState = 0;
    case 3:
        return TRUE;
    }
    return FALSE;
}

BOOL TypeBehavior_SpinUpdate(LocalMapObject *object) {
    DirectionMovementTable spinOrders = gTypeBehaviorSpinOrders;
    volatile TrainerBehaviorState *state = (TrainerBehaviorState *)MapObject_GetBehaviorData(object);
    int i;
    int dir;
    switch (state->turnState) {
    case 0:
        dir = MapObject_GetFacingDirection(object);
        i = 0;
        goto search_check;
search_next:
        i++;
search_check:
        if (i < 4) {
            if (dir != spinOrders.values[state->sequence][i]) {
                goto search_next;
            }
        }
        if (i >= 4) {
            GF_AssertFail();
        }
        state->direction = dir;
        state->index = (i + 1) % 4;
        state->turnState++;
    case 1:
        MapObject_ForceSetHeldMovement(object, GetMovementForDirection(spinOrders.values[state->sequence][state->index], 0));
        state->turnState++;
    case 2:
        if (MapObject_RunForcedMovement(object) == 0) {
            return TRUE;
        }
        state->turnState++;
    case 3:
        state->timer++;
        if (state->timer < 8) {
            return TRUE;
        }
        state->timer = 0;
        state->turns++;
        if (state->turns < 4) {
            state->index = (state->index + 1) % 4;
            state->turnState = 1;
            return TRUE;
        }
        MapObject_SetFacingDirection(object, state->direction);
        state->turnState++;
        state->turns = 0;
        state->state = 0;
    }
    return FALSE;
}

void ApricornTreeObject_Init(LocalMapObject *object) {
    ApricornTreeState *state = (ApricornTreeState *)MapObject_InitDrawData(object, 0x18);
    state->spriteId = 0x106;
    state->apricorns = 0;
    MapObject_CallDrawUpdate(object);
}

void ApricornTreeObject_Update(LocalMapObject *object) {
    MapObjectSpriteState *spriteState = (MapObjectSpriteState *)MapObject_GetMovementData(object);
    ApricornTreeState *state = (ApricornTreeState *)MapObject_GetDrawData(object);
    int apricorn = FieldSystem_ApricornTree_HasApricorn(MapObject_GetFieldSystem(object), object);
    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (state->billboard == NULL) {
        state->spriteId = GetApricornTreeSpriteId(FieldSystem_ApricornTree_TryGetApricorn(MapObject_GetFieldSystem(object), object), apricorn);
        MapObject_SetSpriteID(object, state->spriteId);
        MapObject_RequestBillboard(object, &state->billboard, state->spriteId);
    } else if (apricorn != state->apricorns) {
        MapObject_CancelAndFreeBillboard(object, &state->billboard, state->spriteId);
        state->spriteId = GetApricornTreeSpriteId(FieldSystem_ApricornTree_TryGetApricorn(MapObject_GetFieldSystem(object), object), apricorn);
        MapObject_SetSpriteID(object, state->spriteId);
        if (state->spriteId != 0xFFFF) {
            if (apricorn == 1) {
                ApricornRegrowth_Spawn(object);
            }
            MapObject_RequestBillboard(object, &state->billboard, state->spriteId);
        } else {
            GF_AssertFail();
        }
        spriteState->unk2 = 0;
    }
    state->apricorns = apricorn;
    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (state->billboard == NULL) {
        return;
    }
    MapObject_UpdateBillboardPosition(object, state->billboard);
    if (MapObject_IsBillboardAnimPaused(object) == 0) {
        switch (MapObject_GetAnimation(object)) {
        case 0:
            if (BillboardObject_GetSequence(state->billboard) != 0) {
                BillboardObject_SetSequence(state->billboard, 0);
                BillboardObject_SetSequenceFrame(state->billboard, 0);
            }
            break;
        case 1:
            if (BillboardObject_GetSequence(state->billboard) != 1) {
                BillboardObject_SetSequence(state->billboard, 1);
                BillboardObject_SetSequenceFrame(state->billboard, 0);
            }
            break;
        case 2:
            if (BillboardObject_GetSequence(state->billboard) != 2) {
                BillboardObject_SetSequence(state->billboard, 2);
                BillboardObject_SetSequenceFrame(state->billboard, 0);
            }
            break;
        default:
            GF_AssertFail();
            if (BillboardObject_GetSequence(state->billboard) != 0) {
                BillboardObject_SetSequence(state->billboard, 0);
                BillboardObject_SetSequenceFrame(state->billboard, 0);
            }
            break;
        }
        BillboardObject_AdvanceFrame(state->billboard, FX32_ONE);
    }
    MapObject_SetBillboardVisibleFromFlags(object, state->billboard);
}

void ApricornTreeObject_Delete(LocalMapObject *object) {
    ApricornTreeState *state = (ApricornTreeState *)MapObject_GetDrawData(object);
    MapObject_CancelAndFreeBillboard(object, &state->billboard, state->spriteId);
}

void ApricornTreeObject_Pause(LocalMapObject *object) {
    ApricornTreeState *state = (ApricornTreeState *)MapObject_GetDrawData(object);
    if (state->billboard != NULL) {
        BillboardObject_SaveState(state->billboard, &state->savedState);
    }
    MapObject_CancelAndFreeBillboard(object, &state->billboard, state->spriteId);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

void ApricornTreeObject_Resume(LocalMapObject *object) {
    ApricornTreeState *state = (ApricornTreeState *)MapObject_GetDrawData(object);
    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (state->billboard == NULL) {
        if (state->spriteId != 0xFFFF) {
            MapObject_RequestBillboard(object, &state->billboard, state->spriteId);
        } else {
            GF_AssertFail();
        }
    }
    if (state->billboard == NULL) {
        return;
    }
    BillboardObject_RestoreState(state->billboard, &state->savedState);
    MapObject_UpdateBillboardPosition(object, state->billboard);
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

void *ApricornTreeObject_GetBillboard(LocalMapObject *object) {
    if (MapObject_IsBillboardPending(object) == TRUE) {
        GF_AssertFail();
        return NULL;
    }
    return ((ApricornTreeState *)MapObject_GetDrawData(object))->billboard;
}

u32 GetApricornTreeSpriteId(int apricorn, int apricornCount) {
    if (apricornCount == 0) {
        return 0x106;
    }
    if (apricorn == 0) {
        GF_AssertFail();
    }
    return ov01_02207274[apricorn];
}

BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible) {
    EngageParams first;
    EngageParams second;
    EngageParams partner;
    PlayerAvatar *avatar = fieldSystem->playerAvatar;
    MapObjectManager *manager = fieldSystem->mapObjectManager;

    if (!CheckSeenByNpcTrainers(fieldSystem, manager, avatar, NULL, &first)) {
        return FALSE;
    }
    if (first.isDouble == 0) {
        StartMapSceneScript(fieldSystem, std_trainer_approach, first.object);
        if (!doublesEligible || !CheckSeenByNpcTrainers(fieldSystem, manager, avatar, first.object, &second)) {
            FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 0, 0);
            return TRUE;
        }
        FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 2, 0);
        FieldSystem_SetEngagedTrainer(fieldSystem, second.object, second.x, second.z, second.scriptId, second.trainerNum, 2, 1);
        return TRUE;
    }
    if (first.isDouble == 1) {
        if (!doublesEligible) {
            return FALSE;
        }
        GetEngagingTrainerParams(&partner, (LocalMapObject *)MapObjectManager_FindTrainerPartner(fieldSystem, manager, first.object, first.trainerNum), first.x, first.z);
        StartMapSceneScript(fieldSystem, std_trainer_approach, first.object);
        FieldSystem_SetEngagedTrainer(fieldSystem, first.object, first.x, first.z, first.scriptId, first.trainerNum, 1, 0);
        FieldSystem_SetEngagedTrainer(fieldSystem, partner.object, partner.x, partner.z, partner.scriptId, partner.trainerNum, 1, 1);
        return TRUE;
    }
    GF_AssertFail();
    return FALSE;
}

BOOL CheckSeenByNpcTrainers(FieldSystem *fieldSystem, MapObjectManager *manager, PlayerAvatar *avatar, LocalMapObject *exclude, EngageParams *out) {
    int x;
    int index = 0;
    int direction;
    LocalMapObject *object = NULL;

    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE)) {
        do {
            if (exclude == NULL || exclude != object) {
                x = TrainerSight_FindPlayer(object, avatar, &direction);
                if (x != -1) {
                    if (!TrainerFlagCheck(fieldSystem->saveData, (u16)MapObject_GetTrainerNum(object))) {
                        GetEngagingTrainerParams(out, object, x, direction);
                        return TRUE;
                    }
                }
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE));
    }
    return FALSE;
}

void GetEngagingTrainerParams(EngageParams *out, LocalMapObject *object, int x, int z) {
    out->x = x;
    out->z = z;
    out->scriptId = MapObject_GetScriptID(object);
    out->trainerNum = ScriptNumToTrainerNum((u16)out->scriptId);
    out->isDouble = TrainerNumIsDouble((u16)out->trainerNum);
    out->object = object;
}

int MapObject_GetTrainerSightType(LocalMapObject *object) {
    int type = MapObject_GetType(object);
    switch (type) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        type = 1;
        break;
    }
    return type;
}

int TrainerSight_FindPlayer(LocalMapObject *object, PlayerAvatar *player, int *outDir) {
    int mode = MapObject_GetTrainerSightType(object);

    if (mode == 1) {
        int x = PlayerAvatar_GetXCoord(player);
        int z = PlayerAvatar_GetZCoord(player);
        int dir = MapObject_GetFacingDirection(object);
        int param = MapObject_GetParam(object, 0);
        int dist = TrainerSight_GetDistanceInDirection(object, dir, param, x, z, 0);
        if (dist != -1 && TrainerSight_IsPathBlocked(object, dir, dist) == 0) {
            *outDir = dir;
            return dist;
        }
        return -1;
    }
    if (mode == 2) {
        int x = PlayerAvatar_GetXCoord(player);
        int z = PlayerAvatar_GetZCoord(player);
        int param = MapObject_GetParam(object, 0);
        int dir;
        for (dir = 0; dir < 4; dir++) {
            int dist = TrainerSight_GetDistanceInDirection(object, dir, param, x, z, 0);
            if (dist != -1 && TrainerSight_IsPathBlocked(object, dir, dist) == 0) {
                *outDir = dir;
                return dist;
            }
        }
        return -1;
    }
    return -1;
}

int TrainerSight_GetDistanceInDirection(LocalMapObject *object, int direction, int range, int targetX, int targetZ, int unused) {
    return _020FE194[direction](object, range, targetX, targetZ, unused);
}

int TrainerSight_GetDistanceNorth(LocalMapObject *object, int range, int targetX, int targetZ, int unused) {
    int z, x = MapObject_GetXCoord(object);
    if (x == targetX) {
        z = MapObject_GetZCoord(object);
        if (targetZ < z && targetZ >= z - range) {
            return z - targetZ;
        }
    }
    return -1;
}

int TrainerSight_GetDistanceSouth(LocalMapObject *object, int range, int targetX, int targetZ, int unused) {
    int z, x = MapObject_GetXCoord(object);
    if (x == targetX) {
        z = MapObject_GetZCoord(object);
        if (targetZ > z && targetZ <= z + range) {
            return targetZ - z;
        }
    }
    return -1;
}

int TrainerSight_GetDistanceWest(LocalMapObject *object, int range, int targetX, int targetZ, int unused) {
    int x, z = MapObject_GetZCoord(object);
    if (z == targetZ) {
        x = MapObject_GetXCoord(object);
        if (targetX < x && targetX >= x - range) {
            return x - targetX;
        }
    }
    return -1;
}

int TrainerSight_GetDistanceEast(LocalMapObject *object, int range, int targetX, int targetZ, int unused) {
    int x, z = MapObject_GetZCoord(object);
    if (z == targetZ) {
        x = MapObject_GetXCoord(object);
        if (targetX > x && targetX <= x + range) {
            return targetX - x;
        }
    }
    return -1;
}

int TrainerSight_IsPathBlocked(LocalMapObject *object, int direction, int distance) {
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

u32 MapObject_GetTrainerNum(LocalMapObject *object) {
    return ScriptNumToTrainerNum((u16)MapObject_GetScriptID(object));
}

u32 sub_02064518(LocalMapObject *object) {
    return MapObject_GetTrainerNum(object);
}

LocalMapObject *MapObjectManager_FindTrainerPartner(void *unused, MapObjectManager *manager, LocalMapObject *exclude, u32 trainerNum) {
    s32 index = 0;
    LocalMapObject *object;
    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE)) {
        do {
            if (object != exclude && (u32)(MapObject_GetTrainerSightType(object) - 1) <= 1 && MapObject_GetTrainerNum(object) == trainerNum) {
                return object;
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE));
    }
    GF_AssertFail();
    return NULL;
}

SysTask *TrainerApproach_Start(FieldSystem *fieldSystem, LocalMapObject *trainer, PlayerAvatar *player, int movement, int remaining, int unk10, int unk14, int unk18) {
    return TrainerApproach_CreateTask(fieldSystem, trainer, player, movement, remaining, unk10, unk14, unk18);
}

BOOL TrainerApproach_IsDone(SysTask *task) {
    if (task == NULL) {
        GF_AssertFail();
    }
    return TrainerApproach_GetDone(task);
}

void TrainerApproach_Delete(SysTask *task) {
    TrainerApproach_DestroyTask(task);
}

SysTask *TrainerApproach_CreateTask(FieldSystem *fieldSystem, LocalMapObject *trainer, PlayerAvatar *player, int movement, int remaining, int unk10, int unk14, int unk18) {
    TrainerApproachWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(TrainerApproachWork));
    SysTask *task;
    if (work == NULL) {
        GF_AssertFail();
    }
    memset(work, 0, sizeof(TrainerApproachWork));
    work->direction = movement;
    work->distance = remaining;
    work->unk10 = unk10;
    work->encounterType = unk14;
    work->trainerIndex = unk18;
    work->fieldSystem = fieldSystem;
    work->trainer = trainer;
    work->player = player;
    task = SysTask_CreateOnMainQueue(TrainerApproach_Task, work, 0xFF);
    if (task == NULL) {
        GF_AssertFail();
    }
    return task;
}

BOOL TrainerApproach_GetDone(SysTask *task) {
    TrainerApproachWork *work = SysTask_GetData(task);
    return work->done;
}

void TrainerApproach_DestroyTask(SysTask *task) {
    void *work = SysTask_GetData(task);
    Heap_FreeExplicit(HEAP_ID_FIELD1, work);
    SysTask_Destroy(task);
}

void TrainerApproach_Task(SysTask *task, void *data) {
    TrainerApproachWork *work = data;
    while (gTrainerApproachStateFuncs[work->state](work) == TRUE) {}
}

BOOL TrainerApproach_UnpauseTrainer(TrainerApproachWork *work) {
    LocalMapObject *object = work->trainer;
    if (MapObject_CheckSingleMovement(object) == TRUE) {
        MapObject_UnpauseMovement(object);
    }
    work->state = 1;
    return TRUE;
}

BOOL TrainerApproach_FaceDirection(TrainerApproachWork *work) {
    LocalMapObject *object = work->trainer;
    if (MapObject_CheckSingleMovement(object) == TRUE) {
        return FALSE;
    }
    MapObject_SetFacingAndUpdateDraw(work->trainer, work->direction);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_MOVEMENT_PAUSED);
    work->state = 2;
    return TRUE;
}

BOOL TrainerApproach_WaitForPlayer(TrainerApproachWork *work) {
    LocalMapObject *player = PlayerAvatar_GetMapObject(work->player);
    if (!MapObject_IsMovementPaused(player)) {
        return FALSE;
    }
    switch (MapObject_GetMovement(work->trainer)) {
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
        work->state = 7;
        return TRUE;
    default:
        work->state = 3;
        return TRUE;
    }
}

BOOL TrainerApproach_StartFace(TrainerApproachWork *work) {
    if (!MapObject_AreBitsSetForMovementScriptInit(work->trainer)) {
        return FALSE;
    }
    if (work->direction == -1) {
        GF_AssertFail();
    }
    MapObject_SetHeldMovement(work->trainer, GetMovementForDirection(work->direction, 0));
    work->state = 4;
    return FALSE;
}

BOOL TrainerApproach_WaitFace(TrainerApproachWork *work) {
    if (!MapObject_IsMovementPaused(work->trainer)) {
        return FALSE;
    }
    work->state = 5;
    return TRUE;
}

BOOL TrainerApproach_SpawnExclamation(TrainerApproachWork *work) {
    work->effect = EmoteMark_Spawn(work->trainer, 0, 0);
    work->state = 6;
    return FALSE;
}

BOOL TrainerApproach_WaitExclamation(TrainerApproachWork *work) {
    if (EmoteMark_IsDone(work->effect) == TRUE) {
        FieldEffect_Destroy(work->effect);
        work->state = 9;
    }
    return FALSE;
}

BOOL TrainerApproach_StartReveal(TrainerApproachWork *work) {
    MapObject_SetHeldMovement(work->trainer, 0x65);
    work->state = 8;
    return FALSE;
}

BOOL TrainerApproach_WaitReveal(TrainerApproachWork *work) {
    if (MapObject_IsMovementPaused(work->trainer) == TRUE) {
        work->state = 9;
    }
    return FALSE;
}

BOOL TrainerApproach_Wait(TrainerApproachWork *work) {
    if (++work->timer >= 30) {
        work->timer = 0;
        work->state = 10;
    }
    return FALSE;
}

BOOL TrainerApproach_CheckDistance(TrainerApproachWork *work) {
    if (work->distance <= 1) {
        work->state = 13;
        return TRUE;
    }
    work->state = 11;
    return TRUE;
}

BOOL TrainerApproach_StartStep(TrainerApproachWork *work) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->trainer) == TRUE) {
        MapObject_SetHeldMovement(work->trainer, GetMovementForDirection(work->direction, 12));
        work->state = 12;
    }
    return FALSE;
}

BOOL TrainerApproach_WaitStep(TrainerApproachWork *work) {
    if (!MapObject_IsMovementPaused(work->trainer)) {
        return FALSE;
    }
    work->distance--;
    work->state = 10;
    return TRUE;
}

BOOL TrainerApproach_WaitArrived(TrainerApproachWork *work) {
    if (++work->timer < 8) {
        return FALSE;
    }
    work->timer = 0;
    work->state = 14;
    return TRUE;
}

BOOL TrainerApproach_TurnPlayer(TrainerApproachWork *work) {
    LocalMapObject *object = PlayerAvatar_GetMapObject(work->player);
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    int tx = MapObject_GetXCoord(work->trainer);
    int tz = MapObject_GetZCoord(work->trainer);
    int direction = GetDirectionTowards(x, z, tx, tz);
    if (direction != PlayerAvatar_GetFacingDirection(work->player) && (work->trainerIndex == 0 || work->encounterType == 2)) {
        if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK7);
            MapObject_SetHeldMovement(object, GetMovementForDirection(direction, 0));
            work->state = 15;
        }
    } else {
        work->state = 16;
    }
    return FALSE;
}

BOOL TrainerApproach_WaitPlayerTurn(TrainerApproachWork *work) {
    LocalMapObject *player = PlayerAvatar_GetMapObject(work->player);
    if (!MapObject_IsMovementPaused(player)) {
        return FALSE;
    }
    MapObject_ClearHeldMovementIfActive(player);
    work->state = 16;
    return TRUE;
}

BOOL TrainerApproach_ResetTrainerMovement(TrainerApproachWork *work) {
    MapObject_ClearHeldMovementIfActive(work->trainer);
    MapObject_ChangeMovement(work->trainer, 0);
    work->state = 17;
    return TRUE;
}

BOOL TrainerApproach_Finish(TrainerApproachWork *work) {
    work->done = TRUE;
    return FALSE;
}
