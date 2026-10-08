#pragma force_active on
#include "global.h"
#include "field_system.h"
#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "task.h"
#include "player_avatar.h"
#include "map_object.h"
#include "unk_0200FA24.h"
#include "overlay_01.h"
#include "heap.h"
#include "unk_02062108.h"
#include "field_metatile_query_internal.h"

typedef struct DoorSEData {
    u32 unk00;
    u8 doorType;
    u8 filler05[0x13];
} DoorSEData;

typedef struct DoorExitWork {
    int state;
    int x;
    int z;
    u8 active;
    u8 cameraStep;
    u16 cameraAngle;
    int adjustCamera;
} DoorExitWork;

int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening);
NARC *ov01_021FB904(AreaDataManager *areaDataManager);
NNSG3dResTex *ov01_021FB9E0(AreaDataManager *areaDataManager);
void ov01_021E9610(Camera *camera, u8 *state);
BOOL ov01_021E9C40(TaskManager *taskman);
// @fn ov01_021E9C40
BOOL ov01_021E9C40(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    DoorExitWork *work = TaskManager_GetEnvironment(taskman);

    switch (work->state) {
    case 0: {
        fx32 rect[4];
        DoorSEData doorData;
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
        work->adjustCamera = 0;
        work->cameraStep = 0;
        work->cameraAngle = Camera_GetPerspectiveAngle(fieldSystem->camera);
        Camera_AdjustPerspectiveAngle(0xFFA0, fieldSystem->camera);
        BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 6, 1, HEAP_ID_FIELD2);
        work->adjustCamera = 1;
        if (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == DIR_SOUTH) {
            work->active = 1;
            found = FALSE;
            sub_02054A60(work->x, work->z, -1, found, 3, 1, rect);
            narc = ov01_021FB904(fieldSystem->areaDataManager);
            props = sub_02054D10(fieldSystem, HEAP_ID_FIELD1, 4, rect, 0);
            for (i = 0; i < 4; i++) {
                if (props[i] != 0) {
                    NARC_ReadWholeMember(narc, props[i], &doorData);
                    if (doorData.doorType != 0) {
                        sub_02054AE4(fieldSystem, props[i], rect, &prop);
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
                texture = ov01_021FB9E0(fieldSystem->areaDataManager);
                renderObj = MapProp_GetRenderSurface(prop);
                model = MapProp_GetResModel(prop);
                MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1, modelId, renderObj, model, texture, count, 1, FALSE);
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
        MapObject_SetHeldMovement(obj, 13);
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
        MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 0, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1), TRUE));
        work->state = 5;
        break;
    case 5:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1)) {
            MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), FALSE);
            work->state = 6;
        }
        break;
    case 6:
        MapObject_SetHeldMovement(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), 13);
        work->state = 7;
        break;
    case 7: {
        LocalMapObject *obj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(obj) == TRUE) {
            MapObject_ClearHeldMovementIfActive(obj);
            ov01_02205790(fieldSystem, 1);
            MapPropOneShotAnimationManager_PlayAnimationWithSoundEffect(fieldSystem->mapPropOneShotAnimationManager, 1, 1, GetDoorSE(fieldSystem, MapPropOneShotAnimationManager_GetAnimationMapPropModelID(fieldSystem->mapPropOneShotAnimationManager, 1), FALSE));
            work->state++;
        }
        break;
    }
    case 8:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, 1) && IsPaletteFadeFinished() && work->cameraAngle == Camera_GetPerspectiveAngle(fieldSystem->camera)) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, 1);
            return TRUE;
        }
        break;
    }
    if (work->adjustCamera) {
        ov01_021E9610(fieldSystem->camera, &work->cameraStep);
    }
    return FALSE;
}
