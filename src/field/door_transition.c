#include "field/door_transition.h"

#include "global.h"

#include "constants/movements.h"
#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/field_follow_mon.h"
#include "field/fieldmap.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"

#include "camera.h"
#include "camera_translation.h"
#include "field_metatile_query_internal.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_header.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "field_transition_effects.h"

// Door prop slot used by the one-shot animation manager for front doors and for side doors.
#define DOOR_ANIM_TAG      1
#define SIDE_DOOR_ANIM_TAG 2

typedef struct DoorTransitionWork {
    int state;                               // 0x00
    int x;                                   // 0x04
    int z;                                   // 0x08
    u8 animTag;                              // 0x0C
    u8 cameraStep;                           // 0x0D
    u16 cameraAngle;                         // 0x0E
    BOOL adjustCamera;                       // 0x10
    u32 unk14;                               // 0x14
    GFCameraTranslationWrapper *translation; // 0x18
    u16 translationDone;                     // 0x1C
    u16 cameraPathIndex;                     // 0x1E
} DoorTransitionWork; // size: 0x20

typedef struct DoorTransitionTaskWork {
    u8 state;         // 0x00
    u8 cameraStep;    // 0x01
    u16 cameraAngle;  // 0x02
    BOOL adjustCamera; // 0x04
} DoorTransitionTaskWork; // size: 0x08

// Same layout as MapPropAnimListFile; a nonzero door type marks a prop as a door and selects its sound effects.
typedef struct DoorAnimListEntry {
    u32 unk00;
    u8 doorType; // 0x04
    u8 filler05[0x13];
} DoorAnimListEntry;

typedef struct DoorModelList {
    int ids[4];
} DoorModelList;

extern const DoorModelList gSideDoorExitModels;
extern const DoorModelList gSideDoorEntryModels;
extern const CameraTranslationPathTemplate gDoorCameraPaths[2];

FieldTransitionWork *DoorTransition_New(void) {
    DoorTransitionWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(DoorTransitionWork));
    work->state = 0;
    work->translationDone = 0;
    return (FieldTransitionWork *)work;
}

void DoorTransition_Free(FieldTransitionWork *work) {
    Heap_Free(work);
}

void DoorTransition_SetPosition(u32 x, u32 z, FieldTransitionWork *arg) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    work->x = x;
    work->z = z;
}

BOOL DoorTransition_WalkIntoDoor(FieldSystem *fieldSystem, FieldTransitionWork *arg) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    fx32 rect[4];
    DoorAnimListEntry entry;
    MapProp *prop;
    int found;
    int modelId0;
    NARC *narc;
    int *props;

    int animationCount;
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    int i;

    switch (work->state) {
    case 0:
        work->adjustCamera = FALSE;
        work->cameraStep = 0;
        Terrain_MakeTileRect(work->x, work->z, -1, -1, 3, 1, rect);
        work->animTag = DOOR_ANIM_TAG;
        found = FALSE;
        narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
        props = FieldSystem_CollectMapPropModelsInRect(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
        for (i = 0; i < 4; i++) {
            if (props[i] != 0) {
                NARC_ReadWholeMember(narc, props[i], &entry);
                if (entry.doorType != 0) {
                    FieldSystem_FindMapPropInRect(fieldSystem, props[i], rect, &prop);
                    found = TRUE;
                    modelId0 = props[i];
                    break;
                }
            }
        }
        Heap_Free(props);
        if (!found) {
            goto no_prop;
        }
        animationCount = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId0);
        if (animationCount == 0) {
            return TRUE;
        }
        texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        renderObj = MapProp_GetRenderSurface(prop);
        model = MapProp_GetResModel(prop);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, modelId0, renderObj, model, texture, animationCount, 1, FALSE);
        MapPropOneShotAnimationManager_SetAnimationField35(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 1);
        goto prop_loaded;
no_prop:
        return TRUE;
