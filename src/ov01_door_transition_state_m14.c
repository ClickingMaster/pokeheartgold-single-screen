#include "field/area_data.h"
#include "field/fieldmap.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "camera_translation.h"
#include "field_metatile_query_internal.h"
#include "follow_mon.h"
#include "map_object.h"
#include "ov01_door_transition_state_m14_internal.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "field/field_follow_mon.h"
#include "player_avatar.h"
#include "map_object_movement_commands.h"

typedef struct DoorTransitionWork {
    int state;
    int x;
    int z;
    u8 active;
    u8 cameraStep;
    u16 cameraAngle;
    int adjustCamera;
    u32 unk14;
    GFCameraTranslationWrapper *translation;
    u16 translationDone;
    u16 modelIndex;
} DoorTransitionWork;

extern const AreaLightTemplate ov01_02206428[2];
extern int GetDoorSE(FieldSystem *, int, BOOL);
extern BOOL ov01_021EA1F4(FieldSystem *, int);
extern void ov01_021E95CC(Camera *, u8 *);
extern void ov01_021E9610(Camera *, u8 *);
extern int ov01_021E9700(int);

BOOL ov01_021E90E4(FieldSystem *fieldSystem, FieldEnvSubUnk18 *arg) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    fx32 rect[4];
    DoorSEData doorData;
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
        work->adjustCamera = 0;
        work->cameraStep = 0;
        sub_02054A60(work->x, work->z, -1, -1, 3, 1, rect);
        work->active = 1;
        found = FALSE;
        narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
        props = sub_02054D10(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
        for (i = 0; i < 4; i++) {
            if (props[i] != 0) {
                NARC_ReadWholeMember(narc, props[i], &doorData);
                if (doorData.doorType != 0) {
                    sub_02054AE4(fieldSystem, props[i], rect, &prop);
                    found = TRUE;
                    modelId0 = props[i];
                    break;
                }
            }
        }
        Heap_Free(props);
        if (!found) {
            goto no_prop_open;
        }
        animationCount = MapPropAnimationManager_GetPropAnimationCount(fieldSystem->mapPropAnimationManager, modelId0);
        if (animationCount == 0) {
            return TRUE;
        }
        texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        renderObj = MapProp_GetRenderSurface(prop);
        model = MapProp_GetResModel(prop);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, modelId0, renderObj, model, texture, animationCount, 1, FALSE);
        ov01_021E90B0(fieldSystem->mapPropOneShotAnimationManager, 1, 1);
        goto prop_loaded_open;
no_prop_open:
        return TRUE;
prop_loaded_open:
        work->modelIndex = ov01_021E9700(modelId0);
        if (work->modelIndex != 0xFFFF) {
            work->state = 100;
        } else {
            work->state++;
        }
        break;
    case 1: {
        int modelId;
        modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1);
        if (!ov01_021EA1F4(fieldSystem, modelId)) {
            Camera_ClearFixedTarget(fieldSystem->camera);
            work->adjustCamera = 1;
        }
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, GetDoorSE(fieldSystem, modelId, TRUE));
        work->state++;
        break;
    }
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1)) {
            work->state++;
        }
        break;
    case 3: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetHeldMovement(obj, 12);
        if (FollowMon_IsActive(fieldSystem) && !FollowMon_CheckVisibleFlag(fieldSystem)) {
            int x = MapObject_GetPreviousXCoord(obj);
            int z = MapObject_GetPreviousZCoord(obj);
            FollowMon_QueuePlayerStep(12, x, z, &fieldSystem->followMon);
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
        SetCameraTranslationPath(work->translation, (struct CameraTranslationPathTemplate *)&ov01_02206428[work->modelIndex], 24);
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
        ov01_021E95CC(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}

BOOL ov01_021E9374(FieldSystem *fieldSystem, FieldEnvSubUnk18 *arg) {
    DoorTransitionWork *work = (DoorTransitionWork *)arg;
    fx32 rect[4];
    DoorSEData doorData;
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
        sub_02054A60(work->x, work->z, -1, 0, 3, 1, rect);
        work->active = 1;
        found = FALSE;
        narc = AreaDataManager_GetMapPropAnimListNarc(fieldSystem->areaDataManager);
        props = sub_02054D10(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
        for (i = 0; i < 4; i++) {
            if (props[i] != 0) {
                NARC_ReadWholeMember(narc, props[i], &doorData);
                if (doorData.doorType != 0) {
                    sub_02054AE4(fieldSystem, props[i], rect, &prop);
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
            FieldMap_FadeScreen(1);
            work->state = 6;
            return FALSE;
        }
        texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        renderObj = MapProp_GetRenderSurface(prop);
        model = MapProp_GetResModel(prop);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, modelId0, renderObj, model, texture, animationCount, 1, FALSE);
        goto prop_loaded;
no_prop:
        FieldMap_FadeScreen(1);
        work->state = 6;
        return FALSE;
prop_loaded:
        work->cameraAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        if (!ov01_021EA1F4(fieldSystem, modelId0)) {
            Camera_AdjustPerspectiveAngle(-96, fieldSystem->camera);
        }
        work->state++;
        break;
    case 1: {
        int modelId;
        NewFieldFadeEnvironment(fieldSystem->taskman, (enum FadeMode)0, (enum FadeType)9, (enum FadeType)1, 0, 6, 1, HEAP_ID_FIELD2);
        modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1);
        if (!ov01_021EA1F4(fieldSystem, modelId)) {
            work->adjustCamera = 1;
        }
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, GetDoorSE(fieldSystem, modelId, TRUE));
        work->state++;
        break;
    }
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1)) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), FALSE);
            work->state++;
        }
        break;
    case 3:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 13);
        work->state++;
        break;
    case 4: {
        int modelId;
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            FollowMon_MoveToPlayer(fieldSystem, 1);
            modelId = MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 1, GetDoorSE(fieldSystem, modelId, FALSE));
            work->state++;
        }
        break;
    }
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1) && IsPaletteFadeFinished() && work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1);
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
        ov01_021E9610(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}
