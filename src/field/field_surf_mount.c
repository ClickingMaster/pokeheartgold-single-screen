#include "global.h"

#include "constants/global_fieldmap.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "geometry_math.h"
#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "field_effects.h"

// The Pokemon the player rides while surfing (field effect renderer 4): one model, turned to the
// rider's facing; while bobbing it rides along with the map object, rising and sinking between one and
// four units and lifting the rider with it.

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 4).
typedef struct SurfMount {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub model;
    NNSG3dRenderObj renderObj;
} SurfMount;

typedef struct SurfMountEffectArgs {
    u32 direction;
    FieldEffectManager *manager;
    SurfMount *mount;
    LocalMapObject *mapObject;
} SurfMountEffectArgs;

typedef struct SurfMountEffect {
    BOOL hidden;
    int facing;
    u32 direction;
    u32 objectId;
    u32 mapId;
    int timer;
    BOOL bobbing; // follow the map object and bob
    fx32 height;
    fx32 heightStep;
    SurfMountEffectArgs args;
} SurfMountEffect;

SurfMount *SurfMount_New(FieldEffectManager *manager);
void SurfMount_Free(SurfMount *mount);
void SurfMount_LoadGfx(SurfMount *mount);
void SurfMount_FreeGfx(SurfMount *mount);
FieldEffect *SurfMountEffect_Create(LocalMapObject *mapObject, u32 x, u32 z, u32 direction, BOOL bobbing);
BOOL SurfMountEffect_Init(FieldEffect *effect, SurfMountEffect *work);
void SurfMountEffect_Destroy(FieldEffect *effect, SurfMountEffect *work);
void SurfMountEffect_Update(FieldEffect *effect, SurfMountEffect *work);
void SurfMountEffect_Draw(FieldEffect *effect, SurfMountEffect *work);
void SurfMountEffect_SetBobbing(FieldEffect *effect, BOOL bobbing);

const VecFx32 ov01_022090D0 = { 0, 4 * FX32_ONE, 4 * FX32_ONE };
const VecFx32 ov01_022090C4 = { FX32_ONE, FX32_ONE, FX32_ONE };

const FieldEffectBasicTemplate ov01_022090DC = {
    sizeof(SurfMountEffect),
    (FieldEffectInitFunc)SurfMountEffect_Init,
    (FieldEffectFunc)SurfMountEffect_Destroy,
    (FieldEffectFunc)SurfMountEffect_Update,
    (FieldEffectFunc)SurfMountEffect_Draw,
};

SurfMount *SurfMount_New(FieldEffectManager *manager) {
    SurfMount *mount = FieldEffectManager_HeapAllocAndFill(manager, sizeof(SurfMount), FALSE, 0);
    mount->manager = manager;
    SurfMount_LoadGfx(mount);
    return mount;
}

void SurfMount_Free(SurfMount *mount) {
    SurfMount_FreeGfx(mount);
    FieldEffectManager_HeapFree(mount);
}

void SurfMount_LoadGfx(SurfMount *mount) {
    FieldEffectManager_LoadRenderModel(mount->manager, &mount->model, 0, 0x56, FALSE);
    RenderObj_InitFromModel(&mount->renderObj, &mount->model);
}

void SurfMount_FreeGfx(SurfMount *mount) {
    RenderModel_Free(&mount->model);
}

FieldEffect *SurfMountEffect_Create(LocalMapObject *mapObject, u32 x, u32 z, u32 direction, BOOL bobbing) {
    SurfMountEffectArgs args;
    VecFx32 position = { 0, 0, 0 };
    VecFx32 offset;

    args.direction = direction;
    args.manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.mount = FieldEffectManager_GetRendererData(args.manager, 4);
    args.mapObject = mapObject;
    if (bobbing == FALSE) {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
        position.x = (x << 16) + (2 << 14);
        position.z = (z << 16) + (2 << 14);
        FieldSystem_SnapToTerrainHeight(fieldSystem, &position);
    } else {
        offset = ov01_022090D0;
        MapObject_CopyPositionVector(mapObject, &position);
        MapObject_SetDrawOffset(mapObject, &offset);
    }
    return FieldEffectManager_CreateEffect(args.manager, &ov01_022090DC, &position, bobbing, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL SurfMountEffect_Init(FieldEffect *effect, SurfMountEffect *work) {
    SurfMountEffectArgs *args = (SurfMountEffectArgs *)FieldEffect_GetUserData(effect);
    LocalMapObject *mapObject = args->mapObject;

    work->args = *args;
    work->objectId = MapObject_GetID(mapObject);
    work->mapId = MapObject_GetMapID(mapObject);
    work->direction = args->direction;
    work->bobbing = FieldEffect_GetParam(effect);
    work->height = FX32_ONE;
    work->heightStep = FX32_ONE >> 2;
    return TRUE;
}

void SurfMountEffect_Destroy(FieldEffect *effect, SurfMountEffect *work) {
    LocalMapObject *mapObject = work->args.mapObject;
    VecFx32 offset = { 0, 0, 0 };

    MapObject_SetDrawOffset(mapObject, &offset);
}

void SurfMountEffect_Update(FieldEffect *effect, SurfMountEffect *work) {
    VecFx32 offset;
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    int facing;
    fx32 height;

    if (!MapObject_CheckIDAndMap(mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->hidden = FALSE;
    facing = MapObject_GetFacingDirection(mapObject);
    work->facing = facing;
    if (facing == -1) {
        work->hidden = TRUE;
        return;
    }
    if (work->bobbing) {
        work->direction = facing;
        height = work->height + work->heightStep;
        work->height = height;
        if (height >= 4 * FX32_ONE) {
            work->height = 4 * FX32_ONE;
            work->heightStep = -work->heightStep;
        } else if (height <= FX32_ONE) {
            work->height = FX32_ONE;
            work->heightStep = -work->heightStep;
        }
        offset.x = 0;
        offset.y = work->height + 4 * FX32_ONE;
        offset.z = 4 * FX32_ONE;
        MapObject_SetDrawOffset(mapObject, &offset);
        MapObject_CopyPositionVector(mapObject, &position);
        position.y += work->height - FX32_ONE;
        FieldEffect_SetPosition(effect, &position);
    }
}

void SurfMountEffect_Draw(FieldEffect *effect, SurfMountEffect *work) {
    MtxFx33 rotation;
    VecFx32 position;
    VecFx32 scale;
    NNSG3dRenderObj *renderObj;
    int angle;

    if (work->hidden == TRUE) {
        return;
    }
    if (work->direction == -1) {
        return;
    }
    work->timer++;
    angle = 0;
    scale = ov01_022090C4;
    renderObj = &work->args.mount->renderObj;
    switch (work->direction) {
    case DIR_NORTH:
        angle = 180;
        break;
    case DIR_SOUTH:
        break;
    case DIR_WEST:
        angle = 270;
        break;
    case DIR_EAST:
        angle = 90;
        break;
    }
    Math_CalcRotationMatrixXYZDeg(&rotation, 0, angle, 0);
    FieldEffect_GetPosition(effect, &position);
    RenderObj_Draw(renderObj, &position, &scale, &rotation);
}

void SurfMountEffect_SetBobbing(FieldEffect *effect, BOOL bobbing) {
    SurfMountEffect *work = FieldEffect_GetState(effect);

    work->bobbing = bobbing;
    work->height = FX32_ONE;
    work->heightStep = FX32_ONE >> 2;
}