prop_loaded:
        work->cameraPathIndex = DoorTransition_GetCameraPathIndex(modelId0);
        if (work->cameraPathIndex != 0xFFFF) {
            work->state = 100;
        } else {
            work->state++;
        }
        break;
    case 1: {
        int modelId;
        modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG);
        if (!DoorTransition_ModelSkipsCameraZoom(fieldSystem, modelId)) {
            Camera_ClearFixedTarget(fieldSystem->camera);
            work->adjustCamera = TRUE;
        }
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 0, GetDoorSE(fieldSystem, modelId, TRUE));
        work->state++;
        break;
    }
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG)) {
            work->state++;
        }
        break;
    case 3: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetHeldMovement(obj, MOVEMENT_STEP_UP);
        if (FollowMon_IsActive(fieldSystem) && !FollowMon_CheckVisibleFlag(fieldSystem)) {
            int x = MapObject_GetPreviousXCoord(obj);
            int z = MapObject_GetPreviousZCoord(obj);
            FollowMon_QueuePlayerStep(MOVEMENT_STEP_UP, x, z, &fieldSystem->followMon);
        }
        work->state++;
        break;
    }
    case 4: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            if (FollowMon_IsActive(fieldSystem)) {
                LocalMapObject *mon = FollowMon_GetMapObject(fieldSystem);
                if (MapObject_IsMovementPaused(mon) != TRUE) {
                    break;
                }
                MapObject_ClearHeldMovementIfActive(obj);
                MapObject_ClearHeldMovementIfActive(mon);
                work->state++;
            } else {
                work->state++;
            }
        }
        break;
    }
    case 5:
        MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), TRUE);
        return TRUE;
    case 100: {
        Camera_SetPerspectiveClippingPlane(0x96000, 0x456000, fieldSystem->camera);
        work->translation = CreateCameraTranslationWrapper(HEAP_ID_FIELD1, fieldSystem->camera);
        SetCameraTranslationPath(work->translation, (CameraTranslationPathTemplate *)&gDoorCameraPaths[work->cameraPathIndex], 24);
        work->state++;
        break;
    }
    case 101:
        if (IsCameraTranslationFinished(work->translation)) {
            DeleteCameraTranslationWrapper(work->translation);
            work->translationDone = 1;
            work->state = 1;
        }
        break;
    }
    if (work->adjustCamera && !work->translationDone) {
        DoorTransition_StepCameraZoomIn(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

BOOL DoorTransition_WalkOutOfDoor(FieldSystem *fieldSystem, FieldTransitionWork *arg) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    fx32 rect[4];
    DoorAnimListEntry entry;
    MapProp *prop;
    int found;
    int modelId0;
    NARC *narc;
    int *props;
    int zero;
    int animationCount;
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    int i;

    switch (work->state) {
    case 0:
        zero = FALSE;
        work->adjustCamera = zero;
        work->cameraStep = zero;
        Terrain_MakeTileRect(work->x, work->z, -1, 0, 3, 1, rect);
        work->animTag = DOOR_ANIM_TAG;
        found = FALSE;
        narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
        props = FieldSystem_CollectMapPropModelsInRect(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
        for (i = 0; i < 4; i++) {
            if (props[i] != 0) {
                NARC_ReadWholeMember(narc, props[i], &entry);
                if (entry.doorType != 0) {
                    FieldSystem_FindMapPropInRect(fieldSystem, props[i], rect, &prop);
                    found = TRUE;
                    modelId0 = props[i];
                    break;
                }
            }
        }
        Heap_Free(props);
        if (!found) {
            goto no_prop;
        }
        animationCount = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId0);
        if (animationCount == 0) {
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
            work->state = 6;
            return FALSE;
        }
        texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        renderObj = MapProp_GetRenderSurface(prop);
        model = MapProp_GetResModel(prop);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, modelId0, renderObj, model, texture, animationCount, 1, FALSE);
        goto prop_loaded;
no_prop:
        FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
        work->state = 6;
        return FALSE;
prop_loaded:
        work->cameraAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        if (!DoorTransition_ModelSkipsCameraZoom(fieldSystem, modelId0)) {
            Camera_AdjustPerspectiveAngle(-96, fieldSystem->camera);
        }
        work->state++;
        break;
    case 1: {
        int modelId;
        NewFieldFadeEnvironment(fieldSystem->taskman, FADE_BOTH_SCREENS, FADE_TYPE_UNK_9, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_FIELD2);
        modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG);
        if (!DoorTransition_ModelSkipsCameraZoom(fieldSystem, modelId)) {
            work->adjustCamera = TRUE;
        }
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 0, GetDoorSE(fieldSystem, modelId, TRUE));
        work->state++;
        break;
    }
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG)) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), FALSE);
            work->state++;
        }
        break;
    case 3:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), MOVEMENT_STEP_DOWN);
        work->state++;
        break;
    case 4: {
        int modelId;
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            FollowMon_MoveToPlayer(fieldSystem, DIR_SOUTH);
            modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 1, GetDoorSE(fieldSystem, modelId, FALSE));
            work->state++;
        }
        break;
    }
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG) && IsPaletteFadeFinished() && work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG);
            return TRUE;
        }
        break;
    case 6:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera) {
        DoorTransition_StepCameraZoomOut(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

// Narrows the field of view by 12 per call, 0x60 in total.
void DoorTransition_StepCameraZoomIn(Camera *camera, u8 *step) {
    u8 delta = 12;
    int total = *step * delta;
    if (total > 0x60) {
        return;
    }
    if ((u16)total > 0x60) {
        delta = (u16)total - 0x60;
    }
    if ((u16)(Camera_GetPerspectiveAngle(camera) - delta) != 0) {
        Camera_AdjustPerspectiveAngle(-delta, camera);
    }
    (*step)++;
}

// Widens the field of view by 16 per call, 0x60 in total.
void DoorTransition_StepCameraZoomOut(Camera *camera, u8 *step) {
    int total = *step << 4;
    u8 delta;
    int angle;
    if (total >= 0x60) {
        return;
    }
    if ((u16)total > 0x60) {
        delta = (u16)total - 0x60;
    } else {
        delta = 16;
    }
    angle = Camera_GetPerspectiveAngle(camera);
    angle += delta;
    if ((u16)angle != 0) {
        Camera_AdjustPerspectiveAngle(delta, camera);
    }
    (*step)++;
}

int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening) {
    DoorAnimListEntry entry;

    NARC_ReadWholeMember(AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager), modelId, &entry);
    if (opening) {
        switch (entry.doorType) {
        case 1:
            return SEQ_SE_DP_DOOR_OPEN;
        case 2:
            return SEQ_SE_DP_DOOR10;
        case 3:
            return SEQ_SE_PL_DOOR_OPEN5;
        case 4:
            return SEQ_SE_GS_HIKIDO_OPEN;
        }
        GF_ASSERT(FALSE);
        return SEQ_SE_DP_DOOR_OPEN;
    }
    switch (entry.doorType) {
    case 1:
        return SEQ_SE_DP_DOOR_CLOSE2;
    case 2:
        return 0;
    case 3:
        return 0;
    case 4:
        return SEQ_SE_GS_HIKIDO_CLOSE;
    }
    GF_ASSERT(FALSE);
    return SEQ_SE_DP_DOOR_CLOSE2;
}

