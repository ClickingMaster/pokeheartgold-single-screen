#include "overlay01_door_transition_sideways_private.h"

BOOL ov01_021E971C(FieldSystem *fieldSystem, DoorTransitionWork2 *work, u8 direction) {
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    LocalMapObject *obj;

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorModelList models = ov01_02206408;
        MapProp *prop;
        int modelId;
        if (direction == DIR_WEST) {
            sub_02054A60(work->x, work->z, -1, 0, 2, 1, rect);
        } else {
            sub_02054A60(work->x, work->z, 0, 0, 2, 1, rect);
        }
        work->active = 2;
        if (sub_02054B74(fieldSystem, models.ids, 4, rect, &prop, &modelId)) {
            texture = ov01_021FB9E0(fieldSystem->areaDataManager);
            renderObj = MapProp_GetRenderSurface(prop);
            model = MapProp_GetResModel(prop);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 2, modelId, renderObj, model, texture, 1, 1, 1);
        } else {
            GF_ASSERT(FALSE);
            return TRUE;
        }
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, 0x49);
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    }
    case 1:
        FieldMap_FadeScreen(1);
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 2, 0, SEQ_SE_DP_ESUKA);
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
            MapObject_SetHeldMovement(obj, 0x4A);
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
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 2) && IsPaletteFadeFinished()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 2);
            StopSE(SEQ_SE_DP_ESUKA, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL ov01_021E98F0(FieldSystem *fieldSystem, DoorTransitionWork2 *work, u8 direction) {
    NNSG3dResTex *texture;
    NNSG3dRenderObj *renderObj;
    NNSG3dResMdl *model;
    LocalMapObject *obj;
    int movement;
    int x;

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorModelList models = ov01_02206418;
        MapProp *prop;
        int modelId;
        if (direction == DIR_WEST) {
            sub_02054A60(work->x, work->z, -1, 0, 2, 1, rect);
        } else {
            sub_02054A60(work->x, work->z, 0, 0, 2, 1, rect);
        }
        work->active = 2;
        if (sub_02054B74(fieldSystem, models.ids, 4, rect, &prop, &modelId)) {
            texture = ov01_021FB9E0(fieldSystem->areaDataManager);
            renderObj = MapProp_GetRenderSurface(prop);
            model = MapProp_GetResModel(prop);
            MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 2, modelId, renderObj, model, texture, 1, 1, 0);
        } else {
            GF_ASSERT(FALSE);
            return TRUE;
        }
        obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(obj) == TRUE) {
            MapObject_SetHeldMovement(obj, 0x49);
        } else {
            GF_ASSERT(FALSE);
        }
        work->state++;
        break;
    }
    case 1:
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 2, 0, SEQ_SE_DP_ESUKA);
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
            MapObject_SetHeldMovement(obj, 0x4A);
            FieldMap_FadeScreen(0);
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
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 2) && IsPaletteFadeFinished()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 2);
            StopSE(SEQ_SE_DP_ESUKA, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