// Doors whose entry plays a camera path from gDoorCameraPaths; 0xFFFF for all others.
int DoorTransition_GetCameraPathIndex(int modelId) {
    int ret = 0xFFFF;
    switch (modelId) {
    case 0x3D:
        ret = 0;
        break;
    case 0xE0:
        ret = 1;
        break;
    }
    return ret;
}

BOOL DoorTransition_WalkOutOfSideDoor(FieldSystem *fieldSystem, FieldTransitionWork *arg, u8 direction) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    LocalMapObject *obj;

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorModelList models = gSideDoorExitModels;
        MapProp *prop;
        int modelId;
        if (direction == DIR_WEST) {
            Terrain_MakeTileRect(work->x, work->z, -1, 0, 2, 1, rect);
        } else {
            Terrain_MakeTileRect(work->x, work->z, 0, 0, 2, 1, rect);
        }
        work->animTag = SIDE_DOOR_ANIM_TAG;
        if (FieldSystem_FindMapPropOfModelsInRect(fieldSystem, models.ids, 4, rect, &prop, &modelId)) {
            texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
            renderObj = MapProp_GetRenderSurface(prop);
            model = MapProp_GetResModel(prop);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG, modelId, renderObj, model, texture, 1, 1, 1);
        } else {
            GF_ASSERT(FALSE);
            return TRUE;
        }
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, MOVEMENT_UNK_73);
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    }
    case 1:
        FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG, 0, SEQ_SE_DP_ESUKA);
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, direction == DIR_EAST ? 0xB : 0xA);
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    case 2:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, MOVEMENT_UNK_74);
            work->state++;
        }
        break;
    case 3:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, direction == DIR_EAST ? 0xB : 0xA);
            work->state++;
        }
        break;
    case 4:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            FollowMon_MoveToPlayer(fieldSystem, direction);
            work->state++;
        }
        break;
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG) && IsPaletteFadeFinished()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG);
            StopSE(SEQ_SE_DP_ESUKA, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL DoorTransition_WalkIntoSideDoor(FieldSystem *fieldSystem, FieldTransitionWork *arg, u8 direction) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    LocalMapObject *obj;
    int movement;
    int x;

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorModelList models = gSideDoorEntryModels;
        MapProp *prop;
        int modelId;
        if (direction == DIR_WEST) {
            Terrain_MakeTileRect(work->x, work->z, -1, 0, 2, 1, rect);
        } else {
            Terrain_MakeTileRect(work->x, work->z, 0, 0, 2, 1, rect);
        }
        work->animTag = SIDE_DOOR_ANIM_TAG;
        if (FieldSystem_FindMapPropOfModelsInRect(fieldSystem, models.ids, 4, rect, &prop, &modelId)) {
            texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
            renderObj = MapProp_GetRenderSurface(prop);
            model = MapProp_GetResModel(prop);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG, modelId, renderObj, model, texture, 1, 1, 0);
        } else {
            GF_ASSERT(FALSE);
            return TRUE;
        }
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, MOVEMENT_UNK_73);
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    }
    case 1:
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG, 0, SEQ_SE_DP_ESUKA);
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            movement = direction == DIR_WEST ? 0xA : 0xB;
            MapObject_SetHeldMovement(obj, movement);
            if (FollowMon_IsActive(fieldSystem)) {
                x = MapObject_GetPreviousXCoord(obj);
                FollowMon_QueuePlayerStep(movement, x, MapObject_GetPreviousZCoord(obj), &fieldSystem->followMon);
            }
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    case 2:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, MOVEMENT_UNK_74);
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
            work->state++;
        }
        break;
    case 3:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            work->state++;
        }
        break;
    case 4:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG) && IsPaletteFadeFinished()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, SIDE_DOOR_ANIM_TAG);
            StopSE(SEQ_SE_DP_ESUKA, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL Task_WaitDoorAnimation(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    u8 *tag = TaskManager_GetEnvironment(taskman);
    if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, *tag)) {
        Heap_Free(tag);
        return TRUE;
    }
    return FALSE;
}

// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
// Loads the animations of the door prop in front of (x, z) into the one-shot animation slot tag.
void DoorAnimation_Load(FieldSystem *fieldSystem, int x, int z, u8 tag) {
    fx32 rect[4];
    struct {
        MapProp *prop;
        DoorAnimListEntry entry;
    } door;
    NARC *narc;
    int found;
    int modelId;
    int *props;
    int i;
    int *cursor;

    Terrain_MakeTileRect(x, z, -1, 0, 3, 1, rect);
    narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
    props = FieldSystem_CollectMapPropModelsInRect(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
    found = FALSE;
    for (i = 0, cursor = props; i < 4; i++, cursor++) {
        if (*cursor) {
            NARC_ReadWholeMember(narc, *cursor, &door.entry);
            if (door.entry.doorType) {
                FieldSystem_FindMapPropInRect(fieldSystem, props[i], rect, &door.prop);
                found = TRUE;
                modelId = props[i];
                break;
            }
        }
    }
    Heap_Free(props);
    if (found) {
        int count = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId);
        if (count) {
            NNSG3dResTex *texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
            NNSG3dRenderObj *renderObj = MapProp_GetRenderSurface(door.prop);
            NNSG3dResMdl *model = MapProp_GetResModel(door.prop);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, tag, modelId, renderObj, model, texture, count, 1, FALSE);
        }
    } else {
        GF_ASSERT(FALSE);
    }
}
#pragma force_active reset

void DoorAnimation_PlayOpen(FieldSystem *fieldSystem, u8 tag) {
    int se = GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, tag), TRUE);
    MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, tag, FALSE, se);
}

void DoorAnimation_PlayClose(FieldSystem *fieldSystem, u8 tag) {
    int se = GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, tag), FALSE);
    MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, tag, TRUE, se);
}

void DoorAnimation_WaitForEnd(FieldSystem *fieldSystem, u8 tag) {
    u8 *env = Heap_AllocAtEnd(HEAP_ID_FIELD1, 1);
    *env = tag;
    TaskManager_Call(fieldSystem->taskman, Task_WaitDoorAnimation, env);
}

void DoorAnimation_Unload(FieldSystem *fieldSystem, u8 tag) {
    MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, tag);
}

void *DoorTransition_NewTaskWork(void) {
    DoorTransitionTaskWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(DoorTransitionTaskWork));
    work->state = 0;
    return work;
}

#pragma force_active on
// After a warp: fades in from white with the camera zoomed in; facing south in front of a door, the door opens and
// the player steps out of it.
BOOL Task_WalkOutOfDoorAfterWarp(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    DoorTransitionWork *work = TaskManager_GetEnvironment(taskman);

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorAnimListEntry entry;
        MapProp *prop;
        NNSG3dResTex *texture;
        LocalMapObject *playerObj;
        int found;
        int modelId;
        NARC *narc;
        int *props;
        int i;
        int count;
        NNSG3dRenderObj *renderObj;
        NNSG3dResMdl *model;

        playerObj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        work->adjustCamera = FALSE;
        work->cameraStep = 0;
        work->cameraAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        Camera_AdjustPerspectiveAngle(0xFFA0, fieldSystem->camera);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, HEAP_ID_FIELD2);
        work->adjustCamera = TRUE;
        if (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == DIR_SOUTH) {
            work->animTag = DOOR_ANIM_TAG;
            found = FALSE;
            Terrain_MakeTileRect(work->x, work->z, -1, found, 3, 1, rect);
            narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
            props = FieldSystem_CollectMapPropModelsInRect(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
            for (i = 0; i < 4; i++) {
                if (props[i] != 0) {
                    NARC_ReadWholeMember(narc, props[i], &entry);
                    if (entry.doorType != 0) {
                        FieldSystem_FindMapPropInRect(fieldSystem, props[i], rect, &prop);
                        found = TRUE;
                        modelId = props[i];
                        break;
                    }
                }
            }
            Heap_Free(props);
            if (found) {
                count = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId);
                if (count == 0) {
                    return FALSE;
                }
                texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
                renderObj = MapProp_GetRenderSurface(prop);
                model = MapProp_GetResModel(prop);
                MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, modelId, renderObj, model, texture, count, 1, FALSE);
                work->state = 4;
                break;
            }
        }
        if (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == DIR_SOUTH) {
            MapObject_SetVisible(playerObj, TRUE);
            work->state = 1;
        } else {
            MapObject_SetVisible(playerObj, FALSE);
            work->state = 3;
        }
        break;
    }
    case 1: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetVisible(obj, FALSE);
        MapObject_SetHeldMovement(obj, MOVEMENT_STEP_DOWN);
        work->state++;
        break;
    }
    case 2: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            work->state++;
        }
        break;
    }
    case 3:
        if (IsPaletteFadeFinished()) {
            if (work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
                return TRUE;
            }
        }
        break;
    case 4:
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 0, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG), TRUE));
        work->state = 5;
        break;
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG)) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), FALSE);
            work->state = 6;
        }
        break;
    case 6:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), MOVEMENT_STEP_DOWN);
        work->state = 7;
        break;
    case 7: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            FollowMon_MoveToPlayer(fieldSystem, DIR_SOUTH);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG, 1, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG), FALSE));
            work->state++;
        }
        break;
    }
    case 8:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG) && IsPaletteFadeFinished() && work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, DOOR_ANIM_TAG);
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera) {
        DoorTransition_StepCameraZoomOut(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}
#pragma force_active reset

// Stairs: hides the player (MapObject_SetVisible(TRUE) sets the hidden flag) and fades to white while zooming in.
BOOL Task_StairsFadeToWhite(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    DoorTransitionTaskWork *work = TaskManager_GetEnvironment(taskman);
    switch (work->state) {
    case 0:
        MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), TRUE);
        work->adjustCamera = FALSE;
        work->cameraStep = 0;
        PlaySE(SEQ_SE_DP_KAIDAN2);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_FIELD2);
        work->adjustCamera = TRUE;
        work->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            Heap_Free(work);
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera != FALSE) {
        DoorTransition_StepCameraZoomIn(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

BOOL Task_StairsFadeOut(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    DoorTransitionTaskWork *work = TaskManager_GetEnvironment(taskman);
    switch (work->state) {
    case 0:
        work->adjustCamera = FALSE;
        work->cameraStep = 0;
        PlaySE(SEQ_SE_DP_KAIDAN2);
        NewFieldFadeEnvironment(taskman, FADE_BOTH_SCREENS, (enum FadeType)16, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD2);
        work->adjustCamera = TRUE;
        work->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            Heap_Free(work);
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera != FALSE) {
        DoorTransition_StepCameraZoomIn(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

#pragma force_active on
// After a warp that does not end on a door: fades in with a fade type chosen by the facing direction while the
// camera zooms out; facing south, the player steps forward.
BOOL Task_ArriveWithDirectionalFade(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    DoorTransitionTaskWork *work = TaskManager_GetEnvironment(taskman);
    LocalMapObject *obj;
    u8 direction;
    int fadeType;

    switch (work->state) {
    case 0:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
        if (direction == DIR_SOUTH) {
            MapObject_SetVisible(obj, TRUE);
            work->state = 1;
        } else {
            MapObject_SetVisible(obj, FALSE);
            work->state = 3;
        }
        work->adjustCamera = FALSE;
        work->cameraStep = 0;
        work->cameraAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        Camera_AdjustPerspectiveAngle(0xFFA0, fieldSystem->camera);
        switch (direction) {
        case DIR_NORTH:
            fadeType = 3;
            break;
        case DIR_SOUTH:
            fadeType = 5;
            break;
        case DIR_WEST:
            fadeType = 7;
            break;
        case DIR_EAST:
            fadeType = 39;
            break;
        default:
            GF_ASSERT(FALSE);
            break;
        }
        NewFieldFadeEnvironment(taskman, FADE_BOTH_SCREENS, (enum FadeType)fadeType, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_FIELD2);
        work->adjustCamera = TRUE;
        break;
    case 1:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetVisible(obj, FALSE);
        MapObject_SetHeldMovement(obj, MOVEMENT_STEP_DOWN);
        work->state++;
        break;
    case 2:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            work->state++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished() && work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            Heap_Free(work);
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera) {
        DoorTransition_StepCameraZoomOut(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

// As above without the camera zoom and with a plain fade in.
BOOL Task_ArriveWithFade(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    u8 *state = TaskManager_GetEnvironment(taskman);
    LocalMapObject *obj;

    switch (*state) {
    case 0:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if ((u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == DIR_SOUTH) {
            MapObject_SetVisible(obj, TRUE);
            *state = 1;
        } else {
            MapObject_SetVisible(obj, FALSE);
            *state = 3;
        }
        NewFieldFadeEnvironment(taskman, FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_FIELD2);
        break;
    case 1:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetVisible(obj, FALSE);
        MapObject_SetHeldMovement(obj, MOVEMENT_STEP_DOWN);
        (*state)++;
        break;
    case 2:
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            (*state)++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            Heap_Free(state);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

// Doors that keep the camera still: in buildings models 0x3F, 0xA2 and 0xC5, outside model 0x7C.
BOOL DoorTransition_ModelSkipsCameraZoom(FieldSystem *fieldSystem, int modelId) {
    if (MapHeader_IsInBuilding(fieldSystem->location->mapId)) {
        if (modelId == 0x3F || modelId == 0xA2 || modelId == 0xC5) {
            return TRUE;
        }
    } else if (modelId == 0x7C) {
        return TRUE;
    }
    return FALSE;
}
